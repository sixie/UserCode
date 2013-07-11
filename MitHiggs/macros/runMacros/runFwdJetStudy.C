// $Id: runFwdJetStudy.C,v 1.2 2009/03/06 16:50:04 phedex Exp $

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
#include "MitPhysics/SelMods/interface/VBFHwwEvtSelMod.h"
#endif


//--------------------------------------------------------------------------------------------------
void FwdJetStudy(const char *fileset  = "",
                 const char *dataset    = "s8-ttbar-id9",
                 const char *book       = "mit/filler/004",
                 const char *catalogDir = "/home/mitprod/catalog",
                 const char *outputName = "VBFJetTagStudy",
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
  GeneratorMod->SetFillHist(true);
  GeneratorMod->SetIsMC    (true);

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
  muonID->SetTrackIsolationCut(3.0);
  muonID->SetCaloIsolationCut (3.0);
  ElectronIDMod       *electronID       = new ElectronIDMod;
  electronID->SetElectronIsoType("TrackCalo");
  electronID->SetTrackIsolationCut(5.0);
  electronID->SetCaloIsolationCut (5.0);
  JetIDMod            *JetID            = new JetIDMod;            
  JetID->SetJetName(TString("CaloJets"));                          //name of jet algorithm
  JetID->SetGoodJetsName(TString("GoodJets"));
  JetID->SetUseJetCorrection(true);
  JetID->SetJetEtCut(30.0);
  ElectronCleaningMod *electronCleaning = new ElectronCleaningMod;
  JetCleaningMod      *JetCleaning      = new JetCleaningMod;
  JetCleaning->SetGoodJetsName (TString("GoodJets"));
  JetCleaning->SetCleanJetsName(TString("CleanJets"));

  //------------------------------------------------------------------------------------------------
  //  Fwd Jet Tagging Study
  //------------------------------------------------------------------------------------------------
  VBFJetTagStudyMod *VBFTagMod = new VBFJetTagStudyMod;
  VBFTagMod->SetCleanJetsName(TString("CleanJets"));
  VBFTagMod->SetFillHist(true);

  //------------------------------------------------------------------------------------------------
  // making analysis chain
  //------------------------------------------------------------------------------------------------
  //GeneratorMod->Add(hltmod); hltmod->Add(muonID);
  GeneratorMod->Add(muonID);
  muonID->Add(electronID);
  electronID->Add(JetID);
  JetID->Add(electronCleaning);
  electronCleaning->Add(JetCleaning);
  JetCleaning->Add(VBFTagMod);
  //------------------------------------------------------------------------------------------------
  // setup analysis
  //------------------------------------------------------------------------------------------------
  Analysis *ana = new Analysis;
  ana->SetUseHLT(true);
  ana->SetKeepHierarchy(false);
  if (nEvents >= 0)
    ana->SetProcessNevents(nEvents);
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
void runFwdJetStudy(const char *fileset      = "",
                        const char *dataset      = "s8-ttbar-id9",
                        const char *book         = "mit/filler/004",
                        const char *catalogDir   = "/home/mitprod/catalog",
                        const char *outputName   = "VBFHWWSelection",
                        int         nEvents      = -1, 
                        int         runTypeIndex = -1)
{

  TString outfileName = TString(outputName);

  if (runTypeIndex == -1) {
    FwdJetStudy(fileset,dataset,book,catalogDir,outfileName,nEvents);
  }
}
