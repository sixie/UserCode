// $Id: runElectronIsolationStudy.C,v 1.1 2008/11/12 15:04:52 sixie Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitAna/Validation/interface/JetValidationMod.h"
#endif

//--------------------------------------------------------------------------------------------------
void runElectronIsolationStudy()
{
  //run the jet Validation Module.
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 1;

//   //For Higgs. real electrons
  TString files1      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateH165WW_2l-id9/mitPrivateH165WW_2l-id9_*.root");
  //isolationStudy(files1, TString("mit-ElectronIsoStudy_HWW_hist.root"), false,true);
  
  //For QCD. Assume all are fake.
  TString files2      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-qcddj_50_80-id9/*.root");
  //isolationStudy(files2, TString("mit-ElectronIsoStudy_QCDDJ50-80_hist.root"),false, false);
  
//For QCD. Assume all are fake.
  TString files3      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-qcddj_30_50-id9/*.root");
  //isolationStudy(files3, TString("mit-ElectronIsoStudy_QCDDJ30-50_hist.root"),false, false);
  
//For QCD. Assume all are fake.
  TString files4      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-qcddj_170_230-id9/*.root");
  //isolationStudy(files4, TString("mit-ElectronIsoStudy_QCDDJ170-230_hist.root"),false, false);
  
  
//   //For QCD. Assume all are fake.
  TString files10      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-qcdem_20_30-id9/*.root");
  //isolationStudy(files10, TString("mit-ElectronIsoStudy_QCDEM20-30_hist.root"),false, false);


  //Impose Electron ID
  //For Higgs. real electrons
  TString files101      = TString("/server/04d/sixie/OAKSamples/004/mitPrivateH165WW_2l-id9/mitPrivateH165WW_2l-id9_*.root");
  isolationStudy(files101, TString("mit-ElectronIsoStudy_HWW_ImposeElectronID_hist.root"), true,true);

  //For QCD. Assume all are fake.
  TString files102      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-qcddj_50_80-id9/*.root");
  //isolationStudy(files102, TString("mit-ElectronIsoStudy_QCDDJ50-80_ImposeElectronID_hist.root"),true, false);

//For QCD. Assume all are fake.
  TString files103      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-qcddj_30_50-id9/*.root");
  //isolationStudy(files103, TString("mit-ElectronIsoStudy_QCDDJ30-50_ImposeElectronID_hist.root"),true, false);

//For QCD. Assume all are fake.
  TString files104      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-qcddj_170_230-id9/*.root");
  //isolationStudy(files104, TString("mit-ElectronIsoStudy_QCDDJ170-230_ImposeElectronID_hist.root"),true, false);

  //For QCD. Assume all are fake.
  TString files110      = TString("castor:/castor/cern.ch/user/p/paus/filler/004/s8-qcdem_20_30-id9/*.root");
  //isolationStudy(files110, TString("mit-ElectronIsoStudy_QCDEM20-30_ImposeElectronID_hist.root"),true, false);


}

void isolationStudy(TString inputfiles, TString outputfile, bool imposeElectronID, bool matchToGenElectrons) {
  // set up the modules 
  GeneratorMod *GeneratorMod1 = new GeneratorMod;
  GeneratorMod1->SetFillHist(false);
  GeneratorMod1->SetPrintDebug(false);  
  ElectronIsolationStudyMod *studyMod = new ElectronIsolationStudyMod;
  studyMod->SetMatchToRealElectrons(matchToGenElectrons);
  studyMod->SetImposeElectronID(imposeElectronID);
  //chain together modules
  GeneratorMod1->Add(studyMod);
  // set up analysis
  Analysis *ana = new Analysis;
  ana->SetUseHLT(false);
  if (matchToGenElectrons)
    ana->SetSuperModule(GeneratorMod1);
  else 
    ana->SetSuperModule(studyMod);
  ana->AddFile(inputfiles);
  //ana->SetProcessNevents(100000);
  ana->SetOutputName(outputfile.Data());
  // run the analysis after successful initialisation
  ana->Run(! gROOT->IsBatch());
}
