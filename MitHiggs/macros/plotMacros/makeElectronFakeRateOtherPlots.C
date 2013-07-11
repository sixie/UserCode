//root -l $CMSSW_BASE/src/MitHiggs/macros/plotMacros/makeElectronFakeRateOtherPlots.C+
//root -l $CMSSW_BASE/src/MitHiggs/macros/plotMacros/makeElectronFakeRateOtherPlots.C+
//root -l $CMSSW_BASE/src/MitHiggs/macros/plotMacros/makeElectronFakeRateOtherPlots.C+
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
#include <TGraphAsymmErrors.h>
#include <MitStyle.h>
#include "MitAna/Utils/interface/SimpleTable.h"
#include <vector>

using namespace std;
//using namespace mithep;
#endif

#include <vector>


Double_t LUMINOSITY = 200.0;
//cross section table
mithep::SimpleTable xstab("$CMSSW_BASE/src/MitPhysics/data/xs.dat");  

//define colors
Int_t COLORS[10] = {kRed,kBlue,kMagenta,kGreen,kCyan,kYellow,kBlack,kGray,10,11};
Int_t MARKERS[10] = {21,20,22,23,24,25,26,27,28,30};

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


  TH1F *newHist = (TH1F*)hist->Rebin(xlowedges.size()-1,hist->GetName(),xbins);

  TH1F *rebinHist = new TH1F(hist->GetName(), hist->GetTitle(), xlowedges.size() - 1, xbins);
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

//   for (UInt_t i=0; i<xlowedges.size();++i)
//     cout << xLow[i] << endl;
//   for (UInt_t i=0; i<ylowedges.size();++i)
//     cout << yLow[i] << endl;



  TH2F *rebinHist = new TH2F(hist->GetName(), hist->GetTitle(), xlowedges.size() - 1, xLow, ylowedges.size() - 1, yLow);

  //refill the histogram
  for (UInt_t i=0;int(i)<=hist->GetXaxis()->GetNbins()+1;++i) {
    for (UInt_t j=0;int(j)<=hist->GetYaxis()->GetNbins()+1;++j) {
      

      Double_t x = hist->GetXaxis()->GetBinCenter(i);
      Double_t y = hist->GetYaxis()->GetBinCenter(j);
      UInt_t xbin = 0;
      UInt_t ybin = 0;
      //cout << x << " " << y << " ";

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

      //cout << "-> " << xbin << " " << ybin << " : " << hist->GetBinContent(i,j) << " +- " << hist->GetBinError(i,j) << endl;
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

  //check errors
//   for (UInt_t binx=0; binx < xlowedges.size()+1 ;++binx) {
//     for (UInt_t biny=0; biny < ylowedges.size()+1 ;++biny) {
//       cout << binx << " " << biny << " : " << rebinHist->GetBinContent(binx,biny) << " +- " << rebinHist->GetBinError(binx,biny) << endl;
//     }
//   } 

  return rebinHist;
}



//--------------------------------------------------------------------------------------------------
// Add histograms from multiple files together weighted by cross-section
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
    double weight = CrossSection * LUMINOSITY / NEvents;
    if (i==0) {
      for (int b=0;b<=finalHist->GetNbinsX()+1;b++) {
        finalHist->SetBinContent(b,finalHist->GetBinContent(b)*weight);
        finalHist->SetBinError(b,finalHist->GetBinError(b)*weight);
      }
    } else {
      TH1F *tmpHist = getHisto(datasetFiles[i], dirName, histName);
      for (int b=0;b<=finalHist->GetNbinsX();b++) {
        tmpHist->SetBinContent(b,tmpHist->GetBinContent(b)*weight);
        tmpHist->SetBinError(b,tmpHist->GetBinError(b)*weight);
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
THStack* addAllSamplesStacked(vector<string> datasetFiles, vector<string> datasetNames,
                    string dirName, string histName) {

  assert(datasetFiles.size() > 0);
  TH1F *tmp = getHisto(datasetFiles[0], dirName, histName);
  assert(tmp);

  THStack *stackedHist = new THStack(tmp->GetName(),tmp->GetName());

  for (UInt_t i=0; i < datasetFiles.size() ;i++) {
    double CrossSection = xstab.Get(datasetNames[i].c_str());
    TH1F *tmpNEventsHist = getHisto(datasetFiles[i],"AnaFwkMod", "hDEvents");
    double NEvents = tmpNEventsHist->Integral();
    double weight = CrossSection * LUMINOSITY / NEvents;

    TH1F *tmpHist = getHisto(datasetFiles[i], dirName, histName);

    if (tmpHist) {
      tmpHist->SetTitle(datasetNames[i].c_str());

      //scale by weight
      for (int b=0;b<=tmpHist->GetNbinsX();b++) {
        tmpHist->SetBinContent(b,tmpHist->GetBinContent(b)*weight);
        tmpHist->SetBinError(b,tmpHist->GetBinError(b)*weight);
      }    
      
      //add to stack
      tmpHist->SetFillStyle(1001);
      tmpHist->SetFillColor(COLORS[i]);
      tmpHist->SetLineWidth(1);  
      stackedHist->Add(tmpHist);
    } else {
      cerr << "could not get histogram " << datasetNames[i] << "\n";
    }
    tmp = 0;
  }
  return stackedHist;
}


void saveFakeRate(TFile *file, vector<TH1F*> fakeRateHistograms, vector<TF1*> fakeRateFits, Bool_t fitFakeRate ) {

  for (UInt_t i=0; i<fakeRateHistograms.size();i++) {
    //write the histograms. delete any existing one with the same name
    if (file->FindObjectAny(fakeRateHistograms[i]->GetName())) {
      string tmpname = fakeRateHistograms[i]->GetName();        
      cout << "found and delete " << tmpname << endl;
      file->Delete((tmpname+";*").c_str());
    }
    
    cout << "save " << fakeRateHistograms[i]->GetName() << endl;
    fakeRateHistograms[i]->SetDirectory(file);
    file->WriteTObject(fakeRateHistograms[i]);
    
    //write the fit functions. delete any existing one with the same name
    if (fitFakeRate) {
      if (file->FindObjectAny(fakeRateFits[i]->GetName())) {
        string tmpname = fakeRateFits[i]->GetName();        
        cout << "found and delete " << tmpname << endl;
        file->Delete((tmpname+";*").c_str());
      }
      file->WriteTObject(fakeRateFits[i]);
    }
  }
}


void saveFakeRate(TFile *file, vector<TH2F*> fakeRateHistograms, vector<TF2*> fakeRateFits, Bool_t fitFakeRate ) {

  for (UInt_t i=0; i<fakeRateHistograms.size();i++) {
    //write the histograms. delete any existing one with the same name
    if (file->FindObjectAny(fakeRateHistograms[i]->GetName())) {
      string tmpname = fakeRateHistograms[i]->GetName();        
      //cout << "found and delete " << tmpname << endl;
      file->Delete((tmpname+";*").c_str());
    }
    
    //cout << "save " << fakeRateHistograms[i]->GetName() << endl;
    fakeRateHistograms[i]->SetDirectory(file);
    file->WriteTObject(fakeRateHistograms[i]);
    
    //write the fit functions. delete any existing one with the same name
    if (fitFakeRate) {
      if (file->FindObjectAny(fakeRateFits[i]->GetName())) {
        string tmpname = fakeRateFits[i]->GetName();        
        //cout << "found and delete " << tmpname << endl;
        file->Delete((tmpname+";*").c_str());
      }
      file->WriteTObject(fakeRateFits[i]);
    }
  }
}


//--------------------------------------------------------------------------------------------------
// make fake rate histogram from numerator and denominators
//--------------------------------------------------------------------------------------------------
// The Bayes calculation is wrong...Have to figure out how to do this myself.
//
TGraphAsymmErrors* createFakeRatePtGraph(vector<string> datasetFiles, vector<string> datasetNames, string dirName,
                                        string numeratorHistname, string denominatorHistname) {

  TH1F *denominator = addAllSamples(datasetFiles, datasetNames, dirName, denominatorHistname);
  TH1F *numerator = addAllSamples(datasetFiles, datasetNames, dirName, numeratorHistname);

  //create fake rate hist from denominator binning
  //TH1F *fakeRateHist = (TH1F*)numerator->Clone(histName.c_str());
  //fakeRateHist->GetYaxis()->SetTitle("Fake Rate");
  //divide by the denominator to get fake rate;
  //fakeRateHist->Divide(denominator);  

  vector<double> binLowEdges;
  
  double cumulativeCount = 0;
  for (int i=1; i<=numerator->GetXaxis()->GetNbins();i++) {    
    cumulativeCount += numerator->GetBinContent(i);
    if (cumulativeCount > 10) {
      binLowEdges.push_back(numerator->GetXaxis()->GetBinLowEdge(i));
      cumulativeCount = 0;
    }   
  }
  binLowEdges.push_back(numerator->GetXaxis()->GetBinUpEdge(numerator->GetXaxis()->GetNbins()));
  
  Double_t xbins[binLowEdges.size()];
  for (UInt_t i=0;i<binLowEdges.size();i++) {
    xbins[i] = binLowEdges[i];
  }
  
  numerator->Rebin(binLowEdges.size()-1,"hnumerator",xbins);
  denominator->Rebin(binLowEdges.size()-1,"hdenominator",xbins);

  //denominator->Scale(5000);
  //numerator->Scale(5000);
  //TGraphAsymmErrors* fakeRate = new TGraphAsymmErrors(numerator, denominator);
  //TGraphAsymmErrors* fakeRate = new TGraphAsymmErrors(numerator);

  
  TGraphAsymmErrors* fakeRate = new TGraphAsymmErrors(denominator);
  //fakeRate->BayesDivide(numerator,denominator);

  //for (int i=1 ; i <= fakeRateHist->GetXaxis()->GetNbins(); i++) {
    //cout << i << " : " << fakeRateHist->GetXaxis()->GetBinCenter(i) << " " << fakeRateHist->GetBinContent(i) << "  +- " << fakeRateHist->GetBinError(i) << endl;
    //if (fakeRateHist->GetBinContent(i) > 0 && fakeRateHist->GetBinError(i) < 0.01) {
      //fakeRateHist->SetBinError(i,0.01);
    //}
  //}

  return fakeRate;
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
  

//   for (int i=1; i<=numerator->GetXaxis()->GetNbins();i++) {    
//     cout << i << " : " << numerator->GetXaxis()->GetBinCenter(i) << " : " << numerator->GetBinContent(i) << endl;
//   }

//   //*****************************************************************************************
//   //Rebin to get at least 10 events per bin
//   //*****************************************************************************************
//   vector<double> binLowEdges;
//   binLowEdges.push_back(0.0);
   
//   double cumulativeCount = 0;
//   for (int i=1; i<=numerator->GetXaxis()->GetNbins();i++) {    
//     cumulativeCount += numerator->GetBinContent(i);
//     if (cumulativeCount > minEventsPerBin) {
//       binLowEdges.push_back(numerator->GetXaxis()->GetBinLowEdge(i));
//       cumulativeCount = 0;
//     }   
//   }
//   binLowEdges.push_back(numerator->GetXaxis()->GetBinUpEdge(numerator->GetXaxis()->GetNbins()));
  
//  Double_t xbins[binLowEdges.size()];
//   for (UInt_t i=0;i<binLowEdges.size();i++) {
//     xbins[i] = binLowEdges[i];
//     //cout << "bin " << i << " " << xbins[i] << endl;
//   }  
//   cout << xbins << endl;

//   TH1F *n = (TH1F*)numerator->Rebin(binLowEdges.size()-1,"hnumerator",xbins);
//   TH1F *d = (TH1F*)denominator->Rebin(binLowEdges.size()-1,"hdenominator",xbins);
  TH1F *n = rebin(numerator,ptbins);
  TH1F *d = rebin(denominator,ptbins);
//   for (int i=1; i<=n->GetXaxis()->GetNbins();i++) {    
//     cout << i << " : " << n->GetXaxis()->GetBinCenter(i) << " : " << n->GetBinContent(i) << endl;
//   }

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
// Fit histogram with function
//--------------------------------------------------------------------------------------------------
TF1* fitFakeRatePtHist(TGraphAsymmErrors *FakeRatePt) {

  TF1 *fitFunction = new TF1("fitFunction", "[0]*(1 - [3]*exp(-1*[1]*(x - [2])))",10,50);
  fitFunction->SetParameter(0,0.05);
  fitFunction->SetParameter(1,0.07);
  fitFunction->SetParameter(2,10.0);
  fitFunction->SetParameter(3,0.1);
  fitFunction->SetParLimits(2,9.5,10.5);
  
  FakeRatePt->Fit("fitFunction", "R+");
  
  return fitFunction;

}

TF1* fitFakeRatePtHist(TH1F *FakeRatePt) {

  TF1 *fitFunction = new TF1("fitFunction", "[0]*(1 - [3]*exp(-1*[1]*(x - [2])))",10,60);
  fitFunction->SetParameter(0,0.05);
  fitFunction->SetParameter(1,0.07);
  fitFunction->SetParameter(2,10.0);
  fitFunction->SetParameter(3,0.1);
  fitFunction->SetParLimits(2,9.5,10.5);
  
  FakeRatePt->Fit("fitFunction", "R+");
  
  return fitFunction;

}

TF1* fitFakeRateEtaHist(TH1F *FakeRatePt) {
  return 0;
}


TF2* fitFakeRateHist(TH2F *FakeRatePtEta) {

  TF2 *fitFunction = new TF2("fitFunction", "[0]*(1 - [3]*exp(-1*[1]*(x - [2]))) * ([4] + [5]*y + [6]*y*y) ",13,250,-3,3);
   fitFunction->SetParameter(0,0.05);
   fitFunction->SetParameter(1,0.07);
   fitFunction->SetParameter(2,10.0);
   fitFunction->SetParameter(3,0.1);
   fitFunction->SetParameter(4,0.05);
   fitFunction->SetParameter(5,0.0);
   fitFunction->SetParameter(6,0.025);
   fitFunction->SetParLimits(2,9.5,10.5);
 
   FakeRatePtEta->Fit("fitFunction", "R+");
   return fitFunction;  

}

//--------------------------------------------------------------------------------------------------
// Draw many plots on same canvas
//--------------------------------------------------------------------------------------------------
void drawPlots( vector<TH1F*> fakeRateHistograms, 
                vector<string> fakeRateLegendNames , string plotname , Bool_t normalize = false) {

  assert(fakeRateHistograms.size() == fakeRateLegendNames.size());
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  TLegend *leg1=0;
  leg1 = new TLegend(0.25,0.8,0.65,0.9);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.025);
  
  if (normalize) {
    Double_t area = fakeRateHistograms[0]->Integral();    
    cout << area << endl;
    for (UInt_t i=0; i<fakeRateHistograms.size() ; i++) {  
      if (i>0) {
        Double_t scaleFactor = 0;
        if (area > 0)
          scaleFactor = area / fakeRateHistograms[i]->Integral();
        
        cout << "scale " << scaleFactor << endl;
        for (int c=0;c<=fakeRateHistograms[i]->GetNbinsX()+1;c++) {        
          fakeRateHistograms[i]->SetBinContent(c,fakeRateHistograms[i]->GetBinContent(c)*scaleFactor);
          fakeRateHistograms[i]->SetBinError(c,fakeRateHistograms[i]->GetBinError(c)*scaleFactor);
        }      
      }
    }
  }

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
    fakeRateHistograms[i]->SetMarkerStyle(MARKERS[i]);

    leg1->AddEntry(fakeRateHistograms[i], fakeRateLegendNames[i].c_str(), "LP"); 
  
    if (i==0) { 
      fakeRateHistograms[i]->Draw();
    } else {
      fakeRateHistograms[i]->Draw("same");
    }    
  }

  leg1->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}

//--------------------------------------------------------------------------------------------------
// Draw a Stack
//--------------------------------------------------------------------------------------------------
void drawStackedPlot(THStack *stackedHist , string plotname, Bool_t logY = false) {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  TLegend *leg1=0;
  leg1 = new TLegend(0.65,0.50,0.95,0.95);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);

  for (int i=0; i < stackedHist->GetStack()->GetEntries(); ++i) {
    leg1->AddEntry(stackedHist->GetStack()->At(i),stackedHist->GetStack()->At(i)->GetTitle(), "FP"); 
  }

  stackedHist->Draw("hist");
  stackedHist->SetMinimum(0.1);
  stackedHist->GetXaxis()->SetTitle(((TH1F*)(stackedHist->GetHists()->At(0)))->GetXaxis()->GetTitle());
  stackedHist->GetYaxis()->SetTitle(((TH1F*)(stackedHist->GetHists()->At(0)))->GetYaxis()->GetTitle());

  leg1->Draw();
  if (logY) cv->SetLogy();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}



void makeEtaSliceComparisonPlots( TH2F* hist1, TH2F* hist2 , string plotname, string legend1 = "", string legend2 = "") {

  for(UInt_t j=0;int(j) <= hist1->GetYaxis()->GetNbins()+1;j++) {
    char tmp[20]; 
    sprintf(tmp, "Eta%.1f",  hist1->GetYaxis()->GetBinLowEdge(j));
    string sliceEta = tmp;
    string title = plotname+"_"+sliceEta;
    string filename = title + ".gif";

    TH1D *slice1 = hist1->ProjectionX("projectionX1" , j,j);
    TH1D *slice2 = hist2->ProjectionX("projectionX2" , j,j);

    double MAXY = slice1->GetMaximum() * 1.2;
    if (slice2->GetMaximum() > slice1->GetMaximum())
      MAXY = slice2->GetMaximum() * 1.2;
    MAXY = 0.1;

    slice1->SetMaximum(MAXY);
    slice2->SetMaximum(MAXY);
    slice1->SetMinimum(0.0);
    slice2->SetMinimum(0.0);

    TCanvas *cv = MakeCanvas("cv", title.c_str(), 800, 600);
    TLegend *leg1=0;
    leg1 = new TLegend(0.45,0.8,0.85,0.9);   
    leg1->SetBorderSize(1);
    leg1->SetTextSize(0.025);
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
    
    slice1->SetMarkerColor(COLORS[0]);
    slice2->SetMarkerColor(COLORS[1]);
    slice1->SetLineColor(kBlack);
    slice2->SetLineColor(kBlack);
   
    slice1->Draw("same");
    slice2->Draw("same");
    leg1->Draw();
    cv->SaveAs(filename.c_str());
  }
  return;
}

void makePtSliceComparisonPlots( TH2F* hist1, TH2F* hist2 , string plotname, string legend1 = "", string legend2 = "") {

  for(UInt_t i=0;int(i) <= hist1->GetXaxis()->GetNbins()+1;i++) {
    char tmp[20]; 
    sprintf(tmp, "Pt%.1f",  hist1->GetXaxis()->GetBinLowEdge(i));
    string slicePt = tmp;
    string title = plotname+"_"+slicePt;
    string filename = title + ".gif";

    //cout << "hists: " << hist1 << " " << hist2 << endl;

    TH1D *slice1 =0;
    TH1D *slice2 =0;
    
    slice1 = hist1->ProjectionY("projectionY1" , i,i);
    slice2 = hist2->ProjectionY("projectionY2" , i,i);

    double MAXY = slice1->GetMaximum() * 1.2;
    if (slice2->GetMaximum() > slice1->GetMaximum())
      MAXY = slice2->GetMaximum() * 1.2;
    MAXY = 0.1;
 
    slice1->SetMaximum(MAXY);
    slice2->SetMaximum(MAXY);
    slice1->SetMinimum(0.0);
    slice2->SetMinimum(0.0);

    TCanvas *cv = MakeCanvas("cv", title.c_str(), 800, 600);
    TLegend *leg1=0;
    leg1 = new TLegend(0.35,0.8,0.65,0.9);   
    leg1->SetBorderSize(1);
    leg1->SetTextSize(0.025);
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
    
    slice1->SetMarkerColor(COLORS[0]);
    slice2->SetMarkerColor(COLORS[1]);
    slice1->SetLineColor(kBlack);
    slice2->SetLineColor(kBlack);
    
    cout << "Slice1: " << slice1->Integral() << endl;
    cout << "Slice2: " << slice2->Integral() << endl;

    slice1->Draw("same");
    slice2->Draw("same");
    leg1->Draw();
    cv->SaveAs(filename.c_str());
  }
  return;
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
  for (UInt_t binx=1; binx <= jetFlavor->GetXaxis()->GetNbins() ;++binx) {
    jetFlavor->SetBinContent(binx,BinContent[binx-1]);
    jetFlavor->SetBinError(binx,TMath::Sqrt(BinErrorSquared[binx-1]));     
  }
  return jetFlavor;
}


void print2DHist( TH2F* fakeRateHistogram, string fakeRateLegendName) {
  double total = 0;

  cout.precision(3);
  cout << endl << fakeRateLegendName << endl;  

  cout.setf(ios_base::left);
  cout.width(8);
  cout << "Axis" << " ";
  for (UInt_t k=0; int(k)<=fakeRateHistogram->GetYaxis()->GetNbins()+1 ; ++k) { 
      cout.setf(ios_base::left);
      cout.width(8);
      cout << fakeRateHistogram->GetYaxis()->GetBinLowEdge(k) << " ";          
    }
  cout << endl;

  for (UInt_t j=0; int(j)<=fakeRateHistogram->GetXaxis()->GetNbins()+1 ; ++j) {    
    cout.setf(ios_base::left);
    cout.width(8);
    cout << fakeRateHistogram->GetXaxis()->GetBinLowEdge(j) << " ";
    for (UInt_t k=0; int(k)<=fakeRateHistogram->GetYaxis()->GetNbins()+1 ; ++k) { 
      cout.setf(ios_base::left);
      cout.width(8);
      cout << fakeRateHistogram->GetBinContent(j,k) << " ";          
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
// Do Fake Rate Computation From WJets sample
//--------------------------------------------------------------------------------------------------
void makePlots() {

  //Samples
  vector<string> madgraphWJetsDatasetFiles;
  vector<string> madgraphWJetsDatasetNames;
  vector<string> madgraphWLightJetsDatasetFiles;
  vector<string> madgraphWLightJetsDatasetNames;
  vector<string> madgraphVQQDatasetFiles;
  vector<string> madgraphVQQDatasetNames;
  vector<string> madgraphWcDatasetFiles;
  vector<string> madgraphWcDatasetNames;
  vector<string> pythiaWJetsDatasetFiles;
  vector<string> pythiaWJetsDatasetNames;
  vector<string> pythiaWeDatasetFiles;
  vector<string> pythiaWeDatasetNames;
  vector<string> pythiaWmDatasetFiles;
  vector<string> pythiaWmDatasetNames;
  vector<string> pythiaWtDatasetFiles;
  vector<string> pythiaWtDatasetNames;

  vector<string> madgraphQCDDatasetFiles;
  vector<string> madgraphQCDDatasetNames;
  vector<string> madgraphQCD100To250DatasetFiles;
  vector<string> madgraphQCD100To250DatasetNames;
  vector<string> madgraphQCD250To500DatasetFiles;
  vector<string> madgraphQCD250To500DatasetNames;
  vector<string> pythiaQCDEnrichedDatasetFiles;
  vector<string> pythiaQCDEnrichedDatasetNames;
  vector<string> pythiaInclMuonDatasetFiles;
  vector<string> pythiaInclMuonDatasetNames;

  vector<string> pythiaQCDEMEnrichedDatasetFiles;
  vector<string> pythiaQCDEMEnrichedDatasetNames;
  vector<string> pythiaQCDBCEEnrichedDatasetFiles;
  vector<string> pythiaQCDBCEEnrichedDatasetNames;
  vector<string> pythiaQCDEMEnriched20To30DatasetFiles;
  vector<string> pythiaQCDEMEnriched20To30DatasetNames;
  vector<string> pythiaQCDEMEnriched30To80DatasetFiles;
  vector<string> pythiaQCDEMEnriched30To80DatasetNames;
  vector<string> pythiaQCDEMEnriched80To170DatasetFiles;
  vector<string> pythiaQCDEMEnriched80To170DatasetNames;
  vector<string> pythiaQCDBCEEnriched20To30DatasetFiles;
  vector<string> pythiaQCDBCEEnriched20To30DatasetNames;
  vector<string> pythiaQCDBCEEnriched30To80DatasetFiles;
  vector<string> pythiaQCDBCEEnriched30To80DatasetNames;
  vector<string> pythiaQCDBCEEnriched80To170DatasetFiles;
  vector<string> pythiaQCDBCEEnriched80To170DatasetNames;

  
  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************
  madgraphWJetsDatasetFiles.clear(); 
  madgraphWJetsDatasetNames.clear();
  madgraphWLightJetsDatasetFiles.clear();
  madgraphWLightJetsDatasetNames.clear();
  madgraphVQQDatasetFiles.clear();
  madgraphVQQDatasetNames.clear();
  madgraphWcDatasetFiles.clear();
  madgraphWcDatasetNames.clear();
  pythiaWJetsDatasetFiles.clear();
  pythiaWJetsDatasetNames.clear();
  pythiaWeDatasetFiles.clear();
  pythiaWeDatasetNames.clear();
  pythiaWmDatasetFiles.clear();
  pythiaWmDatasetNames.clear();
  pythiaWtDatasetFiles.clear();
  pythiaWtDatasetNames.clear();

  madgraphQCDDatasetFiles.clear(); 
  madgraphQCDDatasetNames.clear();
  madgraphQCD100To250DatasetFiles.clear(); 
  madgraphQCD100To250DatasetNames.clear();
  madgraphQCD250To500DatasetFiles.clear(); 
  madgraphQCD250To500DatasetNames.clear();
  pythiaQCDEnrichedDatasetFiles.clear();
  pythiaQCDEnrichedDatasetNames.clear();
  pythiaInclMuonDatasetFiles.clear();
  pythiaInclMuonDatasetNames.clear();
  pythiaQCDEMEnrichedDatasetFiles.clear();
  pythiaQCDEMEnrichedDatasetNames.clear();
  pythiaQCDBCEEnrichedDatasetFiles.clear();
  pythiaQCDBCEEnrichedDatasetNames.clear();
  pythiaQCDEMEnriched20To30DatasetFiles.clear();
  pythiaQCDEMEnriched20To30DatasetNames.clear();
  pythiaQCDEMEnriched30To80DatasetFiles.clear();
  pythiaQCDEMEnriched30To80DatasetNames.clear();
  pythiaQCDEMEnriched80To170DatasetFiles.clear();
  pythiaQCDEMEnriched80To170DatasetNames.clear();
  pythiaQCDBCEEnriched20To30DatasetFiles.clear();
  pythiaQCDBCEEnriched20To30DatasetNames.clear();
  pythiaQCDBCEEnriched30To80DatasetFiles.clear();


  //Madgraph WJets
  madgraphWJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wjets-mg-id11_noskim.root");
  madgraphWJetsDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphWJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-vqq-mg-id11_noskim.root");
  madgraphWJetsDatasetNames.push_back("s8-vqq-mg-id11");

  madgraphWLightJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wjets-mg-id11_noskim.root");
  madgraphWLightJetsDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphVQQDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-vqq-mg-id11_noskim.root");
  madgraphVQQDatasetNames.push_back("s8-vqq-mg-id11");
  madgraphWcDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_f8-wc-mg-id11_noskim.root");
  madgraphWcDatasetNames.push_back("f8-wc-mg-id11");

  //Pythia WJets
  pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-we-id11_noskim.root");
  pythiaWJetsDatasetNames.push_back("s8-we-id11");
  pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wm-id11_noskim.root");
  pythiaWJetsDatasetNames.push_back("s8-wm-id11");
  pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wt-id11_noskim.root");
  pythiaWJetsDatasetNames.push_back("s8-wt-id11");
  pythiaWeDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-we-id11_noskim.root");
  pythiaWeDatasetNames.push_back("s8-we-id11");
  pythiaWmDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wm-id11_noskim.root");
  pythiaWmDatasetNames.push_back("s8-wm-id11");
  pythiaWtDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wt-id11_noskim.root");
  pythiaWtDatasetNames.push_back("s8-wt-id11");
  
  //Madgraph QCD
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_f8-qcd100_250-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_f8-qcd250_500-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCD100To250DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_f8-qcd100_250-id11_noskim.root");
  madgraphQCD100To250DatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCD250To500DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_f8-qcd250_500-id11_noskim.root");
  madgraphQCD250To500DatasetNames.push_back("f8-qcd250_500-id11");
  
  //Pythia QCDEM
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_20_30-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdem_20_30-id11");
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_30_80-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdem_30_80-id11");
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_80_170-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdem_80_170-id11");
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_20_30-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdbce_20_30-id11");
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_30_80-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdbce_30_80-id11");
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_80_170-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdbce_80_170-id11");

  pythiaQCDEMEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_20_30-id11_noskim.root");
  pythiaQCDEMEnrichedDatasetNames.push_back("s8-qcdem_20_30-id11");
  pythiaQCDEMEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_30_80-id11_noskim.root");
  pythiaQCDEMEnrichedDatasetNames.push_back("s8-qcdem_30_80-id11");
  pythiaQCDEMEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_80_170-id11_noskim.root");
  pythiaQCDEMEnrichedDatasetNames.push_back("s8-qcdem_80_170-id11");
  pythiaQCDBCEEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_20_30-id11_noskim.root");
  pythiaQCDBCEEnrichedDatasetNames.push_back("s8-qcdbce_20_30-id11");
  pythiaQCDBCEEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_30_80-id11_noskim.root");
  pythiaQCDBCEEnrichedDatasetNames.push_back("s8-qcdbce_30_80-id11");
  pythiaQCDBCEEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_80_170-id11_noskim.root");
  pythiaQCDBCEEnrichedDatasetNames.push_back("s8-qcdbce_80_170-id11");
  pythiaQCDEMEnriched20To30DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_20_30-id11_noskim.root");
  pythiaQCDEMEnriched20To30DatasetNames.push_back("s8-qcdem_20_30-id11");
  pythiaQCDEMEnriched30To80DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_30_80-id11_noskim.root");
  pythiaQCDEMEnriched30To80DatasetNames.push_back("s8-qcdem_30_80-id11");
  pythiaQCDEMEnriched80To170DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_80_170-id11_noskim.root");
  pythiaQCDEMEnriched80To170DatasetNames.push_back("s8-qcdem_80_170-id11");
  pythiaQCDBCEEnriched20To30DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_20_30-id11_noskim.root");
  pythiaQCDBCEEnriched20To30DatasetNames.push_back("s8-qcdbce_20_30-id11");
  pythiaQCDBCEEnriched30To80DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_30_80-id11_noskim.root");
  pythiaQCDBCEEnriched30To80DatasetNames.push_back("s8-qcdbce_30_80-id11");
  pythiaQCDBCEEnriched80To170DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_80_170-id11_noskim.root");
  pythiaQCDBCEEnriched80To170DatasetNames.push_back("s8-qcdbce_80_170-id11");


  //*****************************************************************************************
  //Construct the fake rate Histgrams. Fit with function.
  //*****************************************************************************************
  vector<TH1F*> Histograms; Histograms.clear();
  vector<string> LegendNames; LegendNames.clear();
  TH1F *FakeCategory = NULL;
  TH1F *JetFlavor = NULL;

//   //*****************************************************************************************
//   //GsfTrack Denominator Fake Categories Madgraph: WJets Vs QCD
//   //*****************************************************************************************
//   Histograms.clear();
//   LegendNames.clear();

//   //madgraph wjets gsf track
//   FakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorFakeCategory");
//   Histograms.push_back(FakeCategory);
//   LegendNames.push_back("Madgraph WJets");

//   //madgraph QCD gsf track
//   FakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorFakeCategory");
//   Histograms.push_back(FakeCategory);
//   LegendNames.push_back("Madgraph QCD");

//   drawPlots(Histograms,LegendNames, "GsfTrackDenominatorFakeCategory_Madgraph_WJetsVsQCD", false);

  //*****************************************************************************************
  //GsfTrack Denominator Fake Categories Enriched Vs NotEnriched
  //*****************************************************************************************
//   Histograms.clear();
//   LegendNames.clear();

//   //madgraph wjets gsf track
//   FakeCategory = addAllSamples(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorFakeCategory");
//   Histograms.push_back(FakeCategory);
//   LegendNames.push_back("Pythia QCDEnriched");

//   //madgraph QCD gsf track
//   FakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorFakeCategory");
//   Histograms.push_back(FakeCategory);
//   LegendNames.push_back("Madgraph QCD");

//   drawPlots(Histograms,LegendNames, "GsfTrackDenominatorFakeCategory_QCD_EnrichedVsNotEnriched");

//   //*****************************************************************************************
//   //GsfTrack Denominator Fake Categories WJets Pythia Vs Madgraph
//   //*****************************************************************************************
//   Histograms.clear();
//   LegendNames.clear();

//   //madgraph wjets gsf track
//   FakeCategory = addAllSamples(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorFakeCategory");
//   Histograms.push_back(FakeCategory);
//   LegendNames.push_back("Pythia WJets");

//   //madgraph QCD gsf track
//   FakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorFakeCategory");
//   Histograms.push_back(FakeCategory);
//   LegendNames.push_back("Madgraph WJets");

//   drawPlots(Histograms,LegendNames, "GsfTrackDenominatorFakeCategory_WJets_PythiaVsMadgraph");




//   //*****************************************************************************************
//   //Reco Denominator Fake Categories Madgraph: WJets Vs QCD
//   //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  FakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD gsf track
  FakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "RecoDenominatorFakeCategory_Madgraph_WJetsVsQCD", true);

//   //*****************************************************************************************
//   //Reco Denominator Fake Categories Enriched Vs NotEnriched
//   //*****************************************************************************************
//   Histograms.clear();
//   LegendNames.clear();

//   //madgraph wjets gsf track
//   FakeCategory = addAllSamples(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorFakeCategory");
//   Histograms.push_back(FakeCategory);
//   LegendNames.push_back("Pythia QCDEnriched");

//   //madgraph QCD gsf track
//   FakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorFakeCategory");
//   Histograms.push_back(FakeCategory);
//   LegendNames.push_back("Madgraph QCD");

//   drawPlots(Histograms,LegendNames, "RecoDenominatorFakeCategory_QCD_EnrichedVsNotEnriched");

//   //*****************************************************************************************
//   //Reco Denominator Fake Categories WJets Pythia Vs Madgraph
//   //*****************************************************************************************
//   Histograms.clear();
//   LegendNames.clear();

//   //madgraph wjets gsf track
//   FakeCategory = addAllSamples(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFakeCategory");
//   Histograms.push_back(FakeCategory);
//   LegendNames.push_back("Pythia WJets");

//   //madgraph QCD gsf track
//   FakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFakeCategory");
//   Histograms.push_back(FakeCategory);
//   LegendNames.push_back("Madgraph WJets");

//   drawPlots(Histograms,LegendNames, "RecoDenominatorFakeCategory_WJets_PythiaVsMadgraph");





//   //*****************************************************************************************
//   //GsfTrack Numerator Fake Categories Madgraph: WJets Vs QCD
//   //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  FakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD gsf track
  FakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorFakeCategory");
  Histograms.push_back(FakeCategory);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "ElectronNumeratorFakeCategory_Madgraph_WJetsVsQCD", false);


//   //*****************************************************************************************
//   //GsfTrack Numerator Fake Categories Enriched Vs NotEnriched
//   //*****************************************************************************************
//   Histograms.clear();
//   LegendNames.clear();

//   //madgraph wjets gsf track
//   FakeCategory = addAllSamples(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorFakeCategory");
//   Histograms.push_back(FakeCategory);
//   LegendNames.push_back("Pythia QCDEnriched");

//   //madgraph QCD gsf track
//   FakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorFakeCategory");
//   Histograms.push_back(FakeCategory);
//   LegendNames.push_back("Madgraph QCD");

//   drawPlots(Histograms,LegendNames, "ElectronNumeratorFakeCategory_QCD_EnrichedVsNotEnriched");

//   //*****************************************************************************************
//   //GsfTrack Numerator Fake Categories WJets Pythia Vs Madgraph
//   //*****************************************************************************************
//   Histograms.clear();
//   LegendNames.clear();

//   //madgraph wjets gsf track
//   FakeCategory = addAllSamples(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorFakeCategory");
//   Histograms.push_back(FakeCategory);
//   LegendNames.push_back("Pythia WJets");

//   //madgraph QCD gsf track
//   FakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorFakeCategory");
//   Histograms.push_back(FakeCategory);
//   LegendNames.push_back("Madgraph WJets");

//   drawPlots(Histograms,LegendNames, "ElectronNumeratorFakeCategory_WJets_PythiaVsMadgraph");











  //*****************************************************************************************
  //GsfTrack Denominator Fake JetFlavor Madgraph: WJets Vs QCD
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD gsf track
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "GsfTrackDenominatorJetFlavor_Madgraph_WJetsVsQCD", true);


//   //*****************************************************************************************
//   //GsfTrack Denominator Fake JetFlavor Enriched Vs NotEnriched
//   //*****************************************************************************************
//   Histograms.clear();
//   LegendNames.clear();

//   //madgraph wjets gsf track
//   JetFlavor = addAllSamples(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorJetFlavor");
//   Histograms.push_back(JetFlavor);
//   LegendNames.push_back("Pythia QCDEnriched");

//   //madgraph QCD gsf track
//   JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorJetFlavor");
//   Histograms.push_back(JetFlavor);
//   LegendNames.push_back("Madgraph QCD");

//   drawPlots(Histograms,LegendNames, "GsfTrackDenominatorJetFlavor_QCD_EnrichedVsNotEnriched");

//   //*****************************************************************************************
//   //GsfTrack Denominator Fake JetFlavor WJets Pythia Vs Madgraph
//   //*****************************************************************************************
//   Histograms.clear();
//   LegendNames.clear();

//   //madgraph wjets gsf track
//   JetFlavor = addAllSamples(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorJetFlavor");
//   Histograms.push_back(JetFlavor);
//   LegendNames.push_back("Pythia WJets");

//   //madgraph QCD gsf track
//   JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorJetFlavor");
//   Histograms.push_back(JetFlavor);
//   LegendNames.push_back("Madgraph WJets");

//   drawPlots(Histograms,LegendNames, "GsfTrackDenominatorJetFlavor_WJets_PythiaVsMadgraph");




  //*****************************************************************************************
  //Reco Denominator Fake JetFlavor Madgraph: WJets Vs QCD
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD gsf track
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "RecoDenominatorJetFlavor_Madgraph_WJetsVsQCD", true);


//   //*****************************************************************************************
//   //Reco Denominator Fake JetFlavor Enriched Vs NotEnriched
//   //*****************************************************************************************
//   Histograms.clear();
//   LegendNames.clear();

//   //madgraph wjets gsf track
//   JetFlavor = addAllSamples(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorJetFlavor");
//   Histograms.push_back(JetFlavor);
//   LegendNames.push_back("Pythia QCDEnriched");

//   //madgraph QCD gsf track
//   JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorJetFlavor");
//   Histograms.push_back(JetFlavor);
//   LegendNames.push_back("Madgraph QCD");

//   drawPlots(Histograms,LegendNames, "RecoDenominatorJetFlavor_QCD_EnrichedVsNotEnriched");

//   //*****************************************************************************************
//   //Reco Denominator Fake JetFlavor WJets Pythia Vs Madgraph
//   //*****************************************************************************************
//   Histograms.clear();
//   LegendNames.clear();

//   //madgraph wjets gsf track
//   JetFlavor = addAllSamples(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorJetFlavor");
//   Histograms.push_back(JetFlavor);
//   LegendNames.push_back("Pythia WJets");

//   //madgraph QCD gsf track
//   JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorJetFlavor");
//   Histograms.push_back(JetFlavor);
//   LegendNames.push_back("Madgraph WJets");

//   drawPlots(Histograms,LegendNames, "RecoDenominatorJetFlavor_WJets_PythiaVsMadgraph");





  //*****************************************************************************************
  //Numerator Fake JetFlavor Madgraph: WJets Vs QCD
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD gsf track
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorJetFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "ElectronNumeratorJetFlavor_Madgraph_WJetsVsQCD", true);


//   //*****************************************************************************************
//   //Numerator Fake JetFlavor Enriched Vs NotEnriched
//   //*****************************************************************************************
//   Histograms.clear();
//   LegendNames.clear();

//   //madgraph wjets gsf track
//   JetFlavor = addAllSamples(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorJetFlavor");
//   Histograms.push_back(JetFlavor);
//   LegendNames.push_back("Pythia QCDEnriched");

//   //madgraph QCD gsf track
//   JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorJetFlavor");
//   Histograms.push_back(JetFlavor);
//   LegendNames.push_back("Madgraph QCD");

//   drawPlots(Histograms,LegendNames, "ElectronNumeratorJetFlavor_QCD_EnrichedVsNotEnriched");

//   //*****************************************************************************************
//   //Numerator Fake JetFlavor WJets Pythia Vs Madgraph
//   //*****************************************************************************************
//   Histograms.clear();
//   LegendNames.clear();

//   //madgraph wjets gsf track
//   JetFlavor = addAllSamples(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorJetFlavor");
//   Histograms.push_back(JetFlavor);
//   LegendNames.push_back("Pythia WJets");

//   //madgraph QCD gsf track
//   JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorJetFlavor");
//   Histograms.push_back(JetFlavor);
//   LegendNames.push_back("Madgraph WJets");

//   drawPlots(Histograms,LegendNames, "ElectronNumeratorJetFlavor_WJets_PythiaVsMadgraph");













  //*****************************************************************************************
  //Numerator Fake JetFlavor Madgraph: WJets Vs QCD For Conversion Category
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorJetFlavor_Conversion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD gsf track
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorJetFlavor_Conversion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "ElectronNumeratorJetFlavor_ConversionCategory_Madgraph_WJetsVsQCD", true);

  //*****************************************************************************************
  //Denominator Fake JetFlavor Madgraph: WJets Vs QCD For Conversion Category
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorJetFlavor_Conversion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD gsf track
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorJetFlavor_Conversion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "ElectronDenominatorJetFlavor_ConversionCategory_Madgraph_WJetsVsQCD", true);

  //*****************************************************************************************
  //Numerator Fake JetFlavor Madgraph: WJets Vs QCD For ChargedPion Category
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorJetFlavor_ChargedPion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD gsf track
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorJetFlavor_ChargedPion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "ElectronNumeratorJetFlavor_ChargedPionCategory_Madgraph_WJetsVsQCD", false);

  //*****************************************************************************************
  //Denominator Fake JetFlavor Madgraph: WJets Vs QCD For ChargedPion Category
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorJetFlavor_ChargedPion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD gsf track
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorJetFlavor_ChargedPion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "ElectronDenominatorJetFlavor_ChargedPionCategory_Madgraph_WJetsVsQCD", true);











   //*****************************************************************************************
   //*****************************************************************************************
   //Fragmentation X Variable Plots
   //*****************************************************************************************
   //*****************************************************************************************


  //*****************************************************************************************
  //Numerator FragmentationX
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorFragmentationX");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD gsf track
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorFragmentationX");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "ElectronNumeratorFragmentationX_WJetsVsQCD", false);


  //*****************************************************************************************
  //Denominator FragmentationX
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFragmentationX");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD gsf track
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorFragmentationX");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "ElectronDenominatorFragmentationX_WJetsVsQCD", true);


  //*****************************************************************************************
  //Numerator FragmentationX for different fake categories for WJets
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorFragmentationX_Conversion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets Conversion");
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorFragmentationX_ChargedPion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets ChargedPion");
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorFragmentationX_ChargedKaon");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets ChargedKaon");
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorFragmentationX_HeavyFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets HeavyFlavor");

  //drawPlots(Histograms,LegendNames, "ElectronNumeratorFragmentationX_FakeCategories_WJets", true);


  //*****************************************************************************************
  //Numerator FragmentationX for different fake categories for QCD
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph QCD gsf track
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorFragmentationX_Conversion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD Conversion");
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorFragmentationX_ChargedPion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD ChargedPion");
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorFragmentationX_ChargedKaon");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD ChargedKaon");
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorFragmentationX_HeavyFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD HeavyFlavor");

  //drawPlots(Histograms,LegendNames, "ElectronNumeratorFragmentationX_FakeCategories_QCD", true);


  //*****************************************************************************************
  //Denominator FragmentationX for different fake categories for WJets
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFragmentationX_Conversion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets Conversion");
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFragmentationX_ChargedPion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets ChargedPion");
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFragmentationX_ChargedKaon");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets ChargedKaon");
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFragmentationX_HeavyFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph WJets HeavyFlavor");

  //drawPlots(Histograms,LegendNames, "ElectronDenominatorFragmentationX_FakeCategories_WJets", true);


  //*****************************************************************************************
  //Denominator FragmentationX for different fake categories for QCD
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph QCD gsf track
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorFragmentationX_Conversion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD Conversion");
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorFragmentationX_ChargedPion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD ChargedPion");
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorFragmentationX_ChargedKaon");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD ChargedKaon");
  JetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorFragmentationX_HeavyFlavor");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Madgraph QCD HeavyFlavor");

  //drawPlots(Histograms,LegendNames, "ElectronDenominatorFragmentationX_FakeCategories_QCD", true);


   //*****************************************************************************************
  //FragmentationX Numerator Vs Denominator for Charged Pions
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFragmentationX_ChargedPion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Denominator ChargedPion");
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorFragmentationX_ChargedPion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Numerator ChargedPion");
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFragmentationX_Conversion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Denominator Conversion");
  JetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorFragmentationX_Conversion");
  Histograms.push_back(JetFlavor);
  LegendNames.push_back("Numerator Conversion");

  //drawPlots(Histograms,LegendNames, "ElectronDenominatorFragmentationX_FakeCategories_WJets", true);



  return;

}


//--------------------------------------------------------------------------------------------------
// Do Fake Rate Computation From WJets sample
//--------------------------------------------------------------------------------------------------
void makeFakeRatePlots() {

  //Samples
  vector<string> madgraphWJetsDatasetFiles;
  vector<string> madgraphWJetsDatasetNames;
  vector<string> madgraphWLightJetsDatasetFiles;
  vector<string> madgraphWLightJetsDatasetNames;
  vector<string> madgraphVQQDatasetFiles;
  vector<string> madgraphVQQDatasetNames;
  vector<string> madgraphWcDatasetFiles;
  vector<string> madgraphWcDatasetNames;
  vector<string> pythiaWJetsDatasetFiles;
  vector<string> pythiaWJetsDatasetNames;
  vector<string> pythiaWeDatasetFiles;
  vector<string> pythiaWeDatasetNames;
  vector<string> pythiaWmDatasetFiles;
  vector<string> pythiaWmDatasetNames;
  vector<string> pythiaWtDatasetFiles;
  vector<string> pythiaWtDatasetNames;

  vector<string> madgraphQCDDatasetFiles;
  vector<string> madgraphQCDDatasetNames;
  vector<string> madgraphQCD100To250DatasetFiles;
  vector<string> madgraphQCD100To250DatasetNames;
  vector<string> madgraphQCD250To500DatasetFiles;
  vector<string> madgraphQCD250To500DatasetNames;
  vector<string> pythiaQCDEnrichedDatasetFiles;
  vector<string> pythiaQCDEnrichedDatasetNames;
  vector<string> pythiaInclMuonDatasetFiles;
  vector<string> pythiaInclMuonDatasetNames;

  vector<string> pythiaQCDEMEnrichedDatasetFiles;
  vector<string> pythiaQCDEMEnrichedDatasetNames;
  vector<string> pythiaQCDBCEEnrichedDatasetFiles;
  vector<string> pythiaQCDBCEEnrichedDatasetNames;
  vector<string> pythiaQCDEMEnriched20To30DatasetFiles;
  vector<string> pythiaQCDEMEnriched20To30DatasetNames;
  vector<string> pythiaQCDEMEnriched30To80DatasetFiles;
  vector<string> pythiaQCDEMEnriched30To80DatasetNames;
  vector<string> pythiaQCDEMEnriched80To170DatasetFiles;
  vector<string> pythiaQCDEMEnriched80To170DatasetNames;
  vector<string> pythiaQCDBCEEnriched20To30DatasetFiles;
  vector<string> pythiaQCDBCEEnriched20To30DatasetNames;
  vector<string> pythiaQCDBCEEnriched30To80DatasetFiles;
  vector<string> pythiaQCDBCEEnriched30To80DatasetNames;
  vector<string> pythiaQCDBCEEnriched80To170DatasetFiles;
  vector<string> pythiaQCDBCEEnriched80To170DatasetNames;

  
  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************
  madgraphWJetsDatasetFiles.clear(); 
  madgraphWJetsDatasetNames.clear();
  madgraphWLightJetsDatasetFiles.clear();
  madgraphWLightJetsDatasetNames.clear();
  madgraphVQQDatasetFiles.clear();
  madgraphVQQDatasetNames.clear();
  madgraphWcDatasetFiles.clear();
  madgraphWcDatasetNames.clear();
  pythiaWJetsDatasetFiles.clear();
  pythiaWJetsDatasetNames.clear();
  pythiaWeDatasetFiles.clear();
  pythiaWeDatasetNames.clear();
  pythiaWmDatasetFiles.clear();
  pythiaWmDatasetNames.clear();
  pythiaWtDatasetFiles.clear();
  pythiaWtDatasetNames.clear();

  madgraphQCDDatasetFiles.clear(); 
  madgraphQCDDatasetNames.clear();
  madgraphQCD100To250DatasetFiles.clear(); 
  madgraphQCD100To250DatasetNames.clear();
  madgraphQCD250To500DatasetFiles.clear(); 
  madgraphQCD250To500DatasetNames.clear();
  pythiaQCDEnrichedDatasetFiles.clear();
  pythiaQCDEnrichedDatasetNames.clear();
  pythiaInclMuonDatasetFiles.clear();
  pythiaInclMuonDatasetNames.clear();
  pythiaQCDEMEnrichedDatasetFiles.clear();
  pythiaQCDEMEnrichedDatasetNames.clear();
  pythiaQCDBCEEnrichedDatasetFiles.clear();
  pythiaQCDBCEEnrichedDatasetNames.clear();
  pythiaQCDEMEnriched20To30DatasetFiles.clear();
  pythiaQCDEMEnriched20To30DatasetNames.clear();
  pythiaQCDEMEnriched30To80DatasetFiles.clear();
  pythiaQCDEMEnriched30To80DatasetNames.clear();
  pythiaQCDEMEnriched80To170DatasetFiles.clear();
  pythiaQCDEMEnriched80To170DatasetNames.clear();
  pythiaQCDBCEEnriched20To30DatasetFiles.clear();
  pythiaQCDBCEEnriched20To30DatasetNames.clear();
  pythiaQCDBCEEnriched30To80DatasetFiles.clear();


  //Madgraph WJets
  madgraphWJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wjets-mg-id11_noskim.root");
  madgraphWJetsDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphWJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-vqq-mg-id11_noskim.root");
  madgraphWJetsDatasetNames.push_back("s8-vqq-mg-id11");

  madgraphWLightJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wjets-mg-id11_noskim.root");
  madgraphWLightJetsDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphVQQDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-vqq-mg-id11_noskim.root");
  madgraphVQQDatasetNames.push_back("s8-vqq-mg-id11");
  madgraphWcDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_f8-wc-mg-id11_noskim.root");
  madgraphWcDatasetNames.push_back("f8-wc-mg-id11");

  //Pythia WJets
  pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-we-id11_noskim.root");
  pythiaWJetsDatasetNames.push_back("s8-we-id11");
  pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wm-id11_noskim.root");
  pythiaWJetsDatasetNames.push_back("s8-wm-id11");
  pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wt-id11_noskim.root");
  pythiaWJetsDatasetNames.push_back("s8-wt-id11");
  pythiaWeDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-we-id11_noskim.root");
  pythiaWeDatasetNames.push_back("s8-we-id11");
  pythiaWmDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wm-id11_noskim.root");
  pythiaWmDatasetNames.push_back("s8-wm-id11");
  pythiaWtDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wt-id11_noskim.root");
  pythiaWtDatasetNames.push_back("s8-wt-id11");
  
  //Madgraph QCD
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_f8-qcd100_250-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_f8-qcd250_500-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCD100To250DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_f8-qcd100_250-id11_noskim.root");
  madgraphQCD100To250DatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCD250To500DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_f8-qcd250_500-id11_noskim.root");
  madgraphQCD250To500DatasetNames.push_back("f8-qcd250_500-id11");
  
  //Pythia QCDEM
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_20_30-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdem_20_30-id11");
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_30_80-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdem_30_80-id11");
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_80_170-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdem_80_170-id11");
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_20_30-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdbce_20_30-id11");
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_30_80-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdbce_30_80-id11");
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_80_170-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdbce_80_170-id11");

  pythiaQCDEMEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_20_30-id11_noskim.root");
  pythiaQCDEMEnrichedDatasetNames.push_back("s8-qcdem_20_30-id11");
  pythiaQCDEMEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_30_80-id11_noskim.root");
  pythiaQCDEMEnrichedDatasetNames.push_back("s8-qcdem_30_80-id11");
  pythiaQCDEMEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_80_170-id11_noskim.root");
  pythiaQCDEMEnrichedDatasetNames.push_back("s8-qcdem_80_170-id11");
  pythiaQCDBCEEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_20_30-id11_noskim.root");
  pythiaQCDBCEEnrichedDatasetNames.push_back("s8-qcdbce_20_30-id11");
  pythiaQCDBCEEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_30_80-id11_noskim.root");
  pythiaQCDBCEEnrichedDatasetNames.push_back("s8-qcdbce_30_80-id11");
  pythiaQCDBCEEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_80_170-id11_noskim.root");
  pythiaQCDBCEEnrichedDatasetNames.push_back("s8-qcdbce_80_170-id11");
  pythiaQCDEMEnriched20To30DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_20_30-id11_noskim.root");
  pythiaQCDEMEnriched20To30DatasetNames.push_back("s8-qcdem_20_30-id11");
  pythiaQCDEMEnriched30To80DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_30_80-id11_noskim.root");
  pythiaQCDEMEnriched30To80DatasetNames.push_back("s8-qcdem_30_80-id11");
  pythiaQCDEMEnriched80To170DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_80_170-id11_noskim.root");
  pythiaQCDEMEnriched80To170DatasetNames.push_back("s8-qcdem_80_170-id11");
  pythiaQCDBCEEnriched20To30DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_20_30-id11_noskim.root");
  pythiaQCDBCEEnriched20To30DatasetNames.push_back("s8-qcdbce_20_30-id11");
  pythiaQCDBCEEnriched30To80DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_30_80-id11_noskim.root");
  pythiaQCDBCEEnriched30To80DatasetNames.push_back("s8-qcdbce_30_80-id11");
  pythiaQCDBCEEnriched80To170DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdbce_80_170-id11_noskim.root");
  pythiaQCDBCEEnriched80To170DatasetNames.push_back("s8-qcdbce_80_170-id11");


  //*****************************************************************************************
  //Construct the fake rate Histgrams. Fit with function.
  //*****************************************************************************************
  vector<TH1F*> Histograms; Histograms.clear();
  vector<string> LegendNames; LegendNames.clear();
  TH1F *NumeratorFakeCategory = NULL;
  TH1F *DenominatorFakeCategory = NULL;
  TH1F *FakeRateFakeCategory = NULL;
  TH1F *NumeratorJetFlavor = NULL;
  TH1F *DenominatorJetFlavor = NULL;
  TH1F *FakeRateJetFlavor = NULL;

  //*****************************************************************************************
  //Find Fake Rate as function of Fake Category for GsfTrack Denominator
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  NumeratorFakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_MadgraphWJets");
  FakeRateFakeCategory->GetYaxis()->SetTitle("Fake Rate");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  
  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("Madgraph WJets");


//   //pythia wjets gsf track
//   NumeratorFakeCategory = addAllSamples(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorFakeCategory");
//   DenominatorFakeCategory = addAllSamples(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorFakeCategory");
//   FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_PythiaWJets");
//   FakeRateFakeCategory->GetYaxis()->SetTitle("Fake Rate");
//   FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");

//   Histograms.push_back(FakeRateFakeCategory);
//   LegendNames.push_back("Madgraph WJets");


  //madgraph QCD gsf track
  NumeratorFakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_MadgraphQCD");
  FakeRateFakeCategory->GetYaxis()->SetTitle("Fake Rate");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");

  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("Madgraph QCD");


//   //Pythia QCD Enriched
//   NumeratorFakeCategory = addAllSamples(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorFakeCategory");
//   DenominatorFakeCategory = addAllSamples(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorFakeCategory");
//   FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_MadgraphQCD");
//   FakeRateFakeCategory->GetYaxis()->SetTitle("Fake Rate");
//   FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");

//   Histograms.push_back(FakeRateFakeCategory);
//   LegendNames.push_back("Pythia QCDEnriched");

  //drawPlots(Histograms,LegendNames, "GsfTrackFakeRate_FakeCategory");


  //*****************************************************************************************
  //Find Fake Rate as function of Fake Category for Reco Denominator
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  NumeratorFakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_MadgraphWJets");
  FakeRateFakeCategory->GetYaxis()->SetTitle("Fake Rate");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");

  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("Madgraph WJets");


//   //pythia wjets gsf track
//   NumeratorFakeCategory = addAllSamples(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorFakeCategory");
//   DenominatorFakeCategory = addAllSamples(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFakeCategory");
//   FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_PythiaWJets");
//   FakeRateFakeCategory->GetYaxis()->SetTitle("Fake Rate");
//   FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");

//   Histograms.push_back(FakeRateFakeCategory);
//   LegendNames.push_back("Madgraph WJets");


  //madgraph QCD gsf track
  NumeratorFakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_MadgraphQCD");
  FakeRateFakeCategory->GetYaxis()->SetTitle("Fake Rate");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");

  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("Madgraph QCD");


//   //Pythia QCD Enriched
//   NumeratorFakeCategory = addAllSamples(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorFakeCategory");
//   DenominatorFakeCategory = addAllSamples(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorFakeCategory");
//   FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_MadgraphQCD");
//   FakeRateFakeCategory->GetYaxis()->SetTitle("Fake Rate");
//   FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");

//   Histograms.push_back(FakeRateFakeCategory);
//   LegendNames.push_back("Pythia QCDEnriched");

//  drawPlots(Histograms,LegendNames, "RecoFakeRate_FakeCategory");







  //*****************************************************************************************
  //Find Fake Rate as function of Jet Flavor for GsfTrack Denominator
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  NumeratorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorJetFlavor");
  DenominatorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphWJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph WJets");


//   //pythia wjets gsf track
//   NumeratorJetFlavor = addAllSamples(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorJetFlavor");
//   DenominatorJetFlavor = addAllSamples(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorJetFlavor");
//   FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_PythiaWJets");
//   FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
//   FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");

//   Histograms.push_back(FakeRateJetFlavor);
//   LegendNames.push_back("Madgraph WJets");


  //madgraph QCD gsf track
  NumeratorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorJetFlavor");
  DenominatorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphQCD");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph QCD");


//   //Pythia QCD Enriched
//   NumeratorJetFlavor = addAllSamples(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorJetFlavor");
//   DenominatorJetFlavor = addAllSamples(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorJetFlavor");
//   FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphQCD");
//   FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
//   FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");

//   Histograms.push_back(FakeRateJetFlavor);
//   LegendNames.push_back("Pythia QCDEnriched");

//  drawPlots(Histograms,LegendNames, "GsfTrackFakeRate_JetFlavor");


  //*****************************************************************************************
  //Find Fake Rate as function of JetFlavor for Reco Denominator
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets gsf track
  NumeratorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorJetFlavor");
  DenominatorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphWJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph WJets");


//   //pythia wjets gsf track
//   NumeratorJetFlavor = addAllSamples(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorJetFlavor");
//   DenominatorJetFlavor = addAllSamples(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorJetFlavor");
//   FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_PythiaWJets");
//   FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
//   FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");

//   Histograms.push_back(FakeRateJetFlavor);
//   LegendNames.push_back("Madgraph WJets");


  //madgraph QCD gsf track
  NumeratorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorJetFlavor");
  DenominatorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphQCD");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph QCD");


//   //Pythia QCD Enriched
//   NumeratorJetFlavor = addAllSamples(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorJetFlavor");
//   DenominatorJetFlavor = addAllSamples(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorJetFlavor");
//   FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphQCD");
//   FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
//   FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");

//   Histograms.push_back(FakeRateJetFlavor);
//   LegendNames.push_back("Pythia QCDEnriched");

  //drawPlots(Histograms,LegendNames, "RecoFakeRate_JetFlavor");











  //*****************************************************************************************
  //Find Weighted Fake Rate as function of Fake Category for Reco Denominator
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets 
  NumeratorFakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_MadgraphWJets");
  FakeRateFakeCategory->GetYaxis()->SetTitle("Fake Rate");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  
  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateFakeCategory->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorFakeCategory->Integral() > 0)
      weight = DenominatorFakeCategory->GetBinContent(i) / DenominatorFakeCategory->Integral();
    FakeRateFakeCategory->SetBinContent(i, FakeRateFakeCategory->GetBinContent(i)*weight);
    FakeRateFakeCategory->SetBinError(i, FakeRateFakeCategory->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("Madgraph WJets");


  //madgraph QCD 
  NumeratorFakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_MadgraphQCD");
  FakeRateFakeCategory->GetYaxis()->SetTitle("Fake Rate");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  
  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateFakeCategory->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorFakeCategory->Integral() > 0)
      weight = DenominatorFakeCategory->GetBinContent(i) / DenominatorFakeCategory->Integral();
    FakeRateFakeCategory->SetBinContent(i, FakeRateFakeCategory->GetBinContent(i)*weight);
    FakeRateFakeCategory->SetBinError(i, FakeRateFakeCategory->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "RecoWeightedFakeRate_FakeCategory");

  //*****************************************************************************************
  //Find Weighted Fake Rate as function of Fake Category for GsfTrack Denominator
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets 
  NumeratorFakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_MadgraphWJets");
  FakeRateFakeCategory->GetYaxis()->SetTitle("Fake Rate");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  
  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateFakeCategory->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorFakeCategory->Integral() > 0)
      weight = DenominatorFakeCategory->GetBinContent(i) / DenominatorFakeCategory->Integral();
    FakeRateFakeCategory->SetBinContent(i, FakeRateFakeCategory->GetBinContent(i)*weight);
    FakeRateFakeCategory->SetBinError(i, FakeRateFakeCategory->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("Madgraph WJets");


  //madgraph QCD 
  NumeratorFakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorFakeCategory");
  DenominatorFakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorFakeCategory");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_MadgraphQCD");
  FakeRateFakeCategory->GetYaxis()->SetTitle("Fake Rate");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  
  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateFakeCategory->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorFakeCategory->Integral() > 0)
      weight = DenominatorFakeCategory->GetBinContent(i) / DenominatorFakeCategory->Integral();
    FakeRateFakeCategory->SetBinContent(i, FakeRateFakeCategory->GetBinContent(i)*weight);
    FakeRateFakeCategory->SetBinError(i, FakeRateFakeCategory->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "GsfTrackWeightedFakeRate_FakeCategory");








  //*****************************************************************************************
  //Make JetFlavor Fake Rate plots for Conversion Category
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();


  //madgraph wjets gsf track
  NumeratorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorJetFlavor_Conversion");
  DenominatorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphWJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateJetFlavor->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorJetFlavor->Integral() > 0)
      weight = DenominatorJetFlavor->GetBinContent(i) / DenominatorJetFlavor->Integral();
    FakeRateJetFlavor->SetBinContent(i, FakeRateJetFlavor->GetBinContent(i)*weight);
    FakeRateJetFlavor->SetBinError(i, FakeRateJetFlavor->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD gsf track
  NumeratorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorJetFlavor_Conversion");
  DenominatorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphQCD");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateJetFlavor->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorJetFlavor->Integral() > 0)
      weight = DenominatorJetFlavor->GetBinContent(i) / DenominatorJetFlavor->Integral();
    FakeRateJetFlavor->SetBinContent(i, FakeRateJetFlavor->GetBinContent(i)*weight);
    FakeRateJetFlavor->SetBinError(i, FakeRateJetFlavor->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "RecoWeightedFakeRate_JetFlavor_Conversion");


  //*****************************************************************************************
  //Make JetFlavor Fake Rate plots for Charged Pion Category
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //to get fraction of charged pion category of denominator
  DenominatorFakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFakeCategory");

  //madgraph wjets 
  NumeratorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorJetFlavor_ChargedPion");
  DenominatorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphWJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");


  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateJetFlavor->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorJetFlavor->Integral() > 0)
      weight = DenominatorJetFlavor->GetBinContent(i) / DenominatorJetFlavor->Integral();
    FakeRateJetFlavor->SetBinContent(i, FakeRateJetFlavor->GetBinContent(i)*weight);
    FakeRateJetFlavor->SetBinError(i, FakeRateJetFlavor->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD 
  NumeratorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorJetFlavor_ChargedPion");
  DenominatorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphQCD");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateJetFlavor->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorJetFlavor->Integral() > 0)
      weight = DenominatorJetFlavor->GetBinContent(i) / DenominatorJetFlavor->Integral();
    FakeRateJetFlavor->SetBinContent(i, FakeRateJetFlavor->GetBinContent(i)*weight);
    FakeRateJetFlavor->SetBinError(i, FakeRateJetFlavor->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "RecoWeightedFakeRate_JetFlavor_ChargedPion");


  //*****************************************************************************************
  //Make JetFlavor Fake Rate plots for Charged Kaon Category
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //to get fraction of charged pion category of denominator
  DenominatorFakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFakeCategory");

  //madgraph wjets 
  NumeratorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorJetFlavor_ChargedKaon");
  DenominatorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphWJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");


  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateJetFlavor->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorJetFlavor->Integral() > 0)
      weight = DenominatorJetFlavor->GetBinContent(i) / DenominatorJetFlavor->Integral();
    FakeRateJetFlavor->SetBinContent(i, FakeRateJetFlavor->GetBinContent(i)*weight);
    FakeRateJetFlavor->SetBinError(i, FakeRateJetFlavor->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD 
  NumeratorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorJetFlavor_ChargedKaon");
  DenominatorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphQCD");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateJetFlavor->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorJetFlavor->Integral() > 0)
      weight = DenominatorJetFlavor->GetBinContent(i) / DenominatorJetFlavor->Integral();
    FakeRateJetFlavor->SetBinContent(i, FakeRateJetFlavor->GetBinContent(i)*weight);
    FakeRateJetFlavor->SetBinError(i, FakeRateJetFlavor->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "RecoWeightedFakeRate_JetFlavor_ChargedKaon");



  //*****************************************************************************************
  //Make JetFlavor Fake Rate plots for Heavy Flavor Category
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //to get fraction of charged pion category of denominator
  DenominatorFakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorFakeCategory");

  //madgraph wjets 
  NumeratorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorJetFlavor_HeavyFlavor");
  DenominatorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphWJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");


  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateJetFlavor->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    //weight = DenominatorFakeCategory->GetBinContent(3) / DenominatorFakeCategory->Integral();
    if (DenominatorJetFlavor->Integral() > 0)
      weight = DenominatorJetFlavor->GetBinContent(i) / DenominatorJetFlavor->Integral();
    FakeRateJetFlavor->SetBinContent(i, FakeRateJetFlavor->GetBinContent(i)*weight);
    FakeRateJetFlavor->SetBinError(i, FakeRateJetFlavor->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD 
  NumeratorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorJetFlavor_HeavyFlavor");
  DenominatorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphQCD");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateJetFlavor->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    //weight = DenominatorFakeCategory->GetBinContent(3) / DenominatorFakeCategory->Integral();
    if (DenominatorJetFlavor->Integral() > 0)
      weight = DenominatorJetFlavor->GetBinContent(i) / DenominatorJetFlavor->Integral();
    FakeRateJetFlavor->SetBinContent(i, FakeRateJetFlavor->GetBinContent(i)*weight);
    FakeRateJetFlavor->SetBinError(i, FakeRateJetFlavor->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "RecoWeightedFakeRate_JetFlavor_HeavyFlavor");







  //*****************************************************************************************
  //Make JetFlavor GsfTrack Fake Rate plots for Conversion Category
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();


  //madgraph wjets gsf track
  NumeratorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorJetFlavor_Conversion");
  DenominatorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphWJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateJetFlavor->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorJetFlavor->Integral() > 0)
      weight = DenominatorJetFlavor->GetBinContent(i) / DenominatorJetFlavor->Integral();
    FakeRateJetFlavor->SetBinContent(i, FakeRateJetFlavor->GetBinContent(i)*weight);
    FakeRateJetFlavor->SetBinError(i, FakeRateJetFlavor->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph WJets");


  //madgraph QCD gsf track
  NumeratorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorJetFlavor_Conversion");
  DenominatorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphQCD");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateJetFlavor->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorJetFlavor->Integral() > 0)
      weight = DenominatorJetFlavor->GetBinContent(i) / DenominatorJetFlavor->Integral();
    FakeRateJetFlavor->SetBinContent(i, FakeRateJetFlavor->GetBinContent(i)*weight);
    FakeRateJetFlavor->SetBinError(i, FakeRateJetFlavor->GetBinError(i)*weight);
  } 

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "GsfTrackWeightedFakeRate_JetFlavor_Conversion");


  //*****************************************************************************************
  //Make JetFlavor GsfTrack  Fake Rate plots for Charged Pion Category
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets 
  NumeratorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorJetFlavor_ChargedPion");
  DenominatorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphWJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");


  //weight by denominator fraction of each bin
//   for (int i=0; i <= FakeRateJetFlavor->GetXaxis()->GetNbins()+1 ; i++) {
//     double weight = 0;
//     if (DenominatorJetFlavor->Integral() > 0)
//       weight = DenominatorJetFlavor->GetBinContent(i) / DenominatorJetFlavor->Integral();
//     FakeRateJetFlavor->SetBinContent(i, FakeRateJetFlavor->GetBinContent(i)*weight);
//     FakeRateJetFlavor->SetBinError(i, FakeRateJetFlavor->GetBinError(i)*weight);
//   }

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD 
  NumeratorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorJetFlavor_ChargedPion");
  DenominatorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphQCD");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  //weight by denominator fraction of each bin
//   for (int i=0; i <= FakeRateJetFlavor->GetXaxis()->GetNbins()+1 ; i++) {
//     double weight = 0;
//     if (DenominatorJetFlavor->Integral() > 0)
//       weight = DenominatorJetFlavor->GetBinContent(i) / DenominatorJetFlavor->Integral();
//     FakeRateJetFlavor->SetBinContent(i, FakeRateJetFlavor->GetBinContent(i)*weight);
//     FakeRateJetFlavor->SetBinError(i, FakeRateJetFlavor->GetBinError(i)*weight);
//   }

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "GsfTrackWeightedFakeRate_JetFlavor_ChargedPion");


  //*****************************************************************************************
  //Make JetFlavor GsfTrack Fake Rate plots for Charged Kaon Category
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets 
  NumeratorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorJetFlavor_ChargedKaon");
  DenominatorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphWJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");


  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateJetFlavor->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorJetFlavor->Integral() > 0)
      weight = DenominatorJetFlavor->GetBinContent(i) / DenominatorJetFlavor->Integral();
    FakeRateJetFlavor->SetBinContent(i, FakeRateJetFlavor->GetBinContent(i)*weight);
    FakeRateJetFlavor->SetBinError(i, FakeRateJetFlavor->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD 
  NumeratorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorJetFlavor_ChargedKaon");
  DenominatorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphQCD");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateJetFlavor->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorJetFlavor->Integral() > 0)
      weight = DenominatorJetFlavor->GetBinContent(i) / DenominatorJetFlavor->Integral();
    FakeRateJetFlavor->SetBinContent(i, FakeRateJetFlavor->GetBinContent(i)*weight);
    FakeRateJetFlavor->SetBinError(i, FakeRateJetFlavor->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "GsfTrackWeightedFakeRate_JetFlavor_ChargedKaon");



  //*****************************************************************************************
  //Make JetFlavor GsfTrack Fake Rate plots for Heavy Flavor Category
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets 
  NumeratorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorJetFlavor_HeavyFlavor");
  DenominatorJetFlavor = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphWJets");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");


  //weight by denominator fraction of each bin
//   for (int i=0; i <= FakeRateJetFlavor->GetXaxis()->GetNbins()+1 ; i++) {
//     double weight = 0;
//     if (DenominatorJetFlavor->Integral() > 0)
//       weight = DenominatorJetFlavor->GetBinContent(i) / DenominatorJetFlavor->Integral();
//     FakeRateJetFlavor->SetBinContent(i, FakeRateJetFlavor->GetBinContent(i)*weight);
//     FakeRateJetFlavor->SetBinError(i, FakeRateJetFlavor->GetBinError(i)*weight);
//   }

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph WJets");

  //madgraph QCD 
  NumeratorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorJetFlavor_HeavyFlavor");
  DenominatorJetFlavor = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorJetFlavor");
  FakeRateJetFlavor = (TH1F*)NumeratorJetFlavor->Clone("FakeRate_JetFlavor_MadgraphQCD");
  FakeRateJetFlavor->GetYaxis()->SetTitle("Fake Rate");
  FakeRateJetFlavor->Divide(NumeratorJetFlavor, DenominatorJetFlavor, 1.0, 1.0, "B");
//   for (int i=0; i <= FakeRateJetFlavor->GetXaxis()->GetNbins()+1 ; i++) {
//     double weight = 0;
//     //weight = DenominatorFakeCategory->GetBinContent(3) / DenominatorFakeCategory->Integral();
//     if (DenominatorJetFlavor->Integral() > 0)
//       weight = DenominatorJetFlavor->GetBinContent(i) / DenominatorJetFlavor->Integral();
//     FakeRateJetFlavor->SetBinContent(i, FakeRateJetFlavor->GetBinContent(i)*weight);
//     FakeRateJetFlavor->SetBinError(i, FakeRateJetFlavor->GetBinError(i)*weight);
//   }

  Histograms.push_back(FakeRateJetFlavor);
  LegendNames.push_back("Madgraph QCD");

//  drawPlots(Histograms,LegendNames, "GsfTrackWeightedFakeRate_JetFlavor_HeavyFlavor");



  





  //*****************************************************************************************
  //Find Weighted Fake Rate as function of Pt for Reco Denominator
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets 
  NumeratorFakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronNumeratorPt");
  DenominatorFakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco", "hElectronDenominatorPt");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_MadgraphWJets");
  FakeRateFakeCategory->GetYaxis()->SetTitle("Fake Rate");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  
  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateFakeCategory->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorFakeCategory->Integral() > 0)
      weight = DenominatorFakeCategory->GetBinContent(i) / DenominatorFakeCategory->Integral();
    FakeRateFakeCategory->SetBinContent(i, FakeRateFakeCategory->GetBinContent(i)*weight);
    FakeRateFakeCategory->SetBinError(i, FakeRateFakeCategory->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("Madgraph WJets");


  //madgraph QCD 
  NumeratorFakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronNumeratorPt");
  DenominatorFakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet50", "hElectronDenominatorPt");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_MadgraphQCD");
  FakeRateFakeCategory->GetYaxis()->SetTitle("Fake Rate");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  
  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateFakeCategory->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorFakeCategory->Integral() > 0)
      weight = DenominatorFakeCategory->GetBinContent(i) / DenominatorFakeCategory->Integral();
    FakeRateFakeCategory->SetBinContent(i, FakeRateFakeCategory->GetBinContent(i)*weight);
    FakeRateFakeCategory->SetBinError(i, FakeRateFakeCategory->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "RecoWeightedFakeRate_Pt");


  //*****************************************************************************************
  //Find Weighted Fake Rate as function of Pt for GsfTrack Denominator
  //*****************************************************************************************
  Histograms.clear();
  LegendNames.clear();

  //madgraph wjets 
  NumeratorFakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorPt");
  DenominatorFakeCategory = addAllSamples(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorPt");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_MadgraphWJets");
  FakeRateFakeCategory->GetYaxis()->SetTitle("Fake Rate");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  
  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateFakeCategory->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorFakeCategory->Integral() > 0)
      weight = DenominatorFakeCategory->GetBinContent(i) / DenominatorFakeCategory->Integral();
    FakeRateFakeCategory->SetBinContent(i, FakeRateFakeCategory->GetBinContent(i)*weight);
    FakeRateFakeCategory->SetBinError(i, FakeRateFakeCategory->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("Madgraph WJets");


  //madgraph QCD 
  NumeratorFakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronNumeratorPt");
  DenominatorFakeCategory = addAllSamples(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeElectronFakeRateMod_Jet50", "hElectronDenominatorPt");
  FakeRateFakeCategory = (TH1F*)NumeratorFakeCategory->Clone("FakeRate_FakeCategory_MadgraphQCD");
  FakeRateFakeCategory->GetYaxis()->SetTitle("Fake Rate");
  FakeRateFakeCategory->Divide(NumeratorFakeCategory, DenominatorFakeCategory, 1.0, 1.0, "B");
  
  //weight by denominator fraction of each bin
  for (int i=0; i <= FakeRateFakeCategory->GetXaxis()->GetNbins()+1 ; i++) {
    double weight = 0;
    if (DenominatorFakeCategory->Integral() > 0)
      weight = DenominatorFakeCategory->GetBinContent(i) / DenominatorFakeCategory->Integral();
    FakeRateFakeCategory->SetBinContent(i, FakeRateFakeCategory->GetBinContent(i)*weight);
    FakeRateFakeCategory->SetBinError(i, FakeRateFakeCategory->GetBinError(i)*weight);
  }

  Histograms.push_back(FakeRateFakeCategory);
  LegendNames.push_back("Madgraph QCD");

  //drawPlots(Histograms,LegendNames, "GsfTrackWeightedFakeRate_Pt");




  return;
}


//--------------------------------------------------------------------------------------------------
// Do Fake Rate Computation From WJets sample
//--------------------------------------------------------------------------------------------------
void makeNumeratorDenominatorPlots() {

  vector<string> madgraphJetTriggerDatasetFiles;
  vector<string> madgraphJetTriggerDatasetNames;
  vector<string> madgraphPhotonTriggerDatasetFiles;
  vector<string> madgraphPhotonTriggerDatasetNames;



  madgraphJetTriggerDatasetFiles.clear();
  madgraphJetTriggerDatasetNames.clear();
  madgraphPhotonTriggerDatasetFiles.clear();
  madgraphPhotonTriggerDatasetNames.clear();



  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-zjets-mg-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("s8-zjets-mg-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-ttbar-mg-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("f8-ttbar-mg-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-wjets-mg-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("s8-pj200_inf-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("s8-pj100_200-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("s8-pj40_100-id11");
  
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("f8-qcd100_250-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("f8-qcd1000_inf-id11");





  madgraphPhotonTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-zjets-mg-id11_noskim.root");
  madgraphPhotonTriggerDatasetNames.push_back("s8-zjets-mg-id11");
  madgraphPhotonTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-ttbar-mg-id11_noskim.root");
  madgraphPhotonTriggerDatasetNames.push_back("f8-ttbar-mg-id11");
  madgraphPhotonTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-wjets-mg-id11_noskim.root");
  madgraphPhotonTriggerDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphPhotonTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphPhotonTriggerDatasetNames.push_back("f8-qcd1000_inf-id11");
  madgraphPhotonTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphPhotonTriggerDatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphPhotonTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphPhotonTriggerDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphPhotonTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphPhotonTriggerDatasetNames.push_back("f8-qcd100_250-id11");

  madgraphPhotonTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphPhotonTriggerDatasetNames.push_back("s8-pj200_inf-id11");
  madgraphPhotonTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphPhotonTriggerDatasetNames.push_back("s8-pj100_200-id11");
  madgraphPhotonTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090507_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphPhotonTriggerDatasetNames.push_back("s8-pj40_100-id11");

  //*****************************************************************************************
  //Construct the fake rate Histgrams. Fit with function.
  //*****************************************************************************************
  vector<TH1F*> Histograms; Histograms.clear();
  vector<string> LegendNames; LegendNames.clear();

  THStack *NumeratorStack = 0;
  THStack *DenominatorStack = 0;
  THStack *TriggerPhotonPtStack = 0;
  THStack *TriggerJetPtStack = 0;

  //*****************************************************************************************
  //Jet Trigger Sample 
  //*****************************************************************************************

//   //madgraph wjets gsf track
  TriggerJetPtStack = addAllSamplesStacked(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet80", "hTriggerJetPt");
  NumeratorStack = addAllSamplesStacked(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet80", "hElectronNumeratorPt");
  DenominatorStack = addAllSamplesStacked(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet80", "hElectronDenominatorPt");
  
  //drawStackedPlot(TriggerJetPtStack, "JetTriggerPt_JetTriggerSample", false);
  drawStackedPlot(TriggerJetPtStack, "JetTriggerPt_JetTriggerSample_logY", true);
  drawStackedPlot(NumeratorStack, "NumeratorPtStacked_JetTriggerSample", false);
  //drawStackedPlot(NumeratorStack, "NumeratorPtStacked_JetTriggerSample_logY", true);
  //drawStackedPlot(DenominatorStack, "RecoElectronDenominatorPtStacked_JetTriggerSample",false);
  drawStackedPlot(DenominatorStack, "RecoElectronDenominatorPtStacked_JetTriggerSample_logY",true);

  NumeratorStack = addAllSamplesStacked(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet80", "hElectronNumeratorEta");
  DenominatorStack = addAllSamplesStacked(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, "ComputeElectronFakeRateMod_Reco_Jet80", "hElectronDenominatorEta");
  drawStackedPlot(NumeratorStack, "NumeratorEtaStacked_JetTriggerSample", false);
  drawStackedPlot(DenominatorStack, "RecoElectronDenominatorEtaStacked_JetTriggerSample_logY",true);





  //*****************************************************************************************
  //Photon Trigger Sample
  //*****************************************************************************************
  TriggerPhotonPtStack = addAllSamplesStacked(madgraphPhotonTriggerDatasetFiles, madgraphPhotonTriggerDatasetNames, "ComputeElectronFakeRateMod_Reco_Photon15", "hTriggerPhotonPt");
  NumeratorStack = addAllSamplesStacked(madgraphPhotonTriggerDatasetFiles, madgraphPhotonTriggerDatasetNames, "ComputeElectronFakeRateMod_Reco_Photon15", "hElectronNumeratorPt");
  DenominatorStack = addAllSamplesStacked(madgraphPhotonTriggerDatasetFiles, madgraphPhotonTriggerDatasetNames, "ComputeElectronFakeRateMod_Reco_Photon15", "hElectronDenominatorPt");

  //drawStackedPlot(TriggerPhotonPtStack, "TriggerPhotonPt_PhotonTriggerSample", false);
  drawStackedPlot(TriggerPhotonPtStack, "TriggerPhotonPt_PhotonTriggerSample_logY", true);
  drawStackedPlot(NumeratorStack, "NumeratorStacked_PhotonTriggerSample", false);
  //drawStackedPlot(NumeratorStack, "NumeratorStacked_PhotonTriggerSample_logY", true);
  //drawStackedPlot(DenominatorStack, "RecoElectronDenominatorStacked_PhotonTriggerSample", false);
  drawStackedPlot(DenominatorStack, "RecoElectronDenominatorStacked_PhotonTriggerSample_logY", true);

  return;
}


//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void makeElectronFakeRateOtherPlots() {

  //makePlots();
  //makeFakeRatePlots();
  makeNumeratorDenominatorPlots();

  return;
}


