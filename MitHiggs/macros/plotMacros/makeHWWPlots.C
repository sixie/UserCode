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
void makeHWWPlots()
{
  //------------------------------------------------------------------------------------------------
  // Process the full set of dimuon mass histograms which come in predefined bins of the dimuon pT
  // to obtain in the end a distribution showing the measured dimuon mass versus its pT.
  //------------------------------------------------------------------------------------------------
  // access the histogram file and load TAMTopModule to load into memory
  gStyle->SetPalette(1);

  bool     doLeg   = true;
  double   legx[2] = { 0.5, 0.7 };
  double   legy[2] = { 0.6, 0.8 };
  double   legTextSize = 0.03;
  TString legTitle = "";

  string plotname;
  string filename;
  string specialLabel = "_SC5";
  
  string filename_signal = "mit-HWWSelection-003-hist_HWWSignal" + specialLabel + ".root";
  vector<string> filenames;
  vector<string> datasetnames;
vector<double> scaleFactors;
//   filenames.push_back("mit-HWWSelection-003-hist_HWWSignal" + specialLabel + ".root");
//   datasetnames.push_back("H165WW");
//   scaleFactors.push_back(0.325);
  filenames.push_back("mit-HWWSelection-003-hist_WWBkg" + specialLabel + ".root");
  datasetnames.push_back("WW");
  scaleFactors.push_back(7.517/9500);
  filenames.push_back("mit-HWWSelection-003-hist_ttbarBkg" + specialLabel + ".root");
  datasetnames.push_back("TTBAR");
  scaleFactors.push_back(836.5/108927);
  filenames.push_back("mit-HWWSelection-003-hist_WZBkg" + specialLabel + ".root");
  datasetnames.push_back("WZ");
  scaleFactors.push_back(0.585/9100);
  filenames.push_back("mit-HWWSelection-003-hist_ZZBkg" + specialLabel + ".root");
  datasetnames.push_back("ZZ");
  scaleFactors.push_back(0.22/8500);

  double scaleFactorSignal = 0.325/23300;

  assert(filenames.size() == datasetnames.size());
  assert(filenames.size() == scaleFactors.size());
  
  
  plotname = "HWWSelectionStacked";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, 
                  "HWW Signal", "hHWWSelection", plotname+specialLabel);

  plotname = "MtHiggsStacked";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, 
                  "HWW Signal", "hMtHiggs", plotname+specialLabel);

  plotname = "MetAfterCut";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, 
                  "HWW Signal", "hMetPtAfterSelectionHist", plotname+specialLabel);

  plotname = "NJets";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, 
                  "HWW Signal", "hNCentralJets", plotname+specialLabel);

  plotname = "DeltaPhiLeptons";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, 
                  "HWW Signal", "hDeltaPhiLeptons", plotname+specialLabel);

  plotname = "DileptonMass";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, 
                  "HWW Signal", "hDileptonMass", plotname+specialLabel);

  plotname = "Lepton1Pt";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, 
                  "HWW Signal", "hLepton1Pt_afterCuts", plotname+specialLabel);

  plotname = "Lepton2Pt";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, 
                  "HWW Signal", "hLepton2Pt_afterCuts", plotname+specialLabel);

  return;

}

TH1F* getHisto(string filename, string histoname) {
  TFile *tmpfile = new TFile(filename.c_str(),"READ"); assert(tmpfile);
  tmpfile->FindObjectAny("TAMTopModule");
  tmpfile->FindObjectAny("ObjectCleaning");
  tmpfile->FindObjectAny("HwwEvtSelMod");

  const char* histname = histoname.c_str();
  TH1F* tmpHist = (TH1F*) tmpfile->Get(histoname.c_str());  
  tmpHist->SetDirectory(0);
  tmpfile->Close();
  return tmpHist;
}


void makeStackedPlot(vector<string> bkgFilenames, vector<double> bkgScaleFactors, vector<string> bkgLegendNames, string signalFilename, double signalScaleFactor, string signalLegendName, string histName, string plotname) {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  THStack *hBkgStack = new THStack(plotname.c_str(),plotname.c_str());
  for (int i=0;i<bkgFilenames.size();i++) {   
    TH1F *tmp = getHisto( bkgFilenames[i],histName);
    if (tmp != NULL) {
      InitHist(tmp, "Xtitle", "Number of Events", 3+i)
      tmp->Scale(bkgScaleFactors[i]);
      tmp->SetFillStyle(1001);
      tmp->SetFillColor(3+i);
      tmp->SetLineWidth(1);  
      tmp->SetMinimum(0);
      hBkgStack->Add(tmp);
      cout << i << " " << bkgFilenames[i] << " " << tmp << " " << " " << hBkgStack->GetStack()->At(i) << endl;
    } else {
      cerr << "could not get histogram " << bkgFilenames[i] << ":hHWWSelection\n";
    }
    tmp = 0;
  }
  TH1F* hHWWSignal = getHisto( signalFilename,histName) ;
  hHWWSignal->Scale(signalScaleFactor);
  hHWWSignal->SetFillStyle(0);
  hHWWSignal->SetLineColor(2);
  hHWWSignal->SetLineWidth(2); 

  //make the plot
  //TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  hBkgStack->Draw();
  hHWWSignal->Draw("hist, same"); 
  TLegend *leg1=0;
  leg1 = new TLegend(0.5,0.6,0.7,0.8);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);
  leg1->AddEntry(hHWWSignal, signalLegendName.c_str(), "LP"); 
  for (int i=bkgFilenames.size()-1;i>= 0;i--)
    leg1->AddEntry(hBkgStack->GetStack()->At(i),bkgLegendNames[i].c_str(), "FP"); 
  leg1->Draw();
  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

}
