// $Id: runValidation.C,v 1.1 2008/08/30 14:02:17 sixie Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/TreeMod/interface/ObjectCleaning.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#endif

//--------------------------------------------------------------------------------------------------
void runValidation(const char *files = "mit-filler-test_000.root")
{

  gROOT->Macro("$CMSSW_BASE/src/MitAna/macros/setRootEnv.C+");
  gSystem->Load("$CMSSW_BASE/lib/slc4_ia32_gcc345/libMitAnaValidation.so");

  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 1;

  // set up the modules
  GenRelValMod *mod = new GenRelValMod;

  // set up analysis
  Analysis *ana = new Analysis;
  ana->SetSuperModule(mod);
  ana->AddFile(files);

  // run the analysis after successful initialisation
  ana->Run(! gROOT->IsBatch());
}
