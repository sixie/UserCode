// $Id: ComputeMuonFakeRateMod.cc,v 1.7 2009/06/30 08:58:50 phedex Exp $

#include "MitHiggs/FakeRateMods/interface/ComputeMuonFakeRateMod.h"
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

ClassImp(mithep::ComputeMuonFakeRateMod)

//--------------------------------------------------------------------------------------------------
  ComputeMuonFakeRateMod::ComputeMuonFakeRateMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fVetoTriggerJet(true),
  fVetoGeneratorLeptons(false),
  fSampleType(0),
  fMCPartBranchName(Names::gkMCPartBrn),
  fGenJetBranchName(Names::gkSC5GenJetBrn),
  fTriggerName("NotSpecified"),
  fTriggerObjectsName("NotSpecified"),
  fTrackBranchName(Names::gkTrackBrn),
  fMuonBranchName(Names::gkMuonBrn),
  fMetName("NotSet"),
  fCleanMuonsName(ModNames::gkCleanMuonsName),
  fGoodJetsName(ModNames::gkGoodJetsName),
  fCleanJetsName(ModNames::gkCleanJetsName),
  fMCLeptonsName(ModNames::gkMCLeptonsName),
  fMCTausName(ModNames::gkMCTausName),
  fMuonFakeableObjectsName(ModNames::gkMuonFakeableObjectsName),
  fParticles(0),
  fGenJets(0),
  fTracks(0),
  fMuons(0)
{ 
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void ComputeMuonFakeRateMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void ComputeMuonFakeRateMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fMCPartBranchName,                fParticles);
  ReqBranch(fGenJetBranchName,                fGenJets);
  ReqBranch(fTrackBranchName,                 fTracks);
  ReqBranch(fMuonBranchName,                  fMuons);

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
  AddTH1(fMuonDenominatorPt, "hMuonDenominatorPt", 
         ";MuonDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonDenominatorEta, "hMuonDenominatorEta", 
         ";MuonDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonDenominatorPhi, "hMuonDenominatorPhi", 
         ";MuonDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonDenominatorPtEta, "hMuonDenominatorPtEta", 
         ";MuonDenominatorPt;MuonDenominatorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fPlusMuonDenominatorPt, "hPlusMuonDenominatorPt", 
         ";PlusMuonDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fPlusMuonDenominatorEta, "hPlusMuonDenominatorEta", 
         ";PlusMuonDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fPlusMuonDenominatorPhi, "hPlusMuonDenominatorPhi", 
         ";PlusMuonDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fMinusMuonDenominatorPt, "hMinusMuonDenominatorPt", 
         ";MinusMuonDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMinusMuonDenominatorEta, "hMinusMuonDenominatorEta", 
         ";MinusMuonDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMinusMuonDenominatorPhi, "hMinusMuonDenominatorPhi", 
         ";MinusMuonDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fPlusMuonPlusWDenominatorPt, "hPlusMuonPlusWDenominatorPt", 
         ";PlusMuonPlusWDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fPlusMuonPlusWDenominatorEta, "hPlusPlusWMuonDenominatorEta", 
         ";PlusMuonPlusWDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fPlusMuonPlusWDenominatorPhi, "hPlusMuonPlusWDenominatorPhi", 
         ";PlusMuonPlusWDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fMinusMuonPlusWDenominatorPt, "hMinusMuonPlusWDenominatorPt", 
         ";MinusMuonPlusWDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMinusMuonPlusWDenominatorEta, "hMinusMuonPlusWDenominatorEta", 
         ";MinusMuonPlusWDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMinusMuonPlusWDenominatorPhi, "hMinusMuonPlusWDenominatorPhi", 
         ";MinusMuonPlusWDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fPlusMuonMinusWDenominatorPt, "hPlusMuonMinusWDenominatorPt", 
         ";PlusMuonMinusWDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fPlusMuonMinusWDenominatorEta, "hPlusMinusWMuonDenominatorEta", 
         ";PlusMuonMinusWDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fPlusMuonMinusWDenominatorPhi, "hPlusMuonMinusWDenominatorPhi", 
         ";PlusMuonMinusWDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fMinusMuonMinusWDenominatorPt, "hMinusMuonMinusWDenominatorPt", 
         ";MinusMuonMinusWDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMinusMuonMinusWDenominatorEta, "hMinusMuonMinusWDenominatorEta", 
         ";MinusMuonMinusWDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMinusMuonMinusWDenominatorPhi, "hMinusMuonMinusWDenominatorPhi", 
         ";MinusMuonMinusWDenominatorPhi;Number of Events", 100, -3.2, 3.2);

  AddTH1(fMuonDenominatorFakeCategory, "hMuonDenominatorFakeCategory", 
         ";Muon Fake Category;Number of Events", 8,-0.5,7.5);
  TAxis *xa = fMuonDenominatorFakeCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"PionDecayInFlight");
  xa->SetBinLabel(3,"KaonDecayInFlight");
  xa->SetBinLabel(4,"MuonFromOtherProcess");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"PionPunchThru");
  xa->SetBinLabel(7,"OtherNonMuon");
  xa->SetBinLabel(8,"RealIsolatedMuon");

  AddTH1(fMuonDenominatorJetFlavor, "hMuonDenominatorJetFlavor", 
         ";Jet Flavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fMuonDenominatorFragmentationX, "hMuonDenominatorFragmentationX", 
         ";AncestorFragmentaionX;Number of Events", 100, 0,2);


  //*******************************************************************************************
  //Numerator Histograms
  //*******************************************************************************************

  AddTH1(fMuonNumeratorPt, "hMuonNumeratorPt", 
         ";MuonNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonNumeratorEta, "hMuonNumeratorEta", 
         ";MuonNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonNumeratorPhi, "hMuonNumeratorPhi", 
         ";MuonNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonNumeratorPtEta, "hMuonNumeratorPtEta", 
         ";MuonNumeratorPt;MuonNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fPlusMuonNumeratorPt, "hPlusMuonNumeratorPt", 
         ";PlusMuonNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fPlusMuonNumeratorEta, "hPlusMuonNumeratorEta", 
         ";PlusMuonNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fPlusMuonNumeratorPhi, "hPlusMuonNumeratorPhi", 
         ";PlusMuonNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fMinusMuonNumeratorPt, "hMinusMuonNumeratorPt", 
         ";MinusMuonNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fMinusMuonNumeratorEta, "hMinusMuonNumeratorEta", 
         ";MinusMuonNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMinusMuonNumeratorPhi, "hMinusMuonNumeratorPhi", 
         ";MinusMuonNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fPlusMuonPlusWNumeratorPt, "hPlusMuonPlusWNumeratorPt", 
         ";PlusMuonPlusWNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fPlusMuonPlusWNumeratorEta, "hPlusPlusWMuonNumeratorEta", 
         ";PlusMuonPlusWNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fPlusMuonPlusWNumeratorPhi, "hPlusMuonPlusWNumeratorPhi", 
         ";PlusMuonPlusWNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fMinusMuonPlusWNumeratorPt, "hMinusMuonPlusWNumeratorPt", 
         ";MinusMuonPlusWNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fMinusMuonPlusWNumeratorEta, "hMinusMuonPlusWNumeratorEta", 
         ";MinusMuonPlusWNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMinusMuonPlusWNumeratorPhi, "hMinusMuonPlusWNumeratorPhi", 
         ";MinusMuonPlusWNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fPlusMuonMinusWNumeratorPt, "hPlusMuonMinusWNumeratorPt", 
         ";PlusMuonMinusWNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fPlusMuonMinusWNumeratorEta, "hPlusMinusWMuonNumeratorEta", 
         ";PlusMuonMinusWNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fPlusMuonMinusWNumeratorPhi, "hPlusMuonMinusWNumeratorPhi", 
         ";PlusMuonMinusWNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fMinusMuonMinusWNumeratorPt, "hMinusMuonMinusWNumeratorPt", 
         ";MinusMuonMinusWNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fMinusMuonMinusWNumeratorEta, "hMinusMuonMinusWNumeratorEta", 
         ";MinusMuonMinusWNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMinusMuonMinusWNumeratorPhi, "hMinusMuonMinusWNumeratorPhi", 
         ";MinusMuonMinusWNumeratorPhi;Number of Events", 100, -3.2, 3.2);

  AddTH1(fMuonNumeratorFakeCategory, "hMuonNumeratorFakeCategory", 
         ";Muon Fake Category;Number of Events", 8,-0.5,7.5);
  xa = fMuonNumeratorFakeCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"PionDecayInFlight");
  xa->SetBinLabel(3,"KaonDecayInFlight");
  xa->SetBinLabel(4,"MuonFromOtherProcess");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"PionPunchThru");
  xa->SetBinLabel(7,"OtherNonMuon");
  xa->SetBinLabel(8,"RealIsolatedMuon");

  AddTH1(fMuonNumeratorJetFlavor, "hMuonNumeratorJetFlavor", 
         ";Jet Flavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fMuonNumeratorFragmentationX, "hMuonNumeratorFragmentationX", 
         ";AncestorFragmentationX;Number of Events", 100, 0,2);

  //*******************************************************************************************
  //Denominator For Different Fake Categories
  //*******************************************************************************************
  AddTH1(fMuonDenominatorPt_NoMatch, "hMuonDenominatorPt_NoMatch", 
         ";MuonDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonDenominatorEta_NoMatch, "hMuonDenominatorEta_NoMatch", 
         ";MuonDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonDenominatorPhi_NoMatch, "hMuonDenominatorPhi_NoMatch", 
         ";MuonDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonDenominatorPtEta_NoMatch, "hMuonDenominatorPtEta_NoMatch", 
         ";MuonDenominatorPt;MuonDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  AddTH1(fMuonDenominatorPt_ChargedPion, "hMuonDenominatorPt_ChargedPion", 
         ";MuonDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonDenominatorEta_ChargedPion, "hMuonDenominatorEta_ChargedPion", 
         ";MuonDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonDenominatorPhi_ChargedPion, "hMuonDenominatorPhi_ChargedPion", 
         ";MuonDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonDenominatorPtEta_ChargedPion, "hMuonDenominatorPtEta_ChargedPion", 
         ";MuonDenominatorPt;MuonDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  AddTH1(fMuonDenominatorPt_ChargedKaon, "hMuonDenominatorPt_ChargedKaon", 
         ";MuonDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonDenominatorEta_ChargedKaon, "hMuonDenominatorEta_ChargedKaon", 
         ";MuonDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonDenominatorPhi_ChargedKaon, "hMuonDenominatorPhi_ChargedKaon", 
         ";MuonDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonDenominatorPtEta_ChargedKaon, "hMuonDenominatorPtEta_ChargedKaon", 
         ";MuonDenominatorPt;MuonDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  AddTH1(fMuonDenominatorPt_MuonFromOtherProcess, "hMuonDenominatorPt_MuonFromOtherProcess", 
         ";MuonDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonDenominatorEta_MuonFromOtherProcess, "hMuonDenominatorEta_MuonFromOtherProcess", 
         ";MuonDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonDenominatorPhi_MuonFromOtherProcess, "hMuonDenominatorPhi_MuonFromOtherProcess", 
         ";MuonDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonDenominatorPtEta_MuonFromOtherProcess, "hMuonDenominatorPtEta_MuonFromOtherProcess", 
         ";MuonDenominatorPt;MuonDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  AddTH1(fMuonDenominatorPt_HeavyFlavor, "hMuonDenominatorPt_HeavyFlavor", 
         ";MuonDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonDenominatorEta_HeavyFlavor, "hMuonDenominatorEta_HeavyFlavor", 
         ";MuonDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonDenominatorPhi_HeavyFlavor, "hMuonDenominatorPhi_HeavyFlavor", 
         ";MuonDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonDenominatorPtEta_HeavyFlavor, "hMuonDenominatorPtEta_HeavyFlavor", 
         ";MuonDenominatorPt;MuonDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  AddTH1(fMuonDenominatorPt_PionPunchThru, "hMuonDenominatorPt_PionPunchThru", 
         ";MuonDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonDenominatorEta_PionPunchThru, "hMuonDenominatorEta_PionPunchThru", 
         ";MuonDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonDenominatorPhi_PionPunchThru, "hMuonDenominatorPhi_PionPunchThru", 
         ";MuonDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonDenominatorPtEta_PionPunchThru, "hMuonDenominatorPtEta_PionPunchThru", 
         ";MuonDenominatorPt;MuonDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  AddTH1(fMuonDenominatorPt_OtherNonMuon, "hMuonDenominatorPt_OtherNonMuon", 
         ";MuonDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonDenominatorEta_OtherNonMuon, "hMuonDenominatorEta_OtherNonMuon", 
         ";MuonDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonDenominatorPhi_OtherNonMuon, "hMuonDenominatorPhi_OtherNonMuon", 
         ";MuonDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonDenominatorPtEta_OtherNonMuon, "hMuonDenominatorPtEta_OtherNonMuon", 
         ";MuonDenominatorPt;MuonDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  AddTH1(fMuonDenominatorPt_NoAncestor, "hMuonDenominatorPt_NoAncestor", 
         ";MuonDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonDenominatorEta_NoAncestor, "hMuonDenominatorEta_NoAncestor", 
         ";MuonDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonDenominatorPhi_NoAncestor, "hMuonDenominatorPhi_NoAncestor", 
         ";MuonDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonDenominatorPtEta_NoAncestor, "hMuonDenominatorPtEta_NoAncestor", 
         ";MuonDenominatorPt;MuonDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  //*******************************************************************************************
  //Numerator For Different Fake Categories
  //*******************************************************************************************
  AddTH1(fMuonNumeratorPt_NoMatch, "hMuonNumeratorPt_NoMatch", 
         ";MuonNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonNumeratorEta_NoMatch, "hMuonNumeratorEta_NoMatch", 
         ";MuonNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonNumeratorPhi_NoMatch, "hMuonNumeratorPhi_NoMatch", 
         ";MuonNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonNumeratorPtEta_NoMatch, "hMuonNumeratorPtEta_NoMatch", 
         ";MuonNumeratorPt;MuonNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fMuonNumeratorPt_ChargedPion, "hMuonNumeratorPt_ChargedPion", 
         ";MuonNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonNumeratorEta_ChargedPion, "hMuonNumeratorEta_ChargedPion", 
         ";MuonNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonNumeratorPhi_ChargedPion, "hMuonNumeratorPhi_ChargedPion", 
         ";MuonNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonNumeratorPtEta_ChargedPion, "hMuonNumeratorPtEta_ChargedPion", 
         ";MuonNumeratorPt;MuonNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fMuonNumeratorPt_ChargedKaon, "hMuonNumeratorPt_ChargedKaon", 
         ";MuonNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonNumeratorEta_ChargedKaon, "hMuonNumeratorEta_ChargedKaon", 
         ";MuonNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonNumeratorPhi_ChargedKaon, "hMuonNumeratorPhi_ChargedKaon", 
         ";MuonNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonNumeratorPtEta_ChargedKaon, "hMuonNumeratorPtEta_ChargedKaon", 
         ";MuonNumeratorPt;MuonNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fMuonNumeratorPt_MuonFromOtherProcess, "hMuonNumeratorPt_MuonFromOtherProcess", 
         ";MuonNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonNumeratorEta_MuonFromOtherProcess, "hMuonNumeratorEta_MuonFromOtherProcess", 
         ";MuonNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonNumeratorPhi_MuonFromOtherProcess, "hMuonNumeratorPhi_MuonFromOtherProcess", 
         ";MuonNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonNumeratorPtEta_MuonFromOtherProcess, "hMuonNumeratorPtEta_MuonFromOtherProcess", 
         ";MuonNumeratorPt;MuonNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fMuonNumeratorPt_HeavyFlavor, "hMuonNumeratorPt_HeavyFlavor", 
         ";MuonNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonNumeratorEta_HeavyFlavor, "hMuonNumeratorEta_HeavyFlavor", 
         ";MuonNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonNumeratorPhi_HeavyFlavor, "hMuonNumeratorPhi_HeavyFlavor", 
         ";MuonNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonNumeratorPtEta_HeavyFlavor, "hMuonNumeratorPtEta_HeavyFlavor", 
         ";MuonNumeratorPt;MuonNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fMuonNumeratorPt_PionPunchThru, "hMuonNumeratorPt_PionPunchThru", 
         ";MuonNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonNumeratorEta_PionPunchThru, "hMuonNumeratorEta_PionPunchThru", 
         ";MuonNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonNumeratorPhi_PionPunchThru, "hMuonNumeratorPhi_PionPunchThru", 
         ";MuonNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonNumeratorPtEta_PionPunchThru, "hMuonNumeratorPtEta_PionPunchThru", 
         ";MuonNumeratorPt;MuonNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fMuonNumeratorPt_OtherNonMuon, "hMuonNumeratorPt_OtherNonMuon", 
         ";MuonNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonNumeratorEta_OtherNonMuon, "hMuonNumeratorEta_OtherNonMuon", 
         ";MuonNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonNumeratorPhi_OtherNonMuon, "hMuonNumeratorPhi_OtherNonMuon", 
         ";MuonNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonNumeratorPtEta_OtherNonMuon, "hMuonNumeratorPtEta_OtherNonMuon", 
         ";MuonNumeratorPt;MuonNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);

  AddTH1(fMuonNumeratorPt_NoAncestor, "hMuonNumeratorPt_NoAncestor", 
         ";MuonNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonNumeratorEta_NoAncestor, "hMuonNumeratorEta_NoAncestor", 
         ";MuonNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonNumeratorPhi_NoAncestor, "hMuonNumeratorPhi_NoAncestor", 
         ";MuonNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonNumeratorPtEta_NoAncestor, "hMuonNumeratorPtEta_NoAncestor", 
         ";MuonNumeratorPt;MuonNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);




  //*******************************************************************************************
  //Denominator Histograms With Cuts
  //*******************************************************************************************
  AddTH1(fMuonDenominatorPt_WithCuts, "hMuonDenominatorPt_WithCuts", 
         ";MuonDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonDenominatorEta_WithCuts, "hMuonDenominatorEta_WithCuts", 
         ";MuonDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonDenominatorPhi_WithCuts, "hMuonDenominatorPhi_WithCuts", 
         ";MuonDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonDenominatorPtEta_WithCuts, "hMuonDenominatorPtEta_WithCuts", 
         ";MuonDenominatorPt;MuonDenominatorEta;Number of Events_WithCuts", 50, 0, 250, 100,-5.0,5.0);
//   AddTH3(fMuonDenominatorPtEtaPhi_WithCuts, "hMuonDenominatorPtEtaPhi_WithCuts", 
//          ";MuonDenominatorPt;MuonDenominatorEta;MuonDenominatorPhi;Number of Events", 
//          50, 0, 250,100, -5.0, 5.0,100, -3.2, 3.2);


  AddTH1(fMuonDenominatorFakeCategory_WithCuts, "hMuonDenominatorFakeCategory_WithCuts", 
         ";Muon Fake Category;Number of Events", 8,-0.5,7.5);
  xa = fMuonDenominatorFakeCategory_WithCuts->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"PionDecayInFlight");
  xa->SetBinLabel(3,"KaonDecayInFlight");
  xa->SetBinLabel(4,"MuonFromOtherProcess");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"PionPunchThru");
  xa->SetBinLabel(7,"OtherNonMuon");
  xa->SetBinLabel(8,"RealIsolatedMuon");

  AddTH1(fMuonDenominatorJetFlavor_WithCuts, "hMuonDenominatorJetFlavor_WithCuts", 
         ";Jet Flavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fMuonDenominatorFragmentationX_WithCuts, "hMuonDenominatorFragmentationX_WithCuts", 
         ";AncestorFragmentaionX;Number of Events", 100, 0,2);


  //*******************************************************************************************
  //Numerator Histograms With Cuts
  //*******************************************************************************************

  AddTH1(fMuonNumeratorPt_WithCuts, "hMuonNumeratorPt_WithCuts", 
         ";MuonNumeratorEt;Number of Events", 50, 0, 250);
  AddTH1(fMuonNumeratorEta_WithCuts, "hMuonNumeratorEta_WithCuts", 
         ";MuonNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonNumeratorPhi_WithCuts, "hMuonNumeratorPhi_WithCuts", 
         ";MuonNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fMuonNumeratorPtEta_WithCuts, "hMuonNumeratorPtEta_WithCuts", 
         ";MuonNumeratorPt;MuonNumeratorEta;Number of Events", 50, 0, 250, 100,-5.0,5.0);
//   AddTH3(fMuonNumeratorPtEtaPhi_WithCuts, "hMuonNumeratorPtEtaPhi_WithCuts", 
//          ";MuonNumeratorPt;MuonNumeratorEta;MuonNumeratorPhi;Number of Events", 
//          50, 0, 250,100, -5.0, 5.0,100, -3.2, 3.2);


  AddTH1(fMuonNumeratorFakeCategory_WithCuts, "hMuonNumeratorFakeCategory_WithCuts", 
         ";Muon Fake Category;Number of Events", 8,-0.5,7.5);
  xa = fMuonNumeratorFakeCategory_WithCuts->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"PionDecayInFlight");
  xa->SetBinLabel(3,"KaonDecayInFlight");
  xa->SetBinLabel(4,"MuonFromOtherProcess");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"PionPunchThru");
  xa->SetBinLabel(7,"OtherNonMuon");
  xa->SetBinLabel(8,"RealIsolatedMuon");

  AddTH1(fMuonNumeratorJetFlavor_WithCuts, "hMuonNumeratorJetFlavor_WithCuts", 
         ";Jet Flavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fMuonNumeratorFragmentationX_WithCuts, "hMuonNumeratorFragmentationX_WithCuts", 
         ";AncestorFragmentationX;Number of Events", 100, 0,2);


}

//--------------------------------------------------------------------------------------------------
void ComputeMuonFakeRateMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and
  //output debug info or not   

  LoadBranch(fMCPartBranchName);
  LoadBranch(fGenJetBranchName);
  LoadBranch(fMuonBranchName);
  LoadBranch(fTrackBranchName);

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
  const MuonCol *CleanMuons = 0;
  if (!fCleanMuonsName.IsNull())
    CleanMuons = GetObjThisEvt<MuonCol>(fCleanMuonsName);
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
  const MuonCol *MuonFakeableObjects = 0;
  if (!fMuonFakeableObjectsName.IsNull())
    MuonFakeableObjects = GetObjThisEvt<MuonCol>(fMuonFakeableObjectsName);

  //Load Trigger Objects
  const TriggerObjectCol *triggerObjects = GetHLTObjects(fTriggerObjectsName);
  if (!triggerObjects && fVetoTriggerJet) {
    cout << "Error: Could not load Trigger Object Collection with name " << fTriggerObjectsName << endl;
  }

  bool printdebug = false;
  //save denomiantors for debugging
  MuonOArr *denominators = new MuonOArr;


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
    //Make Distributions for removing real muon background. 
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
    //Make Distributions for removing real muon background. 
    //Zee (use lepton-jet mass) , Wenu (use Met)
    //************************************************************
    Int_t NLeptons = CleanLeptons->GetEntries();
    fNCleanLeptons->Fill(NLeptons);
    fMetHist->Fill(caloMet->Pt());
   
    //Remove events not satisfying these cuts.
    //remove 2 lepton events
    //Note: If we don't subtract Z events correctly, this will bias the fake rate to low values
    //      because numerators are vetoed but denominators are still counted for Z events.
    if (!(NLeptons <= 1 )) {
      return;
    }
  }

  //***********************************************************************************************
  //Find Denominator Objects for Muons
  //Use Denominator Definition from GenerateFakeableObjects
  //***********************************************************************************************
  for (UInt_t i=0; i<MuonFakeableObjects->GetEntries(); ++i) {
    const Muon* mu = MuonFakeableObjects->At(i);
    const Track* muonTrack = mu->Trk();
    if (mu->TrackerTrk()) muonTrack = mu->TrackerTrk();
    //Double_t trackIsolation = IsolationTools::TrackIsolation(muonTrack , 0.3, 0.015, 1.0, 
    //                                                         1000.0, fTracks);

    //Veto denominators matching to real muons
    Bool_t IsMCLepton = false;
    for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
      if (MathUtils::DeltaR(mu->Phi(), mu->Eta(),
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
        Double_t deltaR = MathUtils::DeltaR(mu->Phi(), mu->Eta(),
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
        tempParticle->AddDaughter(mu);
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
      for (UInt_t l=0; l<CleanPhotons->GetEntries(); l++) {  
        Double_t deltaR = MathUtils::DeltaR(mu->Phi(), mu->Eta(),
                                            CleanPhotons->At(l)->Phi(), 
                                            CleanPhotons->At(l)->Eta());
        if ( CleanPhotons->At(l)->Pt() > 20.0 && deltaR < 0.3 ) {
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
        tempParticle->AddDaughter(mu);
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

    if (mu->Pt() > 10.0 && passesSampleDependantRequirements
        && !(fVetoGeneratorLeptons && IsMCLepton)
      ) {

      //Categorize Fake Muon
      Int_t printDebugLevel  = 0;    
      Bool_t isTrackerTrack = false;
      const MCParticle *matchedSimParticle = 0;
      if (mu->TrackerTrk()) { 
        isTrackerTrack = true;      
        matchedSimParticle = GeneratorTools::MatchMuonToSimParticle(
          fParticles, mu->TrackerTrk(), isTrackerTrack, printDebugLevel, false);
      } else {
        isTrackerTrack = false;      
        matchedSimParticle = GeneratorTools::MatchMuonToSimParticle(
          fParticles, mu->Trk(), isTrackerTrack, printDebugLevel, false);
      }

      Int_t FakeCategory = GeneratorTools::CategorizeFakeMuon(matchedSimParticle);      

      //Find Matching GenJet and find its flavor
      Double_t Flavor = -9;
      Double_t minDR = 5000.0;
      const GenJet *matchedGenJet = 0;
      for (UInt_t j=0; j<fGenJets->GetEntries(); j++) {        
        Double_t dr = MathUtils::DeltaR(mu->Phi(), mu->Eta(),
                                        fGenJets->At(j)->Phi(), fGenJets->At(j)->Eta());
        if ( dr < minDR ) {
          minDR = dr;
          matchedGenJet = fGenJets->At(j);
        }
      }
      if (minDR < 0.3) {
        Flavor = matchedGenJet->MatchedMCFlavor();
        // if (ancestor) {
//           ancestorFragmentationX = ancestor->Pt() / matchedGenJet->Pt();
//         }
      }


      if (FakeCategory == -10) {
        //printdebug = true;

        if (FakeCategory != 0) {
          cout << "Match : "  
               << matchedSimParticle->PdgId()  
               << " " << matchedSimParticle->Status() << " " << matchedSimParticle->Pt()
               << " " << matchedSimParticle->Eta() << " " << matchedSimParticle->Phi()
               << " produced at ";
          if (matchedSimParticle->Mother())
            cout << matchedSimParticle->Mother()->DecayVertex().Rho();
          else 
            cout << "0.0";
          cout << " decays at ";
          if (matchedSimParticle->NDaughters() > 0)
            cout << matchedSimParticle->DecayVertex().Rho();
          else
            cout << " Stable ";
          cout << endl;
        }
        
        cout << "Fake Muon : " << mu->Pt() << " " << mu->Eta() << " " << mu->Phi() 
             << "  FakeCategory = " << FakeCategory << endl;

        Int_t printDebugLevel  = 7;    
        Bool_t isTrackerTrack = false;
        const MCParticle *tmp = 0;
        if (mu->TrackerTrk()) { 
          isTrackerTrack = true;      
          tmp = GeneratorTools::MatchMuonToSimParticle(
            fParticles, mu->TrackerTrk(), isTrackerTrack, printDebugLevel, false);
        } else {
          isTrackerTrack = false;      
          tmp = GeneratorTools::MatchMuonToSimParticle(
            fParticles, mu->Trk(), isTrackerTrack, printDebugLevel, false);
        }
      }

      fMuonDenominatorFakeCategory->Fill(FakeCategory);
      fMuonDenominatorJetFlavor->Fill(Flavor);

      //Fill Histograms
      fMuonDenominatorPt->Fill(mu->Pt());
      fMuonDenominatorEta->Fill(mu->Eta());
      fMuonDenominatorPhi->Fill(mu->Phi());
      fMuonDenominatorPtEta->Fill(mu->Pt(), mu->Eta());
      //fMuonDenominatorPtEtaPhi->Fill(mu->Pt(),mu->Eta(), mu->Phi());  

      if (passesRealLeptonRemovalRequirements) {
        fMuonDenominatorFakeCategory_WithCuts->Fill(FakeCategory);
        fMuonDenominatorJetFlavor_WithCuts->Fill(Flavor);
        fMuonDenominatorPt_WithCuts->Fill(mu->Pt());
        fMuonDenominatorEta_WithCuts->Fill(mu->Eta());
        fMuonDenominatorPhi_WithCuts->Fill(mu->Phi());
        fMuonDenominatorPtEta_WithCuts->Fill(mu->Pt(), mu->Eta());
        //fMuonDenominatorPtEtaPhi_WithCuts->Fill(mu->Pt(),mu->Eta(), mu->Phi());  
      }

      //Fill Histograms in different categories
      if (FakeCategory == 0) {
        fMuonDenominatorPt_NoMatch->Fill(mu->Pt());
        fMuonDenominatorEta_NoMatch->Fill(mu->Eta());
        fMuonDenominatorPhi_NoMatch->Fill(mu->Phi());
        fMuonDenominatorPtEta_NoMatch->Fill(mu->Pt(), mu->Eta());
      } else if (FakeCategory == 1) {
        fMuonDenominatorPt_ChargedPion->Fill(mu->Pt());
        fMuonDenominatorEta_ChargedPion->Fill(mu->Eta());
        fMuonDenominatorPhi_ChargedPion->Fill(mu->Phi());
        fMuonDenominatorPtEta_ChargedPion->Fill(mu->Pt(), mu->Eta());
      } else if (FakeCategory == 2) {
         fMuonDenominatorPt_ChargedKaon->Fill(mu->Pt());
        fMuonDenominatorEta_ChargedKaon->Fill(mu->Eta());
        fMuonDenominatorPhi_ChargedKaon->Fill(mu->Phi());
        fMuonDenominatorPtEta_ChargedKaon->Fill(mu->Pt(), mu->Eta());
     } else if (FakeCategory == 3) {
        fMuonDenominatorPt_MuonFromOtherProcess->Fill(mu->Pt());
        fMuonDenominatorEta_MuonFromOtherProcess->Fill(mu->Eta());
        fMuonDenominatorPhi_MuonFromOtherProcess->Fill(mu->Phi());
        fMuonDenominatorPtEta_MuonFromOtherProcess->Fill(mu->Pt(), mu->Eta());
      } else if (FakeCategory == 4) {
        fMuonDenominatorPt_HeavyFlavor->Fill(mu->Pt());
        fMuonDenominatorEta_HeavyFlavor->Fill(mu->Eta());
        fMuonDenominatorPhi_HeavyFlavor->Fill(mu->Phi());
        fMuonDenominatorPtEta_HeavyFlavor->Fill(mu->Pt(), mu->Eta());
      } else if (FakeCategory == 5) {
        fMuonDenominatorPt_PionPunchThru->Fill(mu->Pt());
        fMuonDenominatorEta_PionPunchThru->Fill(mu->Eta());
        fMuonDenominatorPhi_PionPunchThru->Fill(mu->Phi());
        fMuonDenominatorPtEta_PionPunchThru->Fill(mu->Pt(), mu->Eta());
      } else if (FakeCategory == 6) {
        fMuonDenominatorPt_OtherNonMuon->Fill(mu->Pt());
        fMuonDenominatorEta_OtherNonMuon->Fill(mu->Eta());
        fMuonDenominatorPhi_OtherNonMuon->Fill(mu->Phi());
        fMuonDenominatorPtEta_OtherNonMuon->Fill(mu->Pt(), mu->Eta());
      } else if (FakeCategory == 7) {
        fMuonDenominatorPt_NoAncestor->Fill(mu->Pt());
        fMuonDenominatorEta_NoAncestor->Fill(mu->Eta());
        fMuonDenominatorPhi_NoAncestor->Fill(mu->Phi());
        fMuonDenominatorPtEta_NoAncestor->Fill(mu->Pt(), mu->Eta());
      }

 
      //add to denominators
      denominators->Add(mu);

      if (mu->Charge() == 1) {
        fPlusMuonDenominatorPt->Fill(mu->Pt());
        fPlusMuonDenominatorEta->Fill(mu->Eta());
        fPlusMuonDenominatorPhi->Fill(mu->Phi());
      } else if (mu->Charge() == -1) {
        fMinusMuonDenominatorPt->Fill(mu->Pt());
        fMinusMuonDenominatorEta->Fill(mu->Eta());
        fMinusMuonDenominatorPhi->Fill(mu->Phi());
      }

      //Let's find the charge of the W.
      if (GenLeptons->GetEntries() == 1) {
        if (GenLeptons->At(0)->Charge() == 1) {
          if (mu->Charge() == 1) {
            fPlusMuonPlusWDenominatorPt->Fill(mu->Pt());
            fPlusMuonPlusWDenominatorEta->Fill(mu->Eta());
            fPlusMuonPlusWDenominatorPhi->Fill(mu->Phi());
          } else if (mu->Charge() == -1) {
            fMinusMuonPlusWDenominatorPt->Fill(mu->Pt());
            fMinusMuonPlusWDenominatorEta->Fill(mu->Eta());
            fMinusMuonPlusWDenominatorPhi->Fill(mu->Phi());
          }
        } else {
          if (mu->Charge() == 1) {
            fPlusMuonMinusWDenominatorPt->Fill(mu->Pt());
            fPlusMuonMinusWDenominatorEta->Fill(mu->Eta());
            fPlusMuonMinusWDenominatorPhi->Fill(mu->Phi());
          } else if (mu->Charge() == -1) {
            fMinusMuonMinusWDenominatorPt->Fill(mu->Pt());
            fMinusMuonMinusWDenominatorEta->Fill(mu->Eta());
            fMinusMuonMinusWDenominatorPhi->Fill(mu->Phi());
          }
        }
      }
    }
  }

  //***********************************************************************************************
  //Find Numerator Objects for Muons
  //Numerators are Clean Muons
  //***********************************************************************************************
  for (UInt_t i=0; i<CleanMuons->GetEntries(); i++) {  

    const Muon *mu = CleanMuons->At(i);
    const Track *muonTrack = muonTrack = mu->Trk();
    if (mu->HasTrackerTrk())
      muonTrack = mu->TrackerTrk();

    //Veto denominators matching to real muons
    Bool_t IsMCLepton = false;
    for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
      if (MathUtils::DeltaR(muonTrack->Phi(), 
                            muonTrack->Eta(),
                            GenLeptonsAndTaus->At(l)->Phi(), 
                            GenLeptonsAndTaus->At(l)->Eta()) < 0.1) {
        IsMCLepton = true;
      }
    }

    bool passesSampleDependantRequirements = true;
    bool passesRealLeptonRemovalRequirements = true;
    if (fSampleType == 10) {
      //Veto on Trigger Jet
      Bool_t IsTriggerJet = false;
      if (fVetoTriggerJet && leadingTriggerJet) {
        Double_t deltaR = MathUtils::DeltaR(mu->Phi(), mu->Eta(),
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
        tempParticle->AddDaughter(mu);
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
      for (UInt_t l=0; l<CleanPhotons->GetEntries(); l++) {  
        Double_t deltaR = MathUtils::DeltaR(mu->Phi(), mu->Eta(),
                                            CleanPhotons->At(l)->Phi(), 
                                            CleanPhotons->At(l)->Eta());
        if ( CleanPhotons->At(l)->Pt() > 20.0 && deltaR < 0.3 ) {
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
        tempParticle->AddDaughter(mu);
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

    //numerator cuts
    if (mu->Pt() > 10.0 && passesSampleDependantRequirements
        && !(fVetoGeneratorLeptons && IsMCLepton)
      ) {

      //Categorize Fake Muon
      Int_t printDebugLevel  = 0;    
      Bool_t isTrackerTrack = false;
      const MCParticle *matchedSimParticle = 0;
      if (mu->TrackerTrk()) { 
        isTrackerTrack = true;      
        matchedSimParticle = GeneratorTools::MatchMuonToSimParticle(
          fParticles, mu->TrackerTrk(), isTrackerTrack, printDebugLevel, false);
      } else {
        isTrackerTrack = false;      
        matchedSimParticle = GeneratorTools::MatchMuonToSimParticle(
          fParticles, mu->Trk(), isTrackerTrack, printDebugLevel, false);
      }

      Int_t FakeCategory = GeneratorTools::CategorizeFakeMuon(matchedSimParticle);      

      //Find Matching GenJet and find its flavor
      Double_t Flavor = -9;
      Double_t minDR = 5000.0;
      const GenJet *matchedGenJet = 0;
      for (UInt_t j=0; j<fGenJets->GetEntries(); j++) {        
        Double_t dr = MathUtils::DeltaR(mu->Phi(), mu->Eta(),
                                        fGenJets->At(j)->Phi(), fGenJets->At(j)->Eta());
        if ( dr < minDR ) {
          minDR = dr;
          matchedGenJet = fGenJets->At(j);
        }
      }
      if (minDR < 0.3) {
        Flavor = matchedGenJet->MatchedMCFlavor();
        // if (ancestor) {
//           ancestorFragmentationX = ancestor->Pt() / matchedGenJet->Pt();
//         }
      }

      if (FakeCategory == -10) {
        //printdebug = true;

        cout << "Match : "  
             << matchedSimParticle->PdgId()  
             << " " << matchedSimParticle->Status() << " " << matchedSimParticle->Pt()
             << " " << matchedSimParticle->Eta() << " " << matchedSimParticle->Phi()
             << " produced at ";
        if (matchedSimParticle->Mother())
          cout << matchedSimParticle->Mother()->DecayVertex().Rho();
        else 
          cout << "0.0";
        cout << " decays at ";
        if (matchedSimParticle->NDaughters() > 0)
          cout << matchedSimParticle->DecayVertex().Rho();
        else
          cout << " Stable ";
        cout << endl;

        cout << "Fake Muon : " << mu->Pt() << " " << mu->Eta() << " " << mu->Phi() 
             << "  FakeCategory = " << FakeCategory << endl;

        Int_t printDebugLevel  = 7;    
        Bool_t isTrackerTrack = false;
        const MCParticle *tmp = 0;
        if (mu->TrackerTrk()) { 
          isTrackerTrack = true;      
          tmp = GeneratorTools::MatchMuonToSimParticle(
            fParticles, mu->TrackerTrk(), isTrackerTrack, printDebugLevel, false);
        } else {
          isTrackerTrack = false;      
          tmp = GeneratorTools::MatchMuonToSimParticle(
            fParticles, mu->Trk(), isTrackerTrack, printDebugLevel, false);
        }
        
      }



      fMuonNumeratorFakeCategory->Fill(FakeCategory);
      fMuonNumeratorJetFlavor->Fill(Flavor);

      //Fill Histograms
      fMuonNumeratorPt->Fill(mu->Pt());
      fMuonNumeratorEta->Fill(mu->Eta());
      fMuonNumeratorPhi->Fill(mu->Phi());
      fMuonNumeratorPtEta->Fill(mu->Pt(), mu->Eta());
      //fMuonNumeratorPtEtaPhi->Fill(mu->Pt(),mu->Eta(), mu->Phi());      

      if (passesRealLeptonRemovalRequirements) {
        fMuonNumeratorFakeCategory_WithCuts->Fill(FakeCategory);
        fMuonNumeratorJetFlavor_WithCuts->Fill(Flavor);
        fMuonNumeratorPt_WithCuts->Fill(mu->Pt());
        fMuonNumeratorEta_WithCuts->Fill(mu->Eta());
        fMuonNumeratorPhi_WithCuts->Fill(mu->Phi());
        fMuonNumeratorPtEta_WithCuts->Fill(mu->Pt(), mu->Eta());
        //fMuonNumeratorPtEtaPhi_WithCuts->Fill(mu->Pt(),mu->Eta(), mu->Phi());      
      }

     //Fill Histograms in different categories
      if (FakeCategory == 0) {
        fMuonNumeratorPt_NoMatch->Fill(mu->Pt());
        fMuonNumeratorEta_NoMatch->Fill(mu->Eta());
        fMuonNumeratorPhi_NoMatch->Fill(mu->Phi());
        fMuonNumeratorPtEta_NoMatch->Fill(mu->Pt(), mu->Eta());
      } else if (FakeCategory == 1) {
        fMuonNumeratorPt_ChargedPion->Fill(mu->Pt());
        fMuonNumeratorEta_ChargedPion->Fill(mu->Eta());
        fMuonNumeratorPhi_ChargedPion->Fill(mu->Phi());
        fMuonNumeratorPtEta_ChargedPion->Fill(mu->Pt(), mu->Eta());
      } else if (FakeCategory == 2) {
         fMuonNumeratorPt_ChargedKaon->Fill(mu->Pt());
        fMuonNumeratorEta_ChargedKaon->Fill(mu->Eta());
        fMuonNumeratorPhi_ChargedKaon->Fill(mu->Phi());
        fMuonNumeratorPtEta_ChargedKaon->Fill(mu->Pt(), mu->Eta());
     } else if (FakeCategory == 3) {
        fMuonNumeratorPt_MuonFromOtherProcess->Fill(mu->Pt());
        fMuonNumeratorEta_MuonFromOtherProcess->Fill(mu->Eta());
        fMuonNumeratorPhi_MuonFromOtherProcess->Fill(mu->Phi());
        fMuonNumeratorPtEta_MuonFromOtherProcess->Fill(mu->Pt(), mu->Eta());
      } else if (FakeCategory == 4) {
        fMuonNumeratorPt_HeavyFlavor->Fill(mu->Pt());
        fMuonNumeratorEta_HeavyFlavor->Fill(mu->Eta());
        fMuonNumeratorPhi_HeavyFlavor->Fill(mu->Phi());
        fMuonNumeratorPtEta_HeavyFlavor->Fill(mu->Pt(), mu->Eta());
      } else if (FakeCategory == 5) {
        fMuonNumeratorPt_PionPunchThru->Fill(mu->Pt());
        fMuonNumeratorEta_PionPunchThru->Fill(mu->Eta());
        fMuonNumeratorPhi_PionPunchThru->Fill(mu->Phi());
        fMuonNumeratorPtEta_PionPunchThru->Fill(mu->Pt(), mu->Eta());
      } else if (FakeCategory == 6) {
        fMuonNumeratorPt_OtherNonMuon->Fill(mu->Pt());
        fMuonNumeratorEta_OtherNonMuon->Fill(mu->Eta());
        fMuonNumeratorPhi_OtherNonMuon->Fill(mu->Phi());
        fMuonNumeratorPtEta_OtherNonMuon->Fill(mu->Pt(), mu->Eta());
      } else if (FakeCategory == 7) {
        fMuonNumeratorPt_NoAncestor->Fill(mu->Pt());
        fMuonNumeratorEta_NoAncestor->Fill(mu->Eta());
        fMuonNumeratorPhi_NoAncestor->Fill(mu->Phi());
        fMuonNumeratorPtEta_NoAncestor->Fill(mu->Pt(), mu->Eta());
      }




      if (mu->Charge() == 1) {
        fPlusMuonNumeratorPt->Fill(mu->Pt());
        fPlusMuonNumeratorEta->Fill(mu->Eta());
        fPlusMuonNumeratorPhi->Fill(mu->Phi());
      } else if (mu->Charge() == -1) {
        fMinusMuonNumeratorPt->Fill(mu->Pt());
        fMinusMuonNumeratorEta->Fill(mu->Eta());
        fMinusMuonNumeratorPhi->Fill(mu->Phi());
      }

      //Let's find the charge of the W.
      if (GenLeptons->GetEntries() == 1) {
        if (GenLeptons->At(0)->Charge() == 1) {
          if (mu->Charge() == 1) {
            fPlusMuonPlusWNumeratorPt->Fill(mu->Pt());
            fPlusMuonPlusWNumeratorEta->Fill(mu->Eta());
            fPlusMuonPlusWNumeratorPhi->Fill(mu->Phi());
          } else if (mu->Charge() == -1) {
            fMinusMuonPlusWNumeratorPt->Fill(mu->Pt());
            fMinusMuonPlusWNumeratorEta->Fill(mu->Eta());
            fMinusMuonPlusWNumeratorPhi->Fill(mu->Phi());
          }
        } else {
          if (mu->Charge() == 1) {
            fPlusMuonMinusWNumeratorPt->Fill(mu->Pt());
            fPlusMuonMinusWNumeratorEta->Fill(mu->Eta());
            fPlusMuonMinusWNumeratorPhi->Fill(mu->Phi());
          } else if (mu->Charge() == -1) {
            fMinusMuonMinusWNumeratorPt->Fill(mu->Pt());
            fMinusMuonMinusWNumeratorEta->Fill(mu->Eta());
            fMinusMuonMinusWNumeratorPhi->Fill(mu->Phi());
          }
        }
      }

      
      //Check if numerator is also in the denominator
      Bool_t isADenominator = false;
      for (UInt_t d=0; d<denominators->GetEntries(); d++) {
        if (MathUtils::DeltaR(denominators->At(d)->Phi(),denominators->At(d)->Eta(),
                              mu->Phi(),mu->Eta()) < 0.3)
          isADenominator = true;
      }

      //Check if a numerator is not in the denominator
      if (!isADenominator) {
        cout << "\nNOT DENOMINATOR!!!";

        cout << "Module: " << GetName() << endl;
        cout << "LumiSec: " << GetEventHeader()->LumiSec() << " RunNum: " 
             << GetEventHeader()->RunNum()
             << " EventNum: " << GetEventHeader()->EvtNum() << endl;
        
        cout << "CleanMuon " << i << " ";
        if (mu->HasGlobalTrk())
          cout << " global ";
        else if (mu->HasTrackerTrk())
          cout << " trackeronly ";
        else if (mu->HasStandaloneTrk())
          cout << " standalone ";
        else
          cout << " none ";
        
        cout << " : " << mu->Pt() << " " 
             << mu->Eta() << " " << mu->Phi() << " " 
             << " tracker track: ";
        if (mu->HasTrackerTrk()) {
          cout << mu->TrackerTrk()->Pt() << " "  
               << mu->TrackerTrk()->Eta() << " " 
               << mu->TrackerTrk()->Phi() << " : ";
        } 
        if (mu->HasStandaloneTrk()) {
          cout << " standalone track : " << mu->StandaloneTrk()->Pt() << " "  
               << mu->StandaloneTrk()->Eta() << " " 
               << mu->StandaloneTrk()->Phi() << " : ";
        } 
        if (mu->HasGlobalTrk()) {
          cout << " global track : " << mu->GlobalTrk()->Pt() << " "  
               << mu->GlobalTrk()->Eta() << " " 
               << mu->GlobalTrk()->Phi() << " : ";
        }
        cout << endl;

        //printdebug = true;
      }  
    }
  }

  //*******************************************************************************************
  // Do Various checks for Passing Numerators.
  //*******************************************************************************************     
  if (printdebug) {
    
    cout << "LumiSec: " << GetEventHeader()->LumiSec() << " RunNum: " 
         << GetEventHeader()->RunNum()
         << " EventNum: " << GetEventHeader()->EvtNum() << endl;
    cout << "***********************************************************************\n";
    cout << "Numerators\n";
    cout << "***********************************************************************\n";
    for (UInt_t i=0; i<CleanMuons->GetEntries(); i++) {  
      cout << "CleanMuon " << i << " ";
      if (CleanMuons->At(i)->HasGlobalTrk())
        cout << " global ";
      else if (CleanMuons->At(i)->HasTrackerTrk())
        cout << " trackeronly ";
      else if (CleanMuons->At(i)->HasStandaloneTrk())
        cout << " standalone ";
      else
        cout << " none ";
      
      cout << " : " << CleanMuons->At(i)->Pt() << " " 
           << CleanMuons->At(i)->Eta() << " " << CleanMuons->At(i)->Phi() << " " 
           << " tracker track: ";
      if (CleanMuons->At(i)->HasTrackerTrk()) {
        cout << CleanMuons->At(i)->TrackerTrk()->Pt() << " "  
             << CleanMuons->At(i)->TrackerTrk()->Eta() << " " 
             << CleanMuons->At(i)->TrackerTrk()->Phi() << " : ";
      } 
      if (CleanMuons->At(i)->HasStandaloneTrk()) {
        cout << " standalone track : " << CleanMuons->At(i)->StandaloneTrk()->Pt() << " "  
             << CleanMuons->At(i)->StandaloneTrk()->Eta() << " " 
             << CleanMuons->At(i)->StandaloneTrk()->Phi() << " : ";
      } 
      if (CleanMuons->At(i)->HasGlobalTrk()) {
        cout << " global track : " << CleanMuons->At(i)->GlobalTrk()->Pt() << " "  
             << CleanMuons->At(i)->GlobalTrk()->Eta() << " " 
             << CleanMuons->At(i)->GlobalTrk()->Phi() << " : ";
      }
      cout << "Isolation : " << CleanMuons->At(i)->IsoR03SumPt() << " "
           << CleanMuons->At(i)->IsoR03EmEt() << " "
           << CleanMuons->At(i)->IsoR03HadEt() << " "
           << " ";
      double iso = IsolationTools::TrackIsolation(CleanMuons->At(i)->Trk(), 0.3, 0.015, 1.0, 0.2,
                                                  fTracks);
      cout << iso << endl;
      cout << endl;
    }
    cout << "***********************************************************************\n";
    cout << "Denominators\n";
    cout << "***********************************************************************\n";
    for (UInt_t d=0; d<MuonFakeableObjects->GetEntries(); d++) {
      cout << " : " << MuonFakeableObjects->At(d)->Pt() << " " 
           << MuonFakeableObjects->At(d)->Eta() << " " << MuonFakeableObjects->At(d)->Phi() << " " 
           << " tracker track: ";
      if (MuonFakeableObjects->At(d)->HasTrackerTrk()) {
        cout << MuonFakeableObjects->At(d)->TrackerTrk()->Pt() << " "  
             << MuonFakeableObjects->At(d)->TrackerTrk()->Eta() << " " 
             << MuonFakeableObjects->At(d)->TrackerTrk()->Phi() << " : ";
      } 
      if (MuonFakeableObjects->At(d)->HasStandaloneTrk()) {
        cout << " standalone track : " << MuonFakeableObjects->At(d)->StandaloneTrk()->Pt() << " "  
             << MuonFakeableObjects->At(d)->StandaloneTrk()->Eta() << " " 
             << MuonFakeableObjects->At(d)->StandaloneTrk()->Phi() << " : ";
      } 
      if (MuonFakeableObjects->At(d)->HasGlobalTrk()) {
        cout << " global track : " << MuonFakeableObjects->At(d)->GlobalTrk()->Pt() << " "  
             << MuonFakeableObjects->At(d)->GlobalTrk()->Eta() << " " 
             << MuonFakeableObjects->At(d)->GlobalTrk()->Phi() << " : ";
      }
      cout << "Isolation : " << MuonFakeableObjects->At(d)->IsoR03SumPt() << " "
           << MuonFakeableObjects->At(d)->IsoR03EmEt() << " "
           << MuonFakeableObjects->At(d)->IsoR03HadEt() << " "
           << " ";
      double iso = IsolationTools::TrackIsolation(MuonFakeableObjects->At(d)->Trk(), 
                                                  0.3, 0.015, 1.0, 0.2,fTracks);
      cout << iso << endl;
      cout << endl;
    }
    
    for (UInt_t d=0; d<GenLeptonsAndTaus->GetEntries(); d++) {
      cout << "GenLeptonsAndTaus: " << d << " : " << GenLeptonsAndTaus->At(d)->PdgId() << " " 
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


  delete GenLeptonsAndTaus;
  delete denominators;
}

//--------------------------------------------------------------------------------------------------
void ComputeMuonFakeRateMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.

}

//--------------------------------------------------------------------------------------------------
void ComputeMuonFakeRateMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
