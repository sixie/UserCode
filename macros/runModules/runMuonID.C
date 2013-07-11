// $Id: runMuonID.C,v 1.1 2008/08/30 14:02:16 sixie Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/TreeMod/interface/MuonID.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#endif

//--------------------------------------------------------------------------------------------------
void runMuonID(const char *files = 
//                      "/server/02a/sixie/OAK/mit-filler-RelVal210_HWW165_000.root"
//               "/server/02a/sixie/OAK/mit-filler-RelVal210_QCDpt50-80_000.root",
//               "/server/02a/sixie/OAK/mit-filler-RelVal210_QCDpt80-120_000.root"
//               "/server/02a/sixie/OAK/mit-filler-RelVal210_QCDpt120-170_000.root"
                      "/server/02a/sixie/OAK/mit-filler-RelVal210_Zmumu_000.root"

  )
{
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 1;

  // set up the modules
  MuonIDMod *centralMuonIDMod = new MuonIDMod("MuonIDMod", "central");
  MuonIDMod *forwardMuonIDMod = new MuonIDMod("MuonIDMod", "forward");
//  MuonIDMod *forwardMuonIDMod = new MuonIDMod("MuonIDMod, "all");

  // set up analysis
  Analysis *ana1 = new Analysis;
  ana1->SetSuperModule(centralMuonIDMod);
  ana1->AddFile(files);
//  ana1->SetOutputName("mit-muonIDs-hist_QCD50-80_centralGlobalMuons.root");
//    ana1->SetOutputName("mit-muonIDs-hist_QCD80-120_centralGlobalMuons.root");
//  ana1->SetOutputName("mit-muonID-hist_QCD120-170_centralGlobalMuons.root");
  ana1->SetOutputName("mit-muonIDs-hist_Zmumu_centralGlobalMuons.root");

  Analysis *ana2 = new Analysis;
  ana2->SetSuperModule(forwardMuonIDMod);
  ana2->AddFile(files);
//  ana2->SetOutputName("mit-muonIDs-hist_QCD50-80_forwardGlobalMuons.root");
//  ana2->SetOutputName("mit-muonIDs-hist_QCD80-120_forwardGlobalMuons.root");
//  ana2->SetOutputName("mit-muonID-hist_QCD120-170_forwardGlobalMuons.root");
  ana2->SetOutputName("mit-muonIDs-hist_Zmumu_forwardGlobalMuons.root");


  // run the analysis after successful initialisation
  ana1->Run(! gROOT->IsBatch());
  ana2->Run(! gROOT->IsBatch());
}
