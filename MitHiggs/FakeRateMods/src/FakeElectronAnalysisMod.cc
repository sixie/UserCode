// $Id: FakeElectronAnalysisMod.cc,v 1.2 2009/03/17 08:33:15 sixie Exp $

#include "MitHiggs/FakeRateMods/interface/FakeElectronAnalysisMod.h"
#include "MitAna/DataUtil/interface/Debug.h"
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitPhysics/Utils/interface/IsolationTools.h"
#include "MitPhysics/Utils/interface/GeneratorTools.h"
#include <map>

using namespace mithep;

ClassImp(mithep::FakeElectronAnalysisMod)

//--------------------------------------------------------------------------------------------------
  FakeElectronAnalysisMod::FakeElectronAnalysisMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fVetoGeneratorLeptons(true),
  fRequireFirstLepton(false),
  fPrintLogs(true),
  fMCPartBranchName(Names::gkMCPartBrn),
  fVertexBranchName(Names::gkPVBeamSpotBrn),
  fGenJetBranchName(Names::gkSC5GenJetBrn),
  fTrackBranchName(Names::gkTrackBrn),
  fGsfTrackBranchName(Names::gkGsfTrackBrn),
  fElectronBranchName(Names::gkElectronBrn),
  fJetBranchName(Names::gkCaloJetBrn),
  fBarrelSuperClusterBranchName(Names::gkBarrelSuperClusterBrn),
  fEndcapSuperClusterBranchName(Names::gkEndcapSuperClusterBrn),
  fCleanElectronsName(ModNames::gkCleanElectronsName),
  fGoodJetsName(ModNames::gkGoodJetsName),
  fMCLeptonsName(ModNames::gkMCLeptonsName),
  fMCTausName(ModNames::gkMCTausName),
  fParticles(0),
  fVertices(0),
  fBarrelSuperClusters(0),
  fEndcapSuperClusters(0),
  fTracks(0),
  fGsfTracks(0),
  fElectrons(0),
  fJets(0),
  count(0)
{ 
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void FakeElectronAnalysisMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void FakeElectronAnalysisMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fMCPartBranchName,                fParticles);
  ReqBranch(fVertexBranchName,                fVertices);
  ReqBranch(fGenJetBranchName,                fGenJets);
  ReqBranch(fTrackBranchName,                 fTracks);
  ReqBranch(fGsfTrackBranchName,              fGsfTracks);
  ReqBranch(fElectronBranchName,              fElectrons);
  ReqBranch(fJetBranchName,                   fJets);
  ReqBranch(fBarrelSuperClusterBranchName,    fBarrelSuperClusters);
  ReqBranch(fEndcapSuperClusterBranchName,    fEndcapSuperClusters);


  //*******************************************************************************************
  //General Event Level Histograms
  //*******************************************************************************************
  AddTH1(fNGenJets, "hNGenJets", ";NGenJets;Number of Events", 10, -0.5,9.5);
  AddTH1(fNCleanLeptons, "hNCleanLeptons", ";NCleanLeptons;Number of Events", 10, -0.5,9.5);
  AddTH1(fNGenLeptons, "hNGenLeptons", ";NGenLeptons;Number of Events", 10, -0.5,9.5);


  //*******************************************************************************************
  //*******************************************************************************************
  //General Fake Electron Studies
  //*******************************************************************************************
  //*******************************************************************************************

  //***********************************************************************************************
  //Histograms for GsfTrack Charge Vs TrackerTrack Charge
  //***********************************************************************************************  

  AddTH1(fCleanBarrelElectronTrackerGsfTrackChargeMatch, 
         "hCleanBarrelElectronTrackerGsfTrackChargeMatch", 
         ";CleanBarrelElectronTrackerGsfTrackChargeMatch;Number of Events", 5, -2.5,2.5);
  AddTH1(fCleanEndcapElectronTrackerGsfTrackChargeMatch, 
         "hCleanEndcapElectronTrackerGsfTrackChargeMatch", 
         ";CleanEndcapElectronTrackerGsfTrackChargeMatch;Number of Events", 5, -2.5,2.5);

  //***********************************************************************************************
  //Histograms for Fake Electron to GenJet Matching
  //***********************************************************************************************  

  AddTH1(fCleanPlusBarrelElectronMatchedJetFlavor, "hCleanPlusBarrelElectronMatchedJetFlavor", 
         ";CleanPlusBarrelElectronMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fCleanMinusBarrelElectronMatchedJetFlavor, "hCleanMinusBarrelElectronMatchedJetFlavor", 
         ";CleanMinusBarrelElectronMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fCleanPlusBarrelElectronMatchedGenJetPt, "hCleanPlusBarrelElectronMatchedGenJetPt", 
         ";CleanPlusBarrelElectronMatchedGenJetPt;Number of Events", 100, 0,100);
  AddTH1(fCleanMinusBarrelElectronMatchedGenJetPt, "hCleanMinusBarrelElectronMatchedGenJetPt", 
         ";CleanMinusBarrelElectronMatchedGenJetPt;Number of Events", 100, 0,100);
  AddTH1(fCleanPlusBarrelElectronMatchedGenJetDeltaR, "hCleanPlusBarrelElectronMatchedGenJetDeltaR", 
         ";CleanPlusBarrelElectronMatchedGenJetDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanMinusBarrelElectronMatchedGenJetDeltaR, "hCleanMinusBarrelElectronMatchedGenJetDeltaR", 
         ";CleanMinusBarrelElectronMatchedGenJetDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanPlusBarrelElectronMatchedGenJetDeltaPtOverPt, "hCleanPlusBarrelElectronMatchedGenJetDeltaPtOverPt", 
         ";CleanPlusBarrelElectronMatchedGenJetDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusBarrelElectronMatchedGenJetDeltaPtOverPt, "hCleanMinusBarrelElectronMatchedGenJetDeltaPtOverPt", 
         ";CleanMinusBarrelElectronMatchedGenJetDeltaPtOverPt;Number of Events", 100, -2.0,1.0);

  AddTH1(fCleanPlusEndcapElectronMatchedJetFlavor, "hCleanPlusEndcapElectronMatchedJetFlavor", 
         ";CleanPlusEndcapElectronMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fCleanMinusEndcapElectronMatchedJetFlavor, "hCleanMinusEndcapElectronMatchedJetFlavor", 
         ";CleanMinusEndcapElectronMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fCleanPlusEndcapElectronMatchedGenJetPt, "hCleanPlusEndcapElectronMatchedGenJetPt", 
         ";CleanPlusEndcapElectronMatchedGenJetPt;Number of Events", 100, 0,100);
  AddTH1(fCleanMinusEndcapElectronMatchedGenJetPt, "hCleanMinusEndcapElectronMatchedGenJetPt", 
         ";CleanMinusEndcapElectronMatchedGenJetPt;Number of Events", 100, 0,100);
  AddTH1(fCleanPlusEndcapElectronMatchedGenJetDeltaR, "hCleanPlusEndcapElectronMatchedGenJetDeltaR", 
         ";CleanPlusEndcapElectronMatchedGenJetDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanMinusEndcapElectronMatchedGenJetDeltaR, "hCleanMinusEndcapElectronMatchedGenJetDeltaR", 
         ";CleanMinusEndcapElectronMatchedGenJetDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanPlusEndcapElectronMatchedGenJetDeltaPtOverPt, "hCleanPlusEndcapElectronMatchedGenJetDeltaPtOverPt", 
         ";CleanPlusEndcapElectronMatchedGenJetDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusEndcapElectronMatchedGenJetDeltaPtOverPt, "hCleanMinusEndcapElectronMatchedGenJetDeltaPtOverPt", 
         ";CleanMinusEndcapElectronMatchedGenJetDeltaPtOverPt;Number of Events", 100, -2.0,1.0);

  //***********************************************************************************************
  //Histograms for Fake Electron to GenParticle Matching
  //***********************************************************************************************  
  AddTH1(fCleanPlusBarrelElectronMatchedGenParticleID, "hCleanPlusBarrelElectronMatchedGenParticleID", 
         ";CleanPlusBarrelElectronMatchedGenParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanMinusBarrelElectronMatchedGenParticleID, "hCleanMinusBarrelElectronMatchedGenParticleID", 
         ";CleanMinusBarrelElectronMatchedGenParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanPlusBarrelElectronMatchedGenParticlePt, "hCleanPlusBarrelElectronMatchedGenParticlePt", 
         ";CleanPlusBarrelElectronMatchedGenParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanMinusBarrelElectronMatchedGenParticlePt, "hCleanMinusBarrelElectronMatchedGenParticlePt", 
         ";CleanMinusBarrelElectronMatchedGenParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanPlusBarrelElectronMatchedGenParticleDeltaR, "hCleanPlusBarrelElectronMatchedGenParticleDeltaR", 
         ";CleanPlusBarrelElectronMatchedGenParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanMinusBarrelElectronMatchedGenParticleDeltaR, "hCleanMinusBarrelElectronMatchedGenParticleDeltaR", 
         ";CleanMinusBarrelElectronMatchedGenParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanPlusBarrelElectronMatchedGenParticleDeltaPtOverPt, "hCleanPlusBarrelElectronMatchedGenParticleDeltaPtOverPt", 
         ";CleanPlusBarrelElectronMatchedGenParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusBarrelElectronMatchedGenParticleDeltaPtOverPt, "hCleanMinusBarrelElectronMatchedGenParticleDeltaPtOverPt", 
         ";CleanMinusBarrelElectronMatchedGenParticleDeltaPtOverPt;Number of Events",  100, -2.0,1.0);

  AddTH1(fCleanPlusEndcapElectronMatchedGenParticleID, "hCleanPlusEndcapElectronMatchedGenParticleID", 
         ";CleanPlusEndcapElectronMatchedGenParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanMinusEndcapElectronMatchedGenParticleID, "hCleanMinusEndcapElectronMatchedGenParticleID", 
         ";CleanMinusEndcapElectronMatchedGenParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanPlusEndcapElectronMatchedGenParticlePt, "hCleanPlusEndcapElectronMatchedGenParticlePt", 
         ";CleanPlusEndcapElectronMatchedGenParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanMinusEndcapElectronMatchedGenParticlePt, "hCleanMinusEndcapElectronMatchedGenParticlePt", 
         ";CleanMinusEndcapElectronMatchedGenParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanPlusEndcapElectronMatchedGenParticleDeltaR, "hCleanPlusEndcapElectronMatchedGenParticleDeltaR", 
         ";CleanPlusEndcapElectronMatchedGenParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanMinusEndcapElectronMatchedGenParticleDeltaR, "hCleanMinusEndcapElectronMatchedGenParticleDeltaR", 
         ";CleanMinusEndcapElectronMatchedGenParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanPlusEndcapElectronMatchedGenParticleDeltaPtOverPt, "hCleanPlusEndcapElectronMatchedGenParticleDeltaPtOverPt", 
         ";CleanPlusEndcapElectronMatchedGenParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusEndcapElectronMatchedGenParticleDeltaPtOverPt, "hCleanMinusEndcapElectronMatchedGenParticleDeltaPtOverPt", 
         ";CleanMinusEndcapElectronMatchedGenParticleDeltaPtOverPt;Number of Events",  100, -2.0,1.0);

  //***********************************************************************************************
  //Histograms for Fake Electron to SimTrack Matching
  //***********************************************************************************************  
  AddTH1(fCleanPlusBarrelElectronMatchedSimChargedParticleID, 
         "hCleanPlusBarrelElectronMatchedSimChargedParticleID", 
         ";CleanPlusBarrelElectronMatchedSimChargedParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanMinusBarrelElectronMatchedSimChargedParticleID, 
         "hCleanMinusBarrelElectronMatchedSimChargedParticleID", 
         ";CleanMinusBarrelElectronMatchedSimChargedParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanPlusBarrelElectronMatchedSimChargedParticlePt, 
         "hCleanPlusBarrelElectronMatchedSimChargedParticlePt", 
         ";CleanPlusBarrelElectronMatchedSimChargedParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanMinusBarrelElectronMatchedSimChargedParticlePt, 
         "hCleanMinusBarrelElectronMatchedSimChargedParticlePt", 
         ";CleanMinusBarrelElectronMatchedSimChargedParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanPlusBarrelElectronMatchedSimChargedParticleDeltaR, 
         "hCleanPlusBarrelElectronMatchedSimChargedParticleDeltaR", 
         ";CleanPlusBarrelElectronMatchedSimChargedParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanMinusBarrelElectronMatchedSimChargedParticleDeltaR, 
         "hCleanMinusBarrelElectronMatchedSimChargedParticleDeltaR", 
         ";CleanMinusBarrelElectronMatchedSimChargedParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanPlusBarrelElectronMatchedSimChargedParticleDeltaPtOverPt, 
         "hCleanPlusBarrelElectronMatchedSimChargedParticleDeltaPtOverPt", 
         ";CleanPlusBarrelElectronMatchedSimChargedParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusBarrelElectronMatchedSimChargedParticleDeltaPtOverPt, 
         "hCleanMinusBarrelElectronMatchedSimChargedParticleDeltaPtOverPt", 
         ";CleanMinusBarrelElectronMatchedSimChargedParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);

  AddTH1(fCleanPlusEndcapElectronMatchedSimChargedParticleID, 
         "hCleanPlusEndcapElectronMatchedSimChargedParticleID", 
         ";CleanPlusEndcapElectronMatchedSimChargedParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanMinusEndcapElectronMatchedSimChargedParticleID, 
         "hCleanMinusEndcapElectronMatchedSimChargedParticleID", 
         ";CleanMinusEndcapElectronMatchedSimChargedParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanPlusEndcapElectronMatchedSimChargedParticlePt, 
         "hCleanPlusEndcapElectronMatchedSimChargedParticlePt", 
         ";CleanPlusEndcapElectronMatchedSimChargedParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanMinusEndcapElectronMatchedSimChargedParticlePt, 
         "hCleanMinusEndcapElectronMatchedSimChargedParticlePt", 
         ";CleanMinusEndcapElectronMatchedSimChargedParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanPlusEndcapElectronMatchedSimChargedParticleDeltaR, 
         "hCleanPlusEndcapElectronMatchedSimChargedParticleDeltaR", 
         ";CleanPlusEndcapElectronMatchedSimChargedParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanMinusEndcapElectronMatchedSimChargedParticleDeltaR, 
         "hCleanMinusEndcapElectronMatchedSimChargedParticleDeltaR", 
         ";CleanMinusEndcapElectronMatchedSimChargedParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanPlusEndcapElectronMatchedSimChargedParticleDeltaPtOverPt, 
         "hCleanPlusEndcapElectronMatchedSimChargedParticleDeltaPtOverPt", 
         ";CleanPlusEndcapElectronMatchedSimChargedParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusEndcapElectronMatchedSimChargedParticleDeltaPtOverPt, 
         "hCleanMinusEndcapElectronMatchedSimChargedParticleDeltaPtOverPt", 
         ";CleanMinusEndcapElectronMatchedSimChargedParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);

  //***********************************************************************************************
  //Histograms for Fake Electron to SimTrack Ancestor Matching
  //***********************************************************************************************  
  AddTH1(fCleanPlusBarrelElectronMatchedAncestorParticleID, 
         "hCleanPlusBarrelElectronMatchedAncestorParticleID", 
         ";CleanPlusBarrelElectronMatchedAncestorParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanMinusBarrelElectronMatchedAncestorParticleID, 
         "hCleanMinusBarrelElectronMatchedAncestorParticleID", 
         ";CleanMinusBarrelElectronMatchedAncestorParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanPlusBarrelElectronMatchedAncestorParticlePt, 
         "hCleanPlusBarrelElectronMatchedAncestorParticlePt", 
         ";CleanPlusBarrelElectronMatchedAncestorParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanMinusBarrelElectronMatchedAncestorParticlePt, 
         "hCleanMinusBarrelElectronMatchedAncestorParticlePt", 
         ";CleanMinusBarrelElectronMatchedAncestorParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanPlusBarrelElectronMatchedAncestorParticleDeltaR, 
         "hCleanPlusBarrelElectronMatchedAncestorParticleDeltaR", 
         ";CleanPlusBarrelElectronMatchedAncestorParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanMinusBarrelElectronMatchedAncestorParticleDeltaR, 
         "hCleanMinusBarrelElectronMatchedAncestorParticleDeltaR", 
         ";CleanMinusBarrelElectronMatchedAncestorParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanPlusBarrelElectronMatchedAncestorParticleDeltaPtOverPt, 
         "hCleanPlusBarrelElectronMatchedAncestorParticleDeltaPtOverPt", 
         ";CleanPlusBarrelElectronMatchedAncestorParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusBarrelElectronMatchedAncestorParticleDeltaPtOverPt, 
         "hCleanMinusBarrelElectronMatchedAncestorParticleDeltaPtOverPt", 
         ";CleanMinusBarrelElectronMatchedAncestorParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);

  AddTH1(fCleanPlusEndcapElectronMatchedAncestorParticleID, 
         "hCleanPlusEndcapElectronMatchedAncestorParticleID", 
         ";CleanPlusEndcapElectronMatchedAncestorParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanMinusEndcapElectronMatchedAncestorParticleID, 
         "hCleanMinusEndcapElectronMatchedAncestorParticleID", 
         ";CleanMinusEndcapElectronMatchedAncestorParticleID;Number of Events", 20001, -10000.5,10000.5);
  AddTH1(fCleanPlusEndcapElectronMatchedAncestorParticlePt, 
         "hCleanPlusEndcapElectronMatchedAncestorParticlePt", 
         ";CleanPlusEndcapElectronMatchedAncestorParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanMinusEndcapElectronMatchedAncestorParticlePt, 
         "hCleanMinusEndcapElectronMatchedAncestorParticlePt", 
         ";CleanMinusEndcapElectronMatchedAncestorParticlePt;Number of Events", 100, 0,100);
  AddTH1(fCleanPlusEndcapElectronMatchedAncestorParticleDeltaR, 
         "hCleanPlusEndcapElectronMatchedAncestorParticleDeltaR", 
         ";CleanPlusEndcapElectronMatchedAncestorParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanMinusEndcapElectronMatchedAncestorParticleDeltaR, 
         "hCleanMinusEndcapElectronMatchedAncestorParticleDeltaR", 
         ";CleanMinusEndcapElectronMatchedAncestorParticleDeltaR;Number of Events", 100, 0,0.3);
  AddTH1(fCleanPlusEndcapElectronMatchedAncestorParticleDeltaPtOverPt, 
         "hCleanPlusEndcapElectronMatchedAncestorParticleDeltaPtOverPt", 
         ";CleanPlusEndcapElectronMatchedAncestorParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusEndcapElectronMatchedAncestorParticleDeltaPtOverPt, 
         "hCleanMinusEndcapElectronMatchedAncestorParticleDeltaPtOverPt", 
         ";CleanMinusEndcapElectronMatchedAncestorParticleDeltaPtOverPt;Number of Events", 100, -2.0,1.0);



  //***********************************************************************************************
  //Histograms for Fake Electron to SimTrack Ancestor Matching
  //***********************************************************************************************  
  AddTH1(fCleanPlusBarrelElectronFakeCategory, 
         "hCleanPlusBarrelElectronFakeCategory", 
         ";CleanPlusBarrelElectronFakeCategory;Number of Events", 8,-0.5,7.5);
  TAxis *xa = fCleanPlusBarrelElectronFakeCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");

  AddTH1(fCleanMinusBarrelElectronFakeCategory, 
         "hCleanMinusBarrelElectronFakeCategory", 
         ";CleanMinusBarrelElectronFakeCategory;Number of Events", 8,-0.5,7.5);
  xa = fCleanMinusBarrelElectronFakeCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");
   
  AddTH1(fCleanPlusEndcapElectronFakeCategory, 
         "hCleanPlusEndcapElectronFakeCategory", 
         ";CleanPlusEndcapElectronFakeCategory;Number of Events", 8,-0.5,7.5);
  xa = fCleanPlusEndcapElectronFakeCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");

  AddTH1(fCleanMinusEndcapElectronFakeCategory, 
         "hCleanMinusEndcapElectronFakeCategory", 
         ";CleanMinusEndcapElectronFakeCategory;Number of Events", 8,-0.5,7.5);
  xa = fCleanMinusEndcapElectronFakeCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");


  //***********************************************************************************************
  //Histograms for Fake Electron unmatched to SimTrack
  //***********************************************************************************************  
  AddTH1(fCleanPlusBarrelElectronUnmatchedSimChargedParticleBestDeltaPtOverPt, 
         "hCleanPlusBarrelElectronUnmatchedSimChargedParticleBestDeltaPtOverPt", 
         ";CleanPlusBarrelElectronUnmatchedSimChargedParticleBestDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusBarrelElectronUnmatchedSimChargedParticleBestDeltaPtOverPt, 
         "hCleanMinusBarrelElectronUnmatchedSimChargedParticleBestDeltaPtOverPt", 
         ";CleanMinusBarrelElectronUnmatchedSimChargedParticleBestDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanPlusEndcapElectronUnmatchedSimChargedParticleBestDeltaPtOverPt, 
         "hCleanPlusEndcapElectronUnmatchedSimChargedParticleBestDeltaPtOverPt", 
         ";CleanPlusEndcapElectronUnmatchedSimChargedParticleBestDeltaPtOverPt;Number of Events", 100, -2.0,1.0);
  AddTH1(fCleanMinusEndcapElectronUnmatchedSimChargedParticleBestDeltaPtOverPt, 
         "hCleanMinusEndcapElectronUnmatchedSimChargedParticleBestDeltaPtOverPt", 
         ";CleanMinusEndcapElectronUnmatchedSimChargedParticleBestDeltaPtOverPt;Number of Events", 100, -2.0,1.0);

  //***********************************************************************************************
  //Distribution of Electron Variables in different Fake Categories
  //***********************************************************************************************  
  AddTH1(fConversionFakeElectronPt, "hConversionFakeElectronPt", ";ElectronPt;Number of Events", 50, 0, 250);
  AddTH1(fConversionFakeElectronEta, "hConversionFakeElectronEta", ";ElectronEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fConversionFakeElectronPhi, "hConversionFakeElectronPhi", ";ElectronPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fConversionFakeElectronTrkIso, "hConversionFakeElectronTrkIso", ";ElectronTrkIso;Number of Events", 100, 0, 10.0);
  AddTH1(fConversionFakeElectronJurassicIso, "hConversionFakeElectronJurassicIso", ";ElectronJurassicIso;Number of Events", 100, 0, 10.0);
  AddTH1(fConversionFakeElectronHOverE, "hConversionFakeElectronHOverE", ";ElectronHOverE;Number of Events", 100, 0, 0.05);
  AddTH1(fConversionFakeElectronEOverP, "hConversionFakeElectronEOverP", ";ElectronEOverP;Number of Events", 100, 0,2.0);
  AddTH1(fConversionFakeElectronSigmaEtaEta, "hConversionFakeElectronSigmaEtaEta", ";ElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fConversionFakeElectronSigmaiEtaiEta, "hConversionFakeElectronSigmaiEtaiEta", ";ElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fConversionFakeElectronDeltaPhi, "hConversionFakeElectronDeltaPhi", ";ElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fConversionFakeElectronDeltaEta, "hConversionFakeElectronDeltaEta", ";ElectronDeltaEta;Number of Events", 100, 0,0.01);
  AddTH1(fConversionFakeElectronfBrem, "hConversionFakeElectronfBrem", ";fBrem;Number of Events", 100, 0.0,1.0);
  AddTH1(fConversionFakeElectronClassification, "hConversionFakeElectronClassification", ";Classification;Number of Events",  141, -0.5,140.5);

  AddTH1(fChargedPionFakeElectronPt, "hChargedPionFakeElectronPt", ";ElectronPt;Number of Events", 50, 0, 250);
  AddTH1(fChargedPionFakeElectronEta, "hChargedPionFakeElectronEta", ";ElectronEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fChargedPionFakeElectronPhi, "hChargedPionFakeElectronPhi", ";ElectronPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fChargedPionFakeElectronTrkIso, "hChargedPionFakeElectronTrkIso", ";ElectronTrkIso;Number of Events", 100, 0, 10.0);
  AddTH1(fChargedPionFakeElectronJurassicIso, "hChargedPionFakeElectronJurassicIso", ";ElectronJurassicIso;Number of Events", 100, 0, 10.0);
  AddTH1(fChargedPionFakeElectronHOverE, "hChargedPionFakeElectronHOverE", ";ElectronHOverE;Number of Events", 100, 0, 0.05);
  AddTH1(fChargedPionFakeElectronEOverP, "hChargedPionFakeElectronEOverP", ";ElectronEOverP;Number of Events", 100, 0,2.0);
  AddTH1(fChargedPionFakeElectronSigmaEtaEta, "hChargedPionFakeElectronSigmaEtaEta", ";ElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fChargedPionFakeElectronSigmaiEtaiEta, "hChargedPionFakeElectronSigmaiEtaiEta", ";ElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fChargedPionFakeElectronDeltaPhi, "hChargedPionFakeElectronDeltaPhi", ";ElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fChargedPionFakeElectronDeltaEta, "hChargedPionFakeElectronDeltaEta", ";ElectronDeltaEta;Number of Events", 100, 0,0.01);
  AddTH1(fChargedPionFakeElectronfBrem, "hChargedPionFakeElectronfBrem", ";fBrem;Number of Events", 100, 0.0,1.0);
  AddTH1(fChargedPionFakeElectronClassification, "hChargedPionFakeElectronClassification", ";Classification;Number of Events",  141, -0.5,140.5);

  AddTH1(fChargedKaonFakeElectronPt, "hChargedKaonFakeElectronPt", ";ElectronPt;Number of Events", 50, 0, 250);
  AddTH1(fChargedKaonFakeElectronEta, "hChargedKaonFakeElectronEta", ";ElectronEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fChargedKaonFakeElectronPhi, "hChargedKaonFakeElectronPhi", ";ElectronPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fChargedKaonFakeElectronTrkIso, "hChargedKaonFakeElectronTrkIso", ";ElectronTrkIso;Number of Events", 100, 0, 10.0);
  AddTH1(fChargedKaonFakeElectronJurassicIso, "hChargedKaonFakeElectronJurassicIso", ";ElectronJurassicIso;Number of Events", 100, 0, 10.0);
  AddTH1(fChargedKaonFakeElectronHOverE, "hChargedKaonFakeElectronHOverE", ";ElectronHOverE;Number of Events", 100, 0, 0.05);
  AddTH1(fChargedKaonFakeElectronEOverP, "hChargedKaonFakeElectronEOverP", ";ElectronEOverP;Number of Events", 100, 0,2.0);
  AddTH1(fChargedKaonFakeElectronSigmaEtaEta, "hChargedKaonFakeElectronSigmaEtaEta", ";ElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fChargedKaonFakeElectronSigmaiEtaiEta, "hChargedKaonFakeElectronSigmaiEtaiEta", ";ElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fChargedKaonFakeElectronDeltaPhi, "hChargedKaonFakeElectronDeltaPhi", ";ElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fChargedKaonFakeElectronDeltaEta, "hChargedKaonFakeElectronDeltaEta", ";ElectronDeltaEta;Number of Events", 100, 0,0.01);
  AddTH1(fChargedKaonFakeElectronfBrem, "hChargedKaonFakeElectronfBrem", ";fBrem;Number of Events", 100, 0.0,1.0);
  AddTH1(fChargedKaonFakeElectronClassification, "hChargedKaonFakeElectronClassification", ";Classification;Number of Events",  141, -0.5,140.5);

  AddTH1(fHeavyFlavorFakeElectronPt, "hHeavyFlavorFakeElectronPt", ";ElectronPt;Number of Events", 50, 0, 250);
  AddTH1(fHeavyFlavorFakeElectronEta, "hHeavyFlavorFakeElectronEta", ";ElectronEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fHeavyFlavorFakeElectronPhi, "hHeavyFlavorFakeElectronPhi", ";ElectronPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fHeavyFlavorFakeElectronTrkIso, "hHeavyFlavorFakeElectronTrkIso", ";ElectronTrkIso;Number of Events", 100, 0, 10.0);
  AddTH1(fHeavyFlavorFakeElectronJurassicIso, "hHeavyFlavorFakeElectronJurassicIso", ";ElectronJurassicIso;Number of Events", 100, 0, 10.0);
  AddTH1(fHeavyFlavorFakeElectronHOverE, "hHeavyFlavorFakeElectronHOverE", ";ElectronHOverE;Number of Events", 100, 0, 0.05);
  AddTH1(fHeavyFlavorFakeElectronEOverP, "hHeavyFlavorFakeElectronEOverP", ";ElectronEOverP;Number of Events", 100, 0,2.0);
  AddTH1(fHeavyFlavorFakeElectronSigmaEtaEta, "hHeavyFlavorFakeElectronSigmaEtaEta", ";ElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fHeavyFlavorFakeElectronSigmaiEtaiEta, "hHeavyFlavorFakeElectronSigmaiEtaiEta", ";ElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fHeavyFlavorFakeElectronDeltaPhi, "hHeavyFlavorFakeElectronDeltaPhi", ";ElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fHeavyFlavorFakeElectronDeltaEta, "hHeavyFlavorFakeElectronDeltaEta", ";ElectronDeltaEta;Number of Events", 100, 0,0.01);
  AddTH1(fHeavyFlavorFakeElectronfBrem, "hHeavyFlavorFakeElectronfBrem", ";fBrem;Number of Events", 100, 0.0,1.0);
  AddTH1(fHeavyFlavorFakeElectronClassification, "hHeavyFlavorFakeElectronClassification", ";Classification;Number of Events",  141, -0.5,140.5);

  AddTH1(fBremFakeElectronPt, "hBremFakeElectronPt", ";ElectronPt;Number of Events", 50, 0, 250);
  AddTH1(fBremFakeElectronEta, "hBremFakeElectronEta", ";ElectronEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fBremFakeElectronPhi, "hBremFakeElectronPhi", ";ElectronPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fBremFakeElectronTrkIso, "hBremFakeElectronTrkIso", ";ElectronTrkIso;Number of Events", 100, 0, 10.0);
  AddTH1(fBremFakeElectronJurassicIso, "hBremFakeElectronJurassicIso", ";ElectronJurassicIso;Number of Events", 100, 0, 10.0);
  AddTH1(fBremFakeElectronHOverE, "hBremFakeElectronHOverE", ";ElectronHOverE;Number of Events", 100, 0, 0.05);
  AddTH1(fBremFakeElectronEOverP, "hBremFakeElectronEOverP", ";ElectronEOverP;Number of Events", 100, 0,2.0);
  AddTH1(fBremFakeElectronSigmaEtaEta, "hBremFakeElectronSigmaEtaEta", ";ElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fBremFakeElectronSigmaiEtaiEta, "hBremFakeElectronSigmaiEtaiEta", ";ElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fBremFakeElectronDeltaPhi, "hBremFakeElectronDeltaPhi", ";ElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fBremFakeElectronDeltaEta, "hBremFakeElectronDeltaEta", ";ElectronDeltaEta;Number of Events", 100, 0,0.01);
  AddTH1(fBremFakeElectronfBrem, "hBremFakeElectronfBrem", ";fBrem;Number of Events", 100, 0.0,1.0);
  AddTH1(fBremFakeElectronClassification, "hBremFakeElectronClassification", ";Classification;Number of Events",  141, -0.5,140.5);

  AddTH1(fPromptPhotonFakeElectronPt, "hPromptPhotonFakeElectronPt", ";ElectronPt;Number of Events", 50, 0, 250);
  AddTH1(fPromptPhotonFakeElectronEta, "hPromptPhotonFakeElectronEta", ";ElectronEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fPromptPhotonFakeElectronPhi, "hPromptPhotonFakeElectronPhi", ";ElectronPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fPromptPhotonFakeElectronTrkIso, "hPromptPhotonFakeElectronTrkIso", ";ElectronTrkIso;Number of Events", 100, 0, 10.0);
  AddTH1(fPromptPhotonFakeElectronJurassicIso, "hPromptPhotonFakeElectronJurassicIso", ";ElectronJurassicIso;Number of Events", 100, 0, 10.0);
  AddTH1(fPromptPhotonFakeElectronHOverE, "hPromptPhotonFakeElectronHOverE", ";ElectronHOverE;Number of Events", 100, 0, 0.05);
  AddTH1(fPromptPhotonFakeElectronEOverP, "hPromptPhotonFakeElectronEOverP", ";ElectronEOverP;Number of Events", 100, 0,2.0);
  AddTH1(fPromptPhotonFakeElectronSigmaEtaEta, "hPromptPhotonFakeElectronSigmaEtaEta", ";ElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fPromptPhotonFakeElectronSigmaiEtaiEta, "hPromptPhotonFakeElectronSigmaiEtaiEta", ";ElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fPromptPhotonFakeElectronDeltaPhi, "hPromptPhotonFakeElectronDeltaPhi", ";ElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fPromptPhotonFakeElectronDeltaEta, "hPromptPhotonFakeElectronDeltaEta", ";ElectronDeltaEta;Number of Events", 100, 0,0.01);
  AddTH1(fPromptPhotonFakeElectronfBrem, "hPromptPhotonFakeElectronfBrem", ";fBrem;Number of Events", 100, 0.0,1.0);
  AddTH1(fPromptPhotonFakeElectronClassification, "hPromptPhotonFakeElectronClassification", ";Classification;Number of Events",  141, -0.5,140.5);

  AddTH1(fUncategorizedFakeElectronPt, "hUncategorizedFakeElectronPt", ";ElectronPt;Number of Events", 50, 0, 250);
  AddTH1(fUncategorizedFakeElectronEta, "hUncategorizedFakeElectronEta", ";ElectronEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fUncategorizedFakeElectronPhi, "hUncategorizedFakeElectronPhi", ";ElectronPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fUncategorizedFakeElectronTrkIso, "hUncategorizedFakeElectronTrkIso", ";ElectronTrkIso;Number of Events", 100, 0, 10.0);
  AddTH1(fUncategorizedFakeElectronJurassicIso, "hUncategorizedFakeElectronJurassicIso", ";ElectronJurassicIso;Number of Events", 100, 0, 10.0);
  AddTH1(fUncategorizedFakeElectronHOverE, "hUncategorizedFakeElectronHOverE", ";ElectronHOverE;Number of Events", 100, 0, 0.05);
  AddTH1(fUncategorizedFakeElectronEOverP, "hUncategorizedFakeElectronEOverP", ";ElectronEOverP;Number of Events", 100, 0,2.0);
  AddTH1(fUncategorizedFakeElectronSigmaEtaEta, "hUncategorizedFakeElectronSigmaEtaEta", ";ElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fUncategorizedFakeElectronSigmaiEtaiEta, "hUncategorizedFakeElectronSigmaiEtaiEta", ";ElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fUncategorizedFakeElectronDeltaPhi, "hUncategorizedFakeElectronDeltaPhi", ";ElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fUncategorizedFakeElectronDeltaEta, "hUncategorizedFakeElectronDeltaEta", ";ElectronDeltaEta;Number of Events", 100, 0,0.01);
  AddTH1(fUncategorizedFakeElectronfBrem, "hUncategorizedFakeElectronfBrem", ";fBrem;Number of Events", 100, 0.0,1.0);
  AddTH1(fUncategorizedFakeElectronClassification, "hUncategorizedFakeElectronClassification", ";Classification;Number of Events",  141, -0.5,140.5);


  //*******************************************************************************************
  //For conversion category, see how sim particle charge matches the reconstructed charge
  //*******************************************************************************************
  AddTH1(fSameSignConversionCategorySimParticleToIsolatedTrackChargeMatch, "hSameSignConversionCategorySimParticleToIsolatedTrackChargeMatch", 
         ";SameSignConversionCategorySimParticleToIsolatedTrackChargeMatch;Number of Events", 5, -2.5,2.5);
  AddTH1(fSameSignConversionCategorySimParticleToElectronChargeMatch, "hSameSignConversionCategorySimParticleToElectronChargeMatch", 
         ";SameSignConversionCategorySimParticleToElectronChargeMatch;Number of Events", 5, -2.5,2.5);
  AddTH1(fSameSignConversionCategorySimParticleToIsolatedElectronChargeMatch, "hSameSignConversionCategorySimParticleToIsolatedElectronChargeMatch", 
         ";SameSignConversionCategorySimParticleToIsolatedElectronChargeMatch;Number of Events", 5, -2.5,2.5);
  AddTH1(fSameSignConversionCategorySimParticleToIsolatedLooseElectronChargeMatch, "hSameSignConversionCategorySimParticleToIsolatedLooseElectronChargeMatch", 
         ";SameSignConversionCategorySimParticleToIsolatedLooseElectronChargeMatch;Number of Events", 5, -2.5,2.5);
  AddTH1(fSameSignConversionCategorySimParticleToIsolatedTightElectronChargeMatch, "hSameSignConversionCategorySimParticleToIsolatedTightElectronChargeMatch", 
         ";SameSignConversionCategorySimParticleToIsolatedTightElectronChargeMatch;Number of Events", 5, -2.5,2.5);
  AddTH1(fSameSignConversionCategorySimParticleToCleanElectronChargeMatch, "hSameSignConversionCategorySimParticleToCleanElectronChargeMatch", 
         ";SameSignConversionCategorySimParticleToCleanElectronChargeMatch;Number of Events", 5, -2.5,2.5);

  AddTH1(fOppositeSignConversionCategorySimParticleToIsolatedTrackChargeMatch, "hOppositeSignConversionCategorySimParticleToIsolatedTrackChargeMatch", 
         ";OppositeSignConversionCategorySimParticleToIsolatedTrackChargeMatch;Number of Events", 5, -2.5,2.5);
  AddTH1(fOppositeSignConversionCategorySimParticleToElectronChargeMatch, "hOppositeSignConversionCategorySimParticleToElectronChargeMatch", 
         ";OppositeSignConversionCategorySimParticleToElectronChargeMatch;Number of Events", 5, -2.5,2.5);
  AddTH1(fOppositeSignConversionCategorySimParticleToIsolatedElectronChargeMatch, "hOppositeSignConversionCategorySimParticleToIsolatedElectronChargeMatch", 
         ";OppositeSignConversionCategorySimParticleToIsolatedElectronChargeMatch;Number of Events", 5, -2.5,2.5);
  AddTH1(fOppositeSignConversionCategorySimParticleToIsolatedLooseElectronChargeMatch, "hOppositeSignConversionCategorySimParticleToIsolatedLooseElectronChargeMatch", 
         ";OppositeSignConversionCategorySimParticleToIsolatedLooseElectronChargeMatch;Number of Events", 5, -2.5,2.5);
  AddTH1(fOppositeSignConversionCategorySimParticleToIsolatedTightElectronChargeMatch, "hOppositeSignConversionCategorySimParticleToIsolatedTightElectronChargeMatch", 
         ";OppositeSignConversionCategorySimParticleToIsolatedTightElectronChargeMatch;Number of Events", 5, -2.5,2.5);
  AddTH1(fOppositeSignConversionCategorySimParticleToCleanElectronChargeMatch, "hOppositeSignConversionCategorySimParticleToCleanElectronChargeMatch", 
         ";OppositeSignConversionCategorySimParticleToCleanElectronChargeMatch;Number of Events", 5, -2.5,2.5);


  //*******************************************************************************************
  //*******************************************************************************************
  //Histograms for W+Jets Fake prediction
  //*******************************************************************************************
  //*******************************************************************************************

  //*******************************************************************************************
  //Charge Correlations among different fake categories
  //*******************************************************************************************
  AddTH1(fDileptonChargeConversionFakes, "hDileptonChargeConversionFakes", 
         ";DileptonChargeConversionFakes;Number of Events", 5, -2.5,2.5);
  AddTH1(fDileptonChargeChargedPionFakes, "hDileptonChargeChargedPionFakes", 
         ";DileptonChargeChargedPionFakes;Number of Events", 5, -2.5,2.5);
  AddTH1(fDileptonChargeChargedKaonFakes, "hDileptonChargeChargedKaonFakes", 
         ";DileptonChargeChargedKaonFakes;Number of Events", 5, -2.5,2.5);
  AddTH1(fDileptonChargeHeavyFlavorFakes, "hDileptonChargeHeavyFlavorFakes", 
         ";DileptonChargeChargedHeavyFlavorFakes;Number of Events", 5, -2.5,2.5);
  AddTH1(fDileptonChargeBremFakes, "hDileptonChargeBremFakes", 
         ";DileptonChargeChargedBremFakes;Number of Events", 5, -2.5,2.5);
  AddTH1(fDileptonChargePromptPhotonFakes, "hDileptonChargePromptPhotonFakes", 
         ";DileptonChargeChargedPromptPhotonFakes;Number of Events", 5, -2.5,2.5);
  AddTH1(fDileptonChargeUncategorizedFakes, "hDileptonChargeUncategorizedFakes", 
         ";DileptonChargeChargedUncategorizedFakes;Number of Events", 5, -2.5,2.5);
  AddTH1(fDileptonChargeNoSimMatchFakes, "hDileptonChargeNoSimMatchFakes", 
         ";DileptonChargeChargedNoSimMatchFakes;Number of Events", 5, -2.5,2.5);

  //*******************************************************************************************
  //W-Charge to Jet Flavor Correlations
  //*******************************************************************************************
  AddTH1(fPlusWJetFlavor, "hPlusWJetFlavor", 
         ";PlusWJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fPlusWTrackMatchedJetFlavor, "hPlusWTrackMatchedJetFlavor", 
         ";PlusWTrackMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fPlusWLooseIsolatedTrackMatchedJetFlavor, "hPlusWLooseIsolatedTrackMatchedJetFlavor", 
         ";PlusWLooseIsolatedTrackMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fPlusWTightIsolatedTrackMatchedJetFlavor, "hPlusWTightIsolatedTrackMatchedJetFlavor", 
         ";PlusWTightIsolatedTrackMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fPlusWRecoElectronMatchedJetFlavor, "hPlusWRecoElectronMatchedJetFlavor", 
         ";PlusWRecoElectronMatchedJetFlavor;Number of Events", 7, 3.5,3.5);
  AddTH1(fPlusWLooseElectronMatchedJetFlavor, "hPlusWLooseElectronMatchedJetFlavor", 
         ";PlusWLooseElectronMatchedJetFlavor;Number of Events", 7, 3.5,3.5);
  AddTH1(fPlusWTightElectronMatchedJetFlavor, "hPlusWTightElectronMatchedJetFlavor", 
         ";PlusWTightElectronMatchedJetFlavor;Number of Events", 7, 3.5,3.5);
  AddTH1(fPlusWCleanElectronMatchedJetFlavor, "hPlusWCleanElectronMatchedJetFlavor", 
         ";PlusWCleanElectronMatchedJetFlavor;Number of Events", 7, 3.5,3.5);

  AddTH1(fMinusWJetFlavor, "hMinusWJetFlavor", 
         ";MinusWJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fMinusWTrackMatchedJetFlavor, "hMinusWTrackMatchedJetFlavor", 
         ";MinusWTrackMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fMinusWLooseIsolatedTrackMatchedJetFlavor, "hMinusWLooseIsolatedTrackMatchedJetFlavor", 
         ";MinusWLooseIsolatedTrackMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fMinusWTightIsolatedTrackMatchedJetFlavor, "hMinusWTightIsolatedTrackMatchedJetFlavor", 
         ";MinusWTightIsolatedTrackMatchedJetFlavor;Number of Events", 7, 3.5,3.5);
  AddTH1(fMinusWRecoElectronMatchedJetFlavor, "hMinusWRecoElectronMatchedJetFlavor", 
         ";MinusWRecoElectronMatchedJetFlavor;Number of Events", 7, 3.5,3.5);
  AddTH1(fMinusWLooseElectronMatchedJetFlavor, "hMinusWLooseElectronMatchedJetFlavor", 
         ";MinusWLooseElectronMatchedJetFlavor;Number of Events", 7, 3.5,3.5);
  AddTH1(fMinusWTightElectronMatchedJetFlavor, "hMinusWTightElectronMatchedJetFlavor", 
         ";MinusWTightElectronMatchedJetFlavor;Number of Events", 7, 3.5,3.5);
  AddTH1(fMinusWCleanElectronMatchedJetFlavor, "hMinusWCleanElectronMatchedJetFlavor", 
         ";MinusWCleanElectronMatchedJetFlavor;Number of Events", 7, 3.5,3.5);

  //*******************************************************************************************
  //W-Charge to Fake Electron Charge Correlations as functions of ID variables
  //*******************************************************************************************
//plot deltaEta between W lepton and fake lepton for OS and SS
//Add Pt, eta, phi of fake lepton to above electron variables,
// add trk iso, jurassic iso, d0
// add fake category plot for same sign and opposite sign

  AddTH1(fSameSignIsolatedTrackTrkIso, "hSameSignIsolatedTrackTrkIso", 
         ";SameSignIsolatedTrackTrkIso;Number of Events",  100, 0, 10.0);
  AddTH1(fSameSignIsolatedTrackD0, "hSameSignIsolatedTrackD0", 
         ";SameSignIsolatedTrackD0;Number of Events",  50, 0, 0.1);
  AddTH1(fSameSignIsolatedTrackEta, "hSameSignIsolatedTrackEta", 
         ";SameSignIsolatedTrackEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fSameSignIsolatedTrackPhi, "hSameSignIsolatedTrackPhi", 
         ";SameSignIsolatedTrackPhi;Number of Events",  25, -3.2,3.2);
  AddTH1(fSameSignIsolatedTrackPt, "hSameSignIsolatedTrackPt", 
         ";SameSignIsolatedTrackPt;Number of Events",  50, 0, 250);
  AddTH1(fSameSignRealToIsolatedTrackDeltaEta, "hSameSignRealToIsolatedTrackDeltaEta", 
         ";SameSignRealToIsolatedTrackDeltaEta;Number of Events",  100, 0.0, 5.0);
  AddTH1(fSameSignRealToIsolatedTrackDeltaPhi, "hSameSignRealToIsolatedTrackDeltaPhi", 
         ";SameSignRealToIsolatedTrackDeltaPhi;Number of Events",  100, 0.0, 3.2);
  AddTH1(fSameSignRealToIsolatedTrackDeltaR, "hSameSignRealToIsolatedTrackDeltaR", 
         ";SameSignRealToIsolatedTrackDeltaR;Number of Events",  100, 0, 5.0);

  AddTH1(fOppositeSignIsolatedTrackTrkIso, "hOppositeSignIsolatedTrackTrkIso", 
         ";OppositeSignIsolatedTrackTrkIso;Number of Events",  100, 0, 10.0);
  AddTH1(fOppositeSignIsolatedTrackD0, "hOppositeSignIsolatedTrackD0", 
         ";OppositeSignIsolatedTrackD0;Number of Events",  50, 0, 0.1);
  AddTH1(fOppositeSignIsolatedTrackEta, "hOppositeSignIsolatedTrackEta", 
         ";OppositeSignIsolatedTrackEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fOppositeSignIsolatedTrackPhi, "hOppositeSignIsolatedTrackPhi", 
         ";OppositeSignIsolatedTrackPhi;Number of Events",  25, -3.2,3.2);
  AddTH1(fOppositeSignIsolatedTrackPt, "hOppositeSignIsolatedTrackPt", 
         ";OppositeSignIsolatedTrackPt;Number of Events",  50, 0, 250);
  AddTH1(fOppositeSignRealToIsolatedTrackDeltaEta, "hOppositeSignRealToIsolatedTrackDeltaEta", 
         ";OppositeSignRealToIsolatedTrackDeltaEta;Number of Events",  100, 0.0, 5.0);
  AddTH1(fOppositeSignRealToIsolatedTrackDeltaPhi, "hOppositeSignRealToIsolatedTrackDeltaPhi", 
         ";OppositeSignRealToIsolatedTrackDeltaPhi;Number of Events",  100, 0.0, 3.2);
  AddTH1(fOppositeSignRealToIsolatedTrackDeltaR, "hOppositeSignRealToIsolatedTrackDeltaR", 
         ";OppositeSignRealToIsolatedTrackDeltaR;Number of Events",  100, 0, 5.0);


  AddTH1(fSameSignFakeElectronHOverE, "hSameSignFakeElectronHOverE", 
         ";SameSignFakeElectronHOverE;Number of Events", 100, 0,0.05);
  AddTH1(fSameSignFakeElectronESuperClusterOverP, "hSameSignFakeElectronESuperClusterOverP", 
         ";SameSignFakeElectronESuperClusterOverP;Number of Events", 100, 0,2.0);
  AddTH1(fSameSignFakeElectronSigmaEtaEta, "hSameSignFakeElectronSigmaEtaEta", 
         ";SameSignFakeElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fSameSignFakeElectronSigmaiEtaiEta, "hSameSignFakeElectronSigmaiEtaiEta", 
         ";SameSignFakeElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fSameSignFakeElectronDeltaPhi, "hSameSignFakeElectronDeltaPhi", 
         ";SameSignFakeElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fSameSignFakeElectronDeltaEta, "hSameSignFakeElectronDeltaEta", 
         ";SameSignFakeElectronDeltaEta;Number of Events", 100, 0,0.1);
  AddTH1(fSameSignFakeElectronfBrem, "hSameSignFakeElectronfBrem", 
         ";SameSignFakeElectronfBrem;Number of Events",  100, 0.0,1.0);
  AddTH1(fSameSignFakeElectronClassification, "hSameSignFakeElectronClassification", 
         ";SameSignFakeElectronClassification;Number of Events",  141, -0.5,140.5);
  AddTH1(fSameSignFakeElectronTrkIso, "hSameSignFakeElectronTrkIso", 
         ";SameSignFakeElectronTrkIso;Number of Events",  100, 0, 10.0);
  AddTH1(fSameSignFakeElectronJurassicIso, "hSameSignFakeElectronJurassicIso", 
         ";SameSignFakeElectronJurassicIso;Number of Events",  100, 0, 10.0);
  AddTH1(fSameSignFakeElectronD0, "hSameSignFakeElectronD0", 
         ";SameSignFakeElectronD0;Number of Events",  50, 0, 0.1);
  AddTH1(fSameSignFakeElectronEta, "hSameSignFakeElectronEta", 
         ";SameSignFakeElectronEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fSameSignFakeElectronPhi, "hSameSignFakeElectronPhi", 
         ";SameSignFakeElectronPhi;Number of Events",  25, -3.2,3.2);
  AddTH1(fSameSignFakeElectronPt, "hSameSignFakeElectronPt", 
         ";SameSignFakeElectronPt;Number of Events",  50, 0, 250);
  AddTH1(fSameSignRealToFakeElectronDeltaEta, "hSameSignRealToFakeElectronDeltaEta", 
         ";SameSignRealToFakeElectronDeltaEta;Number of Events",  100, 0.0, 5.0);
  AddTH1(fSameSignRealToFakeElectronDeltaPhi, "hSameSignRealToFakeElectronDeltaPhi", 
         ";SameSignRealToFakeElectronDeltaPhi;Number of Events",  100, 0.0, 3.2);
  AddTH1(fSameSignRealToFakeElectronDeltaR, "hSameSignRealToFakeElectronDeltaR", 
         ";SameSignRealToFakeElectronDeltaR;Number of Events",  100, 0, 5.0);

  AddTH1(fOppositeSignFakeElectronHOverE, "hOppositeSignFakeElectronHOverE", 
         ";OppositeSignFakeElectronHOverE;Number of Events", 100, 0,0.05);
  AddTH1(fOppositeSignFakeElectronESuperClusterOverP, "hOppositeSignFakeElectronESuperClusterOverP", 
         ";OppositeSignFakeElectronESuperClusterOverP;Number of Events", 100, 0,2.0);
  AddTH1(fOppositeSignFakeElectronSigmaEtaEta, "hOppositeSignFakeElectronSigmaEtaEta", 
         ";OppositeSignFakeElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fOppositeSignFakeElectronSigmaiEtaiEta, "hOppositeSignFakeElectronSigmaiEtaiEta", 
         ";OppositeSignFakeElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fOppositeSignFakeElectronDeltaPhi, "hOppositeSignFakeElectronDeltaPhi", 
         ";OppositeSignFakeElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fOppositeSignFakeElectronDeltaEta, "hOppositeSignFakeElectronDeltaEta", 
         ";OppositeSignFakeElectronDeltaEta;Number of Events", 100, 0,0.1);
  AddTH1(fOppositeSignFakeElectronfBrem, "hOppositeSignFakeElectronfBrem", 
         ";OppositeSignFakeElectronfBrem;Number of Events",  100, 0.0,1.0);
  AddTH1(fOppositeSignFakeElectronClassification, "hOppositeSignFakeElectronClassification", 
         ";OppositeSignFakeElectronClassification;Number of Events",  141, -0.5,140.5);
  AddTH1(fOppositeSignFakeElectronTrkIso, "hOppositeSignFakeElectronTrkIso", 
         ";OppositeSignFakeElectronTrkIso;Number of Events",  100, 0, 10.0);
  AddTH1(fOppositeSignFakeElectronJurassicIso, "hOppositeSignFakeElectronJurassicIso", 
         ";OppositeSignFakeElectronJurassicIso;Number of Events",  100, 0, 10.0);
  AddTH1(fOppositeSignFakeElectronD0, "hOppositeSignFakeElectronD0", 
         ";OppositeSignFakeElectronD0;Number of Events",  50, 0, 0.1);
  AddTH1(fOppositeSignFakeElectronEta, "hOppositeSignFakeElectronEta", 
         ";OppositeSignFakeElectronEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fOppositeSignFakeElectronPhi, "hOppositeSignFakeElectronPhi", 
         ";OppositeSignFakeElectronPhi;Number of Events",  25, -3.2,3.2);
  AddTH1(fOppositeSignFakeElectronPt, "hOppositeSignFakeElectronPt", 
         ";OppositeSignFakeElectronPt;Number of Events",  50, 0, 250);
  AddTH1(fOppositeSignRealToFakeElectronDeltaEta, "hOppositeSignRealToFakeElectronDeltaEta", 
         ";OppositeSignRealToFakeElectronDeltaEta;Number of Events",  100, 0.0, 5.0);
  AddTH1(fOppositeSignRealToFakeElectronDeltaPhi, "hOppositeSignRealToFakeElectronDeltaPhi", 
         ";OppositeSignRealToFakeElectronDeltaPhi;Number of Events",  100, 0.0, 3.2);
  AddTH1(fOppositeSignRealToFakeElectronDeltaR, "hOppositeSignRealToFakeElectronDeltaR", 
         ";OppositeSignRealToFakeElectronDeltaR;Number of Events",  100, 0, 5.0);

  AddTH1(fSameSignFakeIsolatedElectronHOverE, "hSameSignFakeIsolatedElectronHOverE", 
         ";SameSignFakeIsolatedElectronHOverE;Number of Events", 100, 0,0.05);
  AddTH1(fSameSignFakeIsolatedElectronESuperClusterOverP, "hSameSignFakeIsolatedElectronESuperClusterOverP", 
         ";SameSignFakeIsolatedElectronESuperClusterOverP;Number of Events", 100, 0,2.0);
  AddTH1(fSameSignFakeIsolatedElectronSigmaEtaEta, "hSameSignFakeIsolatedElectronSigmaEtaEta", 
         ";SameSignFakeIsolatedElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fSameSignFakeIsolatedElectronSigmaiEtaiEta, "hSameSignFakeIsolatedElectronSigmaiEtaiEta", 
         ";SameSignFakeIsolatedElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fSameSignFakeIsolatedElectronDeltaPhi, "hSameSignFakeIsolatedElectronDeltaPhi", 
         ";SameSignFakeIsolatedElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fSameSignFakeIsolatedElectronDeltaEta, "hSameSignFakeIsolatedElectronDeltaEta", 
         ";SameSignFakeIsolatedElectronDeltaEta;Number of Events", 100, 0,0.1);
  AddTH1(fSameSignFakeIsolatedElectronfBrem, "hSameSignFakeIsolatedElectronfBrem", 
         ";SameSignFakeIsolatedElectronfBrem;Number of Events",  100, 0.0,1.0);
  AddTH1(fSameSignFakeIsolatedElectronClassification, "hSameSignFakeIsolatedElectronClassification", 
         ";SameSignFakeIsolatedElectronClassification;Number of Events",  141, -0.5,140.5);
  AddTH1(fSameSignFakeIsolatedElectronTrkIso, "hSameSignFakeIsolatedElectronTrkIso", 
         ";SameSignFakeIsolatedElectronTrkIso;Number of Events",  100, 0, 10.0);
  AddTH1(fSameSignFakeIsolatedElectronJurassicIso, "hSameSignFakeIsolatedElectronJurassicIso", 
         ";SameSignFakeIsolatedElectronJurassicIso;Number of Events",  100, 0, 10.0);
  AddTH1(fSameSignFakeIsolatedElectronD0, "hSameSignFakeIsolatedElectronD0", 
         ";SameSignFakeIsolatedElectronD0;Number of Events",  50, 0, 0.1);
  AddTH1(fSameSignFakeIsolatedElectronEta, "hSameSignFakeIsolatedElectronEta", 
         ";SameSignFakeIsolatedElectronEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fSameSignFakeIsolatedElectronPhi, "hSameSignFakeIsolatedElectronPhi", 
         ";SameSignFakeIsolatedElectronPhi;Number of Events",  25, -3.2,3.2);
  AddTH1(fSameSignFakeIsolatedElectronPt, "hSameSignFakeIsolatedElectronPt", 
         ";SameSignFakeIsolatedElectronPt;Number of Events",  50, 0, 250);
  AddTH1(fSameSignRealToFakeIsolatedElectronDeltaEta, "hSameSignRealToFakeIsolatedElectronDeltaEta", 
         ";SameSignRealToFakeIsolatedElectronDeltaEta;Number of Events",  100, 0.0, 5.0);
  AddTH1(fSameSignRealToFakeIsolatedElectronDeltaPhi, "hSameSignRealToFakeIsolatedElectronDeltaPhi", 
         ";SameSignRealToFakeIsolatedElectronDeltaPhi;Number of Events",  100, 0.0, 3.2);
  AddTH1(fSameSignRealToFakeIsolatedElectronDeltaR, "hSameSignRealToFakeIsolatedElectronDeltaR", 
         ";SameSignRealToFakeIsolatedElectronDeltaR;Number of Events",  100, 0, 5.0);

  AddTH1(fOppositeSignFakeIsolatedElectronHOverE, "hOppositeSignFakeIsolatedElectronHOverE", 
         ";OppositeSignFakeIsolatedElectronHOverE;Number of Events", 100, 0,0.05);
  AddTH1(fOppositeSignFakeIsolatedElectronESuperClusterOverP, "hOppositeSignFakeIsolatedElectronESuperClusterOverP", 
         ";OppositeSignFakeIsolatedElectronESuperClusterOverP;Number of Events", 100, 0,2.0);
  AddTH1(fOppositeSignFakeIsolatedElectronSigmaEtaEta, "hOppositeSignFakeIsolatedElectronSigmaEtaEta", 
         ";OppositeSignFakeIsolatedElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fOppositeSignFakeIsolatedElectronSigmaiEtaiEta, "hOppositeSignFakeIsolatedElectronSigmaiEtaiEta", 
         ";OppositeSignFakeIsolatedElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fOppositeSignFakeIsolatedElectronDeltaPhi, "hOppositeSignFakeIsolatedElectronDeltaPhi", 
         ";OppositeSignFakeIsolatedElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fOppositeSignFakeIsolatedElectronDeltaEta, "hOppositeSignFakeIsolatedElectronDeltaEta", 
         ";OppositeSignFakeIsolatedElectronDeltaEta;Number of Events", 100, 0,0.1);
  AddTH1(fOppositeSignFakeIsolatedElectronfBrem, "hOppositeSignFakeIsolatedElectronfBrem", 
         ";OppositeSignFakeIsolatedElectronfBrem;Number of Events",  100, 0.0,1.0);
  AddTH1(fOppositeSignFakeIsolatedElectronClassification, "hOppositeSignFakeIsolatedElectronClassification", 
         ";OppositeSignFakeIsolatedElectronClassification;Number of Events",  141, -0.5,140.5);
  AddTH1(fOppositeSignFakeIsolatedElectronTrkIso, "hOppositeSignFakeIsolatedElectronTrkIso", 
         ";OppositeSignFakeIsolatedElectronTrkIso;Number of Events",  100, 0, 10.0);
  AddTH1(fOppositeSignFakeIsolatedElectronJurassicIso, "hOppositeSignFakeIsolatedElectronJurassicIso", 
         ";OppositeSignFakeIsolatedElectronJurassicIso;Number of Events",  100, 0, 10.0);
  AddTH1(fOppositeSignFakeIsolatedElectronD0, "hOppositeSignFakeIsolatedElectronD0", 
         ";OppositeSignFakeIsolatedElectronD0;Number of Events",  50, 0, 0.1);
  AddTH1(fOppositeSignFakeIsolatedElectronEta, "hOppositeSignFakeIsolatedElectronEta", 
         ";OppositeSignFakeIsolatedElectronEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fOppositeSignFakeIsolatedElectronPhi, "hOppositeSignFakeIsolatedElectronPhi", 
         ";OppositeSignFakeIsolatedElectronPhi;Number of Events",  25, -3.2,3.2);
  AddTH1(fOppositeSignFakeIsolatedElectronPt, "hOppositeSignFakeIsolatedElectronPt", 
         ";OppositeSignFakeIsolatedElectronPt;Number of Events",  50, 0, 250);
  AddTH1(fOppositeSignRealToFakeIsolatedElectronDeltaEta, "hOppositeSignRealToFakeIsolatedElectronDeltaEta", 
         ";OppositeSignRealToFakeIsolatedElectronDeltaEta;Number of Events",  100, 0.0, 5.0);
  AddTH1(fOppositeSignRealToFakeIsolatedElectronDeltaPhi, "hOppositeSignRealToFakeIsolatedElectronDeltaPhi", 
         ";OppositeSignRealToFakeIsolatedElectronDeltaPhi;Number of Events",  100, 0.0, 3.2);
  AddTH1(fOppositeSignRealToFakeIsolatedElectronDeltaR, "hOppositeSignRealToFakeIsolatedElectronDeltaR", 
         ";OppositeSignRealToFakeIsolatedElectronDeltaR;Number of Events",  100, 0.0, 5.0);

  AddTH1(fSameSignFakeIsolatedLooseElectronHOverE, "hSameSignFakeIsolatedLooseElectronHOverE", 
         ";SameSignFakeIsolatedLooseElectronHOverE;Number of Events", 100, 0,0.05);
  AddTH1(fSameSignFakeIsolatedLooseElectronESuperClusterOverP, "hSameSignFakeIsolatedLooseElectronESuperClusterOverP", 
         ";SameSignFakeIsolatedLooseElectronESuperClusterOverP;Number of Events", 100, 0,2.0);
  AddTH1(fSameSignFakeIsolatedLooseElectronSigmaEtaEta, "hSameSignFakeIsolatedLooseElectronSigmaEtaEta", 
         ";SameSignFakeIsolatedLooseElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fSameSignFakeIsolatedLooseElectronSigmaiEtaiEta, "hSameSignFakeIsolatedLooseElectronSigmaiEtaiEta", 
         ";SameSignFakeIsolatedLooseElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fSameSignFakeIsolatedLooseElectronDeltaPhi, "hSameSignFakeIsolatedLooseElectronDeltaPhi", 
         ";SameSignFakeIsolatedLooseElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fSameSignFakeIsolatedLooseElectronDeltaEta, "hSameSignFakeIsolatedLooseElectronDeltaEta", 
         ";SameSignFakeIsolatedLooseElectronDeltaEta;Number of Events", 100, 0,0.1);
  AddTH1(fSameSignFakeIsolatedLooseElectronfBrem, "hSameSignFakeIsolatedLooseElectronfBrem", 
         ";SameSignFakeIsolatedLooseElectronfBrem;Number of Events",  100, 0.0,1.0);
  AddTH1(fSameSignFakeIsolatedLooseElectronClassification, "hSameSignFakeIsolatedLooseElectronClassification", 
         ";SameSignFakeIsolatedLooseElectronClassification;Number of Events",  141, -0.5,140.5);
  AddTH1(fSameSignFakeIsolatedLooseElectronTrkIso, "hSameSignFakeIsolatedLooseElectronTrkIso", 
         ";SameSignFakeIsolatedLooseElectronTrkIso;Number of Events",  100, 0, 10.0);
  AddTH1(fSameSignFakeIsolatedLooseElectronJurassicIso, "hSameSignFakeIsolatedLooseElectronJurassicIso", 
         ";SameSignFakeIsolatedLooseElectronJurassicIso;Number of Events",  100, 0, 10.0);
  AddTH1(fSameSignFakeIsolatedLooseElectronD0, "hSameSignFakeIsolatedLooseElectronD0", 
         ";SameSignFakeIsolatedLooseElectronD0;Number of Events",  50, 0, 0.1);
  AddTH1(fSameSignFakeIsolatedLooseElectronEta, "hSameSignFakeIsolatedLooseElectronEta", 
         ";SameSignFakeIsolatedLooseElectronEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fSameSignFakeIsolatedLooseElectronPhi, "hSameSignFakeIsolatedLooseElectronPhi", 
         ";SameSignFakeIsolatedLooseElectronPhi;Number of Events",  25, -3.2,3.2);
  AddTH1(fSameSignFakeIsolatedLooseElectronPt, "hSameSignFakeIsolatedLooseElectronPt", 
         ";SameSignFakeIsolatedLooseElectronPt;Number of Events",  50, 0, 250);
  AddTH1(fSameSignRealToFakeIsolatedLooseElectronDeltaEta, "hSameSignRealToFakeIsolatedLooseElectronDeltaEta", 
         ";SameSignRealToFakeIsolatedLooseElectronDeltaEta;Number of Events",  100, 0.0, 5.0);
  AddTH1(fSameSignRealToFakeIsolatedLooseElectronDeltaPhi, "hSameSignRealToFakeIsolatedLooseElectronDeltaPhi", 
         ";SameSignRealToFakeIsolatedLooseElectronDeltaPhi;Number of Events",  100, 0.0, 3.2);
  AddTH1(fSameSignRealToFakeIsolatedLooseElectronDeltaR, "hSameSignRealToFakeIsolatedLooseElectronDeltaR", 
         ";SameSignRealToFakeIsolatedLooseElectronDeltaR;Number of Events",  100, 0, 5.0);

  AddTH1(fOppositeSignFakeIsolatedLooseElectronHOverE, "hOppositeSignFakeIsolatedLooseElectronHOverE", 
         ";OppositeSignFakeIsolatedLooseElectronHOverE;Number of Events", 100, 0,0.05);
  AddTH1(fOppositeSignFakeIsolatedLooseElectronESuperClusterOverP, "hOppositeSignFakeIsolatedLooseElectronESuperClusterOverP", 
         ";OppositeSignFakeIsolatedLooseElectronESuperClusterOverP;Number of Events", 100, 0,2.0);
  AddTH1(fOppositeSignFakeIsolatedLooseElectronSigmaEtaEta, "hOppositeSignFakeIsolatedLooseElectronSigmaEtaEta", 
         ";OppositeSignFakeIsolatedLooseElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fOppositeSignFakeIsolatedLooseElectronSigmaiEtaiEta, "hOppositeSignFakeIsolatedLooseElectronSigmaiEtaiEta", 
         ";OppositeSignFakeIsolatedLooseElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fOppositeSignFakeIsolatedLooseElectronDeltaPhi, "hOppositeSignFakeIsolatedLooseElectronDeltaPhi", 
         ";OppositeSignFakeIsolatedLooseElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fOppositeSignFakeIsolatedLooseElectronDeltaEta, "hOppositeSignFakeIsolatedLooseElectronDeltaEta", 
         ";OppositeSignFakeIsolatedLooseElectronDeltaEta;Number of Events", 100, 0,0.1);
  AddTH1(fOppositeSignFakeIsolatedLooseElectronfBrem, "hOppositeSignFakeIsolatedLooseElectronfBrem", 
         ";OppositeSignFakeIsolatedLooseElectronfBrem;Number of Events",  100, 0.0,1.0);
  AddTH1(fOppositeSignFakeIsolatedLooseElectronClassification, "hOppositeSignFakeIsolatedLooseElectronClassification", 
         ";OppositeSignFakeIsolatedLooseElectronClassification;Number of Events",  141, -0.5,140.5);
  AddTH1(fOppositeSignFakeIsolatedLooseElectronTrkIso, "hOppositeSignFakeIsolatedLooseElectronTrkIso", 
         ";OppositeSignFakeIsolatedLooseElectronTrkIso;Number of Events",  100, 0, 10.0);
  AddTH1(fOppositeSignFakeIsolatedLooseElectronJurassicIso, "hOppositeSignFakeIsolatedLooseElectronJurassicIso", 
         ";OppositeSignFakeIsolatedLooseElectronJurassicIso;Number of Events",  100, 0, 10.0);
  AddTH1(fOppositeSignFakeIsolatedLooseElectronD0, "hOppositeSignFakeIsolatedLooseElectronD0", 
         ";OppositeSignFakeIsolatedLooseElectronD0;Number of Events",  50, 0, 0.1);
  AddTH1(fOppositeSignFakeIsolatedLooseElectronEta, "hOppositeSignFakeIsolatedLooseElectronEta", 
         ";OppositeSignFakeIsolatedLooseElectronEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fOppositeSignFakeIsolatedLooseElectronPhi, "hOppositeSignFakeIsolatedLooseElectronPhi", 
         ";OppositeSignFakeIsolatedLooseElectronPhi;Number of Events",  25, -3.2,3.2);
  AddTH1(fOppositeSignFakeIsolatedLooseElectronPt, "hOppositeSignFakeIsolatedLooseElectronPt", 
         ";OppositeSignFakeIsolatedLooseElectronPt;Number of Events",  50, 0, 250);
  AddTH1(fOppositeSignRealToFakeIsolatedLooseElectronDeltaEta, "hOppositeSignRealToFakeIsolatedLooseElectronDeltaEta", 
         ";OppositeSignRealToFakeIsolatedLooseElectronDeltaEta;Number of Events",  100, 0.0, 5.0);
  AddTH1(fOppositeSignRealToFakeIsolatedLooseElectronDeltaPhi, "hOppositeSignRealToFakeIsolatedLooseElectronDeltaPhi", 
         ";OppositeSignRealToFakeIsolatedLooseElectronDeltaPhi;Number of Events",  100, 0.0, 3.2);
  AddTH1(fOppositeSignRealToFakeIsolatedLooseElectronDeltaR, "hOppositeSignRealToFakeIsolatedLooseElectronDeltaR", 
         ";OppositeSignRealToFakeIsolatedLooseElectronDeltaR;Number of Events",  100, 0, 5.0);

  AddTH1(fSameSignFakeIsolatedTightElectronHOverE, "hSameSignFakeIsolatedTightElectronHOverE", 
         ";SameSignFakeIsolatedTightElectronHOverE;Number of Events", 100, 0,0.05);
  AddTH1(fSameSignFakeIsolatedTightElectronESuperClusterOverP, "hSameSignFakeIsolatedTightElectronESuperClusterOverP", 
         ";SameSignFakeIsolatedTightElectronESuperClusterOverP;Number of Events", 100, 0,2.0);
  AddTH1(fSameSignFakeIsolatedTightElectronSigmaEtaEta, "hSameSignFakeIsolatedTightElectronSigmaEtaEta", 
         ";SameSignFakeIsolatedTightElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fSameSignFakeIsolatedTightElectronSigmaiEtaiEta, "hSameSignFakeIsolatedTightElectronSigmaiEtaiEta", 
         ";SameSignFakeIsolatedTightElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fSameSignFakeIsolatedTightElectronDeltaPhi, "hSameSignFakeIsolatedTightElectronDeltaPhi", 
         ";SameSignFakeIsolatedTightElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fSameSignFakeIsolatedTightElectronDeltaEta, "hSameSignFakeIsolatedTightElectronDeltaEta", 
         ";SameSignFakeIsolatedTightElectronDeltaEta;Number of Events", 100, 0,0.1);
  AddTH1(fSameSignFakeIsolatedTightElectronfBrem, "hSameSignFakeIsolatedTightElectronfBrem", 
         ";SameSignFakeIsolatedTightElectronfBrem;Number of Events",  100, 0.0,1.0);
  AddTH1(fSameSignFakeIsolatedTightElectronClassification, "hSameSignFakeIsolatedTightElectronClassification", 
         ";SameSignFakeIsolatedTightElectronClassification;Number of Events",  141, -0.5,140.5);
  AddTH1(fSameSignFakeIsolatedTightElectronTrkIso, "hSameSignFakeIsolatedTightElectronTrkIso", 
         ";SameSignFakeIsolatedTightElectronTrkIso;Number of Events",  100, 0, 10.0);
  AddTH1(fSameSignFakeIsolatedTightElectronJurassicIso, "hSameSignFakeIsolatedTightElectronJurassicIso", 
         ";SameSignFakeIsolatedTightElectronJurassicIso;Number of Events",  100, 0, 10.0);
  AddTH1(fSameSignFakeIsolatedTightElectronD0, "hSameSignFakeIsolatedTightElectronD0", 
         ";SameSignFakeIsolatedTightElectronD0;Number of Events",  50, 0, 0.1);
  AddTH1(fSameSignFakeIsolatedTightElectronEta, "hSameSignFakeIsolatedTightElectronEta", 
         ";SameSignFakeIsolatedTightElectronEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fSameSignFakeIsolatedTightElectronPhi, "hSameSignFakeIsolatedTightElectronPhi", 
         ";SameSignFakeIsolatedTightElectronPhi;Number of Events",  25, -3.2,3.2);
  AddTH1(fSameSignFakeIsolatedTightElectronPt, "hSameSignFakeIsolatedTightElectronPt", 
         ";SameSignFakeIsolatedTightElectronPt;Number of Events",  50, 0, 250);
  AddTH1(fSameSignRealToFakeIsolatedTightElectronDeltaEta, "hSameSignRealToFakeIsolatedTightElectronDeltaEta", 
         ";SameSignRealToFakeIsolatedTightElectronDeltaEta;Number of Events",  100, 0.0, 5.0);
  AddTH1(fSameSignRealToFakeIsolatedTightElectronDeltaPhi, "hSameSignRealToFakeIsolatedTightElectronDeltaPhi", 
         ";SameSignRealToFakeIsolatedTightElectronDeltaPhi;Number of Events",  100, 0.0, 3.2);
  AddTH1(fSameSignRealToFakeIsolatedTightElectronDeltaR, "hSameSignRealToFakeIsolatedTightElectronDeltaR", 
         ";SameSignRealToFakeIsolatedTightElectronDeltaR;Number of Events",  100, 0, 5.0);

  AddTH1(fOppositeSignFakeIsolatedTightElectronHOverE, "hOppositeSignFakeIsolatedTightElectronHOverE", 
         ";OppositeSignFakeIsolatedTightElectronHOverE;Number of Events", 100, 0,0.05);
  AddTH1(fOppositeSignFakeIsolatedTightElectronESuperClusterOverP, "hOppositeSignFakeIsolatedTightElectronESuperClusterOverP", 
         ";OppositeSignFakeIsolatedTightElectronESuperClusterOverP;Number of Events", 100, 0,2.0);
  AddTH1(fOppositeSignFakeIsolatedTightElectronSigmaEtaEta, "hOppositeSignFakeIsolatedTightElectronSigmaEtaEta", 
         ";OppositeSignFakeIsolatedTightElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fOppositeSignFakeIsolatedTightElectronSigmaiEtaiEta, "hOppositeSignFakeIsolatedTightElectronSigmaiEtaiEta", 
         ";OppositeSignFakeIsolatedTightElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fOppositeSignFakeIsolatedTightElectronDeltaPhi, "hOppositeSignFakeIsolatedTightElectronDeltaPhi", 
         ";OppositeSignFakeIsolatedTightElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fOppositeSignFakeIsolatedTightElectronDeltaEta, "hOppositeSignFakeIsolatedTightElectronDeltaEta", 
         ";OppositeSignFakeIsolatedTightElectronDeltaEta;Number of Events", 100, 0,0.1);
  AddTH1(fOppositeSignFakeIsolatedTightElectronfBrem, "hOppositeSignFakeIsolatedTightElectronfBrem", 
         ";OppositeSignFakeIsolatedTightElectronfBrem;Number of Events",  100, 0.0,1.0);
  AddTH1(fOppositeSignFakeIsolatedTightElectronClassification, "hOppositeSignFakeIsolatedTightElectronClassification", 
         ";OppositeSignFakeIsolatedTightElectronClassification;Number of Events",  141, -0.5,140.5);
  AddTH1(fOppositeSignFakeIsolatedTightElectronTrkIso, "hOppositeSignFakeIsolatedTightElectronTrkIso", 
         ";OppositeSignFakeIsolatedTightElectronTrkIso;Number of Events",  100, 0, 10.0);
  AddTH1(fOppositeSignFakeIsolatedTightElectronJurassicIso, "hOppositeSignFakeIsolatedTightElectronJurassicIso", 
         ";OppositeSignFakeIsolatedTightElectronJurassicIso;Number of Events",  100, 0, 10.0);
  AddTH1(fOppositeSignFakeIsolatedTightElectronD0, "hOppositeSignFakeIsolatedTightElectronD0", 
         ";OppositeSignFakeIsolatedTightElectronD0;Number of Events",  50, 0, 0.1);
  AddTH1(fOppositeSignFakeIsolatedTightElectronEta, "hOppositeSignFakeIsolatedTightElectronEta", 
         ";OppositeSignFakeIsolatedTightElectronEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fOppositeSignFakeIsolatedTightElectronPhi, "hOppositeSignFakeIsolatedTightElectronPhi", 
         ";OppositeSignFakeIsolatedTightElectronPhi;Number of Events",  25, -3.2,3.2);
  AddTH1(fOppositeSignFakeIsolatedTightElectronPt, "hOppositeSignFakeIsolatedTightElectronPt", 
         ";OppositeSignFakeIsolatedTightElectronPt;Number of Events",  50, 0, 250);
  AddTH1(fOppositeSignRealToFakeIsolatedTightElectronDeltaEta, "hOppositeSignRealToFakeIsolatedTightElectronDeltaEta", 
         ";OppositeSignRealToFakeIsolatedTightElectronDeltaEta;Number of Events",  100, 0.0, 5.0);
  AddTH1(fOppositeSignRealToFakeIsolatedTightElectronDeltaPhi, "hOppositeSignRealToFakeIsolatedTightElectronDeltaPhi", 
         ";OppositeSignRealToFakeIsolatedTightElectronDeltaPhi;Number of Events",  100, 0.0, 3.2);
  AddTH1(fOppositeSignRealToFakeIsolatedTightElectronDeltaR, "hOppositeSignRealToFakeIsolatedTightElectronDeltaR", 
         ";OppositeSignRealToFakeIsolatedTightElectronDeltaR;Number of Events",  100, 0, 5.0);

  AddTH1(fSameSignFakeCleanElectronHOverE, "hSameSignFakeCleanElectronHOverE", 
         ";SameSignFakeCleanElectronHOverE;Number of Events", 100, 0,0.05);
  AddTH1(fSameSignFakeCleanElectronESuperClusterOverP, "hSameSignFakeCleanElectronESuperClusterOverP", 
         ";SameSignFakeCleanElectronESuperClusterOverP;Number of Events", 100, 0,2.0);
  AddTH1(fSameSignFakeCleanElectronSigmaEtaEta, "hSameSignFakeCleanElectronSigmaEtaEta", 
         ";SameSignFakeCleanElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fSameSignFakeCleanElectronSigmaiEtaiEta, "hSameSignFakeCleanElectronSigmaiEtaiEta", 
         ";SameSignFakeCleanElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fSameSignFakeCleanElectronDeltaPhi, "hSameSignFakeCleanElectronDeltaPhi", 
         ";SameSignFakeCleanElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fSameSignFakeCleanElectronDeltaEta, "hSameSignFakeCleanElectronDeltaEta", 
         ";SameSignFakeCleanElectronDeltaEta;Number of Events", 100, 0,0.1);
  AddTH1(fSameSignFakeCleanElectronfBrem, "hSameSignFakeCleanElectronfBrem", 
         ";SameSignFakeCleanElectronfBrem;Number of Events",  100, 0.0,1.0);
  AddTH1(fSameSignFakeCleanElectronClassification, "hSameSignFakeCleanElectronClassification", 
         ";SameSignFakeCleanElectronClassification;Number of Events",  141, -0.5,140.5);
  AddTH1(fSameSignFakeCleanElectronTrkIso, "hSameSignFakeCleanElectronTrkIso", 
         ";SameSignFakeCleanElectronTrkIso;Number of Events",  100, 0, 10.0);
  AddTH1(fSameSignFakeCleanElectronJurassicIso, "hSameSignFakeCleanElectronJurassicIso", 
         ";SameSignFakeCleanElectronJurassicIso;Number of Events",  100, 0, 10.0);
  AddTH1(fSameSignFakeCleanElectronD0, "hSameSignFakeCleanElectronD0", 
         ";SameSignFakeCleanElectronD0;Number of Events",  50, 0, 0.1);
  AddTH1(fSameSignFakeCleanElectronEta, "hSameSignFakeCleanElectronEta", 
         ";SameSignFakeCleanElectronEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fSameSignFakeCleanElectronPhi, "hSameSignFakeCleanElectronPhi", 
         ";SameSignFakeCleanElectronPhi;Number of Events",  25, -3.2,3.2);
  AddTH1(fSameSignFakeCleanElectronPt, "hSameSignFakeCleanElectronPt", 
         ";SameSignFakeCleanElectronPt;Number of Events",  50, 0, 250);
  AddTH1(fSameSignRealToFakeCleanElectronDeltaEta, "hSameSignRealToFakeCleanElectronDeltaEta", 
         ";SameSignRealToFakeCleanElectronDeltaEta;Number of Events", 100, 0.0, 5.0);
  AddTH1(fSameSignRealToFakeCleanElectronDeltaPhi, "hSameSignRealToFakeCleanElectronDeltaPhi", 
         ";SameSignRealToFakeCleanElectronDeltaPhi;Number of Events",  100, 0.0, 3.2);
  AddTH1(fSameSignRealToFakeCleanElectronDeltaR, "hSameSignRealToFakeCleanElectronDeltaR", 
         ";SameSignRealToFakeCleanElectronDeltaR;Number of Events",  100, 0.0, 5.0);

  AddTH1(fOppositeSignFakeCleanElectronHOverE, "hOppositeSignFakeCleanElectronHOverE", 
         ";OppositeSignFakeCleanElectronHOverE;Number of Events", 100, 0,0.05);
  AddTH1(fOppositeSignFakeCleanElectronESuperClusterOverP, "hOppositeSignFakeCleanElectronESuperClusterOverP", 
         ";OppositeSignFakeCleanElectronESuperClusterOverP;Number of Events", 100, 0,2.0);
  AddTH1(fOppositeSignFakeCleanElectronSigmaEtaEta, "hOppositeSignFakeCleanElectronSigmaEtaEta", 
         ";OppositeSignFakeCleanElectronSigmaEtaEta;Number of Events", 100, 0,0.002);
  AddTH1(fOppositeSignFakeCleanElectronSigmaiEtaiEta, "hOppositeSignFakeCleanElectronSigmaiEtaiEta", 
         ";OppositeSignFakeCleanElectronSigmaiEtaiEta;Number of Events", 100, 0,0.002);
  AddTH1(fOppositeSignFakeCleanElectronDeltaPhi, "hOppositeSignFakeCleanElectronDeltaPhi", 
         ";OppositeSignFakeCleanElectronDeltaPhi;Number of Events", 100, 0,0.1);
  AddTH1(fOppositeSignFakeCleanElectronDeltaEta, "hOppositeSignFakeCleanElectronDeltaEta", 
         ";OppositeSignFakeCleanElectronDeltaEta;Number of Events", 100, 0,0.1);
  AddTH1(fOppositeSignFakeCleanElectronfBrem, "hOppositeSignFakeCleanElectronfBrem", 
         ";OppositeSignFakeCleanElectronfBrem;Number of Events",  100, 0.0,1.0);
  AddTH1(fOppositeSignFakeCleanElectronClassification, "hOppositeSignFakeCleanElectronClassification", 
         ";OppositeSignFakeCleanElectronClassification;Number of Events",  141, -0.5,140.5);
  AddTH1(fOppositeSignFakeCleanElectronTrkIso, "hOppositeSignFakeCleanElectronTrkIso", 
         ";OppositeSignFakeCleanElectronTrkIso;Number of Events",  100, 0, 10.0);
  AddTH1(fOppositeSignFakeCleanElectronJurassicIso, "hOppositeSignFakeCleanElectronJurassicIso", 
         ";OppositeSignFakeCleanElectronJurassicIso;Number of Events",  100, 0, 10.0);
  AddTH1(fOppositeSignFakeCleanElectronD0, "hOppositeSignFakeCleanElectronD0", 
         ";OppositeSignFakeCleanElectronD0;Number of Events",  50, 0, 0.1);
  AddTH1(fOppositeSignFakeCleanElectronEta, "hOppositeSignFakeCleanElectronEta", 
         ";OppositeSignFakeCleanElectronEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fOppositeSignFakeCleanElectronPhi, "hOppositeSignFakeCleanElectronPhi", 
         ";OppositeSignFakeCleanElectronPhi;Number of Events",  25, -3.2,3.2);
  AddTH1(fOppositeSignFakeCleanElectronPt, "hOppositeSignFakeCleanElectronPt", 
         ";OppositeSignFakeCleanElectronPt;Number of Events",  50, 0, 250);
  AddTH1(fOppositeSignRealToFakeCleanElectronDeltaEta, "hOppositeSignRealToFakeCleanElectronDeltaEta", 
         ";OppositeSignRealToFakeCleanElectronDeltaEta;Number of Events",  100, 0.0, 5.0);
  AddTH1(fOppositeSignRealToFakeCleanElectronDeltaPhi, "hOppositeSignRealToFakeCleanElectronDeltaPhi", 
         ";OppositeSignRealToFakeCleanElectronDeltaPhi;Number of Events",  100, 0.0, 3.2);
  AddTH1(fOppositeSignRealToFakeCleanElectronDeltaR, "hOppositeSignRealToFakeCleanElectronDeltaR", 
         ";OppositeSignRealToFakeCleanElectronDeltaR;Number of Events",  100, 0.0, 5.0);

  AddTH1(fSameSignIsolatedTrackCategory, 
         "hSameSignIsolatedTrackCategory", 
         ";SameSignIsolatedTrackCategory;Number of Events", 8,-0.5,7.5);
  xa = fSameSignIsolatedTrackCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");

  AddTH1(fOppositeSignIsolatedTrackCategory, 
         "hOppositeSignIsolatedTrackCategory", 
         ";OppositeSignIsolatedTrackCategory;Number of Events", 8,-0.5,7.5);
  xa = fOppositeSignIsolatedTrackCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");


  AddTH1(fSameSignFakeElectronCategory, 
         "hSameSignFakeElectronCategory", 
         ";SameSignFakeElectronCategory;Number of Events", 8,-0.5,7.5);
  xa = fSameSignFakeElectronCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");

  AddTH1(fOppositeSignFakeElectronCategory, 
         "hOppositeSignFakeElectronCategory", 
         ";OppositeSignFakeElectronCategory;Number of Events", 8,-0.5,7.5);
  xa = fOppositeSignFakeElectronCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");


  AddTH1(fSameSignFakeIsolatedElectronCategory, 
         "hSameSignFakeIsolatedElectronCategory", 
         ";SameSignFakeIsolatedElectronCategory;Number of Events", 8,-0.5,7.5);
  xa = fSameSignFakeIsolatedElectronCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");


  AddTH1(fOppositeSignFakeIsolatedElectronCategory, 
         "hOppositeSignFakeIsolatedElectronCategory", 
         ";OppositeSignFakeIsolatedElectronCategory;Number of Events", 8,-0.5,7.5);
  xa = fOppositeSignFakeIsolatedElectronCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");

  AddTH1(fSameSignFakeIsolatedLooseElectronCategory, 
         "hSameSignFakeIsolatedLooseElectronCategory", 
         ";SameSignFakeIsolatedLooseElectronCategory;Number of Events", 8,-0.5,7.5);
  xa = fSameSignFakeIsolatedLooseElectronCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");


  AddTH1(fOppositeSignFakeIsolatedLooseElectronCategory, 
         "hOppositeSignFakeIsolatedLooseElectronCategory", 
         ";OppositeSignFakeIsolatedLooseElectronCategory;Number of Events", 8,-0.5,7.5);
  xa = fOppositeSignFakeIsolatedLooseElectronCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");

  AddTH1(fSameSignFakeIsolatedTightElectronCategory, 
         "hSameSignFakeIsolatedTightElectronCategory", 
         ";SameSignFakeIsolatedTightElectronCategory;Number of Events", 8,-0.5,7.5);
  xa = fSameSignFakeIsolatedTightElectronCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");


  AddTH1(fOppositeSignFakeIsolatedTightElectronCategory, 
         "hOppositeSignFakeIsolatedTightElectronCategory", 
         ";OppositeSignFakeIsolatedTightElectronCategory;Number of Events", 8,-0.5,7.5);
  xa = fOppositeSignFakeIsolatedTightElectronCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");


  AddTH1(fSameSignFakeCleanElectronCategory, 
         "hSameSignFakeCleanElectronCategory", 
         ";SameSignFakeCleanElectronCategory;Number of Events", 8,-0.5,7.5);
  xa = fSameSignFakeCleanElectronCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");

  AddTH1(fOppositeSignFakeCleanElectronCategory, 
         "hOppositeSignFakeCleanElectronCategory", 
         ";OppositeSignFakeCleanElectronCategory;Number of Events",  8,-0.5,7.5);
  xa = fOppositeSignFakeCleanElectronCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");

}

//--------------------------------------------------------------------------------------------------
void FakeElectronAnalysisMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and
  //output debug info or not   

  //cout << "Event : " << count << endl;
  count++;

  LoadBranch(fMCPartBranchName);
  LoadBranch(fVertexBranchName);
  LoadBranch(fGenJetBranchName);
  LoadBranch(fElectronBranchName);
  LoadBranch(fTrackBranchName);
  LoadBranch(fGsfTrackBranchName);
  LoadBranch(fJetBranchName);
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

  // get input clean object collections
  const ElectronCol *CleanElectrons = 0;
  if (!fCleanElectronsName.IsNull())
    CleanElectrons = GetObjThisEvt<ElectronCol>(fCleanElectronsName);
  mithep::ParticleOArr *CleanLeptons = dynamic_cast<mithep::ParticleOArr*>
    (FindObjThisEvt(ModNames::gkMergedLeptonsName));
  const JetCol *GoodJets = 0;
  if (!fGoodJetsName.IsNull())
    GoodJets = GetObjThisEvt<JetCol>(fGoodJetsName);


  //Combine Barrel and Endcap superclusters into the same ObjArray
  ObjArray <SuperCluster> *SuperClusters = new ObjArray <SuperCluster>;
  for (UInt_t i=0; i<fBarrelSuperClusters->GetEntries(); i++) {
    SuperClusters->Add(fBarrelSuperClusters->At(i));
  }
  for (UInt_t i=0; i<fEndcapSuperClusters->GetEntries(); i++) {
    SuperClusters->Add(fEndcapSuperClusters->At(i));
  }

  ObjArray<Track> *DenominatorGsfTracks = new ObjArray<Track>;
  ObjArray<Track> *FrameworkMatchedGsfTracks = new ObjArray<Track>;
  ObjArray<Track> *DeltaRMatchedGsfTracks = new ObjArray<Track>;

  //count GenJets above pt > 20GeV.
  Int_t NGenJets = 0;
  for (UInt_t i=0;i<fGenJets->GetEntries();i++) {
    if (fGenJets->At(i)->Pt() >= 20.0) 
      NGenJets++;
  }
  fNGenJets->Fill(NGenJets);
  fNCleanLeptons->Fill(CleanLeptons->GetEntries());
  fNGenLeptons->Fill(GenLeptonsAndTaus->GetEntries());


  //***********************************************************************************************
  //Fill Jet flavor for W+jet events
  //***********************************************************************************************  
  if (NGenJets == 1 && GenLeptons->GetEntries() == 1) {
    if (MathUtils::DeltaR(fGenJets->At(0)->Phi(), fGenJets->At(0)->Eta(),
                          GenLeptons->At(0)->Phi(), 
                          GenLeptons->At(0)->Eta())>= 0.3) {
      if (GenLeptons->At(0)->Charge() > 0)
        fPlusWJetFlavor->Fill(fGenJets->At(0)->MatchedMCFlavor());
      else 
        fMinusWJetFlavor->Fill(fGenJets->At(0)->MatchedMCFlavor());
    }
  }

  //***********************************************************************************************
  //Find lepton from W
  //***********************************************************************************************  
  ChargedParticle *RealWLepton = NULL;
  for (UInt_t i=0; i<CleanLeptons->GetEntries(); i++) {
    //Veto denominators matching to real genleptons
    Bool_t IsMCLepton = false;
    for (UInt_t l=0; l<GenLeptons->GetEntries(); l++) {
      if (MathUtils::DeltaR(CleanLeptons->At(i)->Phi(), CleanLeptons->At(i)->Eta(),
                            GenLeptons->At(l)->Phi(), 
                            GenLeptons->At(l)->Eta()) < 0.3) {
        IsMCLepton = true;
        RealWLepton = dynamic_cast<mithep::ChargedParticle*>(CleanLeptons->At(i));
        break;
      }
    }
  }
  
  //***********************************************************************************************
  //Decide which primary vertex to choose.
  //***********************************************************************************************  
  const Vertex *primaryVertex = NULL;
  Double_t bestVertexProb = -1.0;
  for (UInt_t i=0; i<fVertices->GetEntries(); i++) {
    if (fVertices->At(i)->Prob() > bestVertexProb) {
      bestVertexProb = fVertices->At(i)->Prob();
      primaryVertex = fVertices->At(i);
    }
  }

  //***********************************************************************************************
  //Study Fake Categories
  //***********************************************************************************************  
  if ( (!fRequireFirstLepton && CleanLeptons->GetEntries() > 0) || 
       (fRequireFirstLepton && CleanLeptons->GetEntries() == 2 && GenLeptons->GetEntries() == 1 && NGenJets == 1)               
    ) {
                  
    bool printHepMCTable = false;  
  
    if(fPrintLogs) {
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
    //Find Clean Electrons not matching to a GenLepton
    //***********************************************************************************************  
    for (UInt_t i=0; i<CleanElectrons->GetEntries(); i++) {
      const Electron *ele = CleanElectrons->At(i);
    
      //Veto denominators matching to real genleptons
      Bool_t IsMCLepton = false;
      for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
        if (MathUtils::DeltaR(ele->Phi(), ele->Eta(),
                              GenLeptonsAndTaus->At(l)->Phi(), 
                              GenLeptonsAndTaus->At(l)->Eta()) < 0.3 &&
            GenLeptonsAndTaus->At(l)->AbsPdgId() == 11
          ) {
          
          IsMCLepton = true;
        }
      }
    
      //Print out Electron        
      if(fPrintLogs) {        
        cout << "********************************************************************************\n";
        cout << "Electron : " << ele->Pt() << " " << ele->Eta() << " " << ele->Phi();
        if (ele->GsfTrk()) {
          cout << "  | GsfTrack: Charge " << ele->GsfTrk()->Charge() << "   " 
               << ele->GsfTrk()->P() << " " 
               << ele->GsfTrk()->Pt() << " " << ele->GsfTrk()->Eta() << " "
               << ele->GsfTrk()->Phi();
        }
        cout << " | ";
        if (ele->TrackerTrk()) {
          cout << " TrackerTrack: Charge " << ele->TrackerTrk()->Charge() << "   " 
               << ele->TrackerTrk()->P() << " "
               << ele->TrackerTrk()->Pt() << " " << ele->TrackerTrk()->Eta()
               << " " << ele->TrackerTrk()->Phi();
        }
        cout << " SuperCluster: " << ele->SCluster()->Energy() << " " << ele->SCluster()->Eta() 
             << " " << ele->SCluster()->Phi() << endl;
        cout << "E/P: " << ele->ESuperClusterOverP() << " , Had/EM: " << ele->HadronicOverEm() 
             << " , TrackIso: " << ele->TrackIsolation() << " , JurassicEcalIso: "
             << ele->EcalJurassicIsolation() << " , fBrem: " 
             << (ele->PIn() - ele->POut())/ ele->PIn()
             << endl;
        
        cout << endl;        
      }
      
      if (ele->Pt() > 10.0 && !(fVetoGeneratorLeptons && IsMCLepton)) {
        //Found a fake electron.
                   
       //***********************************************************************************************
        //GsfTrack to TrackerTrack Charge Matching
        //***********************************************************************************************  

        int ChargeMatchCode = 0;
        if (ele->GsfTrk() && ele->TrackerTrk()) {        
          if (ele->GsfTrk()->Charge() + ele->TrackerTrk()->Charge() == 0) {
            ChargeMatchCode = 2*ele->GsfTrk()->Charge() + ele->TrackerTrk()->Charge();
          } else {
            ChargeMatchCode = ele->GsfTrk()->Charge() + ele->TrackerTrk()->Charge();
          }
        }
        if (fabs(ele->Eta()) < 1.479) {
          fCleanBarrelElectronTrackerGsfTrackChargeMatch->Fill(ChargeMatchCode);
        } else {
          fCleanEndcapElectronTrackerGsfTrackChargeMatch->Fill(ChargeMatchCode);
        }

        //***********************************************************************************************
        //Match Clean Electron to a GenJet
        //***********************************************************************************************  
        //match the clean electron to a genJet.
        //try to match it to a genjet
        Double_t minDR = 5000.0;
        const GenJet *matchedGenJet = NULL;
        for(UInt_t j=0;j<fGenJets->GetEntries();j++) {
          Double_t DR = MathUtils::DeltaR(ele->Phi(), ele->Eta(), fGenJets->At(j)->Phi(),
                                          fGenJets->At(j)->Eta());
          if (DR < minDR && DR < 0.3) {
            minDR = DR;
            matchedGenJet = fGenJets->At(j);
          }
        }
        //if it matched
        if (fabs(ele->Eta()) < 1.479) {
          if (matchedGenJet) {
            if (ele->Charge() > 0) {
              fCleanPlusBarrelElectronMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
              fCleanPlusBarrelElectronMatchedGenJetPt->Fill(matchedGenJet->Pt());
              fCleanPlusBarrelElectronMatchedGenJetDeltaR->Fill(minDR);
              fCleanPlusBarrelElectronMatchedGenJetDeltaPtOverPt->Fill((ele->Pt() - matchedGenJet->Pt())/ele->Pt());
            } else {
              fCleanMinusBarrelElectronMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
              fCleanMinusBarrelElectronMatchedGenJetPt->Fill(matchedGenJet->Pt());
              fCleanMinusBarrelElectronMatchedGenJetDeltaR->Fill(minDR);
              fCleanMinusBarrelElectronMatchedGenJetDeltaPtOverPt->Fill((ele->Pt() - matchedGenJet->Pt())/ele->Pt());
            }
          } else {
            if (ele->Charge() > 0) {
              fCleanPlusBarrelElectronMatchedJetFlavor->Fill(-9);
            } else {
              fCleanMinusBarrelElectronMatchedJetFlavor->Fill(-9);
            }
          }
        } else {
          if (matchedGenJet) {
            if (ele->Charge() > 0) {
              fCleanPlusEndcapElectronMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
              fCleanPlusEndcapElectronMatchedGenJetPt->Fill(matchedGenJet->Pt());
              fCleanPlusEndcapElectronMatchedGenJetDeltaR->Fill(minDR);
              fCleanPlusEndcapElectronMatchedGenJetDeltaPtOverPt->Fill((ele->Pt() - matchedGenJet->Pt())/ele->Pt());
            } else {
              fCleanMinusEndcapElectronMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
              fCleanMinusEndcapElectronMatchedGenJetPt->Fill(matchedGenJet->Pt());
              fCleanMinusEndcapElectronMatchedGenJetDeltaR->Fill(minDR);
              fCleanMinusEndcapElectronMatchedGenJetDeltaPtOverPt->Fill((ele->Pt() - matchedGenJet->Pt())/ele->Pt());
            }
          } else {
            if (ele->Charge() > 0) {
              fCleanPlusEndcapElectronMatchedJetFlavor->Fill(-9);
            } else {
              fCleanMinusEndcapElectronMatchedJetFlavor->Fill(-9);
            }
          }
        }
        
        
        //***********************************************************************************************
        //Match Clean Electron to Leading GenParticle (not parton)
        //***********************************************************************************************  
        Double_t highestPt = 0.0;
        Double_t matchedDR = 5000.0;
        const MCParticle *matchedGenParticle = NULL;
        for(UInt_t j=0;j<fParticles->GetEntries();j++) {
          Double_t DR = MathUtils::DeltaR(ele->Phi(), ele->Eta(), fParticles->At(j)->Phi(),
                                          fParticles->At(j)->Eta());
          if (fParticles->At(j)->Pt() > highestPt && DR < 0.3 && !fParticles->At(j)->IsParton() 
              && fParticles->At(j)->PdgId() != 92) {
            highestPt = fParticles->At(j)->Pt();
            matchedGenParticle = fParticles->At(j);
            matchedDR = DR;
          }
        }
                
        if (fabs(ele->Eta()) < 1.479) {
          if (matchedGenParticle && matchedGenParticle->Pt()>5.0) {
            if (ele->Charge() > 0) {
              fCleanPlusBarrelElectronMatchedGenParticleID->Fill(matchedGenParticle->PdgId());
              fCleanPlusBarrelElectronMatchedGenParticlePt->Fill(matchedGenParticle->Pt());
              fCleanPlusBarrelElectronMatchedGenParticleDeltaR->Fill(matchedDR);
              fCleanPlusBarrelElectronMatchedGenParticleDeltaPtOverPt->Fill((ele->Pt() - matchedGenParticle->Pt())/ele->Pt());
            } else {
              fCleanMinusBarrelElectronMatchedGenParticleID->Fill(matchedGenParticle->PdgId());
              fCleanMinusBarrelElectronMatchedGenParticlePt->Fill(matchedGenParticle->Pt());
              fCleanMinusBarrelElectronMatchedGenParticleDeltaR->Fill(matchedDR);
              fCleanMinusBarrelElectronMatchedGenParticleDeltaPtOverPt->Fill((ele->Pt() - matchedGenParticle->Pt())/ele->Pt());
            }
          } else {
            if (ele->Charge() > 0) {
              fCleanPlusBarrelElectronMatchedGenParticleID->Fill(0);
            } else {
              fCleanMinusBarrelElectronMatchedGenParticleID->Fill(0);
            }
          }
        } else {
          if (matchedGenParticle && matchedGenParticle->Pt()>5.0) {
            if (ele->Charge() > 0) {
              fCleanPlusEndcapElectronMatchedGenParticleID->Fill(matchedGenParticle->PdgId());
              fCleanPlusEndcapElectronMatchedGenParticlePt->Fill(matchedGenParticle->Pt());
              fCleanPlusEndcapElectronMatchedGenParticleDeltaR->Fill(matchedDR);
              fCleanPlusEndcapElectronMatchedGenParticleDeltaPtOverPt->Fill((ele->Pt() - matchedGenParticle->Pt())/ele->Pt());
            } else {
              fCleanMinusEndcapElectronMatchedGenParticleID->Fill(matchedGenParticle->PdgId());
              fCleanMinusEndcapElectronMatchedGenParticlePt->Fill(matchedGenParticle->Pt());
              fCleanMinusEndcapElectronMatchedGenParticleDeltaR->Fill(matchedDR);
              fCleanMinusEndcapElectronMatchedGenParticleDeltaPtOverPt->Fill((ele->Pt() - matchedGenParticle->Pt())/ele->Pt());
            }
          } else {
            if (ele->Charge() > 0) {
              fCleanPlusEndcapElectronMatchedGenParticleID->Fill(0);
            } else {
              fCleanMinusEndcapElectronMatchedGenParticleID->Fill(0);
            }
          }
        }
        
        //***********************************************************************************************
        //Match Clean Electron to a SimTrack and then trace back up the mother chain.
        //***********************************************************************************************  
        Double_t bestDeltaPtOverPt = 0.0;
        Double_t matchDeltaPtOverPt = 0.0;
        Double_t secondaryMatchDeltaPtOverPt = 0.0;
        Double_t minProductionVertexRho = 10000.0;
        Double_t secondaryMatchMinDR = 5000.0;
        minDR = 5000.0;
        const MCParticle *matchedSimChargedParticle = NULL;
        const MCParticle *secondaryMatchedSimChargedParticle = NULL;

        if(fPrintLogs) cout << "\nSimulated particles near Electron\n";
        for(UInt_t j=0;j<fParticles->GetEntries();j++) {          
          if (fParticles->At(j)->IsSimulated() ) {
            Bool_t isStable = false;
            Bool_t isTrackable = false;
            if(fParticles->At(j)->NDaughters() == 0 || fParticles->At(j)->DecayVertex().Rho() > 10.0)
              isStable = true;
          
            if (fParticles->At(j)->Mother() && fParticles->At(j)->Mother()->DecayVertex().Rho() <= 40)
              isTrackable = true;

            Double_t DR = MathUtils::DeltaR(ele->Phi(), ele->Eta(), fParticles->At(j)->Phi(),
                                            fParticles->At(j)->Eta());

            Double_t dPtOverPt = 5000.0;
            if (ele->TrackerTrk())
              dPtOverPt = (ele->TrackerTrk()->Pt() - fParticles->At(j)->Pt())/ele->TrackerTrk()->Pt();
            else
              dPtOverPt = (ele->Pt() - fParticles->At(j)->Pt())/ele->Pt();
            
            //remove Neutrals
            if (!fParticles->At(j)->IsNeutral()) { 
              if(fPrintLogs && DR < 0.3) { 
                          
                cout << "ChargedSimulatedParticle: " << j << " : " << fabs(dPtOverPt) << " : " << DR 
                     << "   " << fParticles->At(j)->PdgId()  
                     << " " << fParticles->At(j)->Status() << " " << fParticles->At(j)->Pt()
                     << " " << fParticles->At(j)->Eta() << " " << fParticles->At(j)->Phi()
                     << " produced at ";
                if (fParticles->At(j)->Mother())
                  cout << fParticles->At(j)->Mother()->DecayVertex().Rho();
                else 
                  cout << "0.0";
                cout << " decays at ";
                if (fParticles->At(j)->NDaughters() > 0)
                  cout << fParticles->At(j)->DecayVertex().Rho();
                else
                  cout << " Stable ";
                cout << endl;
              }

              //regular match
              if (DR < minDR && fabs(dPtOverPt) < 0.5 && DR < 0.3 && isStable && isTrackable) {
                minDR = DR;
                matchedSimChargedParticle = fParticles->At(j); 
                matchDeltaPtOverPt = dPtOverPt;
              }   

              Double_t productionVertexRho = 20000.0;
              if (fParticles->At(j)->Mother())
                productionVertexRho = fParticles->At(j)->Mother()->DecayVertex().Rho();

              //perform secondary match for GSF track only electrons
              if ( DR < 0.3 && fParticles->At(j)->AbsPdgId() == 11 && fParticles->At(j)->Pt() > 3.0
                   && productionVertexRho < minProductionVertexRho ) {
                minProductionVertexRho = productionVertexRho;
                secondaryMatchedSimChargedParticle = fParticles->At(j);
                secondaryMatchDeltaPtOverPt = dPtOverPt;
                secondaryMatchMinDR = DR;                
              }
              
              if (fabs(dPtOverPt) < fabs(bestDeltaPtOverPt) && DR < 0.3)
                bestDeltaPtOverPt = dPtOverPt;
           
            }
          }
        }
      
        if (!(matchedSimChargedParticle && minDR < 0.3)) {
          if (secondaryMatchedSimChargedParticle &&  !ele->TrackerTrk()) {
            matchedSimChargedParticle = secondaryMatchedSimChargedParticle;
            matchDeltaPtOverPt = secondaryMatchDeltaPtOverPt;
            minDR = secondaryMatchMinDR;
          }
        }
      
        Int_t FakeCategory = 0;
        Int_t SimParticleChargeMatchCode = 0;
        if (matchedSimChargedParticle && minDR < 0.3) {

          if (matchedSimChargedParticle->Charge() + ele->Charge() == 0) {
            SimParticleChargeMatchCode = 2*matchedSimChargedParticle->Charge() + ele->Charge();
          } else {
            SimParticleChargeMatchCode = matchedSimChargedParticle->Charge() + ele->Charge();
          }
          
          if (fPrintLogs) {
            cout << "Matched SimParticle: " << matchedSimChargedParticle->PdgId() << " " 
                 << matchedSimChargedParticle->Status() << " " << matchedSimChargedParticle->Pt() 
                 << " " << matchedSimChargedParticle->Eta() << " " << matchedSimChargedParticle->Phi()
                 << endl;
            cout << "ChargeMatchCode: " << SimParticleChargeMatchCode << endl;
          }
        
          //***********************************************************************************************
          //Make Histograms Using the Ancestor of the SimParticle matched to the fake electron
          //***********************************************************************************************  
          //Use matched SimTrack to trace back up the mother chain.
          const MCParticle *tmpParticle = matchedSimChargedParticle;
          while (tmpParticle->Mother() && 
                 (tmpParticle->Mother()->PdgId() != 92 && !tmpParticle->Mother()->IsParton())
            ) {
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
              if ((matchedAncestor->AbsPdgId() >= 400 && matchedAncestor->AbsPdgId() < 600) ||
                  (matchedAncestor->AbsPdgId() >= 4000 && matchedAncestor->AbsPdgId() < 6000)
                )
                cout << "Heavyflavor\n";
            }

            Double_t ancestorDR = MathUtils::DeltaR(ele->Phi(), ele->Eta(), matchedAncestor->Phi(),
                                                    matchedAncestor->Eta());
            Double_t ancestorDeltaPtOverPt = (ele->Pt() - matchedAncestor->Pt())/ele->Pt();
            if (fabs(ele->Eta()) < 1.479) {
              if (ele->Charge() > 0) {
                fCleanPlusBarrelElectronMatchedAncestorParticleID->Fill(matchedAncestor->PdgId());
                fCleanPlusBarrelElectronMatchedAncestorParticlePt->Fill(matchedAncestor->Pt());
                fCleanPlusBarrelElectronMatchedAncestorParticleDeltaR->Fill(ancestorDR);
                fCleanPlusBarrelElectronMatchedAncestorParticleDeltaPtOverPt->Fill(ancestorDeltaPtOverPt);
              } else {
                fCleanMinusBarrelElectronMatchedAncestorParticleID->Fill(matchedAncestor->PdgId());
                fCleanMinusBarrelElectronMatchedAncestorParticlePt->Fill(matchedAncestor->Pt());
                fCleanMinusBarrelElectronMatchedAncestorParticleDeltaR->Fill(ancestorDR);
                fCleanMinusBarrelElectronMatchedAncestorParticleDeltaPtOverPt->Fill(ancestorDeltaPtOverPt);
              }
            } else {
              if (ele->Charge() > 0) {
                fCleanPlusEndcapElectronMatchedAncestorParticleID->Fill(matchedAncestor->PdgId());
                fCleanPlusEndcapElectronMatchedAncestorParticlePt->Fill(matchedAncestor->Pt());
                fCleanPlusEndcapElectronMatchedAncestorParticleDeltaR->Fill(ancestorDR);
                fCleanPlusEndcapElectronMatchedAncestorParticleDeltaPtOverPt->Fill(ancestorDeltaPtOverPt);
              } else {
                fCleanMinusEndcapElectronMatchedAncestorParticleID->Fill(matchedAncestor->PdgId());
                fCleanMinusEndcapElectronMatchedAncestorParticlePt->Fill(matchedAncestor->Pt());
                fCleanMinusEndcapElectronMatchedAncestorParticleDeltaR->Fill(ancestorDR);
                fCleanMinusEndcapElectronMatchedAncestorParticleDeltaPtOverPt->Fill(ancestorDeltaPtOverPt);
              }
            }                        
          } //if we found the ancestor

        
          //***********************************************************************************************
          //Do Categorization according to photon conversion, heavy flavor, charged pion/kaon
          //***********************************************************************************************  
          //Use matched SimTrack to trace back up the mother chain.

          //Try to match to conversion
          if (matchedSimChargedParticle->AbsPdgId() == 11) {
            const MCParticle *mother = matchedSimChargedParticle->Mother();

            if (!mother) {
              if (fPrintLogs) cout << "no mother\n";
              FakeCategory = 7;
            } else {
              //try to find an ancestor that was a pi0 or a heavy meson
              if (fPrintLogs) cout << "find pi0 or heavy meson ancestor\n";
              const MCParticle *tmpParticle = matchedSimChargedParticle;
              while (tmpParticle->Mother() 
                     && 
                     tmpParticle->Mother()->PdgId() != 111 
                     && 
                     tmpParticle->Mother()->PdgId() != 221 
                     && 
                     tmpParticle->Mother()->PdgId() != 223 
                     && 
                     !(tmpParticle->Mother()->PdgId() == 22 && tmpParticle->Mother()->Mother() && 
                       tmpParticle->Mother()->Mother()->IsParton())
                     &&
                     !((tmpParticle->Mother()->AbsPdgId() >= 400 && 
                        tmpParticle->Mother()->AbsPdgId() < 600) ||
                       (tmpParticle->Mother()->AbsPdgId() >= 4000 && 
                        tmpParticle->Mother()->AbsPdgId() < 6000)
                       ) 
                     && 
                     tmpParticle->Mother()->AbsPdgId() != 24
                     && 
                     tmpParticle->Mother()->AbsPdgId() != 23                     
                     &&
                     tmpParticle->Mother()->PdgId() != 92 
                     && 
                     !tmpParticle->Mother()->IsParton()
                ) {           
                if (fPrintLogs) {
                  cout << "find : " << tmpParticle->Mother()->PdgId() << " " << tmpParticle->Mother()->Status() 
                       << " " << tmpParticle->Mother()->Pt() << " " << tmpParticle->Mother()->Eta() << " " 
                       << tmpParticle->Mother()->Phi() << endl;  
                }
                tmpParticle = tmpParticle->Mother();
              }
              const MCParticle *Ancestor = tmpParticle->Mother();
              if (fPrintLogs) {
                cout << "Found ancestor: " << Ancestor->PdgId() << " " 
                     << Ancestor->Status() << " " << Ancestor->Pt()
                     << " " << Ancestor->Eta() << " " 
                     << Ancestor->Phi() << "  HasElectronDaughter: " << Ancestor->HasDaughter(11)
                     << endl;
              }
                            
              if (Ancestor->PdgId() == 111 || Ancestor->PdgId() == 221 || Ancestor->PdgId() == 223) {
                FakeCategory = 1; //pi0->photon->conversion
              } else if (Ancestor->PdgId() == 22) {
                FakeCategory = 6; //prompt photon -> conversion
              } else if ((Ancestor->AbsPdgId() >= 400 && 
                          Ancestor->AbsPdgId() < 600) ||
                         (Ancestor->AbsPdgId() >= 4000 && 
                          Ancestor->AbsPdgId() < 6000) &&
                         Ancestor->HasDaughter(11)) {
                FakeCategory = 4; //heavy flavor 
              } else if (Ancestor->AbsPdgId() == 24 || Ancestor->AbsPdgId() == 23) {
                FakeCategory = 5;
              } else {
                FakeCategory = 7;
              }
            }
          } else if (matchedSimChargedParticle->AbsPdgId() == 211) {
            FakeCategory = 2;
          } else if (matchedSimChargedParticle->AbsPdgId() == 321) {
            FakeCategory = 3;
          } else {            
            FakeCategory = 7;
          }          
          
          if (FakeCategory == 7) if (fPrintLogs) cout << "Uncategorized\n";

          if (fabs(ele->Eta()) < 1.479) {
            if (ele->Charge() > 0) {
              fCleanPlusBarrelElectronFakeCategory->Fill(FakeCategory);
              } else {
                fCleanMinusBarrelElectronFakeCategory->Fill(FakeCategory);
              }
          } else {
            if (ele->Charge() > 0) {
              fCleanPlusEndcapElectronFakeCategory->Fill(FakeCategory);
            } else {
              fCleanMinusEndcapElectronFakeCategory->Fill(FakeCategory);
            }
          } 
            
          //***********************************************************************************************
          //Make Histograms Using the SimParticle matched to the fake electron
          //***********************************************************************************************  
          if (fabs(ele->Eta()) < 1.479) {
            if (ele->Charge() > 0) {
              fCleanPlusBarrelElectronMatchedSimChargedParticleID->Fill(matchedSimChargedParticle->PdgId());
              fCleanPlusBarrelElectronMatchedSimChargedParticlePt->Fill(matchedSimChargedParticle->Pt());
              fCleanPlusBarrelElectronMatchedSimChargedParticleDeltaR->Fill(minDR);
              fCleanPlusBarrelElectronMatchedSimChargedParticleDeltaPtOverPt->Fill(matchDeltaPtOverPt);
            } else {
              fCleanMinusBarrelElectronMatchedSimChargedParticleID->Fill(matchedSimChargedParticle->PdgId());
              fCleanMinusBarrelElectronMatchedSimChargedParticlePt->Fill(matchedSimChargedParticle->Pt());
              fCleanMinusBarrelElectronMatchedSimChargedParticleDeltaR->Fill(minDR);
              fCleanMinusBarrelElectronMatchedSimChargedParticleDeltaPtOverPt->Fill(matchDeltaPtOverPt);
            }
          } else {
            if (ele->Charge() > 0) {
              fCleanPlusEndcapElectronMatchedSimChargedParticleID->Fill(matchedSimChargedParticle->PdgId());
              fCleanPlusEndcapElectronMatchedSimChargedParticlePt->Fill(matchedSimChargedParticle->Pt());
              fCleanPlusEndcapElectronMatchedSimChargedParticleDeltaR->Fill(minDR);
              fCleanPlusEndcapElectronMatchedSimChargedParticleDeltaPtOverPt->Fill(matchDeltaPtOverPt);
            } else {
              fCleanMinusEndcapElectronMatchedSimChargedParticleID->Fill(matchedSimChargedParticle->PdgId());
              fCleanMinusEndcapElectronMatchedSimChargedParticlePt->Fill(matchedSimChargedParticle->Pt());
              fCleanMinusEndcapElectronMatchedSimChargedParticleDeltaR->Fill(minDR);
              fCleanMinusEndcapElectronMatchedSimChargedParticleDeltaPtOverPt->Fill(matchDeltaPtOverPt);
            }
          }        
        } else {       
          if (fPrintLogs) cout << "No Sim match\n";
          printHepMCTable = true;
          //histogram the bestDeltaPtOverPt inside 0.3 cone for ones which didn't pass
          
          if (fabs(ele->Eta()) < 1.479) {
            if (ele->Charge() > 0) {
              fCleanPlusBarrelElectronFakeCategory->Fill(0);
              fCleanPlusBarrelElectronUnmatchedSimChargedParticleBestDeltaPtOverPt->Fill(bestDeltaPtOverPt);
            } else {
              fCleanMinusBarrelElectronFakeCategory->Fill(0);
              fCleanMinusBarrelElectronUnmatchedSimChargedParticleBestDeltaPtOverPt->Fill(bestDeltaPtOverPt);
            }
          } else {
            if (ele->Charge() > 0) {
              fCleanPlusEndcapElectronFakeCategory->Fill(0);
              fCleanPlusEndcapElectronUnmatchedSimChargedParticleBestDeltaPtOverPt->Fill(bestDeltaPtOverPt);
            } else {
              fCleanMinusEndcapElectronFakeCategory->Fill(0);
              fCleanMinusEndcapElectronUnmatchedSimChargedParticleBestDeltaPtOverPt->Fill(bestDeltaPtOverPt);
            }
          }             
        }
        //***********************************************************************************************
        //Get Dilepton Charge Code
        //***********************************************************************************************  
        Double_t DileptonChargeCode = 0;
        if (RealWLepton) {
          if (RealWLepton->Charge() + ele->Charge() == 0) {
            DileptonChargeCode = 2*RealWLepton->Charge() + ele->Charge();
          } else {
            DileptonChargeCode = RealWLepton->Charge() + ele->Charge();
          }
        }
        //***********************************************************************************************
        //Fill Electron variables for various Fake Categories
        //***********************************************************************************************  
        if (FakeCategory == 1) {
          fConversionFakeElectronPt->Fill(ele->Pt());
          fConversionFakeElectronEta->Fill(ele->Eta());
          fConversionFakeElectronPhi->Fill(ele->Phi());
          fConversionFakeElectronTrkIso->Fill(ele->TrackIsolation());
          fConversionFakeElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
          fConversionFakeElectronHOverE->Fill(ele->HadronicOverEm());
          fConversionFakeElectronEOverP->Fill(ele->ESuperClusterOverP());
          fConversionFakeElectronSigmaEtaEta->Fill(ele->CovEtaEta());
          fConversionFakeElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
          fConversionFakeElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
          fConversionFakeElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
          fConversionFakeElectronfBrem->Fill(ele->FBrem());
          fConversionFakeElectronClassification->Fill(ele->Classification());
          fDileptonChargeConversionFakes->Fill(DileptonChargeCode);
        } else if (FakeCategory == 2) {
          fChargedPionFakeElectronPt->Fill(ele->Pt());
          fChargedPionFakeElectronEta->Fill(ele->Eta());
          fChargedPionFakeElectronPhi->Fill(ele->Phi());
          fChargedPionFakeElectronTrkIso->Fill(ele->TrackIsolation());
          fChargedPionFakeElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
          fChargedPionFakeElectronHOverE->Fill(ele->HadronicOverEm());
          fChargedPionFakeElectronEOverP->Fill(ele->ESuperClusterOverP());
          fChargedPionFakeElectronSigmaEtaEta->Fill(ele->CovEtaEta());
          fChargedPionFakeElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
          fChargedPionFakeElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
          fChargedPionFakeElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
          fChargedPionFakeElectronfBrem->Fill(ele->FBrem());
          fChargedPionFakeElectronClassification->Fill(ele->Classification());
          fDileptonChargeChargedPionFakes->Fill(DileptonChargeCode);
        } else if (FakeCategory == 3) {
          fChargedKaonFakeElectronPt->Fill(ele->Pt());
          fChargedKaonFakeElectronEta->Fill(ele->Eta());
          fChargedKaonFakeElectronPhi->Fill(ele->Phi());
          fChargedKaonFakeElectronTrkIso->Fill(ele->TrackIsolation());
          fChargedKaonFakeElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
          fChargedKaonFakeElectronHOverE->Fill(ele->HadronicOverEm());
          fChargedKaonFakeElectronEOverP->Fill(ele->ESuperClusterOverP());
          fChargedKaonFakeElectronSigmaEtaEta->Fill(ele->CovEtaEta());
          fChargedKaonFakeElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
          fChargedKaonFakeElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
          fChargedKaonFakeElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
          fChargedKaonFakeElectronfBrem->Fill(ele->FBrem());
          fChargedKaonFakeElectronClassification->Fill(ele->Classification());
          fDileptonChargeChargedKaonFakes->Fill(DileptonChargeCode);
        } else if (FakeCategory == 4) {
          fHeavyFlavorFakeElectronPt->Fill(ele->Pt());
          fHeavyFlavorFakeElectronEta->Fill(ele->Eta());
          fHeavyFlavorFakeElectronPhi->Fill(ele->Phi());
          fHeavyFlavorFakeElectronTrkIso->Fill(ele->TrackIsolation());
          fHeavyFlavorFakeElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
          fHeavyFlavorFakeElectronHOverE->Fill(ele->HadronicOverEm());
          fHeavyFlavorFakeElectronEOverP->Fill(ele->ESuperClusterOverP());
          fHeavyFlavorFakeElectronSigmaEtaEta->Fill(ele->CovEtaEta());
          fHeavyFlavorFakeElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
          fHeavyFlavorFakeElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
          fHeavyFlavorFakeElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
          fHeavyFlavorFakeElectronfBrem->Fill(ele->FBrem());
          fHeavyFlavorFakeElectronClassification->Fill(ele->Classification());
          fDileptonChargeHeavyFlavorFakes->Fill(DileptonChargeCode);
        } else if (FakeCategory == 5) {
          fBremFakeElectronPt->Fill(ele->Pt());
          fBremFakeElectronEta->Fill(ele->Eta());
          fBremFakeElectronPhi->Fill(ele->Phi());
          fBremFakeElectronTrkIso->Fill(ele->TrackIsolation());
          fBremFakeElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
          fBremFakeElectronHOverE->Fill(ele->HadronicOverEm());
          fBremFakeElectronEOverP->Fill(ele->ESuperClusterOverP());
          fBremFakeElectronSigmaEtaEta->Fill(ele->CovEtaEta());
          fBremFakeElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
          fBremFakeElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
          fBremFakeElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
          fBremFakeElectronfBrem->Fill(ele->FBrem());
          fBremFakeElectronClassification->Fill(ele->Classification());
          fDileptonChargeBremFakes->Fill(DileptonChargeCode);
        } else if (FakeCategory == 6) {
          fPromptPhotonFakeElectronPt->Fill(ele->Pt());
          fPromptPhotonFakeElectronEta->Fill(ele->Eta());
          fPromptPhotonFakeElectronPhi->Fill(ele->Phi());
          fPromptPhotonFakeElectronTrkIso->Fill(ele->TrackIsolation());
          fPromptPhotonFakeElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
          fPromptPhotonFakeElectronHOverE->Fill(ele->HadronicOverEm());
          fPromptPhotonFakeElectronEOverP->Fill(ele->ESuperClusterOverP());
          fPromptPhotonFakeElectronSigmaEtaEta->Fill(ele->CovEtaEta());
          fPromptPhotonFakeElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
          fPromptPhotonFakeElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
          fPromptPhotonFakeElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
          fPromptPhotonFakeElectronfBrem->Fill(ele->FBrem());
          fPromptPhotonFakeElectronClassification->Fill(ele->Classification());
          fDileptonChargePromptPhotonFakes->Fill(DileptonChargeCode);
        } else if (FakeCategory == 7) {
          fUncategorizedFakeElectronPt->Fill(ele->Pt());
          fUncategorizedFakeElectronEta->Fill(ele->Eta());
          fUncategorizedFakeElectronPhi->Fill(ele->Phi());
          fUncategorizedFakeElectronTrkIso->Fill(ele->TrackIsolation());
          fUncategorizedFakeElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
          fUncategorizedFakeElectronHOverE->Fill(ele->HadronicOverEm());
          fUncategorizedFakeElectronEOverP->Fill(ele->ESuperClusterOverP());
          fUncategorizedFakeElectronSigmaEtaEta->Fill(ele->CovEtaEta());
          fUncategorizedFakeElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
          fUncategorizedFakeElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
          fUncategorizedFakeElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
          fUncategorizedFakeElectronfBrem->Fill(ele->FBrem());
          fUncategorizedFakeElectronClassification->Fill(ele->Classification());
          fDileptonChargeUncategorizedFakes->Fill(DileptonChargeCode);
        } else if (FakeCategory == 0) {
          fDileptonChargeNoSimMatchFakes->Fill(DileptonChargeCode);
        }

        MDB(kAnalysis, 5) { printHepMCTable = true; }          
        //Print out HepMC Table
        if(fPrintLogs) {      
          cout << "FakeCategory = " << FakeCategory << endl;
          //print out HepMC table
          if (printHepMCTable)
            GeneratorTools::PrintHepMCTable(fParticles, true, -1);
          
          //print out all particles and all descendants that are within dR of 0.3 of the electron
          //GeneratorTools::PrintNearbyParticles(fParticles, ele->Eta(), ele->Phi(), 0.3);
        }  
   
        //***********************************************************************************************
        //Fill W - fake charge correlation histograms for Clean Electrons
        //***********************************************************************************************  
        if (RealWLepton) {
          if (RealWLepton->Charge() == ele->Charge()) {
            fSameSignFakeCleanElectronPt->Fill(ele->Pt());
            fSameSignFakeCleanElectronEta->Fill(ele->Eta());
            fSameSignFakeCleanElectronPhi->Fill(ele->Phi());
            fSameSignFakeCleanElectronTrkIso->Fill(ele->TrackIsolation());
            fSameSignFakeCleanElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
            fSameSignFakeCleanElectronHOverE->Fill(ele->HadronicOverEm());
            fSameSignFakeCleanElectronESuperClusterOverP->Fill(ele->ESuperClusterOverP());
            fSameSignFakeCleanElectronSigmaEtaEta->Fill(ele->CovEtaEta());
            fSameSignFakeCleanElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
            fSameSignFakeCleanElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
            fSameSignFakeCleanElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
            fSameSignFakeCleanElectronfBrem->Fill(ele->FBrem());
            fSameSignFakeCleanElectronClassification->Fill(ele->Classification());
            fSameSignFakeCleanElectronD0->Fill(fabs(ele->Trk()->D0Corrected(primaryVertex)));
            fSameSignRealToFakeCleanElectronDeltaEta->Fill(fabs(RealWLepton->Eta() - ele->Eta()));
            fSameSignRealToFakeCleanElectronDeltaPhi->Fill(MathUtils::DeltaPhi(RealWLepton->Phi(),ele->Phi()));
            fSameSignRealToFakeCleanElectronDeltaR->Fill(MathUtils::DeltaR(RealWLepton->Phi(),RealWLepton->Eta(),
                                                                           ele->Phi(),ele->Eta()));
            fSameSignFakeCleanElectronCategory->Fill(FakeCategory);
            if (FakeCategory == 1) fSameSignConversionCategorySimParticleToIsolatedTrackChargeMatch->Fill(SimParticleChargeMatchCode);
          } else {
            fOppositeSignFakeCleanElectronPt->Fill(ele->Pt());
            fOppositeSignFakeCleanElectronEta->Fill(ele->Eta());
            fOppositeSignFakeCleanElectronPhi->Fill(ele->Phi());
            fOppositeSignFakeCleanElectronTrkIso->Fill(ele->TrackIsolation());
            fOppositeSignFakeCleanElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
            fOppositeSignFakeCleanElectronHOverE->Fill(ele->HadronicOverEm());
            fOppositeSignFakeCleanElectronESuperClusterOverP->Fill(ele->ESuperClusterOverP());
            fOppositeSignFakeCleanElectronSigmaEtaEta->Fill(ele->CovEtaEta());
            fOppositeSignFakeCleanElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
            fOppositeSignFakeCleanElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
            fOppositeSignFakeCleanElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
            fOppositeSignFakeCleanElectronfBrem->Fill(ele->FBrem());
            fOppositeSignFakeCleanElectronClassification->Fill(ele->Classification());
            fOppositeSignFakeCleanElectronD0->Fill(fabs(ele->Trk()->D0Corrected(primaryVertex)));
            fOppositeSignRealToFakeCleanElectronDeltaEta->Fill(fabs(RealWLepton->Eta() - ele->Eta()));
            fOppositeSignRealToFakeCleanElectronDeltaPhi->Fill(MathUtils::DeltaPhi(RealWLepton->Phi(),ele->Phi()));
            fOppositeSignRealToFakeCleanElectronDeltaR->Fill(MathUtils::DeltaR(RealWLepton->Phi(),RealWLepton->Eta(),
                                                                                ele->Phi(),ele->Eta()));
            fOppositeSignFakeCleanElectronCategory->Fill(FakeCategory);
            if (FakeCategory == 1) fOppositeSignConversionCategorySimParticleToIsolatedTrackChargeMatch->Fill(SimParticleChargeMatchCode);
          }
        }
      } //if electron is not W electron     
    } //loop over Clean electrons.
  } //if ngenjet == 1, ncleanlepton == 2

  //***********************************************************************************************
  //For Reco electrons and isolated tracks, only require 1 clean lepton
  //***********************************************************************************************      
  if (fRequireFirstLepton && CleanLeptons->GetEntries() >= 1 && GenLeptons->GetEntries() == 1 && NGenJets == 1) {
    cout << "genlepton\n";
    //***********************************************************************************************
    //fake charge correlation histograms for RECO Electrons
    //***********************************************************************************************          
    for (UInt_t i=0; i<fElectrons->GetEntries(); i++) {
      const Electron *ele = fElectrons->At(i);

      //Veto denominators matching to real genleptons
      Bool_t IsMCLepton = false;
      for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
        if (MathUtils::DeltaR(ele->Phi(), ele->Eta(),
                              GenLeptonsAndTaus->At(l)->Phi(), 
                              GenLeptonsAndTaus->At(l)->Eta()) < 0.3 &&
            GenLeptonsAndTaus->At(l)->AbsPdgId() == 11
          ) {
          
          IsMCLepton = true;
        }
      }
 
      MDB(kAnalysis, 8) cout << "electron " << i << " " << ele->Pt() << " " << ele->Eta() << " " << ele->Phi() << " isMCLepton=" << IsMCLepton << endl;


      if (ele->Pt() > 10.0 && !IsMCLepton && RealWLepton) {
        
        const MCParticle *matchedSimParticle = NULL;
        
        Int_t printDebugLevel  = 0; MDB(kAnalysis, 8) printDebugLevel = 8;
        Bool_t isGsfTrack;
        if (ele->TrackerTrk()) {
          isGsfTrack = false;
          matchedSimParticle = GeneratorTools::MatchElectronToSimParticle(fParticles, ele->TrackerTrk(), isGsfTrack, printDebugLevel, false);
        } else {
          isGsfTrack = true;
          matchedSimParticle = GeneratorTools::MatchElectronToSimParticle(fParticles, ele->GsfTrk(), isGsfTrack, printDebugLevel,false);
        }

        MDB(kAnalysis, 8) {       
          cout << "********************************************************************************\n";
          cout << "Real W Lepton : ";
          if (RealWLepton->ObjType() == kElectron)
            cout << "e ";
          else if (RealWLepton->ObjType() == kMuon)
            cout << "mu ";
          else
            cout << "unknown ";

          cout  << RealWLepton->Pt() << " " << RealWLepton->Eta() << " " 
               << RealWLepton->Phi() << " , Track: Charge" << RealWLepton->Trk()->Charge() << " "
               << RealWLepton->Trk()->Pt() << " " << RealWLepton->Trk()->Eta() << " " 
               << RealWLepton->Trk()->Phi() << " ";
          if (RealWLepton->TrackerTrk()) 
            cout << " TrackerTrk: " << RealWLepton->TrackerTrk()->Charge() << " "
                 << RealWLepton->TrackerTrk()->Pt() << " " << RealWLepton->TrackerTrk()->Eta() << " " 
                 << RealWLepton->TrackerTrk()->Phi()
                 << endl;

          cout << "RECO Electron : " << ele->Pt() << " " << ele->Eta() << " " << ele->Phi();
          if (ele->GsfTrk()) {
            cout << "  | GsfTrack: Charge " << ele->GsfTrk()->Charge() << "   " 
                 << ele->GsfTrk()->P() << " " 
                 << ele->GsfTrk()->Pt() << " " << ele->GsfTrk()->Eta() << " "
                 << ele->GsfTrk()->Phi();
          }
          cout << " | ";
          if (ele->TrackerTrk()) {
            cout << " TrackerTrack: Charge " << ele->TrackerTrk()->Charge() << "   " 
                 << ele->TrackerTrk()->P() << " "
                 << ele->TrackerTrk()->Pt() << " " << ele->TrackerTrk()->Eta()
                 << " " << ele->TrackerTrk()->Phi();
          }
          cout << " SuperCluster: " << ele->SCluster()->Energy() << " " << ele->SCluster()->Eta() 
               << " " << ele->SCluster()->Phi() << endl;
          cout << "E/P: " << ele->ESuperClusterOverP() << " , Had/EM: " << ele->HadronicOverEm() 
               << " , TrackIso: " << ele->TrackIsolation() << " , JurassicEcalIso: "
               << ele->EcalJurassicIsolation() << " , fBrem: " 
               << (ele->PIn() - ele->POut())/ ele->PIn()
               << endl;
          
          cout << endl;        
        }
        
        
        Int_t FakeCategory = GeneratorTools::CategorizeFakeElectron(matchedSimParticle);
        
        MDB(kAnalysis, 8) cout << "RecoElectron Fake Category: " << FakeCategory << endl;

        Int_t SimParticleChargeMatchCode = 0;
        if (matchedSimParticle) {
          if (matchedSimParticle->Charge() + ele->Charge() == 0) {
            SimParticleChargeMatchCode = 2*matchedSimParticle->Charge() + ele->Charge();
          } else {
            SimParticleChargeMatchCode = matchedSimParticle->Charge() + ele->Charge();
          }
        }
        MDB(kAnalysis, 8) cout << "ChargeMatchCode: " << SimParticleChargeMatchCode << endl;
        
        MDB(kAnalysis, 8) {
          if (FakeCategory == 7)
            GeneratorTools::PrintHepMCTable(fParticles, true, -1);
        }


        //*******************************************************************************************
        //RECO Electrons
        //*******************************************************************************************      
        if (RealWLepton->Charge() == ele->Charge()) {
          fSameSignFakeElectronPt->Fill(ele->Pt());
          fSameSignFakeElectronEta->Fill(ele->Eta());
          fSameSignFakeElectronPhi->Fill(ele->Phi());
          fSameSignFakeElectronTrkIso->Fill(ele->TrackIsolation());
          fSameSignFakeElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
          fSameSignFakeElectronHOverE->Fill(ele->HadronicOverEm());
          fSameSignFakeElectronESuperClusterOverP->Fill(ele->ESuperClusterOverP());
          fSameSignFakeElectronSigmaEtaEta->Fill(ele->CovEtaEta());
          fSameSignFakeElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
          fSameSignFakeElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
          fSameSignFakeElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
          fSameSignFakeElectronfBrem->Fill(ele->FBrem());
          fSameSignFakeElectronClassification->Fill(ele->Classification());
          fSameSignFakeElectronD0->Fill(fabs(ele->Trk()->D0Corrected(primaryVertex)));
          fSameSignRealToFakeElectronDeltaEta->Fill(fabs(RealWLepton->Eta() - ele->Eta()));
          fSameSignRealToFakeElectronDeltaPhi->Fill(MathUtils::DeltaPhi(RealWLepton->Phi(),ele->Phi()));
          fSameSignRealToFakeElectronDeltaR->Fill(MathUtils::DeltaR(RealWLepton->Phi(),RealWLepton->Eta(),
                                                                    ele->Phi(),ele->Eta()));
          fSameSignFakeElectronCategory->Fill(FakeCategory);
          if (FakeCategory == 1) fSameSignConversionCategorySimParticleToElectronChargeMatch->Fill(SimParticleChargeMatchCode);
        } else {
          fOppositeSignFakeElectronPt->Fill(ele->Pt());
          fOppositeSignFakeElectronEta->Fill(ele->Eta());
          fOppositeSignFakeElectronPhi->Fill(ele->Phi());
          fOppositeSignFakeElectronTrkIso->Fill(ele->TrackIsolation());
          fOppositeSignFakeElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
          fOppositeSignFakeElectronHOverE->Fill(ele->HadronicOverEm());
          fOppositeSignFakeElectronESuperClusterOverP->Fill(ele->ESuperClusterOverP());
          fOppositeSignFakeElectronSigmaEtaEta->Fill(ele->CovEtaEta());
          fOppositeSignFakeElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
          fOppositeSignFakeElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
          fOppositeSignFakeElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
          fOppositeSignFakeElectronfBrem->Fill(ele->FBrem());
          fOppositeSignFakeElectronClassification->Fill(ele->Classification());
          fOppositeSignFakeElectronD0->Fill(fabs(ele->Trk()->D0Corrected(primaryVertex)));
          fOppositeSignRealToFakeElectronDeltaEta->Fill(fabs(RealWLepton->Eta() - ele->Eta()));
          fOppositeSignRealToFakeElectronDeltaPhi->Fill(MathUtils::DeltaPhi(RealWLepton->Phi(),ele->Phi()));
          fOppositeSignRealToFakeElectronDeltaR->Fill(MathUtils::DeltaR(RealWLepton->Phi(),RealWLepton->Eta(),
                                                                        ele->Phi(),ele->Eta()));
          fOppositeSignFakeElectronCategory->Fill(FakeCategory);
          if (FakeCategory == 1) fOppositeSignConversionCategorySimParticleToElectronChargeMatch->Fill(SimParticleChargeMatchCode);
        }

        //*******************************************************************************************
        //Isolated RECO Electrons
        //*******************************************************************************************      
        if (ele->TrackIsolation()+ele->EcalJurassicIsolation() - 1.5 < 10.0) {
          if (RealWLepton->Charge() == ele->Charge()) {
            fSameSignFakeIsolatedElectronPt->Fill(ele->Pt());
            fSameSignFakeIsolatedElectronEta->Fill(ele->Eta());
            fSameSignFakeIsolatedElectronPhi->Fill(ele->Phi());
            fSameSignFakeIsolatedElectronTrkIso->Fill(ele->TrackIsolation());
            fSameSignFakeIsolatedElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
            fSameSignFakeIsolatedElectronHOverE->Fill(ele->HadronicOverEm());
            fSameSignFakeIsolatedElectronESuperClusterOverP->Fill(ele->ESuperClusterOverP());
            fSameSignFakeIsolatedElectronSigmaEtaEta->Fill(ele->CovEtaEta());
            fSameSignFakeIsolatedElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
            fSameSignFakeIsolatedElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
            fSameSignFakeIsolatedElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
            fSameSignFakeIsolatedElectronfBrem->Fill(ele->FBrem());
            fSameSignFakeIsolatedElectronClassification->Fill(ele->Classification());
            fSameSignFakeIsolatedElectronD0->Fill(fabs(ele->Trk()->D0Corrected(primaryVertex)));
            fSameSignRealToFakeIsolatedElectronDeltaEta->Fill(fabs(RealWLepton->Eta() - ele->Eta()));
            fSameSignRealToFakeIsolatedElectronDeltaPhi->Fill(MathUtils::DeltaPhi(RealWLepton->Phi(),ele->Phi()));
            fSameSignRealToFakeIsolatedElectronDeltaR->Fill(MathUtils::DeltaR(RealWLepton->Phi(),RealWLepton->Eta(),
                                                                           ele->Phi(),ele->Eta()));
            fSameSignFakeIsolatedElectronCategory->Fill(FakeCategory);
            if (FakeCategory == 1) fSameSignConversionCategorySimParticleToIsolatedElectronChargeMatch->Fill(SimParticleChargeMatchCode);
          } else {
            fOppositeSignFakeIsolatedElectronPt->Fill(ele->Pt());
            fOppositeSignFakeIsolatedElectronEta->Fill(ele->Eta());
            fOppositeSignFakeIsolatedElectronPhi->Fill(ele->Phi());
            fOppositeSignFakeIsolatedElectronTrkIso->Fill(ele->TrackIsolation());
            fOppositeSignFakeIsolatedElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
            fOppositeSignFakeIsolatedElectronHOverE->Fill(ele->HadronicOverEm());
            fOppositeSignFakeIsolatedElectronESuperClusterOverP->Fill(ele->ESuperClusterOverP());
            fOppositeSignFakeIsolatedElectronSigmaEtaEta->Fill(ele->CovEtaEta());
            fOppositeSignFakeIsolatedElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
            fOppositeSignFakeIsolatedElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
            fOppositeSignFakeIsolatedElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
            fOppositeSignFakeIsolatedElectronfBrem->Fill(ele->FBrem());
            fOppositeSignFakeIsolatedElectronClassification->Fill(ele->Classification());
            fOppositeSignFakeIsolatedElectronD0->Fill(fabs(ele->Trk()->D0Corrected(primaryVertex)));
            fOppositeSignRealToFakeIsolatedElectronDeltaEta->Fill(fabs(RealWLepton->Eta() - ele->Eta()));
            fOppositeSignRealToFakeIsolatedElectronDeltaPhi->Fill(MathUtils::DeltaPhi(RealWLepton->Phi(),ele->Phi()));
            fOppositeSignRealToFakeIsolatedElectronDeltaR->Fill(MathUtils::DeltaR(RealWLepton->Phi(),RealWLepton->Eta(),
                                                                                  ele->Phi(),ele->Eta()));
            fOppositeSignFakeIsolatedElectronCategory->Fill(FakeCategory);
            if (FakeCategory == 1) fOppositeSignConversionCategorySimParticleToIsolatedElectronChargeMatch->Fill(SimParticleChargeMatchCode);
          }
          
          //*****************************************************************************************
          //Isolated Loose RECO Electrons
          //*****************************************************************************************      
          if (ele->PassLooseID()) {
            if (RealWLepton->Charge() == ele->Charge()) {
              fSameSignFakeIsolatedLooseElectronPt->Fill(ele->Pt());
              fSameSignFakeIsolatedLooseElectronEta->Fill(ele->Eta());
              fSameSignFakeIsolatedLooseElectronPhi->Fill(ele->Phi());
              fSameSignFakeIsolatedLooseElectronTrkIso->Fill(ele->TrackIsolation());
              fSameSignFakeIsolatedLooseElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
              fSameSignFakeIsolatedLooseElectronHOverE->Fill(ele->HadronicOverEm());
              fSameSignFakeIsolatedLooseElectronESuperClusterOverP->Fill(ele->ESuperClusterOverP());
              fSameSignFakeIsolatedLooseElectronSigmaEtaEta->Fill(ele->CovEtaEta());
              fSameSignFakeIsolatedLooseElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
              fSameSignFakeIsolatedLooseElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
              fSameSignFakeIsolatedLooseElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
              fSameSignFakeIsolatedLooseElectronfBrem->Fill(ele->FBrem());
              fSameSignFakeIsolatedLooseElectronClassification->Fill(ele->Classification());
              fSameSignFakeIsolatedLooseElectronD0->Fill(fabs(ele->Trk()->D0Corrected(primaryVertex)));
              fSameSignRealToFakeIsolatedLooseElectronDeltaEta->Fill(fabs(RealWLepton->Eta() - ele->Eta()));
              fSameSignRealToFakeIsolatedLooseElectronDeltaPhi->Fill(MathUtils::DeltaPhi(RealWLepton->Phi(),ele->Phi()));
              fSameSignRealToFakeIsolatedLooseElectronDeltaR->Fill(MathUtils::DeltaR(RealWLepton->Phi(),RealWLepton->Eta(),
                                                                                     ele->Phi(),ele->Eta()));
              fSameSignFakeIsolatedLooseElectronCategory->Fill(FakeCategory);
              if (FakeCategory == 1) fSameSignConversionCategorySimParticleToIsolatedLooseElectronChargeMatch->Fill(SimParticleChargeMatchCode);
            } else {
              fOppositeSignFakeIsolatedLooseElectronPt->Fill(ele->Pt());
              fOppositeSignFakeIsolatedLooseElectronEta->Fill(ele->Eta());
              fOppositeSignFakeIsolatedLooseElectronPhi->Fill(ele->Phi());
              fOppositeSignFakeIsolatedLooseElectronTrkIso->Fill(ele->TrackIsolation());
              fOppositeSignFakeIsolatedLooseElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
              fOppositeSignFakeIsolatedLooseElectronHOverE->Fill(ele->HadronicOverEm());
              fOppositeSignFakeIsolatedLooseElectronESuperClusterOverP->Fill(ele->ESuperClusterOverP());
              fOppositeSignFakeIsolatedLooseElectronSigmaEtaEta->Fill(ele->CovEtaEta());
              fOppositeSignFakeIsolatedLooseElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
              fOppositeSignFakeIsolatedLooseElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
              fOppositeSignFakeIsolatedLooseElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
              fOppositeSignFakeIsolatedLooseElectronfBrem->Fill(ele->FBrem());
              fOppositeSignFakeIsolatedLooseElectronClassification->Fill(ele->Classification());
              fOppositeSignFakeIsolatedLooseElectronD0->Fill(fabs(ele->Trk()->D0Corrected(primaryVertex)));
              fOppositeSignRealToFakeIsolatedLooseElectronDeltaEta->Fill(fabs(RealWLepton->Eta() - ele->Eta()));
              fOppositeSignRealToFakeIsolatedLooseElectronDeltaPhi->Fill(MathUtils::DeltaPhi(RealWLepton->Phi(),ele->Phi()));
              fOppositeSignRealToFakeIsolatedLooseElectronDeltaR->Fill(MathUtils::DeltaR(RealWLepton->Phi(),RealWLepton->Eta(),
                                                                                         ele->Phi(),ele->Eta()));
              fOppositeSignFakeIsolatedLooseElectronCategory->Fill(FakeCategory);
              if (FakeCategory == 1) fOppositeSignConversionCategorySimParticleToIsolatedLooseElectronChargeMatch->Fill(SimParticleChargeMatchCode);
            }        
          }//if loose ele

          //*****************************************************************************************
          //Isolated Tight RECO Electrons
          //*****************************************************************************************      
          if (ele->PassTightID()) {
            if (RealWLepton->Charge() == ele->Charge()) {
              fSameSignFakeIsolatedTightElectronPt->Fill(ele->Pt());
              fSameSignFakeIsolatedTightElectronEta->Fill(ele->Eta());
              fSameSignFakeIsolatedTightElectronPhi->Fill(ele->Phi());
              fSameSignFakeIsolatedTightElectronTrkIso->Fill(ele->TrackIsolation());
              fSameSignFakeIsolatedTightElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
              fSameSignFakeIsolatedTightElectronHOverE->Fill(ele->HadronicOverEm());
              fSameSignFakeIsolatedTightElectronESuperClusterOverP->Fill(ele->ESuperClusterOverP());
              fSameSignFakeIsolatedTightElectronSigmaEtaEta->Fill(ele->CovEtaEta());
              fSameSignFakeIsolatedTightElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
              fSameSignFakeIsolatedTightElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
              fSameSignFakeIsolatedTightElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
              fSameSignFakeIsolatedTightElectronfBrem->Fill(ele->FBrem());
              fSameSignFakeIsolatedTightElectronClassification->Fill(ele->Classification());
              fSameSignFakeIsolatedTightElectronD0->Fill(fabs(ele->Trk()->D0Corrected(primaryVertex)));
              fSameSignRealToFakeIsolatedTightElectronDeltaEta->Fill(fabs(RealWLepton->Eta() - ele->Eta()));
              fSameSignRealToFakeIsolatedTightElectronDeltaPhi->Fill(MathUtils::DeltaPhi(RealWLepton->Phi(),ele->Phi()));
              fSameSignRealToFakeIsolatedTightElectronDeltaR->Fill(MathUtils::DeltaR(RealWLepton->Phi(),RealWLepton->Eta(),
                                                                                     ele->Phi(),ele->Eta()));
              fSameSignFakeIsolatedTightElectronCategory->Fill(FakeCategory);
              if (FakeCategory == 1) fSameSignConversionCategorySimParticleToIsolatedTightElectronChargeMatch->Fill(SimParticleChargeMatchCode);
            } else {
              fOppositeSignFakeIsolatedTightElectronPt->Fill(ele->Pt());
              fOppositeSignFakeIsolatedTightElectronEta->Fill(ele->Eta());
              fOppositeSignFakeIsolatedTightElectronPhi->Fill(ele->Phi());
              fOppositeSignFakeIsolatedTightElectronTrkIso->Fill(ele->TrackIsolation());
              fOppositeSignFakeIsolatedTightElectronJurassicIso->Fill(ele->EcalJurassicIsolation());
              fOppositeSignFakeIsolatedTightElectronHOverE->Fill(ele->HadronicOverEm());
              fOppositeSignFakeIsolatedTightElectronESuperClusterOverP->Fill(ele->ESuperClusterOverP());
              fOppositeSignFakeIsolatedTightElectronSigmaEtaEta->Fill(ele->CovEtaEta());
              fOppositeSignFakeIsolatedTightElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
              fOppositeSignFakeIsolatedTightElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
              fOppositeSignFakeIsolatedTightElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
              fOppositeSignFakeIsolatedTightElectronfBrem->Fill(ele->FBrem());
              fOppositeSignFakeIsolatedTightElectronClassification->Fill(ele->Classification());
              fOppositeSignFakeIsolatedTightElectronD0->Fill(fabs(ele->Trk()->D0Corrected(primaryVertex)));
              fOppositeSignRealToFakeIsolatedTightElectronDeltaEta->Fill(fabs(RealWLepton->Eta() - ele->Eta()));
              fOppositeSignRealToFakeIsolatedTightElectronDeltaPhi->Fill(MathUtils::DeltaPhi(RealWLepton->Phi(),ele->Phi()));
              fOppositeSignRealToFakeIsolatedTightElectronDeltaR->Fill(MathUtils::DeltaR(RealWLepton->Phi(),RealWLepton->Eta(),
                                                                                         ele->Phi(),ele->Eta()));
              fOppositeSignFakeIsolatedTightElectronCategory->Fill(FakeCategory);
              if (FakeCategory == 1) fOppositeSignConversionCategorySimParticleToIsolatedTightElectronChargeMatch->Fill(SimParticleChargeMatchCode);
            }
          }//if tight ele
        }// if isolated

        
      } //if found RealWLepton and RealWLepton != ele
    } //loop over all RECO electrons
  
                   
    //***********************************************************************************************
    //Find Track TrackDenominator Objects for Electrons
    //Isolated Tracks
    //***********************************************************************************************  
    for (UInt_t i=0; i<fTracks->GetEntries(); i++) {
      const Track *track = fTracks->At(i);
      
      Double_t trackIsolation = IsolationTools::TrackIsolation(track, 0.3, 0.015, 1.0, 
                                                               1000.0, fTracks);
      //Veto denominators matching to real genleptons
      Bool_t IsMCLepton = false;
      for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
        if (MathUtils::DeltaR(track->Phi(), track->Eta(),
                              GenLeptonsAndTaus->At(l)->Phi(), 
                              GenLeptonsAndTaus->At(l)->Eta()) < 0.3) {
          IsMCLepton = true;
        }
      }
      
      //define denominator cuts
      if (track->Pt() > 10.0 && !(fVetoGeneratorLeptons && IsMCLepton) && RealWLepton
        ) {
        
        MDB(kAnalysis, 8) cout << "found track: " << track->Pt() << " " << track->Eta() << " " << track->Phi() << endl;

        //try to match it to a genjet
        Double_t minDR = 5000.0;
        const GenJet *matchedGenJet = NULL;
        for(UInt_t j=0;j<fGenJets->GetEntries();j++) {
          Double_t DR = MathUtils::DeltaR(track->Phi(), track->Eta(), fGenJets->At(j)->Phi(),
                                          fGenJets->At(j)->Eta());
          if (DR < minDR && DR < 0.5) {
            minDR = DR;
            matchedGenJet = fGenJets->At(j);
          }
        }
              
        //if it matched
        if (matchedGenJet) {
          
          MDB(kAnalysis, 8) {
            //cout << "track matched jet flavor: " << matchedGenJet->MatchedMCFlavor()  << endl;
          }
          
          if (GenLeptons->At(0)->Charge() > 0)
            fPlusWTrackMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
          else 
            fMinusWTrackMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
          
          if (trackIsolation < 10.0) {
            if (GenLeptons->At(0)->Charge() > 0)
              fPlusWLooseIsolatedTrackMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
            else 
              fMinusWLooseIsolatedTrackMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());          
          }
          if (trackIsolation < 5.0) {
            if (GenLeptons->At(0)->Charge() > 0)
              fPlusWTightIsolatedTrackMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
            else 
              fMinusWTightIsolatedTrackMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());  
          }
        }
      
        //***********************************************************************************************
        //match isolated Track to Simulation particle
        //***********************************************************************************************         
        if (trackIsolation < 10.0) {
          Int_t printDebugLevel = 0; MDB(kAnalysis, 8) printDebugLevel = 8;
          const MCParticle *matchedSimParticle = GeneratorTools::MatchElectronToSimParticle(fParticles, track, false, printDebugLevel, false);

          MDB(kAnalysis, 8) {
            cout << "Track sim match : ";
            if (matchedSimParticle) cout << matchedSimParticle->Pt() << " " << matchedSimParticle->Eta() << " " << matchedSimParticle->Phi();
            cout << endl;
          }
          Int_t FakeCategory = GeneratorTools::CategorizeFakeElectron(matchedSimParticle);
          MDB(kAnalysis, 8) cout << "Track Fake Category: " << FakeCategory << endl;

          MDB(kAnalysis, 8) cout << "primary Vertex : " << primaryVertex << endl;
          MDB(kAnalysis, 8) cout << "Track D0 : " << track->D0Corrected(primaryVertex) << endl;

          Int_t SimParticleChargeMatchCode = 0;
          if (matchedSimParticle) {
            if (matchedSimParticle->Charge() + track->Charge() == 0) {
              SimParticleChargeMatchCode = 2*matchedSimParticle->Charge() + track->Charge();
            } else {
              SimParticleChargeMatchCode = matchedSimParticle->Charge() + track->Charge();
            }
          }
          MDB(kAnalysis, 8) cout << "ChargeMatchCode: " << SimParticleChargeMatchCode << endl;          
          
          if (RealWLepton->Charge() == track->Charge()) {
            fSameSignIsolatedTrackTrkIso->Fill(trackIsolation);
            fSameSignIsolatedTrackD0->Fill(fabs(track->D0Corrected(primaryVertex)));
            fSameSignIsolatedTrackEta->Fill(track->Eta());
            fSameSignIsolatedTrackPhi->Fill(track->Phi());
            fSameSignIsolatedTrackPt->Fill(track->Pt());
            fSameSignRealToIsolatedTrackDeltaEta->Fill(fabs(track->Eta() - RealWLepton->Eta()));
            fSameSignRealToIsolatedTrackDeltaPhi->Fill(MathUtils::DeltaPhi(RealWLepton->Phi(),track->Phi()));
            fSameSignRealToIsolatedTrackDeltaR->Fill(MathUtils::DeltaR(RealWLepton->Phi(),RealWLepton->Eta(),
                                                                       track->Phi(),track->Eta()));
            fSameSignIsolatedTrackCategory->Fill(FakeCategory);
            if (FakeCategory == 1) fSameSignConversionCategorySimParticleToIsolatedTrackChargeMatch->Fill(SimParticleChargeMatchCode);
          } else {
            fOppositeSignIsolatedTrackTrkIso->Fill(trackIsolation);
            fOppositeSignIsolatedTrackD0->Fill(fabs(track->D0Corrected(primaryVertex)));
            fOppositeSignIsolatedTrackEta->Fill(track->Eta());
            fOppositeSignIsolatedTrackPhi->Fill(track->Phi());
            fOppositeSignIsolatedTrackPt->Fill(track->Pt());
            fOppositeSignRealToIsolatedTrackDeltaEta->Fill(fabs(track->Eta() - RealWLepton->Eta()));
            fOppositeSignRealToIsolatedTrackDeltaPhi->Fill(MathUtils::DeltaPhi(RealWLepton->Phi(),track->Phi()));
            fOppositeSignRealToIsolatedTrackDeltaR->Fill(MathUtils::DeltaR(RealWLepton->Phi(),RealWLepton->Eta(),
                                                                           track->Phi(),track->Eta()));
            fOppositeSignIsolatedTrackCategory->Fill(FakeCategory);
            if (FakeCategory == 1) fOppositeSignConversionCategorySimParticleToIsolatedTrackChargeMatch->Fill(SimParticleChargeMatchCode);
          }
        } //passes loose isolation
      } //passes track cuts and RealWLepton 
    } //loop over all tracks      
  }//if NCleanLeptons > 0, NGenLeptons == 1, NGenJets == 1
}
  
//--------------------------------------------------------------------------------------------------
void FakeElectronAnalysisMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.

}

//--------------------------------------------------------------------------------------------------
void FakeElectronAnalysisMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
