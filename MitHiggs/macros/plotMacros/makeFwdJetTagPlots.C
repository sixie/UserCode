#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <string>
#include <TFile.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TF1.h>
#include <TH1F.h>
#include <MitStyle.h>
using namespace std;
using namespace mithep;
#endif

#include <vector>


int  colors[] = { 2,3,4,6,7,8,kOrange,kBlack,kGray,kYellow,kAzure,kViolet,
                  kRed-1,kRed-2,kRed-3,kRed-4,kRed-5,kRed-6,kRed-7,kRed-8,
                  kWhite
};


//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void makeFwdJetTagPlots()
{
  //Make Plots for Jet Validation
  gStyle->SetPalette(1);

  bool     doLeg   = true;
  double   legx[2] = { 0.5, 0.75 };
  double   legy[2] = { 0.7, 0.8 };
  double   legTextSize = 0.03;
  TString legTitle = "";


//**************************************************************************************************
// Define input histogram files
//**************************************************************************************************
  //string signalFilename = "VBFHWWSelection_mitPrivateH165WW_2l-id9_0000.root";
  //string ttbarBkgFilename = "VBFHWWSelection_mitPrivateTTbar_dilepton-id9_0000.root";
  string signalFilename = "FwdJetStudy_mitPrivateH165WW_2l-id9_0000.root";
  string ttbarBkgFilename = "FwdJetStudy_mitPrivateTTbar_dilepton-id9_0000.root";


  vector<string> LegendNames;
  vector<string> filenames;  
  filenames.clear();
  LegendNames.clear();
  filenames.push_back(signalFilename);
  filenames.push_back(ttbarBkgFilename);
  LegendNames.push_back("HWW");
  LegendNames.push_back("TTBAR");

  string sample = "";
  string plotname;
  string filename;
  
  //Fake Backgrounds
  vector <string> inclMuonBkgFiles;
  vector<double> inclMuonBkgWeight;
  //inclMuonBkgFiles.push_back("/home/sixie/hist/MuonIsolationStudy/filler/004/MuonIsolationStudy_PtRange40ToInf_s8-incmu_5_50-id9.root");
  inclMuonBkgFiles.push_back("/home/sixie/hist/MuonIsolationStudy/filler/004/MuonIsolationStudy_PtRange40ToInf_s8-incmu_5_50-id9_0000.root");
  inclMuonBkgWeight.push_back(400000000.0/ 5000000);

  vector <string> QCDDJBkgFiles;
  vector<double> QCDDJBkgWeight;
  QCDDJBkgFiles.push_back("mit-MuonIsoStudy_QCDDJ30-50_hist.root");
  QCDDJBkgFiles.push_back("mit-MuonIsoStudy_QCDDJ50-80_hist.root");
  QCDDJBkgFiles.push_back("mit-MuonIsoStudy_QCDDJ170-230_hist.root");
  QCDDJBkgWeight.push_back(94702500.0/ 32400.0);
  QCDDJBkgWeight.push_back(12195900.0 / 6480.0);
  QCDDJBkgWeight.push_back(48325.0 / 34560.0);

//**************************************************************************************************
//
// Generator Level Plots
//
//**************************************************************************************************
  //makeGeneratorPlots( signalFilename);

//**************************************************************************************************
//
// Comparison Distributions
//
//**************************************************************************************************
  vector<string> histNames;
  vector<string> auxLegendNames;

  plotname = "CleanJetsEta";
  histNames.clear();
  histNames.push_back("hCleanJetEta_VBF");
  histNames.push_back("hCleanJetEta_nonVBF");
  auxLegendNames.clear();
  auxLegendNames.push_back("_Matched");
  auxLegendNames.push_back("_WrongMatch");
  //makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,-10,10, "logy");

  plotname = "CleanJet1Pt";
  histNames.clear();
  histNames.push_back("hCleanJet1Pt_VBF");
  histNames.push_back("hCleanJet1Pt_nonVBF");
  auxLegendNames.clear();
  auxLegendNames.push_back("_Matched");
  auxLegendNames.push_back("_WrongMatch");
  //makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,0,300, "logy");

  plotname = "CleanJet2Pt";
  histNames.clear();
  histNames.push_back("hCleanJet2Pt_VBF");
  histNames.push_back("hCleanJet2Pt_nonVBF");
  auxLegendNames.clear();
  auxLegendNames.push_back("_Matched");
  auxLegendNames.push_back("_WrongMatch");
  //makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,0,300, "logy");

  plotname = "FwdJetsSameEtaSide_SequentialCuts";
  histNames.clear();
  histNames.push_back("hVBFJet_SameEtaSide");
  auxLegendNames.clear();
  auxLegendNames.push_back("");
  makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,-1,1,0,7000);

  plotname = "FwdJetsDeltaEta_SequentialCuts";
  histNames.clear();
  histNames.push_back("hVBFJet_DeltaEta");
  auxLegendNames.clear();
  auxLegendNames.push_back("");
  makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,0,10.0,0,250);

  plotname = "FwdJetsDijetMass_SequentialCuts";
  histNames.clear();
  histNames.push_back("hVBFJet_DijetMass");
  auxLegendNames.clear();
  auxLegendNames.push_back("");
  makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,0,4000,0,150);

  plotname = "FwdJetsNNOutput";
  histNames.clear();
  histNames.push_back("hVBFJet_NNOutput");
  auxLegendNames.clear();
  auxLegendNames.push_back("");
  makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,0.0,1.0,0,150);

  plotname = "FwdJetsSameEtaSide_NMinusOne";
  histNames.clear();
  histNames.push_back("hVBFJet_SameEtaSide_NMinusOne");
  auxLegendNames.clear();
  auxLegendNames.push_back("");
  //makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,-1,1,0,7000);

  plotname = "FwdJetsDeltaEta_NMinusOne";
  histNames.clear();
  histNames.push_back("hVBFJet_DeltaEta_NMinusOne");
  auxLegendNames.clear();
  auxLegendNames.push_back("");
  //makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,0,10.0,0,250);

  plotname = "FwdJetsDijetMass_NMinusOne";
  histNames.clear();
  histNames.push_back("hVBFJet_DijetMass_NMinusOne");
  auxLegendNames.clear();
  auxLegendNames.push_back("");
  //makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,0,4000,0,150);


  plotname = "FwdJetsDeltaPhi";
  histNames.clear();
  histNames.push_back("hDFwdJetSel_0");
  histNames.push_back("hDFwdJetSel_100");
  auxLegendNames.clear();
  auxLegendNames.push_back("_Matched");
  auxLegendNames.push_back("_WrongMatch");
  //makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,0,200, "logy");

  plotname = "FwdJetsDeltaEta";
  histNames.clear();
  histNames.push_back("hDFwdJetSel_1");
  histNames.push_back("hDFwdJetSel_101");
  auxLegendNames.clear();
  auxLegendNames.push_back("_Matched");
  auxLegendNames.push_back("_WrongMatch");
  //makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,0,200, "logy");

  plotname = "FwdJetsPtMax";
  histNames.clear();
  histNames.push_back("hDFwdJetSel_2");
  histNames.push_back("hDFwdJetSel_102");
  auxLegendNames.clear();
  auxLegendNames.push_back("_Matched");
  auxLegendNames.push_back("_WrongMatch");
  //makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,0,200, "logy");

  plotname = "FwdJetsPtMin";
  histNames.clear();
  histNames.push_back("hDFwdJetSel_3");
  histNames.push_back("hDFwdJetSel_103");
  auxLegendNames.clear();
  auxLegendNames.push_back("_Matched");
  auxLegendNames.push_back("_WrongMatch");
  //makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,0,200, "logy");

  plotname = "FwdJetsDijetMass";
  histNames.clear();
  histNames.push_back("hDFwdJetSel_4");
  histNames.push_back("hDFwdJetSel_104");
  auxLegendNames.clear();
  auxLegendNames.push_back("_Matched");
  auxLegendNames.push_back("_WrongMatch");
  //makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,0,4000, "logy");

  plotname = "FwdJetsSameEtaSide";
  histNames.clear();
  histNames.push_back("hDFwdJetSel_5");
  histNames.push_back("hDFwdJetSel_105");
  auxLegendNames.clear();
  auxLegendNames.push_back("_Matched");
  auxLegendNames.push_back("_WrongMatch");
  //makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,-1,1, "logy");

  plotname = "FwdJetsNNOutput";
  histNames.clear();
  histNames.push_back("hDFwdJetSel_6");
  histNames.push_back("hDFwdJetSel_106");
  auxLegendNames.clear();
  auxLegendNames.push_back("_Matched");
  auxLegendNames.push_back("_WrongMatch");
  //makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,-1,1, "logy");

  plotname = "FwdJetsPassCut";
  histNames.clear();
  histNames.push_back("hDFwdJetSel_7");
  histNames.push_back("hDFwdJetSel_107");
  auxLegendNames.clear();
  auxLegendNames.push_back("_Matched");
  auxLegendNames.push_back("_WrongMatch");
  //makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,-1,1, "logy");

  plotname = "FwdJetsPassNN";
  histNames.clear();
  histNames.push_back("hDFwdJetSel_8");
  histNames.push_back("hDFwdJetSel_108");
  auxLegendNames.clear();
  auxLegendNames.push_back("_Matched");
  auxLegendNames.push_back("_WrongMatch");
  //makeComparisonPlot( filenames, histNames, LegendNames,auxLegendNames, plotname,-1,1, "logy");

//**************************************************************************************************
//
// 1D Signal Efficiency
//
//**************************************************************************************************
  plotname = "VBFJetTag_Jet1PtCutEfficiency";
  //makeCutEfficiencyPlot( signalFilename, "hCleanJet1Pt_VBF" , plotname, "below");

  plotname = "VBFJetTag_SameEtaSideCutEfficiency";
  //makeCutEfficiencyPlot( signalFilename, "hVBFJet_SameEtaSide" , plotname, "above");

  plotname = "VBFJetTag_DeltaEtaCutEfficiency";
  //makeCutEfficiencyPlot( signalFilename, "hVBFJet_DeltaEta" , plotname, "below");

  plotname = "VBFJetTag_DijetMassCutEfficiency";
  //makeCutEfficiencyPlot( signalFilename, "hVBFJet_DijetMass" , plotname, "below");

  plotname = "VBFJetTag_NNCutEfficiency";
  //makeCutEfficiencyPlot( signalFilename, "hVBFJet_NNOutput" , plotname, "below");

  return;


//**************************************************************************************************
//
// Efficiency Plots
//
//**************************************************************************************************


//Define isolation variables for comparison
  vector<string> histnames;
  vector<string> npassnames;
  vector<string> isolationnames;
//define QCD samples with proper weights
  vector <string> bkgfiles;
  vector<double> bkgweight;

  plotname = "MuonIsolationR03SignalVsBkgEff_InclMu";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hDFwdJetSel_1");
  histnames.push_back("hDFwdJetSel_101");
  npassnames.push_back("hNMuonPassTrackIsoR03");
  npassnames.push_back("hNMuonPassEcalIsoR03");
  npassnames.push_back("hNMuonPassHcalIsoR03");
  npassnames.push_back("hNMuonPassEcalPlusHcalIsoR03");
  npassnames.push_back("hNMuonPassOnePointFiveEcalPlusHcalIsoR03");
  isolationnames.push_back("TrackIso R03");
  isolationnames.push_back("EcalIso R03");
  isolationnames.push_back("HcalIso R03");
  isolationnames.push_back("Ecal+Hcal Iso R03");
  isolationnames.push_back("1.5*Ecal+Hcal Iso R03");
  //makeEfficiencyComparisonPlot( signalFilename,inclMuonBkgFiles,inclMuonBkgWeight, histnames, npassnames, isolationnames, plotname, 0.75, 1.0);

  plotname = "MuonIsolationR05SignalVsBkgEff_InclMu";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackIsoR05Eff");
  histnames.push_back("hMuonEcalIsoR05Eff");
  histnames.push_back("hMuonHcalIsoR05Eff");
  histnames.push_back("hMuonEcalPlusHcalIsoR05Eff");
  histnames.push_back("hMuonOnePointFiveEcalPlusHcalIsoR05Eff");
  npassnames.push_back("hNMuonPassTrackIsoR05");
  npassnames.push_back("hNMuonPassEcalIsoR05");
  npassnames.push_back("hNMuonPassHcalIsoR05");
  npassnames.push_back("hNMuonPassEcalPlusHcalIsoR05");
  npassnames.push_back("hNMuonPassOnePointFiveEcalPlusHcalIsoR05");
  isolationnames.push_back("TrackIso R05");
  isolationnames.push_back("EcalIso R05");
  isolationnames.push_back("HcalIso R05");
  isolationnames.push_back("Ecal+Hcal Iso R05");
  isolationnames.push_back("1.5*Ecal+Hcal Iso R05");
  //makeEfficiencyComparisonPlot( signalFilename,inclMuonBkgFiles,inclMuonBkgWeight, histnames, npassnames, isolationnames, plotname, 0.5, 1.0);

  plotname = "MuonIsolationR03SignalVsBkgEff_QCDDJ";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackIsoR03Eff");
  histnames.push_back("hMuonEcalIsoR03Eff");
  histnames.push_back("hMuonHcalIsoR03Eff");
  histnames.push_back("hMuonEcalPlusHcalIsoR03Eff");
  histnames.push_back("hMuonOnePointFiveEcalPlusHcalIsoR03Eff");
  npassnames.push_back("hNMuonPassTrackIsoR03");
  npassnames.push_back("hNMuonPassEcalIsoR03");
  npassnames.push_back("hNMuonPassHcalIsoR03");
  npassnames.push_back("hNMuonPassEcalPlusHcalIsoR03");
  npassnames.push_back("hNMuonPassOnePointFiveEcalPlusHcalIsoR03");
  isolationnames.push_back("TrackIso R03");
  isolationnames.push_back("EcalIso R03");
  isolationnames.push_back("HcalIso R03");
  isolationnames.push_back("Ecal+Hcal Iso R03");
  isolationnames.push_back("1.5*Ecal+Hcal Iso R03");
  //makeEfficiencyComparisonPlot( signalFilename,QCDDJBkgFiles,QCDDJBkgWeight, histnames, npassnames, isolationnames, plotname, 0.75, 1.0);

  plotname = "MuonIsolationR05SignalVsBkgEff_QCDDJ";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackIsoR05Eff");
  histnames.push_back("hMuonEcalIsoR05Eff");
  histnames.push_back("hMuonHcalIsoR05Eff");
  histnames.push_back("hMuonEcalPlusHcalIsoR05Eff");
  histnames.push_back("hMuonOnePointFiveEcalPlusHcalIsoR05Eff");
  npassnames.push_back("hNMuonPassTrackIsoR05");
  npassnames.push_back("hNMuonPassEcalIsoR05");
  npassnames.push_back("hNMuonPassHcalIsoR05");
  npassnames.push_back("hNMuonPassEcalPlusHcalIsoR05");
  npassnames.push_back("hNMuonPassOnePointFiveEcalPlusHcalIsoR05");
  isolationnames.push_back("TrackIso R05");
  isolationnames.push_back("EcalIso R05");
  isolationnames.push_back("HcalIso R05");
  isolationnames.push_back("Ecal+Hcal Iso R05");
  isolationnames.push_back("1.5*Ecal+Hcal Iso R05");
  //makeEfficiencyComparisonPlot( signalFilename,QCDDJBkgFiles,QCDDJBkgWeight, histnames, npassnames, isolationnames, plotname, 0.5, 1.0);

  plotname = "MuonCombinedIsolationR03SignalVsBkgEff_InclMu";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR03Eff");
  histnames.push_back("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03Eff");
  histnames.push_back("hMuonTwoTrackPlusEcalPlusHcalIsoR03Eff");
  histnames.push_back("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03Eff");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR03");
  npassnames.push_back("hNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR03");
  npassnames.push_back("hNMuonPassTwoTrackPlusEcalPlusHcalIsoR03");
  npassnames.push_back("hNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03");
  isolationnames.push_back("Track+Ecal+Hcal Iso R03");
  isolationnames.push_back("Track+1.5*Ecal+Hcal Iso R03");
  isolationnames.push_back("2*Track+Ecal+Hcal Iso R03");
  isolationnames.push_back("2*Track+1.5*Ecal+Hcal Iso R03");
  //makeEfficiencyComparisonPlot( signalFilename,inclMuonBkgFiles,inclMuonBkgWeight, histnames, npassnames, isolationnames, plotname, 0.75, 1.0);

  plotname = "MuonCombinedIsolationR05SignalVsBkgEff_InclMu";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR05Eff");
  histnames.push_back("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05Eff");
  histnames.push_back("hMuonTwoTrackPlusEcalPlusHcalIsoR05Eff");
  histnames.push_back("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05Eff");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR05");
  npassnames.push_back("hNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR05");
  npassnames.push_back("hNMuonPassTwoTrackPlusEcalPlusHcalIsoR05");
  npassnames.push_back("hNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05");
  isolationnames.push_back("Track+Ecal+Hcal Iso R05");
  isolationnames.push_back("Track+1.5*Ecal+Hcal Iso R05");
  isolationnames.push_back("2*Track+Ecal+Hcal Iso R05");
  isolationnames.push_back("2*Track+1.5*Ecal+Hcal Iso R05");
  //makeEfficiencyComparisonPlot( signalFilename,inclMuonBkgFiles,inclMuonBkgWeight, histnames, npassnames, isolationnames, plotname, 0.5, 1.0);

  plotname = "MuonCombinedIsolationR03SignalVsBkgEff_QCDDJ";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR03Eff");
  histnames.push_back("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03Eff");
  histnames.push_back("hMuonTwoTrackPlusEcalPlusHcalIsoR03Eff");
  histnames.push_back("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03Eff");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR03");
  npassnames.push_back("hNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR03");
  npassnames.push_back("hNMuonPassTwoTrackPlusEcalPlusHcalIsoR03");
  npassnames.push_back("hNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03");
  isolationnames.push_back("Track+Ecal+Hcal Iso R03");
  isolationnames.push_back("Track+1.5*Ecal+Hcal Iso R03");
  isolationnames.push_back("2*Track+Ecal+Hcal Iso R03");
  isolationnames.push_back("2*Track+1.5*Ecal+Hcal Iso R03");
  //makeEfficiencyComparisonPlot( signalFilename,QCDDJBkgFiles,QCDDJBkgWeight, histnames, npassnames, isolationnames, plotname, 0.75, 1.0);

  plotname = "MuonCombinedIsolationR05SignalVsBkgEff_QCDDJ";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR05Eff");
  histnames.push_back("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05Eff");
  histnames.push_back("hMuonTwoTrackPlusEcalPlusHcalIsoR05Eff");
  histnames.push_back("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05Eff");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR05");
  npassnames.push_back("hNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR05");
  npassnames.push_back("hNMuonPassTwoTrackPlusEcalPlusHcalIsoR05");
  npassnames.push_back("hNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05");
  isolationnames.push_back("Track+Ecal+Hcal Iso R05");
  isolationnames.push_back("Track+1.5*Ecal+Hcal Iso R05");
  isolationnames.push_back("2*Track+Ecal+Hcal Iso R05");
  isolationnames.push_back("2*Track+1.5*Ecal+Hcal Iso R05");
  //makeEfficiencyComparisonPlot( signalFilename,QCDDJBkgFiles,QCDDJBkgWeight, histnames, npassnames, isolationnames, plotname, 0.5, 1.0);

//**************************************************************************************************
//
// Efficiency Plots for Isolation / Pt
//
//**************************************************************************************************


//Define isolation variables for comparison
  vector<string> histnames;
  vector<string> npassnames;
  vector<string> isolationnames;
//define QCD samples with proper weights
  vector <string> bkgfiles;
  vector<double> bkgweight;

  plotname = "MuonIsolationR03OverPtSignalVsBkgEff_InclMu";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackIsoR03Eff");
  histnames.push_back("hMuonTrackIsoR03OverPtEff");
  histnames.push_back("hMuonEcalIsoR03OverPtEff");
  histnames.push_back("hMuonHcalIsoR03OverPtEff");
  histnames.push_back("hMuonEcalPlusHcalIsoR03OverPtEff");
  histnames.push_back("hMuonOnePointFiveEcalPlusHcalIsoR03OverPtEff");
  npassnames.push_back("hNMuonPassTrackIsoR03");
  npassnames.push_back("hNMuonPassTrackIsoR03OverPt");
  npassnames.push_back("hNMuonPassEcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassHcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassEcalPlusHcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassOnePointFiveEcalPlusHcalIsoR03OverPt");
  isolationnames.push_back("TrackIso R03");
  isolationnames.push_back("TrackIso R03 / Pt");
  isolationnames.push_back("EcalIso R03 / Pt");
  isolationnames.push_back("HcalIso R03 / Pt");
  isolationnames.push_back("Ecal+Hcal Iso R03 / Pt");
  isolationnames.push_back("1.5*Ecal+Hcal Iso R03 / Pt");
  //makeEfficiencyComparisonPlot( signalFilename,inclMuonBkgFiles,inclMuonBkgWeight, histnames, npassnames, isolationnames, plotname, 0.75, 1.0);
  plotname = "MuonIsolationR05OverPtSignalVsBkgEff_InclMu";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackIsoR05Eff");
  histnames.push_back("hMuonTrackIsoR05OverPtEff");
  histnames.push_back("hMuonEcalIsoR05OverPtEff");
  histnames.push_back("hMuonHcalIsoR05OverPtEff");
  histnames.push_back("hMuonEcalPlusHcalIsoR05OverPtEff");
  histnames.push_back("hMuonOnePointFiveEcalPlusHcalIsoR05OverPtEff");
  npassnames.push_back("hNMuonPassTrackIsoR05");
  npassnames.push_back("hNMuonPassTrackIsoR05OverPt");
  npassnames.push_back("hNMuonPassEcalIsoR05OverPt");
  npassnames.push_back("hNMuonPassHcalIsoR05OverPt");
  npassnames.push_back("hNMuonPassEcalPlusHcalIsoR05OverPt");
  npassnames.push_back("hNMuonPassOnePointFiveEcalPlusHcalIsoR05OverPt");
  isolationnames.push_back("TrackIso R05");
  isolationnames.push_back("TrackIso R05 / Pt");
  isolationnames.push_back("EcalIso R05 / Pt");
  isolationnames.push_back("HcalIso R05 / Pt");
  isolationnames.push_back("Ecal+Hcal Iso R05 / Pt");
  isolationnames.push_back("1.5*Ecal+Hcal Iso R05 / Pt");
  //makeEfficiencyComparisonPlot( signalFilename,inclMuonBkgFiles,inclMuonBkgWeight, histnames, npassnames, isolationnames, plotname, 0.75, 1.0);

  plotname = "MuonIsolationR03OverPtSignalVsBkgEff_QCDDJ";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackIsoR03Eff");
  histnames.push_back("hMuonTrackIsoR03OverPtEff");
  histnames.push_back("hMuonEcalIsoR03OverPtEff");
  histnames.push_back("hMuonHcalIsoR03OverPtEff");
  histnames.push_back("hMuonEcalPlusHcalIsoR03OverPtEff");
  histnames.push_back("hMuonOnePointFiveEcalPlusHcalIsoR03OverPtEff");
  npassnames.push_back("hNMuonPassTrackIsoR03");
  npassnames.push_back("hNMuonPassTrackIsoR03OverPt");
  npassnames.push_back("hNMuonPassEcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassHcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassEcalPlusHcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassOnePointFiveEcalPlusHcalIsoR03OverPt");
  isolationnames.push_back("TrackIso R03");
  isolationnames.push_back("TrackIso R03 / Pt");
  isolationnames.push_back("EcalIso R03 / Pt");
  isolationnames.push_back("HcalIso R03 / Pt");
  isolationnames.push_back("Ecal+Hcal Iso R03 / Pt");
  isolationnames.push_back("1.5*Ecal+Hcal Iso R03 / Pt");
  //makeEfficiencyComparisonPlot( signalFilename,QCDDJBkgFiles,QCDDJBkgWeight, histnames, npassnames, isolationnames, plotname, 0.75, 1.0);

  plotname = "MuonIsolationR05OverPtSignalVsBkgEff_QCDDJ";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackIsoR05Eff");
  histnames.push_back("hMuonTrackIsoR05OverPtEff");
  histnames.push_back("hMuonEcalIsoR05OverPtEff");
  histnames.push_back("hMuonHcalIsoR05OverPtEff");
  histnames.push_back("hMuonEcalPlusHcalIsoR05OverPtEff");
  histnames.push_back("hMuonOnePointFiveEcalPlusHcalIsoR05OverPtEff");
  npassnames.push_back("hNMuonPassTrackIsoR05");
  npassnames.push_back("hNMuonPassTrackIsoR05OverPt");
  npassnames.push_back("hNMuonPassEcalIsoR05OverPt");
  npassnames.push_back("hNMuonPassHcalIsoR05OverPt");
  npassnames.push_back("hNMuonPassEcalPlusHcalIsoR05OverPt");
  npassnames.push_back("hNMuonPassOnePointFiveEcalPlusHcalIsoR05OverPt");
  isolationnames.push_back("TrackIso R05");
  isolationnames.push_back("TrackIso R05 / Pt");
  isolationnames.push_back("EcalIso R05 / Pt");
  isolationnames.push_back("HcalIso R05 / Pt");
  isolationnames.push_back("Ecal+Hcal Iso R05 / Pt");
  isolationnames.push_back("1.5*Ecal+Hcal Iso R05 / Pt");
  //makeEfficiencyComparisonPlot( signalFilename,QCDDJBkgFiles,QCDDJBkgWeight, histnames, npassnames, isolationnames, plotname, 0.75, 1.0);


  plotname = "MuonCombinedIsolationR03OverPtSignalVsBkgEff_InclMu";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR03Eff");
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR03OverPtEff");
  histnames.push_back("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPtEff");
  histnames.push_back("hMuonTwoTrackPlusEcalPlusHcalIsoR03OverPtEff");
  histnames.push_back("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPtEff");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR03");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassTwoTrackPlusEcalPlusHcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt");
  isolationnames.push_back("Track+Ecal+Hcal Iso R03");
  isolationnames.push_back("(Track+Ecal+Hcal Iso R03) / Pt");
  isolationnames.push_back("(Track+1.5*Ecal+Hcal Iso R03) / Pt");
  isolationnames.push_back("(2*Track+Ecal+Hcal Iso R03) / Pt");
  isolationnames.push_back("(2*Track+1.5*Ecal+Hcal Iso R03) / Pt");
  //makeEfficiencyComparisonPlot( signalFilename,inclMuonBkgFiles,inclMuonBkgWeight, histnames, npassnames, isolationnames, plotname, 0.75, 1.0);

  plotname = "MuonCombinedIsolationR05OverPtSignalVsBkgEff_InclMu";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR05Eff");
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR05OverPtEff");
  histnames.push_back("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPtEff");
  histnames.push_back("hMuonTwoTrackPlusEcalPlusHcalIsoR05OverPtEff");
  histnames.push_back("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPtEff");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR05");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR05OverPt");
  npassnames.push_back("hNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt");
  npassnames.push_back("hNMuonPassTwoTrackPlusEcalPlusHcalIsoR05OverPt");
  npassnames.push_back("hNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt");
  isolationnames.push_back("Track+Ecal+Hcal Iso R05");
  isolationnames.push_back("(Track+Ecal+Hcal Iso R05) / Pt");
  isolationnames.push_back("(Track+1.5*Ecal+Hcal Iso R05) / Pt");
  isolationnames.push_back("(2*Track+Ecal+Hcal Iso R05) / Pt");
  isolationnames.push_back("(2*Track+1.5*Ecal+Hcal Iso R05) / Pt");
  //makeEfficiencyComparisonPlot( signalFilename,inclMuonBkgFiles,inclMuonBkgWeight, histnames, npassnames, isolationnames, plotname, 0.75, 1.0);

  plotname = "MuonCombinedIsolationR03OverPtSignalVsBkgEff_QCDDJ";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR03Eff");
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR03OverPtEff");
  histnames.push_back("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPtEff");
  histnames.push_back("hMuonTwoTrackPlusEcalPlusHcalIsoR03OverPtEff");
  histnames.push_back("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPtEff");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR03");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassTwoTrackPlusEcalPlusHcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt");
  isolationnames.push_back("Track+Ecal+Hcal Iso R03");
  isolationnames.push_back("(Track+Ecal+Hcal Iso R03) / Pt");
  isolationnames.push_back("(Track+1.5*Ecal+Hcal Iso R03) / Pt");
  isolationnames.push_back("(2*Track+Ecal+Hcal Iso R03) / Pt");
  isolationnames.push_back("(2*Track+1.5*Ecal+Hcal Iso R03) / Pt");
  //makeEfficiencyComparisonPlot( signalFilename,QCDDJBkgFiles,QCDDJBkgWeight, histnames, npassnames, isolationnames, plotname, 0.75, 1.0);

  plotname = "MuonCombinedIsolationR05OverPtSignalVsBkgEff_QCDDJ";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR05Eff");
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR05OverPtEff");
  histnames.push_back("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPtEff");
  histnames.push_back("hMuonTwoTrackPlusEcalPlusHcalIsoR05OverPtEff");
  histnames.push_back("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPtEff");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR05");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR05OverPt");
  npassnames.push_back("hNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt");
  npassnames.push_back("hNMuonPassTwoTrackPlusEcalPlusHcalIsoR05OverPt");
  npassnames.push_back("hNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt");
  isolationnames.push_back("Track+Ecal+Hcal Iso R05");
  isolationnames.push_back("(Track+Ecal+Hcal Iso R05) / Pt");
  isolationnames.push_back("(Track+1.5*Ecal+Hcal Iso R05) / Pt");
  isolationnames.push_back("(2*Track+Ecal+Hcal Iso R05) / Pt");
  isolationnames.push_back("(2*Track+1.5*Ecal+Hcal Iso R05) / Pt");
  //makeEfficiencyComparisonPlot( signalFilename,QCDDJBkgFiles,QCDDJBkgWeight, histnames, npassnames, isolationnames, plotname, 0.75, 1.0);

//**************************************************************************************************
//
// Eff vs optimization Plots
//
//**************************************************************************************************
  plotname = "MuonIsolationR03SignalEffVsSignificance_InclMu";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackIsoR03Eff");
  histnames.push_back("hMuonEcalIsoR03Eff");
  histnames.push_back("hMuonHcalIsoR03Eff");
  histnames.push_back("hMuonEcalPlusHcalIsoR03Eff");
  histnames.push_back("hMuonOnePointFiveEcalPlusHcalIsoR03Eff");
  histnames.push_back("hMuonTrackIsoR03OverPtEff");
  histnames.push_back("hMuonEcalIsoR03OverPtEff");
  histnames.push_back("hMuonHcalIsoR03OverPtEff");
  histnames.push_back("hMuonEcalPlusHcalIsoR03OverPtEff");
  histnames.push_back("hMuonOnePointFiveEcalPlusHcalIsoR03OverPtEff");
  npassnames.push_back("hNMuonPassTrackIsoR03");
  npassnames.push_back("hNMuonPassEcalIsoR03");
  npassnames.push_back("hNMuonPassHcalIsoR03");
  npassnames.push_back("hNMuonPassEcalPlusHcalIsoR03");
  npassnames.push_back("hNMuonPassOnePointFiveEcalPlusHcalIsoR03");
  npassnames.push_back("hNMuonPassTrackIsoR03OverPt");
  npassnames.push_back("hNMuonPassEcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassHcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassEcalPlusHcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassOnePointFiveEcalPlusHcalIsoR03OverPt");
  isolationnames.push_back("TrackIso R03");
  isolationnames.push_back("EcalIso R03");
  isolationnames.push_back("HcalIso R03");
  isolationnames.push_back("Ecal+Hcal Iso R03");
  isolationnames.push_back("1.5*Ecal+Hcal Iso R03");
  isolationnames.push_back("(TrackIso R03) / Pt");
  isolationnames.push_back("(EcalIso R03) / Pt");
  isolationnames.push_back("(HcalIso R03) / Pt");
  isolationnames.push_back("(Ecal+Hcal Iso R03) / Pt");
  isolationnames.push_back("(1.5*Ecal+Hcal Iso R03) / Pt");
  //makeEfficiencyOptimizationPlot( signalFilename,inclMuonBkgFiles,inclMuonBkgWeight, histnames, npassnames, isolationnames, plotname, 0.5, 1.0);
 
  plotname = "MuonIsolationR05SignalEffVsSignificance_InclMu";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackIsoR05Eff");
  histnames.push_back("hMuonEcalIsoR05Eff");
  histnames.push_back("hMuonHcalIsoR05Eff");
  histnames.push_back("hMuonEcalPlusHcalIsoR05Eff");
  histnames.push_back("hMuonOnePointFiveEcalPlusHcalIsoR05Eff");
  npassnames.push_back("hNMuonPassTrackIsoR05");
  npassnames.push_back("hNMuonPassEcalIsoR05");
  npassnames.push_back("hNMuonPassHcalIsoR05");
  npassnames.push_back("hNMuonPassEcalPlusHcalIsoR05");
  npassnames.push_back("hNMuonPassOnePointFiveEcalPlusHcalIsoR05");
  isolationnames.push_back("TrackIso R05");
  isolationnames.push_back("EcalIso R05");
  isolationnames.push_back("HcalIso R05");
  isolationnames.push_back("Ecal+Hcal Iso R05");
  isolationnames.push_back("1.5*Ecal+Hcal Iso R05");
  //makeEfficiencyOptimizationPlot( signalFilename,inclMuonBkgFiles,inclMuonBkgWeight, histnames, npassnames, isolationnames, plotname, 0.5, 1.0);


  plotname = "MuonCombinedIsolationR03SignalVsSignificance_InclMu";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR03Eff");
  histnames.push_back("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03Eff");
  histnames.push_back("hMuonTwoTrackPlusEcalPlusHcalIsoR03Eff");
  histnames.push_back("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03Eff");
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR03OverPtEff");
  histnames.push_back("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPtEff");
  histnames.push_back("hMuonTwoTrackPlusEcalPlusHcalIsoR03OverPtEff");
  histnames.push_back("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPtEff");
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR03OverPtEff");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR03");
  npassnames.push_back("hNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR03");
  npassnames.push_back("hNMuonPassTwoTrackPlusEcalPlusHcalIsoR03");
  npassnames.push_back("hNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassTwoTrackPlusEcalPlusHcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR03OverPt");
  isolationnames.push_back("Track+Ecal+Hcal Iso R03");
  isolationnames.push_back("Track+1.5*Ecal+Hcal Iso R03");
  isolationnames.push_back("2*Track+Ecal+Hcal Iso R03");
  isolationnames.push_back("2*Track+1.5*Ecal+Hcal Iso R03");
  isolationnames.push_back("(Track+Ecal+Hcal Iso R03) / Pt");
  isolationnames.push_back("(Track+1.5*Ecal+Hcal Iso R03) / Pt");
  isolationnames.push_back("(2*Track+Ecal+Hcal Iso R03) / Pt");
  isolationnames.push_back("(2*Track+1.5*Ecal+Hcal Iso R03) / Pt");
  isolationnames.push_back("(Track+Ecal+Hcal Iso R03) / Pt");
  //makeEfficiencyOptimizationPlot( signalFilename,inclMuonBkgFiles,inclMuonBkgWeight, histnames, npassnames, isolationnames, plotname, 0.5, 1.0);

  plotname = "MuonCombinedIsolationR05SignalVsSignificance_InclMu";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR05Eff");
  histnames.push_back("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05Eff");
  histnames.push_back("hMuonTwoTrackPlusEcalPlusHcalIsoR05Eff");
  histnames.push_back("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05Eff");
  histnames.push_back("hMuonTrackPlusEcalPlusHcalIsoR05OverPtEff");
  histnames.push_back("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPtEff");
  histnames.push_back("hMuonTwoTrackPlusEcalPlusHcalIsoR05OverPtEff");
  histnames.push_back("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPtEff");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR05");
  npassnames.push_back("hNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR05");
  npassnames.push_back("hNMuonPassTwoTrackPlusEcalPlusHcalIsoR05");
  npassnames.push_back("hNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05");
  npassnames.push_back("hNMuonPassTrackPlusEcalPlusHcalIsoR05OverPt");
  npassnames.push_back("hNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt");
  npassnames.push_back("hNMuonPassTwoTrackPlusEcalPlusHcalIsoR05OverPt");
  npassnames.push_back("hNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt");
  isolationnames.push_back("Track+Ecal+Hcal Iso R05");
  isolationnames.push_back("Track+1.5*Ecal+Hcal Iso R05");
  isolationnames.push_back("2*Track+Ecal+Hcal Iso R05");
  isolationnames.push_back("2*Track+1.5*Ecal+Hcal Iso R05");
  isolationnames.push_back("(Track+Ecal+Hcal Iso R05) / Pt");
  isolationnames.push_back("(Track+1.5*Ecal+Hcal Iso R05) / Pt");
  isolationnames.push_back("(2*Track+Ecal+Hcal Iso R05) / Pt");
  isolationnames.push_back("(2*Track+1.5*Ecal+Hcal Iso R05) / Pt");
  //makeEfficiencyOptimizationPlot( signalFilename,inclMuonBkgFiles,inclMuonBkgWeight, histnames, npassnames, isolationnames, plotname, 0.5, 1.0);


//**************************************************************************************************
//
// 2D Efficiency Plots
//
//**************************************************************************************************
  vector <string> files;
  vector<double> weight;
//**************************************************************************************************
// Track Iso vs Calo Iso
//**************************************************************************************************
  plotname = "MuonTrackIsoVsEcalIsoR03SignalEff";
  files.clear();
  weight.clear();
  files.push_back("mit-MuonIsoStudy_HWW_hist.root");
  weight.push_back(1.0);
  //make2DEffPlot( files,weight, "hNMuonPassTrackIsoVsEcalIsoR03","hMuonTrackIsoVsEcalIsoEffR03", plotname,0.5,1.0);

  plotname = "MuonTrackIsoVsEcalIsoR03BkgEff_InclMu";
  files.clear();
  weight.clear();
  files.push_back("mit-MuonIsoStudy_InclMu5-50_hist.root");
  weight.push_back(1.0);
  //make2DEffPlot( files,weight, "hNMuonPassTrackIsoVsEcalIsoR03","hMuonTrackIsoVsEcalIsoEffR03", plotname, 0.0, 0.1);

  plotname = "MuonTrackIsoVsEcalIsoR03BkgEff_QCDDJ";
  files.clear();
  weight.clear();
  files.push_back("mit-MuonIsoStudy_QCDDJ30-50_hist.root");
  files.push_back("mit-MuonIsoStudy_QCDDJ50-80_hist.root");
  files.push_back("mit-MuonIsoStudy_QCDDJ170-230_hist.root");
  weight.push_back(94702500.0/ 32400);
  weight.push_back(12195900.0 / 6480);
  weight.push_back(48325.0 / 34560);
  //make2DEffPlot( files,weight, "hNMuonPassTrackIsoVsEcalIsoR03","hMuonTrackIsoVsEcalIsoEffR03", plotname);

  plotname = "MuonTrackIsoVsHcalIsoR03SignalEff";
  files.clear();
  weight.clear();
  files.push_back("mit-MuonIsoStudy_HWW_hist.root");
  weight.push_back(1.0);
  //make2DEffPlot( files,weight, "hNMuonPassTrackIsoVsHcalIsoR03","hMuonTrackIsoVsHcalIsoEffR03", plotname,0.5,1.0);

  plotname = "MuonTrackIsoVsHcalIsoR03BkgEff_InclMu";
  files.clear();
  weight.clear();
  files.push_back("mit-MuonIsoStudy_InclMu5-50_hist.root");
  weight.push_back(1.0);
  //make2DEffPlot( files,weight, "hNMuonPassTrackIsoVsHcalIsoR03","hMuonTrackIsoVsHcalIsoEffR03", plotname,0.0,0.2);

  plotname = "MuonTrackIsoVsHcalIsoR03BkgEff_QCDDJ";
  files.clear();
  weight.clear();
  files.push_back("mit-MuonIsoStudy_QCDDJ30-50_hist.root");
  files.push_back("mit-MuonIsoStudy_QCDDJ50-80_hist.root");
  files.push_back("mit-MuonIsoStudy_QCDDJ170-230_hist.root");
  weight.push_back(94702500.0/ 32400);
  weight.push_back(12195900.0 / 6480);
  weight.push_back(48325.0 / 34560);
  //make2DEffPlot( files,weight, "hNMuonPassTrackIsoVsHcalIsoR03","hMuonTrackIsoVsHcalIsoEffR03", plotname);

  plotname = "MuonTrackIsoVsEcalPlusHcalIsoSignalEffR03";
  files.clear();
  weight.clear();
  files.push_back("mit-MuonIsoStudy_HWW_hist.root");
  weight.push_back(1.0);
  //make2DEffPlot( files,weight, "hNMuonPassTrackIsoVsEcalPlusHcalIsoR03","hMuonTrackIsoVsEcalPlusHcalIsoEffR03", plotname,0.5,1.0);

  plotname = "MuonTrackIsoVsEcalPlusHcalIsoR03BkgEff_InclMu";
  files.clear();
  weight.clear();
  files.push_back("mit-MuonIsoStudy_InclMu5-50_hist.root");
  weight.push_back(1.0);
  //make2DEffPlot( files,weight, "hNMuonPassTrackIsoVsEcalPlusHcalIsoR03","hMuonTrackIsoVsEcalPlusHcalIsoEffR03", plotname,0.0,0.05);

  plotname = "MuonTrackIsoVsEcalPlusHcalIsoR03BkgEff_QCDDJ";
  files.clear();
  weight.clear();
  files.push_back("mit-MuonIsoStudy_QCDDJ30-50_hist.root");
  files.push_back("mit-MuonIsoStudy_QCDDJ50-80_hist.root");
  files.push_back("mit-MuonIsoStudy_QCDDJ170-230_hist.root");
  weight.push_back(94702500.0/ 32400);
  weight.push_back(12195900.0 / 6480);
  weight.push_back(48325.0 / 34560);
  //make2DEffPlot( files,weight, "hNMuonPassTrackIsoVsEcalPlusHcalIsoR03","hMuonTrackIsoVsEcalPlusHcalIsoEffR03", plotname);


  plotname = "MuonTrackIsoVsOnePointFiveEcalPlusHcalIsoSignalEffR03";
  files.clear();
  weight.clear();
  files.push_back("mit-MuonIsoStudy_HWW_hist.root");
  weight.push_back(1.0);
  //make2DEffPlot( files,weight, "hNMuonPassTrackIsoVsOnePointFiveEcalPlusHcalIsoR03","hMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoEffR03", plotname,0.5,1.0);

  plotname = "MuonTrackIsoVsOnePointFiveEcalPlusHcalIsoR03BkgEff_InclMu";
  files.clear();
  weight.clear();
  files.push_back("mit-MuonIsoStudy_InclMu5-50_hist.root");
  weight.push_back(1.0);
  //make2DEffPlot( files,weight, "hNMuonPassTrackIsoVsOnePointFiveEcalPlusHcalIsoR03","hMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoEffR03", plotname,0.0,0.05);

  plotname = "MuonTrackIsoVsOnePointFiveEcalPlusHcalIsoR03BkgEff_QCDDJ";
  files.clear();
  weight.clear();
  files.push_back("mit-MuonIsoStudy_QCDDJ30-50_hist.root");
  files.push_back("mit-MuonIsoStudy_QCDDJ50-80_hist.root");
  files.push_back("mit-MuonIsoStudy_QCDDJ170-230_hist.root");
  weight.push_back(94702500.0/ 32400);
  weight.push_back(12195900.0 / 6480);
  weight.push_back(48325.0 / 34560);
  //make2DEffPlot( files,weight, "hNMuonPassTrackIsoVsOnePointFiveEcalPlusHcalIsoR03","hMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoEffR03", plotname);




//**************************************************************************************************
//
// 2D Efficiency Optimization Plots
//
//**************************************************************************************************

  plotname = "MuonTrackIsoVsEcalIsoR03EffOptimization_InclMu";
  files.clear();
  weight.clear();
  files.push_back("mit-MuonIsoStudy_InclMu5-50_hist.root");
  weight.push_back(1.0);
  //make2DEffOptimizationPlot( "mit-MuonIsoStudy_HWW_hist.root", files,weight, "hNMuonPassTrackIsoVsEcalIsoR03","hMuonTrackIsoVsEcalIsoEffR03", plotname, 0.0, 10.0);

  plotname = "MuonTrackIsoVsHcalIsoR03EffOptimization_InclMu";
  files.clear();
  weight.clear();
  files.push_back("mit-MuonIsoStudy_InclMu5-50_hist.root");
  weight.push_back(1.0);
  //make2DEffOptimizationPlot( "mit-MuonIsoStudy_HWW_hist.root",files,weight, "hNMuonPassTrackIsoVsHcalIsoR03","hMuonTrackIsoVsHcalIsoEffR03", plotname,0.0,10.0);

  plotname = "MuonTrackIsoVsEcalPlusHcalIsoR03EffOptimization_InclMu";
  files.clear();
  weight.clear();
  files.push_back("mit-MuonIsoStudy_InclMu5-50_hist.root");
  weight.push_back(1.0);
  //make2DEffOptimizationPlot( "mit-MuonIsoStudy_HWW_hist.root",files,weight, "hNMuonPassTrackIsoVsEcalPlusHcalIsoR03","hMuonTrackIsoVsEcalPlusHcalIsoEffR03", plotname,0.0,15.0);

  plotname = "MuonTrackIsoVsOnePointFiveEcalPlusHcalIsoR03EffOptimization_InclMu";
  files.clear();
  weight.clear();
  files.push_back("mit-MuonIsoStudy_InclMu5-50_hist.root");
  weight.push_back(1.0);
  //make2DEffOptimizationPlot( "mit-MuonIsoStudy_HWW_hist.root",files,weight, "hNMuonPassTrackIsoVsOnePointFiveEcalPlusHcalIsoR03","hMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoEffR03", plotname,0.0,15.0);


  return;




}

TH1F* getHisto(string filename, string histoname) {
  cerr << filename << " " << histoname << endl;
  TFile *tmpfile = new TFile(filename.c_str(),"READ"); assert(tmpfile);
  tmpfile->FindObjectAny("TAMTopModule");
  const char* histname = histoname.c_str();
  TH1F* tmpHist = (TH1F*) tmpfile->Get(histoname.c_str());  
  tmpHist->SetDirectory(0);
  tmpfile->Close();
  
  return tmpHist;
}

TH2F* get2DHisto(string filename, string histoname) {
  cerr << filename << " " << histoname << endl;
  TFile *tmpfile = new TFile(filename.c_str(),"READ"); assert(tmpfile);
  tmpfile->FindObjectAny("TAMTopModule");
  const char* histname = histoname.c_str();
  TH2F* tmpHist = (TH2F*) tmpfile->Get(histoname.c_str());
  tmpHist->SetDirectory(0);
  tmpfile->Close();
  return tmpHist;
}

TH3F* get3DHisto(string filename, string histoname) {
  TFile *tmpfile = new TFile(filename.c_str(),"READ"); assert(tmpfile);
  tmpfile->FindObjectAny("TAMTopModule");

  const char* histname = histoname.c_str();
  TH3F* tmpHist = (TH3F*) tmpfile->Get(histoname.c_str());  
  tmpHist->SetDirectory(0);
  tmpfile->Close();
  return tmpHist;
}

void make2DEffPlot( vector<string> inputFilenames, vector<double> weight, string NPassHistName, string EffHistName, string plotname, double zmin=0.0, double zmax=1.0, string plottype = "colz") {

  assert(inputFilenames.size() == weight.size());

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);

  TH2F *tmp = get2DHisto( inputFilenames[0],NPassHistName);        
  TH2F *NPassHist = tmp->Clone();
  TH2F *tmp1 = get2DHisto( inputFilenames[0],NPassHistName);        
  TH2F *EffHist = tmp1->Clone();
  
  //get the total number of muons for the denominator
  double NMuons = 0;
  for (int b=0; b<inputFilenames.size() ;b++) {
     TH2F *tmpNPassHist = get2DHisto( inputFilenames[b], NPassHistName);
     TH2F *tempeffHist = get2DHisto( inputFilenames[b], EffHistName);
     NMuons += tmpNPassHist->GetBinContent(tmpNPassHist->GetNbinsX()-1,tmpNPassHist->GetNbinsY()-1)/tempeffHist->GetBinContent(tmpNPassHist->GetNbinsX()-1,tmpNPassHist->GetNbinsY()-1) * weight[b];

     if (b==0) {
       NPassHist->Scale(weight[b]);
     }
     if (b>0) {
       NPassHist->Add(tmpNPassHist, weight[b]);
     }
  }
  
  for (int i=0;i<=EffHist->GetXaxis()->GetNbins();i++) {
    for (int j=0;j<=EffHist->GetYaxis()->GetNbins();j++) {      
      EffHist->SetBinContent(i,j,NPassHist->GetBinContent(i,j) / NMuons)  ;
    }
  }
    
  EffHist->Draw(plottype.c_str());
  EffHist->GetZaxis()->SetRangeUser(zmin, zmax);
  filename = plotname + ".gif";
  cv->SetLeftMargin(0.2);
  cv->SetRightMargin(0.15);
  cv->SaveAs(filename.c_str());
  cv->Delete();
}

void make2DEffOptimizationPlot( string signalFilename, vector<string> bkgFilenames, vector<double> bkgweight, string NPassHistName, string EffHistName, string plotname, double zmin=0.0, double zmax=1.0, string plottype = "colz") {

  assert(bkgFilenames.size() == bkgweight.size());

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);

  TH2F *tmp = get2DHisto( bkgFilenames[0],NPassHistName);        
  TH2F *BkgNPassHist = tmp->Clone();
  TH2F *tmp1 = get2DHisto( bkgFilenames[0],NPassHistName);        
  TH2F *BkgEffHist = tmp1->Clone();    
  
  //get the total number of muons for the denominator
  double NMuons = 0;
  for (int b=0; b<bkgFilenames.size() ;b++) {
     TH2F *tmpNPassHist = get2DHisto( bkgFilenames[b], NPassHistName);
     TH2F *tempeffHist = get2DHisto( bkgFilenames[b], EffHistName);
     NMuons += tmpNPassHist->GetBinContent(tmpNPassHist->GetNbinsX()-1,tmpNPassHist->GetNbinsY()-1)/tempeffHist->GetBinContent(tmpNPassHist->GetNbinsX()-1,tmpNPassHist->GetNbinsY()-1) * bkgweight[b];

     if (b==0) {
       BkgNPassHist->Scale(bkgweight[b]);
     }
     if (b>0) {
       BkgNPassHist->Add(tmpNPassHist, bkgweight[b]);
     }
  }

  for (int i=0;i<=BkgEffHist->GetXaxis()->GetNbins();i++) {
    for (int j=0;j<=BkgEffHist->GetYaxis()->GetNbins();j++) {      
      BkgEffHist->SetBinContent(i,j,BkgNPassHist->GetBinContent(i,j) / NMuons)  ;
    }
  }
  
  //get SignalEff histogram
  TH2F *SignalEffHist = get2DHisto( signalFilename, EffHistName);      
  TH2F *tempOpt = get2DHisto( signalFilename , EffHistName);        
  TH2F *OptimizationHist = tempOpt->Clone();

  for (int i=0;i<=OptimizationHist->GetXaxis()->GetNbins();i++) {
    for (int j=0;j<=OptimizationHist->GetYaxis()->GetNbins();j++) {    
      if (BkgEffHist->GetBinContent(i,j) > 0)
        OptimizationHist->SetBinContent(i,j,SignalEffHist->GetBinContent(i,j) / TMath::Sqrt(BkgEffHist->GetBinContent(i,j)));
    }
  }

  OptimizationHist->Draw(plottype.c_str());
  OptimizationHist->GetZaxis()->SetRangeUser(zmin, zmax);
  filename = plotname + ".gif";
  cv->SetLeftMargin(0.2);
  cv->SetRightMargin(0.15);
  cv->SaveAs(filename.c_str());
  cv->Delete();
}

void makeComparisonPlot( vector<string> inputFilenames, vector<string> histName,
                            vector<string> legendNames, vector<string> auxLegendNames, string plotname, double xlow = -999 , double xhigh = -999, double ylow = -999, double yhigh = -999, string plotstyle = "") {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);

  if (plotstyle == "logy")
    cv->SetLogy();

  TLegend *leg=0;

  leg = new TLegend(0.5,0.7,0.7,0.85);   
  leg->SetBorderSize(1);
  leg->SetTextSize(0.03);

  double Normalization;
  int ncolor = 0;
  for (int i=0; i < inputFilenames.size(); i++) {   
    for (int j=0;j < histName.size();j++) {

      TH1F *tmp1 = getHisto( inputFilenames[i],histName[j]);        
      if (!(xlow == -999 && xhigh == -999))
        tmp1->SetAxisRange(xlow,xhigh); 
      if (!(ylow == -999 && yhigh == -999)) {        
        tmp1->SetMinimum(ylow);
        tmp1->SetMaximum(yhigh);
      }

      tmp1->SetLineColor(colors[ncolor]); ncolor++;
      tmp1->SetFillStyle(0);
      tmp1->SetLineWidth(2);    

      if (i==0 && j==0) {
        Normalization = tmp1->Integral();
        tmp1->Draw();
      } else 
        tmp1->DrawNormalized("same",Normalization); 
      cerr << i <<  " " << j << endl;
      string tmpLegendName = legendNames[i]+auxLegendNames[j];
      cerr << i <<  " " << j << endl;
      cerr << tmpLegendName << endl;
      leg->AddEntry(tmp1, tmpLegendName.c_str(), "LP"); 
    }
  }

  leg->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}

void makeCutEfficiencyPlot( string signalfile, string HistName, string plotname, string cutDirection) {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);

  TH1F *signalHist = getHisto( signalfile, HistName);
  TH1F *signalEff = signalHist->Clone();
  double totalEvents = signalHist->Integral();
  assert (totalEvents > 0);

  for (int k=1;k<=signalHist->GetXaxis()->GetNbins();k++) {
    double NPass = 0;
    if (cutDirection == "below")
      NPass = signalHist->Integral(k,signalHist->GetXaxis()->GetNbins());
    else if (cutDirection == "above")
      NPass = signalHist->Integral(0,k);
    signalEff->SetBinContent(k,NPass/totalEvents);
  }

  signalEff->GetYaxis()->SetTitle("Cut Efficiency");
  signalEff->SetMaximum(1.0);
  signalEff->SetMinimum(0.0);
  signalEff->Draw();

  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}


void makeEfficiencyComparisonPlot( string signalfile, vector<string> bkgfile, vector<double> bkgweight, vector<string> effHistNames, vector<string> NPassHistNames,  vector<string> legendNames, string plotname, double xlow = "-999" , double xhigh = "-999" ) {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);

  TLegend *leg=0;

  leg = new TLegend(0.25,0.65,0.65,0.8);   
  leg->SetBorderSize(1);
  leg->SetTextSize(0.03);

  assert(bkgfile.size() == bkgweight.size());
  assert(bkgfile.size() > 0 );
  assert(effHistNames.size() == NPassHistNames.size());
  assert(NPassHistNames.size() == legendNames.size());

  for (int i=0; i < effHistNames.size(); i++) {   

    TH1F *signalEff = getHisto( signalfile, effHistNames[i]);
    //copy the binning structure from the eff histogram. then clone it.
    TH1F *tmpbkgeff = getHisto( bkgfile[0], effHistNames[0]);
    TH1F *bkgEff = tmpbkgeff->Clone();
    
    for (int k=1;k<=bkgEff->GetXaxis()->GetNbins();k++) {
      double NPass = 0;
      double NMuons = 0;
      for (int b=0; b<bkgfile.size() ;b++) {
        TH1F *bkgNPass = getHisto( bkgfile[b], NPassHistNames[i]);
        TH1F *tempbkgeff = getHisto( bkgfile[b], effHistNames[i]);
        
//         cerr << bkgNPass << " " << tempbkgeff << " " << bkgNPass->GetNbinsX() << " " 
//              << tempbkgeff->GetNbinsX() << endl;
//          cerr << "bkgfile: " << bkgfile[b] << " hist:" << effHistNames[i] 
//              << "   number of total events inconsistent  | NPass[1]=" << bkgNPass->GetBinContent(1)
//              << " Eff[1]=" << tempbkgeff->GetBinContent(1) << "  | NPass[last]=" 
//                << bkgNPass->GetBinContent(bkgNPass->GetNbinsX()-1) << " Eff[last]=" 
//                  << tempbkgeff->GetBinContent(bkgNPass->GetNbinsX()-1) 
//              << endl;

        // if (bkgNPass->GetBinContent(1)/tempbkgeff->GetBinContent(1) != bkgNPass->GetBinContent(bkgNPass->GetNbinsX()) / tempbkgeff->GetBinContent(bkgNPass->GetNbinsX())) {
//         cerr << "bkgfile: " << bkgfile[b] " hist:" << effHistNames[i] 
//                << "   number of total events inconsistent  | NPass[1]=" << bkgNPass->GetBinContent(1)
//                << " Eff[1]=" << tempbkgeff->GetBinContent(1) << "  | NPass[last]=" 
//                << bkgNPass->GetBinContent(bkgNPass->GetNbinsX()) << " Eff[last]=" 
//                << tempbkgeff->GetBinContent(bkgNPass->GetNbinsX()) << endl;
//         }   
         NMuons += bkgNPass->GetBinContent(bkgNPass->GetNbinsX()-1)/tempbkgeff->GetBinContent(bkgNPass->GetNbinsX()-1)*bkgweight[b];
         NPass += bkgNPass->GetBinContent(k) * bkgweight[b];
      }


      double tempeff = NPass / NMuons;
      bkgEff->SetBinContent(k,tempeff);
    }

    vector<double> eff;    
    vector<double> bkgeff;
    eff.clear();
    bkgeff.clear();

    for (int j=1; j <= signalEff->GetNbinsX();j++) {
      eff.push_back(signalEff->GetBinContent(j));
      bkgeff.push_back(bkgEff->GetBinContent(j));   
    }

    TGraph *EffVsRejection = new TGraph(signalEff->GetNbinsX(), (Double_t *)&(eff[0]), (Double_t *)&(bkgeff[0]));
    EffVsRejection->GetXaxis()->SetRangeUser(xlow,xhigh);
    EffVsRejection->SetMarkerStyle(kFullSquare);
    EffVsRejection->GetYaxis()->SetTitle("Bkg Efficiency");
    //EffVsRejection->GetYaxis()->SetLabelOffset(0.001, "Y");
    EffVsRejection->GetXaxis()->SetTitle("Signal Efficiency");
    EffVsRejection->SetMarkerColor(colors[i]);
    EffVsRejection->SetMarkerSize(0.75);

    if (i==0)
      EffVsRejection->Draw("AP");
    else 
      EffVsRejection->Draw("P");
    leg->AddEntry(EffVsRejection,legendNames[i].c_str(), "P");   
  }

  leg->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}


void makeEfficiencyOptimizationPlot( string signalfile, vector<string> bkgfile, vector<double> bkgweight, vector<string> effHistNames, vector<string> NPassHistNames,  vector<string> legendNames, string plotname, double xlow = "-999" , double xhigh = "-999" ) {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);

  TLegend *leg=0;

  leg = new TLegend(0.55,0.75,0.95,0.9);   
  leg->SetBorderSize(1);
  leg->SetTextSize(0.03);

  assert(bkgfile.size() == bkgweight.size());
  assert(bkgfile.size() > 0 );
  assert(effHistNames.size() == NPassHistNames.size());
  assert(NPassHistNames.size() == legendNames.size());

  for (int i=0; i < effHistNames.size(); i++) {   

    TH1F *signalEff = getHisto( signalfile, effHistNames[i]);
    //copy the binning structure from the eff histogram. then clone it.
    TH1F *tmpbkgeff = getHisto( bkgfile[0], effHistNames[0]);
    TH1F *bkgEff = tmpbkgeff->Clone();
    
    for (int k=1;k<=bkgEff->GetXaxis()->GetNbins();k++) {
      double NPass = 0;
      double NMuons = 0;
      for (int b=0; b<bkgfile.size() ;b++) {
        TH1F *bkgNPass = getHisto( bkgfile[b], NPassHistNames[i]);
        TH1F *tempbkgeff = getHisto( bkgfile[b], effHistNames[i]);
        
//         cerr << bkgNPass << " " << tempbkgeff << " " << bkgNPass->GetNbinsX() << " " 
//              << tempbkgeff->GetNbinsX() << endl;
//          cerr << "bkgfile: " << bkgfile[b] << " hist:" << effHistNames[i] 
//              << "   number of total events inconsistent  | NPass[1]=" << bkgNPass->GetBinContent(1)
//              << " Eff[1]=" << tempbkgeff->GetBinContent(1) << "  | NPass[last]=" 
//                << bkgNPass->GetBinContent(bkgNPass->GetNbinsX()-1) << " Eff[last]=" 
//                  << tempbkgeff->GetBinContent(bkgNPass->GetNbinsX()-1) 
//              << endl;

        // if (bkgNPass->GetBinContent(1)/tempbkgeff->GetBinContent(1) != bkgNPass->GetBinContent(bkgNPass->GetNbinsX()) / tempbkgeff->GetBinContent(bkgNPass->GetNbinsX())) {
//         cerr << "bkgfile: " << bkgfile[b] " hist:" << effHistNames[i] 
//                << "   number of total events inconsistent  | NPass[1]=" << bkgNPass->GetBinContent(1)
//                << " Eff[1]=" << tempbkgeff->GetBinContent(1) << "  | NPass[last]=" 
//                << bkgNPass->GetBinContent(bkgNPass->GetNbinsX()) << " Eff[last]=" 
//                << tempbkgeff->GetBinContent(bkgNPass->GetNbinsX()) << endl;
//         }   
         NMuons += bkgNPass->GetBinContent(bkgNPass->GetNbinsX()-1)/tempbkgeff->GetBinContent(bkgNPass->GetNbinsX()-1)*bkgweight[b];
         NPass += bkgNPass->GetBinContent(k) * bkgweight[b];
      }


      double tempeff = NPass / NMuons;
      bkgEff->SetBinContent(k,tempeff);
    }

    vector<double> eff;    
    vector<double> bkgeff;
    vector<double> significance;
    eff.clear();
    bkgeff.clear();
    significance.clear();

    for (int j=1; j <= signalEff->GetNbinsX();j++) {
      eff.push_back(signalEff->GetBinContent(j));
      bkgeff.push_back(bkgEff->GetBinContent(j));   
      if (bkgEff->GetBinContent(j) > 0)
        significance.push_back(signalEff->GetBinContent(j) / TMath::Sqrt(bkgEff->GetBinContent(j)));
      else 
        significance.push_back(0.0);
    }

    TGraph *EffVsSignificance = new TGraph(signalEff->GetNbinsX(), (Double_t *)&(eff[0]), (Double_t *)&(significance[0]));
    EffVsSignificance->GetXaxis()->SetRangeUser(xlow,xhigh);
    EffVsSignificance->SetMarkerStyle(kFullSquare);
    EffVsSignificance->GetYaxis()->SetTitle("Bkg Efficiency");
    EffVsSignificance->GetYaxis()->SetRangeUser(0.0,25);
    //EffVsSignificance->GetYaxis()->SetLabelOffset(0.001, "Y");
    EffVsSignificance->GetXaxis()->SetTitle("Signal Efficiency");
    EffVsSignificance->SetMarkerColor(colors[i]);
    EffVsSignificance->SetMarkerSize(0.75);

    if (i==0)
      EffVsSignificance->Draw("AP");
    else 
      EffVsSignificance->Draw("P");
    leg->AddEntry(EffVsSignificance,legendNames[i].c_str(), "P");   
  }

  leg->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}


void makeGeneratorPlots( string signalFilename ) {
  
  string filename = "";
  string plotname = "";

  plotname = "VBFFwdQuarkEta";
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);  
  TH1F *tmp = getHisto( signalFilename,"hFwdQuarkEta");  
  tmp->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "VBFFwdQuarkPt";
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);  
  TH1F *tmp = getHisto( signalFilename,"hFwdQuarkPt");  
  tmp->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "VBFFwdQuarkDeltaEta";
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);  
  TH1F *tmp = getHisto( signalFilename,"hFwdQuarkDeltaEta");  
  tmp->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "VBFFwdQuarkDijetMass";
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);  
  TH1F *tmp = getHisto( signalFilename,"hFwdQuarkDijetMass");  
  tmp->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "VBFHWWEventClassification";
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);  
  TH1F *tmp = getHisto( signalFilename,"hMCEventClassification");  
  tmp->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

}
