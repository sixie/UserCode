//root -l -q -b $CMSSW_BASE/src/MitHiggs/macros/runMacros/runHWWSelection.C+\(\"0000\",\"s8-we-id9\",\"mit/filler/006\",\"/home/mitprod/catalog\",\"HWWTest\",100,-1\)

// $Id: runHWWSelection.C,v 1.2 2009/03/06 16:50:05 phedex Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <exception>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/Catalog/interface/Catalog.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitAna/TreeMod/interface/HLTMod.h"
#include "MitPhysics/Mods/interface/GeneratorMod.h"
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
#include "MitHiggs/HwwMods/interface/HwwEvtSelMod.h"
#endif

//--------------------------------------------------------------------------------------------------
void executeHWWSelection(const char *fileset  = "",
                                     const char *dataset    = "s8-ttbar-id9",
                                     const char *book       = "mit/filler/006",
                                     const char *catalogDir = "/home/mitprod/catalog",
                                     const char *outputName = "HWWSelectionMod",
                                     int         nEvents    = -1)
{
  //------------------------------------------------------------------------------------------------
  // some global setups
  //------------------------------------------------------------------------------------------------
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 3;

  //------------------------------------------------------------------------------------------------
  // generator information
  //------------------------------------------------------------------------------------------------
  GeneratorMod *generatorMod = new GeneratorMod;

  //------------------------------------------------------------------------------------------------
  // HLT information
  //------------------------------------------------------------------------------------------------
  HLTMod *hltmod = new HLTMod;
  hltmod->SetPrintTable(kTRUE);
  hltmod->AddTrigger("HLT_IsoEle15_L1I");
  hltmod->AddTrigger("HLT_Ele15_SW_L1R");
  hltmod->AddTrigger("HLT_IsoEle15_LW_L1I");
  hltmod->AddTrigger("HLT_IsoEle18_L1R");
  hltmod->AddTrigger("HLT_Ele15_LW_L1R");
  hltmod->AddTrigger("HLT_LooseIsoEle15_LW_L1R");
  hltmod->AddTrigger("HLT_IsoMu15");
  hltmod->AddTrigger("HLT_Mu15");
  hltmod->SetTrigObjsName("myhltobjs");

  //------------------------------------------------------------------------------------------------
  // object id and cleaning sequence
  //------------------------------------------------------------------------------------------------
  MuonIDMod           *muonID           = new MuonIDMod;  
  muonID->SetIDType("Loose");
  muonID->SetIsoType("TrackCaloSliding"); 
  ElectronIDMod       *electronID       = new ElectronIDMod;
  electronID->SetIDType(TString("Tight"));
  electronID->SetIsoType(TString("TrackJuraSliding"));
  PhotonIDMod         *photonID       = new PhotonIDMod;
  photonID->SetIDType(TString("Tight"));
  photonID->SetPtMin(20.0);
  JetIDMod            *jetID            = new JetIDMod;
  jetID->SetInputName(TString("SisCone5Jets"));
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
  // analyses modules
  //------------------------------------------------------------------------------------------------
  HwwEvtSelMod *analysisMod = new HwwEvtSelMod;

  //------------------------------------------------------------------------------------------------
  // making analysis chain
  //------------------------------------------------------------------------------------------------
  hltmod->Add(muonID);
  generatorMod->Add(muonID);
  muonID->Add(electronID);
  electronID->Add(photonID);
  photonID->Add(jetID);
  jetID->Add(electronCleaning);
  electronCleaning->Add(photonCleaning);
  photonCleaning->Add(jetCleaning);
  jetCleaning->Add(mergeLeptonsMod);
  mergeLeptonsMod->Add(analysisMod);

  //------------------------------------------------------------------------------------------------
  // setup analysis
  //------------------------------------------------------------------------------------------------
  Analysis *ana = new Analysis;
  ana->SetUseHLT(kFALSE);
  ana->SetKeepHierarchy(kTRUE);
  if (nEvents >= 0)
    ana->SetProcessNEvents(nEvents);
  ana->SetSuperModule(muonID);

  //------------------------------------------------------------------------------------------------
  // organize input
  //------------------------------------------------------------------------------------------------
  printf("\nRely on Catalog: %s\n",catalogDir);
  printf("  -> Book: %s  Dataset: %s  Fileset: %s <-\n\n",book,dataset,fileset);
  Catalog *c = new Catalog(catalogDir);
  Dataset *d = c->FindDataset(book,dataset,fileset);
  ana->AddDataset(d);

  //------------------------------------------------------------------------------------------------
  // organize output
  //------------------------------------------------------------------------------------------------
  TString rootFile = TString(outputName);
  rootFile += TString("_") + TString(dataset);
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
void runHWWSelection(const char *fileset      = "",
                        const char *dataset      = "s8-ttbar-id9",
                        const char *book         = "mit/filler/006",
                        const char *catalogDir   = "/home/mitprod/catalog",
                        const char *outputName   = "HWWSelectionMod",
                        int         nEvents      = -1, 
                        int         runTypeIndex = -1)
{

  TString outfileName = TString(outputName);

  if (runTypeIndex == -1 || runTypeIndex == 1) {
    executeHWWSelection(fileset,dataset,book,catalogDir,outfileName,nEvents);
  } else if (runTypeIndex == 2) {
    executeHWWSelection(fileset,dataset,book,catalogDir,outfileName,nEvents);
  }
}
