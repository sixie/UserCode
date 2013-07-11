//root -l $CMSSW_BASE/src/MitHiggs/macros/plotMacros/computeElectronFakeRates.C+\(\"MuonFakeRateFile.root\"\)
//root -l $CMSSW_BASE/src/MitHiggs/macros/plotMacros/computeElectronFakeRates.C+\(\"ElectronFakeRateFile.root\"\)
//root -l $CMSSW_BASE/src/MitHiggs/macros/plotMacros/computeElectronFakeRates.C+\(\"FakeRates.root\"\)
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
#include <TMath.h>
#include <TGraphAsymmErrors.h>
#include <MitStyle.h>
#include "MitAna/Utils/interface/SimpleTable.h"
#include "MitCommon/MathTools/interface/MathUtils.h"

#include <vector>

using namespace std;
//using namespace mithep;
#endif

#include <vector>


Double_t LUMINOSITY = 200.0;
//cross section table
mithep::SimpleTable xstab("$CMSSW_BASE/src/MitPhysics/data/xs.dat");  

//define colors
Int_t COLORS[10] = {kRed,kBlue,kMagenta,kGreen,kBlack,kOrange,8,9,10,11};

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
// Add histograms form multiple files together weighted by cross-section
//--------------------------------------------------------------------------------------------------
TH1F* addAllSamples(vector<string> datasetFiles, vector<string> datasetNames,
                    string dirName, string histName) {

  if (datasetFiles.size() <= 0) cout << "Error: datasetFiles.size() == 0. " << dirName << " " << histName << endl;
  assert(datasetFiles.size() > 0);
  TH1F *tmp = getHisto(datasetFiles[0], dirName, histName);
  assert(tmp);
  TH1F *finalHist = (TH1F*)tmp->Clone();
  finalHist->Sumw2();

  double maxWeight = 0;

  for (UInt_t i=0; i < datasetFiles.size() ;i++) {
    double CrossSection = xstab.Get(datasetNames[i].c_str());
    TH1F *tmpNEventsHist = getHisto(datasetFiles[i],"AnaFwkMod", "hDEvents");
    double NEvents = tmpNEventsHist->Integral();
    double weight = CrossSection * LUMINOSITY / NEvents;

    if (weight > maxWeight) maxWeight = weight;
    
    //Add histograms together correctly for weighted sum of histograms.
    TH1F *tmpHist = getHisto(datasetFiles[i], dirName, histName);
    for (int b=0;b<=finalHist->GetNbinsX();b++) {
      for (int count=0;count < tmpHist->GetBinContent(b); ++count) {
        finalHist->Fill(finalHist->GetBinCenter(b), weight);        
      }
    }
  }

  //Make correction to bin error. the error for each bin must be at least as large as the largest weight of each sample. This protects against artificially small errors due to one sample giving 0 yield.
  for (int b=0;b<=finalHist->GetNbinsX();b++) {
    //cout << "bin " << b << " : " << finalHist->GetBinError(b) << " <? " << maxWeight << endl;
    if (finalHist->GetBinError(b) < maxWeight) {
      finalHist->SetBinError(b,maxWeight);
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
        for (int c=0;c<=finalHist->GetNbinsY()+1;c++) {
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


void saveFakeRate(TFile *file, vector<TH1F*> fakeRateHistograms, vector<TF1*> fakeRateFits, Bool_t fitFakeRate ) {
  for (UInt_t i=0; i<fakeRateHistograms.size();i++) {
    //write the histograms. delete any existing one with the same name
    fakeRateHistograms[i]->SetDirectory(file);
    file->WriteTObject(fakeRateHistograms[i], fakeRateHistograms[i]->GetName(), "WriteDelete");
    
    //write the fit functions. delete any existing one with the same name
    if (fitFakeRate) {
      file->WriteTObject(fakeRateFits[i], fakeRateFits[i]->GetName(), "WriteDelete");
    }
  }
}


void saveFakeRate(TFile *file, vector<TH2F*> fakeRateHistograms, vector<TF2*> fakeRateFits, Bool_t fitFakeRate ) {
  for (UInt_t i=0; i<fakeRateHistograms.size();i++) {
    //write the histograms. delete any existing one with the same name
    fakeRateHistograms[i]->SetDirectory(file);
    file->WriteTObject(fakeRateHistograms[i], fakeRateHistograms[i]->GetName(), "WriteDelete");
    
    //write the fit functions. delete any existing one with the same name
    if (fitFakeRate) {
      file->WriteTObject(fakeRateFits[i], fakeRateFits[i]->GetName(), "WriteDelete" );
    }
  }
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

  //find the largest weight among all the samples
  Double_t maxWeight = 0;
  for (int s=0; s<datasetFiles.size() ; ++s ) {      
    double CrossSection = xstab.Get(datasetNames[s].c_str());
    TH1F *tmpNEventsHist = getHisto(datasetFiles[s],"AnaFwkMod", "hDEvents");
    double NEvents = tmpNEventsHist->Integral();
    double weight = CrossSection * LUMINOSITY / NEvents;
    if (weight > maxWeight || maxWeight == 0) maxWeight = weight;     
  }
  
  TH1F *denominator = addAllSamples(datasetFiles, datasetNames, dirName, denominatorHistname);
  TH1F *numerator = addAllSamples(datasetFiles, datasetNames, dirName, numeratorHistname);
  
  TH1F *n = rebin(numerator,ptbins);
  TH1F *d = rebin(denominator,ptbins);

  //create a bogus numerator histogram where any bin that has 0 is filled by one event * maxWeight
  TH1F *n_AddedMaxWeightToZeroBins = (TH1F*)n->Clone(histname.c_str());
  for (int b=0;b<=n_AddedMaxWeightToZeroBins->GetNbinsX()+1;b++) {
    if (n_AddedMaxWeightToZeroBins->GetBinContent(b) == 0) {
      n_AddedMaxWeightToZeroBins->SetBinContent(b,maxWeight);
      n_AddedMaxWeightToZeroBins->SetBinError(b,maxWeight);
    }
  }

  TH1F *fakeRate = (TH1F*)n->Clone(histname.c_str());
  fakeRate->GetYaxis()->SetTitle("Fake Rate");
  fakeRate->Divide(n, d, 1.0,1.0,"B");  

  TH1F *fakeRate_AddedMaxWeightToZeroBins = (TH1F*)n->Clone(histname.c_str());
  fakeRate_AddedMaxWeightToZeroBins->GetYaxis()->SetTitle("Fake Rate");
  fakeRate_AddedMaxWeightToZeroBins->Divide(n_AddedMaxWeightToZeroBins, d, 1.0,1.0,"B");  

  //replace the errors in the bins with 0 by errors from the fake rate where I incremented
  //the bins with 0 numerator to maxWeight.
  for (int b=0;b<=fakeRate->GetNbinsX()+1;b++) {
    if (fakeRate->GetBinContent(b) == 0) {
      fakeRate->SetBinError(b,fakeRate_AddedMaxWeightToZeroBins->GetBinError(b));
    }
  } 


//   //divide by the denominator to get fake rate. Recompute error according to BayesDivide
//   cerr << "HIST: " << histname << endl;
//   for (int b=0;b<=fakeRate->GetNbinsX()+1;b++) {
//     Double_t ratio = 0;
//     Double_t errorLow = 0;
//     Double_t errorHigh = 0;

//     if (d->GetBinContent(b) > 0) {
//       cerr << n->GetBinContent(b) << " " << d->GetBinContent(b)  << " " ;
//       mithep::MathUtils::CalcRatio(n->GetBinContent(b), d->GetBinContent(b), ratio, errorLow, errorHigh);
//       cerr << ratio << " " << errorLow << " " << errorHigh << endl;
//     } 
        
//     //use average of high and low error for now.
//     fakeRate->SetBinContent(b,ratio);
//     fakeRate->SetBinError(b,(errorLow+errorHigh)/2.0);
//   } 


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
  
  //find the largest weight among all the samples
  Double_t maxWeight = 0;
  for (int s=0; s<datasetFiles.size() ; ++s ) {      
    double CrossSection = xstab.Get(datasetNames[s].c_str());
    TH1F *tmpNEventsHist = getHisto(datasetFiles[s],"AnaFwkMod", "hDEvents");
    double NEvents = tmpNEventsHist->Integral();
    double weight = CrossSection * LUMINOSITY / NEvents;
    if (weight > maxWeight || maxWeight == 0) maxWeight = weight;     
  }

  TH1F *denominator = addAllSamples(datasetFiles, datasetNames, dirName, denominatorHistname);
  TH1F *numerator = addAllSamples(datasetFiles, datasetNames, dirName, numeratorHistname);

  TH1F *n = numerator;
  TH1F *d = denominator;  
  n = rebin(numerator,etabins);
  d = rebin(denominator,etabins);

  //create a bogus numerator histogram where any bin that has 0 is filled by one event * maxWeight
  TH1F *n_AddedMaxWeightToZeroBins = (TH1F*)n->Clone(histname.c_str());
  for (int b=0;b<=n_AddedMaxWeightToZeroBins->GetNbinsX()+1;b++) {
    if (n_AddedMaxWeightToZeroBins->GetBinContent(b) == 0) {
      n_AddedMaxWeightToZeroBins->SetBinContent(b,maxWeight);
      n_AddedMaxWeightToZeroBins->SetBinError(b,maxWeight);
    }
  }

  TH1F *fakeRate = (TH1F*)n->Clone(histname.c_str());
  fakeRate->GetYaxis()->SetTitle("Fake Rate");
  fakeRate->Divide(n, d, 1.0,1.0,"B");  

  TH1F *fakeRate_AddedMaxWeightToZeroBins = (TH1F*)n->Clone(histname.c_str());
  fakeRate_AddedMaxWeightToZeroBins->GetYaxis()->SetTitle("Fake Rate");
  fakeRate_AddedMaxWeightToZeroBins->Divide(n_AddedMaxWeightToZeroBins, d, 1.0,1.0,"B");  

  //replace the errors in the bins with 0 by errors from the fake rate where I incremented
  //the bins with 0 numerator to maxWeight.
  for (int b=0;b<=fakeRate->GetNbinsX()+1;b++) {
    if (fakeRate->GetBinContent(b) == 0) {
      fakeRate->SetBinError(b,fakeRate_AddedMaxWeightToZeroBins->GetBinError(b));
    }
  } 

//   //divide by the denominator to get fake rate. Recompute error according to BayesDivide
//   cerr << "HIST: " << histname << endl;
//   for (int b=0;b<=fakeRate->GetNbinsX()+1;b++) {
//     Double_t ratio = 0;
//     Double_t errorLow = 0;
//     Double_t errorHigh = 0;

//     cerr << n->GetBinContent(b) << " " << d->GetBinContent(b)  << " " ;
//     if (d->GetBinContent(b) > 0) {
//       mithep::MathUtils::CalcRatio(n->GetBinContent(b), d->GetBinContent(b), ratio, errorLow, errorHigh);
//       cerr << ratio << " " << errorLow << " " << errorHigh << endl;
//     }
        
//     //use average of high and low error for now.
//     fakeRate->SetBinContent(b,ratio);
//     fakeRate->SetBinError(b,(errorLow+errorHigh)/2.0);
//   }  

  return fakeRate;
}



//--------------------------------------------------------------------------------------------------
// Create Fake Rate Histogram. Use binomial Errors
//--------------------------------------------------------------------------------------------------
TH2F* createFakeRatePtEtaHist(vector<string> datasetFiles, vector<string> datasetNames, string dirName,
                              string numeratorHistname, string denominatorHistname, string histname, 
                              Double_t minEventsPerBin = 5) {
  
  assert(datasetFiles.size() == datasetNames.size());

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

  //find the largest weight among all the samples
  Double_t maxWeight = 0;
  for (int s=0; s<datasetFiles.size() ; ++s ) {      
    double CrossSection = xstab.Get(datasetNames[s].c_str());
    TH1F *tmpNEventsHist = getHisto(datasetFiles[s],"AnaFwkMod", "hDEvents");
    double NEvents = tmpNEventsHist->Integral();
    double weight = CrossSection * LUMINOSITY / NEvents;
    if (weight > maxWeight || maxWeight == 0) maxWeight = weight;     
  }

  TH2F *denominator = addAllSamples2D(datasetFiles, datasetNames, dirName, denominatorHistname,ptbins,etabins);
  TH2F *numerator = addAllSamples2D(datasetFiles, datasetNames, dirName, numeratorHistname,ptbins,etabins);

  //create a bogus numerator histogram where any bin that has 0 is filled by one event * maxWeight
  TH2F *n_AddedMaxWeightToZeroBins = (TH2F*)numerator->Clone(histname.c_str());
  for (int b=0;b<=n_AddedMaxWeightToZeroBins->GetNbinsX()+1;b++) {
    for (int c=0;c<=n_AddedMaxWeightToZeroBins->GetNbinsY()+1;c++) {
      if (n_AddedMaxWeightToZeroBins->GetBinContent(b,c) == 0) {
      n_AddedMaxWeightToZeroBins->SetBinContent(b,c,maxWeight);
      n_AddedMaxWeightToZeroBins->SetBinError(b,c,maxWeight);
      }
    }
  }

  TH2F *fakeRate = (TH2F*)numerator->Clone(histname.c_str());
  fakeRate->GetYaxis()->SetTitle("Fake Rate");
  fakeRate->Divide(numerator, denominator, 1.0,1.0,"B");  

  TH2F *fakeRate_AddedMaxWeightToZeroBins = (TH2F*)numerator->Clone(histname.c_str());
  fakeRate_AddedMaxWeightToZeroBins->GetYaxis()->SetTitle("Fake Rate");
  fakeRate_AddedMaxWeightToZeroBins->Divide(n_AddedMaxWeightToZeroBins, denominator, 1.0,1.0,"B");  

  //replace the errors in the bins with 0 by errors from the fake rate where I incremented
  //the bins with 0 numerator to maxWeight.
  for (int b=0;b<=fakeRate->GetNbinsX()+1;b++) {
    for (int c=0;c<=fakeRate->GetNbinsX()+1;c++) {
      if (fakeRate->GetBinContent(b,c) == 0) {
        fakeRate->SetBinError(b,c,fakeRate_AddedMaxWeightToZeroBins->GetBinError(b,c));
      }
    } 
  }

//   //divide by the denominator to get fake rate. Recompute error according to BayesDivide
//   for (int b=0;b<=fakeRate->GetNbinsX()+1;b++) {
//     for (int c=0;c<=fakeRate->GetNbinsY()+1;c++) {
//       Double_t ratio = 0;
//       Double_t errorLow = 0;
//       Double_t errorHigh = 0;
//       if (denominator->GetBinContent(b) > 0) {
//         cerr << numerator->GetBinContent(b,c) << " " << denominator->GetBinContent(b,c)  << " " ;
//         mithep::MathUtils::CalcRatio(numerator->GetBinContent(b,c), denominator->GetBinContent(b,c), ratio, errorLow, errorHigh);
//         cerr << ratio << " " << errorLow << " " << errorHigh << endl;
//       } 

//       fakeRate->SetBinContent(b,c,ratio);
//       fakeRate->SetBinError(b,c,(errorLow+errorHigh)/2.0);
//     }
//   }
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
// Draw the FakeRate histograms and fit functions
//--------------------------------------------------------------------------------------------------
void drawFakeRatePlots( vector<TGraphAsymmErrors*> fakeRateHistograms, vector<TF1*> fakeRateFits,
                        vector<string> fakeRateLegendNames , string plotname ,Double_t MAXY = 0.2) {

  Bool_t fitFakeRate = true;
  if (!fitFakeRate || fakeRateHistograms.size() != fakeRateFits.size())
    fitFakeRate = false;

  assert(fakeRateHistograms.size() == fakeRateLegendNames.size());
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  TLegend *leg1=0;
  leg1 = new TLegend(0.25,0.8,0.55,0.9);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.025);
  
  for (UInt_t i=0; i<fakeRateHistograms.size() ; i++) {    

    //cerr << fakeRateHistograms[i] << endl;
    //cerr << fakeRateHistograms[i]->GetN() << endl;
    //cerr << COLORS[i] << endl;

    fakeRateHistograms[i]->SetMarkerColor(COLORS[i]);
    fakeRateHistograms[i]->SetMinimum(0.0);
    fakeRateHistograms[i]->SetMaximum(MAXY);
    //fakeRateHistograms[i]->SetLineColor(COLORS[i]);
    

    if (fitFakeRate) fakeRateFits[i]->SetLineColor(COLORS[i]);    
    leg1->AddEntry(fakeRateHistograms[i], fakeRateLegendNames[i].c_str(), "LP"); 
  
    if (i==0) { 
      fakeRateHistograms[i]->Draw("AP");
    } else {
      fakeRateHistograms[i]->Draw("P");
    }    
    if (fitFakeRate)  fakeRateFits[i]->Draw("same");    
  }

  leg1->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}

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

void drawFakeRatePlots( vector<TH2F*> fakeRateHistograms, vector<TF2*> fakeRateFits,
                        vector<string> fakeRateLegendNames , string plotname ) {

  Bool_t fitFakeRate = true;
  if (!fitFakeRate || fakeRateHistograms.size() != fakeRateFits.size())
    fitFakeRate = false;

  assert(fakeRateHistograms.size() == fakeRateLegendNames.size());
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  TLegend *leg1=0;
  leg1 = new TLegend(0.25,0.8,0.55,0.9);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.025);
  

  Double_t maxy = 0.0;
  for (UInt_t i=0; i<fakeRateHistograms.size() ; i++) {    
    if (fakeRateHistograms[i]->GetMaximum() > maxy)
      maxy = fakeRateHistograms[i]->GetMaximum();
  }

  for (UInt_t i=0; i<fakeRateHistograms.size() ; i++) {    
    fakeRateHistograms[i]->SetMarkerColor(COLORS[i]);
    fakeRateHistograms[i]->SetMinimum(0.0);
    fakeRateHistograms[i]->SetMaximum(maxy * 1.5);
    //fakeRateHistograms[i]->SetLineColor(COLORS[i]);
    
    if (fitFakeRate) fakeRateFits[i]->SetLineColor(COLORS[i]);    
    leg1->AddEntry(fakeRateHistograms[i], fakeRateLegendNames[i].c_str(), "LP"); 
  
    if (i==0) { 
      fakeRateHistograms[i]->Draw("lego");
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
void compute1DElectronFakeRatesFromWJets( char *FakeRateFilename ,bool fitFakeRate) {

  //Samples
  vector<string> madgraphDatasetFiles;
  vector<string> madgraphDatasetNames;
  vector<string> madgraphWJetsDatasetFiles;
  vector<string> madgraphWJetsDatasetNames;
  vector<string> madgraphVQQDatasetFiles;
  vector<string> madgraphVQQDatasetNames;
  vector<string> pythiaDatasetFiles;
  vector<string> pythiaDatasetNames;
  vector<string> pythiaWeDatasetFiles;
  vector<string> pythiaWeDatasetNames;
  vector<string> pythiaWmDatasetFiles;
  vector<string> pythiaWmDatasetNames;
  vector<string> pythiaWtDatasetFiles;
  vector<string> pythiaWtDatasetNames;

  //*****************************************************************************************
  //Do FakeRate Computation 
  //*****************************************************************************************

  //Create root file to store fake rates
  TFile *file = new TFile(FakeRateFilename, "UPDATE");
  file->cd();
  
  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************
  madgraphDatasetFiles.clear(); 
  madgraphDatasetNames.clear();
  madgraphWJetsDatasetFiles.clear();
  madgraphWJetsDatasetNames.clear();
  madgraphVQQDatasetFiles.clear();
  madgraphVQQDatasetNames.clear();
  pythiaDatasetFiles.clear();
  pythiaDatasetNames.clear();
  pythiaWeDatasetFiles.clear();
  pythiaWeDatasetNames.clear();
  pythiaWmDatasetFiles.clear();
  pythiaWmDatasetNames.clear();
  pythiaWtDatasetFiles.clear();
  pythiaWtDatasetNames.clear();


  //Madgraph WJets
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522/ComputeElectronFakeRate_s8-wjets-mg-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522/ComputeElectronFakeRate_s8-vqq-mg-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-vqq-mg-id11");
  madgraphWJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522/ComputeElectronFakeRate_s8-wjets-mg-id11_noskim.root");
  madgraphWJetsDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphVQQDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522/ComputeElectronFakeRate_s8-vqq-mg-id11_noskim.root");
  madgraphVQQDatasetNames.push_back("s8-vqq-mg-id11");



  //Pythia WJets
  pythiaWeDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/ComputeElectronFakeRate_s8-we-id11_noskim.root");
  pythiaWeDatasetNames.push_back("s8-we-id11");
  pythiaWmDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/ComputeElectronFakeRate_s8-wm-id11_noskim.root");
  pythiaWmDatasetNames.push_back("s8-wm-id11");
  pythiaWtDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/ComputeElectronFakeRate_s8-wt-id11_noskim.root");
  pythiaWtDatasetNames.push_back("s8-wt-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/ComputeElectronFakeRate_s8-we-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-we-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/ComputeElectronFakeRate_s8-wm-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-wm-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/ComputeElectronFakeRate_s8-wt-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-wt-id11");

  
  
  //*****************************************************************************************
  //Construct the fake rate Histgrams. Fit with function.
  //*****************************************************************************************
  vector<TH1F*> fakeRateHistograms; fakeRateHistograms.clear();
  vector<TF1*> fakeRateFits; fakeRateFits.clear();
  vector<string> fakeRateLegendNames; fakeRateLegendNames.clear();
  TH1F *FakeRatePt_pythia = NULL;
  TF1 *FakeRateFitFunction_pythia = NULL;
  TH1F *FakeRatePt_madgraph = NULL;
  TF1 *FakeRateFitFunction_madgraph = NULL;
  TH1F *FakeRateEta_pythia = NULL;
  TH1F *FakeRateEta_madgraph = NULL;

  //*****************************************************************************************
  //GsfTrack + SC Electron Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaDatasetFiles, pythiaDatasetNames, "ComputeElectronFakeRateMod",
                                           "hElectronNumeratorPt","hElectronDenominatorPt", "GsfTrackSCElectronFakeRate_Pt_PythiaWJets");
  FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("GsfTrackSCElectronFakeRateFunction_Pt_PythiaWJets");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia W+Jets");

  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphDatasetFiles, madgraphDatasetNames, "ComputeElectronFakeRateMod",
                                             "hElectronNumeratorPt","hElectronDenominatorPt", "GsfTrackSCElectronFakeRate_Pt_MadgraphWJets");
  FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphWJets");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph W+Jets");

    
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //GsfTrack + SC Electron Fake Rates
  //Compare for different W decay channels
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRatePt_pythia = createFakeRatePtHist(pythiaWeDatasetFiles, pythiaWeDatasetNames, "ComputeElectronFakeRateMod",
                                           "hElectronNumeratorPt","hElectronDenominatorPt", "GsfTrackSCElectronFakeRate_Pt_PythiaWJets_We");
  FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("GsfTrackSCElectronFakeRateFunction_Pt_PythiaWJets_We");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia W->enu+Jets");

  FakeRatePt_pythia = createFakeRatePtHist(pythiaWmDatasetFiles, pythiaWmDatasetNames, "ComputeElectronFakeRateMod",
                                           "hElectronNumeratorPt","hElectronDenominatorPt", "GsfTrackSCElectronFakeRate_Pt_PythiaWJets_Wm");
  FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("GsfTrackSCElectronFakeRateFunction_Pt_PythiaWJets_Wm");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia W->munu+Jets");

  FakeRatePt_pythia = createFakeRatePtHist(pythiaWtDatasetFiles, pythiaWtDatasetNames, "ComputeElectronFakeRateMod",
                                           "hElectronNumeratorPt","hElectronDenominatorPt", "GsfTrackSCElectronFakeRate_Pt_PythiaWJets_Wt");
  FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("GsfTrackSCElectronFakeRateFunction_Pt_PythiaWJets_Wt");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia W->taunu+Jets");

  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //GsfTrack + SC Electron Fake Rates
  //Compare for different Madgraph jets samples
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRatePt_madgraph = createFakeRatePtHist(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod",
                                           "hElectronNumeratorPt","hElectronDenominatorPt", "GsfTrackSCElectronFakeRate_Pt_MadgraphWJets_LightJets");
  FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphWJets_LightJets");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph W+LightJets");

  FakeRatePt_madgraph = createFakeRatePtHist(madgraphVQQDatasetFiles, madgraphVQQDatasetNames, "ComputeElectronFakeRateMod",
                                           "hElectronNumeratorPt","hElectronDenominatorPt", "GsfTrackSCElectronFakeRate_Pt_MadgraphWJets_VQQ");
  FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphWJets_VQQ");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph VQQ");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);



  //*****************************************************************************************
  //Reco Electron Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();
  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaDatasetFiles, pythiaDatasetNames, "ComputeElectronFakeRateMod_Reco",
                                           "hElectronNumeratorPt","hElectronDenominatorPt", "RecoElectronFakeRate_Pt_PythiaWJets");
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("RecoElectronFakeRateFunction_Pt_PythiaWJets");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia W+Jets");

  
  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(
    madgraphDatasetFiles, madgraphDatasetNames, "ComputeElectronFakeRateMod_Reco",
    "hElectronNumeratorPt","hElectronDenominatorPt", "RecoElectronFakeRate_Pt_MadgraphWJets");
  FakeRatePt_madgraph->GetYaxis()->SetTitle("RecoElectron Fake Rate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName("RecoElectronFakeRateFunction_Pt_MadgraphWJets");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph W+Jets");

 
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Reco Electron Fake Rates
  //Compare for different W decay channels
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRatePt_pythia = createFakeRatePtHist(pythiaWeDatasetFiles, pythiaWeDatasetNames, "ComputeElectronFakeRateMod_Reco",
                                           "hElectronNumeratorPt","hElectronDenominatorPt", "RecoElectronFakeRate_Pt_PythiaWJets_We");
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("RecoElectronFakeRateFunction_Pt_PythiaWJets_We");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia W->enu+Jets");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaWmDatasetFiles, pythiaWmDatasetNames, "ComputeElectronFakeRateMod_Reco",
                                           "hElectronNumeratorPt","hElectronDenominatorPt", "RecoElectronFakeRate_Pt_PythiaWJets_Wm");
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("RecoElectronFakeRateFunction_Pt_PythiaWJets_Wm");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia W->munu+Jets");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaWtDatasetFiles, pythiaWtDatasetNames, "ComputeElectronFakeRateMod_Reco",
                                           "hElectronNumeratorPt","hElectronDenominatorPt", "RecoElectronFakeRate_Pt_PythiaWJets_Wt");
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("RecoElectronFakeRateFunction_Pt_PythiaWJets_Wt");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia W->taunu+Jets");

  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Reco Electron Fake Rates
  //Compare for different Madgraph jets samples
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRatePt_madgraph = createFakeRatePtHist(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco",
                                           "hElectronNumeratorPt","hElectronDenominatorPt", "RecoElectronFakeRate_Pt_MadgraphWJets_LightJets");
  FakeRatePt_madgraph->GetYaxis()->SetTitle("RecoElectronFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName("RecoElectronFakeRateFunction_Pt_MadgraphWJets_LightJets");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph W+LightJets");

  FakeRatePt_madgraph = createFakeRatePtHist(madgraphVQQDatasetFiles, madgraphVQQDatasetNames, "ComputeElectronFakeRateMod_Reco",
                                           "hElectronNumeratorPt","hElectronDenominatorPt", "RecoElectronFakeRate_Pt_MadgraphWJets_VQQ");
  FakeRatePt_madgraph->GetYaxis()->SetTitle("RecoElectronFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName("RecoElectronFakeRateFunction_Pt_MadgraphWJets_VQQ");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph VQQ");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Gsf + SC Fake Rate as function of Eta
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //Pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaDatasetFiles, pythiaDatasetNames, "ComputeElectronFakeRateMod",
                                           "hElectronNumeratorEta","hElectronDenominatorEta", "GsfTrackSCElectronFakeRate_Eta_PythiaWJets");
  FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("GsfTrackSCElectronFakeRateFunction_Eta_PythiaWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia W+Jets");


  //madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphDatasetFiles, madgraphDatasetNames, "ComputeElectronFakeRateMod",
                                             "hElectronNumeratorEta","hElectronDenominatorEta", "GsfTrackSCElectronFakeRate_Eta_MadgraphWJets");
  FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph W+Jets");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Compare Eta Fake Rate across different W channels
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRateEta_pythia = createFakeRateEtaHist(pythiaWeDatasetFiles, pythiaWeDatasetNames, "ComputeElectronFakeRateMod",
                                           "hElectronNumeratorEta","hElectronDenominatorEta", "GsfTrackSCElectronFakeRate_Eta_PythiaWJets_We");
  FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("GsfTrackSCElectronFakeRateFunction_Eta_PythiaWJets_We");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia W->enu+Jets");

  FakeRateEta_pythia = createFakeRateEtaHist(pythiaWmDatasetFiles, pythiaWmDatasetNames, "ComputeElectronFakeRateMod",
                                           "hElectronNumeratorEta","hElectronDenominatorEta", "GsfTrackSCElectronFakeRate_Eta_PythiaWJets_Wm");
  FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("GsfTrackSCElectronFakeRateFunction_Eta_PythiaWJets_Wm");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia W->munu+Jets");

  FakeRateEta_pythia = createFakeRateEtaHist(pythiaWtDatasetFiles, pythiaWtDatasetNames, "ComputeElectronFakeRateMod",
                                           "hElectronNumeratorEta","hElectronDenominatorEta", "GsfTrackSCElectronFakeRate_Eta_PythiaWJets_Wt");
  FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("GsfTrackSCElectronFakeRateFunction_Eta_PythiaWJets_Wt");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia W->taunu+Jets");

  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //GsfTrack + SC Electron Fake Rates Vs Eta
  //Compare for different Madgraph jets samples
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod",
                                           "hElectronNumeratorEta","hElectronDenominatorEta", "GsfTrackSCElectronFakeRate_Eta_MadgraphWJets_LightJets");
  FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphWJets_LightJets");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph W+LightJets");

  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphVQQDatasetFiles, madgraphVQQDatasetNames, "ComputeElectronFakeRateMod",
                                           "hElectronNumeratorEta","hElectronDenominatorEta", "GsfTrackSCElectronFakeRate_Eta_MadgraphWJets_VQQ");
  FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphWJets_VQQ");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph VQQ");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);




  //*****************************************************************************************
  //Reco Fake Rate as function of Eta
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //Pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaDatasetFiles, pythiaDatasetNames, "ComputeElectronFakeRateMod_Reco",
                                           "hElectronNumeratorEta","hElectronDenominatorEta", "RecoElectronFakeRate_Eta_PythiaWJets");
  FakeRateEta_pythia->GetYaxis()->SetTitle("RecoElectronFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("RecoElectronFakeRateFunction_Eta_PythiaWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia W+Jets");


  //madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphDatasetFiles, madgraphDatasetNames, "ComputeElectronFakeRateMod_Reco",
                                             "hElectronNumeratorEta","hElectronDenominatorEta", "RecoElectronFakeRate_Eta_MadgraphWJets");
  FakeRateEta_madgraph->GetYaxis()->SetTitle("RecoElectronFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("RecoElectronFakeRateFunction_Eta_MadgraphWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph W+Jets");

  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Reco Electron Fake Rates Vs Eta
  //Compare for different W decay channels
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRateEta_pythia = createFakeRateEtaHist(pythiaWeDatasetFiles, pythiaWeDatasetNames, "ComputeElectronFakeRateMod_Reco",
                                           "hElectronNumeratorEta","hElectronDenominatorEta", "RecoElectronFakeRate_Eta_PythiaWJets_We");
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("RecoElectronFakeRateFunction_Eta_PythiaWJets_We");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia W->enu+Jets");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaWmDatasetFiles, pythiaWmDatasetNames, "ComputeElectronFakeRateMod_Reco",
                                           "hElectronNumeratorEta","hElectronDenominatorEta", "RecoElectronFakeRate_Eta_PythiaWJets_Wm");
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("RecoElectronFakeRateFunction_Eta_PythiaWJets_Wm");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia W->munu+Jets");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaWtDatasetFiles, pythiaWtDatasetNames, "ComputeElectronFakeRateMod_Reco",
                                           "hElectronNumeratorEta","hElectronDenominatorEta", "RecoElectronFakeRate_Eta_PythiaWJets_Wt");
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("RecoElectronFakeRateFunction_Eta_PythiaWJets_Wt");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia W->taunu+Jets");

  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Reco Electron Fake Rates
  //Compare for different Madgraph jets samples
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeElectronFakeRateMod_Reco",
                                           "hElectronNumeratorEta","hElectronDenominatorEta", "RecoElectronFakeRate_Eta_MadgraphWJets_LightJets");
  FakeRateEta_madgraph->GetYaxis()->SetTitle("RecoElectronFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("RecoElectronFakeRateFunction_Eta_MadgraphWJets_LightJets");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph W+LightJets");

  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphVQQDatasetFiles, madgraphVQQDatasetNames, "ComputeElectronFakeRateMod_Reco",
                                               "hElectronNumeratorEta","hElectronDenominatorEta", "RecoElectronFakeRate_Eta_MadgraphWJets_VQQ");
  FakeRateEta_madgraph->GetYaxis()->SetTitle("RecoElectronFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("RecoElectronFakeRateFunction_Eta_MadgraphWJets_VQQ");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph VQQ");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  file->Close();
  return;

}


//--------------------------------------------------------------------------------------------------
// Do Fake Rate Computation From WJets sample
//--------------------------------------------------------------------------------------------------
void compute2DElectronFakeRatesFromWJets( char *FakeRateFilename ,bool fitFakeRate) {

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
  
  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************
  madgraphDatasetFiles.clear(); 
  madgraphDatasetNames.clear();
  pythiaDatasetFiles.clear();
  pythiaDatasetNames.clear();
  
  //Madgraph WJets
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522/ComputeElectronFakeRate_s8-wjets-mg-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522/ComputeElectronFakeRate_s8-vqq-mg-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-vqq-mg-id11");


  //Pythia WJets
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/ComputeElectronFakeRate_s8-we-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-we-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/ComputeElectronFakeRate_s8-wm-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-wm-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/ComputeElectronFakeRate_s8-wt-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-wt-id11");
  
  //*****************************************************************************************
  //Construct the fake rate Histgrams. Fit with function.
  //*****************************************************************************************
//  vector<TGraphAsymmErrors*> fakeRateHistograms; fakeRateHistograms.clear();
  vector<TH2F*> fakeRateHistograms; fakeRateHistograms.clear();
  vector<TF2*> fakeRateFits; fakeRateFits.clear();
  vector<string> fakeRateLegendNames; fakeRateLegendNames.clear();
  TH2F *FakeRatePtEta_pythia = NULL;
  TF2 *FakeRateFitFunction_pythia = NULL;
  TH2F *FakeRatePtEta_madgraph = NULL;
  TF2 *FakeRateFitFunction_madgraph = NULL;

  //*****************************************************************************************
  //GsfTrack + SC Electron Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //pythia version
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaDatasetFiles, pythiaDatasetNames, "ComputeElectronFakeRateMod",
                                                 "hElectronNumeratorPtEta","hElectronDenominatorPtEta", "GsfTrackSCElectronFakeRate_PtEta_PythiaWJets");
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateHist(FakeRatePtEta_pythia);
    //FakeRateFitFunction_pythia->SetName("GsfTrackSCElectronFakeRateFunction_PtEta_PythiaWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia W+Jets");

  
  //madgraph version
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist(madgraphDatasetFiles, madgraphDatasetNames, "ComputeElectronFakeRateMod",
                                                   "hElectronNumeratorPtEta","hElectronDenominatorPtEta", "GsfTrackSCElectronFakeRate_PtEta_MadgraphWJets");
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePtEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("GsfTrackSCElectronFakeRateFunction_PtEta_MadgraphWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph W+Jets");


  printFakeRate(fakeRateHistograms,fakeRateFits,fakeRateLegendNames, "GsfTrackSCElectronFakeRatePt");
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Reco Electron Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //pythia version
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaDatasetFiles, pythiaDatasetNames, "ComputeElectronFakeRateMod_Reco",
                                              "hElectronNumeratorPtEta","hElectronDenominatorPtEta", "RecoElectronFakeRate_PtEta_PythiaWJets");
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateHist(FakeRatePtEta_pythia);
    //FakeRateFitFunction_pythia->SetName("RecoElectronFakeRateFunction_PtEta_PythiaWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia W+Jets");

  
  //madgraph version
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist( madgraphDatasetFiles, madgraphDatasetNames, "ComputeElectronFakeRateMod_Reco",
                                                 "hElectronNumeratorPtEta","hElectronDenominatorPtEta", "RecoElectronFakeRate_PtEta_MadgraphWJets");
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRateHist(FakeRatePtEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("RecoElectronFakeRateFunction_Pt_MadgraphWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph W+Jets");
 

  printFakeRate(fakeRateHistograms,fakeRateFits,fakeRateLegendNames, "RecoElectronFakeRatePtEta");
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  file->Close();
  return;

}



void compute1DElectronFakeRatesFromQCD( char *FakeRateFilename ,bool fitFakeRate, string JetTrigger) {

  //Samples
  vector<string> madgraphJetTriggerDatasetFiles;
  vector<string> madgraphJetTriggerDatasetNames;

  vector<string> madgraphQCDDatasetFiles;
  vector<string> madgraphQCDDatasetNames;
  vector<string> madgraphQCD100To250DatasetFiles;
  vector<string> madgraphQCD100To250DatasetNames;
  vector<string> madgraphQCD250To500DatasetFiles;
  vector<string> madgraphQCD250To500DatasetNames;
  vector<string> madgraphQCD500To1000DatasetFiles;
  vector<string> madgraphQCD500To1000DatasetNames;
  vector<string> madgraphQCD1000ToInfDatasetFiles;
  vector<string> madgraphQCD1000ToInfDatasetNames;
  vector<string> pythiaQCDEnrichedDatasetFiles;
  vector<string> pythiaQCDEnrichedDatasetNames;

  vector<string> pythiaJetTriggerDatasetFiles;
  vector<string> pythiaJetTriggerDatasetNames;
  vector<string> pythiaQCDDatasetFiles;
  vector<string> pythiaQCDDatasetNames;
  vector<string> pythiaQCD15DatasetFiles;
  vector<string> pythiaQCD15DatasetNames;
  vector<string> pythiaQCD30DatasetFiles;
  vector<string> pythiaQCD30DatasetNames;
  vector<string> pythiaQCD80DatasetFiles;
  vector<string> pythiaQCD80DatasetNames;
  vector<string> pythiaQCD170DatasetFiles;
  vector<string> pythiaQCD170DatasetNames;
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
  //Do FakeRate Computation 
  //*****************************************************************************************

  //Create root file to store fake rates
  TFile *file = new TFile(FakeRateFilename, "UPDATE");
  file->cd();
  
  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************
  madgraphJetTriggerDatasetFiles.clear(); 
  madgraphJetTriggerDatasetNames.clear();
  madgraphQCDDatasetFiles.clear(); 
  madgraphQCDDatasetNames.clear();
  madgraphQCD100To250DatasetFiles.clear(); 
  madgraphQCD100To250DatasetNames.clear();
  madgraphQCD250To500DatasetFiles.clear(); 
  madgraphQCD250To500DatasetNames.clear();
  pythiaJetTriggerDatasetFiles.clear(); 
  pythiaJetTriggerDatasetNames.clear();
  pythiaQCDDatasetFiles.clear();
  pythiaQCDDatasetNames.clear();
  pythiaQCD15DatasetFiles.clear();
  pythiaQCD15DatasetNames.clear();
  pythiaQCD30DatasetFiles.clear();
  pythiaQCD30DatasetNames.clear();
  pythiaQCD80DatasetFiles.clear();
  pythiaQCD80DatasetNames.clear();
  pythiaQCD170DatasetFiles.clear();
  pythiaQCD170DatasetNames.clear();
  pythiaQCDEnrichedDatasetFiles.clear();
  pythiaQCDEnrichedDatasetNames.clear();
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
  pythiaQCDBCEEnriched30To80DatasetNames.clear();
  pythiaQCDBCEEnriched80To170DatasetFiles.clear();
  pythiaQCDBCEEnriched80To170DatasetNames.clear();


  //Madgraph JetTrigger Sample
   madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
   madgraphJetTriggerDatasetNames.push_back("f8-qcd100_250-id11");
   madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("f8-qcd1000_inf-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("s8-pj40_100-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("s8-pj100_200-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("s8-pj200_inf-id11");


  //Madgraph QCD
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd1000_inf-id11");
  madgraphQCD100To250DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphQCD100To250DatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCD250To500DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphQCD250To500DatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCD500To1000DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphQCD500To1000DatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphQCD1000ToInfDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphQCD1000ToInfDatasetNames.push_back("f8-qcd1000_inf-id11");


  //Pythia QCD
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-qcd15-id11");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-qcd30-id11");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-qcd80-id11");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-qcd170-id11");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj15-id12_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-pj15-id12");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj30-id11_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-pj30-id11");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj80-id12_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-pj80-id12");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj170-id11_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-pj170-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd15-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd30-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd80-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd170-id11");
  pythiaQCD15DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaQCD15DatasetNames.push_back("s8-qcd15-id11");
  pythiaQCD30DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaQCD30DatasetNames.push_back("s8-qcd30-id11");
  pythiaQCD80DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaQCD80DatasetNames.push_back("s8-qcd80-id11");
  pythiaQCD170DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaQCD170DatasetNames.push_back("s8-qcd170-id11");

  //Pythia QCDEM
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_20_30-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdem_20_30-id11");
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_30_80-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdem_30_80-id11");
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_80_170-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdem_80_170-id11");
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_20_30-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdbce_20_30-id11");
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_30_80-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdbce_30_80-id11");
  pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_80_170-id11_noskim.root");
  pythiaQCDEnrichedDatasetNames.push_back("s8-qcdbce_80_170-id11");

  pythiaQCDEMEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_20_30-id11_noskim.root");
  pythiaQCDEMEnrichedDatasetNames.push_back("s8-qcdem_20_30-id11");
  pythiaQCDEMEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_30_80-id11_noskim.root");
  pythiaQCDEMEnrichedDatasetNames.push_back("s8-qcdem_30_80-id11");
  pythiaQCDEMEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_80_170-id11_noskim.root");
  pythiaQCDEMEnrichedDatasetNames.push_back("s8-qcdem_80_170-id11");
  pythiaQCDBCEEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_20_30-id11_noskim.root");
  pythiaQCDBCEEnrichedDatasetNames.push_back("s8-qcdbce_20_30-id11");
  pythiaQCDBCEEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_30_80-id11_noskim.root");
  pythiaQCDBCEEnrichedDatasetNames.push_back("s8-qcdbce_30_80-id11");
  pythiaQCDBCEEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_80_170-id11_noskim.root");
  pythiaQCDBCEEnrichedDatasetNames.push_back("s8-qcdbce_80_170-id11");
  pythiaQCDEMEnriched20To30DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_20_30-id11_noskim.root");
  pythiaQCDEMEnriched20To30DatasetNames.push_back("s8-qcdem_20_30-id11");
  pythiaQCDEMEnriched30To80DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_30_80-id11_noskim.root");
  pythiaQCDEMEnriched30To80DatasetNames.push_back("s8-qcdem_30_80-id11");
  pythiaQCDEMEnriched80To170DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdem_80_170-id11_noskim.root");
  pythiaQCDEMEnriched80To170DatasetNames.push_back("s8-qcdem_80_170-id11");
  pythiaQCDBCEEnriched20To30DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_20_30-id11_noskim.root");
  pythiaQCDBCEEnriched20To30DatasetNames.push_back("s8-qcdbce_20_30-id11");
  pythiaQCDBCEEnriched30To80DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_30_80-id11_noskim.root");
  pythiaQCDBCEEnriched30To80DatasetNames.push_back("s8-qcdbce_30_80-id11");
  pythiaQCDBCEEnriched80To170DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/save_20090426_GoodVersion/ComputeElectronFakeRate_s8-qcdbce_80_170-id11_noskim.root");
  pythiaQCDBCEEnriched80To170DatasetNames.push_back("s8-qcdbce_80_170-id11");

    
  //*****************************************************************************************
  //Construct the fake rate Histgrams. Fit with function.
  //*****************************************************************************************
  vector<TH1F*> fakeRateHistograms; fakeRateHistograms.clear();
  vector<TF1*> fakeRateFits; fakeRateFits.clear();
  vector<string> fakeRateLegendNames; fakeRateLegendNames.clear();
  TH1F *FakeRatePt_pythia = NULL;
  TF1 *FakeRateFitFunction_pythia = NULL;
  TH1F *FakeRatePt_madgraph = NULL;
  TF1 *FakeRateFitFunction_madgraph = NULL;
  TH1F *FakeRateEta_pythia = NULL;
  TH1F *FakeRateEta_madgraph = NULL;



  //*****************************************************************************************
  //GsfTrack + SC Electron Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

   //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCD_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD");

  
  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_MadgraphQCD_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD");


  //Full JetTrigger madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_Madgraph_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_Madgraph_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph JetTrigger");


  //Full JetTrigger pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaJetTriggerDatasetFiles, pythiaJetTriggerDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_Pythia_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_Pythia_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia JetTrigger");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //GsfTrack + SC Electron Fake Rates For different Sample Composition
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();
  
   //QCDEM
   FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                            "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCDEnriched" + JetTrigger).c_str());
   FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCDEnriched " + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRatePt_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDEnriched");

   //QCDEM
   FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDEMEnrichedDatasetFiles, pythiaQCDEMEnrichedDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCDEMEnriched_" + JetTrigger).c_str());
   FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCDEMEnriched " + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRatePt_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDEMEnriched");

   //QCDBCE
   FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDBCEEnrichedDatasetFiles, pythiaQCDBCEEnrichedDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCDBCEEnriched_" + JetTrigger).c_str());
   FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCDBCEEnriched_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRatePt_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDBCEEnriched");

   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


   //*****************************************************************************************
   //GsfTrack + SC Electron Fake Rates For QCDBC, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();
   //QCDEM 20To30
   FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDEMEnriched20To30DatasetFiles, pythiaQCDEMEnriched20To30DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCDEMEnriched20To30_" + JetTrigger).c_str());
   FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCDEMEnriched20To30_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRatePt_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDEMEnriched20To30");

   //QCDEM 30To80
   FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDEMEnriched30To80DatasetFiles, pythiaQCDEMEnriched30To80DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCDEMEnriched30To80_" + JetTrigger).c_str());
   FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCDEMEnriched30To80_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRatePt_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDEMEnriched30To80");

   //QCDEM 80To170
   FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDEMEnriched80To170DatasetFiles, pythiaQCDEMEnriched80To170DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCDEMEnriched80To170_" + JetTrigger).c_str());
   FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCDEMEnriched80To170_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRatePt_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDEMEnriched80To170");

   //QCDBCE 20To30
   FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDBCEEnriched20To30DatasetFiles, pythiaQCDBCEEnriched20To30DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCDBCEEnriched20To30_" + JetTrigger).c_str());
   FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCDBCEEnriched20To30_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRatePt_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDBCEEnriched20To30");

   //QCDBCE 30To80
   FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDBCEEnriched30To80DatasetFiles, pythiaQCDBCEEnriched30To80DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
  "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCDBCEEnriched30To80_" + JetTrigger).c_str());
   FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCDBCEEnriched30To80_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRatePt_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDBCEEnriched30To80");

   //QCDBCE 80To170
   FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDBCEEnriched80To170DatasetFiles, pythiaQCDBCEEnriched80To170DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCDBCEEnriched80To170_" + JetTrigger).c_str());
   FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCDBCEEnriched80To170_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRatePt_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDBCEEnriched80To170");

   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


   //*****************************************************************************************
   //GsfTrack + SC Electron Fake Rates For madgraph QCD, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();

   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_MadgraphQCD100To250_" + JetTrigger).c_str());
   FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphQCD100To250_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD 100To250");


  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_MadgraphQCD250To500_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphQCD250To500_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD 250To500");


  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_MadgraphQCD500To1000_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphQCD500To1000_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD 500To1000");


  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD 1000ToInf");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


   //*****************************************************************************************
   //GsfTrack + SC Electron Fake Rates For pythia QCD, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();

   FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD15DatasetFiles, pythiaQCD15DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCD15_" + JetTrigger).c_str());
   FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCD15_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD 15");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD30DatasetFiles, pythiaQCD30DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCD30_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCD30_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD 30");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD80DatasetFiles, pythiaQCD80DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCD80_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCD80_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD 80");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD170DatasetFiles, pythiaQCD170DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCD170_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCD170_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD 170");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);



  //*****************************************************************************************
  //Reco Electron Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();
  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCD_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD");

  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_MadgraphQCD_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_MadgraphQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph QCD");


  //JetTrigger madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_Madgraph_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_Madgraph_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph JetTrigger");

  //JetTrigger pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaJetTriggerDatasetFiles, pythiaJetTriggerDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_Pythia_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_Pythia_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia JetTrigger");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Reco Electron Fake Rates For different Sample Composition
  //*****************************************************************************************
  
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //QCDEM
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDEMEnrichedDatasetFiles, pythiaQCDEMEnrichedDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCDEMEnriched_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCDEMEnriched_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCDEMEnriched");

  //QCDBCE
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDBCEEnrichedDatasetFiles, pythiaQCDBCEEnrichedDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCDBCEEnriched_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCDBCEEnriched_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCDBCEEnriched");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Reco Electron Fake Rates For QCDBC, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //QCDEM 20To30
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDEMEnriched20To30DatasetFiles, pythiaQCDEMEnriched20To30DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCDEMEnriched20To30_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCDEMEnriched20To30_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCDEMEnriched20To30");

  //QCDEM 30To80
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDEMEnriched30To80DatasetFiles, pythiaQCDEMEnriched30To80DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCDEMEnriched30To80_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCDEMEnriched30To80_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCDEMEnriched30To80");

  //QCDEM 80To170
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDEMEnriched80To170DatasetFiles, pythiaQCDEMEnriched80To170DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCDEMEnriched80To170_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCDEMEnriched80To170_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCDEMEnriched80To170");

  //QCDBCE 20To30
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDBCEEnriched20To30DatasetFiles, pythiaQCDBCEEnriched20To30DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCDBCEEnriched20To30_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCDBCEEnriched20To30_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCDBCEEnriched20To30");

  //QCDBCE 30To80
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDBCEEnriched30To80DatasetFiles, pythiaQCDBCEEnriched30To80DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCDBCEEnriched30To80_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCDBCEEnriched30To80_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCDBCEEnriched30To80");

  //QCDBCE 80To170
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDBCEEnriched80To170DatasetFiles, pythiaQCDBCEEnriched80To170DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCDBCEEnriched80To170_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCDBCEEnriched80To170_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCDBCEEnriched80To170");

  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Reco Electron Fake Rates For Madgraph QCD, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();


  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_MadgraphQCD100To250_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_MadgraphQCD100To250_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph QCD 100To250");


  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_MadgraphQCD250To500_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_MadgraphQCD250To500_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph QCD 250To500");


  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_MadgraphQCD500To1000_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_MadgraphQCD500To1000_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph QCD 500To1000");


  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph QCD 1000ToInf");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Reco Electron Fake Rates For Pythia QCD, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();


  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD15DatasetFiles, pythiaQCD15DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCD15_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCD15_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD 15");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD30DatasetFiles, pythiaQCD30DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCD30_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCD30_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD 30");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD80DatasetFiles, pythiaQCD80DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCD80_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCD80_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD 80");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD170DatasetFiles, pythiaQCD170DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCD170_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCD170_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD 170");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);





  //*****************************************************************************************
  //GsfTrack + SC Electron Fake Rates Vs Eta
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

   //pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCD_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD");

  
  //madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                             "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_MadgraphQCD_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD");


  //JetTrigger madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                             "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_Madgraph_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_Madgraph_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph JetTrigger");


  //JetTrigger pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaJetTriggerDatasetFiles, pythiaJetTriggerDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                             "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_Pythia_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_Pythia_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia JetTrigger");


   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

   //*****************************************************************************************
   //GsfTrack + SC Electron Fake Rates For different Sample Composition
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();

   //QCDEM
   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCDEnriched_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCDEnriched_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDEnriched");

   //QCDEM
   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDEMEnrichedDatasetFiles, pythiaQCDEMEnrichedDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCDEMEnriched_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCDEMEnriched_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDEMEnriched");

   //QCDBCE
   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDBCEEnrichedDatasetFiles, pythiaQCDBCEEnrichedDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCDBCEEnriched_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCDBCEEnriched_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDBCEEnriched");

   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


   //*****************************************************************************************
   //GsfTrack + SC Electron Fake Rates For QCDBC, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();
   //QCDEM 20To30
   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDEMEnriched20To30DatasetFiles, pythiaQCDEMEnriched20To30DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCDEMEnriched20To30_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCDEMEnriched20To30_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDEMEnriched20To30");

   //QCDEM 30To80
   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDEMEnriched30To80DatasetFiles, pythiaQCDEMEnriched30To80DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCDEMEnriched30To80_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCDEMEnriched30To80_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDEMEnriched30To80");

   //QCDEM 80To170
   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDEMEnriched80To170DatasetFiles, pythiaQCDEMEnriched80To170DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCDEMEnriched80To170_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCDEMEnriched80To170_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDEMEnriched80To170");

   //QCDBCE 20To30
   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDBCEEnriched20To30DatasetFiles, pythiaQCDBCEEnriched20To30DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCDBCEEnriched20To30_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCDBCEEnriched20To30_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDBCEEnriched20To30");

   //QCDBCE 30To80
   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDBCEEnriched30To80DatasetFiles, pythiaQCDBCEEnriched30To80DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCDBCEEnriched30To80_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCDBCEEnriched30To80_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDBCEEnriched30To80");

   //QCDBCE 80To170
   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDBCEEnriched80To170DatasetFiles, pythiaQCDBCEEnriched80To170DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCDBCEEnriched80To170_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCDBCEEnriched80To170_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDBCEEnriched80To170");

   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


   //*****************************************************************************************
   //GsfTrack + SC Electron Fake Rates For Madgraph QCD, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();


   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                                "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_MadgraphQCD100To250_" + JetTrigger).c_str());
   FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphQCD100To250_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
   fakeRateHistograms.push_back(FakeRateEta_madgraph);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD 100To250");
   

   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                                "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_MadgraphQCD250To500_" + JetTrigger).c_str());
   FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphQCD250To500_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
   fakeRateHistograms.push_back(FakeRateEta_madgraph);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD 250To500");


   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                                "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_MadgraphQCD500To1000_" + JetTrigger).c_str());
   FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphQCD500To1000_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
   fakeRateHistograms.push_back(FakeRateEta_madgraph);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD 500To1000");


   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                                "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
   FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
   fakeRateHistograms.push_back(FakeRateEta_madgraph);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD 1000ToInf");


   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


   //*****************************************************************************************
   //GsfTrack + SC Electron Fake Rates For Madgraph QCD, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();


   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD15DatasetFiles, pythiaQCD15DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                                "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCD15_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCD15_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD 15");
   

   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD30DatasetFiles, pythiaQCD30DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                                "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCD30_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCD30_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD 30");


   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD80DatasetFiles, pythiaQCD80DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                                "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCD80_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCD80_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD 80");


   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD170DatasetFiles, pythiaQCD170DatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                                "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCD170_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCD170_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD 170");

   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  
   //Reco Electron Fake Rates  
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();
   //pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCD_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD");

  //madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_MadgraphQCD_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Eta_MadgraphQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph QCD");


  //JetTrigger madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                               "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_Madgraph_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Eta_Madgraph_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph JetTrigger");


  //JetTrigger pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaJetTriggerDatasetFiles, pythiaJetTriggerDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_Pythia_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_Pythia_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia JetTrigger");



  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Reco Electron Fake Rates For different Sample Composition
  //*****************************************************************************************
  
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //QCDEM
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDEMEnrichedDatasetFiles, pythiaQCDEMEnrichedDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCDEMEnriched_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCDEMEnriched_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCDEMEnriched");

  //QCDBCE
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDBCEEnrichedDatasetFiles, pythiaQCDBCEEnrichedDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCDBCEEnriched_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCDBCEEnriched_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCDBCEEnriched");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Reco Electron Fake Rates For QCDBC, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //QCDEM 20To30
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDEMEnriched20To30DatasetFiles, pythiaQCDEMEnriched20To30DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCDEMEnriched20To30_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCDEMEnriched20To30_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCDEMEnriched20To30");

  //QCDEM 30To80
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDEMEnriched30To80DatasetFiles, pythiaQCDEMEnriched30To80DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCDEMEnriched30To80_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCDEMEnriched30To80_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCDEMEnriched30To80");

  //QCDEM 80To170
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDEMEnriched80To170DatasetFiles, pythiaQCDEMEnriched80To170DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCDEMEnriched80To170_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCDEMEnriched80To170_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCDEMEnriched80To170");

  //QCDBCE 20To30
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDBCEEnriched20To30DatasetFiles, pythiaQCDBCEEnriched20To30DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCDBCEEnriched20To30_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCDBCEEnriched20To30_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCDBCEEnriched20To30");

  //QCDBCE 30To80
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDBCEEnriched30To80DatasetFiles, pythiaQCDBCEEnriched30To80DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCDBCEEnriched30To80_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCDBCEEnriched30To80_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCDBCEEnriched30To80");

  //QCDBCE 80To170
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDBCEEnriched80To170DatasetFiles, pythiaQCDBCEEnriched80To170DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCDBCEEnriched80To170_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCDBCEEnriched80To170_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCDBCEEnriched80To170");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Reco Electron Fake Rates For QCD Madgraph, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_MadgraphQCD100To250_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Eta_MadgraphQCD100To250_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph QCD 100To250");


  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_MadgraphQCD250To500_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Eta_MadgraphQCD250To500_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph QCD 250To500");


  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_MadgraphQCD500To1000_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Eta_MadgraphQCD500To1000_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph QCD 500To1000");


  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Eta_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph QCD 1000ToInf");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);



  //*****************************************************************************************
  //Reco Electron Fake Rates For QCD Pythia, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD15DatasetFiles, pythiaQCD15DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCD15_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCD15_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD 15");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD30DatasetFiles, pythiaQCD30DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCD30_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCD30_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD 30");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD80DatasetFiles, pythiaQCD80DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCD80_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCD80_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD 80");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD170DatasetFiles, pythiaQCD170DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCD170_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCD170_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD 170");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);



  file->Close();
  return;

}



//--------------------------------------------------------------------------------------------------
// Do Fake Rate Computation From WJets sample
//--------------------------------------------------------------------------------------------------
void compute2DElectronFakeRatesFromQCD( char *FakeRateFilename ,bool fitFakeRate, string JetTrigger) {

  //Samples
  vector<string> madgraphJetTriggerDatasetFiles;
  vector<string> madgraphJetTriggerDatasetNames;
  vector<string> pythiaJetTriggerDatasetFiles;
  vector<string> pythiaJetTriggerDatasetNames;

  //*****************************************************************************************
  //Do FakeRate Computation 
  //*****************************************************************************************

  //Create root file to store fake rates
  TFile *file = new TFile(FakeRateFilename, "UPDATE");
  file->cd();
  
  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************
  madgraphJetTriggerDatasetFiles.clear(); 
  madgraphJetTriggerDatasetNames.clear();
  pythiaJetTriggerDatasetFiles.clear();
  pythiaJetTriggerDatasetNames.clear();
  
    //Madgraph QCD
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("f8-qcd100_250-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("f8-qcd1000_inf-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("s8-pj40_100-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("s8-pj100_200-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("s8-pj200_inf-id11");

  
  //Pythia QCD
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-qcd15-id11");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-qcd30-id11");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-qcd80-id11");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-qcd170-id11");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj15-id12_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-pj15-id12");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj30-id11_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-pj30-id11");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj80-id12_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-pj80-id12");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090527_JetTriggerSample/ComputeElectronFakeRate_JetTriggerSample_s8-pj170-id11_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-pj170-id11");
  
  //*****************************************************************************************
  //Construct the fake rate Histgrams. Fit with function.
  //*****************************************************************************************
  vector<TH2F*> fakeRateHistograms; fakeRateHistograms.clear();
  vector<TF2*> fakeRateFits; fakeRateFits.clear();
  vector<string> fakeRateLegendNames; fakeRateLegendNames.clear();
  TH2F *FakeRatePtEta_pythia = NULL;
  TF2 *FakeRateFitFunction_pythia = NULL;
  TH2F *FakeRatePtEta_madgraph = NULL;
  TF2 *FakeRateFitFunction_madgraph = NULL;

  //*****************************************************************************************
  //GsfTrack + SC Electron Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();


//   //pythia version
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaJetTriggerDatasetFiles, pythiaJetTriggerDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                              "hElectronNumeratorPtEta","hElectronDenominatorPtEta", ("GsfTrackSCElectronFakeRate_PtEta_Pythia_" + JetTrigger).c_str());
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateHist(FakeRatePtEta_pythia);
    //FakeRateFitFunction_pythia->SetName("GsfTrackSCElectronFakeRateFunction_PtEta_PythiaQCD");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia " + JetTrigger);
  

  //madgraph version
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, ("ComputeElectronFakeRateMod_" + JetTrigger).c_str(),
                                             "hElectronNumeratorPtEta","hElectronDenominatorPtEta", ("GsfTrackSCElectronFakeRate_PtEta_Madgraph_" + JetTrigger).c_str());
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePtEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphQCD_" + JetTrigger).c_str());
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph " + JetTrigger);
    
  printFakeRate(fakeRateHistograms,fakeRateFits,fakeRateLegendNames, ("GsfTrackSCElectronFakeRatePtEta_" + JetTrigger).c_str());
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Reco Electron Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //pythia version
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaJetTriggerDatasetFiles, pythiaJetTriggerDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                              "hElectronNumeratorPtEta","hElectronDenominatorPtEta", ("RecoElectronFakeRate_PtEta_Pythia_" + JetTrigger).c_str());
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    // FakeRateFitFunction_pythia = fitFakeRateHist(FakeRatePtEta_pythia);
    // FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaWJets_" + JetTrigger).c_str());
    // fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD " + JetTrigger);

  //madgraph version
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + JetTrigger).c_str(),
                                             "hElectronNumeratorPtEta","hElectronDenominatorPtEta", ("RecoElectronFakeRate_PtEta_Madgraph_" + JetTrigger).c_str());
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePtEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_MadgraphQCD_" + JetTrigger).c_str());
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph QCD " + JetTrigger);

  printFakeRate(fakeRateHistograms,fakeRateFits,fakeRateLegendNames, "RecoElectronFakeRatePt " + JetTrigger);
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  file->Close();
  return;

}


void compute1DElectronFakeRatesFromPhotonSample( char *FakeRateFilename ,bool fitFakeRate, string PhotonTrigger) {

  //Samples
  vector<string> madgraphDatasetFiles;
  vector<string> madgraphDatasetNames;

  vector<string> madgraphPhotonJetsDatasetFiles;
  vector<string> madgraphPhotonJetsDatasetNames;
  vector<string> madgraphQCDDatasetFiles;
  vector<string> madgraphQCDDatasetNames;

  vector<string> madgraphQCD100To250DatasetFiles;
  vector<string> madgraphQCD100To250DatasetNames;
  vector<string> madgraphQCD250To500DatasetFiles;
  vector<string> madgraphQCD250To500DatasetNames;
  vector<string> madgraphQCD500To1000DatasetFiles;
  vector<string> madgraphQCD500To1000DatasetNames;
  vector<string> madgraphQCD1000ToInfDatasetFiles;
  vector<string> madgraphQCD1000ToInfDatasetNames;

  vector<string> madgraphPhotonJets40To100DatasetFiles;
  vector<string> madgraphPhotonJets40To100DatasetNames;
  vector<string> madgraphPhotonJets100To200DatasetFiles;
  vector<string> madgraphPhotonJets100To200DatasetNames;
  vector<string> madgraphPhotonJets200ToInfDatasetFiles;
  vector<string> madgraphPhotonJets200ToInfDatasetNames;


  vector<string> pythiaDatasetFiles;
  vector<string> pythiaDatasetNames;

  vector<string> pythiaPhotonJetsDatasetFiles;
  vector<string> pythiaPhotonJetsDatasetNames;
  vector<string> pythiaQCDDatasetFiles;
  vector<string> pythiaQCDDatasetNames;

  vector<string> pythiaQCD15DatasetFiles;
  vector<string> pythiaQCD15DatasetNames;
  vector<string> pythiaQCD30DatasetFiles;
  vector<string> pythiaQCD30DatasetNames;
  vector<string> pythiaQCD80DatasetFiles;
  vector<string> pythiaQCD80DatasetNames;
  vector<string> pythiaQCD170DatasetFiles;
  vector<string> pythiaQCD170DatasetNames;

  vector<string> pythiaPhotonJets15DatasetFiles;
  vector<string> pythiaPhotonJets15DatasetNames;
  vector<string> pythiaPhotonJets30DatasetFiles;
  vector<string> pythiaPhotonJets30DatasetNames;
  vector<string> pythiaPhotonJets80DatasetFiles;
  vector<string> pythiaPhotonJets80DatasetNames;
  vector<string> pythiaPhotonJets170DatasetFiles;
  vector<string> pythiaPhotonJets170DatasetNames;


  //*****************************************************************************************
  //Do FakeRate Computation 
  //*****************************************************************************************

  //Create root file to store fake rates
  TFile *file = new TFile(FakeRateFilename, "UPDATE");
  file->cd();
  
  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************
  madgraphDatasetFiles.clear();
  madgraphDatasetNames.clear();
  madgraphPhotonJetsDatasetFiles.clear();
  madgraphPhotonJetsDatasetNames.clear();
  madgraphQCDDatasetFiles.clear();
  madgraphQCDDatasetNames.clear();
  
  madgraphPhotonJets40To100DatasetFiles.clear();
  madgraphPhotonJets40To100DatasetNames.clear();
  madgraphPhotonJets100To200DatasetFiles.clear();
  madgraphPhotonJets100To200DatasetNames.clear();
  madgraphPhotonJets200ToInfDatasetFiles.clear();
  madgraphPhotonJets200ToInfDatasetNames.clear();
  madgraphQCD100To250DatasetFiles.clear();
  madgraphQCD100To250DatasetNames.clear();
  madgraphQCD250To500DatasetFiles.clear();
  madgraphQCD250To500DatasetNames.clear();
  madgraphQCD500To1000DatasetFiles.clear();
  madgraphQCD500To1000DatasetNames.clear();
  madgraphQCD1000ToInfDatasetFiles.clear();
  madgraphQCD1000ToInfDatasetNames.clear();
  

  pythiaDatasetFiles.clear();
  pythiaDatasetNames.clear();
  pythiaPhotonJetsDatasetFiles.clear();
  pythiaPhotonJetsDatasetNames.clear();
  pythiaQCDDatasetFiles.clear();
  pythiaQCDDatasetNames.clear();
  
  pythiaPhotonJets15DatasetFiles.clear();
  pythiaPhotonJets15DatasetNames.clear();
  pythiaPhotonJets30DatasetFiles.clear();
  pythiaPhotonJets30DatasetNames.clear();
  pythiaPhotonJets80DatasetFiles.clear();
  pythiaPhotonJets80DatasetNames.clear();
  pythiaPhotonJets170DatasetFiles.clear();
  pythiaPhotonJets170DatasetNames.clear();
  pythiaQCD15DatasetFiles.clear();
  pythiaQCD15DatasetNames.clear();
  pythiaQCD30DatasetFiles.clear();
  pythiaQCD30DatasetNames.clear();
  pythiaQCD80DatasetFiles.clear();
  pythiaQCD80DatasetNames.clear();
  pythiaQCD170DatasetFiles.clear();
  pythiaQCD170DatasetNames.clear();
  

  //All samples Together
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-pj40_100-id11");
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-pj100_200-id11");
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-pj200_inf-id11");
//   madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd100_250-id11_noskim.root");
//   madgraphDatasetNames.push_back("f8-qcd100_250-id11");
//   madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd250_500-id11_noskim.root");
//   madgraphDatasetNames.push_back("f8-qcd250_500-id11");
//   madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd500_1000-id11_noskim.root");
//   madgraphDatasetNames.push_back("f8-qcd500_1000-id11");
//   madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd1000_inf-id11_noskim.root");
//   madgraphDatasetNames.push_back("f8-qcd1000_inf-id11");

  //Madgraph PhotonJet
  madgraphPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphPhotonJetsDatasetNames.push_back("s8-pj40_100-id11");
  madgraphPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphPhotonJetsDatasetNames.push_back("s8-pj100_200-id11");
  madgraphPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphPhotonJetsDatasetNames.push_back("s8-pj200_inf-id11");
  madgraphPhotonJets40To100DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphPhotonJets40To100DatasetNames.push_back("s8-pj40_100-id11");
  madgraphPhotonJets100To200DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphPhotonJets100To200DatasetNames.push_back("s8-pj100_200-id11");
  madgraphPhotonJets200ToInfDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphPhotonJets200ToInfDatasetNames.push_back("s8-pj200_inf-id11");


  //Madgraph QCD
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd1000_inf-id11");
  madgraphQCD100To250DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphQCD100To250DatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCD250To500DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphQCD250To500DatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCD500To1000DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphQCD500To1000DatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphQCD1000ToInfDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphQCD1000ToInfDatasetNames.push_back("f8-qcd1000_inf-id11");



    //All samples Together
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj15-id12_noskim.root");
  pythiaDatasetNames.push_back("s8-pj15-id12");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj30-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-pj30-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj80-id12_noskim.root");
  pythiaDatasetNames.push_back("s8-pj80-id12");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj170-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-pj170-id11");
//   pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd15-id11_noskim.root");
//   pythiaDatasetNames.push_back("s8-qcd15-id11");
//   pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd30-id11_noskim.root");
//   pythiaDatasetNames.push_back("s8-qcd30-id11");
//   pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd80-id11_noskim.root");
//   pythiaDatasetNames.push_back("s8-qcd80-id11");
//   pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd170-id11_noskim.root");
//   pythiaDatasetNames.push_back("s8-qcd170-id11");

  //Pythia PhotonJet
  pythiaPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj15-id12_noskim.root");
  pythiaPhotonJetsDatasetNames.push_back("s8-pj15-id12");
  pythiaPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj30-id11_noskim.root");
  pythiaPhotonJetsDatasetNames.push_back("s8-pj30-id11");
  pythiaPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj80-id12_noskim.root");
  pythiaPhotonJetsDatasetNames.push_back("s8-pj80-id12");
  pythiaPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj170-id11_noskim.root");
  pythiaPhotonJetsDatasetNames.push_back("s8-pj170-id11");
  pythiaPhotonJets15DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj15-id12_noskim.root");
  pythiaPhotonJets15DatasetNames.push_back("s8-pj15-id12");
  pythiaPhotonJets30DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj30-id11_noskim.root");
  pythiaPhotonJets30DatasetNames.push_back("s8-pj30-id11");
  pythiaPhotonJets80DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj80-id12_noskim.root");
  pythiaPhotonJets80DatasetNames.push_back("s8-pj80-id12");
  pythiaPhotonJets170DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj170-id11_noskim.root");
  pythiaPhotonJets170DatasetNames.push_back("s8-pj170-id11");


  //Pythia QCD
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd15-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd30-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd80-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd170-id11");
  pythiaQCD15DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaQCD15DatasetNames.push_back("s8-qcd15-id11");
  pythiaQCD30DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaQCD30DatasetNames.push_back("s8-qcd30-id11");
  pythiaQCD80DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaQCD80DatasetNames.push_back("s8-qcd80-id11");
  pythiaQCD170DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaQCD170DatasetNames.push_back("s8-qcd170-id11");

  
  //*****************************************************************************************
  //Construct the fake rate Histgrams. Fit with function.
  //*****************************************************************************************
  vector<TH1F*> fakeRateHistograms; fakeRateHistograms.clear();
  vector<TF1*> fakeRateFits; fakeRateFits.clear();
  vector<string> fakeRateLegendNames; fakeRateLegendNames.clear();
  TH1F *FakeRatePt_pythia = NULL;
  TF1 *FakeRateFitFunction_pythia = NULL;
  TH1F *FakeRatePt_madgraph = NULL;
  TF1 *FakeRateFitFunction_madgraph = NULL;
  TH1F *FakeRateEta_pythia = NULL;
  TH1F *FakeRateEta_madgraph = NULL;



  //*****************************************************************************************
  //GsfTrack + SC Electron Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphDatasetFiles, madgraphDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_Madgraph_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphQCD_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD");

  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaDatasetFiles, pythiaDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_Pythia_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCD_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD");

  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //GsfTrack + SC Electron Fake Rates For QCD vs PhotonJets
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();
  
  //PhotonJets
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJetsDatasetFiles, madgraphPhotonJetsDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_MadgraphPhotonJets_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron MadgraphPhotonJets");
  
//   //QCD
//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
//                                            "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_madgraphQCD_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_madgraphQCD_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDEMEnriched");
  
 
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


   //*****************************************************************************************
   //GsfTrack + SC Electron Fake Rates For PhotonJets, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();
   //PhotonJets 40To100
   FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJets40To100DatasetFiles, madgraphPhotonJets40To100DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
   FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
   }
   fakeRateHistograms.push_back(FakeRatePt_madgraph);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph PhotonJets40To100");


   //PhotonJets 100To200
   FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJets100To200DatasetFiles, madgraphPhotonJets100To200DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
   FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
   }
   fakeRateHistograms.push_back(FakeRatePt_madgraph);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph PhotonJets100To200");


   //PhotonJets 200ToInf
   FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJets200ToInfDatasetFiles, madgraphPhotonJets200ToInfDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
   FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
   }
   fakeRateHistograms.push_back(FakeRatePt_madgraph);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph PhotonJets200ToInf");


   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


//    //*****************************************************************************************
//    //GsfTrack + SC Electron Fake Rates For madgraph QCD, different pthat bins
//    //*****************************************************************************************
//    fakeRateHistograms.clear();
//    fakeRateFits.clear();
//    fakeRateLegendNames.clear();

//    FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
//                                              "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//    FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

//    FakeRateFitFunction_madgraph = NULL;
//    if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD 100To250");


//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
//                                              "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD 250To500");


//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
//                                              "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD 500To1000");


//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
//                                              "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD 1000ToInf");


//   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //GsfTrack + SC Electron Fake Rates For QCD vs PhotonJets
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();
  
  //PhotonJets
  FakeRatePt_pythia = createFakeRatePtHist(pythiaPhotonJetsDatasetFiles, pythiaPhotonJetsDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaPhotonJets_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron PythiaPhotonJets");
  
  //QCD
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_pythiaQCD_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_pythiaQCD_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD");
  
 
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


   //*****************************************************************************************
   //GsfTrack + SC Electron Fake Rates For PhotonJets, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();
   //PhotonJets 15
   FakeRatePt_pythia = createFakeRatePtHist(pythiaPhotonJets15DatasetFiles, pythiaPhotonJets15DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaPhotonJets15_" + PhotonTrigger).c_str());
   FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaPhotonJets15_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRatePt_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia PhotonJets15");


   //PhotonJets 30
   FakeRatePt_pythia = createFakeRatePtHist(pythiaPhotonJets30DatasetFiles, pythiaPhotonJets30DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaPhotonJets30_" + PhotonTrigger).c_str());
   FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaPhotonJets30_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRatePt_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia PhotonJets30");


   //PhotonJets 80
   FakeRatePt_pythia = createFakeRatePtHist(pythiaPhotonJets80DatasetFiles, pythiaPhotonJets80DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaPhotonJets80_" + PhotonTrigger).c_str());
   FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaPhotonJets80_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRatePt_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia PhotonJets80");


   //PhotonJets 170
   FakeRatePt_pythia = createFakeRatePtHist(pythiaPhotonJets170DatasetFiles, pythiaPhotonJets170DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaPhotonJets170_" + PhotonTrigger).c_str());
   FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaPhotonJets170_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRatePt_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia PhotonJets170");


   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


   //*****************************************************************************************
   //GsfTrack + SC Electron Fake Rates For pythia QCD, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();

   FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD15DatasetFiles, pythiaQCD15DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCD15_" + PhotonTrigger).c_str());
   FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCD15_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD 15");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD30DatasetFiles, pythiaQCD30DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCD30_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCD30_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD 30");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD80DatasetFiles, pythiaQCD80DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCD80_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCD80_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD 80");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD170DatasetFiles, pythiaQCD170DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hElectronNumeratorPt","hElectronDenominatorPt", ("GsfTrackSCElectronFakeRate_Pt_PythiaQCD170_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Pt_PythiaQCD170_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD 170");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Reco Electron Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphDatasetFiles, madgraphDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_Madgraph_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_Madgraph_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph");


  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaDatasetFiles, pythiaDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_Pythia_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_Pythia_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Reco Electron Fake Rates For different Sample Composition
  //*****************************************************************************************
  
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //PhotonJets
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJetsDatasetFiles, madgraphPhotonJetsDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_MadgraphPhotonJets_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_MadgraphPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph PhotonJets");

//   //QCD
//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
//                                            "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_MadgraphQCD_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_MadgraphQCD_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("RecoElectron Madgraph QCD");

  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Reco Electron Fake Rates For PhotonJets, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //PhotonJets40To100
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJets40To100DatasetFiles, madgraphPhotonJets40To100DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph PhotonJets40To100");

  //PhotonJets100To200
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJets100To200DatasetFiles, madgraphPhotonJets100To200DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph PhotonJets100To200");

  //PhotonJets200ToInf
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJets200ToInfDatasetFiles, madgraphPhotonJets200ToInfDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph PhotonJets200ToInf");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

//   //*****************************************************************************************
//   //Reco Electron Fake Rates For Madgraph QCD, different pthat bins
//   //*****************************************************************************************
//   fakeRateHistograms.clear();
//   fakeRateFits.clear();
//   fakeRateLegendNames.clear();


//   //madgraph version
//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
//                                            "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("RecoElectron Madgraph QCD 100To250");


//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
//                                            "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("RecoElectron Madgraph QCD 250To500");


//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
//                                            "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("RecoElectron Madgraph QCD 500To1000");


//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
//                                            "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("RecoElectron Madgraph QCD 1000ToInf");


//   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);






  //*****************************************************************************************
  //Reco Electron Fake Rates For different Sample Composition
  //*****************************************************************************************
  
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //PhotonJets
  FakeRatePt_pythia = createFakeRatePtHist(pythiaPhotonJetsDatasetFiles, pythiaPhotonJetsDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaPhotonJets_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia PhotonJets");

  //QCD
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCD_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCD_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD");

  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Reco Electron Fake Rates For PhotonJets, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //PhotonJets15
  FakeRatePt_pythia = createFakeRatePtHist(pythiaPhotonJets15DatasetFiles, pythiaPhotonJets15DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaPhotonJets15_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaPhotonJets15_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia PhotonJets15");

  //PhotonJets30
  FakeRatePt_pythia = createFakeRatePtHist(pythiaPhotonJets30DatasetFiles, pythiaPhotonJets30DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaPhotonJets30_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaPhotonJets30_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia PhotonJets30");

  //PhotonJets80
  FakeRatePt_pythia = createFakeRatePtHist(pythiaPhotonJets80DatasetFiles, pythiaPhotonJets80DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaPhotonJets80_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaPhotonJets80_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia PhotonJets80");


  //PhotonJets170
  FakeRatePt_pythia = createFakeRatePtHist(pythiaPhotonJets80DatasetFiles, pythiaPhotonJets170DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaPhotonJets170_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaPhotonJets170_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia PhotonJets170");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Reco Electron Fake Rates For Pythia QCD, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();


  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD15DatasetFiles, pythiaQCD15DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCD15_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCD15_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD 15");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD30DatasetFiles, pythiaQCD30DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCD30_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCD30_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD 30");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD80DatasetFiles, pythiaQCD80DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCD80_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCD80_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD 80");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD170DatasetFiles, pythiaQCD170DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorPt","hElectronDenominatorPt", ("RecoElectronFakeRate_Pt_PythiaQCD170_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_PythiaQCD170_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD 170");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);




  //*****************************************************************************************
  //GsfTrack + SC Electron Fake Rates Vs Eta
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphDatasetFiles, madgraphDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_Madgraph_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphQCD_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD");


  //pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaDatasetFiles, pythiaDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_Pythia_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCD_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD");


   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //GsfTrack + SC Electron Fake Rates For QCD vs PhotonJets
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();
  
  //PhotonJets
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJetsDatasetFiles, madgraphPhotonJetsDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_MadgraphPhotonJets_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron MadgraphPhotonJets");
  
//   //QCD
//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
//                                            "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_madgraphQCD_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_madgraphQCD_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCDEMEnriched");
  
 
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


   //*****************************************************************************************
   //GsfTrack + SC Electron Fake Rates For PhotonJets, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();
   //PhotonJets 40To100
   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJets40To100DatasetFiles, madgraphPhotonJets40To100DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
   FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
   }
   fakeRateHistograms.push_back(FakeRateEta_madgraph);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph PhotonJets40To100");


   //PhotonJets 100To200
   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJets100To200DatasetFiles, madgraphPhotonJets100To200DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
   FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
   }
   fakeRateHistograms.push_back(FakeRateEta_madgraph);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph PhotonJets100To200");


   //PhotonJets 200ToInf
   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJets200ToInfDatasetFiles, madgraphPhotonJets200ToInfDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
   FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
   }
   fakeRateHistograms.push_back(FakeRateEta_madgraph);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph PhotonJets200ToInf");


   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


//    //*****************************************************************************************
//    //GsfTrack + SC Electron Fake Rates For madgraph QCD, different pthat bins
//    //*****************************************************************************************
//    fakeRateHistograms.clear();
//    fakeRateFits.clear();
//    fakeRateLegendNames.clear();

//    FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
//                                              "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//    FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

//    FakeRateFitFunction_madgraph = NULL;
//    if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD 100To250");


//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
//                                              "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD 250To500");


//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
//                                              "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD 500To1000");


//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
//                                              "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph QCD 1000ToInf");


//   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //GsfTrack + SC Electron Fake Rates For QCD vs PhotonJets
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();
  
  //PhotonJets
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaPhotonJetsDatasetFiles, pythiaPhotonJetsDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaPhotonJets_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron PythiaPhotonJets");
  
  //QCD
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_pythiaQCD_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_pythiaQCD_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD");
  
 
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


   //*****************************************************************************************
   //GsfTrack + SC Electron Fake Rates For PhotonJets, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();
   //PhotonJets 15
   FakeRateEta_pythia = createFakeRateEtaHist(pythiaPhotonJets15DatasetFiles, pythiaPhotonJets15DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaPhotonJets15_" + PhotonTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaPhotonJets15_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia PhotonJets15");


   //PhotonJets 30
   FakeRateEta_pythia = createFakeRateEtaHist(pythiaPhotonJets30DatasetFiles, pythiaPhotonJets30DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaPhotonJets30_" + PhotonTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaPhotonJets30_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia PhotonJets30");


   //PhotonJets 80
   FakeRateEta_pythia = createFakeRateEtaHist(pythiaPhotonJets80DatasetFiles, pythiaPhotonJets80DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaPhotonJets80_" + PhotonTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaPhotonJets80_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia PhotonJets80");


   //PhotonJets 170
   FakeRateEta_pythia = createFakeRateEtaHist(pythiaPhotonJets170DatasetFiles, pythiaPhotonJets170DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaPhotonJets170_" + PhotonTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaPhotonJets170_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
   }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia PhotonJets170");


   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


   //*****************************************************************************************
   //GsfTrack + SC Electron Fake Rates For pythia QCD, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();

   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD15DatasetFiles, pythiaQCD15DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCD15_" + PhotonTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    

   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCD15_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD 15");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD30DatasetFiles, pythiaQCD30DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCD30_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCD30_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD 30");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD80DatasetFiles, pythiaQCD80DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCD80_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCD80_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD 80");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD170DatasetFiles, pythiaQCD170DatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hElectronNumeratorEta","hElectronDenominatorEta", ("GsfTrackSCElectronFakeRate_Eta_PythiaQCD170_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("GsfTrackSCElectronFakeRate");    
  
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GsfTrackSCElectronFakeRateFunction_Eta_PythiaQCD170_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia QCD 170");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Reco Electron Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphDatasetFiles, madgraphDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_Madgraph_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Eta_Madgraph_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph");


  //pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaDatasetFiles, pythiaDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_Pythia_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_Pythia_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Reco Electron Fake Rates For different Sample Composition
  //*****************************************************************************************
  
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //PhotonJets
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJetsDatasetFiles, madgraphPhotonJetsDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_MadgraphPhotonJets_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Eta_MadgraphPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph PhotonJets");

//   //QCD
//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
//                                            "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_MadgraphQCD_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Eta_MadgraphQCD_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("RecoElectron Madgraph QCD");

  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Reco Electron Fake Rates For PhotonJets, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //PhotonJets40To100
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJets40To100DatasetFiles, madgraphPhotonJets40To100DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Eta_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph PhotonJets40To100");

  //PhotonJets100To200
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJets100To200DatasetFiles, madgraphPhotonJets100To200DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Eta_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph PhotonJets100To200");

  //PhotonJets200ToInf
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJets200ToInfDatasetFiles, madgraphPhotonJets200ToInfDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Eta_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph PhotonJets200ToInf");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

//   //*****************************************************************************************
//   //Reco Electron Fake Rates For Madgraph QCD, different pthat bins
//   //*****************************************************************************************
//   fakeRateHistograms.clear();
//   fakeRateFits.clear();
//   fakeRateLegendNames.clear();


//   //madgraph version
//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
//                                            "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Eta_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("RecoElectron Madgraph QCD 100To250");


//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
//                                            "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Eta_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("RecoElectron Madgraph QCD 250To500");


//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
//                                            "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Eta_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("RecoElectron Madgraph QCD 500To1000");


//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
//                                            "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Eta_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("RecoElectron Madgraph QCD 1000ToInf");


//   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);



  //*****************************************************************************************
  //Reco Electron Fake Rates For different Sample Composition
  //*****************************************************************************************
  
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //PhotonJets
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaPhotonJetsDatasetFiles, pythiaPhotonJetsDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaPhotonJets_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia PhotonJets");

  //QCD
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCD_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCD_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD");

  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Reco Electron Fake Rates For PhotonJets, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //PhotonJets15
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaPhotonJets15DatasetFiles, pythiaPhotonJets15DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaPhotonJets15_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaPhotonJets15_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia PhotonJets15");

  //PhotonJets30
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaPhotonJets30DatasetFiles, pythiaPhotonJets30DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaPhotonJets30_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaPhotonJets30_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia PhotonJets30");

  //PhotonJets80
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaPhotonJets80DatasetFiles, pythiaPhotonJets80DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaPhotonJets80_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaPhotonJets80_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia PhotonJets80");


  //PhotonJets170
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaPhotonJets170DatasetFiles, pythiaPhotonJets80DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaPhotonJets170_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaPhotonJets170_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia PhotonJets170");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Reco Electron Fake Rates For Pythia QCD, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();


  //pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD15DatasetFiles, pythiaQCD15DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCD15_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCD15_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD 15");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD30DatasetFiles, pythiaQCD30DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCD30_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCD30_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD 30");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD80DatasetFiles, pythiaQCD80DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCD80_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCD80_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD 80");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD170DatasetFiles, pythiaQCD170DatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                           "hElectronNumeratorEta","hElectronDenominatorEta", ("RecoElectronFakeRate_Eta_PythiaQCD170_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Electron Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Eta_PythiaQCD170_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia QCD 170");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);




  file->Close();
  return;

}


//--------------------------------------------------------------------------------------------------
// Do Fake Rate Computation From WJets sample
//--------------------------------------------------------------------------------------------------
void compute2DElectronFakeRatesFromPhotonSample( char *FakeRateFilename ,bool fitFakeRate, string PhotonTrigger) {

  //Samples
  vector<string> madgraphDatasetFiles;
  vector<string> madgraphDatasetNames;

  vector<string> madgraphPhotonJetsDatasetFiles;
  vector<string> madgraphPhotonJetsDatasetNames;
  vector<string> madgraphQCDDatasetFiles;
  vector<string> madgraphQCDDatasetNames;

  vector<string> madgraphQCD100To250DatasetFiles;
  vector<string> madgraphQCD100To250DatasetNames;
  vector<string> madgraphQCD250To500DatasetFiles;
  vector<string> madgraphQCD250To500DatasetNames;
  vector<string> madgraphQCD500To1000DatasetFiles;
  vector<string> madgraphQCD500To1000DatasetNames;
  vector<string> madgraphQCD1000ToInfDatasetFiles;
  vector<string> madgraphQCD1000ToInfDatasetNames;

  vector<string> madgraphPhotonJets40To100DatasetFiles;
  vector<string> madgraphPhotonJets40To100DatasetNames;
  vector<string> madgraphPhotonJets100To200DatasetFiles;
  vector<string> madgraphPhotonJets100To200DatasetNames;
  vector<string> madgraphPhotonJets200ToInfDatasetFiles;
  vector<string> madgraphPhotonJets200ToInfDatasetNames;


  vector<string> pythiaDatasetFiles;
  vector<string> pythiaDatasetNames;

  vector<string> pythiaPhotonJetsDatasetFiles;
  vector<string> pythiaPhotonJetsDatasetNames;
  vector<string> pythiaQCDDatasetFiles;
  vector<string> pythiaQCDDatasetNames;

  vector<string> pythiaQCD15DatasetFiles;
  vector<string> pythiaQCD15DatasetNames;
  vector<string> pythiaQCD30DatasetFiles;
  vector<string> pythiaQCD30DatasetNames;
  vector<string> pythiaQCD80DatasetFiles;
  vector<string> pythiaQCD80DatasetNames;
  vector<string> pythiaQCD170DatasetFiles;
  vector<string> pythiaQCD170DatasetNames;

  vector<string> pythiaPhotonJets15DatasetFiles;
  vector<string> pythiaPhotonJets15DatasetNames;
  vector<string> pythiaPhotonJets30DatasetFiles;
  vector<string> pythiaPhotonJets30DatasetNames;
  vector<string> pythiaPhotonJets80DatasetFiles;
  vector<string> pythiaPhotonJets80DatasetNames;
  vector<string> pythiaPhotonJets170DatasetFiles;
  vector<string> pythiaPhotonJets170DatasetNames;

  //*****************************************************************************************
  //Do FakeRate Computation 
  //*****************************************************************************************

  //Create root file to store fake rates
  TFile *file = new TFile(FakeRateFilename, "UPDATE");
  file->cd();
  
  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************
  madgraphDatasetFiles.clear();
  madgraphDatasetNames.clear();
  madgraphPhotonJetsDatasetFiles.clear();
  madgraphPhotonJetsDatasetNames.clear();
  madgraphQCDDatasetFiles.clear();
  madgraphQCDDatasetNames.clear();
  
  madgraphQCD100To250DatasetFiles.clear();
  madgraphQCD100To250DatasetNames.clear();
  madgraphQCD250To500DatasetFiles.clear();
  madgraphQCD250To500DatasetNames.clear();
  madgraphQCD500To1000DatasetFiles.clear();
  madgraphQCD500To1000DatasetNames.clear();
  madgraphQCD1000ToInfDatasetFiles.clear();
  madgraphQCD1000ToInfDatasetNames.clear();
  madgraphPhotonJets40To100DatasetFiles.clear();
  madgraphPhotonJets40To100DatasetNames.clear();
  madgraphPhotonJets100To200DatasetFiles.clear();
  madgraphPhotonJets100To200DatasetNames.clear();
  madgraphPhotonJets200ToInfDatasetFiles.clear();
  madgraphPhotonJets200ToInfDatasetNames.clear();

  pythiaDatasetFiles.clear();
  pythiaDatasetNames.clear();
  pythiaPhotonJetsDatasetFiles.clear();
  pythiaPhotonJetsDatasetNames.clear();
  pythiaQCDDatasetFiles.clear();
  pythiaQCDDatasetNames.clear();
  
  pythiaQCD15DatasetFiles.clear();
  pythiaQCD15DatasetNames.clear();
  pythiaQCD30DatasetFiles.clear();
  pythiaQCD30DatasetNames.clear();
  pythiaQCD80DatasetFiles.clear();
  pythiaQCD80DatasetNames.clear();
  pythiaQCD170DatasetFiles.clear();
  pythiaQCD170DatasetNames.clear();
  
  pythiaPhotonJets15DatasetFiles.clear();
  pythiaPhotonJets15DatasetNames.clear();
  pythiaPhotonJets30DatasetFiles.clear();
  pythiaPhotonJets30DatasetNames.clear();
  pythiaPhotonJets80DatasetFiles.clear();
  pythiaPhotonJets80DatasetNames.clear();
  pythiaPhotonJets170DatasetFiles.clear();
  pythiaPhotonJets170DatasetNames.clear();

  //All samples Together
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-pj40_100-id11");
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-pj100_200-id11");
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-pj200_inf-id11");
//   madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd100_250-id11_noskim.root");
//   madgraphDatasetNames.push_back("f8-qcd100_250-id11");
//   madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd250_500-id11_noskim.root");
//   madgraphDatasetNames.push_back("f8-qcd250_500-id11");
//   madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd500_1000-id11_noskim.root");
//   madgraphDatasetNames.push_back("f8-qcd500_1000-id11");
//   madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd1000_inf-id11_noskim.root");
//   madgraphDatasetNames.push_back("f8-qcd1000_inf-id11");

  //Madgraph PhotonJet
  madgraphPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphPhotonJetsDatasetNames.push_back("s8-pj40_100-id11");
  madgraphPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphPhotonJetsDatasetNames.push_back("s8-pj100_200-id11");
  madgraphPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphPhotonJetsDatasetNames.push_back("s8-pj200_inf-id11");
  madgraphPhotonJets40To100DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphPhotonJets40To100DatasetNames.push_back("s8-pj40_100-id11");
  madgraphPhotonJets100To200DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphPhotonJets100To200DatasetNames.push_back("s8-pj100_200-id11");
  madgraphPhotonJets200ToInfDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphPhotonJets200ToInfDatasetNames.push_back("s8-pj200_inf-id11");


  //Madgraph QCD
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd1000_inf-id11");
  madgraphQCD100To250DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphQCD100To250DatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCD250To500DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphQCD250To500DatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCD500To1000DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphQCD500To1000DatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphQCD1000ToInfDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphQCD1000ToInfDatasetNames.push_back("f8-qcd1000_inf-id11");
   


  //All samples Together
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj15-id12_noskim.root");
  pythiaDatasetNames.push_back("s8-pj15-id12");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj30-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-pj30-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj80-id12_noskim.root");
  pythiaDatasetNames.push_back("s8-pj80-id12");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj170-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-pj170-id11");
//   pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd15-id11_noskim.root");
//   pythiaDatasetNames.push_back("s8-qcd15-id11");
//   pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd30-id11_noskim.root");
//   pythiaDatasetNames.push_back("s8-qcd30-id11");
//   pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd80-id11_noskim.root");
//   pythiaDatasetNames.push_back("s8-qcd80-id11");
//   pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd170-id11_noskim.root");
//   pythiaDatasetNames.push_back("s8-qcd170-id11");

  //Pythia PhotonJet
  pythiaPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj15-id12_noskim.root");
  pythiaPhotonJetsDatasetNames.push_back("s8-pj15-id12");
  pythiaPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj30-id11_noskim.root");
  pythiaPhotonJetsDatasetNames.push_back("s8-pj30-id11");
  pythiaPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj80-id12_noskim.root");
  pythiaPhotonJetsDatasetNames.push_back("s8-pj80-id12");
  pythiaPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj170-id11_noskim.root");
  pythiaPhotonJetsDatasetNames.push_back("s8-pj170-id11");
  pythiaPhotonJets15DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj15-id12_noskim.root");
  pythiaPhotonJets15DatasetNames.push_back("s8-pj15-id12");
  pythiaPhotonJets30DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj30-id11_noskim.root");
  pythiaPhotonJets30DatasetNames.push_back("s8-pj30-id11");
  pythiaPhotonJets80DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj80-id12_noskim.root");
  pythiaPhotonJets80DatasetNames.push_back("s8-pj80-id12");
  pythiaPhotonJets170DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-pj170-id11_noskim.root");
  pythiaPhotonJets170DatasetNames.push_back("s8-pj170-id11");


  //Pythia QCD
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd15-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd30-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd80-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd170-id11");
  pythiaQCD15DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaQCD15DatasetNames.push_back("s8-qcd15-id11");
  pythiaQCD30DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaQCD30DatasetNames.push_back("s8-qcd30-id11");
  pythiaQCD80DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaQCD80DatasetNames.push_back("s8-qcd80-id11");
  pythiaQCD170DatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/009/save_20090522_PhotonTriggerSample/ComputeElectronFakeRate_PhotonTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaQCD170DatasetNames.push_back("s8-qcd170-id11");


  //*****************************************************************************************
  //Construct the fake rate Histgrams. Fit with function.
  //*****************************************************************************************
  vector<TH2F*> fakeRateHistograms; fakeRateHistograms.clear();
  vector<TF2*> fakeRateFits; fakeRateFits.clear();
  vector<string> fakeRateLegendNames; fakeRateLegendNames.clear();
  TH2F *FakeRatePtEta_pythia = NULL;
  TF2 *FakeRateFitFunction_pythia = NULL;
  TH2F *FakeRatePtEta_madgraph = NULL;
  TF2 *FakeRateFitFunction_madgraph = NULL;

  //*****************************************************************************************
  //GsfTrack + SC Electron Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();


  //madgraph version
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist(madgraphDatasetFiles, madgraphDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                              "hElectronNumeratorPtEta","hElectronDenominatorPtEta", ("GsfTrackSCElectronFakeRate_PtEta_Madgraph_" + PhotonTrigger).c_str());
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRateHist(FakeRatePtEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("GsfTrackSCElectronFakeRateFunction_PtEta_Madgraph");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Madgraph");


  //pythia version
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaDatasetFiles, pythiaDatasetNames, ("ComputeElectronFakeRateMod_" + PhotonTrigger).c_str(),
                                              "hElectronNumeratorPtEta","hElectronDenominatorPtEta", ("GsfTrackSCElectronFakeRate_PtEta_Pythia_" + PhotonTrigger).c_str());
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateHist(FakeRatePtEta_pythia);
    //FakeRateFitFunction_pythia->SetName("GsfTrackSCElectronFakeRateFunction_PtEta_Pythia");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("GsfTrackSCElectron Pythia");

      
  printFakeRate(fakeRateHistograms,fakeRateFits,fakeRateLegendNames, ("GsfTrackSCElectronFakeRatePtEta " + PhotonTrigger).c_str());
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Reco Electron Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph version
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist(madgraphDatasetFiles, madgraphDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                              "hElectronNumeratorPtEta","hElectronDenominatorPtEta", ("RecoElectronFakeRate_PtEta_Madgraph_" + PhotonTrigger).c_str());
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    // FakeRateFitFunction_madgraph = fitFakeRateHist(FakeRatePtEta_madgraph);
    // FakeRateFitFunction_madgraph->SetName(("RecoElectronFakeRateFunction_Pt_Madgraph_" + PhotonTrigger).c_str());
    // fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("RecoElectron Madgraph");


  //pythia version
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaDatasetFiles, pythiaDatasetNames, ("ComputeElectronFakeRateMod_Reco_" + PhotonTrigger).c_str(),
                                              "hElectronNumeratorPtEta","hElectronDenominatorPtEta", ("RecoElectronFakeRate_PtEta_Pythia_" + PhotonTrigger).c_str());
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    // FakeRateFitFunction_pythia = fitFakeRateHist(FakeRatePtEta_pythia);
    // FakeRateFitFunction_pythia->SetName(("RecoElectronFakeRateFunction_Pt_Pythia_" + PhotonTrigger).c_str());
    // fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("RecoElectron Pythia");


  printFakeRate(fakeRateHistograms,fakeRateFits,fakeRateLegendNames, "RecoElectronFakeRatePtEta " + PhotonTrigger);
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  file->Close();
  return;

}






// //--------------------------------------------------------------------------------------------------
// // Make plots of electron fake rate from many samples
// //--------------------------------------------------------------------------------------------------
void compareElectronFakeRates(char *FakeRateFilename) {

  string plotname;
  string filename;

  TFile *file = new TFile(FakeRateFilename, "READ");
  file->cd();

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);
  TLegend *leg1=0;
  leg1 = new TLegend(0.75,0.6,0.9,0.8);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);

  TH1F *electronFakeRate_Pt_pythiaWJets = 0;
  TH1F *electronFakeRate_Pt_madgraphWJets = 0;
  TH1F *electronFakeRate_Pt_pythiaQCDEnriched = 0;
  TH1F *electronFakeRate_Pt_madgraphQCD = 0;
  TH1F *electronFakeRate_Pt_madgraphPhotonJets = 0;
  TH1F *electronFakeRate_Pt_PhotonTrigger = 0;
  TH1F *electronFakeRate_Pt_JetTrigger = 0;
  TH1F *electronFakeRate_Eta_pythiaWJets = 0;
  TH1F *electronFakeRate_Eta_madgraphWJets = 0;
  TH1F *electronFakeRate_Eta_pythiaQCDEnriched = 0;
  TH1F *electronFakeRate_Eta_madgraphQCD = 0;
  TH1F *electronFakeRate_Eta_madgraphPhotonJets = 0;
  TH1F *electronFakeRate_Eta_PhotonTrigger = 0;
  TH1F *electronFakeRate_Eta_JetTrigger = 0;


  
  //*****************************************************************************************
  //Compare FakeRates For GsfSC Electron Fake Rates Vs Pt  
  //*****************************************************************************************

  leg1->Clear();
  leg1->SetX1NDC(0.75);  leg1->SetX2NDC(0.95); leg1->SetY1NDC(0.7);  leg1->SetY2NDC(0.9); 
  leg1->SetX1(0.75);  leg1->SetX2(0.95); leg1->SetY1(0.7);  leg1->SetY2(0.9); 
  plotname = "GsfElectronFakeRatePt";

   electronFakeRate_Pt_pythiaWJets = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Pt_PythiaWJets");
  electronFakeRate_Pt_madgraphWJets = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Pt_MadgraphWJets");
  electronFakeRate_Pt_pythiaQCDEnriched = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Pt_PythiaQCDEnriched_Jet50");
  electronFakeRate_Pt_madgraphQCD = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Pt_MadgraphQCD_Jet50");
  electronFakeRate_Pt_madgraphPhotonJets = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Pt_MadgraphPhotonJets_Photon15");
  electronFakeRate_Pt_PhotonTrigger = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Pt_Madgraph_Photon15");
  electronFakeRate_Pt_JetTrigger = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Pt_Madgraph_Jet50");

   assert(electronFakeRate_Pt_pythiaWJets);
  assert(electronFakeRate_Pt_madgraphWJets);
  assert(electronFakeRate_Pt_pythiaQCDEnriched);
  assert(electronFakeRate_Pt_madgraphQCD);
  assert(electronFakeRate_Pt_madgraphPhotonJets);
  assert(electronFakeRate_Pt_PhotonTrigger);
  assert(electronFakeRate_Pt_JetTrigger);

   electronFakeRate_Pt_pythiaWJets->SetLineColor(COLORS[0]);
   electronFakeRate_Pt_pythiaWJets->SetMarkerColor(COLORS[0]);
  electronFakeRate_Pt_madgraphWJets->SetLineColor(COLORS[1]);
  electronFakeRate_Pt_madgraphWJets->SetMarkerColor(COLORS[1]);
  electronFakeRate_Pt_pythiaQCDEnriched->SetLineColor(COLORS[2]);
  electronFakeRate_Pt_pythiaQCDEnriched->SetMarkerColor(COLORS[2]);
  electronFakeRate_Pt_madgraphQCD->SetLineColor(COLORS[3]);
  electronFakeRate_Pt_madgraphQCD->SetMarkerColor(COLORS[3]);
  electronFakeRate_Pt_madgraphPhotonJets->SetLineColor(COLORS[4]);
  electronFakeRate_Pt_madgraphPhotonJets->SetMarkerColor(COLORS[4]);
  electronFakeRate_Pt_PhotonTrigger->SetLineColor(COLORS[5]);
  electronFakeRate_Pt_PhotonTrigger->SetMarkerColor(COLORS[5]);
  electronFakeRate_Pt_JetTrigger->SetLineColor(COLORS[6]);
  electronFakeRate_Pt_JetTrigger->SetMarkerColor(COLORS[6]);

  cout << "hello\n";
  leg1->AddEntry(electronFakeRate_Pt_madgraphWJets, "MadgraphWJets", "LP"); 
//  leg1->AddEntry(electronFakeRate_Pt_pythiaWJets, "PythiaWJets", "LP");
//  leg1->AddEntry(electronFakeRate_Pt_pythiaQCDEnriched, "PythiaQCDEnriched", "LP"); 
  leg1->AddEntry(electronFakeRate_Pt_madgraphQCD, "MadgraphQCD", "LP"); 
  leg1->AddEntry(electronFakeRate_Pt_madgraphPhotonJets, "MadgraphPhotonJets", "LP"); 
  leg1->AddEntry(electronFakeRate_Pt_PhotonTrigger, "PhotonTrigger", "LP"); 
  leg1->AddEntry(electronFakeRate_Pt_JetTrigger, "JetTrigger", "LP"); 
 
  electronFakeRate_Pt_madgraphWJets->SetMinimum(0.0);
  electronFakeRate_Pt_madgraphWJets->SetMaximum(0.04);
  electronFakeRate_Pt_madgraphWJets->GetXaxis()->SetRangeUser(0,100);

  electronFakeRate_Pt_madgraphWJets->Draw();
//   electronFakeRate_Pt_pythiaWJets->Draw("same");
//   electronFakeRate_Pt_pythiaQCDEnriched->Draw("same");
  electronFakeRate_Pt_madgraphQCD->Draw("same");
  electronFakeRate_Pt_madgraphPhotonJets->Draw("same");
  electronFakeRate_Pt_PhotonTrigger->Draw("same");
  electronFakeRate_Pt_JetTrigger->Draw("same");
  leg1->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

 //*****************************************************************************************
  //Compare FakeRates For GsfSC Electron Fake Rates Vs Pt
  //Compare Different W channels
  //*****************************************************************************************


  
//   //*****************************************************************************************
//   //Compare FakeRates For GsfSC Electron Fake Rates Vs Eta
//   //*****************************************************************************************
  
//   leg1->Clear();
//   leg1->SetX1NDC(0.50);  leg1->SetX2NDC(0.8); leg1->SetY1NDC(0.6);  leg1->SetY2NDC(0.8); 
//   leg1->SetX1(0.50);  leg1->SetX2(0.8); leg1->SetY1(0.7);  leg1->SetY2(0.9); 
//   plotname = "GsfElectronFakeRateEta";

//   electronFakeRate_Eta_pythiaWJets = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Eta_PythiaWJets");
//   electronFakeRate_Eta_madgraphWJets = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Eta_MadgraphWJets");
//   electronFakeRate_Eta_pythiaQCDEnriched = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Eta_PythiaQCDEnriched_Jet50");
//   electronFakeRate_Eta_madgraphQCD = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Eta_MadgraphQCD_Jet50");
//   electronFakeRate_Eta_madgraphPhotonJets = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Eta_MadgraphPhotonJets_Photon15");
//   electronFakeRate_Eta_PhotonTrigger = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Eta_Madgraph_Photon15");
//   electronFakeRate_Eta_JetTrigger = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Eta_Madgraph_Jet50");

//   assert(electronFakeRate_Eta_pythiaWJets);
//   assert(electronFakeRate_Eta_madgraphWJets);
//   assert(electronFakeRate_Eta_pythiaQCDEnriched);
//   assert(electronFakeRate_Eta_madgraphQCD);
//   assert(electronFakeRate_Eta_madgraphPhotonJets);
//   assert(electronFakeRate_Eta_PhotonTrigger);
//   assert(electronFakeRate_Eta_JetTrigger);

//   electronFakeRate_Eta_pythiaWJets->SetLineColor(COLORS[0]);
//   electronFakeRate_Eta_pythiaWJets->SetMarkerColor(COLORS[0]);
//   electronFakeRate_Eta_madgraphWJets->SetLineColor(COLORS[1]);
//   electronFakeRate_Eta_madgraphWJets->SetMarkerColor(COLORS[1]);
//   electronFakeRate_Eta_pythiaQCDEnriched->SetLineColor(COLORS[2]);
//   electronFakeRate_Eta_pythiaQCDEnriched->SetMarkerColor(COLORS[2]);
//   electronFakeRate_Eta_madgraphQCD->SetLineColor(COLORS[3]);
//   electronFakeRate_Eta_madgraphQCD->SetMarkerColor(COLORS[3]);
//   electronFakeRate_Eta_madgraphPhotonJets->SetLineColor(COLORS[4]);
//   electronFakeRate_Eta_madgraphPhotonJets->SetMarkerColor(COLORS[4]);
//   electronFakeRate_Eta_PhotonTrigger->SetLineColor(COLORS[5]);
//   electronFakeRate_Eta_PhotonTrigger->SetMarkerColor(COLORS[5]);
//   electronFakeRate_Eta_JetTrigger->SetLineColor(COLORS[6]);
//   electronFakeRate_Eta_JetTrigger->SetMarkerColor(COLORS[6]);
 
//   leg1->AddEntry(electronFakeRate_Eta_pythiaWJets, "PythiaWJets", "LP");
//   leg1->AddEntry(electronFakeRate_Eta_madgraphWJets, "MadgraphWJets", "LP"); 
//   leg1->AddEntry(electronFakeRate_Eta_pythiaQCDEnriched, "PythiaQCDEnriched", "LP"); 
//   leg1->AddEntry(electronFakeRate_Eta_madgraphQCD, "MadgraphQCD", "LP"); 
//   leg1->AddEntry(electronFakeRate_Eta_madgraphPhotonJets, "MadgraphPhotonJets", "LP"); 
//   //leg1->AddEntry(electronFakeRate_Eta_PhotonTrigger, "PhotonTrigger", "LP"); 
//   //leg1->AddEntry(electronFakeRate_Eta_JetTrigger, "JetTrigger", "LP"); 
 
//   electronFakeRate_Eta_madgraphWJets->SetMinimum(0.0);
//   electronFakeRate_Eta_madgraphWJets->SetMaximum(0.04);

//   electronFakeRate_Eta_madgraphWJets->Draw();
//   electronFakeRate_Eta_pythiaWJets->Draw("same");
//   electronFakeRate_Eta_pythiaQCDEnriched->Draw("same");
//   electronFakeRate_Eta_madgraphQCD->Draw("same");
//   electronFakeRate_Eta_madgraphPhotonJets->Draw("same");
//   //electronFakeRate_Eta_PhotonTrigger->Draw("same");
//   //electronFakeRate_Eta_JetTrigger->Draw("same");

//   leg1->Draw();
//   filename = plotname + ".gif";
//   cv->SaveAs(filename.c_str());
//   cv->Delete();

 
  //*****************************************************************************************
  //Compare FakeRates For Reco Electron Fake Rates Vs Pt
  //*****************************************************************************************

  leg1->Clear();
  leg1->SetX1NDC(0.75);  leg1->SetX2NDC(0.95); leg1->SetY1NDC(0.7);  leg1->SetY2NDC(0.9); 
  leg1->SetX1(0.65);  leg1->SetX2(0.95); leg1->SetY1(0.7);  leg1->SetY2(0.9); 
  plotname = "RecoElectronFakeRatePt";

  electronFakeRate_Pt_pythiaWJets = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_PythiaWJets");
  electronFakeRate_Pt_madgraphWJets = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_MadgraphWJets");
  electronFakeRate_Pt_pythiaQCDEnriched = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_PythiaQCDEnriched_Jet50");
  electronFakeRate_Pt_madgraphQCD = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_MadgraphQCD_Jet50");
  electronFakeRate_Pt_madgraphPhotonJets = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_MadgraphPhotonJets_Photon15");
  electronFakeRate_Pt_PhotonTrigger = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_Madgraph_Photon15");
  electronFakeRate_Pt_JetTrigger = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_Madgraph_Jet50");

  assert(electronFakeRate_Pt_pythiaWJets);
  assert(electronFakeRate_Pt_madgraphWJets);
  assert(electronFakeRate_Pt_pythiaQCDEnriched);
  assert(electronFakeRate_Pt_madgraphQCD);
  assert(electronFakeRate_Pt_madgraphPhotonJets);
  assert(electronFakeRate_Pt_PhotonTrigger);
  assert(electronFakeRate_Pt_JetTrigger);

  electronFakeRate_Pt_pythiaWJets->SetLineColor(COLORS[0]);
  electronFakeRate_Pt_pythiaWJets->SetMarkerColor(COLORS[0]);
  electronFakeRate_Pt_madgraphWJets->SetLineColor(COLORS[1]);
  electronFakeRate_Pt_madgraphWJets->SetMarkerColor(COLORS[1]);
  electronFakeRate_Pt_pythiaQCDEnriched->SetLineColor(COLORS[2]);
  electronFakeRate_Pt_pythiaQCDEnriched->SetMarkerColor(COLORS[2]);
  electronFakeRate_Pt_madgraphQCD->SetLineColor(COLORS[3]);
  electronFakeRate_Pt_madgraphQCD->SetMarkerColor(COLORS[3]);
  electronFakeRate_Pt_madgraphPhotonJets->SetLineColor(COLORS[4]);
  electronFakeRate_Pt_madgraphPhotonJets->SetMarkerColor(COLORS[4]);
  electronFakeRate_Pt_PhotonTrigger->SetLineColor(COLORS[5]);
  electronFakeRate_Pt_PhotonTrigger->SetMarkerColor(COLORS[5]);
  electronFakeRate_Pt_JetTrigger->SetLineColor(COLORS[6]);
  electronFakeRate_Pt_JetTrigger->SetMarkerColor(COLORS[6]);
 
  leg1->AddEntry(electronFakeRate_Pt_madgraphWJets, "MadgraphWJets", "LP"); 
//   leg1->AddEntry(electronFakeRate_Pt_pythiaWJets, "PythiaWJets", "LP");
//   leg1->AddEntry(electronFakeRate_Pt_pythiaQCDEnriched, "PythiaQCDEnriched", "LP"); 
  leg1->AddEntry(electronFakeRate_Pt_madgraphQCD, "MadgraphQCD", "LP"); 
  leg1->AddEntry(electronFakeRate_Pt_madgraphPhotonJets, "MadgraphPhotonJets", "LP"); 
  leg1->AddEntry(electronFakeRate_Pt_PhotonTrigger, "PhotonTrigger", "LP"); 
  leg1->AddEntry(electronFakeRate_Pt_JetTrigger, "JetTrigger", "LP"); 
 
  electronFakeRate_Pt_madgraphWJets->SetMinimum(0.0);
  electronFakeRate_Pt_madgraphWJets->SetMaximum(0.2);
  electronFakeRate_Pt_madgraphWJets->GetXaxis()->SetRangeUser(0,100);

   electronFakeRate_Pt_madgraphWJets->Draw();
//   electronFakeRate_Pt_pythiaWJets->Draw("same");
//   electronFakeRate_Pt_pythiaQCDEnriched->Draw("same");
  electronFakeRate_Pt_madgraphQCD->Draw("same");
  electronFakeRate_Pt_madgraphPhotonJets->Draw("same");
  electronFakeRate_Pt_PhotonTrigger->Draw("same");
  electronFakeRate_Pt_JetTrigger->Draw("same");

  leg1->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

  //*****************************************************************************************
  //Compare FakeRates For Reco Electron Fake Rates Vs Pt
  //Compare Different W channels
  //*****************************************************************************************


//   //*****************************************************************************************
//   //Compare FakeRates For Reco Electron Fake Rates Vs Eta
//   //*****************************************************************************************
//   leg1->Clear();
//   leg1->SetX1NDC(0.50);  leg1->SetX2NDC(0.7); leg1->SetY1NDC(0.6);  leg1->SetY2NDC(0.8); 
//   leg1->SetX1(0.50);  leg1->SetX2(0.8); leg1->SetY1(0.7);  leg1->SetY2(0.9); 
//   plotname = "RecoElectronFakeRateEta";

//   electronFakeRate_Eta_pythiaWJets = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_PythiaWJets");
//   electronFakeRate_Eta_madgraphWJets = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_MadgraphWJets");
//   electronFakeRate_Eta_pythiaQCDEnriched = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_PythiaQCDEnriched_Jet50");
//   electronFakeRate_Eta_madgraphQCD = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_MadgraphQCD_Jet50");
//   electronFakeRate_Eta_madgraphPhotonJets = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_MadgraphPhotonJets_Photon15");
//   electronFakeRate_Eta_PhotonTrigger = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_Madgraph_Photon15");
//   electronFakeRate_Eta_JetTrigger = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_Madgraph_Jet50");

//   assert(electronFakeRate_Eta_pythiaWJets);
//   assert(electronFakeRate_Eta_madgraphWJets);
//   assert(electronFakeRate_Eta_pythiaQCDEnriched);
//   assert(electronFakeRate_Eta_madgraphQCD);
//   assert(electronFakeRate_Eta_madgraphPhotonJets);
//   assert(electronFakeRate_Eta_PhotonTrigger);
//   assert(electronFakeRate_Eta_JetTrigger);

//   electronFakeRate_Eta_pythiaWJets->SetLineColor(COLORS[0]);
//   electronFakeRate_Eta_pythiaWJets->SetMarkerColor(COLORS[0]);
//   electronFakeRate_Eta_madgraphWJets->SetLineColor(COLORS[1]);
//   electronFakeRate_Eta_madgraphWJets->SetMarkerColor(COLORS[1]);
//   electronFakeRate_Eta_pythiaQCDEnriched->SetLineColor(COLORS[2]);
//   electronFakeRate_Eta_pythiaQCDEnriched->SetMarkerColor(COLORS[2]);
//   electronFakeRate_Eta_madgraphQCD->SetLineColor(COLORS[3]);
//   electronFakeRate_Eta_madgraphQCD->SetMarkerColor(COLORS[3]);
//   electronFakeRate_Eta_madgraphPhotonJets->SetLineColor(COLORS[4]);
//   electronFakeRate_Eta_madgraphPhotonJets->SetMarkerColor(COLORS[4]);
//   electronFakeRate_Eta_PhotonTrigger->SetLineColor(COLORS[5]);
//   electronFakeRate_Eta_PhotonTrigger->SetMarkerColor(COLORS[5]);
//   electronFakeRate_Eta_JetTrigger->SetLineColor(COLORS[6]);
//   electronFakeRate_Eta_JetTrigger->SetMarkerColor(COLORS[6]);
 
//   leg1->AddEntry(electronFakeRate_Eta_pythiaWJets, "PythiaWJets", "LP");
//   leg1->AddEntry(electronFakeRate_Eta_madgraphWJets, "MadgraphWJets", "LP"); 
//   leg1->AddEntry(electronFakeRate_Eta_pythiaQCDEnriched, "PythiaQCD", "LP"); 
//   leg1->AddEntry(electronFakeRate_Eta_madgraphQCD, "MadgraphQCD", "LP"); 
//   leg1->AddEntry(electronFakeRate_Eta_madgraphPhotonJets, "MadgraphPhotonJets", "LP"); 
// //   leg1->AddEntry(electronFakeRate_Eta_PhotonTrigger, "PhotonTrigger", "LP"); 
// //   leg1->AddEntry(electronFakeRate_Eta_JetTrigger, "JetTrigger", "LP"); 
 
//   electronFakeRate_Eta_madgraphWJets->SetMinimum(0.0);
//   electronFakeRate_Eta_madgraphWJets->SetMaximum(0.08);

//   electronFakeRate_Eta_madgraphWJets->Draw();
//   electronFakeRate_Eta_pythiaWJets->Draw("same");
//   electronFakeRate_Eta_pythiaQCDEnriched->Draw("same");
//   electronFakeRate_Eta_madgraphQCD->Draw("same");
//   electronFakeRate_Eta_madgraphPhotonJets->Draw("same");
// //   electronFakeRate_Eta_PhotonTrigger->Draw("same");
// //   electronFakeRate_Eta_JetTrigger->Draw("same");

//   leg1->Draw();
//   filename = plotname + ".gif";
//   cv->SaveAs(filename.c_str());
//   cv->Delete();
  

  //*****************************************************************************************
  //Compare FakeRates For QCD Sample For Different Jet Triggers
  //*****************************************************************************************
//   leg1->Clear();
//   leg1->SetX1NDC(0.70);  leg1->SetX2NDC(0.9); leg1->SetY1NDC(0.6);  leg1->SetY2NDC(0.8); 
//   plotname = "GsfElectronFakeRate_QCD_JetTriggers";

//   TH1F *electronFakeRate_Pt_madgraphQCD_Jet30 = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Pt_MadgraphQCD_Jet30");
//   TH1F *electronFakeRate_Pt_madgraphQCD_Jet50 = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Pt_MadgraphQCD_Jet50");
//   TH1F *electronFakeRate_Pt_madgraphQCD_Jet80 = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Pt_MadgraphQCD_Jet80");
//   TH1F *electronFakeRate_Pt_madgraphQCD_Jet110 = (TH1F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_Pt_MadgraphQCD_Jet110");

//   assert(electronFakeRate_Pt_madgraphQCD_Jet30);
//   assert(electronFakeRate_Pt_madgraphQCD_Jet50);
//   assert(electronFakeRate_Pt_madgraphQCD_Jet80);
//   assert(electronFakeRate_Pt_madgraphQCD_Jet110);

//   electronFakeRate_Pt_madgraphQCD_Jet30->SetLineColor(kRed);
//   electronFakeRate_Pt_madgraphQCD_Jet30->SetMarkerColor(kRed);
//   electronFakeRate_Pt_madgraphQCD_Jet50->SetLineColor(kBlue);
//   electronFakeRate_Pt_madgraphQCD_Jet50->SetMarkerColor(kBlue);
//   electronFakeRate_Pt_madgraphQCD_Jet80->SetLineColor(kMagenta);
//   electronFakeRate_Pt_madgraphQCD_Jet80->SetMarkerColor(kMagenta);
//   electronFakeRate_Pt_madgraphQCD_Jet110->SetLineColor(kGreen);
//   electronFakeRate_Pt_madgraphQCD_Jet110->SetMarkerColor(kGreen);

//   electronFakeRate_Pt_madgraphQCD_Jet30->SetMinimum(0.0);
//   electronFakeRate_Pt_madgraphQCD_Jet50->SetMaximum(0.05);
    
//   leg1->AddEntry(electronFakeRate_Pt_madgraphQCD_Jet30, "Jet30", "LP");
//   leg1->AddEntry(electronFakeRate_Pt_madgraphQCD_Jet50, "Jet50", "LP"); 
//   leg1->AddEntry(electronFakeRate_Pt_madgraphQCD_Jet80, "Jet80", "LP"); 
//   leg1->AddEntry(electronFakeRate_Pt_madgraphQCD_Jet110, "Jet110", "LP"); 

//   electronFakeRate_Pt_madgraphQCD_Jet30->Draw();
//   electronFakeRate_Pt_madgraphQCD_Jet50->Draw("same");
//   electronFakeRate_Pt_madgraphQCD_Jet80->Draw("same");
//   electronFakeRate_Pt_madgraphQCD_Jet110->Draw("same");
  
//   leg1->Draw();
//   filename = plotname + ".gif";
//   cv->SaveAs(filename.c_str());
//   cv->Delete();


//   leg1->Clear();
//   leg1->SetX1NDC(0.70);  leg1->SetX2NDC(0.9); leg1->SetY1NDC(0.6);  leg1->SetY2NDC(0.8); 
//   plotname = "RecoElectronFakeRate_QCD_JetTriggers";

//   electronFakeRate_Pt_madgraphQCD_Jet30 = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_MadgraphQCD_Jet30");
//   electronFakeRate_Pt_madgraphQCD_Jet50 = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_MadgraphQCD_Jet50");
//   electronFakeRate_Pt_madgraphQCD_Jet80 = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_MadgraphQCD_Jet80");
//   electronFakeRate_Pt_madgraphQCD_Jet110 = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_MadgraphQCD_Jet110");

//   assert(electronFakeRate_Pt_madgraphQCD_Jet30);
//   assert(electronFakeRate_Pt_madgraphQCD_Jet50);
//   assert(electronFakeRate_Pt_madgraphQCD_Jet80);
//   assert(electronFakeRate_Pt_madgraphQCD_Jet110);

//   electronFakeRate_Pt_madgraphQCD_Jet30->SetLineColor(kRed);
//   electronFakeRate_Pt_madgraphQCD_Jet30->SetMarkerColor(kRed);
//   electronFakeRate_Pt_madgraphQCD_Jet50->SetLineColor(kBlue);
//   electronFakeRate_Pt_madgraphQCD_Jet50->SetMarkerColor(kBlue);
//   electronFakeRate_Pt_madgraphQCD_Jet80->SetLineColor(kMagenta);
//   electronFakeRate_Pt_madgraphQCD_Jet80->SetMarkerColor(kMagenta);
//   electronFakeRate_Pt_madgraphQCD_Jet110->SetLineColor(kGreen);
//   electronFakeRate_Pt_madgraphQCD_Jet110->SetMarkerColor(kGreen);

//   electronFakeRate_Pt_madgraphQCD_Jet30->SetMinimum(0.0);
//   electronFakeRate_Pt_madgraphQCD_Jet50->SetMaximum(0.1);
    
//   leg1->AddEntry(electronFakeRate_Pt_madgraphQCD_Jet30, "Jet30", "LP");
//   leg1->AddEntry(electronFakeRate_Pt_madgraphQCD_Jet50, "Jet50", "LP"); 
//   leg1->AddEntry(electronFakeRate_Pt_madgraphQCD_Jet80, "Jet80", "LP"); 
//   leg1->AddEntry(electronFakeRate_Pt_madgraphQCD_Jet110, "Jet110", "LP"); 

//   electronFakeRate_Pt_madgraphQCD_Jet30->Draw();
//   electronFakeRate_Pt_madgraphQCD_Jet50->Draw("same");
//   electronFakeRate_Pt_madgraphQCD_Jet80->Draw("same");
//   electronFakeRate_Pt_madgraphQCD_Jet110->Draw("same");
  
//   leg1->Draw();
//   filename = plotname + ".gif";
//   cv->SaveAs(filename.c_str());
//   cv->Delete();

}


//--------------------------------------------------------------------------------------------------
// Do Fake Rate Computation From WJets sample
//--------------------------------------------------------------------------------------------------
void make2DDistribution( char *FakeRateFilename ) {

  //Samples
  vector<string> madgraphDatasetFiles;
  vector<string> madgraphDatasetNames;
  vector<string> pythiaDatasetFiles;
  vector<string> pythiaDatasetNames;

  vector<string> madgraphQCDDatasetFiles;
  vector<string> madgraphQCDDatasetNames;
  vector<string> pythiaQCDEnrichedDatasetFiles;
  vector<string> pythiaQCDEnrichedDatasetNames;

  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************


  //Create root file to store fake rates
  TFile *file = new TFile(FakeRateFilename, "UPDATE");
  file->cd();
  
  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************
  madgraphDatasetFiles.clear(); 
  madgraphDatasetNames.clear();
  pythiaDatasetFiles.clear();
  pythiaDatasetNames.clear();
  madgraphQCDDatasetFiles.clear(); 
  madgraphQCDDatasetNames.clear();
  pythiaQCDEnrichedDatasetFiles.clear();
  pythiaQCDEnrichedDatasetNames.clear();
  
  
   //Madgraph WJets
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wjets-mg-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-wjets-mg-id11");
  //Pythia WJets
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-we-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-we-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wm-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-wm-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wt-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-wt-id11");  

  //Madgraph QCD
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcd_250_500-mg-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("s8-qcd_250_500-mg-id11");
  
  //Pythia QCDEM
//   pythiaQCDEnrichedDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcdem_20_30-id11_noskim.root");
//   pythiaQCDEnrichedDatasetNames.push_back("s8-qcdem_20_30-id11");
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

//   TH2F *denominator = addAllSamples2D(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, ("ComputeElectronFakeRateMod" + JetTrigger).c_str(), "hElectronDenominatorPtEta",ptbins ,etabins);
//   TH2F *numerator = addAllSamples2D(pythiaQCDEnrichedDatasetFiles, pythiaQCDEnrichedDatasetNames, ("ComputeElectronFakeRateMod" + JetTrigger).c_str(), "hElectronNumeratorPtEta",ptbins,etabins);
  TH2F *denominator = addAllSamples2D(pythiaDatasetFiles, pythiaDatasetNames, "ComputeElectronFakeRateMod_Jet30", "hElectronDenominatorPtEta",ptbins ,etabins);
  TH2F *numerator = addAllSamples2D(pythiaDatasetFiles, pythiaDatasetNames, "ComputeElectronFakeRateMod_Jet30", "hElectronNumeratorPtEta",ptbins,etabins);

  print2DHist(numerator, "Pythia WJets Numerator");
  print2DHist(denominator, "Pythia WJets Denominator");

  TCanvas *cv = MakeCanvas("cv", "denominator", 800, 600);
  gStyle->SetPalette(1);
  denominator->Draw("lego");
  cv->SaveAs("denominator.gif");
  
  TCanvas *cv2 = MakeCanvas("cv2", "numerator", 800, 600);
  numerator->Draw("lego");
  cv2->SaveAs("numerator.gif");

//   TH2F *electronFakeRate_PtEta_pythiaWJets = (TH2F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_PtEta_PythiaWJets");
//   assert(electronFakeRate_PtEta_pythiaWJets);
//   print2DHist(electronFakeRate_PtEta_pythiaWJets, "FakeRate");


  denominator = addAllSamples2D(madgraphDatasetFiles, madgraphDatasetNames, "ComputeElectronFakeRateMod", "hElectronDenominatorPtEta",ptbins ,etabins);
  numerator = addAllSamples2D(madgraphDatasetFiles, madgraphDatasetNames, "ComputeElectronFakeRateMod", "hElectronNumeratorPtEta",ptbins,etabins);

  print2DHist(numerator, "Madgraph WJets Numerator");
  print2DHist(denominator, "Madgraph WJets Denominator");


//   denominator = addAllSamples2D(pythiaDatasetFiles, pythiaDatasetNames, "ComputeElectronFakeRateMod", "hPredictionElectronDenominatorPtEta",ptbins ,etabins);
//   numerator = addAllSamples2D(pythiaDatasetFiles, pythiaDatasetNames, "ComputeElectronFakeRateMod", "hPredictionElectronNumeratorPtEta",ptbins,etabins);

//   print2DHist(numerator, "Numerator");
//   print2DHist(denominator, "Denominator");

//   electronFakeRate_PtEta_pythiaWJets = (TH2F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_PtEta_PythiaWJetsPrediction");
//   assert(electronFakeRate_PtEta_pythiaWJets);
//   print2DHist(electronFakeRate_PtEta_pythiaWJets, "FakeRate");


  file->Close();
  return;

}


//--------------------------------------------------------------------------------------------------
// Do Fake Rate Computation From WJets sample
//--------------------------------------------------------------------------------------------------
void compare2DElectronFakeRates( char *FakeRateFilename ) {

  //Samples
  vector<string> madgraphDatasetFiles;
  vector<string> madgraphDatasetNames;
  vector<string> pythiaDatasetFiles;
  vector<string> pythiaDatasetNames;

  vector<string> madgraphQCDDatasetFiles;
  vector<string> madgraphQCDDatasetNames;
  vector<string> pythiaQCDEnrichedDatasetFiles;
  vector<string> pythiaQCDEnrichedDatasetNames;

  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************


  //Create root file to store fake rates
  TFile *file = new TFile(FakeRateFilename, "UPDATE");
  file->cd();
  
  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************
  madgraphDatasetFiles.clear(); 
  madgraphDatasetNames.clear();
  pythiaDatasetFiles.clear();
  pythiaDatasetNames.clear();
  madgraphQCDDatasetFiles.clear(); 
  madgraphQCDDatasetNames.clear();
  pythiaQCDEnrichedDatasetFiles.clear();
  pythiaQCDEnrichedDatasetNames.clear();
  
  
   //Madgraph WJets
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wjets-mg-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-wjets-mg-id11");
  //Pythia WJets
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-we-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-we-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wm-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-wm-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wt-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-wt-id11");  


  //Madgraph QCD
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-qcd_250_500-mg-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("s8-qcd_250_500-mg-id11");
  
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



//   TH2F *numerator = addAllSamples2D(pythiaDatasetFiles, pythiaDatasetNames, "ComputeElectronFakeRateMod","hElectronNumeratorPtEta",ptbins,etabins);
//   TH2F *denominator = addAllSamples2D(pythiaDatasetFiles, pythiaDatasetNames, "ComputeElectronFakeRateMod","hElectronDenominatorPtEta",ptbins,etabins);
//   TH2F *electronFakeRate_PtEta_pythiaWJets = (TH2F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_PtEta_PythiaWJets");
//   assert(electronFakeRate_PtEta_pythiaWJets);
//   print2DHist(electronFakeRate_PtEta_pythiaWJets, "FakeRate");
  

//   TH2F *electronFakeRate_PtEta_pythiaWJets_True = (TH2F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_PtEta_PythiaWJets_True");
//   assert(electronFakeRate_PtEta_pythiaWJets_True);
//   print2DHist(electronFakeRate_PtEta_pythiaWJets_True, "FakeRate True");

//   makePtSliceComparisonPlots(electronFakeRate_PtEta_pythiaWJets, electronFakeRate_PtEta_pythiaWJets_True, "GsfTrackSCElectronFakeRate_PtEta_PythiaWJets_Comparison_PtSlices");
//   makeEtaSliceComparisonPlots(electronFakeRate_PtEta_pythiaWJets, electronFakeRate_PtEta_pythiaWJets_True, "GsfTrackSCElectronFakeRate_PtEta_PythiaWJets_Comparison_EtaSlices");









  //*****************************************************************************************
  //Compare GsfSC FakeRate with RecoElectron FakeRate
  //*****************************************************************************************
//   TH2F *GsfSCElectronFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_PtEta_MadgraphWJets");
//   assert(GsfSCElectronFakeRate_PtEta_MadgraphWJets);

//   TH2F *RecoElectronFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny("RecoElectronFakeRate_PtEta_MadgraphWJets");
//   assert(RecoElectronFakeRate_PtEta_MadgraphWJets);

//   makePtSliceComparisonPlots(GsfSCElectronFakeRate_PtEta_MadgraphWJets, RecoElectronFakeRate_PtEta_MadgraphWJets, "GsfTrackSCElectronFakeRate_PtEta_PythiaWJets_Comparison_PtSlices", "GsfSC", "Reco");
//   makeEtaSliceComparisonPlots(GsfSCElectronFakeRate_PtEta_MadgraphWJets, RecoElectronFakeRate_PtEta_MadgraphWJets, "GsfTrackSCElectronFakeRate_PtEta_PythiaWJets_Comparison_EtaSlices", "GsfSC", "Reco");


  //*****************************************************************************************
  //Compare Pythia Vs Madgraph FakeRates For GsfSC
  //*****************************************************************************************
//   TH2F *GsfSCElectronFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_PtEta_MadgraphWJets");
//   assert(GsfSCElectronFakeRate_PtEta_MadgraphWJets);

//   TH2F *GsfSCElectronFakeRate_PtEta_PythiaWJets = (TH2F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_PtEta_PythiaWJets");
//   assert(GsfSCElectronFakeRate_PtEta_PythiaWJets);

//   makePtSliceComparisonPlots(GsfSCElectronFakeRate_PtEta_MadgraphWJets, GsfSCElectronFakeRate_PtEta_PythiaWJets, "GsfTrackSCElectronFakeRate_PtEta_WJets_PythiaVsMadgraph_PtSlices", "Madgraph", "Pythia");
//   makeEtaSliceComparisonPlots(GsfSCElectronFakeRate_PtEta_MadgraphWJets, GsfSCElectronFakeRate_PtEta_PythiaWJets, "GsfTrackSCElectronFakeRate_PtEta_WJets_PythiaVsMadgraph_EtaSlices", "Madgraph", "Pythia");


//   TH2F *GsfSCElectronFakeRate_PtEta_MadgraphQCD = (TH2F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_PtEta_MadgraphQCD_Jet50");
//   assert(GsfSCElectronFakeRate_PtEta_MadgraphQCD);

//   TH2F *GsfSCElectronFakeRate_PtEta_PythiaQCDEnriched = (TH2F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_PtEta_PythiaQCDEnriched_Jet50");
//   assert(GsfSCElectronFakeRate_PtEta_PythiaQCDEnriched);

//   makePtSliceComparisonPlots(GsfSCElectronFakeRate_PtEta_MadgraphQCD, GsfSCElectronFakeRate_PtEta_PythiaQCDEnriched, "GsfTrackSCElectronFakeRate_PtEta_QCD_PythiaVsMadgraph_PtSlices", "Madgraph", "Pythia QCD Enriched");
//   makeEtaSliceComparisonPlots(GsfSCElectronFakeRate_PtEta_MadgraphQCD, GsfSCElectronFakeRate_PtEta_PythiaQCDEnriched, "GsfTrackSCElectronFakeRate_PtEta_QCD_PythiaVsMadgraph_EtaSlices", "Madgraph", "Pythia QCD Enriched");

  //*****************************************************************************************
  //Compare Pythia Vs Madgraph FakeRates For Reco
  //*****************************************************************************************
//   TH2F *RecoElectronFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny("RecoElectronFakeRate_PtEta_MadgraphWJets");
//   assert(RecoElectronFakeRate_PtEta_MadgraphWJets);

//   TH2F *RecoElectronFakeRate_PtEta_PythiaWJets = (TH2F*)file->FindObjectAny("RecoElectronFakeRate_PtEta_PythiaWJets");
//   assert(RecoElectronFakeRate_PtEta_PythiaWJets);

//   makePtSliceComparisonPlots(RecoElectronFakeRate_PtEta_MadgraphWJets, RecoElectronFakeRate_PtEta_PythiaWJets, "RecoElectronFakeRate_PtEta_WJets_PythiaVsMadgraph_PtSlices", "Madgraph", "Pythia");
//   makeEtaSliceComparisonPlots(RecoElectronFakeRate_PtEta_MadgraphWJets, RecoElectronFakeRate_PtEta_PythiaWJets, "RecoElectronFakeRate_PtEta_WJets_PythiaVsMadgraph_EtaSlices", "Madgraph", "Pythia");


//   TH2F *RecoElectronFakeRate_PtEta_MadgraphQCD = (TH2F*)file->FindObjectAny("RecoElectronFakeRate_PtEta_MadgraphQCD_Jet50");
//   assert(RecoElectronFakeRate_PtEta_MadgraphQCD);

//   TH2F *RecoElectronFakeRate_PtEta_PythiaQCDEnriched = (TH2F*)file->FindObjectAny("RecoElectronFakeRate_PtEta_PythiaQCDEnriched_Jet50");
//   assert(RecoElectronFakeRate_PtEta_PythiaQCDEnriched);

//   makePtSliceComparisonPlots(RecoElectronFakeRate_PtEta_MadgraphQCD, RecoElectronFakeRate_PtEta_PythiaQCDEnriched, "RecoElectronFakeRate_PtEta_QCD_PythiaVsMadgraph_PtSlices", "Madgraph", "Pythia QCD Enriched");
//   makeEtaSliceComparisonPlots(RecoElectronFakeRate_PtEta_MadgraphQCD, RecoElectronFakeRate_PtEta_PythiaQCDEnriched, "RecoElectronFakeRate_PtEta_QCD_PythiaVsMadgraph_EtaSlices", "Madgraph", "Pythia QCD Enriched");


  //*****************************************************************************************
  //Compare WJets Vs QCD FakeRates For GsfSC 
  //*****************************************************************************************
//   TH2F *GsfSCElectronFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_PtEta_MadgraphWJets");
//   assert(GsfSCElectronFakeRate_PtEta_MadgraphWJets);

//   TH2F *GsfSCElectronFakeRate_PtEta_MadgraphQCD = (TH2F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_PtEta_MadgraphQCD_Jet50");
//   assert(GsfSCElectronFakeRate_PtEta_MadgraphQCD);

//   makePtSliceComparisonPlots(GsfSCElectronFakeRate_PtEta_MadgraphWJets, GsfSCElectronFakeRate_PtEta_MadgraphQCD, "GsfTrackSCElectronFakeRate_PtEta_Madgraph_WJetsVsQCD_PtSlices", "WJets", "QCD");
//   makeEtaSliceComparisonPlots(GsfSCElectronFakeRate_PtEta_MadgraphWJets, GsfSCElectronFakeRate_PtEta_MadgraphQCD, "GsfTrackSCElectronFakeRate_PtEta_Madgraph_WJetsVsQCD_EtaSlices", "WJets", "QCD");


//   TH2F *GsfSCElectronFakeRate_PtEta_PythiaWJets = (TH2F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_PtEta_PythiaWJets");
//   assert(GsfSCElectronFakeRate_PtEta_PythiaWJets);

//   TH2F *GsfSCElectronFakeRate_PtEta_PythiaQCDEnriched = (TH2F*)file->FindObjectAny("GsfTrackSCElectronFakeRate_PtEta_PythiaQCDEnriched_Jet50");
//   assert(GsfSCElectronFakeRate_PtEta_PythiaQCDEnriched);

//   makePtSliceComparisonPlots(GsfSCElectronFakeRate_PtEta_MadgraphQCD, GsfSCElectronFakeRate_PtEta_PythiaQCDEnriched, "GsfTrackSCElectronFakeRate_PtEta_Pythia_WJetsVsQCD_PtSlices", "WJets", "QCD Enriched");
//   makeEtaSliceComparisonPlots(GsfSCElectronFakeRate_PtEta_MadgraphQCD, GsfSCElectronFakeRate_PtEta_PythiaQCDEnriched, "GsfTrackSCElectronFakeRate_PtEta_Pythia_WJetsVsQCD_EtaSlices", "WJets", "QCD Enriched");



  //*****************************************************************************************
  //Compare WJets Vs QCD FakeRates For Reco 
  //*****************************************************************************************
//   TH2F *RecoElectronFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny("RecoElectronFakeRate_PtEta_MadgraphWJets");
//   assert(RecoElectronFakeRate_PtEta_MadgraphWJets);

//   TH2F *RecoElectronFakeRate_PtEta_MadgraphQCD = (TH2F*)file->FindObjectAny("RecoElectronFakeRate_PtEta_MadgraphQCD_Jet50");
//   assert(RecoElectronFakeRate_PtEta_MadgraphQCD);

//   makePtSliceComparisonPlots(RecoElectronFakeRate_PtEta_MadgraphWJets, RecoElectronFakeRate_PtEta_MadgraphQCD, "RecoElectronFakeRate_PtEta_Madgraph_WJetsVsQCD_PtSlices", "WJets", "QCD");
//   makeEtaSliceComparisonPlots(RecoElectronFakeRate_PtEta_MadgraphWJets, RecoElectronFakeRate_PtEta_MadgraphQCD, "RecoElectronFakeRate_PtEta_Madgraph_WJetsVsQCD_EtaSlices", "WJets", "QCD");


//   TH2F *RecoElectronFakeRate_PtEta_PythiaWJets = (TH2F*)file->FindObjectAny("RecoElectronFakeRate_PtEta_PythiaWJets");
//   assert(RecoElectronFakeRate_PtEta_PythiaWJets);

//   TH2F *RecoElectronFakeRate_PtEta_PythiaQCDEnriched = (TH2F*)file->FindObjectAny("RecoElectronFakeRate_PtEta_PythiaQCDEnriched_Jet50");
//   assert(RecoElectronFakeRate_PtEta_PythiaQCDEnriched);

//   makePtSliceComparisonPlots(RecoElectronFakeRate_PtEta_MadgraphQCD, RecoElectronFakeRate_PtEta_PythiaQCDEnriched, "RecoElectronFakeRate_PtEta_Pythia_WJetsVsQCD_PtSlices", "WJets", "QCD Enriched");
//   makeEtaSliceComparisonPlots(RecoElectronFakeRate_PtEta_MadgraphQCD, RecoElectronFakeRate_PtEta_PythiaQCDEnriched, "RecoElectronFakeRate_PtEta_Pythia_WJetsVsQCD_EtaSlices", "WJets", "QCD Enriched");




  file->Close();
  return;

}

//--------------------------------------------------------------------------------------------------
// Create Histogram for systematic errors
//--------------------------------------------------------------------------------------------------
void computeSystematicErrors( char *FakeRateFilename ) {

  //Create root file to store fake rates
  TFile *file = new TFile(FakeRateFilename, "UPDATE");
  file->cd();
  
  //*****************************************************************************************
  //Compute systematic errors for 1D Fake Rates vs Pt for Reco denominator 
  //*****************************************************************************************
  TH1F *electronFakeRate_Pt_WJets = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_MadgraphWJets");
  assert(electronFakeRate_Pt_WJets);

  TH1F *electronFakeRate_Pt_JetTrigger = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_Madgraph_Jet50");
  assert(electronFakeRate_Pt_JetTrigger);

  TH1F *electronFakeRate_Pt_PhotonTrigger = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Pt_Madgraph_Photon15");
  assert(electronFakeRate_Pt_PhotonTrigger);

  TH1F *electronFakeRate_Pt_SystematicError = (TH1F*)(electronFakeRate_Pt_WJets->Clone());
  electronFakeRate_Pt_SystematicError->SetName("RecoElectronFakeRate_Pt_Madgraph_SysErrors");

  for (int b=0;b<=electronFakeRate_Pt_WJets->GetXaxis()->GetNbins();b++) {
    Double_t maxDifference = fabs(electronFakeRate_Pt_JetTrigger->GetBinContent(b) - 
                                  electronFakeRate_Pt_PhotonTrigger->GetBinContent(b));
    if (fabs(electronFakeRate_Pt_WJets->GetBinContent(b) - 
             electronFakeRate_Pt_JetTrigger->GetBinContent(b))
        > maxDifference) {
      maxDifference = fabs(electronFakeRate_Pt_WJets->GetBinContent(b) - 
                           electronFakeRate_Pt_JetTrigger->GetBinContent(b));
    }
    if (fabs(electronFakeRate_Pt_WJets->GetBinContent(b) - 
             electronFakeRate_Pt_PhotonTrigger->GetBinContent(b))
        > maxDifference) {
      maxDifference = fabs(electronFakeRate_Pt_WJets->GetBinContent(b) - 
                           electronFakeRate_Pt_PhotonTrigger->GetBinContent(b));
    }
    
    Double_t totalError = TMath::Sqrt(electronFakeRate_Pt_JetTrigger->GetBinError(b)*
                                      electronFakeRate_Pt_JetTrigger->GetBinError(b) + 
                                      (maxDifference/2.0)*(maxDifference/2.0));
    
    electronFakeRate_Pt_SystematicError->SetBinContent(b,totalError);
  }

  electronFakeRate_Pt_SystematicError->SetDirectory(file);
  file->WriteTObject(electronFakeRate_Pt_SystematicError, electronFakeRate_Pt_SystematicError->GetName(), "WriteDelete");

  //*****************************************************************************************
  //Compute systematic errors for 1D Fake Rates vs Eta for Reco denominator 
  //*****************************************************************************************
  TH1F *electronFakeRate_Eta_WJets = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_MadgraphWJets");
  assert(electronFakeRate_Eta_WJets);

  TH1F *electronFakeRate_Eta_JetTrigger = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_Madgraph_Jet50");
  assert(electronFakeRate_Eta_JetTrigger);

  TH1F *electronFakeRate_Eta_PhotonTrigger = (TH1F*)file->FindObjectAny("RecoElectronFakeRate_Eta_Madgraph_Photon15");
  assert(electronFakeRate_Eta_PhotonTrigger);

  TH1F *electronFakeRate_Eta_SystematicError = (TH1F*)(electronFakeRate_Eta_WJets->Clone());
  electronFakeRate_Eta_SystematicError->SetName("RecoElectronFakeRate_Eta_Madgraph_SysErrors");

  for (int b=0;b<=electronFakeRate_Eta_WJets->GetXaxis()->GetNbins();b++) {
    Double_t maxDifference = fabs(electronFakeRate_Eta_JetTrigger->GetBinContent(b) - 
                                  electronFakeRate_Eta_PhotonTrigger->GetBinContent(b));
    if (fabs(electronFakeRate_Eta_WJets->GetBinContent(b) - 
             electronFakeRate_Eta_JetTrigger->GetBinContent(b))
        > maxDifference) {
      maxDifference = fabs(electronFakeRate_Eta_WJets->GetBinContent(b) - 
                           electronFakeRate_Eta_JetTrigger->GetBinContent(b));
    }
    if (fabs(electronFakeRate_Eta_WJets->GetBinContent(b) - 
             electronFakeRate_Eta_PhotonTrigger->GetBinContent(b))
        > maxDifference) {
      maxDifference = fabs(electronFakeRate_Eta_WJets->GetBinContent(b) - 
                           electronFakeRate_Eta_PhotonTrigger->GetBinContent(b));
    }
    
    Double_t totalError = TMath::Sqrt(electronFakeRate_Eta_JetTrigger->GetBinError(b)*
                                      electronFakeRate_Eta_JetTrigger->GetBinError(b) + 
                                      (maxDifference/2.0)*(maxDifference/2.0));
    
    electronFakeRate_Eta_SystematicError->SetBinContent(b,totalError);
  }
  
  electronFakeRate_Eta_SystematicError->SetDirectory(file);
  file->WriteTObject(electronFakeRate_Eta_SystematicError, electronFakeRate_Eta_SystematicError->GetName(), "WriteDelete");


  //*****************************************************************************************
  //Compute systematic errors for 2D FakeRates for Reco denominator 
  //*****************************************************************************************
  TH2F *electronFakeRate_PtEta_WJets = (TH2F*)file->FindObjectAny("RecoElectronFakeRate_PtEta_MadgraphWJets");
  assert(electronFakeRate_PtEta_WJets);

  TH2F *electronFakeRate_PtEta_JetTrigger = (TH2F*)file->FindObjectAny("RecoElectronFakeRate_PtEta_Madgraph_Jet50");
  assert(electronFakeRate_PtEta_JetTrigger);

  TH2F *electronFakeRate_PtEta_PhotonTrigger = (TH2F*)file->FindObjectAny("RecoElectronFakeRate_PtEta_Madgraph_Photon15");
  assert(electronFakeRate_PtEta_PhotonTrigger);

  TH2F *electronFakeRate_PtEta_SystematicError = (TH2F*)(electronFakeRate_PtEta_WJets->Clone());
  electronFakeRate_PtEta_SystematicError->SetName("RecoElectronFakeRate_PtEta_Madgraph_SysErrors");

  for (int b=0;b<=electronFakeRate_PtEta_WJets->GetXaxis()->GetNbins();b++) {
    for (int c=0;c<=electronFakeRate_PtEta_WJets->GetYaxis()->GetNbins();c++) {
      Double_t maxDifference = fabs(electronFakeRate_PtEta_JetTrigger->GetBinContent(b,c) - 
                                  electronFakeRate_PtEta_PhotonTrigger->GetBinContent(b,c));
      if (fabs(electronFakeRate_PtEta_WJets->GetBinContent(b,c) - 
                                  electronFakeRate_PtEta_JetTrigger->GetBinContent(b,c))
          > maxDifference) {
        maxDifference = fabs(electronFakeRate_PtEta_WJets->GetBinContent(b,c) - 
                             electronFakeRate_PtEta_JetTrigger->GetBinContent(b,c));
      }
      if (fabs(electronFakeRate_PtEta_WJets->GetBinContent(b,c) - 
                                  electronFakeRate_PtEta_PhotonTrigger->GetBinContent(b,c))
          > maxDifference) {
        maxDifference = fabs(electronFakeRate_PtEta_WJets->GetBinContent(b,c) - 
                             electronFakeRate_PtEta_PhotonTrigger->GetBinContent(b,c));
      }
      
      Double_t totalError = TMath::Sqrt(electronFakeRate_PtEta_JetTrigger->GetBinError(b,c)*
                                        electronFakeRate_PtEta_JetTrigger->GetBinError(b,c) + 
                                        (maxDifference/2.0)*(maxDifference/2.0));

      electronFakeRate_PtEta_SystematicError->SetBinContent(b,c,totalError);
    }
  }

  electronFakeRate_PtEta_SystematicError->SetDirectory(file);
  file->WriteTObject(electronFakeRate_PtEta_SystematicError, electronFakeRate_PtEta_SystematicError->GetName(), "WriteDelete");


  file->Close();
  return;

}

//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void computeElectronFakeRates(char *FakeRateFilename , bool fitFakeRate = false ) {



//   //*****************************************************************************************
//   //Electron Fake Rate Computation
//   //*****************************************************************************************  

//   //From WJets
//   compute1DElectronFakeRatesFromWJets(FakeRateFilename, fitFakeRate);
//   compute2DElectronFakeRatesFromWJets(FakeRateFilename, fitFakeRate);

//   //From QCD
//   compute1DElectronFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet30");
//   compute1DElectronFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet50");
//   compute1DElectronFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet80");
//   compute1DElectronFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet110");
//    compute2DElectronFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet30");
//    compute2DElectronFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet50");
//    compute2DElectronFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet80");
//    compute2DElectronFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet110");
  
//    //From PhotonTriggerSample
//   compute1DElectronFakeRatesFromPhotonSample(FakeRateFilename, fitFakeRate, "Photon15");
//   //compute1DElectronFakeRatesFromPhotonSample(FakeRateFilename, fitFakeRate, "Photon25");
//   compute2DElectronFakeRatesFromPhotonSample(FakeRateFilename, fitFakeRate, "Photon15");
//   //compute2DElectronFakeRatesFromPhotonSample(FakeRateFilename, fitFakeRate, "Photon25");
  
//   computeSystematicErrors(FakeRateFilename);


//   //make2DDistribution(FakeRateFilename);
//   //compareElectronFakeRates(FakeRateFilename);
//   //compare2DElectronFakeRates(FakeRateFilename);


  compute1DElectronFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet50");

//*****************************************************************************************
//Test that the function can be evaluated correctly
//*****************************************************************************************
//   TFile *file = new TFile("TestFakeRateFile.root");
//   TF1 *f1 = (TF1*)file->FindObjectAny("GlobalMuonFakeRateFunction_Pt_PythiaWJets");
//   f1->Draw();
//   cout << f1->Eval(20.0) << endl;
//   cout << f1->Eval(200.0) << endl;
//   cout << "done\n";

  return;
}


