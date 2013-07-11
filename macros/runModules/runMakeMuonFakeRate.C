// $Id: runMakeMuonFakeRate.C,v 1.2 2008/09/08 18:19:13 sixie Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitAna/TreeMod/interface/ObjectValidationMod.h"
#include "MitHiggs/HwwMods/interface/LeptonFakeBkgMod.h"
#include "MitHiggs/FakeRateMods/interface/MuonFakeRateAnalysis.h"
#endif

//--------------------------------------------------------------------------------------------------
void runMakeMuonFakeRate()
{
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 1;

  // set up the modules 
  ObjectCleaningMod *objectSelectionMod = new ObjectCleaningMod;
  MuonFakeRateAnalysisMod *eventSelectionMod = new MuonFakeRateAnalysisMod;
  objectSelectionMod->Add(eventSelectionMod);

  objectSelectionMod->SetGoodElectronsName(TString("GoodElectrons"));
  objectSelectionMod->SetGoodMuonsName(TString("GoodMuons"));
  objectSelectionMod->SetGoodCentralJetsName(TString("GoodCentralJets"));
  objectSelectionMod->SetGoodForwardJetsName(TString("GoodForwardJets"));
  objectSelectionMod->SetGoodMuonCorrectedMetName(TString("MuonCorrectedMet"));
  eventSelectionMod->SetGoodElectronsName(TString("GoodElectrons"));
  eventSelectionMod->SetGoodMuonsName(TString("GoodMuons"));
  eventSelectionMod->SetGoodCentralJetsName(TString("GoodCentralJets"));
  eventSelectionMod->SetGoodForwardJetsName(TString("GoodForwardJets"));
  eventSelectionMod->SetGoodMuonCorrectedMetName(TString("MuonCorrectedMet"));

  // set up analysis
  Analysis *ana = new Analysis;
  ana->SetSuperModule(objectSelectionMod);

  const char *catalogDir = "/home/mitprod/catalog";
  const char *book       = "mit/filler/001";
  const char *dataset    = "qcdem_pt_30_80-id6";

//  Catalog *c = new Catalog(catalogDir);
//  Dataset *d = c->FindDataset(book,dataset);
//  ana->AddDataset(d);

 

  TString slash      = "/";
  TString castorBase = "castor:/castor/cern.ch/user/s/sixie/";
  //TString localBase  = "/server/03b/paus";
  //TString files      = TString("OAKSamples/test/") + dataset + TString("_000_*.root");
  //ana->AddFile(castorBase + slash + files );

  //TString files      = TString("/server/02a/sixie/CMSSW_2_1_5/src/testProd/crab_0_080831_113313/res/") + dataset + TString("_000_*.root");
  TString files      = TString("/server/04d/sixie/OAKSamples/002/qcdem_pt_30_80-id6/") + dataset + TString("_000_*.root");

  ana->AddFile(files);
  ana->SetOutputName("mit-MuonFakeRate_QCD-hist.root");
  ana->SetProcessNevents(1000000);

  // run the analysis after successful initialisation
  ana->Run(! gROOT->IsBatch());
}
