// $Id: JetIDMod.cc,v 1.3 2009/03/06 16:48:04 phedex Exp $

#include "MitPhysics/Mods/interface/JetIDMod.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitPhysics/Init/interface/ModNames.h"

using namespace mithep;

ClassImp(mithep::JetIDMod)

//--------------------------------------------------------------------------------------------------
JetIDMod::JetIDMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fJetBranchName(Names::gkSC5JetBrn),
  fGoodJetsName(ModNames::gkGoodJetsName),  
  fUseJetCorrection(kTRUE),
  fJetPtCut(35.0),
  fJets(0)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void JetIDMod::Process()
{
  // Process entries of the tree. 

  LoadBranch(fJetBranchName);

  JetOArr *GoodJets = new JetOArr; 
  GoodJets->SetName(fGoodJetsName);

  // loop over jets
  for (UInt_t i=0; i<fJets->GetEntries(); ++i) {
    const Jet *jet = fJets->At(i);

    if (jet->AbsEta() > 5.0) 
      continue;
    
    Double_t jetpt = jet->Pt();
    if (fUseJetCorrection)
      jetpt *= jet->L2RelativeCorrectionScale() * jet->L3AbsoluteCorrectionScale();

    if (jetpt < fJetPtCut)
      continue;
    
    // add good jet to collection
    GoodJets->Add(jet);             
  }

  // sort according to pt
  GoodJets->Sort();
  
  // add to event for other modules to use
  AddObjThisEvt(GoodJets);  
}

//--------------------------------------------------------------------------------------------------
void JetIDMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we just request the jet collection branch.

  ReqBranch(fJetBranchName, fJets);
}
