// $Id: runWWSelection.C,v 1.3 2009/03/06 16:50:06 phedex Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <exception>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitPhysics/Mods/interface/GeneratorMod.h"
#include "MitAna/TreeMod/interface/HLTMod.h"
#include "MitPhysics/Mods/interface/MuonIDMod.h"
#include "MitPhysics/Mods/interface/ElectronIDMod.h"
#include "MitPhysics/Mods/interface/JetCleaningMod.h"
#include "MitPhysics/Mods/interface/ElectronCleaningMod.h"
#include "MitPhysics/Mods/interface/JetIDMod.h"
#include "MitHiggs/DiBosonMods/interface/WWEvtSelMod.h"
#endif


//--------------------------------------------------------------------------------------------------
void WWSelection(const char *fileset  = "",
                        const char *dataset    = "s8-ttbar-id9",
                        const char *book       = "mit/filler/004",
                        const char *catalogDir = "/home/mitprod/catalog",
                        const char *outputName = "WWSelection",
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
  GeneratorMod *GeneratorMod = new GeneratorMod;

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
  muonID->SetMuonIDType("Loose");
  muonID->SetMuonIsoType("TrackCaloSliding"); 
  ElectronIDMod       *electronID       = new ElectronIDMod;
  electronID->SetElectronIDType(TString("Tight"));
  electronID->SetElectronIsoType(TString("TrackJuraSliding"));
  JetIDMod            *JetID            = new JetIDMod;            
  JetID->SetJetBranchName(TString("SisCone5Jets"));                          //name of jet algorithm
  JetID->SetGoodJetsName(TString("GoodJets"));
  JetID->SetUseJetCorrection(true);
  JetID->SetJetEtCut(30.0);
  ElectronCleaningMod *electronCleaning = new ElectronCleaningMod;
  JetCleaningMod      *JetCleaning      = new JetCleaningMod;
  JetCleaning->SetGoodJetsName (TString("GoodJets"));
  JetCleaning->SetCleanJetsName(TString("CleanJets"));

  //------------------------------------------------------------------------------------------------
  // analyses modules
  //------------------------------------------------------------------------------------------------
  WWEvtSelMod *analysisMod = new WWEvtSelMod;
  analysisMod->SetLoadGenParticles(false);
  analysisMod->SetCleanJetsName(TString("CleanJets"));
  analysisMod->SetPrintDebug(false);

  //------------------------------------------------------------------------------------------------
  // making analysis chain
  //------------------------------------------------------------------------------------------------
  //GeneratorMod->Add(hltmod); hltmod->Add(muonID);
  GeneratorMod->Add(muonID);
  muonID->Add(electronID);
  electronID->Add(JetID);
  JetID->Add(electronCleaning);
  electronCleaning->Add(JetCleaning);
  JetCleaning->Add(analysisMod);
  //------------------------------------------------------------------------------------------------
  // setup analysis
  //------------------------------------------------------------------------------------------------
  Analysis *ana = new Analysis;
  ana->SetUseHLT(true);
  ana->SetKeepHierarchy(false);
  if (nEvents >= 0)
    ana->SetProcessNEvents(nEvents);
  ana->SetSuperModule(GeneratorMod);

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
void runWWSelection(const char *fileset      = "",
                        const char *dataset      = "s8-ttbar-id9",
                        const char *book         = "mit/filler/004",
                        const char *catalogDir   = "/home/mitprod/catalog",
                        const char *outputName   = "WWSelection",
                        int         nEvents      = -1, 
                        int         runTypeIndex = -1)
{

  TString outfileName = TString(outputName);

  if (runTypeIndex == -1) {
    WWSelection(fileset,dataset,book,catalogDir,outfileName,nEvents);
  }
}
