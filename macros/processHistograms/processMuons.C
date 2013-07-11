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
void processMuons()
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

  string filenameRealMuons = "mit-muonIDs-hist_Zmumu_" + sample + ".root";
  string filenameFakeMuons = "mit-muonIDs-hist_QCD120-170_" + sample + ".root";
  string moduleName = "MuonIDMod";

  //for normalizing them properly. have to get crossection and #of events
  double RealMuonsScaleFactor = (1.784*(1.0/9.0)) / 24999.0 ;
  double FakeMuonsScaleFactor = 21600.0 / 21699.0;


  plotname = "MuonChi2PerNdof";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonChi2PerNdof") ; 
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonChi2PerNdof") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();


  plotname = "MuonNHits";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonNHits") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonNHits") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();


  plotname = "BestTrackD0";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonBestTrackD0") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonBestTrackD0") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();


  plotname = "IsoR03SumPt";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonIsoR03SumPt") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonIsoR03SumPt") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();


 plotname = "IsoR03SumPtOverPt";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonIsoR03SumPtOverPt") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonIsoR03SumPtOverPt") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();


  plotname = "IsoR03EmEt";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonIsoR03EmEt") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonIsoR03EmEt") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "IsoR03HadEt";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonIsoR03HadEt") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonIsoR03HadEt") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "IsoR03EmAndHadEt";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonIsoR03EmAndHadEt") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonIsoR03EmAndHadEt") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();


  plotname = "IsoR03HoEt";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonIsoR03HoEt") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonIsoR03HoEt") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "IsoR03NTracks";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonIsoR03NTracks") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonIsoR03NTracks") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();


  plotname = "IsoR03NJets";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonIsoR03NJets") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonIsoR03NJets") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();


  plotname = "EmEnergy";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonEmEnergy") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonEmEnergy") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();


  plotname = "HadEnergy";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonHadEnergy") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonHadEnergy") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();


  plotname = "HoEnergy";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonHoEnergy") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonHoEnergy") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();


  plotname = "EmS9Energy";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonEmS9Energy") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonEmS9Energy") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();


  plotname = "HadS9Energy";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonHadS9Energy") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonHadS9Energy") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();


  plotname = "HoS9Energy";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonHoS9Energy") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonHoS9Energy") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();

  plotname = "MuonClass";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonClass") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonClass") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();


  plotname = "MuonPt";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonPtHist") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonPtHist") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();


  plotname = "MuonEta";
  filename = plotname + "_" + sample + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  TH1F* h0 = getHisto( filenameRealMuons,moduleName,"hRealMuonEtaHist") ;
  double h0Entries = h0->GetEntries();  
  h0->SetLineColor(4);
  h0->SetLineWidth(2);
  cv->SetLogy();
  h0->Draw();

  TH1F* h1 = getHisto( filenameRealMuons,moduleName,"hFakeMuonEtaHist") ;
  h1->Scale(h0Entries/h1->GetEntries());
  h1->SetLineColor(2);
  h1->SetLineWidth(2);  
  h1->Draw("same");

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);
  leg->AddEntry(h0, "Real Muons", "LP"); 
  leg->AddEntry(h1, "Fake Muons", "LP"); 
  leg->Draw();

  cv->SaveAs(filename.c_str());
  cv->Delete();

  return;

}

TH1F* getHisto(string filename, string modulename, string histoname) {
  TFile *tmpfile = new TFile(filename.c_str(),"READ"); assert(tmpfile);
  tmpfile->FindObjectAny("TAMTopModule");
  tmpfile->FindObjectAny(modulename.c_str());
  const char* histname = histoname.c_str();
  TH1F* tmpHist = (TH1F*) tmpfile->Get(histoname.c_str());  
  tmpHist->SetDirectory(0);
  tmpfile->Close();
  return tmpHist;
}
