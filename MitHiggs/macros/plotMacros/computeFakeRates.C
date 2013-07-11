//root -l $CMSSW_BASE/src/MitHiggs/macros/plotMacros/computeFakeRates.C+\(\"MuonFakeRateFile.root\"\)
//root -l $CMSSW_BASE/src/MitHiggs/macros/plotMacros/computeFakeRates.C+\(\"ElectronFakeRateFile.root\"\)
//root -l $CMSSW_BASE/src/MitHiggs/macros/plotMacros/computeFakeRates.C+\(\"TestFakeRateFile.root\"\)
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
Int_t COLORS[10] = {kRed,kBlue,kMagenta,kGreen,6,7,8,9,10,11};

TH1F* getHisto(string filename, string directoryname, string histoname) {
  TFile *file = new TFile(filename.c_str(),"READ");
  if (!file) {
    cout << "Could not open file " << filename << endl;
    return 0;
  }

  TDirectory *dir = (TDirectory*)file->FindObjectAny(directoryname.c_str());
  if (!dir) {
    cout << "Could not find directory " << directoryname 
         << " in file " << filename << endl;
    delete file;
    return 0;
  }

  TH1F *hist = (TH1F*)dir->Get(histoname.c_str());
  if (!hist) {
    cout << "Could not find histogram " <<  histoname << " in directory " << directoryname 
         << " in file " << filename << endl;
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
// TGraphAsymmErrors* createFakeRatePtHist(vector<string> datasetFiles, vector<string> datasetNames, 
//                            int denominatortype, int faketype, int chargetype,  
//                            int eventType, string histName) {
TH1F* createFakeRatePtHist(vector<string> datasetFiles, vector<string> datasetNames, 
                           int denominatortype, int faketype, int chargetype,  
                           int eventType, string histName) {

  string numeratorHistName = "";
  string denominatorHistName = "";
  string dirName = "";

  string chargeTypeName = "";
  if (chargetype == -1)
    chargeTypeName = "Minus";
  else if (chargetype == 1)
    chargeTypeName = "Plus";

  string eventTypeName = "";
  if (eventType == -1)
    eventTypeName = "MinusW";
  else if (eventType == 1)
    eventTypeName = "PlusW";

  string denominatorTypeName = "";
  if (denominatortype == 1)
    denominatorTypeName = "Track";

  if (faketype == 11) {
    dirName = "ComputeElectronFakeRateMod";
    numeratorHistName = "h" + chargeTypeName + "Electron" + eventTypeName + "NumeratorPt";
    denominatorHistName = "h" + chargeTypeName + "Electron" + eventTypeName + denominatorTypeName + "DenominatorPt";
  } else if (faketype == 13) {
    dirName = "ComputeMuonFakeRateMod";
    numeratorHistName = "h" + chargeTypeName + "Muon" + eventTypeName + "NumeratorPt";
    denominatorHistName = "h" + chargeTypeName + "Muon" + eventTypeName + denominatorTypeName + "DenominatorPt";
  } else {
    cerr << "Error in createFakeRatePtHist: faketype = " << faketype << " is not recognized." << endl;
    assert(false);
  }

  TH1F *denominator = addAllSamples(datasetFiles, datasetNames, dirName, denominatorHistName);
  TH1F *numerator = addAllSamples(datasetFiles, datasetNames, dirName, numeratorHistName);

  //create fake rate hist from denominator binning
  TH1F *fakeRateHist = (TH1F*)numerator->Clone(histName.c_str());
  fakeRateHist->GetYaxis()->SetTitle("Fake Rate");
  //divide by the denominator to get fake rate;
  fakeRateHist->Divide(denominator);

  //TGraphAsymmErrors* fakeRate = (numerator, denominator);
  

  //for (int i=1 ; i <= fakeRateHist->GetXaxis()->GetNbins(); i++) {
    //cout << i << " : " << fakeRateHist->GetXaxis()->GetBinCenter(i) << " " << fakeRateHist->GetBinContent(i) << "  +- " << fakeRateHist->GetBinError(i) << endl;
    //if (fakeRateHist->GetBinContent(i) > 0 && fakeRateHist->GetBinError(i) < 0.01) {
      //fakeRateHist->SetBinError(i,0.01);
    //}
  //}

  return fakeRateHist;
}

//--------------------------------------------------------------------------------------------------
// Fit histogram with function
//--------------------------------------------------------------------------------------------------
TF1* fitFakeRatePtHist(TH1F *FakeRatePt) {

  TF1 *fitFunction = new TF1("fitFunction", "[0]*(1 - [3]*exp(-1*[1]*(x - [2])))",13,500);
  fitFunction->SetParameter(0,0.05);
  fitFunction->SetParameter(1,0.07);
  fitFunction->SetParameter(2,10.0);
  fitFunction->SetParameter(3,0.1);
  fitFunction->SetParLimits(2,9.5,10.5);
  
  FakeRatePt->Fit("fitFunction", "R+");
  
  return fitFunction;

}

//--------------------------------------------------------------------------------------------------
// Draw the FakeRate histograms and fit functions
//--------------------------------------------------------------------------------------------------
void drawFakeRatePlots( vector<TH1F*> fakeRateHistograms, vector<TF1*> fakeRateFits,
                        vector<string> fakeRateLegendNames , string plotname ,Double_t MAXY = 0.2) {

  Bool_t fitFakeRate = true;
  if (!fitFakeRate || fakeRateHistograms.size() != fakeRateFits.size())
    fitFakeRate = false;

  assert(fakeRateHistograms.size() == fakeRateLegendNames.size());
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  TLegend *leg1=0;
  leg1 = new TLegend(0.65,0.6,0.9,0.8);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);
  
  for (UInt_t i=0; i<fakeRateHistograms.size() ; i++) {    

    cerr << fakeRateHistograms[i] << endl;

    //fakeRateHistograms[i]->SetMarkerSize(0.5);
    cerr << COLORS[i] << endl;
    fakeRateHistograms[i]->SetMarkerColor(COLORS[i]);
    fakeRateHistograms[i]->SetMinimum(0.0);
    fakeRateHistograms[i]->SetMaximum(MAXY);
    fakeRateHistograms[i]->SetLineColor(COLORS[i]);

    
    if (fitFakeRate) fakeRateFits[i]->SetLineColor(COLORS[i]);    
    leg1->AddEntry(fakeRateHistograms[i], fakeRateLegendNames[i].c_str(), "LP"); 
  
    if (i==0) { 
      fakeRateHistograms[i]->Draw();
    } else {
      fakeRateHistograms[i]->Draw("same");
    }    
    if (fitFakeRate)  fakeRateFits[i]->Draw("same");    
  }

  leg1->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}

//--------------------------------------------------------------------------------------------------
// Do Fake Rate Computation From WJets sample
//--------------------------------------------------------------------------------------------------
void computeFakeRatesFromWJets( char *FakeRateFilename ,bool fitFakeRate, int chargetype, int eventType ) {

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
  TFile *file = new TFile(FakeRateFilename, "UPDATE");
  file->cd();

  //for different charges
  string chargeTypeName = "";
  if (chargetype == -1)
    chargeTypeName = "Minus";
  else if (chargetype == 1)
    chargeTypeName = "Plus";

  string eventTypeName = "";
  if (eventType == -1)
    eventTypeName = "MinusW";
  else if (eventType == 1)
    eventTypeName = "PlusW";


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
      assert(false);
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
    TH1F *FakeRatePt_pythia = createFakeRatePtHist(pythiaDatasetFiles, pythiaDatasetNames, 0, faketype, chargetype, eventType,
                                                   (chargeTypeName + fakeTypeString + eventTypeName + "FakeRate_Pt_PythiaWJets").c_str());
    FakeRatePt_pythia->GetYaxis()->SetTitle((chargeTypeName + fakeTypeString + eventTypeName + " Fake Rate").c_str());    
    TF1 *FakeRateFitFunction_pythia = NULL;
    if (fitFakeRate) {
      FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
      FakeRateFitFunction_pythia->SetName((chargeTypeName + fakeTypeString + eventTypeName + "FakeRateFunction_Pt_PythiaWJets").c_str());
      fakeRateFits.push_back(FakeRateFitFunction_pythia);
    }
    fakeRateHistograms.push_back(FakeRatePt_pythia);
    fakeRateLegendNames.push_back((chargeTypeName + fakeTypeString + eventTypeName + "Pythia W+Jets").c_str());

    //madgraph version
    TH1F *FakeRatePt_madgraph = createFakeRatePtHist(
      madgraphDatasetFiles, madgraphDatasetNames,0,faketype,chargetype,eventType,
      (chargeTypeName + fakeTypeString + eventTypeName + "FakeRate_Pt_MadgraphWJets").c_str());
    FakeRatePt_madgraph->GetYaxis()->SetTitle((chargeTypeName + fakeTypeString + eventTypeName + " Fake Rate").c_str());    

    TF1 *FakeRateFitFunction_madgraph = NULL;
    if (fitFakeRate) {
      FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
      FakeRateFitFunction_madgraph->SetName(
      (chargeTypeName + fakeTypeString + eventTypeName + "FakeRateFunction_Pt_MadgraphWJets").c_str());
    }
    if (fitFakeRate) fakeRateFits.push_back(FakeRateFitFunction_madgraph);
    fakeRateHistograms.push_back(FakeRatePt_madgraph);
    fakeRateLegendNames.push_back(chargeTypeName + fakeTypeString + eventTypeName + " Madgraph W+Jets");
    
    //*****************************************************************************************
    //Draw the plots
    //*****************************************************************************************
    drawFakeRatePlots(fakeRateHistograms,fakeRateFits,fakeRateLegendNames,
                      (chargeTypeName + fakeTypeString + eventTypeName + "FakeRatePt").c_str(),maxY);

    //*****************************************************************************************
    //save them to the output file
    //*****************************************************************************************
    for (UInt_t i=0; i<fakeRateHistograms.size();i++) {
      //write the histograms. delete any existing one with the same name
      if (file->FindObjectAny(fakeRateHistograms[i]->GetName())) {
        string tmpname = fakeRateHistograms[i]->GetName();        
        file->Delete((tmpname+";*").c_str());
      }

      fakeRateHistograms[i]->SetDirectory(file);
      file->WriteTObject(fakeRateHistograms[i]);

      //write the fit functions. delete any existing one with the same name
      if (fitFakeRate) {
        if (file->FindObjectAny(fakeRateFits[i]->GetName())) {
          string tmpname = fakeRateFits[i]->GetName();        
          file->Delete((tmpname+";*").c_str());
        }
        file->WriteTObject(fakeRateFits[i]);
      }
    }
  }

  file->Close();
  return;

}

//--------------------------------------------------------------------------------------------------
// Do Fake Rate Computation From WJets sample
//--------------------------------------------------------------------------------------------------
void computeElectronFakeRatesWithTrackDenominatorFromWJets( char *FakeRateFilename ,bool fitFakeRate, int chargetype, int eventType ) {

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
  TFile *file = new TFile(FakeRateFilename, "UPDATE");
  file->cd();

  //for different charges
  string chargeTypeName = "";
  if (chargetype == -1)
    chargeTypeName = "Minus";
  else if (chargetype == 1)
    chargeTypeName = "Plus";

  string eventTypeName = "";
  if (eventType == -1)
    eventTypeName = "MinusW";
  else if (eventType == 1)
    eventTypeName = "PlusW";


  //do both muon and electrons
  Double_t maxY = 1.0;
  for (int faketype=11 ; faketype <= 11; faketype+=2) {
    if (faketype == 11) {
      fakeTypeString = "Electron";
      maxY = 0.1;
    } else if (faketype == 13) {
      fakeTypeString = "Muon";
      maxY = 0.01;
    } else {
      cerr << "Error: faketype = " << faketype << " not recognized.\n";
      assert(false);
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
    TH1F *FakeRatePt_pythia = createFakeRatePtHist(pythiaDatasetFiles, pythiaDatasetNames, 1, faketype, chargetype, eventType,
                                                   (chargeTypeName + fakeTypeString + eventTypeName + "TrackDenominatorFakeRate_Pt_PythiaWJets").c_str());
    FakeRatePt_pythia->GetYaxis()->SetTitle((chargeTypeName + fakeTypeString + eventTypeName + "TrackDenominator Fake Rate").c_str());    
    TF1 *FakeRateFitFunction_pythia = NULL;
    if (fitFakeRate) {
      FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
      FakeRateFitFunction_pythia->SetName((chargeTypeName + fakeTypeString + eventTypeName + "TrackDenominatorFakeRateFunction_Pt_PythiaWJets").c_str());
      fakeRateFits.push_back(FakeRateFitFunction_pythia);
    }
    fakeRateHistograms.push_back(FakeRatePt_pythia);
    fakeRateLegendNames.push_back((chargeTypeName + fakeTypeString + eventTypeName + "TrackDenominator Pythia W+Jets").c_str());

    //madgraph version
//     TH1F *FakeRatePt_madgraph = createFakeRatePtHist(
//       madgraphDatasetFiles, madgraphDatasetNames,1,faketype, chargetype,eventType,
//       (chargeTypeName + fakeTypeString + eventTypeName + "TrackDenominator FakeRate_Pt_MadgraphWJets").c_str());
//     FakeRatePt_madgraph->GetYaxis()->SetTitle((chargeTypeName + fakeTypeString + eventTypeName + "TrackDenominator Fake Rate").c_str());    

//     TF1 *FakeRateFitFunction_madgraph = NULL;
//     if (fitFakeRate) {
//       FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//       FakeRateFitFunction_madgraph->SetName(
//       (chargeTypeName + fakeTypeString + eventTypeName + "TrackDenominatorFakeRateFunction_Pt_MadgraphWJets").c_str());
//     }
//     if (fitFakeRate) fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//     fakeRateHistograms.push_back(FakeRatePt_madgraph);
//     fakeRateLegendNames.push_back(chargeTypeName + fakeTypeString + eventTypeName + "TrackDenominator Madgraph W+Jets");
    
    //*****************************************************************************************
    //Draw the plots
    //*****************************************************************************************
    drawFakeRatePlots(fakeRateHistograms,fakeRateFits,fakeRateLegendNames,
                      (chargeTypeName + fakeTypeString + eventTypeName + "TrackDenominatorFakeRatePt").c_str(),maxY);

    //*****************************************************************************************
    //save them to the output file
    //*****************************************************************************************
    for (UInt_t i=0; i<fakeRateHistograms.size();i++) {
      //write the histograms. delete any existing one with the same name
      if (file->FindObjectAny(fakeRateHistograms[i]->GetName())) {
        string tmpname = fakeRateHistograms[i]->GetName();        
        file->Delete((tmpname+";*").c_str());
      }

      fakeRateHistograms[i]->SetDirectory(file);
      file->WriteTObject(fakeRateHistograms[i]);

      //write the fit functions. delete any existing one with the same name
      if (fitFakeRate) {
        if (file->FindObjectAny(fakeRateFits[i]->GetName())) {
          string tmpname = fakeRateFits[i]->GetName();        
          file->Delete((tmpname+";*").c_str());
        }
        file->WriteTObject(fakeRateFits[i]);
      }
    }
  }

  file->Close();
  return;

}


//--------------------------------------------------------------------------------------------------
// Do Fake Rate Computation From WCD sample
//--------------------------------------------------------------------------------------------------
void computeFakeRatesFromQCD(char *FakeRateFilename ,bool fitFakeRate, int chargetype = 0, int eventType = 0) {

  //Samples
  vector<string> madgraphQCDDatasetFiles;
  vector<string> madgraphQCDDatasetNames;
  vector<string> pythiaQCDemDatasetFiles;
  vector<string> pythiaQCDemDatasetNames;
  vector<string> pythiaInclMuonDatasetFiles;
  vector<string> pythiaInclMuonDatasetNames;

  //*****************************************************************************************
  //Do FakeRate Computation 
  //*****************************************************************************************

  //Create root file to store fake rates
  TFile *file = new TFile(FakeRateFilename, "UPDATE");
  file->cd();

  //different charges
  string chargeTypeName = "";
  if (chargetype == -1)
    chargeTypeName = "Minus";
  else if (chargetype == 1)
    chargeTypeName = "Plus";

  string eventTypeName = "";
  if (eventType == -1)
    eventTypeName = "MinusW";
  else if (eventType == 1)
    eventTypeName = "PlusW";

  //do both muon and electrons
  string fakeTypeString = "";
  Double_t maxY = 1.0;
  for (int faketype=11 ; faketype <= 13; faketype+=2) {
    if (faketype == 11) {
      fakeTypeString = "Electron";
      maxY = 0.5;
    } else if (faketype == 13) {
      fakeTypeString = "Muon";
      maxY = 0.5;
    } else {
      cerr << "Error: faketype = " << faketype << " not recognized.\n";
      assert(false);
    }
    
    //*****************************************************************************************
    //Load input histogram files
    //*****************************************************************************************
    madgraphQCDDatasetFiles.clear(); 
    madgraphQCDDatasetNames.clear();
    pythiaQCDemDatasetFiles.clear();
    pythiaQCDemDatasetNames.clear();
    pythiaInclMuonDatasetFiles.clear();
    pythiaInclMuonDatasetNames.clear();
    
    //Madgraph QCD
    madgraphQCDDatasetFiles.push_back("/home/sixie/hist/Compute" + fakeTypeString + "FakeRate/filler/006/Compute" + fakeTypeString + "FakeRate_s8-qcd_250_500-mg-id9.root");
    madgraphQCDDatasetNames.push_back("s8-qcd_250_500-mg-id9");

    //Pythia QCDEM
    pythiaQCDemDatasetFiles.push_back("/home/sixie/hist/Compute" + fakeTypeString + "FakeRate/filler/006/Compute" + fakeTypeString + "FakeRate_s8-qcdem_20_30-id9.root");
    pythiaQCDemDatasetNames.push_back("s8-qcdem_20_30-id9");
    pythiaQCDemDatasetFiles.push_back("/home/sixie/hist/Compute" + fakeTypeString + "FakeRate/filler/006/Compute" + fakeTypeString + "FakeRate_s8-qcdem_30_80-id9.root");
    pythiaQCDemDatasetNames.push_back("s8-qcdem_30_80-id9");
    pythiaQCDemDatasetFiles.push_back("/home/sixie/hist/Compute" + fakeTypeString + "FakeRate/filler/006/Compute" + fakeTypeString + "FakeRate_s8-qcdem_80_170-id9.root");
    pythiaQCDemDatasetNames.push_back("s8-qcdem_80_170-id9");

    //Pythia InclusiveMuon
    pythiaInclMuonDatasetFiles.push_back("/home/sixie/hist/Compute" + fakeTypeString + "FakeRate/filler/006/Compute" + fakeTypeString + "FakeRate_s8-incmu_15-id9.root");
    pythiaInclMuonDatasetNames.push_back("s8-incmu_15-id9");
    pythiaInclMuonDatasetFiles.push_back("/home/sixie/hist/Compute" + fakeTypeString + "FakeRate/filler/006/Compute" + fakeTypeString + "FakeRate_s8-incmu_5_50-id9.root");
    pythiaInclMuonDatasetNames.push_back("s8-incmu_5_50-id9");


    //*****************************************************************************************
    //Construct the fake rate Histgrams. Fit with function.
    //*****************************************************************************************
    vector<TH1F*> fakeRateHistograms; fakeRateHistograms.clear();
    vector<TF1*> fakeRateFits; fakeRateFits.clear();
    vector<string> fakeRateLegendNames; fakeRateLegendNames.clear();

    //pythia version
    TH1F *FakeRatePt_pythia = NULL;
    if (faketype == 11)
      FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDemDatasetFiles, pythiaQCDemDatasetNames,
                                               0,faketype, chargetype,eventType,
                                               (chargeTypeName + fakeTypeString + eventTypeName + "FakeRate_Pt_PythiaQCDEM").c_str());
    else if (faketype == 13) {
      //FakeRatePt_pythia = createFakeRatePtHist(pythiaInclMuonDatasetFiles, pythiaInclMuonDatasetNames, 
      //                                         0,faketype,chargetype,eventType,
      //                                         (chargeTypeName + fakeTypeString + eventTypeName + "FakeRate_Pt_PythiaInclMuon").c_str());
      FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDemDatasetFiles, pythiaQCDemDatasetNames, 
                                               0,faketype, chargetype,eventType,
                                               (chargeTypeName + fakeTypeString + eventTypeName + "FakeRate_Pt_PythiaQCDEM").c_str());
    } else {
      cerr << "faketype out of range\n";
      assert(false);
    }
    FakeRatePt_pythia->GetYaxis()->SetTitle((chargeTypeName + fakeTypeString + eventTypeName + " Fake Rate").c_str());    
      
    TF1 *FakeRateFitFunction_pythia = NULL;
    if (fitFakeRate) {
      TF1 *FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
      if (faketype == 11)
        FakeRateFitFunction_pythia->SetName((chargeTypeName + fakeTypeString + eventTypeName + "FakeRateFunction_Pt_PythiaQCDEM").c_str());
      else if (faketype == 13) {
      FakeRateFitFunction_pythia->SetName((chargeTypeName + fakeTypeString + eventTypeName + "FakeRateFunction_Pt_PythiaQCDEM").c_str());    
      }
    }

    fakeRateHistograms.push_back(FakeRatePt_pythia);
    if (fitFakeRate) fakeRateFits.push_back(FakeRateFitFunction_pythia);
    if (faketype == 11)
      fakeRateLegendNames.push_back(chargeTypeName + fakeTypeString + eventTypeName + " Pythia QCDEM");
    else if (faketype == 13)
      fakeRateLegendNames.push_back(chargeTypeName + fakeTypeString + eventTypeName + " Pythia QCDEM");
    else {
      cerr << "faketype out of range\n";
      assert(false);
    }

    //madgraph version
    TH1F *FakeRatePt_madgraph = createFakeRatePtHist(
      madgraphQCDDatasetFiles, madgraphQCDDatasetNames,0,faketype,chargetype,eventType,
      (chargeTypeName + fakeTypeString + "FakeRate_Pt_MadgraphQCD").c_str());
    FakeRatePt_madgraph->GetYaxis()->SetTitle((chargeTypeName +fakeTypeString + eventTypeName + " Fake Rate").c_str());    

    TF1 *FakeRateFitFunction_madgraph = NULL;
    if (fitFakeRate) {
      FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
      FakeRateFitFunction_madgraph->SetName(
        (chargeTypeName + fakeTypeString + eventTypeName + "FakeRateFunction_Pt_MadgraphQCD").c_str());      
    }
    
    fakeRateHistograms.push_back(FakeRatePt_madgraph);
    if (fitFakeRate) fakeRateFits.push_back(FakeRateFitFunction_madgraph);
    fakeRateLegendNames.push_back(chargeTypeName + fakeTypeString + eventTypeName + " Madgraph QCD");
    
    //*****************************************************************************************
    //Draw the plots
    //*****************************************************************************************
    drawFakeRatePlots(fakeRateHistograms,fakeRateFits,fakeRateLegendNames,
                      (chargeTypeName + fakeTypeString + eventTypeName + "FakeRatePt").c_str(),maxY);

    //*****************************************************************************************
    //save them to the output file
    //*****************************************************************************************
    for (UInt_t i=0; i<fakeRateHistograms.size();i++) {
      //write the histograms. delete any existing one with the same name
      if (file->FindObjectAny(fakeRateHistograms[i]->GetName())) {
        string tmpname = fakeRateHistograms[i]->GetName();        
        file->Delete((tmpname+";*").c_str());
      }

      fakeRateHistograms[i]->SetDirectory(file);
      file->WriteTObject(fakeRateHistograms[i]);

      //write the fit functions. delete any existing one with the same name
      if (fitFakeRate){
        if (file->FindObjectAny(fakeRateFits[i]->GetName())) {
          string tmpname = fakeRateFits[i]->GetName();        
          file->Delete((tmpname+";*").c_str());
        }
        file->WriteTObject(fakeRateFits[i]);
      }
    }
  }
  file->Close();
  return;
}

//--------------------------------------------------------------------------------------------------
// Make plots of electron fake rate from many samples
//--------------------------------------------------------------------------------------------------
void compareFakeRates(char *FakeRateFilename, int faketype , int chargetype , int eventType) {

  string fakeTypeString = "";
  if (faketype == 11) {
    fakeTypeString = "Electron";
  } else if (faketype == 13) {
    fakeTypeString = "Muon";
  } else {
    cerr << "Error: faketype = " << faketype << " not recognized.\n";
    assert(false);
  }

  //different charges
  string chargeTypeName = "";
  if (chargetype == -1)
    chargeTypeName = "Minus";
  else if (chargetype == 1)
    chargeTypeName = "Plus";

  string eventTypeName = "";
  if (eventType == -1)
    eventTypeName = "MinusW";
  else if (eventType == 1)
    eventTypeName = "PlusW";

  string plotname = chargeTypeName + fakeTypeString + eventTypeName + "FakeRateComparison";

  TFile *file = new TFile(FakeRateFilename, "READ");
  file->cd();

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);
  TLegend *leg1=0;
  leg1 = new TLegend(0.65,0.6,0.9,0.8);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);

  TH1F *electronFakeRate_pythiaWJets = (TH1F*)file->FindObjectAny((chargeTypeName + fakeTypeString + eventTypeName + "FakeRate_Pt_PythiaWJets").c_str());
  TH1F *electronFakeRate_madgraphWJets = (TH1F*)file->FindObjectAny((chargeTypeName + fakeTypeString + eventTypeName + "FakeRate_Pt_MadgraphWJets").c_str());
  TH1F *electronFakeRate_pythiaQCDEM = (TH1F*)file->FindObjectAny((chargeTypeName + fakeTypeString + eventTypeName + "FakeRate_Pt_PythiaQCDEM").c_str());
  TH1F *electronFakeRate_madgraphQCD = (TH1F*)file->FindObjectAny((chargeTypeName + fakeTypeString + eventTypeName + "FakeRate_Pt_MadgraphQCD").c_str());

  assert(electronFakeRate_pythiaWJets);
  assert(electronFakeRate_madgraphWJets);
  assert(electronFakeRate_pythiaQCDEM);
  assert(electronFakeRate_madgraphQCD);

  electronFakeRate_pythiaWJets->SetLineColor(kRed);
  electronFakeRate_pythiaWJets->SetMarkerColor(kRed);
  electronFakeRate_madgraphWJets->SetLineColor(kBlue);
  electronFakeRate_madgraphWJets->SetMarkerColor(kBlue);
  electronFakeRate_pythiaQCDEM->SetLineColor(kMagenta);
  electronFakeRate_pythiaQCDEM->SetMarkerColor(kMagenta);
  electronFakeRate_madgraphQCD->SetLineColor(kGreen);
  electronFakeRate_madgraphQCD->SetMarkerColor(kGreen);
 
  leg1->AddEntry(electronFakeRate_pythiaWJets, (chargeTypeName + fakeTypeString + eventTypeName + "PythiaWJets").c_str(), "LP");
  leg1->AddEntry(electronFakeRate_madgraphWJets, (chargeTypeName + fakeTypeString + eventTypeName + "MadgraphWJets").c_str(), "LP"); 
  leg1->AddEntry(electronFakeRate_pythiaQCDEM, (chargeTypeName + fakeTypeString + eventTypeName + "PythiaQCDEM").c_str(), "LP"); 
  leg1->AddEntry(electronFakeRate_madgraphQCD, (chargeTypeName + fakeTypeString + eventTypeName + "MadgraphQCD").c_str(), "LP"); 
 
  electronFakeRate_pythiaWJets->Draw();
  electronFakeRate_madgraphWJets->Draw("same");
  electronFakeRate_pythiaQCDEM->Draw("same");
  electronFakeRate_madgraphQCD->Draw("same");
  leg1->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}

//--------------------------------------------------------------------------------------------------
// Make plots of fake rates for different charges
//--------------------------------------------------------------------------------------------------
void compareChargeDependantFakeRates(char *FakeRateFilename , string sampleName, int denominatortype, int faketype, 
                                     int onlychargetype = -1000, int onlyeventType = -1000 ) {

  string fakeTypeString = "";
  if (faketype == 11) {
    fakeTypeString = "Electron";
  } else if (faketype == 13) {
    fakeTypeString = "Muon";
  } else {
    cerr << "Error: faketype = " << faketype << " not recognized.\n";
    assert(false);
  }

  string denominatorTypeString = "";
  if (denominatortype == 1) {
    denominatorTypeString = "TrackDenominator";
  }

  string plotname = "ChargeDependant" + fakeTypeString + denominatorTypeString + "FakeRateComparison_" + sampleName;

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);
  TLegend *leg1=0;
  leg1 = new TLegend(0.65,0.6,0.9,0.8);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);

  int color = 0;
  for (int chargetype = -1; chargetype <= 1; chargetype += 2) {
    for (int eventType= -1; eventType <= 1; eventType ++) {

      if (!(onlychargetype == chargetype || onlychargetype == -1000))
        continue;

      if (onlyeventType == 0) {
        if (eventType != 0)
          continue;
      } else {
        if (eventType == 0)
          continue;
      }

      //different charges
      string chargeTypeName = "";
      string LegendString = sampleName + " ";

      string eventTypeName = "";
      if (eventType == -1) {
        eventTypeName = "MinusW";
        LegendString += "-";
      } else if (eventType == 1) {
          eventTypeName = "PlusW";
          LegendString += "+";
      } else if (eventType == 0)
        LegendString += "Fake";
        
      if (chargetype == -1) {
        chargeTypeName = "Minus";
        LegendString += "-";
      } else if (chargetype == 1) {
        chargeTypeName = "Plus";
        LegendString += "+";
      }
           
      TFile *file = new TFile(FakeRateFilename, "READ");
      file->cd();
      
      TH1F *tmp = (TH1F*)file->FindObjectAny((chargeTypeName + fakeTypeString + eventTypeName + denominatorTypeString + "FakeRate_Pt_" + sampleName).c_str());
      if (!tmp) {
        cerr << "can't find :"  << chargeTypeName + fakeTypeString + eventTypeName + denominatorTypeString + "FakeRate_Pt_" + sampleName << endl;
        assert(tmp);
      }
      tmp->SetLineColor(COLORS[color]);
      tmp->SetMarkerColor(COLORS[color]);    
      tmp->SetAxisRange(0,100);

      color++;
      leg1->AddEntry(tmp, LegendString.c_str(), "LP");
      if (chargetype == -1 && eventType == -1)
        tmp->Draw();
      else 
        tmp->Draw("same");
    }
  }
  
  leg1->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}

//--------------------------------------------------------------------------------------------------
// Make plots of fake rates for different charges
//--------------------------------------------------------------------------------------------------
void compareChargeDependantFakeRatesPythiaVsMadgraph(char *FakeRateFilename , int faketype, int chargetype ) {

  string fakeTypeString = "";
  if (faketype == 11) {
    fakeTypeString = "Electron";
  } else if (faketype == 13) {
    fakeTypeString = "Muon";
  } else {
    cerr << "Error: faketype = " << faketype << " not recognized.\n";
    assert(false);
  }

  //different charges
  string chargeTypeName = "";
  if (chargetype == -1)
    chargeTypeName = "Minus";
  else if (chargetype == 1)
    chargeTypeName = "Plus";
  
  string plotname = chargeTypeName + fakeTypeString + "FakeRateComparisonPythiaVsMadgraph";

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);
  TLegend *leg1=0;
  leg1 = new TLegend(0.65,0.6,0.9,0.8);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);

  int color = 0;
  
  for (int eventType= -1; eventType <= 1; eventType += 2) {
    for (int sample=0;sample<=1;sample++) {

      //sample names
      string sampleName = "";
      if (sample == 0)
        sampleName = "PythiaWJets";
      else if (sample == 1)
        sampleName = "MadgraphWJets";

      string LegendString = sampleName + " ";
     
      string eventTypeName = "";
      if (eventType == -1) {
        eventTypeName = "MinusW";
        LegendString += "-";
      } else if (eventType == 1) {
        eventTypeName = "PlusW";
        LegendString += "+";
      }
      if (chargetype == -1) {        
        LegendString += "-";
      } else if (chargetype == 1) {      
        LegendString += "+";
      }

      
      TFile *file = new TFile(FakeRateFilename, "READ");
      file->cd();
      
      TH1F *tmp = (TH1F*)file->FindObjectAny((chargeTypeName + fakeTypeString + eventTypeName + "FakeRate_Pt_" + sampleName).c_str());
      if (!tmp) {
        cerr << "can't find :"  << chargeTypeName + fakeTypeString + eventTypeName + "FakeRate_Pt_" + sampleName << endl;
        assert(tmp);
      }
      tmp->SetLineColor(COLORS[color]);
      tmp->SetMarkerColor(COLORS[color]);    
      color++;
      leg1->AddEntry(tmp, LegendString.c_str(), "LP");
      if (sample == -1 && eventType == -1)
        tmp->Draw();
      else 
        tmp->Draw("same");
    }
  }
  
  leg1->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}

//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void computeFakeRates(char *FakeRateFilename , bool fitFakeRate = false ) {

  //computeFakeRatesFromWJets(FakeRateFilename, fitFakeRate, 0, 0);
  //computeFakeRatesFromQCD(FakeRateFilename , fitFakeRate, 0, 0 );

//   computeFakeRatesFromWJets(FakeRateFilename, fitFakeRate, 1,1);
//   computeFakeRatesFromWJets(FakeRateFilename, fitFakeRate, 1,-1);  
//   computeFakeRatesFromWJets(FakeRateFilename, fitFakeRate, 1,0);
//   computeFakeRatesFromWJets(FakeRateFilename, fitFakeRate, -1,1);  
//   computeFakeRatesFromWJets(FakeRateFilename, fitFakeRate, -1,-1);  
//   computeFakeRatesFromWJets(FakeRateFilename, fitFakeRate, -1,0);  
  
   computeElectronFakeRatesWithTrackDenominatorFromWJets(FakeRateFilename, fitFakeRate, 1,1);
   computeElectronFakeRatesWithTrackDenominatorFromWJets(FakeRateFilename, fitFakeRate, 1,-1);
   computeElectronFakeRatesWithTrackDenominatorFromWJets(FakeRateFilename, fitFakeRate, 1,0);
   computeElectronFakeRatesWithTrackDenominatorFromWJets(FakeRateFilename, fitFakeRate, -1,1);
   computeElectronFakeRatesWithTrackDenominatorFromWJets(FakeRateFilename, fitFakeRate, -1,-1);
   computeElectronFakeRatesWithTrackDenominatorFromWJets(FakeRateFilename, fitFakeRate, -1,0);


  //compareElectronFakeRates(FakeRateFilename,0);
  //compareMuonFakeRates(FakeRateFilename,0);
  //compareElectronFakeRates(FakeRateFilename,1);
  //compareMuonFakeRates(FakeRateFilename,1);
  //compareElectronFakeRates(FakeRateFilename,-1);
  //compareMuonFakeRates(FakeRateFilename,-1);

  // compareChargeDependantFakeRates(FakeRateFilename,"PythiaWJets", 0, 11  );
  // compareChargeDependantFakeRates(FakeRateFilename,"PythiaWJets", 0,13 );
  // compareChargeDependantFakeRates(FakeRateFilename,"MadgraphWJets",0, 11 );
  // compareChargeDependantFakeRates(FakeRateFilename,"MadgraphWJets",0, 13 );

  //compareChargeDependantFakeRates(FakeRateFilename, "PythiaWJets",0,11,-1000,0 );
  //compareChargeDependantFakeRates(FakeRateFilename,"PythiaWJets", 0,13,-1000,0 );
  
  //compareChargeDependantFakeRatesPythiaVsMadgraph(FakeRateFilename, 11, 1 );
  //compareChargeDependantFakeRatesPythiaVsMadgraph(FakeRateFilename, 11, -1 );
  //compareChargeDependantFakeRatesPythiaVsMadgraph(FakeRateFilename, 13, 1 );
  //compareChargeDependantFakeRatesPythiaVsMadgraph(FakeRateFilename, 13, -1 );

//Track denominator plots
  compareChargeDependantFakeRates(FakeRateFilename,"PythiaWJets", 1, 11  );



  //test(FakeRateFilename);

  return;
}


