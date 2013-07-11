//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runJetChargeCorrelationStudy.C+\(\"0000\",\"s8-wm-id9\",\"mit/filler/006\",\"/home/mitprod/catalog\",\"FakeTest\",10000,-1\)
//root -l -q -b  $CMSSW_BASE/src/MitHiggs/macros/runMacros/runJetChargeCorrelationStudy.C+\(\"0000\",\"s8-qcdem_30_80-id9\",\"mit/filler/006\",\"/home/mitprod/catalog\",\"JetChargeTest\",10000,-1\)
// $Id: runJetChargeCorrelationStudy.C,v 1.1 2009/03/06 16:50:05 phedex Exp $

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
#include "MitPhysics/Mods/interface/GenerateFakeableObjectsMod.h"
#include "MitPhysics/Mods/interface/GenerateFakesMod.h"
#include "MitPhysics/Mods/interface/MergeLeptonsMod.h"
#include "MitHiggs/FakeRateMods/interface/JetChargeCorrelationStudyMod.h"
#endif
//--------------------------------------------------------------------------------------------------
void executeComputeElectronFakeRateMod(const char *fileset  = "",
                        const char *dataset    = "s8-ttbar-id9",
                        const char *book       = "mit/filler/006",
                        const char *catalogDir = "/home/mitprod/catalog",
                        const char *outputName = "JetChargeCorrelationStudy",
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
  generatorMod->SetFillHist(kTRUE);

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
  //MergeLeptonsMod *mergeLeptonsMod = new MergeLeptonsMod;

  //------------------------------------------------------------------------------------------------
  // analyses modules
  //------------------------------------------------------------------------------------------------
  JetChargeCorrelationStudyMod *analysisMod = new JetChargeCorrelationStudyMod;
  analysisMod->SetVetoGeneratorLeptons(kTRUE);
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
  jetCleaning->Add(analysisMod);

  //------------------------------------------------------------------------------------------------
  // setup analysis
  //------------------------------------------------------------------------------------------------
  Analysis *ana = new Analysis;
  ana->SetUseHLT(kFALSE);
  ana->SetKeepHierarchy(kTRUE);
  if (nEvents >= 0)
    ana->SetProcessNEvents(nEvents);
  //ana->SetSuperModule(muonID);
  ana->SetSuperModule(generatorMod);

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
void runJetChargeCorrelationStudy(const char *fileset      = "",
                        const char *dataset      = "s8-ttbar-id9",
                        const char *book         = "mit/filler/006",
                        const char *catalogDir   = "/home/mitprod/catalog",
                        const char *outputName   = "JetChargeCorrelationStudy",
                        int         nEvents      = -1, 
                        int         runTypeIndex = -1)
{

  TString outfileName = TString(outputName);

  if (runTypeIndex == -1) {
    executeComputeElectronFakeRateMod(fileset,dataset,book,catalogDir,outfileName,nEvents);
  }
}

