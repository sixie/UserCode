//root -l $CMSSW_BASE/src/MitPhysics/macros/plotWSingleFakeStudy.C+


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






string kristianFilename = "/build/sixie/releases/development/CMSSW_2_1_11/src/UserCode/KristianHahn/fakeEstimates/test/fakeestimate-eledenom-atoa.root";
string myFilename = "~/ntuples/WSingleFakeStudy/filler/006/WSingleFakeStudy_s8-qcdem_20_30-id9.root";
//string myFilename = "~/ntuples/WSingleFakeStudy/filler/006/WSingleFakeStudy_s8-qcdem_30_80-id9.root";
//string myFilename = "~/ntuples/WSingleFakeStudy/filler/006/WSingleFakeStudy_s8-qcdem_80_170-id9.root";


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

void makeComparisonWithKristian(string plotname, string histName, vector<string> datasetFiles, 
                                vector<string> datasetNames ) {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  assert(datasetFiles.size() > 0);
  TH1F *myHist = getHisto(datasetFiles[0], "SingleFakeValidationMod", histName);

  //Add histograms
  for (int i=0; i < datasetFiles.size() ;i++) {

    cout << i << "  sss " << endl;
    double CrossSection = xstab.Get(datasetNames[i].c_str());
    TFile* tmpfile = new TFile(datasetFiles[i].c_str(),"READ"); assert(tmpfile);
    TDirectory *FWDir = tmpfile->GetDirectory("AnaFwkMod"); assert(FWDir);
    TH1F* tmpHist = (TH1F*)FWDir->Get("hDEvents");  assert(tmpHist);
    double NEvents = tmpHist->Integral();
    double weight = CrossSection * 1.0 / NEvents;
    cout << "done\n";
    if (i==0) {
      myHist->Scale(weight);
    } else {
      TH1F *tmpHist = getHisto(datasetFiles[i], "SingleFakeValidationMod", histName);
      tmpHist->Scale(weight);
      myHist->Add(tmpHist);
    }
  }

  TFile *kristianFile = new TFile(kristianFilename.c_str());

  TH1F* kristianHist;
  if (histName == "hFakeElectronEt") {
    kristianHist = (TH1F*)(kristianFile->Get("fake_et_weighted"));
  } else if (histName == "hFakeMt") {
    kristianHist = (TH1F*)(kristianFile->Get("fake_mt_weighted"));
  } else if (histName == "fFakeMet") {
    kristianHist = (TH1F*)(kristianFile->Get("fake_met_weighted"));
  } else {
    cout << "unsupported histogram.\n";
    return;
  }

  cerr << "here" << endl;

  if (!kristianHist) {
    cerr << "cannot get Kristian hist\n";
    return;
  }

  myHist->SetLineColor(kBlue);
  myHist->SetLineWidth(2);
  if (myHist->Integral() > 0)
    //myHist->Scale(kristianHist->Integral()/myHist->Integral());
  
  kristianHist->SetLineColor(kRed);
  kristianHist->SetLineWidth(2);

  myHist->Draw();
  kristianHist->Draw("same");


  TLegend *leg1=0;
  leg1 = new TLegend(0.5,0.6,0.7,0.8);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);
  leg1->AddEntry(myHist, "MyHist", "LP"); 
  leg1->AddEntry(kristianHist, "KristianHist", "LP"); 
  leg1->Draw();
  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

}


void makeFakeRateVsMonteCarloFakeComparison(string plotname, string fakeHistName, 
                                            string MCFakeHistName, 
                                            vector<string> datasetFiles, 
                                            vector<string> datasetNames ) {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  assert(datasetFiles.size() > 0);
  TH1F *fakeHist = getHisto(datasetFiles[0], "SingleFakeValidationMod", fakeHistName);
  TH1F *MCFakeHist = getHisto(datasetFiles[0], "SingleFakeValidationMod", MCFakeHistName);

  //Add histograms
  for (int i=0; i < datasetFiles.size() ;i++) {

    cout << i << "  sss " << endl;
    double CrossSection = xstab.Get(datasetNames[i].c_str());
    TFile* tmpfile = new TFile(datasetFiles[i].c_str(),"READ"); assert(tmpfile);
    TDirectory *FWDir = tmpfile->GetDirectory("AnaFwkMod"); assert(FWDir);
    TH1F* tmpHist = (TH1F*)FWDir->Get("hDEvents");  assert(tmpHist);
    double NEvents = tmpHist->Integral();
    double weight = CrossSection * 1.0 / NEvents;
    cout << "done\n";
    if (i==0) {
      fakeHist->Scale(weight);
      MCFakeHist->Scale(weight);
    } else {
      TH1F *tmpHist = getHisto(datasetFiles[i], "SingleFakeValidationMod", fakeHistName);
      tmpHist->Scale(weight);
      fakeHist->Add(tmpHist);
      tmpHist = getHisto(datasetFiles[i], "SingleFakeValidationMod", MCFakeHistName);
      tmpHist->Scale(weight);
      MCFakeHist->Add(tmpHist);
    }
  }

  fakeHist->SetLineColor(kBlue);
  fakeHist->SetLineWidth(2);  
  MCFakeHist->SetLineColor(kRed);
  MCFakeHist->SetLineWidth(2);
  fakeHist->Draw();
  MCFakeHist->Draw("same");


  TLegend *leg1=0;
  leg1 = new TLegend(0.5,0.6,0.7,0.8);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);
  leg1->AddEntry(fakeHist, "FakeRateApplied", "LP"); 
  leg1->AddEntry(MCFakeHist, "MCFake", "LP"); 
  leg1->Draw();
  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

}

// void makeFakeRateVsMonteCarloFakeComparison(string plotname, string histFile, string histName ) {

//   TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

//   TLegend *leg1=0;
//   leg1 = new TLegend(0.5,0.6,0.7,0.8);   
//   leg1->SetBorderSize(1);
//   leg1->SetTextSize(0.03);
//   //leg1->AddEntry(hHWWSignal, signalLegendName.c_str(), "LP"); 
//   leg1->Draw();
//   string filename = plotname + ".gif";
//   cv->SaveAs(filename.c_str());
//   cv->Delete();

// }

//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void plotWSingleFakeStudy()
{
  //gStyle->SetPalette(1);

  //Samples
  vector<string> datasetFiles;
  vector<string> datasetNames;
  datasetFiles.push_back("/home/sixie/ntuples/WSingleFakeStudy/filler/006/WSingleFakeStudy_s8-qcdem_20_30-id9.root");
  datasetNames.push_back("s8-qcdem_20_30-id9");
  datasetFiles.push_back("/home/sixie/ntuples/WSingleFakeStudy/filler/006/WSingleFakeStudy_s8-qcdem_30_80-id9.root");
  datasetNames.push_back("s8-qcdem_30_80-id9");
  datasetFiles.push_back("/home/sixie/ntuples/WSingleFakeStudy/filler/006/WSingleFakeStudy_s8-qcdem_80_170-id9.root");
  datasetNames.push_back("s8-qcdem_80_170-id9");




  bool     doLeg   = true;
  double   legx[2] = { 0.5, 0.7 };
  double   legy[2] = { 0.6, 0.8 };
  double   legTextSize = 0.03;
  TString legTitle = "";

  string plotname;
  string filename;

  makeComparisonWithKristian("ElectronFakeEtComparisonWithKristian","hFakeElectronEt",datasetFiles, datasetNames);

  //makeComparisonWithKristian("ElectronFakeMt",myFilename,"hFakeMt");

  //makeComparisonWithKristian("ElectronFakeMet",myFilename,"hFakeMet");

  makeFakeRateVsMonteCarloFakeComparison("ElectronFakeEtComparisonWithMC","hFakeElectronEt",
                                         "hMCFakeElectronEt",datasetFiles, datasetNames);

  return;

}
