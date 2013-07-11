// $Id: runMuonIsolationStudy.C,v 1.2 2008/11/27 14:31:39 sixie Exp $

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
void MuonIsolationStudy(const char *fileset    = "",
                        const char *dataset    = "s8-ttbar-id9",
                        const char *book       = "mit/filler/004",
                        const char *catalogDir = "/home/mitprod/catalog",
                        const char *outputName = "VBFHWWSelection",
                        int         nEvents    = -1,
                        double      ptmin      = 0.0,
                        double      ptmax      = 10000.0,
                        bool        loadGeneratorMod = false)
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
  MuonIDMod           *muonID           = new MuonIDMod;  
  muonID->SetTrackIsolationCut(3.0);
  muonID->SetCaloIsolationCut (3.0);
  
  //------------------------------------------------------------------------------------------------
  // analyses modules
  //------------------------------------------------------------------------------------------------

  MuonIsolationStudyMod *studyMod = new MuonIsolationStudyMod;
  studyMod->SetMatchToRealMuons(loadGeneratorMod);
  studyMod->SetImposeMuonID(false);
  studyMod->SetMuonPtMin(ptmin);
  studyMod->SetMuonPtMax(ptmax);

  //------------------------------------------------------------------------------------------------
  // making analysis chain
  //------------------------------------------------------------------------------------------------
  //GeneratorMod->Add(hltmod); hltmod->Add(muonID);
  GeneratorMod->Add(studyMod);

  //------------------------------------------------------------------------------------------------
  // setup analysis
  //------------------------------------------------------------------------------------------------
  Analysis *ana = new Analysis;
  ana->SetUseHLT(true);
  ana->SetKeepHierarchy(false);
  if (nEvents >= 0)
    ana->SetProcessNevents(nEvents);
  if (loadGeneratorMod) {
    ana->SetSuperModule(GeneratorMod);
    cerr << "Load Generator Mod" << endl;
  }
  else 
    ana->SetSuperModule(studyMod);

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
void runMuonIsolationStudy(const char *fileset      = "",
                        const char *dataset      = "s8-ttbar-id9",
                        const char *book         = "mit/filler/004",
                        const char *catalogDir   = "/home/mitprod/catalog",
                        const char *outputName   = "VBFHWWSelection",
                        int         nEvents      = -1, 
                        int         runTypeIndex = -1)
{
 
  double ptmin;
  double ptmax;
  TString outfileName = TString(outputName);
  bool loadGeneratorMod = (runTypeIndex < 0);

  if (runTypeIndex == 1 || runTypeIndex == -1) {
    ptmin = 0.0;
    ptmax = 10000.0;
    outfileName += TString("_FullPtRange");
    MuonIsolationStudy(fileset,dataset,book,catalogDir,outfileName,nEvents,ptmin,ptmax,loadGeneratorMod);
  } else if (runTypeIndex == 2 || runTypeIndex == -2) {
    ptmin = 20.0;
    ptmax = 40.0;
    outfileName += TString("_PtRange20To40");
    MuonIsolationStudy(fileset,dataset,book,catalogDir,outfileName,nEvents,ptmin,ptmax,loadGeneratorMod);
  } else if (runTypeIndex == 11 || runTypeIndex == -11) {
    ptmin = 15.0;
    ptmax = 10000.0;
    outfileName += TString("_PtRange15ToInf");
    MuonIsolationStudy(fileset,dataset,book,catalogDir,outfileName,nEvents,ptmin,ptmax,loadGeneratorMod);
  } else if (runTypeIndex == 12 || runTypeIndex == -12) {
    ptmin = 20.0;
    ptmax = 10000.0;
    outfileName += TString("_PtRange20ToInf");
    MuonIsolationStudy(fileset,dataset,book,catalogDir,outfileName,nEvents,ptmin,ptmax,loadGeneratorMod);
  } else if (runTypeIndex == 13 || runTypeIndex == -13) {
    ptmin = 25.0;
    ptmax = 10000.0;
    outfileName += TString("_PtRange25ToInf");
    MuonIsolationStudy(fileset,dataset,book,catalogDir,outfileName,nEvents,ptmin,ptmax,loadGeneratorMod);
  } else if (runTypeIndex == 14 || runTypeIndex == -14) {
    ptmin = 30.0;
    ptmax = 10000.0;
    outfileName += TString("_PtRange30ToInf");
    MuonIsolationStudy(fileset,dataset,book,catalogDir,outfileName,nEvents,ptmin,ptmax,loadGeneratorMod);
  } else if (runTypeIndex == 15 || runTypeIndex == -15) {
    ptmin = 35.0;
    ptmax = 10000.0;
    outfileName += TString("_PtRange35ToInf");
    MuonIsolationStudy(fileset,dataset,book,catalogDir,outfileName,nEvents,ptmin,ptmax,loadGeneratorMod);
  } else if (runTypeIndex == 16 || runTypeIndex == -16) {
    ptmin = 40.0;
    ptmax = 10000.0;
    outfileName += TString("_PtRange40ToInf");
    MuonIsolationStudy(fileset,dataset,book,catalogDir,outfileName,nEvents,ptmin,ptmax,loadGeneratorMod);
  } else if (runTypeIndex == 17 || runTypeIndex == -17) {
    ptmin = 45.0;
    ptmax = 10000.0;
    outfileName += TString("_PtRange45ToInf");
    MuonIsolationStudy(fileset,dataset,book,catalogDir,outfileName,nEvents,ptmin,ptmax,loadGeneratorMod);             
  } else if (runTypeIndex == 18 || runTypeIndex == -18) {
    ptmin = 50.0;
    ptmax = 10000.0;
    outfileName += TString("_PtRange50ToInf");
    MuonIsolationStudy(fileset,dataset,book,catalogDir,outfileName,nEvents,ptmin,ptmax,loadGeneratorMod);
  }             
}
