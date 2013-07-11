// $Id: ComputeElectronFakeRateMod.cc,v 1.7 2009/06/30 08:58:50 phedex Exp $

#include "MitHiggs/FakeRateMods/interface/ComputeElectronFakeRateMod.h"
#include "MitAna/DataUtil/interface/Debug.h"
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include "MitAna/DataTree/interface/Names.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitPhysics/Utils/interface/IsolationTools.h"
#include "MitPhysics/Utils/interface/GeneratorTools.h"
#include <map>

using namespace mithep;

ClassImp(mithep::ComputeElectronFakeRateMod)

//--------------------------------------------------------------------------------------------------
  ComputeElectronFakeRateMod::ComputeElectronFakeRateMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fVetoTriggerJet(true),
  fVetoGeneratorLeptons(false),
  fSampleType(0),
  fMCPartBranchName(Names::gkMCPartBrn),
  fGenJetBranchName(Names::gkSC5GenJetBrn),
  fTriggerName("NotSpecified"),
  fTriggerObjectsName("NotSpecified"),
  fTrackBranchName(Names::gkTrackBrn),
  fGsfTrackBranchName(Names::gkGsfTrackBrn),
  fElectronBranchName(Names::gkElectronBrn),
  fBarrelSuperClusterBranchName(Names::gkBarrelSuperClusterBrn),
  fEndcapSuperClusterBranchName(Names::gkEndcapSuperClusterBrn),
  fMetName("NotSet"),
  fCleanElectronsName(ModNames::gkCleanElectronsName),
  fGoodJetsName(ModNames::gkGoodJetsName),
  fCleanJetsName(ModNames::gkCleanJetsName),
  fMCLeptonsName(ModNames::gkMCLeptonsName),
  fMCTausName(ModNames::gkMCTausName),
  fElectronFakeableObjectsName(ModNames::gkElectronFakeableObjectsName),
  fParticles(0),
  fBarrelSuperClusters(0),
  fEndcapSuperClusters(0),
  fTracks(0),
  fGsfTracks(0),
  fElectrons(0),
  fMet(0)
{ 
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void ComputeElectronFakeRateMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.

}

//--------------------------------------------------------------------------------------------------
void ComputeElectronFakeRateMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  if (fSampleType == 0) {
    cout 
      << "Error: You did not set SampleType, or you set it to 0. Please set correct SampleType.\n";
    assert(false);
  }

  ReqBranch(fMCPartBranchName,                fParticles);
  ReqBranch(fGenJetBranchName,                fGenJets);
  ReqBranch(fTrackBranchName,                 fTracks);
  ReqBranch(fGsfTrackBranchName,              fGsfTracks);
  ReqBranch(fElectronBranchName,              fElectrons);
  ReqBranch(fBarrelSuperClusterBranchName,    fBarrelSuperClusters);
  ReqBranch(fEndcapSuperClusterBranchName,    fEndcapSuperClusters);


  //*******************************************************************************************
  //Trigger Object Histograms
  //*******************************************************************************************
  AddTH1(fTriggerJetPt, "hTriggerJetPt", 
         ";TriggerJetPt;Number of Events", 50, 0, 250);
  AddTH1(fTriggerJetEta, "hTriggerJetEta", 
         ";TriggerJetEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fTriggerJetPhi, "hTriggerJetPhi", 
         ";TriggerJetPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fTriggerPhotonPt, "hTriggerPhotonPt", 
         ";TriggerPhotonPt;Number of Events", 50, 0, 250);
  AddTH1(fTriggerPhotonEta, "hTriggerPhotonEta", 
         ";TriggerPhotonEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fTriggerPhotonPhi, "hTriggerPhotonPhi", 
         ";TriggerPhotonPhi;Number of Events", 100, -3.2, 3.2);

  //*******************************************************************************************
  //Kinematic Variables for signal contamination removal 
  //*******************************************************************************************
  AddTH1(fNCleanLeptons, "hNCleanLeptons", 
         ";Number of Clean Leptons;Number of Events", 10, -0.5, 9.5);
  AddTH1(fMetHist, "hMetHist", 
         ";Missing Transverse Energy;Number of Events", 150,0.,300.);
  AddTH1(fNumeratorJetMass ,"hNumeratorJetMass",
          ";Mass of the Numerator-Jet system;Number of Events",150,0.,300.);
  AddTH1(fDenominatorJetMass ,"hDenominatorJetMass",
          ";Mass of the Denominator-Jet system;Number of Events",150,0.,300.);
  AddTH1(fNumeratorPhotonMass ,"hNumeratorPhotonMass",
          ";Mass of the Numerator-Photon system;Number of Events",150,0.,300.);
  AddTH1(fDenominatorPhotonMass ,"hDenominatorPhotonMass",
          ";Mass of the Denominator-Photon system;Number of Events",150,0.,300.);
  AddTH1(fDeltaPhiTriggerPhotonToLeadingJet ,"hDeltaPhiTriggerPhotonToLeadingJet",
         ";DeltaPhi TriggerPhoton-LeadingJet;Number of Events",100,0,3.2);

  //*******************************************************************************************
  //Denominator Histograms
  //*******************************************************************************************
  AddTH1(fElectronDenominatorPt, "hElectronDenominatorPt", 
         ";ElectronDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronDenominatorEta, "hElectronDenominatorEta", 
         ";ElectronDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronDenominatorPhi, "hElectronDenominatorPhi", 
         ";ElectronDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronDenominatorPtEta, "hElectronDenominatorPtEta", 
         ";ElectronDenominatorPt;ElectronDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);
  AddTH1(fPlusElectronDenominatorPt, "hPlusElectronDenominatorPt", 
         ";PlusElectronDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fPlusElectronDenominatorEta, "hPlusElectronDenominatorEta", 
         ";PlusElectronDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fPlusElectronDenominatorPhi, "hPlusElectronDenominatorPhi", 
         ";PlusElectronDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fMinusElectronDenominatorPt, "hMinusElectronDenominatorPt", 
         ";MinusElectronDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMinusElectronDenominatorEta, "hMinusElectronDenominatorEta", 
         ";MinusElectronDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMinusElectronDenominatorPhi, "hMinusElectronDenominatorPhi", 
         ";MinusElectronDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fPlusElectronPlusWDenominatorPt, "hPlusElectronPlusWDenominatorPt", 
         ";PlusElectronPlusWDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fPlusElectronPlusWDenominatorEta, "hPlusElectronPlusWDenominatorEta", 
         ";PlusElectronPlusWDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fPlusElectronPlusWDenominatorPhi, "hPlusElectronPlusWDenominatorPhi", 
         ";PlusElectronPlusWDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fMinusElectronPlusWDenominatorPt, "hMinusElectronPlusWDenominatorPt", 
         ";MinusElectronPlusWDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMinusElectronPlusWDenominatorEta, "hMinusElectronPlusWDenominatorEta", 
         ";MinusElectronPlusWDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMinusElectronPlusWDenominatorPhi, "hMinusElectronPlusWDenominatorPhi", 
         ";MinusElectronPlusWDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fPlusElectronMinusWDenominatorPt, "hPlusElectronMinusWDenominatorPt", 
         ";PlusElectronMinusWDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fPlusElectronMinusWDenominatorEta, "hPlusElectronMinusWDenominatorEta", 
         ";PlusElectronMinusWDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fPlusElectronMinusWDenominatorPhi, "hPlusElectronMinusWDenominatorPhi", 
         ";PlusElectronMinusWDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fMinusElectronMinusWDenominatorPt, "hMinusElectronMinusWDenominatorPt", 
         ";MinusElectronMinusWDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMinusElectronMinusWDenominatorEta, "hMinusElectronMinusWDenominatorEta", 
         ";MinusElectronMinusWDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMinusElectronMinusWDenominatorPhi, "hMinusElectronMinusWDenominatorPhi", 
         ";MinusElectronMinusWDenominatorPhi;Number of Events", 100, -3.2, 3.2);

  AddTH2(fPredictionElectronDenominatorPtEta, "hPredictionElectronDenominatorPtEta", 
         ";ElectronDenominatorPt;ElectronDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronDenominatorFakeCategory, "hElectronDenominatorFakeCategory", 
         ";Electron Fake Category;Number of Events", 8,-0.5,7.5);
  TAxis *xa = fElectronDenominatorFakeCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");

  AddTH1(fElectronDenominatorJetFlavor, "hElectronDenominatorJetFlavor", 
         ";Jet Flavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fElectronDenominatorFragmentationX, "hElectronDenominatorFragmentationX", 
         ";AncestorFragmentationX;Number of Events", 100, 0,2);


  //*******************************************************************************************
  //Numerator Histograms
  //*******************************************************************************************
  AddTH1(fElectronNumeratorPt, "hElectronNumeratorPt", 
         ";ElectronNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronNumeratorEta, "hElectronNumeratorEta", 
         ";ElectronNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronNumeratorPhi, "hElectronNumeratorPhi", 
         ";ElectronNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronNumeratorPtEta, "hElectronNumeratorPtEta", 
         ";ElectronNumeratorPt;ElectronNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);
  AddTH1(fPlusElectronNumeratorPt, "hPlusElectronNumeratorPt", 
         ";PlusElectronNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fPlusElectronNumeratorEta, "hPlusElectronNumeratorEta", 
         ";PlusElectronNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fPlusElectronNumeratorPhi, "hPlusElectronNumeratorPhi", 
         ";PlusElectronNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fMinusElectronNumeratorPt, "hMinusElectronNumeratorPt", 
         ";MinusElectronNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fMinusElectronNumeratorEta, "hMinusElectronNumeratorEta", 
         ";MinusElectronNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMinusElectronNumeratorPhi, "hMinusElectronNumeratorPhi", 
         ";MinusElectronNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fPlusElectronPlusWNumeratorPt, "hPlusElectronPlusWNumeratorPt", 
         ";PlusElectronPlusWNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fPlusElectronPlusWNumeratorEta, "hPlusElectronPlusWNumeratorEta", 
         ";PlusElectronPlusWNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fPlusElectronPlusWNumeratorPhi, "hPlusElectronPlusWNumeratorPhi", 
         ";PlusElectronPlusWNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fMinusElectronPlusWNumeratorPt, "hMinusElectronPlusWNumeratorPt", 
         ";MinusElectronPlusWNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fMinusElectronPlusWNumeratorEta, "hMinusElectronPlusWNumeratorEta", 
         ";MinusElectronPlusWNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMinusElectronPlusWNumeratorPhi, "hMinusElectronPlusWNumeratorPhi", 
         ";MinusElectronPlusWNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fPlusElectronMinusWNumeratorPt, "hPlusElectronMinusWNumeratorPt", 
         ";PlusElectronMinusWNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fPlusElectronMinusWNumeratorEta, "hPlusElectronMinusWNumeratorEta", 
         ";PlusElectronMinusWNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fPlusElectronMinusWNumeratorPhi, "hPlusElectronMinusWNumeratorPhi", 
         ";PlusElectronMinusWNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fMinusElectronMinusWNumeratorPt, "hMinusElectronMinusWNumeratorPt", 
         ";MinusElectronMinusWNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fMinusElectronMinusWNumeratorEta, "hMinusElectronMinusWNumeratorEta", 
         ";MinusElectronMinusWNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMinusElectronMinusWNumeratorPhi, "hMinusElectronMinusWNumeratorPhi", 
         ";MinusElectronMinusWNumeratorPhi;Number of Events", 100, -3.2, 3.2);

  AddTH2(fPredictionElectronNumeratorPtEta, "hPredictionElectronNumeratorPtEta", 
         ";ElectronNumeratorPt;ElectronNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronNumeratorFakeCategory, "hElectronNumeratorFakeCategory", 
         ";Electron Fake Category;Number of Events", 8,-0.5,7.5);
  xa = fElectronNumeratorFakeCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");

  AddTH1(fElectronNumeratorJetFlavor, "hElectronNumeratorJetFlavor", 
         ";Jet Flavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fElectronNumeratorFragmentationX, "hElectronNumeratorFragmentationX", 
         ";AncestorFragmentationX;Number of Events", 100, 0,2);


  //*******************************************************************************************
  //Denominator For Different Fake Categories
  //*******************************************************************************************
  AddTH1(fElectronDenominatorPt_NoMatch, "hElectronDenominatorPt_NoMatch", 
         ";ElectronDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronDenominatorEta_NoMatch, "hElectronDenominatorEta_NoMatch", 
         ";ElectronDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronDenominatorPhi_NoMatch, "hElectronDenominatorPhi_NoMatch", 
         ";ElectronDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronDenominatorPtEta_NoMatch, "hElectronDenominatorPtEta_NoMatch", 
         ";ElectronDenominatorPt;ElectronDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronDenominatorPt_Conversion, "hElectronDenominatorPt_Conversion", 
         ";ElectronDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronDenominatorEta_Conversion, "hElectronDenominatorEta_Conversion", 
         ";ElectronDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronDenominatorPhi_Conversion, "hElectronDenominatorPhi_Conversion", 
         ";ElectronDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronDenominatorPtEta_Conversion, "hElectronDenominatorPtEta_Conversion", 
         ";ElectronDenominatorPt;ElectronDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronDenominatorPt_ChargedPion, "hElectronDenominatorPt_ChargedPion", 
         ";ElectronDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronDenominatorEta_ChargedPion, "hElectronDenominatorEta_ChargedPion", 
         ";ElectronDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronDenominatorPhi_ChargedPion, "hElectronDenominatorPhi_ChargedPion", 
         ";ElectronDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronDenominatorPtEta_ChargedPion, "hElectronDenominatorPtEta_ChargedPion", 
         ";ElectronDenominatorPt;ElectronDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronDenominatorPt_ChargedKaon, "hElectronDenominatorPt_ChargedKaon", 
         ";ElectronDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronDenominatorEta_ChargedKaon, "hElectronDenominatorEta_ChargedKaon", 
         ";ElectronDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronDenominatorPhi_ChargedKaon, "hElectronDenominatorPhi_ChargedKaon", 
         ";ElectronDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronDenominatorPtEta_ChargedKaon, "hElectronDenominatorPtEta_ChargedKaon", 
         ";ElectronDenominatorPt;ElectronDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronDenominatorPt_HeavyFlavor, "hElectronDenominatorPt_HeavyFlavor", 
         ";ElectronDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronDenominatorEta_HeavyFlavor, "hElectronDenominatorEta_HeavyFlavor", 
         ";ElectronDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronDenominatorPhi_HeavyFlavor, "hElectronDenominatorPhi_HeavyFlavor", 
         ";ElectronDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronDenominatorPtEta_HeavyFlavor, "hElectronDenominatorPtEta_HeavyFlavor", 
         ";ElectronDenominatorPt;ElectronDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronDenominatorPt_Brem, "hElectronDenominatorPt_Brem", 
         ";ElectronDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronDenominatorEta_Brem, "hElectronDenominatorEta_Brem", 
         ";ElectronDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronDenominatorPhi_Brem, "hElectronDenominatorPhi_Brem", 
         ";ElectronDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronDenominatorPtEta_Brem, "hElectronDenominatorPtEta_Brem", 
         ";ElectronDenominatorPt;ElectronDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronDenominatorPt_PromptPhoton, "hElectronDenominatorPt_PromptPhoton", 
         ";ElectronDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronDenominatorEta_PromptPhoton, "hElectronDenominatorEta_PromptPhoton", 
         ";ElectronDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronDenominatorPhi_PromptPhoton, "hElectronDenominatorPhi_PromptPhoton", 
         ";ElectronDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronDenominatorPtEta_PromptPhoton, "hElectronDenominatorPtEta_PromptPhoton", 
         ";ElectronDenominatorPt;ElectronDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronDenominatorPt_Other, "hElectronDenominatorPt_Other", 
         ";ElectronDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronDenominatorEta_Other, "hElectronDenominatorEta_Other", 
         ";ElectronDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronDenominatorPhi_Other, "hElectronDenominatorPhi_Other", 
         ";ElectronDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronDenominatorPtEta_Other, "hElectronDenominatorPtEta_Other", 
         ";ElectronDenominatorPt;ElectronDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  //*******************************************************************************************
  //Numerator For Different Fake Categories
  //*******************************************************************************************
  AddTH1(fElectronNumeratorPt_NoMatch, "hElectronNumeratorPt_NoMatch", 
         ";ElectronNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronNumeratorEta_NoMatch, "hElectronNumeratorEta_NoMatch", 
         ";ElectronNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronNumeratorPhi_NoMatch, "hElectronNumeratorPhi_NoMatch", 
         ";ElectronNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronNumeratorPtEta_NoMatch, "hElectronNumeratorPtEta_NoMatch", 
         ";ElectronNumeratorPt;ElectronNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronNumeratorPt_Conversion, "hElectronNumeratorPt_Conversion", 
         ";ElectronNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronNumeratorEta_Conversion, "hElectronNumeratorEta_Conversion", 
         ";ElectronNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronNumeratorPhi_Conversion, "hElectronNumeratorPhi_Conversion", 
         ";ElectronNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronNumeratorPtEta_Conversion, "hElectronNumeratorPtEta_Conversion", 
         ";ElectronNumeratorPt;ElectronNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronNumeratorPt_ChargedPion, "hElectronNumeratorPt_ChargedPion", 
         ";ElectronNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronNumeratorEta_ChargedPion, "hElectronNumeratorEta_ChargedPion", 
         ";ElectronNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronNumeratorPhi_ChargedPion, "hElectronNumeratorPhi_ChargedPion", 
         ";ElectronNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronNumeratorPtEta_ChargedPion, "hElectronNumeratorPtEta_ChargedPion", 
         ";ElectronNumeratorPt;ElectronNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronNumeratorPt_ChargedKaon, "hElectronNumeratorPt_ChargedKaon", 
         ";ElectronNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronNumeratorEta_ChargedKaon, "hElectronNumeratorEta_ChargedKaon", 
         ";ElectronNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronNumeratorPhi_ChargedKaon, "hElectronNumeratorPhi_ChargedKaon", 
         ";ElectronNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronNumeratorPtEta_ChargedKaon, "hElectronNumeratorPtEta_ChargedKaon", 
         ";ElectronNumeratorPt;ElectronNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronNumeratorPt_HeavyFlavor, "hElectronNumeratorPt_HeavyFlavor", 
         ";ElectronNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronNumeratorEta_HeavyFlavor, "hElectronNumeratorEta_HeavyFlavor", 
         ";ElectronNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronNumeratorPhi_HeavyFlavor, "hElectronNumeratorPhi_HeavyFlavor", 
         ";ElectronNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronNumeratorPtEta_HeavyFlavor, "hElectronNumeratorPtEta_HeavyFlavor", 
         ";ElectronNumeratorPt;ElectronNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronNumeratorPt_Brem, "hElectronNumeratorPt_Brem", 
         ";ElectronNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronNumeratorEta_Brem, "hElectronNumeratorEta_Brem", 
         ";ElectronNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronNumeratorPhi_Brem, "hElectronNumeratorPhi_Brem", 
         ";ElectronNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronNumeratorPtEta_Brem, "hElectronNumeratorPtEta_Brem", 
         ";ElectronNumeratorPt;ElectronNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronNumeratorPt_PromptPhoton, "hElectronNumeratorPt_PromptPhoton", 
         ";ElectronNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronNumeratorEta_PromptPhoton, "hElectronNumeratorEta_PromptPhoton", 
         ";ElectronNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronNumeratorPhi_PromptPhoton, "hElectronNumeratorPhi_PromptPhoton", 
         ";ElectronNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronNumeratorPtEta_PromptPhoton, "hElectronNumeratorPtEta_PromptPhoton", 
         ";ElectronNumeratorPt;ElectronNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronNumeratorPt_Other, "hElectronNumeratorPt_Other", 
         ";ElectronNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronNumeratorEta_Other, "hElectronNumeratorEta_Other", 
         ";ElectronNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronNumeratorPhi_Other, "hElectronNumeratorPhi_Other", 
         ";ElectronNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronNumeratorPtEta_Other, "hElectronNumeratorPtEta_Other", 
         ";ElectronNumeratorPt;ElectronNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  //*******************************************************************************************
  //Specific Studies For ChargedPion , Conversion Categories
  //*******************************************************************************************

  AddTH1(fElectronNumeratorJetFlavor_Conversion, "hElectronNumeratorJetFlavor_Conversion", 
         ";Jet Flavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fElectronNumeratorJetFlavor_ChargedPion, "hElectronNumeratorJetFlavor_ChargedPion", 
         ";Jet Flavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fElectronNumeratorJetFlavor_ChargedKaon, "hElectronNumeratorJetFlavor_ChargedKaon", 
         ";Jet Flavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fElectronNumeratorJetFlavor_HeavyFlavor, "hElectronNumeratorJetFlavor_HeavyFlavor", 
         ";Jet Flavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fElectronNumeratorFragmentationX_Conversion, 
         "hElectronNumeratorFragmentationX_Conversion", ";Jet Flavor;Number of Events", 100, 0,2);
  AddTH1(fElectronNumeratorFragmentationX_ChargedPion, 
         "hElectronNumeratorFragmentationX_ChargedPion", ";Jet Flavor;Number of Events", 100, 0,2);
  AddTH1(fElectronNumeratorFragmentationX_ChargedKaon, 
         "hElectronNumeratorFragmentationX_ChargedKaon", ";Jet Flavor;Number of Events", 100, 0,2);
  AddTH1(fElectronNumeratorFragmentationX_HeavyFlavor, 
         "hElectronNumeratorFragmentationX_HeavyFlavor", ";Jet Flavor;Number of Events", 100, 0,2);

  AddTH1(fElectronDenominatorJetFlavor_Conversion, "hElectronDenominatorJetFlavor_Conversion", 
         ";Jet Flavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fElectronDenominatorJetFlavor_ChargedPion, "hElectronDenominatorJetFlavor_ChargedPion", 
         ";Jet Flavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fElectronDenominatorJetFlavor_ChargedKaon, "hElectronDenominatorJetFlavor_ChargedKaon", 
         ";Jet Flavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fElectronDenominatorJetFlavor_HeavyFlavor, "hElectronDenominatorJetFlavor_HeavyFlavor", 
         ";Jet Flavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fElectronDenominatorFragmentationX_Conversion, 
         "hElectronDenominatorFragmentationX_Conversion", ";Jet Flavor;Number of Events", 100, 0,2);
  AddTH1(fElectronDenominatorFragmentationX_ChargedPion, 
         "hElectronDenominatorFragmentationX_ChargedPion", ";Jet Flavor;Number of Events", 
         100, 0,2);
  AddTH1(fElectronDenominatorFragmentationX_ChargedKaon, 
         "hElectronDenominatorFragmentationX_ChargedKaon", ";Jet Flavor;Number of Events", 
         100, 0,2);
  AddTH1(fElectronDenominatorFragmentationX_HeavyFlavor, 
         "hElectronDenominatorFragmentationX_HeavyFlavor", ";Jet Flavor;Number of Events", 
         100, 0,2);


  //*******************************************************************************************
  //Additional Studies
  //*******************************************************************************************
  AddTH1(fSCTrackMatchDeltaR_All, "hSCTrackMatchDeltaR_All", 
         ";SCTrackMatchDeltaR;Number of Events", 50, 0, 1.0);
  AddTH1(fSCTrackMatchDeltaR_FrameworkDenominator, "hSCTrackMatchDeltaR_FrameworkDenominator", 
         ";SCTrackMatchDeltaR;Number of Events", 50, 0, 1.0);
  AddTH1(fSCTrackMatchDeltaR_EOverPDenominator, "hSCTrackMatchDeltaR_EOverPDenominator", 
         ";SCTrackMatchDeltaR;Number of Events", 50, 0, 1.0);
  AddTH1(fSCTrackMatchDeltaR_Numerator, "hSCTrackMatchDeltaR_Numerator", 
         ";SCTrackMatchDeltaR;Number of Events", 50, 0, 1.0);
  AddTH1(fSCTrackMatchDeltaPt_DRMatched, "hSCTrackMatchDeltaPt_DRMatched", 
         ";SCTrackMatchDeltaPt;Number of Events", 100, 0, 50);
  AddTH1(fSCTrackMatchDeltaPt_FrameworkDenominator, "hSCTrackMatchDeltaPt_FrameworkDenominator", 
         ";SCTrackMatchDeltaPt;Number of Events", 100, 0, 50); 
  AddTH1(fSCTrackMatchDeltaPt_EOverPDenominator, "hSCTrackMatchDeltaPt_EOverPDenominator", 
         ";SCTrackMatchDeltaPt;Number of Events", 100, 0, 50); 
  AddTH1(fSCTrackMatchEOverP_EOverPDenominator, "hSCTrackMatchEOverP_EOverPDenominator", 
         ";SCTrackMatchEOverP;Number of Events", 100, 0, 50); 
  AddTH1(fGsfTrackToRegularTrackDeltaR, "hGsfTrackToRegularTrackDeltaR", 
         ";GsfTrackToRegularTrackDeltaR;Number of Events", 100, 0, 0.1);
  AddTH1(fGsfTrackToRegularTrackDeltaRWithPtMatch, "hGsfTrackToRegularTrackDeltaRWithPtMatch", 
         ";GsfTrackToRegularTrackDeltaRWithPtMatch;Number of Events", 100, 0, 0.1);
  AddTH1(fGsfTrackToRegularTrackDeltaPt, "hGsfTrackToRegularTrackDeltaPt", 
         ";GsfTrackToRegularTrackDeltaPt;Number of Events", 100, 0, 50);
  AddTH1(fEOverP_RejectedGsfTracksWithinCone, "hEOverP_RejectedGsfTracksWithinCone", 
         ";EOverP_RejectedGsfTracksWithinCone;Number of Events", 100, 0, 5);
  AddTH1(fDeltaR_RejectedGsfTracks, "hDeltaR_RejectedGsfTracks", 
         ";DeltaR_RejectedGsfTracksWithinCone;Number of Events", 100, 0, 5);




  //*******************************************************************************************
  //Denominator Histograms With Cuts
  //*******************************************************************************************
  AddTH1(fElectronDenominatorPt_WithCuts, "hElectronDenominatorPt_WithCuts", 
         ";ElectronDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronDenominatorEta_WithCuts, "hElectronDenominatorEta_WithCuts", 
         ";ElectronDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronDenominatorPhi_WithCuts, "hElectronDenominatorPhi_WithCuts", 
         ";ElectronDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronDenominatorPtEta_WithCuts, "hElectronDenominatorPtEta_WithCuts", 
         ";ElectronDenominatorPt;ElectronDenominatorEta;Number of Events_WithCuts", 50, 0, 250, 100,-5.0,5.0);
  AddTH1(fElectronDenominatorFakeCategory_WithCuts, "hElectronDenominatorFakeCategory_WithCuts", 
         ";Electron Fake Category;Number of Events", 8,-0.5,7.5);
  xa = fElectronDenominatorFakeCategory_WithCuts->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");

  AddTH1(fElectronDenominatorJetFlavor_WithCuts, "hElectronDenominatorJetFlavor_WithCuts", 
         ";Jet Flavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fElectronDenominatorFragmentationX_WithCuts, "hElectronDenominatorFragmentationX_WithCuts", 
         ";AncestorFragmentaionX;Number of Events", 100, 0,2);


  //*******************************************************************************************
  //Numerator Histograms With Cuts
  //*******************************************************************************************

  AddTH1(fElectronNumeratorPt_WithCuts, "hElectronNumeratorPt_WithCuts", 
         ";ElectronNumeratorEt;Number of Events", 50, 0, 250);
  AddTH1(fElectronNumeratorEta_WithCuts, "hElectronNumeratorEta_WithCuts", 
         ";ElectronNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronNumeratorPhi_WithCuts, "hElectronNumeratorPhi_WithCuts", 
         ";ElectronNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fElectronNumeratorPtEta_WithCuts, "hElectronNumeratorPtEta_WithCuts", 
         ";ElectronNumeratorPt;ElectronNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fElectronNumeratorFakeCategory_WithCuts, "hElectronNumeratorFakeCategory_WithCuts", 
         ";Electron Fake Category;Number of Events", 8,-0.5,7.5);
  xa = fElectronNumeratorFakeCategory_WithCuts->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");

  AddTH1(fElectronNumeratorJetFlavor_WithCuts, "hElectronNumeratorJetFlavor_WithCuts", 
         ";Jet Flavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fElectronNumeratorFragmentationX_WithCuts, "hElectronNumeratorFragmentationX_WithCuts", 
         ";AncestorFragmentationX;Number of Events", 100, 0,2);

}

//--------------------------------------------------------------------------------------------------
void ComputeElectronFakeRateMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and
  //output debug info or not   
 
  LoadBranch(fMCPartBranchName);
  LoadBranch(fGenJetBranchName);
  LoadBranch(fElectronBranchName);
  LoadBranch(fTrackBranchName);
  LoadBranch(fGsfTrackBranchName);
  LoadBranch(fBarrelSuperClusterBranchName);
  LoadBranch(fEndcapSuperClusterBranchName);

  //get monte carlo collections
  const MCParticleCol *GenLeptons = 0;
  if (!fMCLeptonsName.IsNull())
    GenLeptons = GetObjThisEvt<MCParticleCol>(fMCLeptonsName);
  const MCParticleCol *GenTaus = 0;
  if (!fMCTausName.IsNull())
    GenTaus = GetObjThisEvt<MCParticleCol>(fMCTausName);
  ObjArray<MCParticle> *GenLeptonsAndTaus = new ObjArray<MCParticle>;
  for (UInt_t i=0; i<GenLeptons->GetEntries(); i++)
    GenLeptonsAndTaus->Add(GenLeptons->At(i));
  for (UInt_t i=0; i<GenTaus->GetEntries(); i++)
    GenLeptonsAndTaus->Add(GenTaus->At(i));
  const MCParticleCol *GenPhotons = 0;
  GenPhotons = GetObjThisEvt<MCParticleCol>(ModNames::gkMCPhotonsName);

  // get input clean object collections
  const ElectronCol *CleanElectrons = 0;
  if (!fCleanElectronsName.IsNull())
    CleanElectrons = GetObjThisEvt<ElectronCol>(fCleanElectronsName);
  const JetCol *CleanJets = 0;
  if (!fCleanJetsName.IsNull())
    CleanJets = GetObjThisEvt<JetCol>(fCleanJetsName);
  mithep::ParticleOArr *CleanLeptons = dynamic_cast<mithep::ParticleOArr*>
    (FindObjThisEvt(ModNames::gkMergedLeptonsName));
  PhotonOArr *CleanPhotons = dynamic_cast<mithep::PhotonOArr*>
    (FindObjThisEvt(ModNames::gkCleanPhotonsName));

  //uncleaned jets
  const JetCol *GoodJets = 0;
  if (!fGoodJetsName.IsNull()) {
    GoodJets = GetObjThisEvt<JetCol>(fGoodJetsName);
  }
  if (!GoodJets) {
    cerr << "Error: cannot get GoodJets collection " << fGoodJetsName.Data() << endl;
  }

  //Get Met
  if (!fMetName.IsNull()) {
    fMet = GetObjThisEvt<MetCol>(fMetName);
  } 
  const Met *caloMet = 0;
  if (fMet) {
    caloMet = fMet->At(0);
  } else {
    cout << "Error: Met Collection " << fMetName << " could not be loaded.\n";
  }

  // get input Fakeable object collections
  const ElectronCol *ElectronFakeableObjects = 0;
  if (!fElectronFakeableObjectsName.IsNull())
    ElectronFakeableObjects = GetObjThisEvt<ElectronCol>(fElectronFakeableObjectsName);
  
  //Combine Barrel and Endcap superclusters into the same ObjArray
  ObjArray <SuperCluster> *SuperClusters = new ObjArray <SuperCluster>;
  for (UInt_t i=0; i<fBarrelSuperClusters->GetEntries(); i++) {
    SuperClusters->Add(fBarrelSuperClusters->At(i));
  }
  for (UInt_t i=0; i<fEndcapSuperClusters->GetEntries(); i++) {
    SuperClusters->Add(fEndcapSuperClusters->At(i));
  }

  //Load Trigger Objects
  const TriggerObjectCol *triggerObjects = GetHLTObjects(fTriggerObjectsName);
  if (!triggerObjects && ( fSampleType == 10 || fSampleType == 20)) {
    cout << "Error: Could not load Trigger Object Collection with name " 
         << fTriggerObjectsName << endl;
  }

  bool printdebug = false;
  //save denomiantors for debugging
  ElectronOArr *denominators = new ElectronOArr;

  //***********************************************************************************************
  //Make sample specific cuts
  //***********************************************************************************************
  //For WJets sample: Remove FSR W+gamma Events.
  if (fSampleType == 1 || fSampleType == 3 || fSampleType == 4 || fSampleType == 5) {
    Bool_t FoundWGammaEvent = false;
    
    for (UInt_t i=0; i<GenPhotons->GetEntries(); i++) {  
      if (GenPhotons->At(i)->Pt() > 10.0) {
        
        //ISR Photon
        if (GenPhotons->At(i)->Mother() && GenPhotons->At(i)->Mother()->IsParton()
            || (GenPhotons->At(i)->Mother()->AbsPdgId() == 22 
                && GenPhotons->At(i)->Mother()->Status() ==3  
                && GenPhotons->At(i)->Mother()->Mother() 
                && GenPhotons->At(i)->Mother()->Mother()->IsParton())
          ) {
          FoundWGammaEvent = true;    
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
          FoundWGammaEvent = true;
        }
        
        //Pythia FSR
        if (GenPhotons->At(i)->Mother() && GenPhotons->At(i)->Mother()->Status() == 3
            && (GenPhotons->At(i)->Mother()->AbsPdgId() == 11 
                || GenPhotons->At(i)->Mother()->AbsPdgId() == 13
                || GenPhotons->At(i)->Mother()->AbsPdgId() == 15)
          ) {
          FoundWGammaEvent = true;
        }      
      }
    }    
    if (FoundWGammaEvent) {
      return;
    }
  }

  //For Photon trigger sample: Only accept events where a clean photon is found 
  //matching to trigger photon
  const Photon *triggerPhoton = 0;
  const TriggerObject *leadingTriggerPhoton = 0;
  if (fSampleType == 20) {
    Bool_t FoundCleanPhoton = false;
    for (UInt_t l=0; l<triggerObjects->GetEntries(); l++) {      
      if (triggerObjects->At(l)->TrigName() == string(fTriggerName.Data()) 
          && triggerObjects->At(l)->Type() == TriggerObject::TriggerPhoton                   
        ) {        
        if (leadingTriggerPhoton) {
          if (triggerObjects->At(l)->Pt() > leadingTriggerPhoton->Pt()) {
            leadingTriggerPhoton = triggerObjects->At(l);
          }
        } else {
          leadingTriggerPhoton = triggerObjects->At(l);
        }
      }
    }

    if (leadingTriggerPhoton) {
      for (UInt_t i=0; i<CleanPhotons->GetEntries(); i++) {  
        Double_t deltaR = MathUtils::DeltaR(CleanPhotons->At(i)->Phi(), CleanPhotons->At(i)->Eta(),
                                            leadingTriggerPhoton->Phi(), 
                                            leadingTriggerPhoton->Eta());
        
        if (CleanPhotons->At(i)->Pt() > 20.0 && deltaR < 0.1) {
          FoundCleanPhoton = true;
          triggerPhoton = CleanPhotons->At(i);
          break;
        }
      }
    }

    if (!FoundCleanPhoton) {
      return;
    } else {
      if (triggerPhoton) {
        fTriggerPhotonPt->Fill(triggerPhoton->Pt());
        fTriggerPhotonEta->Fill(triggerPhoton->Eta());
        fTriggerPhotonPhi->Fill(triggerPhoton->Phi());
      } 

      const Jet *leadingJet = 0;
      for (UInt_t jj=0; jj<GoodJets->GetEntries(); ++jj) {  
        Double_t DR = MathUtils::DeltaR(triggerPhoton->Phi(), triggerPhoton->Eta(),
                                        GoodJets->At(jj)->Phi(), 
                                        GoodJets->At(jj)->Eta());
        //clean the jets of the trigger photon
        if (DR > 0.3 && GoodJets->At(jj)->Pt() > 20.0) {
          if (!leadingJet) {
            leadingJet = GoodJets->At(jj);
          } else if (GoodJets->At(jj)->Pt() > leadingJet->Pt()) {
            leadingJet = GoodJets->At(jj);
          }
        }
      }

      if (leadingJet) {
        Double_t dPhi_triggerPhotonToLeadingJet = 
          MathUtils::DeltaPhi(triggerPhoton->Phi(), leadingJet->Phi());
        fDeltaPhiTriggerPhotonToLeadingJet->Fill(dPhi_triggerPhotonToLeadingJet);
        if (dPhi_triggerPhotonToLeadingJet <= 2.94) {
          return;
        }
      }
    }
  
    //************************************************************
    //Make Distributions for removing real electron background. 
    //Zee (use lepton-jet mass) , Wenu (use Met)
    //************************************************************
    Int_t NLeptons = CleanLeptons->GetEntries();
    fNCleanLeptons->Fill(NLeptons);    
    fMetHist->Fill(caloMet->Pt());
   
    //Remove events not satisfying these cuts.
    //remove 2 lepton events, remove large Met events, remove events inside Z mass window
    //Note: If we don't subtract Z events correctly, this will bias the fake rate to low values
    //      because numerators are vetoed but denominators are still counted for Z events.
    if (!(NLeptons <= 1 )) {
      return;
    }
  }
               
  //For Jet trigger sample
  const Jet *triggerJet = 0;
  const TriggerObject *leadingTriggerJet = 0;
  if (fSampleType == 10) {
    Bool_t FoundCleanJet = false;    
    
    for (UInt_t l=0; l<triggerObjects->GetEntries(); l++) {      
      if (triggerObjects->At(l)->TrigName() == string(fTriggerName.Data()) 
          && triggerObjects->At(l)->Type() == TriggerObject::TriggerJet                  
        ) {        
        if (leadingTriggerJet) {
          if (triggerObjects->At(l)->Pt() > leadingTriggerJet->Pt()) {
            leadingTriggerJet = triggerObjects->At(l);
          }
        } else {
          leadingTriggerJet = triggerObjects->At(l);
        }
      }
    }

    if (leadingTriggerJet) {
      for (UInt_t i=0; i<CleanJets->GetEntries(); i++) {  
        Double_t deltaR = MathUtils::DeltaR(CleanJets->At(i)->Phi(), CleanJets->At(i)->Eta(),
                                            leadingTriggerJet->Phi(), 
                                            leadingTriggerJet->Eta());        
        if (deltaR < 0.3 ) {
          FoundCleanJet = true;
          triggerJet = CleanJets->At(i);
          break;
        }
      }
    }
   
    if (!FoundCleanJet) {
      return;
    } else {
      if (triggerJet) {
        fTriggerJetPt->Fill(triggerJet->Pt());
        fTriggerJetEta->Fill(triggerJet->Eta());
        fTriggerJetPhi->Fill(triggerJet->Phi());
      }
    }
        
    //************************************************************
    //Make Distributions for removing real electron background. 
    //Zee (use lepton-jet mass) , Wenu (use Met)
    //************************************************************
    Int_t NLeptons = CleanLeptons->GetEntries();
    fNCleanLeptons->Fill(NLeptons);
    fMetHist->Fill(caloMet->Pt());
   
    //Remove events not satisfying these cuts.
    //remove 2 lepton events, remove large Met events, remove events inside Z mass window
    //Note: If we don't subtract Z events correctly, this will bias the fake rate to low values
    //      because numerators are vetoed but denominators are still counted for Z events.
    if (!(NLeptons <= 1 )) {
      return;
    }
  }

  //***********************************************************************************************
  //Find Denominator Objects for Electrons
  //Supercluster matched to nearest isolated track.
  //***********************************************************************************************
  //build a new electron array to find all denominator candidates
  for (UInt_t i=0; i<ElectronFakeableObjects->GetEntries(); ++i) {
    const Electron* ele = ElectronFakeableObjects->At(i);
   
    cout << "denominator " << ele->Pt() << " " << ele->Eta() << " " << ele->Phi() << endl;

    //Veto numerators matching to real electrons
    Bool_t IsMCLepton = false;
    for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
      if (MathUtils::DeltaR(ele->Phi(), ele->Eta(),
                            GenLeptonsAndTaus->At(l)->Phi(),
                            GenLeptonsAndTaus->At(l)->Eta()) < 0.1) {
        IsMCLepton = true;
      }
    }
   
    bool passesSampleDependantRequirements = true;
    bool passesRealLeptonRemovalRequirements = true;
    //For QCD sample
    if (fSampleType == 10) {
      //Veto on Trigger Jet
      Bool_t IsTriggerJet = false;
      if (fVetoTriggerJet && leadingTriggerJet) {
        Double_t deltaR = MathUtils::DeltaR(ele->Phi(), ele->Eta(),
                                            leadingTriggerJet->Phi(), 
                                            leadingTriggerJet->Eta());                
        if (deltaR < 0.3) {
          IsTriggerJet = true;
        }
      } 

      if (IsTriggerJet) {
        passesSampleDependantRequirements = false;      
      }
      
      //veto Z-mass, met cut
      if (triggerJet) {
        Double_t tempZMass = 0;
        CompositeParticle *tempParticle = new CompositeParticle;
        tempParticle->AddDaughter(triggerJet);
        tempParticle->AddDaughter(ele);
        tempZMass = tempParticle->Mass();
        delete tempParticle;
        fDenominatorJetMass->Fill(tempZMass);
        if (tempZMass >= 100 && tempZMass <= 130) {
          passesRealLeptonRemovalRequirements = false;      
        }
        if (caloMet->Pt() > 30) {
          passesRealLeptonRemovalRequirements = false;      
        }
      }  
    }

    //For Photon trigger sample: Veto on photon
    if (fSampleType == 20) {
      Bool_t IsPhoton = false;
      if (leadingTriggerPhoton) {
        Double_t deltaR = MathUtils::DeltaR(ele->Phi(), ele->Eta(),
                                            leadingTriggerPhoton->Phi(), 
                                            leadingTriggerPhoton->Eta());
        if ( leadingTriggerPhoton->Pt() > 20.0 && deltaR < 0.3 ) {
          IsPhoton = true;
        }
      }
      if (IsPhoton) {
        passesSampleDependantRequirements = false;  
      }

      //veto Z-candidates
      if (triggerPhoton) {
        Double_t tempZMass = 0;
        CompositeParticle *tempParticle = new CompositeParticle;
        tempParticle->AddDaughter(triggerPhoton);
        tempParticle->AddDaughter(ele);
        tempZMass = tempParticle->Mass();
        delete tempParticle;
        fDenominatorPhotonMass->Fill(tempZMass);
        if (tempZMass >= 75 && tempZMass <= 105) {
          passesRealLeptonRemovalRequirements = false;      
        }
        if (caloMet->Pt() > 30) {
          passesRealLeptonRemovalRequirements = false;      
        }
      }
    }

    //impose denominator cuts
    if (passesSampleDependantRequirements && ele->Pt() > 10.0
        && (!(IsMCLepton && fVetoGeneratorLeptons))
      ) {      

      //Categorize Fake Electron
      Int_t printDebugLevel  = 0;    
      Bool_t isGsfTrack = true;
      const MCParticle *matchedSimParticle = GeneratorTools::MatchElectronToSimParticle(
        fParticles, ele->GsfTrk(), isGsfTrack, printDebugLevel, false);
      Int_t FakeCategory = GeneratorTools::CategorizeFakeElectron(matchedSimParticle);
      fElectronDenominatorFakeCategory->Fill(FakeCategory);

      //Find leading fragmentation ancestor particle
      const MCParticle* ancestor = GeneratorTools::FindElectronFakeAncestor(matchedSimParticle);
      Double_t ancestorFragmentationX = 0;

      //Find Matching GenJet and find its flavor
      Double_t Flavor = -9;
      Double_t minDR = 5000.0;
      const GenJet *matchedGenJet = 0;
      for (UInt_t j=0; j<fGenJets->GetEntries(); j++) {        
        Double_t dr = MathUtils::DeltaR(ele->Phi(), ele->Eta(),
                                        fGenJets->At(j)->Phi(), fGenJets->At(j)->Eta());
        if ( dr < minDR ) {
          minDR = dr;
          matchedGenJet = fGenJets->At(j);
        }
      }
      if (minDR < 0.3) {
        Flavor = matchedGenJet->MatchedMCFlavor();
        if (ancestor) {
          ancestorFragmentationX = ancestor->Pt() / matchedGenJet->Pt();
        }
      }
      fElectronDenominatorJetFlavor->Fill(Flavor);
      fElectronDenominatorFragmentationX->Fill(ancestorFragmentationX);

      //Fill Histograms
      fElectronDenominatorPt->Fill(ele->Pt());
      fElectronDenominatorEta->Fill(ele->Eta());
      fElectronDenominatorPhi->Fill(ele->Phi());
      fElectronDenominatorPtEta->Fill(ele->Pt(), ele->Eta());
//       fElectronDenominatorPtEtaPhi->Fill(ele->Pt(),ele->Eta(), ele->Phi());           
      
      //Fill Histograms With Real Lepton Removal Cuts
      if (passesRealLeptonRemovalRequirements) {
        fElectronDenominatorFakeCategory_WithCuts->Fill(FakeCategory);
        fElectronDenominatorJetFlavor_WithCuts->Fill(Flavor);
        fElectronDenominatorFragmentationX_WithCuts->Fill(ancestorFragmentationX);
        fElectronDenominatorPt_WithCuts->Fill(ele->Pt());
        fElectronDenominatorEta_WithCuts->Fill(ele->Eta());
        fElectronDenominatorPhi_WithCuts->Fill(ele->Phi());
        fElectronDenominatorPtEta_WithCuts->Fill(ele->Pt(), ele->Eta());
//         fElectronDenominatorPtEtaPhi_WithCuts->Fill(ele->Pt(),ele->Eta(), ele->Phi());           
      }
      
      //Fill Histograms in different categories
      if (FakeCategory == 0) {
        fElectronDenominatorPt_NoMatch->Fill(ele->Pt());
        fElectronDenominatorEta_NoMatch->Fill(ele->Eta());
        fElectronDenominatorPhi_NoMatch->Fill(ele->Phi());
        fElectronDenominatorPtEta_NoMatch->Fill(ele->Pt(), ele->Eta());
      } else if (FakeCategory == 1) {
        fElectronDenominatorPt_Conversion->Fill(ele->Pt());
        fElectronDenominatorEta_Conversion->Fill(ele->Eta());
        fElectronDenominatorPhi_Conversion->Fill(ele->Phi());
        fElectronDenominatorPtEta_Conversion->Fill(ele->Pt(), ele->Eta());
        fElectronDenominatorJetFlavor_Conversion->Fill(Flavor);
        fElectronDenominatorFragmentationX_Conversion->Fill(ancestorFragmentationX);
      } else if (FakeCategory == 2) {
        fElectronDenominatorPt_ChargedPion->Fill(ele->Pt());
        fElectronDenominatorEta_ChargedPion->Fill(ele->Eta());
        fElectronDenominatorPhi_ChargedPion->Fill(ele->Phi());
        fElectronDenominatorPtEta_ChargedPion->Fill(ele->Pt(), ele->Eta());
        fElectronDenominatorJetFlavor_ChargedPion->Fill(Flavor);
        fElectronDenominatorFragmentationX_ChargedPion->Fill(ancestorFragmentationX);
      } else if (FakeCategory == 3) {
        fElectronDenominatorPt_ChargedKaon->Fill(ele->Pt());
        fElectronDenominatorEta_ChargedKaon->Fill(ele->Eta());
        fElectronDenominatorPhi_ChargedKaon->Fill(ele->Phi());
        fElectronDenominatorPtEta_ChargedKaon->Fill(ele->Pt(), ele->Eta());
        fElectronDenominatorJetFlavor_ChargedKaon->Fill(Flavor);
        fElectronDenominatorFragmentationX_ChargedKaon->Fill(ancestorFragmentationX);
      } else if (FakeCategory == 4) {
        fElectronDenominatorPt_HeavyFlavor->Fill(ele->Pt());
        fElectronDenominatorEta_HeavyFlavor->Fill(ele->Eta());
        fElectronDenominatorPhi_HeavyFlavor->Fill(ele->Phi());
        fElectronDenominatorPtEta_HeavyFlavor->Fill(ele->Pt(), ele->Eta());
        fElectronDenominatorJetFlavor_HeavyFlavor->Fill(Flavor);
        fElectronDenominatorFragmentationX_HeavyFlavor->Fill(ancestorFragmentationX);
      } else if (FakeCategory == 5) {
        fElectronDenominatorPt_Brem->Fill(ele->Pt());
        fElectronDenominatorEta_Brem->Fill(ele->Eta());
        fElectronDenominatorPhi_Brem->Fill(ele->Phi());
        fElectronDenominatorPtEta_Brem->Fill(ele->Pt(), ele->Eta());
      } else if (FakeCategory == 6) {
        fElectronDenominatorPt_PromptPhoton->Fill(ele->Pt());
        fElectronDenominatorEta_PromptPhoton->Fill(ele->Eta());
        fElectronDenominatorPhi_PromptPhoton->Fill(ele->Phi());
        fElectronDenominatorPtEta_PromptPhoton->Fill(ele->Pt(), ele->Eta());
      } else if (FakeCategory == 7) {
        fElectronDenominatorPt_Other->Fill(ele->Pt());
        fElectronDenominatorEta_Other->Fill(ele->Eta());
        fElectronDenominatorPhi_Other->Fill(ele->Phi());
        fElectronDenominatorPtEta_Other->Fill(ele->Pt(), ele->Eta());
      }

      //add to denominators
      denominators->Add(ele);
      
      //for debug
      if (CleanLeptons->GetEntries() == 1) {
        fPredictionElectronDenominatorPtEta->Fill(ele->Pt(), ele->Eta());              
      }
            
      //Let's find the charge of the W.
      if (GenLeptons->GetEntries() == 1) {
        if (GenLeptons->At(0)->Charge() == 1) {
          if (ele->Charge() == 1) {
            fPlusElectronPlusWDenominatorPt->Fill(ele->Pt());
            fPlusElectronPlusWDenominatorEta->Fill(ele->Eta());
            fPlusElectronPlusWDenominatorPhi->Fill(ele->Phi());
          } else if (ele->Charge() == -1) {
            fMinusElectronPlusWDenominatorPt->Fill(ele->Pt());
            fMinusElectronPlusWDenominatorEta->Fill(ele->Eta());
            fMinusElectronPlusWDenominatorPhi->Fill(ele->Phi());
          }
        } else {
          if (ele->Charge() == 1) {
            fPlusElectronMinusWDenominatorPt->Fill(ele->Pt());
            fPlusElectronMinusWDenominatorEta->Fill(ele->Eta());
            fPlusElectronMinusWDenominatorPhi->Fill(ele->Phi());
          } else if (ele->Charge() == -1) {
            fMinusElectronMinusWDenominatorPt->Fill(ele->Pt());
            fMinusElectronMinusWDenominatorEta->Fill(ele->Eta());
            fMinusElectronMinusWDenominatorPhi->Fill(ele->Phi());
          }
        }
      }
      
      if (ele->Charge() == 1) {
        fPlusElectronDenominatorPt->Fill(ele->Pt());
        fPlusElectronDenominatorEta->Fill(ele->Eta());
        fPlusElectronDenominatorPhi->Fill(ele->Phi());
      } else if (ele->Charge() == -1) {
        fMinusElectronDenominatorPt->Fill(ele->Pt());
        fMinusElectronDenominatorEta->Fill(ele->Eta());
        fMinusElectronDenominatorPhi->Fill(ele->Phi());
      }
    }//passes denominator cuts
  }

  //***********************************************************************************************
  //Find Numerator Objects for Electrons
  //Numerators are basically Clean Electrons
  //***********************************************************************************************
  for (UInt_t i=0; i<CleanElectrons->GetEntries(); i++) {  
    const Electron* ele = CleanElectrons->At(i);

    //Veto numerators matching to real electrons      
    Bool_t IsMCLepton = false;
    for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
      if (MathUtils::DeltaR(CleanElectrons->At(i)->Phi(), CleanElectrons->At(i)->Eta(),
                            GenLeptonsAndTaus->At(l)->Phi(), 
                            GenLeptonsAndTaus->At(l)->Eta()) < 0.1) {
        IsMCLepton = true;
      }
    }
       
    bool passesSampleDependantRequirements = true;
    bool passesRealLeptonRemovalRequirements = true;
    //For QCD sample
    if (fSampleType == 10) {
      //Veto on Trigger Jet
      Bool_t IsTriggerJet = false;
      if (fVetoTriggerJet && leadingTriggerJet) {
        Double_t deltaR = MathUtils::DeltaR(ele->Phi(), ele->Eta(),
                                            leadingTriggerJet->Phi(), 
                                            leadingTriggerJet->Eta());                
        if (deltaR < 0.3) {
          IsTriggerJet = true;
        }
      } 
      if (IsTriggerJet) {
        passesSampleDependantRequirements = false;      
      }
      
      //veto Z-mass, met cut
      if (triggerJet) {
        Double_t tempZMass = 0;
        CompositeParticle *tempParticle = new CompositeParticle;
        tempParticle->AddDaughter(triggerJet);
        tempParticle->AddDaughter(ele);
        tempZMass = tempParticle->Mass();
        delete tempParticle;
        fDenominatorJetMass->Fill(tempZMass);
        if (tempZMass >= 100 && tempZMass <= 130) {
          passesRealLeptonRemovalRequirements = false;      
        }
        if (caloMet->Pt() > 30) {
          passesRealLeptonRemovalRequirements = false;      
        }
      }  
    }
         
    //For Photon trigger sample: Veto on photon
    if (fSampleType == 20) {
      Bool_t IsPhoton = false;
      if (leadingTriggerPhoton) {
        Double_t deltaR = MathUtils::DeltaR(ele->Phi(), ele->Eta(),
                                            leadingTriggerPhoton->Phi(), 
                                            leadingTriggerPhoton->Eta());
        if ( leadingTriggerPhoton->Pt() > 20.0 && deltaR < 0.3 ) {
          IsPhoton = true;
        }
      }
      if (IsPhoton) {
        passesSampleDependantRequirements = false;  
      }

      //veto Z-candidates
      if (triggerPhoton) {
        Double_t tempZMass = 0;
        CompositeParticle *tempParticle = new CompositeParticle;
        tempParticle->AddDaughter(triggerPhoton);
        tempParticle->AddDaughter(ele);
        tempZMass = tempParticle->Mass();
        delete tempParticle;
        fNumeratorPhotonMass->Fill(tempZMass);
        if (tempZMass >= 75 && tempZMass <= 105) {
          passesRealLeptonRemovalRequirements = false;      
        }
        if (caloMet->Pt() > 30) {
          passesRealLeptonRemovalRequirements = false;      
        }
      }
    }

    Double_t ElectronPUsingSCEnergy = 
      TMath::Sqrt(CleanElectrons->At(i)->SCluster()->Energy()*
                  CleanElectrons->At(i)->SCluster()->Energy() - 
                  CleanElectrons->At(i)->Mass()*
                  CleanElectrons->At(i)->Mass());
    Double_t ElectronPtUsingSCEnergy = 
      TMath::Abs(ElectronPUsingSCEnergy*
                 TMath::Cos(CleanElectrons->At(i)->Trk()->Lambda()));
    
    if (passesSampleDependantRequirements && ElectronPtUsingSCEnergy > 10.0
        && (!(IsMCLepton && fVetoGeneratorLeptons))
      ) {
       
      //Categorize Fake Electron
      Int_t printDebugLevel  = 0;    
      Bool_t isGsfTrack = true;
      const MCParticle *matchedSimParticle = 0;
      if (CleanElectrons->At(i)->TrackerTrk()) {
        isGsfTrack = false;
        matchedSimParticle = GeneratorTools::MatchElectronToSimParticle(
          fParticles, CleanElectrons->At(i)->TrackerTrk(), isGsfTrack, printDebugLevel, false);        
      } else {
        isGsfTrack = true;
        matchedSimParticle = GeneratorTools::MatchElectronToSimParticle(
          fParticles, CleanElectrons->At(i)->GsfTrk(), isGsfTrack, printDebugLevel, false);        
      }
      Int_t FakeCategory = GeneratorTools::CategorizeFakeElectron(matchedSimParticle);
      fElectronNumeratorFakeCategory->Fill(FakeCategory);
      
      //Find leading fragmentation ancestor particle
      const MCParticle* ancestor = GeneratorTools::FindElectronFakeAncestor(matchedSimParticle);
      Double_t ancestorFragmentationX = 0;

      //Find Matching GenJet and find its flavor
      Double_t Flavor = -9;
      Double_t minDR = 5000.0;
      const GenJet *matchedGenJet = 0;
      for (UInt_t j=0; j<fGenJets->GetEntries(); j++) {        
        Double_t dr = MathUtils::DeltaR(CleanElectrons->At(i)->Phi(), 
                                        CleanElectrons->At(i)->Eta(),
                                        fGenJets->At(j)->Phi(), fGenJets->At(j)->Eta());
        if ( dr < minDR ) {
          minDR = dr;
          matchedGenJet = fGenJets->At(j);
        }
      }
      if (minDR < 0.3) {
        Flavor = matchedGenJet->MatchedMCFlavor();
        if (ancestor) {
          ancestorFragmentationX = ancestor->Pt() / matchedGenJet->Pt();
        }
      }
      fElectronNumeratorJetFlavor->Fill(Flavor);
      fElectronNumeratorFragmentationX->Fill(ancestorFragmentationX);

      //Fill Histograms        
      //Use Momentum from SC Energy + Track Direction
      fElectronNumeratorPt->Fill(ElectronPtUsingSCEnergy);
      fElectronNumeratorEta->Fill(CleanElectrons->At(i)->Eta());
      fElectronNumeratorPhi->Fill(CleanElectrons->At(i)->Phi());
      fElectronNumeratorPtEta->Fill(ElectronPtUsingSCEnergy, CleanElectrons->At(i)->Eta());
//       fElectronNumeratorPtEtaPhi->Fill(ElectronPtUsingSCEnergy,CleanElectrons->At(i)->Eta(), 
//                                        CleanElectrons->At(i)->Phi());

      //Fill Histograms with real lepton removal cuts
      if (passesRealLeptonRemovalRequirements) {
        fElectronNumeratorFakeCategory_WithCuts->Fill(FakeCategory);
        fElectronNumeratorJetFlavor_WithCuts->Fill(Flavor);
        fElectronNumeratorFragmentationX_WithCuts->Fill(ancestorFragmentationX);        
        fElectronNumeratorPt_WithCuts->Fill(ElectronPtUsingSCEnergy);
        fElectronNumeratorEta_WithCuts->Fill(CleanElectrons->At(i)->Eta());
        fElectronNumeratorPhi_WithCuts->Fill(CleanElectrons->At(i)->Phi());
        fElectronNumeratorPtEta_WithCuts->Fill(ElectronPtUsingSCEnergy, CleanElectrons->At(i)->Eta());
//         fElectronNumeratorPtEtaPhi_WithCuts->Fill(ElectronPtUsingSCEnergy,CleanElectrons->At(i)->Eta(), 
//                                                   CleanElectrons->At(i)->Phi());
      }

      //Fill Histograms in different categories
      if (FakeCategory == 0) {
        fElectronNumeratorPt_NoMatch->Fill(ElectronPtUsingSCEnergy);
        fElectronNumeratorEta_NoMatch->Fill(CleanElectrons->At(i)->Eta());
        fElectronNumeratorPhi_NoMatch->Fill(CleanElectrons->At(i)->Phi());
        fElectronNumeratorPtEta_NoMatch->Fill(ElectronPtUsingSCEnergy, 
                                              CleanElectrons->At(i)->Eta());
      } else if (FakeCategory == 1) {
        fElectronNumeratorPt_Conversion->Fill(ElectronPtUsingSCEnergy);
        fElectronNumeratorEta_Conversion->Fill(CleanElectrons->At(i)->Eta());
        fElectronNumeratorPhi_Conversion->Fill(CleanElectrons->At(i)->Phi());
        fElectronNumeratorPtEta_Conversion->Fill(ElectronPtUsingSCEnergy, 
                                                 CleanElectrons->At(i)->Eta());
        fElectronNumeratorJetFlavor_Conversion->Fill(Flavor);
        fElectronNumeratorFragmentationX_Conversion->Fill(ancestorFragmentationX);
      } else if (FakeCategory == 2) {
        fElectronNumeratorPt_ChargedPion->Fill(ElectronPtUsingSCEnergy);
        fElectronNumeratorEta_ChargedPion->Fill(CleanElectrons->At(i)->Eta());
        fElectronNumeratorPhi_ChargedPion->Fill(CleanElectrons->At(i)->Phi());
        fElectronNumeratorPtEta_ChargedPion->Fill(ElectronPtUsingSCEnergy, 
                                                  CleanElectrons->At(i)->Eta());
        fElectronNumeratorJetFlavor_ChargedPion->Fill(Flavor);
        fElectronNumeratorFragmentationX_ChargedPion->Fill(ancestorFragmentationX);
      } else if (FakeCategory == 3) {
        fElectronNumeratorPt_ChargedKaon->Fill(ElectronPtUsingSCEnergy);
        fElectronNumeratorEta_ChargedKaon->Fill(CleanElectrons->At(i)->Eta());
        fElectronNumeratorPhi_ChargedKaon->Fill(CleanElectrons->At(i)->Phi());
        fElectronNumeratorPtEta_ChargedKaon->Fill(ElectronPtUsingSCEnergy, 
                                                  CleanElectrons->At(i)->Eta());
        fElectronNumeratorJetFlavor_ChargedKaon->Fill(Flavor);
        fElectronNumeratorFragmentationX_ChargedKaon->Fill(ancestorFragmentationX);
      } else if (FakeCategory == 4) {
        fElectronNumeratorPt_HeavyFlavor->Fill(ElectronPtUsingSCEnergy);
        fElectronNumeratorEta_HeavyFlavor->Fill(CleanElectrons->At(i)->Eta());
        fElectronNumeratorPhi_HeavyFlavor->Fill(CleanElectrons->At(i)->Phi());
        fElectronNumeratorPtEta_HeavyFlavor->Fill(ElectronPtUsingSCEnergy, 
                                                  CleanElectrons->At(i)->Eta());
        fElectronNumeratorJetFlavor_HeavyFlavor->Fill(Flavor);
        fElectronNumeratorFragmentationX_HeavyFlavor->Fill(ancestorFragmentationX);
      } else if (FakeCategory == 5) {
        fElectronNumeratorPt_Brem->Fill(ElectronPtUsingSCEnergy);
        fElectronNumeratorEta_Brem->Fill(CleanElectrons->At(i)->Eta());
        fElectronNumeratorPhi_Brem->Fill(CleanElectrons->At(i)->Phi());
        fElectronNumeratorPtEta_Brem->Fill(ElectronPtUsingSCEnergy, CleanElectrons->At(i)->Eta());
      } else if (FakeCategory == 6) {
        fElectronNumeratorPt_PromptPhoton->Fill(ElectronPtUsingSCEnergy);
        fElectronNumeratorEta_PromptPhoton->Fill(CleanElectrons->At(i)->Eta());
        fElectronNumeratorPhi_PromptPhoton->Fill(CleanElectrons->At(i)->Phi());
        fElectronNumeratorPtEta_PromptPhoton->Fill(ElectronPtUsingSCEnergy, 
                                                   CleanElectrons->At(i)->Eta());
      } else if (FakeCategory == 7) {
        fElectronNumeratorPt_Other->Fill(ElectronPtUsingSCEnergy);
        fElectronNumeratorEta_Other->Fill(CleanElectrons->At(i)->Eta());
        fElectronNumeratorPhi_Other->Fill(CleanElectrons->At(i)->Phi());
        fElectronNumeratorPtEta_Other->Fill(ElectronPtUsingSCEnergy, CleanElectrons->At(i)->Eta());
      }

      //for debug
      if (CleanLeptons->GetEntries() == 2) {
        fPredictionElectronNumeratorPtEta->Fill(ElectronPtUsingSCEnergy, 
                                                CleanElectrons->At(i)->Eta());
      }

      if (CleanElectrons->At(i)->Charge() == 1) {
        fPlusElectronNumeratorPt->Fill(ElectronPtUsingSCEnergy);
        fPlusElectronNumeratorEta->Fill(CleanElectrons->At(i)->Eta());
        fPlusElectronNumeratorPhi->Fill(CleanElectrons->At(i)->Phi());
      } else if (CleanElectrons->At(i)->Charge() == -1) {
        fMinusElectronNumeratorPt->Fill(ElectronPtUsingSCEnergy);
        fMinusElectronNumeratorEta->Fill(CleanElectrons->At(i)->Eta());
        fMinusElectronNumeratorPhi->Fill(CleanElectrons->At(i)->Phi());
      }
      
      //Let's find the charge of the W.
      if (GenLeptons->GetEntries() == 1) {
        if (GenLeptons->At(0)->Charge() == 1) {
          if (CleanElectrons->At(i)->Charge() == 1) {
            fPlusElectronPlusWNumeratorPt->Fill(ElectronPtUsingSCEnergy);
            fPlusElectronPlusWNumeratorEta->Fill(CleanElectrons->At(i)->Eta());
            fPlusElectronPlusWNumeratorPhi->Fill(CleanElectrons->At(i)->Phi());
          } else if (CleanElectrons->At(i)->Charge() == -1) {
            fMinusElectronPlusWNumeratorPt->Fill(ElectronPtUsingSCEnergy);
            fMinusElectronPlusWNumeratorEta->Fill(CleanElectrons->At(i)->Eta());
            fMinusElectronPlusWNumeratorPhi->Fill(CleanElectrons->At(i)->Phi());
          }
        } else {
          if (CleanElectrons->At(i)->Charge() == 1) {
            fPlusElectronMinusWNumeratorPt->Fill(ElectronPtUsingSCEnergy);
            fPlusElectronMinusWNumeratorEta->Fill(CleanElectrons->At(i)->Eta());
            fPlusElectronMinusWNumeratorPhi->Fill(CleanElectrons->At(i)->Phi());
          } else if (CleanElectrons->At(i)->Charge() == -1) {
            fMinusElectronMinusWNumeratorPt->Fill(ElectronPtUsingSCEnergy);
            fMinusElectronMinusWNumeratorEta->Fill(CleanElectrons->At(i)->Eta());
            fMinusElectronMinusWNumeratorPhi->Fill(CleanElectrons->At(i)->Phi());
          }
        }
      }      

      fSCTrackMatchDeltaR_Numerator->Fill(MathUtils::DeltaR(
                                            CleanElectrons->At(i)->SCluster()->Phi(),
                                            CleanElectrons->At(i)->SCluster()->Eta(),
                                            CleanElectrons->At(i)->Trk()->Phi(),
                                            CleanElectrons->At(i)->Trk()->Eta()));


      //Check if numerator is also in the denominator
      Bool_t isADenominator = false;
      for (UInt_t d=0; d<denominators->GetEntries(); d++) {
        if (MathUtils::DeltaR(denominators->At(d)->Phi(),denominators->At(d)->Eta(),
                             CleanElectrons->At(i)->Phi(),CleanElectrons->At(i)->Eta()) < 0.3)
          isADenominator = true;
      }


      //Check if a numerator is not in the denominator
      if (!isADenominator) {        
        cout << "\nNOT DENOMINATOR!!!";
        
        cout << "\nElectron " << i << " : " << ElectronPtUsingSCEnergy << " " 
             << CleanElectrons->At(i)->Eta() << " " << CleanElectrons->At(i)->Phi() << " " 
             << CleanElectrons->At(i)->GsfTrk() << endl; 
        if (CleanElectrons->At(i)->GsfTrk()) {
          cout << "Electron GsfTrack : " << CleanElectrons->At(i)->GsfTrk()->Pt() << " "
               << CleanElectrons->At(i)->GsfTrk()->Eta() << " " 
               << CleanElectrons->At(i)->GsfTrk()->Phi() << endl;
        }
        if (CleanElectrons->At(i)->SCluster()) {
          cout << "Electron SCluster : " << CleanElectrons->At(i)->SCluster()->Energy() << " "
               << CleanElectrons->At(i)->SCluster()->Eta() << " " 
               << CleanElectrons->At(i)->SCluster()->Phi() << endl;
        }
        //printdebug = true;
      }
    }//if passes numerator cuts
  }//for all clean electrons


  //***********************************************************************************************
  //Debug Print Out
  //***********************************************************************************************
  if (printdebug ) {
    
    cout << GetName() << endl
         << "LumiSec: " << GetEventHeader()->LumiSec() << " RunNum: " << GetEventHeader()->RunNum()
         << " EventNum: " << GetEventHeader()->EvtNum() << endl;


    cout << "***********************************************************************\n";
    cout << "TriggerObjects\n";
    cout << "***********************************************************************\n";
    for (UInt_t l=0; l<triggerObjects->GetEntries(); l++) {  
      cout << "TriggerObject " << l << triggerObjects->At(l)->TrigName() << " "
           << triggerObjects->At(l)->Type() << " " << triggerObjects->At(l)->Pt()
           << " " << triggerObjects->At(l)->Eta() << " "
           << triggerObjects->At(l)->Phi() << endl;
    }
    

    cout << "***********************************************************************\n";
    cout << "CleanPhotons\n";
    cout << "***********************************************************************\n";
    for (UInt_t i=0; i<CleanPhotons->GetEntries(); i++) {        
      cout << "\nCleanPhotons " << i << " : " << CleanPhotons->At(i)->Pt() << " " 
           << CleanPhotons->At(i)->Eta() << " " << CleanPhotons->At(i)->Phi() 
           << endl;     
    }
    
    cout << "***********************************************************************\n";
    cout << "Numerators\n";
    cout << "***********************************************************************\n";
    for (UInt_t i=0; i<CleanElectrons->GetEntries(); i++) {  
      
      Double_t ElectronPUsingSCEnergy = 
        TMath::Sqrt(CleanElectrons->At(i)->SCluster()->Energy()*
                    CleanElectrons->At(i)->SCluster()->Energy() - 
                    CleanElectrons->At(i)->Mass()*
                    CleanElectrons->At(i)->Mass());
      Double_t ElectronPtUsingSCEnergy = 
        TMath::Abs(ElectronPUsingSCEnergy*
                   TMath::Cos(CleanElectrons->At(i)->Trk()->Lambda()));
            
      cout << "\nElectron " << i << " : " << CleanElectrons->At(i)->Pt() << "/" 
           << ElectronPtUsingSCEnergy << " " 
           << CleanElectrons->At(i)->Eta() << " " << CleanElectrons->At(i)->Phi() << " " 
           << CleanElectrons->At(i)->GsfTrk() << endl; 
      if (CleanElectrons->At(i)->GsfTrk()) {
        cout << "Electron GsfTrack : " << CleanElectrons->At(i)->GsfTrk()->Pt() << " "
             << CleanElectrons->At(i)->GsfTrk()->Eta() << " " 
             << CleanElectrons->At(i)->GsfTrk()->Phi() << endl;
      }
      if (CleanElectrons->At(i)->SCluster()) {
        cout << "Electron SCluster : " << CleanElectrons->At(i)->SCluster()->Energy() << " "
             << CleanElectrons->At(i)->SCluster()->Eta() << " " 
             << CleanElectrons->At(i)->SCluster()->Phi() << endl;
      }
      cout << "Isolation : " << CleanElectrons->At(i)->TrackIsolation() << " "
           << CleanElectrons->At(i)->EcalJurassicIsolation() << " "
           << CleanElectrons->At(i)->HcalIsolation() << " "
           << endl;
      double iso = IsolationTools::TrackIsolation(CleanElectrons->At(i)->Trk(), 
                                                  0.3, 0.015, 1.0, 0.2,
                                                  fTracks);
      cout << iso << endl;
    }
    cout << "***********************************************************************\n";
    cout << "Denominators\n";
    cout << "***********************************************************************\n";
    for (UInt_t d=0; d<ElectronFakeableObjects->GetEntries(); d++) {
      cout << "ElectronFakeableObjects  " << d << " : " << ElectronFakeableObjects->At(d)->Pt() 
           << " " <<  ElectronFakeableObjects->At(d)->Eta()  << " " 
           <<  ElectronFakeableObjects->At(d)->Phi() << " ";
      
      if (ElectronFakeableObjects->At(d)->GsfTrk()) {
        cout << "   GsfTrack: " << ElectronFakeableObjects->At(d)->GsfTrk()->Pt() << " "
             << ElectronFakeableObjects->At(d)->GsfTrk()->Eta() << " " 
             << ElectronFakeableObjects->At(d)->GsfTrk()->Phi() << " ";
      }
      if (ElectronFakeableObjects->At(d)->SCluster()) {
        
        cout <<  " --- SCluster: ";          
        cout << ElectronFakeableObjects->At(d)->SCluster()->Energy() << " "
             << ElectronFakeableObjects->At(d)->SCluster()->Eta() << " "
             << ElectronFakeableObjects->At(d)->SCluster()->Phi();
      }
      cout << endl;
    }


    for (UInt_t d=0; d<denominators->GetEntries(); d++) {
      cout << "Denominator GsfTrack " << d << " : " << denominators->At(d)->Pt() 
           << " " <<  denominators->At(d)->Eta()  << " " 
           <<  denominators->At(d)->Phi() << " ";
      
      if (denominators->At(d)->GsfTrk()) {
        cout << "   GsfTrack: " << denominators->At(d)->GsfTrk()->Pt() << " "
             << denominators->At(d)->GsfTrk()->Eta() << " " 
             << denominators->At(d)->GsfTrk()->Phi() << " ";
      }
      if (denominators->At(d)->SCluster()) {
        
        cout <<  " --- SCluster: ";          
        cout << denominators->At(d)->SCluster()->Energy() << " "
             << denominators->At(d)->SCluster()->Eta() << " "
             << denominators->At(d)->SCluster()->Phi();
      }
      cout << endl;
    }
    cout << "***********************************************************************\n";
    for (UInt_t d=0; d<SuperClusters->GetEntries(); d++) {
      cout << "Supercluster " << d << " : " 
           << SuperClusters->At(d)->Energy() << " " 
           << SuperClusters->At(d)->Eta() << " " << SuperClusters->At(d)->Phi()
           << endl;
    }
    for (UInt_t d=0; d<fGsfTracks->GetEntries(); d++) {
      cout << "GsfTracks " << d << " : " 
           << fGsfTracks->At(d)->Pt() << " " 
           << fGsfTracks->At(d)->Eta() << " " << fGsfTracks->At(d)->Phi() 
           <<  " --- SCluster: ";
      if (fGsfTracks->At(d)->SCluster())
        cout << fGsfTracks->At(d)->SCluster()->Energy() << " "
             << fGsfTracks->At(d)->SCluster()->Eta() << " "
             << fGsfTracks->At(d)->SCluster()->Phi();             
      cout << endl;
    }    
    for (UInt_t l=0; l<CleanJets->GetEntries(); l++) {
      cout << "CleanJets " << l << " : " 
           << CleanJets->At(l)->Pt() << " " 
           << CleanJets->At(l)->Eta() << " " 
           << CleanJets->At(l)->Phi() << " " 
           << endl;
    }

    for (UInt_t d=0; d<GenLeptonsAndTaus->GetEntries(); d++) {
      cout << "GenLeptonsAndTaus: " << d << " : " << GenLeptonsAndTaus->At(d)->Status() << " " 
           << GenLeptonsAndTaus->At(d)->PdgId() << " " 
           <<GenLeptonsAndTaus->At(d)->Pt() 
           << " " <<  GenLeptonsAndTaus->At(d)->Eta()  << " " 
           <<  GenLeptonsAndTaus->At(d)->Phi();
      cout << endl;
    }        
    for (UInt_t d=0; d<fGenJets->GetEntries(); d++) {        
      cout << "fGenJets " << d << " : " << fGenJets->At(d)->Pt() << " " 
           << fGenJets->At(d)->Eta() << " " << fGenJets->At(d)->Phi()  
           << " flavor" << fGenJets->At(d)->MatchedMCFlavor() << endl;
    }
    GeneratorTools::PrintHepMCTable(fParticles, true, -1);      
  }

  //delete temp collections
  delete denominators;
  delete GenLeptonsAndTaus;
  delete SuperClusters;
}

//--------------------------------------------------------------------------------------------------
void ComputeElectronFakeRateMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.

}

//--------------------------------------------------------------------------------------------------
void ComputeElectronFakeRateMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
