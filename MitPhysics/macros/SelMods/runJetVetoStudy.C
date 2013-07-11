// $Id: runJetVetoStudy.C,v 1.1 2008/11/12 15:04:52 sixie Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitAna/PhysicsMod/interface/JetVetoStudyMod.h"
#include "MitAna/TreeMod/interface/GeneratorMod.h"
#endif

//--------------------------------------------------------------------------------------------------
void runJetVetoStudy()
{
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 1;

  //string JetName = "IC5JetPlusTrack";
  string JetName = "CaloJets";

  // TTBAR
  TString files1      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateTTbar_dilepton-id9/*.root");
  //runAnalysisChain(files1, ("mit-JetVetoStudy-004-hist_ttbar_" + JetName + ".root").c_str(), TString(JetName.c_str()));
  //runAnalysisChain(files1, ("mit-JetVetoStudy-004-hist_ttbar_" + JetName + "Corr.root").c_str(), TString(JetName.c_str()),true); //For corrected jets. 
  //runAnalysisChain(files1, ("mit-JetVetoStudy-004-hist_ttbar_" + JetName + "_WithDirtyMuonVeto.root").c_str(), TString(JetName.c_str()),false,false,true);

   //For HWW
  TString files2      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateH165WW_2l-id9/mitPrivateH165WW_2l-id9_*.root");
  //runAnalysisChain(files2, ("mit-JetVetoStudy-004-hist_HWW_" + JetName +".root").c_str(), TString(JetName.c_str()));
  //runAnalysisChain(files2, ("mit-JetVetoStudy-004-hist_HWW_" + JetName +"Corr.root").c_str(), TString(JetName.c_str()),true);
  //runAnalysisChain(files2, ("mit-JetVetoStudy-004-hist_HWW_" + JetName +"_WithDirtyMuonVeto.root").c_str(), TString(JetName.c_str()),false,false,true);

  //For WW
  TString files3      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateWW_2l-id9/mitPrivateWW_2l-id9_000.root");
  //runAnalysisChain(files3, ("mit-JetVetoStudy-004-hist_WW_" + JetName +".root").c_str(), TString(JetName.c_str()));
  //runAnalysisChain(files2, ("mit-JetVetoStudy-004-hist_HWW_" + JetName +"Corr.root").c_str(), TString(JetName.c_str()),true);
  //runAnalysisChain(files3, ("mit-JetVetoStudy-004-hist_WW_" + JetName +"_WithDirtyMuonVeto.root").c_str(), TString(JetName.c_str()),false,false,true);


  //USE All HWW cuts except for JetVeto

  // TTBAR  
  TString files1      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateTTbar_dilepton-id9/*.root");
  //runAnalysisChain(files1, ("mit-JetVetoStudy-004-hist_ttbar_" + JetName + "_WithHWWCuts.root").c_str(), TString(JetName.c_str()),false,true);
  //runAnalysisChain(files1, ("mit-JetVetoStudy-004-hist_ttbar_" + JetName + "Corr_WithHWWCuts.root").c_str(), TString(JetName.c_str()),true,true);
  runAnalysisChain(files1, ("mit-JetVetoStudy-004-hist_ttbar_" + JetName + "_WithHWWCuts_WithDirtyMuonVeto.root").c_str(), TString(JetName.c_str()),true,true,true);

   //For HWW
  TString files2      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateH165WW_2l-id9/mitPrivateH165WW_2l-id9_*.root");
  //runAnalysisChain(files2, ("mit-JetVetoStudy-004-hist_HWW_" + JetName +"_WithHWWCuts.root").c_str(), TString(JetName.c_str()),false,true);
  //runAnalysisChain(files2, ("mit-JetVetoStudy-004-hist_HWW_" + JetName +"Corr_WithHWWCuts.root").c_str(), TString(JetName.c_str()),true,true);
  runAnalysisChain(files2, ("mit-JetVetoStudy-004-hist_HWW_" + JetName +"_WithHWWCuts_WithDirtyMuonVeto.root").c_str(), TString(JetName.c_str()),true,true,true);

  //For WW
  TString files3      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateWW_2l-id9/mitPrivateWW_2l-id9_000.root");
  //runAnalysisChain(files3, ("mit-JetVetoStudy-004-hist_WW_" + JetName +"_WithHWWCuts.root").c_str(), TString(JetName.c_str()),false,true);
  //runAnalysisChain(files3, ("mit-JetVetoStudy-004-hist_HWW_" + JetName +"Corr_WithHWWCuts.root").c_str(), TString(JetName.c_str()),true,true);
  runAnalysisChain(files3, ("mit-JetVetoStudy-004-hist_HWW_" + JetName +"_WithHWWCuts_WithDirtyMuonVeto.root").c_str(), TString(JetName.c_str()),true,true,true);


}


void runAnalysisChain(TString inputfiles, TString outputfileName, TString JetsName, bool doJetCorrections = false, bool imposeSelectionCuts = false, bool imposeDirtyMuonVeto = false) {

  GeneratorMod *GeneratorMod1 = new GeneratorMod;
  GeneratorMod1->SetFillHist(false);
  GeneratorMod1->SetPrintDebug(false);
  //Object ID and Cleaning Sequence;
  MuonIDMod *muonID = new MuonIDMod;
  muonID->SetPrintDebug(false);
  ElectronIDMod *electronID = new ElectronIDMod;
  electronID->SetPrintDebug(false);
  electronID->SetElectronIsoType("TrackCalo");
  JetIDMod *JetID = new JetIDMod; //default names are IC5
  JetID->SetJetName(JetsName); //for SC5 I have to set the names
  JetID->SetGoodJetsName(TString("GoodJets"));
  ElectronCleaningMod *electronCleaning = new ElectronCleaningMod;
  JetCleaningMod *JetCleaning = new JetCleaningMod; //default names are ok for IC5
  JetCleaning->SetGoodJetsName(TString("GoodJets"));
  JetCleaning->SetCleanJetsName(TString("CleanJets"));
  //Event Selection modules
  JetVetoStudyMod *eventSelectionMod = new JetVetoStudyMod;
  eventSelectionMod->SetCleanJetsName(TString("CleanJets"));
  eventSelectionMod->SetJetName(JetsName);
  eventSelectionMod->SetPrintDebug(false); 
  eventSelectionMod->SetImposeSelectionCuts(imposeSelectionCuts);
  eventSelectionMod->SetDoJetCorrections(doJetCorrections);
  eventSelectionMod->SetImposeDirtyMuonVeto(imposeDirtyMuonVeto);
  //chain modules together
  muonID->Add(electronID);
  electronID->Add(JetID);  
  JetID->Add(electronCleaning);
  electronCleaning->Add(JetCleaning);
  JetCleaning->Add(eventSelectionMod);
 // set up analysis
  Analysis *ana1 = new Analysis;
  ana1->SetUseHLT(false);
  ana1->SetKeepHierarchy(kFALSE);
  //ana1->SetProcessNevents(10000);

  ana1->SetSuperModule(muonID);
  ana1->AddFile(inputfiles);
  ana1->SetOutputName(outputfileName.Data());
  // run the analysis after successful initialisation
  ana1->Run(! gROOT->IsBatch());  

  return;


}
