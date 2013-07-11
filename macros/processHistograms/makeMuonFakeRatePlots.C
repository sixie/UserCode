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

#include <deque.h>

//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void makeMuonFakeRatePlots()
{
  //------------------------------------------------------------------------------------------------
  // Process the full set of dimuon mass histograms which come in predefined bins of the dimuon pT
  // to obtain in the end a distribution showing the measured dimuon mass versus its pT.
  //------------------------------------------------------------------------------------------------
  // access the histogram file and load TAMTopModule to load into memory
  gStyle->SetPalette(1);

  bool     doLeg   = true;
  double   legx[2] = { 0.15, 0.45 };
  double   legy[2] = { 0.65, 0.85 };
  double   legTextSize = 0.03;
  TString legTitle = "";

  string sample = "";
  string plotname;
  string filename;

  string inputfilename = "/server/02a/sixie/CMSSW_2_1_6/src/mit-MuonFakeRate_QCD-hist.root";
  string moduleName = "MuonFakeRateAnalysisMod";
  vector<string> flavorNames;
  flavorNames.push_back("");
  flavorNames.push_back("LightFlavor");
  flavorNames.push_back("CharmFlavor");
  flavorNames.push_back("BottomFlavor");
  flavorNames.push_back("UnmatchedFlavor");

  plotname = "MuonFakeRate";
  filename = plotname + ".gif";
  TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 

  TObjArray MuonPtHist(100);
  TObjArray DenominatorPtHist(100);
  TObjArray FakeRateVsPt(100);
  TObjArray MuonFakeRates(100);

  for (int i=0;i<flavorNames.size();i++) {
    TH1F* numerator = getHisto( inputfilename,moduleName,"h" + flavorNames[i] + "MuonPtHist") ; 
    double numeratorEntries = numerator->GetEntries();  
    TH1F* denominator = getHisto( inputfilename,moduleName,"h" + flavorNames[i] + "IsolatedTrackPtHist") ;
    denominator->Draw("same");
    
    numerator->Sumw2();
    denominator->Sumw2();
    
    //Rebinning Code from Kristian
    std::deque<double> bins,tmpbins,newbins;
    std::deque<double> contents, newcontents, denom, newdenom;
    std::deque<double> fakerate;
    std::deque<double> error_x,error_y;
    for( int b=0; b<numerator->GetNbinsX()+2; b++ ) {
      bins.push_back(numerator->GetBinLowEdge(b));
      contents.push_back(numerator->GetBinContent(b));
      denom.push_back(denominator->GetBinContent(b));
      cout << b << " " << numerator->GetBinLowEdge(b) << " " << numerator->GetBinContent(b) 
           << " " << denominator->GetBinContent(b) << endl;
    }

    double binWidth = numerator->GetBinLowEdge(1) - numerator->GetBinLowEdge(0);
    cout << "binwidth = " << binWidth << endl;
    for( int b=bins.size()-1; b>0; b-- ) {
      double sum=contents[b]; 
      double dsum=denom[b];
      double binSize = 0;
      double lowerBinEdge = bins[b];
      while( sum<5 && b>0 || binSize < 5) {
        b--;
        sum += contents[b];
        dsum += denom[b];
        binSize += binWidth;
      }
      newbins.push_front( lowerBinEdge - (lowerBinEdge-bins[b]) / 2.0);
      newcontents.push_front( sum );
      newdenom.push_front( dsum );
      
      double fr = (sum == 0 && dsum == 0)?0:(double)sum/dsum;
      fakerate.push_front(fr);
      error_x.push_front(binSize/2);
      error_y.push_front((sum == 0 && dsum == 0)?0:sqrt( fr*(1-fr)/dsum));
      cout << b << " " << lowerBinEdge << " " << lowerBinEdge - (lowerBinEdge-bins[b])/2 << " " 
           << binSize << " " << sum << " " << dsum << " " << fr << endl;
    }
    
    for( int b=0; b<newbins.size(); b++ ) {
      printf( "bin: %i\tedge: %d +/- %d\tnumer: %i\tF.R.: %f +/- %f\n", 
              b,newbins[b],error_x[b],newcontents[b],fakerate[b],error_y[b]);
    }
    cout << endl;
    for( int b=0; b<newbins.size(); b++ ) {
      printf( "fTightBins.push_back(%d); fTightFR.push_back(%f);\n", 
              newbins[b],fakerate[b]);
    }
    cout << endl;   
    
    TGraphErrors * muonFakeRate = new TGraphErrors ( (Int_t)newbins.size(),
                                                    (Double_t *)&(newbins[0]),
                                                    (Double_t *)&(fakerate[0]),
                                                    (Double_t *)&(error_x[0]),
                                                    (Double_t *)&(error_y[0]));
    muonFakeRate->SetMarkerStyle(kFullSquare);

    muonFakeRate->GetYaxis()->SetTitle("Muon Fake Rate");

    MuonFakeRates->Add((TGraphErrors*)muonFakeRate->Clone());
 
    bins.clear(); newbins.clear();
    contents.clear(); newcontents.clear();
    denom.clear(), newdenom.clear();
    error_x.clear(); error_y.clear();
  }  

  TCanvas *cv = new TCanvas("cv", "Muon Fake Rate" , 800,600); 
  ((TGraphErrors*)MuonFakeRates[0])->SetMarkerColor(2);
  ((TGraphErrors*)MuonFakeRates[0])->GetYaxis()->SetRangeUser(0.0,1.1);
//  ((TH1F*)MuonFakeRates[0])->SetMaximum(2.0);
//  ((TH1F*)MuonFakeRates[0])->SetMinimum(0.5);
  ((TGraphErrors*)MuonFakeRates[0])->Draw("AP");  
  for (int i=1; i<flavorNames.size() ; i++) {
//    ((TH1F*)MuonFakeRates[i])->SetMarkerStyle(20);
    ((TGraphErrors*)MuonFakeRates[i])->SetMarkerColor(2+i);
//    ((TH1F*)MuonFakeRates[0])->SetMaximum(2.0);
//    ((TH1F*)MuonFakeRates[0])->SetMinimum(0.5);
    ((TGraphErrors*)MuonFakeRates[i])->Draw("P");  
    cout << i << endl;
  }

  TLegend *leg=0;
  leg = new TLegend(legx[0],legy[0],legx[1],legy[1]);   
  leg->SetBorderSize(1);
  leg->SetTextSize(legTextSize);   
  leg->AddEntry(MuonFakeRates[0],"All Flavors", "LP"); 
  for (int i=1; i<flavorNames.size(); i++)    
    leg->AddEntry(MuonFakeRates[i],flavorNames[i].c_str(), "LP"); 
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
