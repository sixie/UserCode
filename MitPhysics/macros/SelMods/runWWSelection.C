// $Id: runWWSelection.C,v 1.1 2008/11/12 15:04:52 sixie Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitAna/TreeMod/interface/ObjectValidationMod.h"
#include "MitHiggs/HwwMods/interface/WWEvtSelMod.h"
#endif

//--------------------------------------------------------------------------------------------------
void runWWSelection()
{
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 1;

  string cutType;
  string JetName = "CaloJets";
  //string JetName = "IC5JetPlusTrack";
  string datasetname;
  // HWW signal
  // IC5

//************************************************************************************************
// Sequential Cuts
//************************************************************************************************
  cutType = "Sequential";

  TString files1      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateH165WW_2l-id9/mitPrivateH165WW_2l-id9_*.root");
  datasetname = "HWW";
  runWWAnalysisChain(files1, TString(("mit-WWSelection-004-hist_" + datasetname + "_" + cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files2      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateTTbar_dilepton-id9/mitPrivateTTbar_dilepton-id9_*.root");
  datasetname = "TTBAR";
  runWWAnalysisChain(files2, TString(("mit-WWSelection-004-hist_" + datasetname + "_" + cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files3      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateWW_2l-id9/*.root");
  datasetname = "WW";
  runWWAnalysisChain(files3, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));


  TString files4      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateWZ_3l-id9/*.root");
  datasetname = "WZ";
  runWWAnalysisChain(files4, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files5      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateZZ_4l-id9/*.root");
  datasetname = "ZZ";
  runWWAnalysisChain(files5, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files6      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-zee-id9/*.root");
  datasetname = "Zee";
  //runWWAnalysisChain(files6, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files7      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-zmm-id9/*.root");
  datasetname = "Zmumu";
  //runWWAnalysisChain(files7, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files8      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-ztautau-id9/*.root");
  datasetname = "Ztautau";
  //runWWAnalysisChain(files8, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files9      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-we-id9/*.root");
  datasetname = "Wenu";
  //runWWAnalysisChain(files9, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files10      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-wm-id9/*.root");
  datasetname = "Wmunu";
  //runWWAnalysisChain(files10, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files11      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-wtau-id6/*.root");
  datasetname = "Wtaunu";
  //runWWAnalysisChain(files11, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));





//************************************************************************************************
// N-1 Cuts
//************************************************************************************************

  string cutType = "N-1";
  TString files101      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateH165WW_2l-id9/mitPrivateH165WW_2l-id9_*.root");
  datasetname = "HWW";
  //runWWAnalysisChain(files101, TString(("mit-WWSelection-004-hist_" + datasetname + "_" + cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files102      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateTTbar_dilepton-id9/mitPrivateTTbar_dilepton-id9_*.root");
  datasetname = "TTBAR";
  //runWWAnalysisChain(files102, TString(("mit-WWSelection-004-hist_" + datasetname + "_" + cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files103      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateWW_2l-id9/*.root");
  datasetname = "WW";
  //runWWAnalysisChain(files103, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));


  TString files104      = TString("/server/04d/sixie/OAKSamples/003/mitPrivateWZ_3l-id9/*.root");
  datasetname = "WZ";
  //runWWAnalysisChain(files104, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files105      = TString("/server/04d/sixie/OAKSamples/003/mitPrivateZZ_4l-id9/*.root");
  datasetname = "ZZ";
  //runWWAnalysisChain(files105, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files106      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-zee-id9/*.root");
  datasetname = "Zee";
  //runWWAnalysisChain(files106, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files107      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-zmm-id9/*.root");
  datasetname = "Zmumu";
  //runWWAnalysisChain(files107, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files108      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-ztautau-id9/*.root");
  datasetname = "Ztautau";
  //runWWAnalysisChain(files108, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files109      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-we-id9/*.root");
  datasetname = "Wenu";
  //runWWAnalysisChain(files109, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files110      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-wm-id9/*.root");
  datasetname = "Wmunu";
  //runWWAnalysisChain(files110, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));

  TString files111      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-wtau-id6/*.root");
  datasetname = "Wtaunu";
  //runWWAnalysisChain(files111, TString(("mit-WWSelection-004-hist_" + datasetname + "_" +  cutType + "_" + JetName+".root").c_str()), TString(cutType.c_str()), TString(JetName.c_str()));


  return;
}

void runHLTAnalysis(TString inputfiles, TString outputfileName) {

  //Trigger
  HLTMod *hltmod = new HLTMod;
  hltmod->SetPrintTable(kFALSE);
  hltmod->AddTrigger("HLT_IsoEle15_L1I");
  hltmod->AddTrigger("HLT_Ele15_SW_L1R");
  hltmod->AddTrigger("HLT_IsoEle15_LW_L1I");
  hltmod->AddTrigger("HLT_IsoEle18_L1R");
  hltmod->AddTrigger("HLT_Ele15_LW_L1R");
  hltmod->AddTrigger("HLT_LooseIsoEle15_LW_L1R");
  hltmod->AddTrigger("HLT_IsoMu15");
  hltmod->AddTrigger("HLT_Mu15");
  hltmod->SetTrigObjsName("myhltobjs");

  //PreSelection
  HwwEvtPreSelMod *preSelectionMod = new HwwEvtPreSelMod;

  //chain together
  preSelectionMod->Add(hltmod);

  // set up analysis
  Analysis *ana = new Analysis;  
  ana->SetKeepHierarchy(kFALSE);  
  ana->SetSuperModule(preSelectionMod);  
  ana->AddFile(inputfiles);
  ana->SetOutputName(outputfileName.Data());
  // run the analysis after successful initialisation
  ana->Run(! gROOT->IsBatch());  

  cout << "HLTMod: Accepted " << hltmod->GetNAccepted() 
       << " of " << hltmod->GetNEvents() << " events" << endl;

  return;

}




void runWWAnalysisChain(TString inputfiles, TString outputfileName, TString PlotType, TString JetsName) {

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
  WWEvtSelMod *eventSelectionMod = new WWEvtSelMod;
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
  ana->SetSuperModule(preSelectionMod);
  ana->AddFile(inputfiles);
  ana->SetOutputName(outputfileName.Data());
  // run the analysis after successful initialisation
  ana->Run(! gROOT->IsBatch());

  cout << "HLTMod: Accepted " << hltmod->GetNAccepted() 
       << " of " << hltmod->GetNEvents() << " events" << endl;

  return;

}
