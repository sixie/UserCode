// $Id: runHiggsWWExample.C,v 1.1 2008/11/23 15:59:12 sixie Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitHiggs/HwwMods/interface/HwwMCEvtSelMod.h"
#endif

//--------------------------------------------------------------------------------------------------
void runHiggsWWExample(const char *files = "$CMSSW_BASE/src/mit-hww160_*.root")
{
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 1;

  // set up the modules
  HwwMCEvtSelMod *mod = new HwwMCEvtSelMod;

  // set up analysis
  Analysis *ana = new Analysis;
  ana->SetSuperModule(mod);
  ana->AddFile(files);
  ana->SetOutputName("mit-hww-hist.root");

  // run the analysis after successful initialisation
  ana->Run(!gROOT->IsBatch());
}
