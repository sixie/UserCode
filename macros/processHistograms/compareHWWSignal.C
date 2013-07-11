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
void compareHWWSignal()
{
  //------------------------------------------------------------------------------------------------
  // Process the full set of dimuon mass histograms which come in predefined bins of the dimuon pT
  // to obtain in the end a distribution showing the measured dimuon mass versus its pT.
  //------------------------------------------------------------------------------------------------
  // access the histogram file and load TAMTopModule to load into memory
  gStyle->SetPalette(1);

  bool     doLeg   = true;
  double   legx[2] = { 0.5, 0.75 };
  double   legy[2] = { 0.7, 0.8 };
  double   legTextSize = 0.03;
  TString legTitle = "";

//   string sample = "centralGlobalMuons";
//  string sample = "forwardGlobalMuons";
  string sample = "";
  string plotname;
  string filename;

  string filename1 = "mit-HWWSelection-003-hist_HWWSignal.root";
  string filename2 = "/home/ceballos/releases/CMSSW_1_6_12/histo_H165_WW_2l-CSA07.root";

  plotname = "HWWSelectionStacked";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hHWWSelection");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_0");        
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries();
  tmp2->Scale(tmp1->GetBinContent(1)/tmp2->GetBinContent(1));
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->Draw("same");
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();


  plotname = "MaxLeptonPt";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hLeptonPtMax");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_1");        
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries();
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->DrawNormalized("same",tmp1Entries);
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "MinLeptonPt";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hLeptonPtMin");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_2");        
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries();
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->DrawNormalized("same",tmp1Entries);
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "LeptonEta";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hLeptonEta");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_3");        
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries();
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->DrawNormalized("same",tmp1Entries);
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "NCentralJets";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hNCentralJets");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_4");  
  //tmp1->GetYaxis()->SetRangeUser(0.0,1500);
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries();
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->DrawNormalized("same",tmp1Entries);
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "Met";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hMetPtAfterSelectionHist");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_5");        
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries();
  tmp2->Scale(tmp1Entries/tmp2->GetEntries());
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->Draw("same");
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "DeltaPhiLeptons";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hDeltaPhiLeptons");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_7");        
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries();  
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->DrawNormalized("same",tmp1Entries);
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "DileptonMass";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hDileptonMass");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_6");        
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries(); 
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->DrawNormalized("same",tmp1Entries);
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "hLepton1PtAfterCuts";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hLepton1Pt_afterCuts");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_8");        
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries(); 
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->DrawNormalized("same",tmp1Entries);
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "hLepton2PtAfterCuts";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hLepton2Pt_afterCuts");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_9");        
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries();  
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->DrawNormalized("same",tmp1Entries);
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "mTHiggs";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hMtHiggs");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_10");        
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries();  
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->DrawNormalized("same",tmp1Entries);
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "DeltaEtaLeptons";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hDeltaEtaLeptons");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_11");        
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries();  
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->DrawNormalized("same",tmp1Entries);
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "MinDeltaPhiLeptonMet";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hMinDeltaPhiLeptonMet");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_14");        
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries();  
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->DrawNormalized("same",tmp1Entries);
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "MetSignificanceAfterCuts";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hMetSignificanceAfterCuts");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_12");        
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries();  
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->DrawNormalized("same",tmp1Entries);
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "SumEtAfterCuts";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hSumEtAfterCuts");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_13");        
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries();  
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->DrawNormalized("same",tmp1Entries);
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();


  plotname = "MtLepton1";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hMtLepton1");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_16");        
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries();  
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->DrawNormalized("same",tmp1Entries);
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "MtLepton2";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F *tmp1 = getHisto1( filename1,"hMtLepton2");        
  TH1F *tmp2 = getHisto2( filename2,"hDHWWSel_15");        
  tmp1->SetLineColor(4);
  tmp1->SetLineWidth(2);
  tmp1->Draw(); 
  double tmp1Entries = tmp1->GetEntries();  
  tmp2->SetLineColor(2);
  tmp2->SetLineWidth(2);
  tmp2->DrawNormalized("same",tmp1Entries);
  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(tmp1, "CMSSW 2_1_X", "LP"); 
  leg->AddEntry(tmp2, "CMSSW_1_6_12", "LP"); 
  leg->Draw();
  cv->SaveAs(filename.c_str());
  cv->Delete();

  return;

}

TH1F* getHisto1(string filename, string histoname) {
  TFile *tmpfile = new TFile(filename.c_str(),"READ"); assert(tmpfile);
  tmpfile->FindObjectAny("TAMTopModule");
  tmpfile->FindObjectAny("ObjectCleaning");
  tmpfile->FindObjectAny("HwwEvtSelMod");

  const char* histname = histoname.c_str();
  TH1F* tmpHist = (TH1F*) tmpfile->Get(histoname.c_str());  
  tmpHist->SetDirectory(0);
  tmpfile->Close();
  return tmpHist;
}

TH1F* getHisto2(string filename, string histoname) {
  TFile *tmpfile = new TFile(filename.c_str(),"READ"); assert(tmpfile);  
  const char* histname = histoname.c_str();
  TH1F* tmpHist = (TH1F*) tmpfile->Get(histoname.c_str());  
  tmpHist->SetDirectory(0);
  tmpfile->Close();
  return tmpHist;
}
