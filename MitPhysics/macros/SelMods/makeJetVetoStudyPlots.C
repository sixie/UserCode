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
using namespace std;
using namespace mithep;
#endif

#include <vector>

//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void makeJetVetoStudyPlots()
{
  //Make Plots for Jet Validation
  gStyle->SetPalette(1);

  bool     doLeg   = true;
  double   legx[2] = { 0.5, 0.75 };
  double   legy[2] = { 0.7, 0.8 };
  double   legTextSize = 0.03;
  TString legTitle = "";

  //string sample = "_IC5JetPlusTrack";
  //string sample = "_CaloJets";
  //string sample = "_IC5JetPlusTrack_WithHWWCuts";
  //string sample = "_CaloJets_WithHWWCuts";
  string sample = "_CaloJets_WithDirtyMuonVeto";
  string plotname;
  string filename = "mit-JetValidation-hist.root";

  vector<string> filenames;
  vector<string> datasetnames;
  vector<string> weights;
  filenames.push_back("mit-JetVetoStudy-004-hist_HWW" + sample + ".root");
  filenames.push_back("mit-JetVetoStudy-004-hist_WW" + sample + ".root");
  filenames.push_back("mit-JetVetoStudy-004-hist_ttbar" + sample + ".root");
   datasetnames.push_back("HWW");
   datasetnames.push_back("WW");
  datasetnames.push_back("TTBAR");
  

//    plotname = "NTracksNoGenJet";
//    makeComparisonPlot( filenames,datasetnames,"hNTracksNoGenJets",plotname+sample,true);

//   plotname = "NTracksWithGenJet";
//   makeComparisonPlot( filenames,datasetnames,"hNTracksWithGenJets",plotname+sample,true);

//    plotname = "JetVetoEffVsJetEtCut";
//    makeComparisonPlot( filenames,datasetnames,"hJetVetoEffVsJetEtCut",plotname+sample,false, "upperLeft", 1.0);

//    plotname = "JetVetoEffVsPurity";
//    makeEffVsPurityPlot( "mit-JetVetoStudy-004-hist_HWW" + sample + ".root", "mit-JetVetoStudy-004-hist_ttbar" + sample + ".root",  0.325/23300,  92.9/11700, "hNEventsPassJetVetoVsJetEtCutHist", "hJetVetoEffVsJetEtCut", plotname+sample);


  vector<string> signalfiles;
  vector<string> bkgfiles;
  vector<string> jetnames;

  plotname = "JetVetoSignalEffVsBkgEff";
  signalfiles.clear();
  bkgfiles.clear();
  jetnames.clear();
  signalfiles.push_back("mit-JetVetoStudy-004-hist_HWW_CaloJets.root");
  signalfiles.push_back("mit-JetVetoStudy-004-hist_HWW_CaloJetsCorr.root");
  signalfiles.push_back("mit-JetVetoStudy-004-hist_HWW_IC5JetPlusTrack.root");
  bkgfiles.push_back("mit-JetVetoStudy-004-hist_ttbar_CaloJets.root");
  bkgfiles.push_back("mit-JetVetoStudy-004-hist_ttbar_CaloJetsCorr.root");
  bkgfiles.push_back("mit-JetVetoStudy-004-hist_ttbar_IC5JetPlusTrack.root");
  jetnames.push_back("IC5 CaloJets");
  jetnames.push_back("IC5 Corrected CaloJets");
  jetnames.push_back("IC5 JetPlusTrack");
  // makeEffVsRejectionPlot( signalfiles, bkgfiles, jetnames, 0.325/23300,  92.9/11700, "hJetVetoEffVsJetEtCut", plotname);

  plotname = "JetVetoSignalEffVsBkgEffWithHWWCuts";
  signalfiles.clear();
  bkgfiles.clear();
  jetnames.clear();
  signalfiles.push_back("mit-JetVetoStudy-004-hist_HWW_CaloJets_WithHWWCuts.root");
  signalfiles.push_back("mit-JetVetoStudy-004-hist_HWW_CaloJetsCorr_WithHWWCuts.root");
  signalfiles.push_back("mit-JetVetoStudy-004-hist_HWW_IC5JetPlusTrack_WithHWWCuts.root");
  bkgfiles.push_back("mit-JetVetoStudy-004-hist_ttbar_CaloJets_WithHWWCuts.root");
  bkgfiles.push_back("mit-JetVetoStudy-004-hist_ttbar_CaloJetsCorr_WithHWWCuts.root");
  bkgfiles.push_back("mit-JetVetoStudy-004-hist_ttbar_IC5JetPlusTrack_WithHWWCuts.root");
  jetnames.push_back("IC5 CaloJets");
  jetnames.push_back("IC5 Corrected CaloJets");
  jetnames.push_back("IC5 JetPlusTrack");
  //makeEffVsRejectionPlot( signalfiles, bkgfiles, jetnames, 0.325/23300,  92.9/11700, "hJetVetoEffVsJetEtCut", plotname);


//   plotname = "TrackVetoEff";
//   makeComparisonPlot( filenames,datasetnames,"hTrackVetoEff",plotname+sample,false, "lowerRight", 1.0);

//   plotname = "TrackVetoEffVsPurity";
//   makeEffVsPurityPlot( "mit-JetVetoStudy-004-hist_HWW" + sample + ".root", "mit-JetVetoStudy-004-hist_ttbar" + sample + ".root",  0.325/23300,  92.9/11700, "hNEventsPassTrackVetoHist", "hTrackVetoEff", plotname+sample);


  plotname = "TrackVetoSignalEffVsBkgEffAfterVeto";
  signalfiles.clear();
  bkgfiles.clear();
  jetnames.clear();
  signalfiles.push_back("mit-JetVetoStudy-004-hist_HWW_CaloJets.root");
  signalfiles.push_back("mit-JetVetoStudy-004-hist_HWW_CaloJets_WithDirtyMuonVeto.root");
  signalfiles.push_back("mit-JetVetoStudy-004-hist_HWW_CaloJets_WithHWWCuts.root");
  signalfiles.push_back("mit-JetVetoStudy-004-hist_HWW_CaloJets_WithHWWCuts_WithDirtyMuonVeto.root");
  bkgfiles.push_back("mit-JetVetoStudy-004-hist_ttbar_CaloJets.root");
  bkgfiles.push_back("mit-JetVetoStudy-004-hist_ttbar_CaloJets_WithDirtyMuonVeto.root");
  bkgfiles.push_back("mit-JetVetoStudy-004-hist_ttbar_CaloJets_WithHWWCuts.root");
  bkgfiles.push_back("mit-JetVetoStudy-004-hist_ttbar_CaloJets_WithHWWCuts_WithDirtyMuonVeto.root");
  jetnames.push_back("JetVeto");
  jetnames.push_back("JetVeto, DirtyMuon Veto");
  jetnames.push_back("JetVeto, HWW Cuts");
  jetnames.push_back("JetVeto, DirtyMuon Veto, HWW Cuts");
  makeEffVsRejectionPlot( signalfiles, bkgfiles, jetnames,  0.325/23300,  92.9/11700, "hTrackVetoEffAfterJetVeto", plotname);

//   plotname = "TrackVetoEffAfterJetVeto";
//   makeComparisonPlot( filenames,datasetnames,"hTrackVetoEffAfterJetVeto",plotname+sample,false, "lowerRight", 1.0);

//   plotname = "TrackVetoEffVsPurityAfterJetVeto";
//   makeEffVsPurityPlot( "mit-JetVetoStudy-004-hist_HWW" + sample + ".root", "mit-JetVetoStudy-004-hist_ttbar" + sample + ".root",  0.325/23300,  92.9/11700, "hNEventsPassTrackVetoHistAfterJetVeto", "hTrackVetoEffAfterJetVeto", plotname);


//   plotname = "TrackVetoEffAfterDirtyMuonVeto";
//   makeComparisonPlot( filenames,datasetnames,"hTrackVetoEff",plotname+sample,false, "lowerRight", 1.0);

//   plotname = "JetVetoOptimization";
//   make1DOptimizationPlot( "mit-JetVetoStudy-004-hist_HWW" + sample + ".root", "mit-JetVetoStudy-004-hist_ttbar" + sample + ".root", "HWW", "TTBAR" , 0.325/23300, 92.9/11700, "hNEventsPassJetVetoVsJetEtCutHist" , plotname+sample ,"Jet Et Cut", "upperRight", 0.5);
    
//   plotname = "TrackVetoOptimization";
//   make1DOptimizationPlot( "mit-JetVetoStudy-004-hist_HWW" + sample + ".root", "mit-JetVetoStudy-004-hist_ttbar" + sample + ".root", "HWW", "TTBAR" , 0.325/23300, 92.9/11700, "hNEventsPassTrackVetoHist" , plotname+sample ,"NTrack Cut", "upperRight");

//   plotname = "TrackVetoOptimizationAfterJetVeto";
//   make1DOptimizationPlot( "mit-JetVetoStudy-004-hist_HWW" + sample + ".root", "mit-JetVetoStudy-004-hist_ttbar" + sample + ".root", "HWW", "TTBAR" , 0.325/23300, 92.9/11700, "hNEventsPassTrackVetoHistAfterJetVeto" , plotname+sample ,"NTrack Cut", "upperRight");

//   plotname = "JetAndTrackVetoEff";
//   sample = "_ttbar"; 
//   make2DPlot("mit-JetVetoStudy-004-hist_ttbar.root","hJetAndTrackVetoEff",plotname+sample,"colz");

//   plotname = "JetAndTrackVetoEff";
//   sample = "_HWW";
//   make2DPlot("mit-JetVetoStudy-004-hist_HWW.root","hJetAndTrackVetoEff",plotname+sample,"colz" );

//   plotname = "JetAndTrackVetoEff";
//   sample = "_WW";
//   make2DPlot("mit-JetVetoStudy-004-hist_WW.root","hJetAndTrackVetoEff",plotname+sample,"colz");

//   plotname = "JetOrTrackVetoEff";
//   sample = "_ttbar";
//   make2DPlot("mit-JetVetoStudy-004-hist_ttbar.root","hJetOrTrackVetoEff",plotname+sample,"colz");

//   plotname = "JetOrTrackVetoEff";
//   sample = "_HWW";
//   make2DPlot("mit-JetVetoStudy-004-hist_HWW.root","hJetOrTrackVetoEff",plotname+sample,"colz");

//   plotname = "JetOrTrackVetoEff";
//   sample = "_WW";
//   make2DPlot("mit-JetVetoStudy-004-hist_WW.root","hJetOrTrackVetoEff",plotname+sample,"colz");




//   plotname = "JetAndTrackVetoOptimization";
//   sample = "";
//   make2DOptimizationPlot("mit-JetVetoStudy-004-hist_HWW.root","mit-JetVetoStudy-004-hist_ttbar.root",
//                          "hJetAndTrackVetoEff",plotname+sample); 
//   make2DOptimizationPlot("mit-JetVetoStudy-004-hist_HWW.root","mit-JetVetoStudy-004-hist_ttbar.root",
//                          "hNEventsPassJetAndTrackVeto",plotname+sample); 

  return;

}

TH1F* getHisto(string filename, string histoname) {
  TFile *tmpfile = new TFile(filename.c_str(),"READ"); assert(tmpfile);
  tmpfile->FindObjectAny("TAMTopModule");
  tmpfile->FindObjectAny("JetValidationMod");

  const char* histname = histoname.c_str();
  TH1F* tmpHist = (TH1F*) tmpfile->Get(histoname.c_str());  
  tmpHist->SetDirectory(0);
  tmpfile->Close();
  return tmpHist;
}

TH2F* get2DHisto(string filename, string histoname) {
  TFile *tmpfile = new TFile(filename.c_str(),"READ"); assert(tmpfile);
  tmpfile->FindObjectAny("TAMTopModule");

  const char* histname = histoname.c_str();
  TH2F* tmpHist = (TH2F*) tmpfile->Get(histoname.c_str());
  tmpHist->SetDirectory(0);
  tmpfile->Close();
  return tmpHist;
}

void makeEffVsRejectionPlot( vector<string> signalfiles, vector<string> bkgfiles, vector<string> legendNames, double signalWeight, double bkgWeight, string EffHistName, string plotname ,double ymax = -999) {

    TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);
    TLegend *leg1;
    leg1 = new TLegend(0.23,0.72,0.55,0.92);   
    leg1->SetBorderSize(1);
    leg1->SetTextSize(0.03);

  for (int i=0;i<signalfiles.size();i++) {  
    TH1F *signalEff = getHisto( signalfiles[i], EffHistName);
    TH1F *bkgEff = getHisto( bkgfiles[i], EffHistName);
    
    vector<double> eff;    
    vector<double> bkgeff;
    eff.clear();
    bkgeff.clear();
    
    for (int j=1;j<signalEff->GetNbinsX();j++) {           
      eff.push_back(signalEff->GetBinContent(j));
      bkgeff.push_back(bkgEff->GetBinContent(j));   
    }
    
    TGraph *EffVsRejection = new TGraph(signalEff->GetNbinsX(), (Double_t *)&(eff[0]), (Double_t *)&(bkgeff[0]));
    
    EffVsRejection->SetMarkerStyle(kFullSquare);
    EffVsRejection->GetYaxis()->SetTitle("Bkg Efficiency");
    //EffVsRejection->SetLabelOffset(0.001, "Y");
    EffVsRejection->GetXaxis()->SetTitle("Signal Efficiency");
    EffVsRejection->SetMarkerColor(2+i);
    EffVsRejection->SetMarkerSize(0.75);
    
    if (!(ymax == -999))
      EffVsRejection->SetMaximum(ymax);
    
    if (i==0)
      EffVsRejection->Draw("AP");
    else 
      EffVsRejection->Draw("P");
    leg1->AddEntry(EffVsRejection,legendNames[i].c_str(), "P");   
  }
  leg1->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());  
}

void makeComparisonPlot( vector<string> inputFilenames, vector<string> legendNames, string histName, string plotname , bool normalize, string legendLocation = "upperRight" , double ymax = -999) {

  assert(inputFilenames.size() == legendNames.size());

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);
  TLegend *leg1;
  if (legendLocation == "upperRight")
    leg1 = new TLegend(0.75,0.7,0.95,0.9);   
  else if (legendLocation == "upperLeft")
    leg1 = new TLegend(0.23,0.72,0.38,0.92);   
  else if (legendLocation == "lowerRight")
    leg1 = new TLegend(0.70,0.20,0.90,0.40);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);

  double Normalization = 0;
  for (int i=0;i<inputFilenames.size();i++) {
    cerr << inputFilenames[i] << endl;
    TH1F *tmp = getHisto( inputFilenames[i],histName);     
    tmp->SetLineColor(2+i);
    tmp->SetLineWidth(2); 
    //tmp->GetXaxis()->SetRangeUser(0,25);
    if (!(ymax == -999))
      tmp->SetMaximum(ymax);
    if (i==0) {
      Normalization = tmp->Integral();
      tmp->Draw();
    } else {
      if (normalize)
        tmp->DrawNormalized("same",Normalization);
      else 
        tmp->Draw("same");
    }
    leg1->AddEntry(tmp,legendNames[i].c_str(), "FP");   
  }
  leg1->Draw();
  string filename = plotname + ".gif";
  if (normalize)
    filename = plotname + "_normalized.gif";
  cv->SaveAs(filename.c_str());
}

void makeEffVsPurityPlot( string signalfile, string bkgfile, double signalWeight, double bkgWeight, string NEventsHistName, string EffHistName, string plotname ,double ymax = -999) {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);
  
  TH1F *signal = getHisto( signalfile,NEventsHistName);        
  TH1F *bkg = getHisto( bkgfile,NEventsHistName);        
  TH1F *signalEff = getHisto( signalfile, EffHistName);
  
  vector<double> eff;    
  vector<double> purity;
  eff.clear();
  purity.clear();

  for (int j=1;j<signal->GetNbinsX();j++) {
    
    double NSignal = signal->GetBinContent(j) * signalWeight;
    double NRootSigPlusBkg = TMath::Sqrt((signal->GetBinContent(j) * signalWeight + bkg->GetBinContent(j) * bkgWeight));
    double NRootBkg = TMath::Sqrt(bkg->GetBinContent(j) * bkgWeight);
    double NBkg = bkg->GetBinContent(j);
        
    cerr << j << " " << signalEff->GetBinContent(j) << " " << NSignal << " " << NBkg << " ";

    eff.push_back(signalEff->GetBinContent(j));
    if (NSignal+NBkg > 0) {
      purity.push_back(NSignal / (NSignal+NBkg));
      cerr << NSignal / (NSignal+NBkg);
    } else {
      purity.push_back(0.0);
      cerr << " 0 ";
      
    }    
    cerr << endl;
  }

  TGraph *EffVsPurity = new TGraph(signal->GetNbinsX(), (Double_t *)&(eff[0]), (Double_t *)&(purity[0]));
  
  EffVsPurity->SetMarkerStyle(kFullSquare);
  EffVsPurity->GetYaxis()->SetTitle("Sig / (Sig+Bkg)");
  //EffVsPurity->SetLabelOffset(0.001, "Y");
  EffVsPurity->GetXaxis()->SetTitle("Efficiency");
  //EffVsPurity->SetMarkerColor(2+i);
  EffVsPurity->SetMarkerSize(0.75);
 
  if (!(ymax == -999))
    EffVsPurity->SetMaximum(ymax);

  EffVsPurity->Draw("AP");

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
}

void make1DOptimizationPlot( string signalfile, string bkgfile, string signalLegend, string bkgLegend , double signalWeight, double bkgWeight, string histName, string plotname , string xaxisLabel, string legendLocation = "upperRight" , double ymax = -999) {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);
  TLegend *leg1;
  if (legendLocation == "upperRight")
    leg1 = new TLegend(0.75,0.7,0.95,0.9);   
  else if (legendLocation == "upperLeft")
    leg1 = new TLegend(0.23,0.72,0.38,0.92);   
  else if (legendLocation == "lowerRight")
    leg1 = new TLegend(0.70,0.20,0.90,0.40);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);

  double Normalization = 0;
  
  TH1F *signal = getHisto( signalfile,histName);        
  TH1F *bkg = getHisto( bkgfile,histName);        
  
  TH1F *SigOverRootSigPlusBkg = new TH1F("JetVetoOptimization_SOverRootSPlusB",(";" +  xaxisLabel + ";S/sqrt(S+B)").c_str(),signal->GetNbinsX(),signal->GetXaxis()->GetBinLowEdge(1),signal->GetXaxis()->GetBinUpEdge(signal->GetNbinsX()));
  TH1F *SigOverRootBkg = new TH1F("JetVetoOptimization_SOverRootB",(";" +  xaxisLabel + ";S/sqrt(B);JetEtCut").c_str(),signal->GetNbinsX(),signal->GetXaxis()->GetBinLowEdge(1),signal->GetXaxis()->GetBinUpEdge(signal->GetNbinsX()));
  TH1F *SigOverBkg = new TH1F("JetVetoOptimization_SOverB",(";" +  xaxisLabel + ";S/B;JetEtCut").c_str(),signal->GetNbinsX(),signal->GetXaxis()->GetBinLowEdge(1),signal->GetXaxis()->GetBinUpEdge(signal->GetNbinsX()));
  TH1F *SigOverSigPlusBkg = new TH1F("JetVetoOptimization_SOverSPlusB",(";" +  xaxisLabel + ";JetEtCut;S/(S+B)").c_str(),signal->GetNbinsX(),signal->GetXaxis()->GetBinLowEdge(1),signal->GetXaxis()->GetBinUpEdge(signal->GetNbinsX()));

  for (int j=1;j<signal->GetNbinsX();j++) {
    
    double NSignal = signal->GetBinContent(j) * signalWeight;
    double NRootSigPlusBkg = TMath::Sqrt((signal->GetBinContent(j) * signalWeight + bkg->GetBinContent(j) * bkgWeight));
    double NRootBkg = TMath::Sqrt(bkg->GetBinContent(j) * bkgWeight);
    double NBkg = bkg->GetBinContent(j);
    
    cerr << j << " " << NSignal << " " << NRootSigPlusBkg << " " << NRootBkg << " " << NBkg << " ";

    if (bkg->GetBinContent(j) > 0) {
      
      SigOverRootSigPlusBkg->Fill(signal->GetXaxis()->GetBinCenter(j),NSignal/NRootSigPlusBkg);
      SigOverRootBkg->Fill(signal->GetXaxis()->GetBinCenter(j),NSignal/NRootBkg);
      SigOverBkg->Fill(signal->GetXaxis()->GetBinCenter(j),NSignal/NBkg);
      SigOverSigPlusBkg->Fill(signal->GetXaxis()->GetBinCenter(j),NSignal/(NSignal+NBkg));
      cerr << NSignal/NRootSigPlusBkg << " " << NSignal/NRootBkg << " "  << NSignal/NBkg << " " << NSignal/(NBkg+NSignal) ;
//         if (NSignal/NRootBkg > optimalSB) {
//           optimalSB = NSignal/NRootBkg;
//           optimalSBJetEt = yvalue;
//           optimalSBNTracks = xvalue;
//         }
//         if (NSignal/NRootBkgWithSystematics > optimalSB) {
//           optimalSBWithSys = NSignal/NRootBkgWithSystematics;
//           optimalSBWithSysJetEt = yvalue;
//           optimalSBWithSysNTracks = xvalue;
//         }
    } else {
      cerr << "0 0 0";
    }
    cerr << endl;
  }
  
  SigOverRootSigPlusBkg->SetLineColor(2);
  SigOverRootBkg->SetLineColor(3);
  SigOverBkg->SetLineColor(4);
  SigOverSigPlusBkg->SetLineColor(6);


  SigOverRootSigPlusBkg->SetLineWidth(2);
  SigOverRootBkg->SetLineWidth(2);
  SigOverBkg->SetLineWidth(2);
  SigOverSigPlusBkg->SetLineWidth(2);

  if (!(ymax == -999))
    SigOverRootSigPlusBkg->SetMaximum(ymax);

  SigOverRootSigPlusBkg->Draw();
  SigOverRootBkg->Draw("same");
  SigOverBkg->Draw("same");
  SigOverSigPlusBkg->Draw("same");

  leg1->AddEntry(SigOverRootSigPlusBkg,"S/sqrt(S+B)", "FP");   
  leg1->AddEntry(SigOverRootBkg,"S/sqrt(B)", "FP");   
  leg1->AddEntry(SigOverBkg,"S/B", "FP");   
  leg1->AddEntry(SigOverSigPlusBkg,"S/(S+B)", "FP");   

  leg1->Draw();
  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
}


void make2DPlot( string inputFilename, string histName, string plotname, string plottype = "lego") {
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600);
  TH2F *tmp1 = get2DHisto( inputFilename,histName);          
  tmp1->Draw(plottype.c_str());
  
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}

void make2DOptimizationPlot( string signalfile, string bkgfile, string histName, string plotname, string plottype = "lego") {

  TH2F *signal = get2DHisto( signalfile,histName);        
  TH2F *bkg = get2DHisto( bkgfile,histName);        
 
  TH2F *optimization = new TH2F("JetOrTrackVetoOptimization",";NTracks;JetEtCut",21,-0.5,20.5,21,4.5,25.5);
  TH2F *optimizationWithSystematics = new TH2F("JetOrTrackVetoOptimizationWithSystematics",";NTracks;JetEtCut",21,-0.5,20.5,21,4.5,25.5);

  double optimalSB = 0.0;
  double optimalSBJetEt = 0.0;
  double optimalSBNTracks = 0.0;
  double optimalSBWithSys = 0.0;
  double optimalSBWithSysJetEt = 0.0;
  double optimalSBWithSysNTracks = 0.0;

  for (int i=1;i<21;i++) {
    for (int j=1;j<21;j++) {
      
      double xvalue = signal->GetXaxis()->GetBinCenter(i);
      double yvalue = signal->GetYaxis()->GetBinCenter(j);

      //calculate it for 100pb^-1
      // double NSignal = signal->GetBinContent(i,j)*6397 * (0.325/23300) * 100;
//       double NRootBkg = TMath::Sqrt(bkg->GetBinContent(i,j)*3178 * (92.9/11700) * 100);
//       double NRootBkgWithSystematics = TMath::Sqrt(bkg->GetBinContent(i,j)*3178 * (92.9/9000) * 100 + 0.2*0.2*(bkg->GetBinContent(i,j)*3178 * (92.9/9000) * 100)*(bkg->GetBinContent(i,j)*3178 * (92.9/11700) * 100));
      double NSignal = signal->GetBinContent(i,j) * (0.325/23300) * 100;
      double NRootBkg = TMath::Sqrt(bkg->GetBinContent(i,j) * (92.9/11700) * 100);
      double NRootBkgWithSystematics = TMath::Sqrt(bkg->GetBinContent(i,j) * (92.9/9000) * 100 + 0.2*0.2*(bkg->GetBinContent(i,j)*3178 * (92.9/9000) * 100)*(bkg->GetBinContent(i,j)*3178 * (92.9/11700) * 100));

      cerr << xvalue << " " << yvalue << " " << NSignal << " " << NRootBkg << " " << NRootBkgWithSystematics << " ";
      if (bkg->GetBinContent(i,j) > 0) {
        optimization->Fill(xvalue,yvalue,NSignal/NRootBkg);
        optimizationWithSystematics->Fill(xvalue,yvalue,NSignal/NRootBkgWithSystematics);

        cerr << NSignal/NRootBkg << " " << NSignal/NRootBkgWithSystematics;
        if (NSignal/NRootBkg > optimalSB) {
          optimalSB = NSignal/NRootBkg;
          optimalSBJetEt = yvalue;
          optimalSBNTracks = xvalue;
        }
        if (NSignal/NRootBkgWithSystematics > optimalSB) {
          optimalSBWithSys = NSignal/NRootBkgWithSystematics;
          optimalSBWithSysJetEt = yvalue;
          optimalSBWithSysNTracks = xvalue;
        }
      } else {
        cerr << "0 0";
      }
      cerr << endl;
    }
  }

  cerr << "optimalSB : " << optimalSB << " " << optimalSBNTracks << " " <<  optimalSBJetEt << endl;
  cerr << "optimalSBWithSys : " << optimalSBWithSys << " " << optimalSBWithSysNTracks << " " <<  optimalSBWithSysJetEt << endl;

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600); 
  optimization->Draw(plottype.c_str());
  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

//   TCanvas *cv = MakeCanvas("cv", plotname.c_str(),800,600); 
//   optimizationWithSystematics->Draw("lego");
//   filename = plotname + "WithSystematics.gif";
//   cv->SaveAs(filename.c_str());
//   cv->Delete();
}
