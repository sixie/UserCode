//root -l makePlotsForNote.C

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

using namespace std;
//using namespace mithep;
#endif

#include <vector>


Double_t LUMINOSITY = 200.0;
//cross section table
mithep::SimpleTable xstab("$CMSSW_BASE/src/MitPhysics/data/xs.dat");  

//define colors
Int_t COLORS[10] = {kRed,kBlue,kMagenta,kGreen,kOrange,kCyan,kGreen+3,kRed+3,kBlue+3,kGray};
Int_t MARKERS[10] = {20,21,22,23,24,25,26,27,28,30};
Int_t SysCOLORS[10] = {kBlue,kMagenta,kRed,kGreen,kOrange,kCyan,kGreen+3,kRed+3,kBlue+3,kGray};

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

TH2F* get2DHisto(string filename, string directoryname, string histoname) {
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

  TH2F *hist = (TH2F*)dir->Get(histoname.c_str());
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
// Rebin for 1D hists
//--------------------------------------------------------------------------------------------------
TH1F* rebin(TH1F* hist, vector<double> xlowedges) {

  Double_t xbins[xlowedges.size()];
  for (UInt_t i=0;i<xlowedges.size();i++) {
    xbins[i] = xlowedges[i];
  }
  Double_t BinContent[xlowedges.size()+1];
  Double_t BinError[xlowedges.size()+1];
  for (UInt_t binx=0; binx < xlowedges.size()+1 ;++binx) {
      BinContent[binx] = 0;
      BinError[binx] = 0;
  }

  TH1F *rebinHist = new TH1F(hist->GetName(), hist->GetTitle(), xlowedges.size() - 1, xbins);
  rebinHist->GetXaxis()->SetTitle(hist->GetXaxis()->GetTitle());
  rebinHist->GetYaxis()->SetTitle(hist->GetYaxis()->GetTitle());

  //refill
  for (UInt_t i=0;int(i)<=hist->GetXaxis()->GetNbins()+1;++i) {
    Double_t x = hist->GetXaxis()->GetBinCenter(i);
    UInt_t xbin = 0;

    //Find which x rebinned bin we are in
    for (UInt_t binx=0; binx < xlowedges.size()+1 ;++binx) {
      if (binx == 0) { //underflow 
        if (x <= xlowedges[0]) {
          xbin = 0;
          break;
        }
      } else if (binx < xlowedges.size()) {
        if (x > xlowedges[binx-1] && x <= xlowedges[binx]) {
          xbin = binx;
          break;
        }
      } else { //overflow
        if (x > xlowedges[binx]) {
          xbin = binx;
          break;
        }
      }
    }
    BinContent[xbin] += hist->GetBinContent(i);
    BinError[xbin] += hist->GetBinError(i)*hist->GetBinError(i);
  }
  
  for (UInt_t binx=0; binx < xlowedges.size()+1 ;++binx) {
    rebinHist->SetBinContent(binx,BinContent[binx]);
    rebinHist->SetBinError(binx,TMath::Sqrt(BinError[binx]));     
  } 
  
  //return newHist;
  return rebinHist;
}

//--------------------------------------------------------------------------------------------------
// Rebin for 2D hists
//--------------------------------------------------------------------------------------------------
TH2F* rebin(TH2F* hist, vector<double> xlowedges, vector<double> ylowedges) {

  Double_t xLow[xlowedges.size()];
  Double_t yLow[xlowedges.size()];
  Double_t BinContent[xlowedges.size()+1][ylowedges.size()+1];
  Double_t BinError[xlowedges.size()+1][ylowedges.size()+1];
  for (UInt_t binx=0; binx < xlowedges.size()+1 ;++binx) {
    for (UInt_t biny=0; biny < ylowedges.size()+1 ;++biny) {
      BinContent[binx][biny] = 0;
      BinError[binx][biny] = 0;
    }
  }

  for (UInt_t i=0; i<xlowedges.size();++i)
    xLow[i] = xlowedges[i];
  for (UInt_t i=0; i<ylowedges.size();++i)
    yLow[i] = ylowedges[i];

  TH2F *rebinHist = new TH2F(hist->GetName(), hist->GetTitle(), xlowedges.size() - 1, xLow, ylowedges.size() - 1, yLow);

  //refill the histogram
  for (UInt_t i=0;int(i)<=hist->GetXaxis()->GetNbins()+1;++i) {
    for (UInt_t j=0;int(j)<=hist->GetYaxis()->GetNbins()+1;++j) {
      
      Double_t x = hist->GetXaxis()->GetBinCenter(i);
      Double_t y = hist->GetYaxis()->GetBinCenter(j);
      UInt_t xbin = 0;
      UInt_t ybin = 0;

      //Find which x rebinned bin we are in
      for (UInt_t binx=0; binx < xlowedges.size()+1 ;++binx) {
        if (binx == 0) { //underflow 
          if (x <= xlowedges[0]) {
            xbin = 0;
            break;
          }
        } else if (binx < xlowedges.size()) {
          if (x > xlowedges[binx-1] && x <= xlowedges[binx]) {
            xbin = binx;
            break;
          }
        } else { //overflow
          if (x > xlowedges[binx]) {
            xbin = binx;
            break;
          }
        }
      }
      //Find which y rebinned bin we are in
      for (UInt_t biny=0; biny < ylowedges.size()+1 ;++biny) {
        if (biny == 0) { //underflow 
          if (y <= ylowedges[0]) {
            //cout << " underflow ";
            ybin = 0;
            break;
          }
        } else if (biny < ylowedges.size()) {
          if (y > ylowedges[biny-1] && y <= ylowedges[biny]) {
            //cout << " range " << ylowedges[biny-1] << " - " << ylowedges[biny] << " ";
            ybin = biny;
            break;
          }
        } else { //overflow
          if (y > ylowedges[biny]) {
            //cout << " overflow ";
            ybin = biny;
            break;
          }
        }
      }
      BinContent[xbin][ybin] += hist->GetBinContent(i,j);
      BinError[xbin][ybin] += hist->GetBinError(i,j)*hist->GetBinError(i,j);     
    } 
  }

  for (UInt_t binx=0; binx < xlowedges.size()+1 ;++binx) {
    for (UInt_t biny=0; biny < ylowedges.size()+1 ;++biny) {
      rebinHist->SetBinContent(binx,biny,BinContent[binx][biny]);
      rebinHist->SetBinError(binx,biny,TMath::Sqrt(BinError[binx][biny]));     
    }
  } 

  return rebinHist;
}



//--------------------------------------------------------------------------------------------------
// Add histograms form multiple files together weighted by cross-section
//--------------------------------------------------------------------------------------------------
TH1F* addAllSamples(vector<string> datasetFiles, vector<string> datasetNames,
                    string dirName, string histName, vector<double> bins = vector<double>(0)) {

  assert(datasetFiles.size() > 0);
  TH1F *tmp = getHisto(datasetFiles[0], dirName, histName);
  if (bins.size() > 0) tmp = rebin(tmp, bins);
  assert(tmp);
  TH1F *finalHist = (TH1F*)tmp->Clone();
  finalHist->Sumw2();

  for (UInt_t i=0; i < datasetFiles.size() ;i++) {
    double CrossSection = xstab.Get(datasetNames[i].c_str());
    TH1F *tmpNEventsHist = getHisto(datasetFiles[i],"AnaFwkMod", "hDEvents");
    double NEvents = tmpNEventsHist->Integral();
    double weight = CrossSection * LUMINOSITY / NEvents;
    if (i==0) {
      for (int b=0;b<=finalHist->GetNbinsX()+1;b++) {
        finalHist->SetBinContent(b,finalHist->GetBinContent(b)*weight);
        finalHist->SetBinError(b,finalHist->GetBinError(b)*weight);
      }
    } else {
      TH1F *tmpHist = getHisto(datasetFiles[i], dirName, histName);
      if (bins.size() > 0) tmpHist = rebin(tmpHist, bins);
      for (int b=0;b<=finalHist->GetNbinsX();b++) {
        if (!TMath::IsNaN(tmpHist->GetBinContent(b))) {
          tmpHist->SetBinContent(b,tmpHist->GetBinContent(b)*weight);
          tmpHist->SetBinError(b,tmpHist->GetBinError(b)*weight);
        } else {
          tmpHist->SetBinContent(b,0);
          tmpHist->SetBinError(b,0);          
        }
      }    
      finalHist->Add(tmpHist);
    }
  }
  return finalHist;
}

TH2F* addAllSamples2D(vector<string> datasetFiles, vector<string> datasetNames,
                    string dirName, string histName, vector<double> xbins, vector<double> ybins) {

  assert(datasetFiles.size() > 0);
  TH2F *tmp = get2DHisto(datasetFiles[0], dirName, histName);
  TH2F *tmpRebinned = rebin(tmp,xbins,ybins);
  //TH2F *tmpRebinned = tmp;
  assert(tmp);
  TH2F *finalHist = (TH2F*)tmpRebinned->Clone();
  finalHist->Sumw2();

  for (UInt_t i=0; i < datasetFiles.size() ;i++) {
    double CrossSection = xstab.Get(datasetNames[i].c_str());
    TH2F *tmpNEventsHist = get2DHisto(datasetFiles[i],"AnaFwkMod", "hDEvents");
    double NEvents = tmpNEventsHist->Integral();
    double weight = CrossSection * LUMINOSITY / NEvents;
    if (i==0) {
      for (int b=0;b<=finalHist->GetNbinsX()+1;b++) {
        for (int c=0;c<=finalHist->GetNbinsX()+1;c++) {
          finalHist->SetBinContent(b,c,finalHist->GetBinContent(b,c)*weight);
          finalHist->SetBinError(b,c,finalHist->GetBinError(b,c)*weight);
        }
      }
    } else {
      TH2F *tmpHist = get2DHisto(datasetFiles[i], dirName, histName);
      TH2F *tmpRebinnedHist = rebin(tmpHist,xbins,ybins);
      for (int b=0;b<=tmpRebinnedHist->GetNbinsX()+1;b++) {
        for (int c=0;c<=tmpRebinnedHist->GetNbinsY()+1;c++) {
          tmpRebinnedHist->SetBinContent(b,c,tmpRebinnedHist->GetBinContent(b,c)*weight);
          tmpRebinnedHist->SetBinError(b,c,tmpRebinnedHist->GetBinError(b,c)*weight);
        } 
      }
      finalHist->Add(tmpRebinnedHist);
    }
  }
  return finalHist;
}

//--------------------------------------------------------------------------------------------------
// Add histograms in a stack from multiple files together weighted by cross-section 
//--------------------------------------------------------------------------------------------------
THStack* addAllSamplesStacked(vector<vector<string> > datasetFiles, vector<vector<string> > datasetNames, 
                              string dirName, string histName, 
                              vector<double> bins = vector<double>(0), string xAxisTitle = "",
                              string yAxisTitle = "") {

  assert(datasetFiles.size() > 0);
  assert(datasetFiles[0].size() > 0);
  TH1F *tmp = getHisto(datasetFiles[0][0], dirName, histName);
  assert(tmp);

  THStack *stackedHist = new THStack(tmp->GetName(),"");

  for (UInt_t i=0; i < datasetFiles.size() ;++i) {

    TH1F *tmpTotalHist = 0;
    for (UInt_t j=0; j < datasetFiles[i].size() ;++j) {
    
      double CrossSection = xstab.Get(datasetNames[i][j].c_str());
      TH1F *tmpNEventsHist = getHisto(datasetFiles[i][j],"AnaFwkMod", "hDEvents");
      double NEvents = tmpNEventsHist->Integral();
      double weight = CrossSection * LUMINOSITY / NEvents;
      
      TH1F *tmpHist = getHisto(datasetFiles[i][j], dirName, histName);
      TH1F *tmpRebinnedHist = tmpHist;

      if (bins.size() > 0 ) tmpRebinnedHist = rebin(tmpHist, bins);
      
      if (xAxisTitle != "") tmpRebinnedHist->SetXTitle(xAxisTitle.c_str());
      if (yAxisTitle != "") tmpRebinnedHist->SetYTitle(yAxisTitle.c_str());

      if (tmpRebinnedHist) {
        tmpRebinnedHist->SetTitle(datasetNames[i][j].c_str());
        
        //scale by weight
        for (int b=0;b<=tmpRebinnedHist->GetNbinsX();b++) {
          tmpRebinnedHist->SetBinContent(b,tmpRebinnedHist->GetBinContent(b)*weight);
          tmpRebinnedHist->SetBinError(b,tmpRebinnedHist->GetBinError(b)*weight);
        }    
        
        if (j==0) {
          tmpTotalHist = (TH1F*)tmpRebinnedHist->Clone();
          tmpTotalHist->Sumw2();
        } else {
          tmpTotalHist->Add(tmpRebinnedHist);
        }
      } else {
        cerr << "could not get histogram " << datasetNames[i][j] << "\n";
      }
    }
    //add to stack
    tmpTotalHist->SetFillStyle(1001);
    tmpTotalHist->SetFillColor(COLORS[i]);
    tmpTotalHist->SetLineWidth(1); 

//     for (int b=0;b<=tmpTotalHist->GetXaxis()->GetNbins();b++) {
//       cerr << tmpTotalHist->GetXaxis()->GetBinLabel(b) << ": ---> :";
//       if (string(tmpTotalHist->GetXaxis()->GetBinLabel(b)) == string("")) {
//         tmpTotalHist->GetXaxis()->SetBinLabel(b,"empty");
//         cerr << "changed ---> :";
//       }
//       cerr << tmpTotalHist->GetXaxis()->GetBinLabel(b) << ":" << endl;
//     }

    stackedHist->Add(tmpTotalHist);    
  }
  //if (xAxisLabel != "") stackedHist->GetHistogram()->SetXTitle(xAxisLabel.c_str());
  //if (yAxisLabel != "") stackedHist->GetHistogram()->SetYTitle(yAxisLabel.c_str());
  
  return stackedHist;
}


//--------------------------------------------------------------------------------------------------
// Create Fake Rate Histogram. Use binomial Errors
//--------------------------------------------------------------------------------------------------
TH1F* createFakeRatePtHist(vector<string> datasetFiles, vector<string> datasetNames, string dirName,
                           string numeratorHistname, string denominatorHistname, string histname, 
                           Double_t minEventsPerBin = 5) {

  vector<double> fakebins;
  vector<double> ptbins;
  ptbins.push_back(0);   
  ptbins.push_back(10);  
  ptbins.push_back(15);  
  ptbins.push_back(20);  
  ptbins.push_back(25);  
  ptbins.push_back(30);  
  ptbins.push_back(35);  
  ptbins.push_back(40);
  ptbins.push_back(50);   
  ptbins.push_back(80);  
  ptbins.push_back(120);  
  ptbins.push_back(170);  
  ptbins.push_back(250);

  TH1F *denominator = addAllSamples(datasetFiles, datasetNames, dirName, denominatorHistname);
  TH1F *numerator = addAllSamples(datasetFiles, datasetNames, dirName, numeratorHistname);
  
  TH1F *n = rebin(numerator,ptbins);
  TH1F *d = rebin(denominator,ptbins);

  TH1F *fakeRate = (TH1F*)n->Clone(histname.c_str());
  fakeRate->GetYaxis()->SetTitle("Fake Rate");
  //divide by the denominator to get fake rate, with binomial errors
  fakeRate->Divide(n, d, 1.0,1.0,"B");  
  return fakeRate;
}

//--------------------------------------------------------------------------------------------------
// Create Fake Rate Histogram. Use binomial Errors
//--------------------------------------------------------------------------------------------------
TH1F* createFakeRateEtaHist(vector<string> datasetFiles, vector<string> datasetNames, string dirName,
                           string numeratorHistname, string denominatorHistname, string histname, 
                           Double_t minEventsPerBin = 5) {

  vector<double> fakebins;
  vector<double> etabins;
  etabins.push_back(-3.25);
  etabins.push_back(-2.75);
  etabins.push_back(-2.25);
  etabins.push_back(-1.75);
  etabins.push_back(-1.25);
  etabins.push_back(-0.75);
  etabins.push_back(-0.25);
  etabins.push_back(0.25);
  etabins.push_back(0.75);
  etabins.push_back(1.25);
  etabins.push_back(1.75);
  etabins.push_back(2.25);
  etabins.push_back(2.75);
  etabins.push_back(3.25);
  
  TH1F *denominator = addAllSamples(datasetFiles, datasetNames, dirName, denominatorHistname);
  TH1F *numerator = addAllSamples(datasetFiles, datasetNames, dirName, numeratorHistname);

  TH1F *n = numerator;
  TH1F *d = denominator;  
   n = rebin(numerator,etabins);
   d = rebin(denominator,etabins);

  TH1F *fakeRate = (TH1F*)n->Clone(histname.c_str());
  fakeRate->GetYaxis()->SetTitle("Fake Rate");
  //divide by the denominator to get fake rate, with binomial errors
  fakeRate->Divide(n, d, 1.0,1.0,"B");  
  return fakeRate;
}



//--------------------------------------------------------------------------------------------------
// Create Fake Rate Histogram. Use binomial Errors
//--------------------------------------------------------------------------------------------------
TH2F* createFakeRatePtEtaHist(vector<string> datasetFiles, vector<string> datasetNames, string dirName,
                              string numeratorHistname, string denominatorHistname, string histname, 
                              Double_t minEventsPerBin = 5) {
  
  vector<double> fakebins;
  vector<double> ptbins;
  ptbins.push_back(0);   
  ptbins.push_back(10);  
  ptbins.push_back(15);  
  ptbins.push_back(20);  
  ptbins.push_back(25);  
  ptbins.push_back(30);  
  ptbins.push_back(35);  
  ptbins.push_back(40);
  ptbins.push_back(50);   
  ptbins.push_back(80);  
  ptbins.push_back(120);  
  ptbins.push_back(170);  
  ptbins.push_back(250);
  vector<double> etabins;
  etabins.push_back(-3.25);
  etabins.push_back(-2.75);
  etabins.push_back(-2.25);
  etabins.push_back(-1.75);
  etabins.push_back(-1.25);
  etabins.push_back(-0.75);
  etabins.push_back(-0.25);
  etabins.push_back(0.25);
  etabins.push_back(0.75);
  etabins.push_back(1.25);
  etabins.push_back(1.75);
  etabins.push_back(2.25);
  etabins.push_back(2.75);
  etabins.push_back(3.25);

  TH2F *denominator = addAllSamples2D(datasetFiles, datasetNames, dirName, denominatorHistname,ptbins,etabins);
  TH2F *numerator = addAllSamples2D(datasetFiles, datasetNames, dirName, numeratorHistname,ptbins,etabins);

  TH2F *fakeRate = (TH2F*)numerator->Clone(histname.c_str());
  fakeRate->GetYaxis()->SetTitle("Fake Rate");
  //divide by the denominator to get fake rate, with binomial errors
  fakeRate->Divide(numerator, denominator, 1.0,1.0,"B");  
  return fakeRate;
}

//--------------------------------------------------------------------------------------------------
// Draw the FakeRate histograms and fit functions
//--------------------------------------------------------------------------------------------------

void drawFakeRatePlots( vector<TH1F*> fakeRateHistograms, vector<TF1*> fakeRateFits,
                        vector<string> fakeRateLegendNames , string plotname ) {

  Bool_t fitFakeRate = true;
  if (!fitFakeRate || fakeRateHistograms.size() != fakeRateFits.size())
    fitFakeRate = false;

  assert(fakeRateHistograms.size() == fakeRateLegendNames.size());
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  TLegend *leg1=0;
  leg1 = new TLegend(0.25,0.8,0.65,0.9);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.025);
  

  Double_t maxy = 0.0;
  Double_t totaly = 0.0;
  for (UInt_t i=0; i<fakeRateHistograms.size() ; i++) {    
    totaly += fakeRateHistograms[i]->GetMaximum();
    if (fakeRateHistograms[i]->GetMaximum() > maxy) 
      maxy = fakeRateHistograms[i]->GetMaximum();
  }
  Double_t avgy = 0.0;
  if (fakeRateHistograms.size() > 0) avgy = totaly / fakeRateHistograms.size();
  
  for (UInt_t i=0; i<fakeRateHistograms.size() ; i++) {    
    fakeRateHistograms[i]->SetMarkerColor(COLORS[i]);
    fakeRateHistograms[i]->SetMinimum(0.0);
    fakeRateHistograms[i]->SetMaximum(maxy * 1.2);
    if (maxy == 1.0)
      fakeRateHistograms[i]->SetMaximum(0.1);
    fakeRateHistograms[i]->SetLineColor(COLORS[i]);
    
    if (fitFakeRate) fakeRateFits[i]->SetLineColor(COLORS[i]);    
    leg1->AddEntry(fakeRateHistograms[i], fakeRateLegendNames[i].c_str(), "LP"); 
  
    if (i==0) { 
      fakeRateHistograms[i]->Draw("E1");
    } else {
      fakeRateHistograms[i]->Draw("E1same");
    }    
    if (fitFakeRate)  fakeRateFits[i]->Draw("E1same");    
  }

  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);
  leg1->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}

//--------------------------------------------------------------------------------------------------
// Draw a Stack
//--------------------------------------------------------------------------------------------------
void drawStackedPlot(THStack *stackedHist , string plotname, vector<string> legendNames,
                     Bool_t logY = false, double MaxY = -99, double MinX = -99, double MaxX = -99,
                     double legendX1 = -99, double legendY1 = -99, 
                     double legendX2 = -99, double legendY2 = -99,
                     TH1F *hist = NULL, string histLegendLabel = "") {

  if (stackedHist->GetStack()->GetEntries() != int(legendNames.size())) {
    cerr << "Number of entries in the stack is not equal to the number of legend labels given\n";
    assert(stackedHist->GetStack()->GetEntries() == int(legendNames.size()));
  }

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  TLegend *leg1=0;
  if (legendX1 == -99) {
    leg1 = new TLegend(0.65,0.70,0.95,0.95);   
  } else {
    leg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);   
  }
  leg1->SetBorderSize(1);  
  leg1->SetTextSize(0.03);
  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);

  for (int i=0; i < stackedHist->GetStack()->GetEntries(); ++i) {
    leg1->AddEntry(stackedHist->GetStack()->At(i),legendNames[i].c_str(), "F"); 
//     for (int b=0;b<=dynamic_cast<TH1*>(stackedHist->GetStack()->At(0))->GetXaxis()->GetNbins()+1;b++) {
//       cerr << "Stack[" << i << "] bin " << b << dynamic_cast<TH1*>(stackedHist->GetStack()->At(i))->GetXaxis()->GetBinLabel(b) << ":" <<  endl;
//     }
    dynamic_cast<TH1*>(stackedHist->GetStack()->At(i))->SetTitle("");
  }

  stackedHist->Draw("hist");

  stackedHist->SetMinimum(0.1);
  if (MaxY > 0) stackedHist->SetMaximum(MaxY);
  if (MinX != -99 && MaxX != -99) stackedHist->GetXaxis()->SetRangeUser(MinX, MaxX);

  stackedHist->GetXaxis()->SetTitle(((TH1F*)(stackedHist->GetHists()->At(0)))->GetXaxis()->GetTitle());
  stackedHist->GetYaxis()->SetTitle(((TH1F*)(stackedHist->GetHists()->At(0)))->GetYaxis()->GetTitle());

  //For some crazy reason doing draw changes the bin labels...have to reset it to the correct ones.
//   for (int b=0;b<=stackedHist->GetXaxis()->GetNbins()+1;b++) {
//     cerr << "bin " << b << stackedHist->GetXaxis()->GetBinLabel(b) << ":" <<  endl;
//     stackedHist->GetXaxis()->SetBinLabel(b, dynamic_cast<TH1*>(stackedHist->GetStack()->At(0))->GetXaxis()->GetBinLabel(b));
//     cerr << "bin " << b << stackedHist->GetXaxis()->GetBinLabel(b) << "--" << endl;
// }

  if (hist) {
    leg1->AddEntry(hist,histLegendLabel.c_str(), "FP"); 
    hist->SetLineColor(kBlack);
    hist->SetMarkerColor(kBlack);
    hist->Draw("same");
  }

  leg1->Draw();
  if (logY) cv->SetLogy();

  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());
  cv->Delete();
}

void makeEtaSliceComparisonPlots( TH2F* hist1, TH2F* hist2 ,TH2F* hist3, string filename, string plotname, 
                                  string legend1 = "", string legend2 = "", string legend3 = "", 
                                  string xAxisLabel = "" , string yAxisLabel = "", double maxY = -1,
                                  double legendX1 = -99, double legendY1 = -99, 
                                  double legendX2 = -99, double legendY2 = -99) {
   
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 900);
  cv->Divide(2,3);

  int padIndex = 1;

  for(UInt_t j=7; j <= 12; j++) {
    char tmp[20]; 
    sprintf(tmp, " :  Eta %.2f To %.2f",  hist1->GetYaxis()->GetBinLowEdge(j), 
            hist1->GetYaxis()->GetBinUpEdge(j));
    string sliceEta = tmp;

    TH1D *slice1 = hist1->ProjectionX("projectionX1" , j,j);
    TH1D *slice2 = hist2->ProjectionX("projectionX2" , j,j);
    TH1D *slice3 = hist3->ProjectionX("projectionX3" , j,j);
    slice1->SetTitle((plotname + " " + sliceEta).c_str());

    if (xAxisLabel != "") slice1->GetXaxis()->SetTitle(xAxisLabel.c_str());
    slice1->GetXaxis()->SetTitleOffset(1.0);
    if (yAxisLabel != "") slice1->GetYaxis()->SetTitle(yAxisLabel.c_str());
    slice1->GetYaxis()->SetTitleOffset(1.5);

    double MAXY = 0.04;
    if (maxY > 0) MAXY = maxY;

    slice1->GetXaxis()->SetRangeUser(0,75);
    slice2->GetXaxis()->SetRangeUser(0,75);
    slice3->GetXaxis()->SetRangeUser(0,75);

    slice1->SetMaximum(MAXY);
    slice2->SetMaximum(MAXY);
    slice3->SetMaximum(MAXY);
    slice1->SetMinimum(0.0);
    slice2->SetMinimum(0.0);
    slice3->SetMinimum(0.0);

    TLegend *leg1=0;
    if (legendX1 > -99) {
      leg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);   
    } else {
      leg1 = new TLegend(0.25,0.75,0.55,0.9);   
    }
    leg1->SetBorderSize(1);
    leg1->SetTextSize(0.03);
    if (legend1 == "") {
      leg1->AddEntry(slice1, hist1->GetName(), "LP"); 
    } else {
      leg1->AddEntry(slice1, legend1.c_str(), "LP"); 
    }
    if (legend2 == "") {
      leg1->AddEntry(slice2, hist2->GetName(), "LP"); 
    } else {
      leg1->AddEntry(slice2, legend2.c_str(), "LP"); 
    }
    
    if (legend3 == "") {
      leg1->AddEntry(slice3, hist3->GetName(), "LP"); 
    } else {
      leg1->AddEntry(slice3, legend3.c_str(), "LP"); 
    }

    slice1->SetMarkerColor(COLORS[0]);
    slice1->SetLineColor(kBlack);
    slice1->SetMarkerSize(0.75);
    slice1->SetMarkerStyle(MARKERS[0]);
    slice2->SetMarkerColor(COLORS[1]);
    slice2->SetLineColor(kBlack);
    slice2->SetMarkerSize(0.75);
    slice2->SetMarkerStyle(MARKERS[1]);
    slice3->SetMarkerColor(COLORS[2]);
    slice3->SetLineColor(kBlack);
    slice3->SetMarkerSize(0.75);
    slice3->SetMarkerStyle(MARKERS[2]);
   
    cv->cd(padIndex++);
    slice1->DrawCopy();
    slice2->DrawCopy("same");
    slice3->DrawCopy("same");
    leg1->Draw();    
  }

  cv->SaveAs((filename + ".gif").c_str());
  cv->SaveAs((filename + ".eps").c_str());
  return;
}

void makePtSliceComparisonPlots( TH2F* hist1, TH2F* hist2 ,TH2F* hist3 , string filename, string plotname, 
                                 string legend1 = "", string legend2 = "", string legend3 = "", 
                                 string xAxisLabel = "" , string yAxisLabel = "", double maxY = -1,
                                  double legendX1 = -99, double legendY1 = -99, 
                                  double legendX2 = -99, double legendY2 = -99) {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 900);
  cv->Divide(2,3);

  int padIndex = 1;

  for(UInt_t i=2; i <= 6 ;i++) {
    char tmp[20]; 
    sprintf(tmp, " :  Pt %.1f To %.1f",  hist1->GetXaxis()->GetBinLowEdge(i),hist1->GetXaxis()->GetBinUpEdge(i) );
    string slicePt = tmp;

    TH1D *slice1 =0;
    TH1D *slice2 =0;
    TH1D *slice3 =0;
    
    slice1 = hist1->ProjectionY("projectionY1" , i,i);
    slice2 = hist2->ProjectionY("projectionY2" , i,i);
    slice3 = hist3->ProjectionY("projectionY3" , i,i);
    slice1->SetTitle((plotname + " " + slicePt).c_str());

    if (xAxisLabel != "") slice1->GetXaxis()->SetTitle(xAxisLabel.c_str());
    slice1->GetXaxis()->SetTitleOffset(1.0);
    if (yAxisLabel != "") slice1->GetYaxis()->SetTitle(yAxisLabel.c_str());
    slice1->GetYaxis()->SetTitleOffset(1.5);

    double MAXY = 0.04;
    if (maxY > 0) MAXY = maxY;
 
    slice1->SetMaximum(MAXY);
    slice1->SetMinimum(0.0);
    slice2->SetMaximum(MAXY);
    slice2->SetMinimum(0.0);
    slice3->SetMaximum(MAXY);
    slice3->SetMinimum(0.0);

    TLegend *leg1=0;
    if (legendX1 > -99) {
      leg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);   
    } else {
      leg1 = new TLegend(0.65,0.75,0.95,0.9);   
    }
    leg1->SetBorderSize(1);
    leg1->SetTextSize(0.03);
    if (legend1 == "") {
      leg1->AddEntry(slice1, hist1->GetName(), "LP"); 
    } else {
      leg1->AddEntry(slice1, legend1.c_str(), "LP"); 
    }
    if (legend2 == "") {
      leg1->AddEntry(slice2, hist2->GetName(), "LP"); 
    } else {
      leg1->AddEntry(slice2, legend2.c_str(), "LP"); 
    }
    if (legend3 == "") {
      leg1->AddEntry(slice3, hist3->GetName(), "LP"); 
    } else {
      leg1->AddEntry(slice3, legend3.c_str(), "LP"); 
    }
    
    slice1->SetMarkerColor(COLORS[0]);
    slice1->SetLineColor(kBlack);
    slice1->SetMarkerSize(0.75);
    slice1->SetMarkerStyle(MARKERS[0]);
    slice2->SetMarkerColor(COLORS[1]);
    slice2->SetLineColor(kBlack);
    slice2->SetMarkerSize(0.75);
    slice2->SetMarkerStyle(MARKERS[1]);
    slice3->SetMarkerColor(COLORS[2]);
    slice3->SetLineColor(kBlack);
    slice3->SetMarkerSize(0.75);
    slice3->SetMarkerStyle(MARKERS[2]);
    
    cv->cd(padIndex++);
    slice1->DrawCopy();
    slice2->DrawCopy("same");
    slice3->DrawCopy("same");
    leg1->Draw();    
  }

  cv->SaveAs((filename + ".gif").c_str());
  cv->SaveAs((filename + ".eps").c_str());
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


//--------------------------------------------------------------------------------------------------
void makeCrossDirComparisonPlot( vector<string> datasetfiles, vector<string> datasetnames, 
                                 vector<string> dirnames, vector<string> dirnamelabel, 
                                 vector<string> histNames,vector<string> legendNames, 
                                 vector<double> bins, string plotname , string xAxisLabel = "",
                                 string yAxisLabel = "",
                                 double xlow = -99, double xhigh = -99, 
                                 double ylow = -99, double yhigh = -99, 
                                 double legendX1 = -99 , double legendX2 = -99, 
                                 double legendY1 = -99, double legendY2 = -99, Bool_t useLogY = false ) {

  assert(histNames.size() > 0);
  assert(histNames.size() == legendNames.size());
  assert(datasetfiles.size() == datasetnames.size());
  assert(dirnames.size() == dirnamelabel.size());
   
  vector <TH1F*> hists;
  Double_t MAXY = 0.0;

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  TLegend *leg = 0;
  if(legendX1 == -99) {
    leg = new TLegend(0.55,0.75,0.9,0.9);     
  } else {
    leg = new TLegend(legendX1,legendY1,legendX2,legendY2);     
  }

  leg->SetBorderSize(1);
  leg->SetTextSize(0.03);

  int colorindex = 0;
  for(UInt_t i=0;i<histNames.size();++i) {
    for(UInt_t d=0;d<dirnames.size();++d) {
      
      TH1F *tmphist = addAllSamples(datasetfiles, datasetnames, dirnames[d], 
                                    histNames[i]);
      
      TH1F *hist = tmphist;
      if (bins.size() > 0) hist = rebin(tmphist,bins);
      hist->SetTitle(plotname.c_str());
      hist->SetMarkerColor(COLORS[colorindex]);
      hist->SetLineColor(COLORS[colorindex]);
      hist->SetMarkerStyle(MARKERS[colorindex]);
      hist->SetMarkerSize(1.0);
      if (xlow != -99 && xhigh != -99) {
        hist->GetXaxis()->SetRangeUser(xlow,xhigh);
      } 
      if (xAxisLabel != "") hist->SetXTitle(xAxisLabel.c_str());
      if (yAxisLabel != "") hist->SetYTitle(yAxisLabel.c_str());


      leg->AddEntry(hist, (legendNames[i]+dirnamelabel[d]).c_str(), "LP");       
      hists.push_back(hist);
      if (hist->GetMaximum() > MAXY) MAXY = hist->GetMaximum();

      cout << hist->GetName() << " " << hist->GetXaxis()->GetNbins() << " " << hist->GetBinContent(5) << endl;
      ++colorindex;

    }
  }

  if (useLogY) cv->SetLogy();

  for (UInt_t i=0; i < hists.size();++i) {

    cout << i << " " << hists[i] << endl;
    hists[i]->SetMaximum(1.2*MAXY);
    hists[i]->SetMinimum(0.0);
    hists[i]->SetTitle("");

    if (ylow != -99) hists[i]->SetMinimum(ylow);
    if (yhigh != -99) hists[i]->SetMaximum(yhigh);

    if (i==0) 
      hists[i]->Draw("E1");
    else
      hists[i]->Draw("E1same");
  } 

  leg->SetShadowColor(kWhite);
  leg->SetFillColor(kWhite);
  leg->Draw();
  
  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());
  cv->Delete();
}


//--------------------------------------------------------------------------------------------------
// Compare histograms from different directories with systematic errors added to the histogram
// in the first given directory.
//--------------------------------------------------------------------------------------------------
void makeComparisonPlotWithSystematics( vector<string> datasetfiles, vector<string> datasetnames, 
                                        vector<string> dirnames, vector<string> dirnamelabel, 
                                        vector<string> histNames, vector<string> errorhistNames, vector<string> legendNames, 
                                        vector<double> bins, string plotname , string xAxisLabel = "",
                                        string yAxisLabel = "",
                                        double xlow = -99, double xhigh = -99, 
                                        double ylow = -99, double yhigh = -99, 
                                        double legendX1 = -99 , double legendX2 = -99, 
                                        double legendY1 = -99, double legendY2 = -99, Bool_t useLogY = false ) {
  
  assert(histNames.size() == 1);
  assert(histNames.size() == legendNames.size());
  assert(datasetfiles.size() == datasetnames.size());
  assert(dirnames.size() == dirnamelabel.size());
    
  assert(errorhistNames.size() == histNames.size());
 
  vector <TH1F*> hists;
  Double_t MAXY = 0.0;

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  TLegend *leg = 0;
  if(legendX1 == -99) {
    leg = new TLegend(0.55,0.75,0.9,0.9);     
  } else {
    leg = new TLegend(legendX1,legendY1,legendX2,legendY2);     
  }

  leg->SetBorderSize(1);
  leg->SetTextSize(0.03);
  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);

  int colorindex = 0;
  for(UInt_t i=0;i<histNames.size();++i) {
    for(UInt_t d=0;d<dirnames.size();++d) {
      
      TH1F *tmphist = addAllSamples(datasetfiles, datasetnames, dirnames[d], 
                                    histNames[i]);
      TH1F *tmpErrorHist = addAllSamples(datasetfiles, datasetnames, dirnames[d], 
                                    errorhistNames[i]);
      
      if (tmphist->GetXaxis()->GetNbins() != tmpErrorHist->GetXaxis()->GetNbins()) {
        cerr << "Error: histNames[" << i << "] and errorhistNames[" << i << "] do not have the same bins.\n";
        assert(tmphist->GetXaxis()->GetNbins() == tmpErrorHist->GetXaxis()->GetNbins());
      }

      TH1F *hist = tmphist;
      TH1F *errorHist = tmpErrorHist;
      if (bins.size() > 0) { 
        hist = rebin(tmphist,bins);
        errorHist = rebin(tmpErrorHist,bins);
      }

      hist->SetTitle(plotname.c_str());
      hist->SetMarkerColor(SysCOLORS[colorindex]);
      hist->SetLineColor(SysCOLORS[colorindex]);
      hist->SetFillColor(SysCOLORS[colorindex]);
      hist->SetFillStyle(3544);
      hist->SetMarkerStyle(MARKERS[colorindex]);
      hist->SetMarkerSize(1.0);
      if (xlow != -99 && xhigh != -99) {
        hist->GetXaxis()->SetRangeUser(xlow,xhigh);
      } 
      if (xAxisLabel != "") hist->SetXTitle(xAxisLabel.c_str());
      if (yAxisLabel != "") hist->SetYTitle(yAxisLabel.c_str());

      leg->AddEntry(hist, (legendNames[i]+dirnamelabel[d]).c_str(), "LP");       
      hists.push_back(hist);
      
      if (hist->GetMaximum() > MAXY) MAXY = hist->GetMaximum();

      //for the first directory, add systematic errors to the statistical error
      if (d==0) {
        for (int bin = 0; bin < hist->GetNbinsX()+1; ++bin) {
          Double_t totalError = TMath::Sqrt(hist->GetBinError(bin)*hist->GetBinError(bin) +
                                            errorHist->GetBinContent(bin)*errorHist->GetBinContent(bin));
          if (TMath::IsNaN(errorHist->GetBinContent(bin))) cout << "NAN!!\n";
          hist->SetBinError(bin, totalError);
        }
      }      
      ++colorindex;
    }
  }

  if (useLogY) cv->SetLogy();

  for (UInt_t i=0; i < hists.size();++i) {

    hists[i]->SetMaximum(1.2*MAXY);
    hists[i]->SetMinimum(0.0);

    if (ylow != -99) hists[i]->SetMinimum(ylow);
    if (yhigh != -99) hists[i]->SetMaximum(yhigh);

    if (i==0) 
      hists[i]->Draw("e5");
    else
      hists[i]->Draw("same");
  } 
  leg->Draw();
  
  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());
  cv->Delete();
}


void drawPlots( vector<TH1F*> fakeRateHistograms, 
                vector<string> fakeRateLegendNames , string filename, string plotname , 
                string xAxisLabel = "" , string yAxisLabel = "",
                double MINY = -1, double MAXY = -1,
                Bool_t normalize = false, Bool_t useLogY = false, 
                double legendX1 = -99, double legendY1 = -99, double legendX2 = -99, double legendY2 = -99) {

  assert(fakeRateHistograms.size() == fakeRateLegendNames.size());
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  TLegend *leg1=0;
  if(legendX1 == -99) {
    leg1 = new TLegend(0.65,0.8,0.9,0.9);   
  } else {
    leg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);     
  }
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.025);
  
  if (normalize) {
    for (UInt_t i=0; i<fakeRateHistograms.size() ; i++) {  
      Double_t scaleFactor = 0;
      scaleFactor = 1 / fakeRateHistograms[i]->Integral();
      for (int c=0;c<=fakeRateHistograms[i]->GetNbinsX()+1;c++) {        
        fakeRateHistograms[i]->SetBinContent(c,fakeRateHistograms[i]->GetBinContent(c)*scaleFactor);
        fakeRateHistograms[i]->SetBinError(c,fakeRateHistograms[i]->GetBinError(c)*scaleFactor);
      }
    }
  }

  Double_t maxy = -99.0;
  Double_t miny = -99.0;
  for (UInt_t i=0; i<fakeRateHistograms.size() ; i++) {    
    if (fakeRateHistograms[i]->GetMaximum() > maxy || maxy == -99.0) 
      maxy = fakeRateHistograms[i]->GetMaximum();

    if (fakeRateHistograms[i]->GetMinimum() < miny || miny == -99.0) 
      miny = fakeRateHistograms[i]->GetMinimum();

  }
  maxy = maxy * 1.2;
  miny = miny * 0.5;
  if (normalize) maxy = 1.0;
  if (MAXY > 0) maxy = MAXY;
  if (miny == 0 && useLogY) miny = 0.001;
  if (MINY > 0) miny = MINY;

  for (UInt_t i=0; i<fakeRateHistograms.size() ; i++) {    
    fakeRateHistograms[i]->SetMarkerColor(COLORS[i]);
    fakeRateHistograms[i]->SetMinimum(miny);
    fakeRateHistograms[i]->SetMaximum(maxy);
    fakeRateHistograms[i]->SetLineColor(COLORS[i]);
    fakeRateHistograms[i]->SetMarkerStyle(MARKERS[i]);
    fakeRateHistograms[i]->SetTitleOffset(1.35, "X");
    fakeRateHistograms[i]->SetLabelSize(0.035, "X");

    if (xAxisLabel != "") fakeRateHistograms[i]->GetXaxis()->SetTitle(xAxisLabel.c_str());
    if (yAxisLabel != "") fakeRateHistograms[i]->GetYaxis()->SetTitle(yAxisLabel.c_str());
    fakeRateHistograms[i]->SetTitle("");

    leg1->AddEntry(fakeRateHistograms[i], fakeRateLegendNames[i].c_str(), "LP"); 
  
    if (i==0) { 
      fakeRateHistograms[i]->DrawCopy("E1");
    } else {
      fakeRateHistograms[i]->DrawCopy("E1same");
    }    
    

  }


  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);
  leg1->Draw();

  if (useLogY) cv->SetLogy();
  cv->SaveAs((filename+".gif").c_str());
  cv->SaveAs((filename+".eps").c_str());
  cv->Delete();
}



//--------------------------------------------------------------------------------------------------
// Make 2D Fake rate slice plots
//--------------------------------------------------------------------------------------------------
void make2DElectronFakeRateSlicePlots() {

  //Load FakeRate file
  TFile *file = new TFile("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/FakeRates.root", "UPDATE");
  file->cd();
  
  vector<double> etabins;
  etabins.push_back(-3.25);
  etabins.push_back(-2.75);
  etabins.push_back(-2.25);
  etabins.push_back(-1.75);
  etabins.push_back(-1.25);
  etabins.push_back(-0.75);
  etabins.push_back(-0.25);
  etabins.push_back(0.25);
  etabins.push_back(0.75);
  etabins.push_back(1.25);
  etabins.push_back(1.75);
  etabins.push_back(2.25);
  etabins.push_back(2.75);
  etabins.push_back(3.25);
  vector<double> ptbins;
  ptbins.push_back(0);   
  ptbins.push_back(10);  
  ptbins.push_back(15);  
  ptbins.push_back(20);  
  ptbins.push_back(25);  
  ptbins.push_back(30);  
  ptbins.push_back(60);  


  //*****************************************************************************************
  //2D Fake Rates For GsfSC 
  //*****************************************************************************************
   TH2F *GsfSCElectronFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny(
     "GsfTrackSCElectronFakeRate_PtEta_MadgraphWJets");
   assert(GsfSCElectronFakeRate_PtEta_MadgraphWJets);

   TH2F *GsfSCElectronFakeRate_PtEta_Madgraph_JetTriggerSample = (TH2F*)file->FindObjectAny(
     "GsfTrackSCElectronFakeRate_PtEta_Madgraph_Jet50");
   assert(GsfSCElectronFakeRate_PtEta_Madgraph_JetTriggerSample);

   TH2F *GsfSCElectronFakeRate_PtEta_Madgraph_PhotonTriggerSample = (TH2F*)file->FindObjectAny(
     "GsfTrackSCElectronFakeRate_PtEta_Madgraph_Photon15");
   assert(GsfSCElectronFakeRate_PtEta_Madgraph_PhotonTriggerSample);
   
   makeEtaSliceComparisonPlots(GsfSCElectronFakeRate_PtEta_MadgraphWJets, 
                               GsfSCElectronFakeRate_PtEta_Madgraph_JetTriggerSample, 
                               GsfSCElectronFakeRate_PtEta_Madgraph_PhotonTriggerSample,
                               "GsfTrackElectronFakeRateEtaSlices", 
                               "GsfTrack Electron Fake Rate", 
                               "WJets", "JetTriggerSample", "PhotonTriggerSample", 
                               "electron p_{T} [GeV/c]", "#epsilon_{fake}", 0.075, 0.15,0.75,0.45,0.9);

//    GsfSCElectronFakeRate_PtEta_MadgraphWJets = rebin(GsfSCElectronFakeRate_PtEta_MadgraphWJets,ptbins,etabins);
//    GsfSCElectronFakeRate_PtEta_Madgraph_JetTriggerSample = rebin(GsfSCElectronFakeRate_PtEta_Madgraph_JetTriggerSample,ptbins,etabins);
//    GsfSCElectronFakeRate_PtEta_Madgraph_PhotonTriggerSample = rebin(GsfSCElectronFakeRate_PtEta_Madgraph_PhotonTriggerSample,ptbins,etabins);

   makePtSliceComparisonPlots(GsfSCElectronFakeRate_PtEta_MadgraphWJets, 
                              GsfSCElectronFakeRate_PtEta_Madgraph_JetTriggerSample, 
                              GsfSCElectronFakeRate_PtEta_Madgraph_PhotonTriggerSample,
                              "GsfTrackElectronFakeRatePtSlices", 
                              "GsfTrack Electron Fake Rate", 
                              "WJets", "JetTriggerSample", "PhotonTriggerSample", 
                              "electron #eta", "#epsilon_{fake}", 0.075, 0.65,0.75,0.95,0.9);


  //*****************************************************************************************
  //Compare WJets Vs QCD FakeRates For Reco 
  //*****************************************************************************************
   TH2F *RecoElectronFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny(
     "RecoElectronFakeRate_PtEta_MadgraphWJets");
   assert(RecoElectronFakeRate_PtEta_MadgraphWJets);

   TH2F *RecoElectronFakeRate_PtEta_Madgraph_JetTriggerSample = (TH2F*)file->FindObjectAny(
     "RecoElectronFakeRate_PtEta_Madgraph_Jet50");
   assert(RecoElectronFakeRate_PtEta_Madgraph_JetTriggerSample);

   TH2F *RecoElectronFakeRate_PtEta_Madgraph_PhotonTriggerSample = (TH2F*)file->FindObjectAny(
     "RecoElectronFakeRate_PtEta_Madgraph_Photon15");
   assert(RecoElectronFakeRate_PtEta_Madgraph_PhotonTriggerSample);
   

   makeEtaSliceComparisonPlots(RecoElectronFakeRate_PtEta_MadgraphWJets, 
                               RecoElectronFakeRate_PtEta_Madgraph_JetTriggerSample, 
                               RecoElectronFakeRate_PtEta_Madgraph_PhotonTriggerSample, 
                               "RecoElectronFakeRateEtaSlices",
                               "Reco Electron Fake Rate", 
                               "WJets", "JetTriggerSample", "PhotonTriggerSample", 
                               "electron p_{T} [GeV/c]", "#epsilon_{fake}", 0.25, 0.15,0.75,0.45,0.9);


//    RecoElectronFakeRate_PtEta_MadgraphWJets = rebin(RecoElectronFakeRate_PtEta_MadgraphWJets,ptbins,etabins);
//    RecoElectronFakeRate_PtEta_JetTriggerSample = rebin(RecoElectronFakeRate_PtEta_Madgraph_JetTriggerSample,ptbins,etabins);
//    RecoElectronFakeRate_PtEta_PhotonTriggerSample = rebin(RecoElectronFakeRate_PtEta_Madgraph_PhotonTriggerSample,ptbins,etabins);
   makePtSliceComparisonPlots(RecoElectronFakeRate_PtEta_MadgraphWJets, 
                              RecoElectronFakeRate_PtEta_Madgraph_JetTriggerSample, 
                              RecoElectronFakeRate_PtEta_Madgraph_PhotonTriggerSample,
                              "RecoElectronFakeRatePtSlices", 
                              "Reco Electron Fake Rate", 
                              "WJets", "JetTriggerSample", "PhotonTriggerSample",
                              "electron #eta", "#epsilon_{fake}", 0.25);

  file->Close();
  return;

}

//--------------------------------------------------------------------------------------------------
// Make 2D Fake rate slice plots
//--------------------------------------------------------------------------------------------------
void make2DMuonFakeRateSlicePlots() {

  //Load FakeRate file
  TFile *file = new TFile("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/FakeRates.root", "UPDATE");
  file->cd();
  
  vector<double> etabins;
  etabins.push_back(-3.25);
  etabins.push_back(-2.75);
  etabins.push_back(-2.25);
  etabins.push_back(-1.75);
  etabins.push_back(-1.25);
  etabins.push_back(-0.75);
  etabins.push_back(-0.25);
  etabins.push_back(0.25);
  etabins.push_back(0.75);
  etabins.push_back(1.25);
  etabins.push_back(1.75);
  etabins.push_back(2.25);
  etabins.push_back(2.75);
  etabins.push_back(3.25);
  vector<double> ptbins;
  ptbins.push_back(0);   
  ptbins.push_back(10);  
  ptbins.push_back(15);  
  ptbins.push_back(20);  
  ptbins.push_back(25);  
  ptbins.push_back(30);  
  ptbins.push_back(100);  

  //*****************************************************************************************
  //Compare WJets Vs QCD FakeRates For IsoTrack 
  //*****************************************************************************************
   TH2F *IsoTrackMuonFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny(
     "IsoTrackMuonFakeRate_PtEta_MadgraphWJets");
   assert(IsoTrackMuonFakeRate_PtEta_MadgraphWJets);

   TH2F *IsoTrackMuonFakeRate_PtEta_Madgraph_JetTriggerSample = (TH2F*)file->FindObjectAny(
     "IsoTrackMuonFakeRate_PtEta_Madgraph_Jet50");
   assert(IsoTrackMuonFakeRate_PtEta_Madgraph_JetTriggerSample);

   TH2F *IsoTrackMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample = (TH2F*)file->FindObjectAny(
     "IsoTrackMuonFakeRate_PtEta_Madgraph_Photon15");
   assert(IsoTrackMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample);

   makeEtaSliceComparisonPlots(IsoTrackMuonFakeRate_PtEta_MadgraphWJets, 
                               IsoTrackMuonFakeRate_PtEta_Madgraph_JetTriggerSample, 
                               IsoTrackMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample,
                               "IsoTrackMuonFakeRateEtaSlices", 
                               "IsoTrack Muon Fake Rate", 
                               "WJets", "JetTriggerSample", "PhotonTriggerSample", 
                               "muon p_{T} [GeV/c]", "#epsilon_{fake}", 0.005, 0.15,0.75,0.45,0.9);

//    IsoTrackMuonFakeRate_PtEta_MadgraphWJets = rebin(IsoTrackMuonFakeRate_PtEta_MadgraphWJets, ptbins, etabins);
//    IsoTrackMuonFakeRate_PtEta_Madgraph_JetTriggerSample = rebin(IsoTrackMuonFakeRate_PtEta_Madgraph_JetTriggerSample, ptbins, etabins);
//    IsoTrackMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample = rebin(IsoTrackMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample, ptbins, etabins);
   makePtSliceComparisonPlots(IsoTrackMuonFakeRate_PtEta_MadgraphWJets, 
                              IsoTrackMuonFakeRate_PtEta_Madgraph_JetTriggerSample, 
                              IsoTrackMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample,
                              "IsoTrackMuonFakeRatePtSlices", 
                              "IsoTrack Muon Fake Rate", 
                              "WJets", "JetTriggerSample", "PhotonTriggerSample", 
                              "muon #eta", "#epsilon_{fake}", 0.0075, 0.35,0.75,0.65,0.9);

  //*****************************************************************************************
  //Compare WJets Vs QCD FakeRates For TrackerMuon 
  //*****************************************************************************************
   TH2F *TrackerMuonFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny(
     "TrackerMuonFakeRate_PtEta_MadgraphWJets");
   assert(TrackerMuonFakeRate_PtEta_MadgraphWJets);

   TH2F *TrackerMuonFakeRate_PtEta_Madgraph_JetTriggerSample = (TH2F*)file->FindObjectAny(
     "TrackerMuonFakeRate_PtEta_Madgraph_Jet50");
   assert(TrackerMuonFakeRate_PtEta_Madgraph_JetTriggerSample);

   TH2F *TrackerMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample = (TH2F*)file->FindObjectAny(
     "TrackerMuonFakeRate_PtEta_Madgraph_Photon15");
   assert(TrackerMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample);


   makeEtaSliceComparisonPlots(TrackerMuonFakeRate_PtEta_MadgraphWJets, 
                               TrackerMuonFakeRate_PtEta_Madgraph_JetTriggerSample, 
                               TrackerMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample, 
                               "TrackerMuonFakeRateEtaSlices",
                               "TrackerMuon Muon Fake Rate", 
                               "WJets", "JetTriggerSample", "PhotonTriggerSample", 
                               "muon p_{T} [GeV/c]", "#epsilon_{fake}", 1.0, 0.55,0.75,0.95,0.9);

//    TrackerMuonFakeRate_PtEta_MadgraphWJets = rebin(TrackerMuonFakeRate_PtEta_MadgraphWJets, ptbins, etabins);
//    TrackerMuonFakeRate_PtEta_Madgraph_JetTriggerSample = rebin(TrackerMuonFakeRate_PtEta_Madgraph_JetTriggerSample, ptbins, etabins);
//    TrackerMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample = rebin(TrackerMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample, ptbins, etabins);
   makePtSliceComparisonPlots(TrackerMuonFakeRate_PtEta_MadgraphWJets, 
                              TrackerMuonFakeRate_PtEta_Madgraph_JetTriggerSample, 
                              TrackerMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample,
                              "TrackerMuonFakeRatePtSlices", 
                              "TrackerMuon Muon Fake Rate", 
                              "WJets", "JetTriggerSample", "PhotonTriggerSample", 
                              "muon #eta", "#epsilon_{fake}",1.0, 0.35,0.75,0.65,0.9);

  //*****************************************************************************************
  //Compare WJets Vs QCD FakeRates For Global 
  //*****************************************************************************************
   TH2F *GlobalMuonFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny(
     "GlobalMuonFakeRate_PtEta_MadgraphWJets");
   assert(GlobalMuonFakeRate_PtEta_MadgraphWJets);

   TH2F *GlobalMuonFakeRate_PtEta_Madgraph_JetTriggerSample = (TH2F*)file->FindObjectAny(
     "GlobalMuonFakeRate_PtEta_Madgraph_Jet50");
   assert(GlobalMuonFakeRate_PtEta_Madgraph_JetTriggerSample);

   TH2F *GlobalMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample = (TH2F*)file->FindObjectAny(
     "GlobalMuonFakeRate_PtEta_Madgraph_Photon15");
   assert(GlobalMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample);

   makeEtaSliceComparisonPlots(GlobalMuonFakeRate_PtEta_MadgraphWJets, 
                               GlobalMuonFakeRate_PtEta_Madgraph_JetTriggerSample, 
                               GlobalMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample, 
                               "GlobalMuonFakeRateEtaSlices",
                               "Global Muon Fake Rate", 
                               "WJets", "JetTriggerSample", "PhotonTriggerSample", 
                               "muon p_{T} [GeV/c]", "#epsilon_{fake}", 1.0, 0.55,0.75,0.95,0.9);


//    GlobalMuonFakeRate_PtEta_MadgraphWJets = rebin(GlobalMuonFakeRate_PtEta_MadgraphWJets, ptbins, etabins);
//    GlobalMuonFakeRate_PtEta_Madgraph_JetTriggerSample = rebin(GlobalMuonFakeRate_PtEta_Madgraph_JetTriggerSample, ptbins, etabins);
//    GlobalMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample = rebin(GlobalMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample, ptbins, etabins);
   makePtSliceComparisonPlots(GlobalMuonFakeRate_PtEta_MadgraphWJets, 
                              GlobalMuonFakeRate_PtEta_Madgraph_JetTriggerSample, 
                              GlobalMuonFakeRate_PtEta_Madgraph_PhotonTriggerSample,
                              "GlobalMuonFakeRatePtSlices", 
                              "Global Muon Fake Rate", 
                              "WJets", "JetTriggerSample", "PhotonTriggerSample", 
                              "muon #eta", "#epsilon_{fake}",1.0, 0.35,0.75,0.65,0.9);

  file->Close();
  return;

}



void print2DHist( TH2F* histogram, string histLegendName, string outputFilename) {

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

//--------------------------------------------------------------------------------------------------
// Make 2D Fake rate slice plots
//--------------------------------------------------------------------------------------------------
void make2DElectronFakeRateTables() {

  vector<double> etabins;
  etabins.push_back(-2.75);
  etabins.push_back(-2.25);
  etabins.push_back(-1.75);
  etabins.push_back(-1.25);
  etabins.push_back(-0.75);
  etabins.push_back(-0.25);
  etabins.push_back(0.25);
  etabins.push_back(0.75);
  etabins.push_back(1.25);
  etabins.push_back(1.75);
  etabins.push_back(2.25);
  etabins.push_back(2.75);
  vector<double> ptbins;
  ptbins.push_back(0);   
  ptbins.push_back(10);  
  ptbins.push_back(15);  
  ptbins.push_back(20);  
  ptbins.push_back(25);  
  ptbins.push_back(30);  
  ptbins.push_back(35);  
  ptbins.push_back(40);
  ptbins.push_back(50);   
  ptbins.push_back(80);  
  ptbins.push_back(120);  
  ptbins.push_back(170);  
  ptbins.push_back(250);


  //Load FakeRate file
  TFile *file = new TFile("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/FakeRates.root", "UPDATE");
  file->cd();
  
  //*****************************************************************************************
  //Make 2D Fake Rate Table for GsfTrack denominator
  //*****************************************************************************************
   TH2F *GsfTrackElectronFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny(
     "GsfTrackSCElectronFakeRate_PtEta_MadgraphWJets");
   assert(GsfTrackElectronFakeRate_PtEta_MadgraphWJets);
   TH2F *tmpGsfTrackElectronFakeRate_PtEta_MadgraphWJets = rebin(GsfTrackElectronFakeRate_PtEta_MadgraphWJets, ptbins, etabins);

   TH2F *GsfTrackElectronFakeRate_PtEta_JetTriggerSample = (TH2F*)file->FindObjectAny(
     "GsfTrackSCElectronFakeRate_PtEta_Madgraph_Jet50");
   assert(GsfTrackElectronFakeRate_PtEta_JetTriggerSample);
   TH2F *tmpGsfTrackElectronFakeRate_PtEta_JetTriggerSample = rebin(GsfTrackElectronFakeRate_PtEta_JetTriggerSample, ptbins, etabins);

   TH2F *GsfTrackElectronFakeRate_PtEta_PhotonTriggerSample = (TH2F*)file->FindObjectAny(
     "GsfTrackSCElectronFakeRate_PtEta_Madgraph_Photon15");
   assert(GsfTrackElectronFakeRate_PtEta_PhotonTriggerSample);
   TH2F *tmpGsfTrackElectronFakeRate_PtEta_PhotonTriggerSample = rebin(GsfTrackElectronFakeRate_PtEta_PhotonTriggerSample, ptbins, etabins);

   print2DHist(tmpGsfTrackElectronFakeRate_PtEta_MadgraphWJets, "GsfTrack Denominator Fake Rate for the W+Jets sample", "ElectronFakeRate_PtEta_GsfTrackDenominator_WJets.tex");
   print2DHist(tmpGsfTrackElectronFakeRate_PtEta_JetTriggerSample, "GsfTrack Denominator Fake Rate for the JetTrigger sample", "ElectronFakeRate_PtEta_GsfTrackDenominator_JetTriggerSample.tex");
   print2DHist(tmpGsfTrackElectronFakeRate_PtEta_PhotonTriggerSample, "GsfTrack Denominator Fake Rate for the PhotonTrigger sample", "ElectronFakeRate_PtEta_GsfTrackDenominator_PhotonTriggerSample.tex");


  //*****************************************************************************************
  //Make 2D Fake Rate Table for Reco denominator
  //*****************************************************************************************
   TH2F *RecoElectronFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny(
     "RecoElectronFakeRate_PtEta_MadgraphWJets");
   assert(RecoElectronFakeRate_PtEta_MadgraphWJets);
   RecoElectronFakeRate_PtEta_MadgraphWJets = rebin(RecoElectronFakeRate_PtEta_MadgraphWJets, ptbins, etabins);

   TH2F *RecoElectronFakeRate_PtEta_JetTriggerSample = (TH2F*)file->FindObjectAny(
     "RecoElectronFakeRate_PtEta_Madgraph_Jet50");
   assert(RecoElectronFakeRate_PtEta_JetTriggerSample);
   RecoElectronFakeRate_PtEta_JetTriggerSample = rebin(RecoElectronFakeRate_PtEta_JetTriggerSample, ptbins, etabins);
   

   TH2F *RecoElectronFakeRate_PtEta_PhotonTriggerSample = (TH2F*)file->FindObjectAny(
     "RecoElectronFakeRate_PtEta_Madgraph_Photon15");
   assert(RecoElectronFakeRate_PtEta_PhotonTriggerSample);
   RecoElectronFakeRate_PtEta_PhotonTriggerSample = rebin(RecoElectronFakeRate_PtEta_PhotonTriggerSample, ptbins, etabins);


   print2DHist(RecoElectronFakeRate_PtEta_MadgraphWJets, "Reco Denominator Fake Rate for the W+Jets sample", "ElectronFakeRate_PtEta_RecoDenominator_WJets.tex");
   print2DHist(RecoElectronFakeRate_PtEta_JetTriggerSample, "Reco Denominator Fake Rate for the JetTrigger sample", "ElectronFakeRate_PtEta_RecoDenominator_JetTriggerSample.tex");
   print2DHist(RecoElectronFakeRate_PtEta_PhotonTriggerSample, "Reco Denominator Fake Rate for the PhotonTrigger sample", "ElectronFakeRate_PtEta_RecoDenominator_PhotonTriggerSample.tex");

  file->Close();
  return;

}

//--------------------------------------------------------------------------------------------------
// Make 2D Fake rate slice plots
//--------------------------------------------------------------------------------------------------
void make2DMuonFakeRateTables() {

  vector<double> etabins;
  etabins.push_back(-2.75);
  etabins.push_back(-2.25);
  etabins.push_back(-1.75);
  etabins.push_back(-1.25);
  etabins.push_back(-0.75);
  etabins.push_back(-0.25);
  etabins.push_back(0.25);
  etabins.push_back(0.75);
  etabins.push_back(1.25);
  etabins.push_back(1.75);
  etabins.push_back(2.25);
  etabins.push_back(2.75);
  vector<double> ptbins;
  ptbins.push_back(0);   
  ptbins.push_back(10);  
  ptbins.push_back(15);  
  ptbins.push_back(20);  
  ptbins.push_back(25);  
  ptbins.push_back(30);  
  ptbins.push_back(35);  
  ptbins.push_back(40);
  ptbins.push_back(50);   
  ptbins.push_back(80);  
  ptbins.push_back(120);  
  ptbins.push_back(170);  
  ptbins.push_back(250);

  //Load FakeRate file
  TFile *file = new TFile("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/FakeRates.root", "UPDATE");
  file->cd();
  
  //*****************************************************************************************
  //Make 2D Fake Rate Table for IsoTrack denominator
  //*****************************************************************************************
   TH2F *IsoTrackMuonFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny(
     "IsoTrackMuonFakeRate_PtEta_MadgraphWJets");
   assert(IsoTrackMuonFakeRate_PtEta_MadgraphWJets);
   IsoTrackMuonFakeRate_PtEta_MadgraphWJets = rebin(IsoTrackMuonFakeRate_PtEta_MadgraphWJets, ptbins, etabins);

   TH2F *IsoTrackMuonFakeRate_PtEta_JetTriggerSample = (TH2F*)file->FindObjectAny(
     "IsoTrackMuonFakeRate_PtEta_Madgraph_Jet50");
   assert(IsoTrackMuonFakeRate_PtEta_JetTriggerSample);
   IsoTrackMuonFakeRate_PtEta_JetTriggerSample = rebin(IsoTrackMuonFakeRate_PtEta_JetTriggerSample, ptbins, etabins);

   TH2F *IsoTrackMuonFakeRate_PtEta_PhotonTriggerSample = (TH2F*)file->FindObjectAny(
     "IsoTrackMuonFakeRate_PtEta_Madgraph_Photon15");
   assert(IsoTrackMuonFakeRate_PtEta_PhotonTriggerSample);
   IsoTrackMuonFakeRate_PtEta_PhotonTriggerSample = rebin(IsoTrackMuonFakeRate_PtEta_PhotonTriggerSample, ptbins, etabins);

   print2DHist(IsoTrackMuonFakeRate_PtEta_MadgraphWJets, "IsoTrack Denominator Fake Rate for the W+Jets sample", "MuonFakeRate_PtEta_IsoTrackDenominator_WJets.tex");
   print2DHist(IsoTrackMuonFakeRate_PtEta_JetTriggerSample, "IsoTrack Denominator Fake Rate for the JetTrigger sample", "MuonFakeRate_PtEta_IsoTrackDenominator_JetTriggerSample.tex");
   print2DHist(IsoTrackMuonFakeRate_PtEta_PhotonTriggerSample, "IsoTrack Denominator Fake Rate for the PhotonTrigger sample", "MuonFakeRate_PtEta_IsoTrackDenominator_PhotonTriggerSample.tex");


  //*****************************************************************************************
  //Make 2D Fake Rate Table for TrackerMuon denominator
  //*****************************************************************************************
   TH2F *TrackerMuonFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny(
     "TrackerMuonFakeRate_PtEta_MadgraphWJets");
   assert(TrackerMuonFakeRate_PtEta_MadgraphWJets);
   TrackerMuonFakeRate_PtEta_MadgraphWJets = rebin(TrackerMuonFakeRate_PtEta_MadgraphWJets, ptbins, etabins);

   TH2F *TrackerMuonFakeRate_PtEta_JetTriggerSample = (TH2F*)file->FindObjectAny(
     "TrackerMuonFakeRate_PtEta_Madgraph_Jet50");
   assert(TrackerMuonFakeRate_PtEta_JetTriggerSample);
   TrackerMuonFakeRate_PtEta_JetTriggerSample = rebin(TrackerMuonFakeRate_PtEta_JetTriggerSample, ptbins, etabins);

   TH2F *TrackerMuonFakeRate_PtEta_PhotonTriggerSample = (TH2F*)file->FindObjectAny(
     "TrackerMuonFakeRate_PtEta_Madgraph_Photon15");
   assert(TrackerMuonFakeRate_PtEta_PhotonTriggerSample);
   TrackerMuonFakeRate_PtEta_PhotonTriggerSample = rebin(TrackerMuonFakeRate_PtEta_PhotonTriggerSample, ptbins, etabins);

   print2DHist(TrackerMuonFakeRate_PtEta_MadgraphWJets, "TrackerMuon Denominator Fake Rate for the W+Jets sample", "MuonFakeRate_PtEta_TrackerMuonDenominator_WJets.tex");
   print2DHist(TrackerMuonFakeRate_PtEta_JetTriggerSample, "TrackerMuon Denominator Fake Rate for the JetTrigger sample", "MuonFakeRate_PtEta_TrackerMuonDenominator_JetTriggerSample.tex");
   print2DHist(TrackerMuonFakeRate_PtEta_PhotonTriggerSample, "TrackerMuon Denominator Fake Rate for the PhotonTrigger sample", "MuonFakeRate_PtEta_TrackerMuonDenominator_PhotonTriggerSample.tex");


  //*****************************************************************************************
  //Make 2D Fake Rate Table for Global denominator
  //*****************************************************************************************
   TH2F *GlobalMuonFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny(
     "GlobalMuonFakeRate_PtEta_MadgraphWJets");
   assert(GlobalMuonFakeRate_PtEta_MadgraphWJets);
   GlobalMuonFakeRate_PtEta_MadgraphWJets = rebin(GlobalMuonFakeRate_PtEta_MadgraphWJets, ptbins, etabins);

   TH2F *GlobalMuonFakeRate_PtEta_JetTriggerSample = (TH2F*)file->FindObjectAny(
     "GlobalMuonFakeRate_PtEta_Madgraph_Jet50");
   assert(GlobalMuonFakeRate_PtEta_JetTriggerSample);
   GlobalMuonFakeRate_PtEta_JetTriggerSample = rebin(GlobalMuonFakeRate_PtEta_JetTriggerSample, ptbins, etabins);

   TH2F *GlobalMuonFakeRate_PtEta_PhotonTriggerSample = (TH2F*)file->FindObjectAny(
     "GlobalMuonFakeRate_PtEta_Madgraph_Photon15");
   assert(GlobalMuonFakeRate_PtEta_PhotonTriggerSample);
   GlobalMuonFakeRate_PtEta_PhotonTriggerSample = rebin(GlobalMuonFakeRate_PtEta_PhotonTriggerSample, ptbins, etabins);

   print2DHist(GlobalMuonFakeRate_PtEta_MadgraphWJets, "Global Denominator Fake Rate for the W+Jets sample", "MuonFakeRate_PtEta_GlobalDenominator_WJets.tex");
   print2DHist(GlobalMuonFakeRate_PtEta_JetTriggerSample, "Global Denominator Fake Rate for the JetTrigger sample", "MuonFakeRate_PtEta_GlobalDenominator_JetTriggerSample.tex");
   print2DHist(GlobalMuonFakeRate_PtEta_PhotonTriggerSample, "Global Denominator Fake Rate for the PhotonTrigger sample", "MuonFakeRate_PtEta_GlobalDenominator_PhotonTriggerSample.tex");

  file->Close();
  return;

}

//--------------------------------------------------------------------------------------------------
// Make 1D Fake rate comparison plots
//--------------------------------------------------------------------------------------------------
void make1DElectronFakeRatePlots() {

  //Load FakeRate file
  TFile *file = new TFile("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/FakeRates.root", "UPDATE");
  file->cd();
  
  string plotname;
  string filename;

  //Make Canvas and Legend
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);
  TLegend *leg1=0;
  leg1 = new TLegend(0.75,0.6,0.9,0.8);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);

  TH1F *electronFakeRate_Pt_WJets = 0;
  TH1F *electronFakeRate_Pt_JetTrigger = 0;
  TH1F *electronFakeRate_Pt_PhotonTrigger = 0;
  TH1F *electronFakeRate_Pt_sysErrors = 0;
  TH1F *electronFakeRate_Eta_WJets = 0;
  TH1F *electronFakeRate_Eta_JetTrigger = 0;
  TH1F *electronFakeRate_Eta_PhotonTrigger = 0;
  TH1F *electronFakeRate_Eta_sysErrors = 0;

  //*****************************************************************************************
  //Compare GsfTrack FakeRates between various samples vs Pt
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.25);  leg1->SetX2NDC(0.60); leg1->SetY1NDC(0.75);  leg1->SetY2NDC(0.85); 
  leg1->SetX1(0.25);  leg1->SetX2(0.60); leg1->SetY1(0.75);  leg1->SetY2(0.85); 
  plotname = "GsfTrackElectronFakeRatePt";

  electronFakeRate_Pt_WJets = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Pt_MadgraphWJets");
  electronFakeRate_Pt_JetTrigger = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Pt_Madgraph_Jet50");
  electronFakeRate_Pt_PhotonTrigger = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Pt_Madgraph_Photon15");

  assert(electronFakeRate_Pt_WJets);
  assert(electronFakeRate_Pt_JetTrigger);
  assert(electronFakeRate_Pt_PhotonTrigger);

  electronFakeRate_Pt_WJets->SetLineColor(COLORS[0]);
  electronFakeRate_Pt_WJets->SetFillColor(COLORS[0]);
  electronFakeRate_Pt_WJets->SetFillStyle(3544);
  electronFakeRate_Pt_WJets->SetMarkerColor(COLORS[0]);
  electronFakeRate_Pt_WJets->SetMarkerStyle(MARKERS[0]);
  electronFakeRate_Pt_JetTrigger->SetLineColor(COLORS[1]);
  electronFakeRate_Pt_JetTrigger->SetMarkerColor(COLORS[1]);
  electronFakeRate_Pt_JetTrigger->SetMarkerStyle(MARKERS[1]);
  electronFakeRate_Pt_PhotonTrigger->SetLineColor(COLORS[2]);
  electronFakeRate_Pt_PhotonTrigger->SetMarkerColor(COLORS[2]);
  electronFakeRate_Pt_PhotonTrigger->SetMarkerStyle(MARKERS[2]);

  electronFakeRate_Pt_WJets->SetMaximum(0.05);
  electronFakeRate_Pt_WJets->SetMinimum(0.0);
  electronFakeRate_Pt_WJets->GetXaxis()->SetRangeUser(0,100);

  electronFakeRate_Pt_WJets->SetTitle("");
  electronFakeRate_Pt_WJets->SetXTitle("fake electron p_{T} [GeV/c]");
  electronFakeRate_Pt_WJets->SetYTitle("#epsilon_{fake}");
  electronFakeRate_Pt_WJets->SetTitleOffset(1.5, "Y");

  leg1->AddEntry(electronFakeRate_Pt_WJets, "WJets", "LP"); 
  leg1->AddEntry(electronFakeRate_Pt_JetTrigger, "jet trigger (QCD)", "LP"); 
  leg1->AddEntry(electronFakeRate_Pt_PhotonTrigger, "photon trigger (Photon+Jets)", "LP"); 

  electronFakeRate_Pt_WJets->Draw("E1");
  electronFakeRate_Pt_JetTrigger->Draw("E1same");
  electronFakeRate_Pt_PhotonTrigger->Draw("E1same");
 
  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);
  leg1->Draw();

  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());

  //*****************************************************************************************
  //Compare Reco FakeRates between various samples vs Pt
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.25);  leg1->SetX2NDC(0.60); leg1->SetY1NDC(0.75);  leg1->SetY2NDC(0.85); 
  leg1->SetX1(0.25);  leg1->SetX2(0.60); leg1->SetY1(0.75);  leg1->SetY2(0.85); 
  plotname = "RecoElectronFakeRatePt";

  electronFakeRate_Pt_WJets = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_MadgraphWJets");
  electronFakeRate_Pt_JetTrigger = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_Madgraph_Jet50");
  electronFakeRate_Pt_PhotonTrigger = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_Madgraph_Photon15");

  assert(electronFakeRate_Pt_WJets);
  assert(electronFakeRate_Pt_JetTrigger);

  electronFakeRate_Pt_WJets->SetLineColor(COLORS[0]);
  electronFakeRate_Pt_WJets->SetMarkerColor(COLORS[0]);
  electronFakeRate_Pt_WJets->SetMarkerStyle(MARKERS[0]);
  electronFakeRate_Pt_JetTrigger->SetLineColor(COLORS[1]);
  electronFakeRate_Pt_JetTrigger->SetMarkerColor(COLORS[1]);
  electronFakeRate_Pt_JetTrigger->SetMarkerStyle(MARKERS[1]);
  electronFakeRate_Pt_PhotonTrigger->SetLineColor(COLORS[2]);
  electronFakeRate_Pt_PhotonTrigger->SetMarkerColor(COLORS[2]);
  electronFakeRate_Pt_PhotonTrigger->SetMarkerStyle(MARKERS[2]);

  electronFakeRate_Pt_WJets->SetMaximum(0.16);
  electronFakeRate_Pt_WJets->SetMinimum(0.0);
  electronFakeRate_Pt_WJets->GetXaxis()->SetRangeUser(0,100);

  electronFakeRate_Pt_WJets->SetTitle("");
  electronFakeRate_Pt_WJets->SetXTitle("fake electron p_{T} [GeV/c]");
  electronFakeRate_Pt_WJets->SetYTitle("#epsilon_{fake}");
  electronFakeRate_Pt_WJets->SetTitleOffset(1.5, "Y");

  leg1->AddEntry(electronFakeRate_Pt_WJets, "WJets", "LP"); 
  leg1->AddEntry(electronFakeRate_Pt_JetTrigger, "jet trigger (QCD)", "LP"); 
  leg1->AddEntry(electronFakeRate_Pt_PhotonTrigger, "photon trigger (Photon+Jets)", "LP"); 

  electronFakeRate_Pt_WJets->Draw("E1");
  electronFakeRate_Pt_JetTrigger->Draw("E1same");
  electronFakeRate_Pt_PhotonTrigger->Draw("E1same");
  
  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);
  leg1->Draw();

  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());


  //*****************************************************************************************
  //Compare GsfTrack FakeRates between various samples vs Eta
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.25);  leg1->SetX2NDC(0.60); leg1->SetY1NDC(0.75);  leg1->SetY2NDC(0.85); 
  leg1->SetX1(0.25);  leg1->SetX2(0.60); leg1->SetY1(0.75);  leg1->SetY2(0.85); 
  plotname = "GsfTrackElectronFakeRateEta";

  electronFakeRate_Eta_WJets = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Eta_MadgraphWJets");
  electronFakeRate_Eta_JetTrigger = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Eta_Madgraph_Jet50");
  electronFakeRate_Eta_PhotonTrigger = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Eta_Madgraph_Photon15");

  assert(electronFakeRate_Eta_WJets);
  assert(electronFakeRate_Eta_JetTrigger);

  electronFakeRate_Eta_WJets->SetLineColor(COLORS[0]);
  electronFakeRate_Eta_WJets->SetMarkerColor(COLORS[0]);
  electronFakeRate_Eta_WJets->SetMarkerStyle(MARKERS[0]);
  electronFakeRate_Eta_JetTrigger->SetLineColor(COLORS[1]);
  electronFakeRate_Eta_JetTrigger->SetMarkerColor(COLORS[1]);
  electronFakeRate_Eta_JetTrigger->SetMarkerStyle(MARKERS[1]);
  electronFakeRate_Eta_PhotonTrigger->SetLineColor(COLORS[2]);
  electronFakeRate_Eta_PhotonTrigger->SetMarkerColor(COLORS[2]);
  electronFakeRate_Eta_PhotonTrigger->SetMarkerStyle(MARKERS[2]);

  electronFakeRate_Eta_WJets->SetMaximum(0.02);
  electronFakeRate_Eta_WJets->SetMinimum(0.0);

  electronFakeRate_Eta_WJets->SetTitle("");
  electronFakeRate_Eta_WJets->SetXTitle("fake electron #eta");
  electronFakeRate_Eta_WJets->SetYTitle("#epsilon_{fake}");
  electronFakeRate_Eta_WJets->SetTitleOffset(1.5, "Y");

  leg1->AddEntry(electronFakeRate_Eta_WJets, "WJets", "LP"); 
  leg1->AddEntry(electronFakeRate_Eta_JetTrigger, "jet trigger (QCD)", "LP"); 
  leg1->AddEntry(electronFakeRate_Eta_PhotonTrigger, "photon trigger (Photon+Jets)", "LP"); 

  electronFakeRate_Eta_WJets->Draw("E1");
  electronFakeRate_Eta_JetTrigger->Draw("E1same");
  electronFakeRate_Eta_PhotonTrigger->Draw("E1same");

  
  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);
  leg1->Draw();

  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());


  //*****************************************************************************************
  //Compare Reco FakeRates between various samples vs Eta
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.25);  leg1->SetX2NDC(0.60); leg1->SetY1NDC(0.75);  leg1->SetY2NDC(0.85); 
  leg1->SetX1(0.25);  leg1->SetX2(0.60); leg1->SetY1(0.75);  leg1->SetY2(0.85); 
  plotname = "RecoElectronFakeRateEta";

  electronFakeRate_Eta_WJets = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_MadgraphWJets");
  electronFakeRate_Eta_JetTrigger = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_Madgraph_Jet50");
  electronFakeRate_Eta_PhotonTrigger = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_Madgraph_Photon15");

  assert(electronFakeRate_Eta_WJets);
  assert(electronFakeRate_Eta_JetTrigger);

  electronFakeRate_Eta_WJets->SetLineColor(COLORS[0]);
  electronFakeRate_Eta_WJets->SetMarkerColor(COLORS[0]);
  electronFakeRate_Eta_WJets->SetMarkerStyle(MARKERS[0]);
  electronFakeRate_Eta_JetTrigger->SetLineColor(COLORS[1]);
  electronFakeRate_Eta_JetTrigger->SetMarkerColor(COLORS[1]);
  electronFakeRate_Eta_JetTrigger->SetMarkerStyle(MARKERS[1]);
  electronFakeRate_Eta_PhotonTrigger->SetLineColor(COLORS[2]);
  electronFakeRate_Eta_PhotonTrigger->SetMarkerColor(COLORS[2]);
  electronFakeRate_Eta_PhotonTrigger->SetMarkerStyle(MARKERS[2]);

  electronFakeRate_Eta_WJets->SetMaximum(0.07);
  electronFakeRate_Eta_WJets->SetMinimum(0.0);

  electronFakeRate_Eta_WJets->SetTitle("");
  electronFakeRate_Eta_WJets->SetXTitle("fake electron #eta");
  electronFakeRate_Eta_WJets->SetYTitle("#epsilon_{fake}");
  electronFakeRate_Eta_WJets->SetTitleOffset(1.5, "Y");

  leg1->AddEntry(electronFakeRate_Eta_WJets, "WJets", "LP"); 
  leg1->AddEntry(electronFakeRate_Eta_JetTrigger, "jet trigger (QCD)", "LP"); 
  leg1->AddEntry(electronFakeRate_Eta_PhotonTrigger, "photon trigger (Photon+Jets)", "LP"); 

  electronFakeRate_Eta_WJets->Draw("E1");
  electronFakeRate_Eta_JetTrigger->Draw("E1same");
  electronFakeRate_Eta_PhotonTrigger->Draw("E1same");

  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);
  leg1->Draw();

  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());

  //*****************************************************************************************
  //Reco FakeRates vs Pt with systematics 
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.55);  leg1->SetX2NDC(0.90); leg1->SetY1NDC(0.75);  leg1->SetY2NDC(0.85); 
  leg1->SetX1(0.55);  leg1->SetX2(0.90); leg1->SetY1(0.75);  leg1->SetY2(0.85); 
  plotname = "RecoElectronFakeRatePt_WithSystematics";

  electronFakeRate_Pt_WJets = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_MadgraphWJets");
  electronFakeRate_Pt_JetTrigger = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_Madgraph_Jet50");
  electronFakeRate_Pt_PhotonTrigger = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_Madgraph_Photon15");
  electronFakeRate_Pt_sysErrors = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_Madgraph_SysErrors");

  assert(electronFakeRate_Pt_WJets);
  assert(electronFakeRate_Pt_JetTrigger);
  assert(electronFakeRate_Pt_PhotonTrigger);
  assert(electronFakeRate_Pt_sysErrors);

  //re-calculate errors for the JetTrigger fake rate adding systematics + statistical in quadrature
  for (int bin = 0; bin < electronFakeRate_Pt_JetTrigger->GetNbinsX()+1; ++bin) {
    Double_t totalError = TMath::Sqrt(electronFakeRate_Pt_JetTrigger->GetBinError(bin)*electronFakeRate_Pt_JetTrigger->GetBinError(bin) +
                                      electronFakeRate_Pt_sysErrors->GetBinContent(bin)*electronFakeRate_Pt_sysErrors->GetBinContent(bin));
    cout << "bin " << bin << " : " << totalError << ", " << electronFakeRate_Pt_JetTrigger->GetBinError(bin) << ", " << electronFakeRate_Pt_sysErrors->GetBinContent(bin) << endl;
    if (TMath::IsNaN(electronFakeRate_Pt_sysErrors->GetBinContent(bin))) cout << "NAN!!\n";
    electronFakeRate_Pt_JetTrigger->SetBinError(bin, totalError);
  }

  electronFakeRate_Pt_JetTrigger->SetLineColor(COLORS[1]);
  electronFakeRate_Pt_JetTrigger->SetFillColor(COLORS[1]);
  electronFakeRate_Pt_JetTrigger->SetFillStyle(3544);
  electronFakeRate_Pt_JetTrigger->SetMarkerColor(COLORS[1]);
  electronFakeRate_Pt_JetTrigger->SetMarkerStyle(MARKERS[1]);
  electronFakeRate_Pt_WJets->SetLineColor(COLORS[0]);
  electronFakeRate_Pt_WJets->SetMarkerColor(COLORS[0]);
  electronFakeRate_Pt_WJets->SetMarkerStyle(MARKERS[0]);
  electronFakeRate_Pt_PhotonTrigger->SetLineColor(COLORS[2]);
  electronFakeRate_Pt_PhotonTrigger->SetMarkerColor(COLORS[2]);
  electronFakeRate_Pt_PhotonTrigger->SetMarkerStyle(MARKERS[2]);

  electronFakeRate_Pt_JetTrigger->SetMaximum(0.17);
  electronFakeRate_Pt_JetTrigger->SetMinimum(0.0);
  electronFakeRate_Pt_JetTrigger->GetXaxis()->SetRangeUser(0,100);

  electronFakeRate_Pt_JetTrigger->SetTitle("");
  electronFakeRate_Pt_JetTrigger->SetXTitle("fake electron p_{T} [GeV/c]");
  electronFakeRate_Pt_JetTrigger->SetYTitle("#epsilon_{fake}");
  electronFakeRate_Pt_JetTrigger->SetTitleOffset(1.5, "Y");

  leg1->AddEntry(electronFakeRate_Pt_WJets, "WJets", "LP"); 
  leg1->AddEntry(electronFakeRate_Pt_JetTrigger, "jet trigger (QCD)", "LP"); 
  leg1->AddEntry(electronFakeRate_Pt_PhotonTrigger, "photon trigger (Photon+Jets)", "LP"); 

  electronFakeRate_Pt_JetTrigger->Draw("E5");
  electronFakeRate_Pt_WJets->Draw("same");
  electronFakeRate_Pt_PhotonTrigger->Draw("same");
  
  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);
  leg1->Draw();

  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());

  //*****************************************************************************************
  //Reco FakeRates vs Eta with systematics
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.55);  leg1->SetX2NDC(0.90); leg1->SetY1NDC(0.75);  leg1->SetY2NDC(0.85); 
  leg1->SetX1(0.55);  leg1->SetX2(0.90); leg1->SetY1(0.75);  leg1->SetY2(0.85); 
  plotname = "RecoElectronFakeRateEta_WithSystematics";

  electronFakeRate_Eta_WJets = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_MadgraphWJets");
  electronFakeRate_Eta_JetTrigger = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_Madgraph_Jet50");
  electronFakeRate_Eta_PhotonTrigger = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_Madgraph_Photon15");
  electronFakeRate_Eta_sysErrors = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_Madgraph_SysErrors");

  assert(electronFakeRate_Eta_WJets);
  assert(electronFakeRate_Eta_JetTrigger);
  assert(electronFakeRate_Eta_PhotonTrigger);
  assert(electronFakeRate_Eta_sysErrors);

  //re-calculate errors for the JetTrigger fake rate adding systematics + statistical in quadrature
  for (int bin = 0; bin < electronFakeRate_Eta_JetTrigger->GetNbinsX()+1; ++bin) {
    Double_t totalError = TMath::Sqrt(electronFakeRate_Eta_JetTrigger->GetBinError(bin)*electronFakeRate_Eta_JetTrigger->GetBinError(bin) +
                                      electronFakeRate_Eta_sysErrors->GetBinContent(bin)*electronFakeRate_Eta_sysErrors->GetBinContent(bin));
    cout << "bin " << bin << " : " << totalError << ", " << electronFakeRate_Eta_JetTrigger->GetBinError(bin) << ", " << electronFakeRate_Eta_sysErrors->GetBinContent(bin) << endl;
    if (TMath::IsNaN(electronFakeRate_Eta_sysErrors->GetBinContent(bin))) cout << "NAN!!\n";
    electronFakeRate_Eta_JetTrigger->SetBinError(bin, totalError);
  }

  electronFakeRate_Eta_JetTrigger->SetLineColor(COLORS[1]);
  electronFakeRate_Eta_JetTrigger->SetFillColor(COLORS[1]);
  electronFakeRate_Eta_JetTrigger->SetFillStyle(3544);
  electronFakeRate_Eta_JetTrigger->SetMarkerColor(COLORS[1]);
  electronFakeRate_Eta_JetTrigger->SetMarkerStyle(MARKERS[1]);
  electronFakeRate_Eta_WJets->SetLineColor(COLORS[0]);
  electronFakeRate_Eta_WJets->SetMarkerColor(COLORS[0]);
  electronFakeRate_Eta_WJets->SetMarkerStyle(MARKERS[0]);
  electronFakeRate_Eta_PhotonTrigger->SetLineColor(COLORS[2]);
  electronFakeRate_Eta_PhotonTrigger->SetMarkerColor(COLORS[2]);
  electronFakeRate_Eta_PhotonTrigger->SetMarkerStyle(MARKERS[2]);

  electronFakeRate_Eta_JetTrigger->SetMaximum(0.06);
  electronFakeRate_Eta_JetTrigger->SetMinimum(0.0);

  electronFakeRate_Eta_JetTrigger->SetTitle("");
  electronFakeRate_Eta_JetTrigger->SetXTitle("fake electron #eta");
  electronFakeRate_Eta_JetTrigger->SetYTitle("#epsilon_{fake}");
  electronFakeRate_Eta_JetTrigger->SetTitleOffset(1.5, "Y");

  leg1->AddEntry(electronFakeRate_Eta_WJets, "WJets", "LP"); 
  leg1->AddEntry(electronFakeRate_Eta_JetTrigger, "jet trigger (QCD)", "LP"); 
  leg1->AddEntry(electronFakeRate_Eta_PhotonTrigger, "photon trigger (Photon+Jets)", "LP"); 

  electronFakeRate_Eta_JetTrigger->Draw("E5");
  electronFakeRate_Eta_WJets->Draw("same");
  electronFakeRate_Eta_PhotonTrigger->Draw("same");

  
  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);
  leg1->Draw();

  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());


  file->Close();
  return;

}


//--------------------------------------------------------------------------------------------------
// Make 1D Fake rate comparison plots
//--------------------------------------------------------------------------------------------------
void make1DMuonFakeRatePlots() {

  //Load FakeRate file
  TFile *file = new TFile("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/FakeRates.root", "UPDATE");
  file->cd();
  
  string plotname;
  string filename;

  //Make Canvas and Legend
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);
  TLegend *leg1=0;
  leg1 = new TLegend(0.75,0.6,0.9,0.8);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);

  TH1F *muonFakeRate_Pt_WJets = 0;
  TH1F *muonFakeRate_Pt_JetTrigger = 0;
  TH1F *muonFakeRate_Pt_PhotonTrigger = 0;
  TH1F *muonFakeRate_Pt_sysErrors = 0;
  TH1F *muonFakeRate_Eta_WJets = 0;
  TH1F *muonFakeRate_Eta_JetTrigger = 0;
  TH1F *muonFakeRate_Eta_PhotonTrigger = 0;
  TH1F *muonFakeRate_Eta_sysErrors = 0;


  //*****************************************************************************************
  //Compare IsoTrack FakeRates between various samples vs Pt
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.25);  leg1->SetX2NDC(0.60); leg1->SetY1NDC(0.75);  leg1->SetY2NDC(0.85); 
  leg1->SetX1(0.25);  leg1->SetX2(0.60); leg1->SetY1(0.75);  leg1->SetY2(0.85); 
  plotname = "IsoTrackMuonFakeRatePt";

  muonFakeRate_Pt_WJets = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Pt_MadgraphWJets");
  muonFakeRate_Pt_JetTrigger = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Pt_Madgraph_Jet50");
  muonFakeRate_Pt_PhotonTrigger = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Pt_Madgraph_Photon15");

  assert(muonFakeRate_Pt_WJets);
  assert(muonFakeRate_Pt_JetTrigger);
  assert(muonFakeRate_Pt_PhotonTrigger);

  muonFakeRate_Pt_WJets->SetLineColor(COLORS[0]);
  muonFakeRate_Pt_WJets->SetMarkerColor(COLORS[0]);
  muonFakeRate_Pt_WJets->SetMarkerStyle(MARKERS[0]);
  muonFakeRate_Pt_JetTrigger->SetLineColor(COLORS[1]);
  muonFakeRate_Pt_JetTrigger->SetMarkerColor(COLORS[1]);
  muonFakeRate_Pt_JetTrigger->SetMarkerStyle(MARKERS[1]);
  muonFakeRate_Pt_PhotonTrigger->SetLineColor(COLORS[2]);
  muonFakeRate_Pt_PhotonTrigger->SetMarkerColor(COLORS[2]);
  muonFakeRate_Pt_PhotonTrigger->SetMarkerStyle(MARKERS[2]);

  muonFakeRate_Pt_WJets->SetMaximum(0.002);
  muonFakeRate_Pt_WJets->SetMinimum(0.0);
  muonFakeRate_Pt_WJets->GetXaxis()->SetRangeUser(0,100);

  muonFakeRate_Pt_WJets->SetTitle("");
  muonFakeRate_Pt_WJets->SetXTitle("fake muon p_{T} [GeV/c]");
  muonFakeRate_Pt_WJets->SetYTitle("#epsilon_{fake}");
  muonFakeRate_Pt_WJets->SetTitleOffset(1.75, "Y");

  leg1->AddEntry(muonFakeRate_Pt_WJets, "WJets", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_JetTrigger, "jet trigger (QCD)", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_PhotonTrigger, "photon trigger (Photon+Jets)", "LP"); 


  muonFakeRate_Pt_WJets->Draw("E1");
  muonFakeRate_Pt_JetTrigger->Draw("E1same");
  muonFakeRate_Pt_PhotonTrigger->Draw("E1same");
 
  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);
  leg1->Draw();

  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());


  //*****************************************************************************************
  //Compare TrackerMuon FakeRates between various samples vs Pt
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.25);  leg1->SetX2NDC(0.60); leg1->SetY1NDC(0.75);  leg1->SetY2NDC(0.85); 
  leg1->SetX1(0.25);  leg1->SetX2(0.60); leg1->SetY1(0.75);  leg1->SetY2(0.85); 
  plotname = "TrackerMuonFakeRatePt";

  muonFakeRate_Pt_WJets = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_MadgraphWJets");
  muonFakeRate_Pt_JetTrigger = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_Madgraph_Jet50");
  muonFakeRate_Pt_PhotonTrigger = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_Madgraph_Photon15");

  assert(muonFakeRate_Pt_WJets);
  assert(muonFakeRate_Pt_JetTrigger);

  muonFakeRate_Pt_WJets->SetLineColor(COLORS[0]);
  muonFakeRate_Pt_WJets->SetMarkerColor(COLORS[0]);
  muonFakeRate_Pt_WJets->SetMarkerStyle(MARKERS[0]);
  muonFakeRate_Pt_JetTrigger->SetLineColor(COLORS[1]);
  muonFakeRate_Pt_JetTrigger->SetMarkerColor(COLORS[1]);
  muonFakeRate_Pt_JetTrigger->SetMarkerStyle(MARKERS[1]);
  muonFakeRate_Pt_PhotonTrigger->SetLineColor(COLORS[2]);
  muonFakeRate_Pt_PhotonTrigger->SetMarkerColor(COLORS[2]);
  muonFakeRate_Pt_PhotonTrigger->SetMarkerStyle(MARKERS[2]);

  muonFakeRate_Pt_WJets->SetMaximum(0.25);
  muonFakeRate_Pt_WJets->SetMinimum(0.0);
  muonFakeRate_Pt_WJets->GetXaxis()->SetRangeUser(0,100);

  muonFakeRate_Pt_WJets->SetTitle("");
  muonFakeRate_Pt_WJets->SetXTitle("fake muon p_{T} [GeV]");
  muonFakeRate_Pt_WJets->SetYTitle("#epsilon_{fake}");
  muonFakeRate_Pt_WJets->SetTitleOffset(1.5, "Y");

  leg1->AddEntry(muonFakeRate_Pt_WJets, "WJets", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_JetTrigger, "jet trigger (QCD)", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_PhotonTrigger, "photon trigger (Photon+Jets)", "LP"); 

  muonFakeRate_Pt_WJets->Draw("E1");  
  muonFakeRate_Pt_JetTrigger->Draw("E1same");
  muonFakeRate_Pt_PhotonTrigger->Draw("E1same");

  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);
  leg1->Draw();

  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());


  //*****************************************************************************************
  //Compare Global FakeRates between various samples vs Pt
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.25);  leg1->SetX2NDC(0.60); leg1->SetY1NDC(0.75);  leg1->SetY2NDC(0.85); 
  leg1->SetX1(0.25);  leg1->SetX2(0.60); leg1->SetY1(0.75);  leg1->SetY2(0.85); 
  plotname = "GlobalMuonFakeRatePt";

  muonFakeRate_Pt_WJets = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Pt_MadgraphWJets");
  muonFakeRate_Pt_JetTrigger = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Pt_Madgraph_Jet50");
  muonFakeRate_Pt_PhotonTrigger = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Pt_Madgraph_Photon15");

  assert(muonFakeRate_Pt_WJets);
  assert(muonFakeRate_Pt_JetTrigger);

  muonFakeRate_Pt_WJets->SetLineColor(COLORS[0]);
  muonFakeRate_Pt_WJets->SetMarkerColor(COLORS[0]);
  muonFakeRate_Pt_WJets->SetMarkerStyle(MARKERS[0]);
  muonFakeRate_Pt_JetTrigger->SetLineColor(COLORS[1]);
  muonFakeRate_Pt_JetTrigger->SetMarkerColor(COLORS[1]);
  muonFakeRate_Pt_JetTrigger->SetMarkerStyle(MARKERS[1]);
  muonFakeRate_Pt_PhotonTrigger->SetLineColor(COLORS[2]);
  muonFakeRate_Pt_PhotonTrigger->SetMarkerColor(COLORS[2]);
  muonFakeRate_Pt_PhotonTrigger->SetMarkerStyle(MARKERS[2]);

  muonFakeRate_Pt_WJets->SetMaximum(0.40);
  muonFakeRate_Pt_WJets->SetMinimum(0.0);
  muonFakeRate_Pt_WJets->GetXaxis()->SetRangeUser(0,100);

  muonFakeRate_Pt_WJets->SetTitle("");
  muonFakeRate_Pt_WJets->SetXTitle("fake muon p_{T} [GeV/c]");
  muonFakeRate_Pt_WJets->SetYTitle("#epsilon_{fake}");
  muonFakeRate_Pt_WJets->SetTitleOffset(1.5, "Y");

  leg1->AddEntry(muonFakeRate_Pt_WJets, "WJets", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_JetTrigger, "jet trigger (QCD)", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_PhotonTrigger, "photon trigger (Photon+Jets)", "LP"); 

  muonFakeRate_Pt_WJets->Draw("E1");
  muonFakeRate_Pt_JetTrigger->Draw("E1same");
  muonFakeRate_Pt_PhotonTrigger->Draw("E1same");
  
  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);
  leg1->Draw();

  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());


  //*****************************************************************************************
  //Compare IsoTrack FakeRates between various samples vs Eta
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.30);  leg1->SetX2NDC(0.65); leg1->SetY1NDC(0.75);  leg1->SetY2NDC(0.85); 
  leg1->SetX1(0.30);  leg1->SetX2(0.65); leg1->SetY1(0.75);  leg1->SetY2(0.85); 
  plotname = "IsoTrackMuonFakeRateEta";

  muonFakeRate_Eta_WJets = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Eta_MadgraphWJets");
  muonFakeRate_Eta_JetTrigger = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Eta_Madgraph_Jet50");
  muonFakeRate_Eta_PhotonTrigger = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Eta_Madgraph_Photon15");

  assert(muonFakeRate_Eta_WJets);
  assert(muonFakeRate_Eta_JetTrigger);

  muonFakeRate_Eta_WJets->SetLineColor(COLORS[0]);
  muonFakeRate_Eta_WJets->SetMarkerColor(COLORS[0]);
  muonFakeRate_Eta_WJets->SetMarkerStyle(MARKERS[0]);
  muonFakeRate_Eta_JetTrigger->SetLineColor(COLORS[1]);
  muonFakeRate_Eta_JetTrigger->SetMarkerColor(COLORS[1]);
  muonFakeRate_Eta_JetTrigger->SetMarkerStyle(MARKERS[1]);
  muonFakeRate_Eta_PhotonTrigger->SetLineColor(COLORS[2]);
  muonFakeRate_Eta_PhotonTrigger->SetMarkerColor(COLORS[2]);
  muonFakeRate_Eta_PhotonTrigger->SetMarkerStyle(MARKERS[2]);

  muonFakeRate_Eta_WJets->SetMaximum(0.0012);
  muonFakeRate_Eta_WJets->SetMinimum(0.0);

  muonFakeRate_Eta_WJets->SetTitle("");
  muonFakeRate_Eta_WJets->SetXTitle("fake muon #eta");
  muonFakeRate_Eta_WJets->SetYTitle("#epsilon_{fake}");
  muonFakeRate_Eta_WJets->SetTitleOffset(1.75, "Y");

  leg1->AddEntry(muonFakeRate_Eta_WJets, "WJets", "LP"); 
  leg1->AddEntry(muonFakeRate_Eta_JetTrigger, "jet trigger (QCD)", "LP"); 
  leg1->AddEntry(muonFakeRate_Eta_PhotonTrigger, "photon trigger (Photon+Jets)", "LP"); 

  muonFakeRate_Eta_WJets->Draw("E1");
  muonFakeRate_Eta_JetTrigger->Draw("E1same");
  muonFakeRate_Eta_PhotonTrigger->Draw("E1same");

  
  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);
  leg1->Draw();

  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());


  //*****************************************************************************************
  //Compare TrackerMuon FakeRates between various samples vs Eta
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.30);  leg1->SetX2NDC(0.65); leg1->SetY1NDC(0.75);  leg1->SetY2NDC(0.85); 
  leg1->SetX1(0.30);  leg1->SetX2(0.65); leg1->SetY1(0.75);  leg1->SetY2(0.85); 
  plotname = "TrackerMuonFakeRateEta";

  muonFakeRate_Eta_WJets = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Eta_MadgraphWJets");
  muonFakeRate_Eta_JetTrigger = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Eta_Madgraph_Jet50");
  muonFakeRate_Eta_PhotonTrigger = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Eta_Madgraph_Photon15");

  assert(muonFakeRate_Eta_WJets);
  assert(muonFakeRate_Eta_JetTrigger);

  muonFakeRate_Eta_WJets->SetLineColor(COLORS[0]);
  muonFakeRate_Eta_WJets->SetMarkerColor(COLORS[0]);
  muonFakeRate_Eta_WJets->SetMarkerStyle(MARKERS[0]);
  muonFakeRate_Eta_JetTrigger->SetLineColor(COLORS[1]);
  muonFakeRate_Eta_JetTrigger->SetMarkerColor(COLORS[1]);
  muonFakeRate_Eta_JetTrigger->SetMarkerStyle(MARKERS[1]);
  muonFakeRate_Eta_PhotonTrigger->SetLineColor(COLORS[2]);
  muonFakeRate_Eta_PhotonTrigger->SetMarkerColor(COLORS[2]);
  muonFakeRate_Eta_PhotonTrigger->SetMarkerStyle(MARKERS[2]);

  muonFakeRate_Eta_WJets->SetMaximum(0.14);
  muonFakeRate_Eta_WJets->SetMinimum(0.0);

  muonFakeRate_Eta_WJets->SetTitle("");
  muonFakeRate_Eta_WJets->SetXTitle("fake muon #eta");
  muonFakeRate_Eta_WJets->SetYTitle("#epsilon_{fake}");
  muonFakeRate_Eta_WJets->SetTitleOffset(1.5, "Y");

  leg1->AddEntry(muonFakeRate_Eta_WJets, "WJets", "LP"); 
  leg1->AddEntry(muonFakeRate_Eta_JetTrigger, "jet trigger (QCD)", "LP"); 
  leg1->AddEntry(muonFakeRate_Eta_PhotonTrigger, "photon trigger (Photon+Jets)", "LP"); 

  muonFakeRate_Eta_WJets->Draw("E1");
  muonFakeRate_Eta_JetTrigger->Draw("E1same");
  muonFakeRate_Eta_PhotonTrigger->Draw("E1same");

  
  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);
  leg1->Draw();

  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());



  //*****************************************************************************************
  //Compare Global FakeRates between various samples vs Eta
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.30);  leg1->SetX2NDC(0.65); leg1->SetY1NDC(0.75);  leg1->SetY2NDC(0.85); 
  leg1->SetX1(0.30);  leg1->SetX2(0.65); leg1->SetY1(0.75);  leg1->SetY2(0.85); 
  plotname = "GlobalMuonFakeRateEta";

  muonFakeRate_Eta_WJets = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Eta_MadgraphWJets");
  muonFakeRate_Eta_JetTrigger = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Eta_Madgraph_Jet50");
  muonFakeRate_Eta_PhotonTrigger = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Eta_Madgraph_Photon15");

  assert(muonFakeRate_Eta_WJets);
  assert(muonFakeRate_Eta_JetTrigger);

  muonFakeRate_Eta_WJets->SetLineColor(COLORS[0]);
  muonFakeRate_Eta_WJets->SetMarkerColor(COLORS[0]);
  muonFakeRate_Eta_WJets->SetMarkerStyle(MARKERS[0]);
  muonFakeRate_Eta_JetTrigger->SetLineColor(COLORS[1]);
  muonFakeRate_Eta_JetTrigger->SetMarkerColor(COLORS[1]);
  muonFakeRate_Eta_JetTrigger->SetMarkerStyle(MARKERS[1]);
  muonFakeRate_Eta_PhotonTrigger->SetLineColor(COLORS[2]);
  muonFakeRate_Eta_PhotonTrigger->SetMarkerColor(COLORS[2]);
  muonFakeRate_Eta_PhotonTrigger->SetMarkerStyle(MARKERS[2]);

  muonFakeRate_Eta_WJets->SetMaximum(0.16);
  muonFakeRate_Eta_WJets->SetMinimum(0.0);

  muonFakeRate_Eta_WJets->SetTitle("");
  muonFakeRate_Eta_WJets->SetXTitle("fake muon #eta");
  muonFakeRate_Eta_WJets->SetYTitle("#epsilon_{fake}");
  muonFakeRate_Eta_WJets->SetTitleOffset(1.5, "Y");

  leg1->AddEntry(muonFakeRate_Eta_WJets, "WJets", "LP"); 
  leg1->AddEntry(muonFakeRate_Eta_JetTrigger, "jet trigger (QCD)", "LP"); 
  leg1->AddEntry(muonFakeRate_Eta_PhotonTrigger, "photon trigger (Photon+Jets)", "LP"); 

  muonFakeRate_Eta_WJets->Draw("E1");
  muonFakeRate_Eta_JetTrigger->Draw("E1same");
  muonFakeRate_Eta_PhotonTrigger->Draw("E1same");

  
  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);
  leg1->Draw();

  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());

  //*****************************************************************************************
  //TrackerMuon FakeRates vs Pt with Systematics
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.25);  leg1->SetX2NDC(0.60); leg1->SetY1NDC(0.75);  leg1->SetY2NDC(0.85); 
  leg1->SetX1(0.25);  leg1->SetX2(0.60); leg1->SetY1(0.75);  leg1->SetY2(0.85); 
  plotname = "TrackerMuonFakeRatePt_WithSystematics";

  muonFakeRate_Pt_WJets = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_MadgraphWJets");
  muonFakeRate_Pt_JetTrigger = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_Madgraph_Jet50");
  muonFakeRate_Pt_PhotonTrigger = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_Madgraph_Photon15");
  muonFakeRate_Pt_sysErrors = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_Madgraph_SysErrors");

  assert(muonFakeRate_Pt_WJets);
  assert(muonFakeRate_Pt_JetTrigger);
  assert(muonFakeRate_Pt_PhotonTrigger);
  assert(muonFakeRate_Pt_sysErrors);

  //re-calculate errors for the JetTrigger fake rate adding systematics + statistical in quadrature
  for (int bin = 0; bin < muonFakeRate_Pt_JetTrigger->GetNbinsX()+1; ++bin) {
    Double_t totalError = TMath::Sqrt(muonFakeRate_Pt_JetTrigger->GetBinError(bin)*muonFakeRate_Pt_JetTrigger->GetBinError(bin) +
                                      muonFakeRate_Pt_sysErrors->GetBinContent(bin)*muonFakeRate_Pt_sysErrors->GetBinContent(bin));
    cout << "bin " << bin << " : " << totalError << ", " << muonFakeRate_Pt_JetTrigger->GetBinError(bin) << ", " << muonFakeRate_Pt_sysErrors->GetBinContent(bin) << endl;
    muonFakeRate_Pt_JetTrigger->SetBinError(bin, totalError);
  }

  muonFakeRate_Pt_JetTrigger->SetLineColor(COLORS[1]);
  muonFakeRate_Pt_JetTrigger->SetFillColor(COLORS[1]);
  muonFakeRate_Pt_JetTrigger->SetFillStyle(3544);
  muonFakeRate_Pt_JetTrigger->SetMarkerColor(COLORS[1]);
  muonFakeRate_Pt_JetTrigger->SetMarkerStyle(MARKERS[1]);
  muonFakeRate_Pt_WJets->SetLineColor(COLORS[0]);
  muonFakeRate_Pt_WJets->SetMarkerColor(COLORS[0]);
  muonFakeRate_Pt_WJets->SetMarkerStyle(MARKERS[0]);
  muonFakeRate_Pt_PhotonTrigger->SetLineColor(COLORS[2]);
  muonFakeRate_Pt_PhotonTrigger->SetMarkerColor(COLORS[2]);
  muonFakeRate_Pt_PhotonTrigger->SetMarkerStyle(MARKERS[2]);

  muonFakeRate_Pt_JetTrigger->SetMaximum(0.25);
  muonFakeRate_Pt_JetTrigger->SetMinimum(0.0);
  muonFakeRate_Pt_JetTrigger->GetXaxis()->SetRangeUser(0,100);

  muonFakeRate_Pt_JetTrigger->SetTitle("");
  muonFakeRate_Pt_JetTrigger->SetXTitle("fake muon p_{T} [GeV]");
  muonFakeRate_Pt_JetTrigger->SetYTitle("#epsilon_{fake}");
  muonFakeRate_Pt_JetTrigger->SetTitleOffset(1.5, "Y");

  leg1->AddEntry(muonFakeRate_Pt_WJets, "WJets", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_JetTrigger, "jet trigger (QCD)", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_PhotonTrigger, "photon trigger (Photon+Jets)", "LP"); 

  muonFakeRate_Pt_JetTrigger->Draw("E5");
  muonFakeRate_Pt_WJets->Draw("same");
  muonFakeRate_Pt_PhotonTrigger->Draw("same");
  
  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);
  leg1->Draw();

  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());

  //*****************************************************************************************
  //TrackerMuon FakeRates vs Eta with Systematics
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.25);  leg1->SetX2NDC(0.60); leg1->SetY1NDC(0.75);  leg1->SetY2NDC(0.85); 
  leg1->SetX1(0.25);  leg1->SetX2(0.60); leg1->SetY1(0.75);  leg1->SetY2(0.85); 
  plotname = "TrackerMuonFakeRateEta_WithSystematics";

  muonFakeRate_Eta_WJets = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Eta_MadgraphWJets");
  muonFakeRate_Eta_JetTrigger = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Eta_Madgraph_Jet50");
  muonFakeRate_Eta_PhotonTrigger = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Eta_Madgraph_Photon15");
  muonFakeRate_Eta_sysErrors = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Eta_Madgraph_SysErrors");

  assert(muonFakeRate_Eta_WJets);
  assert(muonFakeRate_Eta_JetTrigger);
  assert(muonFakeRate_Eta_PhotonTrigger);
  assert(muonFakeRate_Eta_sysErrors);

  //re-calculate errors for the JetTrigger fake rate adding systematics + statistical in quadrature
  for (int bin = 0; bin < muonFakeRate_Eta_JetTrigger->GetNbinsX()+1; ++bin) {
    Double_t totalError = TMath::Sqrt(muonFakeRate_Eta_JetTrigger->GetBinError(bin)*muonFakeRate_Eta_JetTrigger->GetBinError(bin) +
                                      muonFakeRate_Eta_sysErrors->GetBinContent(bin)*muonFakeRate_Eta_sysErrors->GetBinContent(bin));
    cout << "bin " << bin << " : " << totalError << ", " << muonFakeRate_Eta_JetTrigger->GetBinError(bin) << ", " << muonFakeRate_Eta_sysErrors->GetBinContent(bin) << endl;
    muonFakeRate_Eta_JetTrigger->SetBinError(bin, totalError);
  }

  muonFakeRate_Eta_JetTrigger->SetLineColor(COLORS[0]);
  muonFakeRate_Eta_JetTrigger->SetFillColor(COLORS[0]);
  muonFakeRate_Eta_JetTrigger->SetFillStyle(3544);
  muonFakeRate_Eta_JetTrigger->SetMarkerColor(COLORS[0]);
  muonFakeRate_Eta_JetTrigger->SetMarkerStyle(MARKERS[0]);
  muonFakeRate_Eta_WJets->SetLineColor(COLORS[1]);
  muonFakeRate_Eta_WJets->SetMarkerColor(COLORS[1]);
  muonFakeRate_Eta_WJets->SetMarkerStyle(MARKERS[1]);
  muonFakeRate_Eta_PhotonTrigger->SetLineColor(COLORS[2]);
  muonFakeRate_Eta_PhotonTrigger->SetMarkerColor(COLORS[2]);
  muonFakeRate_Eta_PhotonTrigger->SetMarkerStyle(MARKERS[2]);

  muonFakeRate_Eta_JetTrigger->SetMaximum(0.15);
  muonFakeRate_Eta_JetTrigger->SetMinimum(0.0);

  muonFakeRate_Eta_JetTrigger->SetTitle("");
  muonFakeRate_Eta_JetTrigger->SetXTitle("fake muon #eta");
  muonFakeRate_Eta_JetTrigger->SetYTitle("#epsilon_{fake}");
  muonFakeRate_Eta_JetTrigger->SetTitleOffset(1.5, "Y");

  leg1->AddEntry(muonFakeRate_Eta_WJets, "WJets", "LP"); 
  leg1->AddEntry(muonFakeRate_Eta_JetTrigger, "jet trigger (QCD)", "LP"); 
  leg1->AddEntry(muonFakeRate_Eta_PhotonTrigger, "photon trigger (Photon+Jets)", "LP"); 

  muonFakeRate_Eta_JetTrigger->Draw("E5");
  muonFakeRate_Eta_WJets->Draw("same");
  muonFakeRate_Eta_PhotonTrigger->Draw("same");

  
  leg1->SetShadowColor(kWhite);
  leg1->SetFillColor(kWhite);
  leg1->Draw();

  cv->SaveAs((plotname+".gif").c_str());
  cv->SaveAs((plotname+".eps").c_str());



  file->Close();
  return;

}



//--------------------------------------------------------------------------------------------------
// Plots related to fake electron categories
//--------------------------------------------------------------------------------------------------
void makeElectronNumeratorDenominatorPlots( vector<vector<string> > JetTriggerDatasetFiles,
                                            vector<vector<string> > JetTriggerDatasetNames,
                                            vector<string> JetTriggerDatasetLegend,
                                            vector<vector<string> > PhotonTriggerDatasetFiles,
                                            vector<vector<string> > PhotonTriggerDatasetNames,
                                            vector<string> PhotonTriggerDatasetLegend ,
                                            string plotLabel = "" ) {

  THStack *NumeratorStack = 0;
  THStack *DenominatorStack = 0;
  vector<double> fakebins;

  string label = "_" + plotLabel;
  //*****************************************************************************************
  //
  //Photon15 Trigger
  //
  //*****************************************************************************************


  //*****************************************************************************************
  //Pt plots
  //*****************************************************************************************
  NumeratorStack = addAllSamplesStacked(PhotonTriggerDatasetFiles, PhotonTriggerDatasetNames, "ComputeElectronFakeRateMod_Reco_Photon15", "hElectronNumeratorPt", fakebins, "electron p_{T} [GeV/c]");
  DenominatorStack = addAllSamplesStacked(PhotonTriggerDatasetFiles, PhotonTriggerDatasetNames, "ComputeElectronFakeRateMod_Reco_Photon15", "hElectronDenominatorPt", fakebins, "electron p_{T} [GeV/c]");

  
  drawStackedPlot(NumeratorStack, "ElectronNumeratorPtStacked_PhotonTriggerSample"+ label ,PhotonTriggerDatasetLegend, false, 800, 0, 100, 0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(NumeratorStack, "ElectronNumeratorPtStacked_PhotonTriggerSample_logY"+ label ,PhotonTriggerDatasetLegend, true, 50000, 0, 100, 0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(DenominatorStack, "RecoElectronDenominatorPtStacked_PhotonTriggerSample"+ label ,PhotonTriggerDatasetLegend,false, 100000, 0, 100, 0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(DenominatorStack, "RecoElectronDenominatorPtStacked_PhotonTriggerSample_logY"+ label ,PhotonTriggerDatasetLegend,true, 1000000, 0, 100, 0.75, 0.70, 0.95, 0.95);

  //*****************************************************************************************
  //Eta plots
  //*****************************************************************************************
  NumeratorStack = addAllSamplesStacked(PhotonTriggerDatasetFiles, PhotonTriggerDatasetNames, "ComputeElectronFakeRateMod_Reco_Photon15", "hElectronNumeratorEta", fakebins, "electron #eta");
  DenominatorStack = addAllSamplesStacked(PhotonTriggerDatasetFiles, PhotonTriggerDatasetNames, "ComputeElectronFakeRateMod_Reco_Photon15", "hElectronDenominatorEta", fakebins, "electron #eta");

  drawStackedPlot(NumeratorStack, "ElectronNumeratorEtaStacked_PhotonTriggerSample"+ label ,PhotonTriggerDatasetLegend, false, 200, -99, -99,  0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(NumeratorStack, "ElectronNumeratorEtaStacked_PhotonTriggerSample_logY"+ label ,PhotonTriggerDatasetLegend, true, 2000, -99, -99,  0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(DenominatorStack, "RecoElectronDenominatorEtaStacked_PhotonTriggerSample"+ label ,PhotonTriggerDatasetLegend,false, 8000, -99, -99,  0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(DenominatorStack, "RecoElectronDenominatorEtaStacked_PhotonTriggerSample_logY"+ label ,PhotonTriggerDatasetLegend,true, 20000, -99, -99,  0.80, 0.70, 0.95, 0.95);


  //*****************************************************************************************
  //
  //Jet50 Trigger
  //
  //*****************************************************************************************

  //*****************************************************************************************
  //Pt plots
  //*****************************************************************************************
  NumeratorStack = addAllSamplesStacked(JetTriggerDatasetFiles, JetTriggerDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet110", "hElectronNumeratorPt", fakebins, "electron p_{T} [GeV/c]");
  DenominatorStack = addAllSamplesStacked(JetTriggerDatasetFiles, JetTriggerDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet110", "hElectronDenominatorPt", fakebins, "electron p_{T} [GeV/c]");

  
  drawStackedPlot(NumeratorStack, "ElectronNumeratorPtStacked_JetTriggerSample"+ label , JetTriggerDatasetLegend, false, 20000, 0, 100, 0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(NumeratorStack, "ElectronNumeratorPtStacked_JetTriggerSample_logY"+ label , JetTriggerDatasetLegend, true, 100000, 0, 100, 0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(DenominatorStack, "RecoElectronDenominatorPtStacked_JetTriggerSample"+ label , JetTriggerDatasetLegend,false, 4000000, 0, 100, 0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(DenominatorStack, "RecoElectronDenominatorPtStacked_JetTriggerSample_logY"+ label , JetTriggerDatasetLegend,true, 5000000, 0, 100, 0.75, 0.70, 0.95, 0.95);


  //*****************************************************************************************
  //Eta plots
  //*****************************************************************************************
  NumeratorStack = addAllSamplesStacked(JetTriggerDatasetFiles, JetTriggerDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet110", "hElectronNumeratorEta", fakebins, "electron #eta");
  DenominatorStack = addAllSamplesStacked(JetTriggerDatasetFiles, JetTriggerDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet110", "hElectronDenominatorEta", fakebins, "electron #eta");


  drawStackedPlot(NumeratorStack, "ElectronNumeratorEtaStacked_JetTriggerSample"+ label , JetTriggerDatasetLegend, false, 5000, -99, -99,  0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(NumeratorStack, "ElectronNumeratorEtaStacked_JetTriggerSample_logY"+ label , JetTriggerDatasetLegend, true, 100000, -99, -99,  0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(DenominatorStack, "RecoElectronDenominatorEtaStacked_JetTriggerSample"+ label , JetTriggerDatasetLegend,false, 400000, -99, -99,  0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(DenominatorStack, "RecoElectronDenominatorEtaStacked_JetTriggerSample_logY"+ label , JetTriggerDatasetLegend,true, 10000000, -99, -99,  0.75, 0.70, 0.95, 0.95);

  //*****************************************************************************************
  //DeltaPhi TriggerJet
  //*****************************************************************************************




  return;
}


//--------------------------------------------------------------------------------------------------
// Plots related to fake electron categories
//--------------------------------------------------------------------------------------------------
void makeMuonNumeratorDenominatorPlots( vector<vector<string> > JetTriggerDatasetFiles,
                                        vector<vector<string> > JetTriggerDatasetNames,
                                        vector<string> JetTriggerDatasetLegend,
                                        vector<vector<string> > PhotonTriggerDatasetFiles,
                                        vector<vector<string> > PhotonTriggerDatasetNames,
                                        vector<string> PhotonTriggerDatasetLegend,
                                        string plotLabel = "") {
  string label = "_"+plotLabel;
  THStack *NumeratorStack = 0;
  THStack *DenominatorStack = 0;

  vector<double> fakebins;

  //*****************************************************************************************
  //
  //Photon15 Trigger
  //
  //*****************************************************************************************



  //*****************************************************************************************
  //Pt plots
  //*****************************************************************************************
  NumeratorStack = addAllSamplesStacked(PhotonTriggerDatasetFiles, PhotonTriggerDatasetNames, "ComputeMuonFakeRateMod_Global_Photon15", "hMuonNumeratorPt", fakebins, "muon p_{T} [GeV/c]", "number of events in 200pb^{-1}");
  DenominatorStack = addAllSamplesStacked(PhotonTriggerDatasetFiles, PhotonTriggerDatasetNames, "ComputeMuonFakeRateMod_Global_Photon15", "hMuonDenominatorPt", fakebins, "muon p_{T} [GeV/c]", "number of events in 200pb^{-1}");

  drawStackedPlot(NumeratorStack, "MuonNumeratorPtStacked_PhotonTriggerSample"+label, PhotonTriggerDatasetLegend, false, -99, 0, 100, 0.65, 0.60, 0.95, 0.95);
  drawStackedPlot(NumeratorStack, "MuonNumeratorPtStacked_PhotonTriggerSample_logY"+label, PhotonTriggerDatasetLegend, true, -99, 0, 100, 0.65, 0.60, 0.95, 0.95);
  drawStackedPlot(DenominatorStack, "GlobalMuonDenominatorPtStacked_PhotonTriggerSample"+label, PhotonTriggerDatasetLegend  ,false, -99, 0, 100, 0.65, 0.60, 0.95, 0.95);
  drawStackedPlot(DenominatorStack, "GlobalMuonDenominatorPtStacked_PhotonTriggerSample_logY"+label, PhotonTriggerDatasetLegend ,true, -99, 0, 100, 0.65, 0.60, 0.95, 0.95);


  //*****************************************************************************************
  //Eta plots
  //*****************************************************************************************
  NumeratorStack = addAllSamplesStacked(PhotonTriggerDatasetFiles, PhotonTriggerDatasetNames, "ComputeMuonFakeRateMod_Global_Photon15", "hMuonNumeratorEta", fakebins, "muon #eta", "number of events in 200pb^{-1}");
  DenominatorStack = addAllSamplesStacked(PhotonTriggerDatasetFiles, PhotonTriggerDatasetNames, "ComputeMuonFakeRateMod_Global_Photon15", "hMuonDenominatorEta", fakebins, "muon #eta", "number of events in 200pb^{-1}");


  drawStackedPlot(NumeratorStack, "MuonNumeratorEtaStacked_PhotonTriggerSample"+label, PhotonTriggerDatasetLegend, false, 200, -99, -99,  0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(NumeratorStack, "MuonNumeratorEtaStacked_PhotonTriggerSample_logY"+label, PhotonTriggerDatasetLegend, true, 10000, -99, -99,  0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(DenominatorStack, "GlobalMuonDenominatorEtaStacked_PhotonTriggerSample"+label, PhotonTriggerDatasetLegend, false, 1000, -99, -99,  0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(DenominatorStack, "GlobalMuonDenominatorEtaStacked_PhotonTriggerSample_logY"+label, PhotonTriggerDatasetLegend, true, 10000, -99, -99,  0.75, 0.70, 0.95, 0.95);


//   //*****************************************************************************************
//   //
//   //Jet50 Trigger
//   //
//   //*****************************************************************************************


  //*****************************************************************************************
  //Pt plots
  //*****************************************************************************************
  NumeratorStack = addAllSamplesStacked(JetTriggerDatasetFiles, JetTriggerDatasetNames, "ComputeMuonFakeRateMod_Global_Jet110", "hMuonNumeratorPt", fakebins, "muon p_{T} [GeV/c]", "number of events in 200pb^{-1}");
  DenominatorStack = addAllSamplesStacked(JetTriggerDatasetFiles, JetTriggerDatasetNames, "ComputeMuonFakeRateMod_Global_Jet110", "hMuonDenominatorPt", fakebins, "muon p_{T} [GeV/c]", "number of events in 200pb^{-1}");


  drawStackedPlot(NumeratorStack, "MuonNumeratorPtStacked_JetTriggerSample"+label, JetTriggerDatasetLegend, false, -99, 0, 100, 0.65, 0.70, 0.95, 0.95);
  drawStackedPlot(NumeratorStack, "MuonNumeratorPtStacked_JetTriggerSample_logY"+label, JetTriggerDatasetLegend, true, 1000000, 0, 100, 0.65, 0.70, 0.95, 0.95);
  drawStackedPlot(DenominatorStack, "GlobalMuonDenominatorPtStacked_JetTriggerSample"+label, JetTriggerDatasetLegend,false, -99, 0, 100, 0.65, 0.70, 0.95, 0.95);
  drawStackedPlot(DenominatorStack, "GlobalMuonDenominatorPtStacked_JetTriggerSample_logY"+label, JetTriggerDatasetLegend,true, 1000000, 0, 100, 0.65, 0.70, 0.95, 0.95);



  //*****************************************************************************************
  //Eta plots
  //*****************************************************************************************
  NumeratorStack = addAllSamplesStacked(JetTriggerDatasetFiles, JetTriggerDatasetNames, "ComputeMuonFakeRateMod_Global_Jet110", "hMuonNumeratorEta", fakebins, "muon #eta", "number of events in 200pb^{-1}");
  DenominatorStack = addAllSamplesStacked(JetTriggerDatasetFiles, JetTriggerDatasetNames, "ComputeMuonFakeRateMod_Global_Jet110", "hMuonDenominatorEta", fakebins, "muon #eta", "number of events in 200pb^{-1}");

  drawStackedPlot(NumeratorStack, "MuonNumeratorEtaStacked_JetTriggerSample"+label, JetTriggerDatasetLegend, false, 2000, -99, -99,  0.80, 0.70, 0.95, 0.95);
  drawStackedPlot(NumeratorStack, "MuonNumeratorEtaStacked_JetTriggerSample_logY"+label, JetTriggerDatasetLegend, true, 10000, -99, -99,  0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(DenominatorStack, "GlobalMuonDenominatorEtaStacked_JetTriggerSample"+label, JetTriggerDatasetLegend, false, 5000, -99, -99,  0.75, 0.70, 0.95, 0.95);
  drawStackedPlot(DenominatorStack, "GlobalMuonDenominatorEtaStacked_JetTriggerSample_logY"+label, JetTriggerDatasetLegend, true, 50000, -99, -99,  0.80, 0.70, 0.95, 0.95);

  return;
}


//--------------------------------------------------------------------------------------------------
// Plots related to fake electron categories
//--------------------------------------------------------------------------------------------------
void makeElectronCategorizationPlots( vector<string> WJetsDatasetFiles,
                                      vector<string> WJetsDatasetNames,
                                      vector<string> QCDDatasetFiles,
                                      vector<string> QCDDatasetNames,
                                      vector<string> PhotonJetsDatasetFiles,
                                      vector<string> PhotonJetsDatasetNames,
                                      string plotLabel = "") {

  string label = "_" + plotLabel;

  //*****************************************************************************************
  //Construct the fake rate Histgrams. Fit with function.
  //*****************************************************************************************
  vector<TH1F*> Histograms; Histograms.clear();
  vector<string> LegendNames; LegendNames.clear();
  TH1F *FakeCategory = NULL;

  //*****************************************************************************************
  //Numerator Fake Categories : WJets Vs QCD
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  // wjets gsf track
  FakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back("WJets");

  // QCD gsf track
  FakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back("QCD");

  // PhotonJets gsf track
  FakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeElectronFakeRateMod_Photon15", "hElectronNumeratorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back("PhotonJets");


  drawPlots(Histograms,LegendNames, "ElectronNumeratorFakeCategory" + label + "_WJetsVsQCD", "electron numerator fake category", "electron fake category", "fraction" , 0, 0.7, true, false);
  drawPlots(Histograms,LegendNames, "ElectronNumeratorFakeCategory" + label + "_WJetsVsQCD_LogY", "electron numerator fake category", "electron fake category", "fraction" , 0,0.7, true, true);


  //*****************************************************************************************
  //GsfTrack Denominator Fake Categories : WJets Vs QCD
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  // wjets gsf track
  FakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back("WJets");

  // QCD gsf track
  FakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back("QCD");

  // PhotonJets gsf track
  FakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeElectronFakeRateMod_Photon15", "hElectronDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back("PhotonJets");


  drawPlots(Histograms,LegendNames, "GsfTrackElectronDenominatorFakeCategory" + label + "_WJetsVsQCD", "electron GsfTrack denominator fake category", "electron fake category", "fraction" , -1,-1, true, false);
  drawPlots(Histograms,LegendNames, "GsfTrackElectronDenominatorFakeCategory" + label + "_WJetsVsQCD_logY", "electron GsfTrack denominator fake category", "electron fake category", "fraction" , 1e-4,-1, true, true);



  //*****************************************************************************************
  //Reco Denominator Fake Categories : WJets Vs QCD
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  // wjets reco
  FakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back(" WJets");

  // QCD reco
  FakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back(" QCD");

  // PhotonJets reco
  FakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeElectronFakeRateMod_Reco_Photon15", "hElectronDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back(" PhotonJets");


  drawPlots(Histograms,LegendNames, "RecoElectronDenominatorFakeCategory" + label + "_WJetsVsQCD", "electron Reco denominator fake category", "electron fake category", "fraction" , -1,-1, true, false);
  drawPlots(Histograms,LegendNames, "RecoElectronDenominatorFakeCategory" + label + "_WJetsVsQCD_logY", "electron reco denominator fake category",  "electron fake category", "fraction" ,-1,-1, true, true);





  //*****************************************************************************************
  //Fake Rates as function of Fake Category
  //*****************************************************************************************
  TH1F *NumeratorFakeCategory = NULL;
  TH1F *DenominatorFakeCategory = NULL;
  TH1F *FakeRateFakeCategory = NULL;

  //*****************************************************************************************
  //Find Fake Rate as function of Fake Category for GsfTrack Denominator
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  // wjets gsf track
  NumeratorFakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_WJets");
  FakeRateFakeCategory->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("WJets");

  // QCD gsf track
  NumeratorFakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_QCD");
  FakeRateFakeCategory->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("QCD");

  // PhotonJets gsf track
  NumeratorFakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeElectronFakeRateMod_Photon15", "hElectronNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeElectronFakeRateMod_Photon15", "hElectronDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_PhotonJets");
  FakeRateFakeCategory->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("PhotonJets");


  drawPlots(Histograms,LegendNames, "GsfTrackElectronFakeRate_FakeCategory" + label, "GsfTrack electron denominator fake rate", "electron fake category", "#epsilon_{fake}" , -1,1, false, true, 0.25, 0.8, 0.5, 0.9);


  //*****************************************************************************************
  //Find Fake Rate as function of Fake Category for Reco Denominator
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  // wjets gsf track
  NumeratorFakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_WJets");
  FakeRateFakeCategory->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("WJets");

  // QCD gsf track
  NumeratorFakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_QCD");
  FakeRateFakeCategory->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("QCD");

  // PhotonJets gsf track
  NumeratorFakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeElectronFakeRateMod_Reco_Photon15", "hElectronNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeElectronFakeRateMod_Reco_Photon15", "hElectronDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_PhotonJets");
  FakeRateFakeCategory->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("PhotonJets");


  drawPlots(Histograms,LegendNames, "RecoElectronFakeRate_FakeCategory" + label, "Reco electron denominator fake rate",  "electron fake category", "#epsilon_{fake}" ,-1, 1, false, true, 0.25, 0.8, 0.5, 0.9 );

}


//--------------------------------------------------------------------------------------------------
// Plots related to fake electron categories
//--------------------------------------------------------------------------------------------------
void makeMuonCategorizationPlots( vector<string> WJetsDatasetFiles,
                                  vector<string> WJetsDatasetNames,
                                  vector<string> QCDDatasetFiles,
                                  vector<string> QCDDatasetNames,
                                  vector<string> PhotonJetsDatasetFiles,
                                  vector<string> PhotonJetsDatasetNames, 
                                  string plotLabel = "" ) {

  string label = "_" + plotLabel;
  //*****************************************************************************************
  //Construct the fake rate Histgrams. Fit with function.
  //*****************************************************************************************
  vector<TH1F*> Histograms; Histograms.clear();
  vector<string> LegendNames; LegendNames.clear();
  TH1F *FakeCategory = NULL;

  //*****************************************************************************************
  //Numerator Fake Categories : WJets Vs QCD
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  // wjets gsf track
  FakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod", "hMuonNumeratorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back(" WJets");

  // QCD gsf track
  FakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_Jet50", "hMuonNumeratorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back(" QCD");

  // PhotonJets isotrack
  FakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_Photon15", "hMuonNumeratorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back(" PhotonJets");

  drawPlots(Histograms,LegendNames, "MuonNumeratorFakeCategory" + label + "_WJetsVsQCD", "Muon Numerator FakeCategory", "muon fake category", "fraction" ,-1, -1, true, false, 0.70, 0.8, 0.90, 0.9);
  drawPlots(Histograms,LegendNames, "MuonNumeratorFakeCategory" + label + "_WJetsVsQCD_LogY", "Muon Numerator FakeCategory", "muon fake category", "fraction" ,-1, 1.0, true, true);

  //*****************************************************************************************
  //IsoTrack Denominator Fake Categories : WJets Vs QCD
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  // wjets gsf track
  FakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod", "hMuonDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back(" WJets");

  // QCD gsf track
  FakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_Jet50", "hMuonDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back(" QCD");

  // PhotonJets gsf track
  FakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_Photon15", "hMuonDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back(" PhotonJets");

  drawPlots(Histograms,LegendNames, "IsoTrackMuonDenominatorFakeCategory" + label + "_WJetsVsQCD", "Muon IsoTrack Denominator FakeCategory", "muon fake category", "fraction" , -1,-1, true, false, 0.25, 0.8, 0.45, 0.9);
  drawPlots(Histograms,LegendNames, "IsoTrackMuonDenominatorFakeCategory" + label + "_WJetsVsQCD_logY", "Muon IsoTrack Denominator FakeCategory", "muon fake category", "fraction" ,-1, 1.00, true, true, 0.25, 0.8, 0.5, 0.9);

  //*****************************************************************************************
  //TrackerMuon Denominator Fake Categories : WJets Vs QCD
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  // wjets gsf track
  FakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon", "hMuonDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back(" WJets");

  // QCD gsf track
  FakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet50", "hMuonDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back(" QCD");

  // PhotonJets gsf track
  FakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Photon15", "hMuonDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back(" PhotonJets");

  drawPlots(Histograms,LegendNames, "TrackerMuonDenominatorFakeCategory" + label + "_WJetsVsQCD", "Muon TrackerMuon Denominator FakeCategory", "muon fake category", "fraction" , -1,-1, true, false, 0.70, 0.8, 0.90, 0.9);
  drawPlots(Histograms,LegendNames, "TrackerMuonDenominatorFakeCategory" + label + "_WJetsVsQCD_logY", "Muon TrackerMuon Denominator FakeCategory",  "muon fake category", "fraction" ,-1,1.0, true, true, 0.25, 0.8, 0.5, 0.9);


  //*****************************************************************************************
  //Global Denominator Fake Categories : WJets Vs QCD
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  // wjets gsf track
  FakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod_Global", "hMuonDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back(" WJets");

  // QCD gsf track
  FakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_Global_Jet50", "hMuonDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back(" QCD");

  // PhotonJets gsf track
  FakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_Global_Photon15", "hMuonDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back(" PhotonJets");

  drawPlots(Histograms,LegendNames, "GlobalMuonDenominatorFakeCategory" + label + "_WJetsVsQCD", "Muon Global Denominator FakeCategory", "muon fake category", "fraction" , -1,-1, true, false, 0.70, 0.8, 0.90, 0.9);
  drawPlots(Histograms,LegendNames, "GlobalMuonDenominatorFakeCategory" + label + "_WJetsVsQCD_logY", "Muon Global Denominator FakeCategory",  "muon fake category", "fraction" ,-1,1.0, true, true, 0.25, 0.8, 0.5, 0.9);





  //*****************************************************************************************
  //Fake Rates as function of Fake Category
  //*****************************************************************************************
  TH1F *NumeratorFakeCategory = NULL;
  TH1F *DenominatorFakeCategory = NULL;
  TH1F *FakeRateFakeCategory = NULL;

  //*****************************************************************************************
  //Find Fake Rate as function of Fake Category for GsfTrack Denominator
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  // wjets isoTrack
  NumeratorFakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod", "hMuonNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod", "hMuonDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_WJets");
  FakeRateFakeCategory->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");  
  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back(" WJets");

  // QCD isoTrack
  NumeratorFakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_Jet50", "hMuonNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_Jet50", "hMuonDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_QCD");
  FakeRateFakeCategory->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back(" QCD");

  // PhotonJets isoTrack
  NumeratorFakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_Photon15", "hMuonNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_Photon15", "hMuonDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_PhotonJets");
  FakeRateFakeCategory->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back(" PhotonJets");

  drawPlots(Histograms,LegendNames, "IsoTrackMuonFakeRate_FakeCategory" + label, "IsoTrack Muon Denominator Fake Rate", "muon fake category", "#epsilon_{fake}" ,0.00000, 1, false, false);
  drawPlots(Histograms,LegendNames, "IsoTrackMuonFakeRate_FakeCategory_logY" + label, "IsoTrack Muon Denominator Fake Rate", "muon fake category", "#epsilon_{fake}" ,0.000001, 1.0, false, true);


  //*****************************************************************************************
  //Find Fake Rate as function of Fake Category for TrackerMuon Denominator
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  // wjets gsf track
  NumeratorFakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon", "hMuonNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon", "hMuonDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_WJets");
  FakeRateFakeCategory->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back(" WJets");

  // QCD gsf track
  NumeratorFakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet50", "hMuonNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet50", "hMuonDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_QCD");
  FakeRateFakeCategory->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back(" QCD");

  // PhotonJets gsf track
  NumeratorFakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Photon15", "hMuonNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Photon15", "hMuonDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_PhotonJets");
  FakeRateFakeCategory->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back(" PhotonJets");

  drawPlots(Histograms,LegendNames, "TrackerMuonFakeRate_FakeCategory" + label, "TrackerMuon Muon Denominator Fake Rate",  "muon fake category", "#epsilon_{fake}" ,0, 0.2, false, false);
  drawPlots(Histograms,LegendNames, "TrackerMuonFakeRate_FakeCategory_logY" + label, "TrackerMuon Muon Denominator Fake Rate",  "muon fake category", "#epsilon_{fake}" ,-1, 1.0, false, true);



  //*****************************************************************************************
  //Find Fake Rate as function of Fake Category for Global Denominator
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  // wjets gsf track
  NumeratorFakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod_Global", "hMuonNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod_Global", "hMuonDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_WJets");
  FakeRateFakeCategory->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back(" WJets");

  // QCD gsf track
  NumeratorFakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_Global_Jet50", "hMuonNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_Global_Jet50", "hMuonDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_QCD");
  FakeRateFakeCategory->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back(" QCD");

  // PhotonJets gsf track
  NumeratorFakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_Global_Photon15", "hMuonNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_Global_Photon15", "hMuonDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_PhotonJets");
  FakeRateFakeCategory->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back(" PhotonJets");

  drawPlots(Histograms,LegendNames, "GlobalMuonFakeRate_FakeCategory" + label, "Global Muon Denominator Fake Rate",  "muon fake category", "#epsilon_{fake}" ,0, 0.2, false, false);
  drawPlots(Histograms,LegendNames, "GlobalMuonFakeRate_FakeCategory_logY" + label, "Global Muon Denominator Fake Rate",  "muon fake category", "#epsilon_{fake}" ,-1, 1.0, false, true);

}


TH1F *makeJetFlavorPlot(vector<string> datasetFiles, vector<string> datasetNames,
                    string dirName, string histName) {

  TH1F *inputJetFlavorHist = addAllSamples(datasetFiles, datasetNames, dirName, histName);

  TH1F *jetFlavor = new TH1F(inputJetFlavorHist->GetName(), ";JetFlavor;Number of Events", 9, -0.5, 8.5);
  TAxis *xa = jetFlavor->GetXaxis();
  xa->SetBinLabel(1,"NoNearGenJet");
  xa->SetBinLabel(2,"NoJetFlavorMatch");
  xa->SetBinLabel(3,"light q");
  xa->SetBinLabel(4,"light qbar");
  xa->SetBinLabel(5,"c");
  xa->SetBinLabel(6,"cbar");
  xa->SetBinLabel(7,"b");
  xa->SetBinLabel(8,"bbar");
  xa->SetBinLabel(9,"gluon");

  Double_t BinContent[9];
  Double_t BinErrorSquared[9];
  
  for (UInt_t i=0; i < 9;++i) {
    BinContent[i] = 0;
    BinErrorSquared[i] = 0;
  }

  for (UInt_t i=0;int(i)<=inputJetFlavorHist->GetXaxis()->GetNbins()+1;++i) {
    Double_t binCenter = inputJetFlavorHist->GetXaxis()->GetBinCenter(i);
    if (binCenter == 1 || binCenter == 2 || binCenter == 3) {
      BinContent[2] += inputJetFlavorHist->GetBinContent(i);
      BinErrorSquared[2] += inputJetFlavorHist->GetBinError(i)*inputJetFlavorHist->GetBinError(i);
    } else if (binCenter == -1 || binCenter == -2 || binCenter == -3) {
      BinContent[3] += inputJetFlavorHist->GetBinContent(i);
      BinErrorSquared[3] += inputJetFlavorHist->GetBinError(i)*inputJetFlavorHist->GetBinError(i);
    } else if (binCenter == 4) {
      BinContent[4] += inputJetFlavorHist->GetBinContent(i);
      BinErrorSquared[4] += inputJetFlavorHist->GetBinError(i)*inputJetFlavorHist->GetBinError(i);
    } else if (binCenter == -4) {
      BinContent[5] += inputJetFlavorHist->GetBinContent(i);
      BinErrorSquared[5] += inputJetFlavorHist->GetBinError(i)*inputJetFlavorHist->GetBinError(i);
    } else if (binCenter == 5) {
      BinContent[6] += inputJetFlavorHist->GetBinContent(i);
      BinErrorSquared[6] += inputJetFlavorHist->GetBinError(i)*inputJetFlavorHist->GetBinError(i);
    } else if (binCenter == -5) {
      BinContent[7] += inputJetFlavorHist->GetBinContent(i);
      BinErrorSquared[7] += inputJetFlavorHist->GetBinError(i)*inputJetFlavorHist->GetBinError(i);
    } else if (binCenter == 21) {
      BinContent[8] += inputJetFlavorHist->GetBinContent(i);
      BinErrorSquared[8] += inputJetFlavorHist->GetBinError(i)*inputJetFlavorHist->GetBinError(i);
    } else if (binCenter == -9) {
      BinContent[0] += inputJetFlavorHist->GetBinContent(i);
      BinErrorSquared[0] += inputJetFlavorHist->GetBinError(i)*inputJetFlavorHist->GetBinError(i);
    } else if (binCenter == 0) {
      BinContent[1] += inputJetFlavorHist->GetBinContent(i);
      BinErrorSquared[1] += inputJetFlavorHist->GetBinError(i)*inputJetFlavorHist->GetBinError(i);
    } else {
      if (inputJetFlavorHist->GetBinContent(i) > 0) {
        cout << "BinCenter: " <<  binCenter << " : " << inputJetFlavorHist->GetBinContent(i) << endl;
      }
    }
  }
  for (UInt_t binx=1; int(binx) <= jetFlavor->GetXaxis()->GetNbins() ;++binx) {
    jetFlavor->SetBinContent(binx,BinContent[binx-1]);
    jetFlavor->SetBinError(binx,TMath::Sqrt(BinErrorSquared[binx-1]));     
  }
  return jetFlavor;
}

//--------------------------------------------------------------------------------------------------
// Plots related to fake electron categories
//--------------------------------------------------------------------------------------------------
void makeElectronJetFlavorPlots( vector<string> WJetsDatasetFiles,
                                 vector<string> WJetsDatasetNames,
                                 vector<string> QCDDatasetFiles,
                                 vector<string> QCDDatasetNames,
                                 vector<string> PhotonJetsDatasetFiles,
                                 vector<string> PhotonJetsDatasetNames,
                                 string plotLabel = "") {

  string label = "_" + plotLabel;

  vector<TH1F*> Histograms; Histograms.clear();
  vector<string> LegendNames; LegendNames.clear();
  TH1F *JetFlavor = NULL;
  TH1F *NumeratorJetFlavor = NULL;
  TH1F *DenominatorJetFlavor = NULL;
  TH1F *FakeRateJetFlavor = NULL;


  //*****************************************************************************************
  //Plot Electron Numerator Jet Flavor
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  JetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("WJets");

  JetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("QCD");

  JetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeElectronFakeRateMod_Photon15", "hElectronNumeratorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("PhotonJets");


  drawPlots(Histograms,LegendNames, "ElectronNumeratorJetFlavor" + label + "_WJetsVsQCD", "Fake Electron Jet Flavor", "jet flavor", "fraction", 0.0 ,0.75, true, false);
  drawPlots(Histograms,LegendNames, "ElectronNumeratorJetFlavor" + label + "_WJetsVsQCD_logY", "Fake Electron Jet Flavor",  "jet flavor", "fraction", 0.0 , 0.75, true, true);
  

  //*****************************************************************************************
  //Plot GsfTrack Electron Denominator Jet Flavor
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  JetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("WJets");

  JetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("QCD");

  JetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeElectronFakeRateMod_Photon15", "hElectronDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("PhotonJets");


  drawPlots(Histograms,LegendNames, "GsfTrackElectronDenominatorJetFlavor" + label + "_WJetsVsQCD", "Fake Electron Jet Flavor",  "jet flavor", "Fraction",-1, -1, true, false);
  drawPlots(Histograms,LegendNames, "GsfTrackElectronDenominatorJetFlavor" + label + "_WJetsVsQCD_logY", "Fake Electron Jet Flavor",  "jet flavor", "Fraction",-1, -1, true, true);
  
  Histograms.clear();
  LegendNames.clear();


  //*****************************************************************************************
  //Plot Reco Electron Denominator Jet Flavor
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  JetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("WJets");

  JetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("QCD");

  JetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeElectronFakeRateMod_Reco_Photon15", "hElectronDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("PhotonJets");


  drawPlots(Histograms,LegendNames, "RecoElectronDenominatorJetFlavor" + label + "_WJetsVsQCD", "Fake Electron Jet Flavor",  "jet flavor", "Fraction", -1,-1, true, false);
  drawPlots(Histograms,LegendNames, "RecoElectronDenominatorJetFlavor" + label + "_WJetsVsQCD_logY", "Fake Electron Jet Flavor",  "jet flavor", "Fraction", -1,-1, true, true);
  
  Histograms.clear();
  LegendNames.clear();


  //*****************************************************************************************
  //Plot GsfTrack Electron Fake Rate as function of Jet Flavor
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  NumeratorJetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorJetFlavor");
  DenominatorJetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_WJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("WJets");


  NumeratorJetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorJetFlavor");
  DenominatorJetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_QCD");
  FakeRateJetFlavor->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("QCD");


  NumeratorJetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeElectronFakeRateMod_Photon15", "hElectronNumeratorJetFlavor");
  DenominatorJetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeElectronFakeRateMod_Photon15", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_PhotonJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("PhotonJets");


  drawPlots(Histograms,LegendNames, "GsfTrackElectronFakeRateJetFlavor" + label + "_WJetsVsQCD", "GsfTrack Electron Fake Rate",  "jet flavor", "#epsilon_{fake}",-1, 1, true, true);

  //*****************************************************************************************
  //Plot Reco Electron Fake Rate as function of Jet Flavor
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  NumeratorJetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorJetFlavor");
  DenominatorJetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_WJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("WJets");


  NumeratorJetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorJetFlavor");
  DenominatorJetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_QCD");
  FakeRateJetFlavor->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("QCD");


  NumeratorJetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeElectronFakeRateMod_Reco_Photon15", "hElectronNumeratorJetFlavor");
  DenominatorJetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeElectronFakeRateMod_Reco_Photon15", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_PhotonJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("PhotonJets");


  drawPlots(Histograms,LegendNames, "RecoElectronFakeRateJetFlavor" + label + "_WJetsVsQCD", "GsfTrack Electron Fake Rate",  "jet flavor", "#epsilon_{fake}",-1, 1.0, true, true);


}

//--------------------------------------------------------------------------------------------------
// Plots related to fake electron categories
//--------------------------------------------------------------------------------------------------
void makeMuonJetFlavorPlots( vector<string> WJetsDatasetFiles,
                             vector<string> WJetsDatasetNames,
                             vector<string> QCDDatasetFiles,
                             vector<string> QCDDatasetNames,
                             vector<string> PhotonJetsDatasetFiles,
                             vector<string> PhotonJetsDatasetNames,
                             string plotLabel = "") {

  string label = "_" + plotLabel;

  vector<TH1F*> Histograms; Histograms.clear();
  vector<string> LegendNames; LegendNames.clear();
  TH1F *JetFlavor = NULL;
  TH1F *NumeratorJetFlavor = NULL;
  TH1F *DenominatorJetFlavor = NULL;
  TH1F *FakeRateJetFlavor = NULL;


  //*****************************************************************************************
  //Plot Muon Numerator Jet Flavor
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  JetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod", "hMuonNumeratorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("WJets");

  JetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_Jet50", "hMuonNumeratorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("QCD");

  JetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_Photon15", "hMuonNumeratorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("PhotonJets");

  drawPlots(Histograms,LegendNames, "MuonNumeratorJetFlavor" + label + "_WJetsVsQCD", "Fake Muon Jet Flavor", "jet flavor", "Fraction", -1,0.7, true, false);
  drawPlots(Histograms,LegendNames, "MuonNumeratorJetFlavor" + label + "_WJetsVsQCD_logY", "Fake Muon Jet Flavor",  "jet flavor", "Fraction", -1,1.0, true, true);
  

  //*****************************************************************************************
  //Plot IsoTrack Muon Denominator Jet Flavor
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  JetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod", "hMuonDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("WJets");

  JetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_Jet50", "hMuonDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("QCD");

  JetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_Photon15", "hMuonDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("PhotonJets");

  drawPlots(Histograms,LegendNames, "IsoTrackMuonDenominatorJetFlavor" + label + "_WJetsVsQCD", "Fake Muon Jet Flavor",  "jet flavor", "Fraction",-1, 0.7, true, false);
  drawPlots(Histograms,LegendNames, "IsoTrackMuonDenominatorJetFlavor" + label + "_WJetsVsQCD_logY", "Fake Muon Jet Flavor",  "jet flavor", "Fraction",-1, 1.0, true, true);
  
  Histograms.clear();
  LegendNames.clear();


  //*****************************************************************************************
  //Plot TrackerMuon Muon Denominator Jet Flavor
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  JetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon", "hMuonDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("WJets");

  JetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet50", "hMuonDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("QCD");

  JetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Photon15", "hMuonDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("PhotonJets");


  drawPlots(Histograms,LegendNames, "TrackerMuonDenominatorJetFlavor" + label + "_WJetsVsQCD", "Fake Muon Jet Flavor",  "jet flavor", "Fraction", -1,0.7, true, false);
  drawPlots(Histograms,LegendNames, "TrackerMuonDenominatorJetFlavor" + label + "_WJetsVsQCD_logY", "Fake Muon Jet Flavor",  "jet flavor", "Fraction", -1,1.0, true, true);
  
  Histograms.clear();
  LegendNames.clear();


  //*****************************************************************************************
  //Plot Global Muon Denominator Jet Flavor
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  JetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod_Global", "hMuonDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("WJets");

  JetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_Global_Jet50", "hMuonDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("QCD");

  JetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_Global_Photon15", "hMuonDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("PhotonJets");


  drawPlots(Histograms,LegendNames, "GlobalMuonDenominatorJetFlavor" + label + "_WJetsVsQCD", "Fake Muon Jet Flavor",  "jet flavor", "Fraction", -1,0.7, true, false);
  drawPlots(Histograms,LegendNames, "GlobalMuonDenominatorJetFlavor" + label + "_WJetsVsQCD_logY", "Fake Muon Jet Flavor",  "jet flavor", "Fraction", -1,1.0, true, true);
  
  Histograms.clear();
  LegendNames.clear();


  //*****************************************************************************************
  //Plot IsoTrack Muon Fake Rate as function of Jet Flavor
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  NumeratorJetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod", "hMuonNumeratorJetFlavor");
  DenominatorJetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod", "hMuonDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_WJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("WJets");


  NumeratorJetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_Jet50", "hMuonNumeratorJetFlavor");
  DenominatorJetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_Jet50", "hMuonDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_QCD");
  FakeRateJetFlavor->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("QCD");

  NumeratorJetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_Photon15", "hMuonNumeratorJetFlavor");
  DenominatorJetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_Photon15", "hMuonDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_PhotonJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("PhotonJets");


  drawPlots(Histograms,LegendNames, "IsoTrackMuonFakeRateJetFlavor" + label + "_WJetsVsQCD", "GsfTrack Muon Fake Rate",  "jet flavor", "#epsilon_{fake}",0, 1, true, false);
  drawPlots(Histograms,LegendNames, "IsoTrackMuonFakeRateJetFlavor" + label + "_WJetsVsQCD_logY", "GsfTrack Muon Fake Rate",  "jet flavor", "#epsilon_{fake}",0.00001, 1.0, true, true, 0.25, 0.8, 0.5, 0.9);



  //*****************************************************************************************
  //Plot TrackerMuon Muon Fake Rate as function of Jet Flavor
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  NumeratorJetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon", "hMuonNumeratorJetFlavor");
  DenominatorJetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon", "hMuonDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_WJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("WJets");


  NumeratorJetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet50", "hMuonNumeratorJetFlavor");
  DenominatorJetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet50", "hMuonDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_QCD");
  FakeRateJetFlavor->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("QCD");


  NumeratorJetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Photon15", "hMuonNumeratorJetFlavor");
  DenominatorJetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Photon15", "hMuonDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_PhotonJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("PhotonJets");


  drawPlots(Histograms,LegendNames, "TrackerMuonFakeRateJetFlavor" + label + "_WJetsVsQCD", "GsfTrack Muon Fake Rate",  "jet flavor", "#epsilon_{fake}",0, 1, true, false);
  drawPlots(Histograms,LegendNames, "TrackerMuonFakeRateJetFlavor" + label + "_WJetsVsQCD_logY", "GsfTrack Muon Fake Rate",  "jet flavor", "#epsilon_{fake}",-1, 1.0, true, true, 0.25, 0.8, 0.5, 0.9);



  //*****************************************************************************************
  //Plot Global Muon Fake Rate as function of Jet Flavor
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  NumeratorJetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod_Global", "hMuonNumeratorJetFlavor");
  DenominatorJetFlavor = makeJetFlavorPlot(WJetsDatasetFiles, WJetsDatasetNames, "ComputeMuonFakeRateMod_Global", "hMuonDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_WJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("WJets");


  NumeratorJetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_Global_Jet50", "hMuonNumeratorJetFlavor");
  DenominatorJetFlavor = makeJetFlavorPlot(QCDDatasetFiles, QCDDatasetNames, "ComputeMuonFakeRateMod_Global_Jet50", "hMuonDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_QCD");
  FakeRateJetFlavor->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("QCD");


  NumeratorJetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_Global_Photon15", "hMuonNumeratorJetFlavor");
  DenominatorJetFlavor = makeJetFlavorPlot(PhotonJetsDatasetFiles, PhotonJetsDatasetNames, "ComputeMuonFakeRateMod_Global_Photon15", "hMuonDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_PhotonJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("#epsilon_{fake}");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("PhotonJets");


  drawPlots(Histograms,LegendNames, "GlobalMuonFakeRateJetFlavor" + label + "_WJetsVsQCD", "GsfTrack Muon Fake Rate",  "jet flavor", "#epsilon_{fake}",0, 1, true, false);
  drawPlots(Histograms,LegendNames, "GlobalMuonFakeRateJetFlavor" + label + "_WJetsVsQCD_logY", "GsfTrack Muon Fake Rate",  "jet flavor", "#epsilon_{fake}",-1, 1.0, true, true);


}


//--------------------------------------------------------------------------------------------------
// Plots related to fake electron categories
//--------------------------------------------------------------------------------------------------
void makeTwoLeptonPredictionPlots( vector<vector<string> > DatasetFiles, vector<vector<string> > DatasetNames, vector<string> legendLabels, vector<string> WJetsDatasetFiles, vector<string> WJetsDatasetNames) {

  //*****************************************************************************************
  //Binning
  //*****************************************************************************************
  vector<double> fakebins;
  vector<double> ptbins;
  ptbins.push_back(0);   
  ptbins.push_back(10);  
  ptbins.push_back(15);  
  ptbins.push_back(20);  
  ptbins.push_back(25);  
  ptbins.push_back(30);  
  ptbins.push_back(35);  
  ptbins.push_back(40);
  ptbins.push_back(50);   
  ptbins.push_back(80);  
  ptbins.push_back(120);  
  ptbins.push_back(170);  
  ptbins.push_back(250);
  vector<double> etabins;
  etabins.push_back(-3.5);
  etabins.push_back(-2.5);
  etabins.push_back(-1.5);
  etabins.push_back(-0.5);
  etabins.push_back(0.5);
  etabins.push_back(1.5);
  etabins.push_back(2.5);
  etabins.push_back(3.5);
  vector<double> phibins;
  phibins.push_back(-3.5);
  phibins.push_back(-2.5);
  phibins.push_back(-1.5);
  phibins.push_back(-0.5);
  phibins.push_back(0.5);
  phibins.push_back(1.5);
  phibins.push_back(2.5);
  phibins.push_back(3.5);
  vector<double> metbins;
  for (int i=0;i<20;i++) {
    metbins.push_back(i*10);
  }
  vector<double> deltaphibins;
  for (int i=0;i<18;i++) {
    deltaphibins.push_back(i*10);
  }
  vector<double> deltaetabins;
  for (int i=0;i<40;i++) {
    deltaetabins.push_back(-5.0 + i*0.25);
  }
  vector<double> dileptonmassbins;
  for (int i=0;i<20;i++) {
    dileptonmassbins.push_back(i*15);
  }


  vector<string> dirnames;
  vector<string> histnames;
  vector<string> errorhistnames;
  vector<string> histlabels;
  vector<string> dirlegendlabel;
  vector<string> legendnames;
  

  //create single vector of datasetfiles, and datasetnames
  vector<string> datasetfiles;
  vector<string> datasetnames;
  for(int i=0; i<DatasetFiles.size() ; ++i) {
    for(int j=0; j<DatasetFiles[i].size() ; ++j) {
      datasetfiles.push_back(DatasetFiles[i][j]);
      datasetnames.push_back(DatasetNames[i][j]);
    }
  }


  //*****************************************************************************************
  //
  //Fake Electrons Kinematics
  //
  //*****************************************************************************************




  //*****************************************************************************************
  //Fake Electrons Pt  MC vs QCD prediction
  //*****************************************************************************************
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  legendnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  dirnames.push_back("WJetsFakeValidationMod_Reco_Global_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_Global_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  histnames.push_back("hFakeElectronPt_weighted");
  legendnames.push_back("");
  makeCrossDirComparisonPlot(WJetsDatasetFiles, WJetsDatasetNames, dirnames,dirlegendlabel, histnames, legendnames, ptbins, "FakeElectronPt" , "fake electron p_{T} [GeV/c])", "number of events / 200pb^{-1}", 0, 100, -99, 35, 0.45,0.90,0.75,0.90, false);

  //*****************************************************************************************
  //Fake Electrons Eta  MC vs QCD prediction
  //*****************************************************************************************
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  legendnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  dirnames.push_back("WJetsFakeValidationMod_Reco_Global_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_Global_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  histnames.push_back("hFakeElectronEta_weighted");
  legendnames.push_back("");
  makeCrossDirComparisonPlot(WJetsDatasetFiles, WJetsDatasetNames, dirnames,dirlegendlabel, histnames, legendnames, etabins, "FakeElectronEta", "fake electron #eta", "number of events / 200pb^{-1}", -3.5, 3.5, 0, 50, 0.25, 0.70, 0.75, 0.9, false);

  //*****************************************************************************************
  //Electron Fake Categories
  //*****************************************************************************************
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  legendnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_Reco_Global_MadgraphWJets");
  dirlegendlabel.push_back("Reco Madgraph WJets");
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  dirnames.push_back("WJetsFakeValidationMod_Reco_Global_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_Global_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  histnames.push_back("hFakeElectronCategory");
  legendnames.push_back("");
  makeCrossDirComparisonPlot(WJetsDatasetFiles, WJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames,fakebins, "ElectronFakeCategory", "electron fake category", "number of events / 200pb^{-1}", -99, -99, 0, 110, 0.25, 0.70, 0.75, 0.9, false);

  //*****************************************************************************************
  //
  //Fake Muon Kinematics
  //
  //*****************************************************************************************




  //*****************************************************************************************
  //Fake Muons Pt  MC vs QCD prediction
  //*****************************************************************************************
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  legendnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  histnames.push_back("hFakeMuonPt_weighted");
  legendnames.push_back("");
  makeCrossDirComparisonPlot(WJetsDatasetFiles, WJetsDatasetNames, dirnames,dirlegendlabel, histnames, legendnames, ptbins, "FakeMuonPt" , "fake muon p_{T} [GeV/c]", "number of events / 200pb^{-1}", 0, 100, -99, -99, 0.45,0.90,0.75,0.90, false);
  //*****************************************************************************************
  //Fake Muons Eta  MC vs QCD prediction
  //*****************************************************************************************
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  legendnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  histnames.push_back("hFakeMuonEta_weighted");
  legendnames.push_back("");
  makeCrossDirComparisonPlot(WJetsDatasetFiles,WJetsDatasetNames, dirnames,dirlegendlabel, histnames, legendnames, etabins, "FakeMuonEta", "fake muon #eta", "number of events / 200pb^{-1}", -3.5, 3.5, 0, 15, 0.25, 0.7, 0.75, 0.9, false);

  //*****************************************************************************************
  //Muon Fake Categories
  //*****************************************************************************************
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  legendnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_MadgraphWJets");
  dirlegendlabel.push_back("Madgraph WJets");
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  histnames.push_back("hFakeMuonCategory");
  legendnames.push_back("");
  makeCrossDirComparisonPlot(WJetsDatasetFiles, WJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames,fakebins, "MuonFakeCategory", "muon fake category", "number of events / 200pb^{-1}", -99, -99, 0, 20, 0.25, 0.7, 0.75, 0.9, false);


  //*****************************************************************************************
  //
  //Stacked Histograms of prediction with MC
  //
  //*****************************************************************************************
  THStack *Stack = NULL;
  TH1F *Hist = NULL;


  //*****************************************************************************************
  //Dilepton Charge  
  //*****************************************************************************************
  Stack = addAllSamplesStacked(DatasetFiles, DatasetNames, "WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50", "hDileptonCharge_weighted", fakebins, "dilepton charge", "number of events in 200pb^{ -1}" );
  Hist = addAllSamples(datasetfiles, datasetnames, "WJetsFakeValidationMod_MC", "hDileptonCharge_weighted", fakebins);
  drawStackedPlot(Stack, "DileptonCharge_Reco_TrackerMuon_Madgraph_Jet50",legendLabels, false, 150, -99, -99, 0.65, 0.70, 0.95, 0.95, Hist, "MC Simulation");

  Stack = addAllSamplesStacked(DatasetFiles, DatasetNames, "WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15", "hDileptonCharge_weighted", fakebins, "dilepton charge", "number of events in 200pb^{ -1}");
  Hist = addAllSamples(datasetfiles, datasetnames, "WJetsFakeValidationMod_MC", "hDileptonCharge_weighted", fakebins);
  drawStackedPlot(Stack, "DileptonCharge_Reco_TrackerMuon_Madgraph_Photon15",legendLabels, false, 150, -99, -99, 0.65, 0.70, 0.95, 0.95, Hist, "MC Simulation");

  //*****************************************************************************************
  //Lepton1 Pt
  //*****************************************************************************************
  Stack = addAllSamplesStacked(DatasetFiles, DatasetNames, "WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50", "hLeptonPtMax", ptbins, "lepton1 p_{T} [GeV/c]", "number of events in 200pb^{ -1}");  
  Hist = addAllSamples(datasetfiles, datasetnames, "WJetsFakeValidationMod_MC", "hLeptonPtMax", ptbins);
  drawStackedPlot(Stack, "Lepton1Pt_Reco_TrackerMuon_Madgraph_Jet50",legendLabels, false, 45, -99, -99, 0.65, 0.65, 0.95, 0.90, Hist, "MC Simulation");
  
  Stack = addAllSamplesStacked(DatasetFiles, DatasetNames, "WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15", "hLeptonPtMax", ptbins, "lepton1 p_{T} [GeV/c]", "number of events in 200pb^{ -1}");
  Hist = addAllSamples(datasetfiles, datasetnames, "WJetsFakeValidationMod_MC", "hLeptonPtMax", ptbins);
  drawStackedPlot(Stack, "Lepton1Pt_Reco_TrackerMuon_Madgraph_Photon15",legendLabels, false, 50, -99, -99, 0.65, 0.65, 0.95, 0.90, Hist, "MC Simulation");
  
  //*****************************************************************************************
  //Lepton2 Pt
  //*****************************************************************************************
  Stack = addAllSamplesStacked(DatasetFiles, DatasetNames, "WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50", "hLeptonPtMin", ptbins, "lepton2 p_{T} [GeV/c]", "number of events in 200pb^{ -1}");  
  Hist = addAllSamples(datasetfiles, datasetnames, "WJetsFakeValidationMod_MC", "hLeptonPtMin", ptbins);
  drawStackedPlot(Stack, "Lepton2Pt_Reco_TrackerMuon_Madgraph_Jet50",legendLabels, false, 45, -99, -99, 0.65, 0.65, 0.95, 0.90, Hist, "MC Simulation");
  
  Stack = addAllSamplesStacked(DatasetFiles, DatasetNames, "WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15", "hLeptonPtMin", ptbins, "lepton2 p_{T} [GeV/c]", "number of events in 200pb^{ -1}");
  Hist = addAllSamples(datasetfiles, datasetnames, "WJetsFakeValidationMod_MC", "hLeptonPtMin", ptbins);
  drawStackedPlot(Stack, "Lepton2Pt_Reco_TrackerMuon_Madgraph_Photon15",legendLabels, false, 45, -99, -99, 0.65, 0.65, 0.95, 0.90, Hist, "MC Simulation");

  //*****************************************************************************************
  //Met
  //*****************************************************************************************
  Stack = addAllSamplesStacked(DatasetFiles, DatasetNames, "WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50", "hMetPtHist", metbins, "Met [GeV/c]" , "number of events in 200pb^{-1}");  
  Hist = addAllSamples(datasetfiles, datasetnames, "WJetsFakeValidationMod_MC", "hMetPtHist", metbins);
  drawStackedPlot(Stack, "Met_Reco_TrackerMuon_Madgraph_Jet50",legendLabels, false, 35, -99, -99, 0.65, 0.70, 0.95, 0.95, Hist, "MC Simulation");
  
  Stack = addAllSamplesStacked(DatasetFiles, DatasetNames, "WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15", "hMetPtHist", metbins, "Met [GeV/c]", "Number of Events in 200pb^{-1}");
  Hist = addAllSamples(datasetfiles, datasetnames, "WJetsFakeValidationMod_MC", "hMetPtHist", metbins);
  drawStackedPlot(Stack, "Met_Reco_TrackerMuon_Madgraph_Photon15",legendLabels, false, 35, -99, -99, 0.65, 0.70, 0.95, 0.95, Hist, "MC Simulation");

  //*****************************************************************************************
  //UncorrMet
  //*****************************************************************************************
  Stack = addAllSamplesStacked(DatasetFiles, DatasetNames, "WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50", "hUncorrMetPtHist", metbins, "Met [GeV/c]", "number of events in 200pb^{-1}");  
  Hist = addAllSamples(datasetfiles, datasetnames, "WJetsFakeValidationMod_MC", "hUncorrMetPtHist", metbins);
  drawStackedPlot(Stack, "UncorrMet_Reco_TrackerMuon_Madgraph_Jet50",legendLabels, false, 35, -99, -99, 0.60, 0.70, 0.90, 0.95, Hist, "MC Simulation");
  
  Stack = addAllSamplesStacked(DatasetFiles, DatasetNames, "WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15", "hUncorrMetPtHist", metbins, "Met [GeV/c]", "number of events in 200pb^{-1}");
  Hist = addAllSamples(datasetfiles, datasetnames, "WJetsFakeValidationMod_MC", "hUncorrMetPtHist", metbins);
  drawStackedPlot(Stack, "UncorrMet_Reco_TrackerMuon_Madgraph_Photon15",legendLabels, false, 35, -99, -99, 0.60, 0.70, 0.90, 0.95, Hist, "MC Simulation");



  //*****************************************************************************************
  //DeltaPhiLeptons
  //*****************************************************************************************
  Stack = addAllSamplesStacked(DatasetFiles, DatasetNames, "WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50", "hDeltaPhiLeptons", deltaphibins, "#Delta #phi between the two leptons", "number of events in 200pb^{-1}");  
  Hist = addAllSamples(datasetfiles, datasetnames, "WJetsFakeValidationMod_MC", "hDeltaPhiLeptons", deltaphibins);
  drawStackedPlot(Stack, "DeltaPhiLeptons_Reco_TrackerMuon_Madgraph_Jet50",legendLabels, false, 35, -99, -99, 0.65, 0.70, 0.95, 0.95, Hist, "MC Simulation");
  
  Stack = addAllSamplesStacked(DatasetFiles, DatasetNames, "WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15", "hDeltaPhiLeptons", deltaphibins, "#Delta #phi between the two leptons", "number of events in 200pb^{-1}");
  Hist = addAllSamples(datasetfiles, datasetnames, "WJetsFakeValidationMod_MC", "hDeltaPhiLeptons", deltaphibins);
  drawStackedPlot(Stack, "DeltaPhiLeptons_Reco_TrackerMuon_Madgraph_Photon15",legendLabels, false, 35, -99, -99, 0.65, 0.70, 0.95, 0.95, Hist, "MC Simulation");


  //*****************************************************************************************
  //DileptonMass
  //*****************************************************************************************
  Stack = addAllSamplesStacked(DatasetFiles, DatasetNames, "WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50", "hDileptonMass", dileptonmassbins, "M_{ll}", "number of events in 200pb^{-1}");  
  Hist = addAllSamples(datasetfiles, datasetnames, "WJetsFakeValidationMod_MC", "hDileptonMass", dileptonmassbins);
  drawStackedPlot(Stack, "DileptonMass_Reco_TrackerMuon_Madgraph_Jet50",legendLabels, false, 50, -99, -99, 0.65, 0.70, 0.95, 0.95, Hist, "MC Simulation");
  
  Stack = addAllSamplesStacked(DatasetFiles, DatasetNames, "WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15", "hDileptonMass", dileptonmassbins, "M_{ll}", "number of events in 200pb^{-1}");
  Hist = addAllSamples(datasetfiles, datasetnames, "WJetsFakeValidationMod_MC", "hDileptonMass", dileptonmassbins);
  drawStackedPlot(Stack, "DileptonMass_Reco_TrackerMuon_Madgraph_Photon15",legendLabels, false, 50, -99, -99, 0.65, 0.70, 0.95, 0.95, Hist, "MC Simulation");


  //*****************************************************************************************
  //Selection
  //*****************************************************************************************
  Stack = addAllSamplesStacked(DatasetFiles, DatasetNames, "WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50", "hHWWSelection", fakebins, "HWW selection cuts", "number of events in 200pb^{-1}");  
  Hist = addAllSamples(datasetfiles, datasetnames, "WJetsFakeValidationMod_MC", "hHWWSelection", fakebins);
  drawStackedPlot(Stack, "HWWSelection_Reco_TrackerMuon_Madgraph_Jet50",legendLabels, false, 200, -99, -99, 0.65, 0.70, 0.95, 0.95, Hist, "MC Simulation");
  drawStackedPlot(Stack, "HWWSelection_Reco_TrackerMuon_Madgraph_Jet50_logY",legendLabels, true, 200, -99, -99, 0.65, 0.70, 0.95, 0.95, Hist, "MC Simulation");
  
  Stack = addAllSamplesStacked(DatasetFiles, DatasetNames, "WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15", "hHWWSelection", fakebins, "HWW selection cuts", "number of events in 200pb^{-1}");
  Hist = addAllSamples(datasetfiles, datasetnames, "WJetsFakeValidationMod_MC", "hHWWSelection", fakebins);
  drawStackedPlot(Stack, "HWWSelection_Reco_TrackerMuon_Madgraph_Photon15",legendLabels, false, 200, -99, -99, 0.65, 0.70, 0.95, 0.95, Hist, "MC Simulation");
  drawStackedPlot(Stack, "HWWSelection_Reco_TrackerMuon_Madgraph_Photon15_logY",legendLabels, true, 200, -99, -99, 0.65, 0.70, 0.95, 0.95, Hist, "MC Simulation");



  //*****************************************************************************************
  //
  //PreSelection Variable Distributions
  //
  //*****************************************************************************************

  //*****************************************************************************************
  //Dilepton Charge MC vs QCD prediction
  //*****************************************************************************************
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  legendnames.clear();
  errorhistnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  histnames.push_back("hDileptonCharge_weighted");
  legendnames.push_back("");
  makeCrossDirComparisonPlot(datasetfiles, datasetnames, dirnames, dirlegendlabel, histnames, legendnames,fakebins, "DileptonCharge", "(real lepton charge, fake lepton charge)", "number of events / 200pb^{-1}", -3.5, 3.5, 0, 130, 0.25, 0.7, 0.75, 0.9, false);

  //*****************************************************************************************
  //Lepton1 Pt
  //*****************************************************************************************  
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  errorhistnames.clear();
  legendnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  histnames.push_back("hLeptonPtMax");
  legendnames.push_back("");
  makeCrossDirComparisonPlot(datasetfiles, datasetnames, dirnames, dirlegendlabel, histnames, legendnames, ptbins,"Lepton1Pt", "lepton 1 p_{T} [GeV/c]", "number of events / 200pb^{-1}", -99, -99, -99, -99, 0.45, 0.90, 0.75, 0.9, false);


  //*****************************************************************************************
  //Lepton2 Pt
  //*****************************************************************************************  
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  errorhistnames.clear();
  legendnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  histnames.push_back("hLeptonPtMin");
  legendnames.push_back("");
  makeCrossDirComparisonPlot(datasetfiles, datasetnames, dirnames, dirlegendlabel, histnames, legendnames, ptbins,"Lepton2Pt", "lepton 2 p_{T} [GeV/c]", "number of events / 200pb^{-1}", -99, -99, -99, -99, 0.45, 0.90, 0.75, 0.9, false);


  //*****************************************************************************************
  //Met
  //*****************************************************************************************  
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  errorhistnames.clear();
  legendnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  histnames.push_back("hMetPtHist");
  legendnames.push_back("");
  makeCrossDirComparisonPlot(datasetfiles, datasetnames, dirnames, dirlegendlabel, histnames, legendnames, ptbins, "Met", "missing transverse energy [GeV]", "number of events / 200pb^{-1}", -99, -99, -99, 40, 0.45, 0.90, 0.75, 0.9, false);

  //*****************************************************************************************
  //DeltaPhiLeptons
  //*****************************************************************************************  
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  errorhistnames.clear();
  legendnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  histnames.push_back("hDeltaPhiLeptons");
  legendnames.push_back("");
  makeCrossDirComparisonPlot(datasetfiles, datasetnames, dirnames, dirlegendlabel, histnames, legendnames, deltaphibins,"DeltaPhiLeptons", "#Delta #phi between the two leptons", "number of events / 200pb^{-1}", -99, -99, 0, 30, 0.25, 0.75, 0.75, 0.9, false);


  //*****************************************************************************************
  //DileptonMass
  //*****************************************************************************************  
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  errorhistnames.clear();
  legendnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  histnames.push_back("hDileptonMass");
  legendnames.push_back("");
  makeCrossDirComparisonPlot(datasetfiles, datasetnames, dirnames, dirlegendlabel, histnames, legendnames, dileptonmassbins,"DileptonMass", "dilepton mass [GeV/c^{2}]", "number of events / 200pb^{-1}", -99, -99, 0, 50, 0.45, 0.90, 0.75, 0.9, false);


  //*****************************************************************************************
  //
  //Selection 
  //
  //*****************************************************************************************
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  errorhistnames.clear();
  legendnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  histnames.push_back("hHWWSelection");
  legendnames.push_back("");
  makeCrossDirComparisonPlot(datasetfiles, datasetnames, dirnames, dirlegendlabel, histnames, legendnames, fakebins,"HWWSelection", "HWW selection cut#", "number of events / 200pb^{-1}", -99, -99, 0.01, 10000, 0.5, 0.9, 0.75, 0.9, true);


  //*****************************************************************************************
  //Met
  //*****************************************************************************************  
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  errorhistnames.clear();
  legendnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  histnames.push_back("hMetPtHist");
  errorhistnames.push_back("hMetPtHist_sysError");
  legendnames.push_back("");
  makeComparisonPlotWithSystematics(datasetfiles, datasetnames, dirnames, dirlegendlabel, histnames, errorhistnames, legendnames, ptbins, "Met_withSysError", "missing transverse energy [GeV]", "number of events / 200pb^{-1}", -99, -99, -99, 35, 0.5, 0.92, 0.75, 0.9, false);

  //*****************************************************************************************
  //DeltaPhiLeptons
  //*****************************************************************************************  
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  errorhistnames.clear();
  legendnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  histnames.push_back("hDeltaPhiLeptons");
  errorhistnames.push_back("hDeltaPhiLeptons_sysError");
  legendnames.push_back("");
  makeComparisonPlotWithSystematics(datasetfiles, datasetnames, dirnames, dirlegendlabel, histnames, errorhistnames, legendnames, deltaphibins,"DeltaPhiLeptons_withSysError", "#Delta #phi between the two leptons", "number of events / 200pb^{-1}", -99, -99, 0, 30, 0.25, 0.75, 0.75, 0.9, false);

  //*****************************************************************************************
  //DileptonMass
  //*****************************************************************************************  
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  errorhistnames.clear();
  legendnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  histnames.push_back("hDileptonMass");
  errorhistnames.push_back("hDileptonMass_sysError");
  legendnames.push_back("");
  makeComparisonPlotWithSystematics(datasetfiles, datasetnames, dirnames, dirlegendlabel, histnames, errorhistnames, legendnames, dileptonmassbins,"DileptonMass_withSysError", "dilepton mass [GeV/c^{2}]", "number of events / 200pb^{-1}", -99, -99, 0, 50, 0.5, 0.92, 0.75, 0.9, false);


  //*****************************************************************************************
  //
  //Selection 
  //
  //*****************************************************************************************
  dirnames.clear();
  dirlegendlabel.clear();
  histnames.clear();
  errorhistnames.clear();
  legendnames.clear();
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Jet50");
  dirlegendlabel.push_back("JetSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_Reco_TrackerMuon_Madgraph_Photon15");
  dirlegendlabel.push_back("PhotonSample FakeRate Prediction");
  dirnames.push_back("WJetsFakeValidationMod_MC");
  dirlegendlabel.push_back("Simulation");
  histnames.push_back("hHWWSelection");
  errorhistnames.push_back("hHWWSelection_sysError");
  legendnames.push_back("");
  makeComparisonPlotWithSystematics(datasetfiles, datasetnames, dirnames, dirlegendlabel, histnames, errorhistnames, legendnames, fakebins,"HWWSelection_withSysError_logY", "HWW selection cut #", "number of events / 200pb^{-1}", -99, -99, 0.01, 10000, 0.45, 0.9, 0.75, 0.9, true);
  makeComparisonPlotWithSystematics(datasetfiles, datasetnames, dirnames, dirlegendlabel, histnames, errorhistnames, legendnames, fakebins,"HWWSelection_withSysError", "HWW selection cut #", "number of events / 200pb^{-1}", -99, -99, 0.01, 200, 0.45, 0.9, 0.75, 0.9, false);




}







//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void makePlotsForNote() {

  //*****************************************************************************************
  //Load MIT Style
  //*****************************************************************************************
  gROOT->Macro("$CMSSW_BASE/src/MitStyle.C+");


  //*****************************************************************************************
  //Input Histograms for ComputeElectronFakeRate
  //*****************************************************************************************
  vector<string> madgraphWJetsComputeElectronFakeRateFiles;
  vector<string> madgraphWJetsComputeElectronFakeRateDatasetNames;
  vector<string> madgraphWLightJetsComputeElectronFakeRateFiles;
  vector<string> madgraphWLightJetsComputeElectronFakeRateDatasetNames;
  vector<string> madgraphVQQComputeElectronFakeRateFiles;
  vector<string> madgraphVQQComputeElectronFakeRateDatasetNames;
  vector<string> pythiaWJetsComputeElectronFakeRateFiles;
  vector<string> pythiaWJetsComputeElectronFakeRateDatasetNames;
  vector<string> pythiaWeComputeElectronFakeRateFiles;
  vector<string> pythiaWeComputeElectronFakeRateDatasetNames;
  vector<string> pythiaWmComputeElectronFakeRateFiles;
  vector<string> pythiaWmComputeElectronFakeRateDatasetNames;
  vector<string> pythiaWtComputeElectronFakeRateFiles;
  vector<string> pythiaWtComputeElectronFakeRateDatasetNames;

  vector<string> madgraphQCDComputeElectronFakeRateFiles;
  vector<string> madgraphQCDComputeElectronFakeRateDatasetNames;
  vector<string> madgraphQCD100To250ComputeElectronFakeRateFiles;
  vector<string> madgraphQCD100To250ComputeElectronFakeRateDatasetNames;
  vector<string> madgraphQCD250To500ComputeElectronFakeRateFiles;
  vector<string> madgraphQCD250To500ComputeElectronFakeRateDatasetNames;
  vector<string> madgraphQCD500To1000ComputeElectronFakeRateFiles;
  vector<string> madgraphQCD500To1000ComputeElectronFakeRateDatasetNames;
  vector<string> madgraphQCD1000ToInfComputeElectronFakeRateFiles;
  vector<string> madgraphQCD1000ToInfComputeElectronFakeRateDatasetNames;

//   vector<string> pythiaQCDEnrichedComputeElectronFakeRateFiles;
//   vector<string> pythiaQCDEnrichedComputeElectronFakeRateDatasetNames;
//   vector<string> pythiaQCDEMEnrichedComputeElectronFakeRateFiles;
//   vector<string> pythiaQCDEMEnrichedComputeElectronFakeRateDatasetNames;
//   vector<string> pythiaQCDBCEEnrichedComputeElectronFakeRateFiles;
//   vector<string> pythiaQCDBCEEnrichedComputeElectronFakeRateDatasetNames;
//   vector<string> pythiaQCDEMEnriched20To30ComputeElectronFakeRateFiles;
//   vector<string> pythiaQCDEMEnriched20To30ComputeElectronFakeRateDatasetNames;
//   vector<string> pythiaQCDEMEnriched30To80ComputeElectronFakeRateFiles;
//   vector<string> pythiaQCDEMEnriched30To80ComputeElectronFakeRateDatasetNames;
//   vector<string> pythiaQCDEMEnriched80To170ComputeElectronFakeRateFiles;
//   vector<string> pythiaQCDEMEnriched80To170ComputeElectronFakeRateDatasetNames;
//   vector<string> pythiaQCDBCEEnriched20To30ComputeElectronFakeRateFiles;
//   vector<string> pythiaQCDBCEEnriched20To30ComputeElectronFakeRateDatasetNames;
//   vector<string> pythiaQCDBCEEnriched30To80ComputeElectronFakeRateFiles;
//   vector<string> pythiaQCDBCEEnriched30To80ComputeElectronFakeRateDatasetNames;
//   vector<string> pythiaQCDBCEEnriched80To170ComputeElectronFakeRateFiles;
//   vector<string> pythiaQCDBCEEnriched80To170ComputeElectronFakeRateDatasetNames;

  vector<string> madgraphPhotonJetsComputeElectronFakeRateFiles;
  vector<string> madgraphPhotonJetsComputeElectronFakeRateDatasetNames;

  vector<vector<string> > madgraphJetTriggerComputeElectronFakeRateFiles;
  vector<vector<string> > madgraphJetTriggerComputeElectronFakeRateDatasetNames;
  vector<string> madgraphJetTriggerComputeElectronFakeRateDatasetLegendLabels;
  vector<vector<string> > madgraphPhotonTriggerComputeElectronFakeRateFiles;
  vector<vector<string> > madgraphPhotonTriggerComputeElectronFakeRateDatasetNames;
  vector<string> madgraphPhotonTriggerComputeElectronFakeRateDatasetLegendLabels;


  vector<string> pythiaQCDComputeElectronFakeRateFiles;
  vector<string> pythiaQCDComputeElectronFakeRateDatasetNames;
  vector<string> pythiaPhotonJetsComputeElectronFakeRateFiles;
  vector<string> pythiaPhotonJetsComputeElectronFakeRateDatasetNames;

  vector<vector<string> > pythiaJetTriggerComputeElectronFakeRateFiles;
  vector<vector<string> > pythiaJetTriggerComputeElectronFakeRateDatasetNames;
  vector<string> pythiaJetTriggerComputeElectronFakeRateDatasetLegendLabels;
  vector<vector<string> > pythiaPhotonTriggerComputeElectronFakeRateFiles;
  vector<vector<string> > pythiaPhotonTriggerComputeElectronFakeRateDatasetNames;
  vector<string> pythiaPhotonTriggerComputeElectronFakeRateDatasetLegendLabels;
  vector<vector<string> > pythiaJetTriggerSeparateComputeElectronFakeRateFiles;
  vector<vector<string> > pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames;
  vector<string> pythiaJetTriggerSeparateComputeElectronFakeRateDatasetLegendLabels;
  vector<vector<string> > pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles;
  vector<vector<string> > pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames;
  vector<string> pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetLegendLabels;

  //*****************************************************************************************
  //Input Histograms for ComputeMuonFakeRate
  //*****************************************************************************************

  vector<string> madgraphWJetsComputeMuonFakeRateFiles;
  vector<string> madgraphWJetsComputeMuonFakeRateDatasetNames;
  vector<string> madgraphWLightJetsComputeMuonFakeRateFiles;
  vector<string> madgraphWLightJetsComputeMuonFakeRateDatasetNames;
  vector<string> madgraphVQQComputeMuonFakeRateFiles;
  vector<string> madgraphVQQComputeMuonFakeRateDatasetNames;
  vector<string> pythiaWJetsComputeMuonFakeRateFiles;
  vector<string> pythiaWJetsComputeMuonFakeRateDatasetNames;
  vector<string> pythiaWeComputeMuonFakeRateFiles;
  vector<string> pythiaWeComputeMuonFakeRateDatasetNames;
  vector<string> pythiaWmComputeMuonFakeRateFiles;
  vector<string> pythiaWmComputeMuonFakeRateDatasetNames;
  vector<string> pythiaWtComputeMuonFakeRateFiles;
  vector<string> pythiaWtComputeMuonFakeRateDatasetNames;

  vector<string> madgraphQCDComputeMuonFakeRateFiles;
  vector<string> madgraphQCDComputeMuonFakeRateDatasetNames;
  vector<string> madgraphQCD100To250ComputeMuonFakeRateFiles;
  vector<string> madgraphQCD100To250ComputeMuonFakeRateDatasetNames;
  vector<string> madgraphQCD250To500ComputeMuonFakeRateFiles;
  vector<string> madgraphQCD250To500ComputeMuonFakeRateDatasetNames;
  vector<string> madgraphQCD500To1000ComputeMuonFakeRateFiles;
  vector<string> madgraphQCD500To1000ComputeMuonFakeRateDatasetNames;
  vector<string> madgraphQCD1000ToInfComputeMuonFakeRateFiles;
  vector<string> madgraphQCD1000ToInfComputeMuonFakeRateDatasetNames;

  vector<string> madgraphPhotonJetsComputeMuonFakeRateFiles;
  vector<string> madgraphPhotonJetsComputeMuonFakeRateDatasetNames;

  vector<vector<string> > madgraphJetTriggerComputeMuonFakeRateFiles;
  vector<vector<string> > madgraphJetTriggerComputeMuonFakeRateDatasetNames;
  vector<string> madgraphJetTriggerComputeMuonFakeRateDatasetLegendLabels;
  vector<vector<string> > madgraphPhotonTriggerComputeMuonFakeRateFiles;
  vector<vector<string> > madgraphPhotonTriggerComputeMuonFakeRateDatasetNames;
  vector<string> madgraphPhotonTriggerComputeMuonFakeRateDatasetLegendLabels;


  vector<string> pythiaQCDComputeMuonFakeRateFiles;
  vector<string> pythiaQCDComputeMuonFakeRateDatasetNames;
  vector<string> pythiaPhotonJetsComputeMuonFakeRateFiles;
  vector<string> pythiaPhotonJetsComputeMuonFakeRateDatasetNames;

  vector<vector<string> > pythiaJetTriggerComputeMuonFakeRateFiles;
  vector<vector<string> > pythiaJetTriggerComputeMuonFakeRateDatasetNames;
  vector<string> pythiaJetTriggerComputeMuonFakeRateDatasetLegendLabels;
  vector<vector<string> > pythiaPhotonTriggerComputeMuonFakeRateFiles;
  vector<vector<string> > pythiaPhotonTriggerComputeMuonFakeRateDatasetNames;
  vector<string> pythiaPhotonTriggerComputeMuonFakeRateDatasetLegendLabels;
  vector<vector<string> > pythiaJetTriggerSeparateComputeMuonFakeRateFiles;
  vector<vector<string> > pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames;
  vector<string> pythiaJetTriggerSeparateComputeMuonFakeRateDatasetLegendLabels;
  vector<vector<string> > pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles;
  vector<vector<string> > pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames;
  vector<string> pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetLegendLabels;


  //*****************************************************************************************
  //Input Histograms for WJets Prediction
  //*****************************************************************************************
  vector<vector<string> > madgraphPredictionFiles;
  vector<vector<string> > madgraphPredictionDatasetNames;
  vector<string> madgraphPredictionDatasetLegendLabels;
  vector<string>  madgraphWJetsPredictionFiles;
  vector<string>  madgraphWJetsPredictionDatasetNames;

  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************
  madgraphWJetsComputeElectronFakeRateFiles.clear(); 
  madgraphWJetsComputeElectronFakeRateDatasetNames.clear();
  madgraphWLightJetsComputeElectronFakeRateFiles.clear();
  madgraphWLightJetsComputeElectronFakeRateDatasetNames.clear();
  madgraphVQQComputeElectronFakeRateFiles.clear();
  madgraphVQQComputeElectronFakeRateDatasetNames.clear();
  pythiaWJetsComputeElectronFakeRateFiles.clear();
  pythiaWJetsComputeElectronFakeRateDatasetNames.clear();
  pythiaWeComputeElectronFakeRateFiles.clear();
  pythiaWeComputeElectronFakeRateDatasetNames.clear();
  pythiaWmComputeElectronFakeRateFiles.clear();
  pythiaWmComputeElectronFakeRateDatasetNames.clear();
  pythiaWtComputeElectronFakeRateFiles.clear();
  pythiaWtComputeElectronFakeRateDatasetNames.clear();

  madgraphQCDComputeElectronFakeRateFiles.clear(); 
  madgraphQCDComputeElectronFakeRateDatasetNames.clear();
  madgraphQCD100To250ComputeElectronFakeRateFiles.clear(); 
  madgraphQCD100To250ComputeElectronFakeRateDatasetNames.clear();
  madgraphQCD250To500ComputeElectronFakeRateFiles.clear(); 
  madgraphQCD250To500ComputeElectronFakeRateDatasetNames.clear();
  madgraphQCD500To1000ComputeElectronFakeRateFiles.clear(); 
  madgraphQCD500To1000ComputeElectronFakeRateDatasetNames.clear();
  madgraphQCD1000ToInfComputeElectronFakeRateFiles.clear(); 
  madgraphQCD1000ToInfComputeElectronFakeRateDatasetNames.clear();
//   pythiaQCDEnrichedComputeElectronFakeRateFiles.clear();
//   pythiaQCDEnrichedComputeElectronFakeRateDatasetNames.clear();
//   pythiaQCDEMEnrichedComputeElectronFakeRateFiles.clear();
//   pythiaQCDEMEnrichedComputeElectronFakeRateDatasetNames.clear();
//   pythiaQCDBCEEnrichedComputeElectronFakeRateFiles.clear();
//   pythiaQCDBCEEnrichedComputeElectronFakeRateDatasetNames.clear();
//   pythiaQCDEMEnriched20To30ComputeElectronFakeRateFiles.clear();
//   pythiaQCDEMEnriched20To30ComputeElectronFakeRateDatasetNames.clear();
//   pythiaQCDEMEnriched30To80ComputeElectronFakeRateFiles.clear();
//   pythiaQCDEMEnriched30To80ComputeElectronFakeRateDatasetNames.clear();
//   pythiaQCDEMEnriched80To170ComputeElectronFakeRateFiles.clear();
//   pythiaQCDEMEnriched80To170ComputeElectronFakeRateDatasetNames.clear();
//   pythiaQCDBCEEnriched20To30ComputeElectronFakeRateFiles.clear();
//   pythiaQCDBCEEnriched20To30ComputeElectronFakeRateDatasetNames.clear();
//   pythiaQCDBCEEnriched30To80ComputeElectronFakeRateFiles.clear();

  madgraphPhotonJetsComputeElectronFakeRateFiles.clear();
  madgraphPhotonJetsComputeElectronFakeRateDatasetNames.clear();

  madgraphJetTriggerComputeElectronFakeRateFiles.clear();
  madgraphJetTriggerComputeElectronFakeRateDatasetNames.clear();
  madgraphJetTriggerComputeElectronFakeRateDatasetLegendLabels.clear();
  madgraphPhotonTriggerComputeElectronFakeRateFiles.clear();
  madgraphPhotonTriggerComputeElectronFakeRateDatasetNames.clear();
  madgraphPhotonTriggerComputeElectronFakeRateDatasetLegendLabels.clear();


  pythiaQCDComputeElectronFakeRateFiles.clear(); 
  pythiaQCDComputeElectronFakeRateDatasetNames.clear();
  pythiaPhotonJetsComputeElectronFakeRateFiles.clear();
  pythiaPhotonJetsComputeElectronFakeRateDatasetNames.clear();

  pythiaJetTriggerComputeElectronFakeRateFiles.clear();
  pythiaJetTriggerComputeElectronFakeRateDatasetNames.clear();
  pythiaJetTriggerComputeElectronFakeRateDatasetLegendLabels.clear();
  pythiaPhotonTriggerComputeElectronFakeRateFiles.clear();
  pythiaPhotonTriggerComputeElectronFakeRateDatasetNames.clear();
  pythiaPhotonTriggerComputeElectronFakeRateDatasetLegendLabels.clear();

  //*****************************************************************************************

  madgraphWJetsComputeMuonFakeRateFiles.clear(); 
  madgraphWJetsComputeMuonFakeRateDatasetNames.clear();
  madgraphWLightJetsComputeMuonFakeRateFiles.clear();
  madgraphWLightJetsComputeMuonFakeRateDatasetNames.clear();
  madgraphVQQComputeMuonFakeRateFiles.clear();
  madgraphVQQComputeMuonFakeRateDatasetNames.clear();
  pythiaWJetsComputeMuonFakeRateFiles.clear();
  pythiaWJetsComputeMuonFakeRateDatasetNames.clear();
  pythiaWeComputeMuonFakeRateFiles.clear();
  pythiaWeComputeMuonFakeRateDatasetNames.clear();
  pythiaWmComputeMuonFakeRateFiles.clear();
  pythiaWmComputeMuonFakeRateDatasetNames.clear();
  pythiaWtComputeMuonFakeRateFiles.clear();
  pythiaWtComputeMuonFakeRateDatasetNames.clear();

  madgraphQCDComputeMuonFakeRateFiles.clear(); 
  madgraphQCDComputeMuonFakeRateDatasetNames.clear();
  madgraphQCD100To250ComputeMuonFakeRateFiles.clear(); 
  madgraphQCD100To250ComputeMuonFakeRateDatasetNames.clear();
  madgraphQCD250To500ComputeMuonFakeRateFiles.clear(); 
  madgraphQCD250To500ComputeMuonFakeRateDatasetNames.clear();
  madgraphQCD500To1000ComputeMuonFakeRateFiles.clear(); 
  madgraphQCD500To1000ComputeMuonFakeRateDatasetNames.clear();
  madgraphQCD1000ToInfComputeMuonFakeRateFiles.clear(); 
  madgraphQCD1000ToInfComputeMuonFakeRateDatasetNames.clear();
 
  madgraphPhotonJetsComputeMuonFakeRateFiles.clear();
  madgraphPhotonJetsComputeMuonFakeRateDatasetNames.clear();

  madgraphJetTriggerComputeMuonFakeRateFiles.clear();
  madgraphJetTriggerComputeMuonFakeRateDatasetNames.clear();
  madgraphJetTriggerComputeMuonFakeRateDatasetLegendLabels.clear();
  madgraphPhotonTriggerComputeMuonFakeRateFiles.clear();
  madgraphPhotonTriggerComputeMuonFakeRateDatasetNames.clear();
  madgraphPhotonTriggerComputeMuonFakeRateDatasetLegendLabels.clear();


  pythiaQCDComputeMuonFakeRateFiles.clear(); 
  pythiaQCDComputeMuonFakeRateDatasetNames.clear();
  pythiaPhotonJetsComputeMuonFakeRateFiles.clear();
  pythiaPhotonJetsComputeMuonFakeRateDatasetNames.clear();

  pythiaJetTriggerComputeMuonFakeRateFiles.clear();
  pythiaJetTriggerComputeMuonFakeRateDatasetNames.clear();
  pythiaJetTriggerComputeMuonFakeRateDatasetLegendLabels.clear();
  pythiaPhotonTriggerComputeMuonFakeRateFiles.clear();
  pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.clear();
  pythiaPhotonTriggerComputeMuonFakeRateDatasetLegendLabels.clear();

  //*****************************************************************************************

  madgraphPredictionFiles.clear();
  madgraphPredictionDatasetNames.clear();
  madgraphPredictionDatasetLegendLabels.clear();
  madgraphWJetsPredictionFiles.clear();
  madgraphWJetsPredictionDatasetNames.clear();

  //*****************************************************************************************
  //For Electron Fake Rate Histograms
  //*****************************************************************************************  
  //Madgraph WJets
  madgraphWJetsComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-wjets-mg-id11_noskim.root");
  madgraphWJetsComputeElectronFakeRateDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphWJetsComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-vqq-mg-id11_noskim.root");
  madgraphWJetsComputeElectronFakeRateDatasetNames.push_back("s8-vqq-mg-id11");

  madgraphWLightJetsComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-wjets-mg-id11_noskim.root");
  madgraphWLightJetsComputeElectronFakeRateDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphVQQComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-vqq-mg-id11_noskim.root");
  madgraphVQQComputeElectronFakeRateDatasetNames.push_back("s8-vqq-mg-id11");

  //Pythia WJets
  pythiaWJetsComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-we-id11_noskim.root");
  pythiaWJetsComputeElectronFakeRateDatasetNames.push_back("s8-we-id11");
  pythiaWJetsComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-wm-id11_noskim.root");
  pythiaWJetsComputeElectronFakeRateDatasetNames.push_back("s8-wm-id11");
  pythiaWJetsComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-wt-id11_noskim.root");
  pythiaWJetsComputeElectronFakeRateDatasetNames.push_back("s8-wt-id11");
  pythiaWeComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-we-id11_noskim.root");
  pythiaWeComputeElectronFakeRateDatasetNames.push_back("s8-we-id11");
  pythiaWmComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-wm-id11_noskim.root");
  pythiaWmComputeElectronFakeRateDatasetNames.push_back("s8-wm-id11");
  pythiaWtComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-wt-id11_noskim.root");
  pythiaWtComputeElectronFakeRateDatasetNames.push_back("s8-wt-id11");
  
  //Madgraph QCD
  madgraphQCDComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphQCDComputeElectronFakeRateDatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCDComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphQCDComputeElectronFakeRateDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCDComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphQCDComputeElectronFakeRateDatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphQCDComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphQCDComputeElectronFakeRateDatasetNames.push_back("f8-qcd1000_inf-id11");
  madgraphQCD100To250ComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphQCD100To250ComputeElectronFakeRateDatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCD250To500ComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphQCD250To500ComputeElectronFakeRateDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCD500To1000ComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphQCD500To1000ComputeElectronFakeRateDatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphQCD1000ToInfComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphQCD1000ToInfComputeElectronFakeRateDatasetNames.push_back("f8-qcd1000_inf-id11");

  //Madgraph PhotonJets
  madgraphPhotonJetsComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphPhotonJetsComputeElectronFakeRateDatasetNames.push_back("s8-pj200_inf-id11");
  madgraphPhotonJetsComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphPhotonJetsComputeElectronFakeRateDatasetNames.push_back("s8-pj100_200-id11");
  madgraphPhotonJetsComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphPhotonJetsComputeElectronFakeRateDatasetNames.push_back("s8-pj40_100-id11");

  
//   //Pythia QCDEM
//   pythiaQCDEnrichedComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_20_30-id11_noskim.root");
//   pythiaQCDEnrichedComputeElectronFakeRateDatasetNames.push_back("s8-qcdem_20_30-id11");
//   pythiaQCDEnrichedComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_30_80-id11_noskim.root");
//   pythiaQCDEnrichedComputeElectronFakeRateDatasetNames.push_back("s8-qcdem_30_80-id11");
//   pythiaQCDEnrichedComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_80_170-id11_noskim.root");
//   pythiaQCDEnrichedComputeElectronFakeRateDatasetNames.push_back("s8-qcdem_80_170-id11");
//   pythiaQCDEnrichedComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_20_30-id11_noskim.root");
//   pythiaQCDEnrichedComputeElectronFakeRateDatasetNames.push_back("s8-qcdbce_20_30-id11");
//   pythiaQCDEnrichedComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_30_80-id11_noskim.root");
//   pythiaQCDEnrichedComputeElectronFakeRateDatasetNames.push_back("s8-qcdbce_30_80-id11");
//   pythiaQCDEnrichedComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_80_170-id11_noskim.root");
//   pythiaQCDEnrichedComputeElectronFakeRateDatasetNames.push_back("s8-qcdbce_80_170-id11");

//   pythiaQCDEMEnrichedComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_20_30-id11_noskim.root");
//   pythiaQCDEMEnrichedComputeElectronFakeRateDatasetNames.push_back("s8-qcdem_20_30-id11");
//   pythiaQCDEMEnrichedComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_30_80-id11_noskim.root");
//   pythiaQCDEMEnrichedComputeElectronFakeRateDatasetNames.push_back("s8-qcdem_30_80-id11");
//   pythiaQCDEMEnrichedComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_80_170-id11_noskim.root");
//   pythiaQCDEMEnrichedComputeElectronFakeRateDatasetNames.push_back("s8-qcdem_80_170-id11");
//   pythiaQCDBCEEnrichedComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_20_30-id11_noskim.root");
//   pythiaQCDBCEEnrichedComputeElectronFakeRateDatasetNames.push_back("s8-qcdbce_20_30-id11");
//   pythiaQCDBCEEnrichedComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_30_80-id11_noskim.root");
//   pythiaQCDBCEEnrichedComputeElectronFakeRateDatasetNames.push_back("s8-qcdbce_30_80-id11");
//   pythiaQCDBCEEnrichedComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_80_170-id11_noskim.root");
//   pythiaQCDBCEEnrichedComputeElectronFakeRateDatasetNames.push_back("s8-qcdbce_80_170-id11");
//   pythiaQCDEMEnriched20To30ComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_20_30-id11_noskim.root");
//   pythiaQCDEMEnriched20To30ComputeElectronFakeRateDatasetNames.push_back("s8-qcdem_20_30-id11");
//   pythiaQCDEMEnriched30To80ComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_30_80-id11_noskim.root");
//   pythiaQCDEMEnriched30To80ComputeElectronFakeRateDatasetNames.push_back("s8-qcdem_30_80-id11");
//   pythiaQCDEMEnriched80To170ComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_80_170-id11_noskim.root");
//   pythiaQCDEMEnriched80To170ComputeElectronFakeRateDatasetNames.push_back("s8-qcdem_80_170-id11");
//   pythiaQCDBCEEnriched20To30ComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_20_30-id11_noskim.root");
//   pythiaQCDBCEEnriched20To30ComputeElectronFakeRateDatasetNames.push_back("s8-qcdbce_20_30-id11");
//   pythiaQCDBCEEnriched30To80ComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_30_80-id11_noskim.root");
//   pythiaQCDBCEEnriched30To80ComputeElectronFakeRateDatasetNames.push_back("s8-qcdbce_30_80-id11");
//   pythiaQCDBCEEnriched80To170ComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_80_170-id11_noskim.root");
//   pythiaQCDBCEEnriched80To170ComputeElectronFakeRateDatasetNames.push_back("s8-qcdbce_80_170-id11");


  //Jet Trigger Sample
  madgraphJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-zjets-mg-id11_noskim.root");
  madgraphJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-zjets-mg-id11");
  madgraphJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("Z+Jets");
  madgraphJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-ttbar-mg-id11_noskim.root");
  madgraphJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-ttbar-mg-id11");
  madgraphJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("TTBAR");
  madgraphJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-wjets-mg-id11_noskim.root");
  madgraphJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-wjets-mg-id11");
  madgraphJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("W+Jets");
  madgraphJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj200_inf-id11");
  madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj100_200-id11");
  madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj40_100-id11");
  madgraphJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("Photon+Jets");
  madgraphJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-qcd100_250-id11");
  madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-qcd250_500-id11");
  madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-qcd500_1000-id11");
  madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-qcd1000_inf-id11");
  madgraphJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("QCD");

//   //Jet Trigger Sample with all components separate
//   madgraphJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
//   madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-zjets-mg-id11_noskim.root");
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-zjets-mg-id11");
//   madgraphJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("Z+Jets");
//   madgraphJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
//   madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-ttbar-mg-id11_noskim.root");
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-ttbar-mg-id11");
//   madgraphJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("TTBAR");
//   madgraphJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
//   madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-wjets-mg-id11_noskim.root");
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-wjets-mg-id11");
//   madgraphJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("W+Jets");
//   madgraphJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
//   madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-pj200_inf-id11_noskim.root");
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj200_inf-id11");
//   madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-pj100_200-id11_noskim.root");
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj100_200-id11");
//   madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-pj40_100-id11_noskim.root");
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj40_100-id11");
//   madgraphJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("Photon+Jets");
//   madgraphJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
//   madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-qcd100_250-id11");
//   madgraphJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("QCD100-250");
//   madgraphJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
//   madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-qcd250_500-id11");
//   madgraphJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("QCD250-500");
//   madgraphJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
//   madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-qcd500_1000-id11");
//   madgraphJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("QCD500-1000");
//   madgraphJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
//   madgraphJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
//   madgraphJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-qcd1000_inf-id11");
//   madgraphJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("QCD1000-Inf");




  //Photon Trigger Sample
  madgraphPhotonTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  madgraphPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-zjets-mg-id11_noskim.root");
  madgraphPhotonTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  madgraphPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-zjets-mg-id11");
  madgraphPhotonTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("Z+Jets");
  madgraphPhotonTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  madgraphPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_f8-ttbar-mg-id11_noskim.root");
  madgraphPhotonTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  madgraphPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-ttbar-mg-id11");
  madgraphPhotonTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("TTBAR");
  madgraphPhotonTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  madgraphPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-wjets-mg-id11_noskim.root");
  madgraphPhotonTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  madgraphPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-wjets-mg-id11");
  madgraphPhotonTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("W+Jets");
  madgraphPhotonTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  madgraphPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphPhotonTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  madgraphPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-qcd1000_inf-id11");
  madgraphPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-qcd500_1000-id11");
  madgraphPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-qcd250_500-id11");
  madgraphPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-qcd100_250-id11");
  madgraphPhotonTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("QCD");
  madgraphPhotonTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  madgraphPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphPhotonTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  madgraphPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj200_inf-id11");
  madgraphPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj100_200-id11");
  madgraphPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj40_100-id11");
  madgraphPhotonTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("Photon+Jets");




  //Pythia QCD
  pythiaQCDComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaQCDComputeElectronFakeRateDatasetNames.push_back("s8-qcd15-id11");
  pythiaQCDComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaQCDComputeElectronFakeRateDatasetNames.push_back("s8-qcd30-id11");
  pythiaQCDComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaQCDComputeElectronFakeRateDatasetNames.push_back("s8-qcd80-id11");
  pythiaQCDComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaQCDComputeElectronFakeRateDatasetNames.push_back("s8-qcd170-id11");

  //Pythia PhotonJets
   pythiaPhotonJetsComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj15-id12_noskim.root");
   pythiaPhotonJetsComputeElectronFakeRateDatasetNames.push_back("s8-pj15-id12");
  pythiaPhotonJetsComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj30-id11_noskim.root");
  pythiaPhotonJetsComputeElectronFakeRateDatasetNames.push_back("s8-pj30-id11");
   pythiaPhotonJetsComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj80-id12_noskim.root");
   pythiaPhotonJetsComputeElectronFakeRateDatasetNames.push_back("s8-pj80-id12");
  pythiaPhotonJetsComputeElectronFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj170-id11_noskim.root");
  pythiaPhotonJetsComputeElectronFakeRateDatasetNames.push_back("s8-pj170-id11");


  //Jet Trigger Sample
  pythiaJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-zjets-mg-id11_noskim.root");
  pythiaJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-zjets-mg-id11");
  pythiaJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("Z+Jets");
  pythiaJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-ttbar-mg-id11_noskim.root");
  pythiaJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-ttbar-mg-id11");
  pythiaJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("TTBAR");
  pythiaJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-wjets-mg-id11_noskim.root");
  pythiaJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-wjets-mg-id11");
  pythiaJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("W+Jets");
  pythiaJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-pj170-id11_noskim.root");
  pythiaJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj170-id11");
   pythiaJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-pj80-id12_noskim.root");
   pythiaJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj80-id12");
  pythiaJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-pj30-id11_noskim.root");
  pythiaJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj30-id11");
   pythiaJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-pj15-id12_noskim.root");
   pythiaJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj15-id12");
  pythiaJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("Photon+Jets");
  pythiaJetTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaJetTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-qcd15-id11");
  pythiaJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-qcd30-id11");
  pythiaJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-qcd80-id11");
  pythiaJetTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaJetTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-qcd170-id11");
  pythiaJetTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("QCD");

  //Photon Trigger Sample
  pythiaPhotonTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-zjets-mg-id11_noskim.root");
  pythiaPhotonTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-zjets-mg-id11");
  pythiaPhotonTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("Z+Jets");
  pythiaPhotonTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_f8-ttbar-mg-id11_noskim.root");
  pythiaPhotonTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("f8-ttbar-mg-id11");
  pythiaPhotonTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("TTBAR");
  pythiaPhotonTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-wjets-mg-id11_noskim.root");
  pythiaPhotonTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-wjets-mg-id11");
  pythiaPhotonTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("W+Jets");
  pythiaPhotonTriggerComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj170-id11_noskim.root");
  pythiaPhotonTriggerComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj170-id11");
   pythiaPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj80-id12_noskim.root");
   pythiaPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj80-id12");
  pythiaPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj30-id11_noskim.root");
  pythiaPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj30-id11");
   pythiaPhotonTriggerComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj15-id12_noskim.root");
   pythiaPhotonTriggerComputeElectronFakeRateDatasetNames.back().push_back("s8-pj15-id12");
  pythiaPhotonTriggerComputeElectronFakeRateDatasetLegendLabels.push_back("Photon+Jets");

 //Jet Trigger Sample All Separated
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-zjets-mg-id11_noskim.root");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-zjets-mg-id11");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetLegendLabels.push_back("Z+Jets");
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_f8-ttbar-mg-id11_noskim.root");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("f8-ttbar-mg-id11");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetLegendLabels.push_back("TTBAR");
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-wjets-mg-id11_noskim.root");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-wjets-mg-id11");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetLegendLabels.push_back("W+Jets");
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-pj170-id11_noskim.root");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-pj170-id11");
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-pj80-id12_noskim.root");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-pj80-id12");
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-pj30-id11_noskim.root");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-pj30-id11");
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-pj15-id12_noskim.root");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-pj15-id12");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetLegendLabels.push_back("Photon+Jets");
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-qcd15-id11");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetLegendLabels.push_back("QCD15");
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-qcd30-id11");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetLegendLabels.push_back("QCD30");
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-qcd80-id11");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetLegendLabels.push_back("QCD80");
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_JetTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-qcd170-id11");
  pythiaJetTriggerSeparateComputeElectronFakeRateDatasetLegendLabels.push_back("QCD170");

  //Photon Trigger Sample all separated
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-zjets-mg-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-zjets-mg-id11");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetLegendLabels.push_back("Z+Jets");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_f8-ttbar-mg-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("f8-ttbar-mg-id11");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetLegendLabels.push_back("TTBAR");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-wjets-mg-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-wjets-mg-id11");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetLegendLabels.push_back("W+Jets");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj170-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-pj170-id11");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetLegendLabels.push_back("Photon+Jets170");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj80-id12_noskim.root");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-pj80-id12");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetLegendLabels.push_back("Photon+Jets80");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj30-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-pj30-id11");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetLegendLabels.push_back("Photon+Jets30");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj15-id12_noskim.root");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-pj15-id12");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetLegendLabels.push_back("Photon+Jets15");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-qcd15-id11");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-qcd30-id11");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-qcd80-id11");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeElectronFakeRate_save_20090426_GoodVersion/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames.back().push_back("s8-qcd170-id11");
  pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetLegendLabels.push_back("QCD");



  //*****************************************************************************************
  //For Muon Fake Rate Histograms
  //*****************************************************************************************  
  //Madgraph WJets
  madgraphWJetsComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_s8-wjets-mg-id11_noskim.root");
  madgraphWJetsComputeMuonFakeRateDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphWJetsComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_s8-vqq-mg-id11_noskim.root");
  madgraphWJetsComputeMuonFakeRateDatasetNames.push_back("s8-vqq-mg-id11");

  madgraphWLightJetsComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_s8-wjets-mg-id11_noskim.root");
  madgraphWLightJetsComputeMuonFakeRateDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphVQQComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_s8-vqq-mg-id11_noskim.root");
  madgraphVQQComputeMuonFakeRateDatasetNames.push_back("s8-vqq-mg-id11");

  //Pythia WJets
  pythiaWJetsComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_s8-we-id11_noskim.root");
  pythiaWJetsComputeMuonFakeRateDatasetNames.push_back("s8-we-id11");
  pythiaWJetsComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_s8-wm-id11_noskim.root");
  pythiaWJetsComputeMuonFakeRateDatasetNames.push_back("s8-wm-id11");
  pythiaWJetsComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_s8-wt-id11_noskim.root");
  pythiaWJetsComputeMuonFakeRateDatasetNames.push_back("s8-wt-id11");
  pythiaWeComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_s8-we-id11_noskim.root");
  pythiaWeComputeMuonFakeRateDatasetNames.push_back("s8-we-id11");
  pythiaWmComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_s8-wm-id11_noskim.root");
  pythiaWmComputeMuonFakeRateDatasetNames.push_back("s8-wm-id11");
  pythiaWtComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_s8-wt-id11_noskim.root");
  pythiaWtComputeMuonFakeRateDatasetNames.push_back("s8-wt-id11");
  
  //Madgraph QCD
  madgraphQCDComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphQCDComputeMuonFakeRateDatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCDComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphQCDComputeMuonFakeRateDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCDComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphQCDComputeMuonFakeRateDatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphQCDComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphQCDComputeMuonFakeRateDatasetNames.push_back("f8-qcd1000_inf-id11");
  madgraphQCD100To250ComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphQCD100To250ComputeMuonFakeRateDatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCD250To500ComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphQCD250To500ComputeMuonFakeRateDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCD500To1000ComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphQCD500To1000ComputeMuonFakeRateDatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphQCD1000ToInfComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphQCD1000ToInfComputeMuonFakeRateDatasetNames.push_back("f8-qcd1000_inf-id11");

  //Madgraph PhotonJets
  madgraphPhotonJetsComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphPhotonJetsComputeMuonFakeRateDatasetNames.push_back("s8-pj200_inf-id11");
  madgraphPhotonJetsComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphPhotonJetsComputeMuonFakeRateDatasetNames.push_back("s8-pj100_200-id11");
  madgraphPhotonJetsComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphPhotonJetsComputeMuonFakeRateDatasetNames.push_back("s8-pj40_100-id11");


  //Jet Trigger Sample
  madgraphJetTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  madgraphJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-zjets-mg-id11_noskim.root");
  madgraphJetTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  madgraphJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-zjets-mg-id11");
  madgraphJetTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("Z+Jets");
  madgraphJetTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  madgraphJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_f8-ttbar-mg-id11_noskim.root");
  madgraphJetTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  madgraphJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("f8-ttbar-mg-id11");
  madgraphJetTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("TTBAR");
  madgraphJetTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  madgraphJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-wjets-mg-id11_noskim.root");
  madgraphJetTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  madgraphJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-wjets-mg-id11");
  madgraphJetTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("W+Jets");
  madgraphJetTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  madgraphJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphJetTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  madgraphJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-pj200_inf-id11");
  madgraphJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-pj100_200-id11");
  madgraphJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-pj40_100-id11");  
  madgraphJetTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("Photon+Jets");
  madgraphJetTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  madgraphJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphJetTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  madgraphJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("f8-qcd100_250-id11");
  madgraphJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("f8-qcd250_500-id11");
  madgraphJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("f8-qcd500_1000-id11");
  madgraphJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("f8-qcd1000_inf-id11");
  madgraphJetTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("QCD");





  //Photon Trigger Sample
  madgraphPhotonTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  madgraphPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-zjets-mg-id11_noskim.root");
  madgraphPhotonTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  madgraphPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-zjets-mg-id11");
  madgraphPhotonTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("Z+Jets");
  madgraphPhotonTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  madgraphPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_f8-ttbar-mg-id11_noskim.root");
  madgraphPhotonTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  madgraphPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("f8-ttbar-mg-id11");
  madgraphPhotonTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("TTBAR");
  madgraphPhotonTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  madgraphPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-wjets-mg-id11_noskim.root");
  madgraphPhotonTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  madgraphPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-wjets-mg-id11");
  madgraphPhotonTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("W+Jets");
  madgraphPhotonTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  madgraphPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphPhotonTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  madgraphPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("f8-qcd1000_inf-id11");
  madgraphPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("f8-qcd500_1000-id11");
  madgraphPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("f8-qcd250_500-id11");
  madgraphPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("f8-qcd100_250-id11");
  madgraphPhotonTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("QCD");
  madgraphPhotonTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  madgraphPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphPhotonTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  madgraphPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-pj200_inf-id11");
  madgraphPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-pj100_200-id11");
  madgraphPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-pj40_100-id11");
  madgraphPhotonTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("Photon+Jets");



  //Pythia QCD
  pythiaQCDComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaQCDComputeMuonFakeRateDatasetNames.push_back("s8-qcd15-id11");
  pythiaQCDComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaQCDComputeMuonFakeRateDatasetNames.push_back("s8-qcd30-id11");
  pythiaQCDComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaQCDComputeMuonFakeRateDatasetNames.push_back("s8-qcd80-id11");
  pythiaQCDComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaQCDComputeMuonFakeRateDatasetNames.push_back("s8-qcd170-id11");

  //Pythia PhotonJets
   pythiaPhotonJetsComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj15-id12_noskim.root");
   pythiaPhotonJetsComputeMuonFakeRateDatasetNames.push_back("s8-pj15-id12");
  pythiaPhotonJetsComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj30-id11_noskim.root");
  pythiaPhotonJetsComputeMuonFakeRateDatasetNames.push_back("s8-pj30-id11");
   pythiaPhotonJetsComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj80-id12_noskim.root");
   pythiaPhotonJetsComputeMuonFakeRateDatasetNames.push_back("s8-pj80-id12");
  pythiaPhotonJetsComputeMuonFakeRateFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj170-id11_noskim.root");
  pythiaPhotonJetsComputeMuonFakeRateDatasetNames.push_back("s8-pj170-id11");


  //Jet Trigger Sample
  pythiaJetTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-zjets-mg-id11_noskim.root");
  pythiaJetTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-zjets-mg-id11");
  pythiaJetTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("Z+Jets");
  pythiaJetTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_f8-ttbar-mg-id11_noskim.root");
  pythiaJetTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("f8-ttbar-mg-id11");
  pythiaJetTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("TTBAR");
  pythiaJetTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-wjets-mg-id11_noskim.root");
  pythiaJetTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-wjets-mg-id11");
  pythiaJetTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("W+Jets");
  pythiaJetTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-pj170-id11_noskim.root");
  pythiaJetTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-pj170-id11");
   pythiaJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-pj80-id12_noskim.root");
   pythiaJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-pj80-id12");
  pythiaJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-pj30-id11_noskim.root");
  pythiaJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-pj30-id11");  
   pythiaJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-pj15-id12_noskim.root");
   pythiaJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-pj15-id12");  
  pythiaJetTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("Photon+Jets");
  pythiaJetTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaJetTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-qcd15-id11");
  pythiaJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-qcd30-id11");
  pythiaJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-qcd80-id11");
  pythiaJetTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaJetTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-qcd170-id11");
  pythiaJetTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("QCD");





  //Photon Trigger Sample
  pythiaPhotonTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-zjets-mg-id11_noskim.root");
  pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-zjets-mg-id11");
  pythiaPhotonTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("Z+Jets");
  pythiaPhotonTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_f8-ttbar-mg-id11_noskim.root");
  pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("f8-ttbar-mg-id11");
  pythiaPhotonTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("TTBAR");
  pythiaPhotonTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-wjets-mg-id11_noskim.root");
  pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-wjets-mg-id11");
  pythiaPhotonTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("W+Jets");
  pythiaPhotonTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-qcd170-id11");
  pythiaPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-qcd80-id11");
  pythiaPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-qcd30-id11");
  pythiaPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-qcd15-id11");
  pythiaPhotonTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("QCD");
  pythiaPhotonTriggerComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj170-id11_noskim.root");
  pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-pj170-id11");
   pythiaPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj80-id12_noskim.root");
   pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-pj80-id12");
  pythiaPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj30-id11_noskim.root");
  pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-pj30-id11");
   pythiaPhotonTriggerComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj15-id12_noskim.root");
   pythiaPhotonTriggerComputeMuonFakeRateDatasetNames.back().push_back("s8-pj15-id12");
  pythiaPhotonTriggerComputeMuonFakeRateDatasetLegendLabels.push_back("Photon+Jets");


  //Jet Trigger Sample
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-zjets-mg-id11_noskim.root");
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-zjets-mg-id11");
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetLegendLabels.push_back("Z+Jets");
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_f8-ttbar-mg-id11_noskim.root");
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("f8-ttbar-mg-id11");
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetLegendLabels.push_back("TTBAR");
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-wjets-mg-id11_noskim.root");
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-wjets-mg-id11");
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetLegendLabels.push_back("W+Jets");
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-pj170-id11_noskim.root");
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-pj170-id11");
   pythiaJetTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-pj80-id12_noskim.root");
   pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-pj80-id12");
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-pj30-id11_noskim.root");
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-pj30-id11");  
   pythiaJetTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-pj15-id12_noskim.root");
   pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-pj15-id12");  
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetLegendLabels.push_back("Photon+Jets");
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-qcd15-id11");
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetLegendLabels.push_back("QCD15");
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-qcd30-id11");
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetLegendLabels.push_back("QCD30");
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-qcd80-id11");
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetLegendLabels.push_back("QCD80");
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_JetTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-qcd170-id11");
  pythiaJetTriggerSeparateComputeMuonFakeRateDatasetLegendLabels.push_back("QCD170");





  //Photon Trigger Sample
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-zjets-mg-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-zjets-mg-id11");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetLegendLabels.push_back("Z+Jets");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_f8-ttbar-mg-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("f8-ttbar-mg-id11");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetLegendLabels.push_back("TTBAR");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-wjets-mg-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-wjets-mg-id11");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetLegendLabels.push_back("W+Jets");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-qcd170-id11");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-qcd80-id11");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-qcd30-id11");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-qcd15-id11");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetLegendLabels.push_back("QCD");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj170-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-pj170-id11");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetLegendLabels.push_back("Photon+Jets170");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.push_back(vector<string>());
   pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj80-id12_noskim.root");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.push_back(vector<string>());
   pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-pj80-id12");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetLegendLabels.push_back("Photon+Jets80");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj30-id11_noskim.root");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.push_back(vector<string>());
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-pj30-id11");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetLegendLabels.push_back("Photon+Jets30");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.push_back(vector<string>());
   pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/ComputeMuonFakeRate_save_20090518/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj15-id12_noskim.root");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.push_back(vector<string>());
   pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames.back().push_back("s8-pj15-id12");
  pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetLegendLabels.push_back("Photon+Jets15");


  //*****************************************************************************************
  //For 2 lepton final state prediction Histograms
  //*****************************************************************************************  
  madgraphPredictionFiles.push_back(vector<string>());
  madgraphPredictionFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/WJetsFakeValidationStudy_save_20090426/WJetsFakeValidationStudy_All_s8-wjets-mg-id11_noskim.root");
  madgraphPredictionDatasetNames.push_back(vector<string>());
  madgraphPredictionDatasetNames.back().push_back("s8-wjets-mg-id11");
  madgraphPredictionDatasetLegendLabels.push_back("W+Jets");
  madgraphPredictionFiles.push_back(vector<string>());
  madgraphPredictionFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/WJetsFakeValidationStudy_save_20090426/WJetsFakeValidationStudy_All_s8-vqq-mg-id11_noskim.root");
  madgraphPredictionDatasetNames.push_back(vector<string>());
  madgraphPredictionDatasetNames.back().push_back("s8-vqq-mg-id11");
  madgraphPredictionDatasetLegendLabels.push_back("VQQ");
  madgraphPredictionFiles.push_back(vector<string>());
  madgraphPredictionFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/WJetsFakeValidationStudy_save_20090426/WJetsFakeValidationStudy_All_s8-zjets-mg-id11_noskim.root" );
  madgraphPredictionDatasetNames.push_back(vector<string>());
  madgraphPredictionDatasetNames.back().push_back("s8-zjets-mg-id11");
  madgraphPredictionDatasetLegendLabels.push_back("Z+Jets");
  madgraphPredictionFiles.push_back(vector<string>());
  madgraphPredictionFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/WJetsFakeValidationStudy_save_20090426/WJetsFakeValidationStudy_All_f8-ttbar-mg-id11_noskim.root");
  madgraphPredictionDatasetNames.push_back(vector<string>());
  madgraphPredictionDatasetNames.back().push_back("s8-ttbar-id11");
  madgraphPredictionDatasetLegendLabels.push_back("TTBAR");
  madgraphPredictionFiles.push_back(vector<string>());
  madgraphPredictionFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/WJetsFakeValidationStudy_save_20090426/WJetsFakeValidationStudy_All_s8-ww-id11_noskim.root");
  madgraphPredictionDatasetNames.push_back(vector<string>());
  madgraphPredictionDatasetNames.back().push_back("s8-ww2l-id11");
  madgraphPredictionDatasetLegendLabels.push_back("WW");
  madgraphPredictionFiles.push_back(vector<string>());
  madgraphPredictionFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/WJetsFakeValidationStudy_save_20090426/WJetsFakeValidationStudy_All_s8-pj40_100-id11_noskim.root");
  madgraphPredictionDatasetNames.push_back(vector<string>());
  madgraphPredictionDatasetNames.back().push_back("s8-pj40_100-id11");
  madgraphPredictionFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/WJetsFakeValidationStudy_save_20090426/WJetsFakeValidationStudy_All_s8-pj100_200-id11_noskim.root");
  madgraphPredictionDatasetNames.back().push_back("s8-pj100_200-id11");
  madgraphPredictionFiles.back().push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/WJetsFakeValidationStudy_save_20090426/WJetsFakeValidationStudy_All_s8-pj200_inf-id11_noskim.root");
  madgraphPredictionDatasetNames.back().push_back("s8-pj200_inf-id11");
  madgraphPredictionDatasetLegendLabels.push_back("Photon+Jets");


  madgraphWJetsPredictionFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/WJetsFakeValidationStudy_save_20090426/WJetsFakeValidationStudy_All_s8-wjets-mg-id11_noskim.root");
  madgraphWJetsPredictionDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphWJetsPredictionFiles.push_back("$CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/hists/WJetsFakeValidationStudy_save_20090426/WJetsFakeValidationStudy_All_s8-vqq-mg-id11_noskim.root");
  madgraphWJetsPredictionDatasetNames.push_back("s8-vqq-mg-id11");
  


  //*****************************************************************************************
  //Electrons with Pythia samples
  //*****************************************************************************************  

//    makeElectronNumeratorDenominatorPlots(pythiaJetTriggerComputeElectronFakeRateFiles,pythiaJetTriggerComputeElectronFakeRateDatasetNames, pythiaJetTriggerComputeElectronFakeRateDatasetLegendLabels, pythiaPhotonTriggerComputeElectronFakeRateFiles,pythiaPhotonTriggerComputeElectronFakeRateDatasetNames, pythiaPhotonTriggerComputeElectronFakeRateDatasetLegendLabels, "Pythia");
    makeElectronNumeratorDenominatorPlots(pythiaJetTriggerSeparateComputeElectronFakeRateFiles,pythiaJetTriggerSeparateComputeElectronFakeRateDatasetNames, pythiaJetTriggerSeparateComputeElectronFakeRateDatasetLegendLabels, pythiaPhotonTriggerSeparateComputeElectronFakeRateFiles,pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetNames, pythiaPhotonTriggerSeparateComputeElectronFakeRateDatasetLegendLabels, "PythiaSeparate");  
//   makeElectronCategorizationPlots(madgraphWJetsComputeElectronFakeRateFiles,madgraphWJetsComputeElectronFakeRateDatasetNames, pythiaQCDComputeElectronFakeRateFiles,pythiaQCDComputeElectronFakeRateDatasetNames,pythiaPhotonJetsComputeElectronFakeRateFiles, pythiaPhotonJetsComputeElectronFakeRateDatasetNames, "Pythia" );
//   makeElectronJetFlavorPlots(madgraphWJetsComputeElectronFakeRateFiles,madgraphWJetsComputeElectronFakeRateDatasetNames, pythiaQCDComputeElectronFakeRateFiles,pythiaQCDComputeElectronFakeRateDatasetNames, pythiaPhotonJetsComputeElectronFakeRateFiles, pythiaPhotonJetsComputeElectronFakeRateDatasetNames, "Pythia" );
  
  //*****************************************************************************************
  //Electrons with madgraph samples
  //*****************************************************************************************  
   make2DElectronFakeRateSlicePlots();
   make2DElectronFakeRateTables();
   make1DElectronFakeRatePlots();
   //makeElectronNumeratorDenominatorPlots(madgraphJetTriggerComputeElectronFakeRateFiles,madgraphJetTriggerComputeElectronFakeRateDatasetNames, madgraphJetTriggerComputeElectronFakeRateDatasetLegendLabels, madgraphPhotonTriggerComputeElectronFakeRateFiles,madgraphPhotonTriggerComputeElectronFakeRateDatasetNames, madgraphPhotonTriggerComputeElectronFakeRateDatasetLegendLabels, "Madgraph");
   makeElectronCategorizationPlots(madgraphWJetsComputeElectronFakeRateFiles,madgraphWJetsComputeElectronFakeRateDatasetNames, madgraphQCDComputeElectronFakeRateFiles,madgraphQCDComputeElectronFakeRateDatasetNames,madgraphPhotonJetsComputeElectronFakeRateFiles, madgraphPhotonJetsComputeElectronFakeRateDatasetNames, "Madgraph" );
   makeElectronJetFlavorPlots(madgraphWJetsComputeElectronFakeRateFiles,madgraphWJetsComputeElectronFakeRateDatasetNames, madgraphQCDComputeElectronFakeRateFiles,madgraphQCDComputeElectronFakeRateDatasetNames, madgraphPhotonJetsComputeElectronFakeRateFiles, madgraphPhotonJetsComputeElectronFakeRateDatasetNames, "Madgraph" );
  


//   //*****************************************************************************************
//   //Muons with pythia sample
//   //*****************************************************************************************  
//     makeMuonNumeratorDenominatorPlots(pythiaJetTriggerComputeMuonFakeRateFiles,pythiaJetTriggerComputeMuonFakeRateDatasetNames, pythiaJetTriggerComputeMuonFakeRateDatasetLegendLabels, pythiaPhotonTriggerComputeMuonFakeRateFiles,pythiaPhotonTriggerComputeMuonFakeRateDatasetNames, pythiaPhotonTriggerComputeMuonFakeRateDatasetLegendLabels, "Pythia");
     makeMuonNumeratorDenominatorPlots(pythiaJetTriggerSeparateComputeMuonFakeRateFiles,pythiaJetTriggerSeparateComputeMuonFakeRateDatasetNames, pythiaJetTriggerSeparateComputeMuonFakeRateDatasetLegendLabels, pythiaPhotonTriggerSeparateComputeMuonFakeRateFiles,pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetNames, pythiaPhotonTriggerSeparateComputeMuonFakeRateDatasetLegendLabels, "PythiaSeparate");
//    makeMuonCategorizationPlots(madgraphWJetsComputeMuonFakeRateFiles,madgraphWJetsComputeMuonFakeRateDatasetNames, pythiaQCDComputeMuonFakeRateFiles,pythiaQCDComputeMuonFakeRateDatasetNames,pythiaPhotonJetsComputeMuonFakeRateFiles,pythiaPhotonJetsComputeMuonFakeRateDatasetNames, "Pythia" );
//    makeMuonJetFlavorPlots(madgraphWJetsComputeMuonFakeRateFiles,madgraphWJetsComputeMuonFakeRateDatasetNames, pythiaQCDComputeMuonFakeRateFiles,pythiaQCDComputeMuonFakeRateDatasetNames,pythiaPhotonJetsComputeMuonFakeRateFiles,pythiaPhotonJetsComputeMuonFakeRateDatasetNames, "Pythia" );
  

  //*****************************************************************************************
  //Muons with madgraph sample
  //*****************************************************************************************  
    make2DMuonFakeRateSlicePlots();
    make2DMuonFakeRateTables();
    make1DMuonFakeRatePlots();
//   makeMuonNumeratorDenominatorPlots(madgraphJetTriggerComputeMuonFakeRateFiles,madgraphJetTriggerComputeMuonFakeRateDatasetNames, madgraphJetTriggerComputeMuonFakeRateDatasetLegendLabels, madgraphPhotonTriggerComputeMuonFakeRateFiles,madgraphPhotonTriggerComputeMuonFakeRateDatasetNames, madgraphPhotonTriggerComputeMuonFakeRateDatasetLegendLabels, "Madgraph");
   makeMuonCategorizationPlots(madgraphWJetsComputeMuonFakeRateFiles,madgraphWJetsComputeMuonFakeRateDatasetNames, madgraphQCDComputeMuonFakeRateFiles,madgraphQCDComputeMuonFakeRateDatasetNames,madgraphPhotonJetsComputeMuonFakeRateFiles,madgraphPhotonJetsComputeMuonFakeRateDatasetNames, "Madgraph" );
   makeMuonJetFlavorPlots(madgraphWJetsComputeMuonFakeRateFiles,madgraphWJetsComputeMuonFakeRateDatasetNames, madgraphQCDComputeMuonFakeRateFiles,madgraphQCDComputeMuonFakeRateDatasetNames,madgraphPhotonJetsComputeMuonFakeRateFiles,madgraphPhotonJetsComputeMuonFakeRateDatasetNames, "Madgraph" );
  
   
   //Prediction Sample  
  makeTwoLeptonPredictionPlots(madgraphPredictionFiles,madgraphPredictionDatasetNames, madgraphPredictionDatasetLegendLabels , madgraphWJetsPredictionFiles,madgraphWJetsPredictionDatasetNames);


  return;
}


