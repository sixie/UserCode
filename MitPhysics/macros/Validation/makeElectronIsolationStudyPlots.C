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

  vector<string> LegendNames;
  vector<string> filenames;  
  filenames.clear();
  LegendNames.clear();
  filenames.push_back("mit-ElectronIsoStudy_HWW_hist.root");
  filenames.push_back("mit-ElectronIsoStudy_QCDEM30-80_hist.root");
  LegendNames.push_back("HWW Real Electrons");
  LegendNames.push_back("QCD EMEnriched Fake");

  string sample = "";
  string plotname;
  string filename;

  string filename1 = "mit-JetValidation-hist.root";


//**************************************************************************************************
//
// Isolation Plots
//
//**************************************************************************************************
 //  plotname = "ElectronTrackIsoPlusCaloIso";
//   makeComparisonPlot( filenames, "hElectronTrackIsoPlusCaloIso", LegendNames, plotname,0,200, "logy");

//   plotname = "ElectronTwoTimesTrackIsoPlusCaloTowerIso";
//   makeComparisonPlot( filenames, "hElectronTwoTimesTrackIsoPlusCaloTowerIso", LegendNames, plotname,0,200, "logy");

//   plotname = "ElectronTwoTimesTrackIsoPlusJurassicIso";
//   makeComparisonPlot( filenames, "hElectronTwoTimesTrackIsoPlusJurassicIso", LegendNames, plotname,0,200, "logy");


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
  histnames.push_back("hElectronTrackIsoPlusCaloIsoCutEff");
  histnames.push_back("hElectronTrackIsoPlusCaloTowerIsoCutEff");
  histnames.push_back("hElectronTwoTimesTrackIsoPlusCaloTowerIsoCutEff");
//  histnames.push_back("hElectronTwoTimesTrackIsoPlusJurassicIsoCutEff");
  npassnames.push_back("hNElectronPassTrackIsoCut");
  npassnames.push_back("hNElectronPassCaloIsoCut");
  npassnames.push_back("hNElectronPassCaloTowerIsoCut");
  npassnames.push_back("hNElectronPassTrackIsoPlusCaloIsoCut");
  npassnames.push_back("hNElectronPassTrackIsoPlusCaloTowerIsoCut");
  npassnames.push_back("hNElectronPassTwoTimesTrackIsoPlusCaloTowerIsoCut");
//  npassnames.push_back("hNElectronPassTwoTimesTrackIsoPlusJurassicIsoCut");

  isolationnames.push_back("TrackIso");
  isolationnames.push_back("CaloIso");
  isolationnames.push_back("CaloTowerIso");
  isolationnames.push_back("Track+CaloIso");
  isolationnames.push_back("Track+CaloTowerIso");
  isolationnames.push_back("2*Track+CaloTowerIso");
//  isolationnames.push_back("2*Track+JurassicIso");

//define QCD samples with proper weights
  vector <string> bkgfiles;
  vector<double> bkgweight;

  plotname = "ElectronIsolationSignalVsBkgEff_QCDEM";
  bkgfiles.clear();
  bkgweight.clear();  
  bkgfiles.push_back("mit-ElectronIsoStudy_QCDEM20-30_hist.root");
  bkgweight.push_back(400000000.0/ 5000000);
  //makeEfficiencyComparisonPlot( "mit-ElectronIsoStudy_HWW_hist.root",bkgfiles,bkgweight, histnames, npassnames, isolationnames, plotname);

  plotname = "ElectronIsolationSignalVsBkgEff_QCDEM30-80_ImposeElectronID";
  bkgfiles.clear();
  bkgweight.clear();  
  bkgfiles.push_back("mit-ElectronIsoStudy_QCDEM20-30_ImposeElectronID_hist.root");
  bkgweight.push_back(400000000.0/ 5000000);
  //makeEfficiencyComparisonPlot( "mit-ElectronIsoStudy_HWW_ImposeElectronID_hist.root",bkgfiles,bkgweight, histnames, npassnames,isolationnames, plotname);


  plotname = "ElectronIsolationSignalVsBkgEff_QCDDJ";
  bkgfiles.clear();
  bkgweight.clear();  
  bkgfiles.push_back("mit-ElectronIsoStudy_QCDDJ30-50_hist.root");
  bkgfiles.push_back("mit-ElectronIsoStudy_QCDDJ50-80_hist.root");
  bkgfiles.push_back("mit-ElectronIsoStudy_QCDDJ170-230_hist.root");
  bkgweight.push_back(94702500.0/ 32400);
  bkgweight.push_back(12195900.0 / 6480);
  bkgweight.push_back(48325.0 / 34560);
  //makeEfficiencyComparisonPlot( "mit-ElectronIsoStudy_HWW_hist.root", bkgfiles,bkgweight, histnames,npassnames, isolationnames, plotname);


  plotname = "ElectronIsolationSignalVsBkgEff_QCDDJ_ImposeElectronID";
  bkgfiles.clear();
  bkgweight.clear();  
  bkgfiles.push_back("mit-ElectronIsoStudy_QCDDJ30-50_ImposeElectronID_hist.root");
  bkgfiles.push_back("mit-ElectronIsoStudy_QCDDJ50-80_ImposeElectronID_hist.root");
  bkgfiles.push_back("mit-ElectronIsoStudy_QCDDJ170-230_ImposeElectronID_hist.root");
  bkgweight.push_back(94702500.0/ 32400);
  bkgweight.push_back(12195900.0 / 6480);
  bkgweight.push_back(48325.0 / 34560);
  //makeEfficiencyComparisonPlot( "mit-ElectronIsoStudy_HWW_ImposeElectronID_hist.root",bkgfiles,bkgweight, histnames, npassnames, isolationnames, plotname);

//**************************************************************************************************
//
// 1D Optimization Plots
//
//**************************************************************************************************

  plotname = "ElectronIsolationSignalOptimization_QCDEM30-80_ImposeElectronID";
  bkgfiles.clear();
  bkgweight.clear();  
  bkgfiles.push_back("mit-ElectronIsoStudy_QCDEM20-30_ImposeElectronID_hist.root");
  bkgweight.push_back(400000000.0/ 5000000);
  makeEfficiencyOptimizationPlot( "mit-ElectronIsoStudy_HWW_ImposeElectronID_hist.root",bkgfiles,bkgweight, histnames, npassnames,isolationnames, plotname);

  
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

    tmp1->SetLineColor(2+i);
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


void makeEfficiencyComparisonPlot( string signalfile, vector<string> bkgfile, vector<double> bkgweight, vector<string> effHistNames, vector<string> NPassHistNames,  vector<string> legendNames, string plotname) {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);

  TLegend *leg=0;

  leg = new TLegend(0.25,0.60,0.55,0.8);   
  leg->SetBorderSize(1);
  leg->SetTextSize(0.03);

  assert(bkgfile.size() == bkgweight.size());
  assert(bkgfile.size() > 0 );
  assert(effHistNames.size() == NPassHistNames.size());
  assert(NPassHistNames.size() == legendNames.size());

  int ncolor = 2;
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
    }
   
    TGraph *EffVsRejection = new TGraph(signalEff->GetNbinsX(), (Double_t *)&(eff[0]), (Double_t *)&(bkgeff[0]));
    EffVsRejection->GetXaxis()->SetRangeUser(0.75,1.0);
    EffVsRejection->SetMarkerStyle(kFullSquare);
    EffVsRejection->GetYaxis()->SetTitle("Bkg Efficiency");
    //EffVsRejection->GetYaxis()->SetLabelOffset(0.001, "Y");
    EffVsRejection->GetXaxis()->SetTitle("Signal Efficiency");
    EffVsRejection->SetMarkerColor(ncolor);
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


void makeEfficiencyOptimizationPlot( string signalfile, vector<string> bkgfile, vector<double> bkgweight, vector<string> effHistNames, vector<string> NPassHistNames,  vector<string> legendNames, string plotname) {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);

  TLegend *leg=0;

  leg = new TLegend(0.25,0.25,0.55,0.4);   
  leg->SetBorderSize(1);
  leg->SetTextSize(0.03);

  assert(bkgfile.size() == bkgweight.size());
  assert(bkgfile.size() > 0 );
  assert(effHistNames.size() == NPassHistNames.size());
  assert(NPassHistNames.size() == legendNames.size());

  int ncolor = 2;
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
    EffVsRejection->GetXaxis()->SetRangeUser(0.75,1.0);
    EffVsRejection->SetMarkerStyle(kFullSquare);
    EffVsRejection->GetYaxis()->SetTitle("Sig Eff / Sqrt (Bkg Eff)");
    EffVsRejection->GetYaxis()->SetRangeUser(0,2);
    //EffVsRejection->GetYaxis()->SetLabelOffset(0.001, "Y");
    EffVsRejection->GetXaxis()->SetTitle("Signal Efficiency");
    EffVsRejection->SetMarkerColor(ncolor);
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
