//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeMuonFakeRate.C+\(\"0000\",\"noskim\",\"s8-wjets-mg-id11\",\"mit/filler/009\",\"/home/mitprod/catalog\",\"ComputeMuonFakeRateTest\",10000,3\)
//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeMuonFakeRate.C+\(\"0000\",\"noskim\",\"s8-vqq-mg-id11\",\"mit/filler/009\",\"/home/mitprod/catalog\",\"ComputeMuonFakeRateTest\",10000,4\)
//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeMuonFakeRate.C+\(\"0003\",\"noskim\",\"f8-qcd100_250-id11\",\"mit/filler/009\",\"/home/mitprod/catalog\",\"ComputeMuonFakeRateTest\",10000,10\)

//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeMuonFakeRate.C+\(\"0005\",\"noskim\",\"s8-pj100_200-id11\",\"mit/filler/009\",\"/home/mitprod/catalog\",\"ComputeMuonFakeRateTest\",10000,20\)
//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeMuonFakeRate.C+\(\"0005\",\"noskim\",\"f8-qcd100_250-id11\",\"mit/filler/009\",\"/home/mitprod/catalog\",\"ComputeMuonFakeRateTest\",10000,20\)
//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeMuonFakeRate.C+\(\"0000\",\"noskim\",\"s8-zjets-mg-id11\",\"mit/filler/009\",\"/home/mitprod/catalog\",\"ComputeMuonFakeRateTest\",10000,20\)



// $Id: runComputeMuonFakeRate.C,v 1.3 2009/06/30 08:11:15 phedex Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <exception>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/Catalog/interface/Catalog.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitAna/TreeMod/interface/HLTMod.h"
#include "MitAna/PhysicsMod/interface/PublisherMod.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitPhysics/Mods/interface/GeneratorMod.h"
#include "MitPhysics/Mods/interface/PartonFlavorHistoryMod.h"
#include "MitPhysics/Mods/interface/MuonIDMod.h"
#include "MitPhysics/Mods/interface/ElectronIDMod.h"
#include "MitPhysics/Mods/interface/PhotonIDMod.h"
#include "MitPhysics/Mods/interface/JetIDMod.h"
#include "MitPhysics/Mods/interface/ElectronCleaningMod.h"
#include "MitPhysics/Mods/interface/PhotonCleaningMod.h"
#include "MitPhysics/Mods/interface/JetCleaningMod.h"
#include "MitPhysics/Mods/interface/GenerateFakeableObjectsMod.h"
#include "MitPhysics/Mods/interface/MergeLeptonsMod.h"
#include "MitHiggs/FakeRateMods/interface/ComputeMuonFakeRateMod.h"
#endif
//--------------------------------------------------------------------------------------------------
void executeComputeMuonFakeRateMod(const char *fileset  = "",
                                   const char *skim         = "noskim",
                                   const char *dataset    = "s8-ttbar-id9",
                                   const char *book       = "mit/filler/006",
                                   const char *catalogDir = "/home/mitprod/catalog",
                                   const char *outputName = "ComputeMuonFakeRateMod",
                                   Int_t       sampletype = 0,
                                   int         nEvents    = -1)
{
  //------------------------------------------------------------------------------------------------
  // some global setups
  //------------------------------------------------------------------------------------------------
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 3;

  string mctype = "";
  Bool_t VetoTriggerJet = false;
  Bool_t VetoGenLeptons = true;
  if (sampletype == 10) {
    VetoTriggerJet = true;
    VetoGenLeptons = false;
  }
  if (sampletype == 20) {
    VetoGenLeptons = false;
  }

  if (sampletype == 3)
    mctype = "kMCTypeVLightJets";
  if (sampletype == 4)
    mctype = "kMCTypeVQQ";
  if (sampletype == 5)
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
  HLTMod *hltmod_Jet30 = new HLTMod;
  hltmod_Jet30->SetName("HLTMod_Jet30");
  hltmod_Jet30->SetPrintTable(kFALSE);
  hltmod_Jet30->AddTrigger("HLT_Jet30");
  hltmod_Jet30->SetTrigObjsName("myhltobjs_Jet30");

  HLTMod *hltmod_Jet50 = new HLTMod;
  hltmod_Jet50->SetName("HLTMod_Jet50");
  hltmod_Jet50->SetPrintTable(kFALSE);
  hltmod_Jet50->AddTrigger("HLT_Jet50");
  hltmod_Jet50->SetTrigObjsName("myhltobjs_Jet50");

  HLTMod *hltmod_Jet80 = new HLTMod;
  hltmod_Jet80->SetName("HLTMod_Jet80");
  hltmod_Jet80->SetPrintTable(kFALSE);
  hltmod_Jet80->AddTrigger("HLT_Jet80");
  hltmod_Jet80->SetTrigObjsName("myhltobjs_Jet80");

  HLTMod *hltmod_Jet110 = new HLTMod;
  hltmod_Jet110->SetName("HLTMod_Jet110");
  hltmod_Jet110->SetPrintTable(kFALSE);
  hltmod_Jet110->AddTrigger("HLT_Jet110");
  hltmod_Jet110->SetTrigObjsName("myhltobjs_Jet110");

  HLTMod *hltmod_Photon15 = new HLTMod;
  hltmod_Photon15->SetName("HLTMod_Photon15");
  hltmod_Photon15->SetPrintTable(kFALSE);
  hltmod_Photon15->AddTrigger("HLT_Photon15_L1R");
  hltmod_Photon15->SetTrigObjsName("myhltobjs_Photon15");

  HLTMod *hltmod_Photon25 = new HLTMod;
  hltmod_Photon25->SetName("HLTMod_Photon25");
  hltmod_Photon25->SetPrintTable(kFALSE);
  hltmod_Photon25->AddTrigger("HLT_Photon25_L1R");
  hltmod_Photon25->SetTrigObjsName("myhltobjs_Photon25");

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
  photonID->SetIDType(TString("Custom"));
  photonID->SetIsoType(TString("CombinedIso"));
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
  // generate fakeable objects
  //------------------------------------------------------------------------------------------------
  GenerateFakeableObjectsMod *generateFakeableObjectsMod = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod->SetName("GenerateFakeableObjectsMod");
  generateFakeableObjectsMod->SetElectronFOType("GsfPlusSC");
  generateFakeableObjectsMod->SetMuonFOType("IsoTrack");
  generateFakeableObjectsMod->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod->SetElectronFakeableObjectsName("GsfPlusSCElectronFakeableObjects");
  generateFakeableObjectsMod->SetMuonFakeableObjectsName("IsoTrackMuonFakeableObjects");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Global = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Global->SetName("GenerateFakeableObjectsMod_Global");
  generateFakeableObjectsMod_Global->SetElectronFOType("Reco");
  generateFakeableObjectsMod_Global->SetMuonFOType("Global");
  generateFakeableObjectsMod_Global->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_Global->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_Global->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_Global->SetElectronFakeableObjectsName("RecoElectronFakeableObjects");
  generateFakeableObjectsMod_Global->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_TrackerMuon = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_TrackerMuon->SetName("GenerateFakeableObjectsMod_TrackerMuon");
  generateFakeableObjectsMod_TrackerMuon->SetElectronFOType("Reco");
  generateFakeableObjectsMod_TrackerMuon->SetMuonFOType("TrackerMuon");
  generateFakeableObjectsMod_TrackerMuon->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_TrackerMuon->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_TrackerMuon->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_TrackerMuon->SetElectronFakeableObjectsName("RecoElectronFakeableObjects2");
  generateFakeableObjectsMod_TrackerMuon->SetMuonFakeableObjectsName("TrackerMuonFakeableObjects");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Jet30 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Jet30->SetName("GenerateFakeableObjectsMod_Jet30");
  generateFakeableObjectsMod_Jet30->SetElectronFOType("GsfPlusSC");
  generateFakeableObjectsMod_Jet30->SetMuonFOType("IsoTrack");
  generateFakeableObjectsMod_Jet30->SetTriggerName("HLT_Jet30");
  generateFakeableObjectsMod_Jet30->SetTriggerObjectsName("myhltobjs_Jet30");
  generateFakeableObjectsMod_Jet30->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_Jet30->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_Jet30->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_Jet30->SetElectronFakeableObjectsName("GsfPlusSCElectronFakeableObjects_Jet30");
  generateFakeableObjectsMod_Jet30->SetMuonFakeableObjectsName("IsoTrackMuonFakeableObjects_Jet30");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Jet50 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Jet50->SetName("GenerateFakeableObjectsMod_Jet50");
  generateFakeableObjectsMod_Jet50->SetElectronFOType("GsfPlusSC");
  generateFakeableObjectsMod_Jet50->SetMuonFOType("IsoTrack");
  generateFakeableObjectsMod_Jet50->SetTriggerName("HLT_Jet50");
  generateFakeableObjectsMod_Jet50->SetTriggerObjectsName("myhltobjs_Jet50");
  generateFakeableObjectsMod_Jet50->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_Jet50->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_Jet50->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_Jet50->SetElectronFakeableObjectsName("GsfPlusSCElectronFakeableObjects_Jet50");
  generateFakeableObjectsMod_Jet50->SetMuonFakeableObjectsName("IsoTrackMuonFakeableObjects_Jet50");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Jet80 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Jet80->SetName("GenerateFakeableObjectsMod_Jet80");
  generateFakeableObjectsMod_Jet80->SetElectronFOType("GsfPlusSC");
  generateFakeableObjectsMod_Jet80->SetMuonFOType("IsoTrack");
  generateFakeableObjectsMod_Jet80->SetTriggerName("HLT_Jet80");
  generateFakeableObjectsMod_Jet80->SetTriggerObjectsName("myhltobjs_Jet80");
  generateFakeableObjectsMod_Jet80->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_Jet80->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_Jet80->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_Jet80->SetElectronFakeableObjectsName("GsfPlusSCElectronFakeableObjects_Jet80");
  generateFakeableObjectsMod_Jet80->SetMuonFakeableObjectsName("IsoTrackMuonFakeableObjects_Jet80");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Jet110 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Jet110->SetName("GenerateFakeableObjectsMod_Jet110");
  generateFakeableObjectsMod_Jet110->SetElectronFOType("GsfPlusSC");
  generateFakeableObjectsMod_Jet110->SetMuonFOType("IsoTrack");
  generateFakeableObjectsMod_Jet110->SetTriggerName("HLT_Jet110");
  generateFakeableObjectsMod_Jet110->SetTriggerObjectsName("myhltobjs_Jet110");
  generateFakeableObjectsMod_Jet110->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_Jet110->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_Jet110->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_Jet110->SetElectronFakeableObjectsName("GsfPlusSCElectronFakeableObjects_Jet110");
  generateFakeableObjectsMod_Jet110->SetMuonFakeableObjectsName("IsoTrackMuonFakeableObjects_Jet110");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Global_Jet30 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Global_Jet30->SetName("GenerateFakeableObjectsMod_Global_Jet30");
  generateFakeableObjectsMod_Global_Jet30->SetElectronFOType("Reco");
  generateFakeableObjectsMod_Global_Jet30->SetMuonFOType("Global");
  generateFakeableObjectsMod_Global_Jet30->SetTriggerName("HLT_Jet30");
  generateFakeableObjectsMod_Global_Jet30->SetTriggerObjectsName("myhltobjs_Jet30");
  generateFakeableObjectsMod_Global_Jet30->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_Global_Jet30->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_Global_Jet30->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_Global_Jet30->SetElectronFakeableObjectsName("RecoElectronFakeableObjects_Jet30");
  generateFakeableObjectsMod_Global_Jet30->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects_Jet30");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Global_Jet50 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Global_Jet50->SetName("GenerateFakeableObjectsMod_Global_Jet50");
  generateFakeableObjectsMod_Global_Jet50->SetElectronFOType("Reco");
  generateFakeableObjectsMod_Global_Jet50->SetMuonFOType("Global");
  generateFakeableObjectsMod_Global_Jet50->SetTriggerName("HLT_Jet50");
  generateFakeableObjectsMod_Global_Jet50->SetTriggerObjectsName("myhltobjs_Jet50");
  generateFakeableObjectsMod_Global_Jet50->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_Global_Jet50->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_Global_Jet50->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_Global_Jet50->SetElectronFakeableObjectsName("RecoElectronFakeableObjects_Jet50");
  generateFakeableObjectsMod_Global_Jet50->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects_Jet50");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Global_Jet80 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Global_Jet80->SetName("GenerateFakeableObjectsMod_Global_Jet80");
  generateFakeableObjectsMod_Global_Jet80->SetElectronFOType("Reco");
  generateFakeableObjectsMod_Global_Jet80->SetMuonFOType("Global");
  generateFakeableObjectsMod_Global_Jet80->SetTriggerName("HLT_Jet80");
  generateFakeableObjectsMod_Global_Jet80->SetTriggerObjectsName("myhltobjs_Jet80");
  generateFakeableObjectsMod_Global_Jet80->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_Global_Jet80->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_Global_Jet80->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_Global_Jet80->SetElectronFakeableObjectsName("RecoElectronFakeableObjects_Jet80");
  generateFakeableObjectsMod_Global_Jet80->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects_Jet80");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Global_Jet110 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Global_Jet110->SetName("GenerateFakeableObjectsMod_Global_Jet110");
  generateFakeableObjectsMod_Global_Jet110->SetElectronFOType("Reco");
  generateFakeableObjectsMod_Global_Jet110->SetMuonFOType("Global");
  generateFakeableObjectsMod_Global_Jet110->SetTriggerName("HLT_Jet110");
  generateFakeableObjectsMod_Global_Jet110->SetTriggerObjectsName("myhltobjs_Jet110");
  generateFakeableObjectsMod_Global_Jet110->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_Global_Jet110->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_Global_Jet110->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_Global_Jet110->SetElectronFakeableObjectsName("RecoElectronFakeableObjects_Jet110");
  generateFakeableObjectsMod_Global_Jet110->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects_Jet110");


  GenerateFakeableObjectsMod *generateFakeableObjectsMod_TrackerMuon_Jet30 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_TrackerMuon_Jet30->SetName("GenerateFakeableObjectsMod_TrackerMuon_Jet30");
  generateFakeableObjectsMod_TrackerMuon_Jet30->SetElectronFOType("Reco");
  generateFakeableObjectsMod_TrackerMuon_Jet30->SetMuonFOType("TrackerMuon");
  generateFakeableObjectsMod_TrackerMuon_Jet30->SetTriggerName("HLT_Jet30");
  generateFakeableObjectsMod_TrackerMuon_Jet30->SetTriggerObjectsName("myhltobjs_Jet30");
  generateFakeableObjectsMod_TrackerMuon_Jet30->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_TrackerMuon_Jet30->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_TrackerMuon_Jet30->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_TrackerMuon_Jet30->SetElectronFakeableObjectsName("RecoElectronFakeableObjects2_Jet30");
  generateFakeableObjectsMod_TrackerMuon_Jet30->SetMuonFakeableObjectsName("TrackerMuonFakeableObjects_Jet30");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_TrackerMuon_Jet50 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_TrackerMuon_Jet50->SetName("GenerateFakeableObjectsMod_TrackerMuon_Jet50");
  generateFakeableObjectsMod_TrackerMuon_Jet50->SetElectronFOType("Reco");
  generateFakeableObjectsMod_TrackerMuon_Jet50->SetMuonFOType("TrackerMuon");
  generateFakeableObjectsMod_TrackerMuon_Jet50->SetTriggerName("HLT_Jet50");
  generateFakeableObjectsMod_TrackerMuon_Jet50->SetTriggerObjectsName("myhltobjs_Jet50");
  generateFakeableObjectsMod_TrackerMuon_Jet50->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_TrackerMuon_Jet50->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_TrackerMuon_Jet50->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_TrackerMuon_Jet50->SetElectronFakeableObjectsName("RecoElectronFakeableObjects2_Jet50");
  generateFakeableObjectsMod_TrackerMuon_Jet50->SetMuonFakeableObjectsName("TrackerMuonFakeableObjects_Jet50");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_TrackerMuon_Jet80 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_TrackerMuon_Jet80->SetName("GenerateFakeableObjectsMod_TrackerMuon_Jet80");
  generateFakeableObjectsMod_TrackerMuon_Jet80->SetElectronFOType("Reco");
  generateFakeableObjectsMod_TrackerMuon_Jet80->SetMuonFOType("TrackerMuon");
  generateFakeableObjectsMod_TrackerMuon_Jet80->SetTriggerName("HLT_Jet80");
  generateFakeableObjectsMod_TrackerMuon_Jet80->SetTriggerObjectsName("myhltobjs_Jet80");
  generateFakeableObjectsMod_TrackerMuon_Jet80->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_TrackerMuon_Jet80->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_TrackerMuon_Jet80->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_TrackerMuon_Jet80->SetElectronFakeableObjectsName("RecoElectronFakeableObjects2_Jet80");
  generateFakeableObjectsMod_TrackerMuon_Jet80->SetMuonFakeableObjectsName("TrackerMuonFakeableObjects_Jet80");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_TrackerMuon_Jet110 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_TrackerMuon_Jet110->SetName("GenerateFakeableObjectsMod_TrackerMuon_Jet110");
  generateFakeableObjectsMod_TrackerMuon_Jet110->SetElectronFOType("Reco");
  generateFakeableObjectsMod_TrackerMuon_Jet110->SetMuonFOType("TrackerMuon");
  generateFakeableObjectsMod_TrackerMuon_Jet110->SetTriggerName("HLT_Jet110");
  generateFakeableObjectsMod_TrackerMuon_Jet110->SetTriggerObjectsName("myhltobjs_Jet110");
  generateFakeableObjectsMod_TrackerMuon_Jet110->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_TrackerMuon_Jet110->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_TrackerMuon_Jet110->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_TrackerMuon_Jet110->SetElectronFakeableObjectsName("RecoElectronFakeableObjects2_Jet110");
  generateFakeableObjectsMod_TrackerMuon_Jet110->SetMuonFakeableObjectsName("TrackerMuonFakeableObjects_Jet110");

  //------------------------------------------------------------------------------------------------
  // analyses modules
  //------------------------------------------------------------------------------------------------

  ComputeMuonFakeRateMod *analysisMod = new ComputeMuonFakeRateMod;
  analysisMod->SetName("ComputeMuonFakeRateMod");
  analysisMod->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod->SetMuonFakeableObjectsName("IsoTrackMuonFakeableObjects");
  analysisMod->SetMetName(pubMet->GetOutputName());
  analysisMod->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_Global = new ComputeMuonFakeRateMod;
  analysisMod_Global->SetName("ComputeMuonFakeRateMod_Global");
  analysisMod_Global->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Global->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Global->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects");
  analysisMod_Global->SetMetName(pubMet->GetOutputName());
  analysisMod_Global->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_TrackerMuon = new ComputeMuonFakeRateMod;
  analysisMod_TrackerMuon->SetName("ComputeMuonFakeRateMod_TrackerMuon");
  analysisMod_TrackerMuon->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_TrackerMuon->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_TrackerMuon->SetMuonFakeableObjectsName("TrackerMuonFakeableObjects");
  analysisMod_TrackerMuon->SetMetName(pubMet->GetOutputName());
  analysisMod_TrackerMuon->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_Jet30 = new ComputeMuonFakeRateMod;
  analysisMod_Jet30->SetName("ComputeMuonFakeRateMod_Jet30");
  analysisMod_Jet30->SetTriggerName("HLT_Jet30");
  analysisMod_Jet30->SetTriggerObjectsName("myhltobjs_Jet30");
  analysisMod_Jet30->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Jet30->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Jet30->SetMuonFakeableObjectsName("IsoTrackMuonFakeableObjects_Jet30");
  analysisMod_Jet30->SetMetName(pubMet->GetOutputName());
  analysisMod_Jet30->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_Jet50 = new ComputeMuonFakeRateMod;
  analysisMod_Jet50->SetName("ComputeMuonFakeRateMod_Jet50");
  analysisMod_Jet50->SetTriggerName("HLT_Jet50");
  analysisMod_Jet50->SetTriggerObjectsName("myhltobjs_Jet50");
  analysisMod_Jet50->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Jet50->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Jet50->SetMuonFakeableObjectsName("IsoTrackMuonFakeableObjects_Jet50");
  analysisMod_Jet50->SetMetName(pubMet->GetOutputName());
  analysisMod_Jet50->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_Jet80 = new ComputeMuonFakeRateMod;
  analysisMod_Jet80->SetName("ComputeMuonFakeRateMod_Jet80");
  analysisMod_Jet80->SetTriggerName("HLT_Jet80");
  analysisMod_Jet80->SetTriggerObjectsName("myhltobjs_Jet80");
  analysisMod_Jet80->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Jet80->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Jet80->SetMuonFakeableObjectsName("IsoTrackMuonFakeableObjects_Jet80");
  analysisMod_Jet80->SetMetName(pubMet->GetOutputName());
  analysisMod_Jet80->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_Jet110 = new ComputeMuonFakeRateMod;
  analysisMod_Jet110->SetName("ComputeMuonFakeRateMod_Jet110");
  analysisMod_Jet110->SetTriggerName("HLT_Jet110");
  analysisMod_Jet110->SetTriggerObjectsName("myhltobjs_Jet110");
  analysisMod_Jet110->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Jet110->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Jet110->SetMuonFakeableObjectsName("IsoTrackMuonFakeableObjects_Jet110");
  analysisMod_Jet110->SetMetName(pubMet->GetOutputName());
  analysisMod_Jet110->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_Global_Jet30 = new ComputeMuonFakeRateMod;
  analysisMod_Global_Jet30->SetName("ComputeMuonFakeRateMod_Global_Jet30");
  analysisMod_Global_Jet30->SetTriggerName("HLT_Jet30");
  analysisMod_Global_Jet30->SetTriggerObjectsName("myhltobjs_Jet30");
  analysisMod_Global_Jet30->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Global_Jet30->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Global_Jet30->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects_Jet30");
  analysisMod_Global_Jet30->SetMetName(pubMet->GetOutputName());
  analysisMod_Global_Jet30->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_Global_Jet50 = new ComputeMuonFakeRateMod;
  analysisMod_Global_Jet50->SetName("ComputeMuonFakeRateMod_Global_Jet50");
  analysisMod_Global_Jet50->SetTriggerName("HLT_Jet50");
  analysisMod_Global_Jet50->SetTriggerObjectsName("myhltobjs_Jet50");
  analysisMod_Global_Jet50->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Global_Jet50->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Global_Jet50->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects_Jet50");
  analysisMod_Global_Jet50->SetMetName(pubMet->GetOutputName());
  analysisMod_Global_Jet50->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_Global_Jet80 = new ComputeMuonFakeRateMod;
  analysisMod_Global_Jet80->SetName("ComputeMuonFakeRateMod_Global_Jet80");
  analysisMod_Global_Jet80->SetTriggerName("HLT_Jet80");
  analysisMod_Global_Jet80->SetTriggerObjectsName("myhltobjs_Jet80");
  analysisMod_Global_Jet80->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Global_Jet80->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Global_Jet80->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects_Jet80");
  analysisMod_Global_Jet80->SetMetName(pubMet->GetOutputName());
  analysisMod_Global_Jet80->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_Global_Jet110 = new ComputeMuonFakeRateMod;
  analysisMod_Global_Jet110->SetName("ComputeMuonFakeRateMod_Global_Jet110");
  analysisMod_Global_Jet110->SetTriggerName("HLT_Jet110");
  analysisMod_Global_Jet110->SetTriggerObjectsName("myhltobjs_Jet110");
  analysisMod_Global_Jet110->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Global_Jet110->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Global_Jet110->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects_Jet110");
  analysisMod_Global_Jet110->SetMetName(pubMet->GetOutputName());
  analysisMod_Global_Jet110->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_TrackerMuon_Jet30 = new ComputeMuonFakeRateMod;
  analysisMod_TrackerMuon_Jet30->SetName("ComputeMuonFakeRateMod_TrackerMuon_Jet30");
  analysisMod_TrackerMuon_Jet30->SetTriggerName("HLT_Jet30");
  analysisMod_TrackerMuon_Jet30->SetTriggerObjectsName("myhltobjs_Jet30");
  analysisMod_TrackerMuon_Jet30->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_TrackerMuon_Jet30->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_TrackerMuon_Jet30->SetMuonFakeableObjectsName("TrackerMuonFakeableObjects_Jet30");
  analysisMod_TrackerMuon_Jet30->SetMetName(pubMet->GetOutputName());
  analysisMod_TrackerMuon_Jet30->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_TrackerMuon_Jet50 = new ComputeMuonFakeRateMod;
  analysisMod_TrackerMuon_Jet50->SetName("ComputeMuonFakeRateMod_TrackerMuon_Jet50");
  analysisMod_TrackerMuon_Jet50->SetTriggerName("HLT_Jet50");
  analysisMod_TrackerMuon_Jet50->SetTriggerObjectsName("myhltobjs_Jet50");
  analysisMod_TrackerMuon_Jet50->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_TrackerMuon_Jet50->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_TrackerMuon_Jet50->SetMuonFakeableObjectsName("TrackerMuonFakeableObjects_Jet50");
  analysisMod_TrackerMuon_Jet50->SetMetName(pubMet->GetOutputName());
  analysisMod_TrackerMuon_Jet50->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_TrackerMuon_Jet80 = new ComputeMuonFakeRateMod;
  analysisMod_TrackerMuon_Jet80->SetName("ComputeMuonFakeRateMod_TrackerMuon_Jet80");
  analysisMod_TrackerMuon_Jet80->SetTriggerName("HLT_Jet80");
  analysisMod_TrackerMuon_Jet80->SetTriggerObjectsName("myhltobjs_Jet80");
  analysisMod_TrackerMuon_Jet80->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_TrackerMuon_Jet80->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_TrackerMuon_Jet80->SetMuonFakeableObjectsName("TrackerMuonFakeableObjects_Jet80");
  analysisMod_TrackerMuon_Jet80->SetMetName(pubMet->GetOutputName());
  analysisMod_TrackerMuon_Jet80->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_TrackerMuon_Jet110 = new ComputeMuonFakeRateMod;
  analysisMod_TrackerMuon_Jet110->SetName("ComputeMuonFakeRateMod_TrackerMuon_Jet110");
  analysisMod_TrackerMuon_Jet110->SetTriggerName("HLT_Jet110");
  analysisMod_TrackerMuon_Jet110->SetTriggerObjectsName("myhltobjs_Jet110");
  analysisMod_TrackerMuon_Jet110->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_TrackerMuon_Jet110->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_TrackerMuon_Jet110->SetMuonFakeableObjectsName("TrackerMuonFakeableObjects_Jet110");
  analysisMod_TrackerMuon_Jet110->SetMetName(pubMet->GetOutputName());
  analysisMod_TrackerMuon_Jet110->SetSampleType(sampletype);


  ComputeMuonFakeRateMod *analysisMod_Photon15 = new ComputeMuonFakeRateMod;
  analysisMod_Photon15->SetName("ComputeMuonFakeRateMod_Photon15");
  analysisMod_Photon15->SetTriggerName("HLT_Photon15_L1R");
  analysisMod_Photon15->SetTriggerObjectsName("myhltobjs_Photon15");
  analysisMod_Photon15->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Photon15->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Photon15->SetMuonFakeableObjectsName("IsoTrackMuonFakeableObjects");
  analysisMod_Photon15->SetMetName(pubMet->GetOutputName());
  analysisMod_Photon15->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_Photon25 = new ComputeMuonFakeRateMod;
  analysisMod_Photon25->SetName("ComputeMuonFakeRateMod_Photon25");
  analysisMod_Photon25->SetTriggerName("HLT_Photon25_L1R");
  analysisMod_Photon25->SetTriggerObjectsName("myhltobjs_Photon25");
  analysisMod_Photon25->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Photon25->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Photon25->SetMuonFakeableObjectsName("IsoTrackMuonFakeableObjects");
  analysisMod_Photon25->SetMetName(pubMet->GetOutputName());
  analysisMod_Photon25->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_Global_Photon15 = new ComputeMuonFakeRateMod;
  analysisMod_Global_Photon15->SetName("ComputeMuonFakeRateMod_Global_Photon15");
  analysisMod_Global_Photon15->SetTriggerName("HLT_Photon15_L1R");
  analysisMod_Global_Photon15->SetTriggerObjectsName("myhltobjs_Photon15");
  analysisMod_Global_Photon15->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Global_Photon15->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Global_Photon15->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects");
  analysisMod_Global_Photon15->SetMetName(pubMet->GetOutputName());
  analysisMod_Global_Photon15->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_Global_Photon25 = new ComputeMuonFakeRateMod;
  analysisMod_Global_Photon25->SetName("ComputeMuonFakeRateMod_Global_Photon25");
  analysisMod_Global_Photon25->SetTriggerName("HLT_Photon25_L1R");
  analysisMod_Global_Photon25->SetTriggerObjectsName("myhltobjs_Photon25");
  analysisMod_Global_Photon25->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Global_Photon25->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Global_Photon25->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects");
  analysisMod_Global_Photon25->SetMetName(pubMet->GetOutputName());
  analysisMod_Global_Photon25->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_TrackerMuon_Photon15 = new ComputeMuonFakeRateMod;
  analysisMod_TrackerMuon_Photon15->SetName("ComputeMuonFakeRateMod_TrackerMuon_Photon15");
  analysisMod_TrackerMuon_Photon15->SetTriggerName("HLT_Photon15_L1R");
  analysisMod_TrackerMuon_Photon15->SetTriggerObjectsName("myhltobjs_Photon15");
  analysisMod_TrackerMuon_Photon15->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_TrackerMuon_Photon15->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_TrackerMuon_Photon15->SetMuonFakeableObjectsName("TrackerMuonFakeableObjects");
  analysisMod_TrackerMuon_Photon15->SetMetName(pubMet->GetOutputName());
  analysisMod_TrackerMuon_Photon15->SetSampleType(sampletype);

  ComputeMuonFakeRateMod *analysisMod_TrackerMuon_Photon25 = new ComputeMuonFakeRateMod;
  analysisMod_TrackerMuon_Photon25->SetName("ComputeMuonFakeRateMod_TrackerMuon_Photon25");
  analysisMod_TrackerMuon_Photon25->SetTriggerName("HLT_Photon25_L1R");
  analysisMod_TrackerMuon_Photon25->SetTriggerObjectsName("myhltobjs_Photon25");
  analysisMod_TrackerMuon_Photon25->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_TrackerMuon_Photon25->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_TrackerMuon_Photon25->SetMuonFakeableObjectsName("TrackerMuonFakeableObjects");
  analysisMod_TrackerMuon_Photon25->SetMetName(pubMet->GetOutputName());
  analysisMod_TrackerMuon_Photon25->SetSampleType(sampletype);


  //------------------------------------------------------------------------------------------------
  // making analysis chain
  //------------------------------------------------------------------------------------------------
  if (sampletype == 3 ||sampletype == 4 || sampletype == 5
      || string(dataset) == "s8-wjets-mg-id11" || string(dataset) == "s8-vqq-mg-id11"

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

  if (sampletype == 3 || sampletype == 4 || sampletype == 5 || sampletype == 1 ) {

    mergeLeptonsMod->Add(generateFakeableObjectsMod);
    generateFakeableObjectsMod->Add(analysisMod);
        
    mergeLeptonsMod->Add(generateFakeableObjectsMod_Global);
    generateFakeableObjectsMod_Global->Add(analysisMod_Global);

    mergeLeptonsMod->Add(generateFakeableObjectsMod_TrackerMuon);
    generateFakeableObjectsMod_TrackerMuon->Add(analysisMod_TrackerMuon);
        
  } else if (sampletype == 10) {
    mergeLeptonsMod->Add(hltmod_Jet30);
    hltmod_Jet30->Add(generateFakeableObjectsMod_Jet30);      
    generateFakeableObjectsMod_Jet30->Add(analysisMod_Jet30);
    hltmod_Jet30->Add(generateFakeableObjectsMod_Global_Jet30);
    generateFakeableObjectsMod_Global_Jet30->Add(analysisMod_Global_Jet30);
    hltmod_Jet30->Add(generateFakeableObjectsMod_TrackerMuon_Jet30);
    generateFakeableObjectsMod_TrackerMuon_Jet30->Add(analysisMod_TrackerMuon_Jet30);

    mergeLeptonsMod->Add(hltmod_Jet50);
    hltmod_Jet50->Add(generateFakeableObjectsMod_Jet50);      
    generateFakeableObjectsMod_Jet50->Add(analysisMod_Jet50);
    hltmod_Jet50->Add(generateFakeableObjectsMod_Global_Jet50);
    generateFakeableObjectsMod_Global_Jet50->Add(analysisMod_Global_Jet50);
    hltmod_Jet50->Add(generateFakeableObjectsMod_TrackerMuon_Jet50);
    generateFakeableObjectsMod_TrackerMuon_Jet50->Add(analysisMod_TrackerMuon_Jet50);

    mergeLeptonsMod->Add(hltmod_Jet80);
     hltmod_Jet80->Add(generateFakeableObjectsMod_Jet80);      
    generateFakeableObjectsMod_Jet80->Add(analysisMod_Jet80);
    hltmod_Jet80->Add(generateFakeableObjectsMod_Global_Jet80);
    generateFakeableObjectsMod_Global_Jet80->Add(analysisMod_Global_Jet80);
    hltmod_Jet80->Add(generateFakeableObjectsMod_TrackerMuon_Jet80);
    generateFakeableObjectsMod_TrackerMuon_Jet80->Add(analysisMod_TrackerMuon_Jet80);

    mergeLeptonsMod->Add(hltmod_Jet110);    
    hltmod_Jet110->Add(generateFakeableObjectsMod_Jet110); 
    generateFakeableObjectsMod_Jet110->Add(analysisMod_Jet110);
    hltmod_Jet110->Add(generateFakeableObjectsMod_Global_Jet110);
    generateFakeableObjectsMod_Global_Jet110->Add(analysisMod_Global_Jet110);
    hltmod_Jet110->Add(generateFakeableObjectsMod_TrackerMuon_Jet110);
    generateFakeableObjectsMod_TrackerMuon_Jet110->Add(analysisMod_TrackerMuon_Jet110);
  } else if (sampletype == 20) {
    //For photon trigger samples
    mergeLeptonsMod->Add(hltmod_Photon15);
    hltmod_Photon15->Add(generateFakeableObjectsMod);
    generateFakeableObjectsMod->Add(analysisMod_Photon15);
    hltmod_Photon15->Add(generateFakeableObjectsMod_Global);
    generateFakeableObjectsMod_Global->Add(analysisMod_Global_Photon15);
    hltmod_Photon15->Add(generateFakeableObjectsMod_TrackerMuon);
    generateFakeableObjectsMod_TrackerMuon->Add(analysisMod_TrackerMuon_Photon15);

//     mergeLeptonsMod->Add(hltmod_Photon25);
//     hltmod_Photon25->Add(generateFakeableObjectsMod);
//     generateFakeableObjectsMod->Add(analysisMod_Photon25);
//     hltmod_Photon25->Add(generateFakeableObjectsMod_Global);
//     generateFakeableObjectsMod_Global->Add(analysisMod_Global_Photon25);
//     hltmod_Photon25->Add(generateFakeableObjectsMod_TrackerMuon);
//     generateFakeableObjectsMod_TrackerMuon->Add(analysisMod_TrackerMuon_Photon25);

  }


  //------------------------------------------------------------------------------------------------
  // setup analysis
  //------------------------------------------------------------------------------------------------
  Analysis *ana = new Analysis;
  ana->SetUseHLT(kTRUE);
  ana->SetKeepHierarchy(kTRUE);
  if (nEvents >= 0)
    ana->SetProcessNEvents(nEvents);
  ana->SetSuperModule(generatorMod);

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

  if (sampletype == 10) {
    cout << "HLTMod Jet30: Accepted " << hltmod_Jet30->GetNAccepted() 
         << " of " << hltmod_Jet30->GetNEvents() << " events" << endl;
    cout << "HLTMod Jet50: Accepted " << hltmod_Jet50->GetNAccepted() 
         << " of " << hltmod_Jet50->GetNEvents() << " events" << endl;
    cout << "HLTMod Jet80: Accepted " << hltmod_Jet80->GetNAccepted() 
         << " of " << hltmod_Jet80->GetNEvents() << " events" << endl;
    cout << "HLTMod Jet110: Accepted " << hltmod_Jet110->GetNAccepted() 
         << " of " << hltmod_Jet110->GetNEvents() << " events" << endl;
  }

  if (sampletype == 20) {
    cout << "HLTMod Photon15: Accepted " << hltmod_Photon15->GetNAccepted() 
         << " of " << hltmod_Photon15->GetNEvents() << " events" << endl;
    cout << "HLTMod Photon25: Accepted " << hltmod_Photon25->GetNAccepted() 
         << " of " << hltmod_Photon25->GetNEvents() << " events" << endl;
  }

  return;
}

//--------------------------------------------------------------------------------------------------
void runComputeMuonFakeRate(const char *fileset      = "",
                            const char *skim         = "noskim",
                            const char *dataset      = "s8-ttbar-id9",
                            const char *book         = "mit/filler/006",
                            const char *catalogDir   = "/home/mitprod/catalog",
                            const char *outputName   = "WWSelection",
                            int         nEvents      = -1, 
                            int         runTypeIndex = -1)
{

  TString outfileName = TString(outputName);

  executeComputeMuonFakeRateMod(fileset,skim,dataset,book,catalogDir,outfileName,runTypeIndex,nEvents);

}

