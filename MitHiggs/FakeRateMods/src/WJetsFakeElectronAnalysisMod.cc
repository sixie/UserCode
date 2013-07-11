// $Id: WJetsFakeElectronAnalysisMod.cc,v 1.3 2009/03/17 08:33:15 sixie Exp $

#include "MitHiggs/FakeRateMods/interface/WJetsFakeElectronAnalysisMod.h"
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

ClassImp(mithep::WJetsFakeElectronAnalysisMod)

//--------------------------------------------------------------------------------------------------
  WJetsFakeElectronAnalysisMod::WJetsFakeElectronAnalysisMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fVetoGeneratorLeptons(true),
  fRequireFirstLepton(false),
  fPrintLogs(true),
  fMCPartBranchName(Names::gkMCPartBrn),
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
void WJetsFakeElectronAnalysisMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void WJetsFakeElectronAnalysisMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fMCPartBranchName,                fParticles);
  ReqBranch(fGenJetBranchName,                fGenJets);
  ReqBranch(fTrackBranchName,                 fTracks);
  ReqBranch(fGsfTrackBranchName,              fGsfTracks);
  ReqBranch(fElectronBranchName,              fElectrons);
  ReqBranch(fJetBranchName,                   fJets);
  ReqBranch(fBarrelSuperClusterBranchName,    fBarrelSuperClusters);
  ReqBranch(fEndcapSuperClusterBranchName,    fEndcapSuperClusters);

  //***********************************************************************************************
  //Histograms for Fake Electron Charge
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
         ";CleanPlusBarrelElectronFakeCategory;Number of Events", 5,-0.5,4.0);
  TAxis *xa = fCleanPlusBarrelElectronFakeCategory->GetXaxis();
  xa->SetBinLabel(1,"No SimMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi/K");
  xa->SetBinLabel(4,"HeavyFlavor");
  xa->SetBinLabel(5,"Other");

  AddTH1(fCleanMinusBarrelElectronFakeCategory, 
         "hCleanMinusBarrelElectronFakeCategory", 
         ";CleanMinusBarrelElectronFakeCategory;Number of Events", 5,-0.5,4.0);
  xa = fCleanMinusBarrelElectronFakeCategory->GetXaxis();
  xa->SetBinLabel(1,"No SimMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi/K");
  xa->SetBinLabel(4,"HeavyFlavor");
  xa->SetBinLabel(5,"Other");
    
  AddTH1(fCleanPlusEndcapElectronFakeCategory, 
         "hCleanPlusEndcapElectronFakeCategory", 
         ";CleanPlusEndcapElectronFakeCategory;Number of Events", 5,-0.5,4.0);
  xa = fCleanPlusEndcapElectronFakeCategory->GetXaxis();
  xa->SetBinLabel(1,"No SimMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi/K");
  xa->SetBinLabel(4,"HeavyFlavor");
  xa->SetBinLabel(5,"Other");

  AddTH1(fCleanMinusEndcapElectronFakeCategory, 
         "hCleanMinusEndcapElectronFakeCategory", 
         ";CleanMinusEndcapElectronFakeCategory;Number of Events", 5,-0.5,4.0);
  xa = fCleanMinusEndcapElectronFakeCategory->GetXaxis();
  xa->SetBinLabel(1,"No SimMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi/K");
  xa->SetBinLabel(4,"HeavyFlavor");
  xa->SetBinLabel(5,"Other");

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

}

//--------------------------------------------------------------------------------------------------
void WJetsFakeElectronAnalysisMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and
  //output debug info or not   

  //cout << "Event : " << count << endl;
  count++;

  LoadBranch(fMCPartBranchName);
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
  
 
  if ( (!fRequireFirstLepton && CleanLeptons->GetEntries() > 0) || 
       (fRequireFirstLepton && CleanLeptons->GetEntries() == 2 && GenLeptons->GetEntries() == 1 && NGenJets == 1)               
    ) {
              

    bool printHepMCTable = false;
  
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
      
      if (ele->Pt() > 10.0 && !(fVetoGeneratorLeptons && IsMCLepton)) {
        //Found a fake electron.
              
        //Print out Electron        
        if(fPrintLogs) {
          
          cout << "********************************************************************************\n";
          cout << "Fake Electron : " << ele->Pt() << " " << ele->Eta() << " " << ele->Phi();
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
        if (ele->Eta() < 1.479) {
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
        if (ele->Eta() < 1.479) {
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
                
        if (ele->Eta() < 1.479) {
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
                          
                cout << "ChargedSimulatedParticle: " << j << " : " << fabs(fabs(dPtOverPt)) << " : " << DR 
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
              if (DR < minDR && fabs(fabs(dPtOverPt)) < 0.5 && DR < 0.3 && isStable && isTrackable) {
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
              
              if (fabs(fabs(dPtOverPt)-1) < fabs(fabs(bestDeltaPtOverPt)) && DR < 0.3)
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
        
        if (matchedSimChargedParticle && minDR < 0.3) {
          if (fPrintLogs) {
            cout << "Matched SimParticle: " << matchedSimChargedParticle->PdgId() << " " 
                 << matchedSimChargedParticle->Status() << " " << matchedSimChargedParticle->Pt() 
                 << " " << matchedSimChargedParticle->Eta() << " " << matchedSimChargedParticle->Phi()
                 << endl;
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
            if (ele->Eta() < 1.479) {
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
          Int_t FakeCategory = -1;

          //Try to match to conversion
          if (matchedSimChargedParticle->AbsPdgId() == 11) {
            const MCParticle *mother = matchedSimChargedParticle->Mother();

            if (!mother) {
              if (fPrintLogs) cout << "no mother\n";
              FakeCategory = 4;
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
                            
              if (Ancestor->PdgId() == 111 || Ancestor->PdgId() == 221 || Ancestor->PdgId() == 223
                  || Ancestor->PdgId() == 22
                ) {
                FakeCategory = 1; //pi0->photon->conversion
              } else if ((Ancestor->AbsPdgId() >= 400 && 
                          Ancestor->AbsPdgId() < 600) ||
                         (Ancestor->AbsPdgId() >= 4000 && 
                          Ancestor->AbsPdgId() < 6000) &&
                         Ancestor->HasDaughter(11)) {
                FakeCategory = 3; //heavy flavor 
              } else {
                FakeCategory = 4;
              }
            }
          } else if (matchedSimChargedParticle->AbsPdgId() == 211 || 321) {
            FakeCategory = 2;
          } else {            
            FakeCategory = 4;
          }          
          
          if (FakeCategory == 4) if (fPrintLogs) cout << "Uncategorized\n";

          if (ele->Eta() < 1.479) {
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
          if (ele->Eta() < 1.479) {
            if (ele->Charge() > 0) {
              if (matchedSimChargedParticle->PdgId() == 11) 
                if (fPrintLogs) cout << "Wrong charge\n";
              fCleanPlusBarrelElectronMatchedSimChargedParticleID->Fill(matchedSimChargedParticle->PdgId());
              fCleanPlusBarrelElectronMatchedSimChargedParticlePt->Fill(matchedSimChargedParticle->Pt());
              fCleanPlusBarrelElectronMatchedSimChargedParticleDeltaR->Fill(minDR);
              fCleanPlusBarrelElectronMatchedSimChargedParticleDeltaPtOverPt->Fill(matchDeltaPtOverPt);
            } else {
              if (matchedSimChargedParticle->PdgId() == -11) 
                if (fPrintLogs) cout << "Wrong charge\n";
              fCleanMinusBarrelElectronMatchedSimChargedParticleID->Fill(matchedSimChargedParticle->PdgId());
              fCleanMinusBarrelElectronMatchedSimChargedParticlePt->Fill(matchedSimChargedParticle->Pt());
              fCleanMinusBarrelElectronMatchedSimChargedParticleDeltaR->Fill(minDR);
              fCleanMinusBarrelElectronMatchedSimChargedParticleDeltaPtOverPt->Fill(matchDeltaPtOverPt);
            }
          } else {
            if (ele->Charge() > 0) {
              if (matchedSimChargedParticle->PdgId() == 11) 
                if (fPrintLogs) cout << "Wrong charge\n";
              fCleanPlusEndcapElectronMatchedSimChargedParticleID->Fill(matchedSimChargedParticle->PdgId());
              fCleanPlusEndcapElectronMatchedSimChargedParticlePt->Fill(matchedSimChargedParticle->Pt());
              fCleanPlusEndcapElectronMatchedSimChargedParticleDeltaR->Fill(minDR);
              fCleanPlusEndcapElectronMatchedSimChargedParticleDeltaPtOverPt->Fill(matchDeltaPtOverPt);
            } else {
              if (matchedSimChargedParticle->PdgId() == -11) 
                if (fPrintLogs) cout << "Wrong charge\n";
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
          if (ele->Eta() < 1.479) {
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

        //Print out HepMC Table
        if(fPrintLogs) {                            
          //print out HepMC table
          if (printHepMCTable)
            GeneratorTools::PrintHepMCTable(fParticles, true, -1);
          
          //print out all particles and all descendants that are within dR of 0.3 of the electron
          //GeneratorTools::PrintNearbyParticles(fParticles, ele->Eta(), ele->Phi(), 0.3);
        }

      }
    } //loop over RECO electrons.   
  } //if njet == 1, nlepton == 1
}

//--------------------------------------------------------------------------------------------------
void WJetsFakeElectronAnalysisMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.

}

//--------------------------------------------------------------------------------------------------
void WJetsFakeElectronAnalysisMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
