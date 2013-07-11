// $Id: runObjectCleaner.C,v 1.1 2008/08/30 14:02:17 sixie Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/TreeMod/interface/ObjectCleaning.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#endif

//--------------------------------------------------------------------------------------------------
void runObjectCleaner(const char *files = 
                      "/server/02a/sixie/OAK/mit-filler-RelVal210_QCDpt120-170_000.root"
//                        "/server/02a/sixie/OAK/mit-filler-RelVal210_Zee_000.root"
//                      "/server/02a/sixie/OAK/mit-filler-RelVal210_HWW165_000.root"
//                        "/server/02a/sixie/OAK/mit-filler-RelVal210_QCDpt50-80_000.root"
//                        "/server/02a/sixie/OAK/mit-filler-RelVal210_QCDpt80-120_000.root"
//                      "/server/02a/sixie/OAK/mit-filler-RelVal210_Zmumu_000.root"
  )
{
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 1;

  // set up the modules
  ObjectCleaningMod *mod = new ObjectCleaningMod;

  // set up analysis
  Analysis *ana = new Analysis;
  ana->SetSuperModule(mod);
  ana->AddFile(files);
//  ana->SetOutputName("mit-objects-hist_QCD50-80.root");
//  ana->SetOutputName("mit-objects-hist_QCD80-120.root");
  ana->SetOutputName("mit-objects-hist_QCD120-170.root");
//  ana->SetOutputName("mit-objects-hist_Zmumu.root");

  // run the analysis after successful initialisation
  ana->Run(! gROOT->IsBatch());
}
