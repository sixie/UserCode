// $Id: runMuonIsolationStudy.C,v 1.1 2008/11/12 15:04:52 sixie Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitAna/Validation/interface/JetValidationMod.h"
#endif

//--------------------------------------------------------------------------------------------------
void runMuonIsolationStudy()
{
  //run the jet Validation Module.
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 1;

   //For Higgs. real electrons
  TString files1      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateH165WW_2l-id9/mitPrivateH165WW_2l-id9_*.root");
  //isolationStudy(files1, TString("mit-MuonIsoStudy_HWW_MuonPt20-40_hist.root"), 20,40, false,true);

  //For QCD. Assume all are fake.
  TString files2      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-qcddj_50_80-id9/*.root");
  //isolationStudy(files2, TString("mit-MuonIsoStudy_QCDDJ50-80_MuonPt20-40_hist.root"),20,40,false, false);

//For QCD. Assume all are fake.
  TString files3      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-qcddj_30_50-id9/*.root");
  //isolationStudy(files3, TString("mit-MuonIsoStudy_QCDDJ30-50_MuonPt20-40_hist.root"),20,40,false, false);

//For QCD. Assume all are fake.
  TString files4      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-qcddj_170_230-id9/*.root");
  //isolationStudy(files4, TString("mit-MuonIsoStudy_QCDDJ170-230_MuonPt20-40_hist.root"),20,40,false, false);
  
//  inclusive muon sample
   TString files10      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-incmu_5_50-id9/*.root");   
   isolationStudy(files10, TString("mit-MuonIsoStudy_InclMu5-50_MuonPt10-15_hist.root"),10,15,false, false);
   isolationStudy(files10, TString("mit-MuonIsoStudy_InclMu5-50_MuonPt15-20_hist.root"),15,20,false, false);
   isolationStudy(files10, TString("mit-MuonIsoStudy_InclMu5-50_MuonPt20-40_hist.root"),20,40,false, false);
}

void isolationStudy(TString inputfiles, TString outputfile, double ptmin, double ptmax, bool imposeID, bool matchToGen) {
  // set up the modules 
  GeneratorMod *GeneratorMod1 = new GeneratorMod;
  GeneratorMod1->SetFillHist(false);
  GeneratorMod1->SetPrintDebug(false);  
  MuonIsolationStudyMod *studyMod = new MuonIsolationStudyMod;
  studyMod->SetMatchToRealMuons(matchToGen);
  studyMod->SetImposeMuonID(imposeID);
  studyMod->SetMuonPtMin(ptmin);
  studyMod->SetMuonPtMax(ptmax);
  //chain together modules
  GeneratorMod1->Add(studyMod);
  // set up analysis
  Analysis *ana = new Analysis;
  ana->SetUseHLT(false);
  if (matchToGen)
    ana->SetSuperModule(GeneratorMod1);
  else 
    ana->SetSuperModule(studyMod);
  ana->AddFile(inputfiles);

  //ana->SetProcessNevents(100000);
  ana->SetOutputName(outputfile.Data());
  // run the analysis after successful initialisation
  ana->Run(! gROOT->IsBatch());
}
