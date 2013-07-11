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
void makeWWPlots()
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
  string specialLabel = "_N-1_CaloJets";
  
  string filename_signal = "mit-WWSelection-004-hist_WW" + specialLabel + ".root";
  double scaleFactorSignal = 114.3*0.106*0.5/10000 * 1000;

  vector<string> filenames;
  vector<string> datasetnames;
  vector<double> scaleFactors;
  filenames.push_back("mit-WWSelection-004-hist_WW" + specialLabel + ".root");
  datasetnames.push_back("WW");
  scaleFactors.push_back(114.3*0.106*0.5/10000 * 1000);
  filenames.push_back("mit-WWSelection-004-hist_TTBAR" + specialLabel + ".root");
  datasetnames.push_back("TTBAR");
  scaleFactors.push_back(450*0.0123/20000 * 1000);
  filenames.push_back("mit-WWSelection-004-hist_Zee" + specialLabel + ".root");
  datasetnames.push_back("Zee");
  scaleFactors.push_back(1233*0.701/1000000 * 1000);
  filenames.push_back("mit-WWSelection-004-hist_Ztautau" + specialLabel + ".root");
  datasetnames.push_back("Ztautau");
  scaleFactors.push_back(1086/1000000 * 1000);
  filenames.push_back("mit-WWSelection-004-hist_Wmunu" + specialLabel + ".root");
  datasetnames.push_back("Wmunu");
  scaleFactors.push_back(11850*0.691/1000000 * 1000);
//   filenames.push_back("mit-WWSelection-004-hist_Wenu" + specialLabel + ".root");
//   datasetnames.push_back("Wenu");
//   scaleFactors.push_back(11850*0.738/1000000 * 1000);
  filenames.push_back("mit-WWSelection-004-hist_Wtaunu" + specialLabel + ".root");
  datasetnames.push_back("Wtaunu");
  scaleFactors.push_back(11850/1000000 * 1000);


  assert(filenames.size() == datasetnames.size());
  assert(filenames.size() == scaleFactors.size());
  
  
  plotname = "WWSelectionStacked";
//  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hHWWSelection", plotname+specialLabel);

  plotname = "LeptonPtMax";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonPtMax", plotname+specialLabel);

  plotname = "LeptonPtMin";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hLeptonPtMin", plotname+specialLabel);

  plotname = "Met";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPtHist", plotname+specialLabel);

  plotname = "DeltaPhiLeptons";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaPhiLeptons", plotname+specialLabel);

  plotname = "NJets";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCentralJets", plotname+specialLabel);

  plotname = "DileptonMass";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hDileptonMass", plotname+specialLabel);

  plotname = "METDeltaPhilEt";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hMETdeltaPhilEtHist", plotname+specialLabel);

  plotname = "NDirtyMuons";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNDirtyMuonsHist", plotname+specialLabel);

  plotname = "NCleanExtraTracksHist";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCleanExtraTracksHist", plotname+specialLabel);


//*************************************************************************************************
//ee final state Plots
//*************************************************************************************************
//   plotname = "WWSelectionStacked_ee";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hHWWSelection_ee", plotname+specialLabel);

//   plotname = "LeptonPtMax_ee";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonPtMax_ee", plotname+specialLabel);

//   plotname = "LeptonPtMin_ee";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hLeptonPtMin_ee", plotname+specialLabel);

//   plotname = "Met_ee";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPtHist_ee", plotname+specialLabel);

//   plotname = "DeltaPhiLeptons_ee";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaPhiLeptons_ee", plotname+specialLabel);

//   plotname = "NJets_ee";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCentralJets_ee", plotname+specialLabel);

//   plotname = "DileptonMass_ee";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hDileptonMass_ee", plotname+specialLabel);

//   plotname = "METDeltaPhilEt_ee";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hMETdeltaPhilEtHist_ee", plotname+specialLabel);

//   plotname = "NDirtyMuons_ee";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNDirtyMuonsHist_ee", plotname+specialLabel);

//   plotname = "NCleanExtraTracksHist_ee";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCleanExtraTracksHist_ee", plotname+specialLabel);

//*************************************************************************************************
//mumu final state Plots
//*************************************************************************************************
//   plotname = "WWSelectionStacked_mumu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hHWWSelection_mumu", plotname+specialLabel);

//   plotname = "LeptonPtMax_mumu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonPtMax_mumu", plotname+specialLabel);

//   plotname = "LeptonPtMin_mumu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hLeptonPtMin_mumu", plotname+specialLabel);

//   plotname = "Met_mumu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPtHist_mumu", plotname+specialLabel);

//   plotname = "DeltaPhiLeptons_mumu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaPhiLeptons_mumu", plotname+specialLabel);

//   plotname = "NJets_mumu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCentralJets_mumu", plotname+specialLabel);

//   plotname = "DileptonMass_mumu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hDileptonMass_mumu", plotname+specialLabel);

//   plotname = "METDeltaPhilEt_mumu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hMETdeltaPhilEtHist_mumu", plotname+specialLabel);

//   plotname = "NDirtyMuons_mumu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNDirtyMuonsHist_mumu", plotname+specialLabel);

//   plotname = "NCleanExtraTracksHist_mumu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCleanExtraTracksHist_mumu", plotname+specialLabel);

//*************************************************************************************************
//emu final state Plots
//*************************************************************************************************
//   plotname = "WWSelectionStacked_emu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hHWWSelection_emu", plotname+specialLabel);

//   plotname = "LeptonPtMax_emu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonPtMax_emu", plotname+specialLabel);

//   plotname = "LeptonPtMin_emu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hLeptonPtMin_emu", plotname+specialLabel);

//   plotname = "Met_emu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPtHist_emu", plotname+specialLabel);

//   plotname = "DeltaPhiLeptons_emu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaPhiLeptons_emu", plotname+specialLabel);

//   plotname = "NJets_emu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCentralJets_emu", plotname+specialLabel);

//   plotname = "DileptonMass_emu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hDileptonMass_emu", plotname+specialLabel);

//   plotname = "METDeltaPhilEt_emu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hMETdeltaPhilEtHist_emu", plotname+specialLabel);

//   plotname = "NDirtyMuons_emu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNDirtyMuonsHist_emu", plotname+specialLabel);

//   plotname = "NCleanExtraTracksHist_emu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCleanExtraTracksHist_emu", plotname+specialLabel);

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
      InitHist(tmp, "Xtitle", "Number of Events", 3+i);
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
