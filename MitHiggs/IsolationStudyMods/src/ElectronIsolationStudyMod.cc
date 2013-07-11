// $Id: ElectronIsolationStudyMod.cc,v 1.3 2008/12/04 19:19:50 sixie Exp $

#include "MitHiggs/IsolationStudyMods/interface/ElectronIsolationStudyMod.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitPhysics/Utils/interface/IsolationTools.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include <TH1D.h>
#include <TH2D.h>

using namespace mithep;

ClassImp(mithep::ElectronIsolationStudyMod)

//--------------------------------------------------------------------------------------------------
  ElectronIsolationStudyMod::ElectronIsolationStudyMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fPrintDebug(false),
  fElectronName(Names::gkElectronBrn),
  fMCLeptonsName(ModNames::gkMCLeptonsName),
  fElectrons(0),
  fNEventsProcessed(0),
  fMatchToRealElectrons(false),
  fImposeElectronID(false),
  fIDLikelihoodCut(0.9),
  fElectronPtMin(0.0),
  fElectronPtMax(10000.0),
  fNElectrons(0),
  fNElectronsPassDefaultIsolation(0)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void ElectronIsolationStudyMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void ElectronIsolationStudyMod::Process()
{
  // Process entries of the tree. 

  fNEventsProcessed++;
 
  if (fNEventsProcessed % 1000 == 0 || fPrintDebug) {
    time_t systime;
    systime = time(NULL);

    cerr << endl << "ElectronIsolationStudyMod : Process Event " << fNEventsProcessed << "  Time: " << ctime(&systime) << endl;  
  }  
  
  ObjArray<MCParticle> *GenLeptons = 0;
  if (fMatchToRealElectrons)
    GenLeptons = dynamic_cast<ObjArray<MCParticle>* > (FindObjThisEvt(fMCLeptonsName.Data()));

  //Get Electrons
  LoadBranch(fElectronName);

  for (UInt_t i=0; i<fElectrons->GetEntries(); ++i) {    
    const Electron *e = fElectrons->At(i);        
        
    //Impose Electron ID
    //reject electrons if they do not pass ID requirements
    //this means denominator electrons are electrons that pass ID requirements
    if (fImposeElectronID) {
      if (!(e->PassTightID()))
        continue;
    }
    
    //Match to real Electrons.
    //Ignore electrons candidates that do not match to generator electron
    if (fMatchToRealElectrons) {
      bool matched = false;
      for (UInt_t j=0; j<GenLeptons->GetEntries(); ++j) {    
        if (GenLeptons->At(j)->AbsPdgId() == 11 && MathUtils::DeltaR(GenLeptons->At(j)->Mom(),
                                                                     e->Mom()) < 0.1) {
          matched = true;
          break;
        }
      }
      if (!matched)
        continue;
    }
    

    //get eta and pt distribution
    fElectronPtHist->Fill(e->Pt());
    fElectronEtaHist->Fill(e->Eta());

    //2D distribution of Isolation Versus Pt
    //     //here i want all pt range
    fElectronTrackIsoVsPt->Fill(e->TrackIsolation(), e->Pt());
    fElectronCaloIsoVsPt->Fill(e->CaloIsolation(), e->Pt());
    fElectronCaloTowerIsoVsPt->Fill(e->CaloTowerIsolation(),e->Pt());
    fElectronJurassicEcalIsoVsPt->Fill(e->EcalJurassicIsolation(), e->Pt());
    fElectronHcalIsoVsPt->Fill(e->HcalIsolation(),e->Pt());
    fElectronJurassicIsoVsPt->Fill(e->EcalJurassicIsolation()+e->HcalIsolation(),e->Pt());
    fElectronTrackPlusJurassicIsoVsPt->Fill(e->TrackIsolation() + e->EcalJurassicIsolation() + e->HcalIsolation(),e->Pt());

    //only look at electrons in this pt range
    if (!(e->Pt() > fElectronPtMin && e->Pt() < fElectronPtMax))
      continue;

    fNElectrons++;
    
    if (e->TrackIsolation() < 5.0 && e->CaloIsolation() < 5.0)
      fNElectronsPassDefaultIsolation++;

    //1D isolation histograms
    fElectronTrackIso->Fill(e->TrackIsolation());
    fElectronCaloIso->Fill(e->CaloIsolation());
    fElectronCaloTowerIso->Fill(e->CaloTowerIsolation());
    fElectronEcalJurassicIso->Fill(e->EcalJurassicIsolation());
    fElectronHcalIso->Fill(e->HcalIsolation());    
    fElectronTotalJurassicIso->Fill(e->EcalJurassicIsolation() + e->HcalIsolation());    
    fElectronTrackIsoPlusCaloIso->Fill(e->TrackIsolation() + e->CaloIsolation());
    fElectronTrackIsoPlusCaloTowerIso->Fill(e->TrackIsolation() + e->CaloTowerIsolation());
    fElectronTrackIsoPlusJurassicIso->Fill(e->TrackIsolation() + e->EcalJurassicIsolation() 
                                           + e->HcalIsolation());
    fElectronTwoTimesTrackIsoPlusCaloTowerIso->Fill(2*e->TrackIsolation() + e->CaloTowerIsolation());
    fElectronTwoTimesTrackIsoPlusJurassicIso->Fill(2*e->TrackIsolation() + e->EcalJurassicIsolation() 
                                                   + e->HcalIsolation());
    //isolation over pt
    fElectronTrackIsoOverPt->Fill(e->TrackIsolation()/e->Pt());
    fElectronCaloIsoOverPt->Fill(e->CaloIsolation()/e->Pt());
    fElectronCaloTowerIsoOverPt->Fill(e->CaloTowerIsolation()/e->Pt());
    fElectronEcalJurassicIsoOverPt->Fill(e->EcalJurassicIsolation()/e->Pt());
    fElectronHcalIsoOverPt->Fill(e->HcalIsolation()/e->Pt());    
    fElectronTotalJurassicIsoOverPt->Fill((e->EcalJurassicIsolation() + e->HcalIsolation())/e->Pt());    
    fElectronTrackIsoPlusCaloIsoOverPt->Fill((e->TrackIsolation() + e->CaloIsolation())/e->Pt());
    fElectronTrackIsoPlusCaloTowerIsoOverPt->Fill((e->TrackIsolation() + e->CaloTowerIsolation())/e->Pt());
    fElectronTrackIsoPlusJurassicIsoOverPt->Fill((e->TrackIsolation() + e->EcalJurassicIsolation() 
                                           + e->HcalIsolation())/e->Pt());
    fElectronTwoTimesTrackIsoPlusCaloTowerIsoOverPt->Fill((2*e->TrackIsolation() + e->CaloTowerIsolation())/e->Pt());
    fElectronTwoTimesTrackIsoPlusJurassicIsoOverPt->Fill((2*e->TrackIsolation() + e->EcalJurassicIsolation() 
                                                   + e->HcalIsolation())/e->Pt());

    //2D histograms
    fElectronTrackIsoVsCaloIso->Fill(e->TrackIsolation(),e->CaloIsolation());
    fElectronTrackIsoVsCaloTowerIso->Fill(e->TrackIsolation(),e->CaloTowerIsolation());
    fElectronTrackIsoVsEcalJurassicIso->Fill(e->TrackIsolation(),e->EcalJurassicIsolation());
    fElectronTrackIsoVsHcalIso->Fill(e->TrackIsolation(),e->HcalIsolation());

    //do 2D matrix of trackIso and caloIso Cuts
    for (int t = 0;t < 101; t++) {
      double trackIsoCut = 0.0 + t*0.2;
      for (int c = 0; c < 101 ; c++ ) {
        double caloIsoCut = 0.0 + c*0.2;
        if (e->TrackIsolation() < trackIsoCut && e->CaloIsolation() < caloIsoCut)
          fNElectronPassTrackAndCaloIso->Fill(trackIsoCut, caloIsoCut) ;
      }
      
      for (int c = 0; c < 101 ; c++ ) {
        double caloTowerIsoCut = 0.0 + c*0.2;
        if (e->TrackIsolation() < trackIsoCut && e->CaloTowerIsolation() < caloTowerIsoCut)
          fNElectronPassTrackAndCaloTowerIso->Fill(trackIsoCut, caloTowerIsoCut) ;
      }
    }
  }
  
  //Final Summary Debug Output   
  if ( fPrintDebug ) {
    cerr << "Event Dump: " << fNEventsProcessed << endl;    
  }   
  
}


//--------------------------------------------------------------------------------------------------
void ElectronIsolationStudyMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fElectronName,              fElectrons);
  fElectronPtHist                     = new TH1D("hElectronPtHist",";p_{t};#",100,0.,200.);  
  fElectronEtaHist                    = new TH1D("hElectronEtaHist",";#eta;#",100,-5.,5.);

  fElectronTrackIsoVsCaloIso  = 
    new TH2D("hElectronTrackIsoVsCaloIso",";TrackIsolation;CaloIsolation",
             80,0,20.0,80,0,20.0);  
  fElectronTrackIsoVsCaloTowerIso  = 
    new TH2D("hElectronTrackIsoVsCaloTowerIso",";TrackIsolation;CaloTowerIsolation",
             80,0,20.0,80,0,20.0);  
  fElectronTrackIsoVsEcalJurassicIso  = 
    new TH2D("hElectronTrackIsoVsEcalJurassicIso",";TrackIsolation;EcalJurassicIso",
             80,0,20.0,80,0,20.0); 
  fElectronTrackIsoVsHcalIso  = 
    new TH2D("hElectronTrackIsoVsHcalIso",";TrackIsolation;HcalIso",
             80,0,20.0,80,0,20.0);
  fNElectronPassTrackAndCaloIso= 
    new TH2D("hNElectronPassTrackAndCaloIso",";Track Isolation Cut; Calo Isolation Cut",
             101,0,20.0,101,0,20.0);
  fNElectronPassTrackAndCaloTowerIso= 
    new TH2D("hNElectronPassTrackAndCaloTowerIso",";Track Isolation Cut;CaloTower Isolation Cut",
             101,0,20.0,101,0,20.0);
  fElectronTrackAndCaloIsoCutEff = 
    new TH2D("hElectronTrackAndCaloIsoCutEff",";Track Isolation Cut;Calo Isolation Cut",
             101,0,20.0,101,0,20.0);

  fElectronTrackAndCaloTowerIsoCutEff= 
    new TH2D("hElectronTrackAndCaloTowerIsoCutEff",";Track Isolation Cut;CaloTower Isolation Cut",
             101,0,20.0,101,0,20.0);

//*************************************************************************************************
// 1D Isolation Histograms
//*************************************************************************************************
  fElectronTrackIso  = 
    new TH1D("hElectronTrackIso",";TrackIso ;Number of Events", 101,0,20.0); 
  fElectronCaloIso  = 
    new TH1D("hElectronCaloIso",";CaloIso;Number of Events", 101,0,20.0); 
  fElectronCaloTowerIso  = 
    new TH1D("hElectronCaloTowerIso",";CaloTowerIso;Number of Events", 101,0,20.0); 
  fElectronEcalJurassicIso  = 
    new TH1D("hElectronEcalJurassicIso",";EcalJurassicIso;Number of Events", 101,0,20.0); 
  fElectronHcalIso  = 
    new TH1D("hElectronHcalIso",";HcalIso;Number of Events", 101,0,20.0); 
  fElectronTotalJurassicIso  = 
    new TH1D("hElectronTotalJurassicIso",";TotalJurassicIso;Number of Events", 101,0,20.0); 
  fElectronTrackIsoPlusCaloIso  = 
    new TH1D("hElectronTrackIsoPlusCaloIso",";TrackIso + CaloIso;Number of Events", 101,0,20.0); 
  fElectronTrackIsoPlusCaloTowerIso  = 
    new TH1D("hElectronTrackIsoPlusCaloTowerIso",";TrackIso + CaloTowerIso;Number of Events", 101,0,20.0); 
  fElectronTrackIsoPlusJurassicIso  = 
    new TH1D("hElectronTrackIsoPlusJurassicIso",";TrackIso + JurassicIso;Number of Events", 101,0,20.0); 
  fElectronTwoTimesTrackIsoPlusCaloTowerIso  = 
    new TH1D("hElectronTwoTimesTrackIsoPlusCaloTowerIso",
             ";2 * TrackIso + CaloTowerIso;Number of Events", 101,0,20.0); 
  fElectronTwoTimesTrackIsoPlusJurassicIso  = 
    new TH1D("hElectronTwoTimesTrackIsoPlusJurassicIso",
             ";2 * TrackIso + JurassicIso;Number of Events", 101,0,20.0); 
  fElectronTrackIsoOverPt  = 
    new TH1D("hElectronTrackIsoOverPt",";TrackIso / Pt;Number of Events", 201,0,4.0); 
  fElectronCaloIsoOverPt  = 
    new TH1D("hElectronCaloIsoOverPt",";CaloIso / Pt;Number of Events", 201,0,4.0); 
  fElectronCaloTowerIsoOverPt  = 
    new TH1D("hElectronCaloTowerIsoOverPt",";CaloTowerIso / Pt;Number of Events", 201,0,4.0); 
  fElectronEcalJurassicIsoOverPt  = 
    new TH1D("hElectronEcalJurassicIsoOverPt",";EcalJurassicIso / Pt;Number of Events", 201,0,4.0); 
  fElectronHcalIsoOverPt  = 
    new TH1D("hElectronHcalIsoOverPt",";HcalIso / Pt;Number of Events", 201,0,4.0); 
  fElectronTotalJurassicIsoOverPt  = 
    new TH1D("hElectronTotalJurassicIsoOverPt",";TotalJurassicIso / Pt;Number of Events", 201,0,4.0); 
  fElectronTrackIsoPlusCaloIsoOverPt  = 
    new TH1D("hElectronTrackIsoPlusCaloIsoOverPt",";TrackIso + CaloIso / Pt;Number of Events", 201,0,4.0); 
  fElectronTrackIsoPlusCaloTowerIsoOverPt  = 
    new TH1D("hElectronTrackIsoPlusTowerCaloIsoOverPt",";TrackIso + CaloTowerIso / Pt;Number of Events", 201,0,4.0); 
  fElectronTrackIsoPlusJurassicIsoOverPt  = 
    new TH1D("hElectronTrackIsoPlusJurassicIsoOverPt",";TrackIso + JurassicIso / Pt;Number of Events", 201,0,4.0); 
  fElectronTwoTimesTrackIsoPlusCaloTowerIsoOverPt  = 
    new TH1D("hElectronTwoTimesTrackIsoPlusCaloTowerIsoOverPt",
             ";2 * TrackIso + CaloTowerIso / Pt;Number of Events", 201,0,4.0); 
  fElectronTwoTimesTrackIsoPlusJurassicIsoOverPt = 
    new TH1D("hElectronTwoTimesTrackIsoPlusJurassicIsoOverPt",
             ";2 * TrackIso + JurassicIso / Pt;Number of Events", 201,0,4.0); 

  fNElectronPassTrackIsoCut  = 
    new TH1D("hNElectronPassTrackIsoCut",";TrackIso;N Electrons Passed", 101,0,20.0); 
  fNElectronPassCaloIsoCut  = 
    new TH1D("hNElectronPassCaloIsoCut",";CaloIso;N Electrons Passed", 101,0,20.0); 
  fNElectronPassCaloTowerIsoCut  = 
    new TH1D("hNElectronPassCaloTowerIsoCut",";CaloTowerIso;N Electrons Passed", 101,0,20.0); 
  fNElectronPassEcalJurassicIsoCut  = 
    new TH1D("hNElectronPassEcalJurassicIsoCut",";EcalJurassicIso;N Electrons Passed", 101,0,20.0); 
  fNElectronPassHcalIsoCut  = 
    new TH1D("hNElectronPassHcalIsoCut",";HcalIso;N Electrons Passed", 101,0,20.0); 
  fNElectronPassTotalJurassicIsoCut  = 
    new TH1D("hNElectronPassTotalJurassicIsoCut",";TotalJurassicIso;N Electrons Passed", 101,0,20.0); 
  fNElectronPassTrackIsoPlusCaloIsoCut  = 
    new TH1D("hNElectronPassTrackIsoPlusCaloIsoCut",";TrackIso + CaloIso;Efficiency", 101,0,20.0); 
  fNElectronPassTrackIsoPlusCaloTowerIsoCut  = 
    new TH1D("hNElectronPassTrackIsoPlusCaloTowerIsoCut",";TrackIso + CaloTowerIso;Efficiency", 101,0,20.0); 
  fNElectronPassTrackIsoPlusJurassicIsoCut  = 
    new TH1D("hNElectronPassTrackIsoPlusJurassicIsoCut", ";TrackIso + JurassicIso;Efficiency", 101,0,20.0); 
  fNElectronPassTwoTimesTrackIsoPlusCaloTowerIsoCut  = 
    new TH1D("hNElectronPassTwoTimesTrackIsoPlusCaloTowerIsoCut",
             ";2 * TrackIso + CaloTowerIso;Efficiency", 101,0,20.0); 
  fNElectronPassTwoTimesTrackIsoPlusJurassicIsoCut  = 
    new TH1D("hNElectronPassTwoTimesTrackIsoPlusJurassicIsoCut",
             ";2 * TrackIso + JurassicIso;Efficiency", 101,0,20.0); 
  fNElectronPassTrackIsoOverPtCut  = 
    new TH1D("hNElectronPassTrackIsoOverPtCut",";TrackIso / Pt;N Electrons Passed", 201,0,4.0); 
  fNElectronPassCaloIsoOverPtCut  = 
    new TH1D("hNElectronPassCaloIsoOverPtCut",";CaloIso / Pt;N Electrons Passed", 201,0,4.0); 
  fNElectronPassCaloTowerIsoOverPtCut  = 
    new TH1D("hNElectronPassCaloTowerIsoOverPtCut",";CaloTowerIso / Pt;N Electrons Passed", 201,0,4.0); 
  fNElectronPassEcalJurassicIsoOverPtCut  = 
    new TH1D("hNElectronPassEcalJurassicIsoOverPtCut",";EcalJurassicIso / Pt;N Electrons Passed", 201,0,4.0); 
  fNElectronPassHcalIsoOverPtCut  = 
    new TH1D("hNElectronPassHcalIsoOverPtCut",";HcalIso / Pt;N Electrons Passed", 201,0,4.0); 
  fNElectronPassTotalJurassicIsoOverPtCut  = 
    new TH1D("hNElectronPassTotalJurassicIsoOverPtCut",";TotalJurassicIso / Pt;N Electrons Passed", 201,0,4.0); 
  fNElectronPassTrackIsoPlusCaloIsoOverPtCut  = 
    new TH1D("hNElectronPassTrackIsoPlusCaloIsoOverPtCut",";TrackIso + CaloIso / Pt;Efficiency", 201,0,4.0); 
  fNElectronPassTrackIsoPlusCaloTowerIsoOverPtCut  = 
    new TH1D("hNElectronPassTrackIsoPlusCaloTowerIsoOverPtCut",";TrackIso + CaloTowerIso / Pt;Efficiency", 201,0,4.0); 
  fNElectronPassTrackIsoPlusJurassicIsoOverPtCut  = 
    new TH1D("hNElectronPassTrackIsoPlusJurassicIsoOverPtCut", ";TrackIso + JurassicIso / Pt;Efficiency", 201,0,4.0); 
  fNElectronPassTwoTimesTrackIsoPlusCaloTowerIsoOverPtCut  = 
    new TH1D("hNElectronPassTwoTimesTrackIsoPlusCaloTowerIsoOverPtCut",
             ";2 * TrackIso + CaloTowerIso / Pt;Efficiency", 201,0,4.0); 
  fNElectronPassTwoTimesTrackIsoPlusJurassicIsoOverPtCut  = 
    new TH1D("hNElectronPassTwoTimesTrackIsoPlusJurassicIsoOverPtCut",
             ";2 * TrackIso + JurassicIso / Pt;Efficiency", 201,0,4.0); 

  fElectronTrackIsoCutEff  = 
    new TH1D("hElectronTrackIsoCutEff",";TrackIso;Efficiency", 101,0,20.0); 
  fElectronCaloIsoCutEff  = 
    new TH1D("hElectronCaloIsoCutEff",";CaloIso;Efficiency", 101,0,20.0); 
  fElectronCaloTowerIsoCutEff  = 
    new TH1D("hElectronCaloTowerIsoCutEff",";CaloTowerIso;Efficiency", 101,0,20.0); 
  fElectronEcalJurassicIsoCutEff  = 
    new TH1D("hElectronEcalJurassicIsoCutEff",";EcalJurassicIso;Efficiency", 101,0,20.0); 
  fElectronHcalIsoCutEff  = 
    new TH1D("hElectronHcalIsoCutEff",";HcalIso;Efficiency", 101,0,20.0); 
  fElectronTotalJurassicIsoCutEff  = 
    new TH1D("hElectronTotalJurassicIsoCutEff",";TotalJurassicIso;Efficiency", 101,0,20.0); 
  fElectronTrackIsoPlusCaloIsoCutEff  = 
    new TH1D("hElectronTrackIsoPlusCaloIsoCutEff",";TrackIso + CaloIso;Efficiency", 101,0,20.0); 
  fElectronTrackIsoPlusCaloTowerIsoCutEff  = 
    new TH1D("hElectronTrackIsoPlusCaloTowerIsoCutEff",";TrackIso + CaloTowerIso;Efficiency", 101,0,20.0); 
  fElectronTrackIsoPlusJurassicIsoCutEff  = 
    new TH1D("hElectronTrackIsoPlusJurassicIsoCutEff", ";TrackIso + JurassicIso;Efficiency", 101,0,20.0); 
  fElectronTwoTimesTrackIsoPlusCaloTowerIsoCutEff  = 
    new TH1D("hElectronTwoTimesTrackIsoPlusCaloTowerIsoCutEff",
             ";2 * TrackIso + CaloTowerIso;Efficiency", 101,0,20.0); 
  fElectronTwoTimesTrackIsoPlusJurassicIsoCutEff  = 
    new TH1D("hElectronTwoTimesTrackIsoPlusJurassicIsoCutEff",
             ";2 * TrackIso + JurassicIso;Efficiency", 101,0,20.0); 
  fElectronTrackIsoOverPtCutEff  = 
    new TH1D("hElectronTrackIsoOverPtCutEff",";TrackIso / Pt;Efficiency", 201,0,4.0); 
  fElectronCaloIsoOverPtCutEff  = 
    new TH1D("hElectronCaloIsoOverPtCutEff",";CaloIso / Pt;Efficiency", 201,0,4.0); 
  fElectronCaloTowerIsoOverPtCutEff  = 
    new TH1D("hElectronCaloTowerIsoOverPtCutEff",";CaloTowerIso / Pt;Efficiency", 201,0,4.0); 
  fElectronEcalJurassicIsoOverPtCutEff  = 
    new TH1D("hElectronEcalJurassicIsoOverPtCutEff",";EcalJurassicIso / Pt;Efficiency", 201,0,4.0); 
  fElectronHcalIsoOverPtCutEff  = 
    new TH1D("hElectronHcalIsoOverPtCutEff",";HcalIso / Pt;Efficiency", 201,0,4.0); 
  fElectronTotalJurassicIsoOverPtCutEff  = 
    new TH1D("hElectronTotalJurassicIsoOverPtCutEff",";TotalJurassicIso / Pt;Efficiency", 201,0,4.0);
  fElectronTrackIsoPlusCaloIsoOverPtCutEff  = 
    new TH1D("hElectronTrackIsoPlusCaloIsoOverPtCutEff",";TrackIso + CaloIso / Pt;Efficiency", 201,0,4.0); 
  fElectronTrackIsoPlusCaloTowerIsoOverPtCutEff  = 
    new TH1D("hElectronTrackIsoPlusCaloTowerIsoOverPtCutEff",";TrackIso + CaloTowerIso / Pt;Efficiency", 201,0,4.0); 
  fElectronTrackIsoPlusJurassicIsoOverPtCutEff  = 
    new TH1D("hElectronTrackIsoPlusJurassicIsoOverPtCutEff", ";TrackIso + JurassicIso / Pt;Efficiency", 201,0,4.0); 
  fElectronTwoTimesTrackIsoPlusCaloTowerIsoOverPtCutEff  = 
    new TH1D("hElectronTwoTimesTrackIsoPlusCaloTowerIsoOverPtCutEff",
             ";2 * TrackIso + CaloTowerIso / Pt;Efficiency", 201,0,4.0); 
  fElectronTwoTimesTrackIsoPlusJurassicIsoOverPtCutEff  = 
    new TH1D("hElectronTwoTimesTrackIsoPlusJurassicIsoOverPtCutEff",
             ";2 * TrackIso + JurassicIso / Pt;Efficiency", 201,0,4.0); 

//*************************************************************************************************
// Isolation Vs Pt Plots
//*************************************************************************************************
  fElectronTrackIsoVsPt = 
    new TH2D("hElectronTrackIsoVsPt",";TrackIsolation;Pt",
             101,0,20.0,101,0,100.0);  
  fElectronCaloIsoVsPt = 
    new TH2D("hElectronCaloIsoVsPt",";Ecal Isolation;Pt",
             101,0,20.0,101,0,100.0);  
  fElectronCaloTowerIsoVsPt = 
    new TH2D("hElectronCaloTowerIsoVsPt",";Hcal Isolation;Pt",
             101,0,20.0,101,0,100.0);  
  fElectronJurassicEcalIsoVsPt = 
    new TH2D("hElectronJurassicEcalIsoVsPt",";Jurassic Ecal Isolation;Pt",
             101,0,20.0,101,0,100.0);  
  fElectronHcalIsoVsPt = 
    new TH2D("hElectronHcalIsoVsPt",";Jurassic Hcal Isolation;Pt",
             101,0,20.0,101,0,100.0);  
  fElectronJurassicIsoVsPt = 
    new TH2D("hElectronJurassicIsoVsPt",";Jurassic Ecal + Hcal Isolation;Pt",
             101,0,20.0,101,0,100.0);  
  fElectronTrackPlusJurassicIsoVsPt = 
    new TH2D("hElectronTrackPlusJurassicIsoVsPt",";Track + Jurassic Ecal + Hcal Isolation;Pt",
             101,0,20.0,101,0,100.0);  


  AddOutput(fElectronPtHist);
  AddOutput(fElectronEtaHist);
  AddOutput(fElectronTrackIsoVsCaloIso);
  AddOutput(fElectronTrackIsoVsCaloTowerIso);
  AddOutput(fElectronTrackIsoVsEcalJurassicIso);
  AddOutput(fElectronTrackIsoVsHcalIso);
  AddOutput(fNElectronPassTrackAndCaloIso );
  AddOutput(fNElectronPassTrackAndCaloTowerIso );
  AddOutput(fElectronTrackAndCaloIsoCutEff  );
  AddOutput(fElectronTrackAndCaloTowerIsoCutEff );
  AddOutput(fElectronTrackIso);
  AddOutput(fElectronCaloIso);
  AddOutput(fElectronCaloTowerIso);
  AddOutput(fElectronEcalJurassicIso);
  AddOutput(fElectronHcalIso);
  AddOutput(fElectronTotalJurassicIso);
  AddOutput(fElectronTrackIsoPlusCaloIso);
  AddOutput(fElectronTrackIsoPlusCaloTowerIso);
  AddOutput(fElectronTrackIsoPlusJurassicIso);
  AddOutput(fElectronTwoTimesTrackIsoPlusCaloTowerIso);
  AddOutput(fElectronTwoTimesTrackIsoPlusJurassicIso);
  AddOutput(fElectronTrackIsoOverPt);
  AddOutput(fElectronCaloIsoOverPt);
  AddOutput(fElectronCaloTowerIsoOverPt);
  AddOutput(fElectronEcalJurassicIsoOverPt);
  AddOutput(fElectronHcalIsoOverPt);
  AddOutput(fElectronTotalJurassicIsoOverPt);
  AddOutput(fElectronTrackIsoPlusCaloIsoOverPt);
  AddOutput(fElectronTrackIsoPlusCaloTowerIsoOverPt);
  AddOutput(fElectronTrackIsoPlusJurassicIsoOverPt);
  AddOutput(fElectronTwoTimesTrackIsoPlusCaloTowerIsoOverPt);
  AddOutput(fElectronTwoTimesTrackIsoPlusJurassicIsoOverPt);
  AddOutput(fNElectronPassTrackIsoCut);
  AddOutput(fNElectronPassCaloIsoCut);
  AddOutput(fNElectronPassCaloTowerIsoCut);
  AddOutput(fNElectronPassEcalJurassicIsoCut);
  AddOutput(fNElectronPassHcalIsoCut);
  AddOutput(fNElectronPassTotalJurassicIsoCut);
  AddOutput(fNElectronPassTrackIsoPlusCaloIsoCut);
  AddOutput(fNElectronPassTrackIsoPlusCaloTowerIsoCut);
  AddOutput(fNElectronPassTrackIsoPlusJurassicIsoCut);
  AddOutput(fNElectronPassTwoTimesTrackIsoPlusCaloTowerIsoCut);
  AddOutput(fNElectronPassTwoTimesTrackIsoPlusJurassicIsoCut);
  AddOutput(fNElectronPassTrackIsoOverPtCut);
  AddOutput(fNElectronPassCaloIsoOverPtCut);
  AddOutput(fNElectronPassCaloTowerIsoOverPtCut);
  AddOutput(fNElectronPassEcalJurassicIsoOverPtCut);
  AddOutput(fNElectronPassHcalIsoOverPtCut);
  AddOutput(fNElectronPassTotalJurassicIsoOverPtCut);
  AddOutput(fNElectronPassTrackIsoPlusCaloIsoOverPtCut);
  AddOutput(fNElectronPassTrackIsoPlusCaloTowerIsoOverPtCut);
  AddOutput(fNElectronPassTrackIsoPlusJurassicIsoOverPtCut);
  AddOutput(fNElectronPassTwoTimesTrackIsoPlusCaloTowerIsoOverPtCut);
  AddOutput(fNElectronPassTwoTimesTrackIsoPlusJurassicIsoOverPtCut); 
  AddOutput(fElectronTrackIsoCutEff);
  AddOutput(fElectronCaloIsoCutEff);
  AddOutput(fElectronCaloTowerIsoCutEff);
  AddOutput(fElectronEcalJurassicIsoCutEff);
  AddOutput(fElectronHcalIsoCutEff);
  AddOutput(fElectronTotalJurassicIsoCutEff);
  AddOutput(fElectronTrackIsoPlusCaloIsoCutEff);
  AddOutput(fElectronTrackIsoPlusCaloTowerIsoCutEff);
  AddOutput(fElectronTrackIsoPlusJurassicIsoCutEff);
  AddOutput(fElectronTwoTimesTrackIsoPlusCaloTowerIsoCutEff);
  AddOutput(fElectronTwoTimesTrackIsoPlusJurassicIsoCutEff);
  AddOutput(fElectronTrackIsoOverPtCutEff);
  AddOutput(fElectronCaloIsoOverPtCutEff);
  AddOutput(fElectronCaloTowerIsoOverPtCutEff);
  AddOutput(fElectronEcalJurassicIsoOverPtCutEff);
  AddOutput(fElectronHcalIsoOverPtCutEff);
  AddOutput(fElectronTotalJurassicIsoOverPtCutEff);
  AddOutput(fElectronTrackIsoPlusCaloIsoOverPtCutEff);
  AddOutput(fElectronTrackIsoPlusCaloTowerIsoOverPtCutEff);
  AddOutput(fElectronTrackIsoPlusJurassicIsoOverPtCutEff);
  AddOutput(fElectronTwoTimesTrackIsoPlusCaloTowerIsoOverPtCutEff);
  AddOutput(fElectronTwoTimesTrackIsoPlusJurassicIsoOverPtCutEff);
  AddOutput(fElectronTrackIsoVsPt);
  AddOutput(fElectronCaloIsoVsPt);
  AddOutput(fElectronCaloTowerIsoVsPt); 
  AddOutput(fElectronJurassicEcalIsoVsPt);
  AddOutput(fElectronHcalIsoVsPt); 
  AddOutput(fElectronJurassicIsoVsPt); 
  AddOutput(fElectronTrackPlusJurassicIsoVsPt); 

}

//--------------------------------------------------------------------------------------------------
void ElectronIsolationStudyMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.

  cerr << "Number of Events" << fNEventsProcessed  << endl;


  double electronWeight = 1 / (fNElectrons + 0.0);
  cerr << "weight = " << electronWeight << endl;

  for (int i=1;i<=100;i++) {    
    double isolation = 0.0 + i*0.2;   
    fNElectronPassTrackIsoCut->Fill(isolation,fElectronTrackIso->Integral(0,i));
    fNElectronPassCaloIsoCut->Fill(isolation,fElectronCaloIso->Integral(0,i));
    fNElectronPassCaloTowerIsoCut->Fill(isolation,fElectronCaloTowerIso->Integral(0,i));
    fNElectronPassEcalJurassicIsoCut->Fill(isolation,fElectronEcalJurassicIso->Integral(0,i));
    fNElectronPassHcalIsoCut->Fill(isolation,fElectronHcalIso->Integral(0,i));
    fNElectronPassTotalJurassicIsoCut->Fill(isolation,fElectronTotalJurassicIso->Integral(0,i));
    fNElectronPassTrackIsoPlusCaloIsoCut->Fill(isolation,fElectronTrackIsoPlusCaloIso->Integral(0,i));
    fNElectronPassTrackIsoPlusCaloTowerIsoCut->Fill(isolation,fElectronTrackIsoPlusCaloTowerIso->Integral(0,i));
    fNElectronPassTrackIsoPlusJurassicIsoCut->Fill(isolation,fElectronTrackIsoPlusJurassicIso->Integral(0,i));
    fNElectronPassTwoTimesTrackIsoPlusCaloTowerIsoCut->Fill(isolation,fElectronTwoTimesTrackIsoPlusCaloTowerIso->Integral(0,i));
    fNElectronPassTwoTimesTrackIsoPlusJurassicIsoCut->Fill(isolation,fElectronTwoTimesTrackIsoPlusJurassicIso->Integral(0,i));
    
    
    fElectronTrackIsoCutEff->Fill(isolation,electronWeight*fElectronTrackIso->Integral(0,i));
    fElectronCaloIsoCutEff->Fill(isolation,electronWeight*fElectronCaloIso->Integral(0,i));
    fElectronCaloTowerIsoCutEff->Fill(isolation,electronWeight*fElectronCaloTowerIso->Integral(0,i));
    fElectronEcalJurassicIsoCutEff->Fill(isolation,electronWeight*fElectronEcalJurassicIso->Integral(0,i));
    fElectronHcalIsoCutEff->Fill(isolation,electronWeight*fElectronHcalIso->Integral(0,i));
    fElectronTotalJurassicIsoCutEff->Fill(isolation,electronWeight*fElectronTotalJurassicIso->Integral(0,i));
    fElectronTrackIsoPlusCaloIsoCutEff->Fill(isolation,electronWeight*fElectronTrackIsoPlusCaloIso->Integral(0,i));
    fElectronTrackIsoPlusCaloTowerIsoCutEff->Fill(isolation,electronWeight*fElectronTrackIsoPlusCaloTowerIso->Integral(0,i));
    fElectronTrackIsoPlusJurassicIsoCutEff->Fill(isolation,electronWeight*fElectronTrackIsoPlusJurassicIso->Integral(0,i));
    fElectronTwoTimesTrackIsoPlusCaloTowerIsoCutEff->Fill(isolation,electronWeight*fElectronTwoTimesTrackIsoPlusCaloTowerIso->Integral(0,i));
    fElectronTwoTimesTrackIsoPlusJurassicIsoCutEff->Fill(isolation,electronWeight*fElectronTwoTimesTrackIsoPlusJurassicIso->Integral(0,i));
  }

  //isolation over pt  
  for (int i=1;i<=200;i++) {
    double relativeIsolation = 0.0 + i*0.02;
    fNElectronPassTrackIsoOverPtCut->Fill(relativeIsolation,fElectronTrackIsoOverPt->Integral(0,i));
    fNElectronPassCaloIsoOverPtCut->Fill(relativeIsolation,fElectronCaloIsoOverPt->Integral(0,i));
    fNElectronPassCaloTowerIsoOverPtCut->Fill(relativeIsolation,fElectronCaloTowerIsoOverPt->Integral(0,i));
    fNElectronPassEcalJurassicIsoOverPtCut->Fill(relativeIsolation,fElectronEcalJurassicIsoOverPt->Integral(0,i));
    fNElectronPassHcalIsoOverPtCut->Fill(relativeIsolation,fElectronHcalIsoOverPt->Integral(0,i));
    fNElectronPassTotalJurassicIsoOverPtCut->Fill(relativeIsolation,fElectronTotalJurassicIsoOverPt->Integral(0,i));
    fNElectronPassTrackIsoPlusCaloIsoOverPtCut->Fill(relativeIsolation,fElectronTrackIsoPlusCaloIsoOverPt->Integral(0,i));
    fNElectronPassTrackIsoPlusCaloTowerIsoOverPtCut->Fill(relativeIsolation,fElectronTrackIsoPlusCaloTowerIsoOverPt->Integral(0,i));
    fNElectronPassTrackIsoPlusJurassicIsoOverPtCut->Fill(relativeIsolation,fElectronTrackIsoPlusJurassicIsoOverPt->Integral(0,i));
    fNElectronPassTwoTimesTrackIsoPlusCaloTowerIsoOverPtCut->Fill(relativeIsolation,fElectronTwoTimesTrackIsoPlusCaloTowerIsoOverPt->Integral(0,i));
    fNElectronPassTwoTimesTrackIsoPlusJurassicIsoOverPtCut->Fill(relativeIsolation,fElectronTwoTimesTrackIsoPlusJurassicIsoOverPt->Integral(0,i));
    fElectronTrackIsoOverPtCutEff->Fill(relativeIsolation,electronWeight*fElectronTrackIsoOverPt->Integral(0,i));
    fElectronCaloIsoOverPtCutEff->Fill(relativeIsolation,electronWeight*fElectronCaloIsoOverPt->Integral(0,i));
    fElectronCaloTowerIsoOverPtCutEff->Fill(relativeIsolation,electronWeight*fElectronCaloTowerIsoOverPt->Integral(0,i));
    fElectronEcalJurassicIsoOverPtCutEff->Fill(relativeIsolation,electronWeight*fElectronEcalJurassicIsoOverPt->Integral(0,i));
    fElectronHcalIsoOverPtCutEff->Fill(relativeIsolation,electronWeight*fElectronHcalIsoOverPt->Integral(0,i));
    fElectronTotalJurassicIsoOverPtCutEff->Fill(relativeIsolation,electronWeight*fElectronTotalJurassicIsoOverPt->Integral(0,i));
    fElectronTrackIsoPlusCaloIsoOverPtCutEff->Fill(relativeIsolation,electronWeight*fElectronTrackIsoPlusCaloIsoOverPt->Integral(0,i));
    fElectronTrackIsoPlusCaloTowerIsoOverPtCutEff->Fill(relativeIsolation,electronWeight*fElectronTrackIsoPlusCaloTowerIsoOverPt->Integral(0,i));
    fElectronTrackIsoPlusJurassicIsoOverPtCutEff->Fill(relativeIsolation,electronWeight*fElectronTrackIsoPlusJurassicIsoOverPt->Integral(0,i));
    fElectronTwoTimesTrackIsoPlusCaloTowerIsoOverPtCutEff->Fill(relativeIsolation,electronWeight*fElectronTwoTimesTrackIsoPlusCaloTowerIsoOverPt->Integral(0,i));
    fElectronTwoTimesTrackIsoPlusJurassicIsoOverPtCutEff->Fill(relativeIsolation,electronWeight*fElectronTwoTimesTrackIsoPlusJurassicIsoOverPt->Integral(0,i));
  }

  for (int t = 0;t < 101; t++) {
    double trackIsoCut = 0.0 + t*0.2;
    for (int c = 0; c < 101 ; c++ ) {      
      double caloIsoCut = 0.0 + c*0.2;
      fElectronTrackAndCaloIsoCutEff->Fill(trackIsoCut,caloIsoCut,electronWeight*fNElectronPassTrackAndCaloIso->GetBinContent(t+1,c+1));
    }
    
    for (int c = 0; c < 101 ; c++ ) {
      double caloTowerIsoCut = 0.0 + c*0.2;
      fElectronTrackAndCaloTowerIsoCutEff->Fill(trackIsoCut, caloTowerIsoCut, electronWeight*fNElectronPassTrackAndCaloTowerIso->GetBinContent(t+1,c+1)) ;
    }
  }

  cerr << "efficiency of default cuts : " << fNElectronsPassDefaultIsolation / (0.0+fNElectrons) << endl;

}

//--------------------------------------------------------------------------------------------------
void ElectronIsolationStudyMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
