// $Id: runLeptonFakeBkg.C,v 1.1 2008/08/30 14:02:16 sixie Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitAna/TreeMod/interface/ObjectValidationMod.h"
#include "MitHiggs/EwkMods/interface/WenuAnalysisMod.h"
#include "MitHiggs/EwkMods/interface/WmunuAnalysisMod.h"
#include "MitHiggs/EwkMods/interface/ZeeAnalysisMod.h"
#include "MitHiggs/EwkMods/interface/ZmumuAnalysisMod.h"
#include "MitHiggs/HwwMods/interface/HwwEvtSelMod.h"
#include "MitHiggs/HwwMods/interface/LeptonFakeBkgMod.h"
#endif

//--------------------------------------------------------------------------------------------------
void runLeptonFakeBkg(const char *files = 
//		      "mit-filler-RelVal210_Zee_000.root"
//		      "/server/02a/sixie/OAK/mit-filler-RelVal210_Wenu_000.root"
		      "/server/02a/sixie/OAK/mit-filler-RelVal210_Wmunu_000.root"
//		      "/server/02a/sixie/OAK/mit-filler-RelVal210_HWW165_000.root"
//		      "/server/02a/sixie/OAK/mit-filler-RelVal210_Zee_000.root"
//		      "/server/02a/sixie/OAK/mit-filler-RelVal210_Zmumu_001.root"
//		      "/server/02a/sixie/OAK/mit-filler-RelVal210_TTBAR_000.root"
//                    "/server/02a/sixie/OAK/mit-filler-RelVal210_QCDpt120-170_000.root"
)
{
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 1;

  // set up the modules 
  ObjectCleaningMod *objectSelectionMod = new ObjectCleaningMod;
  //ZeeAnalysisMod *eventSelectionMod = new ZeeAnalysisMod;
  //ZmumuAnalysisMod *eventSelectionMod = new ZmumuAnalysisMod;
  //WenuAnalysisMod *eventSelectionMod = new WenuAnalysisMod;
  //WmunuAnalysisMod *eventSelectionMod = new WmunuAnalysisMod;
  //HwwEvtSelMod *eventSelectionMod = new HwwEvtSelMod;
  LeptonFakeBkgMod *eventSelectionMod = new LeptonFakeBkgMod;

  // mod->Add(mod2);
  objectSelectionMod->Add(eventSelectionMod);

  // set up analysis
  Analysis *ana = new Analysis;
  ana->SetSuperModule(objectSelectionMod);
  ana->AddFile(files);
  ana->SetOutputName("mit-FakeLeptonBkg_Wmunu-hist.root");


  // run the analysis after successful initialisation
  ana->Run(! gROOT->IsBatch());
}
