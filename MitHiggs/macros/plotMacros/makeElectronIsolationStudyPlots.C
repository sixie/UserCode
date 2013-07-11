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

int  colors[] = { 2,4,6,7,8,kOrange,kBlack,kGray,kYellow,kAzure,kViolet,
                  kRed-1,kRed-2,kRed-3,kRed-4,kRed-5,kRed-6,kRed-7,kRed-8,
                  kWhite
};

//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void makeElectronIsolationStudyPlots()
{
  //Make Plots for Jet Validation
  gStyle->SetPalette(1);

  bool     doLeg   = true;
  double   legx[2] = { 0.5, 0.75 };
  double   legy[2] = { 0.7, 0.8 };
  double   legTextSize = 0.03;
  TString legTitle = "";

//**************************************************************************************************
// Define Special Label
//**************************************************************************************************
  string PtCutLabel = "FullPtRange_" ;
  //string PtCutLabel = "PtRange25ToInf_" ;

//**************************************************************************************************
// Define signal histogram files
//**************************************************************************************************
  string signalFilename = "/home/sixie/hist/ElectronIsolationStudy/filler/006/ElectronIsolationStudy_" + PtCutLabel + "s8-zee-id9.root";

//**************************************************************************************************
// Compute Signal Weights
//**************************************************************************************************
  TH1F *NEvents = getHisto( signalFilename, "hDEvents");
  double signalWeight = 1233.0 * 0.701 / (0.0 + NEvents->Integral());
  cerr << "signal NEvents: " << NEvents->Integral() << " weight: " << signalWeight << endl;



//**************************************************************************************************
// Define bkg histogram files
//**************************************************************************************************
  string QCDBkgFilename = "/home/sixie/hist/ElectronIsolationStudy/filler/006/ElectronIsolationStudy_" + PtCutLabel + "s8-qcdbc_30_80-id9.root";

  vector <string> bkgfiles;
  vector<double> bkgCrossSection;
  vector<double> bkgweight;
  bkgfiles.clear();
  bkgweight.clear();  
  bkgfiles.push_back("/home/sixie/hist/ElectronIsolationStudy/filler/006/ElectronIsolationStudy_" + PtCutLabel + "s8-qcdbc_20_30-id9.root");
  //bkgfiles.push_back("/home/sixie/hist/ElectronIsolationStudy/filler/006/ElectronIsolationStudy_" + PtCutLabel + "s8-qcdbc_30_80-id9.root");
  //bkgfiles.push_back("/home/sixie/hist/ElectronIsolationStudy/filler/006/ElectronIsolationStudy_" + PtCutLabel + "s8-qcdbc_80_170-id9.root");
  bkgfiles.push_back("/home/sixie/hist/ElectronIsolationStudy/filler/006/ElectronIsolationStudy_" + PtCutLabel + "s8-qcdem_20_30-id9.root");
  //bkgfiles.push_back("/home/sixie/hist/ElectronIsolationStudy/filler/006/ElectronIsolationStudy_" + PtCutLabel + "s8-qcdem_30_80-id9.root");
  //bkgfiles.push_back("/home/sixie/hist/ElectronIsolationStudy/filler/006/ElectronIsolationStudy_" + PtCutLabel + "s8-qcdem_80_170-id9.root");
  bkgCrossSection.push_back(400000000.0*0.008);
   //bkgCrossSection.push_back(100000000.0*0.047);
   //bkgCrossSection.push_back(1900000.0*0.15);
   bkgCrossSection.push_back(400000000.0*0.00048);
   //bkgCrossSection.push_back(100000000.0*0.0024);
   //bkgCrossSection.push_back(1900000.0*0.012);

//**************************************************************************************************
// Compute Bkg Weights
//**************************************************************************************************
  assert(bkgfiles.size() == bkgCrossSection.size());
  for (int i=0;i<bkgfiles.size();i++) {
    TH1F *bkgNEvents = getHisto( bkgfiles[i], "hDEvents");
    bkgweight.push_back(bkgCrossSection[i] / (0.0 + bkgNEvents->Integral()));
    cerr << bkgfiles[i] << " : " << bkgNEvents->Integral() << " weight:" << bkgweight[i] << endl;
  }


  vector <string> QCDDJBkgFiles;
  vector<double> QCDDJBkgWeight;
  QCDDJBkgFiles.clear();
  QCDDJBkgWeight.clear();  
  QCDDJBkgFiles.push_back("mit-ElectronIsoStudy_QCDDJ30-50_hist.root");
  QCDDJBkgFiles.push_back("mit-ElectronIsoStudy_QCDDJ50-80_hist.root");
  QCDDJBkgFiles.push_back("mit-ElectronIsoStudy_QCDDJ170-230_hist.root");
  QCDDJBkgWeight.push_back(94702500.0/ 32400);
  QCDDJBkgWeight.push_back(12195900.0 / 6480);
  QCDDJBkgWeight.push_back(48325.0 / 34560);


  vector<string> LegendNames;
  vector<string> filenames;  
  filenames.clear();
  LegendNames.clear();
//   filenames.push_back("mit-ElectronIsoStudy_HWW_hist.root");
//   filenames.push_back("mit-ElectronIsoStudy_QCDEM30-80_hist.root");
  filenames.push_back("ElectronIsolationStudy_FullPtRange_s8-zee-id9.root");
  filenames.push_back("ElectronIsolationStudy_FullPtRange_s8-qcdbc_30_80-id9.root");
  LegendNames.push_back("Zee");
  LegendNames.push_back("QCD BC");

  string sample = "";
  string plotname;
  string filename;

  string filename1 = "mit-JetValidation-hist.root";


//**************************************************************************************************
//
// Isolation Plots
//
//**************************************************************************************************
  plotname = "ElectronTrackIso";
  //makeComparisonPlot( filenames, "hElectronTrackIso", LegendNames, plotname,-999,-999, "logy");

  plotname = "ElectronCaloIso";
  //makeComparisonPlot( filenames, "hElectronCaloIso", LegendNames, plotname,-999,-999, "logy");

  plotname = "ElectronCaloTowerIso";
  //makeComparisonPlot( filenames, "hElectronCaloTowerIso", LegendNames, plotname,-999,-999, "logy");

  plotname = "ElectronEcalJurassicIso";
  //makeComparisonPlot( filenames, "hElectronEcalJurassicIso", LegendNames, plotname,-999,-999, "logy");

  plotname = "ElectronHcalJurassicIso";
  //makeComparisonPlot( filenames, "hElectronHcalJurassicIso", LegendNames, plotname,-999,-999, "logy");

  plotname = "ElectronTotalJurassicIso";
  //makeComparisonPlot( filenames, "hElectronTotalJurassicIso", LegendNames, plotname,-999,-999, "logy");

  plotname = "ElectronTrackIsoPlusCaloIso";
 //  makeComparisonPlot( filenames, "hElectronTrackIsoPlusCaloIso", LegendNames, plotname,0,200, "logy");

  plotname = "ElectronTrackIsoPlusCaloTowerIso";
 //  makeComparisonPlot( filenames, "hElectronTrackIsoPlusCaloTowerIso", LegendNames, plotname,0,200, "logy");

  plotname = "ElectronTrackIsoPlusJurassicIso";
 //  makeComparisonPlot( filenames, "hElectronTrackIsoPlusJurassicIso", LegendNames, plotname,0,200, "logy");

  plotname = "ElectronTwoTimesTrackIsoPlusCaloTowerIso";
//   makeComparisonPlot( filenames, "hElectronTwoTimesTrackIsoPlusCaloTowerIso", LegendNames, plotname,0,200, "logy");
  
  plotname = "ElectronTwoTimesTrackIsoPlusJurassicIso";
//   makeComparisonPlot( filenames, "hElectronTwoTimesTrackIsoPlusJurassicIso", LegendNames, plotname,0,200, "logy");

//**************************************************************************************************
//
// Isolation Over Pt Plots
//
//**************************************************************************************************

  plotname = "ElectronTrackIsoOverPt";
  //makeComparisonPlot( filenames, "hElectronTrackIsoOverPt", LegendNames, plotname,-999,-999, "logy");

  plotname = "ElectronCaloIsoOverPt";
  //makeComparisonPlot( filenames, "hElectronCaloIsoOverPt", LegendNames, plotname,-999,-999, "logy");

  plotname = "ElectronCaloTowerIsoOverPt";
  //makeComparisonPlot( filenames, "hElectronCaloTowerIsoOverPt", LegendNames, plotname,-999,-999, "logy");

  plotname = "ElectronEcalJurassicIsoOverPt";
  //makeComparisonPlot( filenames, "hElectronEcalJurassicIsoOverPt", LegendNames, plotname,-999,-999, "logy");

  plotname = "ElectronHcalJurassicIsoOverPt";
  //makeComparisonPlot( filenames, "hElectronHcalJurassicIsoOverPt", LegendNames, plotname,-999,-999, "logy");

  plotname = "ElectronTotalJurassicIsoOverPt";
  //makeComparisonPlot( filenames, "hElectronTotalJurassicIsoOverPt", LegendNames, plotname,-999,-999, "logy");

  plotname = "ElectronTrackIsoPlusCaloIsoOverPt";
  //makeComparisonPlot( filenames, "hElectronTrackIsoPlusCaloIsoOverPt", LegendNames, plotname,0,200, "logy");

  plotname = "ElectronTrackIsoPlusCaloTowerIsoOverPt";
  //makeComparisonPlot( filenames, "hElectronTrackIsoPlusTowerCaloIsoOverPt", LegendNames, plotname,0,200, "logy");

  plotname = "ElectronTrackIsoPlusJurassicIsoOverPt";
  //makeComparisonPlot( filenames, "hElectronTrackIsoPlusJurassicIsoOverPt", LegendNames, plotname,0,200, "logy");

  plotname = "ElectronTwoTimesTrackIsoPlusCaloTowerIsoOverPt";
  //makeComparisonPlot( filenames, "hElectronTwoTimesTrackIsoPlusCaloTowerIsoOverPt", LegendNames, plotname,0,200, "logy");
  
  plotname = "ElectronTwoTimesTrackIsoPlusJurassicIsoOverPt";
  //makeComparisonPlot( filenames, "hElectronTwoTimesTrackIsoPlusJurassicIsoOverPt", LegendNames, plotname,0,200, "logy");

//**************************************************************************************************
//
// Efficiency Plots
//
//**************************************************************************************************


//Define isolation variables for comparison
  vector<string> histnames;
  vector<string> npassnames;
  vector<string> isolationnames;
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
   histnames.push_back("hElectronTrackIsoCutEff");
  histnames.push_back("hElectronCaloIsoCutEff");
    histnames.push_back("hElectronCaloTowerIsoCutEff");
    histnames.push_back("hElectronEcalJurassicIsoCutEff");
    histnames.push_back("hElectronHcalIsoCutEff");
    histnames.push_back("hElectronTotalJurassicIsoCutEff");
   histnames.push_back("hElectronTrackIsoPlusCaloIsoCutEff");
   histnames.push_back("hElectronTrackIsoPlusCaloTowerIsoCutEff");
  histnames.push_back("hElectronTwoTimesTrackIsoPlusCaloTowerIsoCutEff");
  histnames.push_back("hElectronTrackIsoPlusJurassicIsoCutEff");
  histnames.push_back("hElectronTwoTimesTrackIsoPlusJurassicIsoCutEff");
   npassnames.push_back("hNElectronPassTrackIsoCut");
  npassnames.push_back("hNElectronPassCaloIsoCut");
   npassnames.push_back("hNElectronPassCaloTowerIsoCut");
   npassnames.push_back("hNElectronPassEcalJurassicIsoCut");
   npassnames.push_back("hNElectronPassHcalIsoCut");
   npassnames.push_back("hNElectronPassTotalJurassicIsoCut");
  npassnames.push_back("hNElectronPassTrackIsoPlusCaloIsoCut");
  npassnames.push_back("hNElectronPassTrackIsoPlusCaloTowerIsoCut");
  npassnames.push_back("hNElectronPassTwoTimesTrackIsoPlusCaloTowerIsoCut");
  npassnames.push_back("hNElectronPassTrackIsoPlusJurassicIsoCut");
  npassnames.push_back("hNElectronPassTwoTimesTrackIsoPlusJurassicIsoCut");
   isolationnames.push_back("TrackIso");
  isolationnames.push_back("CaloIso");
   isolationnames.push_back("CaloTowerIso");
   isolationnames.push_back("Ecal Jurassic Iso");
   isolationnames.push_back("Hcal Iso");
   isolationnames.push_back("Ecal Jurassic + Hcal Iso");
  isolationnames.push_back("Track+CaloIso");
  isolationnames.push_back("Track+CaloTowerIso");
  isolationnames.push_back("2*Track+CaloTowerIso");
  isolationnames.push_back("Track+JurassicIso");
  isolationnames.push_back("2*Track+JurassicIso");

  plotname = "ElectronIsolationSignalVsBkgEff_QCDEM";
  //makeEfficiencyComparisonPlot( signalFilename,bkgfiles,bkgweight, histnames, npassnames, isolationnames, plotname, 0.0, 1.0, 0.0, 1.0);


  plotname = "ElectronIsolationSignalVsBkgEff_QCDDJ";
  //makeEfficiencyComparisonPlot( signalFilename, QCDDJBkgFiles,QCDDJBkgWeight, histnames,npassnames, isolationnames, plotname);

//************************************************************************************************
//
// Isolation Over Pt Efficiency Plots
//
//**************************************************************************************************


//Define isolation variables for comparison
  vector<string> histnames;
  vector<string> npassnames;
  vector<string> isolationnames;
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
//   histnames.push_back("hElectronTrackIsoCutEff");
//   histnames.push_back("hElectronTrackIsoPlusJurassicIsoCutEff");
//   histnames.push_back("hElectronTrackIsoOverPtCutEff");
//   histnames.push_back("hElectronCaloTowerIsoOverPtCutEff");
//   histnames.push_back("hElectronTrackIsoPlusCaloTowerIsoOverPtCutEff");
  histnames.push_back("hElectronTrackIsoPlusJurassicIsoOverPtCutEff");
//   npassnames.push_back("hNElectronPassTrackIsoCut");
//   npassnames.push_back("hNElectronPassTrackIsoPlusJurassicIsoCut");
//   npassnames.push_back("hNElectronPassTrackIsoOverPtCut");
//   npassnames.push_back("hNElectronPassCaloTowerIsoOverPtCut");
//   npassnames.push_back("hNElectronPassTrackIsoPlusCaloTowerIsoOverPtCut");
  npassnames.push_back("hNElectronPassTrackIsoPlusJurassicIsoOverPtCut");
//   isolationnames.push_back("TrackIso");
//   isolationnames.push_back("Track+JurassicIso");
//   isolationnames.push_back("TrackIso / Pt");
//   isolationnames.push_back("CaloTowerIso / Pt");
//   isolationnames.push_back("Track+CaloTowerIso / Pt");
  isolationnames.push_back("(Track+JurassicIso) / Pt");

  plotname = "ElectronIsolationOverPtSignalVsBkgEff_QCDEM";
  makeEfficiencyComparisonPlot( signalFilename,bkgfiles,bkgweight, histnames, npassnames, isolationnames, plotname, 0.0, 1.0, 0.0, 1.0);

  plotname = "ElectronIsolationOverPtSignalVsBkgEff_QCDDJ";
  //makeEfficiencyComparisonPlot( signalFilename, QCDDJBkgFiles,QCDDJBkgWeight, histnames,npassnames, isolationnames, plotname);


//**************************************************************************************************
//
// 1D Optimization Plots
//
//**************************************************************************************************

  plotname = "ElectronIsolationOptimization_QCDEM30-80";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hElectronTrackIsoCutEff");
//  histnames.push_back("hElectronCaloIsoCutEff");
  histnames.push_back("hElectronCaloTowerIsoCutEff");
  histnames.push_back("hElectronEcalJurassicIsoCutEff");
  histnames.push_back("hElectronHcalJurassicIsoCutEff");
//   histnames.push_back("hElectronTrackIsoPlusCaloIsoCutEff");
  histnames.push_back("hElectronTrackIsoPlusCaloTowerIsoCutEff");
//  histnames.push_back("hElectronTwoTimesTrackIsoPlusCaloTowerIsoCutEff");
  histnames.push_back("hElectronTrackIsoPlusJurassicIsoCutEff");
//  histnames.push_back("hElectronTwoTimesTrackIsoPlusJurassicIsoCutEff");
  npassnames.push_back("hNElectronPassTrackIsoCut");
//  npassnames.push_back("hNElectronPassCaloIsoCut");
  npassnames.push_back("hNElectronPassCaloTowerIsoCut");
  npassnames.push_back("hNElectronPassEcalJurassicIsoCut");
  npassnames.push_back("hNElectronPassHcalJurassicIsoCut");
//  npassnames.push_back("hNElectronPassTrackIsoPlusCaloIsoCut");
  npassnames.push_back("hNElectronPassTrackIsoPlusCaloTowerIsoCut");
//  npassnames.push_back("hNElectronPassTwoTimesTrackIsoPlusCaloTowerIsoCut");
  npassnames.push_back("hNElectronPassTrackIsoPlusJurassicIsoCut");
//  npassnames.push_back("hNElectronPassTwoTimesTrackIsoPlusJurassicIsoCut");
  isolationnames.push_back("TrackIso");
//  isolationnames.push_back("CaloIso");
  isolationnames.push_back("CaloTowerIso");
  isolationnames.push_back("Ecal Jurassic Iso");
  isolationnames.push_back("Hcal Jurassic Iso");
//  isolationnames.push_back("Track+CaloIso");
  isolationnames.push_back("Track+CaloTowerIso");
//  isolationnames.push_back("2*Track+CaloTowerIso");
  isolationnames.push_back("Track+JurassicIso");
//  isolationnames.push_back("2*Track+JurassicIso");
  //makeEfficiencyOptimizationPlot( "ElectronIsolationStudy_FullPtRange_s8-zee-id9_0000.root",bkgfiles,bkgweight, histnames, npassnames,isolationnames, plotname);


  plotname = "ElectronIsolationOverPtOptimization_QCDEM30-80";
  histnames.clear();
  npassnames.clear();
  isolationnames.clear();
  histnames.push_back("hElectronTrackIsoCutEff");
  histnames.push_back("hElectronTrackIsoPlusJurassicIsoCutEff");
  histnames.push_back("hElectronTrackIsoOverPtCutEff");
  histnames.push_back("hElectronCaloTowerIsoOverPtCutEff");
  histnames.push_back("hElectronTrackIsoPlusCaloTowerIsoOverPtCutEff");
  histnames.push_back("hElectronTrackIsoPlusJurassicIsoOverPtCutEff");
  npassnames.push_back("hNElectronPassTrackIsoCut");
  npassnames.push_back("hNElectronPassTrackIsoPlusJurassicIsoCut");
  npassnames.push_back("hNElectronPassTrackIsoOverPtCut");
  npassnames.push_back("hNElectronPassCaloTowerIsoOverPtCut");
  npassnames.push_back("hNElectronPassTrackIsoPlusCaloTowerIsoOverPtCut");
  npassnames.push_back("hNElectronPassTrackIsoPlusJurassicIsoOverPtCut");
  isolationnames.push_back("TrackIso");
  isolationnames.push_back("Track+JurassicIso");
  isolationnames.push_back("TrackIso / Pt");
  isolationnames.push_back("CaloTowerIso / Pt");
  isolationnames.push_back("Track+CaloTowerIso / Pt");
  isolationnames.push_back("(Track+JurassicIso) / Pt");
  //makeEfficiencyOptimizationPlot( "ElectronIsolationStudy_FullPtRange_s8-zee-id9_0000.root",bkgfiles,bkgweight, histnames, npassnames,isolationnames, plotname, 0.5, 1.0, 0.0, 15.0);


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
  plotname = "ElectronTrackAndCaloIsoSignalEff";
  files.clear();
  weight.clear();
  files.push_back("mit-ElectronIsoStudy_HWW_hist.root");
  weight.push_back(1.0);
  //make2DEffPlot( files,weight, "hNElectronPassTrackAndCaloIso","hElectronTrackAndCaloIsoCutEff", plotname,0.84,1.0);

  plotname = "ElectronTrackAndCaloIsoBkgEff_QCDEM";
  files.clear();
  weight.clear();
  files.push_back("mit-ElectronIsoStudy_QCDEM20-30_hist.root");
  weight.push_back(400000000.0/ 5000000);
  //make2DEffPlot( files,weight, "hNElectronPassTrackAndCaloIso", "hElectronTrackAndCaloIsoCutEff", plotname);

  plotname = "ElectronTrackAndCaloIsoBkgEff_QCDDJ";
  files.clear();
  weight.clear();
  files.push_back("mit-ElectronIsoStudy_QCDDJ30-50_hist.root");
  files.push_back("mit-ElectronIsoStudy_QCDDJ50-80_hist.root");
  files.push_back("mit-ElectronIsoStudy_QCDDJ170-230_hist.root");
  weight.push_back(94702500.0/ 32400);
  weight.push_back(12195900.0 / 6480);
  weight.push_back(48325.0 / 34560);
  //make2DEffPlot( files,weight, "hNElectronPassTrackAndCaloIso","hElectronTrackAndCaloIsoCutEff", plotname);

  //IMPOSE Electron ID
  //-------------------------------------------------------------------------------------------------

  plotname = "ElectronTrackAndCaloIsoSignalEff_ImposeElectronID";
  files.clear();
  weight.clear();
  files.push_back("mit-ElectronIsoStudy_HWW_ImposeElectronID_hist.root");
  weight.push_back(1.0);
  //make2DEffPlot( files,weight, "hNElectronPassTrackAndCaloIso","hElectronTrackAndCaloIsoCutEff", plotname,0.84,1.0);

  plotname = "ElectronTrackAndCaloIsoSignalEff_ImposeElectronID";
  files.clear();
  weight.clear();
  files.push_back("mit-ElectronIsoStudy_QCDEM20-30_ImposeElectronID_hist.root");
  weight.push_back(400000000.0/ 5000000);
  //make2DEffPlot( files,weight, "hNElectronPassTrackAndCaloIso", "hElectronTrackAndCaloIsoCutEff", plotname);

  plotname = "ElectronTrackAndCaloIsoBkgEff_QCDDJ_ImposeElectronID";
 files.clear();
  weight.clear();
  files.push_back("mit-ElectronIsoStudy_QCDDJ30-50_ImposeElectronID_hist.root");
  files.push_back("mit-ElectronIsoStudy_QCDDJ50-80_ImposeElectronID_hist.root");
  files.push_back("mit-ElectronIsoStudy_QCDDJ170-230_ImposeElectronID_hist.root");
  weight.push_back(94702500.0/ 32400);
  weight.push_back(12195900.0 / 6480);
  weight.push_back(48325.0 / 34560);
  //make2DEffPlot( files,weight, "hNElectronPassTrackAndCaloIso","hElectronTrackAndCaloIsoCutEff", plotname);  


//**************************************************************************************************
// Track Iso vs CaloTower Iso
//**************************************************************************************************
  plotname = "ElectronTrackAndCaloTowerIsoSignalEff";
  files.clear();
  weight.clear();
  files.push_back("mit-ElectronIsoStudy_HWW_hist.root");
  weight.push_back(1.0);
  //make2DEffPlot( files,weight, "hNElectronPassTrackAndCaloTowerIso","hElectronTrackAndCaloTowerIsoCutEff",plotname, 0.75, 1.0);

  plotname = "ElectronTrackAndCaloTowerIsoBkgEff_QCDEM";
  files.clear();
  weight.clear();
  files.push_back("mit-ElectronIsoStudy_QCDEM20-30_ImposeElectronID_hist.root");
  weight.push_back(400000000.0/ 5000000);
  //make2DEffPlot(  files,weight, "hNElectronPassTrackAndCaloTowerIso","hElectronTrackAndCaloTowerIsoCutEff", plotname);

  plotname = "ElectronTrackAndCaloTowerIsoBkgEff_QCDDJ";
  files.clear();
  weight.clear();
  files.push_back("mit-ElectronIsoStudy_QCDDJ30-50_hist.root");
  files.push_back("mit-ElectronIsoStudy_QCDDJ50-80_hist.root");
  files.push_back("mit-ElectronIsoStudy_QCDDJ170-230_hist.root");
  weight.push_back(94702500.0/ 32400);
  weight.push_back(12195900.0 / 6480);
  weight.push_back(48325.0 / 34560);
  //make2DEffPlot(  files,weight, "hNElectronPassTrackAndCaloTowerIso","hElectronTrackAndCaloTowerIsoCutEff", plotname);

  //IMPOSE Electron ID
  //-------------------------------------------------------------------------------------------------

  plotname = "ElectronTrackAndCaloTowerIsoSignalEff_ImposeElectronID";
  files.clear();
  weight.clear();
  files.push_back("mit-ElectronIsoStudy_HWW_ImposeElectronID_hist.root");
  weight.push_back(1.0);
  //make2DEffPlot( files,weight, "hNElectronPassTrackAndCaloTowerIso","hElectronTrackAndCaloTowerIsoCutEff",plotname, 0.75, 1.0);

  plotname = "ElectronTrackAndCaloTowerIsoBkgEff_QCDEM_ImposeElectronID";
  files.clear();
  weight.clear();
  files.push_back("mit-ElectronIsoStudy_QCDEM20-30_ImposeElectronID_hist.root");
  weight.push_back(400000000.0/ 5000000);
  //make2DEffPlot( files,weight, "hNElectronPassTrackAndCaloTowerIso","hElectronTrackAndCaloTowerIsoCutEff", plotname);

  plotname = "ElectronTrackAndCaloTowerIsoBkgEff_QCDDJ_ImposeElectronID";
  files.clear();
  weight.clear();
  files.push_back("mit-ElectronIsoStudy_QCDDJ30-50_ImposeElectronID_hist.root");
  files.push_back("mit-ElectronIsoStudy_QCDDJ50-80_ImposeElectronID_hist.root");
  files.push_back("mit-ElectronIsoStudy_QCDDJ170-230_ImposeElectronID_hist.root");
  weight.push_back(94702500.0/ 32400);
  weight.push_back(12195900.0 / 6480);
  weight.push_back(48325.0 / 34560);
  //make2DEffPlot( files,weight, "hNElectronPassTrackAndCaloTowerIso","hElectronTrackAndCaloTowerIsoCutEff", plotname);

  plotname = "ElectronTrackAndCaloTowerIsoSignalEffOverBkgEff";
  //make2DEffOptimizationPlot( "mit-ElectronIsoStudy_HWW_hist.root", "mit-ElectronIsoStudy_QCDDJ50-80_hist.root", "hTrackAndCaloTowerIsoCutEff", plotname, 0.0, 16.0, "colz");

//**************************************************************************************************
//
// 2D Efficiency OptimizationPlots
//
//**************************************************************************************************

  plotname = "ElectronTrackAndCaloIsoOptimization_QCDEM";
  files.clear();
  weight.clear();
  files.push_back("mit-ElectronIsoStudy_QCDEM20-30_hist.root");  
  weight.push_back(400000000.0/ 5000000);
  //make2DOptimizationPlot( "mit-ElectronIsoStudy_HWW_hist.root", files,weight, "hNElectronPassTrackAndCaloIso", "hElectronTrackAndCaloIsoCutEff", plotname, 0.9, 1.6);

  plotname = "ElectronTrackAndCaloIsoOptimization_QCDEM_ImposeElectronID";
  files.clear();
  weight.clear();
  files.push_back("mit-ElectronIsoStudy_QCDEM20-30_ImposeElectronID_hist.root");  
  weight.push_back(400000000.0/ 5000000);
  //make2DOptimizationPlot( "mit-ElectronIsoStudy_HWW_ImposeElectronID_hist.root", files,weight, "hNElectronPassTrackAndCaloIso", "hElectronTrackAndCaloIsoCutEff", plotname, 0.9, 1.6);

  plotname = "ElectronTrackAndCaloTowerIsoOptimization_QCDEM";
  files.clear();
  weight.clear();
  files.push_back("mit-ElectronIsoStudy_QCDEM20-30_hist.root");  
  weight.push_back(400000000.0/ 5000000);
  //make2DOptimizationPlot( "mit-ElectronIsoStudy_HWW_hist.root", files,weight, "hNElectronPassTrackAndCaloTowerIso","hElectronTrackAndCaloTowerIsoCutEff", plotname, 0.9, 1.8);

  plotname = "ElectronTrackAndCaloTowerIsoOptimization_QCDEM_ImposeElectronID";
  files.clear();
  weight.clear();
  files.push_back("mit-ElectronIsoStudy_QCDEM20-30_ImposeElectronID_hist.root");  
  weight.push_back(400000000.0/ 5000000);
  //make2DOptimizationPlot( "mit-ElectronIsoStudy_HWW_ImposeElectronID_hist.root", files,weight, "hNElectronPassTrackAndCaloTowerIso","hElectronTrackAndCaloTowerIsoCutEff", plotname, 0.9, 1.5);


//**************************************************************************************************
//
// Isolation vs Pt scatter plot
//
//**************************************************************************************************
  plotname = "ElectronTrackIsolationVsPtScatter";
  //makeIsolationVsPtScatterPlot( signalFilename, inclusiveMuonBkgFilename, "hElectronTrackIsoVsPt" , "Zmumu", "QCD_BC", plotname);

  plotname = "ElectronJurassicIsolationVsPtScatter";
  //makeIsolationVsPtScatterPlot( signalFilename, signalWeight, bkgfiles, bkgCrossSection, "hElectronJurassicIsoVsPt" , "Zee","QCD", plotname);

  plotname = "ElectronJurassicIsolationVsPtScatter";
  //makeIsolationVsPtScatterPlot( signalFilename, signalWeight, bkgfiles, bkgCrossSection, "hElectronTrackPlusJurassicIsoVsPt" , "Zee","QCD", plotname);

//**************************************************************************************************
//
// Calculate Sliding Isolation Cut Efficiency
//
//**************************************************************************************************

  vector <string> sampleFile;
  vector<double> sampleCrossSection;

  sampleFile.clear();
  sampleCrossSection.clear();  
  sampleFile.push_back("/home/sixie/hist/ElectronIsolationStudy/filler/006/ElectronIsolationStudy_" + PtCutLabel + "s8-zee-id9.root");
  sampleCrossSection.push_back(1233.0 * 0.701);
//   calculateHybridIsolationCutEfficiency(sampleFile, sampleCrossSection, "hElectronTrackPlusJurassicIsoVsPt", 25.0, 3.0, 8.0);
  calculateHybridIsolationCutEfficiency(sampleFile, sampleCrossSection, "hElectronTrackPlusJurassicIsoVsPt", 25.0, 10.0, 8.0);

  sampleFile.clear();
  sampleCrossSection.clear();  
//  sampleFile.push_back("/home/sixie/hist/ElectronIsolationStudy/filler/006/ElectronIsolationStudy_PtRange15ToInf_s8-qcdbc_30_80-id9.root");
//  sampleCrossSection.push_back(100000000.0*0.0024);

   sampleFile.push_back("/home/sixie/hist/ElectronIsolationStudy/filler/006/ElectronIsolationStudy_" + PtCutLabel + "s8-qcdbc_20_30-id9.root");
   //sampleFile.push_back("/home/sixie/hist/ElectronIsolationStudy/filler/006/ElectronIsolationStudy_" + PtCutLabel + "s8-qcdbc_30_80-id9.root");
   //sampleFile.push_back("/home/sixie/hist/ElectronIsolationStudy/filler/006/ElectronIsolationStudy_" + PtCutLabel + "s8-qcdbc_80_170-id9.root");
   sampleFile.push_back("/home/sixie/hist/ElectronIsolationStudy/filler/006/ElectronIsolationStudy_" + PtCutLabel + "s8-qcdem_20_30-id9.root");
   //sampleFile.push_back("/home/sixie/hist/ElectronIsolationStudy/filler/006/ElectronIsolationStudy_" + PtCutLabel + "s8-qcdem_30_80-id9.root");
   //sampleFile.push_back("/home/sixie/hist/ElectronIsolationStudy/filler/006/ElectronIsolationStudy_" + PtCutLabel + "s8-qcdem_80_170-id9.root");
   sampleCrossSection.push_back(400000000.0*0.008);
   //sampleCrossSection.push_back(100000000.0*0.047);
   //sampleCrossSection.push_back(1900000.0*0.15);
   sampleCrossSection.push_back(400000000.0*0.00048);
   //sampleCrossSection.push_back(100000000.0*0.0024);
   //sampleCrossSection.push_back(1900000.0*0.012);
//   calculateHybridIsolationCutEfficiency(sampleFile, sampleCrossSection, "hElectronTrackPlusJurassicIsoVsPt", 25.0, 3.0, 8.0);
  calculateHybridIsolationCutEfficiency(sampleFile, sampleCrossSection, "hElectronTrackPlusJurassicIsoVsPt", 25.0, 10.0, 8.0);

  return;


}

TH1F* getHisto(string filename, string histoname) {
  //cerr << filename << " " << histoname << endl;
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
  
  //get the total number of electrons for the denominator
  double NElectrons = 0;
  for (int b=0; b<inputFilenames.size() ;b++) {
     TH2F *tmpNPassHist = get2DHisto( inputFilenames[b], NPassHistName);
     TH2F *tempeffHist = get2DHisto( inputFilenames[b], EffHistName);
     NElectrons += tmpNPassHist->GetBinContent(tmpNPassHist->GetNbinsX()-1,tmpNPassHist->GetNbinsY()-1)/tempeffHist->GetBinContent(tmpNPassHist->GetNbinsX()-1,tmpNPassHist->GetNbinsY()-1) * weight[b];

     if (b==0) 
       NPassHist->Scale(weight[b]);
     if (b>0) {
       NPassHist->Add(tmpNPassHist, weight[b]);
     }
  }
  
//   for (int b=0; b<inputFilenames.size() ;b++) {
//     if (b>0) {
//       TH2F *tmpNPass = get2DHisto( inputFilenames[b], NPassHistName);
//       NPassHist->Add(tmpNPass, weight[b]);
//     }
//   }

  for (int i=0;i<=EffHist->GetXaxis()->GetNbins();i++) {
    for (int j=0;j<=EffHist->GetYaxis()->GetNbins();j++) {      
      EffHist->SetBinContent(i,j,NPassHist->GetBinContent(i,j) / NElectrons)  ;
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


void make2DOptimizationPlot(string signalFilename, vector<string> bkgFilenames, vector<double> bkgweight, string NPassHistName, string EffHistName, string plotname, double zmin=0.0, double zmax=1.0, string plottype = "colz") {

  assert(bkgFilenames.size() == bkgweight.size());

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);

  TH2F *tmp = get2DHisto( bkgFilenames[0],NPassHistName);        
  TH2F *BkgNPassHist = tmp->Clone();
  TH2F *tmp1 = get2DHisto( bkgFilenames[0],NPassHistName);        
  TH2F *BkgEffHist = tmp1->Clone();
  
  //get the total number of electrons for the denominator
  double NElectrons = 0;
  for (int b=0; b<bkgFilenames.size() ;b++) {
     TH2F *tmpNPassHist = get2DHisto( bkgFilenames[b], NPassHistName);
     TH2F *tempeffHist = get2DHisto( bkgFilenames[b], EffHistName);
     NElectrons += tmpNPassHist->GetBinContent(tmpNPassHist->GetNbinsX()-1,tmpNPassHist->GetNbinsY()-1)/tempeffHist->GetBinContent(tmpNPassHist->GetNbinsX()-1,tmpNPassHist->GetNbinsY()-1) * bkgweight[b];

     if (b==0) 
       BkgNPassHist->Scale(bkgweight[b]);
     if (b>0) {
       BkgNPassHist->Add(tmpNPassHist, bkgweight[b]);
     }
  }

  for (int i=0;i<=BkgEffHist->GetXaxis()->GetNbins();i++) {
    for (int j=0;j<=BkgEffHist->GetYaxis()->GetNbins();j++) {      
      BkgEffHist->SetBinContent(i,j,BkgNPassHist->GetBinContent(i,j) / NElectrons)  ;
    }
  }
    
//get SignalEff histogram
  TH2F *SignalEffHist = get2DHisto( signalFilename, EffHistName);      
  TH2F *tempOpt = get2DHisto( signalFilename , EffHistName);        
  TH2F *OptimizationHist = tempOpt->Clone();

  for (int i=0;i<=OptimizationHist->GetXaxis()->GetNbins();i++) {
    for (int j=0;j<=OptimizationHist->GetYaxis()->GetNbins();j++) {    
      if (BkgEffHist->GetBinContent(i,j) > 0) {
        OptimizationHist->SetBinContent(i,j,SignalEffHist->GetBinContent(i,j) / TMath::Sqrt(BkgEffHist->GetBinContent(i,j)));
        //cerr << i << " " << j << " " << SignalEffHist->GetBinContent(i,j) << " / " << TMath::Sqrt(BkgEffHist->GetBinContent(i,j)) << endl;
      }
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

// void make2DEffOptimizationPlot( string signalFilename, string bkgFilename, string histName, string plotname, double zmin=0.0, double zmax=1.0, string plottype = "colz") {

//   TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);

//   TH2F *signal = get2DHisto( signalFilename,histName);        
//   TH2F *bkg = get2DHisto( bkgFilename,histName);        

// //   TH2F *signalEffOverBkgEff = new TH2D("hsignalEffOverBkgEff",";Track Isolation Cut;CaloTower Isolation Cut",
// //              101,0,20.0,101,0,20.0);

//   TH2F *signalEffOverBkgEff = signal->Clone();
//   signalEffOverBkgEff->Divide(bkg);
//   signalEffOverBkgEff->Draw(plottype.c_str());
//   signalEffOverBkgEff->GetZaxis()->SetRangeUser(zmin, zmax);
//   filename = plotname + ".gif";
//   cv->SetLeftMargin(0.10);
//   cv->SetRightMargin(0.15);
//   cv->SaveAs(filename.c_str());
//   cv->Delete();
// }

void makeComparisonPlot( vector<string> inputFilenames, string histName,
                         vector<string> legendNames, string plotname, double xlow = "-999" , double xhigh = "-999", string plotstyle = "") {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);

  if (plotstyle == "logy")
    cv->SetLogy();

  TLegend *leg=0;

  leg = new TLegend(0.5,0.7,0.75,0.8);   
  leg->SetBorderSize(1);
  leg->SetTextSize(0.03);

  double Normalization;
  for (int i=0; i < inputFilenames.size(); i++) {   
    TH1F *tmp1 = getHisto( inputFilenames[i],histName);        
    if (!(xlow == -999 && xhigh == -999))
      tmp1->SetAxisRange(xlow,xhigh);

    tmp1->SetLineColor(colors[i]);
    tmp1->SetFillStyle(0);
    tmp1->SetLineWidth(2);    
    if (i==0) {
      Normalization = tmp1->Integral();
      tmp1->Draw();
    } else 
      tmp1->DrawNormalized("same",Normalization); 
    leg->AddEntry(tmp1, legendNames[i].c_str(), "LP"); 
  }

  leg->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}


void makeEfficiencyComparisonPlot( string signalfile, vector<string> bkgfile, vector<double> bkgweight, vector<string> effHistNames, vector<string> NPassHistNames,  vector<string> legendNames, string plotname, double xlow = -999 , double xhigh = -999, double ylow = -999 , double yhigh = -999) {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);

  TLegend *leg=0;

  leg = new TLegend(0.45,0.20,0.90,0.4);   
  leg->SetBorderSize(1);
  leg->SetTextSize(0.05);

  assert(bkgfile.size() == bkgweight.size());
  assert(bkgfile.size() > 0 );
  assert(effHistNames.size() == NPassHistNames.size());
  assert(NPassHistNames.size() == legendNames.size());

  int ncolor = 0;
  for (int i=0; i < effHistNames.size(); i++) {   

    TH1F *signalEff = getHisto( signalfile, effHistNames[i]);
    //copy the binning structure from the eff histogram. then clone it.
    TH1F *tmpbkgeff = getHisto( bkgfile[0], effHistNames[0]);
    TH1F *bkgEff = tmpbkgeff->Clone();
    
    for (int k=1;k<=bkgEff->GetXaxis()->GetNbins();k++) {
      double NPass = 0;
      double NElectrons = 0;
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
         NElectrons += bkgNPass->GetBinContent(bkgNPass->GetNbinsX()-1)/tempbkgeff->GetBinContent(bkgNPass->GetNbinsX()-1)*bkgweight[b];
         NPass += bkgNPass->GetBinContent(k) * bkgweight[b];
      }


      double tempeff = NPass / NElectrons;
      bkgEff->SetBinContent(k,tempeff);
    }

    vector<double> eff;    
    vector<double> bkgeff;
    eff.clear();
    bkgeff.clear();

    for (int j=1; j <= signalEff->GetNbinsX();j++) {
      eff.push_back(signalEff->GetBinContent(j));
      bkgeff.push_back(bkgEff->GetBinContent(j));   
      cerr << j << " " << signalEff->GetBinContent(j) << " " << bkgEff->GetBinContent(j) << endl;
    }
   
    TGraph *EffVsRejection = new TGraph(signalEff->GetNbinsX(), (Double_t *)&(bkgeff[0]), (Double_t *)&(eff[0]));

    if (!(xlow == -999 && xhigh == -999)) 
      EffVsRejection->GetXaxis()->SetRangeUser(xlow,xhigh);
    if (!(ylow == -999 && yhigh == -999)) {    
      EffVsRejection->SetMinimum(ylow);
      EffVsRejection->SetMaximum(yhigh);
    } else {
      EffVsRejection->SetMinimum(0.0);
      EffVsRejection->SetMaximum(1.0);
    }
    EffVsRejection->SetMarkerStyle(kFullSquare);
    EffVsRejection->GetYaxis()->SetTitle("Signal Efficiency");
    //EffVsRejection->GetYaxis()->SetLabelOffset(0.001, "Y");
    EffVsRejection->GetXaxis()->SetTitle("Bkg Efficiency");
    EffVsRejection->SetMarkerColor(colors[ncolor]);
    EffVsRejection->SetMarkerSize(0.75);
    ncolor++;

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


void makeEfficiencyOptimizationPlot( string signalfile, vector<string> bkgfile, vector<double> bkgweight, vector<string> effHistNames, vector<string> NPassHistNames,  vector<string> legendNames, string plotname, double xlow = "-999" , double xhigh = "-999", double ylow = "-999" , double yhigh = "-999") {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);

  TLegend *leg=0;

  leg = new TLegend(0.25,0.25,0.55,0.4);   
  leg->SetBorderSize(1);
  leg->SetTextSize(0.03);

  assert(bkgfile.size() == bkgweight.size());
  assert(bkgfile.size() > 0 );
  assert(effHistNames.size() == NPassHistNames.size());
  assert(NPassHistNames.size() == legendNames.size());

  int ncolor = 0;
  for (int i=0; i < effHistNames.size(); i++) {   

    TH1F *signalEff = getHisto( signalfile, effHistNames[i]);
    //copy the binning structure from the eff histogram. then clone it.
    TH1F *tmpbkgeff = getHisto( bkgfile[0], effHistNames[0]);
    TH1F *bkgEff = tmpbkgeff->Clone();
    
    for (int k=1;k<=bkgEff->GetXaxis()->GetNbins();k++) {
      double NPass = 0;
      double NElectrons = 0;
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
         NElectrons += bkgNPass->GetBinContent(bkgNPass->GetNbinsX()-1)/tempbkgeff->GetBinContent(bkgNPass->GetNbinsX()-1)*bkgweight[b];
         NPass += bkgNPass->GetBinContent(k) * bkgweight[b];
      }


      double tempeff = NPass / NElectrons;
      bkgEff->SetBinContent(k,tempeff);
    }

    vector<double> eff;    
    vector<double> significance;
    eff.clear();
    significance.clear();

    for (int j=1; j <= signalEff->GetNbinsX();j++) {
      eff.push_back(signalEff->GetBinContent(j));
      if (bkgEff->GetBinContent(j) > 0) 
        significance.push_back(signalEff->GetBinContent(j) / TMath::Sqrt(bkgEff->GetBinContent(j)));
      else 
        significance.push_back(0);
    }

    TGraph *EffVsRejection = new TGraph(signalEff->GetNbinsX(), (Double_t *)&(eff[0]), (Double_t *)&(significance[0]));
    EffVsRejection->GetXaxis()->SetRangeUser(xlow,xhigh);
    EffVsRejection->SetMarkerStyle(kFullSquare);
    EffVsRejection->GetYaxis()->SetTitle("Sig Eff / Sqrt (Bkg Eff)");
    EffVsRejection->GetYaxis()->SetRangeUser(ylow,yhigh);
    //EffVsRejection->GetYaxis()->SetLabelOffset(0.001, "Y");
    EffVsRejection->GetXaxis()->SetTitle("Signal Efficiency");
    EffVsRejection->SetMarkerColor(colors[ncolor]);
    EffVsRejection->SetMarkerSize(0.75);
    ncolor++;
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

void makeIsolationVsPtScatterPlot( string signalFilenames, double signalWeight, vector<string> bkgfile, vector<double> bkgCrossSection, string histName, string signalLegendName, string bkgLegendName, string plotname) {

  assert (bkgfile.size() == bkgCrossSection.size());
  assert(bkgfile.size() > 0);

  //Obtain weights for the background from cross section
  vector<double> bkgweight;
  for (int i=0;i<bkgfile.size();i++) {
    TH1F *NEvents = getHisto( bkgfile[i], "hDEvents");
    bkgweight.push_back(bkgCrossSection[i] / (0.0 + NEvents->Integral()));
    cerr << bkgfile[i] << " : " << NEvents->Integral() << " weight:" << bkgweight[i] << endl;
  }



  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);
  TLegend *leg=0;
  leg = new TLegend(0.7,0.2,0.9,0.4);   
  leg->SetBorderSize(1);
  leg->SetTextSize(0.03);

  TH2F *signalHist = get2DHisto( signalFilenames, histName);
  signalHist->Scale(signalWeight * 100);

  TH2F *tempBkgHist = get2DHisto( bkgfile[0], histName);  
  TH2F *bkgHist = tempBkgHist->Clone();

  //normalize weight to number of signal events. avoids weird behaviour of 2D histograms when the weights are < 1.0
  bkgHist->Scale(bkgweight[0] * 100);
  for (int i=1; i<bkgfile.size();i++) {
    TH2F *temp = get2DHisto( bkgfile[0], histName);
    bkgHist->Add(temp,bkgweight[0] * 100); 
  }
  
  signalHist->SetMarkerColor(colors[0]);
  bkgHist->SetMarkerColor(colors[1]);
  signalHist->SetMarkerSize(0.1);
  bkgHist->SetMarkerSize(0.1);

  signalHist->Draw("same");
  bkgHist->Draw("same");
  

  
  leg->AddEntry(signalHist,signalLegendName.c_str(), "P");
  leg->AddEntry(bkgHist,bkgLegendName.c_str(), "P");
  leg->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}


// void calculateHybridIsolationCutEfficiency( vector<string> files, vector<double> crossSection, string histName, double ptThreshold, double isolationCutAtZeroPt, double constantIsolationCut) {
void calculateHybridIsolationCutEfficiency( vector<string> files, vector<double> crossSection, string histName, double ptThreshold, double ptAtZeroIsolationCut, double constantIsolationCut) {

  assert (files.size() == crossSection.size());
  assert(files.size() > 0);
  vector<double> weight;

  for (int i=0;i<files.size();i++) {
    TH1F *NEvents = getHisto( files[i], "hDEvents");
    weight.push_back(crossSection[i] / (0.0 + NEvents->Integral()));
    cerr << files[i] << " : " << NEvents->Integral() << " weight:" << weight[i] << endl;
  }
 
  TH2F *tempBkgHist = get2DHisto( files[0], histName);
  TH2F *bkgHist = tempBkgHist->Clone();
  
  //normalize weight to number of signal events. avoids weird behaviour of 2D histograms when the weights are < 1.0
  bkgHist->Scale(weight[0] * 100);
  cerr << "weight : " << weight[0] << endl;
  for (int i=1; i<files.size();i++) {
    TH2F *temp = get2DHisto( files[0], histName);
    bkgHist->Add(temp,weight[0] * 100); 
  }
  
  double totalCount = 0;
  double passCount = 0;
  for (int i=0; i<=bkgHist->GetXaxis()->GetNbins()+1; i++) {
    for (int j=0; j<=bkgHist->GetYaxis()->GetNbins()+1; j++) {
      totalCount += bkgHist->GetBinContent(i,j);

      double pt = bkgHist->GetYaxis()->GetBinCenter(j);
      double isolation = bkgHist->GetXaxis()->GetBinCenter(i);

      //electron type
      // if ( (isolation < isolationCutAtZeroPt + pt*(constantIsolationCut - isolationCutAtZeroPt)/(ptThreshold)) ||
//            (isolation < constantIsolationCut && pt > ptThreshold) )
//         passCount += bkgHist->GetBinContent(i,j);  

      //Muon type
      if ( (pt <= ptThreshold && (isolation < (pt - ptAtZeroIsolationCut)*(constantIsolationCut)/(ptThreshold - ptAtZeroIsolationCut))) ||
           (pt > ptThreshold && isolation < constantIsolationCut)
           || isolation == 0.0 ) {
        passCount += bkgHist->GetBinContent(i,j);  
        //cerr << i << " " << j << " " << pt << " " << isolation << " " << (pt - ptAtZeroIsolationCut)*(constantIsolationCut)/(ptThreshold - ptAtZeroIsolationCut) << endl;
        
      }
   
    }
  }
  
  cout << "pass : " << passCount  << "  Total: " << totalCount << "  Eff:" 
       << passCount/ (0.0+totalCount) << endl;

}
