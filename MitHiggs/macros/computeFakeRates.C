//root -l $CMSSW_BASE/src/MitHiggs/macros/plotMacros/computeFakeRates.C+\(11,\"MuonFakeRateFile.root\"\)
//root -l $CMSSW_BASE/src/MitHiggs/macros/plotMacros/computeFakeRates.C+\(13,\"ElectronFakeRateFile.root\"\)
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
#include "MitAna/Utils/interface/SimpleTable.h"
#include <vector>

using namespace std;
//using namespace mithep;
#endif

#include <vector>

//cross section table
mithep::SimpleTable xstab("$CMSSW_BASE/src/MitPhysics/data/xs.dat");  

//define colors
Int_t COLORS[10] = {1,2,3,4,5,6,7,8,9,10};

TH1F* getHisto(string filename, string directoryname, string histoname) {
  TFile *file = new TFile(filename.c_str(),"READ");
  if (!file) {
    cout << "Could not open file " << filename << endl;
    return 0;
  }

  TDirectory *dir = (TDirectory*)file->FindObjectAny(directoryname.c_str());
  if (!dir) {
    cout << "Could not find directory " << directoryname << endl;
    delete file;
    return 0;
  }

  TH1F *hist = (TH1F*)dir->Get(histoname.c_str());
  if (!hist) {
    cout << "Could not find histogram " <<  histoname << " in directory " << directoryname << endl;
    delete dir;
    delete file;
    return 0;
  }

  hist->SetDirectory(0);
  delete dir;
  delete file;
  return hist;

}

//--------------------------------------------------------------------------------------------------
// Add histograms form multiple files together weighted by cross-section
//--------------------------------------------------------------------------------------------------
TH1F* addAllSamples(vector<string> datasetFiles, vector<string> datasetNames,
                    string dirName, string histName) {

  assert(datasetFiles.size() > 0);
  TH1F *tmp = getHisto(datasetFiles[0], dirName, histName);
  assert(tmp);
  TH1F *finalHist = (TH1F*)tmp->Clone();
  finalHist->Sumw2();

  for (UInt_t i=0; i < datasetFiles.size() ;i++) {
    double CrossSection = xstab.Get(datasetNames[i].c_str());
    TH1F *tmpNEventsHist = getHisto(datasetFiles[i],"AnaFwkMod", "hDEvents");
    double NEvents = tmpNEventsHist->Integral();
    double weight = CrossSection * 1.0 / NEvents;
    if (i==0) {
      finalHist->Scale(weight);
    } else {
      TH1F *tmpHist = getHisto(datasetFiles[i], dirName, histName);
      tmpHist->Scale(weight);
      finalHist->Add(tmpHist);
    }
  }
  return finalHist;
}

//--------------------------------------------------------------------------------------------------
// make fake rate histogram from numerator and denominators
//--------------------------------------------------------------------------------------------------
TH1F* createFakeRatePtHist(vector<string> datasetFiles, vector<string> datasetNames,
                           int faketype, string histName) {

  string numeratorHistName = "";
  string denominatorHistName = "";
  string dirName = "";

  if (faketype == 11) {
    dirName = "ComputeElectronFakeRateMod";
    numeratorHistName = "hElectronNumeratorEt";
    denominatorHistName = "hElectronDenominatorEt";
  } else if (faketype == 13) {
    dirName = "ComputeMuonFakeRateMod";
    numeratorHistName = "hMuonNumeratorPt";
    denominatorHistName = "hMuonDenominatorPt";
  } else {
    cerr << "Error: faketype = " << faketype << " is not recognized." << endl;
    return 0;
  }

  TH1F *denominator = addAllSamples(datasetFiles, datasetNames, dirName, denominatorHistName);
  TH1F *numerator = addAllSamples(datasetFiles, datasetNames, dirName, numeratorHistName);

  //create fake rate hist from denominator binning
  TH1F *fakeRateHist = (TH1F*)numerator->Clone(histName.c_str());
  fakeRateHist->GetYaxis()->SetTitle("Fake Rate");
  //divide by the denominator to get fake rate;
  fakeRateHist->Divide(denominator);

  return fakeRateHist;
}

//--------------------------------------------------------------------------------------------------
// Fit histogram with function
//--------------------------------------------------------------------------------------------------
TF1* fitFakeRatePtHist(TH1F *FakeRatePt) {

  TF1 *fitFunction = new TF1("fitFunction", "[0]*(1 - exp(-1*[1]*(x - [2])))",-10,10);
  fitFunction->SetParameter(0,0.05);
  fitFunction->SetParameter(1,0.07);
  fitFunction->SetParameter(2,10.0);
  fitFunction->SetParLimits(2,9.5,10.5);
  
  FakeRatePt->Fit("fitFunction");
  
  return fitFunction;

}

//--------------------------------------------------------------------------------------------------
// Draw the FakeRate histograms and fit functions
//--------------------------------------------------------------------------------------------------
void drawFakeRatePlots( vector<TH1F*> fakeRateHistograms, vector<TF1*> fakeRateFits,
                        vector<string> fakeRateLegendNames , string plotname ,Double_t MAXY = 0.2) {

  assert(fakeRateHistograms.size() == fakeRateFits.size());
  assert(fakeRateHistograms.size() == fakeRateLegendNames.size());
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);
  
  TLegend *leg1=0;
  leg1 = new TLegend(0.65,0.6,0.9,0.8);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);
  
  for (UInt_t i=0; i<fakeRateHistograms.size() ; i++) {    
    //fakeRateHistograms[i]->SetMarkerSize(0.5);
    fakeRateHistograms[i]->SetMarkerColor(COLORS[i]);
    fakeRateHistograms[i]->SetMinimum(0.0);
    fakeRateHistograms[i]->SetMaximum(MAXY);
    fakeRateHistograms[i]->SetLineColor(COLORS[i]);
    fakeRateFits[i]->SetLineColor(COLORS[i]);    
    leg1->AddEntry(fakeRateFits[i], fakeRateLegendNames[i].c_str(), "LP"); 
  
    if (i==0) { 
      fakeRateHistograms[i]->Draw();
    } else {
      fakeRateHistograms[i]->Draw("same");
    }
    fakeRateFits[i]->Draw("same");    
  }

  leg1->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}

//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void computeFakeRates(char *FakeRateFilename ) {

  string fakeTypeString = "";
  //Samples
  vector<string> madgraphDatasetFiles;
  vector<string> madgraphDatasetNames;
  vector<string> pythiaDatasetFiles;
  vector<string> pythiaDatasetNames;

  //*****************************************************************************************
  //Do FakeRate Computation 
  //*****************************************************************************************

  //Create root file to store fake rates
  TFile *file = new TFile(FakeRateFilename, "RECREATE");
  file->cd();

  //do both muon and electrons
  Double_t maxY = 1.0;
  for (int faketype=11 ; faketype <= 13; faketype+=2) {
    if (faketype == 11) {
      fakeTypeString = "Electron";
      maxY = 0.5;
    } else if (faketype == 13) {
      fakeTypeString = "Muon";
      maxY = 0.01;
    } else {
      cerr << "Error: faketype = " << faketype << " not recognized.\n";
      return;
    }
    
    //*****************************************************************************************
    //Load input histogram files
    //*****************************************************************************************
    madgraphDatasetFiles.clear(); 
    madgraphDatasetNames.clear();
    pythiaDatasetFiles.clear();
    pythiaDatasetNames.clear();
    
    //Madgraph WJets
    madgraphDatasetFiles.push_back("/home/sixie/hist/Compute" + fakeTypeString + "FakeRate/filler/006/Compute" + fakeTypeString + "FakeRate_f8-wjets-mg-id9.root");
    madgraphDatasetNames.push_back("f8-wjets-mg-id9");
    //Pythia WJets
    pythiaDatasetFiles.push_back("/home/sixie/hist/Compute" + fakeTypeString + "FakeRate/filler/006/Compute" + fakeTypeString + "FakeRate_s8-we-id9.root");
    pythiaDatasetNames.push_back("s8-we-id9");
    pythiaDatasetFiles.push_back("/home/sixie/hist/Compute" + fakeTypeString + "FakeRate/filler/006/Compute" + fakeTypeString + "FakeRate_s8-wm-id9.root");
    pythiaDatasetNames.push_back("s8-wm-id9");
    pythiaDatasetFiles.push_back("/home/sixie/hist/Compute" + fakeTypeString + "FakeRate/filler/006/Compute" + fakeTypeString + "FakeRate_s8-wtau-id9.root");
    pythiaDatasetNames.push_back("s8-wtau-id9");

    //*****************************************************************************************
    //Construct the fake rate Histgrams. Fit with function.
    //*****************************************************************************************
    vector<TH1F*> fakeRateHistograms; fakeRateHistograms.clear();
    vector<TF1*> fakeRateFits; fakeRateFits.clear();
    vector<string> fakeRateLegendNames; fakeRateLegendNames.clear();

    //pythia version
    TH1F *FakeRatePt_pythia = createFakeRatePtHist(pythiaDatasetFiles, pythiaDatasetNames, faketype,
                                                   (fakeTypeString + "FakeRate_Pt_pythia").c_str());
    FakeRatePt_pythia->GetYaxis()->SetTitle((fakeTypeString + " Fake Rate").c_str());    
    TF1 *FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName((fakeTypeString + "FakeRateFunction_Pt_pythia").c_str());
    
    fakeRateHistograms.push_back(FakeRatePt_pythia);
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
    fakeRateLegendNames.push_back("Pythia W+Jets");

    //madgraph version
    TH1F *FakeRatePt_madgraph = createFakeRatePtHist(
      madgraphDatasetFiles, madgraphDatasetNames,faketype,
      (fakeTypeString + "FakeRate_Pt_madgraph").c_str());
    FakeRatePt_madgraph->GetYaxis()->SetTitle((fakeTypeString + " Fake Rate").c_str());    
    TF1 *FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(
      (fakeTypeString + "FakeRateFunction_Pt_madgraph").c_str());

    fakeRateHistograms.push_back(FakeRatePt_madgraph);
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
    fakeRateLegendNames.push_back("Madgraph W+Jets");

    cout << "pythia at x = 30 : " << FakeRateFitFunction_pythia->Eval(30.0) << endl;
    cout << "pythia at x = 30 : " << FakeRateFitFunction_madgraph->Eval(30.0) << endl;

    //*****************************************************************************************
    //Draw the plots
    //*****************************************************************************************
    drawFakeRatePlots(fakeRateHistograms,fakeRateFits,fakeRateLegendNames,
                      (fakeTypeString + "FakeRateEt").c_str(),maxY);

    //*****************************************************************************************
    //save them to the output file
    //*****************************************************************************************
    for (UInt_t i=0; i<fakeRateHistograms.size();i++) {
      fakeRateHistograms[i]->SetDirectory(file);
      file->WriteTObject(fakeRateFits[i]);
    }
  }

  file->Write();
  file->Close();
  return;

}
