//root -l $CMSSW_BASE/src/MitHiggs/macros/plotMacros/makeJetChargeCorrelationPlots.C+


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
// study charge correlation in quark to track match
//--------------------------------------------------------------------------------------------------
void makeSSOSRatioForQuarkToTrackMatch( vector<string> datasetfiles, vector<string> datasetnames, string label, int quarktype ) {


  TLegend *leg1=0;
  leg1 = new TLegend(0.7,0.7,0.9,0.9);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.02);

  string quarkTypeName = "";
  if (quarktype == 1) {
    quarkTypeName = "Down";
  } else if (quarktype == 2) {
    quarkTypeName = "Up";
  } else {
    cerr << "Error: quarktype = " << quarktype << " is not allowed.\n";
    assert(false);
  }
  
  string plotname = "SSOSRatio_";
  plotname +=  quarkTypeName + "Jets_" + label;
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  string histname[7] = {"h" + quarkTypeName + "JetFlavorMatchedTrackCharge",
                        "h" + quarkTypeName + "JetFlavorMatchedLooseIsolatedTrackCharge",
                        "h" + quarkTypeName + "JetFlavorMatchedTightIsolatedTrackCharge",
                        "h" + quarkTypeName + "JetFlavorMatchedRecoElectronCharge",
                        "h" + quarkTypeName + "JetFlavorMatchedLooseElectronCharge",
                        "h" + quarkTypeName + "JetFlavorMatchedTightElectronCharge",
                        "h" + quarkTypeName + "JetFlavorMatchedCleanElectronCharge"};
 
  TH1F *SSOSRatioHist = new TH1F(("SSOSRatio_"+quarkTypeName + "Jets_" + label).c_str(), 
                                 ("SSOSRatio_"+quarkTypeName + "Jets_" +label).c_str(), 7, -0.5, 6.5);
  TH1F *PlusJetSSOSRatioHist = new TH1F(("SSOSRatio_"+quarkTypeName + "Jets_" +label).c_str(), 
                                        ("SSOSRatio_"+quarkTypeName + "Jets_" +label).c_str(), 7, -0.5, 6.5);
  TH1F *MinusJetSSOSRatioHist = new TH1F(("SSOSRatio_"+quarkTypeName + "Jets_" +label).c_str(), 
                                         ("SSOSRatio_"+quarkTypeName + "Jets_" +label).c_str(), 7, -0.5, 6.5);

  for (int i = 0; i < 7;i++) {

    TH1F *ChargeCodeHist = addAllSamples(datasetfiles, datasetnames, "JetChargeCorrelationStudyMod" ,histname[i]);

    Double_t plusRatio;
    Double_t plusRatioError;
    if (ChargeCodeHist->GetBinContent(4) > 0) {
      plusRatio = ChargeCodeHist->GetBinContent(5) / ChargeCodeHist->GetBinContent(4);
      plusRatioError = plusRatio * TMath::Sqrt((ChargeCodeHist->GetBinError(5)/ChargeCodeHist->GetBinContent(5))*
                                               (ChargeCodeHist->GetBinError(5)/ChargeCodeHist->GetBinContent(5))
                                                + (ChargeCodeHist->GetBinError(4)/ChargeCodeHist->GetBinContent(4))*
                                               (ChargeCodeHist->GetBinError(4)/ChargeCodeHist->GetBinContent(4)));
    } else {
      plusRatio = 0;
      plusRatioError = 0;
    }
    PlusJetSSOSRatioHist->SetBinContent(i+1,plusRatio);
    PlusJetSSOSRatioHist->SetBinError(i+1,plusRatioError);
    PlusJetSSOSRatioHist->SetMarkerColor(kBlue);
    PlusJetSSOSRatioHist->SetLineColor(kBlue);


    Double_t minusRatio;
    Double_t minusRatioError;
    if (ChargeCodeHist->GetBinContent(2) > 0) {
      minusRatio = ChargeCodeHist->GetBinContent(1) / ChargeCodeHist->GetBinContent(2);
      minusRatioError = minusRatio * TMath::Sqrt((ChargeCodeHist->GetBinError(1)/ChargeCodeHist->GetBinContent(1))*
                                               (ChargeCodeHist->GetBinError(1)/ChargeCodeHist->GetBinContent(1))
                                                + (ChargeCodeHist->GetBinError(2)/ChargeCodeHist->GetBinContent(2))*
                                               (ChargeCodeHist->GetBinError(2)/ChargeCodeHist->GetBinContent(2)));
    } else {
      minusRatio = 0;
      minusRatioError = 0;
    }
    MinusJetSSOSRatioHist->SetBinContent(i+1,minusRatio);
    MinusJetSSOSRatioHist->SetBinError(i+1,minusRatioError);
    MinusJetSSOSRatioHist->SetMarkerColor(kRed);
    MinusJetSSOSRatioHist->SetLineColor(kRed);

    Double_t Ratio;
    Double_t RatioError;
    Double_t Numerator = ChargeCodeHist->GetBinContent(1) + ChargeCodeHist->GetBinContent(5);
    Double_t NumeratorError = TMath::Sqrt(ChargeCodeHist->GetBinError(1)*ChargeCodeHist->GetBinError(1) + 
                                          ChargeCodeHist->GetBinError(5)*ChargeCodeHist->GetBinError(5));
    Double_t Denominator = ChargeCodeHist->GetBinContent(2) + ChargeCodeHist->GetBinContent(4);
    Double_t DenominatorError = TMath::Sqrt(ChargeCodeHist->GetBinError(2)*ChargeCodeHist->GetBinError(2) + 
                                          ChargeCodeHist->GetBinError(4)*ChargeCodeHist->GetBinError(4));

    if (Denominator > 0) {
      Ratio = Numerator / Denominator;
      RatioError = Ratio * TMath::Sqrt((NumeratorError/Numerator)*(NumeratorError/Numerator) + 
                                       (DenominatorError / Denominator)* (DenominatorError / Denominator));
    } else {
      Ratio = 0;
      RatioError = 0;
    }
    SSOSRatioHist->SetBinContent(i+1,Ratio);
    SSOSRatioHist->SetBinError(i+1,RatioError);
    SSOSRatioHist->SetMarkerColor(kBlack);
    SSOSRatioHist->SetLineColor(kBlack);

  }
  leg1->AddEntry(PlusJetSSOSRatioHist,"PlusJets", "LP");
  leg1->AddEntry(MinusJetSSOSRatioHist,"MinusJets", "LP");
  leg1->AddEntry(SSOSRatioHist,"AllJets", "LP");

  if (quarktype == 1) {
    SSOSRatioHist->SetMaximum(1.0);
    SSOSRatioHist->SetMinimum(0.4);
  } else if (quarktype == 2) {
    SSOSRatioHist->SetMaximum(4.0);
    SSOSRatioHist->SetMinimum(1.0);
  } else {
    cerr << "Error: quarktype = " << quarktype << " is not allowed.\n";
    assert(false);
  }

  SSOSRatioHist->Draw();
  PlusJetSSOSRatioHist->Draw("same");
  MinusJetSSOSRatioHist->Draw("same");
  leg1->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

}

//--------------------------------------------------------------------------------------------------
// study charge correlation in W to jet
//--------------------------------------------------------------------------------------------------
void makeJetFlavor( vector<string> datasetfiles, vector<string> datasetnames, string label, int quarktype ) {


  TLegend *leg1=0;
  leg1 = new TLegend(0.7,0.7,0.9,0.9);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.02);

  string quarkTypeName = "";
  if (quarktype == 1) {
    quarkTypeName = "Down";
  } else if (quarktype == 2) {
    quarkTypeName = "Up";
  } else {
    cerr << "Error: quarktype = " << quarktype << " is not allowed.\n";
    assert(false);
  }
  
  string plotname = "SSOSRatio_";
  plotname +=  quarkTypeName + "Jets_" + label;
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  string histname[7] = {"h" + quarkTypeName + "JetFlavorMatchedTrackCharge",
                        "h" + quarkTypeName + "JetFlavorMatchedLooseIsolatedTrackCharge",
                        "h" + quarkTypeName + "JetFlavorMatchedTightIsolatedTrackCharge",
                        "h" + quarkTypeName + "JetFlavorMatchedRecoElectronCharge",
                        "h" + quarkTypeName + "JetFlavorMatchedLooseElectronCharge",
                        "h" + quarkTypeName + "JetFlavorMatchedTightElectronCharge",
                        "h" + quarkTypeName + "JetFlavorMatchedCleanElectronCharge"};
 
  TH1F *SSOSRatioHist = new TH1F(("SSOSRatio_"+quarkTypeName + "Jets_" + label).c_str(), 
                                 ("SSOSRatio_"+quarkTypeName + "Jets_" +label).c_str(), 7, -0.5, 6.5);
  TH1F *PlusJetSSOSRatioHist = new TH1F(("SSOSRatio_"+quarkTypeName + "Jets_" +label).c_str(), 
                                        ("SSOSRatio_"+quarkTypeName + "Jets_" +label).c_str(), 7, -0.5, 6.5);
  TH1F *MinusJetSSOSRatioHist = new TH1F(("SSOSRatio_"+quarkTypeName + "Jets_" +label).c_str(), 
                                         ("SSOSRatio_"+quarkTypeName + "Jets_" +label).c_str(), 7, -0.5, 6.5);

  for (int i = 0; i < 7;i++) {

    TH1F *ChargeCodeHist = addAllSamples(datasetfiles, datasetnames, "JetChargeCorrelationStudyMod" ,histname[i]);

    Double_t plusRatio;
    Double_t plusRatioError;
    if (ChargeCodeHist->GetBinContent(4) > 0) {
      plusRatio = ChargeCodeHist->GetBinContent(5) / ChargeCodeHist->GetBinContent(4);
      plusRatioError = plusRatio * TMath::Sqrt((ChargeCodeHist->GetBinError(5)/ChargeCodeHist->GetBinContent(5))*
                                               (ChargeCodeHist->GetBinError(5)/ChargeCodeHist->GetBinContent(5))
                                                + (ChargeCodeHist->GetBinError(4)/ChargeCodeHist->GetBinContent(4))*
                                               (ChargeCodeHist->GetBinError(4)/ChargeCodeHist->GetBinContent(4)));
    } else {
      plusRatio = 0;
      plusRatioError = 0;
    }
    PlusJetSSOSRatioHist->SetBinContent(i+1,plusRatio);
    PlusJetSSOSRatioHist->SetBinError(i+1,plusRatioError);
    PlusJetSSOSRatioHist->SetMarkerColor(kBlue);
    PlusJetSSOSRatioHist->SetLineColor(kBlue);


    Double_t minusRatio;
    Double_t minusRatioError;
    if (ChargeCodeHist->GetBinContent(2) > 0) {
      minusRatio = ChargeCodeHist->GetBinContent(1) / ChargeCodeHist->GetBinContent(2);
      minusRatioError = minusRatio * TMath::Sqrt((ChargeCodeHist->GetBinError(1)/ChargeCodeHist->GetBinContent(1))*
                                               (ChargeCodeHist->GetBinError(1)/ChargeCodeHist->GetBinContent(1))
                                                + (ChargeCodeHist->GetBinError(2)/ChargeCodeHist->GetBinContent(2))*
                                               (ChargeCodeHist->GetBinError(2)/ChargeCodeHist->GetBinContent(2)));
    } else {
      minusRatio = 0;
      minusRatioError = 0;
    }
    MinusJetSSOSRatioHist->SetBinContent(i+1,minusRatio);
    MinusJetSSOSRatioHist->SetBinError(i+1,minusRatioError);
    MinusJetSSOSRatioHist->SetMarkerColor(kRed);
    MinusJetSSOSRatioHist->SetLineColor(kRed);

    Double_t Ratio;
    Double_t RatioError;
    Double_t Numerator = ChargeCodeHist->GetBinContent(1) + ChargeCodeHist->GetBinContent(5);
    Double_t NumeratorError = TMath::Sqrt(ChargeCodeHist->GetBinError(1)*ChargeCodeHist->GetBinError(1) + 
                                          ChargeCodeHist->GetBinError(5)*ChargeCodeHist->GetBinError(5));
    Double_t Denominator = ChargeCodeHist->GetBinContent(2) + ChargeCodeHist->GetBinContent(4);
    Double_t DenominatorError = TMath::Sqrt(ChargeCodeHist->GetBinError(2)*ChargeCodeHist->GetBinError(2) + 
                                          ChargeCodeHist->GetBinError(4)*ChargeCodeHist->GetBinError(4));

    if (Denominator > 0) {
      Ratio = Numerator / Denominator;
      RatioError = Ratio * TMath::Sqrt((NumeratorError/Numerator)*(NumeratorError/Numerator) + 
                                       (DenominatorError / Denominator)* (DenominatorError / Denominator));
    } else {
      Ratio = 0;
      RatioError = 0;
    }
    SSOSRatioHist->SetBinContent(i+1,Ratio);
    SSOSRatioHist->SetBinError(i+1,RatioError);
    SSOSRatioHist->SetMarkerColor(kBlack);
    SSOSRatioHist->SetLineColor(kBlack);

  }
  leg1->AddEntry(PlusJetSSOSRatioHist,"PlusJets", "LP");
  leg1->AddEntry(MinusJetSSOSRatioHist,"MinusJets", "LP");
  leg1->AddEntry(SSOSRatioHist,"AllJets", "LP");

  if (quarktype == 1) {
    SSOSRatioHist->SetMaximum(1.0);
    SSOSRatioHist->SetMinimum(0.4);
  } else if (quarktype == 2) {
    SSOSRatioHist->SetMaximum(4.0);
    SSOSRatioHist->SetMinimum(1.0);
  } else {
    cerr << "Error: quarktype = " << quarktype << " is not allowed.\n";
    assert(false);
  }

  SSOSRatioHist->Draw();
  PlusJetSSOSRatioHist->Draw("same");
  MinusJetSSOSRatioHist->Draw("same");
  leg1->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

}

//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void makeJetChargeCorrelationPlots()
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
    madgraphWJetsDatasetFiles.push_back("/home/sixie/hist/JetChargeStudy/filler/006/JetChargeStudy_f8-wjets-mg-id9.root");
    madgraphWJetsDatasetNames.push_back("f8-wjets-mg-id9");
    //Pythia WJets
    pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/JetChargeStudy/filler/006/JetChargeStudy_s8-we-id9.root");
    pythiaWJetsDatasetNames.push_back("s8-we-id9");
    pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/JetChargeStudy/filler/006/JetChargeStudy_s8-wm-id9.root");
    pythiaWJetsDatasetNames.push_back("s8-wm-id9");
    pythiaWJetsDatasetFiles.push_back("/home/sixie/hist/JetChargeStudy/filler/006/JetChargeStudy_s8-wtau-id9.root");
    pythiaWJetsDatasetNames.push_back("s8-wtau-id9");
    //QCDEM WJets
    QCDEMDatasetFiles.push_back("/home/sixie/hist/JetChargeStudy/filler/006/JetChargeStudy_s8-qcdem_20_30-id9.root");
    QCDEMDatasetNames.push_back("s8-qcdem_20_30-id9");
    QCDEMDatasetFiles.push_back("/home/sixie/hist/JetChargeStudy/filler/006/JetChargeStudy_s8-qcdem_30_80-id9.root");
    QCDEMDatasetNames.push_back("s8-qcdem_30_80-id9");
    QCDEMDatasetFiles.push_back("/home/sixie/hist/JetChargeStudy/filler/006/JetChargeStudy_s8-qcdem_80_170-id9.root");
    QCDEMDatasetNames.push_back("s8-qcdem_80_170-id9");
    

    makeSSOSRatioForQuarkToTrackMatch(pythiaWJetsDatasetFiles,pythiaWJetsDatasetNames, "pythiaWJets", 1);
    makeSSOSRatioForQuarkToTrackMatch(pythiaWJetsDatasetFiles,pythiaWJetsDatasetNames, "pythiaWJets", 2);

   return;
   
}
