//root -l $CMSSW_BASE/src/MitPhysics/macros/makeWJetsFakeValidationPlots.C+


#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <string>
#include <TFile.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TF1.h>
#include <TF2.h>
#include <TH1F.h>
#include <TH2F.h>
#include <MitStyle.h>
#include "MitAna/Utils/interface/SimpleTable.h"
#include "MitPhysics/Utils/interface/GeneratorTools.h"
#include <vector>
#include <utility>
#include <map>

using namespace std;
//using namespace mithep;
#endif

#include <vector>
Double_t LUMINOSITY = 200.0;

//cross section table
mithep::SimpleTable xstab("$CMSSW_BASE/src/MitPhysics/data/xs.dat");  
mithep::GeneratorTools genTools;

//define colors
Int_t COLORS[10] = {kRed,kBlue,kMagenta,kGreen,kCyan,kBlack,8,9,10,11};
Int_t MARKERS[10] = {20,21,22,23,24,25,26,27,28,30};

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
  for (UInt_t i=0;i<=hist->GetXaxis()->GetNbins()+1;i++) {
    for (UInt_t j=0;j<=hist->GetYaxis()->GetNbins()+1;j++) {
      

      Double_t x = hist->GetXaxis()->GetBinCenter(i);
      Double_t y = hist->GetYaxis()->GetBinCenter(j);
      UInt_t xbin = 0;
      UInt_t ybin = 0;
      //cout << x << " " << y << " ";

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
                    string dirName, string histName, string skimName) {

  assert(datasetFiles.size() > 0);
  TH1F *tmp = getHisto(datasetFiles[0], dirName, histName);
  assert(tmp);
  TH1F *finalHist = (TH1F*)tmp->Clone();
  finalHist->Sumw2();

  for (UInt_t i=0; i < datasetFiles.size() ;i++) {
    double CrossSection = xstab.Get(datasetNames[i].c_str());
    TH1F *tmpNEventsHist = getHisto(datasetFiles[i],"AnaFwkMod", "hDEvents");
    double NEvents = tmpNEventsHist->Integral();

    //for skims we handle this in a special way
    if (skimName != "noskim") {
      if (datasetNames[i] == "s8-qcdbc_20_30-id9")
        NEvents = 2218990;
      if (datasetNames[i] == "s8-qcdbc_30_80-id9")
        NEvents = 1972649;
      if (datasetNames[i] == "s8-qcdbc_80_170-id9")
        NEvents = 778039;
      if (datasetNames[i] == "s8-qcdem_20_30-id9")
        NEvents = 5166176;
      if (datasetNames[i] == "s8-qcdem_30_80-id9")
        NEvents = 11665529;
      if (datasetNames[i] == "s8-qcdem_80_170-id9")
        NEvents = 5551833;
    }

    double weight = CrossSection * LUMINOSITY / NEvents;
    cout << datasetNames[i].c_str() << " : " << CrossSection << " " << NEvents << " " << weight << endl;

    if (i==0) {
      //do our own scaling
      for (int b=0;b<=finalHist->GetNbinsX()+1;b++) {
        finalHist->SetBinContent(b,finalHist->GetBinContent(b)*weight);
        finalHist->SetBinError(b,finalHist->GetBinError(b)*weight);
      }
    } else {
      TH1F *tmpHist = getHisto(datasetFiles[i], dirName, histName);
      for (int b=0;b<=finalHist->GetNbinsX()+1;b++) {
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

void makeEtaSliceComparisonPlots( TH2F* hist1, TH2F* hist2 , string plotname) {

  for(UInt_t j=0;j <= hist1->GetYaxis()->GetNbins()+1;j++) {
    char tmp[20]; 
    sprintf(tmp, "Eta%.1f",  hist1->GetYaxis()->GetBinLowEdge(j));
    string sliceEta = tmp;
    string title = plotname+"_"+sliceEta;
    string filename = title + ".gif";

    TH1D *slice1 = hist1->ProjectionX("projectionX1" , j,j);
    TH1D *slice2 = hist2->ProjectionX("projectionX2" , j,j);
 
    TCanvas *cv = MakeCanvas("cv", title.c_str(), 800, 600);
    TLegend *leg1=0;
    leg1 = new TLegend(0.15,0.2,0.35,0.3);   
    leg1->SetBorderSize(1);
    leg1->SetTextSize(0.025);
    leg1->AddEntry(slice1, hist1->GetName(), "LP"); 
    leg1->AddEntry(slice2, hist2->GetName(), "LP"); 
    
    slice1->SetMarkerColor(COLORS[0]);
    slice2->SetMarkerColor(COLORS[1]);
    
    slice1->Draw("same");
    slice2->Draw("same");
    leg1->Draw();
    cv->SaveAs(filename.c_str());
  }
  return;
}

void makePtSliceComparisonPlots( TH2F* hist1, TH2F* hist2 , string plotname) {

  for(UInt_t i=0;i <= hist1->GetXaxis()->GetNbins()+1;i++) {
    char tmp[20]; 
    sprintf(tmp, "Pt%.1f",  hist1->GetXaxis()->GetBinLowEdge(i));
    string slicePt = tmp;
    string title = plotname+"_"+slicePt;
    string filename = title + ".gif";

    TH1D *slice1 =0;
    TH1D *slice2 =0;
    
    slice1 = hist1->ProjectionY("projectionY1" , i,i);
    slice2 = hist2->ProjectionY("projectionY2" , i,i);
 
    TCanvas *cv = MakeCanvas("cv", title.c_str(), 800, 600);
    TLegend *leg1=0;
    leg1 = new TLegend(0.25,0.8,0.55,0.9);   
    leg1->SetBorderSize(1);
    leg1->SetTextSize(0.025);
    leg1->AddEntry(slice1, hist1->GetName(), "LP"); 
    leg1->AddEntry(slice2, hist2->GetName(), "LP"); 
    
    slice1->SetMarkerColor(COLORS[0]);
    slice2->SetMarkerColor(COLORS[1]);
    
    slice1->Draw("same");
    slice2->Draw("same");
    leg1->Draw();
    cv->SaveAs(filename.c_str());
  }
  return;
}


void print2DHist( TH2F* fakeRateHistogram, string fakeRateLegendName) {
  double total = 0;
  double nonzeroBins = 0;

  cout.precision(3);
  cout << endl << fakeRateLegendName << endl;  

  cout.setf(ios_base::left);
  cout.width(8);
  cout << "Axis" << " ";
  for (UInt_t k=0; k<=fakeRateHistogram->GetYaxis()->GetNbins()+1 ; ++k) { 
      cout.setf(ios_base::left);
      cout.width(8);
      cout << fakeRateHistogram->GetYaxis()->GetBinLowEdge(k) << " ";          
    }
  cout << endl;

  for (UInt_t j=0; j<=fakeRateHistogram->GetXaxis()->GetNbins()+1 ; ++j) {    
    cout.setf(ios_base::left);
    cout.width(8);
    cout << fakeRateHistogram->GetXaxis()->GetBinLowEdge(j) << " ";
    for (UInt_t k=0; k<=fakeRateHistogram->GetYaxis()->GetNbins()+1 ; ++k) { 
      cout.setf(ios_base::left);
      cout.width(8);
      cout << fakeRateHistogram->GetBinContent(j,k) << " ";          
      total += fakeRateHistogram->GetBinContent(j,k);
      if (fakeRateHistogram->GetBinContent(j,k) > 0) ++nonzeroBins;
    }
    cout << endl;
  }
  cout << "Total: " << total << endl;
  cout << "Average NonZero Bin: " << total/nonzeroBins << endl;
  cout << endl;
}


//--------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------
void makeDistributionComparisonPlot( vector<string> datasetfiles, vector<string> datasetnames, 
                                     vector<string> histNames, vector<string> legendNames, bool normalizeArea,
                                     int nbins, string plotname ) {
  assert(histNames.size() == legendNames.size());

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);
  TLegend *leg = new TLegend(0.25,0.80 - 0.025*histNames.size(),0.45,0.80);   
  leg->SetBorderSize(1);
  leg->SetTextSize(0.03);
  vector <TH1F*> hists;
  Double_t MAXY = 0.0;
  Int_t MaxIndex = -1;
  Double_t Normalization = 0;

  for(UInt_t i=0;i<histNames.size();i++) {

    TH1F *hist = addAllSamples(datasetfiles, datasetnames, "FakeElectronAnalysisMod", 
                               histNames[i], "singlelepton" );

    hist->SetMarkerColor(COLORS[i]);
    hist->SetMarkerSize(1.0);
    leg->AddEntry(hist, legendNames[i].c_str(), "LP"); 
    if (nbins > 0) {
      hist->Rebin(hist->GetXaxis()->GetNbins() / nbins);
    }

    hists.push_back(hist);
    if (hist->GetMaximum() > MAXY) {
      MAXY = hist->GetMaximum();
      MaxIndex = i;
      Normalization = hist->Integral();
    }
    
  }
    
  if (normalizeArea) {
    for(UInt_t i=0;i<hists.size();i++) {
      if (hists[i]->Integral() > 0) hists[i]->Scale(Normalization/hists[i]->Integral());      
    }
  
    MAXY = 0.0;
    for(UInt_t i=0;i<hists.size();i++) {
      if (hists[i]->GetMaximum() > MAXY) {
        MAXY = hists[i]->GetMaximum();
      }
    }
  }

  for(UInt_t i=0;i<hists.size();i++) {
    //do plots here
    if (i==0) {
      hists[i]->SetMaximum(MAXY*1.1);
      hists[i]->Draw();      
    } else {
      hists[i]->Draw("same");        
    }


  }
  leg->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

}

//--------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------
void makeCrossDatasetComparisonPlot( vector<string> dataset1files, vector<string> dataset1names, string dataset1label,
                                     vector<string> dataset2files, vector<string> dataset2names, string dataset2label,
                                     vector<string> histNames, vector<string> legendNames, 
                                     string plotname ) {

  assert(histNames.size() == legendNames.size());

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);
  TLegend *leg = new TLegend(0.65,0.6,0.9,0.8);   
  leg->SetBorderSize(1);
  leg->SetTextSize(0.03);
  
  for(UInt_t i=0;i<histNames.size();i++) {

    TH1F *hist1 = addAllSamples(dataset1files, dataset1names, "WJetsFakeValidationMod_PythiaWJets", 
                               histNames[i], "noskim" );
    TH1F *hist2 = addAllSamples(dataset2files, dataset2names, "WJetsFakeValidationMod_PythiaWJets",
                               histNames[i], "noskim" );    

    hist1->SetMarkerColor(COLORS[i*2]);
    hist1->SetMarkerSize(1.0);
    hist2->SetMarkerColor(COLORS[i*2+1]);
    hist2->SetMarkerSize(1.0);
    leg->AddEntry(hist1, (dataset1label+legendNames[i]).c_str(), "LP"); 
    leg->AddEntry(hist2, (dataset2label+legendNames[i]).c_str(), "LP"); 

    //do plots here
    if (i==0)
      hist1->Draw("same");
    else 
      hist1->Draw("same");  

    hist1->Draw("same");
    hist2->Draw("same");
  }
  leg->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

}


//--------------------------------------------------------------------------------------------------
void makeCrossDirComparisonPlot( vector<string> datasetfiles, vector<string> datasetnames, vector<string> dirnames,
                                 vector<string> dirnamelabel, vector<string> histNames, vector<string> legendNames, 
                                 vector<double> bins, string plotname ) {

  assert(histNames.size() == legendNames.size());
  assert(datasetfiles.size() == datasetnames.size());
  assert(dirnames.size() == dirnamelabel.size());

  vector <TH1F*> hists;
  Double_t MAXY = 0.0;

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);
  TLegend *leg = new TLegend(0.55,0.6,0.85,0.75);   
  leg->SetBorderSize(1);
  leg->SetTextSize(0.03);

  int colorindex = 0;
  for(UInt_t i=0;i<histNames.size();++i) {
    for(UInt_t d=0;d<dirnames.size();++d) {
      
      TH1F *tmphist = addAllSamples(datasetfiles, datasetnames, dirnames[d], 
                                 histNames[i], "noskim" );
      
      TH1F *hist = tmphist;
      if (bins.size() > 0) hist = rebin(tmphist,bins);

      hist->SetMarkerColor(COLORS[colorindex]);
      hist->SetMarkerStyle(MARKERS[colorindex]);
      //hist->SetLineColor(COLORS[colorindex]);
      hist->SetMarkerSize(1.0);
      leg->AddEntry(hist, (legendNames[i]+"_"+dirnamelabel[d]).c_str(), "LP"); 
      
      hists.push_back(hist);
      if (hist->GetMaximum() > MAXY) MAXY = hist->GetMaximum();

      cout << hist->GetName() << " " << hist->GetXaxis()->GetNbins() << " " << hist->GetBinContent(5) << endl;
      ++colorindex;

    }
  }

  for (UInt_t i=0; i < hists.size();++i) {
    hists[i]->SetMaximum(1.2*MAXY);
    hists[i]->SetMinimum(0.0);
    if (i==0) 
      hists[i]->Draw("");
    else
      hists[i]->Draw("same");
  } 

  leg->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

}

void makeWJetsFakeElectronPlots() {

  vector<string> madgraphWJetsDatasetFiles;
  vector<string> madgraphWJetsDatasetNames;
  vector<string> madgraphWLightJetsDatasetFiles;
  vector<string> madgraphWLightJetsDatasetNames;
  vector<string> madgraphWcDatasetFiles;
  vector<string> madgraphWcDatasetNames;
  vector<string> madgraphVQQDatasetFiles;
  vector<string> madgraphVQQDatasetNames;
  vector<string> pythiaWJetsDatasetFiles;
  vector<string> pythiaWJetsDatasetNames;

  vector<string> pythiaWJetsComputeFakeRatesDatasetFiles;
  vector<string> pythiaWJetsComputeFakeRatesDatasetNames;

  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************
  madgraphWJetsDatasetFiles.clear(); 
  madgraphWJetsDatasetNames.clear();
  madgraphWcDatasetFiles.clear(); 
  madgraphWcDatasetNames.clear();
  madgraphVQQDatasetFiles.clear(); 
  madgraphVQQDatasetNames.clear();
  pythiaWJetsDatasetFiles.clear();
  pythiaWJetsDatasetNames.clear();
  pythiaWJetsComputeFakeRatesDatasetFiles.clear();
  pythiaWJetsComputeFakeRatesDatasetNames.clear();

  //Madgraph WJets
  madgraphWJetsDatasetFiles.push_back("/home/sixie/hist/WJetsFakeValidationStudy_All/filler/008/WJetsFakeValidationStudy_All_s8-wjets-mg-id11_noskim.root");
  madgraphWJetsDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphWJetsDatasetFiles.push_back("/home/sixie/hist/WJetsFakeValidationStudy_All/filler/008/WJetsFakeValidationStudy_All_s8-vqq-mg-id11_noskim.root");
  madgraphWJetsDatasetNames.push_back("s8-vqq-mg-id11");

  //Madgraph samples separated
  madgraphWLightJetsDatasetFiles.push_back("/home/sixie/hist/WJetsFakeValidationStudy_All/filler/008/WJetsFakeValidationStudy_All_s8-wjets-mg-id11_noskim.root");
  madgraphWLightJetsDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphWcDatasetFiles.push_back("/home/sixie/hist/WJetsFakeValidationStudy_All/filler/008/WJetsFakeValidationStudy_All_f8-wc-mg-id11_noskim.root");
  madgraphWcDatasetNames.push_back("f8-wc-mg-id11");
  madgraphVQQDatasetFiles.push_back("/home/sixie/hist/WJetsFakeValidationStudy_All/filler/008/WJetsFakeValidationStudy_All_s8-vqq-mg-id11_noskim.root");
  madgraphVQQDatasetNames.push_back("s8-vqq-mg-id11");
  
  //Pythia WJets
  pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/WJetsFakeValidationStudy_All/filler/008/WJetsFakeValidationStudy_All_s8-we-id11_noskim.root");
  pythiaWJetsDatasetNames.push_back("s8-we-id11");
  pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/WJetsFakeValidationStudy_All/filler/008/WJetsFakeValidationStudy_All_s8-wm-id11_noskim.root");
  pythiaWJetsDatasetNames.push_back("s8-wm-id11");
  pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/WJetsFakeValidationStudy_All/filler/008/WJetsFakeValidationStudy_All_s8-wt-id11_noskim.root");
  pythiaWJetsDatasetNames.push_back("s8-wt-id11");

  vector<string> DatasetFiles;
  vector<string> DatasetNames;
  //DatasetFiles = QCDEnrichedWithConversionFilterDatasetFiles;
  //DatasetNames = QCDEnrichedWithConversionFilterDatasetNames;
//  DatasetFiles = madgraphWJetsDatasetFiles;
//  DatasetNames = madgraphWJetsDatasetNames;
  DatasetFiles = pythiaWJetsDatasetFiles;
  DatasetNames = pythiaWJetsDatasetNames;



  pythiaWJetsComputeFakeRatesDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-we-id11_noskim.root");
  pythiaWJetsComputeFakeRatesDatasetNames.push_back("s8-we-id11");
  pythiaWJetsComputeFakeRatesDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wm-id11_noskim.root");
  pythiaWJetsComputeFakeRatesDatasetNames.push_back("s8-wm-id11");
  pythiaWJetsComputeFakeRatesDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wt-id11_noskim.root");
  pythiaWJetsComputeFakeRatesDatasetNames.push_back("s8-wt-id11");



  //*****************************************************************************************
  //eta and pt bins
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
  vector<double> phibins;
  phibins.push_back(-3.5);
  phibins.push_back(-2.5);
  phibins.push_back(-1.5);
  phibins.push_back(-0.5);
  phibins.push_back(0.5);
  phibins.push_back(1.5);
  phibins.push_back(2.5);
  phibins.push_back(3.5);


  //*****************************************************************************************
  //make plots for gen particle matching
  //*****************************************************************************************  
  vector<string> dirnames;
  vector<string> histnames;
  vector<string> histlabels;
  vector<string> dirlegendlabel;
  vector<string> legendnames;


  //*****************************************************************************************
  //Compare Pt, Eta for pythia WJets and madgraph  from MC Simulation
  //*****************************************************************************************  

//   histnames.clear();
//   legendnames.clear();
//   histnames.push_back("hLepton1Pt");
//   legendnames.push_back("");
//   makeCrossDatasetComparisonPlot(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, "Pythia WJets",
//                                  madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "Madgraph WJets",
//                                  histnames, legendnames, "FakeElectronPt_FromMC_PythiaVsMadgraph");


//   histnames.clear();
//   legendnames.clear();
//   histnames.push_back("hFakeElectronPt");
//   legendnames.push_back("");
//   makeCrossDatasetComparisonPlot(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, "Pythia WJets",
//                                  madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, "Madgraph WJets",
//                                  histnames, legendnames, "FakeElectronPt_FromMC_PythiaVsMadgraph");


  //*****************************************************************************************
  //Charge, Pt, Eta, Phi For Fake Electrons : Pythia WJets
  //*****************************************************************************************  

//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_PythiaWJets");
//   dirlegendlabel.push_back("GsfSC Pythia WJets");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_PythiaWJets");
//   dirlegendlabel.push_back("Reco Pythia WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MadgraphWJets");
//   dirlegendlabel.push_back("GsfSC Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
// // //   dirnames.push_back("WJetsFakeValidationMod_True");
// // //   dirlegendlabel.push_back("Use MC True Fake Rate");
// //  dirnames.push_back("WJetsFakeValidationMod_PythiaQCDEnriched");
// //  dirlegendlabel.push_back("QCD Fake Rate");
//   histnames.push_back("hDileptonCharge_weighted");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames,fakebins,"DileptonChargeComparison_Pythia");


//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod");
//   dirlegendlabel.push_back("GsfSC Prediction");
//   dirnames.push_back("WJetsFakeValidationMod_MadgraphWJets");
//   dirlegendlabel.push_back("GsfSC Prediction Madgraph");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph");
//   dirnames.push_back("WJetsFakeValidationMod_Reco");
//   dirlegendlabel.push_back("Reco Prediction");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("MC");
// //   dirnames.push_back("WJetsFakeValidationMod_True");
// //   dirlegendlabel.push_back("Use MC True Fake Rate");
//   dirnames.push_back("WJetsFakeValidationMod_PythiaQCDEnriched");
//   dirlegendlabel.push_back("QCD Fake Rate");
//   histnames.push_back("hFakeElectronPt_weighted");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, dirnames,dirlegendlabel, histnames, legendnames, ptbins, "FakeElectronPtComparison_Pythia");


//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod");
//   dirlegendlabel.push_back("GsfSC Prediction");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("MC");
//   dirnames.push_back("WJetsFakeValidationMod_Reco");
//   dirlegendlabel.push_back("Reco Prediction");
// //   dirnames.push_back("WJetsFakeValidationMod_True");
// //   dirlegendlabel.push_back("Use MC True Fake Rate");
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("QCD Fake Rate");
//   histnames.push_back("hFakeElectronEta_weighted");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, dirnames,dirlegendlabel, histnames, legendnames, etabins, "FakeElectronEtaComparison_Pythia");

//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod");
//   dirlegendlabel.push_back("GsfSC Prediction");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("MC");
//    dirnames.push_back("WJetsFakeValidationMod_Reco");
//    dirlegendlabel.push_back("Reco Prediction");
// //   dirnames.push_back("WJetsFakeValidationMod_True");
// //   dirlegendlabel.push_back("Use MC True Fake Rate");
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("QCD Fake Rate");
//   histnames.push_back("hFakeElectronPhi_weighted");
//   legendnames.push_back("");
//  makeCrossDirComparisonPlot(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, dirnames,dirlegendlabel, histnames, legendnames, phibins, "FakeElectronPhiComparison_Pythia");


//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod");
//   dirlegendlabel.push_back("GsfSC Prediction");
//   dirnames.push_back("WJetsFakeValidationMod_Reco");
//   dirlegendlabel.push_back("Reco");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("MC");
//   histnames.push_back("hFakeElectronCategory");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames,fakebins,"FakeCategory_Pythia");

//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod");
//   dirlegendlabel.push_back("GsfSC Prediction");
//   dirnames.push_back("WJetsFakeValidationMod_Reco");
//   dirlegendlabel.push_back("Reco");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("MC");
//   histnames.push_back("hFakeElectronCategoryPlusMinus");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames,fakebins,"FakeCategoryPlusMinus_Pythia");

  //*****************************************************************************************
  //Charge, Pt, Eta, Phi For Fake Electrons in +- bin : Pythia WJets
  //*****************************************************************************************  


//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod");
//   dirlegendlabel.push_back("GsfSC Prediction");
//   dirnames.push_back("WJetsFakeValidationMod_Reco");
//   dirlegendlabel.push_back("Reco Prediction");
//   dirnames.push_back("WJetsFakeValidationMod_MadgraphWJets");
//   dirlegendlabel.push_back("GsfSC Prediction Madgraph");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph");
// //   dirnames.push_back("WJetsFakeValidationMod_True");
// //   dirlegendlabel.push_back("Use MC True Fake Rate");
//   dirnames.push_back("WJetsFakeValidationMod_PythiaQCDEnriched");
//   dirlegendlabel.push_back("QCD Fake Rate");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("MC");
//   histnames.push_back("hFakeElectronPt_weighted_PlusMinus");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, dirnames,dirlegendlabel, histnames, legendnames, ptbins, "FakeElectronPtComparison_Pythia_PlusMinus");


//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod");
//   dirlegendlabel.push_back("GsfSC Prediction");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("MC");
//   dirnames.push_back("WJetsFakeValidationMod_Reco");
//   dirlegendlabel.push_back("Reco Prediction");
// //   dirnames.push_back("WJetsFakeValidationMod_True");
// //   dirlegendlabel.push_back("Use MC True Fake Rate");
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("QCD Fake Rate");
//   histnames.push_back("hFakeElectronEta_weighted");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, dirnames,dirlegendlabel, histnames, legendnames, etabins, "FakeElectronEtaComparison_Pythia_PlusMinus");

//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod");
//   dirlegendlabel.push_back("GsfSC Prediction");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("MC");
//    dirnames.push_back("WJetsFakeValidationMod_Reco");
//    dirlegendlabel.push_back("Reco Prediction");
// //   dirnames.push_back("WJetsFakeValidationMod_True");
// //   dirlegendlabel.push_back("Use MC True Fake Rate");
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("QCD Fake Rate");
//   histnames.push_back("hFakeElectronPhi_weighted");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(pythiaWJetsDatasetFiles, pythiaWJetsDatasetNames, dirnames,dirlegendlabel, histnames, legendnames, phibins, "FakeElectronPhiComparison_Pythia_PlusMinus");



  //*****************************************************************************************
  //Charge, Pt, Eta, Phi For Fake Electrons : Madgraph WJets
  //*****************************************************************************************  

//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaWJets");
// //   dirlegendlabel.push_back("GsfSC Pythia WJets");
// //   dirnames.push_back("WJetsFakeValidationMod_Reco_PythiaWJets");
// //   dirlegendlabel.push_back("Reco Pythia WJets");
// //   dirnames.push_back("WJetsFakeValidationMod_MadgraphWJets");
// //   dirlegendlabel.push_back("GsfSC Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("GsfSC Prediction QCD");
// //   dirnames.push_back("WJetsFakeValidationMod_MadgraphQCD");
// //   dirlegendlabel.push_back("GsfSC Prediction QCD");
// //   dirnames.push_back("WJetsFakeValidationMod_Reco_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("Reco Prediction QCD");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hDileptonCharge_weighted");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames,fakebins,"DileptonChargeComparison_Madgraph");
   

//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaWJets");
// //   dirlegendlabel.push_back("GsfSC Pythia WJets");
// //   dirnames.push_back("WJetsFakeValidationMod_Reco_PythiaWJets");
// //   dirlegendlabel.push_back("Reco Pythia WJets");
// //   dirnames.push_back("WJetsFakeValidationMod_MadgraphWJets");
// //   dirlegendlabel.push_back("GsfSC Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("GsfSC Prediction QCD");
//   dirnames.push_back("WJetsFakeValidationMod_MadgraphQCD");
//   dirlegendlabel.push_back("GsfSC Prediction QCD");
// //   dirnames.push_back("WJetsFakeValidationMod_Reco_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("Reco Prediction QCD");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hFakeElectronPt_weighted");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames,dirlegendlabel, histnames, legendnames, ptbins, "FakeElectronPtComparison_Madgraph");


//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaWJets");
// //   dirlegendlabel.push_back("GsfSC Pythia WJets");
// //   dirnames.push_back("WJetsFakeValidationMod_Reco_PythiaWJets");
// //   dirlegendlabel.push_back("Reco Pythia WJets");
// //   dirnames.push_back("WJetsFakeValidationMod_MadgraphWJets");
// //   dirlegendlabel.push_back("GsfSC Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("GsfSC Prediction QCD");
// //   dirnames.push_back("WJetsFakeValidationMod_MadgraphQCD");
// //   dirlegendlabel.push_back("GsfSC Prediction QCD");
// //   dirnames.push_back("WJetsFakeValidationMod_Reco_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("Reco Prediction QCD");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hFakeElectronEta_weighted");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames,dirlegendlabel, histnames, legendnames, etabins, "FakeElectronEtaComparison_Madgraph");



//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaWJets");
// //   dirlegendlabel.push_back("GsfSC Pythia WJets");
// //   dirnames.push_back("WJetsFakeValidationMod_Reco_PythiaWJets");
// //   dirlegendlabel.push_back("Reco Pythia WJets");
// //   dirnames.push_back("WJetsFakeValidationMod_MadgraphWJets");
// //   dirlegendlabel.push_back("GsfSC Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("GsfSC Prediction QCD");
// //   dirnames.push_back("WJetsFakeValidationMod_MadgraphQCD");
// //   dirlegendlabel.push_back("GsfSC Prediction QCD");
// //   dirnames.push_back("WJetsFakeValidationMod_Reco_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("Reco Prediction QCD");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hFakeElectronPhi_weighted");
//   legendnames.push_back("");
//  makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames,dirlegendlabel, histnames, legendnames, phibins, "FakeElectronPhiComparison_Madgraph");



//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaWJets");
// //   dirlegendlabel.push_back("GsfSC Pythia WJets");
// //   dirnames.push_back("WJetsFakeValidationMod_Reco_PythiaWJets");
// //   dirlegendlabel.push_back("Reco Pythia WJets");
// //   dirnames.push_back("WJetsFakeValidationMod_MadgraphWJets");
// //   dirlegendlabel.push_back("GsfSC Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("GsfSC Prediction QCD");
// //   dirnames.push_back("WJetsFakeValidationMod_MadgraphQCD");
// //   dirlegendlabel.push_back("GsfSC Prediction QCD");
// //   dirnames.push_back("WJetsFakeValidationMod_Reco_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("Reco Prediction QCD");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hFakeElectronCategory");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames,fakebins,"FakeCategory_Madgraph");

  //*****************************************************************************************
  //Charge, Pt, Eta, Phi For Fake Electrons in +- bin : Madgraph WJets
  //*****************************************************************************************  


//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
// //   dirnames.push_back("WJetsFakeValidationMod");
// //   dirlegendlabel.push_back("GsfSC Prediction");
// //   dirnames.push_back("WJetsFakeValidationMod_Reco");
// //   dirlegendlabel.push_back("Reco");
//   dirnames.push_back("WJetsFakeValidationMod_MadgraphWJets");
//   dirlegendlabel.push_back("GsfSC Prediction Madgraph");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("MC");
// //   dirnames.push_back("WJetsFakeValidationMod_True");
// //   dirlegendlabel.push_back("Use MC True Fake Rate");
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("QCD Fake Rate");
//   histnames.push_back("hFakeElectronPt_weighted_PlusMinus");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames,dirlegendlabel, histnames, legendnames, ptbins, "FakeElectronPtComparison_Madgraph_PlusMinus");


//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
// //   dirnames.push_back("WJetsFakeValidationMod");
// //   dirlegendlabel.push_back("GsfSC Prediction");
// //   dirnames.push_back("WJetsFakeValidationMod_Reco");
// //   dirlegendlabel.push_back("Reco");
//   dirnames.push_back("WJetsFakeValidationMod_MadgraphWJets");
//   dirlegendlabel.push_back("GsfSC Prediction Madgraph");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("MC");
// //   dirnames.push_back("WJetsFakeValidationMod_True");
// //   dirlegendlabel.push_back("Use MC True Fake Rate");
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("QCD Fake Rate");
//   histnames.push_back("hFakeElectronEta_weighted");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames,dirlegendlabel, histnames, legendnames, etabins, "FakeElectronEtaComparison_Madgraph_PlusMinus");

//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
// //   dirnames.push_back("WJetsFakeValidationMod");
// //   dirlegendlabel.push_back("GsfSC Prediction");
// //   dirnames.push_back("WJetsFakeValidationMod_Reco");
// //   dirlegendlabel.push_back("Reco");
//   dirnames.push_back("WJetsFakeValidationMod_MadgraphWJets");
//   dirlegendlabel.push_back("GsfSC Prediction Madgraph");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("MC");
// //   dirnames.push_back("WJetsFakeValidationMod_True");
// //   dirlegendlabel.push_back("Use MC True Fake Rate");
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("QCD Fake Rate");
//   histnames.push_back("hFakeElectronPhi_weighted");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames,dirlegendlabel, histnames, legendnames, phibins, "FakeElectronPhiComparison_Madgraph_PlusMinus");

//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
// //   dirnames.push_back("WJetsFakeValidationMod");
// //   dirlegendlabel.push_back("GsfSC Prediction");
// //   dirnames.push_back("WJetsFakeValidationMod_Reco");
// //   dirlegendlabel.push_back("Reco");
//   dirnames.push_back("WJetsFakeValidationMod_MadgraphWJets");
//   dirlegendlabel.push_back("GsfSC Prediction Madgraph");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("MC");
// //   dirnames.push_back("WJetsFakeValidationMod_True");
// //   dirlegendlabel.push_back("Use MC True Fake Rate");
// //   dirnames.push_back("WJetsFakeValidationMod_PythiaQCDEnriched");
// //   dirlegendlabel.push_back("QCD Fake Rate");
//   histnames.push_back("hFakeElectronCategoryPlusMinus");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames,fakebins,"FakeCategoryPlusMinus_Madgraph");

}


void makeWJetsAnalysisPlots() {

  vector<string> madgraphWJetsDatasetFiles;
  vector<string> madgraphWJetsDatasetNames;
  vector<string> madgraphWLightJetsDatasetFiles;
  vector<string> madgraphWLightJetsDatasetNames;
  vector<string> madgraphWcDatasetFiles;
  vector<string> madgraphWcDatasetNames;
  vector<string> madgraphVQQDatasetFiles;
  vector<string> madgraphVQQDatasetNames;
  vector<string> pythiaWJetsDatasetFiles;
  vector<string> pythiaWJetsDatasetNames;

  vector<string> pythiaWJetsComputeFakeRatesDatasetFiles;
  vector<string> pythiaWJetsComputeFakeRatesDatasetNames;

  //*****************************************************************************************
  //Load input histogram files
  //*****************************************************************************************
  madgraphWJetsDatasetFiles.clear(); 
  madgraphWJetsDatasetNames.clear();
  madgraphWcDatasetFiles.clear(); 
  madgraphWcDatasetNames.clear();
  madgraphVQQDatasetFiles.clear(); 
  madgraphVQQDatasetNames.clear();
  pythiaWJetsDatasetFiles.clear();
  pythiaWJetsDatasetNames.clear();
  pythiaWJetsComputeFakeRatesDatasetFiles.clear();
  pythiaWJetsComputeFakeRatesDatasetNames.clear();

  //Madgraph WJets
  madgraphWJetsDatasetFiles.push_back("/home/sixie/hist/WJetsFakeValidationStudy_All/filler/008/WJetsFakeValidationStudy_All_s8-wjets-mg-id11_noskim.root");
  madgraphWJetsDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphWJetsDatasetFiles.push_back("/home/sixie/hist/WJetsFakeValidationStudy_All/filler/008/WJetsFakeValidationStudy_All_s8-vqq-mg-id11_noskim.root");
  madgraphWJetsDatasetNames.push_back("s8-vqq-mg-id11");

  //Madgraph samples separated
  madgraphWLightJetsDatasetFiles.push_back("/home/sixie/hist/WJetsFakeValidationStudy_All/filler/008/WJetsFakeValidationStudy_All_s8-wjets-mg-id11_noskim.root");
  madgraphWLightJetsDatasetNames.push_back("s8-wjets-mg-id11");
  madgraphWcDatasetFiles.push_back("/home/sixie/hist/WJetsFakeValidationStudy_All/filler/008/WJetsFakeValidationStudy_All_f8-wc-mg-id11_noskim.root");
  madgraphWcDatasetNames.push_back("f8-wc-mg-id11");
  madgraphVQQDatasetFiles.push_back("/home/sixie/hist/WJetsFakeValidationStudy_All/filler/008/WJetsFakeValidationStudy_All_s8-vqq-mg-id11_noskim.root");
  madgraphVQQDatasetNames.push_back("s8-vqq-mg-id11");
  
  //Pythia WJets
  pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/WJetsFakeValidationStudy_All/filler/008/WJetsFakeValidationStudy_All_s8-we-id11_noskim.root");
  pythiaWJetsDatasetNames.push_back("s8-we-id11");
  pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/WJetsFakeValidationStudy_All/filler/008/WJetsFakeValidationStudy_All_s8-wm-id11_noskim.root");
  pythiaWJetsDatasetNames.push_back("s8-wm-id11");
  pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/WJetsFakeValidationStudy_All/filler/008/WJetsFakeValidationStudy_All_s8-wt-id11_noskim.root");
  pythiaWJetsDatasetNames.push_back("s8-wt-id11");

  vector<string> DatasetFiles;
  vector<string> DatasetNames;
  //DatasetFiles = QCDEnrichedWithConversionFilterDatasetFiles;
  //DatasetNames = QCDEnrichedWithConversionFilterDatasetNames;
//  DatasetFiles = madgraphWJetsDatasetFiles;
//  DatasetNames = madgraphWJetsDatasetNames;
  DatasetFiles = pythiaWJetsDatasetFiles;
  DatasetNames = pythiaWJetsDatasetNames;



  pythiaWJetsComputeFakeRatesDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-we-id11_noskim.root");
  pythiaWJetsComputeFakeRatesDatasetNames.push_back("s8-we-id11");
  pythiaWJetsComputeFakeRatesDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wm-id11_noskim.root");
  pythiaWJetsComputeFakeRatesDatasetNames.push_back("s8-wm-id11");
  pythiaWJetsComputeFakeRatesDatasetFiles.push_back("/home/sixie/hist/ComputeElectronFakeRate/filler/008/ComputeElectronFakeRate_s8-wt-id11_noskim.root");
  pythiaWJetsComputeFakeRatesDatasetNames.push_back("s8-wt-id11");



  //*****************************************************************************************
  //eta and pt bins
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
  vector<double> phibins;
  phibins.push_back(-3.5);
  phibins.push_back(-2.5);
  phibins.push_back(-1.5);
  phibins.push_back(-0.5);
  phibins.push_back(0.5);
  phibins.push_back(1.5);
  phibins.push_back(2.5);
  phibins.push_back(3.5);
  vector<double> deltaphibins;
  for (int i=0;i<18;i++) {
    deltaphibins.push_back(i*10);
  }
  vector<double> deltaetabins;
  for (int i=0;i<40;i++) {
    deltaetabins.push_back(-5.0 + i*0.25);
  }
  vector<double> dileptonmassbins;
  for (int i=0;i<30;i++) {
    dileptonmassbins.push_back(i*10);
  }


  //*****************************************************************************************
  //make plots for gen particle matching
  //*****************************************************************************************  
  vector<string> dirnames;
  vector<string> histnames;
  vector<string> histlabels;
  vector<string> dirlegendlabel;
  vector<string> legendnames;


  //*****************************************************************************************
  //Pre-Selection Distributions
  //*****************************************************************************************  

  //*****************************************************************************************
  //Lepton1 Pt
  //*****************************************************************************************  
//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hLepton1Pt_weighted");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames, ptbins,"Lepton1Pt");


  //*****************************************************************************************
  //Lepton2 Pt
  //*****************************************************************************************  
//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hLepton2Pt_weighted");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames, ptbins,"Lepton2Pt");

  //*****************************************************************************************
  //Lepton3Pt
  //*****************************************************************************************  
//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hLepton3Pt_weighted");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames, ptbins,"Lepton3Pt");

  //*****************************************************************************************
  //NLeptons 
  //*****************************************************************************************  
//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hNLeptons_weighted");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames, fakebins,"NLeptons");






  //*****************************************************************************************
  //Distributions After No Cuts
  //*****************************************************************************************  

  //*****************************************************************************************
  //Lepton Eta 
  //*****************************************************************************************  
//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hLeptonEta");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames, etabins,"LeptonEta");

  //*****************************************************************************************
  //PtMax
  //*****************************************************************************************  
//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hLeptonPtMax");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames, ptbins,"LeptonPtMax");


  //*****************************************************************************************
  //PtMin
  //*****************************************************************************************  
//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hLeptonPtMin");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames, ptbins,"LeptonPtMin");


  //*****************************************************************************************
  //Met
  //*****************************************************************************************  
//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hMetPtHist");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames, ptbins,"Met");

  //*****************************************************************************************
  //MetNotCorrectedForFakeMuons
  //*****************************************************************************************  
//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hUncorrMetPtHist");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames, ptbins,"MetNotCorrectedForFakeMuons");


  //*****************************************************************************************
  //DeltaPhiLeptons
  //*****************************************************************************************  
//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hDeltaPhiLeptons");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames, deltaphibins,"DeltaPhiLeptons");


  //*****************************************************************************************
  //DeltaEtaLeptons
  //*****************************************************************************************  
//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hDeltaEtaLeptons");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames, deltaetabins,"DeltaEtaLeptons");


  //*****************************************************************************************
  //DileptonMass
  //*****************************************************************************************  
//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hDileptonMass");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames, dileptonmassbins,"DileptonMass");









  //*****************************************************************************************
  //HWW Selection 
  //*****************************************************************************************  
//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hHWWSelection");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames, fakebins,"HWWSelection");







  //*****************************************************************************************
  //N-1 Distributions  
  //*****************************************************************************************  

  //*****************************************************************************************
  //LeptonPtMax_NMinusOne 
  //*****************************************************************************************  
//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hLeptonPtMax_NMinusOne");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames, ptbins,"LeptonPtMax_NMinusOne");


  //*****************************************************************************************
  //MetPtHist_NMinusOne 
  //*****************************************************************************************  
//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hMetPtHist_NMinusOne");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames, ptbins,"MetPtHist_NMinusOne");


  //*****************************************************************************************
  //NCentralJets_NMinusOne 
  //*****************************************************************************************  
//   dirnames.clear();
//   dirlegendlabel.clear();
//   histnames.clear();
//   legendnames.clear();
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphWJets");
//   dirlegendlabel.push_back("Reco Madgraph WJets");
//   dirnames.push_back("WJetsFakeValidationMod_MC");
//   dirlegendlabel.push_back("Simulation");
//   dirnames.push_back("WJetsFakeValidationMod_Reco_MadgraphQCD");
//   dirlegendlabel.push_back("Reco Prediction QCD");
//   histnames.push_back("hNCentralJets_NMinusOne");
//   legendnames.push_back("");
//   makeCrossDirComparisonPlot(madgraphWJetsDatasetFiles, madgraphWJetsDatasetNames, dirnames, dirlegendlabel, histnames, legendnames, fakebins,"NCentralJets_NMinusOne");






}

//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void makeWJetsFakeValidationPlots()
{

  //makeWJetsFakeElectronPlots();
  makeWJetsAnalysisPlots();

  return;   
}
