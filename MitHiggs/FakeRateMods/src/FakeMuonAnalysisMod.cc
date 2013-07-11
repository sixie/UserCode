// $Id: FakeMuonAnalysisMod.cc,v 1.2 2009/03/17 08:33:15 sixie Exp $

#include "MitHiggs/FakeRateMods/interface/FakeMuonAnalysisMod.h"
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

ClassImp(mithep::FakeMuonAnalysisMod)

//--------------------------------------------------------------------------------------------------
  FakeMuonAnalysisMod::FakeMuonAnalysisMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fVetoGeneratorLeptons(true),
  fRequireFirstLepton(false),
  fPrintLogs(true),
  fMCPartBranchName(Names::gkMCPartBrn),
  fGenJetBranchName(Names::gkSC5GenJetBrn),
  fTrackBranchName(Names::gkTrackBrn),
  fMuonBranchName(Names::gkMuonBrn),
  fJetBranchName(Names::gkCaloJetBrn),
  fCleanMuonsName(ModNames::gkCleanMuonsName),
  fGoodJetsName(ModNames::gkGoodJetsName),
  fMCLeptonsName(ModNames::gkMCLeptonsName),
  fMCTausName(ModNames::gkMCTausName),
  fParticles(0),
  fTracks(0),
  fMuons(0),
  fJets(0)
{ 
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void FakeMuonAnalysisMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void FakeMuonAnalysisMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fMCPartBranchName,                fParticles);
  ReqBranch(fGenJetBranchName,                fGenJets);
  ReqBranch(fTrackBranchName,                 fTracks);
  ReqBranch(fMuonBranchName,                  fMuons);
  ReqBranch(fJetBranchName,                   fJets);

  //***********************************************************************************************
  //Histograms for Fake Muon Charge
  //***********************************************************************************************  

  AddTH1(fCleanCentralMuonTrackerGlobalTrackChargeMatch, 
         "hCleanCentralMuonTrackerGlobalTrackChargeMatch", 
         ";CleanCentralMuonTrackerGlobalTrackChargeMatch;Number of Events", 5, -2.5,2.5);
  AddTH1(fCleanForwardMuonTrackerGlobalTrackChargeMatch, 
         "hCleanForwardMuonTrackerGlobalTrackChargeMatch", 
         ";CleanForwardMuonTrackerGlobalTrackChargeMatch;Number of Events", 5, -2.5,2.5);


  //***********************************************************************************************
  //Histograms for Fake Muon to GenJet Matching
  //***********************************************************************************************  

  AddTH1(fCleanPlusCentralMuonMatchedJetFlavor, "hCleanPlusCentralMuonMatchedJetFlavor", 
         ";CleanPlusCentralMuonMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fCleanMinusCentralMuonMatchedJetFlavor, "hCleanMinusCentralMuonMatchedJetFlavor", 
         ";CleanMinusCentralMuonMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fCleanPlusCentralMuonMatchedGenJetPt, "hCleanPlusCentralMuonMatchedGenJetPt", 
         ";CleanPlusCentralMuonMatchedGenJetPt;Number of Events", 100, 0,100);
  AddTH1(fCleanMinusCentralMuonMatchedGenJetPt, "hCleanMinusCentralMuonMatchedGenJetPt", 
         ";CleanMinusCentralMuonMatchedGenJetPt;Number of Events", 100, 0,100);
  AddTH1(fCleanPlusCentralMuonMatchedGenJetDeltaR, "hCleanPlusCentralMuonMatchedGenJetDeltaR", 
         ";CleanPlusCentralMuonMatchedGenJetDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanMinusCentralMuonMatchedGenJetDeltaR, "hCleanMinusCentralMuonMatchedGenJetDeltaR", 
         ";CleanMinusCentralMuonMatchedGenJetDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanPlusCentralMuonMatchedGenJetDeltaPtOverPt, "hCleanPlusCentralMuonMatchedGenJetDeltaPtOverPt", 
         ";CleanPlusCentralMuonMatchedGenJetDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusCentralMuonMatchedGenJetDeltaPtOverPt, "hCleanMinusCentralMuonMatchedGenJetDeltaPtOverPt", 
         ";CleanMinusCentralMuonMatchedGenJetDeltaPtOverPt;Number of Events", 100, -2.0,1.0);

  AddTH1(fCleanPlusForwardMuonMatchedJetFlavor, "hCleanPlusForwardMuonMatchedJetFlavor", 
         ";CleanPlusForwardMuonMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fCleanMinusForwardMuonMatchedJetFlavor, "hCleanMinusForwardMuonMatchedJetFlavor", 
         ";CleanMinusForwardMuonMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fCleanPlusForwardMuonMatchedGenJetPt, "hCleanPlusForwardMuonMatchedGenJetPt", 
         ";CleanPlusForwardMuonMatchedGenJetPt;Number of Events", 100, 0,100);
  AddTH1(fCleanMinusForwardMuonMatchedGenJetPt, "hCleanMinusForwardMuonMatchedGenJetPt", 
         ";CleanMinusForwardMuonMatchedGenJetPt;Number of Events", 100, 0,100);
  AddTH1(fCleanPlusForwardMuonMatchedGenJetDeltaR, "hCleanPlusForwardMuonMatchedGenJetDeltaR", 
         ";CleanPlusForwardMuonMatchedGenJetDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanMinusForwardMuonMatchedGenJetDeltaR, "hCleanMinusForwardMuonMatchedGenJetDeltaR", 
         ";CleanMinusForwardMuonMatchedGenJetDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanPlusForwardMuonMatchedGenJetDeltaPtOverPt, "hCleanPlusForwardMuonMatchedGenJetDeltaPtOverPt", 
         ";CleanPlusForwardMuonMatchedGenJetDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusForwardMuonMatchedGenJetDeltaPtOverPt, "hCleanMinusForwardMuonMatchedGenJetDeltaPtOverPt", 
         ";CleanMinusForwardMuonMatchedGenJetDeltaPtOverPt;Number of Events", 100, -2.0,1.0);

  //***********************************************************************************************
  //Histograms for Fake Muon to GenParticle Matching
  //***********************************************************************************************  
  AddTH1(fCleanPlusCentralMuonMatchedGenParticleID, "hCleanPlusCentralMuonMatchedGenParticleID", 
         ";CleanPlusCentralMuonMatchedGenParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanMinusCentralMuonMatchedGenParticleID, "hCleanMinusCentralMuonMatchedGenParticleID", 
         ";CleanMinusCentralMuonMatchedGenParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanPlusCentralMuonMatchedGenParticlePt, "hCleanPlusCentralMuonMatchedGenParticlePt", 
         ";CleanPlusCentralMuonMatchedGenParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanMinusCentralMuonMatchedGenParticlePt, "hCleanMinusCentralMuonMatchedGenParticlePt", 
         ";CleanMinusCentralMuonMatchedGenParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanPlusCentralMuonMatchedGenParticleDeltaR, "hCleanPlusCentralMuonMatchedGenParticleDeltaR", 
         ";CleanPlusCentralMuonMatchedGenParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanMinusCentralMuonMatchedGenParticleDeltaR, "hCleanMinusCentralMuonMatchedGenParticleDeltaR", 
         ";CleanMinusCentralMuonMatchedGenParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanPlusCentralMuonMatchedGenParticleDeltaPtOverPt, "hCleanPlusCentralMuonMatchedGenParticleDeltaPtOverPt", 
         ";CleanPlusCentralMuonMatchedGenParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusCentralMuonMatchedGenParticleDeltaPtOverPt, "hCleanMinusCentralMuonMatchedGenParticleDeltaPtOverPt", 
         ";CleanMinusCentralMuonMatchedGenParticleDeltaPtOverPt;Number of Events",  100, -2.0,1.0);

  AddTH1(fCleanPlusForwardMuonMatchedGenParticleID, "hCleanPlusForwardMuonMatchedGenParticleID", 
         ";CleanPlusForwardMuonMatchedGenParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanMinusForwardMuonMatchedGenParticleID, "hCleanMinusForwardMuonMatchedGenParticleID", 
         ";CleanMinusForwardMuonMatchedGenParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanPlusForwardMuonMatchedGenParticlePt, "hCleanPlusForwardMuonMatchedGenParticlePt", 
         ";CleanPlusForwardMuonMatchedGenParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanMinusForwardMuonMatchedGenParticlePt, "hCleanMinusForwardMuonMatchedGenParticlePt", 
         ";CleanMinusForwardMuonMatchedGenParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanPlusForwardMuonMatchedGenParticleDeltaR, "hCleanPlusForwardMuonMatchedGenParticleDeltaR", 
         ";CleanPlusForwardMuonMatchedGenParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanMinusForwardMuonMatchedGenParticleDeltaR, "hCleanMinusForwardMuonMatchedGenParticleDeltaR", 
         ";CleanMinusForwardMuonMatchedGenParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanPlusForwardMuonMatchedGenParticleDeltaPtOverPt, "hCleanPlusForwardMuonMatchedGenParticleDeltaPtOverPt", 
         ";CleanPlusForwardMuonMatchedGenParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusForwardMuonMatchedGenParticleDeltaPtOverPt, "hCleanMinusForwardMuonMatchedGenParticleDeltaPtOverPt", 
         ";CleanMinusForwardMuonMatchedGenParticleDeltaPtOverPt;Number of Events",  100, -2.0,1.0);

  //***********************************************************************************************
  //Histograms for Fake Muon to SimTrack Matching
  //***********************************************************************************************  
  AddTH1(fCleanPlusCentralMuonMatchedSimChargedParticleID, 
         "hCleanPlusCentralMuonMatchedSimChargedParticleID", 
         ";CleanPlusCentralMuonMatchedSimChargedParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanMinusCentralMuonMatchedSimChargedParticleID, 
         "hCleanMinusCentralMuonMatchedSimChargedParticleID", 
         ";CleanMinusCentralMuonMatchedSimChargedParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanPlusCentralMuonMatchedSimChargedParticlePt, 
         "hCleanPlusCentralMuonMatchedSimChargedParticlePt", 
         ";CleanPlusCentralMuonMatchedSimChargedParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanMinusCentralMuonMatchedSimChargedParticlePt, 
         "hCleanMinusCentralMuonMatchedSimChargedParticlePt", 
         ";CleanMinusCentralMuonMatchedSimChargedParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanPlusCentralMuonMatchedSimChargedParticleDeltaR, 
         "hCleanPlusCentralMuonMatchedSimChargedParticleDeltaR", 
         ";CleanPlusCentralMuonMatchedSimChargedParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanMinusCentralMuonMatchedSimChargedParticleDeltaR, 
         "hCleanMinusCentralMuonMatchedSimChargedParticleDeltaR", 
         ";CleanMinusCentralMuonMatchedSimChargedParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanPlusCentralMuonMatchedSimChargedParticleDeltaPtOverPt, 
         "hCleanPlusCentralMuonMatchedSimChargedParticleDeltaPtOverPt", 
         ";CleanPlusCentralMuonMatchedSimChargedParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusCentralMuonMatchedSimChargedParticleDeltaPtOverPt, 
         "hCleanMinusCentralMuonMatchedSimChargedParticleDeltaPtOverPt", 
         ";CleanMinusCentralMuonMatchedSimChargedParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);

  AddTH1(fCleanPlusForwardMuonMatchedSimChargedParticleID, 
         "hCleanPlusForwardMuonMatchedSimChargedParticleID", 
         ";CleanPlusForwardMuonMatchedSimChargedParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanMinusForwardMuonMatchedSimChargedParticleID, 
         "hCleanMinusForwardMuonMatchedSimChargedParticleID", 
         ";CleanMinusForwardMuonMatchedSimChargedParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanPlusForwardMuonMatchedSimChargedParticlePt, 
         "hCleanPlusForwardMuonMatchedSimChargedParticlePt", 
         ";CleanPlusForwardMuonMatchedSimChargedParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanMinusForwardMuonMatchedSimChargedParticlePt, 
         "hCleanMinusForwardMuonMatchedSimChargedParticlePt", 
         ";CleanMinusForwardMuonMatchedSimChargedParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanPlusForwardMuonMatchedSimChargedParticleDeltaR, 
         "hCleanPlusForwardMuonMatchedSimChargedParticleDeltaR", 
         ";CleanPlusForwardMuonMatchedSimChargedParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanMinusForwardMuonMatchedSimChargedParticleDeltaR, 
         "hCleanMinusForwardMuonMatchedSimChargedParticleDeltaR", 
         ";CleanMinusForwardMuonMatchedSimChargedParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanPlusForwardMuonMatchedSimChargedParticleDeltaPtOverPt, 
         "hCleanPlusForwardMuonMatchedSimChargedParticleDeltaPtOverPt", 
         ";CleanPlusForwardMuonMatchedSimChargedParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusForwardMuonMatchedSimChargedParticleDeltaPtOverPt, 
         "hCleanMinusForwardMuonMatchedSimChargedParticleDeltaPtOverPt", 
         ";CleanMinusForwardMuonMatchedSimChargedParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);

  //***********************************************************************************************
  //Histograms for Fake Muon to SimTrack Ancestor Matching
  //***********************************************************************************************  
  AddTH1(fCleanPlusCentralMuonMatchedAncestorParticleID, 
         "hCleanPlusCentralMuonMatchedAncestorParticleID", 
         ";CleanPlusCentralMuonMatchedAncestorParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanMinusCentralMuonMatchedAncestorParticleID, 
         "hCleanMinusCentralMuonMatchedAncestorParticleID", 
         ";CleanMinusCentralMuonMatchedAncestorParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanPlusCentralMuonMatchedAncestorParticlePt, 
         "hCleanPlusCentralMuonMatchedAncestorParticlePt", 
         ";CleanPlusCentralMuonMatchedAncestorParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanMinusCentralMuonMatchedAncestorParticlePt, 
         "hCleanMinusCentralMuonMatchedAncestorParticlePt", 
         ";CleanMinusCentralMuonMatchedAncestorParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanPlusCentralMuonMatchedAncestorParticleDeltaR, 
         "hCleanPlusCentralMuonMatchedAncestorParticleDeltaR", 
         ";CleanPlusCentralMuonMatchedAncestorParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanMinusCentralMuonMatchedAncestorParticleDeltaR, 
         "hCleanMinusCentralMuonMatchedAncestorParticleDeltaR", 
         ";CleanMinusCentralMuonMatchedAncestorParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanPlusCentralMuonMatchedAncestorParticleDeltaPtOverPt, 
         "hCleanPlusCentralMuonMatchedAncestorParticleDeltaPtOverPt", 
         ";CleanPlusCentralMuonMatchedAncestorParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusCentralMuonMatchedAncestorParticleDeltaPtOverPt, 
         "hCleanMinusCentralMuonMatchedAncestorParticleDeltaPtOverPt", 
         ";CleanMinusCentralMuonMatchedAncestorParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);

  AddTH1(fCleanPlusForwardMuonMatchedAncestorParticleID, 
         "hCleanPlusForwardMuonMatchedAncestorParticleID", 
         ";CleanPlusForwardMuonMatchedAncestorParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanMinusForwardMuonMatchedAncestorParticleID, 
         "hCleanMinusForwardMuonMatchedAncestorParticleID", 
         ";CleanMinusForwardMuonMatchedAncestorParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanPlusForwardMuonMatchedAncestorParticlePt, 
         "hCleanPlusForwardMuonMatchedAncestorParticlePt", 
         ";CleanPlusForwardMuonMatchedAncestorParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanMinusForwardMuonMatchedAncestorParticlePt, 
         "hCleanMinusForwardMuonMatchedAncestorParticlePt", 
         ";CleanMinusForwardMuonMatchedAncestorParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanPlusForwardMuonMatchedAncestorParticleDeltaR, 
         "hCleanPlusForwardMuonMatchedAncestorParticleDeltaR", 
         ";CleanPlusForwardMuonMatchedAncestorParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanMinusForwardMuonMatchedAncestorParticleDeltaR, 
         "hCleanMinusForwardMuonMatchedAncestorParticleDeltaR", 
         ";CleanMinusForwardMuonMatchedAncestorParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanPlusForwardMuonMatchedAncestorParticleDeltaPtOverPt, 
         "hCleanPlusForwardMuonMatchedAncestorParticleDeltaPtOverPt", 
         ";CleanPlusForwardMuonMatchedAncestorParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusForwardMuonMatchedAncestorParticleDeltaPtOverPt, 
         "hCleanMinusForwardMuonMatchedAncestorParticleDeltaPtOverPt", 
         ";CleanMinusForwardMuonMatchedAncestorParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);


  //***********************************************************************************************
  //Histograms for Fake Muon unmatched to SimTrack
  //***********************************************************************************************  
  AddTH1(fCleanPlusCentralMuonUnmatchedSimChargedParticleBestDeltaPtOverPt, 
         "hCleanPlusCentralMuonUnmatchedSimChargedParticleBestDeltaPtOverPt", 
         ";CleanPlusCentralMuonUnmatchedSimChargedParticleBestDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusCentralMuonUnmatchedSimChargedParticleBestDeltaPtOverPt, 
         "hCleanMinusCentralMuonUnmatchedSimChargedParticleBestDeltaPtOverPt", 
         ";CleanMinusCentralMuonUnmatchedSimChargedParticleBestDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanPlusForwardMuonUnmatchedSimChargedParticleBestDeltaPtOverPt, 
         "hCleanPlusForwardMuonUnmatchedSimChargedParticleBestDeltaPtOverPt", 
         ";CleanPlusForwardMuonUnmatchedSimChargedParticleBestDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusForwardMuonUnmatchedSimChargedParticleBestDeltaPtOverPt, 
         "hCleanMinusForwardMuonUnmatchedSimChargedParticleBestDeltaPtOverPt", 
         ";CleanMinusForwardMuonUnmatchedSimChargedParticleBestDeltaPtOverPt;Number of Events", 100, -2.0,1.0);

  cerr << "hello\n";


}

//--------------------------------------------------------------------------------------------------
void FakeMuonAnalysisMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and
  //output debug info or not   

  LoadBranch(fMCPartBranchName);
  LoadBranch(fGenJetBranchName);
  LoadBranch(fMuonBranchName);
  LoadBranch(fTrackBranchName);
  LoadBranch(fJetBranchName);

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

  // get input clean object collections
  const MuonCol *CleanMuons = 0;
  if (!fCleanMuonsName.IsNull())
    CleanMuons = GetObjThisEvt<MuonCol>(fCleanMuonsName);
  mithep::ParticleOArr *CleanLeptons = dynamic_cast<mithep::ParticleOArr*>
    (FindObjThisEvt(ModNames::gkMergedLeptonsName));
  const JetCol *GoodJets = 0;
  if (!fGoodJetsName.IsNull())
    GoodJets = GetObjThisEvt<JetCol>(fGoodJetsName);


  //count GenJets above pt > 20GeV.
  Int_t NGenJets = 0;
  for (UInt_t i=0;i<fGenJets->GetEntries();i++) {
    if (fGenJets->At(i)->Pt() >= 20.0) 
      NGenJets++;
  }
 
  //***********************************************************************************************
  //Charge CorrelationStudy Between W and the jet
  //***********************************************************************************************  

  if ( (!fRequireFirstLepton && CleanLeptons->GetEntries() > 0) || 
       (fRequireFirstLepton && CleanLeptons->GetEntries() == 2 && GenLeptons->GetEntries() == 1 && NGenJets == 1)               
    ) {
                
    //***********************************************************************************************
    //Find Clean Muons not matching to a GenLepton
    //***********************************************************************************************  
    for (UInt_t i=0; i<CleanMuons->GetEntries(); i++) {
      const Muon *mu = CleanMuons->At(i);

      //Veto denominators matching to real genleptons
      Bool_t IsMCLepton = false;
      for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
        if (MathUtils::DeltaR(mu->Phi(), mu->Eta(),
                              GenLeptonsAndTaus->At(l)->Phi(), 
                              GenLeptonsAndTaus->At(l)->Eta()) < 0.3) {
          IsMCLepton = true;
        }
      }
      
      if (mu->Pt() > 10.0 && !(fVetoGeneratorLeptons && IsMCLepton)
        ) {
        //Found a fake muon.
              
        //Print out Muon        
        if(fPrintLogs) {
          
          cout << "********************************************************************************\n";
          cout << "Fake Muon : " << mu->Pt() << " " << mu->Eta() << " " << mu->Phi();
          if (mu->GlobalTrk()) {
            cout << "  | GlobalTrack: Charge " << mu->GlobalTrk()->Charge() << "   " 
                 << mu->GlobalTrk()->Pt() << " " << mu->GlobalTrk()->Eta() << " "
                 << mu->GlobalTrk()->Phi();
          }
          cout << " | ";
          if (mu->TrackerTrk()) {
            cout << " TrackerTrack: Charge " << mu->TrackerTrk()->Charge() << "   " 
                 << mu->TrackerTrk()->Pt() << " " << mu->TrackerTrk()->Eta()
                 << " " << mu->TrackerTrk()->Phi();
          }
          cout << endl;
          cout << "TrackIso: " << mu->IsoR03SumPt() << " , EcalIso: " << mu->IsoR03EmEt() 
               << " , HadIso: " << mu->IsoR03HadEt()
               << endl;
          cout << endl;
          
          //print out gen leptons:
          for (UInt_t d=0; d<GenLeptonsAndTaus->GetEntries(); d++) {
            cout << "GenLeptonsAndTaus: " << d << " : " << GenLeptonsAndTaus->At(d)->PdgId() << " " 
                 <<GenLeptonsAndTaus->At(d)->Pt() 
                 << " " <<  GenLeptonsAndTaus->At(d)->Eta()  << " " 
                 <<  GenLeptonsAndTaus->At(d)->Phi();
            cout << endl;
          }
          
          //print out genjets
          for (UInt_t d=0; d<fGenJets->GetEntries(); d++) {    
            if (fGenJets->At(d)->Pt() > 10.0) {
              cout << "GenJets: " << d << " : " << fGenJets->At(d)->Pt() << " " 
                   << fGenJets->At(d)->Eta() << " " << fGenJets->At(d)->Phi()  
                   << " | flavor" << fGenJets->At(d)->MatchedMCFlavor() << endl;
            }
          }          
        }

       //***********************************************************************************************
        //GlobalTrack to TrackerTrack Charge Matching
        //***********************************************************************************************
        int ChargeMatchCode = 0;
        if (mu->GlobalTrk() && mu->TrackerTrk()) {        
          if (mu->GlobalTrk()->Charge() + mu->TrackerTrk()->Charge() == 0) {
            ChargeMatchCode = 2*mu->GlobalTrk()->Charge() + mu->TrackerTrk()->Charge();
          } else {
            ChargeMatchCode = mu->GlobalTrk()->Charge() + mu->TrackerTrk()->Charge();
          }
        }
        if (mu->Eta() < 1.479) {
          fCleanCentralMuonTrackerGlobalTrackChargeMatch->Fill(ChargeMatchCode);
        } else {
          fCleanForwardMuonTrackerGlobalTrackChargeMatch->Fill(ChargeMatchCode);
        }

        //***********************************************************************************************
        //Match Clean Muon to a GenJet
        //***********************************************************************************************  
        //match the clean muon to a genJet.
        //try to match it to a genjet
        Double_t minDR = 5000.0;
        const GenJet *matchedGenJet = NULL;
        for(UInt_t j=0;j<fGenJets->GetEntries();j++) {
          Double_t DR = MathUtils::DeltaR(mu->Phi(), mu->Eta(), fGenJets->At(j)->Phi(),
                                          fGenJets->At(j)->Eta());
          if (DR < minDR && DR < 0.3) {
            minDR = DR;
            matchedGenJet = fGenJets->At(j);
          }
        }
        //if it matched
        if (mu->Eta() < 1.479) {
          if (matchedGenJet) {
            if (mu->Charge() > 0) {
              fCleanPlusCentralMuonMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
              fCleanPlusCentralMuonMatchedGenJetPt->Fill(matchedGenJet->Pt());
              fCleanPlusCentralMuonMatchedGenJetDeltaR->Fill(minDR);
              fCleanPlusCentralMuonMatchedGenJetDeltaPtOverPt->Fill((mu->Pt() - matchedGenJet->Pt())/mu->Pt());
            } else {
              fCleanMinusCentralMuonMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
              fCleanMinusCentralMuonMatchedGenJetPt->Fill(matchedGenJet->Pt());
              fCleanMinusCentralMuonMatchedGenJetDeltaR->Fill(minDR);
              fCleanMinusCentralMuonMatchedGenJetDeltaPtOverPt->Fill((mu->Pt() - matchedGenJet->Pt())/mu->Pt());
            }
          } else {
            if (mu->Charge() > 0) {
              fCleanPlusCentralMuonMatchedJetFlavor->Fill(-9);
            } else {
              fCleanMinusCentralMuonMatchedJetFlavor->Fill(-9);
            }
          }
        } else {
          if (matchedGenJet) {
            if (mu->Charge() > 0) {
              fCleanPlusForwardMuonMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
              fCleanPlusForwardMuonMatchedGenJetPt->Fill(matchedGenJet->Pt());
              fCleanPlusForwardMuonMatchedGenJetDeltaR->Fill(minDR);
              fCleanPlusForwardMuonMatchedGenJetDeltaPtOverPt->Fill((mu->Pt() - matchedGenJet->Pt())/mu->Pt());
            } else {
              fCleanMinusForwardMuonMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
              fCleanMinusForwardMuonMatchedGenJetPt->Fill(matchedGenJet->Pt());
              fCleanMinusForwardMuonMatchedGenJetDeltaR->Fill(minDR);
              fCleanMinusForwardMuonMatchedGenJetDeltaPtOverPt->Fill((mu->Pt() - matchedGenJet->Pt())/mu->Pt());
            }
          } else {
            if (mu->Charge() > 0) {
              fCleanPlusForwardMuonMatchedJetFlavor->Fill(-9);
            } else {
              fCleanMinusForwardMuonMatchedJetFlavor->Fill(-9);
            }
          }
        }
        
        //***********************************************************************************************
        //Match Clean Muon to Leading GenParticle (not parton)
        //***********************************************************************************************  
        Double_t highestPt = 0.0;
        Double_t matchedDR = 5000.0;
        const MCParticle *matchedGenParticle = NULL;
        for(UInt_t j=0;j<fParticles->GetEntries();j++) {
          Double_t DR = MathUtils::DeltaR(mu->Phi(), mu->Eta(), fParticles->At(j)->Phi(),
                                          fParticles->At(j)->Eta());
          if (fParticles->At(j)->Pt() > highestPt && DR < 0.3 && !fParticles->At(j)->IsParton() 
              && fParticles->At(j)->PdgId() != 92) {
            highestPt = fParticles->At(j)->Pt();
            matchedGenParticle = fParticles->At(j);
            matchedDR = DR;
          }
        }
        
        if (mu->Eta() < 1.479) {
          if (matchedGenParticle && matchedGenParticle->Pt()>5.0) {
            if (mu->Charge() > 0) {
              fCleanPlusCentralMuonMatchedGenParticleID->Fill(matchedGenParticle->PdgId());
              fCleanPlusCentralMuonMatchedGenParticlePt->Fill(matchedGenParticle->Pt());
              fCleanPlusCentralMuonMatchedGenParticleDeltaR->Fill(matchedDR);
              fCleanPlusCentralMuonMatchedGenParticleDeltaPtOverPt->Fill((mu->Pt() - matchedGenParticle->Pt())/mu->Pt());
            } else {
              fCleanMinusCentralMuonMatchedGenParticleID->Fill(matchedGenParticle->PdgId());
              fCleanMinusCentralMuonMatchedGenParticlePt->Fill(matchedGenParticle->Pt());
              fCleanMinusCentralMuonMatchedGenParticleDeltaR->Fill(matchedDR);
              fCleanMinusCentralMuonMatchedGenParticleDeltaPtOverPt->Fill((mu->Pt() - matchedGenParticle->Pt())/mu->Pt());
            }
          } else {
            if (mu->Charge() > 0) {
              fCleanPlusCentralMuonMatchedGenParticleID->Fill(0);
            } else {
              fCleanMinusCentralMuonMatchedGenParticleID->Fill(0);
            }
          }
        } else {
          if (matchedGenParticle && matchedGenParticle->Pt()>5.0) {
            if (mu->Charge() > 0) {
              fCleanPlusForwardMuonMatchedGenParticleID->Fill(matchedGenParticle->PdgId());
              fCleanPlusForwardMuonMatchedGenParticlePt->Fill(matchedGenParticle->Pt());
              fCleanPlusForwardMuonMatchedGenParticleDeltaR->Fill(matchedDR);
              fCleanPlusForwardMuonMatchedGenParticleDeltaPtOverPt->Fill((mu->Pt() - matchedGenParticle->Pt())/mu->Pt());
            } else {
              fCleanMinusForwardMuonMatchedGenParticleID->Fill(matchedGenParticle->PdgId());
              fCleanMinusForwardMuonMatchedGenParticlePt->Fill(matchedGenParticle->Pt());
              fCleanMinusForwardMuonMatchedGenParticleDeltaR->Fill(matchedDR);
              fCleanMinusForwardMuonMatchedGenParticleDeltaPtOverPt->Fill((mu->Pt() - matchedGenParticle->Pt())/mu->Pt());
            }
          } else {
            if (mu->Charge() > 0) {
              fCleanPlusForwardMuonMatchedGenParticleID->Fill(0);
            } else {
              fCleanMinusForwardMuonMatchedGenParticleID->Fill(0);
            }
          }
        }
       
        //***********************************************************************************************
        //Match Clean Muon to a SimTrack and then trace back up the mother chain.
        //***********************************************************************************************  
        Double_t bestDeltaPtOverPt = 0.0;
        Double_t matchDeltaPtOverPt = 0.0;
        minDR = 5000.0;
        const MCParticle *matchedSimChargedParticle = NULL;
        if(fPrintLogs) cout << "\nSimulated particles near Muon\n";

        for(UInt_t j=0;j<fParticles->GetEntries();j++) {
          if (fParticles->At(j)->IsSimulated()) {
            Double_t DR = MathUtils::DeltaR(mu->Phi(), mu->Eta(), fParticles->At(j)->Phi(),
                                            fParticles->At(j)->Eta());
            
            Double_t dPtOverPt = (mu->Pt() - fParticles->At(j)->Pt())/mu->Pt();
            if (mu->TrackerTrk())
              dPtOverPt = (mu->TrackerTrk()->Pt() - fParticles->At(j)->Pt())/mu->TrackerTrk()->Pt();

            //remove Neutrals
            if (!fParticles->At(j)->IsNeutral()) { 
              if(fPrintLogs && fabs(fabs(dPtOverPt)) < 0.5 ) { 
                          
                cout << "ChargedSimulatedParticle: " << j << " : " << fabs(fabs(dPtOverPt)) << " : " << DR 
                     << "   " << fParticles->At(j)->PdgId()  
                     << " " << fParticles->At(j)->Status() << " " << fParticles->At(j)->Pt()
                     << " " << fParticles->At(j)->Eta() << " " << fParticles->At(j)->Phi()
                     << endl;
              }
              if (DR < minDR && fabs(fabs(dPtOverPt)) < 0.5  ) {
                minDR = DR;
                matchedSimChargedParticle = fParticles->At(j); 
                matchDeltaPtOverPt = dPtOverPt;
              }   

              if (fabs(fabs(dPtOverPt)-1) < fabs(fabs(bestDeltaPtOverPt)) && DR < 0.3)
                bestDeltaPtOverPt = dPtOverPt;
           
            }
          }
        }

        if (matchedSimChargedParticle && minDR < 0.3) {

          if (fPrintLogs) {
            cout << "Matched SimParticle: " << matchedSimChargedParticle->PdgId() << " " 
                 << matchedSimChargedParticle->Status() << " " << matchedSimChargedParticle->Pt() 
                 << " " << matchedSimChargedParticle->Eta() << " " << matchedSimChargedParticle->Phi()
                 << endl;
          }
          
          //***********************************************************************************************
          //Make Histograms Using the Ancestor of the SimParticle matched to the fake muon
          //***********************************************************************************************  
          //Use matched SimTrack to trace back up the mother chain.
          const MCParticle *tmpParticle = matchedSimChargedParticle;
          while (tmpParticle->Mother() && tmpParticle->Mother()->PdgId() != 92 && 
                 !tmpParticle->Mother()->IsParton()) {
            if (fPrintLogs) {
              cout << "mother : " << tmpParticle->Mother()->PdgId() << " " << tmpParticle->Mother()->Status() 
                   << " " << tmpParticle->Mother()->Pt() << " " << tmpParticle->Mother()->Eta() << " " 
                   << tmpParticle->Mother()->Phi() << endl;
            }
            tmpParticle = tmpParticle->Mother();
          }
          const MCParticle *matchedAncestor = tmpParticle;
          if (matchedAncestor) {
            if (fPrintLogs) {
              cout << "Ancestor: " << matchedAncestor->PdgId() << " " 
                   << matchedAncestor->Status() << " " << matchedAncestor->Pt()
                   << " " << matchedAncestor->Eta() << " " 
                   << matchedAncestor->Phi()
                   << endl;
            }

            Double_t ancestorDR = MathUtils::DeltaR(mu->Phi(), mu->Eta(), matchedAncestor->Phi(),
                                                    matchedAncestor->Eta());
            Double_t ancestorDeltaPtOverPt = (mu->Pt() - matchedAncestor->Pt())/mu->Pt();
            if (mu->Eta() < 1.479) {
              if (mu->Charge() > 0) {
                fCleanPlusCentralMuonMatchedAncestorParticleID->Fill(matchedAncestor->PdgId());
                fCleanPlusCentralMuonMatchedAncestorParticlePt->Fill(matchedAncestor->Pt());
                fCleanPlusCentralMuonMatchedAncestorParticleDeltaR->Fill(ancestorDR);
                fCleanPlusCentralMuonMatchedAncestorParticleDeltaPtOverPt->Fill(ancestorDeltaPtOverPt);
              } else {
                fCleanMinusCentralMuonMatchedAncestorParticleID->Fill(matchedAncestor->PdgId());
                fCleanMinusCentralMuonMatchedAncestorParticlePt->Fill(matchedAncestor->Pt());
                fCleanMinusCentralMuonMatchedAncestorParticleDeltaR->Fill(ancestorDR);
                fCleanMinusCentralMuonMatchedAncestorParticleDeltaPtOverPt->Fill(ancestorDeltaPtOverPt);
              }
            } else {
              if (mu->Charge() > 0) {
                fCleanPlusForwardMuonMatchedAncestorParticleID->Fill(matchedAncestor->PdgId());
                fCleanPlusForwardMuonMatchedAncestorParticlePt->Fill(matchedAncestor->Pt());
                fCleanPlusForwardMuonMatchedAncestorParticleDeltaR->Fill(ancestorDR);
                fCleanPlusForwardMuonMatchedAncestorParticleDeltaPtOverPt->Fill(ancestorDeltaPtOverPt);
              } else {
                fCleanMinusForwardMuonMatchedAncestorParticleID->Fill(matchedAncestor->PdgId());
                fCleanMinusForwardMuonMatchedAncestorParticlePt->Fill(matchedAncestor->Pt());
                fCleanMinusForwardMuonMatchedAncestorParticleDeltaR->Fill(ancestorDR);
                fCleanMinusForwardMuonMatchedAncestorParticleDeltaPtOverPt->Fill(ancestorDeltaPtOverPt);
              }
            }                        
          } //if we found the ancestor

          //***********************************************************************************************
          //Make Histograms Using the SimParticle matched to the fake muon
          //***********************************************************************************************  
          if (mu->Eta() < 1.479) {
            if (mu->Charge() > 0) {
              fCleanPlusCentralMuonMatchedSimChargedParticleID->Fill(matchedSimChargedParticle->PdgId());
              fCleanPlusCentralMuonMatchedSimChargedParticlePt->Fill(matchedSimChargedParticle->Pt());
              fCleanPlusCentralMuonMatchedSimChargedParticleDeltaR->Fill(minDR);
              fCleanPlusCentralMuonMatchedSimChargedParticleDeltaPtOverPt->Fill(matchDeltaPtOverPt);
            } else {
              fCleanMinusCentralMuonMatchedSimChargedParticleID->Fill(matchedSimChargedParticle->PdgId());
              fCleanMinusCentralMuonMatchedSimChargedParticlePt->Fill(matchedSimChargedParticle->Pt());
              fCleanMinusCentralMuonMatchedSimChargedParticleDeltaR->Fill(minDR);
              fCleanMinusCentralMuonMatchedSimChargedParticleDeltaPtOverPt->Fill(matchDeltaPtOverPt);
            }
          } else {
            if (mu->Charge() > 0) {
              fCleanPlusForwardMuonMatchedSimChargedParticleID->Fill(matchedSimChargedParticle->PdgId());
              fCleanPlusForwardMuonMatchedSimChargedParticlePt->Fill(matchedSimChargedParticle->Pt());
              fCleanPlusForwardMuonMatchedSimChargedParticleDeltaR->Fill(minDR);
              fCleanPlusForwardMuonMatchedSimChargedParticleDeltaPtOverPt->Fill(matchDeltaPtOverPt);
            } else {
              fCleanMinusForwardMuonMatchedSimChargedParticleID->Fill(matchedSimChargedParticle->PdgId());
              fCleanMinusForwardMuonMatchedSimChargedParticlePt->Fill(matchedSimChargedParticle->Pt());
              fCleanMinusForwardMuonMatchedSimChargedParticleDeltaR->Fill(minDR);
              fCleanMinusForwardMuonMatchedSimChargedParticleDeltaPtOverPt->Fill(matchDeltaPtOverPt);
            }
          }
        } else {       
        //histogram the bestDeltaPtOverPt inside 0.3 cone for ones which didn't pass
          if (mu->Eta() < 1.479) {
            if (mu->Charge() > 0) {
              fCleanPlusCentralMuonUnmatchedSimChargedParticleBestDeltaPtOverPt->Fill(bestDeltaPtOverPt);
            } else {
              fCleanMinusCentralMuonUnmatchedSimChargedParticleBestDeltaPtOverPt->Fill(bestDeltaPtOverPt);
            }
          } else {
            if (mu->Charge() > 0) {
              fCleanPlusForwardMuonUnmatchedSimChargedParticleBestDeltaPtOverPt->Fill(bestDeltaPtOverPt);
            } else {
              fCleanMinusForwardMuonUnmatchedSimChargedParticleBestDeltaPtOverPt->Fill(bestDeltaPtOverPt);
            }
          }
        }

        //Print out HepMC Table
        if(fPrintLogs) {                            
          //print out HepMC table
          GeneratorTools::PrintHepMCTable(fParticles, true, -1);
          
          //print out all particles and all descendants that are within dR of 0.3 of the muon
          //GeneratorTools::PrintNearbyParticles(fParticles, mu->Eta(), mu->Phi(), 0.3);
        }

      }
    } //loop over RECO muons.   
  } //if njet == 1, nlepton == 1
}

//--------------------------------------------------------------------------------------------------
void FakeMuonAnalysisMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.

}

//--------------------------------------------------------------------------------------------------
void FakeMuonAnalysisMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
