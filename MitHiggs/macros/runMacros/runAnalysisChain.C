// $Id: runAnalysisChain.C,v 1.1 2008/11/23 15:59:11 sixie Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitHiggs/HwwMods/interface/HwwEvtSelMod.h"
#include "MitAna/TreeMod/interface/ObjectValidationMod.h"
#endif

//--------------------------------------------------------------------------------------------------
void runAnalysisChain(const char *files = 
		      //"/server/02a/sixie/OAK/mit-hww-test_hasMuonElectronInfo.root"
		      "mit-filler-test_000.root"
)
{
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 1;

  // set up the modules 
  ObjectCleaningMod *objectSelectionMod = new ObjectCleaningMod;
  HwwEvtSelMod *eventSelectionMod = new HwwEvtSelMod;

  // mod->Add(mod2);
  objectSelectionMod->Add(eventSelectionMod);


  // set up analysis
  Analysis *ana = new Analysis;
  ana->SetSuperModule(objectSelectionMod);
  ana->AddFile(files);
  ana->SetOutputName("mit-AnalysisChain-hist.root");

  // run the analysis after successful initialisation
  ana->Run(! gROOT->IsBatch());
}
