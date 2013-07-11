// $Id $

#include "MitPhysics/Mods/interface/MonteCarloSampleFilterMod.h"
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include <TH1D.h>
#include <TH2D.h>

using namespace mithep;

ClassImp(mithep::MonteCarloSampleFilterMod)

//--------------------------------------------------------------------------------------------------
MonteCarloSampleFilterMod::MonteCarloSampleFilterMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fMCPartName(Names::gkMCPartBrn),
  fMCSampleName("NotSet"),
  fRemoveWGammaProcess(kFALSE),
  fParticles(0)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void MonteCarloSampleFilterMod::Process()
{
  // Process entries of the tree.

  // load MCParticle branch
  LoadBranch(fMCPartName);
  //***********************************************************************************************
  //Load input generator collections
  //***********************************************************************************************
  const MCParticleCol *GenLeptons = 0;
  GenLeptons = GetObjThisEvt<MCParticleCol>(ModNames::gkMCLeptonsName);
  const MCParticleCol *GenPhotons = 0;
  GenPhotons = GetObjThisEvt<MCParticleCol>(ModNames::gkMCPhotonsName);
  const MCParticleCol *GenTaus = 0;  
  GenTaus = GetObjThisEvt<MCParticleCol>(ModNames::gkMCTausName);
  const MCParticleCol *GenBosons = 0;
  GenBosons = GetObjThisEvt<MCParticleCol>(ModNames::gkMCBosonsName);
  //***********************************************************************************************
  //Filter W+Gamma
  //***********************************************************************************************
  if(fRemoveWGammaProcess) {
    Bool_t FoundGamma = false;
    Bool_t FoundW = false;
    for (UInt_t i=0; i<GenPhotons->GetEntries(); i++) {  
      if (GenPhotons->At(i)->Pt() > 10.0) {
        
        //ISR Photon
        if (GenPhotons->At(i)->Mother() && GenPhotons->At(i)->Mother()->IsParton()
            || (GenPhotons->At(i)->Mother()->AbsPdgId() == 22 
                && GenPhotons->At(i)->Mother()->Status() == 3  
                && GenPhotons->At(i)->Mother()->Mother() 
                && GenPhotons->At(i)->Mother()->Mother()->IsParton())
          ) {
          FoundGamma = true;    
        }
        
        //WWgamma vertex  
        if ((GenPhotons->At(i)->Mother() && GenPhotons->At(i)->Mother()->AbsPdgId() == 24) 
            || 
            (GenPhotons->At(i)->Mother()->AbsPdgId() == 22 
             && GenPhotons->At(i)->Mother()->Status() == 3
             && GenPhotons->At(i)->Mother()->Mother()
             && GenPhotons->At(i)->Mother()->Mother()->AbsPdgId() == 24
              )
          ) {
          FoundGamma = true;
        }
        
        //Pythia FSR
        if (GenPhotons->At(i)->Mother() && GenPhotons->At(i)->Mother()->Status() == 3
            && (GenPhotons->At(i)->Mother()->AbsPdgId() == 11 
                || GenPhotons->At(i)->Mother()->AbsPdgId() == 13
                || GenPhotons->At(i)->Mother()->AbsPdgId() == 15)
          ) {
          FoundGamma = true;
        }      
      }
    }

    for (UInt_t i=0; i<GenBosons->GetEntries(); i++) {  
      if (GenBosons->At(i)->AbsPdgId() == 24) {
        FoundW = true;
      }
    }

    if (FoundW && FoundGamma) {
      this->SkipEvent();
      return;
    }
  }
}

//--------------------------------------------------------------------------------------------------
void MonteCarloSampleFilterMod::SlaveBegin()
{
  // Book branch and histograms if wanted.
  ReqBranch(fMCPartName, fParticles);
}
