//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeElectronFakeRate.C+\(\"0000\",\"noskim\",\"s8-we-id11\",\"mit/filler/008\",\"/home/mitprod/catalog\",\"ComputeElectronFakeRateTest\",10000,1\)
//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeElectronFakeRate.C+\(\"0000\",\"noskim\",\"s8-wjets-mg-id11\",\"mit/filler/008\",\"/home/mitprod/catalog\",\"ComputeElectronFakeRateTest\",10000,3\)
//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeElectronFakeRate.C+\(\"0000\",\"noskim\",\"s8-vqq-mg-id11\",\"mit/filler/008\",\"/home/mitprod/catalog\",\"ComputeElectronFakeRateTest\",10000,4\)
//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeElectronFakeRate.C+\(\"0000\",\"noskim\",\"f8-wc-mg-id11\",\"mit/filler/008\",\"/home/mitprod/catalog\",\"ComputeElectronFakeRateTest\",10000,5\)
//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeElectronFakeRate.C+\(\"0000\",\"noskim\",\"s8-qcdbce_20_30-id11\",\"mit/filler/008\",\"/home/mitprod/catalog\",\"ComputeElectronFakeRateTest\",10000,10\)
//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeElectronFakeRate.C+\(\"0000\",\"noskim\",\"f8-qcd100_250-id11\",\"mit/filler/009\",\"/home/mitprod/catalog\",\"ComputeElectronFakeRateTest\",10000,10\)
//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeElectronFakeRate.C+\(\"0000\",\"noskim\",\"s8-wjets-mg-id11\",\"mit/filler/008\",\"/home/mitprod/catalog\",\"ComputeElectronFakeRateTest\",10000,10\)
//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeElectronFakeRate.C+\(\"0000\",\"noskim\",\"s8-zjets-mg-id11\",\"mit/filler/008\",\"/home/mitprod/catalog\",\"ComputeElectronFakeRateTest\",10000,10\)


//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeElectronFakeRate.C+\(\"0005\",\"noskim\",\"s8-pj15-id11\",\"mit/filler/009\",\"/home/mitprod/catalog\",\"ComputeElectronFakeRateTest\",10000,20\)
//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeElectronFakeRate.C+\(\"0005\",\"noskim\",\"f8-qcd100_250-id11\",\"mit/filler/009\",\"/home/mitprod/catalog\",\"ComputeElectronFakeRateTest\",10000,20\)
//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runComputeElectronFakeRate.C+\(\"0000\",\"noskim\",\"s8-zjets-mg-id11\",\"mit/filler/008\",\"/home/mitprod/catalog\",\"ComputeElectronFakeRateTest\",10000,20\)


// $Id: runComputeElectronFakeRate.C,v 1.3 2009/06/30 08:11:15 phedex Exp $

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
#include "MitHiggs/FakeRateMods/interface/ComputeElectronFakeRateMod.h"
#endif
//--------------------------------------------------------------------------------------------------
void executeComputeElectronFakeRateMod(const char *fileset  = "",
                                       const char *skim         = "noskim",
                                       const char *dataset    = "s8-ttbar-id9",
                                       const char *book       = "mit/filler/006",
                                       const char *catalogDir = "/home/mitprod/catalog",
                                       const char *outputName = "ComputeElectronFakeRateMod",
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

  if (sampletype == 3 || string(dataset) == "s8-wjets-mg-id11" )
    mctype = "kMCTypeVLightJets";
  if (sampletype == 4 || string(dataset) == "s8-vqq-mg-id11")
    mctype = "kMCTypeVQQ";
  if (sampletype == 5 || string(dataset) == "f8-wc-mg-id11")
    mctype = "kMCTypeWc";

  //------------------------------------------------------------------------------------------------
  // generator information
  //------------------------------------------------------------------------------------------------
  GeneratorMod *generatorMod = new GeneratorMod;
  //generatorMod->SetFillHist(kTRUE);

  PartonFlavorHistoryMod *partonFlavorHistoryMod = new PartonFlavorHistoryMod;
  partonFlavorHistoryMod->SetMCSampleType(mctype.c_str());
  partonFlavorHistoryMod->SetApplyPartonFlavorFilter(kTRUE);

  //------------------------------------------------------------------------------------------------
  // HLT information
  //------------------------------------------------------------------------------------------------
//   HLTMod *hltmod = new HLTMod;
//   hltmod->SetPrintTable(kTRUE);
//   hltmod->AddTrigger("HLT_IsoEle15_L1I");
//   hltmod->AddTrigger("HLT_Ele15_SW_L1R");
//   hltmod->AddTrigger("HLT_IsoEle15_LW_L1I");
//   hltmod->AddTrigger("HLT_IsoEle18_L1R");
//   hltmod->AddTrigger("HLT_Ele15_LW_L1R");
//   hltmod->AddTrigger("HLT_LooseIsoEle15_LW_L1R");
//   hltmod->AddTrigger("HLT_IsoMu15");
//   hltmod->AddTrigger("HLT_Mu15");
//   hltmod->SetTrigObjsName("myhltobjs");

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

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Reco = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Reco->SetName("GenerateFakeableObjectsMod_Reco");
  generateFakeableObjectsMod_Reco->SetElectronFOType("Reco");
  generateFakeableObjectsMod_Reco->SetMuonFOType("Global");
  generateFakeableObjectsMod_Reco->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_Reco->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_Reco->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_Reco->SetElectronFakeableObjectsName("RecoElectronFakeableObjects");
  generateFakeableObjectsMod_Reco->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Jet30 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Jet30->SetName("GenerateFakeableObjectsMod_Jet30");
  generateFakeableObjectsMod_Jet30->SetElectronFOType("GsfPlusSC");
  generateFakeableObjectsMod_Jet30->SetMuonFOType("IsoTrack");
  generateFakeableObjectsMod_Jet30->SetTriggerName("HLTMod_Jet30");
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
  generateFakeableObjectsMod_Jet50->SetTriggerName("HLTMod_Jet50");
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
  generateFakeableObjectsMod_Jet80->SetTriggerName("HLTMod_Jet80");
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
  generateFakeableObjectsMod_Jet110->SetTriggerName("HLTMod_Jet110");
  generateFakeableObjectsMod_Jet110->SetTriggerObjectsName("myhltobjs_Jet110");
  generateFakeableObjectsMod_Jet110->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_Jet110->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_Jet110->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_Jet110->SetElectronFakeableObjectsName("GsfPlusSCElectronFakeableObjects_Jet110");
  generateFakeableObjectsMod_Jet110->SetMuonFakeableObjectsName("IsoTrackMuonFakeableObjects_Jet110");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Reco_Jet30 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Reco_Jet30->SetName("GenerateFakeableObjectsMod_Reco_Jet30");
  generateFakeableObjectsMod_Reco_Jet30->SetElectronFOType("Reco");
  generateFakeableObjectsMod_Reco_Jet30->SetMuonFOType("Global");
  generateFakeableObjectsMod_Reco_Jet30->SetTriggerName("HLTMod_Jet30");
  generateFakeableObjectsMod_Reco_Jet30->SetTriggerObjectsName("myhltobjs_Jet30");
  generateFakeableObjectsMod_Reco_Jet30->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_Reco_Jet30->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_Reco_Jet30->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_Reco_Jet30->SetElectronFakeableObjectsName("RecoElectronFakeableObjects_Jet30");
  generateFakeableObjectsMod_Reco_Jet30->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects_Jet30");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Reco_Jet50 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Reco_Jet50->SetName("GenerateFakeableObjectsMod_Reco_Jet50");
  generateFakeableObjectsMod_Reco_Jet50->SetElectronFOType("Reco");
  generateFakeableObjectsMod_Reco_Jet50->SetMuonFOType("Global");
  generateFakeableObjectsMod_Reco_Jet50->SetTriggerName("HLTMod_Jet50");
  generateFakeableObjectsMod_Reco_Jet50->SetTriggerObjectsName("myhltobjs_Jet50");
  generateFakeableObjectsMod_Reco_Jet50->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_Reco_Jet50->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_Reco_Jet50->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_Reco_Jet50->SetElectronFakeableObjectsName("RecoElectronFakeableObjects_Jet50");
  generateFakeableObjectsMod_Reco_Jet50->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects_Jet50");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Reco_Jet80 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Reco_Jet80->SetName("GenerateFakeableObjectsMod_Reco_Jet80");
  generateFakeableObjectsMod_Reco_Jet80->SetElectronFOType("Reco");
  generateFakeableObjectsMod_Reco_Jet80->SetMuonFOType("Global");
  generateFakeableObjectsMod_Reco_Jet80->SetTriggerName("HLTMod_Jet80");
  generateFakeableObjectsMod_Reco_Jet80->SetTriggerObjectsName("myhltobjs_Jet80");
  generateFakeableObjectsMod_Reco_Jet80->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_Reco_Jet80->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_Reco_Jet80->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_Reco_Jet80->SetElectronFakeableObjectsName("RecoElectronFakeableObjects_Jet80");
  generateFakeableObjectsMod_Reco_Jet80->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects_Jet80");

  GenerateFakeableObjectsMod *generateFakeableObjectsMod_Reco_Jet110 = new GenerateFakeableObjectsMod;
  generateFakeableObjectsMod_Reco_Jet110->SetName("GenerateFakeableObjectsMod_Reco_Jet110");
  generateFakeableObjectsMod_Reco_Jet110->SetElectronFOType("Reco");
  generateFakeableObjectsMod_Reco_Jet110->SetMuonFOType("Global");
  generateFakeableObjectsMod_Reco_Jet110->SetTriggerName("HLTMod_Jet110");
  generateFakeableObjectsMod_Reco_Jet110->SetTriggerObjectsName("myhltobjs_Jet110");
  generateFakeableObjectsMod_Reco_Jet110->SetVetoTriggerJet(VetoTriggerJet);
  generateFakeableObjectsMod_Reco_Jet110->SetVetoGenLeptons(VetoGenLeptons);
  generateFakeableObjectsMod_Reco_Jet110->SetVetoCleanLeptons(kFALSE);
  generateFakeableObjectsMod_Reco_Jet110->SetElectronFakeableObjectsName("RecoElectronFakeableObjects_Jet110");
  generateFakeableObjectsMod_Reco_Jet110->SetMuonFakeableObjectsName("GlobalMuonFakeableObjects_Jet110");


  //------------------------------------------------------------------------------------------------
  // analyses modules
  //------------------------------------------------------------------------------------------------
  ComputeElectronFakeRateMod *analysisMod = new ComputeElectronFakeRateMod;
  analysisMod->SetName("ComputeElectronFakeRateMod");
  analysisMod->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod->SetElectronFakeableObjectsName("GsfPlusSCElectronFakeableObjects");
  analysisMod->SetMetName(pubMet->GetOutputName());
  analysisMod->SetSampleType(sampletype);

  ComputeElectronFakeRateMod *analysisMod_Reco = new ComputeElectronFakeRateMod;
  analysisMod_Reco->SetName("ComputeElectronFakeRateMod_Reco");
  analysisMod_Reco->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Reco->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Reco->SetElectronFakeableObjectsName("RecoElectronFakeableObjects");
  analysisMod_Reco->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco->SetSampleType(sampletype);

  ComputeElectronFakeRateMod *analysisMod_Jet30 = new ComputeElectronFakeRateMod;
  analysisMod_Jet30->SetName("ComputeElectronFakeRateMod_Jet30");
  analysisMod_Jet30->SetTriggerName("HLT_Jet30");
  analysisMod_Jet30->SetTriggerObjectsName("myhltobjs_Jet30");
  analysisMod_Jet30->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Jet30->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Jet30->SetElectronFakeableObjectsName("GsfPlusSCElectronFakeableObjects_Jet30");
  analysisMod_Jet30->SetMetName(pubMet->GetOutputName());
  analysisMod_Jet30->SetSampleType(sampletype);

  ComputeElectronFakeRateMod *analysisMod_Jet50 = new ComputeElectronFakeRateMod;
  analysisMod_Jet50->SetName("ComputeElectronFakeRateMod_Jet50");
  analysisMod_Jet50->SetTriggerName("HLT_Jet50");
  analysisMod_Jet50->SetTriggerObjectsName("myhltobjs_Jet50");
  analysisMod_Jet50->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Jet50->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Jet50->SetElectronFakeableObjectsName("GsfPlusSCElectronFakeableObjects_Jet50");
  analysisMod_Jet50->SetMetName(pubMet->GetOutputName());
  analysisMod_Jet50->SetSampleType(sampletype);

  ComputeElectronFakeRateMod *analysisMod_Jet80 = new ComputeElectronFakeRateMod;
  analysisMod_Jet80->SetName("ComputeElectronFakeRateMod_Jet80");
  analysisMod_Jet80->SetTriggerName("HLT_Jet80");
  analysisMod_Jet80->SetTriggerObjectsName("myhltobjs_Jet80");
  analysisMod_Jet80->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Jet80->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Jet80->SetElectronFakeableObjectsName("GsfPlusSCElectronFakeableObjects_Jet80");
  analysisMod_Jet80->SetMetName(pubMet->GetOutputName());
  analysisMod_Jet80->SetSampleType(sampletype);

  ComputeElectronFakeRateMod *analysisMod_Jet110 = new ComputeElectronFakeRateMod;
  analysisMod_Jet110->SetName("ComputeElectronFakeRateMod_Jet110");
  analysisMod_Jet110->SetTriggerName("HLT_Jet110");
  analysisMod_Jet110->SetTriggerObjectsName("myhltobjs_Jet110");
  analysisMod_Jet110->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Jet110->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Jet110->SetElectronFakeableObjectsName("GsfPlusSCElectronFakeableObjects_Jet110");
  analysisMod_Jet110->SetMetName(pubMet->GetOutputName());
  analysisMod_Jet110->SetSampleType(sampletype);

  ComputeElectronFakeRateMod *analysisMod_Reco_Jet30 = new ComputeElectronFakeRateMod;
  analysisMod_Reco_Jet30->SetName("ComputeElectronFakeRateMod_Reco_Jet30");
  analysisMod_Reco_Jet30->SetTriggerName("HLT_Jet30");
  analysisMod_Reco_Jet30->SetTriggerObjectsName("myhltobjs_Jet30");
  analysisMod_Reco_Jet30->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Reco_Jet30->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Reco_Jet30->SetElectronFakeableObjectsName("RecoElectronFakeableObjects_Jet30");
  analysisMod_Reco_Jet30->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_Jet30->SetSampleType(sampletype);

  ComputeElectronFakeRateMod *analysisMod_Reco_Jet50 = new ComputeElectronFakeRateMod;
  analysisMod_Reco_Jet50->SetName("ComputeElectronFakeRateMod_Reco_Jet50");
  analysisMod_Reco_Jet50->SetTriggerName("HLT_Jet50");
  analysisMod_Reco_Jet50->SetTriggerObjectsName("myhltobjs_Jet50");
  analysisMod_Reco_Jet50->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Reco_Jet50->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Reco_Jet50->SetElectronFakeableObjectsName("RecoElectronFakeableObjects_Jet50");
  analysisMod_Reco_Jet50->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_Jet50->SetSampleType(sampletype);

  ComputeElectronFakeRateMod *analysisMod_Reco_Jet80 = new ComputeElectronFakeRateMod;
  analysisMod_Reco_Jet80->SetName("ComputeElectronFakeRateMod_Reco_Jet80");
  analysisMod_Reco_Jet80->SetTriggerName("HLT_Jet80");
  analysisMod_Reco_Jet80->SetTriggerObjectsName("myhltobjs_Jet80");
  analysisMod_Reco_Jet80->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Reco_Jet80->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Reco_Jet80->SetElectronFakeableObjectsName("RecoElectronFakeableObjects_Jet80");
  analysisMod_Reco_Jet80->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_Jet80->SetSampleType(sampletype);

  ComputeElectronFakeRateMod *analysisMod_Reco_Jet110 = new ComputeElectronFakeRateMod;
  analysisMod_Reco_Jet110->SetName("ComputeElectronFakeRateMod_Reco_Jet110");
  analysisMod_Reco_Jet110->SetTriggerName("HLT_Jet110");
  analysisMod_Reco_Jet110->SetTriggerObjectsName("myhltobjs_Jet110");
  analysisMod_Reco_Jet110->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Reco_Jet110->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Reco_Jet110->SetElectronFakeableObjectsName("RecoElectronFakeableObjects_Jet110");
  analysisMod_Reco_Jet110->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_Jet110->SetSampleType(sampletype);

  ComputeElectronFakeRateMod *analysisMod_Photon15 = new ComputeElectronFakeRateMod;
  analysisMod_Photon15->SetName("ComputeElectronFakeRateMod_Photon15");
  analysisMod_Photon15->SetTriggerName("HLT_Photon15_L1R");
  analysisMod_Photon15->SetTriggerObjectsName("myhltobjs_Photon15");
  analysisMod_Photon15->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Photon15->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Photon15->SetElectronFakeableObjectsName("GsfPlusSCElectronFakeableObjects");
  analysisMod_Photon15->SetMetName(pubMet->GetOutputName());
  analysisMod_Photon15->SetSampleType(sampletype);

  ComputeElectronFakeRateMod *analysisMod_Photon25 = new ComputeElectronFakeRateMod;
  analysisMod_Photon25->SetName("ComputeElectronFakeRateMod_Photon25");
  analysisMod_Photon25->SetTriggerName("HLT_Photon25_L1R");
  analysisMod_Photon25->SetTriggerObjectsName("myhltobjs_Photon25");
  analysisMod_Photon25->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Photon25->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Photon25->SetElectronFakeableObjectsName("GsfPlusSCElectronFakeableObjects");
  analysisMod_Photon25->SetMetName(pubMet->GetOutputName());
  analysisMod_Photon25->SetSampleType(sampletype);

  ComputeElectronFakeRateMod *analysisMod_Reco_Photon15 = new ComputeElectronFakeRateMod;
  analysisMod_Reco_Photon15->SetName("ComputeElectronFakeRateMod_Reco_Photon15");
  analysisMod_Reco_Photon15->SetTriggerName("HLT_Photon15_L1R");
  analysisMod_Reco_Photon15->SetTriggerObjectsName("myhltobjs_Photon15");
  analysisMod_Reco_Photon15->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Reco_Photon15->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Reco_Photon15->SetElectronFakeableObjectsName("RecoElectronFakeableObjects");
  analysisMod_Reco_Photon15->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_Photon15->SetSampleType(sampletype);

  ComputeElectronFakeRateMod *analysisMod_Reco_Photon25 = new ComputeElectronFakeRateMod;
  analysisMod_Reco_Photon25->SetName("ComputeElectronFakeRateMod_Reco_Photon25");
  analysisMod_Reco_Photon25->SetTriggerName("HLT_Photon25_L1R");
  analysisMod_Reco_Photon25->SetTriggerObjectsName("myhltobjs_Photon25");
  analysisMod_Reco_Photon25->SetVetoTriggerJet(VetoTriggerJet);
  analysisMod_Reco_Photon25->SetVetoGeneratorLeptons(VetoGenLeptons);
  analysisMod_Reco_Photon25->SetElectronFakeableObjectsName("RecoElectronFakeableObjects");
  analysisMod_Reco_Photon25->SetMetName(pubMet->GetOutputName());
  analysisMod_Reco_Photon25->SetSampleType(sampletype);

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

  //For W+Jet samples
  if (sampletype == 3 || sampletype == 4 || sampletype == 5 || sampletype == 1 ) {

    mergeLeptonsMod->Add(generateFakeableObjectsMod);
    generateFakeableObjectsMod->Add(analysisMod);
        
    mergeLeptonsMod->Add(generateFakeableObjectsMod_Reco);
    generateFakeableObjectsMod_Reco->Add(analysisMod_Reco);
        
  } else if (sampletype == 20) {
    //For photon trigger samples
    mergeLeptonsMod->Add(hltmod_Photon15);
    hltmod_Photon15->Add(generateFakeableObjectsMod);
    generateFakeableObjectsMod->Add(analysisMod_Photon15);
    hltmod_Photon15->Add(generateFakeableObjectsMod_Reco);
    generateFakeableObjectsMod_Reco->Add(analysisMod_Reco_Photon15);

//     mergeLeptonsMod->Add(hltmod_Photon25);
//     hltmod_Photon25->Add(generateFakeableObjectsMod);
//     generateFakeableObjectsMod->Add(analysisMod_Photon25);
//     hltmod_Photon25->Add(generateFakeableObjectsMod_Reco);
//     generateFakeableObjectsMod_Reco->Add(analysisMod_Reco_Photon25);

  } else if (sampletype == 10) {
    //For Jet Trigger samples
    mergeLeptonsMod->Add(hltmod_Jet30);
    hltmod_Jet30->Add(generateFakeableObjectsMod_Jet30);      
    generateFakeableObjectsMod_Jet30->Add(analysisMod_Jet30);
    hltmod_Jet30->Add(generateFakeableObjectsMod_Reco_Jet30);
    generateFakeableObjectsMod_Reco_Jet30->Add(analysisMod_Reco_Jet30);

    mergeLeptonsMod->Add(hltmod_Jet50);
    hltmod_Jet50->Add(generateFakeableObjectsMod_Jet50);      
    generateFakeableObjectsMod_Jet50->Add(analysisMod_Jet50);
    hltmod_Jet50->Add(generateFakeableObjectsMod_Reco_Jet50);
    generateFakeableObjectsMod_Reco_Jet50->Add(analysisMod_Reco_Jet50);

    mergeLeptonsMod->Add(hltmod_Jet80);
     hltmod_Jet80->Add(generateFakeableObjectsMod_Jet80);      
    generateFakeableObjectsMod_Jet80->Add(analysisMod_Jet80);
    hltmod_Jet80->Add(generateFakeableObjectsMod_Reco_Jet80);
    generateFakeableObjectsMod_Reco_Jet80->Add(analysisMod_Reco_Jet80);

    mergeLeptonsMod->Add(hltmod_Jet110);    
    hltmod_Jet110->Add(generateFakeableObjectsMod_Jet110); 
    generateFakeableObjectsMod_Jet110->Add(analysisMod_Jet110);
    hltmod_Jet110->Add(generateFakeableObjectsMod_Reco_Jet110);
    generateFakeableObjectsMod_Reco_Jet110->Add(analysisMod_Reco_Jet110);
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
  //ana->SetSkipFirstNEvents(47500);

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
void runComputeElectronFakeRate(const char *fileset      = "",
                                const char *skim         = "noskim",
                                const char *dataset      = "s8-ttbar-id9",
                                const char *book         = "mit/filler/006",
                                const char *catalogDir   = "/home/mitprod/catalog",
                                const char *outputName   = "WWSelection",
                                int         nEvents      = -1, 
                                int         runTypeIndex = -1)
{

  TString outfileName = TString(outputName);

  executeComputeElectronFakeRateMod(fileset,skim,dataset,book,catalogDir,outfileName,runTypeIndex,nEvents);

}

