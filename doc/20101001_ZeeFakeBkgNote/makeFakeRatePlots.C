//root -l -b -q makeFakeRatePlots.C+\(\)
 
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <string>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TF1.h>
#include <TF2.h>
#include <TH1F.h>
#include <TH2F.h>
#include <THStack.h>
#include <TMath.h>
#include <MitStyle.h>
#include <TGraphAsymmErrors.h>
#include "MitAna/Utils/interface/SimpleTable.h"
#include <vector>
#include <fstream>
#include "MitHiggs/Utils/interface/PlotUtils.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitCommon/DataFormats/interface/TH2DAsymErr.h"

using namespace std;
#endif

Double_t LUMINOSITY = 200.0;
//cross section table
mithep::SimpleTable xstab("$CMSSW_BASE/src/MitPhysics/data/xs.dat");  

//define colors
Int_t COLORS[10] = {kRed,kBlue,kMagenta,kCyan,kGreen,kOrange,kGreen+3,kRed+3,kBlue+3,kGray};
Int_t MARKERS[10] = {20,21,22,23,24,25,26,27,28,30};
Int_t SysCOLORS[10] = {kBlue,kMagenta,kRed,kCyan,kGreen,kOrange,kGreen+3,kRed+3,kBlue+3,kGray};

void makeEtaSliceComparisonPlots(   vector<mithep::TH2DAsymErr*> efficiencyGraphs,
                                    vector<string> efficiencyGraphLabels,
                                    string filename, string plotname,                                     
                                    string xAxisLabel = "" , string yAxisLabel = "", double maxY = -1,
                                    double legendX1 = -99, double legendY1 = -99, 
                                    double legendX2 = -99, double legendY2 = -99) {
   
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 900);
//   cv->Divide(2,3);

  int padIndex = 1;
    

  for(UInt_t j=1; j <= efficiencyGraphs[0]->GetYaxis()->GetNbins(); j++) {
  
    cout << "Start " << j << endl;
    char tmp[20]; 
    sprintf(tmp, "_Eta%.2fTo%.2f",  efficiencyGraphs[0]->GetYaxis()->GetBinLowEdge(j), 
            efficiencyGraphs[0]->GetYaxis()->GetBinUpEdge(j));
    string sliceEta = tmp;

    TLegend *leg1=0;
    if (legendX1 > -99) {
      leg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);   
    } else {
      leg1 = new TLegend(0.25,0.75,0.55,0.9);   
    }
    leg1->SetBorderSize(0);
    leg1->SetTextSize(0.03);
    
    vector<TH1D*> slices;
    for(int s=0; s < efficiencyGraphs.size(); ++s) {
      char tmp1[20];
      sprintf(tmp, "%d", s);
      string sliceString = tmp;

      cout << efficiencyGraphs[s]->GetName() << endl;
      TH1D *slice = efficiencyGraphs[s]->ProjectionX(("projectionX1" + sliceString).c_str() , j,j);
      slices.push_back(slice);
      cout << "here1 \n";

      slice->SetTitle((plotname + " " + sliceEta).c_str());

      if (xAxisLabel != "") slice->GetXaxis()->SetTitle(xAxisLabel.c_str());
      slice->GetXaxis()->SetTitleOffset(1.0);
      slice->GetXaxis()->SetLabelSize(0.03);
      if (yAxisLabel != "") slice->GetYaxis()->SetTitle(yAxisLabel.c_str());
      slice->GetYaxis()->SetTitleOffset(1.75);
      double MAXY = 0.04;
      if (maxY > 0) MAXY = maxY;
      slice->SetMaximum(MAXY);
      slice->SetMinimum(0.0);
      cout << "here2 \n";

      leg1->AddEntry(slice, efficiencyGraphLabels[s].c_str(), "LP"); 
      slice->SetMarkerColor(COLORS[s]);
      slice->SetLineColor(COLORS[s]);
      slice->SetMarkerSize(0.75);
      slice->SetMarkerStyle(MARKERS[s]);
      cout << "here3 \n";

      if (s==0) {
        slice->Draw();
      } else {
        slice->Draw("same");
      }
    }
    leg1->Draw();
    cv->SaveAs((filename + sliceEta + ".gif").c_str());
    cv->SaveAs((filename + sliceEta + ".eps").c_str());
  }

  return;
}

void makePtSliceComparisonPlots(   vector<mithep::TH2DAsymErr*> efficiencyGraphs,
                                    vector<string> efficiencyGraphLabels,
                                    string filename, string plotname, 
                                    string xAxisLabel = "" , string yAxisLabel = "", double maxY = -1,
                                    double legendX1 = -99, double legendY1 = -99, 
                                    double legendX2 = -99, double legendY2 = -99) {
   
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 900);
  cv->Divide(2,3);

  int padIndex = 1;

  for(UInt_t j=1; j <= efficiencyGraphs[0]->GetXaxis()->GetNbins(); j++) {
    char tmp[20]; 
    sprintf(tmp, "_Pt%.2fTo%.2f",  efficiencyGraphs[0]->GetXaxis()->GetBinLowEdge(j), 
            efficiencyGraphs[0]->GetXaxis()->GetBinUpEdge(j));
    string slicePt = tmp;

    TLegend *leg1=0;
    if (legendX1 > -99) {
      leg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);   
    } else {
      leg1 = new TLegend(0.25,0.75,0.55,0.9);   
    }
    leg1->SetBorderSize(0);
    leg1->SetTextSize(0.03);
    
    vector<TH1D*> slices;
    for(int s=0; s < efficiencyGraphs.size(); ++s) {
      char tmp1[20];
      sprintf(tmp, "%d", s);
      string sliceString = tmp;

      cout << efficiencyGraphs[s]->GetName() << endl;
      TH1D *slice = efficiencyGraphs[s]->ProjectionY(("projectionY1" + sliceString).c_str() , j,j);
      slices.push_back(slice);
      cout << "here1 \n";

      if (xAxisLabel != "") slice->GetXaxis()->SetTitle(xAxisLabel.c_str());
      slice->GetXaxis()->SetTitleOffset(1.0);
      slice->GetXaxis()->SetLabelSize(0.035);
      if (yAxisLabel != "") slice->GetYaxis()->SetTitle(yAxisLabel.c_str());
      slice->GetYaxis()->SetTitleOffset(1.75);
      slice->GetYaxis()->SetLabelSize(0.04);
      double MAXY = 0.04;
      if (maxY > 0) MAXY = maxY;
      slice->SetMaximum(MAXY);    
      slice->SetMinimum(0.0);
     
      leg1->AddEntry(slice, efficiencyGraphLabels[s].c_str(), "LP"); 
      slice->SetMarkerColor(COLORS[s]);
      slice->SetLineColor(COLORS[s]);
      slice->SetMarkerSize(0.75);
      slice->SetMarkerStyle(MARKERS[s]);

      if (s==0) {
        slice->Draw();
      } else {
        slice->Draw("same");
      }
    }
    leg1->Draw();      
    cv->SaveAs((filename + slicePt + ".gif").c_str());
    cv->SaveAs((filename + slicePt + ".eps").c_str());
  }  
 return;
}





void print2DHist( TH2F* fakeRateHistogram, string fakeRateLegendName) {
  double total = 0;

  cout.precision(3);
  cout << endl << fakeRateLegendName << endl;  

  //cout.setf(ios_base::left);
  cout.width(8);
  cout << "Axis" << " ";
  for (UInt_t k=0; int(k)<=fakeRateHistogram->GetYaxis()->GetNbins()+1 ; ++k) { 
    //cout.setf(ios_base::left);
      cout.width(8);
      cout << fakeRateHistogram->GetYaxis()->GetBinLowEdge(k) << " ";          
    }
  cout << endl;

  for (UInt_t j=0; int(j)<=fakeRateHistogram->GetXaxis()->GetNbins()+1 ; ++j) {    
    //cout.setf(ios_base::left);
    cout.width(5);
    cout << fakeRateHistogram->GetXaxis()->GetBinLowEdge(j) << " ";
    for (UInt_t k=0; int(k)<=fakeRateHistogram->GetYaxis()->GetNbins()+1 ; ++k) { 
      //cout.setf(ios_base::left);
      cout.width(8);
      char tempNumber[50];
      sprintf(tempNumber, "%.3e",fakeRateHistogram->GetBinContent(j,k));
      //cout << fakeRateHistogram->GetBinContent(j,k) << " ";          
      cout << tempNumber << " ";          
      total += fakeRateHistogram->GetBinContent(j,k);
    }
    cout << endl;
  }
  cout << "Total: " << total << endl;
  cout << endl;
}

//--------------------------------------------------------------------------------------------------
// Print out the fake rate matrix in pt and eta
//--------------------------------------------------------------------------------------------------
void printFakeRate( vector<TH2F*> fakeRateHistograms, vector<TF2*> fakeRateFits,
                        vector<string> fakeRateLegendNames , string plotname ) {

  for (UInt_t i=0; i<fakeRateHistograms.size() ; i++) { 
    print2DHist(fakeRateHistograms[i],fakeRateLegendNames[i]);
  } 
}






void print2DHist( mithep::TH2DAsymErr* histogram, string histLegendName, string outputFilename) {

  std::ofstream outputFile(outputFilename.c_str());

  Int_t NumOfColumns = histogram->GetYaxis()->GetNbins() - 1;
  Int_t NumOfRows = histogram->GetXaxis()->GetNbins() + 1;
  Int_t NumberOfSubTables = int(floor((double(NumOfColumns) - 1.00) / 7.00) + 1);

  cout << histLegendName << endl;
  cout << "NumberOfColumns = " << NumOfColumns << " , NumOfRows = " << NumOfRows << " NumberOfSubTables = " << NumberOfSubTables << endl;
  if (NumOfColumns > 8) NumOfColumns = 8; //max number of columns

  outputFile << "\\large \n" << histLegendName << endl;
  outputFile << "\\footnotesize \n";

  for (int s=0; s<NumberOfSubTables; ++s) {
    
    if (NumberOfSubTables > 1) {
      if (s < NumberOfSubTables - 1) {
        NumOfColumns = 8;
      } else {
        NumOfColumns = histogram->GetYaxis()->GetNbins() - s*7  + 1;
      }
    }
    cout << "s = " << s << " : NumOfColumns = " << histogram->GetYaxis()->GetNbins() << " - s*7 + 1 = " << NumOfColumns << endl;

    if (s < NumberOfSubTables - 1) {
      outputFile << "\\begin{tabular*}{\\textwidth}{|";
    } else {
      outputFile << "\\begin{tabular}{|";
    }
    for (Int_t i=0; i < NumOfColumns;++i) outputFile << "c|";
    outputFile << "}\\hline \n";
    
    outputFile << "$p_T$ [GeV/c] / $\\eta$ ";
    Int_t start_index = s*7+1;
    if (s == 0) start_index = 1;
    Int_t end_index = start_index + (NumOfColumns-1);
    for (Int_t i= start_index; i< end_index;++i) {
      char tmp[20]; 
      sprintf(tmp, "%.2f To %.2f", histogram->GetYaxis()->GetBinLowEdge(i), 
              histogram->GetYaxis()->GetBinUpEdge(i));
      string sliceEta = tmp;
      outputFile << " & " << sliceEta;
      cout << i << " ";
    }
    cout << endl;

    outputFile << " \\\\ \n";
    outputFile << " \\hline \n";
    for (Int_t i=1; i<NumOfRows;++i) { 
      char tmp[20]; 
      sprintf(tmp, "%.1f - %.1f",  histogram->GetXaxis()->GetBinLowEdge(i),
              histogram->GetXaxis()->GetBinUpEdge(i) );
      string slicePt = tmp;
      
      outputFile << slicePt;
      
      outputFile.precision(2);
      //outputFile.setf(ios_base::fixed);
      for (Int_t j=start_index; j < end_index;++j) {   
        char tmpNumber[20]; 
        sprintf(tmpNumber, "%.3e", histogram->GetBinContent(i,j) );        
        outputFile << " & " << tmpNumber;
      }        
      outputFile << " \\\\ \n";
    }
    outputFile << " \\hline \n";
    if (s < NumberOfSubTables - 1) {
      outputFile << "\\end{tabular*} \n";
    } else {
      outputFile << "\\end{tabular} \n";
    }
  }

  outputFile.close();

}



void plotEfficiency(TGraphAsymmErrors *eff1, string legend1, TGraphAsymmErrors *eff2, string legend2,
                    string plotname, 
                    Double_t xmin = -99, Double_t xmax = -99, 
                    Double_t ymin  = -99 , Double_t ymax = -99 ) {

  TCanvas *cv = MakeCanvas("cv", "cv", 800, 600);
  TLegend *leg = new TLegend(0.60,0.80,0.90,0.90);
  leg->SetBorderSize(0);  
  leg->SetTextSize(0.03);
  

  eff1->SetTitle("");
  eff2->SetTitle("");
  eff1->SetLineColor(kRed);
  eff1->SetMarkerColor(kRed);
  eff2->SetLineColor(kBlue);
  eff2->SetMarkerColor(kBlue);

  leg->Clear();
  leg->AddEntry(eff1, legend1.c_str(), "P");
  leg->AddEntry(eff2, legend2.c_str(), "P");

  if (xmin != -99 && xmax != -99) eff1->GetXaxis()->SetRangeUser(xmin,xmax);
  if (ymin != -99) eff1->SetMinimum(ymin);
  if (ymax != -99) eff1->SetMaximum(ymax);

  eff1->Draw("AP");
  eff2->Draw("Psame");
  leg->Draw();
  cv->SaveAs((plotname + ".gif").c_str());
   cv->SaveAs((plotname + ".eps").c_str());

}

void plotEfficiency(vector<TGraphAsymmErrors*> efficiencyGraphs, vector<string> efficiencyGraphLabels, 
                    string plotname, 
                    Double_t xmin = -99, Double_t xmax = -99, 
                    Double_t ymin  = -99 , Double_t ymax = -99 ) {

  assert(efficiencyGraphs.size() > 0);
  assert(efficiencyGraphs.size() == efficiencyGraphLabels.size());

  TCanvas *cv = MakeCanvas("cv", "cv", 800, 600);
  TLegend *leg = new TLegend(0.60,0.80,0.90,0.90);
  leg->SetBorderSize(0);  
  leg->SetTextSize(0.03);

  leg->Clear();
  for (int i=0; i<efficiencyGraphs.size() ; ++i) {
    efficiencyGraphs[i]->SetTitle("");
    efficiencyGraphs[i]->SetLineColor(COLORS[i]);
    efficiencyGraphs[i]->SetMarkerColor(COLORS[i]);
    efficiencyGraphs[i]->SetMarkerStyle(MARKERS[i]);
    efficiencyGraphs[i]->GetYaxis()->SetTitle("Fake Rate");
    leg->AddEntry(efficiencyGraphs[i], efficiencyGraphLabels[i].c_str(), "P");
  }

  if (xmin != -99 && xmax != -99) efficiencyGraphs[0]->GetXaxis()->SetRangeUser(xmin,xmax);
  if (ymin != -99) efficiencyGraphs[0]->SetMinimum(ymin);
  if (ymax != -99) efficiencyGraphs[0]->SetMaximum(ymax);

  for (int i=0; i<efficiencyGraphs.size() ; ++i) {
    if (i==0) efficiencyGraphs[i]->Draw("AP");
    else efficiencyGraphs[i]->Draw("Psame");
  }
  leg->Draw();
  cv->SaveAs((plotname + ".gif").c_str());
   cv->SaveAs((plotname + ".eps").c_str());

}


//--------------------------------------------------------------------------------------------------
// Make 2D Fake rate slice plots
//--------------------------------------------------------------------------------------------------
void make2DFakeRateSlicePlots() {

  //Load FakeRate file
  TFile *ElectronEfficiencyFile = new TFile("UserCode/sixie/doc/20100610_EGammaFakeRateNote/new/note/ElectronFakeRate_2010Data.20100913.root", "READ");
  ElectronEfficiencyFile->cd();
  
  vector<mithep::TH2DAsymErr*> efficiencyGraphs;
  vector<string> efficiencyGraphLabels;

  //*****************************************************************************************
  //Compare WJets Vs QCD FakeRates
  //*****************************************************************************************
  efficiencyGraphs.clear();
  efficiencyGraphLabels.clear();
  efficiencyGraphs.push_back((mithep::TH2DAsymErr*)ElectronEfficiencyFile->Get("RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator_data_Jet30_PtEta"));
  efficiencyGraphs.push_back((mithep::TH2DAsymErr*)ElectronEfficiencyFile->Get("RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator_QCD20MCv26a_Jet30_PtEta"));
  efficiencyGraphLabels.push_back("Data Jet30");
  efficiencyGraphLabels.push_back("QCD MC");

  for(int i=0; i<efficiencyGraphs.size(); ++i) {    
    assert(efficiencyGraphs[i]);
    cout << "found " << i << endl;
  }

  
  makePtSliceComparisonPlots(efficiencyGraphs, efficiencyGraphLabels,
                             "ElectronFakeRate_RecoDenominator_VBTF95WPIdIsoNumerator_PtSlices",                             
                             "VBTF95WP Electron Fake Rate", 
                             "electron #eta", "Fake Rate", 0.10, 0.35,0.75,0.70,0.9);
  
  
  print2DHist(efficiencyGraphs[0], efficiencyGraphLabels[0], "Jet30FakeRate.tex");
  print2DHist(efficiencyGraphs[1], efficiencyGraphLabels[1], "QCDFakeRate.tex");

  ElectronEfficiencyFile->Close();



  cout << "done\n";


  //Load FakeRate file
  ElectronEfficiencyFile = new TFile("UserCode/sixie/doc/20100610_EGammaFakeRateNote/new/note/ElectronJetFakeRate_2010Data.20100913.root", "READ");
  ElectronEfficiencyFile->cd();

  //*****************************************************************************************
  //Compare WJets Vs QCD FakeRates
  //*****************************************************************************************
  efficiencyGraphs.clear();
  efficiencyGraphLabels.clear();
  efficiencyGraphs.push_back((mithep::TH2DAsymErr*)ElectronEfficiencyFile->Get("RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator_data_Jet30_PtEta"));
  efficiencyGraphs.push_back((mithep::TH2DAsymErr*)ElectronEfficiencyFile->Get("RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator_QCD20MCv26a_Jet30_PtEta"));
  efficiencyGraphLabels.push_back("Data Jet30");
  efficiencyGraphLabels.push_back("QCD MC");

  for(int i=0; i<efficiencyGraphs.size(); ++i) {    
    assert(efficiencyGraphs[i]);
    cout << "found " << i << endl;
  }

  
  makePtSliceComparisonPlots(efficiencyGraphs, efficiencyGraphLabels,
                             "ElectronFakeRate_JetDenominator_VBTF95WPIdIsoNumerator_PtSlices",                             
                             "VBTF95WP Electron Fake Rate", 
                             "electron #eta", "Fake Rate", 0.003, 0.35,0.75,0.70,0.9);
  
  
  print2DHist(efficiencyGraphs[0], efficiencyGraphLabels[0], "Jet30FakeRate.tex");
  print2DHist(efficiencyGraphs[1], efficiencyGraphLabels[1], "QCDFakeRate.tex");

  ElectronEfficiencyFile->Close();



  return;

}



//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void makeFakeRatePlots() {

  string jetPtLabel = "";

  //*****************************************************************************************
  //Load MIT Style
  //*****************************************************************************************
  gROOT->Macro("$CMSSW_BASE/src/MitStyle.C+");

  mithep::PlotUtils *plotUtils = new mithep::PlotUtils();
  plotUtils->SetLuminosity(10.0);

  
  //*****************************************************************************************
  //Set up 
  //*****************************************************************************************
//   string plotname;
//   TCanvas *cv = MakeCanvas("cv", "cv", 800, 600);
//   TLegend *leg = new TLegend(0.60,0.80,0.95,0.90);
//   leg->SetBorderSize(0);  
//   leg->SetTextSize(0.03);

  TFile *ElectronEfficiencyFile = new TFile("UserCode/sixie/doc/20100610_EGammaFakeRateNote/new/note/ElectronFakeRate_2010Data.20100913.root", "READ");
  TGraphAsymmErrors *efficiencyData_Pt = 0;
  TGraphAsymmErrors *efficiencyData_Eta = 0;
  TGraphAsymmErrors *efficiencyData_Phi = 0;
  TGraphAsymmErrors *efficiencyMinbiasMC_Pt = 0;
  TGraphAsymmErrors *efficiencyMinbiasMC_Eta = 0;
  TGraphAsymmErrors *efficiencyMinbiasMC_Phi = 0;
  TGraphAsymmErrors *efficiencyWMC_Pt = 0;
  TGraphAsymmErrors *efficiencyWMC_Eta = 0;
  TGraphAsymmErrors *efficiencyWMC_Phi = 0;
  TGraphAsymmErrors *efficiencyQCD20MC_Pt = 0;
  TGraphAsymmErrors *efficiencyQCD20MC_Eta = 0;
  TGraphAsymmErrors *efficiencyQCD20MC_Phi = 0;
  TGraphAsymmErrors *efficiencyPhotonJet30MC_Pt = 0;
  TGraphAsymmErrors *efficiencyPhotonJet30MC_Eta = 0;
  TGraphAsymmErrors *efficiencyPhotonJet30MC_Phi = 0;

  vector<TGraphAsymmErrors*> efficiencyGraphs;
  vector<string> efficiencyGraphLabels;






  //*****************************************************************************************
  //1D Vs Pt
  //*****************************************************************************************
    
  efficiencyGraphs.clear();
  efficiencyGraphLabels.clear();
  efficiencyGraphs.push_back((TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator_data_Jet30" + jetPtLabel + "_Pt").c_str()));
  efficiencyGraphs.push_back((TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator_WMCv26a" + jetPtLabel + "_Pt").c_str()));
  efficiencyGraphs.push_back((TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator_QCD20MCv26a_Jet30" + jetPtLabel + "_Pt").c_str()));
  efficiencyGraphLabels.push_back("Data");
  efficiencyGraphLabels.push_back("W+Jets MC");
  efficiencyGraphLabels.push_back("QCD MC");

  plotEfficiency(efficiencyGraphs, efficiencyGraphLabels, 
                 "RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator" + jetPtLabel + "_Pt", 20, 80, 0, 0.05);






  //*****************************************************************************************
  //1D Vs Eta
  //*****************************************************************************************


  efficiencyData_Eta =  (TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator_data_Jet30" + jetPtLabel + "_Eta").c_str());
  efficiencyWMC_Eta =  (TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator_WMCv26a" + jetPtLabel + "_Eta").c_str());
  efficiencyQCD20MC_Eta =  (TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator_QCD20MCv26a_Jet30" + jetPtLabel + "_Eta").c_str());

  assert(efficiencyData_Eta);
  assert(efficiencyWMC_Eta);
  assert(efficiencyQCD20MC_Eta);

  efficiencyGraphs.clear();
  efficiencyGraphLabels.clear();
  efficiencyGraphs.push_back(efficiencyData_Eta);
  efficiencyGraphs.push_back(efficiencyWMC_Eta);
   efficiencyGraphs.push_back(efficiencyQCD20MC_Eta);
  efficiencyGraphLabels.push_back("Data");
   efficiencyGraphLabels.push_back("W+Jets MC");
  efficiencyGraphLabels.push_back("QCD MC");

  plotEfficiency(efficiencyGraphs, efficiencyGraphLabels, 
                 "RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator" + jetPtLabel + "_Eta", -99, -99, 0, 0.1);


  //*****************************************************************************************
  //1D Vs Phi
  //*****************************************************************************************

  efficiencyData_Phi =  (TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator_data_Jet30" + jetPtLabel + "_Phi").c_str());
  efficiencyWMC_Phi =  (TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator_WMCv26a" + jetPtLabel + "_Phi").c_str());
  efficiencyQCD20MC_Phi =  (TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator_QCD20MCv26a_Jet30" + jetPtLabel + "_Phi").c_str());

  assert(efficiencyData_Phi);
  assert(efficiencyWMC_Phi);
  assert(efficiencyQCD20MC_Phi);

  efficiencyGraphs.clear();
  efficiencyGraphLabels.clear();
  efficiencyGraphs.push_back(efficiencyData_Phi);
   efficiencyGraphs.push_back(efficiencyWMC_Phi);
   efficiencyGraphs.push_back(efficiencyQCD20MC_Phi);
  efficiencyGraphLabels.push_back("Data");
   efficiencyGraphLabels.push_back("W+Jets MC");
   efficiencyGraphLabels.push_back("QCD MC");

  plotEfficiency(efficiencyGraphs, efficiencyGraphLabels, 
                 "RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator" + jetPtLabel + "_Phi", -99, -99, 0, 0.05);





  //*****************************************************************************************
  //Jet Trigger Systematics
  //*****************************************************************************************

  efficiencyGraphs.clear();
  efficiencyGraphLabels.clear();
  efficiencyGraphs.push_back((TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator_data_Jet30" + jetPtLabel + "_Pt").c_str()));
  efficiencyGraphs.push_back((TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator_data_Jet15" + jetPtLabel + "_Pt").c_str()));
  efficiencyGraphLabels.push_back("Jet30");
  efficiencyGraphLabels.push_back("Jet15");
  plotEfficiency(efficiencyGraphs, efficiencyGraphLabels, 
                 "RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator_JetTriggerSystematics" + jetPtLabel + "_Pt", 20, 80, 0, 0.1);




















  ElectronEfficiencyFile = new TFile("UserCode/sixie/doc/20100610_EGammaFakeRateNote/new/note/ElectronJetFakeRate_2010Data.20100913.root", "READ");

 
  //*****************************************************************************************
  //1D Vs Pt
  //*****************************************************************************************
    
  efficiencyGraphs.clear();
  efficiencyGraphLabels.clear();
  efficiencyGraphs.push_back((TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator_data_Jet30" + jetPtLabel + "_Pt").c_str()));
  efficiencyGraphs.push_back((TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator_WMCv26a" + jetPtLabel + "_Pt").c_str()));
  efficiencyGraphs.push_back((TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator_QCD20MCv26a_Jet30" + jetPtLabel + "_Pt").c_str()));
  efficiencyGraphLabels.push_back("Data Jet30");
  efficiencyGraphLabels.push_back("W+Jets MC");
  efficiencyGraphLabels.push_back("QCD MC");

  plotEfficiency(efficiencyGraphs, efficiencyGraphLabels, 
                 "RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator" + jetPtLabel + "_Pt", 20, 80, 0, 0.003);






  //*****************************************************************************************
  //1D Vs Eta
  //*****************************************************************************************


  efficiencyData_Eta =  (TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator_data_Jet30" + jetPtLabel + "_Eta").c_str());
  efficiencyWMC_Eta =  (TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator_WMCv26a" + jetPtLabel + "_Eta").c_str());
  efficiencyQCD20MC_Eta =  (TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator_QCD20MCv26a_Jet30" + jetPtLabel + "_Eta").c_str());

  assert(efficiencyData_Eta);
  assert(efficiencyWMC_Eta);
  assert(efficiencyQCD20MC_Eta);

  efficiencyGraphs.clear();
  efficiencyGraphLabels.clear();
  efficiencyGraphs.push_back(efficiencyData_Eta);
  efficiencyGraphs.push_back(efficiencyWMC_Eta);
   efficiencyGraphs.push_back(efficiencyQCD20MC_Eta);
  efficiencyGraphLabels.push_back("Data");
   efficiencyGraphLabels.push_back("W+Jets MC");
  efficiencyGraphLabels.push_back("QCD MC");

  plotEfficiency(efficiencyGraphs, efficiencyGraphLabels, 
                 "RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator" + jetPtLabel + "_Eta", -99, -99, 0, 0.003);


  //*****************************************************************************************
  //1D Vs Phi
  //*****************************************************************************************

  efficiencyData_Phi =  (TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator_data_Jet30" + jetPtLabel + "_Phi").c_str());
  efficiencyWMC_Phi =  (TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator_WMCv26a" + jetPtLabel + "_Phi").c_str());
  efficiencyQCD20MC_Phi =  (TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator_QCD20MCv26a_Jet30" + jetPtLabel + "_Phi").c_str());

  assert(efficiencyData_Phi);
  assert(efficiencyWMC_Phi);
  assert(efficiencyQCD20MC_Phi);

  efficiencyGraphs.clear();
  efficiencyGraphLabels.clear();
  efficiencyGraphs.push_back(efficiencyData_Phi);
   efficiencyGraphs.push_back(efficiencyWMC_Phi);
   efficiencyGraphs.push_back(efficiencyQCD20MC_Phi);
  efficiencyGraphLabels.push_back("Data");
   efficiencyGraphLabels.push_back("W+Jets MC");
   efficiencyGraphLabels.push_back("QCD MC");

  plotEfficiency(efficiencyGraphs, efficiencyGraphLabels, 
                 "RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator" + jetPtLabel + "_Phi", -99, -99, 0, 0.002);





  //*****************************************************************************************
  //Jet Trigger Systematics
  //*****************************************************************************************

  efficiencyGraphs.clear();
  efficiencyGraphLabels.clear();
  efficiencyGraphs.push_back((TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator_data_Jet30" + jetPtLabel + "_Pt").c_str()));
  efficiencyGraphs.push_back((TGraphAsymmErrors*)ElectronEfficiencyFile->Get(("RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator_data_Jet15" + jetPtLabel + "_Pt").c_str()));
  efficiencyGraphLabels.push_back("Jet30");
  efficiencyGraphLabels.push_back("Jet15");
  plotEfficiency(efficiencyGraphs, efficiencyGraphLabels, 
                 "RecoElectronEfficiency_JetDenominator_VBTF95WPIdIsoNumerator_JetTriggerSystematics" + jetPtLabel + "_Pt", 20, 80, 0, 0.002);










  //*****************************************************************************************
  //2D Fake Rates
  //*****************************************************************************************
  make2DFakeRateSlicePlots();


}


