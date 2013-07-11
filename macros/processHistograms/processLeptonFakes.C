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



//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void processLeptonFakes()
{
  //------------------------------------------------------------------------------------------------
  // Process the full set of dimuon mass histograms which come in predefined bins of the dimuon pT
  // to obtain in the end a distribution showing the measured dimuon mass versus its pT.
  //------------------------------------------------------------------------------------------------
  // access the histogram file and load TAMTopModule to load into memory
  gStyle->SetPalette(1);

  bool     doLeg   = true;
  double   legx[2] = { 0.5, 0.7 };
  double   legy[2] = { 0.7, 0.8 };
  double   legTextSize = 0.03;
  TString legTitle = "";

//   string sample = "centralGlobalMuons";
  string sample = "forwardGlobalMuons";
  string plotname;
  string filename;

  string filenameLeptonPlusFO = "mit-FakeLeptonBkg_Wmunu-hist.root";
  string filenameDilepton = "mit-FakeLeptonBkg_Wmunu-hist.root";
  string moduleName = "LeptonFakeBkgMod";

  //for normalizing them properly. have to get crossection and #of events


  plotname = "DileptonMass";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameLeptonPlusFO,moduleName,"hDileptonMass") ; 
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  //cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameDilepton,moduleName,"hDileptonMass") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  //h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "l+fo", "LP"); 
  leg->AddEntry(h1, "l+l", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "LeptonPtMax";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameLeptonPlusFO,moduleName,"hLeptonPtMax") ; 
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  //cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameDilepton,moduleName,"hLeptonPtMax") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  //h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "l+fo", "LP"); 
  leg->AddEntry(h1, "l+l", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "LeptonPtMin";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameLeptonPlusFO,moduleName,"hLeptonPtMin") ; 
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  //cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameDilepton,moduleName,"hLeptonPtMin") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  //h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "l+fo", "LP"); 
  leg->AddEntry(h1, "l+l", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "DeltaPhiLeptons";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameLeptonPlusFO,moduleName,"hDeltaPhiLeptons") ; 
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  //cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameDilepton,moduleName,"hDeltaPhiLeptons") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  //h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "l+fo", "LP"); 
  leg->AddEntry(h1, "l+l", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "NGoodJetsFailLeptonSelection";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameLeptonPlusFO,moduleName,"hNGoodJetsFailLeptonSelection") ; 
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  //cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameDilepton,moduleName,"hNGoodJetsFailLeptonSelection") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  //h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "l+fo", "LP"); 
  leg->AddEntry(h1, "l+l", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();

  return;

}

TH1F* getHisto(string filename, string modulename, string histoname) {
  TFile *tmpfile = new TFile(filename.c_str(),"READ"); assert(tmpfile);
  tmpfile->FindObjectAny("TAMTopModule");
  tmpfile->FindObjectAny("ObjectCleaningMod");
  tmpfile->FindObjectAny(modulename.c_str());
  const char* histname = histoname.c_str();
  TH1F* tmpHist = (TH1F*) tmpfile->Get(histoname.c_str());  
  tmpHist->SetDirectory(0);
  tmpfile->Close();
  return tmpHist;
}
