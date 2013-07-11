// $Id: runHWWHLTAnalysis.C,v 1.1 2008/11/23 15:59:11 sixie Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitAna/TreeMod/interface/ObjectValidationMod.h"
#include "MitHiggs/HwwMods/interface/HwwEvtSelMod.h"
#endif

//--------------------------------------------------------------------------------------------------
void runHWWHLTAnalysis()
{
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 1;

  TString cutType = TString("N-1");
  string JetName = "CaloJets";
  //string JetName = "IC5JetPlusTrack";
  string datasetname;
  // HWW signal
  // IC5
//   TString files1      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateH165WW_2l-id9/mitPrivateH165WW_2l-id9_*.root");
//   datasetname = "HWW";
//   //runHWWAnalysisChain(files1, TString(("mit-HWWSelection-004-hist_" + datasetname + "_" + JetName+".root").c_str()), cutType, TString(JetName.c_str()));

//   TString files2      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateTTbar_dilepton-id9/mitPrivateTTbar_dilepton-id9_*.root");
//   datasetname = "TTbar";
//   //runHWWAnalysisChain(files2, TString(("mit-HWWSelection-004-hist_" + datasetname + "_" + JetName+".root").c_str()), cutType, TString(JetName.c_str()));

//   TString files3      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateWW_2l-id9/*.root");
//   datasetname = "WW";
//   //runHWWAnalysisChain(files3, TString(("mit-HWWSelection-004-hist_" + datasetname + "_" + JetName+".root").c_str()), cutType, TString(JetName.c_str()));


//   TString files4      = TString("/server/04d/sixie/OAKSamples/003/mitPrivateWZ_3l-id9/*.root");
//   datasetname = "WZ";
//   //runHWWAnalysisChain(files4, TString(("mit-HWWSelection-004-hist_" + datasetname + "_" + JetName+".root").c_str()), cutType, TString(JetName.c_str()));

//   TString files5      = TString("/server/04d/sixie/OAKSamples/003/mitPrivateZZ_4l-id9/*.root");
//   datasetname = "ZZ";
//   //runHWWAnalysisChain(files5, TString(("mit-HWWSelection-004-hist_" + datasetname + "_" + JetName+".root").c_str()), cutType, TString(JetName.c_str()));

//   TString files6      = TString("castor:castor/cern.ch/user/p/paus/filler/004/s8-zee-id9/*.root");
//   datasetname = "Z";
//   //runHWWAnalysisChain(files6, TString(("mit-HWWSelection-004-hist_" + datasetname + "_" + JetName+".root").c_str()), cutType, TString(JetName.c_str()));


  //Test HLT Triggers
  TString files1      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateH165WW_2l-id9/mitPrivateH165WW_2l-id9_*.root");
  datasetname = "HWW";
  runHLTAnalysis(files1, TString(("mit-HLTAnalysis-004-hist_" + datasetname + ".root").c_str()));

//Test HLT Triggers
  TString files2      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateWW_2l-id9/*.root");
  datasetname = "WW";
  //runHLTAnalysis(files2, TString(("mit-HLTAnalysis-004-hist_" + datasetname + ".root").c_str()));

  TString files3      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateTTbar_dilepton-id9/mitPrivateTTbar_dilepton-id9_*.root");
  datasetname = "TTBAR";
  //runHLTAnalysis(files3, TString(("mit-HLTAnalysis-004-hist_" + datasetname + ".root").c_str()));

  TString files4      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateWZ_3l-id9/*.root");
  datasetname = "WZ";
  //runHLTAnalysis(files4, TString(("mit-HLTAnalysis-004-hist_" + datasetname + ".root").c_str()));

  TString files5      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateZZ_4l-id9/*.root");
  datasetname = "ZZ";
  //runHLTAnalysis(files5, TString(("mit-HLTAnalysis-004-hist_" + datasetname + ".root").c_str()));

  TString files6      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-zee-id9/*.root");
  datasetname = "Zee";
  //runHLTAnalysis(files6, TString(("mit-HLTAnalysis-004-hist_" + datasetname + ".root").c_str()));

  return;
}

void runHLTAnalysis(TString inputfiles, TString outputfileName) {

  //Trigger
  HwwHLTMod *hltmod = new HwwHLTMod;
  //HLTMod *hltmod = new HLTMod;
  hltmod->SetPrintTable(kFALSE);
  hltmod->AddTrigger("HLT_IsoEle15_L1I");
  hltmod->AddTrigger("HLT_Ele15_SW_L1R");
  hltmod->AddTrigger("HLT_IsoEle15_LW_L1I");
  hltmod->AddTrigger("HLT_IsoEle18_L1R");
  hltmod->AddTrigger("HLT_Ele15_LW_L1R");
  hltmod->AddTrigger("HLT_LooseIsoEle15_LW_L1R");
  hltmod->AddTrigger("HLT_IsoMu15");
  hltmod->AddTrigger("HLT_Mu15");

//   hltmod->AddTrigger("HLT_Mu3");
//   hltmod->AddTrigger("HLT_Ele10_SW_L1R");
  hltmod->SetTrigObjsName("myhltobjs");
  hltmod->SetAbortIfNotAccepted(kFALSE);

  //Cleaning Modules
  MuonIDMod *muonID = new MuonIDMod;  
  ElectronIDMod *electronID = new ElectronIDMod;
  ElectronCleaningMod *electronCleaning = new ElectronCleaningMod;

  //PreSelection
  HwwEvtPreSelMod *preSelectionMod = new HwwEvtPreSelMod;
  preSelectionMod->SetPreSelectionType(TString("Tight"));

  HwwHLTStudyMod *hltstudyMod = new HwwHLTStudyMod;
  hltstudyMod->SetPreSelectionType(TString("Tight"));
  hltstudyMod->SetDoHLTStudy(true);
   
  //chain together
   muonID->Add(electronID);
   electronID->Add(electronCleaning);
   electronCleaning->Add(preSelectionMod);
   preSelectionMod->Add(hltmod);
   hltmod->Add(hltstudyMod);

   // set up analysis
   Analysis *ana = new Analysis;  
   //ana->SetKeepHierarchy(kFALSE);  
   ana->SetSuperModule(muonID);  
   ana->AddFile(inputfiles);
   ana->SetOutputName(outputfileName.Data());
   ana->SetProcessNevents(50000);
   // run the analysis after successful initialisation
   ana->Run(! gROOT->IsBatch());  
   
   cout << "HLTMod: Accepted " << hltmod->GetNAccepted() 
        << " of " << hltmod->GetNEvents() << " events" << endl;
   
   return;

}




void runHWWAnalysisChain(TString inputfiles, TString outputfileName, TString PlotType, TString JetsName) {

  //Trigger
  HLTMod *hltmod = new HLTMod;
  hltmod->SetPrintTable(kFALSE);
//   hltmod->AddTrigger("HLT_IsoMu15");
//   hltmod->AddTrigger("HLT_IsoEle15_L1I");
  hltmod->AddTrigger("HLT_Mu5");
  hltmod->AddTrigger("HLT_Ele10_SW_L1R");
  hltmod->SetTrigObjsName("myhltobjs");

  //PreSelection
  HwwEvtPreSelMod *preSelectionMod = new HwwEvtPreSelMod;

  //Object ID and Cleaning Sequence;
  MuonIDMod *muonID = new MuonIDMod;  
  ElectronIDMod *electronID = new ElectronIDMod;
  JetIDMod *JetID = new JetIDMod; //default names are IC5
  JetID->SetJetName(JetsName); //for SC5 I have to set the names
  JetID->SetGoodJetsName(TString("GoodJets"));
  ElectronCleaningMod *electronCleaning = new ElectronCleaningMod;
  JetCleaningMod *JetCleaning = new JetCleaningMod; //default names are ok for IC5
  JetCleaning->SetGoodJetsName(TString("GoodJets"));
  JetCleaning->SetCleanJetsName(TString("CleanJets"));

  //Event Selection modules
  HwwEvtSelMod *eventSelectionMod = new HwwEvtSelMod;
  eventSelectionMod->SetLoadGenParticles(false);
  eventSelectionMod->SetCleanJetsName(TString("CleanJets"));
  eventSelectionMod->SetPlotType(PlotType);
  eventSelectionMod->SetPrintDebug(false);

  //chain modules together
  hltmod->Add(preSelectionMod);
  preSelectionMod->Add(muonID);
  muonID->Add(electronID);
  electronID->Add(JetID);  
  JetID->Add(electronCleaning);
  electronCleaning->Add(JetCleaning);
  JetCleaning->Add(eventSelectionMod);

  // set up analysis
  Analysis *ana = new Analysis;
  //ana->SetUseHLT(false);
  ana->SetKeepHierarchy(kFALSE);
  //ana->SetSuperModule(preSelectionMod);  
  ana->SetSuperModule(hltmod);  
  ana->AddFile(inputfiles);
  ana->SetOutputName(outputfileName.Data());
  // run the analysis after successful initialisation
  ana->Run(! gROOT->IsBatch());  

  cout << "HLTMod: Accepted " << hltmod->GetNAccepted() 
       << " of " << hltmod->GetNEvents() << " events" << endl;

  return;

}
