//root -l -q -b $CMSSW_BASE/src/MitPhysics/macros/runWJetsFakeValidationStudy.C+\(\"0000\",\"noskim\",\"s8-we-id11\",\"mit/filler/009\",\"/home/mitprod/catalog\",\"WJetsValidation\",10000,-10\)
//root -l -q -b $CMSSW_BASE/src/MitPhysics/macros/runWJetsFakeValidationStudy.C+\(\"0000\",\"noskim\",\"s8-wjets-mg-id11\",\"mit/filler/009\",\"/home/mitprod/catalog\",\"WJetsValidation\",10000,-10\)
//root -l -q -b $CMSSW_BASE/src/MitPhysics/macros/runWJetsFakeValidationStudy.C+\(\"0000\",\"noskim\",\"s8-vqq-mg-id11\",\"mit/filler/009\",\"/home/mitprod/catalog\",\"WJetsValidation\",10000,-10\)
//root -l -q -b $CMSSW_BASE/src/MitPhysics/macros/runWJetsFakeValidationStudy.C+\(\"0000\",\"noskim\",\"s8-zjets-mg-id11\",\"mit/filler/009\",\"/home/mitprod/catalog\",\"WJetsValidation\",10000,-10\)

// $Id: runWJetsFakeValidationStudy.C,v 1.4 2009/06/30 08:13:52 phedex Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <exception>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/Catalog/interface/Catalog.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitAna/TreeMod/interface/HLTMod.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitAna/PhysicsMod/interface/PublisherMod.h"
#include "MitPhysics/Mods/interface/GeneratorMod.h"
#include "MitPhysics/Mods/interface/PartonFlavorHistoryMod.h"
#include "MitPhysics/Mods/interface/MuonIDMod.h"
#include "MitPhysics/Mods/interface/ElectronIDMod.h"
#include "MitPhysics/Mods/interface/PhotonIDMod.h"
#include "MitPhysics/Mods/interface/JetIDMod.h"
#include "MitPhysics/Mods/interface/ElectronCleaningMod.h"
#include "MitPhysics/Mods/interface/PhotonCleaningMod.h"
#include "MitPhysics/Mods/interface/JetCleaningMod.h"
#include "MitPhysics/Mods/interface/MergeLeptonsMod.h"
#include "MitPhysics/Mods/interface/GenerateFakeableObjectsMod.h"
#include "MitPhysics/Mods/interface/GenerateFakesMod.h"
#include "MitPhysics/Validation/interface/WJetsFakeValidationMod.h"
#endif

//--------------------------------------------------------------------------------------------------
void executeWJetsFakeValidationStudy(const char *fileset  = "",
                                     const char *skim         = "noskim",
                                     const char *dataset    = "s8-ttbar-id9",
                                     const char *book       = "mit/filler/006",
                                     const char *catalogDir = "/home/mitprod/catalog",
                                     const char *outputName = "WJetsFakeValidationMod",
                                     Bool_t UseMCFakes = false,
                                     Bool_t PerformFakeMuonMetCorrection = true,                                     
                                     Int_t FakeableObjectType = 1,
                                     int         nEvents    = -1)
{
  //------------------------------------------------------------------------------------------------
  // some global setups
  //------------------------------------------------------------------------------------------------
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 3;

  if (string(dataset) == "s8-we-id11" 
      || string(dataset) == "s8-wm-id11"
      || string(dataset) == "s8-wt-id11"
      || string(dataset) == "s8-wjets-mg-id11"
      || string(dataset) == "s8-vqq-mg-id11"
      || string(dataset) == "f8-wc-mg-id11"

    ) {
    gDebugLevel = 3;
  }

  string mctype = "";
  if (string(dataset) == "s8-wjets-mg-id11" || string(dataset) == "f8-wjets-mg-id11" 
      || string(dataset) == "s8-zjets-mg-id11")
    mctype = "kMCTypeVLightJets";
  if (string(dataset) == "s8-vqq-mg-id11" || string(dataset) == "f8-vqq-mg-id11")
    mctype = "kMCTypeVQQ";
  if (string(dataset) == "f8-wc-mg-id11" )
    mctype = "kMCTypeWc";

  //------------------------------------------------------------------------------------------------
  // generator information
  //------------------------------------------------------------------------------------------------
  GeneratorMod *generatorMod = new GeneratorMod;

  PartonFlavorHistoryMod *partonFlavorHistoryMod = new PartonFlavorHistoryMod;
  partonFlavorHistoryMod->SetMCSampleType(mctype.c_str());
  partonFlavorHistoryMod->SetApplyPartonFlavorFilter(kTRUE);

  //------------------------------------------------------------------------------------------------
  // HLT information
  //------------------------------------------------------------------------------------------------
  HLTMod *hltmod = new HLTMod;
  hltmod->SetPrintTable(kTRUE);
//  hltmod->AddTrigger("HLT_IsoEle15_L1I");
//   hltmod->AddTrigger("HLT_Ele15_SW_L1R");
//   hltmod->AddTrigger("HLT_IsoEle15_LW_L1I");
  hltmod->AddTrigger("HLT_IsoEle18_L1R");
//   hltmod->AddTrigger("HLT_Ele15_LW_L1R");
//   hltmod->AddTrigger("HLT_LooseIsoEle15_LW_L1R");
//  hltmod->AddTrigger("HLT_IsoMu15");
  hltmod->AddTrigger("HLT_Mu15");
  hltmod->SetTrigObjsName("myhltobjs");

  //------------------------------------------------------------------------------------------------
  // publisher Mod
  //------------------------------------------------------------------------------------------------
  PublisherMod<CaloJet,Jet> *pubJet = new PublisherMod<CaloJet,Jet>("JetPub");
  pubJet->SetInputName("SisCone5Jets");
  pubJet->SetOutputName("PubSisCone5Jets");

  PublisherMod<CaloMet,Met> *pubMet = new PublisherMod<CaloMet,Met>("MetPub");
  pubMet->SetInputName("SisCone5Met");
  pubMet->SetOutputName(ModNames::gkCleanCaloMetName);

  //------------------------------------------------------------------------------------------------
  // object id and cleaning sequence
  //------------------------------------------------------------------------------------------------
  MuonIDMod           *muonID           = new MuonIDMod;  
  muonID->SetIDType("Loose");
  muonID->SetIsoType("TrackCaloSliding"); 
  ElectronIDMod       *electronID       = new ElectronIDMod;
  electronID->SetIDType(TString("Tight"));
  electronID->SetIsoType(TString("TrackJuraSliding"));
  electronID->SetApplyConversionFilter(kTRUE);
  PhotonIDMod         *photonID       = new PhotonIDMod;
  photonID->SetIDType(TString("Tight"));
  photonID->SetPtMin(20.0);
  JetIDMod            *jetID            = new JetIDMod;
  jetID->SetInputName(pubJet->GetOutputName());
  jetID->SetUseCorrection(kTRUE);
  jetID->SetPtCut(35.0);
  ElectronCleaningMod *electronCleaning = new ElectronCleaningMod;
  PhotonCleaningMod   *photonCleaning   = new PhotonCleaningMod;
  JetCleaningMod      *jetCleaning      = new JetCleaningMod;

  //------------------------------------------------------------------------------------------------
  // merge modules
  //------------------------------------------------------------------------------------------------
  MergeLeptonsMod *mergeLeptonsMod = new MergeLeptonsMod;

  //------------------------------------------------------------------------------------------------
  // generate fakes module
  //------------------------------------------------------------------------------------------------
  GenerateFakeableObjectsMod *generateFakeableObjectsMod_GsfTrack_IsoTrack = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_GsfTrack_IsoTrack->SetName("GenerateFakeableObjectsMod_GsfTrack_IsoTrack");
  generateFakeableObjectsMod_GsfTrack_IsoTrack->SetVetoTriggerJet(kFALSE);
  generateFakeableObjectsMod_GsfTrack_IsoTrack->SetVetoGenLeptons(kFALSE);
  generateFakeableObjectsMod_GsfTrack_IsoTrack->SetVetoCleanLeptons(kTRUE);
  generateFakeableObjectsMod_GsfTrack_IsoTrack->SetElectronFOType("GsfPlusSC");
  generateFakeableObjectsMod_GsfTrack_IsoTrack->SetMuonFOType("IsoTrack");
  generateFakeableObjectsMod_GsfTrack_IsoTrack->SetElectronFakeableObjectsName("ElectronFakeableObjects_GsfTrack");
  generateFakeableObjectsMod_GsfTrack_IsoTrack->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");

  GenerateFakesMod *generateFakesMod_GsfTrack_IsoTrack_PythiaWJets = new GenerateFakesMod;
  generateFakesMod_GsfTrack_IsoTrack_PythiaWJets->SetName("GenerateFakesMod_GsfTrack_IsoTrack_PythiaWJets");
  generateFakesMod_GsfTrack_IsoTrack_PythiaWJets->SetElectronFakeableObjectsName("ElectronFakeableObjects_GsfTrack");
  generateFakesMod_GsfTrack_IsoTrack_PythiaWJets->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  generateFakesMod_GsfTrack_IsoTrack_PythiaWJets->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_GsfTrack_IsoTrack_PythiaWJets->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_GsfTrack_IsoTrack_PythiaWJets->SetUse2DFakeRate(kTRUE);
  generateFakesMod_GsfTrack_IsoTrack_PythiaWJets->SetUseFitFunction(kFALSE);
  generateFakesMod_GsfTrack_IsoTrack_PythiaWJets->SetElectronFRHistName("GsfTrackSCElectronFakeRate_PtEta_PythiaWJets");
  generateFakesMod_GsfTrack_IsoTrack_PythiaWJets->SetMuonFRHistName("IsoTrackMuonFakeRate_PtEta_PythiaWJets");
  generateFakesMod_GsfTrack_IsoTrack_PythiaWJets->LoadFakeRate();
  generateFakesMod_GsfTrack_IsoTrack_PythiaWJets->SetOutputName("FakeEventHeaders_GsfTrack_IsoTrack_PythiaWJets");

  GenerateFakesMod *generateFakesMod_GsfTrack_IsoTrack_MadgraphWJets = new GenerateFakesMod;
  generateFakesMod_GsfTrack_IsoTrack_MadgraphWJets->SetName("GenerateFakesMod_GsfTrack_IsoTrack_MadgraphWJets");  
  generateFakesMod_GsfTrack_IsoTrack_MadgraphWJets->SetElectronFakeableObjectsName("ElectronFakeableObjects_GsfTrack");
  generateFakesMod_GsfTrack_IsoTrack_MadgraphWJets->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  generateFakesMod_GsfTrack_IsoTrack_MadgraphWJets->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_GsfTrack_IsoTrack_MadgraphWJets->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_GsfTrack_IsoTrack_MadgraphWJets->SetUse2DFakeRate(kTRUE);
  generateFakesMod_GsfTrack_IsoTrack_MadgraphWJets->SetUseFitFunction(kFALSE);
  generateFakesMod_GsfTrack_IsoTrack_MadgraphWJets->SetElectronFRHistName("GsfTrackSCElectronFakeRate_PtEta_MadgraphWJets");
  generateFakesMod_GsfTrack_IsoTrack_MadgraphWJets->SetMuonFRHistName("IsoTrackMuonFakeRate_PtEta_MadgraphWJets");
  generateFakesMod_GsfTrack_IsoTrack_MadgraphWJets->LoadFakeRate();
  generateFakesMod_GsfTrack_IsoTrack_MadgraphWJets->SetOutputName("FakeEventHeaders_GsfTrack_IsoTrack_MadgraphWJets");

  GenerateFakesMod *generateFakesMod_GsfTrack_IsoTrack_Pythia_Jet50 = new GenerateFakesMod;
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Jet50->SetName("GenerateFakesMod_GsfTrack_IsoTrack_Pythia_Jet50");
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Jet50->SetElectronFakeableObjectsName("ElectronFakeableObjects_GsfTrack");
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Jet50->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Jet50->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Jet50->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Jet50->SetUse2DFakeRate(kTRUE);
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Jet50->SetUseFitFunction(kFALSE);
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Jet50->SetElectronFRHistName("GsfTrackSCElectronFakeRate_PtEta_Pythia_Jet50");
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Jet50->SetMuonFRHistName("IsoTrackMuonFakeRate_PtEta_Pythia_Jet50");
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Jet50->LoadFakeRate();
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Jet50->SetOutputName("FakeEventHeaders_GsfTrack_IsoTrack_Pythia_Jet50");

  GenerateFakesMod *generateFakesMod_GsfTrack_IsoTrack_Pythia_Photon15 = new GenerateFakesMod;
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Photon15->SetName("GenerateFakesMod_GsfTrack_IsoTrack_Pythia_Photon15");
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Photon15->SetElectronFakeableObjectsName("ElectronFakeableObjects_GsfTrack");
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Photon15->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Photon15->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Photon15->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Photon15->SetUse2DFakeRate(kTRUE);
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Photon15->SetUseFitFunction(kFALSE);
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Photon15->SetElectronFRHistName("GsfTrackSCElectronFakeRate_PtEta_Pythia_Photon15");
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Photon15->SetMuonFRHistName("IsoTrackMuonFakeRate_PtEta_Pythia_Photon15");
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Photon15->LoadFakeRate();
  generateFakesMod_GsfTrack_IsoTrack_Pythia_Photon15->SetOutputName("FakeEventHeaders_GsfTrack_IsoTrack_Pythia_Photon15");

  GenerateFakesMod *generateFakesMod_GsfTrack_IsoTrack_Madgraph_Jet50 = new GenerateFakesMod;
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetName("GenerateFakesMod_GsfTrack_IsoTrack_Madgraph_Jet50");
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetElectronFakeableObjectsName("ElectronFakeableObjects_GsfTrack");
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetUse2DFakeRate(kTRUE);
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetUseFitFunction(kFALSE);
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetElectronFRHistName("GsfTrackSCElectronFakeRate_PtEta_Madgraph_Jet50");
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetMuonFRHistName("IsoTrackMuonFakeRate_PtEta_Madgraph_Jet50");
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Jet50->LoadFakeRate();
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetOutputName("FakeEventHeaders_GsfTrack_IsoTrack_Madgraph_Jet50");

  GenerateFakesMod *generateFakesMod_GsfTrack_IsoTrack_Madgraph_Photon15 = new GenerateFakesMod;
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetName("GenerateFakesMod_GsfTrack_IsoTrack_Madgraph_Photon15");
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetElectronFakeableObjectsName("ElectronFakeableObjects_GsfTrack");
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetUse2DFakeRate(kTRUE);
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetUseFitFunction(kFALSE);
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetElectronFRHistName("GsfTrackSCElectronFakeRate_PtEta_Madgraph_Photon15");
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetMuonFRHistName("IsoTrackMuonFakeRate_PtEta_Madgraph_Photon15");
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Photon15->LoadFakeRate();
  generateFakesMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetOutputName("FakeEventHeaders_GsfTrack_IsoTrack_Madgraph_Photon15");


//****************************************************************************************************************
//RECO Denominator objects

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Reco_IsoTrack = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Reco_IsoTrack->SetName("GenerateFakeableObjectsMod_Reco_IsoTrack");
  generateFakeableObjectsMod_Reco_IsoTrack->SetVetoTriggerJet(kFALSE);
  generateFakeableObjectsMod_Reco_IsoTrack->SetVetoGenLeptons(kFALSE);
  generateFakeableObjectsMod_Reco_IsoTrack->SetVetoCleanLeptons(kTRUE);
  generateFakeableObjectsMod_Reco_IsoTrack->SetElectronFOType("Reco");
  generateFakeableObjectsMod_Reco_IsoTrack->SetMuonFOType("IsoTrack");
  generateFakeableObjectsMod_Reco_IsoTrack->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakeableObjectsMod_Reco_IsoTrack->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Reco_TrackerMuon = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Reco_TrackerMuon->SetName("GenerateFakeableObjectsMod_Reco_TrackerMuon");
  generateFakeableObjectsMod_Reco_TrackerMuon->SetVetoTriggerJet(kFALSE);
  generateFakeableObjectsMod_Reco_TrackerMuon->SetVetoGenLeptons(kFALSE);
  generateFakeableObjectsMod_Reco_TrackerMuon->SetVetoCleanLeptons(kTRUE);
  generateFakeableObjectsMod_Reco_TrackerMuon->SetElectronFOType("Reco");
  generateFakeableObjectsMod_Reco_TrackerMuon->SetMuonFOType("TrackerMuon");
  generateFakeableObjectsMod_Reco_TrackerMuon->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco2");
  generateFakeableObjectsMod_Reco_TrackerMuon->SetMuonFakeableObjectsName("MuonFakeableObjects_TrackerMuon");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Reco_Global = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Reco_Global->SetName("GenerateFakeableObjectsMod_Reco_Global");
  generateFakeableObjectsMod_Reco_Global->SetVetoTriggerJet(kFALSE);
  generateFakeableObjectsMod_Reco_Global->SetVetoGenLeptons(kFALSE);
  generateFakeableObjectsMod_Reco_Global->SetVetoCleanLeptons(kTRUE);
  generateFakeableObjectsMod_Reco_Global->SetElectronFOType("Reco");
  generateFakeableObjectsMod_Reco_Global->SetMuonFOType("Global");
  generateFakeableObjectsMod_Reco_Global->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakeableObjectsMod_Reco_Global->SetMuonFakeableObjectsName("MuonFakeableObjects_Global");

  GenerateFakesMod *generateFakesMod_Reco_IsoTrack_PythiaWJets = new GenerateFakesMod;
  generateFakesMod_Reco_IsoTrack_PythiaWJets->SetName("GenerateFakesMod_Reco_IsoTrack_PythiaWJets");
  generateFakesMod_Reco_IsoTrack_PythiaWJets->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_IsoTrack_PythiaWJets->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  generateFakesMod_Reco_IsoTrack_PythiaWJets->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_IsoTrack_PythiaWJets->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_IsoTrack_PythiaWJets->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_IsoTrack_PythiaWJets->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_IsoTrack_PythiaWJets->SetElectronFRHistName("RecoElectronFakeRate_PtEta_PythiaWJets");
  generateFakesMod_Reco_IsoTrack_PythiaWJets->SetMuonFRHistName("IsoTrackMuonFakeRate_PtEta_PythiaWJets");
  generateFakesMod_Reco_IsoTrack_PythiaWJets->LoadFakeRate();
  generateFakesMod_Reco_IsoTrack_PythiaWJets->SetOutputName("FakeEventHeaders_Reco_IsoTrack_PythiaWJets");

  GenerateFakesMod *generateFakesMod_Reco_IsoTrack_MadgraphWJets = new GenerateFakesMod;
  generateFakesMod_Reco_IsoTrack_MadgraphWJets->SetName("GenerateFakesMod_Reco_IsoTrack_MadgraphWJets");
  generateFakesMod_Reco_IsoTrack_MadgraphWJets->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_IsoTrack_MadgraphWJets->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  generateFakesMod_Reco_IsoTrack_MadgraphWJets->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_IsoTrack_MadgraphWJets->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_IsoTrack_MadgraphWJets->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_IsoTrack_MadgraphWJets->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_IsoTrack_MadgraphWJets->SetElectronFRHistName("RecoElectronFakeRate_PtEta_MadgraphWJets");
  generateFakesMod_Reco_IsoTrack_MadgraphWJets->SetMuonFRHistName("IsoTrackMuonFakeRate_PtEta_MadgraphWJets");
  generateFakesMod_Reco_IsoTrack_MadgraphWJets->LoadFakeRate();
  generateFakesMod_Reco_IsoTrack_MadgraphWJets->SetOutputName("FakeEventHeaders_Reco_IsoTrack_MadgraphWJets");

  GenerateFakesMod *generateFakesMod_Reco_IsoTrack_Pythia_Jet50 = new GenerateFakesMod;
  generateFakesMod_Reco_IsoTrack_Pythia_Jet50->SetName("GenerateFakesMod_Reco_IsoTrack_Pythia_Jet50");
  generateFakesMod_Reco_IsoTrack_Pythia_Jet50->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_IsoTrack_Pythia_Jet50->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  generateFakesMod_Reco_IsoTrack_Pythia_Jet50->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_IsoTrack_Pythia_Jet50->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_IsoTrack_Pythia_Jet50->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_IsoTrack_Pythia_Jet50->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_IsoTrack_Pythia_Jet50->SetElectronFRHistName("RecoElectronFakeRate_PtEta_Pythia_Jet50");
  generateFakesMod_Reco_IsoTrack_Pythia_Jet50->SetMuonFRHistName("IsoTrackMuonFakeRate_PtEta_Pythia_Jet50");
  generateFakesMod_Reco_IsoTrack_Pythia_Jet50->LoadFakeRate();
  generateFakesMod_Reco_IsoTrack_Pythia_Jet50->SetOutputName("FakeEventHeaders_Reco_IsoTrack_Pythia_Jet50");

  GenerateFakesMod *generateFakesMod_Reco_IsoTrack_Pythia_Photon15 = new GenerateFakesMod;
  generateFakesMod_Reco_IsoTrack_Pythia_Photon15->SetName("GenerateFakesMod_Reco_IsoTrack_Pythia_Photon15");
  generateFakesMod_Reco_IsoTrack_Pythia_Photon15->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_IsoTrack_Pythia_Photon15->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  generateFakesMod_Reco_IsoTrack_Pythia_Photon15->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_IsoTrack_Pythia_Photon15->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_IsoTrack_Pythia_Photon15->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_IsoTrack_Pythia_Photon15->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_IsoTrack_Pythia_Photon15->SetElectronFRHistName("RecoElectronFakeRate_PtEta_Pythia_Photon15");
  generateFakesMod_Reco_IsoTrack_Pythia_Photon15->SetMuonFRHistName("IsoTrackMuonFakeRate_PtEta_Pythia_Photon15");
  generateFakesMod_Reco_IsoTrack_Pythia_Photon15->LoadFakeRate();
  generateFakesMod_Reco_IsoTrack_Pythia_Photon15->SetOutputName("FakeEventHeaders_Reco_IsoTrack_Pythia_Photon15");

  GenerateFakesMod *generateFakesMod_Reco_IsoTrack_Madgraph_Jet50 = new GenerateFakesMod;
  generateFakesMod_Reco_IsoTrack_Madgraph_Jet50->SetName("GenerateFakesMod_Reco_IsoTrack_Madgraph_Jet50");
  generateFakesMod_Reco_IsoTrack_Madgraph_Jet50->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_IsoTrack_Madgraph_Jet50->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  generateFakesMod_Reco_IsoTrack_Madgraph_Jet50->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_IsoTrack_Madgraph_Jet50->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_IsoTrack_Madgraph_Jet50->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_IsoTrack_Madgraph_Jet50->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_IsoTrack_Madgraph_Jet50->SetElectronFRHistName("RecoElectronFakeRate_PtEta_Madgraph_Jet50");
  generateFakesMod_Reco_IsoTrack_Madgraph_Jet50->SetMuonFRHistName("IsoTrackMuonFakeRate_PtEta_Madgraph_Jet50");
  generateFakesMod_Reco_IsoTrack_Madgraph_Jet50->LoadFakeRate();
  generateFakesMod_Reco_IsoTrack_Madgraph_Jet50->SetOutputName("FakeEventHeaders_Reco_IsoTrack_Madgraph_Jet50");

  GenerateFakesMod *generateFakesMod_Reco_IsoTrack_Madgraph_Photon15 = new GenerateFakesMod;
  generateFakesMod_Reco_IsoTrack_Madgraph_Photon15->SetName("GenerateFakesMod_Reco_IsoTrack_Madgraph_Photon15");
  generateFakesMod_Reco_IsoTrack_Madgraph_Photon15->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_IsoTrack_Madgraph_Photon15->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  generateFakesMod_Reco_IsoTrack_Madgraph_Photon15->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_IsoTrack_Madgraph_Photon15->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_IsoTrack_Madgraph_Photon15->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_IsoTrack_Madgraph_Photon15->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_IsoTrack_Madgraph_Photon15->SetElectronFRHistName("RecoElectronFakeRate_PtEta_Madgraph_Photon15");
  generateFakesMod_Reco_IsoTrack_Madgraph_Photon15->SetMuonFRHistName("IsoTrackMuonFakeRate_PtEta_Madgraph_Photon15");
  generateFakesMod_Reco_IsoTrack_Madgraph_Photon15->LoadFakeRate();
  generateFakesMod_Reco_IsoTrack_Madgraph_Photon15->SetOutputName("FakeEventHeaders_Reco_IsoTrack_Madgraph_Photon15");


  GenerateFakesMod *generateFakesMod_Reco_TrackerMuon_PythiaWJets = new GenerateFakesMod;
  generateFakesMod_Reco_TrackerMuon_PythiaWJets->SetName("GenerateFakesMod_Reco_TrackerMuon_PythiaWJets");
  generateFakesMod_Reco_TrackerMuon_PythiaWJets->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_TrackerMuon_PythiaWJets->SetMuonFakeableObjectsName("MuonFakeableObjects_TrackerMuon");
  generateFakesMod_Reco_TrackerMuon_PythiaWJets->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_TrackerMuon_PythiaWJets->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_TrackerMuon_PythiaWJets->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_TrackerMuon_PythiaWJets->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_TrackerMuon_PythiaWJets->SetElectronFRHistName("RecoElectronFakeRate_PtEta_PythiaWJets");
  generateFakesMod_Reco_TrackerMuon_PythiaWJets->SetMuonFRHistName("TrackerMuonFakeRate_PtEta_PythiaWJets");
  generateFakesMod_Reco_TrackerMuon_PythiaWJets->LoadFakeRate();
  generateFakesMod_Reco_TrackerMuon_PythiaWJets->SetOutputName("FakeEventHeaders_Reco_TrackerMuon_PythiaWJets");

  GenerateFakesMod *generateFakesMod_Reco_TrackerMuon_MadgraphWJets = new GenerateFakesMod;
  generateFakesMod_Reco_TrackerMuon_MadgraphWJets->SetName("GenerateFakesMod_Reco_TrackerMuon_MadgraphWJets");
  generateFakesMod_Reco_TrackerMuon_MadgraphWJets->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_TrackerMuon_MadgraphWJets->SetMuonFakeableObjectsName("MuonFakeableObjects_TrackerMuon");
  generateFakesMod_Reco_TrackerMuon_MadgraphWJets->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_TrackerMuon_MadgraphWJets->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_TrackerMuon_MadgraphWJets->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_TrackerMuon_MadgraphWJets->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_TrackerMuon_MadgraphWJets->SetElectronFRHistName("RecoElectronFakeRate_PtEta_MadgraphWJets");
  generateFakesMod_Reco_TrackerMuon_MadgraphWJets->SetMuonFRHistName("TrackerMuonFakeRate_PtEta_MadgraphWJets");
  generateFakesMod_Reco_TrackerMuon_MadgraphWJets->LoadFakeRate();
  generateFakesMod_Reco_TrackerMuon_MadgraphWJets->SetOutputName("FakeEventHeaders_Reco_TrackerMuon_MadgraphWJets");

  GenerateFakesMod *generateFakesMod_Reco_TrackerMuon_Pythia_Jet50 = new GenerateFakesMod;
  generateFakesMod_Reco_TrackerMuon_Pythia_Jet50->SetName("GenerateFakesMod_Reco_TrackerMuon_Pythia_Jet50");
  generateFakesMod_Reco_TrackerMuon_Pythia_Jet50->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_TrackerMuon_Pythia_Jet50->SetMuonFakeableObjectsName("MuonFakeableObjects_TrackerMuon");
  generateFakesMod_Reco_TrackerMuon_Pythia_Jet50->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_TrackerMuon_Pythia_Jet50->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_TrackerMuon_Pythia_Jet50->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_TrackerMuon_Pythia_Jet50->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_TrackerMuon_Pythia_Jet50->SetElectronFRHistName("RecoElectronFakeRate_PtEta_Pythia_Jet50");
  generateFakesMod_Reco_TrackerMuon_Pythia_Jet50->SetMuonFRHistName("TrackerMuonFakeRate_PtEta_Pythia_Jet50");
  generateFakesMod_Reco_TrackerMuon_Pythia_Jet50->LoadFakeRate();
  generateFakesMod_Reco_TrackerMuon_Pythia_Jet50->SetOutputName("FakeEventHeaders_Reco_TrackerMuon_Pythia_Jet50");

  GenerateFakesMod *generateFakesMod_Reco_TrackerMuon_Pythia_Photon15 = new GenerateFakesMod;
  generateFakesMod_Reco_TrackerMuon_Pythia_Photon15->SetName("GenerateFakesMod_Reco_TrackerMuon_Pythia_Photon15");
  generateFakesMod_Reco_TrackerMuon_Pythia_Photon15->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_TrackerMuon_Pythia_Photon15->SetMuonFakeableObjectsName("MuonFakeableObjects_TrackerMuon");
  generateFakesMod_Reco_TrackerMuon_Pythia_Photon15->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_TrackerMuon_Pythia_Photon15->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_TrackerMuon_Pythia_Photon15->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_TrackerMuon_Pythia_Photon15->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_TrackerMuon_Pythia_Photon15->SetElectronFRHistName("RecoElectronFakeRate_PtEta_Pythia_Photon15");
  generateFakesMod_Reco_TrackerMuon_Pythia_Photon15->SetMuonFRHistName("TrackerMuonFakeRate_PtEta_Pythia_Photon15");
  generateFakesMod_Reco_TrackerMuon_Pythia_Photon15->LoadFakeRate();
  generateFakesMod_Reco_TrackerMuon_Pythia_Photon15->SetOutputName("FakeEventHeaders_Reco_TrackerMuon_Pythia_Photon15");

  GenerateFakesMod *generateFakesMod_Reco_TrackerMuon_Madgraph_Jet50 = new GenerateFakesMod;
  generateFakesMod_Reco_TrackerMuon_Madgraph_Jet50->SetName("GenerateFakesMod_Reco_TrackerMuon_Madgraph_Jet50");
  generateFakesMod_Reco_TrackerMuon_Madgraph_Jet50->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_TrackerMuon_Madgraph_Jet50->SetMuonFakeableObjectsName("MuonFakeableObjects_TrackerMuon");
  generateFakesMod_Reco_TrackerMuon_Madgraph_Jet50->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_TrackerMuon_Madgraph_Jet50->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_TrackerMuon_Madgraph_Jet50->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_TrackerMuon_Madgraph_Jet50->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_TrackerMuon_Madgraph_Jet50->SetElectronFRHistName("RecoElectronFakeRate_PtEta_Madgraph_Jet50");
  generateFakesMod_Reco_TrackerMuon_Madgraph_Jet50->SetMuonFRHistName("TrackerMuonFakeRate_PtEta_Madgraph_Jet50");
  generateFakesMod_Reco_TrackerMuon_Madgraph_Jet50->LoadFakeRate();
  generateFakesMod_Reco_TrackerMuon_Madgraph_Jet50->SetOutputName("FakeEventHeaders_Reco_TrackerMuon_Madgraph_Jet50");

  GenerateFakesMod *generateFakesMod_Reco_TrackerMuon_Madgraph_Photon15 = new GenerateFakesMod;
  generateFakesMod_Reco_TrackerMuon_Madgraph_Photon15->SetName("GenerateFakesMod_Reco_TrackerMuon_Madgraph_Photon15");
  generateFakesMod_Reco_TrackerMuon_Madgraph_Photon15->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_TrackerMuon_Madgraph_Photon15->SetMuonFakeableObjectsName("MuonFakeableObjects_TrackerMuon");
  generateFakesMod_Reco_TrackerMuon_Madgraph_Photon15->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_TrackerMuon_Madgraph_Photon15->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_TrackerMuon_Madgraph_Photon15->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_TrackerMuon_Madgraph_Photon15->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_TrackerMuon_Madgraph_Photon15->SetElectronFRHistName("RecoElectronFakeRate_PtEta_Madgraph_Photon15");
  generateFakesMod_Reco_TrackerMuon_Madgraph_Photon15->SetMuonFRHistName("TrackerMuonFakeRate_PtEta_Madgraph_Photon15");
  generateFakesMod_Reco_TrackerMuon_Madgraph_Photon15->LoadFakeRate();
  generateFakesMod_Reco_TrackerMuon_Madgraph_Photon15->SetOutputName("FakeEventHeaders_Reco_TrackerMuon_Madgraph_Photon15");


  GenerateFakesMod *generateFakesMod_Reco_Global_PythiaWJets = new GenerateFakesMod;
  generateFakesMod_Reco_Global_PythiaWJets->SetName("GenerateFakesMod_Reco_Global_PythiaWJets");
  generateFakesMod_Reco_Global_PythiaWJets->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_Global_PythiaWJets->SetMuonFakeableObjectsName("MuonFakeableObjects_Global");
  generateFakesMod_Reco_Global_PythiaWJets->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_Global_PythiaWJets->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_Global_PythiaWJets->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_Global_PythiaWJets->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_Global_PythiaWJets->SetElectronFRHistName("RecoElectronFakeRate_PtEta_PythiaWJets");
  generateFakesMod_Reco_Global_PythiaWJets->SetMuonFRHistName("GlobalMuonFakeRate_PtEta_PythiaWJets");
  generateFakesMod_Reco_Global_PythiaWJets->LoadFakeRate();
  generateFakesMod_Reco_Global_PythiaWJets->SetOutputName("FakeEventHeaders_Reco_Global_PythiaWJets");

  GenerateFakesMod *generateFakesMod_Reco_Global_MadgraphWJets = new GenerateFakesMod;
  generateFakesMod_Reco_Global_MadgraphWJets->SetName("GenerateFakesMod_Reco_Global_MadgraphWJets");
  generateFakesMod_Reco_Global_MadgraphWJets->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_Global_MadgraphWJets->SetMuonFakeableObjectsName("MuonFakeableObjects_Global");
  generateFakesMod_Reco_Global_MadgraphWJets->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_Global_MadgraphWJets->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_Global_MadgraphWJets->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_Global_MadgraphWJets->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_Global_MadgraphWJets->SetElectronFRHistName("RecoElectronFakeRate_PtEta_MadgraphWJets");
  generateFakesMod_Reco_Global_MadgraphWJets->SetMuonFRHistName("GlobalMuonFakeRate_PtEta_MadgraphWJets");
  generateFakesMod_Reco_Global_MadgraphWJets->LoadFakeRate();
  generateFakesMod_Reco_Global_MadgraphWJets->SetOutputName("FakeEventHeaders_Reco_Global_MadgraphWJets");

  GenerateFakesMod *generateFakesMod_Reco_Global_Pythia_Jet50 = new GenerateFakesMod;
  generateFakesMod_Reco_Global_Pythia_Jet50->SetName("GenerateFakesMod_Reco_Global_Pythia_Jet50");
  generateFakesMod_Reco_Global_Pythia_Jet50->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_Global_Pythia_Jet50->SetMuonFakeableObjectsName("MuonFakeableObjects_Global");
  generateFakesMod_Reco_Global_Pythia_Jet50->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_Global_Pythia_Jet50->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_Global_Pythia_Jet50->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_Global_Pythia_Jet50->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_Global_Pythia_Jet50->SetElectronFRHistName("RecoElectronFakeRate_PtEta_Pythia_Jet50");
  generateFakesMod_Reco_Global_Pythia_Jet50->SetMuonFRHistName("GlobalMuonFakeRate_PtEta_Pythia_Jet50");
  generateFakesMod_Reco_Global_Pythia_Jet50->LoadFakeRate();
  generateFakesMod_Reco_Global_Pythia_Jet50->SetOutputName("FakeEventHeaders_Reco_Global_Pythia_Jet50");

  GenerateFakesMod *generateFakesMod_Reco_Global_Pythia_Photon15 = new GenerateFakesMod;
  generateFakesMod_Reco_Global_Pythia_Photon15->SetName("GenerateFakesMod_Reco_Global_Pythia_Photon15");
  generateFakesMod_Reco_Global_Pythia_Photon15->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_Global_Pythia_Photon15->SetMuonFakeableObjectsName("MuonFakeableObjects_Global");
  generateFakesMod_Reco_Global_Pythia_Photon15->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_Global_Pythia_Photon15->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_Global_Pythia_Photon15->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_Global_Pythia_Photon15->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_Global_Pythia_Photon15->SetElectronFRHistName("RecoElectronFakeRate_PtEta_Pythia_Photon15");
  generateFakesMod_Reco_Global_Pythia_Photon15->SetMuonFRHistName("GlobalMuonFakeRate_PtEta_Pythia_Photon15");
  generateFakesMod_Reco_Global_Pythia_Photon15->LoadFakeRate();
  generateFakesMod_Reco_Global_Pythia_Photon15->SetOutputName("FakeEventHeaders_Reco_Global_Pythia_Photon15");

  GenerateFakesMod *generateFakesMod_Reco_Global_Madgraph_Jet50 = new GenerateFakesMod;
  generateFakesMod_Reco_Global_Madgraph_Jet50->SetName("GenerateFakesMod_Reco_Global_Madgraph_Jet50");
  generateFakesMod_Reco_Global_Madgraph_Jet50->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_Global_Madgraph_Jet50->SetMuonFakeableObjectsName("MuonFakeableObjects_Global");
  generateFakesMod_Reco_Global_Madgraph_Jet50->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_Global_Madgraph_Jet50->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_Global_Madgraph_Jet50->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_Global_Madgraph_Jet50->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_Global_Madgraph_Jet50->SetElectronFRHistName("RecoElectronFakeRate_PtEta_Madgraph_Jet50");
  generateFakesMod_Reco_Global_Madgraph_Jet50->SetMuonFRHistName("GlobalMuonFakeRate_PtEta_Madgraph_Jet50");
  generateFakesMod_Reco_Global_Madgraph_Jet50->LoadFakeRate();
  generateFakesMod_Reco_Global_Madgraph_Jet50->SetOutputName("FakeEventHeaders_Reco_Global_Madgraph_Jet50");

  GenerateFakesMod *generateFakesMod_Reco_Global_Madgraph_Photon15 = new GenerateFakesMod;
  generateFakesMod_Reco_Global_Madgraph_Photon15->SetName("GenerateFakesMod_Reco_Global_Madgraph_Photon15");
  generateFakesMod_Reco_Global_Madgraph_Photon15->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  generateFakesMod_Reco_Global_Madgraph_Photon15->SetMuonFakeableObjectsName("MuonFakeableObjects_Global");
  generateFakesMod_Reco_Global_Madgraph_Photon15->SetElectronFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_Global_Madgraph_Photon15->SetMuonFRFilename("$CMSSW_BASE/src/MitPhysics/data/FakeRates.root");
  generateFakesMod_Reco_Global_Madgraph_Photon15->SetUse2DFakeRate(kTRUE);
  generateFakesMod_Reco_Global_Madgraph_Photon15->SetUseFitFunction(kFALSE);
  generateFakesMod_Reco_Global_Madgraph_Photon15->SetElectronFRHistName("RecoElectronFakeRate_PtEta_Madgraph_Photon15");
  generateFakesMod_Reco_Global_Madgraph_Photon15->SetMuonFRHistName("GlobalMuonFakeRate_PtEta_Madgraph_Photon15");
  generateFakesMod_Reco_Global_Madgraph_Photon15->LoadFakeRate();
  generateFakesMod_Reco_Global_Madgraph_Photon15->SetOutputName("FakeEventHeaders_Reco_Global_Madgraph_Photon15");



  //------------------------------------------------------------------------------------------------
  // analyses modules
  //------------------------------------------------------------------------------------------------
  WJetsFakeValidationMod *analysisMod_MC = new WJetsFakeValidationMod;
  analysisMod_MC->SetName("WJetsFakeValidationMod_MC");
  analysisMod_MC->SetUseMCFake(kTRUE);
  analysisMod_MC->SetFakeEventHeaderName("NotSet");
  analysisMod_MC->SetElectronFakeableObjectsName("ElectronFakeableObjects_GsfTrack");
  analysisMod_MC->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  analysisMod_MC->SetPerformFakeMuonMetCorrection(kFALSE);
  analysisMod_MC->SetSSFakerateCorrection(1.0);
  analysisMod_MC->SetOSFakerateCorrection(1.0);
  analysisMod_MC->SetTriggerObjectsName("myhltobjs");
  analysisMod_MC->SetMetName(pubMet->GetOutputName());
  analysisMod_MC->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_GsfTrack_IsoTrack_PythiaWJets = new WJetsFakeValidationMod;
  analysisMod_GsfTrack_IsoTrack_PythiaWJets->SetName("WJetsFakeValidationMod_GsfTrack_IsoTrack_PythiaWJets");
  analysisMod_GsfTrack_IsoTrack_PythiaWJets->SetUseMCFake(UseMCFakes);
  analysisMod_GsfTrack_IsoTrack_PythiaWJets->SetFakeEventHeaderName("FakeEventHeaders_GsfTrack_IsoTrack_PythiaWJets");
  analysisMod_GsfTrack_IsoTrack_PythiaWJets->SetElectronFakeableObjectsName("ElectronFakeableObjects_GsfTrack");
  analysisMod_GsfTrack_IsoTrack_PythiaWJets->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  analysisMod_GsfTrack_IsoTrack_PythiaWJets->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_GsfTrack_IsoTrack_PythiaWJets->SetSSFakerateCorrection(1.0);
  analysisMod_GsfTrack_IsoTrack_PythiaWJets->SetOSFakerateCorrection(1.0);
  analysisMod_GsfTrack_IsoTrack_PythiaWJets->SetTriggerObjectsName("myhltobjs");
  analysisMod_GsfTrack_IsoTrack_PythiaWJets->SetMetName(pubMet->GetOutputName());
  analysisMod_GsfTrack_IsoTrack_PythiaWJets->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_GsfTrack_IsoTrack_MadgraphWJets = new WJetsFakeValidationMod;
  analysisMod_GsfTrack_IsoTrack_MadgraphWJets->SetName("WJetsFakeValidationMod_GsfTrack_IsoTrack_MadgraphWJets");
  analysisMod_GsfTrack_IsoTrack_MadgraphWJets->SetUseMCFake(UseMCFakes);
  analysisMod_GsfTrack_IsoTrack_MadgraphWJets->SetFakeEventHeaderName("FakeEventHeaders_GsfTrack_IsoTrack_MadgraphWJets");
  analysisMod_GsfTrack_IsoTrack_MadgraphWJets->SetElectronFakeableObjectsName("ElectronFakeableObjects_GsfTrack");
  analysisMod_GsfTrack_IsoTrack_MadgraphWJets->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  analysisMod_GsfTrack_IsoTrack_MadgraphWJets->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_GsfTrack_IsoTrack_MadgraphWJets->SetSSFakerateCorrection(1.0);
  analysisMod_GsfTrack_IsoTrack_MadgraphWJets->SetOSFakerateCorrection(1.0);
  analysisMod_GsfTrack_IsoTrack_MadgraphWJets->SetTriggerObjectsName("myhltobjs");
  analysisMod_GsfTrack_IsoTrack_MadgraphWJets->SetMetName(pubMet->GetOutputName());
  analysisMod_GsfTrack_IsoTrack_MadgraphWJets->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_GsfTrack_IsoTrack_Pythia_Jet50 = new WJetsFakeValidationMod;
  analysisMod_GsfTrack_IsoTrack_Pythia_Jet50->SetName("WJetsFakeValidationMod_GsfTrack_IsoTrack_Pythia_Jet50");
  analysisMod_GsfTrack_IsoTrack_Pythia_Jet50->SetUseMCFake(UseMCFakes);
  analysisMod_GsfTrack_IsoTrack_Pythia_Jet50->SetFakeEventHeaderName("FakeEventHeaders_GsfTrack_IsoTrack_Pythia_Jet50");
  analysisMod_GsfTrack_IsoTrack_Pythia_Jet50->SetElectronFakeableObjectsName("ElectronFakeableObjects_GsfTrack");
  analysisMod_GsfTrack_IsoTrack_Pythia_Jet50->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  analysisMod_GsfTrack_IsoTrack_Pythia_Jet50->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_GsfTrack_IsoTrack_Pythia_Jet50->SetSSFakerateCorrection(1.0);
  analysisMod_GsfTrack_IsoTrack_Pythia_Jet50->SetOSFakerateCorrection(1.0);
  analysisMod_GsfTrack_IsoTrack_Pythia_Jet50->SetTriggerObjectsName("myhltobjs");
  analysisMod_GsfTrack_IsoTrack_Pythia_Jet50->SetMetName(pubMet->GetOutputName());
  analysisMod_GsfTrack_IsoTrack_Pythia_Jet50->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_GsfTrack_IsoTrack_Pythia_Photon15 = new WJetsFakeValidationMod;
  analysisMod_GsfTrack_IsoTrack_Pythia_Photon15->SetName("WJetsFakeValidationMod_GsfTrack_IsoTrack_Pythia_Photon15");
  analysisMod_GsfTrack_IsoTrack_Pythia_Photon15->SetUseMCFake(UseMCFakes);
  analysisMod_GsfTrack_IsoTrack_Pythia_Photon15->SetFakeEventHeaderName("FakeEventHeaders_GsfTrack_IsoTrack_Pythia_Photon15");
  analysisMod_GsfTrack_IsoTrack_Pythia_Photon15->SetElectronFakeableObjectsName("ElectronFakeableObjects_GsfTrack");
  analysisMod_GsfTrack_IsoTrack_Pythia_Photon15->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  analysisMod_GsfTrack_IsoTrack_Pythia_Photon15->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_GsfTrack_IsoTrack_Pythia_Photon15->SetSSFakerateCorrection(1.0);
  analysisMod_GsfTrack_IsoTrack_Pythia_Photon15->SetOSFakerateCorrection(1.0);
  analysisMod_GsfTrack_IsoTrack_Pythia_Photon15->SetTriggerObjectsName("myhltobjs");
  analysisMod_GsfTrack_IsoTrack_Pythia_Photon15->SetMetName(pubMet->GetOutputName());
  analysisMod_GsfTrack_IsoTrack_Pythia_Photon15->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_GsfTrack_IsoTrack_Madgraph_Jet50 = new WJetsFakeValidationMod;
  analysisMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetName("WJetsFakeValidationMod_GsfTrack_IsoTrack_Madgraph_Jet50");
  analysisMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetUseMCFake(UseMCFakes);
  analysisMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetFakeEventHeaderName("FakeEventHeaders_GsfTrack_IsoTrack_Madgraph_Jet50");
  analysisMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetElectronFakeableObjectsName("ElectronFakeableObjects_GsfTrack");
  analysisMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  analysisMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetSSFakerateCorrection(1.0);
  analysisMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetOSFakerateCorrection(1.0);
  analysisMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetTriggerObjectsName("myhltobjs");
  analysisMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetMetName(pubMet->GetOutputName());
  analysisMod_GsfTrack_IsoTrack_Madgraph_Jet50->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_GsfTrack_IsoTrack_Madgraph_Photon15 = new WJetsFakeValidationMod;
  analysisMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetName("WJetsFakeValidationMod_GsfTrack_IsoTrack_Madgraph_Photon15");
  analysisMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetUseMCFake(UseMCFakes);
  analysisMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetFakeEventHeaderName("FakeEventHeaders_GsfTrack_IsoTrack_Madgraph_Photon15");
  analysisMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetElectronFakeableObjectsName("ElectronFakeableObjects_GsfTrack");
  analysisMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  analysisMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetSSFakerateCorrection(1.0);
  analysisMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetOSFakerateCorrection(1.0);
  analysisMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetTriggerObjectsName("myhltobjs");
  analysisMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetMetName(pubMet->GetOutputName());
  analysisMod_GsfTrack_IsoTrack_Madgraph_Photon15->SetSampleName(dataset);


  WJetsFakeValidationMod *analysisMod_Reco_IsoTrack_PythiaWJets = new WJetsFakeValidationMod;
  analysisMod_Reco_IsoTrack_PythiaWJets->SetName("WJetsFakeValidationMod_Reco_IsoTrack_PythiaWJets");
  analysisMod_Reco_IsoTrack_PythiaWJets->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_IsoTrack_PythiaWJets->SetFakeEventHeaderName("FakeEventHeaders_Reco_IsoTrack_PythiaWJets");
  analysisMod_Reco_IsoTrack_PythiaWJets->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_IsoTrack_PythiaWJets->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  analysisMod_Reco_IsoTrack_PythiaWJets->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_IsoTrack_PythiaWJets->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_IsoTrack_PythiaWJets->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_IsoTrack_PythiaWJets->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_IsoTrack_PythiaWJets->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_IsoTrack_PythiaWJets->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_Reco_IsoTrack_MadgraphWJets = new WJetsFakeValidationMod;
  analysisMod_Reco_IsoTrack_MadgraphWJets->SetName("WJetsFakeValidationMod_Reco_IsoTrack_MadgraphWJets");
  analysisMod_Reco_IsoTrack_MadgraphWJets->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_IsoTrack_MadgraphWJets->SetFakeEventHeaderName("FakeEventHeaders_Reco_IsoTrack_MadgraphWJets");
  analysisMod_Reco_IsoTrack_MadgraphWJets->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_IsoTrack_MadgraphWJets->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  analysisMod_Reco_IsoTrack_MadgraphWJets->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_IsoTrack_MadgraphWJets->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_IsoTrack_MadgraphWJets->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_IsoTrack_MadgraphWJets->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_IsoTrack_MadgraphWJets->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_IsoTrack_MadgraphWJets->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_Reco_IsoTrack_Pythia_Jet50 = new WJetsFakeValidationMod;
  analysisMod_Reco_IsoTrack_Pythia_Jet50->SetName("WJetsFakeValidationMod_Reco_IsoTrack_Pythia_Jet50");
  analysisMod_Reco_IsoTrack_Pythia_Jet50->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_IsoTrack_Pythia_Jet50->SetFakeEventHeaderName("FakeEventHeaders_Reco_IsoTrack_Pythia_Jet50");
  analysisMod_Reco_IsoTrack_Pythia_Jet50->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_IsoTrack_Pythia_Jet50->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  analysisMod_Reco_IsoTrack_Pythia_Jet50->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_IsoTrack_Pythia_Jet50->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_IsoTrack_Pythia_Jet50->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_IsoTrack_Pythia_Jet50->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_IsoTrack_Pythia_Jet50->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_IsoTrack_Pythia_Jet50->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_Reco_IsoTrack_Pythia_Photon15 = new WJetsFakeValidationMod;
  analysisMod_Reco_IsoTrack_Pythia_Photon15->SetName("WJetsFakeValidationMod_Reco_IsoTrack_Pythia_Photon15");
  analysisMod_Reco_IsoTrack_Pythia_Photon15->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_IsoTrack_Pythia_Photon15->SetFakeEventHeaderName("FakeEventHeaders_Reco_IsoTrack_Pythia_Photon15");
  analysisMod_Reco_IsoTrack_Pythia_Photon15->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_IsoTrack_Pythia_Photon15->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  analysisMod_Reco_IsoTrack_Pythia_Photon15->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_IsoTrack_Pythia_Photon15->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_IsoTrack_Pythia_Photon15->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_IsoTrack_Pythia_Photon15->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_IsoTrack_Pythia_Photon15->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_IsoTrack_Pythia_Photon15->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_Reco_IsoTrack_Madgraph_Jet50 = new WJetsFakeValidationMod;
  analysisMod_Reco_IsoTrack_Madgraph_Jet50->SetName("WJetsFakeValidationMod_Reco_IsoTrack_Madgraph_Jet50");
  analysisMod_Reco_IsoTrack_Madgraph_Jet50->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_IsoTrack_Madgraph_Jet50->SetFakeEventHeaderName("FakeEventHeaders_Reco_IsoTrack_Madgraph_Jet50");
  analysisMod_Reco_IsoTrack_Madgraph_Jet50->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_IsoTrack_Madgraph_Jet50->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  analysisMod_Reco_IsoTrack_Madgraph_Jet50->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_IsoTrack_Madgraph_Jet50->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_IsoTrack_Madgraph_Jet50->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_IsoTrack_Madgraph_Jet50->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_IsoTrack_Madgraph_Jet50->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_IsoTrack_Madgraph_Jet50->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_Reco_IsoTrack_Madgraph_Photon15 = new WJetsFakeValidationMod;
  analysisMod_Reco_IsoTrack_Madgraph_Photon15->SetName("WJetsFakeValidationMod_Reco_IsoTrack_Madgraph_Photon15");
  analysisMod_Reco_IsoTrack_Madgraph_Photon15->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_IsoTrack_Madgraph_Photon15->SetFakeEventHeaderName("FakeEventHeaders_Reco_IsoTrack_Madgraph_Photon15");
  analysisMod_Reco_IsoTrack_Madgraph_Photon15->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_IsoTrack_Madgraph_Photon15->SetMuonFakeableObjectsName("MuonFakeableObjects_IsoTrack");
  analysisMod_Reco_IsoTrack_Madgraph_Photon15->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_IsoTrack_Madgraph_Photon15->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_IsoTrack_Madgraph_Photon15->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_IsoTrack_Madgraph_Photon15->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_IsoTrack_Madgraph_Photon15->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_IsoTrack_Madgraph_Photon15->SetSampleName(dataset);


  WJetsFakeValidationMod *analysisMod_Reco_TrackerMuon_PythiaWJets = new WJetsFakeValidationMod;
  analysisMod_Reco_TrackerMuon_PythiaWJets->SetName("WJetsFakeValidationMod_Reco_TrackerMuon_PythiaWJets");
  analysisMod_Reco_TrackerMuon_PythiaWJets->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_TrackerMuon_PythiaWJets->SetFakeEventHeaderName("FakeEventHeaders_Reco_TrackerMuon_PythiaWJets");
  analysisMod_Reco_TrackerMuon_PythiaWJets->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_TrackerMuon_PythiaWJets->SetMuonFakeableObjectsName("MuonFakeableObjects_TrackerMuon");
  analysisMod_Reco_TrackerMuon_PythiaWJets->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_TrackerMuon_PythiaWJets->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_TrackerMuon_PythiaWJets->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_TrackerMuon_PythiaWJets->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_TrackerMuon_PythiaWJets->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_TrackerMuon_PythiaWJets->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_Reco_TrackerMuon_MadgraphWJets = new WJetsFakeValidationMod;
  analysisMod_Reco_TrackerMuon_MadgraphWJets->SetName("WJetsFakeValidationMod_Reco_TrackerMuon_MadgraphWJets");
  analysisMod_Reco_TrackerMuon_MadgraphWJets->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_TrackerMuon_MadgraphWJets->SetFakeEventHeaderName("FakeEventHeaders_Reco_TrackerMuon_MadgraphWJets");
  analysisMod_Reco_TrackerMuon_MadgraphWJets->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_TrackerMuon_MadgraphWJets->SetMuonFakeableObjectsName("MuonFakeableObjects_TrackerMuon");
  analysisMod_Reco_TrackerMuon_MadgraphWJets->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_TrackerMuon_MadgraphWJets->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_TrackerMuon_MadgraphWJets->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_TrackerMuon_MadgraphWJets->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_TrackerMuon_MadgraphWJets->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_TrackerMuon_MadgraphWJets->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_Reco_TrackerMuon_Pythia_Jet50 = new WJetsFakeValidationMod;
  analysisMod_Reco_TrackerMuon_Pythia_Jet50->SetName("WJetsFakeValidationMod_Reco_TrackerMuon_Pythia_Jet50");
  analysisMod_Reco_TrackerMuon_Pythia_Jet50->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_TrackerMuon_Pythia_Jet50->SetFakeEventHeaderName("FakeEventHeaders_Reco_TrackerMuon_Pythia_Jet50");
  analysisMod_Reco_TrackerMuon_Pythia_Jet50->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_TrackerMuon_Pythia_Jet50->SetMuonFakeableObjectsName("MuonFakeableObjects_TrackerMuon");
  analysisMod_Reco_TrackerMuon_Pythia_Jet50->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_TrackerMuon_Pythia_Jet50->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_TrackerMuon_Pythia_Jet50->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_TrackerMuon_Pythia_Jet50->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_TrackerMuon_Pythia_Jet50->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_TrackerMuon_Pythia_Jet50->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_Reco_TrackerMuon_Pythia_Photon15 = new WJetsFakeValidationMod;
  analysisMod_Reco_TrackerMuon_Pythia_Photon15->SetName("WJetsFakeValidationMod_Reco_TrackerMuon_Pythia_Photon15");
  analysisMod_Reco_TrackerMuon_Pythia_Photon15->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_TrackerMuon_Pythia_Photon15->SetFakeEventHeaderName("FakeEventHeaders_Reco_TrackerMuon_Pythia_Photon15");
  analysisMod_Reco_TrackerMuon_Pythia_Photon15->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_TrackerMuon_Pythia_Photon15->SetMuonFakeableObjectsName("MuonFakeableObjects_TrackerMuon");
  analysisMod_Reco_TrackerMuon_Pythia_Photon15->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_TrackerMuon_Pythia_Photon15->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_TrackerMuon_Pythia_Photon15->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_TrackerMuon_Pythia_Photon15->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_TrackerMuon_Pythia_Photon15->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_TrackerMuon_Pythia_Photon15->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_Reco_TrackerMuon_Madgraph_Jet50 = new WJetsFakeValidationMod;
  analysisMod_Reco_TrackerMuon_Madgraph_Jet50->SetName("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50");
  analysisMod_Reco_TrackerMuon_Madgraph_Jet50->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_TrackerMuon_Madgraph_Jet50->SetFakeEventHeaderName("FakeEventHeaders_Reco_TrackerMuon_Madgraph_Jet50");
  analysisMod_Reco_TrackerMuon_Madgraph_Jet50->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_TrackerMuon_Madgraph_Jet50->SetMuonFakeableObjectsName("MuonFakeableObjects_TrackerMuon");
  analysisMod_Reco_TrackerMuon_Madgraph_Jet50->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_TrackerMuon_Madgraph_Jet50->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_TrackerMuon_Madgraph_Jet50->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_TrackerMuon_Madgraph_Jet50->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_TrackerMuon_Madgraph_Jet50->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_TrackerMuon_Madgraph_Jet50->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_Reco_TrackerMuon_Madgraph_Photon15 = new WJetsFakeValidationMod;
  analysisMod_Reco_TrackerMuon_Madgraph_Photon15->SetName("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15");
  analysisMod_Reco_TrackerMuon_Madgraph_Photon15->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_TrackerMuon_Madgraph_Photon15->SetFakeEventHeaderName("FakeEventHeaders_Reco_TrackerMuon_Madgraph_Photon15");
  analysisMod_Reco_TrackerMuon_Madgraph_Photon15->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_TrackerMuon_Madgraph_Photon15->SetMuonFakeableObjectsName("MuonFakeableObjects_TrackerMuon");
  analysisMod_Reco_TrackerMuon_Madgraph_Photon15->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_TrackerMuon_Madgraph_Photon15->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_TrackerMuon_Madgraph_Photon15->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_TrackerMuon_Madgraph_Photon15->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_TrackerMuon_Madgraph_Photon15->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_TrackerMuon_Madgraph_Photon15->SetSampleName(dataset);



  WJetsFakeValidationMod *analysisMod_Reco_Global_PythiaWJets = new WJetsFakeValidationMod;
  analysisMod_Reco_Global_PythiaWJets->SetName("WJetsFakeValidationMod_Reco_Global_PythiaWJets");
  analysisMod_Reco_Global_PythiaWJets->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_Global_PythiaWJets->SetFakeEventHeaderName("FakeEventHeaders_Reco_Global_PythiaWJets");
  analysisMod_Reco_Global_PythiaWJets->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_Global_PythiaWJets->SetMuonFakeableObjectsName("MuonFakeableObjects_Global");
  analysisMod_Reco_Global_PythiaWJets->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_Global_PythiaWJets->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_Global_PythiaWJets->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_Global_PythiaWJets->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_Global_PythiaWJets->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_Global_PythiaWJets->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_Reco_Global_MadgraphWJets = new WJetsFakeValidationMod;
  analysisMod_Reco_Global_MadgraphWJets->SetName("WJetsFakeValidationMod_Reco_Global_MadgraphWJets");
  analysisMod_Reco_Global_MadgraphWJets->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_Global_MadgraphWJets->SetFakeEventHeaderName("FakeEventHeaders_Reco_Global_MadgraphWJets");
  analysisMod_Reco_Global_MadgraphWJets->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_Global_MadgraphWJets->SetMuonFakeableObjectsName("MuonFakeableObjects_Global");
  analysisMod_Reco_Global_MadgraphWJets->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_Global_MadgraphWJets->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_Global_MadgraphWJets->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_Global_MadgraphWJets->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_Global_MadgraphWJets->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_Global_MadgraphWJets->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_Reco_Global_Pythia_Jet50 = new WJetsFakeValidationMod;
  analysisMod_Reco_Global_Pythia_Jet50->SetName("WJetsFakeValidationMod_Reco_Global_Pythia_Jet50");
  analysisMod_Reco_Global_Pythia_Jet50->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_Global_Pythia_Jet50->SetFakeEventHeaderName("FakeEventHeaders_Reco_Global_Pythia_Jet50");
  analysisMod_Reco_Global_Pythia_Jet50->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_Global_Pythia_Jet50->SetMuonFakeableObjectsName("MuonFakeableObjects_Global");
  analysisMod_Reco_Global_Pythia_Jet50->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_Global_Pythia_Jet50->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_Global_Pythia_Jet50->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_Global_Pythia_Jet50->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_Global_Pythia_Jet50->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_Global_Pythia_Jet50->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_Reco_Global_Pythia_Photon15 = new WJetsFakeValidationMod;
  analysisMod_Reco_Global_Pythia_Photon15->SetName("WJetsFakeValidationMod_Reco_Global_Pythia_Photon15");
  analysisMod_Reco_Global_Pythia_Photon15->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_Global_Pythia_Photon15->SetFakeEventHeaderName("FakeEventHeaders_Reco_Global_Pythia_Photon15");
  analysisMod_Reco_Global_Pythia_Photon15->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_Global_Pythia_Photon15->SetMuonFakeableObjectsName("MuonFakeableObjects_Global");
  analysisMod_Reco_Global_Pythia_Photon15->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_Global_Pythia_Photon15->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_Global_Pythia_Photon15->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_Global_Pythia_Photon15->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_Global_Pythia_Photon15->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_Global_Pythia_Photon15->SetSampleName(dataset);


  WJetsFakeValidationMod *analysisMod_Reco_Global_Madgraph_Jet50 = new WJetsFakeValidationMod;
  analysisMod_Reco_Global_Madgraph_Jet50->SetName("WJetsFakeValidationMod_Reco_Global_Madgraph_Jet50");
  analysisMod_Reco_Global_Madgraph_Jet50->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_Global_Madgraph_Jet50->SetFakeEventHeaderName("FakeEventHeaders_Reco_Global_Madgraph_Jet50");
  analysisMod_Reco_Global_Madgraph_Jet50->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_Global_Madgraph_Jet50->SetMuonFakeableObjectsName("MuonFakeableObjects_Global");
  analysisMod_Reco_Global_Madgraph_Jet50->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_Global_Madgraph_Jet50->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_Global_Madgraph_Jet50->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_Global_Madgraph_Jet50->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_Global_Madgraph_Jet50->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_Global_Madgraph_Jet50->SetSampleName(dataset);

  WJetsFakeValidationMod *analysisMod_Reco_Global_Madgraph_Photon15 = new WJetsFakeValidationMod;
  analysisMod_Reco_Global_Madgraph_Photon15->SetName("WJetsFakeValidationMod_Reco_Global_Madgraph_Photon15");
  analysisMod_Reco_Global_Madgraph_Photon15->SetUseMCFake(UseMCFakes);
  analysisMod_Reco_Global_Madgraph_Photon15->SetFakeEventHeaderName("FakeEventHeaders_Reco_Global_Madgraph_Photon15");
  analysisMod_Reco_Global_Madgraph_Photon15->SetElectronFakeableObjectsName("ElectronFakeableObjects_Reco");
  analysisMod_Reco_Global_Madgraph_Photon15->SetMuonFakeableObjectsName("MuonFakeableObjects_Global");
  analysisMod_Reco_Global_Madgraph_Photon15->SetPerformFakeMuonMetCorrection(PerformFakeMuonMetCorrection);
  analysisMod_Reco_Global_Madgraph_Photon15->SetSSFakerateCorrection(1.0);
  analysisMod_Reco_Global_Madgraph_Photon15->SetOSFakerateCorrection(1.0);
  analysisMod_Reco_Global_Madgraph_Photon15->SetTriggerObjectsName("myhltobjs");
  analysisMod_Reco_Global_Madgraph_Photon15->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_Global_Madgraph_Photon15->SetSampleName(dataset);


  //------------------------------------------------------------------------------------------------
  // making analysis chain
  //------------------------------------------------------------------------------------------------

  hltmod->Add(generatorMod);

  if ( string(dataset) == "s8-wjets-mg-id11"
       || string(dataset) == "s8-vqq-mg-id11"
       || string(dataset) == "f8-wc-mg-id11"
       || string(dataset) == "s8-zjets-mg-id11"
    ) {
    generatorMod->Add(partonFlavorHistoryMod);
    partonFlavorHistoryMod->Add(pubJet);
  } else {
    generatorMod->Add(pubJet);
  }
  
  pubJet->Add(pubMet);
  pubMet->Add(muonID);
  muonID->Add(electronID);
  electronID->Add(photonID);
  photonID->Add(jetID);
  jetID->Add(electronCleaning);
  electronCleaning->Add(photonCleaning);
  photonCleaning->Add(jetCleaning);
  jetCleaning->Add(mergeLeptonsMod);
  
  if (FakeableObjectType == 1) {
    //original denominator definitions
    mergeLeptonsMod->Add(generateFakeableObjectsMod_GsfTrack_IsoTrack);
    generateFakeableObjectsMod_GsfTrack_IsoTrack->Add(generateFakesMod_GsfTrack_IsoTrack_PythiaWJets);
    generateFakesMod_GsfTrack_IsoTrack_PythiaWJets->Add(analysisMod_GsfTrack_IsoTrack_PythiaWJets);  
  } else if (FakeableObjectType == 2) {
    //tighter denominator definitions
    mergeLeptonsMod->Add(generateFakeableObjectsMod_Reco_Global);
    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_Global_PythiaWJets);
    generateFakesMod_Reco_Global_PythiaWJets->Add(analysisMod_Reco_Global_PythiaWJets);
  } else if (FakeableObjectType == -10) {
    mergeLeptonsMod->Add(generateFakeableObjectsMod_GsfTrack_IsoTrack);
    generateFakeableObjectsMod_GsfTrack_IsoTrack->Add(generateFakeableObjectsMod_Reco_TrackerMuon);
    generateFakeableObjectsMod_Reco_TrackerMuon->Add(generateFakeableObjectsMod_Reco_Global);


    //GsfTrack + IsoTrack
     generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_GsfTrack_IsoTrack_PythiaWJets);
      generateFakesMod_GsfTrack_IsoTrack_PythiaWJets->Add(analysisMod_GsfTrack_IsoTrack_PythiaWJets);  

     generateFakesMod_GsfTrack_IsoTrack_PythiaWJets->Add(analysisMod_MC); 
    
    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_GsfTrack_IsoTrack_MadgraphWJets);
    generateFakesMod_GsfTrack_IsoTrack_MadgraphWJets->Add(analysisMod_GsfTrack_IsoTrack_MadgraphWJets);  
    
    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_GsfTrack_IsoTrack_Pythia_Jet50);
    generateFakesMod_GsfTrack_IsoTrack_Pythia_Jet50->Add(analysisMod_GsfTrack_IsoTrack_Pythia_Jet50);  
    
    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_GsfTrack_IsoTrack_Pythia_Photon15);
    generateFakesMod_GsfTrack_IsoTrack_Pythia_Photon15->Add(analysisMod_GsfTrack_IsoTrack_Pythia_Photon15);  

    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_GsfTrack_IsoTrack_Madgraph_Jet50);
    generateFakesMod_GsfTrack_IsoTrack_Madgraph_Jet50->Add(analysisMod_GsfTrack_IsoTrack_Madgraph_Jet50);  
    
    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_GsfTrack_IsoTrack_Madgraph_Photon15);
    generateFakesMod_GsfTrack_IsoTrack_Madgraph_Photon15->Add(analysisMod_GsfTrack_IsoTrack_Madgraph_Photon15);  
     
     
    //Reco + IsoTrack
    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_IsoTrack_PythiaWJets);
    generateFakesMod_Reco_IsoTrack_PythiaWJets->Add(analysisMod_Reco_IsoTrack_PythiaWJets);
    
    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_IsoTrack_MadgraphWJets);
    generateFakesMod_Reco_IsoTrack_MadgraphWJets->Add(analysisMod_Reco_IsoTrack_MadgraphWJets);
    
    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_IsoTrack_Pythia_Jet50);
    generateFakesMod_Reco_IsoTrack_Pythia_Jet50->Add(analysisMod_Reco_IsoTrack_Pythia_Jet50);

    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_IsoTrack_Pythia_Photon15);
    generateFakesMod_Reco_IsoTrack_Pythia_Photon15->Add(analysisMod_Reco_IsoTrack_Pythia_Photon15);

    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_IsoTrack_Madgraph_Jet50);
    generateFakesMod_Reco_IsoTrack_Madgraph_Jet50->Add(analysisMod_Reco_IsoTrack_Madgraph_Jet50);

    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_IsoTrack_Madgraph_Photon15);
    generateFakesMod_Reco_IsoTrack_Madgraph_Photon15->Add(analysisMod_Reco_IsoTrack_Madgraph_Photon15);

  
    //Reco + TrackerMuon
    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_TrackerMuon_PythiaWJets);
    generateFakesMod_Reco_TrackerMuon_PythiaWJets->Add(analysisMod_Reco_TrackerMuon_PythiaWJets);
    
    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_TrackerMuon_MadgraphWJets);
    generateFakesMod_Reco_TrackerMuon_MadgraphWJets->Add(analysisMod_Reco_TrackerMuon_MadgraphWJets);
        
    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_TrackerMuon_Pythia_Jet50);
    generateFakesMod_Reco_TrackerMuon_Pythia_Jet50->Add(analysisMod_Reco_TrackerMuon_Pythia_Jet50);

    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_TrackerMuon_Pythia_Photon15);
    generateFakesMod_Reco_TrackerMuon_Pythia_Photon15->Add(analysisMod_Reco_TrackerMuon_Pythia_Photon15);

    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_TrackerMuon_Madgraph_Jet50);
    generateFakesMod_Reco_TrackerMuon_Madgraph_Jet50->Add(analysisMod_Reco_TrackerMuon_Madgraph_Jet50);

    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_TrackerMuon_Madgraph_Photon15);
    generateFakesMod_Reco_TrackerMuon_Madgraph_Photon15->Add(analysisMod_Reco_TrackerMuon_Madgraph_Photon15);

    //Reco + Global
    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_Global_PythiaWJets);
    generateFakesMod_Reco_Global_PythiaWJets->Add(analysisMod_Reco_Global_PythiaWJets);
    
    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_Global_MadgraphWJets);
    generateFakesMod_Reco_Global_MadgraphWJets->Add(analysisMod_Reco_Global_MadgraphWJets);
        
    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_Global_Pythia_Jet50);
    generateFakesMod_Reco_Global_Pythia_Jet50->Add(analysisMod_Reco_Global_Pythia_Jet50);

    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_Global_Pythia_Photon15);
    generateFakesMod_Reco_Global_Pythia_Photon15->Add(analysisMod_Reco_Global_Pythia_Photon15);

    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_Global_Madgraph_Jet50);
    generateFakesMod_Reco_Global_Madgraph_Jet50->Add(analysisMod_Reco_Global_Madgraph_Jet50);

    generateFakeableObjectsMod_Reco_Global->Add(generateFakesMod_Reco_Global_Madgraph_Photon15);
    generateFakesMod_Reco_Global_Madgraph_Photon15->Add(analysisMod_Reco_Global_Madgraph_Photon15);
   
  }

  //------------------------------------------------------------------------------------------------
  // setup analysis
  //------------------------------------------------------------------------------------------------
  Analysis *ana = new Analysis;
  ana->SetUseHLT(kTRUE);
  ana->SetKeepHierarchy(kTRUE);
   if (nEvents >= 0)
    ana->SetProcessNEvents(nEvents);
  ana->SetSuperModule(hltmod);

  //------------------------------------------------------------------------------------------------
  // organize input
  //------------------------------------------------------------------------------------------------
  printf("\nRely on Catalog: %s\n",catalogDir);
  printf("  -> Book: %s  Dataset: %s  Skim: %s  Fileset: %s <-\n\n",book,dataset,skim,fileset);
  Catalog *c = new Catalog(catalogDir);
  TString skimdataset = TString(dataset)+TString("/") +TString(skim);
  Dataset *d = NULL;
  if (TString(skim).CompareTo("noskim") == 0)
    d = c->FindDataset(book,dataset,fileset);
  else 
    d = c->FindDataset(book,skimdataset.Data(),fileset);
  ana->AddDataset(d);

  //------------------------------------------------------------------------------------------------
  // organize output
  //------------------------------------------------------------------------------------------------
  TString rootFile = TString(outputName);
  rootFile += TString("_") + TString(dataset) + TString("_") + TString(skim);
  if (TString(fileset) != TString(""))
    rootFile += TString("_") + TString(fileset);
  rootFile += TString(".root");
  printf("\nRoot output: %s\n\n",rootFile.Data());  
  ana->SetOutputName(rootFile.Data());

  //------------------------------------------------------------------------------------------------
  // run the analysis after successful initialisation
  //------------------------------------------------------------------------------------------------
  ana->Run(!gROOT->IsBatch());  

  return;
}

//--------------------------------------------------------------------------------------------------
void runWJetsFakeValidationStudy(const char *fileset      = "",
                                 const char *skim         = "noskim",
                                 const char *dataset      = "s8-ttbar-id9",
                                 const char *book         = "mit/filler/006",
                                 const char *catalogDir   = "/home/mitprod/catalog",
                                 const char *outputName   = "WJetsFakeValidation",
                                 int         nEvents      = -1, 
                                 int         runTypeIndex = -1)
{

  TString outfileName = TString(outputName);

  if (runTypeIndex == 1) {
    //Use original FO definitions
    executeWJetsFakeValidationStudy(fileset,skim,dataset,book,catalogDir,outfileName,
                                    false,true,1,nEvents);
  } else if (runTypeIndex == 2) {
    //Use MC Fakes
    executeWJetsFakeValidationStudy(fileset,skim,dataset,book,catalogDir,outfileName,
                                    true,false,1,nEvents);
  } else if (runTypeIndex == 3) {
    //Use tighter FO definitions
    executeWJetsFakeValidationStudy(fileset,skim,dataset,book,catalogDir,outfileName,
                                    false,true,2,nEvents);
  } else if (runTypeIndex == -10) {
    //run all definitions
    executeWJetsFakeValidationStudy(fileset,skim,dataset,book,catalogDir,outfileName,
                                    false,true,-10,nEvents);
  }

}

