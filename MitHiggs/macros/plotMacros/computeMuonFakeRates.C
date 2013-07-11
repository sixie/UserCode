//root -l $CMSSW_BASE/src/MitHiggs/macros/plotMacros/computeMuonFakeRates.C+\(\"MuonFakeRateFile.root\"\)
//root -l $CMSSW_BASE/src/MitHiggs/macros/plotMacros/computeMuonFakeRates.C+\(\"ElectronFakeRateFile.root\"\)
//root -l $CMSSW_BASE/src/MitHiggs/macros/plotMacros/computeMuonFakeRates.C+\(\"FakeRates.root\"\)
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
  //TH1F *finalHist = (TH1F*)tmp->Clone();
  TH1F *finalHist = new TH1F (tmp->GetName(), tmp->GetTitle(), tmp->GetXaxis()->GetNbins(), tmp->GetXaxis()->GetBinLowEdge(1), tmp->GetXaxis()->GetBinUpEdge(tmp->GetNbinsX()));
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

void saveFakeRate(TFile *file, vector<TH1F*> fakeRateHistograms, vector<TF1*> fakeRateFits, Bool_t fitFakeRate ) {

  for (UInt_t i=0; i<fakeRateHistograms.size();i++) {
    //write the histograms. delete any existing one with the same name
    fakeRateHistograms[i]->SetDirectory(file);
    file->WriteTObject(fakeRateHistograms[i], fakeRateHistograms[i]->GetName(), "WriteDelete");
    
    //write the fit functions. delete any existing one with the same name
    if (fitFakeRate) {
      file->WriteTObject(fakeRateFits[i],fakeRateFits[i]->GetName(), "WriteDelete" );
    }
  }
}

void saveFakeRate(TFile *file, vector<TH2F*> fakeRateHistograms, vector<TF2*> fakeRateFits, Bool_t fitFakeRate ) {
  for (UInt_t i=0; i<fakeRateHistograms.size();i++) {
    //write the histograms. delete any existing one with the same name
    fakeRateHistograms[i]->SetDirectory(file);
    file->WriteTObject(fakeRateHistograms[i], fakeRateHistograms[i]->GetName(), "WriteDelete" );
    
    //write the fit functions. delete any existing one with the same name
    if (fitFakeRate) {
      file->WriteTObject(fakeRateFits[i], fakeRateFits[i]->GetName(), "WriteDelete");
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
  //ptbins.push_back(80);  
  ptbins.push_back(120);  
  //ptbins.push_back(170);  
  //ptbins.push_back(250);

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

//   cout << "BIN ERRORS:\n";
//   for (int b=0;b<=denominator->GetNbinsX()+1;b++) {
//     cout << "bin " << b << " : " << numerator->GetBinContent(b) << "+-" << numerator->GetBinError(b) << " / " << denominator->GetBinContent(b) << "+-" <<  denominator->GetBinError(b) << endl;
//   }


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


//   cout << "BIN ERRORS:\n";
//   for (int b=0;b<=fakeRate->GetNbinsX()+1;b++) {
//     cout << "bin " << b << " : " << fakeRate->GetXaxis()->GetBinCenter(b) << " " << n->GetBinContent(b) << "+-" << n->GetBinError(b) << " / " << d->GetBinContent(b) << "+-" <<  d->GetBinError(b) << "  =  " << fakeRate->GetBinContent(b) << "+-" << fakeRate->GetBinError(b) << endl;
//   }


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

//   cout << "Adjusted BIN ERRORS:\n";
//   for (int b=0;b<=fakeRate->GetNbinsX()+1;b++) {
//     cout << "bin " << b << " : " << fakeRate->GetXaxis()->GetBinCenter(b) << " " << n->GetBinContent(b) << "+-" << n->GetBinError(b) << " / " << d->GetBinContent(b) << "+-" <<  d->GetBinError(b) << "  =  " << fakeRate->GetBinContent(b) << "+-" << fakeRate->GetBinError(b) << endl;
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
void compute1DMuonFakeRatesFromWJets( char *FakeRateFilename ,bool fitFakeRate) {

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
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601/ComputeMuonFakeRate_s8-wjets-mg-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601/ComputeMuonFakeRate_s8-vqq-mg-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-vqq-mg-id11");

  madgraphWJetsDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601/ComputeMuonFakeRate_s8-wjets-mg-id11_noskim.root");
  madgraphWJetsDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphVQQDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601/ComputeMuonFakeRate_s8-vqq-mg-id11_noskim.root");
  madgraphVQQDatasetNames.push_back("s8-vqq-mg-id11");

  //Pythia WJets
  pythiaWeDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090519/ComputeMuonFakeRate_s8-we-id11_noskim.root");
  pythiaWeDatasetNames.push_back("s8-we-id11");
  pythiaWmDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090519/ComputeMuonFakeRate_s8-wm-id11_noskim.root");
  pythiaWmDatasetNames.push_back("s8-wm-id11");
  pythiaWtDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090519/ComputeMuonFakeRate_s8-wt-id11_noskim.root");
  pythiaWtDatasetNames.push_back("s8-wt-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090519/ComputeMuonFakeRate_s8-we-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-we-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090519/ComputeMuonFakeRate_s8-wm-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-wm-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090519/ComputeMuonFakeRate_s8-wt-id11_noskim.root");
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
  //IsoTrack Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaDatasetFiles, pythiaDatasetNames, "ComputeMuonFakeRateMod",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "IsoTrackMuonFakeRate_Pt_PythiaWJets");
  FakeRatePt_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("IsoTrackMuonFakeRateFunction_Pt_PythiaWJets");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia W+Jets");

  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphDatasetFiles, madgraphDatasetNames, "ComputeMuonFakeRateMod",
                                             "hMuonNumeratorPt","hMuonDenominatorPt", "IsoTrackMuonFakeRate_Pt_MadgraphWJets");
  FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName("IsoTrackMuonFakeRateFunction_Pt_MadgraphWJets");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph W+Jets");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //IsoTrack Muon Fake Rates
  //Compare for different W decay channels
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRatePt_pythia = createFakeRatePtHist(pythiaWeDatasetFiles, pythiaWeDatasetNames, "ComputeMuonFakeRateMod",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "IsoTrackMuonFakeRate_Pt_PythiaWJets_We");
  FakeRatePt_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("IsoTrackMuonFakeRateFunction_Pt_PythiaWJets_We");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia W->enu+Jets");

  FakeRatePt_pythia = createFakeRatePtHist(pythiaWmDatasetFiles, pythiaWmDatasetNames, "ComputeMuonFakeRateMod",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "IsoTrackMuonFakeRate_Pt_PythiaWJets_Wm");
  FakeRatePt_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("IsoTrackMuonFakeRateFunction_Pt_PythiaWJets_Wm");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia W->munu+Jets");

  FakeRatePt_pythia = createFakeRatePtHist(pythiaWtDatasetFiles, pythiaWtDatasetNames, "ComputeMuonFakeRateMod",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "IsoTrackMuonFakeRate_Pt_PythiaWJets_Wt");
  FakeRatePt_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("IsoTrackMuonFakeRateFunction_Pt_PythiaWJets_Wt");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia W->taunu+Jets");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //IsoTrack Muon Fake Rates
  //Compare for different Madgraph jets samples
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRatePt_madgraph = createFakeRatePtHist(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeMuonFakeRateMod",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "IsoTrackMuonFakeRate_Pt_MadgraphWJets_LightJets");
  FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName("IsoTrackMuonFakeRateFunction_Pt_MadgraphWJets_LightJets");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph W+LightJets");

  FakeRatePt_madgraph = createFakeRatePtHist(madgraphVQQDatasetFiles, madgraphVQQDatasetNames, "ComputeMuonFakeRateMod",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "IsoTrackMuonFakeRate_Pt_MadgraphWJets_VQQ");
  FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName("IsoTrackMuonFakeRateFunction_Pt_MadgraphWJets_VQQ");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph VQQ");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //TrackerMuon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaDatasetFiles, pythiaDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "TrackerMuonFakeRate_Pt_PythiaWJets");
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("TrackerMuonFakeRateFunction_Pt_PythiaWJets");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia W+Jets");

  
  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(
    madgraphDatasetFiles, madgraphDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon",
    "hMuonNumeratorPt","hMuonDenominatorPt", "TrackerMuonFakeRate_Pt_MadgraphWJets");
  FakeRatePt_madgraph->GetYaxis()->SetTitle("TrackerMuon Fake Rate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName("TrackerMuonFakeRateFunction_Pt_MadgraphWJets");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph W+Jets");

 
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Tracker Muon Fake Rates
  //Compare for different W decay channels
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRatePt_pythia = createFakeRatePtHist(pythiaWeDatasetFiles, pythiaWeDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "TrackerMuonFakeRate_Pt_PythiaWJets_We");
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("TrackerMuonMuonFakeRateFunction_Pt_PythiaWJets_We");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("TrackerMuonMuon Pythia W->enu+Jets");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaWmDatasetFiles, pythiaWmDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "TrackerMuonFakeRate_Pt_PythiaWJets_Wm");
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("TrackerMuonMuonFakeRateFunction_Pt_PythiaWJets_Wm");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("TrackerMuonMuon Pythia W->munu+Jets");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaWtDatasetFiles, pythiaWtDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "TrackerMuonFakeRate_Pt_PythiaWJets_Wt");
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("TrackerMuonMuonFakeRateFunction_Pt_PythiaWJets_Wt");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("TrackerMuonMuon Pythia W->taunu+Jets");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Tracker Muon Fake Rates
  //Compare for different Madgraph jets samples
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRatePt_madgraph = createFakeRatePtHist(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "TrackerMuonFakeRate_Pt_MadgraphWJets_LightJets");
  FakeRatePt_madgraph->GetYaxis()->SetTitle("TrackerMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName("TrackerMuonFakeRateFunction_Pt_MadgraphWJets_LightJets");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph W+LightJets");

  FakeRatePt_madgraph = createFakeRatePtHist(madgraphVQQDatasetFiles, madgraphVQQDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "TrackerMuonFakeRate_Pt_MadgraphWJets_VQQ");
  FakeRatePt_madgraph->GetYaxis()->SetTitle("TrackerMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName("TrackerMuonFakeRateFunction_Pt_MadgraphWJets_VQQ");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph VQQ");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Global Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();
  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaDatasetFiles, pythiaDatasetNames, "ComputeMuonFakeRateMod_Global",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "GlobalMuonFakeRate_Pt_PythiaWJets");
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("GlobalMuonFakeRateFunction_Pt_PythiaWJets");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia W+Jets");

  
  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(
    madgraphDatasetFiles, madgraphDatasetNames, "ComputeMuonFakeRateMod_Global",
    "hMuonNumeratorPt","hMuonDenominatorPt", "GlobalMuonFakeRate_Pt_MadgraphWJets");
  FakeRatePt_madgraph->GetYaxis()->SetTitle("GlobalMuon Fake Rate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName("GlobalMuonFakeRateFunction_Pt_MadgraphWJets");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph W+Jets");

 
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Global Muon Fake Rates
  //Compare for different W decay channels
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRatePt_pythia = createFakeRatePtHist(pythiaWeDatasetFiles, pythiaWeDatasetNames, "ComputeMuonFakeRateMod_Global",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "GlobalMuonFakeRate_Pt_PythiaWJets_We");
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("GlobalMuonFakeRateFunction_Pt_PythiaWJets_We");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia W->enu+Jets");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaWmDatasetFiles, pythiaWmDatasetNames, "ComputeMuonFakeRateMod_Global",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "GlobalMuonFakeRate_Pt_PythiaWJets_Wm");
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("GlobalMuonFakeRateFunction_Pt_PythiaWJets_Wm");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia W->munu+Jets");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaWtDatasetFiles, pythiaWtDatasetNames, "ComputeMuonFakeRateMod_Global",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "GlobalMuonFakeRate_Pt_PythiaWJets_Wt");
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName("GlobalMuonFakeRateFunction_Pt_PythiaWJets_Wt");
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia W->taunu+Jets");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Global Muon Fake Rates
  //Compare for different Madgraph jets samples
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRatePt_madgraph = createFakeRatePtHist(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeMuonFakeRateMod_Global",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "GlobalMuonFakeRate_Pt_MadgraphWJets_LightJets");
  FakeRatePt_madgraph->GetYaxis()->SetTitle("GlobalMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName("GlobalMuonFakeRateFunction_Pt_MadgraphWJets_LightJets");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph W+LightJets");

  FakeRatePt_madgraph = createFakeRatePtHist(madgraphVQQDatasetFiles, madgraphVQQDatasetNames, "ComputeMuonFakeRateMod_Global",
                                           "hMuonNumeratorPt","hMuonDenominatorPt", "GlobalMuonFakeRate_Pt_MadgraphWJets_VQQ");
  FakeRatePt_madgraph->GetYaxis()->SetTitle("GlobalMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName("GlobalMuonFakeRateFunction_Pt_MadgraphWJets_VQQ");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph VQQ");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //IsoTrack Muon Fake Rate as function of Eta
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  cout << "start\n";

  //Pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaDatasetFiles, pythiaDatasetNames, "ComputeMuonFakeRateMod",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "IsoTrackMuonFakeRate_Eta_PythiaWJets");
  FakeRateEta_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("IsoTrackMuonFakeRateFunction_Eta_PythiaWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia W+Jets");


  //madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphDatasetFiles, madgraphDatasetNames, "ComputeMuonFakeRateMod",
                                             "hMuonNumeratorEta","hMuonDenominatorEta", "IsoTrackMuonFakeRate_Eta_MadgraphWJets");
  FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("IsoTrackMuonFakeRateFunction_Eta_MadgraphWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph W+Jets");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //IsoTrack Muon Fake Rate
  //Compare Eta Fake Rate across different W channels
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRateEta_pythia = createFakeRateEtaHist(pythiaWeDatasetFiles, pythiaWeDatasetNames, "ComputeMuonFakeRateMod",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "IsoTrackMuonFakeRate_Eta_PythiaWJets_We");
  FakeRateEta_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("IsoTrackMuonFakeRateFunction_Eta_PythiaWJets_We");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia W->enu+Jets");

  FakeRateEta_pythia = createFakeRateEtaHist(pythiaWmDatasetFiles, pythiaWmDatasetNames, "ComputeMuonFakeRateMod",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "IsoTrackMuonFakeRate_Eta_PythiaWJets_Wm");
  FakeRateEta_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("IsoTrackMuonFakeRateFunction_Eta_PythiaWJets_Wm");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia W->munu+Jets");

  FakeRateEta_pythia = createFakeRateEtaHist(pythiaWtDatasetFiles, pythiaWtDatasetNames, "ComputeMuonFakeRateMod",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "IsoTrackMuonFakeRate_Eta_PythiaWJets_Wt");
  FakeRateEta_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("IsoTrackMuonFakeRateFunction_Eta_PythiaWJets_Wt");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia W->taunu+Jets");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //IsoTrack Muon Muon Fake Rates Vs Eta
  //Compare for different Madgraph jets samples
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeMuonFakeRateMod",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "IsoTrackMuonFakeRate_Eta_MadgraphWJets_LightJets");
  FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName("IsoTrackMuonFakeRateFunction_Eta_MadgraphWJets_LightJets");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph W+LightJets");

  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphVQQDatasetFiles, madgraphVQQDatasetNames, "ComputeMuonFakeRateMod",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "IsoTrackMuonFakeRate_Eta_MadgraphWJets_VQQ");
  FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName("IsoTrackMuonFakeRateFunction_Eta_MadgraphWJets_VQQ");
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph VQQ");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);



  //*****************************************************************************************
  //TrackerMuon Fake Rate as function of Eta
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //Pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaDatasetFiles, pythiaDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "TrackerMuonFakeRate_Eta_PythiaWJets");
  FakeRateEta_pythia->GetYaxis()->SetTitle("TrackerMuonFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("TrackerMuonFakeRateFunction_Eta_PythiaWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia W+Jets");


  //madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphDatasetFiles, madgraphDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon",
                                             "hMuonNumeratorEta","hMuonDenominatorEta", "TrackerMuonFakeRate_Eta_MadgraphWJets");
  FakeRateEta_madgraph->GetYaxis()->SetTitle("TrackerMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("TrackerMuonFakeRateFunction_Eta_MadgraphWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph W+Jets");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Tracker Muon Fake Rates Vs Eta
  //Compare for different W decay channels
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRateEta_pythia = createFakeRateEtaHist(pythiaWeDatasetFiles, pythiaWeDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "TrackerMuonFakeRate_Eta_PythiaWJets_We");
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("TrackerMuonFakeRateFunction_Eta_PythiaWJets_We");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia W->enu+Jets");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaWmDatasetFiles, pythiaWmDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "TrackerMuonFakeRate_Eta_PythiaWJets_Wm");
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("TrackerMuonFakeRateFunction_Eta_PythiaWJets_Wm");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia W->munu+Jets");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaWtDatasetFiles, pythiaWtDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "TrackerMuonFakeRate_Eta_PythiaWJets_Wt");
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("TrackerMuonFakeRateFunction_Eta_PythiaWJets_Wt");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia W->taunu+Jets");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Tracker Muon Fake Rates
  //Compare for different Madgraph jets samples
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "TrackerMuonFakeRate_Eta_MadgraphWJets_LightJets");
  FakeRateEta_madgraph->GetYaxis()->SetTitle("TrackerMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("TrackerMuonFakeRateFunction_Eta_MadgraphWJets_LightJets");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph W+LightJets");

  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphVQQDatasetFiles, madgraphVQQDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "TrackerMuonFakeRate_Eta_MadgraphWJets_VQQ");
  FakeRateEta_madgraph->GetYaxis()->SetTitle("TrackerMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("TrackerMuonFakeRateFunction_Eta_MadgraphWJets_VQQ");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph VQQ");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);






  //*****************************************************************************************
  //Global Fake Rate as function of Eta
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //Pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaDatasetFiles, pythiaDatasetNames, "ComputeMuonFakeRateMod_Global",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "GlobalMuonFakeRate_Eta_PythiaWJets");
  FakeRateEta_pythia->GetYaxis()->SetTitle("GlobalMuonFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("GlobalMuonFakeRateFunction_Eta_PythiaWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia W+Jets");


  //madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphDatasetFiles, madgraphDatasetNames, "ComputeMuonFakeRateMod_Global",
                                             "hMuonNumeratorEta","hMuonDenominatorEta", "GlobalMuonFakeRate_Eta_MadgraphWJets");
  FakeRateEta_madgraph->GetYaxis()->SetTitle("GlobalMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("GlobalMuonFakeRateFunction_Eta_MadgraphWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph W+Jets");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Global Muon Fake Rates Vs Eta
  //Compare for different W decay channels
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRateEta_pythia = createFakeRateEtaHist(pythiaWeDatasetFiles, pythiaWeDatasetNames, "ComputeMuonFakeRateMod_Global",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "GlobalMuonFakeRate_Eta_PythiaWJets_We");
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("GlobalMuonFakeRateFunction_Eta_PythiaWJets_We");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia W->enu+Jets");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaWmDatasetFiles, pythiaWmDatasetNames, "ComputeMuonFakeRateMod_Global",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "GlobalMuonFakeRate_Eta_PythiaWJets_Wm");
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("GlobalMuonFakeRateFunction_Eta_PythiaWJets_Wm");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia W->munu+Jets");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaWtDatasetFiles, pythiaWtDatasetNames, "ComputeMuonFakeRateMod_Global",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "GlobalMuonFakeRate_Eta_PythiaWJets_Wt");
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    //FakeRateFitFunction_pythia->SetName("GlobalMuonFakeRateFunction_Eta_PythiaWJets_Wt");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia W->taunu+Jets");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Global Muon Fake Rates
  //Compare for different Madgraph jets samples
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "ComputeMuonFakeRateMod_Global",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "GlobalMuonFakeRate_Eta_MadgraphWJets_LightJets");
  FakeRateEta_madgraph->GetYaxis()->SetTitle("GlobalMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("GlobalMuonFakeRateFunction_Eta_MadgraphWJets_LightJets");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph W+LightJets");

  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphVQQDatasetFiles, madgraphVQQDatasetNames, "ComputeMuonFakeRateMod_Global",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", "GlobalMuonFakeRate_Eta_MadgraphWJets_VQQ");
  FakeRateEta_madgraph->GetYaxis()->SetTitle("GlobalMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("GlobalMuonFakeRateFunction_Eta_MadgraphWJets_VQQ");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph VQQ");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);



  file->Close();
  return;

}


//--------------------------------------------------------------------------------------------------
// Do Fake Rate Computation From WJets sample
//--------------------------------------------------------------------------------------------------
void compute2DMuonFakeRatesFromWJets( char *FakeRateFilename ,bool fitFakeRate) {

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
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601/ComputeMuonFakeRate_s8-wjets-mg-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601/ComputeMuonFakeRate_s8-vqq-mg-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-vqq-mg-id11");

  //Pythia WJets
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090519/ComputeMuonFakeRate_s8-we-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-we-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090519/ComputeMuonFakeRate_s8-wm-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-wm-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090519/ComputeMuonFakeRate_s8-wt-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-wt-id11");
  
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
  //IsoTrack Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //pythia version
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaDatasetFiles, pythiaDatasetNames, "ComputeMuonFakeRateMod",
                                                 "hMuonNumeratorPtEta","hMuonDenominatorPtEta", "IsoTrackMuonFakeRate_PtEta_PythiaWJets");
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateHist(FakeRatePtEta_pythia);
    //FakeRateFitFunction_pythia->SetName("IsoTrackMuonFakeRateFunction_PtEta_PythiaWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia W+Jets");

  
  //madgraph version
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist(madgraphDatasetFiles, madgraphDatasetNames, "ComputeMuonFakeRateMod",
                                                   "hMuonNumeratorPtEta","hMuonDenominatorPtEta", "IsoTrackMuonFakeRate_PtEta_MadgraphWJets");
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePtEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("IsoTrackMuonFakeRateFunction_PtEta_MadgraphWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph W+Jets");


  printFakeRate(fakeRateHistograms,fakeRateFits,fakeRateLegendNames, "IsoTrackMuonFakeRatePt");
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Tracker Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //pythia version
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaDatasetFiles, pythiaDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon",
                                              "hMuonNumeratorPtEta","hMuonDenominatorPtEta", "TrackerMuonFakeRate_PtEta_PythiaWJets");
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateHist(FakeRatePtEta_pythia);
    //FakeRateFitFunction_pythia->SetName("TrackerMuonFakeRateFunction_PtEta_PythiaWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia W+Jets");

  
  //madgraph version
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist( madgraphDatasetFiles, madgraphDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon",
                                                 "hMuonNumeratorPtEta","hMuonDenominatorPtEta", "TrackerMuonFakeRate_PtEta_MadgraphWJets");
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRateHist(FakeRatePtEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("TrackerMuonFakeRateFunction_Pt_MadgraphWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph W+Jets");
 

  printFakeRate(fakeRateHistograms,fakeRateFits,fakeRateLegendNames, "TrackerMuonFakeRatePtEta");
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);



  //*****************************************************************************************
  //Global Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //pythia version
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaDatasetFiles, pythiaDatasetNames, "ComputeMuonFakeRateMod_Global",
                                              "hMuonNumeratorPtEta","hMuonDenominatorPtEta", "GlobalMuonFakeRate_PtEta_PythiaWJets");
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateHist(FakeRatePtEta_pythia);
    //FakeRateFitFunction_pythia->SetName("GlobalMuonFakeRateFunction_PtEta_PythiaWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia W+Jets");

  
  //madgraph version
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist( madgraphDatasetFiles, madgraphDatasetNames, "ComputeMuonFakeRateMod_Global",
                                                 "hMuonNumeratorPtEta","hMuonDenominatorPtEta", "GlobalMuonFakeRate_PtEta_MadgraphWJets");
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRateHist(FakeRatePtEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("GlobalMuonFakeRateFunction_Pt_MadgraphWJets");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph W+Jets");
 

  printFakeRate(fakeRateHistograms,fakeRateFits,fakeRateLegendNames, "GlobalMuonFakeRatePtEta");
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  file->Close();
  return;

}



void compute1DMuonFakeRatesFromQCD( char *FakeRateFilename ,bool fitFakeRate, string JetTrigger) {

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
  madgraphQCD500To1000DatasetFiles.clear(); 
  madgraphQCD500To1000DatasetNames.clear();
  madgraphQCD1000ToInfDatasetFiles.clear(); 
  madgraphQCD1000ToInfDatasetNames.clear();

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

  //Madgraph JetTrigger Sample
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("f8-qcd100_250-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("f8-qcd1000_inf-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("s8-pj40_100-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("s8-pj100_200-id11");
  madgraphJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphJetTriggerDatasetNames.push_back("s8-pj200_inf-id11");


  //Madgraph QCD
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd1000_inf-id11");
  madgraphQCD100To250DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphQCD100To250DatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCD250To500DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphQCD250To500DatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCD500To1000DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphQCD500To1000DatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphQCD1000ToInfDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphQCD1000ToInfDatasetNames.push_back("f8-qcd1000_inf-id11");
 

  //Pythia JetTrigger Sample
//   pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd15-id11_noskim.root");
//   pythiaJetTriggerDatasetNames.push_back("s8-qcd15-id11");
//   pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd30-id11_noskim.root");
//   pythiaJetTriggerDatasetNames.push_back("s8-qcd30-id11");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-qcd80-id11");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-qcd170-id11");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-pj15-id12_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-pj15-id12");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-pj30-id11_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-pj30-id11");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-pj80-id12_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-pj80-id12");
  pythiaJetTriggerDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-pj170-id11_noskim.root");
  pythiaJetTriggerDatasetNames.push_back("s8-pj170-id11");


  //Pythia QCD
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd15-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd30-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd80-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd170-id11");
  pythiaQCD15DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaQCD15DatasetNames.push_back("s8-qcd15-id11");
  pythiaQCD30DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaQCD30DatasetNames.push_back("s8-qcd30-id11");
  pythiaQCD80DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaQCD80DatasetNames.push_back("s8-qcd80-id11");
  pythiaQCD170DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd170-id11_noskim.root");
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
  //IsoTrack Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_MadgraphQCD_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_MadgraphQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD");


  //Full JetTrigger version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_Madgraph_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_Madgraph_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph JetTrigger");


  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_PythiaQCD_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Pt_PythiaQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia QCD");


  //Full JetTrigger version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaJetTriggerDatasetFiles, pythiaJetTriggerDatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_Pythia_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Pt_Pythia_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia JetTrigger");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);



   //*****************************************************************************************
   //IsoTrack Muon Fake Rates For madgraph QCD, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();

   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_MadgraphQCD100To250_" + JetTrigger).c_str());
   FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_MadgraphQCD100To250_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD 100To250");


  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_MadgraphQCD250To500_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_MadgraphQCD250To500_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD 250To500");


  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_MadgraphQCD500To1000_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_MadgraphQCD500To1000_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD 500To1000");


  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD 1000ToInf");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


   //*****************************************************************************************
   //IsoTrack Muon Fake Rates For pythia QCD, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();

   FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD15DatasetFiles, pythiaQCD15DatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_PythiaQCD15_" + JetTrigger).c_str());
   FakeRatePt_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Pt_PythiaQCD15_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia QCD 15");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD30DatasetFiles, pythiaQCD30DatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_PythiaQCD30_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Pt_PythiaQCD30_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia QCD 30");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD80DatasetFiles, pythiaQCD80DatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_PythiaQCD80_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Pt_PythiaQCD80_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia QCD 80");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD170DatasetFiles, pythiaQCD170DatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_PythiaQCD170_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Pt_PythiaQCD170_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia QCD 170");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);



  //*****************************************************************************************
  //Global Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeMuonFakeRateMod_Global_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_MadgraphQCD_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_MadgraphQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD");


  //JetTrigger version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, ("ComputeMuonFakeRateMod_Global_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_Madgraph_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_Madgraph_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph JetTrigger");


  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeMuonFakeRateMod_Global_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_PythiaQCD_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Pt_PythiaQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia QCD");


  //JetTrigger version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaJetTriggerDatasetFiles, pythiaJetTriggerDatasetNames, ("ComputeMuonFakeRateMod_Global_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_Pythia_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Pt_Pythia_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia JetTrigger");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //TrackerMuon Muon Fake Rates For Madgraph QCD, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();


  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_MadgraphQCD100To250_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_MadgraphQCD100To250_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD 100To250");


  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_MadgraphQCD250To500_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_MadgraphQCD250To500_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD 250To500");


  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_MadgraphQCD500To1000_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_MadgraphQCD500To1000_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD 500To1000");


  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD 1000ToInf");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //TrackerMuon Muon Fake Rates For Pythia QCD, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();


  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD15DatasetFiles, pythiaQCD15DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_PythiaQCD15_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Pt_PythiaQCD15_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia QCD 15");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD30DatasetFiles, pythiaQCD30DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_PythiaQCD30_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Pt_PythiaQCD30_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia QCD 30");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD80DatasetFiles, pythiaQCD80DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_PythiaQCD80_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Pt_PythiaQCD80_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia QCD 80");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD170DatasetFiles, pythiaQCD170DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_PythiaQCD170_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Pt_PythiaQCD170_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia QCD 170");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);




  //*****************************************************************************************
  //TrackerMuon Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_MadgraphQCD_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_MadgraphQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD");


  //JetTrigger version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_Madgraph_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_Madgraph_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph JetTrigger");


  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_PythiaQCD_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Pt_PythiaQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia QCD");


  //JetTrigger version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaJetTriggerDatasetFiles, pythiaJetTriggerDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_Pythia_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Pt_Pythia_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia JetTrigger");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Global Muon Fake Rates For Madgraph QCD, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();


  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeMuonFakeRateMod_Global_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_MadgraphQCD100To250_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_MadgraphQCD100To250_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD 100To250");


  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeMuonFakeRateMod_Global_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_MadgraphQCD250To500_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_MadgraphQCD250To500_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD 250To500");


  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeMuonFakeRateMod_Global_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_MadgraphQCD500To1000_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_MadgraphQCD500To1000_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD 500To1000");


  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeMuonFakeRateMod_Global_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD 1000ToInf");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);



  //*****************************************************************************************
  //Global Muon Fake Rates For Pythia QCD, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();


  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD15DatasetFiles, pythiaQCD15DatasetNames, ("ComputeMuonFakeRateMod_Global_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_PythiaQCD15_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Pt_PythiaQCD15_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia QCD 15");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD30DatasetFiles, pythiaQCD30DatasetNames, ("ComputeMuonFakeRateMod_Global_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_PythiaQCD30_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Pt_PythiaQCD30_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia QCD 30");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD80DatasetFiles, pythiaQCD80DatasetNames, ("ComputeMuonFakeRateMod_Global_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_PythiaQCD80_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Pt_PythiaQCD80_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia QCD 80");


  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCD170DatasetFiles, pythiaQCD170DatasetNames, ("ComputeMuonFakeRateMod_Global_" + JetTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_PythiaQCD170_" + JetTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Pt_PythiaQCD170_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia QCD 170");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //IsoTrack Muon Fake Rates Vs Eta
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  
  //madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_MadgraphQCD_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Eta_MadgraphQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD");


  //JetTrigger version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_Madgraph_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Eta_Madgraph_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph JetTrigger");


  //pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_PythiaQCD_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Eta_PythiaQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia QCD");


  //JetTrigger version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaJetTriggerDatasetFiles, pythiaJetTriggerDatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_Pythia_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Eta_Pythia_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia JetTrigger");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);



   //*****************************************************************************************
   //IsoTrack Muon Fake Rates For Madgraph QCD, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();


   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                                "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_MadgraphQCD100To250_" + JetTrigger).c_str());
   FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
   
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Eta_MadgraphQCD100To250_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
   fakeRateHistograms.push_back(FakeRateEta_madgraph);
   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD 100To250");
   

   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                                "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_MadgraphQCD250To500_" + JetTrigger).c_str());
   FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
   
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Eta_MadgraphQCD250To500_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
   fakeRateHistograms.push_back(FakeRateEta_madgraph);
   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD 250To500");


   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                                "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_MadgraphQCD500To1000_" + JetTrigger).c_str());
   FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
   
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Eta_MadgraphQCD500To1000_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
   fakeRateHistograms.push_back(FakeRateEta_madgraph);
   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD 500To1000");


   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                                "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
   FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
   
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Eta_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
   fakeRateHistograms.push_back(FakeRateEta_madgraph);
   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD 1000ToInf");


   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


   //*****************************************************************************************
   //IsoTrack Muon Fake Rates For Pythia QCD, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();


   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD15DatasetFiles, pythiaQCD15DatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                                "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_PythiaQCD15_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
   
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Eta_PythiaQCD15_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("IsoTrackMuon Pythia QCD 15");
   

   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD30DatasetFiles, pythiaQCD30DatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                                "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_PythiaQCD30_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
   
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Eta_PythiaQCD30_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("IsoTrackMuon Pythia QCD 30");


   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD80DatasetFiles, pythiaQCD80DatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                                "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_PythiaQCD80_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
   
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Eta_PythiaQCD80_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("IsoTrackMuon Pythia QCD 80");


   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD170DatasetFiles, pythiaQCD170DatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                                "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_PythiaQCD170_" + JetTrigger).c_str());
   FakeRateEta_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
   
   FakeRateFitFunction_pythia = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
     FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Eta_PythiaQCD170_" + JetTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
   fakeRateHistograms.push_back(FakeRateEta_pythia);
   fakeRateLegendNames.push_back("IsoTrackMuon Pythia QCD 170");


   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);





  //*****************************************************************************************
  //TrackerMuon Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();


  //madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_MadgraphQCD_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Eta_MadgraphQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD");


  //JetTrigger version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_Madgraph_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Eta_Madgraph_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph JetTrigger");


  //pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_PythiaQCD_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Eta_PythiaQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia QCD");


  //JetTrigger version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaJetTriggerDatasetFiles, pythiaJetTriggerDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_Pythia_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Eta_Pythia_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia JetTrigger");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);



  //*****************************************************************************************
  //TrackerMuon Muon Fake Rates For QCD Madgraph, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_MadgraphQCD100To250_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Eta_MadgraphQCD100To250_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD 100To250");


  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_MadgraphQCD250To500_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Eta_MadgraphQCD250To500_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD 250To500");


  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_MadgraphQCD500To1000_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Eta_MadgraphQCD500To1000_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD 500To1000");


  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Eta_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD 1000ToInf");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);



  //*****************************************************************************************
  //TrackerMuon Muon Fake Rates For QCD Pythia, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD15DatasetFiles, pythiaQCD15DatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_PythiaQCD15_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Eta_PythiaQCD15_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia QCD 15");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD30DatasetFiles, pythiaQCD30DatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_PythiaQCD30_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Eta_PythiaQCD30_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia QCD 30");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD80DatasetFiles, pythiaQCD80DatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_PythiaQCD80_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Eta_PythiaQCD80_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia QCD 80");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD170DatasetFiles, pythiaQCD170DatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_PythiaQCD170_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Eta_PythiaQCD170_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia QCD 170");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);








  //*****************************************************************************************
  //Global Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();


  //madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeMuonFakeRateMod_Global_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_MadgraphQCD_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Eta_MadgraphQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD");


  //JetTrigger version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphJetTriggerDatasetFiles, madgraphJetTriggerDatasetNames, "ComputeMuonFakeRateMod_Global_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_Madgraph_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Eta_Madgraph_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph JetTrigger");


  //pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, "ComputeMuonFakeRateMod_Global_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_PythiaQCD_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Eta_PythiaQCD_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia QCD");


  //JetTrigger version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaJetTriggerDatasetFiles, pythiaJetTriggerDatasetNames, "ComputeMuonFakeRateMod_Global_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_Pythia_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Eta_Pythia_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia JetTrigger");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);



  //*****************************************************************************************
  //Global Muon Fake Rates For QCD Madgraph, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, "ComputeMuonFakeRateMod_Global_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_MadgraphQCD100To250_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Eta_MadgraphQCD100To250_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD 100To250");


  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, "ComputeMuonFakeRateMod_Global_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_MadgraphQCD250To500_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Eta_MadgraphQCD250To500_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD 250To500");


  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, "ComputeMuonFakeRateMod_Global_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_MadgraphQCD500To1000_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Eta_MadgraphQCD500To1000_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD 500To1000");


  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, "ComputeMuonFakeRateMod_Global_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Eta_MadgraphQCD1000ToInf_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD 1000ToInf");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Global Muon Fake Rates For QCD Pythia, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD15DatasetFiles, pythiaQCD15DatasetNames, "ComputeMuonFakeRateMod_Global_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_PythiaQCD15_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Eta_PythiaQCD15_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia QCD 15");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD30DatasetFiles, pythiaQCD30DatasetNames, "ComputeMuonFakeRateMod_Global_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_PythiaQCD30_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Eta_PythiaQCD30_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia QCD 30");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD80DatasetFiles, pythiaQCD80DatasetNames, "ComputeMuonFakeRateMod_Global_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_PythiaQCD80_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Eta_PythiaQCD80_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia QCD 80");


  FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCD170DatasetFiles, pythiaQCD170DatasetNames, "ComputeMuonFakeRateMod_Global_Jet30",
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_PythiaQCD170_" + JetTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Eta_PythiaQCD170_" + JetTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia QCD 170");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);







  file->Close();
  return;

}



//--------------------------------------------------------------------------------------------------
// Do Fake Rate Computation From WJets sample
//--------------------------------------------------------------------------------------------------
void compute2DMuonFakeRatesFromQCD( char *FakeRateFilename ,bool fitFakeRate, string JetTrigger) {

  //Samples
  vector<string> madgraphJetTriggerSampleDatasetFiles;
  vector<string> madgraphJetTriggerSampleDatasetNames;
  vector<string> madgraphQCDDatasetFiles;
  vector<string> madgraphQCDDatasetNames;


  vector<string> pythiaJetTriggerSampleDatasetFiles;
  vector<string> pythiaJetTriggerSampleDatasetNames;
  vector<string> pythiaQCDDatasetFiles;
  vector<string> pythiaQCDDatasetNames;


  //*****************************************************************************************
  //Do FakeRate Computation 
  //*****************************************************************************************

  //Create root file to store fake rates
  TFile *file = new TFile(FakeRateFilename, "UPDATE");
  file->cd();
  
  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************
  madgraphQCDDatasetFiles.clear(); 
  madgraphQCDDatasetNames.clear();
  madgraphJetTriggerSampleDatasetFiles.clear();
  madgraphJetTriggerSampleDatasetNames.clear();

  pythiaQCDDatasetFiles.clear(); 
  pythiaQCDDatasetNames.clear();
  pythiaJetTriggerSampleDatasetFiles.clear();
  pythiaJetTriggerSampleDatasetNames.clear();
 
  //Madgraph JetTriggerSample
  madgraphJetTriggerSampleDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphJetTriggerSampleDatasetNames.push_back("f8-qcd100_250-id11");
  madgraphJetTriggerSampleDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphJetTriggerSampleDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphJetTriggerSampleDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphJetTriggerSampleDatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphJetTriggerSampleDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphJetTriggerSampleDatasetNames.push_back("f8-qcd1000_inf-id11");
  madgraphJetTriggerSampleDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphJetTriggerSampleDatasetNames.push_back("s8-pj40_100-id11");
  madgraphJetTriggerSampleDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphJetTriggerSampleDatasetNames.push_back("s8-pj100_200-id11");
  madgraphJetTriggerSampleDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphJetTriggerSampleDatasetNames.push_back("s8-pj200_inf-id11");

 
  //Madgraph QCD
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd1000_inf-id11");
 

   //Pythia JetTriggerSample
//   pythiaJetTriggerSampleDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd15-id11_noskim.root");
//   pythiaJetTriggerSampleDatasetNames.push_back("s8-qcd15-id11");
//   pythiaJetTriggerSampleDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd30-id11_noskim.root");
//   pythiaJetTriggerSampleDatasetNames.push_back("s8-qcd30-id11");
  pythiaJetTriggerSampleDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaJetTriggerSampleDatasetNames.push_back("s8-qcd80-id11");
  pythiaJetTriggerSampleDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaJetTriggerSampleDatasetNames.push_back("s8-qcd170-id11");
  pythiaJetTriggerSampleDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-pj15-id12_noskim.root");
  pythiaJetTriggerSampleDatasetNames.push_back("s8-pj15-id12");
  pythiaJetTriggerSampleDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-pj30-id11_noskim.root");
  pythiaJetTriggerSampleDatasetNames.push_back("s8-pj30-id11");
  pythiaJetTriggerSampleDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-pj80-id12_noskim.root");
  pythiaJetTriggerSampleDatasetNames.push_back("s8-pj80-id12");
  pythiaJetTriggerSampleDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-pj170-id11_noskim.root");
  pythiaJetTriggerSampleDatasetNames.push_back("s8-pj170-id11");

 
  //Pythia QCD
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd15-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd30-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd80-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_JetTriggerSample/ComputeMuonFakeRate_JetTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd170-id11");
 

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
  //IsoTrack Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();


  //madgraph version
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("IsoTrackMuonFakeRate_PtEta_MadgraphQCD_" + JetTrigger).c_str());
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePtEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_MadgraphQCD_" + JetTrigger).c_str());
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD");


  //madgraph version
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist(madgraphJetTriggerSampleDatasetFiles, madgraphJetTriggerSampleDatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("IsoTrackMuonFakeRate_PtEta_Madgraph_" + JetTrigger).c_str());
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePtEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_Madgraph_" + JetTrigger).c_str());
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph JetTriggerSample");


  //pythia version
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("IsoTrackMuonFakeRate_PtEta_PythiaQCD_" + JetTrigger).c_str());
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePtEta_pythia);
    //FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Pt_PythiaQCD_" + JetTrigger).c_str());
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia QCD");


  //pythia version
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaJetTriggerSampleDatasetFiles, pythiaJetTriggerSampleDatasetNames, ("ComputeMuonFakeRateMod_" + JetTrigger).c_str(),
                                             "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("IsoTrackMuonFakeRate_PtEta_Pythia_" + JetTrigger).c_str());
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePtEta_pythia);
    //FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Pt_Pythia_" + JetTrigger).c_str());
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia JetTriggerSample");

    
  printFakeRate(fakeRateHistograms,fakeRateFits,fakeRateLegendNames, ("IsoTrackMuonFakeRatePtEta_" + JetTrigger).c_str());
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //TrackerMuon Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph version
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet30",
                                             "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("TrackerMuonFakeRate_PtEta_MadgraphQCD_" + JetTrigger).c_str());
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePtEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_MadgraphQCD_" + JetTrigger).c_str());
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD");


  //madgraph version
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist(madgraphJetTriggerSampleDatasetFiles, madgraphJetTriggerSampleDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + JetTrigger).c_str(),
                                                   "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("TrackerMuonFakeRate_PtEta_Madgraph_" + JetTrigger).c_str());
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePtEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_Madgraph_" + JetTrigger).c_str());
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph JetTriggerSample");


  //pythia version
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, "ComputeMuonFakeRateMod_TrackerMuon_Jet30",
                                             "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("TrackerMuonFakeRate_PtEta_PythiaQCD_" + JetTrigger).c_str());
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePtEta_pythia);
    //FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Pt_PythiaQCD_" + JetTrigger).c_str());
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia QCD");


  //pythia version
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaJetTriggerSampleDatasetFiles, pythiaJetTriggerSampleDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + JetTrigger).c_str(),
                                                   "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("TrackerMuonFakeRate_PtEta_Pythia_" + JetTrigger).c_str());
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePtEta_pythia);
    //FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Pt_Pythia_" + JetTrigger).c_str());
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia JetTriggerSample");


  printFakeRate(fakeRateHistograms,fakeRateFits,fakeRateLegendNames, "TrackerMuonFakeRatePt QCD " + JetTrigger);
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);




  //*****************************************************************************************
  //Global Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph version
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, "ComputeMuonFakeRateMod_Global_Jet30",
                                             "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("GlobalMuonFakeRate_PtEta_MadgraphQCD_" + JetTrigger).c_str());
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePtEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_MadgraphQCD_" + JetTrigger).c_str());
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD");


  //madgraph version
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist(madgraphJetTriggerSampleDatasetFiles, madgraphJetTriggerSampleDatasetNames, ("ComputeMuonFakeRateMod_Global_" + JetTrigger).c_str(),
                                                   "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("GlobalMuonFakeRate_PtEta_Madgraph_" + JetTrigger).c_str());
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePtEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_Madgraph_" + JetTrigger).c_str());
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph JetTriggerSample");


  //pythia version
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, "ComputeMuonFakeRateMod_Global_Jet30",
                                             "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("GlobalMuonFakeRate_PtEta_PythiaQCD_" + JetTrigger).c_str());
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePtEta_pythia);
    //FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Pt_PythiaQCD_" + JetTrigger).c_str());
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia QCD");


  //pythia version
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaJetTriggerSampleDatasetFiles, pythiaJetTriggerSampleDatasetNames, ("ComputeMuonFakeRateMod_Global_" + JetTrigger).c_str(),
                                                   "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("GlobalMuonFakeRate_PtEta_Pythia_" + JetTrigger).c_str());
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePtEta_pythia);
    //FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Pt_Pythia_" + JetTrigger).c_str());
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia JetTriggerSample");


  printFakeRate(fakeRateHistograms,fakeRateFits,fakeRateLegendNames, "GlobalMuonFakeRatePt QCD " + JetTrigger);
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  file->Close();
  return;

}
 

void compute1DMuonFakeRatesFromPhotonSample( char *FakeRateFilename ,bool fitFakeRate, string PhotonTrigger) {

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
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-pj40_100-id11");
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-pj100_200-id11");
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-pj200_inf-id11");
//   madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd100_250-id11_noskim.root");
//   madgraphDatasetNames.push_back("f8-qcd100_250-id11");
//   madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd250_500-id11_noskim.root");
//   madgraphDatasetNames.push_back("f8-qcd250_500-id11");
//   madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd500_1000-id11_noskim.root");
//   madgraphDatasetNames.push_back("f8-qcd500_1000-id11");
//   madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd1000_inf-id11_noskim.root");
//   madgraphDatasetNames.push_back("f8-qcd1000_inf-id11");

  //Madgraph PhotonJet
  madgraphPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphPhotonJetsDatasetNames.push_back("s8-pj40_100-id11");
  madgraphPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphPhotonJetsDatasetNames.push_back("s8-pj100_200-id11");
  madgraphPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphPhotonJetsDatasetNames.push_back("s8-pj200_inf-id11");
  madgraphPhotonJets40To100DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphPhotonJets40To100DatasetNames.push_back("s8-pj40_100-id11");
  madgraphPhotonJets100To200DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphPhotonJets100To200DatasetNames.push_back("s8-pj100_200-id11");
  madgraphPhotonJets200ToInfDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphPhotonJets200ToInfDatasetNames.push_back("s8-pj200_inf-id11");


  //Madgraph QCD
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphQCDDatasetNames.push_back("f8-qcd1000_inf-id11");
  madgraphQCD100To250DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd100_250-id11_noskim.root");
  madgraphQCD100To250DatasetNames.push_back("f8-qcd100_250-id11");
  madgraphQCD250To500DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd250_500-id11_noskim.root");
  madgraphQCD250To500DatasetNames.push_back("f8-qcd250_500-id11");
  madgraphQCD500To1000DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd500_1000-id11_noskim.root");
  madgraphQCD500To1000DatasetNames.push_back("f8-qcd500_1000-id11");
  madgraphQCD1000ToInfDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd1000_inf-id11_noskim.root");
  madgraphQCD1000ToInfDatasetNames.push_back("f8-qcd1000_inf-id11");
  

  //All samples Together
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj15-id12_noskim.root");
  pythiaDatasetNames.push_back("s8-pj15-id12");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj30-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-pj30-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj80-id12_noskim.root");
  pythiaDatasetNames.push_back("s8-pj80-id12");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj170-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-pj170-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-qcd15-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-qcd30-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-qcd80-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-qcd170-id11");

  //Pythia PhotonJet
  pythiaPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj15-id12_noskim.root");
  pythiaPhotonJetsDatasetNames.push_back("s8-pj15-id12");
  pythiaPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj30-id11_noskim.root");
  pythiaPhotonJetsDatasetNames.push_back("s8-pj30-id11");
  pythiaPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj80-id12_noskim.root");
  pythiaPhotonJetsDatasetNames.push_back("s8-pj80-id12");
  pythiaPhotonJetsDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj170-id11_noskim.root");
  pythiaPhotonJetsDatasetNames.push_back("s8-pj170-id11");
  pythiaPhotonJets15DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj15-id12_noskim.root");
  pythiaPhotonJets15DatasetNames.push_back("s8-pj15-id12");
  pythiaPhotonJets30DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj30-id11_noskim.root");
  pythiaPhotonJets30DatasetNames.push_back("s8-pj30-id11");
  pythiaPhotonJets80DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj80-id12_noskim.root");
  pythiaPhotonJets80DatasetNames.push_back("s8-pj80-id12");
  pythiaPhotonJets170DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj170-id11_noskim.root");
  pythiaPhotonJets170DatasetNames.push_back("s8-pj170-id11");


  //Pythia QCD
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd15-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd30-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd80-id11");
  pythiaQCDDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaQCDDatasetNames.push_back("s8-qcd170-id11");
  pythiaQCD15DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaQCD15DatasetNames.push_back("s8-qcd15-id11");
  pythiaQCD30DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaQCD30DatasetNames.push_back("s8-qcd30-id11");
  pythiaQCD80DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaQCD80DatasetNames.push_back("s8-qcd80-id11");
  pythiaQCD170DatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd170-id11_noskim.root");
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
  //IsoTrack Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphDatasetFiles, madgraphDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_Madgraph_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_MadgraphQCD_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD");


  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaDatasetFiles, pythiaDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_Pythia_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Pt_PythiaQCD_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia QCD");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //IsoTrack Muon Fake Rates For QCD vs PhotonJets
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();
  
  //PhotonJets
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJetsDatasetFiles, madgraphPhotonJetsDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_MadgraphPhotonJets_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_MadgraphPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon MadgraphPhotonJets");
  
  //QCD
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_MadgraphQCD_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_MadgraphQCD_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia Madgraph QCD");


  //PhotonJets
  FakeRatePt_pythia = createFakeRatePtHist(pythiaPhotonJetsDatasetFiles, pythiaPhotonJetsDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_PythiaPhotonJets_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Pt_PythiaPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon PythiaPhotonJets");
  
  //QCD
  FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_PythiaQCD_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Pt_PythiaQCD_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia Pythia QCD");
  
 
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //IsoTrack Muon Fake Rates For PhotonJets, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();
  //PhotonJets 40To100
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJets40To100DatasetFiles, madgraphPhotonJets40To100DatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph PhotonJets40To100");
  
  
  //PhotonJets 100To200
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJets100To200DatasetFiles, madgraphPhotonJets100To200DatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph PhotonJets100To200");
  

  //PhotonJets 200ToInf
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJets200ToInfDatasetFiles, madgraphPhotonJets200ToInfDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
   }
   fakeRateHistograms.push_back(FakeRatePt_madgraph);
   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph PhotonJets200ToInf");


   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


//    //*****************************************************************************************
//    //IsoTrack Muon Fake Rates For madgraph QCD, different pthat bins
//    //*****************************************************************************************
//    fakeRateHistograms.clear();
//    fakeRateFits.clear();
//    fakeRateLegendNames.clear();

//    FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
//                                              "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//    FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

//    FakeRateFitFunction_madgraph = NULL;
//    if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD 100To250");


//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
//                                              "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD 250To500");


//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
//                                              "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD 500To1000");


//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
//                                              "hMuonNumeratorPt","hMuonDenominatorPt", ("IsoTrackMuonFakeRate_Pt_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Pt_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD 1000ToInf");


//   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);



  //*****************************************************************************************
  //TrackerMuon Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphDatasetFiles, madgraphDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_Madgraph_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_Madgraph_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph");


  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaDatasetFiles, pythiaDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_Pythia_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Pt_Pythia_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia");

  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //TrackerMuon Muon Fake Rates For different Sample Composition
  //*****************************************************************************************
  
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //PhotonJets
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJetsDatasetFiles, madgraphPhotonJetsDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_MadgraphPhotonJets_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_MadgraphPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph PhotonJets");

//   //QCD
//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_MadgraphQCD_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_MadgraphQCD_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD");


  //PhotonJets
  FakeRatePt_pythia = createFakeRatePtHist(pythiaPhotonJetsDatasetFiles, pythiaPhotonJetsDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_PythiaPhotonJets_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Pt_PythiaPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia PhotonJets");

//   //QCD
//   FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_PythiaQCD_" + PhotonTrigger).c_str());
//   FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_pythia = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
//     FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Pt_PythiaQCD_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_pythia);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_pythia);
//   fakeRateLegendNames.push_back("TrackerMuon Pythia QCD");

  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //TrackerMuon Muon Fake Rates For PhotonJets, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //PhotonJets40To100
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJets40To100DatasetFiles, madgraphPhotonJets40To100DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph PhotonJets40To100");

  //PhotonJets100To200
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJets100To200DatasetFiles, madgraphPhotonJets100To200DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph PhotonJets100To200");

  //PhotonJets200ToInf
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJets200ToInfDatasetFiles, madgraphPhotonJets200ToInfDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph PhotonJets200ToInf");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

//   //*****************************************************************************************
//   //TrackerMuon Muon Fake Rates For Madgraph QCD, different pthat bins
//   //*****************************************************************************************
//   fakeRateHistograms.clear();
//   fakeRateFits.clear();
//   fakeRateLegendNames.clear();


//   //madgraph version
//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD 100To250");


//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD 250To500");


//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD 500To1000");


//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorPt","hMuonDenominatorPt", ("TrackerMuonFakeRate_Pt_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD 1000ToInf");


//   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);






  //*****************************************************************************************
  //Global Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph version
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphDatasetFiles, madgraphDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_Madgraph_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_Madgraph_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph");


  //pythia version
  FakeRatePt_pythia = createFakeRatePtHist(pythiaDatasetFiles, pythiaDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_Pythia_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Pt_Pythia_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia");

  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Global Muon Fake Rates For different Sample Composition
  //*****************************************************************************************
  
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //PhotonJets
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJetsDatasetFiles, madgraphPhotonJetsDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_MadgraphPhotonJets_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_MadgraphPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph PhotonJets");

//   //QCD
//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_MadgraphQCD_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_MadgraphQCD_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD");


  //PhotonJets
  FakeRatePt_pythia = createFakeRatePtHist(pythiaPhotonJetsDatasetFiles, pythiaPhotonJetsDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_PythiaPhotonJets_" + PhotonTrigger).c_str());
  FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
    FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Pt_PythiaPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePt_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia PhotonJets");

//   //QCD
//   FakeRatePt_pythia = createFakeRatePtHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_PythiaQCD_" + PhotonTrigger).c_str());
//   FakeRatePt_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_pythia = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_pythia = fitFakeRatePtHist(FakeRatePt_pythia);
//     FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Pt_PythiaQCD_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_pythia);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_pythia);
//   fakeRateLegendNames.push_back("GlobalMuon Pythia QCD");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Global Muon Fake Rates For PhotonJets, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //PhotonJets40To100
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJets40To100DatasetFiles, madgraphPhotonJets40To100DatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph PhotonJets40To100");

  //PhotonJets100To200
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJets100To200DatasetFiles, madgraphPhotonJets100To200DatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph PhotonJets100To200");

  //PhotonJets200ToInf
  FakeRatePt_madgraph = createFakeRatePtHist(madgraphPhotonJets200ToInfDatasetFiles, madgraphPhotonJets200ToInfDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
  FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePt_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph PhotonJets200ToInf");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

//   //*****************************************************************************************
//   //Global Muon Fake Rates For Madgraph QCD, different pthat bins
//   //*****************************************************************************************
//   fakeRateHistograms.clear();
//   fakeRateFits.clear();
//   fakeRateLegendNames.clear();


//   //madgraph version
//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD 100To250");


//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD 250To500");


//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD 500To1000");


//   FakeRatePt_madgraph = createFakeRatePtHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorPt","hMuonDenominatorPt", ("GlobalMuonFakeRate_Pt_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//   FakeRatePt_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRatePtHist(FakeRatePt_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRatePt_madgraph);
//   fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD 1000ToInf");


//   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);










  //*****************************************************************************************
  //IsoTrack Muon Fake Rates Vs Eta
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphDatasetFiles, madgraphDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_Madgraph_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Eta_MadgraphQCD_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD");


  //pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaDatasetFiles, pythiaDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                             "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_Pythia_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Eta_PythiaQCD_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia QCD");

  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //IsoTrack Muon Fake Rates For QCD vs PhotonJets
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();
  
  //PhotonJets
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJetsDatasetFiles, madgraphPhotonJetsDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_MadgraphPhotonJets_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Eta_MadgraphPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph PhotonJets");
  
//   //QCD
//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_MadgraphQCD_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Eta_MadgraphQCD_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph MadgraphQCD");
  

  
  //PhotonJets
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaPhotonJetsDatasetFiles, pythiaPhotonJetsDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_PythiaPhotonJets_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Eta_PythiaPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia PhotonJets");
  
//   //QCD
//   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_PythiaQCD_" + PhotonTrigger).c_str());
//   FakeRateEta_pythia->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
//   FakeRateFitFunction_pythia = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
//     FakeRateFitFunction_pythia->SetName(("IsoTrackMuonFakeRateFunction_Eta_PythiaQCD_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_pythia);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_pythia);
//   fakeRateLegendNames.push_back("IsoTrackMuon Pythia QCD");
  
 
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


   //*****************************************************************************************
   //IsoTrack Muon Fake Rates For PhotonJets, different pthat bins
   //*****************************************************************************************
   fakeRateHistograms.clear();
   fakeRateFits.clear();
   fakeRateLegendNames.clear();
   //PhotonJets 40To100
   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJets40To100DatasetFiles, madgraphPhotonJets40To100DatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
   FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Eta_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
   }
   fakeRateHistograms.push_back(FakeRateEta_madgraph);
   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph PhotonJets40To100");


   //PhotonJets 100To200
   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJets100To200DatasetFiles, madgraphPhotonJets100To200DatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
   FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Eta_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
   }
   fakeRateHistograms.push_back(FakeRateEta_madgraph);
   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph PhotonJets100To200");


   //PhotonJets 200ToInf
   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJets200ToInfDatasetFiles, madgraphPhotonJets200ToInfDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
   FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
   FakeRateFitFunction_madgraph = NULL;
   if (fitFakeRate) {
     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Eta_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
   }
   fakeRateHistograms.push_back(FakeRateEta_madgraph);
   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph PhotonJets200ToInf");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


//    //*****************************************************************************************
//    //IsoTrack Muon Fake Rates For madgraph QCD, different pthat bins
//    //*****************************************************************************************
//    fakeRateHistograms.clear();
//    fakeRateFits.clear();
//    fakeRateLegendNames.clear();

//    FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
//                                              "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//    FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    

//    FakeRateFitFunction_madgraph = NULL;
//    if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Eta_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD 100To250");


//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
//                                              "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Eta_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD 250To500");


//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
//                                              "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Eta_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD 500To1000");


//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
//                                              "hMuonNumeratorEta","hMuonDenominatorEta", ("IsoTrackMuonFakeRate_Eta_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("IsoTrackMuonFakeRate");    
  
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("IsoTrackMuonFakeRateFunction_Eta_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("IsoTrackMuon Madgraph QCD 1000ToInf");


//   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);




  //*****************************************************************************************
  //TrackerMuon Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphDatasetFiles, madgraphDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_Madgraph_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Eta_Madgraph_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph");


  //pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaDatasetFiles, pythiaDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_Pythia_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Eta_Pythia_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //TrackerMuon Muon Fake Rates For different Sample Composition
  //*****************************************************************************************
  
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //PhotonJets
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJetsDatasetFiles, madgraphPhotonJetsDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_MadgraphPhotonJets_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Eta_MadgraphPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph PhotonJets");

//   //QCD
//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_MadgraphQCD_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Eta_MadgraphQCD_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD");


  //PhotonJets
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaPhotonJetsDatasetFiles, pythiaPhotonJetsDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_PythiaPhotonJets_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Eta_PythiaPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia PhotonJets");

//   //QCD
//   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_PythiaQCD_" + PhotonTrigger).c_str());
//   FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_pythia = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
//     FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Eta_PythiaQCD_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_pythia);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_pythia);
//   fakeRateLegendNames.push_back("TrackerMuon Pythia QCD");


   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //TrackerMuon Muon Fake Rates For PhotonJets, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //PhotonJets40To100
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJets40To100DatasetFiles, madgraphPhotonJets40To100DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Eta_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph PhotonJets40To100");

  //PhotonJets100To200
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJets100To200DatasetFiles, madgraphPhotonJets100To200DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Eta_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph PhotonJets100To200");

  //PhotonJets200ToInf
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJets200ToInfDatasetFiles, madgraphPhotonJets200ToInfDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Eta_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph PhotonJets200ToInf");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

//   //*****************************************************************************************
//   //TrackerMuon Muon Fake Rates For Madgraph QCD, different pthat bins
//   //*****************************************************************************************
//   fakeRateHistograms.clear();
//   fakeRateFits.clear();
//   fakeRateLegendNames.clear();


//   //madgraph version
//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Eta_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD 100To250");


//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Eta_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD 250To500");


//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Eta_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD 500To1000");


//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorEta","hMuonDenominatorEta", ("TrackerMuonFakeRate_Eta_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Eta_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("TrackerMuon Madgraph QCD 1000ToInf");


//   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);





  //*****************************************************************************************
  //Global Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph version
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphDatasetFiles, madgraphDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_Madgraph_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Eta_Madgraph_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph");


  //pythia version
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaDatasetFiles, pythiaDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_Pythia_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Eta_Pythia_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  //*****************************************************************************************
  //Global Muon Fake Rates For different Sample Composition
  //*****************************************************************************************
  
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //PhotonJets
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJetsDatasetFiles, madgraphPhotonJetsDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_MadgraphPhotonJets_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Eta_MadgraphPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph PhotonJets");

//   //QCD
//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCDDatasetFiles, madgraphQCDDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_MadgraphQCD_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Eta_MadgraphQCD_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD");


  //PhotonJets
  FakeRateEta_pythia = createFakeRateEtaHist(pythiaPhotonJetsDatasetFiles, pythiaPhotonJetsDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_PythiaPhotonJets_" + PhotonTrigger).c_str());
  FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
    FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Eta_PythiaPhotonJets_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRateEta_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia PhotonJets");

//   //QCD
//   FakeRateEta_pythia = createFakeRateEtaHist(pythiaQCDDatasetFiles, pythiaQCDDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_PythiaQCD_" + PhotonTrigger).c_str());
//   FakeRateEta_pythia->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_pythia = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_pythia = fitFakeRateEtaHist(FakeRateEta_pythia);
//     FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Eta_PythiaQCD_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_pythia);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_pythia);
//   fakeRateLegendNames.push_back("GlobalMuon Pythia QCD");


   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Global Muon Fake Rates For PhotonJets, different pthat bins
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //PhotonJets40To100
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJets40To100DatasetFiles, madgraphPhotonJets40To100DatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Eta_MadgraphPhotonJets40To100_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph PhotonJets40To100");

  //PhotonJets100To200
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJets100To200DatasetFiles, madgraphPhotonJets100To200DatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Eta_MadgraphPhotonJets100To200_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph PhotonJets100To200");

  //PhotonJets200ToInf
  FakeRateEta_madgraph = createFakeRateEtaHist(madgraphPhotonJets200ToInfDatasetFiles, madgraphPhotonJets200ToInfDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
                                           "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
  FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
    FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Eta_MadgraphPhotonJets200ToInf_" + PhotonTrigger).c_str());
    fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRateEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph PhotonJets200ToInf");


  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

//   //*****************************************************************************************
//   //Global Muon Fake Rates For Madgraph QCD, different pthat bins
//   //*****************************************************************************************
//   fakeRateHistograms.clear();
//   fakeRateFits.clear();
//   fakeRateLegendNames.clear();


//   //madgraph version
//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD100To250DatasetFiles, madgraphQCD100To250DatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Eta_MadgraphQCD100To250_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD 100To250");


//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD250To500DatasetFiles, madgraphQCD250To500DatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Eta_MadgraphQCD250To500_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD 250To500");


//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD500To1000DatasetFiles, madgraphQCD500To1000DatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Eta_MadgraphQCD500To1000_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD 500To1000");


//   FakeRateEta_madgraph = createFakeRateEtaHist(madgraphQCD1000ToInfDatasetFiles, madgraphQCD1000ToInfDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
//                                            "hMuonNumeratorEta","hMuonDenominatorEta", ("GlobalMuonFakeRate_Eta_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//   FakeRateEta_madgraph->GetYaxis()->SetTitle("Reco Muon Fake Rate");    
//   FakeRateFitFunction_madgraph = NULL;
//   if (fitFakeRate) {
//     FakeRateFitFunction_madgraph = fitFakeRateEtaHist(FakeRateEta_madgraph);
//     FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Eta_MadgraphQCD1000ToInf_" + PhotonTrigger).c_str());
//     fakeRateFits.push_back(FakeRateFitFunction_madgraph);
//   }
//   fakeRateHistograms.push_back(FakeRateEta_madgraph);
//   fakeRateLegendNames.push_back("GlobalMuon Madgraph QCD 1000ToInf");


//   saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);




  file->Close();
  return;

}


//--------------------------------------------------------------------------------------------------
// Do Fake Rate Computation From WJets sample
//--------------------------------------------------------------------------------------------------
void compute2DMuonFakeRatesFromPhotonSample( char *FakeRateFilename ,bool fitFakeRate, string PhotonTrigger) {

  //Samples
  vector<string> madgraphDatasetFiles;
  vector<string> madgraphDatasetNames;

  vector<string> madgraphPhotonJetsDatasetFiles;
  vector<string> madgraphPhotonJetsDatasetNames;
  vector<string> madgraphQCDDatasetFiles;
  vector<string> madgraphQCDDatasetNames;

  vector<string> pythiaDatasetFiles;
  vector<string> pythiaDatasetNames;

  vector<string> pythiaPhotonJetsDatasetFiles;
  vector<string> pythiaPhotonJetsDatasetNames;
  vector<string> pythiaQCDDatasetFiles;
  vector<string> pythiaQCDDatasetNames;


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
  

  pythiaDatasetFiles.clear();
  pythiaDatasetNames.clear();
  pythiaPhotonJetsDatasetFiles.clear();
  pythiaPhotonJetsDatasetNames.clear();
  pythiaQCDDatasetFiles.clear();
  pythiaQCDDatasetNames.clear();
  

  
  //All samples Together
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj40_100-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-pj40_100-id11");
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj100_200-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-pj100_200-id11");
  madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj200_inf-id11_noskim.root");
  madgraphDatasetNames.push_back("s8-pj200_inf-id11");
//   madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd100_250-id11_noskim.root");
//   madgraphDatasetNames.push_back("f8-qcd100_250-id11");
//   madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd250_500-id11_noskim.root");
//   madgraphDatasetNames.push_back("f8-qcd250_500-id11");
//   madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd500_1000-id11_noskim.root");
//   madgraphDatasetNames.push_back("f8-qcd500_1000-id11");
//   madgraphDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_f8-qcd1000_inf-id11_noskim.root");
//   madgraphDatasetNames.push_back("f8-qcd1000_inf-id11");


  //All samples Together
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj15-id12_noskim.root");
  pythiaDatasetNames.push_back("s8-pj15-id12");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj30-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-pj30-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj80-id12_noskim.root");
  pythiaDatasetNames.push_back("s8-pj80-id12");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-pj170-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-pj170-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd15-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-qcd15-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd30-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-qcd30-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd80-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-qcd80-id11");
  pythiaDatasetFiles.push_back("/home/sixie/hist/ComputeMuonFakeRate/filler/009/save_20090601_PhotonTriggerSample/ComputeMuonFakeRate_PhotonTriggerSample_s8-qcd170-id11_noskim.root");
  pythiaDatasetNames.push_back("s8-qcd170-id11");





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
  //IsoTrack Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();


//   //madgraph photon trigger sample
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist(madgraphDatasetFiles, madgraphDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                              "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("IsoTrackMuonFakeRate_PtEta_Madgraph_" + PhotonTrigger).c_str());
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_madgraph = fitFakeRateHist(FakeRatePtEta_madgraph);
    //FakeRateFitFunction_madgraph->SetName("IsoTrackMuonFakeRateFunction_PtEta_Madgraph");
    //fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("IsoTrackMuon Madgraph");


   //   //pythia photon trigger sample
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaDatasetFiles, pythiaDatasetNames, ("ComputeMuonFakeRateMod_" + PhotonTrigger).c_str(),
                                              "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("IsoTrackMuonFakeRate_PtEta_Pythia_" + PhotonTrigger).c_str());
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    //FakeRateFitFunction_pythia = fitFakeRateHist(FakeRatePtEta_pythia);
    //FakeRateFitFunction_pythia->SetName("IsoTrackMuonFakeRateFunction_PtEta_Pythia");
    //fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("IsoTrackMuon Pythia");

   
  printFakeRate(fakeRateHistograms,fakeRateFits,fakeRateLegendNames, ("IsoTrackMuonFakeRatePtEta " + PhotonTrigger).c_str());
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //TrackerMuon Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph photon trigger sample
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist(madgraphDatasetFiles, madgraphDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
                                              "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("TrackerMuonFakeRate_PtEta_Madgraph_" + PhotonTrigger).c_str());
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    // FakeRateFitFunction_madgraph = fitFakeRateHist(FakeRatePtEta_madgraph);
    // FakeRateFitFunction_madgraph->SetName(("TrackerMuonFakeRateFunction_Pt_Madgraph_" + PhotonTrigger).c_str());
    // fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("TrackerMuon Madgraph");


  //pythia photon trigger sample
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaDatasetFiles, pythiaDatasetNames, ("ComputeMuonFakeRateMod_TrackerMuon_" + PhotonTrigger).c_str(),
                                              "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("TrackerMuonFakeRate_PtEta_Pythia_" + PhotonTrigger).c_str());
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    // FakeRateFitFunction_pythia = fitFakeRateHist(FakeRatePtEta_pythia);
    // FakeRateFitFunction_pythia->SetName(("TrackerMuonFakeRateFunction_Pt_Pythia_" + PhotonTrigger).c_str());
    // fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("TrackerMuon Pythia");


  printFakeRate(fakeRateHistograms,fakeRateFits,fakeRateLegendNames, "TrackerMuonFakeRatePtEta " + PhotonTrigger);
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);


  //*****************************************************************************************
  //Global Muon Fake Rates
  //*****************************************************************************************
  fakeRateHistograms.clear();
  fakeRateFits.clear();
  fakeRateLegendNames.clear();

  //madgraph photon trigger sample
  FakeRatePtEta_madgraph = createFakeRatePtEtaHist(madgraphDatasetFiles, madgraphDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
                                              "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("GlobalMuonFakeRate_PtEta_Madgraph_" + PhotonTrigger).c_str());
  FakeRateFitFunction_madgraph = NULL;
  if (fitFakeRate) {
    // FakeRateFitFunction_madgraph = fitFakeRateHist(FakeRatePtEta_madgraph);
    // FakeRateFitFunction_madgraph->SetName(("GlobalMuonFakeRateFunction_Pt_Madgraph_" + PhotonTrigger).c_str());
    // fakeRateFits.push_back(FakeRateFitFunction_madgraph);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_madgraph);
  fakeRateLegendNames.push_back("GlobalMuon Madgraph");


  //pythia photon trigger sample
  FakeRatePtEta_pythia = createFakeRatePtEtaHist(pythiaDatasetFiles, pythiaDatasetNames, ("ComputeMuonFakeRateMod_Global_" + PhotonTrigger).c_str(),
                                              "hMuonNumeratorPtEta","hMuonDenominatorPtEta", ("GlobalMuonFakeRate_PtEta_Pythia_" + PhotonTrigger).c_str());
  FakeRateFitFunction_pythia = NULL;
  if (fitFakeRate) {
    // FakeRateFitFunction_pythia = fitFakeRateHist(FakeRatePtEta_pythia);
    // FakeRateFitFunction_pythia->SetName(("GlobalMuonFakeRateFunction_Pt_Pythia_" + PhotonTrigger).c_str());
    // fakeRateFits.push_back(FakeRateFitFunction_pythia);
  }
  fakeRateHistograms.push_back(FakeRatePtEta_pythia);
  fakeRateLegendNames.push_back("GlobalMuon Pythia");


  printFakeRate(fakeRateHistograms,fakeRateFits,fakeRateLegendNames, "GlobalMuonFakeRatePtEta " + PhotonTrigger);
  saveFakeRate(file, fakeRateHistograms,fakeRateFits, fitFakeRate);

  file->Close();
  return;

}






// //--------------------------------------------------------------------------------------------------
// // Make plots of electron fake rate from many samples
// //--------------------------------------------------------------------------------------------------
void compareMuonFakeRates(char *FakeRateFilename) {

  string plotname;
  string filename;

  TFile *file = new TFile(FakeRateFilename, "READ");
  file->cd();

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);
  TLegend *leg1=0;
  leg1 = new TLegend(0.75,0.6,0.9,0.8);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);

  TH1F *muonFakeRate_Pt_pythiaWJets = 0;
  TH1F *muonFakeRate_Pt_madgraphWJets = 0;
  TH1F *muonFakeRate_Pt_madgraphQCD = 0;
  TH1F *muonFakeRate_Pt_madgraphPhotonJets = 0;
  TH1F *muonFakeRate_Pt_PhotonTrigger = 0;
  TH1F *muonFakeRate_Pt_JetTrigger = 0;
  TH1F *muonFakeRate_Eta_pythiaWJets = 0;
  TH1F *muonFakeRate_Eta_madgraphWJets = 0;
  TH1F *muonFakeRate_Eta_madgraphQCD = 0;
  TH1F *muonFakeRate_Eta_madgraphPhotonJets = 0;
  TH1F *muonFakeRate_Eta_PhotonTrigger = 0;
  TH1F *muonFakeRate_Eta_JetTrigger = 0;

 
  
  //*****************************************************************************************
  //Compare FakeRates For  IsoTrack Muon Fake Rates Vs Pt  
  //*****************************************************************************************

  leg1->Clear();
  leg1->SetX1NDC(0.65);  leg1->SetX2NDC(0.95); leg1->SetY1NDC(0.7);  leg1->SetY2NDC(0.9); 
  leg1->SetX1(0.65);  leg1->SetX2(0.95); leg1->SetY1(0.7);  leg1->SetY2(0.9); 
  plotname = "IsoTrackMuonFakeRatePt";

  muonFakeRate_Pt_pythiaWJets = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Pt_PythiaWJets");
  muonFakeRate_Pt_madgraphWJets = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Pt_MadgraphWJets");
  muonFakeRate_Pt_madgraphQCD = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Pt_MadgraphQCD_Jet50");
  muonFakeRate_Pt_madgraphPhotonJets = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Pt_MadgraphPhotonJets_Photon15");
//  muonFakeRate_Pt_PhotonTrigger = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Pt_Madgraph_Photon15");
//  muonFakeRate_Pt_JetTrigger = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Pt_Madgraph_Jet50");

  assert(muonFakeRate_Pt_pythiaWJets);
  assert(muonFakeRate_Pt_madgraphWJets);
  assert(muonFakeRate_Pt_madgraphQCD);
  assert(muonFakeRate_Pt_madgraphPhotonJets);
//   assert(muonFakeRate_Pt_PhotonTrigger);
//  assert(muonFakeRate_Pt_JetTrigger);

  muonFakeRate_Pt_pythiaWJets->SetLineColor(COLORS[0]);
  muonFakeRate_Pt_pythiaWJets->SetMarkerColor(COLORS[0]);
  muonFakeRate_Pt_madgraphWJets->SetLineColor(COLORS[1]);
  muonFakeRate_Pt_madgraphWJets->SetMarkerColor(COLORS[1]);
  muonFakeRate_Pt_madgraphQCD->SetLineColor(COLORS[2]);
  muonFakeRate_Pt_madgraphQCD->SetMarkerColor(COLORS[2]);
  muonFakeRate_Pt_madgraphPhotonJets->SetLineColor(COLORS[3]);
  muonFakeRate_Pt_madgraphPhotonJets->SetMarkerColor(COLORS[3]);
//   muonFakeRate_Pt_PhotonTrigger->SetLineColor(COLORS[4]);
//   muonFakeRate_Pt_PhotonTrigger->SetMarkerColor(COLORS[4]);
//   muonFakeRate_Pt_JetTrigger->SetLineColor(COLORS[5]);
//   muonFakeRate_Pt_JetTrigger->SetMarkerColor(COLORS[5]);

  leg1->AddEntry(muonFakeRate_Pt_pythiaWJets, "PythiaWJets", "LP");
  leg1->AddEntry(muonFakeRate_Pt_madgraphWJets, "MadgraphWJets", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_madgraphQCD, "MadgraphQCD", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_madgraphPhotonJets, "MadgraphPhotonJets", "LP"); 
//  leg1->AddEntry(muonFakeRate_Pt_PhotonTrigger, "PhotonTrigger", "LP"); 
//  leg1->AddEntry(muonFakeRate_Pt_JetTrigger, "JetTrigger", "LP"); 
 
  muonFakeRate_Pt_madgraphWJets->SetMinimum(0.0);
  muonFakeRate_Pt_madgraphWJets->SetMaximum(0.005);
  muonFakeRate_Pt_madgraphWJets->GetXaxis()->SetRangeUser(0,100);

  muonFakeRate_Pt_madgraphWJets->Draw();
  muonFakeRate_Pt_pythiaWJets->Draw("same");
  muonFakeRate_Pt_madgraphQCD->Draw("same");
  muonFakeRate_Pt_madgraphPhotonJets->Draw("same");
//   muonFakeRate_Pt_PhotonTrigger->Draw("same");
//   muonFakeRate_Pt_JetTrigger->Draw("same");
  leg1->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();



  //*****************************************************************************************
  //Compare FakeRates For  IsoTrack Muon Fake Rates Vs Pt
  //Compare Different W channels
  //*****************************************************************************************


  
  //*****************************************************************************************
  //Compare FakeRates For IsoTrack Muon Fake Rates Vs Eta
  //*****************************************************************************************
  
  leg1->Clear();
  leg1->SetX1NDC(0.50);  leg1->SetX2NDC(0.8); leg1->SetY1NDC(0.7);  leg1->SetY2NDC(0.9); 
  leg1->SetX1(0.50);  leg1->SetX2(0.8); leg1->SetY1(0.7);  leg1->SetY2(0.9); 
  plotname = "IsoMuonFakeRateEta";

  muonFakeRate_Eta_pythiaWJets = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Eta_PythiaWJets");
  muonFakeRate_Eta_madgraphWJets = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Eta_MadgraphWJets");
  muonFakeRate_Eta_madgraphQCD = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Eta_MadgraphQCD_Jet50");
  muonFakeRate_Eta_madgraphPhotonJets = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Eta_MadgraphPhotonJets_Photon15");
//   muonFakeRate_Eta_PhotonTrigger = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Eta_Madgraph_Photon15");
//   muonFakeRate_Eta_JetTrigger = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Eta_Madgraph_Jet50");

  assert(muonFakeRate_Eta_pythiaWJets);
  assert(muonFakeRate_Eta_madgraphWJets);
  assert(muonFakeRate_Eta_madgraphQCD);
  assert(muonFakeRate_Eta_madgraphPhotonJets);
//   assert(muonFakeRate_Eta_PhotonTrigger);
//   assert(muonFakeRate_Eta_JetTrigger);

  muonFakeRate_Eta_pythiaWJets->SetLineColor(COLORS[0]);
  muonFakeRate_Eta_pythiaWJets->SetMarkerColor(COLORS[0]);
  muonFakeRate_Eta_madgraphWJets->SetLineColor(COLORS[1]);
  muonFakeRate_Eta_madgraphWJets->SetMarkerColor(COLORS[1]);
  muonFakeRate_Eta_madgraphQCD->SetLineColor(COLORS[2]);
  muonFakeRate_Eta_madgraphQCD->SetMarkerColor(COLORS[2]);
  muonFakeRate_Eta_madgraphPhotonJets->SetLineColor(COLORS[3]);
  muonFakeRate_Eta_madgraphPhotonJets->SetMarkerColor(COLORS[3]);
//   muonFakeRate_Eta_PhotonTrigger->SetLineColor(COLORS[4]);
//   muonFakeRate_Eta_PhotonTrigger->SetMarkerColor(COLORS[4]);
//   muonFakeRate_Eta_JetTrigger->SetLineColor(COLORS[5]);
//   muonFakeRate_Eta_JetTrigger->SetMarkerColor(COLORS[5]);
 
  leg1->AddEntry(muonFakeRate_Eta_pythiaWJets, "PythiaWJets", "LP");
  leg1->AddEntry(muonFakeRate_Eta_madgraphWJets, "MadgraphWJets", "LP"); 
  leg1->AddEntry(muonFakeRate_Eta_madgraphQCD, "MadgraphQCD", "LP"); 
  leg1->AddEntry(muonFakeRate_Eta_madgraphPhotonJets, "MadgraphPhotonJets", "LP"); 
//   leg1->AddEntry(muonFakeRate_Eta_PhotonTrigger, "PhotonTrigger", "LP"); 
//   leg1->AddEntry(muonFakeRate_Eta_JetTrigger, "JetTrigger", "LP"); 
 
  muonFakeRate_Eta_madgraphWJets->SetMinimum(0.0);
  muonFakeRate_Eta_madgraphWJets->SetMaximum(0.002);

  muonFakeRate_Eta_madgraphWJets->Draw();
  muonFakeRate_Eta_pythiaWJets->Draw("same");
  muonFakeRate_Eta_madgraphQCD->Draw("same");
  muonFakeRate_Eta_madgraphPhotonJets->Draw("same");
//   muonFakeRate_Eta_PhotonTrigger->Draw("same");
//   muonFakeRate_Eta_JetTrigger->Draw("same");

  leg1->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();


  //*****************************************************************************************
  //Compare FakeRates For TrackerMuon Muon Fake Rates Vs Pt
  //*****************************************************************************************

  leg1->Clear();
  leg1->SetX1NDC(0.60);  leg1->SetX2NDC(0.95); leg1->SetY1NDC(0.7);  leg1->SetY2NDC(0.9); 
  leg1->SetX1(0.60);  leg1->SetX2(0.95); leg1->SetY1(0.7);  leg1->SetY2(0.9); 
  plotname = "TrackerMuonFakeRatePt";

  muonFakeRate_Pt_pythiaWJets = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_PythiaWJets");
  muonFakeRate_Pt_madgraphWJets = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_MadgraphWJets");
  muonFakeRate_Pt_madgraphQCD = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_MadgraphQCD_Jet50");
  muonFakeRate_Pt_madgraphPhotonJets = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_MadgraphPhotonJets_Photon15");
//   muonFakeRate_Pt_PhotonTrigger = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_Madgraph_Photon15");
//   muonFakeRate_Pt_JetTrigger = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_Madgraph_Jet50");

  assert(muonFakeRate_Pt_pythiaWJets);
  assert(muonFakeRate_Pt_madgraphWJets);
  assert(muonFakeRate_Pt_madgraphQCD);
  assert(muonFakeRate_Pt_madgraphPhotonJets);
//   assert(muonFakeRate_Pt_PhotonTrigger);
//   assert(muonFakeRate_Pt_JetTrigger);

  muonFakeRate_Pt_pythiaWJets->SetLineColor(COLORS[0]);
  muonFakeRate_Pt_pythiaWJets->SetMarkerColor(COLORS[0]);
  muonFakeRate_Pt_madgraphWJets->SetLineColor(COLORS[1]);
  muonFakeRate_Pt_madgraphWJets->SetMarkerColor(COLORS[1]);
  muonFakeRate_Pt_madgraphQCD->SetLineColor(COLORS[2]);
  muonFakeRate_Pt_madgraphQCD->SetMarkerColor(COLORS[2]);
  muonFakeRate_Pt_madgraphPhotonJets->SetLineColor(COLORS[3]);
  muonFakeRate_Pt_madgraphPhotonJets->SetMarkerColor(COLORS[3]);
//   muonFakeRate_Pt_PhotonTrigger->SetLineColor(COLORS[4]);
//   muonFakeRate_Pt_PhotonTrigger->SetMarkerColor(COLORS[4]);
//   muonFakeRate_Pt_JetTrigger->SetLineColor(COLORS[5]);
//   muonFakeRate_Pt_JetTrigger->SetMarkerColor(COLORS[5]);

  leg1->AddEntry(muonFakeRate_Pt_pythiaWJets, "PythiaWJets", "LP");
  leg1->AddEntry(muonFakeRate_Pt_madgraphWJets, "MadgraphWJets", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_madgraphQCD, "MadgraphQCD", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_madgraphPhotonJets, "MadgraphPhotonJets", "LP"); 
//   leg1->AddEntry(muonFakeRate_Pt_PhotonTrigger, "PhotonTrigger", "LP"); 
//   leg1->AddEntry(muonFakeRate_Pt_JetTrigger, "JetTrigger", "LP"); 
 
  muonFakeRate_Pt_madgraphWJets->SetMinimum(0.0);
  muonFakeRate_Pt_madgraphWJets->SetMaximum(0.5);
  muonFakeRate_Pt_madgraphWJets->GetXaxis()->SetRangeUser(0,100);

  muonFakeRate_Pt_madgraphWJets->Draw();
  muonFakeRate_Pt_pythiaWJets->Draw("same");
  muonFakeRate_Pt_madgraphQCD->Draw("same");
  muonFakeRate_Pt_madgraphPhotonJets->Draw("same");
//   muonFakeRate_Pt_PhotonTrigger->Draw("same");
//   muonFakeRate_Pt_JetTrigger->Draw("same");

  leg1->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();


  //*****************************************************************************************
  //Compare FakeRates For TrackerMuon Fake Rates Vs Pt
  //Compare Different W channels
  //*****************************************************************************************


  //*****************************************************************************************
  //Compare FakeRates For TrackerMuon Fake Rates Vs Eta
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.50);  leg1->SetX2NDC(0.8); leg1->SetY1NDC(0.7);  leg1->SetY2NDC(0.9); 
  leg1->SetX1(0.50);  leg1->SetX2(0.8); leg1->SetY1(0.7);  leg1->SetY2(0.9); 
  plotname = "TrackerMuonFakeRateEta";

  muonFakeRate_Eta_pythiaWJets = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Eta_PythiaWJets");
  muonFakeRate_Eta_madgraphWJets = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Eta_MadgraphWJets");
  muonFakeRate_Eta_madgraphQCD = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Eta_MadgraphQCD_Jet50");
  muonFakeRate_Eta_madgraphPhotonJets = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Eta_MadgraphPhotonJets_Photon15");
//   muonFakeRate_Eta_PhotonTrigger = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Eta_Madgraph_Photon15");
//   muonFakeRate_Eta_JetTrigger = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Eta_Madgraph_Jet50");

  assert(muonFakeRate_Eta_pythiaWJets);
  assert(muonFakeRate_Eta_madgraphWJets);
  assert(muonFakeRate_Eta_madgraphQCD);
  assert(muonFakeRate_Eta_madgraphPhotonJets);
//   assert(muonFakeRate_Eta_PhotonTrigger);
//   assert(muonFakeRate_Eta_JetTrigger);

  muonFakeRate_Eta_pythiaWJets->SetLineColor(COLORS[0]);
  muonFakeRate_Eta_pythiaWJets->SetMarkerColor(COLORS[0]);
  muonFakeRate_Eta_madgraphWJets->SetLineColor(COLORS[1]);
  muonFakeRate_Eta_madgraphWJets->SetMarkerColor(COLORS[1]);
  muonFakeRate_Eta_madgraphQCD->SetLineColor(COLORS[2]);
  muonFakeRate_Eta_madgraphQCD->SetMarkerColor(COLORS[2]);
  muonFakeRate_Eta_madgraphPhotonJets->SetLineColor(COLORS[3]);
  muonFakeRate_Eta_madgraphPhotonJets->SetMarkerColor(COLORS[3]);
//   muonFakeRate_Eta_PhotonTrigger->SetLineColor(COLORS[4]);
//   muonFakeRate_Eta_PhotonTrigger->SetMarkerColor(COLORS[4]);
//   muonFakeRate_Eta_JetTrigger->SetLineColor(COLORS[5]);
//   muonFakeRate_Eta_JetTrigger->SetMarkerColor(COLORS[5]);

  leg1->AddEntry(muonFakeRate_Eta_pythiaWJets, "PythiaWJets", "LP");
  leg1->AddEntry(muonFakeRate_Eta_madgraphWJets, "MadgraphWJets", "LP"); 
  leg1->AddEntry(muonFakeRate_Eta_madgraphQCD, "MadgraphQCD", "LP"); 
  leg1->AddEntry(muonFakeRate_Eta_madgraphPhotonJets, "MadgraphPhotonJets", "LP"); 
//   leg1->AddEntry(muonFakeRate_Eta_PhotonTrigger, "PhotonTrigger", "LP"); 
//   leg1->AddEntry(muonFakeRate_Eta_JetTrigger, "JetTrigger", "LP"); 
 
  muonFakeRate_Eta_madgraphWJets->SetMinimum(0.0);
  muonFakeRate_Eta_madgraphWJets->SetMaximum(0.2);

  muonFakeRate_Eta_madgraphWJets->Draw();
  muonFakeRate_Eta_pythiaWJets->Draw("same");
  muonFakeRate_Eta_madgraphQCD->Draw("same");
  muonFakeRate_Eta_madgraphPhotonJets->Draw("same");
//   muonFakeRate_Eta_PhotonTrigger->Draw("same");
//   muonFakeRate_Eta_JetTrigger->Draw("same");

  leg1->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
  

  //*****************************************************************************************
  //Compare FakeRates For Global Muon Fake Rates Vs Pt
  //*****************************************************************************************

  leg1->Clear();
  leg1->SetX1NDC(0.60);  leg1->SetX2NDC(0.95); leg1->SetY1NDC(0.7);  leg1->SetY2NDC(0.9); 
  leg1->SetX1(0.60);  leg1->SetX2(0.95); leg1->SetY1(0.7);  leg1->SetY2(0.9); 
  plotname = "GlobalMuonFakeRatePt";

  muonFakeRate_Pt_pythiaWJets = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Pt_PythiaWJets");
  muonFakeRate_Pt_madgraphWJets = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Pt_MadgraphWJets");
  muonFakeRate_Pt_madgraphQCD = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Pt_MadgraphQCD_Jet50");
  muonFakeRate_Pt_madgraphPhotonJets = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Pt_MadgraphPhotonJets_Photon15");
//   muonFakeRate_Pt_PhotonTrigger = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Pt_Madgraph_Photon15");
//   muonFakeRate_Pt_JetTrigger = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Pt_Madgraph_Jet50");

  assert(muonFakeRate_Pt_pythiaWJets);
  assert(muonFakeRate_Pt_madgraphWJets);
  assert(muonFakeRate_Pt_madgraphQCD);
  assert(muonFakeRate_Pt_madgraphPhotonJets);
//   assert(muonFakeRate_Pt_PhotonTrigger);
//   assert(muonFakeRate_Pt_JetTrigger);

  muonFakeRate_Pt_pythiaWJets->SetLineColor(COLORS[0]);
  muonFakeRate_Pt_pythiaWJets->SetMarkerColor(COLORS[0]);
  muonFakeRate_Pt_madgraphWJets->SetLineColor(COLORS[1]);
  muonFakeRate_Pt_madgraphWJets->SetMarkerColor(COLORS[1]);
  muonFakeRate_Pt_madgraphQCD->SetLineColor(COLORS[2]);
  muonFakeRate_Pt_madgraphQCD->SetMarkerColor(COLORS[2]);
  muonFakeRate_Pt_madgraphPhotonJets->SetLineColor(COLORS[3]);
  muonFakeRate_Pt_madgraphPhotonJets->SetMarkerColor(COLORS[3]);
//   muonFakeRate_Pt_PhotonTrigger->SetLineColor(COLORS[4]);
//   muonFakeRate_Pt_PhotonTrigger->SetMarkerColor(COLORS[4]);
//   muonFakeRate_Pt_JetTrigger->SetLineColor(COLORS[5]);
//   muonFakeRate_Pt_JetTrigger->SetMarkerColor(COLORS[5]);

  leg1->AddEntry(muonFakeRate_Pt_pythiaWJets, "PythiaWJets", "LP");
  leg1->AddEntry(muonFakeRate_Pt_madgraphWJets, "MadgraphWJets", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_madgraphQCD, "MadgraphQCD", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_madgraphPhotonJets, "MadgraphPhotonJets", "LP"); 
//   leg1->AddEntry(muonFakeRate_Pt_PhotonTrigger, "PhotonTrigger", "LP"); 
//   leg1->AddEntry(muonFakeRate_Pt_JetTrigger, "JetTrigger", "LP"); 
 
  muonFakeRate_Pt_madgraphWJets->SetMinimum(0.0);
  muonFakeRate_Pt_madgraphWJets->SetMaximum(0.5);
  muonFakeRate_Pt_madgraphWJets->GetXaxis()->SetRangeUser(0,100);

  muonFakeRate_Pt_madgraphWJets->Draw();
  muonFakeRate_Pt_pythiaWJets->Draw("same");
  muonFakeRate_Pt_madgraphQCD->Draw("same");
  muonFakeRate_Pt_madgraphPhotonJets->Draw("same");
//   muonFakeRate_Pt_PhotonTrigger->Draw("same");
//   muonFakeRate_Pt_JetTrigger->Draw("same");

  leg1->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();


  //*****************************************************************************************
  //Compare FakeRates For Global Muon Fake Rates Vs Pt
  //Compare Different W channels
  //*****************************************************************************************


  //*****************************************************************************************
  //Compare FakeRates For Global Muon Fake Rates Vs Eta
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.50);  leg1->SetX2NDC(0.8); leg1->SetY1NDC(0.7);  leg1->SetY2NDC(0.9); 
  leg1->SetX1(0.50);  leg1->SetX2(0.8); leg1->SetY1(0.7);  leg1->SetY2(0.9); 
  plotname = "GlobalMuonFakeRateEta";

  muonFakeRate_Eta_pythiaWJets = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Eta_PythiaWJets");
  muonFakeRate_Eta_madgraphWJets = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Eta_MadgraphWJets");
  muonFakeRate_Eta_madgraphQCD = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Eta_MadgraphQCD_Jet50");
  muonFakeRate_Eta_madgraphPhotonJets = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Eta_MadgraphPhotonJets_Photon15");
//   muonFakeRate_Eta_PhotonTrigger = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Eta_Madgraph_Photon15");
//   muonFakeRate_Eta_JetTrigger = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Eta_Madgraph_Jet50");

  assert(muonFakeRate_Eta_pythiaWJets);
  assert(muonFakeRate_Eta_madgraphWJets);
  assert(muonFakeRate_Eta_madgraphQCD);
  assert(muonFakeRate_Eta_madgraphPhotonJets);
//   assert(muonFakeRate_Eta_PhotonTrigger);
//   assert(muonFakeRate_Eta_JetTrigger);

  muonFakeRate_Eta_pythiaWJets->SetLineColor(COLORS[0]);
  muonFakeRate_Eta_pythiaWJets->SetMarkerColor(COLORS[0]);
  muonFakeRate_Eta_madgraphWJets->SetLineColor(COLORS[1]);
  muonFakeRate_Eta_madgraphWJets->SetMarkerColor(COLORS[1]);
  muonFakeRate_Eta_madgraphQCD->SetLineColor(COLORS[2]);
  muonFakeRate_Eta_madgraphQCD->SetMarkerColor(COLORS[2]);
  muonFakeRate_Eta_madgraphPhotonJets->SetLineColor(COLORS[3]);
  muonFakeRate_Eta_madgraphPhotonJets->SetMarkerColor(COLORS[3]);
//   muonFakeRate_Eta_PhotonTrigger->SetLineColor(COLORS[4]);
//   muonFakeRate_Eta_PhotonTrigger->SetMarkerColor(COLORS[4]);
//   muonFakeRate_Eta_JetTrigger->SetLineColor(COLORS[5]);
//   muonFakeRate_Eta_JetTrigger->SetMarkerColor(COLORS[5]);

  leg1->AddEntry(muonFakeRate_Eta_pythiaWJets, "PythiaWJets", "LP");
  leg1->AddEntry(muonFakeRate_Eta_madgraphWJets, "MadgraphWJets", "LP"); 
  leg1->AddEntry(muonFakeRate_Eta_madgraphQCD, "MadgraphQCD", "LP"); 
  leg1->AddEntry(muonFakeRate_Eta_madgraphPhotonJets, "MadgraphPhotonJets", "LP"); 
//   leg1->AddEntry(muonFakeRate_Eta_PhotonTrigger, "PhotonTrigger", "LP"); 
//   leg1->AddEntry(muonFakeRate_Eta_JetTrigger, "JetTrigger", "LP"); 
 
  muonFakeRate_Eta_madgraphWJets->SetMinimum(0.0);
  muonFakeRate_Eta_madgraphWJets->SetMaximum(0.2);

  muonFakeRate_Eta_madgraphWJets->Draw();
  muonFakeRate_Eta_pythiaWJets->Draw("same");
  muonFakeRate_Eta_madgraphQCD->Draw("same");
  muonFakeRate_Eta_madgraphPhotonJets->Draw("same");
//   muonFakeRate_Eta_PhotonTrigger->Draw("same");
//   muonFakeRate_Eta_JetTrigger->Draw("same");

  leg1->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
  

   TH1F *muonFakeRate_Pt_madgraphQCD_Jet30 = 0;
   TH1F *muonFakeRate_Pt_madgraphQCD_Jet50 = 0;
   TH1F *muonFakeRate_Pt_madgraphQCD_Jet80 = 0;
   TH1F *muonFakeRate_Pt_madgraphQCD_Jet110 = 0;

  //*****************************************************************************************
  //Compare IsoTrack FakeRates For QCD Sample For Different Jet Triggers
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.70);  leg1->SetX2NDC(0.9); leg1->SetY1NDC(0.6);  leg1->SetY2NDC(0.9); 
  leg1->SetX1(0.70);  leg1->SetX2(0.9); leg1->SetY1(0.6);  leg1->SetY2(0.9); 
  plotname = "IsoTrackMuonFakeRate_QCD_JetTriggers";

  muonFakeRate_Pt_madgraphQCD_Jet30 = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Pt_MadgraphQCD_Jet30");
  muonFakeRate_Pt_madgraphQCD_Jet50 = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Pt_MadgraphQCD_Jet50");
  muonFakeRate_Pt_madgraphQCD_Jet80 = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Pt_MadgraphQCD_Jet80");
  muonFakeRate_Pt_madgraphQCD_Jet110 = (TH1F*)file->FindObjectAny("IsoTrackMuonFakeRate_Pt_MadgraphQCD_Jet110");

  assert(muonFakeRate_Pt_madgraphQCD_Jet30);
  assert(muonFakeRate_Pt_madgraphQCD_Jet50);
  assert(muonFakeRate_Pt_madgraphQCD_Jet80);
  assert(muonFakeRate_Pt_madgraphQCD_Jet110);

  muonFakeRate_Pt_madgraphQCD_Jet30->SetLineColor(COLORS[0]);
  muonFakeRate_Pt_madgraphQCD_Jet30->SetMarkerColor(COLORS[0]);
  muonFakeRate_Pt_madgraphQCD_Jet50->SetLineColor(COLORS[1]);
  muonFakeRate_Pt_madgraphQCD_Jet50->SetMarkerColor(COLORS[1]);
  muonFakeRate_Pt_madgraphQCD_Jet80->SetLineColor(COLORS[2]);
  muonFakeRate_Pt_madgraphQCD_Jet80->SetMarkerColor(COLORS[2]);
  muonFakeRate_Pt_madgraphQCD_Jet110->SetLineColor(COLORS[3]);
  muonFakeRate_Pt_madgraphQCD_Jet110->SetMarkerColor(COLORS[3]);

  muonFakeRate_Pt_madgraphQCD_Jet30->SetMinimum(0.0);
  muonFakeRate_Pt_madgraphQCD_Jet50->SetMaximum(0.005);
  muonFakeRate_Pt_madgraphQCD_Jet50->GetXaxis()->SetRangeUser(0,100);
    
  leg1->AddEntry(muonFakeRate_Pt_madgraphQCD_Jet30, "Jet30", "LP");
  leg1->AddEntry(muonFakeRate_Pt_madgraphQCD_Jet50, "Jet50", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_madgraphQCD_Jet80, "Jet80", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_madgraphQCD_Jet110, "Jet110", "LP"); 

  muonFakeRate_Pt_madgraphQCD_Jet30->Draw();
  muonFakeRate_Pt_madgraphQCD_Jet50->Draw("same");
  muonFakeRate_Pt_madgraphQCD_Jet80->Draw("same");
  muonFakeRate_Pt_madgraphQCD_Jet110->Draw("same");
  
  leg1->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();


  //*****************************************************************************************
  //Compare TrackerMuon FakeRates For QCD Sample For Different Jet Triggers
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.70);  leg1->SetX2NDC(0.9); leg1->SetY1NDC(0.6);  leg1->SetY2NDC(0.8); 
  plotname = "TrackerMuonFakeRate_QCD_JetTriggers";

  muonFakeRate_Pt_madgraphQCD_Jet30 = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_MadgraphQCD_Jet30");
  muonFakeRate_Pt_madgraphQCD_Jet50 = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_MadgraphQCD_Jet50");
  muonFakeRate_Pt_madgraphQCD_Jet80 = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_MadgraphQCD_Jet80");
  muonFakeRate_Pt_madgraphQCD_Jet110 = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_MadgraphQCD_Jet110");

  assert(muonFakeRate_Pt_madgraphQCD_Jet30);
  assert(muonFakeRate_Pt_madgraphQCD_Jet50);
  assert(muonFakeRate_Pt_madgraphQCD_Jet80);
  assert(muonFakeRate_Pt_madgraphQCD_Jet110);

  muonFakeRate_Pt_madgraphQCD_Jet30->SetLineColor(COLORS[0]);
  muonFakeRate_Pt_madgraphQCD_Jet30->SetMarkerColor(COLORS[0]);
  muonFakeRate_Pt_madgraphQCD_Jet50->SetLineColor(COLORS[1]);
  muonFakeRate_Pt_madgraphQCD_Jet50->SetMarkerColor(COLORS[1]);
  muonFakeRate_Pt_madgraphQCD_Jet80->SetLineColor(COLORS[2]);
  muonFakeRate_Pt_madgraphQCD_Jet80->SetMarkerColor(COLORS[2]);
  muonFakeRate_Pt_madgraphQCD_Jet110->SetLineColor(COLORS[3]);
  muonFakeRate_Pt_madgraphQCD_Jet110->SetMarkerColor(COLORS[3]);

  muonFakeRate_Pt_madgraphQCD_Jet30->SetMinimum(0.0);
  muonFakeRate_Pt_madgraphQCD_Jet50->SetMaximum(0.1);
  muonFakeRate_Pt_madgraphQCD_Jet50->GetXaxis()->SetRangeUser(0,100);
    
  leg1->AddEntry(muonFakeRate_Pt_madgraphQCD_Jet30, "Jet30", "LP");
  leg1->AddEntry(muonFakeRate_Pt_madgraphQCD_Jet50, "Jet50", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_madgraphQCD_Jet80, "Jet80", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_madgraphQCD_Jet110, "Jet110", "LP"); 

  muonFakeRate_Pt_madgraphQCD_Jet30->Draw();
  muonFakeRate_Pt_madgraphQCD_Jet50->Draw("same");
  muonFakeRate_Pt_madgraphQCD_Jet80->Draw("same");
  muonFakeRate_Pt_madgraphQCD_Jet110->Draw("same");
  
  leg1->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();


  //*****************************************************************************************
  //Compare Global FakeRates For QCD Sample For Different Jet Triggers
  //*****************************************************************************************
  leg1->Clear();
  leg1->SetX1NDC(0.70);  leg1->SetX2NDC(0.9); leg1->SetY1NDC(0.6);  leg1->SetY2NDC(0.8); 
  plotname = "GlobalMuonFakeRate_QCD_JetTriggers";

  muonFakeRate_Pt_madgraphQCD_Jet30 = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Pt_MadgraphQCD_Jet30");
  muonFakeRate_Pt_madgraphQCD_Jet50 = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Pt_MadgraphQCD_Jet50");
  muonFakeRate_Pt_madgraphQCD_Jet80 = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Pt_MadgraphQCD_Jet80");
  muonFakeRate_Pt_madgraphQCD_Jet110 = (TH1F*)file->FindObjectAny("GlobalMuonFakeRate_Pt_MadgraphQCD_Jet110");

  assert(muonFakeRate_Pt_madgraphQCD_Jet30);
  assert(muonFakeRate_Pt_madgraphQCD_Jet50);
  assert(muonFakeRate_Pt_madgraphQCD_Jet80);
  assert(muonFakeRate_Pt_madgraphQCD_Jet110);

  muonFakeRate_Pt_madgraphQCD_Jet30->SetLineColor(COLORS[0]);
  muonFakeRate_Pt_madgraphQCD_Jet30->SetMarkerColor(COLORS[0]);
  muonFakeRate_Pt_madgraphQCD_Jet50->SetLineColor(COLORS[1]);
  muonFakeRate_Pt_madgraphQCD_Jet50->SetMarkerColor(COLORS[1]);
  muonFakeRate_Pt_madgraphQCD_Jet80->SetLineColor(COLORS[2]);
  muonFakeRate_Pt_madgraphQCD_Jet80->SetMarkerColor(COLORS[2]);
  muonFakeRate_Pt_madgraphQCD_Jet110->SetLineColor(COLORS[3]);
  muonFakeRate_Pt_madgraphQCD_Jet110->SetMarkerColor(COLORS[3]);

  muonFakeRate_Pt_madgraphQCD_Jet30->SetMinimum(0.0);
  muonFakeRate_Pt_madgraphQCD_Jet50->SetMaximum(0.1);
  muonFakeRate_Pt_madgraphQCD_Jet50->GetXaxis()->SetRangeUser(0,100);
    
  leg1->AddEntry(muonFakeRate_Pt_madgraphQCD_Jet30, "Jet30", "LP");
  leg1->AddEntry(muonFakeRate_Pt_madgraphQCD_Jet50, "Jet50", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_madgraphQCD_Jet80, "Jet80", "LP"); 
  leg1->AddEntry(muonFakeRate_Pt_madgraphQCD_Jet110, "Jet110", "LP"); 

  muonFakeRate_Pt_madgraphQCD_Jet30->Draw();
  muonFakeRate_Pt_madgraphQCD_Jet50->Draw("same");
  muonFakeRate_Pt_madgraphQCD_Jet80->Draw("same");
  muonFakeRate_Pt_madgraphQCD_Jet110->Draw("same");
  
  leg1->Draw();
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

}




//--------------------------------------------------------------------------------------------------
// Do Fake Rate Computation From WJets sample
//--------------------------------------------------------------------------------------------------
void compare2DMuonFakeRates( char *FakeRateFilename ) {


  //Create root file to store fake rates
  TFile *file = new TFile(FakeRateFilename, "UPDATE");
  file->cd();
  

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




  //*****************************************************************************************
  //Compare GsfSC FakeRate with GlobalMuon FakeRate
  //*****************************************************************************************
   TH2F *GsfSCMuonFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny("IsoTrackMuonFakeRate_PtEta_MadgraphWJets");
   assert(GsfSCMuonFakeRate_PtEta_MadgraphWJets);

   TH2F *GlobalMuonFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny("GlobalMuonFakeRate_PtEta_MadgraphWJets");
   assert(GlobalMuonFakeRate_PtEta_MadgraphWJets);

   makePtSliceComparisonPlots(GsfSCMuonFakeRate_PtEta_MadgraphWJets, GlobalMuonFakeRate_PtEta_MadgraphWJets, "IsoTrackMuonFakeRate_PtEta_PythiaWJets_Comparison_PtSlices", "GsfSC", "Reco");
   makeEtaSliceComparisonPlots(GsfSCMuonFakeRate_PtEta_MadgraphWJets, GlobalMuonFakeRate_PtEta_MadgraphWJets, "IsoTrackMuonFakeRate_PtEta_PythiaWJets_Comparison_EtaSlices", "GsfSC", "Reco");


  //*****************************************************************************************
  //Compare Pythia Vs Madgraph FakeRates For GsfSC
  //*****************************************************************************************
//   TH2F *GsfSCMuonFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny("IsoTrackMuonFakeRate_PtEta_MadgraphWJets");
//   assert(GsfSCMuonFakeRate_PtEta_MadgraphWJets);

//   TH2F *GsfSCMuonFakeRate_PtEta_PythiaWJets = (TH2F*)file->FindObjectAny("IsoTrackMuonFakeRate_PtEta_PythiaWJets");
//   assert(GsfSCMuonFakeRate_PtEta_PythiaWJets);

//   makePtSliceComparisonPlots(GsfSCMuonFakeRate_PtEta_MadgraphWJets, GsfSCMuonFakeRate_PtEta_PythiaWJets, "IsoTrackMuonFakeRate_PtEta_WJets_PythiaVsMadgraph_PtSlices", "Madgraph", "Pythia");
//   makeEtaSliceComparisonPlots(GsfSCMuonFakeRate_PtEta_MadgraphWJets, GsfSCMuonFakeRate_PtEta_PythiaWJets, "IsoTrackMuonFakeRate_PtEta_WJets_PythiaVsMadgraph_EtaSlices", "Madgraph", "Pythia");


//   TH2F *GsfSCMuonFakeRate_PtEta_MadgraphQCD = (TH2F*)file->FindObjectAny("IsoTrackMuonFakeRate_PtEta_MadgraphQCD_Jet50");
//   assert(GsfSCMuonFakeRate_PtEta_MadgraphQCD);

//   TH2F *GsfSCMuonFakeRate_PtEta_PythiaQCDEnriched = (TH2F*)file->FindObjectAny("IsoTrackMuonFakeRate_PtEta_PythiaQCDEnriched_Jet50");
//   assert(GsfSCMuonFakeRate_PtEta_PythiaQCDEnriched);

//   makePtSliceComparisonPlots(GsfSCMuonFakeRate_PtEta_MadgraphQCD, GsfSCMuonFakeRate_PtEta_PythiaQCDEnriched, "IsoTrackMuonFakeRate_PtEta_QCD_PythiaVsMadgraph_PtSlices", "Madgraph", "Pythia QCD Enriched");
//   makeEtaSliceComparisonPlots(GsfSCMuonFakeRate_PtEta_MadgraphQCD, GsfSCMuonFakeRate_PtEta_PythiaQCDEnriched, "IsoTrackMuonFakeRate_PtEta_QCD_PythiaVsMadgraph_EtaSlices", "Madgraph", "Pythia QCD Enriched");

  //*****************************************************************************************
  //Compare Pythia Vs Madgraph FakeRates For Reco
  //*****************************************************************************************
//   TH2F *GlobalMuonFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny("GlobalMuonFakeRate_PtEta_MadgraphWJets");
//   assert(GlobalMuonFakeRate_PtEta_MadgraphWJets);

//   TH2F *GlobalMuonFakeRate_PtEta_PythiaWJets = (TH2F*)file->FindObjectAny("GlobalMuonFakeRate_PtEta_PythiaWJets");
//   assert(GlobalMuonFakeRate_PtEta_PythiaWJets);

//   makePtSliceComparisonPlots(GlobalMuonFakeRate_PtEta_MadgraphWJets, GlobalMuonFakeRate_PtEta_PythiaWJets, "GlobalMuonFakeRate_PtEta_WJets_PythiaVsMadgraph_PtSlices", "Madgraph", "Pythia");
//   makeEtaSliceComparisonPlots(GlobalMuonFakeRate_PtEta_MadgraphWJets, GlobalMuonFakeRate_PtEta_PythiaWJets, "GlobalMuonFakeRate_PtEta_WJets_PythiaVsMadgraph_EtaSlices", "Madgraph", "Pythia");


//   TH2F *GlobalMuonFakeRate_PtEta_MadgraphQCD = (TH2F*)file->FindObjectAny("GlobalMuonFakeRate_PtEta_MadgraphQCD_Jet50");
//   assert(GlobalMuonFakeRate_PtEta_MadgraphQCD);

//   TH2F *GlobalMuonFakeRate_PtEta_PythiaQCDEnriched = (TH2F*)file->FindObjectAny("GlobalMuonFakeRate_PtEta_PythiaQCDEnriched_Jet50");
//   assert(GlobalMuonFakeRate_PtEta_PythiaQCDEnriched);

//   makePtSliceComparisonPlots(GlobalMuonFakeRate_PtEta_MadgraphQCD, GlobalMuonFakeRate_PtEta_PythiaQCDEnriched, "GlobalMuonFakeRate_PtEta_QCD_PythiaVsMadgraph_PtSlices", "Madgraph", "Pythia QCD Enriched");
//   makeEtaSliceComparisonPlots(GlobalMuonFakeRate_PtEta_MadgraphQCD, GlobalMuonFakeRate_PtEta_PythiaQCDEnriched, "GlobalMuonFakeRate_PtEta_QCD_PythiaVsMadgraph_EtaSlices", "Madgraph", "Pythia QCD Enriched");


  //*****************************************************************************************
  //Compare WJets Vs QCD FakeRates For GsfSC 
  //*****************************************************************************************
//   TH2F *GsfSCMuonFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny("IsoTrackMuonFakeRate_PtEta_MadgraphWJets");
//   assert(GsfSCMuonFakeRate_PtEta_MadgraphWJets);

//   TH2F *GsfSCMuonFakeRate_PtEta_MadgraphQCD = (TH2F*)file->FindObjectAny("IsoTrackMuonFakeRate_PtEta_MadgraphQCD_Jet50");
//   assert(GsfSCMuonFakeRate_PtEta_MadgraphQCD);

//   makePtSliceComparisonPlots(GsfSCMuonFakeRate_PtEta_MadgraphWJets, GsfSCMuonFakeRate_PtEta_MadgraphQCD, "IsoTrackMuonFakeRate_PtEta_Madgraph_WJetsVsQCD_PtSlices", "WJets", "QCD");
//   makeEtaSliceComparisonPlots(GsfSCMuonFakeRate_PtEta_MadgraphWJets, GsfSCMuonFakeRate_PtEta_MadgraphQCD, "IsoTrackMuonFakeRate_PtEta_Madgraph_WJetsVsQCD_EtaSlices", "WJets", "QCD");


//   TH2F *GsfSCMuonFakeRate_PtEta_PythiaWJets = (TH2F*)file->FindObjectAny("IsoTrackMuonFakeRate_PtEta_PythiaWJets");
//   assert(GsfSCMuonFakeRate_PtEta_PythiaWJets);

//   TH2F *GsfSCMuonFakeRate_PtEta_PythiaQCDEnriched = (TH2F*)file->FindObjectAny("IsoTrackMuonFakeRate_PtEta_PythiaQCDEnriched_Jet50");
//   assert(GsfSCMuonFakeRate_PtEta_PythiaQCDEnriched);

//   makePtSliceComparisonPlots(GsfSCMuonFakeRate_PtEta_MadgraphQCD, GsfSCMuonFakeRate_PtEta_PythiaQCDEnriched, "IsoTrackMuonFakeRate_PtEta_Pythia_WJetsVsQCD_PtSlices", "WJets", "QCD Enriched");
//   makeEtaSliceComparisonPlots(GsfSCMuonFakeRate_PtEta_MadgraphQCD, GsfSCMuonFakeRate_PtEta_PythiaQCDEnriched, "IsoTrackMuonFakeRate_PtEta_Pythia_WJetsVsQCD_EtaSlices", "WJets", "QCD Enriched");



  //*****************************************************************************************
  //Compare WJets Vs QCD FakeRates For Reco 
  //*****************************************************************************************
//   TH2F *GlobalMuonFakeRate_PtEta_MadgraphWJets = (TH2F*)file->FindObjectAny("GlobalMuonFakeRate_PtEta_MadgraphWJets");
//   assert(GlobalMuonFakeRate_PtEta_MadgraphWJets);

//   TH2F *GlobalMuonFakeRate_PtEta_MadgraphQCD = (TH2F*)file->FindObjectAny("GlobalMuonFakeRate_PtEta_MadgraphQCD_Jet50");
//   assert(GlobalMuonFakeRate_PtEta_MadgraphQCD);

//   makePtSliceComparisonPlots(GlobalMuonFakeRate_PtEta_MadgraphWJets, GlobalMuonFakeRate_PtEta_MadgraphQCD, "GlobalMuonFakeRate_PtEta_Madgraph_WJetsVsQCD_PtSlices", "WJets", "QCD");
//   makeEtaSliceComparisonPlots(GlobalMuonFakeRate_PtEta_MadgraphWJets, GlobalMuonFakeRate_PtEta_MadgraphQCD, "GlobalMuonFakeRate_PtEta_Madgraph_WJetsVsQCD_EtaSlices", "WJets", "QCD");


//   TH2F *GlobalMuonFakeRate_PtEta_PythiaWJets = (TH2F*)file->FindObjectAny("GlobalMuonFakeRate_PtEta_PythiaWJets");
//   assert(GlobalMuonFakeRate_PtEta_PythiaWJets);

//   TH2F *GlobalMuonFakeRate_PtEta_PythiaQCDEnriched = (TH2F*)file->FindObjectAny("GlobalMuonFakeRate_PtEta_PythiaQCDEnriched_Jet50");
//   assert(GlobalMuonFakeRate_PtEta_PythiaQCDEnriched);

//   makePtSliceComparisonPlots(GlobalMuonFakeRate_PtEta_MadgraphQCD, GlobalMuonFakeRate_PtEta_PythiaQCDEnriched, "GlobalMuonFakeRate_PtEta_Pythia_WJetsVsQCD_PtSlices", "WJets", "QCD Enriched");
//   makeEtaSliceComparisonPlots(GlobalMuonFakeRate_PtEta_MadgraphQCD, GlobalMuonFakeRate_PtEta_PythiaQCDEnriched, "GlobalMuonFakeRate_PtEta_Pythia_WJetsVsQCD_EtaSlices", "WJets", "QCD Enriched");




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
  TH1F *muonFakeRate_Pt_WJets = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_MadgraphWJets");
  assert(muonFakeRate_Pt_WJets);

  TH1F *muonFakeRate_Pt_JetTrigger = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_Madgraph_Jet50");
  assert(muonFakeRate_Pt_JetTrigger);

  TH1F *muonFakeRate_Pt_PhotonTrigger = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Pt_Madgraph_Photon15");
  assert(muonFakeRate_Pt_PhotonTrigger);

  TH1F *muonFakeRate_Pt_SystematicError = (TH1F*)(muonFakeRate_Pt_WJets->Clone());
  muonFakeRate_Pt_SystematicError->SetName("TrackerMuonFakeRate_Pt_Madgraph_SysErrors");

  for (int b=0;b<=muonFakeRate_Pt_WJets->GetXaxis()->GetNbins();b++) {
    Double_t maxDifference = fabs(muonFakeRate_Pt_JetTrigger->GetBinContent(b) - 
                                  muonFakeRate_Pt_PhotonTrigger->GetBinContent(b));
    if (fabs(muonFakeRate_Pt_WJets->GetBinContent(b) - 
             muonFakeRate_Pt_JetTrigger->GetBinContent(b))
        > maxDifference) {
      maxDifference = fabs(muonFakeRate_Pt_WJets->GetBinContent(b) - 
                           muonFakeRate_Pt_JetTrigger->GetBinContent(b));
    }
    if (fabs(muonFakeRate_Pt_WJets->GetBinContent(b) - 
             muonFakeRate_Pt_PhotonTrigger->GetBinContent(b))
        > maxDifference) {
      maxDifference = fabs(muonFakeRate_Pt_WJets->GetBinContent(b) - 
                           muonFakeRate_Pt_PhotonTrigger->GetBinContent(b));
    }
    
    Double_t totalError = TMath::Sqrt(muonFakeRate_Pt_JetTrigger->GetBinError(b)*
                                      muonFakeRate_Pt_JetTrigger->GetBinError(b) + 
                                      (maxDifference/2.0)*(maxDifference/2.0));
    
    muonFakeRate_Pt_SystematicError->SetBinContent(b,totalError);
  }
  
  muonFakeRate_Pt_SystematicError->SetDirectory(file);
  file->WriteTObject(muonFakeRate_Pt_SystematicError, muonFakeRate_Pt_SystematicError->GetName(), "WriteDelete");


  //*****************************************************************************************
  //Compute systematic errors for 1D Fake Rates vs Eta for Reco denominator 
  //*****************************************************************************************
  TH1F *muonFakeRate_Eta_WJets = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Eta_MadgraphWJets");
  assert(muonFakeRate_Eta_WJets);

  TH1F *muonFakeRate_Eta_JetTrigger = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Eta_Madgraph_Jet50");
  assert(muonFakeRate_Eta_JetTrigger);

  TH1F *muonFakeRate_Eta_PhotonTrigger = (TH1F*)file->FindObjectAny("TrackerMuonFakeRate_Eta_Madgraph_Photon15");
  assert(muonFakeRate_Eta_PhotonTrigger);

  TH1F *muonFakeRate_Eta_SystematicError = (TH1F*)(muonFakeRate_Eta_WJets->Clone());
  muonFakeRate_Eta_SystematicError->SetName("TrackerMuonFakeRate_Eta_Madgraph_SysErrors");

  for (int b=0;b<=muonFakeRate_Eta_WJets->GetXaxis()->GetNbins();b++) {
    Double_t maxDifference = fabs(muonFakeRate_Eta_JetTrigger->GetBinContent(b) - 
                                  muonFakeRate_Eta_PhotonTrigger->GetBinContent(b));
    if (fabs(muonFakeRate_Eta_WJets->GetBinContent(b) - 
             muonFakeRate_Eta_JetTrigger->GetBinContent(b))
        > maxDifference) {
      maxDifference = fabs(muonFakeRate_Eta_WJets->GetBinContent(b) - 
                           muonFakeRate_Eta_JetTrigger->GetBinContent(b));
    }
    if (fabs(muonFakeRate_Eta_WJets->GetBinContent(b) - 
             muonFakeRate_Eta_PhotonTrigger->GetBinContent(b))
        > maxDifference) {
      maxDifference = fabs(muonFakeRate_Eta_WJets->GetBinContent(b) - 
                           muonFakeRate_Eta_PhotonTrigger->GetBinContent(b));
    }
    
    Double_t totalError = TMath::Sqrt(muonFakeRate_Eta_JetTrigger->GetBinError(b)*
                                      muonFakeRate_Eta_JetTrigger->GetBinError(b) + 
                                      (maxDifference/2.0)*(maxDifference/2.0));
    
    muonFakeRate_Eta_SystematicError->SetBinContent(b,totalError);
  }
  
  muonFakeRate_Eta_SystematicError->SetDirectory(file);
  file->WriteTObject(muonFakeRate_Eta_SystematicError, muonFakeRate_Eta_SystematicError->GetName(), "WriteDelete" );


  //*****************************************************************************************
  //Compute systematic errors for 2D FakeRates for Reco denominator 
  //*****************************************************************************************

  TH2F *muonFakeRate_PtEta_WJets = (TH2F*)file->FindObjectAny("TrackerMuonFakeRate_PtEta_MadgraphWJets");
  assert(muonFakeRate_PtEta_WJets);

  TH2F *muonFakeRate_PtEta_JetTrigger = (TH2F*)file->FindObjectAny("TrackerMuonFakeRate_PtEta_Madgraph_Jet50");
  assert(muonFakeRate_PtEta_JetTrigger);

  TH2F *muonFakeRate_PtEta_PhotonTrigger = (TH2F*)file->FindObjectAny("TrackerMuonFakeRate_PtEta_Madgraph_Photon15");
  assert(muonFakeRate_PtEta_PhotonTrigger);

  TH2F *muonFakeRate_PtEta_SystematicError = (TH2F*)(muonFakeRate_PtEta_WJets->Clone());
  muonFakeRate_PtEta_SystematicError->SetName("TrackerMuonFakeRate_PtEta_Madgraph_SysErrors");

  for (int b=0;b<=muonFakeRate_PtEta_WJets->GetXaxis()->GetNbins();b++) {
    for (int c=0;c<=muonFakeRate_PtEta_WJets->GetYaxis()->GetNbins();c++) {
      Double_t maxDifference = fabs(muonFakeRate_PtEta_JetTrigger->GetBinContent(b,c) - 
                                  muonFakeRate_PtEta_PhotonTrigger->GetBinContent(b,c));
      if (fabs(muonFakeRate_PtEta_WJets->GetBinContent(b,c) - 
                                  muonFakeRate_PtEta_JetTrigger->GetBinContent(b,c))
          > maxDifference) {
        maxDifference = fabs(muonFakeRate_PtEta_WJets->GetBinContent(b,c) - 
                             muonFakeRate_PtEta_JetTrigger->GetBinContent(b,c));
      }
      if (fabs(muonFakeRate_PtEta_WJets->GetBinContent(b,c) - 
                                  muonFakeRate_PtEta_PhotonTrigger->GetBinContent(b,c))
          > maxDifference) {
        maxDifference = fabs(muonFakeRate_PtEta_WJets->GetBinContent(b,c) - 
                             muonFakeRate_PtEta_PhotonTrigger->GetBinContent(b,c));
      }
       
      Double_t totalError = TMath::Sqrt(muonFakeRate_PtEta_JetTrigger->GetBinError(b,c)*
                                        muonFakeRate_PtEta_JetTrigger->GetBinError(b,c) + 
                                        (maxDifference/2.0)*(maxDifference/2.0));

      muonFakeRate_PtEta_SystematicError->SetBinContent(b,c,maxDifference/2);
    }
  }

  muonFakeRate_PtEta_SystematicError->SetDirectory(file);
  file->WriteTObject(muonFakeRate_PtEta_SystematicError, muonFakeRate_PtEta_SystematicError->GetName(), "WriteDelete");


  file->Close();
  return;

}


//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void computeMuonFakeRates(char *FakeRateFilename , bool fitFakeRate = false ) {



  //*****************************************************************************************
  //Muon Fake Rate Computation
  //*****************************************************************************************  
  //From WJets
   compute1DMuonFakeRatesFromWJets(FakeRateFilename, fitFakeRate);
   compute2DMuonFakeRatesFromWJets(FakeRateFilename, fitFakeRate);

  //From QCD
   compute1DMuonFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet30");
   compute1DMuonFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet50");
   compute1DMuonFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet80");
   compute1DMuonFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet110");
    compute2DMuonFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet30");
    compute2DMuonFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet50");
    compute2DMuonFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet80");
    compute2DMuonFakeRatesFromQCD(FakeRateFilename, fitFakeRate, "Jet110");
  
  //From PhotonTriggerSample
  compute1DMuonFakeRatesFromPhotonSample(FakeRateFilename, fitFakeRate, "Photon15");
  //compute1DMuonFakeRatesFromPhotonSample(FakeRateFilename, fitFakeRate, "Photon25");
  compute2DMuonFakeRatesFromPhotonSample(FakeRateFilename, fitFakeRate, "Photon15");
  //compute2DMuonFakeRatesFromPhotonSample(FakeRateFilename, fitFakeRate, "Photon25");
  
  computeSystematicErrors(FakeRateFilename);

  //make2DDistribution(FakeRateFilename);
  //compareMuonFakeRates(FakeRateFilename);
  //compare2DMuonFakeRates(FakeRateFilename);



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


