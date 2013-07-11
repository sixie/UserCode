//root -l -q -b $CMSSW_BASE/src/MitHiggs/macros/runMacros/runAllHZZNtupler.C+\(\"0000\",\"noskim\",\"r11a-dmu-m10-v1\",\"cern/filefi/022\",\"/home/mitprod/catalog\",\"AllNtupler\",10000,-1\) 
//root -l -q -b $CMSSW_BASE/src/MitHiggs/macros/runMacros/runAllHZZNtupler.C+\(\"0000\",\"noskim\",\"s11-h250zz2l-gf-v1g1-pu\",\"cern/filefi/022\",\"/home/mitprod/catalog\",\"AllNtupler\",10000,250\) 
//root -l -q -b $CMSSW_BASE/src/MitHiggs/macros/runMacros/runAllHZZNtupler.C+\(\"0000\",\"noskim\",\"s11-zmmm20-v11-pu\",\"cern/filefi/022\",\"/home/mitprod/catalog\",\"AllNtupler\",10000,6\) 
//root -l -q -b $CMSSW_BASE/src/MitHiggs/macros/runMacros/runAllHZZNtupler.C+\(\"0000\",\"noskim\",\"w10-wjets-z2-v8-pu11-2l\",\"cern/filler/020\",\"/home/ceballos/catalog\",\"AllNtupler\",-1,10333\)   

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/Catalog/interface/Catalog.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "EWKAna/Ntupler/interface/HwwNtuplerMod.hh"
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/Catalog/interface/Catalog.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitAna/TreeMod/interface/HLTMod.h"
#include "MitAna/PhysicsMod/interface/PublisherMod.h"
#include "MitAna/PhysicsMod/interface/RunLumiSelectionMod.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitPhysics/Mods/interface/GoodPVFilterMod.h"
#include "MitPhysics/Mods/interface/GeneratorMod.h"
#include "MitPhysics/Mods/interface/PDFProducerMod.h"
#include "MitPhysics/Mods/interface/HKFactorProducer.h"
#include "MitPhysics/Mods/interface/JetCorrectionMod.h"
#include "MitPhysics/Mods/interface/CaloMetCorrectionMod.h"
#include "MitPhysics/Mods/interface/MuonIDMod.h"
#include "MitPhysics/Mods/interface/ElectronIDMod.h"
#include "MitPhysics/Mods/interface/PhotonIDMod.h"
#include "MitPhysics/Mods/interface/PFTauIDMod.h"
#include "MitPhysics/Mods/interface/JetIDMod.h"
#include "MitPhysics/Mods/interface/ElectronCleaningMod.h"
#include "MitPhysics/Mods/interface/PhotonCleaningMod.h"
#include "MitPhysics/Mods/interface/PFTauCleaningMod.h"
#include "MitPhysics/Mods/interface/JetCleaningMod.h"
#include "MitPhysics/Mods/interface/MergeLeptonsMod.h"
#include "MitPhysics/Mods/interface/PartonFlavorHistoryMod.h"
#include "MitAna/DataTree/interface/PFJetCol.h"
#include "MitAna/DataTree/interface/JetCol.h"
#include "MitAna/DataTree/interface/CaloJetCol.h"
#include "MitAna/DataTree/interface/MetCol.h" 
#include "MitAna/DataTree/interface/PFMetCol.h"
#include "MitAna/DataTree/interface/CaloMetCol.h"
#include "MitPhysics/SelMods/interface/GenericSelMod.h"
#include "MitHiggs/EwkMods/interface/SignalFilterForJettinessMod.h"
#include "MitHiggs/HwwMods/interface/HwwMakeNtupleMod.h"

#endif

using namespace mithep;

//==================================================================================================
/*
 * Triggers of interest
 *

  kHLT_Mu9                                   = 0x0000001,
  kHLT_Mu11                                  = 0x0000002,
  kHLT_Mu13_v1                               = 0x0000004,
  kHLT_Mu15_v1                               = 0x0000008,
  kHLT_DoubleMu5_v1                          = 0x0000010,
  kHLT_Jet15U                                = 0x0000020,
  kHLT_Jet30U                                = 0x0000040,
  kHLT_Jet50U                                = 0x0000080,
  kHLT_Photon10_L1R                          = 0x0000100,
  kHLT_Photon10_Cleaned_L1R                  = 0x0000200,
  kHLT_Photon15_L1R                          = 0x0000400,
  kHLT_Photon15_Cleaned_L1R                  = 0x0000800,
  kHLT_Photon20_L1R                          = 0x0001000,
  kHLT_Photon20_Cleaned_L1R                  = 0x0002000,
  kHLT_Photon30_Cleaned_L1R                  = 0x0004000,
  kHLT_Ele15_SW_L1R                          = 0x0008000,
  kHLT_Ele15_LW_L1R                          = 0x0010000,
  kHLT_Ele15_SW_CaloEleId_L1R                = 0x0020000,
  kHLT_Ele17_SW_L1R                          = 0x0040000,
  kHLT_Ele17_SW_CaloEleId_L1R                = 0x0080000,       
  kHLT_Ele17_SW_TightEleId_L1R               = 0x0100000,
  kHLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v1 = 0x0200000,
  kHLT_Ele17_SW_TightCaloEleId_SC8HE_L1R_v1  = 0x0400000,
  kHLT_Ele17_SW_TighterEleIdIsol_L1R_v2      = 0x0800000,
  kHLT_DoubleEle10_SW_L1R                    = 0x1000000,
  kHLT_DoubleEle15_SW_L1R_v1                 = 0x2000000

  */
    
//==================================================================================================
/*
 * Run on a BAMBU fileset
 *
 * Example usage:
 *   root -l -q -b runZeeNtupler.C+\(\"0000\",\"p10-zee-v26\",\"cern/filler/014a\",\"/home/ceballos/catalog\",1,0,1,-1,0,1\)
 *
 * Output file name has standard format: <dataset>_<fileset>_ntuple.root
 *
 */
void runAllHZZNtupler(
  const char *fileset  = "",
  const char *skim         = "noskim",
  const char *dataset    = "s8-ttbar-id9",
  const char *book       = "mit/filler/006",
  const char *catalogDir = "/home/mitprod/catalog",
  const char *outputName = "HwwHiggsNtupleMaker",
  int   nEvents          = -1,
  int   sampleID         = -1
  )
{
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 3;

  //******************************************************************
  //Set up Options
  //******************************************************************
  bool doHWWNtupler       = true;
  bool doSmurfNtupler     = true;
  bool usePDFProducer     = false;
  string pdfSetName = "";

  bool isData             = false;
  bool isDataMuonElectron = false;
  bool isDataDMuon        = false;
  bool isDataSMuon        = false;
  bool isDataDElectron    = false;
  bool isDataSElectron    = false;
  bool isDataSPhoton      = false;
  bool applyMllGenCut     = false;
  bool applyZVetoGenCut   = false;

  double ptJetCut = 30.0;
  double etaJetCut = 5.0;

  int processid = -999999999;
  TString fInputFilenameKF = "/home/ceballos/releases/CMSSW_4_2_2/src/MitPhysics/data/HWW_KFactors_PowhegToNNLL_160_7TeV.dat";
  TString MCType = "kMCTypeUndef";
  Bool_t applyPartonFlavorFilter = kFALSE;
  Bool_t applyISRFilter          = kFALSE;
  Bool_t applyVVFilter           = kFALSE;
  Int_t fakeRatePredictionType = 0;
  Bool_t useSelectGenLeptons   = kTRUE;
  Bool_t isPhotonControlSample = kFALSE;

  int fDecay = sampleID; 
  if (sampleID < 0) fDecay = (-1) * (abs(sampleID) % 10);
  if (sampleID == 10333) fDecay = 3;
  
  Int_t runSkim = 0; if (sampleID <= -1 && sampleID >= -5) runSkim = 1; 
  if (sampleID <= -11 && sampleID >= -15) runSkim = 2; 
  if (sampleID <= -21 && sampleID >= -25) runSkim = 3; 

  if (fDecay < 0) {
    isData = true;
    if (fDecay == -1) isDataDMuon = true;
    if (fDecay == -2) isDataDElectron = true;
    if (fDecay == -3) isDataMuonElectron = true;
    if (fDecay == -4) isDataSMuon = true;
    if (fDecay == -5) isDataSElectron = true;
    if (fDecay == -6) isDataSPhoton = true;
  }

  if (fDecay == 14) applyVVFilter = kTRUE;
  if (fDecay > 20000) {
    fakeRatePredictionType = 1;
    useSelectGenLeptons    = kFALSE;
  }
  if (isDataSPhoton || fDecay == 10666 || fDecay == 11666) {
    isPhotonControlSample = kTRUE;
    doHWWNtupler = kFALSE;
  }



  cout << "Summarize Run Options: " << "fDecay == " << fDecay << " "
       << "runSkim == " << runSkim << " "
       << endl;

  //******************************************************************
  //Modules
  //******************************************************************

  // Generator info
  GeneratorMod *GeneratorMod1 = new GeneratorMod;
  GeneratorMod1->SetPrintDebug(kFALSE);
  GeneratorMod1->SetPtLeptonMin(0.0);
  GeneratorMod1->SetEtaLeptonMax(2.7);
  GeneratorMod1->SetPtPhotonMin(15.0);
  GeneratorMod1->SetEtaPhotonMax(2.7);
  GeneratorMod1->SetPtRadPhotonMin(10.0);
  GeneratorMod1->SetEtaRadPhotonMax(2.7);
  GeneratorMod1->SetIsData(isData);
  GeneratorMod1->SetFillHist(!isData);
  if(applyMllGenCut == kTRUE){
    GeneratorMod1->SetPdgIdCut(23);
    GeneratorMod1->SetMassMaxCut(50.);
  }
  else if(applyZVetoGenCut == kTRUE){
    GeneratorMod1->SetPdgIdCut(23);
    GeneratorMod1->SetMassMinCut(20000.);
    GeneratorMod1->SetMassMaxCut(20000.);
  }
  GeneratorMod1->SetApplyISRFilter(applyISRFilter);
  GeneratorMod1->SetApplyVVFilter(applyVVFilter);
  GeneratorMod1->SetAllowWWEvents(kTRUE);
  GeneratorMod1->SetAllowWZEvents(kFALSE);
  GeneratorMod1->SetAllowZZEvents(kFALSE);

  PartonFlavorHistoryMod *PartonFlavorHistoryMod1 = new PartonFlavorHistoryMod;
  PartonFlavorHistoryMod1->SetMCSampleType(MCType);
  PartonFlavorHistoryMod1->SetApplyPartonFlavorFilter(applyPartonFlavorFilter);

  // HLT info
  // HLT info
  HLTMod *hltmod = new HLTMod;
//   if(nsel == 299) {
//     hltmod->AddTrigger("HLT_Mu15_v2");
//     hltmod->AddTrigger("!HLT_Mu15_v2");
//     hltmod->AddTrigger("HLT_Mu9");
//     hltmod->AddTrigger("!HLT_Mu9");
//   }
//   else 
  if(isData == true && isDataMuonElectron == true) {
    hltmod->AddTrigger("HLT_Mu17_Ele8_CaloIdL_v1",150000,161176);
    hltmod->AddTrigger("HLT_Mu8_Ele17_CaloIdL_v1",150000,161176);
    hltmod->AddTrigger("HLT_Mu17_Ele8_CaloIdL_v2",161179,163261);
    hltmod->AddTrigger("HLT_Mu8_Ele17_CaloIdL_v2",161179,163261);
    hltmod->AddTrigger("HLT_Mu17_Ele8_CaloIdL_v3",163262,164237);
    hltmod->AddTrigger("HLT_Mu8_Ele17_CaloIdL_v3",163262,164237);
    hltmod->AddTrigger("HLT_Mu17_Ele8_CaloIdL_v4",165085,165888);
    hltmod->AddTrigger("HLT_Mu8_Ele17_CaloIdL_v4",165085,165888);
    hltmod->AddTrigger("HLT_Mu17_Ele8_CaloIdL_v5",165900,166967);
    hltmod->AddTrigger("HLT_Mu8_Ele17_CaloIdL_v5",165900,166967);
    hltmod->AddTrigger("HLT_Mu17_Ele8_CaloIdL_v6",166968,170053);
    hltmod->AddTrigger("HLT_Mu8_Ele17_CaloIdL_v6",166968,170053);
    hltmod->AddTrigger("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v3",170054,999999);
    hltmod->AddTrigger("HLT_Mu17_Ele8_CaloIdL_v8",170054,999999); 
  }
  else if(isData == true && isDataDMuon == true) {
    cout << "Double muon\n";
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v1&!HLT_Mu17_Ele8_CaloIdL_v1&HLT_DoubleMu7_v1",150000,161176);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v2&!HLT_Mu17_Ele8_CaloIdL_v2&HLT_DoubleMu7_v1",161179,163261);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v3&!HLT_Mu17_Ele8_CaloIdL_v3&HLT_DoubleMu7_v2",163262,164237);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v4&!HLT_Mu17_Ele8_CaloIdL_v4&HLT_Mu13_Mu8_v2" ,165085,165888);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v5&!HLT_Mu17_Ele8_CaloIdL_v5&!HLT_Mu8_Ele17_CaloIdL_v6&!HLT_Mu17_Ele8_CaloIdL_v6&HLT_Mu13_Mu8_v2" ,165900,167043);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v5&!HLT_Mu17_Ele8_CaloIdL_v5&!HLT_Mu8_Ele17_CaloIdL_v6&!HLT_Mu17_Ele8_CaloIdL_v6&HLT_Mu13_Mu8_v4" ,167044,170053);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v3&!HLT_Mu17_Ele8_CaloIdL_v8&HLT_Mu13_Mu8_v6" ,170054,999999);
  }
  else if(isData == true && isDataSMuon == true) {
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v1&!HLT_Mu17_Ele8_CaloIdL_v1&!HLT_DoubleMu7_v1&HLT_Mu15_v2",150000,161176);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v2&!HLT_Mu17_Ele8_CaloIdL_v2&!HLT_DoubleMu7_v1&HLT_Mu15_v2",161179,163261);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v3&!HLT_Mu17_Ele8_CaloIdL_v3&!HLT_DoubleMu7_v2&HLT_Mu24_v2",163262,164237);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v3&!HLT_Mu17_Ele8_CaloIdL_v3&!HLT_DoubleMu7_v2&HLT_IsoMu17_v6",163262,164237);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v4&!HLT_Mu17_Ele8_CaloIdL_v4&!HLT_Mu13_Mu8_v2&HLT_Mu30_v3",165085,165888);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v4&!HLT_Mu17_Ele8_CaloIdL_v4&!HLT_Mu13_Mu8_v2&HLT_IsoMu17_v8",165085,165888);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v5&!HLT_Mu17_Ele8_CaloIdL_v5&!HLT_Mu8_Ele17_CaloIdL_v6&!HLT_Mu17_Ele8_CaloIdL_v6&!HLT_Mu13_Mu8_v2&HLT_Mu30_v3",165900,167043);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v5&!HLT_Mu17_Ele8_CaloIdL_v5&!HLT_Mu8_Ele17_CaloIdL_v6&!HLT_Mu17_Ele8_CaloIdL_v6&!HLT_Mu13_Mu8_v2&HLT_IsoMu17_v9",165900,167043);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v5&!HLT_Mu17_Ele8_CaloIdL_v5&!HLT_Mu8_Ele17_CaloIdL_v6&!HLT_Mu17_Ele8_CaloIdL_v6&!HLT_Mu13_Mu8_v4&HLT_Mu30_v5",167044,170053);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v5&!HLT_Mu17_Ele8_CaloIdL_v5&!HLT_Mu8_Ele17_CaloIdL_v6&!HLT_Mu17_Ele8_CaloIdL_v6&!HLT_Mu13_Mu8_v4&HLT_IsoMu17_eta2p1_v1",167044,170053);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v3&!HLT_Mu17_Ele8_CaloIdL_v8&!HLT_Mu13_Mu8_v6&HLT_Mu40_v5",170071,999999);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v3&!HLT_Mu17_Ele8_CaloIdL_v8&!HLT_Mu13_Mu8_v6&HLT_IsoMu24_v8",170071,999999);
  }
  else if(isData == true && isDataDElectron == true) {
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v1&!HLT_Mu17_Ele8_CaloIdL_v1&!HLT_DoubleMu7_v1&!HLT_Mu15_v2&HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v1",150000,161176);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v2&!HLT_Mu17_Ele8_CaloIdL_v2&!HLT_DoubleMu7_v1&!HLT_Mu15_v2&HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v2",161179,163261);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v3&!HLT_Mu17_Ele8_CaloIdL_v3&!HLT_DoubleMu7_v2&!HLT_Mu24_v2&!HLT_IsoMu17_v6&HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v3",163262,164237);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v4&!HLT_Mu17_Ele8_CaloIdL_v4&!HLT_Mu13_Mu8_v2&!HLT_Mu30_v3&!HLT_IsoMu17_v8&HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v4",165085,165888);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v5&!HLT_Mu17_Ele8_CaloIdL_v5&!HLT_Mu8_Ele17_CaloIdL_v6&!HLT_Mu17_Ele8_CaloIdL_v6&!HLT_Mu13_Mu8_v2&!HLT_Mu13_Mu8_v4&!HLT_Mu30_v3&!HLT_Mu30_v5&!HLT_IsoMu17_v9&!HLT_IsoMu17_v11&HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v5",165900,166967);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v5&!HLT_Mu17_Ele8_CaloIdL_v5&!HLT_Mu8_Ele17_CaloIdL_v6&!HLT_Mu17_Ele8_CaloIdL_v6&!HLT_Mu13_Mu8_v2&!HLT_Mu13_Mu8_v4&!HLT_Mu30_v3&!HLT_Mu30_v5&!HLT_IsoMu17_v9&!HLT_IsoMu17_v11&HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v6",166968,170053);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v3&!HLT_Mu17_Ele8_CaloIdL_v8&!HLT_Mu13_Mu8_v6&!HLT_Mu40_v5&!HLT_IsoMu24_v8&HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v6",170054,170825);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v3&!HLT_Mu17_Ele8_CaloIdL_v8&!HLT_Mu13_Mu8_v6&!HLT_Mu40_v5&!HLT_IsoMu24_v8&HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v7",170826,999999);


  }
  else if(isData == true && isDataSElectron == true) {
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v1&!HLT_Mu17_Ele8_CaloIdL_v1&!HLT_DoubleMu7_v1&!HLT_Mu15_v2&!HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v1&HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1",150000,161176);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v2&!HLT_Mu17_Ele8_CaloIdL_v2&!HLT_DoubleMu7_v1&!HLT_Mu15_v2&!HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v2&HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2",161179,163261);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v3&!HLT_Mu17_Ele8_CaloIdL_v3&!HLT_DoubleMu7_v2&!HLT_Mu24_v2&!HLT_IsoMu17_v6&!HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v3&HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3",163262,164237);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v4&!HLT_Mu17_Ele8_CaloIdL_v4&!HLT_Mu13_Mu8_v2&!HLT_Mu30_v3&!HLT_IsoMu17_v8&!HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v4&HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3",165085,165888);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v5&!HLT_Mu17_Ele8_CaloIdL_v5&!HLT_Mu8_Ele17_CaloIdL_v6&!HLT_Mu17_Ele8_CaloIdL_v6&!HLT_Mu13_Mu8_v2&!HLT_Mu13_Mu8_v4&!HLT_Mu30_v3&!HLT_Mu30_v5&!HLT_IsoMu17_v9&!HLT_IsoMu17_v11&!HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v5&HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v4",165900,166967);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdL_v5&!HLT_Mu17_Ele8_CaloIdL_v5&!HLT_Mu8_Ele17_CaloIdL_v6&!HLT_Mu17_Ele8_CaloIdL_v6&!HLT_Mu13_Mu8_v2&!HLT_Mu13_Mu8_v4&!HLT_Mu30_v3&!HLT_Mu30_v5&!HLT_IsoMu17_v9&!HLT_IsoMu17_v11&!HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v6&HLT_Ele52_CaloIdVT_TrkIdT_v3",166968,170053);
    hltmod->AddTrigger("!HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v3&!HLT_Mu17_Ele8_CaloIdL_v8&!HLT_Mu13_Mu8_v6&!HLT_Mu40_v5&!HLT_IsoMu24_v8&!HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v6&!HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v7&HLT_Ele65_CaloIdVT_TrkIdT_v3",170054,999999);

  }
  else if(isData == true && isDataSPhoton == true) {
    hltmod->AddTrigger("HLT_Photon20_CaloIdVL_IsoL_v1");
    hltmod->AddTrigger("HLT_Photon20_CaloIdVL_IsoL_v2");
    hltmod->AddTrigger("HLT_Photon20_CaloIdVL_IsoL_v3");
    hltmod->AddTrigger("HLT_Photon20_CaloIdVL_IsoL_v4");
    hltmod->AddTrigger("HLT_Photon20_CaloIdVL_IsoL_v5");
    hltmod->AddTrigger("HLT_Photon20_CaloIdVL_IsoL_v6");

    hltmod->AddTrigger("HLT_Photon30_CaloIdVL_v1");
    hltmod->AddTrigger("HLT_Photon30_CaloIdVL_v2");
    hltmod->AddTrigger("HLT_Photon30_CaloIdVL_v3");
    hltmod->AddTrigger("HLT_Photon30_CaloIdVL_v4");
    hltmod->AddTrigger("HLT_Photon30_CaloIdVL_v5");
    hltmod->AddTrigger("HLT_Photon30_CaloIdVL_v6");
    hltmod->AddTrigger("HLT_Photon30_CaloIdVL_IsoL_v1");
    hltmod->AddTrigger("HLT_Photon30_CaloIdVL_IsoL_v2");
    hltmod->AddTrigger("HLT_Photon30_CaloIdVL_IsoL_v3");
    hltmod->AddTrigger("HLT_Photon30_CaloIdVL_IsoL_v4");
    hltmod->AddTrigger("HLT_Photon30_CaloIdVL_IsoL_v5");
    hltmod->AddTrigger("HLT_Photon30_CaloIdVL_IsoL_v6");
    hltmod->AddTrigger("HLT_Photon30_CaloIdVL_IsoL_v7");

    hltmod->AddTrigger("HLT_Photon50_CaloIdVL_IsoL_v1");
    hltmod->AddTrigger("HLT_Photon50_CaloIdVL_IsoL_v2");
    hltmod->AddTrigger("HLT_Photon50_CaloIdVL_IsoL_v3");
    hltmod->AddTrigger("HLT_Photon50_CaloIdVL_IsoL_v4");
    hltmod->AddTrigger("HLT_Photon50_CaloIdVL_IsoL_v5");
    hltmod->AddTrigger("HLT_Photon50_CaloIdVL_IsoL_v6");
    hltmod->AddTrigger("HLT_Photon50_CaloIdVL_v1");
    hltmod->AddTrigger("HLT_Photon50_CaloIdVL_v2");
    hltmod->AddTrigger("HLT_Photon50_CaloIdVL_v3");

    hltmod->AddTrigger("HLT_Photon75_CaloIdVL_v1");
    hltmod->AddTrigger("HLT_Photon75_CaloIdVL_v2");
    hltmod->AddTrigger("HLT_Photon75_CaloIdVL_v3");
    hltmod->AddTrigger("HLT_Photon75_CaloIdVL_v4");
    hltmod->AddTrigger("HLT_Photon75_CaloIdVL_v5");
    hltmod->AddTrigger("HLT_Photon75_CaloIdVL_v6");
    hltmod->AddTrigger("HLT_Photon75_CaloIdVL_v7");
    hltmod->AddTrigger("HLT_Photon75_CaloIdVL_IsoL_v1");
    hltmod->AddTrigger("HLT_Photon75_CaloIdVL_IsoL_v2");
    hltmod->AddTrigger("HLT_Photon75_CaloIdVL_IsoL_v3");
    hltmod->AddTrigger("HLT_Photon75_CaloIdVL_IsoL_v4");
    hltmod->AddTrigger("HLT_Photon75_CaloIdVL_IsoL_v5");
    hltmod->AddTrigger("HLT_Photon75_CaloIdVL_IsoL_v6");

    hltmod->AddTrigger("HLT_Photon90_CaloIdVL_IsoL_v1");
    hltmod->AddTrigger("HLT_Photon90_CaloIdVL_IsoL_v2");
    hltmod->AddTrigger("HLT_Photon90_CaloIdVL_IsoL_v3");
    hltmod->AddTrigger("HLT_Photon90_CaloIdVL_IsoL_v4");
    hltmod->AddTrigger("HLT_Photon90_CaloIdVL_v1");
    hltmod->AddTrigger("HLT_Photon90_CaloIdVL_v2");
    hltmod->AddTrigger("HLT_Photon90_CaloIdVL_v3");
    hltmod->AddTrigger("HLT_Photon125_v1");
    hltmod->AddTrigger("HLT_Photon125_v2");
    hltmod->AddTrigger("HLT_Photon135_v1");

 
  } 
  else if(sampleID == 10333 || sampleID == 11666 || sampleID == 10666 ) {
    hltmod->AddTrigger("HLT_Mu15_v1");
    hltmod->AddTrigger("!HLT_Mu15_v1");
  }
  else {
//     hltmod->AddTrigger("HLT_Mu15_v2");
//     hltmod->AddTrigger("HLT_IsoMu17_v5");
//     hltmod->AddTrigger("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2");
//     hltmod->AddTrigger("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v2");
//     hltmod->AddTrigger("HLT_DoubleMu7_v1");
//     hltmod->AddTrigger("HLT_Mu8_Ele17_CaloIdL_v2");
//     hltmod->AddTrigger("HLT_Mu17_Ele8_CaloIdL_v2"); 
//     hltmod->AddTrigger("!HLT_Mu15_v2");
    hltmod->SetAbortIfNotAccepted(kFALSE);
  }
  hltmod->SetTrigObjsName("myhltobjs");



  //------------------------------------------------------------------------------------------------
  // Dilepton Skim
  //------------------------------------------------------------------------------------------------
  MuonIDMod *muTightId = new MuonIDMod;  
  muTightId->SetClassType ("Global");
  muTightId->SetIDType    ("WWMuIdV2");
  muTightId->SetIsoType   ("CombinedRelativeConeAreaCorrected");
  muTightId->SetCombRelativeIsoCut(0.15);
  muTightId->SetApplyD0Cut(kTRUE);
  muTightId->SetApplyDZCut(kTRUE);
  muTightId->SetWhichVertex(0);
  muTightId->SetOutputName("SkimMuons");

  ElectronIDMod       *electronTightId       = new ElectronIDMod;
  electronTightId->SetApplyEcalFiducial(kTRUE);
  electronTightId->SetIDType(TString("VBTFWorkingPoint80Id"));
  electronTightId->SetIsoType(TString("CombinedRelativeConeAreaCorrected"));
  electronTightId->SetApplyConversionFilterType1(kFALSE);
  electronTightId->SetApplyConversionFilterType2(kTRUE);
  electronTightId->SetChargeFilter(kFALSE);
  electronTightId->SetApplyD0Cut(kFALSE);
  electronTightId->SetApplyDZCut(kFALSE);
  electronTightId->SetWhichVertex(0);
  electronTightId->SetNExpectedHitsInnerCut(0);
  electronTightId->SetOutputName("SkimElectrons");


  MuonIDMod *muDenominator = new MuonIDMod;  
  muDenominator->SetClassType ("Global");
  muDenominator->SetIDType    ("WWMuIdV2");
  muDenominator->SetIsoType   ("CombinedRelativeConeAreaCorrected");
  muDenominator->SetApplyD0Cut(kTRUE);
  muDenominator->SetApplyDZCut(kTRUE);
  muDenominator->SetD0Cut(0.20);
  muDenominator->SetDZCut(0.10);
  muDenominator->SetCombIsoCut(0.4); 
  muDenominator->SetOutputName("SkimDenominatorMuons");
  muDenominator->SetWhichVertex(0);

  ElectronIDMod *electronDenominator = new ElectronIDMod;
  electronDenominator->SetApplyEcalFiducial(kTRUE);
  electronDenominator->SetIDType("VBTFWorkingPointFakeableId");
  electronDenominator->SetIsoType("TrackJura");
  electronDenominator->SetTrackIsoCut(0.2);
  electronDenominator->SetEcalJurIsoCut(0.2);
  electronDenominator->SetHcalIsoCut(0.2);
  electronDenominator->SetApplyConversionFilterType1(kFALSE);
  electronDenominator->SetApplyConversionFilterType2(kTRUE);
  electronDenominator->SetChargeFilter              (kFALSE);
  electronDenominator->SetApplyD0Cut                (kFALSE);
  electronDenominator->SetApplyDZCut                (kFALSE);
  electronDenominator->SetNExpectedHitsInnerCut     (0);
  electronDenominator->SetDZCut(0.10);
  electronDenominator->SetOutputName("SkimDenominatorElectrons");

  MergeLeptonsMod *mergedTight = new MergeLeptonsMod;
  mergedTight->SetMuonsName    (muTightId->GetOutputName());
  mergedTight->SetElectronsName(electronTightId->GetOutputName());
  mergedTight->SetOutputName("mergedTightLeptons");

  MergeLeptonsMod *mergedLoose = new MergeLeptonsMod;
  mergedLoose->SetMuonsName    (muDenominator->GetOutputName());
  mergedLoose->SetElectronsName(electronDenominator->GetOutputName());
  mergedLoose->SetOutputName("mergedLooseLeptons");

  GenericSelMod<mithep::Particle> *selModTight = new GenericSelMod<mithep::Particle>;
  selModTight->SetPtMin(0.0);
  selModTight->SetMinCounts(1);
  selModTight->SetColName(mergedTight->GetOutputName());

  GenericSelMod<mithep::Particle> *selModLoose = new GenericSelMod<mithep::Particle>;
  selModLoose->SetPtMin(0.0);
  selModLoose->SetMinCounts(1);
  selModLoose->SetColName(mergedLoose->GetOutputName());

  GenericSelMod<mithep::Particle> *selModDoubleLoose = new GenericSelMod<mithep::Particle>;
  selModDoubleLoose->SetPtMin(0.0);
  selModDoubleLoose->SetMinCounts(2);
  selModDoubleLoose->SetColName(mergedLoose->GetOutputName());

  //------------------------------------------------------------------------------------------------
  // Run RunLumiSelectionMod
  //------------------------------------------------------------------------------------------------
  RunLumiSelectionMod *runLumiSelection = new RunLumiSelectionMod;      
  runLumiSelection->SetAcceptMC(!isData);
  runLumiSelection->SetAcceptAll(kTRUE);
  runLumiSelection->AddJSONFile("certifiedUCSD.json");
  runLumiSelection->SetAbortIfNotAccepted(kFALSE);

  //------------------------------------------------------------------------------------------------
  // KFactor Producer Mainly for MC@NLO
  //------------------------------------------------------------------------------------------------
  HKFactorProducer *HKFactorProducer1 = new HKFactorProducer;
  HKFactorProducer1->SetProcessID(0); //0 means don't use weights. 998 uses weights
  HKFactorProducer1->SetFillHist(!isData);

  //------------------------------------------------------------------------------------------------
  // PV filter selection
  //------------------------------------------------------------------------------------------------
  GoodPVFilterMod *goodPVFilterMod = new GoodPVFilterMod;
  goodPVFilterMod->SetMinVertexNTracks(0);
  goodPVFilterMod->SetMinNDof(4);
  goodPVFilterMod->SetMaxAbsZ(24.0);
  goodPVFilterMod->SetMaxRho(2.0);
  if (sampleID == 100333 || sampleID > 100000) { goodPVFilterMod->SetVertexesName("DAPrimaryVertexes"); cout << "use DAPrimaryVertexes\n";}
  else goodPVFilterMod->SetVertexesName("PrimaryVertexes");

  Bool_t isFastSim = kFALSE;

  PDFProducerMod *PDFProducerMod1 = new PDFProducerMod;
  PDFProducerMod1->SetPrintDebug(kFALSE);
  PDFProducerMod1->SetFillHist(kTRUE);
  PDFProducerMod1->SetRunPDF(usePDFProducer);
  PDFProducerMod1->SetPDFName("cteq65.LHgrid");
  //PDFProducerMod1->SetPDFName("cteq6ll.LHpdf");
  PDFProducerMod1->SetIsData(isData);

  //***********************************************************************************8
  //Lepton Selection
  //***********************************************************************************8

  // Object ID and Cleaning Sequence
  MuonIDMod *muonID1 = new MuonIDMod;
  muonID1->SetClassType("Global");
  muonID1->SetIDType("WWMuIdV2");
  muonID1->SetIsoType("CombinedRelativeConeAreaCorrected");
  muonID1->SetCombRelativeIsoCut(0.15);
  muonID1->SetApplyD0Cut(kTRUE);
  muonID1->SetApplyDZCut(kTRUE);
  muonID1->SetWhichVertex(0);

  ElectronIDMod *electronID1 = new ElectronIDMod;
  electronID1->SetApplyEcalFiducial(kTRUE);
  electronID1->SetIDType("VBTFWorkingPoint80Id");
  electronID1->SetIsoType("CombinedRelativeConeAreaCorrected");
  electronID1->SetApplyConversionFilterType1(kFALSE);
  electronID1->SetApplyConversionFilterType2(kTRUE);
  electronID1->SetChargeFilter(kFALSE);
  electronID1->SetApplyD0Cut(kFALSE);
  electronID1->SetApplyDZCut(kFALSE);
  electronID1->SetWhichVertex(0);
  electronID1->SetNExpectedHitsInnerCut(0);

  PhotonIDMod *photonIDMod1 = new PhotonIDMod;
  photonIDMod1->SetIsoType("MITPUCorrected");
  photonIDMod1->SetHadOverEmMax(0.05);
  photonIDMod1->SetApplyPixelSeed(kFALSE);
  photonIDMod1->SetApplyElectronVetoConvRecovery(kTRUE);
  photonIDMod1->SetApplyConversionId(kTRUE);

  PFTauIDMod *pftauIDMod1 = new PFTauIDMod;
  pftauIDMod1->SetPFTausName("HPSTaus");
  pftauIDMod1->SetIsHPSSel(kFALSE);

  //***********************************************************************************8
  //Jet Selection
  //***********************************************************************************8

  const char *jetInput1 = "AKt5PFJets";
  PublisherMod<PFJet,Jet> *pubJet1 = new PublisherMod<PFJet,Jet>("JetPub");
  pubJet1->SetInputName(jetInput1);
  pubJet1->SetOutputName(Form("Pub%s",jetInput1));

  JetCorrectionMod *jetCorr1_ntuple = new JetCorrectionMod;
  jetCorr1_ntuple->AddCorrectionFromFile("MitPhysics/data/START42_V12_AK5PF_L1FastJet.txt"); 
  jetCorr1_ntuple->AddCorrectionFromFile("MitPhysics/data/START42_V12_AK5PF_L2Relative.txt"); 
  jetCorr1_ntuple->AddCorrectionFromFile("MitPhysics/data/START42_V12_AK5PF_L3Absolute.txt");
  if(isData == true){
    jetCorr1_ntuple->AddCorrectionFromFile("MitPhysics/data/START42_V12_AK5PF_L2L3Residual.txt");  
  }
  jetCorr1_ntuple->SetInputName(pubJet1->GetOutputName());
  jetCorr1_ntuple->SetCorrectedName("CorrectedJets_ntuple");

  JetIDMod *theJetID1_ntuple = new JetIDMod;
  theJetID1_ntuple->SetInputName(jetCorr1_ntuple->GetOutputName());
  theJetID1_ntuple->SetPtCut(20.0);
  theJetID1_ntuple->SetEtaMaxCut(etaJetCut);
  theJetID1_ntuple->SetJetEEMFractionMinCut(0.00);
  theJetID1_ntuple->SetOutputName("GoodJets_ntuple");
  theJetID1_ntuple->SetApplyBetaCut(kFALSE);



  //***********************************************************************************8
  //Cleaning
  //***********************************************************************************8
  ElectronCleaningMod *electronCleaning1 = new ElectronCleaningMod;
  PhotonCleaningMod *photonCleaningMod1 = new PhotonCleaningMod;
  PFTauCleaningMod *pftauCleaningMod1 = new PFTauCleaningMod;

  JetCleaningMod *theJetCleaning1_ntuple = new JetCleaningMod;
  theJetCleaning1_ntuple->SetGoodJetsName("GoodJets_ntuple");
  theJetCleaning1_ntuple->SetCleanJetsName("CleanJets_ntuple");
  if (isPhotonControlSample) {
    theJetCleaning1_ntuple->SetApplyPhotonRemoval(kTRUE);
  }

  MergeLeptonsMod *merger1 = new MergeLeptonsMod;
  merger1->SetMuonsName(muonID1->GetOutputName());
  merger1->SetElectronsName(electronCleaning1->GetOutputName());

  JetIDMod *theJetID2_ntuple = new JetIDMod;
  theJetID2_ntuple->SetInputName(jetCorr1_ntuple->GetOutputName());
  theJetID2_ntuple->SetPtCut(0.0);
  theJetID2_ntuple->SetEtaMaxCut(etaJetCut);
  theJetID2_ntuple->SetJetEEMFractionMinCut(0.00);
  theJetID2_ntuple->SetOutputName("GoodJetsNoPtCut_ntuple");
  theJetID2_ntuple->SetApplyBetaCut(kFALSE);

  JetCleaningMod *theJetCleaning2_ntuple = new JetCleaningMod;
  theJetCleaning2_ntuple->SetGoodJetsName("GoodJetsNoPtCut_ntuple");
  theJetCleaning2_ntuple->SetCleanJetsName("CleanJetsNoPtCut_ntuple");
  if (isPhotonControlSample) {
    theJetCleaning2_ntuple->SetApplyPhotonRemoval(kTRUE);
  }

  //***********************************************************************************8
  //MET
  //***********************************************************************************8
  const char *metInput = "TCMet";
  PublisherMod<Met,Met> *pubMet = new PublisherMod<Met,Met>("MetPub");
  pubMet->SetInputName(metInput);
  pubMet->SetOutputName(Form("Pub%s",metInput));

  PublisherMod<CaloMet> *pubCaloMet = new PublisherMod<CaloMet>;
  pubCaloMet->SetName("CaloMetPub");
  pubCaloMet->SetInputName("CorMuonMet");
  pubCaloMet->SetOutputName("pubCaloMet");

  CaloMetCorrectionMod *metCaloCorr = new CaloMetCorrectionMod;
  metCaloCorr->SetInputName(pubCaloMet->GetOutputName());
  metCaloCorr->SetCorrectedJetsName(jetCorr1_ntuple->GetOutputName());
  metCaloCorr->SetOutputName("pubCaloCorrectedMet");

  const char *metPFInput = "PFMet";
  PublisherMod<PFMet,Met> *pubPFMet = new PublisherMod<PFMet,Met>("MetPFPub");
  pubPFMet->SetInputName(metPFInput);
  pubPFMet->SetOutputName(Form("Pub%s",metPFInput));

//   ScaleFactorMod *ScaleFactorMod1 = new ScaleFactorMod;
//   ScaleFactorMod1->SetTrigObjsName("myhltobjs");
//   //ScaleFactorMod1->SetIsData(isData);
//   ScaleFactorMod1->SetIsData(kTRUE);
//   ScaleFactorMod1->SetApplyVertex(kTRUE);

  //***********************************************************************************8
  //Fakeable Objects Definition
  //***********************************************************************************8

  // Lepton ID with loose requirements
  MuonIDMod *muonIDFakeable = new MuonIDMod;
  muonIDFakeable->SetClassType("Global");
  muonIDFakeable->SetIDType("WWMuIdV2");
  muonIDFakeable->SetIsoType("CombinedRelativeConeAreaCorrected");
  muonIDFakeable->SetApplyD0Cut(kTRUE);
  muonIDFakeable->SetApplyDZCut(kTRUE);
  muonIDFakeable->SetD0Cut(0.20);
  muonIDFakeable->SetDZCut(0.10);
  muonIDFakeable->SetCombIsoCut(0.40);
  muonIDFakeable->SetCleanMuonsName("CleanMuonsFakeable");
  muonIDFakeable->SetWhichVertex(0);

  ElectronIDMod *electronIDFakeable = new ElectronIDMod;
  electronIDFakeable->SetIDType("VBTFWorkingPointFakeableId");
  electronIDFakeable->SetIsoType("TrackJura");
  electronIDFakeable->SetTrackIsoCut(0.2);
  electronIDFakeable->SetEcalJurIsoCut(0.2);
  electronIDFakeable->SetHcalIsoCut(0.2);
  electronIDFakeable->SetApplyConversionFilterType1(kFALSE);
  electronIDFakeable->SetApplyConversionFilterType2(kTRUE);
  electronIDFakeable->SetChargeFilter(kFALSE);
  electronIDFakeable->SetApplyD0Cut(kFALSE);
  electronIDFakeable->SetApplyDZCut(kFALSE);
  electronIDFakeable->SetNExpectedHitsInnerCut(0);
  electronIDFakeable->SetGoodElectronsName("GoodElectronsFakeable");
  electronIDFakeable->SetWhichVertex(0);

  ElectronCleaningMod *electronCleaningFakeable = new ElectronCleaningMod;
  electronCleaningFakeable->SetCleanMuonsName(muonIDFakeable->GetOutputName());
  electronCleaningFakeable->SetGoodElectronsName(electronIDFakeable->GetOutputName());
  electronCleaningFakeable->SetCleanElectronsName("CleanElectronsFakeable");

  MergeLeptonsMod *mergerFakeable = new MergeLeptonsMod;
  mergerFakeable->SetMuonsName(muonIDFakeable->GetOutputName());
  mergerFakeable->SetElectronsName(electronCleaningFakeable->GetOutputName());
  mergerFakeable->SetMergedName("MergedLeptonsFakeable");

  //------------------------------------------------------------------------------------------------
  // organize output
  //------------------------------------------------------------------------------------------------
  TString rootFile =  TString("/home/sixie/temphist/") + TString(outputName);
  rootFile += TString("_") + TString(dataset) + TString("_") + TString(skim);
  if (TString(fileset) != TString(""))
    rootFile += TString("_") + TString(fileset);
  rootFile += TString(".root");
  printf("\nRoot output: %s\n\n",rootFile.Data());  


  //***********************************************************************************8
  //Smurf Ntupler
  //***********************************************************************************8
  TString rootFileHwwMake0 = TString("/home/sixie/temphist/");
  rootFileHwwMake0 += TString(outputName);
  rootFileHwwMake0 += TString("_smurf0_") + TString(dataset) + TString("_") + TString(skim); 
  if (TString(fileset) != TString(""))
    rootFileHwwMake0 += TString("_") + TString(fileset);
  rootFileHwwMake0 += TString(".root");
  HwwMakeNtupleMod *HwwMakeNtupleMod0 = new HwwMakeNtupleMod;
  HwwMakeNtupleMod0->SetCleanJetsName("CleanJets_ntuple");
  HwwMakeNtupleMod0->SetCleanJetsNoPtCutName("CleanJetsNoPtCut_ntuple");
  HwwMakeNtupleMod0->SetMetName(pubPFMet->GetOutputName());
  HwwMakeNtupleMod0->SetJetScaleSyst(0.0);
  HwwMakeNtupleMod0->SetPtJetCut(ptJetCut);
  HwwMakeNtupleMod0->SetEtaJetCut(etaJetCut);
  HwwMakeNtupleMod0->SetProcessID(fDecay);
  HwwMakeNtupleMod0->SetIsData(isData);
  HwwMakeNtupleMod0->SetFakeRatePredictionType(0);
  HwwMakeNtupleMod0->SetMuonFakeName("CleanMuonsFakeable");
  HwwMakeNtupleMod0->SetElectronFakeName("CleanElectronsFakeable");
  HwwMakeNtupleMod0->SetLeptonFakeName("MergedLeptonsFakeable");
  HwwMakeNtupleMod0->SetFillNtupleType(0);
  HwwMakeNtupleMod0->SetOutputName(rootFileHwwMake0);


  TString rootFileHwwMake1 = TString("/home/sixie/temphist/");
  rootFileHwwMake1 += TString(outputName);
  rootFileHwwMake1 += TString("_smurf1_") + TString(dataset) + TString("_") + TString(skim); 
  if (TString(fileset) != TString(""))
    rootFileHwwMake1 += TString("_") + TString(fileset);
  rootFileHwwMake1 += TString(".root");
  HwwMakeNtupleMod *HwwMakeNtupleMod1 = new HwwMakeNtupleMod;
  HwwMakeNtupleMod1->SetCleanJetsName("CleanJets_ntuple");
  HwwMakeNtupleMod1->SetCleanJetsNoPtCutName("CleanJetsNoPtCut_ntuple");
  HwwMakeNtupleMod1->SetMetName(pubPFMet->GetOutputName());
  HwwMakeNtupleMod1->SetJetScaleSyst(0.0);
  HwwMakeNtupleMod1->SetPtJetCut(ptJetCut);
  HwwMakeNtupleMod1->SetEtaJetCut(etaJetCut);
  HwwMakeNtupleMod1->SetProcessID(fDecay);
  HwwMakeNtupleMod1->SetIsData(isData);
  HwwMakeNtupleMod1->SetFakeRatePredictionType(1);
  HwwMakeNtupleMod1->SetMuonFakeName("CleanMuonsFakeable");
  HwwMakeNtupleMod1->SetElectronFakeName("CleanElectronsFakeable");
  HwwMakeNtupleMod1->SetLeptonFakeName("MergedLeptonsFakeable");
  HwwMakeNtupleMod1->SetFillNtupleType(1);
  HwwMakeNtupleMod1->SetOutputName(rootFileHwwMake1);


  TString rootFileHwwMake2 = TString("/home/sixie/temphist/");
  rootFileHwwMake2 += TString(outputName);
  rootFileHwwMake2 += TString("_smurf2_") + TString(dataset) + TString("_") + TString(skim); 
  if (TString(fileset) != TString(""))
    rootFileHwwMake2 += TString("_") + TString(fileset);
  rootFileHwwMake2 += TString(".root");
  HwwMakeNtupleMod *HwwMakeNtupleMod2 = new HwwMakeNtupleMod;
  HwwMakeNtupleMod2->SetCleanJetsName("CleanJets_ntuple");
  HwwMakeNtupleMod2->SetCleanJetsNoPtCutName("CleanJetsNoPtCut_ntuple");
  HwwMakeNtupleMod2->SetMetName(pubPFMet->GetOutputName());
  HwwMakeNtupleMod2->SetJetScaleSyst(0.0);
  HwwMakeNtupleMod2->SetPtJetCut(ptJetCut);
  HwwMakeNtupleMod2->SetEtaJetCut(etaJetCut);
  HwwMakeNtupleMod2->SetProcessID(fDecay);
  HwwMakeNtupleMod2->SetIsData(isData);
  HwwMakeNtupleMod2->SetFakeRatePredictionType(2);
  HwwMakeNtupleMod2->SetMuonFakeName("CleanMuonsFakeable");
  HwwMakeNtupleMod2->SetElectronFakeName("CleanElectronsFakeable");
  HwwMakeNtupleMod2->SetLeptonFakeName("MergedLeptonsFakeable");
  HwwMakeNtupleMod2->SetFillNtupleType(2);
  HwwMakeNtupleMod2->SetOutputName(rootFileHwwMake2);


  TString rootFileHwwMakePhoton = TString("/home/sixie/temphist/");
  rootFileHwwMakePhoton += TString(outputName);
  rootFileHwwMakePhoton += TString("_smurfPhoton_") + TString(dataset) + TString("_") + TString(skim); 
  if (TString(fileset) != TString(""))
    rootFileHwwMakePhoton += TString("_") + TString(fileset);
  rootFileHwwMakePhoton += TString(".root");
  HwwMakeNtupleMod *HwwMakeNtupleModPhoton = new HwwMakeNtupleMod;
  HwwMakeNtupleModPhoton->SetCleanJetsName("CleanJets_ntuple");
  HwwMakeNtupleModPhoton->SetCleanJetsNoPtCutName("CleanJetsNoPtCut_ntuple");
  HwwMakeNtupleModPhoton->SetMetName(pubPFMet->GetOutputName());
  HwwMakeNtupleModPhoton->SetJetScaleSyst(0.0);
  HwwMakeNtupleModPhoton->SetPtJetCut(ptJetCut);
  HwwMakeNtupleModPhoton->SetEtaJetCut(etaJetCut);
  HwwMakeNtupleModPhoton->SetProcessID(fDecay);
  HwwMakeNtupleModPhoton->SetIsData(isData);
  HwwMakeNtupleModPhoton->SetFakeRatePredictionType(0);
  HwwMakeNtupleModPhoton->SetFillPhotonTemplate(kTRUE);
  HwwMakeNtupleModPhoton->SetMuonFakeName("CleanMuonsFakeable");
  HwwMakeNtupleModPhoton->SetElectronFakeName("CleanElectronsFakeable");
  HwwMakeNtupleModPhoton->SetLeptonFakeName("MergedLeptonsFakeable");
  HwwMakeNtupleModPhoton->SetFillNtupleType(0);
  HwwMakeNtupleModPhoton->SetOutputName(rootFileHwwMakePhoton);



  //------------------------------------------------------------------------------------------------
  //
  // HWW Ntupler
  //
  //------------------------------------------------------------------------------------------------
  TString rootFileHwwNtuple = TString("/home/sixie/temphist/");
  rootFileHwwNtuple += TString(outputName);
  rootFileHwwNtuple += TString("_HWWNtuple_") + TString(dataset) + TString("_") + TString(skim); 
  rootFileHwwNtuple += TString("_") + TString(fileset);
  rootFileHwwNtuple += TString(".root");

  HwwNtuplerMod *mymod = new HwwNtuplerMod;
  mymod->SetOutputName(rootFileHwwNtuple.Data());          // output ntuple file name
  mymod->SetUseGen(!isData);              // look at generator information (must set to kFALSE if MCParticle collection do not exist)
  mymod->SetSkipIfHLTFail(kFALSE);  // skip to next event if no HLT accept
  mymod->SetFSRMode(kFALSE);
  mymod->SetMuonPtMin(3);
  mymod->SetMuonPtMax(99999999);
  mymod->SetMuonEtaMin(-3);
  mymod->SetMuonEtaMax(3);
  mymod->SetCleanJetsName(theJetCleaning1_ntuple->GetOutputName());
  mymod->SetCleanJetsNoPtCutName(theJetID2_ntuple->GetOutputName());
  mymod->SetJetPtMin(7);
  mymod->SetComputePDFWeights(usePDFProducer);
  mymod->SetPDFName(pdfSetName.c_str());

 //Single Lepton Triggers
  mymod->AddTrigger("HLT_Mu30_v2",                                   kHLT_Mu30, kHLTObject_Mu30 , "hltSingleMu30L3Filtered30" );
  mymod->AddTrigger("HLT_Mu30_v3",                                   kHLT_Mu30, kHLTObject_Mu30 , "hltSingleMu30L3Filtered30" );
  mymod->AddTrigger("HLT_Mu30_v5",                                   kHLT_Mu30, kHLTObject_Mu30 , "hltSingleMu30L3Filtered30" );
  mymod->AddTrigger("HLT_IsoMu17_v5",                                kHLT_IsoMu17, kHLTObject_IsoMu17, "hltSingleMuIsoL3IsoFiltered17" );
  mymod->AddTrigger("HLT_IsoMu17_v6",                                kHLT_IsoMu17, kHLTObject_IsoMu17, "hltSingleMuIsoL3IsoFiltered17" );
  mymod->AddTrigger("HLT_IsoMu17_v7",                                kHLT_IsoMu17, kHLTObject_IsoMu17, "hltSingleMuIsoL3IsoFiltered17" );
  mymod->AddTrigger("HLT_IsoMu17_v8",                                kHLT_IsoMu17, kHLTObject_IsoMu17, "hltSingleMuIsoL3IsoFiltered17" );
  mymod->AddTrigger("HLT_IsoMu17_v9",                                kHLT_IsoMu17, kHLTObject_IsoMu17, "hltSingleMuIsoL3IsoFiltered17" );
  mymod->AddTrigger("HLT_IsoMu17_v11",                               kHLT_IsoMu17, kHLTObject_IsoMu17, "hltSingleMuIsoL3IsoFiltered17" );
  //Backups
  mymod->AddTrigger("HLT_Mu24_v1",                                   kHLT_Mu24, kHLTObject_Mu24 , "hltSingleMu24L3Filtered24" );
  mymod->AddTrigger("HLT_Mu24_v2",                                   kHLT_Mu24, kHLTObject_Mu24 , "hltSingleMu24L3Filtered24" );
  mymod->AddTrigger("HLT_Mu24_v3",                                   kHLT_Mu24, kHLTObject_Mu24 , "hltSingleMu24L3Filtered24" );
  mymod->AddTrigger("HLT_Mu30_v1",                                   kHLT_Mu30, kHLTObject_Mu30 , "hltSingleMu30L3Filtered30" );
  mymod->AddTrigger("HLT_IsoMu12_v1",                                kHLT_IsoMu12, kHLTObject_IsoMu12, "hltSingleMuIsoL3IsoFiltered12" );
  mymod->AddTrigger("HLT_IsoMu12_v2",                                kHLT_IsoMu12, kHLTObject_IsoMu12, "hltSingleMuIsoL3IsoFiltered12" );
  mymod->AddTrigger("HLT_IsoMu12_v3",                                kHLT_IsoMu12, kHLTObject_IsoMu12, "hltSingleMuIsoL3IsoFiltered12" );
  mymod->AddTrigger("HLT_IsoMu12_v4",                                kHLT_IsoMu12, kHLTObject_IsoMu12, "hltSingleMuIsoL3IsoFiltered12" );
  mymod->AddTrigger("HLT_IsoMu12_v5",                                kHLT_IsoMu12, kHLTObject_IsoMu12, "hltSingleMuIsoL3IsoFiltered12" );
  mymod->AddTrigger("HLT_IsoMu24_v1",                                kHLT_IsoMu24, kHLTObject_IsoMu24, "hltSingleMuIsoL3IsoFiltered24" );
  mymod->AddTrigger("HLT_IsoMu24_v2",                                kHLT_IsoMu24, kHLTObject_IsoMu24, "hltSingleMuIsoL3IsoFiltered24" );
  mymod->AddTrigger("HLT_IsoMu24_v3",                                kHLT_IsoMu24, kHLTObject_IsoMu24, "hltSingleMuIsoL3IsoFiltered24" );
  mymod->AddTrigger("HLT_IsoMu24_v4",                                kHLT_IsoMu24, kHLTObject_IsoMu24, "hltSingleMuIsoL3IsoFiltered24" );
  mymod->AddTrigger("HLT_IsoMu24_v5",                                kHLT_IsoMu24, kHLTObject_IsoMu24, "hltSingleMuIsoL3IsoFiltered24" );
  mymod->AddTrigger("HLT_IsoMu30_v1",                                kHLT_IsoMu30, kHLTObject_IsoMu30, "hltSingleMuIsoL3IsoFiltered30" );
  mymod->AddTrigger("HLT_IsoMu30_v2",                                kHLT_IsoMu30, kHLTObject_IsoMu30, "hltSingleMuIsoL3IsoFiltered30" );
  mymod->AddTrigger("HLT_IsoMu30_v3",                                kHLT_IsoMu30, kHLTObject_IsoMu30, "hltSingleMuIsoL3IsoFiltered30" );
  mymod->AddTrigger("HLT_IsoMu30_v4",                                kHLT_IsoMu30, kHLTObject_IsoMu30, "hltSingleMuIsoL3IsoFiltered30" );
  mymod->AddTrigger("HLT_IsoMu30_v5",                                kHLT_IsoMu30, kHLTObject_IsoMu30, "hltSingleMuIsoL3IsoFiltered30" );



  mymod->AddTrigger("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1", kHLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, kHLTObject_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, "hltEle27CaloIdTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter");
  mymod->AddTrigger("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2", kHLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, kHLTObject_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, "hltEle27CaloIdTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter");
  mymod->AddTrigger("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3", kHLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, kHLTObject_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, "hltEle27CaloIdTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter");
  mymod->AddTrigger("HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1", kHLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, kHLTObject_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, "hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter");
  mymod->AddTrigger("HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2", kHLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, kHLTObject_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, "hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter");
  mymod->AddTrigger("HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3", kHLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, kHLTObject_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, "hltEle32CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter");
  mymod->AddTrigger("HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v4", kHLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, kHLTObject_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, "hltEle32CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter");
  mymod->AddTrigger("HLT_Ele52_CaloIdVT_TrkIdT_v3",                  kHLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT, kHLTObject_Ele52_CaloIdVT_TrkIdT, "hltEle52CaloIdVTTrkIdTDphiFilter");


  //Main Dielectron Triggers
  mymod->AddTrigger("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v1", kHLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL, kHLTObject_Ele17_CaloIdL_CaloIsoVL, "hltEle17CaloIdLCaloIsoVLPixelMatchFilter",kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle17CaloIdIsoEle8CaloIdIsoPixelMatchDoubleFilter" );
  mymod->AddTrigger("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v2", kHLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL, kHLTObject_Ele17_CaloIdL_CaloIsoVL, "hltEle17CaloIdLCaloIsoVLPixelMatchFilter",kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle17CaloIdIsoEle8CaloIdIsoPixelMatchDoubleFilter"); 
  mymod->AddTrigger("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v3", kHLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL, kHLTObject_Ele17_CaloIdL_CaloIsoVL, "hltEle17CaloIdLCaloIsoVLPixelMatchFilter",kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle17CaloIdIsoEle8CaloIdIsoPixelMatchDoubleFilter"); 
  mymod->AddTrigger("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v4", kHLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL, kHLTObject_Ele17_CaloIdL_CaloIsoVL, "hltEle17CaloIdLCaloIsoVLPixelMatchFilter",kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle17CaloIdIsoEle8CaloIdIsoPixelMatchDoubleFilter"); 
  mymod->AddTrigger("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v5", kHLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL, kHLTObject_Ele17_CaloIdL_CaloIsoVL, "hltEle17CaloIdLCaloIsoVLPixelMatchFilter",kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle17CaloIdIsoEle8CaloIdIsoPixelMatchDoubleFilter"); 
  mymod->AddTrigger("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v6", kHLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL, kHLTObject_Ele17_CaloIdL_CaloIsoVL, "hltEle17CaloIdLCaloIsoVLPixelMatchFilter",kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle17CaloIdIsoEle8CaloIdIsoPixelMatchDoubleFilter"); 
  mymod->AddTrigger("HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v1", kHLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL, kHLTObject_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL , "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolFilter",kHLTObject_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL, "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolDoubleFilter" );
  mymod->AddTrigger("HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v2", kHLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL, kHLTObject_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL , "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolFilter",kHLTObject_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL, "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolDoubleFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v3", kHLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL, kHLTObject_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL , "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolFilter",kHLTObject_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL, "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolDoubleFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v4", kHLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL, kHLTObject_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL , "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolFilter",kHLTObject_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL, "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolDoubleFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v5", kHLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL, kHLTObject_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL , "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolFilter",kHLTObject_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL, "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolDoubleFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v6", kHLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL, kHLTObject_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL , "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolFilter",kHLTObject_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL, "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolDoubleFilter");



  //Main Dimuon Triggers
  mymod->AddTrigger("HLT_DoubleMu7_v1",                          kHLT_DoubleMu7, kHLTObject_Mu7, "hltDiMuonL3PreFiltered7" );
  mymod->AddTrigger("HLT_DoubleMu7_v2",                          kHLT_DoubleMu7, kHLTObject_Mu7, "hltDiMuonL3PreFiltered7" );
  mymod->AddTrigger("HLT_DoubleMu7_v3",                          kHLT_DoubleMu7, kHLTObject_Mu7, "hltDiMuonL3PreFiltered7" );
  mymod->AddTrigger("HLT_Mu13_Mu8_v1",                           kHLT_Mu17_Mu8,  kHLTObject_Mu13, "hltSingleMu13L3Filtered13", kHLTObject_Mu8, "hltDiMuonL3PreFiltered8");
  mymod->AddTrigger("HLT_Mu13_Mu8_v2",                           kHLT_Mu17_Mu8, kHLTObject_Mu13, "hltSingleMu13L3Filtered13", kHLTObject_Mu8, "hltDiMuonL3PreFiltered8");
  mymod->AddTrigger("HLT_Mu13_Mu8_v4",                           kHLT_Mu17_Mu8, kHLTObject_Mu13, "hltSingleMu13L3Filtered13", kHLTObject_Mu8, "hltDiMuonL3PreFiltered8");
  mymod->AddTrigger("HLT_Mu17_Mu8_v1",                           kHLT_Mu17_Mu8, kHLTObject_Mu17, "hltSingleMu13L3Filtered17", kHLTObject_Mu8, "hltDiMuonL3PreFiltered8");
  mymod->AddTrigger("HLT_Mu17_Mu8_v2",                           kHLT_Mu17_Mu8, kHLTObject_Mu17, "hltSingleMu13L3Filtered17", kHLTObject_Mu8, "hltDiMuonL3PreFiltered8");
  mymod->AddTrigger("HLT_Mu17_Mu8_v4",                           kHLT_Mu17_Mu8, kHLTObject_Mu17, "hltSingleMu13L3Filtered17", kHLTObject_Mu8, "hltDiMuonL3PreFiltered8");
  //Note: This guy was put in with new menu in run 166346, but that menu was immediately switched off due to problems.
//   mymod->AddTrigger("HLT_Mu13_Mu8_v3",                           kHLT_Mu17_Mu8, kHLTObject_Mu13, "hltSingleMu13L3Filtered13", kHLTObject_Mu8, "hltDiMuonL3PreFiltered8");
//   mymod->AddTrigger("HLT_Mu17_Mu8_v3",                           kHLT_Mu17_Mu8, kHLTObject_Mu17, "hltSingleMu13L3Filtered17", kHLTObject_Mu8, "hltDiMuonL3PreFiltered8");
  


  //Main EMu Triggers
  mymod->AddTrigger("HLT_Mu17_Ele8_CaloIdL_v1",               kHLT_Mu17_Ele8_CaloIdL, kHLTObject_Mu17, "hltL1Mu3EG5L3Filtered17", kHLTObject_Ele8_CaloIdL, "hltL1NonIsoHLTNonIsoMu17Ele8PixelMatchFilter"  );
  mymod->AddTrigger("HLT_Mu17_Ele8_CaloIdL_v2",               kHLT_Mu17_Ele8_CaloIdL, kHLTObject_Mu17, "hltL1Mu3EG5L3Filtered17", kHLTObject_Ele8_CaloIdL, "hltL1NonIsoHLTNonIsoMu17Ele8PixelMatchFilter");
  mymod->AddTrigger("HLT_Mu17_Ele8_CaloIdL_v3",               kHLT_Mu17_Ele8_CaloIdL, kHLTObject_Mu17, "hltL1MuOpenEG5L3Filtered17", kHLTObject_Ele8_CaloIdL, "hltL1NonIsoHLTNonIsoMu17Ele8PixelMatchFilter");
  mymod->AddTrigger("HLT_Mu17_Ele8_CaloIdL_v4",               kHLT_Mu17_Ele8_CaloIdL, kHLTObject_Mu17, "hltL1MuOpenEG5L3Filtered17", kHLTObject_Ele8_CaloIdL, "hltL1NonIsoHLTNonIsoMu17Ele8PixelMatchFilter");
  mymod->AddTrigger("HLT_Mu17_Ele8_CaloIdL_v5",               kHLT_Mu17_Ele8_CaloIdL, kHLTObject_Mu17, "hltL1MuOpenEG5L3Filtered17", kHLTObject_Ele8_CaloIdL, "hltL1NonIsoHLTNonIsoMu17Ele8PixelMatchFilter");
  mymod->AddTrigger("HLT_Mu17_Ele8_CaloIdL_v6",               kHLT_Mu17_Ele8_CaloIdL, kHLTObject_Mu17, "hltL1MuOpenEG5L3Filtered17", kHLTObject_Ele8_CaloIdL, "hltL1NonIsoHLTNonIsoMu17Ele8PixelMatchFilter");
  mymod->AddTrigger("HLT_Mu8_Ele17_CaloIdL_v1",               kHLT_Mu8_Ele17_CaloIdL,  kHLTObject_Mu8, "hltL1Mu3EG5L3Filtered8",kHLTObject_Ele17_CaloIdL, "hltL1NonIsoHLTNonIsoMu8Ele17PixelMatchFilter" );
  mymod->AddTrigger("HLT_Mu8_Ele17_CaloIdL_v2",               kHLT_Mu8_Ele17_CaloIdL,  kHLTObject_Mu8, "hltL1Mu3EG5L3Filtered8",kHLTObject_Ele17_CaloIdL, "hltL1NonIsoHLTNonIsoMu8Ele17PixelMatchFilter");
  mymod->AddTrigger("HLT_Mu8_Ele17_CaloIdL_v3",               kHLT_Mu8_Ele17_CaloIdL,  kHLTObject_Mu8, "hltL1MuOpenEG5L3Filtered8",kHLTObject_Ele17_CaloIdL, "hltL1NonIsoHLTNonIsoMu8Ele17PixelMatchFilter");
  mymod->AddTrigger("HLT_Mu8_Ele17_CaloIdL_v4",               kHLT_Mu8_Ele17_CaloIdL,  kHLTObject_Mu8, "hltL1MuOpenEG5L3Filtered8",kHLTObject_Ele17_CaloIdL, "hltL1NonIsoHLTNonIsoMu8Ele17PixelMatchFilter");
  mymod->AddTrigger("HLT_Mu8_Ele17_CaloIdL_v5",               kHLT_Mu8_Ele17_CaloIdL,  kHLTObject_Mu8, "hltL1MuOpenEG5L3Filtered8",kHLTObject_Ele17_CaloIdL, "hltL1NonIsoHLTNonIsoMu8Ele17PixelMatchFilter");
  mymod->AddTrigger("HLT_Mu8_Ele17_CaloIdL_v6",               kHLT_Mu8_Ele17_CaloIdL,  kHLTObject_Mu8, "hltL1MuOpenEG5L3Filtered8",kHLTObject_Ele17_CaloIdL, "hltL1NonIsoHLTNonIsoMu8Ele17PixelMatchFilter");



  //T&P triggers
  mymod->AddTrigger("HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v1",                   kHLT_Ele32_CaloIdL_CaloIsoVL_SC17, kHLTObject_Ele32_CaloIdL_CaloIsoVL, "hltEle32CaloIdLCaloIsoVLSC17PixelMatchFilter", kHLTObject_SC17, "hltEle32CaloIdLCaloIsoVLSC17HEDoubleFilter"   );
  mymod->AddTrigger("HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v2",                   kHLT_Ele32_CaloIdL_CaloIsoVL_SC17, kHLTObject_Ele32_CaloIdL_CaloIsoVL, "hltEle32CaloIdLCaloIsoVLSC17PixelMatchFilter", kHLTObject_SC17, "hltEle32CaloIdLCaloIsoVLSC17HEDoubleFilter" );
  mymod->AddTrigger("HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v3",                   kHLT_Ele32_CaloIdL_CaloIsoVL_SC17, kHLTObject_Ele32_CaloIdL_CaloIsoVL, "hltEle32CaloIdLCaloIsoVLSC17PixelMatchFilter", kHLTObject_SC17, "hltEle32CaloIdLCaloIsoVLSC17HEDoubleFilter" );
  mymod->AddTrigger("HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_v1",     kHLT_Ele32_CaloIdL_CaloIsoVL_SC17, kHLTObject_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT, "hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17PixelMatchFilter", kHLTObject_SC17, "hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17TrackIsolFilter" );
  mymod->AddTrigger("HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_v2",     kHLT_Ele32_CaloIdL_CaloIsoVL_SC17, kHLTObject_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT, "hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17PixelMatchFilter", kHLTObject_SC17, "hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17TrackIsolFilter" );
  mymod->AddTrigger("HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_v3",     kHLT_Ele32_CaloIdL_CaloIsoVL_SC17, kHLTObject_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT, "hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17PixelMatchFilter", kHLTObject_SC17, "hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17TrackIsolFilter" );
  mymod->AddTrigger("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v1", kHLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30, kHLTObject_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT , "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8TrackIsolFilter", kHLTObject_SC8, "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8PMMassFilter" );
  mymod->AddTrigger("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v2", kHLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30, kHLTObject_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT , "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8TrackIsolFilter", kHLTObject_SC8, "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8PMMassFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v3", kHLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30, kHLTObject_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT , "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8TrackIsolFilter", kHLTObject_SC8, "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8PMMassFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v4", kHLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30, kHLTObject_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT , "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8TrackIsolFilter", kHLTObject_SC8, "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8PMMassFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v5", kHLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30, kHLTObject_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT , "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8TrackIsolFilter", kHLTObject_SC8, "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8PMMassFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v6", kHLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30, kHLTObject_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT , "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8TrackIsolFilter", kHLTObject_SC8, "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8PMMassFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass30_v2", kHLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30, kHLTObject_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT , "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8TrackIsolFilter", kHLTObject_Ele8, "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8PMMassFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass30_v3", kHLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30, kHLTObject_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT , "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8TrackIsolFilter", kHLTObject_Ele8, "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8PMMassFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass30_v4", kHLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30, kHLTObject_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT , "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8TrackIsolFilter", kHLTObject_Ele8, "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8PMMassFilter");



  //Other triggers
//   mymod->AddTrigger("HLT_Photon26_CaloIdL_IsoVL_Photon18_v1",    kHLT_Photon26_CaloIdL_IsoVL_Photon18, kHLTObject_Photon26_CaloIdL_IsoVL, "hltEG26CaloIdLIsoVLTrackIsoFilter");
//   mymod->AddTrigger("HLT_Photon26_CaloIdL_IsoVL_Photon18_v2",    kHLT_Photon26_CaloIdL_IsoVL_Photon18, kHLTObject_Photon26_CaloIdL_IsoVL, "hltEG26CaloIdLIsoVLTrackIsoFilter");
//   mymod->AddTrigger("HLT_Photon26_CaloIdL_IsoVL_Photon18_v3",    kHLT_Photon26_CaloIdL_IsoVL_Photon18, kHLTObject_Photon26_CaloIdL_IsoVL, "hltEG26CaloIdLIsoVLTrackIsoFilter");
//   mymod->AddTrigger("HLT_Photon26_IsoVL_Photon18_v1",            kHLT_Photon26_IsoVL_Photon18);
//   mymod->AddTrigger("HLT_Photon26_IsoVL_Photon18_IsoVL_v1",                 kHLT_Photon26_IsoVL_Photon18_IsoVL );
//   mymod->AddTrigger("HLT_Photon26_CaloIdL_IsoVL_Photon18_CaloIdL_IsoVL_v1",  kHLT_Photon26_CaloIdL_IsoVL_Photon18_CaloIdL_IsoVL, kHLTObject_Photon26_CaloIdL_IsoVL, "hltEG26CaloIdLIsoVLHcalIsoLastFilter");
//   mymod->AddTrigger("HLT_Photon26_CaloIdL_IsoVL_Photon18_CaloIdL_IsoVL_v2",  kHLT_Photon26_CaloIdL_IsoVL_Photon18_CaloIdL_IsoVL, kHLTObject_Photon26_CaloIdL_IsoVL, "hltEG26CaloIdLIsoVLHcalIsoLastFilter");
//   mymod->AddTrigger("HLT_Photon26_CaloIdL_IsoVL_Photon18_CaloIdL_IsoVL_v3",  kHLT_Photon26_CaloIdL_IsoVL_Photon18_CaloIdL_IsoVL, kHLTObject_Photon26_CaloIdL_IsoVL, "hltEG26CaloIdLIsoVLHcalIsoLastFilter");
//   mymod->AddTrigger("HLT_TripleEle10_CaloIdL_TrkIdVL_v1",                    kHLT_TripleEle10_CaloIdL_TrkIdVL);
//   mymod->AddTrigger("HLT_DoubleEle10_CaloIdL_TrkIdVL_Ele10_v1",              kHLT_DoubleEle10_CaloIdL_TrkIdVL_Ele10);
//   mymod->AddTrigger("HLT_DoubleMu5_Ele8_v2",                                 kHLT_DoubleMu5_Ele8);
//   mymod->AddTrigger("HLT_Mu5_DoubleEle8_v2",                                 kHLT_Mu5_DoubleEle8);
//   mymod->AddTrigger("HLT_TripleMu5_v2",                                      kHLT_TripleMu5);

  //Fake Rate triggers
  mymod->AddTrigger("HLT_Mu8_v1",                                kHLT_Mu8, kHLTObject_Mu8, "hltSingleMu8L3Filtered8");
  mymod->AddTrigger("HLT_Mu8_v2",                                kHLT_Mu8, kHLTObject_Mu8, "hltSingleMu8L3Filtered8");
  mymod->AddTrigger("HLT_Mu8_v3",                                kHLT_Mu8, kHLTObject_Mu8, "hltSingleMu8L3Filtered8");
  mymod->AddTrigger("HLT_Mu8_v5",                                kHLT_Mu8, kHLTObject_Mu8, "hltSingleMu8L3Filtered8");
  mymod->AddTrigger("HLT_Mu15_v1",                               kHLT_Mu15, kHLTObject_Mu15 ,  "hltL3Muon15");
  mymod->AddTrigger("HLT_Mu15_v2",                               kHLT_Mu15, kHLTObject_Mu15 , "hltL3Muon15");
  mymod->AddTrigger("HLT_Mu15_v3",                               kHLT_Mu15, kHLTObject_Mu15 , "hltSingleMu15L3Filtered15");
  mymod->AddTrigger("HLT_Mu15_v4",                               kHLT_Mu15, kHLTObject_Mu15 , "hltSingleMu15L3Filtered15");
  mymod->AddTrigger("HLT_Mu15_v6",                               kHLT_Mu15, kHLTObject_Mu15 , "hltSingleMu15L3Filtered15");
  mymod->AddTrigger("HLT_Mu8_Jet40_v1",                          kHLT_Mu8_Jet40,kHLTObject_Mu8,"hltL3Mu8Jet20L3Filtered8" );
  mymod->AddTrigger("HLT_Mu8_Jet40_v2",                          kHLT_Mu8_Jet40,kHLTObject_Mu8,"hltL3Mu8Jet20L3Filtered8");
  mymod->AddTrigger("HLT_Mu8_Jet40_v3",                          kHLT_Mu8_Jet40,kHLTObject_Mu8,"hltL3Mu8Jet20L3Filtered8");
  mymod->AddTrigger("HLT_Mu8_Jet40_v4",                          kHLT_Mu8_Jet40,kHLTObject_Mu8,"hltL3Mu8Jet20L3Filtered8");
  mymod->AddTrigger("HLT_Mu8_Jet40_v5",                          kHLT_Mu8_Jet40,kHLTObject_Mu8,"hltL3Mu8Jet20L3Filtered8");
  mymod->AddTrigger("HLT_Mu8_Jet40_v6",                          kHLT_Mu8_Jet40,kHLTObject_Mu8,"hltL3Mu8Jet20L3Filtered8");
  mymod->AddTrigger("HLT_Mu8_Jet40_v7",                          kHLT_Mu8_Jet40,kHLTObject_Mu8,"hltL3Mu8Jet20L3Filtered8");
  mymod->AddTrigger("HLT_Mu8_Photon20_CaloIdVT_IsoT_v2",         kHLT_Mu8_Photon20_CaloIdVT_IsoT ,kHLTObject_Photon20_CaloIdVT_IsoT,"hltPhoton20CaloIdVTIsoTTrackIsoFilter",kHLT_Mu8,"hltSingleMu8EG5L3Filtered8");
  mymod->AddTrigger("HLT_Mu8_Photon20_CaloIdVT_IsoT_v3",         kHLT_Mu8_Photon20_CaloIdVT_IsoT ,kHLTObject_Photon20_CaloIdVT_IsoT,"hltPhoton20CaloIdVTIsoTMu8TrackIsoFilter",kHLT_Mu8,"hltSingleMu8EG5L3Filtered8");
  mymod->AddTrigger("HLT_Mu8_Photon20_CaloIdVT_IsoT_v3",         kHLT_Mu8_Photon20_CaloIdVT_IsoT ,kHLTObject_Photon20_CaloIdVT_IsoT,"hltPhoton20CaloIdVTIsoTMu8TrackIsoFilter",kHLT_Mu8,"hltSingleMu8EG5L3Filtered8");
  mymod->AddTrigger("HLT_Mu8_Photon20_CaloIdVT_IsoT_v4",         kHLT_Mu8_Photon20_CaloIdVT_IsoT ,kHLTObject_Photon20_CaloIdVT_IsoT,"hltPhoton20CaloIdVTIsoTMu8TrackIsoFilter",kHLT_Mu8,"hltSingleMu8EG5L3Filtered8");
  mymod->AddTrigger("HLT_Mu8_Photon20_CaloIdVT_IsoT_v5",         kHLT_Mu8_Photon20_CaloIdVT_IsoT ,kHLTObject_Photon20_CaloIdVT_IsoT,"hltPhoton20CaloIdVTIsoTMu8TrackIsoFilter",kHLT_Mu8,"hltSingleMu8EG5L3Filtered8");
  mymod->AddTrigger("HLT_Mu8_Photon20_CaloIdVT_IsoT_v6",         kHLT_Mu8_Photon20_CaloIdVT_IsoT ,kHLTObject_Photon20_CaloIdVT_IsoT,"hltPhoton20CaloIdVTIsoTMu8TrackIsoFilter",kHLT_Mu8,"hltSingleMu8EG5L3Filtered8");
  mymod->AddTrigger("HLT_Ele8_v1",                               kHLT_Ele8,kHLTObject_Ele8 , "hltEle8PixelMatchFilter" );
  mymod->AddTrigger("HLT_Ele8_v2",                               kHLT_Ele8,kHLTObject_Ele8 , "hltEle8PixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_v3",                               kHLT_Ele8,kHLTObject_Ele8 , "hltEle8PixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_v4",                               kHLT_Ele8,kHLTObject_Ele8 , "hltEle8PixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_v5",                               kHLT_Ele8,kHLTObject_Ele8 , "hltEle8PixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_v6",                               kHLT_Ele8,kHLTObject_Ele8 , "hltEle8PixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_CaloIdL_CaloIsoVL_v1",             kHLT_Ele8_CaloIdL_CaloIsoVL, kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle8CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_CaloIdL_CaloIsoVL_v2",             kHLT_Ele8_CaloIdL_CaloIsoVL, kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle8CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_CaloIdL_CaloIsoVL_v3",             kHLT_Ele8_CaloIdL_CaloIsoVL, kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle8CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_CaloIdL_CaloIsoVL_v4",             kHLT_Ele8_CaloIdL_CaloIsoVL, kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle8CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_CaloIdL_CaloIsoVL_v5",             kHLT_Ele8_CaloIdL_CaloIsoVL, kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle8CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_CaloIdL_CaloIsoVL_v6",             kHLT_Ele8_CaloIdL_CaloIsoVL, kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle8CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v3", kHLT_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL, kHLTObject_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL, "hltEle8TightIdLooseIsoTrackIsolFilter");
  mymod->AddTrigger("HLT_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v4", kHLT_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL, kHLTObject_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL, "hltEle8TightIdLooseIsoTrackIsolFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdL_CaloIsoVL_v1",            kHLT_Ele17_CaloIdL_CaloIsoVL, kHLTObject_Ele17_CaloIdL_CaloIsoVL, "hltEle17CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdL_CaloIsoVL_v2",            kHLT_Ele17_CaloIdL_CaloIsoVL, kHLTObject_Ele17_CaloIdL_CaloIsoVL, "hltEle17CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdL_CaloIsoVL_v3",            kHLT_Ele17_CaloIdL_CaloIsoVL, kHLTObject_Ele17_CaloIdL_CaloIsoVL, "hltEle17CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdL_CaloIsoVL_v4",            kHLT_Ele17_CaloIdL_CaloIsoVL, kHLTObject_Ele17_CaloIdL_CaloIsoVL, "hltEle17CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdL_CaloIsoVL_v5",            kHLT_Ele17_CaloIdL_CaloIsoVL, kHLTObject_Ele17_CaloIdL_CaloIsoVL, "hltEle17CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele17_CaloIdL_CaloIsoVL_v6",            kHLT_Ele17_CaloIdL_CaloIsoVL, kHLTObject_Ele17_CaloIdL_CaloIsoVL, "hltEle17CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_CaloIdL_CaloIsoVL_Jet40_v1",       kHLT_Ele8_CaloIdL_CaloIsoVL_Jet40, kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle8CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_CaloIdL_CaloIsoVL_Jet40_v2",       kHLT_Ele8_CaloIdL_CaloIsoVL_Jet40, kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle8CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_CaloIdL_CaloIsoVL_Jet40_v3",       kHLT_Ele8_CaloIdL_CaloIsoVL_Jet40, kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle8CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_CaloIdL_CaloIsoVL_Jet40_v4",       kHLT_Ele8_CaloIdL_CaloIsoVL_Jet40, kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle8CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_CaloIdL_CaloIsoVL_Jet40_v5",       kHLT_Ele8_CaloIdL_CaloIsoVL_Jet40, kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle8CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Ele8_CaloIdL_CaloIsoVL_Jet40_v6",       kHLT_Ele8_CaloIdL_CaloIsoVL_Jet40, kHLTObject_Ele8_CaloIdL_CaloIsoVL, "hltEle8CaloIdLCaloIsoVLPixelMatchFilter");
  mymod->AddTrigger("HLT_Photon20_CaloIdVT_IsoT_Ele8_CaloIdL_CaloIsoVL_v1", kHLT_Photon20_CaloIdVT_IsoT_Ele8_CaloIdL_CaloIsoVL , kHLTObject_Photon20_CaloIdVT_IsoT , "hltPhoton20CaloIdVTIsoTTrackIsoFilter", kHLTObject_Ele8_CaloIdL_CaloIsoVL,"hltEle8CaloIdLCaloIsoVLNoL1SeedPixelMatchFilter" );
  mymod->AddTrigger("HLT_Photon20_CaloIdVT_IsoT_Ele8_CaloIdL_CaloIsoVL_v2", kHLT_Photon20_CaloIdVT_IsoT_Ele8_CaloIdL_CaloIsoVL , kHLTObject_Photon20_CaloIdVT_IsoT, "hltPhoton20CaloIdVTIsoTTrackIsoFilter", kHLTObject_Ele8_CaloIdL_CaloIsoVL,"hltEle8CaloIdLCaloIsoVLNoL1SeedPixelMatchFilter" );
  mymod->AddTrigger("HLT_Photon20_CaloIdVT_IsoT_Ele8_CaloIdL_CaloIsoVL_v3", kHLT_Photon20_CaloIdVT_IsoT_Ele8_CaloIdL_CaloIsoVL , kHLTObject_Photon20_CaloIdVT_IsoT, "hltPhoton20CaloIdVTIsoTTrackIsoFilter", kHLTObject_Ele8_CaloIdL_CaloIsoVL,"hltEle8CaloIdLCaloIsoVLNoL1SeedPixelMatchFilter" );
  mymod->AddTrigger("HLT_Photon20_CaloIdVT_IsoT_Ele8_CaloIdL_CaloIsoVL_v4", kHLT_Photon20_CaloIdVT_IsoT_Ele8_CaloIdL_CaloIsoVL , kHLTObject_Photon20_CaloIdVT_IsoT, "hltPhoton20CaloIdVTIsoTTrackIsoFilter", kHLTObject_Ele8_CaloIdL_CaloIsoVL,"hltEle8CaloIdLCaloIsoVLNoL1SeedPixelMatchFilter" );
  mymod->AddTrigger("HLT_Photon20_CaloIdVT_IsoT_Ele8_CaloIdL_CaloIsoVL_v5", kHLT_Photon20_CaloIdVT_IsoT_Ele8_CaloIdL_CaloIsoVL , kHLTObject_Photon20_CaloIdVT_IsoT, "hltPhoton20CaloIdVTIsoTTrackIsoFilter", kHLTObject_Ele8_CaloIdL_CaloIsoVL,"hltEle8CaloIdLCaloIsoVLNoL1SeedPixelMatchFilter" );
  mymod->AddTrigger("HLT_Photon20_CaloIdVT_IsoT_Ele8_CaloIdL_CaloIsoVL_v6", kHLT_Photon20_CaloIdVT_IsoT_Ele8_CaloIdL_CaloIsoVL , kHLTObject_Photon20_CaloIdVT_IsoT, "hltPhoton20CaloIdVTIsoTTrackIsoFilter", kHLTObject_Ele8_CaloIdL_CaloIsoVL,"hltEle8CaloIdLCaloIsoVLNoL1SeedPixelMatchFilter" );



//   mymod->AddTrigger("HLT_Jet30_v1", kHLT_Jet30 , kHLTObject_Jet, "hltSingleJet30");
//   mymod->AddTrigger("HLT_Jet30_v2", kHLT_Jet30 , kHLTObject_Jet, "hltSingleJet30");
//   mymod->AddTrigger("HLT_Jet60_v1", kHLT_Jet60 , kHLTObject_Jet, "hltSingleJet60Regional");
//   mymod->AddTrigger("HLT_Jet60_v2", kHLT_Jet60 , kHLTObject_Jet, "hltSingleJet60Regional");
//   mymod->AddTrigger("HLT_Jet110_v1", kHLT_Jet110 , kHLTObject_Jet, "hltSingleJet110Regional");
//   mymod->AddTrigger("HLT_Jet110_v2", kHLT_Jet110 , kHLTObject_Jet, "hltSingleJet110Regional");


  //Add L1SeedModules
  mymod->AddL1SeedModule("hltL1sL1SingleEG20",    kL1_SingleEG20 );
  mymod->AddL1SeedModule("hltL1sL1SingleEG12",    kL1_SingleEG12 );
  mymod->AddL1SeedModule("hltL1sL1SingleEG5",    kL1_SingleEG5 );
  mymod->AddL1SeedModule("hltL1sL1SingleMu3",    kL1_SingleMu3 );



  //Add L1Triggers
  mymod->AddL1Trigger("L1_SingleEG5",    kL1_SingleEG5 );
  mymod->AddL1Trigger("L1_SingleEG12", kL1_SingleEG12  );                  
  mymod->AddL1Trigger("L1_SingleEG20", kL1_SingleEG20  );                  
  mymod->AddL1Trigger("L1_SingleEG30", kL1_SingleEG30  );                  
  mymod->AddL1Trigger("L1_SingleMu3", kL1_SingleMu3 );                     
  mymod->AddL1Trigger("L1_SingleMu7 ", kL1_SingleMu7 );                    
  mymod->AddL1Trigger("L1_SingleMu10",kL1_SingleMu10 );                    
  mymod->AddL1Trigger("L1_SingleMu12", kL1_SingleMu12 );                   
  mymod->AddL1Trigger("L1_SingleMu20", kL1_SingleMu20 );
  mymod->AddL1Trigger("L1_SingleMu25", kL1_SingleMu25 );
  mymod->AddL1Trigger("L1_MuOpen_EG12", kL1_MuOpen_EG12 );                 
  mymod->AddL1Trigger("L1_MuOpen_EG5", kL1_MuOpen_EG5 );                   
  mymod->AddL1Trigger("L1_Mu3_EG5",   kL1_Mu3_EG5  );                    
  mymod->AddL1Trigger("L1_Mu5_EG12",   kL1_Mu5_EG12  );                    
  mymod->AddL1Trigger("L1_Mu7_EG5",      kL1_Mu7_EG5 );                    
  mymod->AddL1Trigger("L1_Mu12_EG5",  kL1_Mu12_EG5 );                      
  mymod->AddL1Trigger("L1_DoubleMu0", kL1_DoubleMu0 );                     
  mymod->AddL1Trigger("L1_DoubleMu5",  kL1_DoubleMu5 );                    
  mymod->AddL1Trigger("L1_DoubleEG3", kL1_DoubleEG3  );                    
  mymod->AddL1Trigger("L1_DoubleEG5", kL1_DoubleEG5  );                    
  mymod->AddL1Trigger("L1_DoubleEG_12_5", kL1_DoubleEG_12_5  );            
  mymod->AddL1Trigger("L1_Mu3_Jet20_Central", kL1_Mu3_Jet20_Central  );    
  mymod->AddL1Trigger("L1_EG5_Jet36_deltaPhi1",  kL1_EG5_Jet36_deltaPhi1 );


  mymod->SetPrintHLT(kTRUE); // print HLT table at start of analysis?
  



  //------------------------------------------------------------------------------------------------
  // making analysis chain
  //------------------------------------------------------------------------------------------------
  // Chain modules together
  if(isData == false){
    GeneratorMod1->Add(PartonFlavorHistoryMod1);
    PartonFlavorHistoryMod1->Add(runLumiSelection);
    runLumiSelection->Add(HKFactorProducer1);
    HKFactorProducer1->Add(goodPVFilterMod);
  }
  else {
    GeneratorMod1->Add(runLumiSelection);
    runLumiSelection->Add(goodPVFilterMod);
  }

  //------------------------------------------------------------------------------------------------
  // Run Lepton + Denominator Skim
  //------------------------------------------------------------------------------------------------

  //loose+loose
  if (runSkim == 1) {
    goodPVFilterMod->Add(muTightId);
    muTightId->Add(electronTightId);
    electronTightId->Add(muDenominator);
    muDenominator->Add(electronDenominator);
    electronDenominator->Add(mergedTight);
    mergedTight->Add(mergedLoose);
    mergedLoose->Add(selModDoubleLoose);
    selModDoubleLoose->Add(muonID1); 
  } 
  
  //loose
  else if (runSkim == 2) {
    goodPVFilterMod->Add(muTightId);
    muTightId->Add(electronTightId);
    electronTightId->Add(muDenominator);
    muDenominator->Add(electronDenominator);
    electronDenominator->Add(mergedTight);
    mergedTight->Add(mergedLoose);
    mergedLoose->Add(selModLoose);
    selModLoose->Add(muonID1); 
  }
  //tight+loose
  else if (runSkim == 3) {
    goodPVFilterMod->Add(muTightId);
    muTightId->Add(electronTightId);
    electronTightId->Add(muDenominator);
    muDenominator->Add(electronDenominator);
    electronDenominator->Add(mergedTight);
    mergedTight->Add(mergedLoose);
    mergedLoose->Add(selModTight);
    selModTight->Add(selModDoubleLoose); 
    selModDoubleLoose->Add(muonID1);
  } else {
   goodPVFilterMod->Add(muonID1);
  }


  //Standard Sequence
  muonID1->Add(electronID1);
  electronID1->Add(photonIDMod1);
  photonIDMod1->Add(pftauIDMod1);
  pftauIDMod1->Add(pubJet1);
  pubJet1->Add(jetCorr1_ntuple);
  jetCorr1_ntuple->Add(theJetID1_ntuple);
  theJetID1_ntuple->Add(electronCleaning1);
  electronCleaning1->Add(photonCleaningMod1);
  photonCleaningMod1->Add(pftauCleaningMod1);
  pftauCleaningMod1->Add(theJetCleaning1_ntuple);
  theJetCleaning1_ntuple->Add(theJetID2_ntuple);
  theJetID2_ntuple->Add(theJetCleaning2_ntuple);
  theJetCleaning2_ntuple->Add(merger1);
  merger1->Add(pubMet);
  pubMet->Add(pubCaloMet);
  pubCaloMet->Add(metCaloCorr);
  metCaloCorr->Add(pubPFMet);
  pubPFMet->Add(muonIDFakeable);
  muonIDFakeable->Add(electronIDFakeable);
  electronIDFakeable->Add(electronCleaningFakeable);
  electronCleaningFakeable->Add(mergerFakeable);

  //SmurfNtupler
  if (doSmurfNtupler) {
    mergerFakeable->Add(hltmod);
    
    if (isPhotonControlSample) {
      hltmod->Add(HwwMakeNtupleModPhoton);
    } else {
      hltmod->Add(HwwMakeNtupleMod0);
      HwwMakeNtupleMod0->Add(HwwMakeNtupleMod1);
      HwwMakeNtupleMod1->Add(HwwMakeNtupleMod2);
    }
  }
  
  //------------------------------------------------------------------------------------------------
  //
  // setup analysis object
  //
  //------------------------------------------------------------------------------------------------
  Analysis *ana = new Analysis;
  ana->SetUseHLT(kTRUE);
  ana->SetKeepHierarchy(kFALSE);
  if(nEvents >= 0) 
    ana->SetProcessNEvents(nEvents);

  ana->AddSuperModule(GeneratorMod1);
  ana->SetPrintScale(100);

  //------------------------------------------------------------------------------------------------
  // organize input
  //------------------------------------------------------------------------------------------------
  printf("\nRely on Catalog: %s\n",catalogDir);
  printf("  -> Book: %s  Dataset: %s  Skim: %s  Fileset: %s <-\n\n",book,dataset,skim,fileset);
  Catalog *c = new Catalog(catalogDir);
  TString skimdataset = TString(dataset)+TString("/") +TString(skim);
  Dataset *d = NULL;
  if (TString(skim).CompareTo("noskim") == 0)
    d = c->FindDataset(book,dataset,fileset);
  else 
    d = c->FindDataset(book,skimdataset.Data(),fileset);
  ana->AddDataset(d);

//   ana->AddFile("/castor/cern.ch/user/c/ceballos/cms/filler/020/w10-wjets-z2-v8-pu11-2l/outfilename_000_10_1_4Oe.root");
//   ana->AddFile("/castor/cern.ch/user/c/ceballos/cms/filler/020/w10-wjets-z2-v8-pu11-2l//outfilename_000_3_1_J4a.root");


  //------------------------------------------------------------------------------------------------
  // organize output
  //------------------------------------------------------------------------------------------------
  ana->SetOutputName(rootFile.Data());
  ana->SetCacheSize(0);


  //
  // run analysis after successful initialisation
  //
  ana->Run(!gROOT->IsBatch());
}

