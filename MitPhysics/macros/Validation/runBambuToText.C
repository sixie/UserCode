// $Id: runBambuToText.C,v 1.1 2008/11/29 18:51:07 sixie Exp $

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
#include "MitPhysics/Validation/interface/BambuToTextMod.h"
#endif


//--------------------------------------------------------------------------------------------------
void BambuToText(const char *fileset  = "",
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
  GeneratorMod->SetFillHist(false);

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
  MuonIDMod           *muonID         = new MuonIDMod;  
  muonID->SetMuonIDType("Loose");
  muonID->SetMuonIsoType("TrackCaloSliding"); 
  muonID->SetMuonPtMin(10.0);
  ElectronIDMod       *electronID     = new ElectronIDMod;
  electronID->SetElectronIDType(TString("Tight"));
  electronID->SetElectronIsoType(TString("TrackJuraSliding"));
  electronID->SetElectronPtMin(10.0);
  PhotonIDMod         *photonID       = new PhotonIDMod;
  photonID->SetPhotonIDType(TString("Tight"));
  photonID->SetPhotonPtMin(20.0);
  JetIDMod            *JetID          = new JetIDMod;            
  JetID->SetJetBranchName(TString("SisCone5Jets"));                      //name of jet algorithm
  JetID->SetGoodJetsName(TString("GoodJets"));
  JetID->SetUseJetCorrection(true);
  JetID->SetJetEtCut(30.0);
  ElectronCleaningMod    *electronCleaning = new ElectronCleaningMod;
  PhotonCleaningMod      *photonCleaning   = new PhotonCleaningMod;
  JetCleaningMod         *jetCleaning      = new JetCleaningMod;
  jetCleaning->SetGoodJetsName (TString("GoodJets"));
  jetCleaning->SetCleanJetsName(TString("CleanJets"));

  //------------------------------------------------------------------------------------------------
  // analyses modules
  //------------------------------------------------------------------------------------------------
  BambuToTextMod *analysisMod = new BambuToTextMod;
  analysisMod->SetCleanMuonName(TString("CleanMuons"));
  analysisMod->SetCleanElectronName(TString("CleanElectrons"));
  analysisMod->SetCleanPhotonName(TString("CleanPhotons"));
  analysisMod->SetCleanJetName(TString("CleanJets"));
  analysisMod->SetProcessName(TString(dataset));
  //met name is default

  //Define text output file
  TString textFile = TString(outputName);  
  textFile += TString("_") + TString(dataset);
  if (TString(fileset) != TString(""))
    textFile += TString("_") + TString(fileset);
  textFile += TString(".txt");
  printf("\nTextFile output: %s\n\n",textFile.Data());  
  analysisMod->SetOutputFilename(textFile);


  //------------------------------------------------------------------------------------------------
  // making analysis chain
  //------------------------------------------------------------------------------------------------
  //GeneratorMod->Add(hltmod); hltmod->Add(muonID);
  GeneratorMod->Add(muonID);
  muonID->Add(electronID);
  electronID->Add(photonID);
  photonID->Add(JetID);
  JetID->Add(electronCleaning);
  electronCleaning->Add(photonCleaning);
  photonCleaning->Add(jetCleaning);
  jetCleaning->Add(analysisMod);
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
void runBambuToText(const char *fileset      = "",
                        const char *dataset      = "s8-ttbar-id9",
                        const char *book         = "mit/filler/004",
                        const char *catalogDir   = "/home/mitprod/catalog",
                        const char *outputName   = "WWSelection",
                        int         nEvents      = -1, 
                        int         runTypeIndex = -1)
{
  TString outfileName = TString(outputName);

  if (runTypeIndex == -1) {
    BambuToText(fileset,dataset,book,catalogDir,outfileName,nEvents);
  }
}
