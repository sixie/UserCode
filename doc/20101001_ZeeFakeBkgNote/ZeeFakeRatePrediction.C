//root -l -b $CMSSW_BASE/src/UserCode/sixie/doc/20100610_EGammaFakeRateNote/ZeeFakeRatePrediction/ZeeFakeBkgPrediction.C+\(\"/home/sixie/ntuples/ZeeAnalysis/ZeeAnalysis_EGdata_noskim_normalized.root\",\"/home/sixie/ntuples/ZeeAnalysis/ZeeAnalysis_p10-zee-powheg-cteq66-v26_noskim_normalized.root\",\"/home/sixie/ntuples/ZeeAnalysis/ZeeAnalysis_fakebkg_Enriched.root\",\"/home/sixie/ntuples/ZeeAnalysis/ZeeAnalysis_fakebkg_Nonenriched.root\",\"Fake\"\)
 

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <utility>
#include "TFile.h"
#include "TMath.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TH1F.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "MitZeeEvent.C"
#include <vector>
//#include <MitStyle.h>
//#include <tdrstyle.C>
#include "MitCommon/DataFormats/interface/Types.h"
#include "MitHiggs/Utils/interface/PlotUtils.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitAna/DataTree/interface/Electron.h"
#include "MitAna/DataTree/interface/Met.h"
#include "MitAna/DataTree/interface/CompositeParticle.h"
#include "MitPhysics/FakeMods/interface/FakeRate.h"

#define NVARIABLES 12
#define NBINS 50
#define NPROCESSES 48
#define LUMINOSITY 0.01658

int fColors[NPROCESSES] = {kRed, kRed+1, kRed+2, kBlue, kBlue+1, kBlue+2,
                           kRed, kRed+1, kRed+2, kBlue, kBlue+1, kBlue+2,
                           kGreen, kBlack, kGreen+3, kGreen+3, kGreen+3, kGreen+3,
                           kRed+5, kBlue+5,
                           kCyan, kCyan, kCyan, kCyan, kCyan, kCyan, kCyan,
                           kCyan, kCyan, kCyan, kCyan, kCyan, kCyan,
                           kCyan, kCyan, kCyan, kCyan , kCyan,
                           kMagenta, kMagenta, kMagenta, kMagenta , kMagenta};

int fProcessCode[NPROCESSES] = { 0, 1, 2, 6, 7, 8,
                                 21000, 21001, 21002, 21006, 21007, 21008,
                                 4 , 29, 27, 19, 22, 28,
                                 17, 18,
                                 11,12,13,14,15,16, -1,
                                 21011,21012,21013,21014,21015,21016,
                                 100, 101, 102, 103, 104,
                                 21060, 21061, 21062, 21063, 21064,
                                 200, 201, 202, 203, 204};


string fProcessName[NPROCESSES] = { "W #rightarrow #mu#nu", "W #rightarrow e#nu","W #rightarrow #tau#nu","Z #rightarrow #mu#mu", "Z#rightarrow ee","Z #rightarrow #\tau#tau",
                                    "WmunuFR", "WenuFR", "WtaunuFR", "ZmumuFR", "ZeeFR", "ZtautauFR",
                                    "t#bar{t}", "WW_2l", "WZ", "WZ_incl", "ZZ_2l2n", "ZZ",
                                    "Wgamma", "Zgamma",
                                    "QCDEM20_30", "QCDEM30_80", "QCDEM80_170", "QCDBCE20_30", "QCDBCE30_80", "QCDBCE80_170", "InclMu15",
                                    "QCDEM20_30FR", "QCDEM30_80FR", "QCDEM80_170FR", "QCDBCE20_30FR", "QCDBCE30_80FR", "QCDBCE80_170FR",
                                    "QCD15", "QCD30","QCD80","QCD170","QCD300",
                                    "QCD15FR", "QCD30FR","QCD80FR","QCD170FR","QCD300FR",
                                    "PhotonJet15", "PhotonJet30","PhotonJet80","PhotonJet170","PhotonJet300"};


string fVariableNames[NVARIABLES] = { "ptmax", "ptmin", "lepton1Eta", "lepton1Phi", "lepton2Eta", "lepton2Phi", 
                                      "caloMet" , "TCMet", "PFMet", "dielectronmass", "dielectronmass_tightloose",
                                      "dielectronmass_bothtight"  };
string fVariableAxisLabels[NVARIABLES] = { "ptMax [GeV/c]", "ptMin [GeV/c]", "lepton1 #eta", "lepton1 #phi", "lepton2 #eta", "lepton2 #phi", 
                                           "CaloMet [GeV/c]", "TCMet [GeV/c]", "PFMet [GeV/c]", 
                                           "M_{ee} [GeV/c^{2}]", "M_{ee} [GeV/c^{2}]", "M_{ee} [GeV/c^{2}]" };
string fYAxisLabels[NVARIABLES] = { "Events / 2.0 GeV/c^{2}","Events / 2.0 GeV/c^{2}","Events","Events","Events","Events","Events","Events","Events","Events / 3.0 GeV/c^{2}","Events / 3.0 GeV/c^{2}","Events / 3.0 GeV/c^{2}",  };
Float_t fVariableRangeMin[NVARIABLES] = { 0.0, 0.0, -3.0, -3.0, -3.2, -3.2, 0, 0 , 0, 0, 0, 0 };
Float_t fVariableRangeMax[NVARIABLES] = { 100.0, 100.0, 3.0, 3.0, 3.2, 3.2, 50, 50, 50, 200, 200, 200 };

Float_t fVariableMinY[NVARIABLES] = { -999, -999, -999, -999, -999, -999, -999, -999 , -999, -999, -999, -999 };
Float_t fVariableMaxY[NVARIABLES] = { -999, -999, -999, -999, -999, -999, -999, -999 , -999, -999, -999, -999 };
Float_t fVariableLogMinY[NVARIABLES] = { 1e-2, 1e-2, 1e-2, 1e-2, 1e-2, 1e-2, 1e-2, 1e-2 , 1e-2,1e-8, 1e-4, 1e-4  };
Float_t fVariableLogMaxY[NVARIABLES] = { 1e4, 1e4, -999, -999, -999, -999, -999, -999 , -999, 1e-2, 1e3, 1e2 };


//------------------------------------------------------------------------------
// getTreeFromFile
//------------------------------------------------------------------------------
TTree* getTreeFromFile(const char* infname, const char* tname)
{
  bool verbose = false;

  if (verbose) {
    cout << "--- Open file " << infname << endl;
  }
  
  TFile* inf = new TFile(infname,"read");
  assert(inf);

  TTree* t = (TTree*)inf->Get(tname);
  
  if (!t) {
    TDirectory *dir = (TDirectory*)inf->FindObjectAny("ZeeAnalysisMod");
    if (!dir) {
      cout << "Cannot get Directory ElectronCommissioningMod from file " << infname << endl;
      assert(dir);
    }
    t = (TTree*)dir->Get(tname);
  }

  if (!t) {
    cout << "Cannot get Tree with name " << tname << " from file " << infname << endl;
  }
  assert(t);


  if (verbose) {
    cout << "---\tRecovered tree " << t->GetName()
	 << " with "<< t->GetEntries() << " entries" << endl;
  }
  
  return t;
}



//*************************************************************************************************
//Convert int to string
//*************************************************************************************************
string IntToString(int i) {
  char temp[100];
  sprintf(temp, "%d", i);
  string str = temp;
  return str;
}

//*************************************************************************************************
//Convert int to string
//*************************************************************************************************
string DoubleToString(double i) {
  char temp[100];
  sprintf(temp, "%.4f", i);
  string str = temp;
  return str;
}

Double_t IntegralError(TH1F *hist, TH1F* sysErrorHist, Double_t xmin, Double_t xmax) {

  Int_t minBin = hist->GetXaxis()->FindFixBin(xmin);
  Int_t maxBin = hist->GetXaxis()->FindFixBin(xmax);

  Double_t statErrorSqr = 0;
  Double_t sysError = 0;
  for (int b=minBin; b<=maxBin;b++) {
    statErrorSqr += pow(hist->GetBinError(b),2);
//     cout << "bin " << b << " : " << hist->GetBinError(b) << endl;
  }

  if (sysErrorHist) {
    for (int b=minBin; b<=maxBin;b++) {
      sysError += sysErrorHist->GetBinContent(b);
//       cout << "bin " << b << " : " << sysErrorHist->GetBinContent(b) << endl;
    }
  }

  return TMath::Sqrt(statErrorSqr + pow(sysError,2));
  
}


Bool_t passEventSelectionCuts(MitZeeEvent *event) {
  
  Bool_t pass = kTRUE;

  if (!( event->nEle >= 2 &&  event->electronPt[0] > 20.0 && event->electronPt[1] > 20.0)) pass = kFALSE;

  return pass;
}


Bool_t passElectronCuts(MitZeeEvent *event, Int_t i) {
  
  Bool_t pass = kTRUE;

  //ECAL driven only
  if (event->electronType[i] == 10) {
    pass = kFALSE;
    return pass;
  }
  
  //Barrel 
  if (event->electronLocation[i] == 1) {
    if (!
        ( event->electronCoviEtaiEta[i] < 0.01 
          && fabs(event->electronDeltaEta[i]) < 0.007
          && fabs(event->electronDeltaPhi[i]) < 0.8
          && event->electronHadOverEm[i] < 0.05
          && (event->electronTrackIsolationDr03[i] + 
              TMath::Max(event->electronEcalRecHitIsoDr04[i] - 1.0, 0.0) + 
              event->electronHcalTowerSumEtDr04[i]) / event->electronPt[i] < 0.15
          && event->electronNMissingHits[i] <= 1
          )
      ) {
      pass = kFALSE;
    }      
  }
  //Endcap
  else if (event->electronLocation[i] == 2) {
    if (!
        (  event->electronCoviEtaiEta[i] < 0.03
           && fabs(event->electronDeltaEta[i]) < 0.010
           && fabs(event->electronDeltaPhi[i]) < 0.7
           && event->electronHadOverEm[i] < 0.07
           && (event->electronTrackIsolationDr03[i] + 
               TMath::Max(event->electronEcalRecHitIsoDr04[i] - 1.0, 0.0) + 
               event->electronHcalTowerSumEtDr04[i]) / event->electronPt[i] < 0.10
           && event->electronNMissingHits[i] <= 1
          )
      ) {
      pass = kFALSE;
    }
  } else {
    pass = kFALSE;
    return pass;
  }

  return pass;
}


Bool_t passDenominatorCuts(MitZeeEvent *event, Int_t i) {
  
  Bool_t pass = kTRUE;

  //ECAL driven only
  if (event->electronType[i] == 10) {
    pass = kFALSE;
    return pass;
  }
  
//   if (!
//       ( (event->electronTrackIsolationDr03[i] + 
//          TMath::Max(event->electronEcalRecHitIsoDr04[i] - 1.0, 0.0) + 
//          event->electronHcalTowerSumEtDr04[i]) / event->electronPt[i] < 0.5
//         )
//     ) {
//     pass = kFALSE;
//   }

  return pass;
}



//*************************************************************************************************
//Draws the signal and background histograms together and makes gif file
//*************************************************************************************************
void DrawTotalPredictionComparison(   TH1F* bkg_LooseLoosePredicted,  TH1F* bkg_TightLoosePredicted, TH1F* bkg_MC,
                                      string histname, 
                                      Double_t minY = -999 , Double_t maxY = -999,
                                Bool_t useLogY = kFALSE) {
  
  string filename = histname;
  if (useLogY) filename += "_logY";
//  filename += ".eps";

  TCanvas *cv = new TCanvas(histname.c_str(), histname.c_str(), 0,0,800,600);
  if (useLogY) cv->SetLogy();

  TLegend *tmpLegend = new TLegend(0.73,0.55,0.93,0.90);
  tmpLegend->SetTextSize(0.03);
  tmpLegend->SetBorderSize(1);
  tmpLegend->AddEntry(bkg_MC, "MC", "L");
  tmpLegend->AddEntry(bkg_LooseLoosePredicted, "LooseLoosePredicted", "LP");
  tmpLegend->AddEntry(bkg_TightLoosePredicted, "TightLoosePredicted", "LP");

  if (minY != -999) {
    bkg_MC->SetMinimum(minY);
    bkg_LooseLoosePredicted->SetMinimum(minY);
    bkg_TightLoosePredicted->SetMinimum(minY);
  }
  if (maxY != -999) {
    bkg_MC->SetMaximum(maxY);
    bkg_LooseLoosePredicted->SetMaximum(maxY);
    bkg_TightLoosePredicted->SetMaximum(maxY);
  }

  bkg_MC->GetYaxis()->SetTitleOffset(1.1);
  bkg_MC->GetXaxis()->SetTitleOffset(1.0);

  bkg_MC->Draw("hist");
  bkg_LooseLoosePredicted->SetLineColor(kBlue);
  bkg_LooseLoosePredicted->SetMarkerColor(kBlue);
  bkg_LooseLoosePredicted->Draw("E1,same");
  bkg_TightLoosePredicted->SetLineColor(kRed);
  bkg_TightLoosePredicted->SetMarkerColor(kRed);
  bkg_TightLoosePredicted->Draw("E1,same");
  
  tmpLegend->Draw();
  cv->SaveAs((filename + ".gif").c_str());
  cv->SaveAs((filename + ".eps").c_str());

}



//*************************************************************************************************
//Draws the signal and background histograms together and makes gif file
//*************************************************************************************************
void DrawStackPredictionComparison(   THStack* bkg_LooseLoosePredicted,  THStack* bkg_TightLoosePredicted, THStack* bkg_MC,
                                      vector<string> processNames, 
                                      string histname, 
                                      Double_t minY = -999 , Double_t maxY = -999,
                                      Bool_t useLogY = kFALSE) {
  
  string filename = histname;
  if (useLogY) filename += "_logY";
//  filename += ".eps";

  TCanvas *cv = new TCanvas(histname.c_str(), histname.c_str(), 0,0,800,600);
  if (useLogY) cv->SetLogy();

  //Draw Stacks
  TLegend *tmpLegend = new TLegend(0.73,0.55,0.93,0.90);
  tmpLegend->SetTextSize(0.03);
  tmpLegend->SetBorderSize(1);
  for(int i=0; i<bkg_MC->GetStack()->GetEntries(); ++i) {
    tmpLegend->AddEntry((TH1F*)bkg_MC->GetStack()->At(i), processNames[i].c_str(), "F");
  }
  bkg_MC->Draw("hist");
  tmpLegend->Draw();
  cv->SaveAs((histname+"_MCStack.gif").c_str());

  TLegend *tmpLegend_LooseLoosePredicted = new TLegend(0.73,0.55,0.93,0.90);
  tmpLegend_LooseLoosePredicted->SetTextSize(0.03);
  tmpLegend_LooseLoosePredicted->SetBorderSize(1);
  for(int i=0; i<bkg_LooseLoosePredicted->GetStack()->GetEntries(); ++i) {
    tmpLegend->AddEntry((TH1F*)bkg_LooseLoosePredicted->GetStack()->At(i), processNames[i].c_str(), "F");
  }
  bkg_LooseLoosePredicted->Draw("hist");
  tmpLegend_LooseLoosePredicted->Draw();
  cv->SaveAs((histname+"_LooseLoosePredictedStack.gif").c_str());

  TLegend *tmpLegend_TightLoosePredicted = new TLegend(0.73,0.55,0.93,0.90);
  tmpLegend_TightLoosePredicted->SetTextSize(0.03);
  tmpLegend_TightLoosePredicted->SetBorderSize(1);
  for(int i=0; i<bkg_TightLoosePredicted->GetStack()->GetEntries(); ++i) {
    tmpLegend->AddEntry((TH1F*)bkg_TightLoosePredicted->GetStack()->At(i), processNames[i].c_str(), "F");
  }
  bkg_TightLoosePredicted->Draw("hist");
  tmpLegend_TightLoosePredicted->Draw();
  cv->SaveAs((histname+"_TightLoosePredictedStack.gif").c_str());


  //Draw individual components
  for(int i=0; i<bkg_TightLoosePredicted->GetStack()->GetEntries(); ++i) {
    TH1F* tmpHist_MC = (TH1F*)bkg_MC->GetStack()->At(i);
    TH1F* tmpHist_LooseLoosePredicted = (TH1F*)bkg_LooseLoosePredicted->GetStack()->At(i);
    TH1F* tmpHist_TightLoosePredicted = (TH1F*)bkg_TightLoosePredicted->GetStack()->At(i);

    TLegend *tmpLegend1 = new TLegend(0.73,0.55,0.93,0.90);
    tmpLegend1->SetTextSize(0.03);
    tmpLegend1->SetBorderSize(1);
    tmpLegend1->AddEntry(tmpHist_MC, "MC", "L");
    tmpLegend1->AddEntry(tmpHist_LooseLoosePredicted, "LooseLoosePredicted", "LP");
    tmpLegend1->AddEntry(tmpHist_TightLoosePredicted, "TightLoosePredicted", "LP");

    if (minY != -999) {
      tmpHist_MC->SetMinimum(minY);
      tmpHist_LooseLoosePredicted->SetMinimum(minY);
      tmpHist_TightLoosePredicted->SetMinimum(minY);
    }
    if (maxY != -999) {
      tmpHist_MC->SetMaximum(maxY);
      tmpHist_LooseLoosePredicted->SetMaximum(maxY);
      tmpHist_TightLoosePredicted->SetMaximum(maxY);
    }
    
    tmpHist_MC->GetYaxis()->SetTitleOffset(1.1);
    tmpHist_MC->GetXaxis()->SetTitleOffset(1.0);
    
    tmpHist_MC->Draw("hist");
    tmpHist_LooseLoosePredicted->SetLineColor(kBlue);
    tmpHist_LooseLoosePredicted->SetMarkerColor(kBlue);
    tmpHist_LooseLoosePredicted->Draw("E1,same");
    tmpHist_TightLoosePredicted->SetLineColor(kRed);
    tmpHist_TightLoosePredicted->SetMarkerColor(kRed);
    tmpHist_TightLoosePredicted->Draw("E1,same");
    
    tmpLegend1->Draw();
    cv->SaveAs((histname + "_" + processNames[i] + ".gif").c_str());
    cv->SaveAs((filename + "_" + processNames[i] + ".eps").c_str());
    
  }
  
}



//*************************************************************************************************
//Draws the signal and background histograms together and makes gif file
//*************************************************************************************************
void DrawComparison(THStack* data, TH1F* sig, THStack* bkg, TLegend *legend , string histname, 
                             Double_t minY = -999 , Double_t maxY = -999,
                             Bool_t useLogY = kFALSE) {

  string filename = histname;
  if (useLogY) filename += "_logY";
//  filename += ".eps";

  TCanvas *cv = new TCanvas(histname.c_str(), histname.c_str(), 0,0,800,600);
  if (useLogY) cv->SetLogy();

  sig->SetLineWidth(1);
  sig->SetLineColor(kAzure+2);
  sig->SetFillStyle(1001);
  sig->SetFillColor(kAzure+6);

  legend->SetTextSize(0.03);
  legend->SetBorderSize(1);

//   legend->AddEntry(data, "Data", "P");  
//   legend->AddEntry(sig, "Z #rightarrow ee", "F");
  
  THStack *tmpBkg = (THStack*)bkg->Clone("bkgStack");
  tmpBkg->Add(sig);



  //set best Y-range
  double MaxY = sig->GetMaximum();
  if (data->GetMaximum() > MaxY )
    MaxY = data->GetMaximum();

  tmpBkg->Draw("hist");
  if (tmpBkg->GetMaximum() > MaxY)
    MaxY = tmpBkg->GetMaximum();

  tmpBkg->GetYaxis()->SetTitleOffset(1.1);
  sig->GetYaxis()->SetTitleOffset(1.1);
  data->GetYaxis()->SetTitleOffset(1.1);
  tmpBkg->GetXaxis()->SetTitleOffset(1.0);
  sig->GetXaxis()->SetTitleOffset(1.0);
  data->GetXaxis()->SetTitleOffset(1.0);

  tmpBkg->SetMinimum(0.01);
  tmpBkg->SetMaximum(MaxY*1.2);  
  sig->SetMinimum(0.01);
  sig->SetMaximum(MaxY*1.2);
  data->SetMinimum(0.01);
  data->SetMaximum(MaxY*1.2);
  
  if (minY != -999) {
    tmpBkg->SetMinimum(minY);
    sig->SetMinimum(minY);
    data->SetMinimum(minY);    
  }
  if (maxY != -999) {
    tmpBkg->SetMaximum(maxY);
    sig->SetMaximum(maxY);
    data->SetMaximum(maxY);
  }

  //CMS Preliminary label
  TPaveText *prelimLabel = new TPaveText(0.21,0.85,0.41,0.90,"NDC");
  prelimLabel->SetTextColor(kBlack);
  prelimLabel->SetFillColor(kWhite);
  prelimLabel->SetBorderSize(0);
  prelimLabel->SetTextAlign(12);
  prelimLabel->SetTextSize(0.03);
  prelimLabel->AddText("CMS Preliminary 2010 #sqrt{s} = 7 TeV");
  prelimLabel->Draw();

  //Luminosity label
  TPaveText *tb = new TPaveText(0.21,0.77,0.41,0.82,"NDC");
  tb->SetTextColor(kBlack);
  tb->SetFillColor(kWhite);
  tb->SetBorderSize(0);
  tb->SetTextAlign(12);
  string lumi = DoubleToString(LUMINOSITY);
  tb->AddText((string("#int#font[12]{L}dt = ") + lumi + string(" pb^{ -1}")).c_str());
  tb->Draw();

  sig->Draw("samehist");
  tmpBkg->Draw("samehist");
  data->Draw("sameE1");

  legend->Draw();
  cv->SaveAs((filename + ".gif").c_str());
  cv->SaveAs((filename + ".eps").c_str());
  cv->SaveAs((filename + ".C").c_str());
  delete cv;
}

//*************************************************************************************************
//Main part of the macro
//*************************************************************************************************
void ZeeFakeBkgPrediction(const string dataInputFilename, 
               const string signalInputFilename, 
               const string mcBkgInputFilename,        
               const string fakePredictionBkgInputFilename,        
               const string Label =  "") {

  string label = Label;
  if (Label != "") label = "_" + Label;

  //*************************************************************************************************                                                               
  //Global Definition of Processes                                                                                                                                  
  //*************************************************************************************************                                                               
  vector<vector<Int_t> > processes;
  processes.push_back(vector<Int_t>());
  processes.back().push_back(1);
  processes.back().push_back(31001);
  processes.back().push_back(2);
  processes.back().push_back(31002);
  processes.push_back(vector<Int_t>());
  processes.back().push_back(17);
  processes.push_back(vector<Int_t>());
  processes.back().push_back(11);
  processes.back().push_back(12);
  processes.back().push_back(13);
  processes.back().push_back(14);
  processes.back().push_back(15);
  processes.back().push_back(16);
  processes.back().push_back(100);
  processes.back().push_back(101);
  processes.back().push_back(102);
  processes.back().push_back(103);
  processes.back().push_back(104);
  processes.push_back(vector<Int_t>());
  processes.back().push_back(200);
  processes.back().push_back(201);
  processes.back().push_back(202);
  processes.back().push_back(203);
  processes.back().push_back(204);
  processes.push_back(vector<Int_t>());
  processes.back().push_back(8);
  processes.push_back(vector<Int_t>());
  processes.back().push_back(4);
  processes.push_back(vector<Int_t>());
  processes.back().push_back(29);
  processes.push_back(vector<Int_t>());
  processes.back().push_back(27);
  processes.push_back(vector<Int_t>());
  processes.back().push_back(28);

  vector<string> processNames;
  processNames.push_back("W+Jets");
  processNames.push_back("W+#gamma");
  processNames.push_back("QCD");
  processNames.push_back("#gamma+Jets");
  processNames.push_back("Z #rightarrow #tau#tau");
  processNames.push_back("t#bar{t}");
  processNames.push_back("WW");
  processNames.push_back("WZ");
  processNames.push_back("ZZ");

  vector<string> processLabel;
  processLabel.push_back("WJets");
  processLabel.push_back("Wgamma");
  processLabel.push_back("QCD");
  processLabel.push_back("PhotonJets");
  processLabel.push_back("Ztautau");
  processLabel.push_back("ttbar");
  processLabel.push_back("WW");
  processLabel.push_back("WZ");
  processLabel.push_back("ZZ");

  vector<double> processYMax;
  processYMax.push_back(0.001);
  processYMax.push_back(0.002);
  processYMax.push_back(0.02);
  processYMax.push_back(0.001);
  processYMax.push_back(0.002);
  processYMax.push_back(0.002);
  processYMax.push_back(0.002);
  processYMax.push_back(0.002);
  processYMax.push_back(0.002);

  vector<Int_t> colors;
  colors.push_back(kAzure+2);
  colors.push_back(kAzure+4);
  colors.push_back(kGreen+1);
  colors.push_back(kYellow+1);
  colors.push_back(kMagenta);
  colors.push_back(kRed+1);
  colors.push_back(kCyan);
  colors.push_back(kCyan+2);
  colors.push_back(kCyan+4);

  assert(processNames.size() == processes.size());
  assert(processNames.size() == colors.size());


  //********************************************************
  // Set up Fake Rate
  //********************************************************
  Bool_t use2DFakeRate = kTRUE;
  Bool_t useFitFunction = kFALSE;
  mithep::FakeRate *fFakeRate = new mithep::FakeRate("/build/sixie/releases/analysis/CMSSW_3_6_1/src/MitPhysics/data/ElectronFakeRate_2010Data.ZeeAnalysis.root",
                           "/build/sixie/releases/analysis/CMSSW_3_6_1/src/MitPhysics/data/FakeRates.20100323.root",
                           "", "",
                           "RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator_WithSystematics_data_Jet30_PtEta",
                           "TrackerMuonFakeRate_PtEta_Pythia_Jet30",
                           use2DFakeRate, useFitFunction );

  mithep::FakeRate *fMCFakeRate = new mithep::FakeRate("/build/sixie/releases/analysis/CMSSW_3_6_1/src/MitPhysics/data/ElectronFakeRate_2010Data.ZeeAnalysis.root",
                           "/build/sixie/releases/analysis/CMSSW_3_6_1/src/MitPhysics/data/FakeRates.20100323.root",
                           "", "",
                           "RecoElectronEfficiency_RecoDenominator_VBTF95WPIdIsoNumerator_QCD30MCv26a_Jet30_PtEta",
                           "TrackerMuonFakeRate_PtEta_Pythia_Jet30",
                           use2DFakeRate, useFitFunction );

//   mithep::FakeRate *fFakeRate = new mithep::FakeRate("/build/sixie/releases/analysis/CMSSW_3_6_1/src/MitPhysics/data/ElectronFakeRate_2010Data.root",
//                            "/build/sixie/releases/analysis/CMSSW_3_6_1/src/MitPhysics/data/FakeRates.20100323.root",
//                            "", "",
//                            "RecoElectronEfficiency_LooseIsoDenominator_VBTF95WPIdIsoNumerator_data_Jet15_PtEta",
//                            "TrackerMuonFakeRate_PtEta_Pythia_Jet30",
//                            use2DFakeRate, useFitFunction );
//   mithep::FakeRate *fFakeRate = new mithep::FakeRate("/build/sixie/releases/analysis/CMSSW_3_6_1/src/MitPhysics/data/ElectronFakeRate_2010Data.root",
//                                                      "/build/sixie/releases/analysis/CMSSW_3_6_1/src/MitPhysics/data/FakeRates.20100323.root",
//                                                      "", "",
//                                                      "RecoElectronEfficiency_LooseIsoDenominator_VBTF95WPIdIsoNumerator_QCD30MCv26a_Jet30_PtEta",
//                                                      "TrackerMuonFakeRate_PtEta_Pythia_Jet30",
//                                                      use2DFakeRate, useFitFunction );


  //********************************************************
  // Define Histograms
  //********************************************************

  vector<TH1F*> dataDistributions;
  vector<TH1F*> dataDistributions_LooseLoosePrediction;
  vector<TH1F*> dataDistributions_LooseLoosePrediction_sysErrors;
  vector<TH1F*> dataDistributions_TightLoosePrediction;
  vector<TH1F*> dataDistributions_TightLoosePrediction_sysErrors;
  vector<TH1F*> signalDistributions;
  vector<TH1F*> signalDistributions_LooseLoosePrediction;
  vector<TH1F*> signalDistributions_LooseLoosePrediction_sysErrors;
  vector<TH1F*> signalDistributions_TightLoosePrediction;
  vector<TH1F*> signalDistributions_TightLoosePrediction_sysErrors;

  vector<THStack*> bkgStacks_MC;
  vector<vector<TH1F*> > bkgDistributions_MC;
  vector<TH1F*> totalBkgDistributions_MC;

  vector<THStack*> bkgStacks_LooseLoosePrediction;
  vector<vector<TH1F*> > bkgDistributions_LooseLoosePrediction;
  vector<TH1F*> totalBkgDistributions_LooseLoosePrediction;
  vector<vector<TH1F*> > bkgDistributions_LooseLoosePrediction_sysErrors;
  vector<TH1F*> totalBkgDistributions_LooseLoosePrediction_sysErrors;

  vector<THStack*> bkgStacks_TightLoosePrediction;
  vector<vector<TH1F*> > bkgDistributions_TightLoosePrediction;
  vector<TH1F*> totalBkgDistributions_TightLoosePrediction;
  vector<vector<TH1F*> > bkgDistributions_TightLoosePrediction_sysErrors;
  vector<TH1F*> totalBkgDistributions_TightLoosePrediction_sysErrors;

  vector<TLegend*> legends;

  for (int v = 0; v < NVARIABLES;v++) {
    string histName = "h" + fVariableNames[v];
    string axisLabel = ";" + fVariableAxisLabels[v] + ";" + fYAxisLabels[v];
    TH1F* tmpDataHist = new TH1F((histName+"_data_"+label).c_str(),axisLabel.c_str(),NBINS, 
			     fVariableRangeMin[v], fVariableRangeMax[v]);
    TH1F* tmpDataHist_LooseLoosePrediction = new TH1F((histName+"_data_LooseLoosePrediction_"+label).c_str(),axisLabel.c_str(),NBINS, 
			     fVariableRangeMin[v], fVariableRangeMax[v]);
    TH1F* tmpDataHist_TightLoosePrediction = new TH1F((histName+"_data_TightLoosePrediction_"+label).c_str(),axisLabel.c_str(),NBINS, 
			     fVariableRangeMin[v], fVariableRangeMax[v]);
    TH1F* tmpDataHist_LooseLoosePrediction_sysErrors = new TH1F((histName+"_data_LooseLoosePrediction_sysErrors_"+label).c_str(),axisLabel.c_str(),NBINS, 
			     fVariableRangeMin[v], fVariableRangeMax[v]);
    TH1F* tmpDataHist_TightLoosePrediction_sysErrors = new TH1F((histName+"_data_TightLoosePrediction_sysErrors_"+label).c_str(),axisLabel.c_str(),NBINS, 
			     fVariableRangeMin[v], fVariableRangeMax[v]);
   TH1F* tmpSigHist = new TH1F((histName+"_sig_"+label).c_str(),axisLabel.c_str(),NBINS, 
			     fVariableRangeMin[v], fVariableRangeMax[v]);
    TH1F* tmpSigHist_LooseLoosePrediction = new TH1F((histName+"_signal_LooseLoosePrediction_"+label).c_str(),axisLabel.c_str(),NBINS, 
			     fVariableRangeMin[v], fVariableRangeMax[v]);
    TH1F* tmpSigHist_TightLoosePrediction = new TH1F((histName+"_signal_TightLoosePrediction_"+label).c_str(),axisLabel.c_str(),NBINS, 
			     fVariableRangeMin[v], fVariableRangeMax[v]);
    TH1F* tmpSigHist_LooseLoosePrediction_sysErrors = new TH1F((histName+"_signal_LooseLoosePrediction_sysErrors_"+label).c_str(),axisLabel.c_str(),NBINS, 
			     fVariableRangeMin[v], fVariableRangeMax[v]);
    TH1F* tmpSigHist_TightLoosePrediction_sysErrors = new TH1F((histName+"_signal_TightLoosePrediction_sysErrors_"+label).c_str(),axisLabel.c_str(),NBINS, 
			     fVariableRangeMin[v], fVariableRangeMax[v]);
    TH1F* tmpBkgHist_LooseLoosePrediction = new TH1F((histName+"_totalBkg_LooseLoosePrediction_"+label).c_str(),axisLabel.c_str(),NBINS, 
			     fVariableRangeMin[v], fVariableRangeMax[v]);
    TH1F* tmpBkgHist_TightLoosePrediction = new TH1F((histName+"_totalBkg_TightLoosePrediction_"+label).c_str(),axisLabel.c_str(),NBINS, 
			     fVariableRangeMin[v], fVariableRangeMax[v]);
    TH1F* tmpBkgHist_LooseLoosePrediction_sysErrors = new TH1F((histName+"_totalBkg_LooseLoosePrediction_sysErrors_"+label).c_str(),axisLabel.c_str(),NBINS, 
			     fVariableRangeMin[v], fVariableRangeMax[v]);
    TH1F* tmpBkgHist_TightLoosePrediction_sysErrors = new TH1F((histName+"_totalBkg_TightLoosePrediction_sysErrors_"+label).c_str(),axisLabel.c_str(),NBINS, 
			     fVariableRangeMin[v], fVariableRangeMax[v]);
    TH1F* tmpBkgHist_MC = new TH1F((histName+"_totalBkg_MC_"+label).c_str(),axisLabel.c_str(),NBINS, 
			     fVariableRangeMin[v], fVariableRangeMax[v]);
    tmpDataHist->Sumw2();
    tmpDataHist_LooseLoosePrediction->Sumw2();
    tmpDataHist_TightLoosePrediction->Sumw2();
    tmpSigHist->Sumw2();
    tmpSigHist_LooseLoosePrediction->Sumw2();
    tmpSigHist_TightLoosePrediction->Sumw2();
    tmpBkgHist_LooseLoosePrediction->Sumw2();
    tmpBkgHist_TightLoosePrediction->Sumw2();
    tmpBkgHist_MC->Sumw2();



    tmpSigHist->SetFillColor(kBlue-3);
    THStack *tmpBkgStack_MC = new THStack((histName+"_bkg_MC_"+label).c_str(),axisLabel.c_str());
    THStack *tmpBkgStack_LooseLoosePrediction = new THStack((histName+"_bkg_LooseLoosePrediction_"+label).c_str(),axisLabel.c_str());
    THStack *tmpBkgStack_TightLoosePrediction = new THStack((histName+"_bkg_TightLoosePrediction_"+label).c_str(),axisLabel.c_str());

    vector<TH1F*> tmpBkgDistributions_MC;
    vector<TH1F*> tmpBkgDistributions_LooseLoosePrediction;
    vector<TH1F*> tmpBkgDistributions_TightLoosePrediction;
    vector<TH1F*> tmpBkgDistributions_LooseLoosePrediction_sysErrors;
    vector<TH1F*> tmpBkgDistributions_TightLoosePrediction_sysErrors;
    TLegend *tmpLegend = new TLegend(0.73,0.55,0.93,0.90);

    if (dataInputFilename != "") {
      tmpLegend->AddEntry(tmpDataHist, "Data", "P"); 
    }

    for (int j=0;j<processes.size();++j) {
      string temphistname = histName+"_"+processNames[j];
      TH1F *temphist_MC = new TH1F((temphistname+"_MC").c_str(),axisLabel.c_str(),NBINS, 
				fVariableRangeMin[v], 
				fVariableRangeMax[v]);  
      TH1F *temphist_LooseLoosePrediction = new TH1F((temphistname+"_LooseLoosePrediction").c_str(),axisLabel.c_str(),NBINS, 
				fVariableRangeMin[v], 
				fVariableRangeMax[v]);  
      TH1F *temphist_TightLoosePrediction = new TH1F((temphistname+"_TightLoosePrediction").c_str(),axisLabel.c_str(),NBINS, 
				fVariableRangeMin[v], 
				fVariableRangeMax[v]);  
      TH1F *temphist_LooseLoosePrediction_sysErrors = new TH1F((temphistname+"_LooseLoosePrediction_sysErrors").c_str(),axisLabel.c_str(),NBINS, 
				fVariableRangeMin[v], 
				fVariableRangeMax[v]);  
      TH1F *temphist_TightLoosePrediction_sysErrors = new TH1F((temphistname+"_TightLoosePrediction_sysErrors").c_str(),axisLabel.c_str(),NBINS, 
				fVariableRangeMin[v], 
				fVariableRangeMax[v]);  
      temphist_MC->Sumw2();
      temphist_MC->SetDirectory(0);      
      temphist_MC->SetFillStyle(1001);
      temphist_MC->SetFillColor(colors[j]);
      temphist_MC->SetLineWidth(1);
            
      temphist_LooseLoosePrediction->Sumw2();
      temphist_LooseLoosePrediction->SetDirectory(0);      
      temphist_LooseLoosePrediction->SetFillStyle(1001);
      temphist_LooseLoosePrediction->SetFillColor(colors[j]);
      temphist_LooseLoosePrediction->SetLineWidth(1);

      temphist_TightLoosePrediction->Sumw2();
      temphist_TightLoosePrediction->SetDirectory(0);      
      temphist_TightLoosePrediction->SetFillStyle(1001);
      temphist_TightLoosePrediction->SetFillColor(colors[j]);
      temphist_TightLoosePrediction->SetLineWidth(1);

      tmpBkgDistributions_MC.push_back(temphist_MC);
      tmpBkgStack_MC->Add(temphist_MC);
      tmpBkgDistributions_LooseLoosePrediction.push_back(temphist_LooseLoosePrediction);
      tmpBkgDistributions_LooseLoosePrediction_sysErrors.push_back(temphist_LooseLoosePrediction_sysErrors);
      tmpBkgStack_LooseLoosePrediction->Add(temphist_LooseLoosePrediction);      
      tmpBkgDistributions_TightLoosePrediction.push_back(temphist_TightLoosePrediction);
      tmpBkgDistributions_TightLoosePrediction_sysErrors.push_back(temphist_TightLoosePrediction_sysErrors);
      tmpBkgStack_TightLoosePrediction->Add(temphist_TightLoosePrediction);
      
      
      //Add legend entry
      tmpLegend->AddEntry(temphist_MC, processNames[j].c_str(), "F"); 
    }
    tmpLegend->AddEntry(tmpSigHist, "Z #rightarrow ee", "F"); 

    dataDistributions.push_back(tmpDataHist);
    dataDistributions_LooseLoosePrediction.push_back(tmpDataHist_LooseLoosePrediction);
    dataDistributions_TightLoosePrediction.push_back(tmpDataHist_TightLoosePrediction);
    dataDistributions_LooseLoosePrediction_sysErrors.push_back(tmpDataHist_LooseLoosePrediction_sysErrors);
    dataDistributions_TightLoosePrediction_sysErrors.push_back(tmpDataHist_TightLoosePrediction_sysErrors);
    signalDistributions.push_back(tmpSigHist);
    signalDistributions_LooseLoosePrediction.push_back(tmpSigHist_LooseLoosePrediction);
    signalDistributions_TightLoosePrediction.push_back(tmpSigHist_TightLoosePrediction);
    signalDistributions_LooseLoosePrediction_sysErrors.push_back(tmpSigHist_LooseLoosePrediction_sysErrors);
    signalDistributions_TightLoosePrediction_sysErrors.push_back(tmpSigHist_TightLoosePrediction_sysErrors);
    totalBkgDistributions_LooseLoosePrediction.push_back(tmpBkgHist_LooseLoosePrediction);
    totalBkgDistributions_TightLoosePrediction.push_back(tmpBkgHist_TightLoosePrediction);
    totalBkgDistributions_LooseLoosePrediction_sysErrors.push_back(tmpBkgHist_LooseLoosePrediction_sysErrors);
    totalBkgDistributions_TightLoosePrediction_sysErrors.push_back(tmpBkgHist_TightLoosePrediction_sysErrors);
    totalBkgDistributions_MC.push_back(tmpBkgHist_MC);
    bkgStacks_MC.push_back(tmpBkgStack_MC);
    bkgStacks_LooseLoosePrediction.push_back(tmpBkgStack_LooseLoosePrediction);
    bkgStacks_TightLoosePrediction.push_back(tmpBkgStack_TightLoosePrediction);
    bkgDistributions_MC.push_back(tmpBkgDistributions_MC);
    bkgDistributions_LooseLoosePrediction.push_back(tmpBkgDistributions_LooseLoosePrediction);
    bkgDistributions_TightLoosePrediction.push_back(tmpBkgDistributions_TightLoosePrediction);
    bkgDistributions_LooseLoosePrediction_sysErrors.push_back(tmpBkgDistributions_LooseLoosePrediction_sysErrors);
    bkgDistributions_TightLoosePrediction_sysErrors.push_back(tmpBkgDistributions_TightLoosePrediction_sysErrors);
    legends.push_back(tmpLegend);
  }

   //*************************************************************************************************
  //Global Initialization for Event Yield
  //*************************************************************************************************
  Double_t TotalDataCount = 0.0;
  Double_t TotalDataCountErrSqr = 0.0;
  Double_t TotalSignalCount = 0.0;
  Double_t TotalSignalCountErrSqr = 0.0;
  Double_t TotalBkg = 0.0;
  Double_t TotalBkgErrSqr = 0.0;
  vector <Double_t> TotalBkgCount;
  vector <Double_t> TotalBkgCountLowErrSqr;
  vector <Double_t> TotalBkgCountHighErrSqr;
  vector <Double_t> maxEventweightLowErr;
  vector <Double_t> maxEventweightHighErr;
  for (int j=0;j<processes.size();++j) {
    TotalBkgCount.push_back(0.0);
    TotalBkgCountLowErrSqr.push_back(0.0);
    TotalBkgCountHighErrSqr.push_back(0.0);
    maxEventweightLowErr.push_back(0.0);
    maxEventweightHighErr.push_back(0.0);
  }    

  //********************************************************
  // Get Tree
  //********************************************************
  TTree* signalTree = getTreeFromFile(signalInputFilename.c_str(),"ZeeTree");
  assert(signalTree);
  MitZeeEvent *signalEvent = new MitZeeEvent(signalTree);

  TTree* bkgTree = getTreeFromFile(mcBkgInputFilename.c_str(),"ZeeTree");
  assert(bkgTree);
  MitZeeEvent *bkgEvent = new MitZeeEvent(bkgTree);

  TTree* fakePredictionBkgTree = getTreeFromFile(fakePredictionBkgInputFilename.c_str(),"ZeeTree");
  assert(fakePredictionBkgTree);
  MitZeeEvent *fakePredictionBkgEvent = new MitZeeEvent(fakePredictionBkgTree);

  TTree* dataTree = 0;
  MitZeeEvent *dataEvent = 0;  
  if (dataInputFilename != "") {
    dataTree = getTreeFromFile(dataInputFilename.c_str(),"ZeeTree");
    assert(dataTree);
    dataEvent = new MitZeeEvent(dataTree);
  }
  
  //*****************************************************************************************
  //Loop over Data Tree
  //*****************************************************************************************
  vector<double> RunNumber;
  vector<double> EventsInRun;

  if (dataInputFilename != "") {
    for(UInt_t n=0; n < dataTree->GetEntries() ; ++n) {
      dataEvent->GetEntry(n);
      if (n % 100000 == 0) cout << "Data Event " << n << endl;

      //don't count events beyond run 136100
      if (dataEvent->event_branch_runNumber > 136100) continue;

     //Find the Run
      Int_t runIndex = -1;
      for (int r=0;r < RunNumber.size(); ++r) {
        if (RunNumber[r] == dataEvent->event_branch_runNumber) {
          runIndex = r;
          EventsInRun[r]++;
          break;
        }
      }

      if (runIndex == -1) {
        RunNumber.push_back(dataEvent->event_branch_runNumber);
        EventsInRun.push_back(1);
      }

      float eventweight = dataEvent->event_branch_weight;
      Int_t nEle = dataEvent->nEle;    

      //skip events with less than 2 electrons.
      if (nEle < 2) continue;
      if (!passEventSelectionCuts(dataEvent)) continue;

 
      dataDistributions[0]->Fill(dataEvent->electronPt[0]);
      dataDistributions[1]->Fill(dataEvent->electronPt[1]);
      dataDistributions[2]->Fill(dataEvent->electronEta[0]);
      dataDistributions[3]->Fill(dataEvent->electronEta[1]);
      dataDistributions[4]->Fill(dataEvent->electronPhi[0]);
      dataDistributions[5]->Fill(dataEvent->electronPhi[1]);
      dataDistributions[6]->Fill(TMath::Sqrt(dataEvent->event_branch_caloMetX*dataEvent->event_branch_caloMetX + 
                                             dataEvent->event_branch_caloMetY*dataEvent->event_branch_caloMetY));
      dataDistributions[7]->Fill(TMath::Sqrt(dataEvent->event_branch_tcMetX*dataEvent->event_branch_tcMetX + 
                                             dataEvent->event_branch_tcMetY*dataEvent->event_branch_tcMetY));
      dataDistributions[8]->Fill(TMath::Sqrt(dataEvent->event_branch_pfMetX*dataEvent->event_branch_pfMetX + 
                                             dataEvent->event_branch_pfMetY*dataEvent->event_branch_pfMetY));

      //Loop over Electrons
      for (int i=0; i < nEle; ++i) {

        if (dataEvent->electronPt[i] < 10) continue;
        mithep::Electron *ele1 = new mithep::Electron;
        ele1->SetPtEtaPhi(dataEvent->electronPt[i], dataEvent->electronEta[i],dataEvent->electronPhi[i]);

        for (int j=i+1; j < nEle; ++j) {
          if (dataEvent->electronPt[j] < 10) continue;
          mithep::Electron *ele2 = new mithep::Electron;
          ele2->SetPtEtaPhi(dataEvent->electronPt[j], dataEvent->electronEta[j],dataEvent->electronPhi[j]);

          double tmpzmass = TMath::Sqrt((ele1->Mom() + ele2->Mom()).mag2());
          
          if (passDenominatorCuts(dataEvent, i) && passDenominatorCuts(dataEvent, j)
              && !passElectronCuts(dataEvent, i) && !passElectronCuts(dataEvent, j)
            ) {
            
            double fakeRate1 = fFakeRate->ElectronFakeRate(ele1->Pt(), ele1->Eta(), ele1->Phi()) / (1-fFakeRate->ElectronFakeRate(ele1->Pt(), ele1->Eta(), ele1->Phi()));
            double fakeRate1ErrorLow = fFakeRate->ElectronFakeRateErrorLow(ele1->Pt(), ele1->Eta(), ele1->Phi()) / pow((1- fFakeRate->ElectronFakeRate(ele1->Pt(), ele1->Eta(), ele1->Phi())),2);
            double fakeRate1ErrorHigh = fFakeRate->ElectronFakeRateErrorHigh(ele1->Pt(), ele1->Eta(), ele1->Phi()) / pow((1- fFakeRate->ElectronFakeRate(ele1->Pt(), ele1->Eta(), ele1->Phi())),2);
            double fakeRate2 = fFakeRate->ElectronFakeRate(ele2->Pt(), ele2->Eta(), ele2->Phi()) / (1- fFakeRate->ElectronFakeRate(ele2->Pt(), ele2->Eta(), ele2->Phi()));
            double fakeRate2ErrorLow = fFakeRate->ElectronFakeRateErrorLow(ele2->Pt(), ele2->Eta(), ele2->Phi()) / pow((1- fFakeRate->ElectronFakeRate(ele2->Pt(), ele2->Eta(), ele2->Phi())),2);
            double fakeRate2ErrorHigh = fFakeRate->ElectronFakeRateErrorHigh(ele2->Pt(), ele2->Eta(), ele2->Phi()) / pow((1- fFakeRate->ElectronFakeRate(ele2->Pt(), ele2->Eta(), ele2->Phi())),2);
            double fakeRate = fakeRate1*fakeRate2;
            double fakeRateErrorLow = (fakeRate > 0 ? fakeRate * TMath::Sqrt(pow(fakeRate1ErrorLow/fakeRate1,2) + pow(fakeRate2ErrorLow/fakeRate2,2)) : 0.0 );
            double fakeRateErrorHigh = (fakeRate > 0 ? fakeRate * TMath::Sqrt(pow(fakeRate1ErrorHigh/fakeRate1,2) + pow(fakeRate2ErrorHigh/fakeRate2,2)) : 0.0 );
            
            dataDistributions[9]->Fill(tmpzmass);
            dataDistributions_LooseLoosePrediction[11]->Fill(tmpzmass,fakeRate);
            dataDistributions_LooseLoosePrediction_sysErrors[11]->Fill(tmpzmass,(fakeRateErrorLow+fakeRateErrorHigh)/2);

//             cout << fakeRate1 << " " << fakeRate1ErrorLow << " " << fakeRate1ErrorHigh << " | " << fakeRate2 << " " << fakeRate2ErrorLow << " " << fakeRate2ErrorHigh << " ::: " << fakeRate << " " << fakeRateErrorLow << " " << fakeRateErrorHigh << endl;

            cout << fFakeRate->ElectronFakeRateStatErrorLow(ele1->Pt(), ele1->Eta(), ele1->Phi()) << " " 
                 << fFakeRate->ElectronFakeRateSysErrorLow(ele1->Pt(), ele1->Eta(), ele1->Phi()) << " "
                 << fFakeRate->ElectronFakeRateErrorLow(ele1->Pt(), ele1->Eta(), ele1->Phi()) << " "                  
                 << fFakeRate->ElectronFakeRateStatErrorHigh(ele1->Pt(), ele1->Eta(), ele1->Phi()) << " "
                 << fFakeRate->ElectronFakeRateSysErrorHigh(ele1->Pt(), ele1->Eta(), ele1->Phi()) << " "
                 << fFakeRate->ElectronFakeRateErrorHigh(ele1->Pt(), ele1->Eta(), ele1->Phi()) << " " 
                 << endl;


          }

          //apply electron cuts
          if (
            (passElectronCuts(dataEvent, i) && passDenominatorCuts(dataEvent, j) && !passElectronCuts(dataEvent, j)) ||
            (passElectronCuts(dataEvent, j) && passDenominatorCuts(dataEvent, i) && !passElectronCuts(dataEvent, i)) 
            ) {

            mithep::Electron *looseEle = (passElectronCuts(dataEvent, i) ? ele2 : ele1);

            double fakeRate2 = fFakeRate->ElectronFakeRate(looseEle->Pt(), looseEle->Eta(), looseEle->Phi()) / (1- fFakeRate->ElectronFakeRate(looseEle->Pt(), looseEle->Eta(), looseEle->Phi()));
            double fakeRate2ErrorLow = fFakeRate->ElectronFakeRateErrorLow(looseEle->Pt(), looseEle->Eta(), looseEle->Phi()) / pow((1- fFakeRate->ElectronFakeRate(looseEle->Pt(), looseEle->Eta(), looseEle->Phi())),2);
            double fakeRate2ErrorHigh = fFakeRate->ElectronFakeRateErrorHigh(looseEle->Pt(), looseEle->Eta(), looseEle->Phi()) / pow((1- fFakeRate->ElectronFakeRate(looseEle->Pt(), looseEle->Eta(), looseEle->Phi())),2);
            
            cout << fakeRate2 << " " << fakeRate2ErrorLow << " " << fakeRate2ErrorHigh << endl;


            dataDistributions[10]->Fill(tmpzmass);            
            dataDistributions_TightLoosePrediction[11]->Fill(tmpzmass,fakeRate2);
            dataDistributions_TightLoosePrediction_sysErrors[11]->Fill(tmpzmass,(fakeRate2ErrorLow+fakeRate2ErrorHigh)/2);
          }
        
          if (passElectronCuts(dataEvent, i) && passElectronCuts(dataEvent, j)) {
            dataDistributions[11]->Fill(tmpzmass);            
          }
          
          delete ele2;
        }
        delete ele1;
      }             
    } // event loop
  }

//   //Add Systematic Errors to the Fake Rate predictions
//   for (int v = 0; v < NVARIABLES;v++) {
//     TH1F *tmpHist = dataDistributions_LooseLoosePrediction[v];
//     TH1F *tmpHist_sysErrors = dataDistributions_LooseLoosePrediction_sysErrors[v];
    
//     //add systematics
//     for (int bin = 0; bin < tmpHist->GetNbinsX()+1; ++bin) {
//       Double_t totalError = TMath::Sqrt( pow(tmpHist->GetBinError(bin),2) +
//                                          pow(tmpHist_sysErrors->GetBinContent(bin), 2) );
//       cout << "bin " << bin << " : " << totalError << ", " << tmpHist->GetBinError(bin) << ", " << tmpHist_sysErrors->GetBinContent(bin) << endl;
//       if (TMath::IsNaN(tmpHist->GetBinContent(bin))) cout << "NAN!!\n";
//       tmpHist->SetBinError(bin, totalError);
//     }

//     tmpHist = dataDistributions_TightLoosePrediction[v];
//     tmpHist_sysErrors = dataDistributions_TightLoosePrediction_sysErrors[v];
    
//     //add systematics
//     for (int bin = 0; bin < tmpHist->GetNbinsX()+1; ++bin) {
//       Double_t totalError = TMath::Sqrt( pow(tmpHist->GetBinError(bin),2) +
//                                          pow(tmpHist_sysErrors->GetBinContent(bin), 2) );
//       cout << "bin " << bin << " : " << totalError << ", " << tmpHist->GetBinError(bin) << ", " << tmpHist_sysErrors->GetBinContent(bin) << endl;
//       if (TMath::IsNaN(tmpHist->GetBinContent(bin))) cout << "NAN!!\n";
//       tmpHist->SetBinError(bin, totalError);
//     }

//   }





//   cout << "Runs Analyzed\n";
//   //sort by run number first
//   int i, j, flag = 1;    // set flag to 1 to start first pass
//   int tempRun, tempNEvents;             // holding variable
//   for(i = 0; i < RunNumber.size() && flag; i++) {
//     flag = 0;
//     for (j=0; j < (RunNumber.size() -1); j++)
//     {
//       if (RunNumber[j+1] < RunNumber[j])      // ascending order simply changes to <
//       { 
//         tempRun = RunNumber[j];             // swap elements
//         RunNumber[j] = RunNumber[j+1];
//         RunNumber[j+1] = tempRun;
//         tempNEvents = EventsInRun[j];             // swap elements
//         EventsInRun[j] = EventsInRun[j+1];
//         EventsInRun[j+1] = tempNEvents;
//         flag = 1;               // indicates that a swap occurred.
//       }
//     }
//   }

//   double totalTriggers = 0;
//   for (int r=0;r < RunNumber.size(); ++r) {
//     cout << "Run : " << RunNumber[r] << " : " << EventsInRun[r] << endl;    
//     totalTriggers += EventsInRun[r];
//   }
//   cout << "Total: " << totalTriggers << endl;

 
  //*****************************************************************************************
  //Loop over SIGNAL Tree
  //*****************************************************************************************

  for(UInt_t n=0; n < signalTree->GetEntries() ; ++n) {
    signalEvent->GetEntry(n);
    if (n % 100000 == 0) cout << "MC Signal Event " << n << endl;


    float eventweight = signalEvent->event_branch_weight * LUMINOSITY;
    Int_t nEle = signalEvent->nEle;    

    //skip events with less than 2 electrons.
    if (nEle < 2) continue;
    if (!passEventSelectionCuts(signalEvent)) continue;

    signalDistributions[0]->Fill(signalEvent->electronPt[0],eventweight);
    signalDistributions[1]->Fill(signalEvent->electronPt[1],eventweight);
    signalDistributions[2]->Fill(signalEvent->electronEta[0],eventweight);
    signalDistributions[3]->Fill(signalEvent->electronEta[1],eventweight);
    signalDistributions[4]->Fill(signalEvent->electronPhi[0],eventweight);
    signalDistributions[5]->Fill(signalEvent->electronPhi[1],eventweight);
    signalDistributions[6]->Fill(TMath::Sqrt(signalEvent->event_branch_caloMetX*signalEvent->event_branch_caloMetX + 
                                             signalEvent->event_branch_caloMetY*signalEvent->event_branch_caloMetY),eventweight);
    signalDistributions[7]->Fill(TMath::Sqrt(signalEvent->event_branch_tcMetX*signalEvent->event_branch_tcMetX + 
                                             signalEvent->event_branch_tcMetY*signalEvent->event_branch_tcMetY),eventweight);
    signalDistributions[8]->Fill(TMath::Sqrt(signalEvent->event_branch_pfMetX*signalEvent->event_branch_pfMetX + 
                                             signalEvent->event_branch_pfMetY*signalEvent->event_branch_pfMetY),eventweight);
    
    //Loop over Electrons
    for (int i=0; i < nEle; ++i) {
      
      if (signalEvent->electronPt[i] < 10) continue;
      mithep::Electron *ele1 = new mithep::Electron;
      ele1->SetPtEtaPhi(signalEvent->electronPt[i], signalEvent->electronEta[i],signalEvent->electronPhi[i]);
      
      for (int j=i+1; j < nEle; ++j) {
        if (signalEvent->electronPt[j] < 10) continue;
        mithep::Electron *ele2 = new mithep::Electron;
        ele2->SetPtEtaPhi(signalEvent->electronPt[j], signalEvent->electronEta[j],signalEvent->electronPhi[j]);
        

        double tmpzmass = TMath::Sqrt((ele1->Mom() + ele2->Mom()).mag2());
        signalDistributions[9]->Fill(tmpzmass,eventweight);
       

        if (passDenominatorCuts(signalEvent, i) && passDenominatorCuts(signalEvent, j)
            && !passElectronCuts(signalEvent, i) && !passElectronCuts(signalEvent, j)
          ) {
           
        double fakeRate1 = fFakeRate->ElectronFakeRate(ele1->Pt(), ele1->Eta(), ele1->Phi()) / (1-fFakeRate->ElectronFakeRate(ele1->Pt(), ele1->Eta(), ele1->Phi()));
        double fakeRate1ErrorLow = fFakeRate->ElectronFakeRateErrorLow(ele1->Pt(), ele1->Eta(), ele1->Phi()) / pow((1- fFakeRate->ElectronFakeRate(ele1->Pt(), ele1->Eta(), ele1->Phi())),2);
        double fakeRate1ErrorHigh = fFakeRate->ElectronFakeRateErrorHigh(ele1->Pt(), ele1->Eta(), ele1->Phi()) / pow((1- fFakeRate->ElectronFakeRate(ele1->Pt(), ele1->Eta(), ele1->Phi())),2);
        double fakeRate2 = fFakeRate->ElectronFakeRate(ele2->Pt(), ele2->Eta(), ele2->Phi()) / (1- fFakeRate->ElectronFakeRate(ele2->Pt(), ele2->Eta(), ele2->Phi()));
        double fakeRate2ErrorLow = fFakeRate->ElectronFakeRateErrorLow(ele2->Pt(), ele2->Eta(), ele2->Phi()) / pow((1- fFakeRate->ElectronFakeRate(ele2->Pt(), ele2->Eta(), ele2->Phi())),2);
        double fakeRate2ErrorHigh = fFakeRate->ElectronFakeRateErrorHigh(ele2->Pt(), ele2->Eta(), ele2->Phi()) / pow((1- fFakeRate->ElectronFakeRate(ele2->Pt(), ele2->Eta(), ele2->Phi())),2);
        double fakeRate = fakeRate1*fakeRate2;
        double fakeRateErrorLow = (fakeRate > 0 ? fakeRate * TMath::Sqrt(pow(fakeRate1ErrorLow/fakeRate1,2) + pow(fakeRate2ErrorLow/fakeRate2,2)) : 0.0 );
        double fakeRateErrorHigh = (fakeRate > 0 ? fakeRate * TMath::Sqrt(pow(fakeRate1ErrorHigh/fakeRate1,2) + pow(fakeRate2ErrorHigh/fakeRate2,2)) : 0.0 );
        
        
        signalDistributions_LooseLoosePrediction[9]->Fill(tmpzmass,eventweight);
        signalDistributions_LooseLoosePrediction[11]->Fill(tmpzmass,eventweight*fakeRate);
        signalDistributions_LooseLoosePrediction_sysErrors[11]->Fill(tmpzmass,eventweight*(fakeRateErrorLow+fakeRateErrorHigh)/2);
        }

        //apply electron cuts
        if (
          (passElectronCuts(signalEvent, i) && passDenominatorCuts(signalEvent, j) && !passElectronCuts(signalEvent, j)) ||
          (passElectronCuts(signalEvent, j) && passDenominatorCuts(signalEvent, i) && !passElectronCuts(signalEvent, i)) 
          ) {
          
          mithep::Electron *looseEle = (passElectronCuts(signalEvent, i) ? ele2 : ele1);
          
          double fakeRate2 = fFakeRate->ElectronFakeRate(looseEle->Pt(), looseEle->Eta(), looseEle->Phi()) / (1- fFakeRate->ElectronFakeRate(looseEle->Pt(), looseEle->Eta(), looseEle->Phi()));
          double fakeRate2ErrorLow = fFakeRate->ElectronFakeRateErrorLow(looseEle->Pt(), looseEle->Eta(), looseEle->Phi()) / pow((1- fFakeRate->ElectronFakeRate(looseEle->Pt(), looseEle->Eta(), looseEle->Phi())),2);
          double fakeRate2ErrorHigh = fFakeRate->ElectronFakeRateErrorHigh(looseEle->Pt(), looseEle->Eta(), looseEle->Phi()) / pow((1- fFakeRate->ElectronFakeRate(looseEle->Pt(), looseEle->Eta(), looseEle->Phi())),2);
          
          signalDistributions[10]->Fill(tmpzmass,eventweight);
          signalDistributions_TightLoosePrediction[10]->Fill(tmpzmass,eventweight);
          signalDistributions_TightLoosePrediction[11]->Fill(tmpzmass,eventweight*fakeRate2);
          signalDistributions_TightLoosePrediction_sysErrors[11]->Fill(tmpzmass,eventweight*(fakeRate2ErrorLow+fakeRate2ErrorHigh)/2);
        }
        
        if (passElectronCuts(signalEvent, i) && passElectronCuts(signalEvent, j)) {
          signalDistributions[11]->Fill(tmpzmass,eventweight);
          
          if (tmpzmass > 70 && tmpzmass < 110) {
            TotalSignalCount += eventweight;
            TotalSignalCountErrSqr += eventweight;
          }
        }
        
        delete ele2;
      }
      delete ele1;
    }
    
  } // event loop



  //*****************************************************************************************
  //Loop over Fake Prediction Bkg Tree
  //*****************************************************************************************  
  cout << "Fake Prediction BKG Events: " << fakePredictionBkgTree->GetEntries() << endl;
  for(UInt_t n=0; n < fakePredictionBkgTree->GetEntries() ; ++n) {
    fakePredictionBkgEvent->GetEntry(n);

    if (n % 100000 == 0) cout << "BKG Event " << n << endl;

    float eventweight = fakePredictionBkgEvent->event_branch_weight * LUMINOSITY;
    Int_t nEle = fakePredictionBkgEvent->nEle;    


    //skip events with less than 2 electrons.
    if (nEle < 2) continue;
    if (!passEventSelectionCuts(fakePredictionBkgEvent)) continue;

    //Search for the bkg process to see if it already exists
    int sampleType = (int)fakePredictionBkgEvent->event_branch_sampleType;
    bool foundProcess = false;
    int foundIndex = -1;      
    for (UInt_t p=0;p<processes.size();++p) {
      for (UInt_t gg=0;gg<processes[p].size();++gg) {
        if (sampleType == processes[p][gg]) {
          foundProcess = true;
          foundIndex = p;
        }
      }
    }

    if (foundIndex == -1) {
      cerr << "process " << sampleType << " not found in list.\n";
    } else {
      //compute event weight errors
      Double_t tmpEventweightErrLow = eventweight + 
        fakePredictionBkgEvent->event_branch_weightErrorLow*fakePredictionBkgEvent->event_branch_weightErrorLow;
      Double_t tmpEventweightErrHigh = eventweight + 
        fakePredictionBkgEvent->event_branch_weightErrorHigh*fakePredictionBkgEvent->event_branch_weightErrorHigh;
      if (maxEventweightLowErr[foundIndex] < tmpEventweightErrLow) {
        maxEventweightLowErr[foundIndex] = tmpEventweightErrLow;
      }
      if (maxEventweightHighErr[foundIndex] < tmpEventweightErrHigh) {
        maxEventweightHighErr[foundIndex] = tmpEventweightErrHigh;
      }    

      
     
      //Loop over Electrons
      for (int i=0; i < nEle; ++i) {
      
        if (fakePredictionBkgEvent->electronPt[i] < 10) continue;
        mithep::Electron *ele1 = new mithep::Electron;
        ele1->SetPtEtaPhi(fakePredictionBkgEvent->electronPt[i], fakePredictionBkgEvent->electronEta[i],fakePredictionBkgEvent->electronPhi[i]);

        for (int j=i+1; j < nEle; ++j) {
          if (fakePredictionBkgEvent->electronPt[j] < 10) continue;
          mithep::Electron *ele2 = new mithep::Electron;
          ele2->SetPtEtaPhi(fakePredictionBkgEvent->electronPt[j], fakePredictionBkgEvent->electronEta[j],fakePredictionBkgEvent->electronPhi[j]);

          double tmpzmass = TMath::Sqrt((ele1->Mom() + ele2->Mom()).mag2());

          if (!(passDenominatorCuts(fakePredictionBkgEvent, i) && passDenominatorCuts(fakePredictionBkgEvent, j)     
                && !passElectronCuts(dataEvent, i) && !passElectronCuts(dataEvent, j)            
                )) {
          

            double fakeRate1 = fMCFakeRate->ElectronFakeRate(ele1->Pt(), ele1->Eta(), ele1->Phi()) / (1-fMCFakeRate->ElectronFakeRate(ele1->Pt(), ele1->Eta(), ele1->Phi()));
            double fakeRate1ErrorLow = fMCFakeRate->ElectronFakeRateErrorLow(ele1->Pt(), ele1->Eta(), ele1->Phi()) / pow((1- fMCFakeRate->ElectronFakeRate(ele1->Pt(), ele1->Eta(), ele1->Phi())),2);
            double fakeRate1ErrorHigh = fMCFakeRate->ElectronFakeRateErrorHigh(ele1->Pt(), ele1->Eta(), ele1->Phi()) / pow((1- fMCFakeRate->ElectronFakeRate(ele1->Pt(), ele1->Eta(), ele1->Phi())),2);
            double fakeRate2 = fMCFakeRate->ElectronFakeRate(ele2->Pt(), ele2->Eta(), ele2->Phi()) / (1- fMCFakeRate->ElectronFakeRate(ele2->Pt(), ele2->Eta(), ele2->Phi()));
            double fakeRate2ErrorLow = fMCFakeRate->ElectronFakeRateErrorLow(ele2->Pt(), ele2->Eta(), ele2->Phi()) / pow((1- fMCFakeRate->ElectronFakeRate(ele2->Pt(), ele2->Eta(), ele2->Phi())),2);
            double fakeRate2ErrorHigh = fMCFakeRate->ElectronFakeRateErrorHigh(ele2->Pt(), ele2->Eta(), ele2->Phi()) / pow((1- fMCFakeRate->ElectronFakeRate(ele2->Pt(), ele2->Eta(), ele2->Phi())),2);
            double fakeRate = fakeRate1*fakeRate2;
            double fakeRateErrorLow = (fakeRate > 0 ? fakeRate * TMath::Sqrt(pow(fakeRate1ErrorLow/fakeRate1,2) + pow(fakeRate2ErrorLow/fakeRate2,2)) : 0.0 );
            double fakeRateErrorHigh = (fakeRate > 0 ? fakeRate * TMath::Sqrt(pow(fakeRate1ErrorHigh/fakeRate1,2) + pow(fakeRate2ErrorHigh/fakeRate2,2)) : 0.0 );
            
            
            bkgDistributions_LooseLoosePrediction[0][foundIndex]->Fill(fakePredictionBkgEvent->electronPt[0], eventweight*fakeRate);
            bkgDistributions_LooseLoosePrediction[1][foundIndex]->Fill(fakePredictionBkgEvent->electronPt[1], eventweight*fakeRate);
            bkgDistributions_LooseLoosePrediction[2][foundIndex]->Fill(fakePredictionBkgEvent->electronEta[0], eventweight*fakeRate);
            bkgDistributions_LooseLoosePrediction[3][foundIndex]->Fill(fakePredictionBkgEvent->electronEta[1], eventweight*fakeRate);
            bkgDistributions_LooseLoosePrediction[4][foundIndex]->Fill(fakePredictionBkgEvent->electronPhi[0], eventweight*fakeRate);
            bkgDistributions_LooseLoosePrediction[5][foundIndex]->Fill(fakePredictionBkgEvent->electronPhi[1], eventweight*fakeRate);
            bkgDistributions_LooseLoosePrediction[6][foundIndex]->Fill(TMath::Sqrt(fakePredictionBkgEvent->event_branch_caloMetX*fakePredictionBkgEvent->event_branch_caloMetX + 
                                                                                   fakePredictionBkgEvent->event_branch_caloMetY*fakePredictionBkgEvent->event_branch_caloMetY), eventweight*fakeRate);
            bkgDistributions_LooseLoosePrediction[7][foundIndex]->Fill(TMath::Sqrt(fakePredictionBkgEvent->event_branch_tcMetX*fakePredictionBkgEvent->event_branch_tcMetX + 
                                                                                   fakePredictionBkgEvent->event_branch_tcMetY*fakePredictionBkgEvent->event_branch_tcMetY), eventweight*fakeRate);
            bkgDistributions_LooseLoosePrediction[8][foundIndex]->Fill(TMath::Sqrt(fakePredictionBkgEvent->event_branch_pfMetX*fakePredictionBkgEvent->event_branch_pfMetX + 
                                                                                   fakePredictionBkgEvent->event_branch_pfMetY*fakePredictionBkgEvent->event_branch_pfMetY), eventweight*fakeRate);

            totalBkgDistributions_LooseLoosePrediction[0]->Fill(fakePredictionBkgEvent->electronPt[0], eventweight*fakeRate);
            totalBkgDistributions_LooseLoosePrediction[1]->Fill(fakePredictionBkgEvent->electronPt[1], eventweight*fakeRate);
            totalBkgDistributions_LooseLoosePrediction[2]->Fill(fakePredictionBkgEvent->electronEta[0], eventweight*fakeRate);
            totalBkgDistributions_LooseLoosePrediction[3]->Fill(fakePredictionBkgEvent->electronEta[1], eventweight*fakeRate);
            totalBkgDistributions_LooseLoosePrediction[4]->Fill(fakePredictionBkgEvent->electronPhi[0], eventweight*fakeRate);
            totalBkgDistributions_LooseLoosePrediction[5]->Fill(fakePredictionBkgEvent->electronPhi[1], eventweight*fakeRate);
            totalBkgDistributions_LooseLoosePrediction[6]->Fill(TMath::Sqrt(fakePredictionBkgEvent->event_branch_caloMetX*fakePredictionBkgEvent->event_branch_caloMetX + 
                                                                            fakePredictionBkgEvent->event_branch_caloMetY*fakePredictionBkgEvent->event_branch_caloMetY), eventweight*fakeRate);
            totalBkgDistributions_LooseLoosePrediction[7]->Fill(TMath::Sqrt(fakePredictionBkgEvent->event_branch_tcMetX*fakePredictionBkgEvent->event_branch_tcMetX + 
                                                                            fakePredictionBkgEvent->event_branch_tcMetY*fakePredictionBkgEvent->event_branch_tcMetY), eventweight*fakeRate);
            totalBkgDistributions_LooseLoosePrediction[8]->Fill(TMath::Sqrt(fakePredictionBkgEvent->event_branch_pfMetX*fakePredictionBkgEvent->event_branch_pfMetX + 
                                                                            fakePredictionBkgEvent->event_branch_pfMetY*fakePredictionBkgEvent->event_branch_pfMetY), eventweight*fakeRate);
 
            bkgDistributions_LooseLoosePrediction[9][foundIndex]->Fill(tmpzmass, eventweight);
            totalBkgDistributions_LooseLoosePrediction[9]->Fill(tmpzmass, eventweight);           
            bkgDistributions_LooseLoosePrediction[11][foundIndex]->Fill(tmpzmass, eventweight*fakeRate);
            bkgDistributions_LooseLoosePrediction_sysErrors[11][foundIndex]->Fill(tmpzmass, eventweight*(fakeRateErrorLow + fakeRateErrorHigh)/2);
            totalBkgDistributions_LooseLoosePrediction[11]->Fill(tmpzmass, eventweight*fakeRate);
            totalBkgDistributions_LooseLoosePrediction_sysErrors[11]->Fill(tmpzmass, eventweight*fakeRate*(fakeRateErrorLow + fakeRateErrorHigh)/2);
            
          }

          //apply electron cuts
          if (
            (passElectronCuts(fakePredictionBkgEvent, i) && passDenominatorCuts(fakePredictionBkgEvent, j) && !passElectronCuts(fakePredictionBkgEvent, j))  ||
            (passElectronCuts(fakePredictionBkgEvent, j) && passDenominatorCuts(fakePredictionBkgEvent, i) && !passElectronCuts(dataEvent, i))
            ) {

            double fakeRate2 = fMCFakeRate->ElectronFakeRate(ele2->Pt(), ele2->Eta(), ele2->Phi()) / (1- fMCFakeRate->ElectronFakeRate(ele2->Pt(), ele2->Eta(), ele2->Phi()));
            double fakeRate2ErrorLow = fMCFakeRate->ElectronFakeRateErrorLow(ele2->Pt(), ele2->Eta(), ele2->Phi()) / pow((1- fMCFakeRate->ElectronFakeRate(ele2->Pt(), ele2->Eta(), ele2->Phi())),2);
            double fakeRate2ErrorHigh = fMCFakeRate->ElectronFakeRateErrorHigh(ele2->Pt(), ele2->Eta(), ele2->Phi()) / pow((1- fMCFakeRate->ElectronFakeRate(ele2->Pt(), ele2->Eta(), ele2->Phi())),2);
            


            bkgDistributions_TightLoosePrediction[0][foundIndex]->Fill(fakePredictionBkgEvent->electronPt[0], eventweight*fakeRate2);
            bkgDistributions_TightLoosePrediction[1][foundIndex]->Fill(fakePredictionBkgEvent->electronPt[1], eventweight*fakeRate2);
            bkgDistributions_TightLoosePrediction[2][foundIndex]->Fill(fakePredictionBkgEvent->electronEta[0], eventweight*fakeRate2);
            bkgDistributions_TightLoosePrediction[3][foundIndex]->Fill(fakePredictionBkgEvent->electronEta[1], eventweight*fakeRate2);
            bkgDistributions_TightLoosePrediction[4][foundIndex]->Fill(fakePredictionBkgEvent->electronPhi[0], eventweight*fakeRate2);
            bkgDistributions_TightLoosePrediction[5][foundIndex]->Fill(fakePredictionBkgEvent->electronPhi[1], eventweight*fakeRate2);
            bkgDistributions_TightLoosePrediction[6][foundIndex]->Fill(TMath::Sqrt(fakePredictionBkgEvent->event_branch_caloMetX*fakePredictionBkgEvent->event_branch_caloMetX + 
                                                                                   fakePredictionBkgEvent->event_branch_caloMetY*fakePredictionBkgEvent->event_branch_caloMetY), eventweight*fakeRate2);
            bkgDistributions_TightLoosePrediction[7][foundIndex]->Fill(TMath::Sqrt(fakePredictionBkgEvent->event_branch_tcMetX*fakePredictionBkgEvent->event_branch_tcMetX + 
                                                                                   fakePredictionBkgEvent->event_branch_tcMetY*fakePredictionBkgEvent->event_branch_tcMetY), eventweight*fakeRate2);
            bkgDistributions_TightLoosePrediction[8][foundIndex]->Fill(TMath::Sqrt(fakePredictionBkgEvent->event_branch_pfMetX*fakePredictionBkgEvent->event_branch_pfMetX + 
                                                                                   fakePredictionBkgEvent->event_branch_pfMetY*fakePredictionBkgEvent->event_branch_pfMetY), eventweight*fakeRate2);
            
            totalBkgDistributions_TightLoosePrediction[0]->Fill(fakePredictionBkgEvent->electronPt[0], eventweight*fakeRate2);
            totalBkgDistributions_TightLoosePrediction[1]->Fill(fakePredictionBkgEvent->electronPt[1], eventweight*fakeRate2);
            totalBkgDistributions_TightLoosePrediction[2]->Fill(fakePredictionBkgEvent->electronEta[0], eventweight*fakeRate2);
            totalBkgDistributions_TightLoosePrediction[3]->Fill(fakePredictionBkgEvent->electronEta[1], eventweight*fakeRate2);
            totalBkgDistributions_TightLoosePrediction[4]->Fill(fakePredictionBkgEvent->electronPhi[0], eventweight*fakeRate2);
            totalBkgDistributions_TightLoosePrediction[5]->Fill(fakePredictionBkgEvent->electronPhi[1], eventweight*fakeRate2);
            totalBkgDistributions_TightLoosePrediction[6]->Fill(TMath::Sqrt(fakePredictionBkgEvent->event_branch_caloMetX*fakePredictionBkgEvent->event_branch_caloMetX + 
                                                                            fakePredictionBkgEvent->event_branch_caloMetY*fakePredictionBkgEvent->event_branch_caloMetY), eventweight*fakeRate2);
            totalBkgDistributions_TightLoosePrediction[7]->Fill(TMath::Sqrt(fakePredictionBkgEvent->event_branch_tcMetX*fakePredictionBkgEvent->event_branch_tcMetX + 
                                                                            fakePredictionBkgEvent->event_branch_tcMetY*fakePredictionBkgEvent->event_branch_tcMetY), eventweight*fakeRate2);
            totalBkgDistributions_TightLoosePrediction[8]->Fill(TMath::Sqrt(fakePredictionBkgEvent->event_branch_pfMetX*fakePredictionBkgEvent->event_branch_pfMetX + 
                                                                            fakePredictionBkgEvent->event_branch_pfMetY*fakePredictionBkgEvent->event_branch_pfMetY), eventweight*fakeRate2);
            
            bkgDistributions_TightLoosePrediction[10][foundIndex]->Fill(tmpzmass, eventweight);            
            totalBkgDistributions_TightLoosePrediction[10]->Fill(tmpzmass, eventweight);
            bkgDistributions_TightLoosePrediction[11][foundIndex]->Fill(tmpzmass, eventweight*fakeRate2);
            totalBkgDistributions_TightLoosePrediction[11]->Fill(tmpzmass, eventweight*fakeRate2);
            bkgDistributions_TightLoosePrediction_sysErrors[11][foundIndex]->Fill(tmpzmass, eventweight*(fakeRate2ErrorLow+fakeRate2ErrorHigh)/2);
            totalBkgDistributions_TightLoosePrediction_sysErrors[11]->Fill(tmpzmass, eventweight*(fakeRate2ErrorLow+fakeRate2ErrorHigh)/2);

          }

          if (passElectronCuts(fakePredictionBkgEvent, i) && passElectronCuts(fakePredictionBkgEvent, j)) {

          }
          delete ele2;
        }
        delete ele1;
      }           
    }

  } // event loop


  //Add Systematic Errors to the Fake Rate predictions

  for (int v = 0; v < NVARIABLES;v++) {

    TH1F *tmpHist = totalBkgDistributions_LooseLoosePrediction[v];
    TH1F *tmpHist_sysErrors = totalBkgDistributions_LooseLoosePrediction_sysErrors[v];
    
    //add systematics
    for (int bin = 0; bin < tmpHist->GetNbinsX()+1; ++bin) {
      Double_t totalError = TMath::Sqrt( pow(tmpHist->GetBinError(bin),2) +
                                         pow(tmpHist_sysErrors->GetBinContent(bin), 2) );
      cout << "bin " << bin << " : " << totalError << ", " << tmpHist->GetBinError(bin) << ", " << tmpHist_sysErrors->GetBinContent(bin) << endl;
      if (TMath::IsNaN(tmpHist->GetBinContent(bin))) cout << "NAN!!\n";
      tmpHist->SetBinError(bin, totalError);
    }
    
    tmpHist = totalBkgDistributions_TightLoosePrediction[v];
    tmpHist_sysErrors = totalBkgDistributions_TightLoosePrediction_sysErrors[v];
    
    //add systematics
    for (int bin = 0; bin < tmpHist->GetNbinsX()+1; ++bin) {
      Double_t totalError = TMath::Sqrt( pow(tmpHist->GetBinError(bin),2) +
                                         pow(tmpHist_sysErrors->GetBinContent(bin), 2) );
      cout << "bin " << bin << " : " << totalError << ", " << tmpHist->GetBinError(bin) << ", " << tmpHist_sysErrors->GetBinContent(bin) << endl;
      if (TMath::IsNaN(tmpHist->GetBinContent(bin))) cout << "NAN!!\n";
      tmpHist->SetBinError(bin, totalError);
    }
    
    
    for (int p=0; p < processes.size(); ++p) {
      tmpHist = bkgDistributions_LooseLoosePrediction[v][p];
      tmpHist_sysErrors = bkgDistributions_LooseLoosePrediction_sysErrors[v][p];
      
      //add systematics
      for (int bin = 0; bin < tmpHist->GetNbinsX()+1; ++bin) {
        Double_t totalError = TMath::Sqrt( pow(tmpHist->GetBinError(bin),2) +
                                           pow(tmpHist_sysErrors->GetBinContent(bin), 2) );
        cout << "bin " << bin << " : " << totalError << ", " << tmpHist->GetBinError(bin) << ", " << tmpHist_sysErrors->GetBinContent(bin) << endl;
        if (TMath::IsNaN(tmpHist->GetBinContent(bin))) cout << "NAN!!\n";
        tmpHist->SetBinError(bin, totalError);
      }
      
      tmpHist = bkgDistributions_TightLoosePrediction[v][p];
      tmpHist_sysErrors = bkgDistributions_TightLoosePrediction_sysErrors[v][p];
      
      //add systematics
      for (int bin = 0; bin < tmpHist->GetNbinsX()+1; ++bin) {
        Double_t totalError = TMath::Sqrt( pow(tmpHist->GetBinError(bin),2) +
                                           pow(tmpHist_sysErrors->GetBinContent(bin), 2) );
        cout << "bin " << bin << " : " << totalError << ", " << tmpHist->GetBinError(bin) << ", " << tmpHist_sysErrors->GetBinContent(bin) << endl;
        if (TMath::IsNaN(tmpHist->GetBinContent(bin))) cout << "NAN!!\n";
        tmpHist->SetBinError(bin, totalError);
      }      
    }
  }



  //*****************************************************************************************
  //Loop over Bkg Tree
  //*****************************************************************************************  
  cout << "Total Bkg Events: " << bkgTree->GetEntries() << endl;
  for(UInt_t n=0; n < bkgTree->GetEntries() ; ++n) {
//     if (n < 3700000) continue;
//     cout << "start1\n";   
    bkgEvent->GetEntry(n);
//     cout << "start2\n";   

    if (n % 100000 == 0) cout << "BKG Event " << n << endl;

    float eventweight = bkgEvent->event_branch_weight * LUMINOSITY;
    Int_t nEle = bkgEvent->nEle;    


    //skip events with less than 2 electrons.
    if (nEle < 2) continue;
    if (!passEventSelectionCuts(bkgEvent)) continue;

    //Search for the bkg process to see if it already exists
    int sampleType = (int)bkgEvent->event_branch_sampleType;
    bool foundProcess = false;
    int foundIndex = -1;      
    for (UInt_t p=0;p<processes.size();++p) {
      for (UInt_t gg=0;gg<processes[p].size();++gg) {
        if (sampleType == processes[p][gg]) {
          foundProcess = true;
          foundIndex = p;
        }
      }
    }

//     cout << "here1\n";

    if (foundIndex == -1) {
      cerr << "process " << sampleType << " not found in list.\n";
    } else {
//       cout << "here2 " << sampleType << "\n";

      //compute event weight errors
      Double_t tmpEventweightErrLow = eventweight + 
        bkgEvent->event_branch_weightErrorLow*bkgEvent->event_branch_weightErrorLow;
      Double_t tmpEventweightErrHigh = eventweight + 
        bkgEvent->event_branch_weightErrorHigh*bkgEvent->event_branch_weightErrorHigh;
      if (maxEventweightLowErr[foundIndex] < tmpEventweightErrLow) {
        maxEventweightLowErr[foundIndex] = tmpEventweightErrLow;
      }
      if (maxEventweightHighErr[foundIndex] < tmpEventweightErrHigh) {
        maxEventweightHighErr[foundIndex] = tmpEventweightErrHigh;
      }    

      bkgDistributions_MC[0][foundIndex]->Fill(bkgEvent->electronPt[0], eventweight);
      bkgDistributions_MC[1][foundIndex]->Fill(bkgEvent->electronPt[1], eventweight);
      bkgDistributions_MC[2][foundIndex]->Fill(bkgEvent->electronEta[0], eventweight);
      bkgDistributions_MC[3][foundIndex]->Fill(bkgEvent->electronEta[1], eventweight);
      bkgDistributions_MC[4][foundIndex]->Fill(bkgEvent->electronPhi[0], eventweight);
      bkgDistributions_MC[5][foundIndex]->Fill(bkgEvent->electronPhi[1], eventweight);
      bkgDistributions_MC[6][foundIndex]->Fill(TMath::Sqrt(bkgEvent->event_branch_caloMetX*bkgEvent->event_branch_caloMetX + 
                                                        bkgEvent->event_branch_caloMetY*bkgEvent->event_branch_caloMetY), eventweight);
      bkgDistributions_MC[7][foundIndex]->Fill(TMath::Sqrt(bkgEvent->event_branch_tcMetX*bkgEvent->event_branch_tcMetX + 
                                                        bkgEvent->event_branch_tcMetY*bkgEvent->event_branch_tcMetY), eventweight);
      bkgDistributions_MC[8][foundIndex]->Fill(TMath::Sqrt(bkgEvent->event_branch_pfMetX*bkgEvent->event_branch_pfMetX + 
                                                        bkgEvent->event_branch_pfMetY*bkgEvent->event_branch_pfMetY), eventweight);
      
      totalBkgDistributions_MC[0]->Fill(bkgEvent->electronPt[0], eventweight);
      totalBkgDistributions_MC[1]->Fill(bkgEvent->electronPt[1], eventweight);
      totalBkgDistributions_MC[2]->Fill(bkgEvent->electronEta[0], eventweight);
      totalBkgDistributions_MC[3]->Fill(bkgEvent->electronEta[1], eventweight);
      totalBkgDistributions_MC[4]->Fill(bkgEvent->electronPhi[0], eventweight);
      totalBkgDistributions_MC[5]->Fill(bkgEvent->electronPhi[1], eventweight);
      totalBkgDistributions_MC[6]->Fill(TMath::Sqrt(bkgEvent->event_branch_caloMetX*bkgEvent->event_branch_caloMetX + 
                                                        bkgEvent->event_branch_caloMetY*bkgEvent->event_branch_caloMetY), eventweight);
      totalBkgDistributions_MC[7]->Fill(TMath::Sqrt(bkgEvent->event_branch_tcMetX*bkgEvent->event_branch_tcMetX + 
                                                        bkgEvent->event_branch_tcMetY*bkgEvent->event_branch_tcMetY), eventweight);
      totalBkgDistributions_MC[8]->Fill(TMath::Sqrt(bkgEvent->event_branch_pfMetX*bkgEvent->event_branch_pfMetX + 
                                                        bkgEvent->event_branch_pfMetY*bkgEvent->event_branch_pfMetY), eventweight);
//     cout << "here3\n";

      //Loop over Electrons
      for (int i=0; i < nEle; ++i) {
      
        if (bkgEvent->electronPt[i] < 10) continue;
        mithep::Electron *ele1 = new mithep::Electron;
        ele1->SetPtEtaPhi(bkgEvent->electronPt[i], bkgEvent->electronEta[i],bkgEvent->electronPhi[i]);

        for (int j=i+1; j < nEle; ++j) {
          if (bkgEvent->electronPt[j] < 10) continue;
          mithep::Electron *ele2 = new mithep::Electron;
          ele2->SetPtEtaPhi(bkgEvent->electronPt[j], bkgEvent->electronEta[j],bkgEvent->electronPhi[j]);

          double tmpzmass = TMath::Sqrt((ele1->Mom() + ele2->Mom()).mag2());

//               cout << "here41\n";

          bkgDistributions_MC[9][foundIndex]->Fill(tmpzmass, eventweight);
//             cout << foundIndex << " " << tmpzmass << " " << eventweight << endl;

          totalBkgDistributions_MC[9]->Fill(tmpzmass, eventweight);

          //apply electron cuts
          if (
            (passElectronCuts(bkgEvent, i) && passDenominatorCuts(bkgEvent, j)) 
            ) {

            bkgDistributions_MC[10][foundIndex]->Fill(tmpzmass, eventweight);
            totalBkgDistributions_MC[10]->Fill(tmpzmass, eventweight);
          }
//               cout << "here42\n";

          if (passElectronCuts(bkgEvent, i) && passElectronCuts(bkgEvent, j)) {
 //            cout << "here43\n";

            bkgDistributions_MC[11][foundIndex]->Fill(tmpzmass, eventweight);
 //            cout << "here44\n";
            totalBkgDistributions_MC[11]->Fill(tmpzmass, eventweight);


           if (tmpzmass > 70 && tmpzmass < 110) {
              TotalBkg += eventweight;
              TotalBkgErrSqr += eventweight;
              if (!foundProcess) {
                //cerr << "process " << sampleType << " not found in list.\n";
              } else {       
              
                TotalBkgCount[foundIndex] += eventweight;
                TotalBkgCountLowErrSqr[foundIndex] += eventweight + 
                  bkgEvent->event_branch_weightErrorLow*bkgEvent->event_branch_weightErrorLow;
                TotalBkgCountHighErrSqr[foundIndex] += eventweight + 
                  bkgEvent->event_branch_weightErrorHigh*bkgEvent->event_branch_weightErrorHigh;
              }
            }
 //           cout << "here49\n";

          }
//           cout << "here5\n";
          delete ele2;
        } //loop over ele2
//           cout << "here6\n";
        delete ele1;
      } //loop over ele1
//       cout << "here7\n";   
    }
//     cout << "here8\n";   
  } // event loop



  cout << "done\n";

  //Fix asym errors
  for (int j=0;j<processes.size();++j) {
    if (TotalBkgCountLowErrSqr[j] == 0) {
      TotalBkgCountLowErrSqr[j] = maxEventweightLowErr[j];
    }
    if (TotalBkgCountHighErrSqr[j] == 0) {
      TotalBkgCountHighErrSqr[j] = maxEventweightHighErr[j];
    }
  }


  TCanvas *cv1 = new TCanvas("test", "test", 0,0,800,600);
  cout << "LooseLoose Integral: " << dataDistributions[9]->Integral() << endl;
  cout << "TightLoose Integral: " << dataDistributions[10]->Integral() << endl;
  cout << "FakeRate LooseLoose Integral: " << dataDistributions_LooseLoosePrediction[11]->Integral() << endl;
  cout << "FakeRate TightLoose Integral: " << dataDistributions_TightLoosePrediction[11]->Integral() << endl;
  cout << "MC Integral: " << totalBkgDistributions_MC[11]->Integral() << endl;
  dataDistributions[9]->Draw("E1");
  cv1->SaveAs("LooseLoose_data.gif");
  dataDistributions[10]->Draw("E1");
  cv1->SaveAs("TightLoose_data.gif");

  cout << "DONE\n";

  //*****************************************************************************************
  //Make Plot
  //*****************************************************************************************
  string tmpLabel = label;
  //if (pairType != "") tmpLabel = tmpLabel + string("_") + pairType;
  cout << "*************************************************************************" << endl;
  cout << "Compare Predictions" << endl;
  cout << "*************************************************************************" << endl;
  
  //Draw Comparison Plots for data prediction
  TLegend *tmpLegend = new TLegend(0.2,0.55,0.6,0.90);
  tmpLegend->SetTextSize(0.04);
  tmpLegend->SetBorderSize(1);
  tmpLegend->AddEntry(totalBkgDistributions_MC[11], "MC", "F");
  tmpLegend->AddEntry(dataDistributions_LooseLoosePrediction[11], "Data Fake Rate Prediction", "LP");
//   tmpLegend->AddEntry(dataDistributions_TightLoosePrediction[11], "Data TightLoosePrediction", "LP");

  totalBkgDistributions_MC[11]->SetMaximum(0.03); 
  totalBkgDistributions_MC[11]->SetFillStyle(1001);
  totalBkgDistributions_MC[11]->SetFillColor(kMagenta);
  totalBkgDistributions_MC[11]->GetYaxis()->SetTitleOffset(1.5);
  totalBkgDistributions_MC[11]->GetXaxis()->SetTitleOffset(1.0);
  totalBkgDistributions_MC[11]->Draw("hist");

  dataDistributions_LooseLoosePrediction[11]->GetYaxis()->SetTitleOffset(1.5);
  dataDistributions_LooseLoosePrediction[11]->GetXaxis()->SetTitleOffset(1.0);
  dataDistributions_LooseLoosePrediction[11]->SetLineColor(kBlue);
  dataDistributions_LooseLoosePrediction[11]->SetMarkerColor(kBlue);
  dataDistributions_LooseLoosePrediction[11]->Draw("E1,same");
//   dataDistributions_TightLoosePrediction[11]->SetLineColor(kRed);
//   dataDistributions_TightLoosePrediction[11]->SetMarkerColor(kRed);
//   dataDistributions_TightLoosePrediction[11]->Draw("E1,same");
  tmpLegend->Draw();
  cv1->SaveAs("TotalBackgroundFakeRatePrediction_Data.gif"); 

  //Draw Comparison Plots for total background
  tmpLegend->Clear();
  tmpLegend->AddEntry(totalBkgDistributions_MC[11], "MC", "F");
  tmpLegend->AddEntry(totalBkgDistributions_LooseLoosePrediction[11], "MC Fake Rate Prediction", "LP");
//   tmpLegend->AddEntry(totalBkgDistributions_TightLoosePrediction[11], "TightLoosePrediction", "LP");
    
  totalBkgDistributions_MC[11]->SetMaximum(0.006);
  totalBkgDistributions_MC[11]->SetFillStyle(1001);
  totalBkgDistributions_MC[11]->SetFillColor(kMagenta);
  totalBkgDistributions_MC[11]->GetYaxis()->SetTitleOffset(1.5);
  totalBkgDistributions_MC[11]->GetXaxis()->SetTitleOffset(1.0);
  totalBkgDistributions_MC[11]->Draw("hist");
  totalBkgDistributions_LooseLoosePrediction[11]->GetYaxis()->SetTitleOffset(1.5);
  totalBkgDistributions_LooseLoosePrediction[11]->GetXaxis()->SetTitleOffset(1.0);
  totalBkgDistributions_LooseLoosePrediction[11]->SetLineColor(kBlue);
  totalBkgDistributions_LooseLoosePrediction[11]->SetMarkerColor(kBlue);
  totalBkgDistributions_LooseLoosePrediction[11]->Draw("E1,same");
//   totalBkgDistributions_TightLoosePrediction[11]->SetLineColor(kRed);
//   totalBkgDistributions_TightLoosePrediction[11]->SetMarkerColor(kRed);
//   totalBkgDistributions_TightLoosePrediction[11]->Draw("E1,same");
  tmpLegend->Draw();
  cv1->SaveAs("TotalBackgroundFakeRatePrediction_MC.gif"); 


  //Draw Fake rate prediction contamination from signal
  //Draw Loose+Loose , Tight+Loose for each process 
  signalDistributions_LooseLoosePrediction[9]->Draw();
  cv1->SaveAs("LooseLoose_MC_Zee.gif");
  signalDistributions_TightLoosePrediction[10]->Draw();
  cv1->SaveAs("TightLoose_MC_Zee.gif");       
  
  //Draw TightTight vs FakeRate Prediction Plots
  TLegend *tmpLegend1 = new TLegend(0.6,0.55,0.93,0.90);
  tmpLegend1->SetTextSize(0.05);
  tmpLegend1->SetBorderSize(1);
  tmpLegend1->AddEntry(signalDistributions_LooseLoosePrediction[11], "LooseLoosePrediction", "LP");
//   tmpLegend1->AddEntry(signalDistributions_TightLoosePrediction[11], "TightLoosePrediction", "LP");
  tmpLegend1->AddEntry(totalBkgDistributions_MC[11], "MC Fake Bkg", "LP");
  
  double maxY = 0.015;
  double minY = 0.0;
  if (minY != -999) {
    signalDistributions_LooseLoosePrediction[11]->SetMinimum(minY);
    signalDistributions_TightLoosePrediction[11]->SetMinimum(minY);
    totalBkgDistributions_MC[11]->SetMinimum(minY);
  }
  if (maxY != -999) {
    signalDistributions_LooseLoosePrediction[11]->SetMaximum(maxY);
    signalDistributions_TightLoosePrediction[11]->SetMaximum(maxY);
    totalBkgDistributions_MC[11]->SetMaximum(maxY);
  }
  signalDistributions_LooseLoosePrediction[11]->GetYaxis()->SetTitleOffset(1.5);
  signalDistributions_LooseLoosePrediction[11]->GetXaxis()->SetTitleOffset(1.0);

  totalBkgDistributions_MC[11]->SetFillStyle(1001);
  totalBkgDistributions_MC[11]->SetFillColor(kMagenta);
  totalBkgDistributions_MC[11]->Draw("hist");
  signalDistributions_LooseLoosePrediction[11]->SetLineColor(kBlue);
  signalDistributions_LooseLoosePrediction[11]->SetMarkerColor(kBlue);
  signalDistributions_LooseLoosePrediction[11]->Draw("E1,same");
//   signalDistributions_TightLoosePrediction[11]->SetLineColor(kRed);
//   signalDistributions_TightLoosePrediction[11]->SetMarkerColor(kRed);
//   signalDistributions_TightLoosePrediction[11]->Draw("E1,same");
  tmpLegend1->Draw();
  cv1->SaveAs("FakeRateMethodPrediction_Zee.gif");
  


  //Draw plots separately for each process
  for (int p=0; p<processLabel.size() ; ++p) {
    TCanvas *cv = new TCanvas("test", "test", 0,0,800,600);

    //Draw Loose+Loose , Tight+Loose for each process 
    bkgDistributions_LooseLoosePrediction[9][p]->Draw();
    cv->SaveAs((string("LooseLoose_MC_") + processLabel[p] + ".gif").c_str());
    bkgDistributions_TightLoosePrediction[10][p]->Draw();
    cv->SaveAs((string("TightLoose_MC_") + processLabel[p] + ".gif").c_str());       

    //Draw TightTight vs FakeRate Prediction Plots
    TLegend *tmpLegend1 = new TLegend(0.5,0.55,0.93,0.90);
    tmpLegend1->SetTextSize(0.03);
    tmpLegend1->SetBorderSize(1);
    tmpLegend1->AddEntry(bkgDistributions_MC[11][p], "MC", "L");
    tmpLegend1->AddEntry(bkgDistributions_LooseLoosePrediction[11][p], "LooseLoosePrediction", "LP");
    tmpLegend1->AddEntry(bkgDistributions_TightLoosePrediction[11][p], "TightLoosePrediction", "LP");

    double maxY = processYMax[p];
    double minY = 0.0;
    if (minY != -999) {
      bkgDistributions_MC[11][p]->SetMinimum(minY);
      bkgDistributions_LooseLoosePrediction[11][p]->SetMinimum(minY);
      bkgDistributions_TightLoosePrediction[11][p]->SetMinimum(minY);
    }
    if (maxY != -999) {
      bkgDistributions_MC[11][p]->SetMaximum(maxY);
      bkgDistributions_LooseLoosePrediction[11][p]->SetMaximum(maxY);
      bkgDistributions_TightLoosePrediction[11][p]->SetMaximum(maxY);
    }
    bkgDistributions_MC[11][p]->GetYaxis()->SetTitleOffset(1.75);
    bkgDistributions_MC[11][p]->GetXaxis()->SetTitleOffset(1.0);

    bkgDistributions_MC[11][p]->Draw("hist");
    bkgDistributions_LooseLoosePrediction[11][p]->SetLineColor(kBlue);
    bkgDistributions_LooseLoosePrediction[11][p]->SetMarkerColor(kBlue);
    bkgDistributions_LooseLoosePrediction[11][p]->Draw("E1,same");
    bkgDistributions_TightLoosePrediction[11][p]->SetLineColor(kRed);
    bkgDistributions_TightLoosePrediction[11][p]->SetMarkerColor(kRed);
    bkgDistributions_TightLoosePrediction[11][p]->Draw("E1,same");
    
    tmpLegend1->Draw();
    cv->SaveAs(("FakeRateMethodPrediction_" + processLabel[p] + ".gif").c_str());

  }

  TCanvas *cv2 = new TCanvas("test", "test", 0,0,800,600);
  //Draw Stacks
  cv2->SetLogy();
  tmpLegend->Clear();
  for(int i=0; i<bkgStacks_MC[11]->GetStack()->GetEntries(); ++i) {
    tmpLegend->AddEntry((TH1F*)bkgStacks_MC[11]->GetStack()->At(i), processNames[i].c_str(), "F");
  }
  bkgStacks_MC[11]->Draw("hist");
  tmpLegend->Draw();
  cv2->SaveAs("BkgStack_MC.gif");

  TLegend *tmpLegend_LooseLoosePrediction = new TLegend(0.5,0.55,0.93,0.90);
  tmpLegend_LooseLoosePrediction->SetTextSize(0.03);
  tmpLegend_LooseLoosePrediction->SetBorderSize(1);
  for(int i=0; i<bkgStacks_LooseLoosePrediction[11]->GetStack()->GetEntries(); ++i) {
    tmpLegend->AddEntry((TH1F*)bkgStacks_LooseLoosePrediction[11]->GetStack()->At(i), processNames[i].c_str(), "F");
  }
  bkgStacks_LooseLoosePrediction[11]->Draw("hist");
  tmpLegend_LooseLoosePrediction->Draw();
  cv2->SaveAs("BkgStack_LooseLoosePredictionStack.gif");

  TLegend *tmpLegend_TightLoosePrediction = new TLegend(0.5,0.55,0.93,0.90);
  tmpLegend_TightLoosePrediction->SetTextSize(0.03);
  tmpLegend_TightLoosePrediction->SetBorderSize(1);
  for(int i=0; i<bkgStacks_TightLoosePrediction[11]->GetStack()->GetEntries(); ++i) {
    tmpLegend->AddEntry((TH1F*)bkgStacks_TightLoosePrediction[11]->GetStack()->At(i), processNames[i].c_str(), "F");
  }
  bkgStacks_TightLoosePrediction[11]->Draw("hist");
  tmpLegend_TightLoosePrediction->Draw();
  cv2->SaveAs("BkgStack_TightLoosePredictionStack.gif");






  cout << "*************************************************************************" << endl;
  cout << "Print Final Results" << endl;
  cout << "*************************************************************************" << endl;
  double massMin = 60.0;
  double massMax = 120.0;
 

  double dataLooseLooseIntegral = dataDistributions[9]->Integral(dataDistributions_LooseLoosePrediction[9]->GetXaxis()->FindFixBin(massMin), dataDistributions_LooseLoosePrediction[9]->GetXaxis()->FindFixBin(massMax));
  double dataLooseLooseIntegralError = IntegralError(dataDistributions_LooseLoosePrediction[9], NULL, massMin, massMax);
  cout << "Data Loose+Loose Events : " << dataLooseLooseIntegral << " +- " <<  dataLooseLooseIntegralError << endl;

  double dataTightLooseIntegral = dataDistributions[10]->Integral(dataDistributions_TightLoosePrediction[10]->GetXaxis()->FindFixBin(massMin), dataDistributions_TightLoosePrediction[10]->GetXaxis()->FindFixBin(massMax));
  double dataTightLooseIntegralError = IntegralError(dataDistributions_TightLoosePrediction[10], NULL, massMin, massMax);
  cout << "Data Tight+Loose Events : " << dataTightLooseIntegral << " +- " <<  dataTightLooseIntegralError << endl;

  double dataLooseLoosePredictionIntegral = dataDistributions_LooseLoosePrediction[11]->Integral(dataDistributions_LooseLoosePrediction[11]->GetXaxis()->FindFixBin(massMin), dataDistributions_LooseLoosePrediction[11]->GetXaxis()->FindFixBin(massMax));
  double dataLooseLoosePredictionIntegralError = IntegralError(dataDistributions_LooseLoosePrediction[11], dataDistributions_LooseLoosePrediction_sysErrors[11],massMin, massMax);
  cout << "Data Loose+Loose Predicted Events : " << dataLooseLoosePredictionIntegral << " +- " <<  dataLooseLoosePredictionIntegralError << endl;

  double dataTightLoosePredictionIntegral = dataDistributions_TightLoosePrediction[11]->Integral(dataDistributions_TightLoosePrediction[11]->GetXaxis()->FindFixBin(massMin), dataDistributions_TightLoosePrediction[11]->GetXaxis()->FindFixBin(massMax));
  double dataTightLoosePredictionIntegralError = IntegralError(dataDistributions_TightLoosePrediction[11], dataDistributions_TightLoosePrediction_sysErrors[11],massMin, massMax);
  cout << "Data Tight+Loose Predicted Events : " << dataTightLoosePredictionIntegral << " +- " <<  dataTightLoosePredictionIntegralError << endl;
 
  double signalLooseLooseIntegral = signalDistributions_LooseLoosePrediction[9]->Integral(signalDistributions_LooseLoosePrediction[9]->GetXaxis()->FindFixBin(massMin), signalDistributions_LooseLoosePrediction[9]->GetXaxis()->FindFixBin(massMax));
  double signalLooseLooseIntegralError = IntegralError(signalDistributions_LooseLoosePrediction[9], NULL, massMin, massMax);
  cout << "Signal Loose+Loose Events : " << signalLooseLooseIntegral << " +- " <<  signalLooseLooseIntegralError << endl;

  double signalTightLooseIntegral = signalDistributions_TightLoosePrediction[10]->Integral(signalDistributions_TightLoosePrediction[10]->GetXaxis()->FindFixBin(massMin), signalDistributions_TightLoosePrediction[10]->GetXaxis()->FindFixBin(massMax));
  double signalTightLooseIntegralError = IntegralError(signalDistributions_TightLoosePrediction[10], NULL, massMin, massMax);
  cout << "Signal Tight+Loose Events : " << signalTightLooseIntegral << " +- " <<  signalTightLooseIntegralError << endl;


  double signalLooseLoosePredictionIntegral = signalDistributions_LooseLoosePrediction[11]->Integral(signalDistributions_LooseLoosePrediction[11]->GetXaxis()->FindFixBin(massMin), signalDistributions_LooseLoosePrediction[11]->GetXaxis()->FindFixBin(massMax));
  double signalLooseLoosePredictionIntegralError = IntegralError(signalDistributions_LooseLoosePrediction[11], signalDistributions_LooseLoosePrediction_sysErrors[11], massMin, massMax);
  cout << "Signal Loose+Loose Predicted Events : " << signalLooseLoosePredictionIntegral << " +- " <<  signalLooseLoosePredictionIntegralError << endl;

  double signalTightLoosePredictionIntegral = signalDistributions_TightLoosePrediction[11]->Integral(signalDistributions_TightLoosePrediction[11]->GetXaxis()->FindFixBin(massMin), signalDistributions_TightLoosePrediction[11]->GetXaxis()->FindFixBin(massMax));
  double signalTightLoosePredictionIntegralError = IntegralError(signalDistributions_TightLoosePrediction[11], signalDistributions_TightLoosePrediction_sysErrors[11],massMin, massMax);
  cout << "Signal Tight+Loose Predicted Events : " << signalTightLoosePredictionIntegral << " +- " <<  signalTightLoosePredictionIntegralError << endl;
  
  cout << endl << endl;
  cout << "Backgrounds\n";
  for (int p=0; p<processLabel.size() ; ++p) {

    cout << processLabel[p] << " MC Prediction : " << bkgDistributions_MC[11][p]->Integral(bkgDistributions_MC[11][p]->GetXaxis()->FindFixBin(massMin), bkgDistributions_MC[11][p]->GetXaxis()->FindFixBin(massMax)) << endl;
    double bkgLooseLooseIntegral = bkgDistributions_LooseLoosePrediction[9][p]->Integral(bkgDistributions_LooseLoosePrediction[9][p]->GetXaxis()->FindFixBin(massMin), bkgDistributions_LooseLoosePrediction[9][p]->GetXaxis()->FindFixBin(massMax));
    double bkgLooseLooseIntegralError = IntegralError(bkgDistributions_LooseLoosePrediction[9][p], NULL, massMin, massMax);
    cout << "Bkg Loose+Loose Events : " << bkgLooseLooseIntegral << " +- " <<  bkgLooseLooseIntegralError << endl;

     double bkgTightLooseIntegral = bkgDistributions_TightLoosePrediction[10][p]->Integral(bkgDistributions_TightLoosePrediction[10][p]->GetXaxis()->FindFixBin(massMin), bkgDistributions_TightLoosePrediction[10][p]->GetXaxis()->FindFixBin(massMax));
   double bkgTightLooseIntegralError = IntegralError(bkgDistributions_TightLoosePrediction[10][p], NULL, massMin, massMax);
    cout << "Bkg Tight+Loose Events : " << bkgTightLooseIntegral << " +- " <<  bkgTightLooseIntegralError << endl;


    double bkgLooseLoosePredictionIntegral = bkgDistributions_LooseLoosePrediction[11][p]->Integral(bkgDistributions_LooseLoosePrediction[11][p]->GetXaxis()->FindFixBin(massMin), bkgDistributions_LooseLoosePrediction[11][p]->GetXaxis()->FindFixBin(massMax));
   double bkgLooseLoosePredictionIntegralError = IntegralError(bkgDistributions_LooseLoosePrediction[11][p], bkgDistributions_LooseLoosePrediction_sysErrors[11][p],massMin, massMax);;
     cout << "Bkg Loose+Loose Predicted Events : " << bkgLooseLoosePredictionIntegral << " +- " <<  bkgLooseLoosePredictionIntegralError << endl;

    double bkgTightLoosePredictionIntegral = bkgDistributions_TightLoosePrediction[11][p]->Integral(bkgDistributions_TightLoosePrediction[11][p]->GetXaxis()->FindFixBin(massMin), bkgDistributions_TightLoosePrediction[11][p]->GetXaxis()->FindFixBin(massMax));
    double bkgTightLoosePredictionIntegralError = IntegralError(bkgDistributions_TightLoosePrediction[11][p], bkgDistributions_TightLoosePrediction_sysErrors[11][p],massMin, massMax);;
    cout << "Bkg Tight+Loose Predicted Events : " << bkgTightLoosePredictionIntegral << " +- " <<  bkgTightLoosePredictionIntegralError << endl;
  
    
  }


  delete fFakeRate;
  delete fMCFakeRate;


}


