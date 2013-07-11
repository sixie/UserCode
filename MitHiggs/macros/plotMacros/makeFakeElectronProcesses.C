//root -l $CMSSW_BASE/src/MitHiggs/macros/plotMacros/makeFakeElectronProcesses.C+


#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <string>
#include <TFile.h>
#include <TMath.h>
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

//--------------------------------------------------------------------------------------------------
// Get Histogram function
//--------------------------------------------------------------------------------------------------
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
// 
//--------------------------------------------------------------------------------------------------
void makeSSOSRatioAsFunctionOfVariableCut( vector<string> datasetfiles, vector<string> datasetnames,
                                           string variableName, string isolation = "" ) {


  TLegend *leg1=0;
  leg1 = new TLegend(0.7,0.7,0.9,0.9);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.02);
  
  string plotname = "SSOSRatio_" + isolation + "Electron" + variableName;
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  TH1F *SSDistribution = addAllSamples(datasetfiles, datasetnames, 
                                       "ElectronIDChargeCorrelationStudyMod", 
                                       "hSameSignFake"+isolation+"Electron"+variableName);

  TH1F *OSDistribution = addAllSamples(datasetfiles, datasetnames, 
                                       "ElectronIDChargeCorrelationStudyMod", 
                                       "hOppositeSignFake"+isolation+"Electron"+variableName);

  TH1F *SSPassCut = (TH1F*)SSDistribution->Clone("SSPassCut");
  for (int i = 1; i<=SSDistribution->GetXaxis()->GetNbins();i++) {
    if (variableName == "DeltafBrem") {
      SSPassCut->SetBinContent(i,SSDistribution->Integral(i,SSDistribution->GetXaxis()->GetNbins()+1));
    } else {
      SSPassCut->SetBinContent(i,SSDistribution->Integral(0,i));
    }
  }

  TH1F *OSPassCut = (TH1F*)OSDistribution->Clone("OSPassCut");
  for (int i = 1; i<=OSDistribution->GetXaxis()->GetNbins();i++) {
    if (variableName == "DeltafBrem") {
      OSPassCut->SetBinContent(i,OSDistribution->Integral(i,OSDistribution->GetXaxis()->GetNbins()+1));
      
    } else {
      OSPassCut->SetBinContent(i,OSDistribution->Integral(0,i));
    }
  }

  TH1F *SSOSRatio = (TH1F*)SSPassCut->Clone(("SSOSRatio_"+isolation + "Electron" + variableName).c_str());
  SSOSRatio->Divide(OSPassCut);

  SSOSRatio->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

}

//--------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------
void makeSSOSRatioAsFunctionOfEOverPCut( vector<string> datasetfiles, vector<string> datasetnames,
                                         string isolation = "" ) {


  TLegend *leg1=0;
  leg1 = new TLegend(0.7,0.7,0.9,0.9);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.02);
  
  string plotname = "SSOSRatio_" + isolation + "ElectronESuperClusterOverP" ;
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  TH1F *SSDistribution = addAllSamples(datasetfiles, datasetnames, 
                                       "ElectronIDChargeCorrelationStudyMod", 
                                       "hSameSignFake"+isolation+"ElectronESuperClusterOverP");

  TH1F *OSDistribution = addAllSamples(datasetfiles, datasetnames, 
                                       "ElectronIDChargeCorrelationStudyMod", 
                                       "hOppositeSignFake"+isolation+"ElectronESuperClusterOverP");

  TH1F *SSPassCut = new TH1F("SSPassCut",";fabs(EOverP - 1);Number of Events", 100, 0, 1);
  for (int i = 1; i<=SSDistribution->GetXaxis()->GetNbins();i++) {
    double x = 0.01 * i;    
    double sum = 0;
    for (int j = 1; j<=SSDistribution->GetXaxis()->GetNbins();j++) {
      if(fabs(SSDistribution->GetXaxis()->GetBinLowEdge(j) - 1) < x)
        sum += SSDistribution->GetBinContent(j);
    }
    SSPassCut->SetBinContent(i,sum);
  }

  TH1F *OSPassCut = new TH1F("OSPassCut",  ";fabs(EOverP - 1);Number of Events", 100, 0, 1);
  for (int i = 1; i<=OSDistribution->GetXaxis()->GetNbins();i++) {
    double x = 0.01 * i;    
    double sum = 0;
    for (int j = 1; j<=OSDistribution->GetXaxis()->GetNbins();j++) {
      if(fabs(OSDistribution->GetXaxis()->GetBinLowEdge(j) - 1) < x)
        sum += OSDistribution->GetBinContent(j);
    }
    OSPassCut->SetBinContent(i,sum);
  }

  TH1F *SSOSRatio = (TH1F*)SSPassCut->Clone(("abs(SSOSRatio_"+isolation + "ElectronESuperClusterOverP - 1)").c_str());
  SSOSRatio->GetYaxis()->SetTitle("SS/OS  Ratio");
  SSOSRatio->Divide(OSPassCut);

  SSOSRatio->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

}


//--------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------
void makeSSOSRatioVsVariable( vector<string> datasetfiles, vector<string> datasetnames,
                                         string variableName, string isolation = "" ) {


  TLegend *leg1=0;
  leg1 = new TLegend(0.7,0.7,0.9,0.9);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.02);
  
  string plotname = "SSOSRatio_" + isolation + "Electron" + variableName ;
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  TH1F *SSDistribution = addAllSamples(datasetfiles, datasetnames, 
                                       "ElectronIDChargeCorrelationStudyMod", 
                                       "hSameSignFake"+isolation+"Electron" + variableName);

  TH1F *OSDistribution = addAllSamples(datasetfiles, datasetnames, 
                                       "ElectronIDChargeCorrelationStudyMod", 
                                       "hOppositeSignFake"+isolation+"Electron" + variableName);


  TH1F *SSOSRatio = (TH1F*)SSDistribution->Clone(("SSOSRatio_"+isolation + "Electron" + variableName).c_str());
  SSOSRatio->GetYaxis()->SetTitle("SS/OS  Ratio");
  SSOSRatio->Divide(OSDistribution);

  SSOSRatio->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

}



//--------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------
void makePlot( vector<string> datasetfiles, vector<string> datasetnames,
               string histName ) {


 
  string plotname = histName ;
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);


  //do plots here

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

}


//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void makeFakeElectronProcesses()
{

  TCanvas *cv = MakeCanvas("cv", "test", 800, 600);

  Double_t xbins[51];
  for (int i=0;i<51; i++) {
    xbins[i] = 0 + 2*i;
  }

  vector<string> madgraphWJetsDatasetFiles;
  vector<string> madgraphWJetsDatasetNames;
  vector<string> pythiaWJetsDatasetFiles;
  vector<string> pythiaWJetsDatasetNames;
  vector<string> QCDEMDatasetFiles;
  vector<string> QCDEMDatasetNames;

  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************
  madgraphWJetsDatasetFiles.clear(); 
  madgraphWJetsDatasetNames.clear();
  pythiaWJetsDatasetFiles.clear();
  pythiaWJetsDatasetNames.clear();
  QCDEMDatasetFiles.clear();
  QCDEMDatasetNames.clear();
  
  //Madgraph WJets
  madgraphWJetsDatasetFiles.push_back("/home/sixie/hist/ElectronIDChargeStudy/filler/006/ElectronIDChargeStudy_f8-wjets-mg-id9.root");
  madgraphWJetsDatasetNames.push_back("f8-wjets-mg-id9");
  //Pythia WJets
  pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/ElectronIDChargeStudy/filler/006/ElectronIDChargeStudy_s8-we-id9.root");
  pythiaWJetsDatasetNames.push_back("s8-we-id9");
  pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/ElectronIDChargeStudy/filler/006/ElectronIDChargeStudy_s8-wm-id9.root");
  pythiaWJetsDatasetNames.push_back("s8-wm-id9");
  pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/ElectronIDChargeStudy/filler/006/ElectronIDChargeStudy_s8-wtau-id9.root");
  pythiaWJetsDatasetNames.push_back("s8-wtau-id9");
  
  
  makePlot("histname");

   return;
   
}
