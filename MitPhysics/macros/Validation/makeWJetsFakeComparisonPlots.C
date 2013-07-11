//root -l $CMSSW_BASE/src/MitPhysics/macros/makeWJetsFakeComparisonPlots.C+


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


void makeComparison(string plotname, string histName, 
                    vector<string> DatasetFilesA, 
                    vector<string> DatasetNamesA,
                    string legendLabelA,
                    vector<string> DatasetFilesB, 
                    vector<string> DatasetNamesB,
                    string legendLabelB ) {
  
  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  TH1F *tmpHistA = addAllSamples(DatasetFilesA, DatasetNamesA, "WJetsFakeValidationMod",histName);
  TH1F *tmpHistB = addAllSamples(DatasetFilesB, DatasetNamesB,
                              "WJetsFakeValidationMod",histName);

  TH1F *HistA = (TH1F*)tmpHistA->Rebin(5);
  TH1F *HistB = (TH1F*)tmpHistB->Rebin(5);

  HistA->SetLineColor(kRed);
  HistA->SetMarkerSize(0.5);
  HistA->SetLineWidth(2);  
  HistB->SetLineColor(kBlue);
  HistB->SetMarkerSize(0.5);
  HistB->SetLineWidth(2);  

  //make legend
  TLegend *leg1=0;
  leg1 = new TLegend(0.5,0.6,0.7,0.8);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);
  leg1->AddEntry(HistA, legendLabelA.c_str(), "LP"); 
  leg1->AddEntry(HistB, legendLabelB.c_str(), "LP"); 

  //Draw histograms
  HistA->Draw();
  HistB->Draw("same");
  leg1->Draw();

  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}


//--------------------------------------------------------------------------------------------------
// Compare all histograms of Pythia vs Madgraph
//--------------------------------------------------------------------------------------------------
void makeComparisonPythiaVsMadgraph( vector<string> fakeHistogramFiles_pythia, 
                                     vector<string> fakeDatasetNames_pythia,
                                     vector<string> fakeHistogramFiles_madgraph, 
                                     vector<string> fakeDatasetNames_madgraph,
                                     string additionalLabel = "") {
  
  //***********************************************************************************************
  // Distributions of kinematics of the Fake object
  //***********************************************************************************************  
  //Electrons
  makeComparison("ElectronDenominatorPtHist_FR_pythiaVsMadgraph"+additionalLabel, "hElectronDenominatorPt", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("ElectronDenominatorEtaHist_FR_pythiaVsMadgraph"+additionalLabel, "hElectronDenominatorEta", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("ElectronDenominatorPhiHist_FR_pythiaVsMadgraph"+additionalLabel, "hElectronDenominatorPhi", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("FakeElectronPtHist_FR_pythiaVsMadgraph"+additionalLabel, "hFakeElectronPt", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("FakeElectronEtaHist_FR_pythiaVsMadgraph"+additionalLabel, "hFakeElectronEta", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("FakeElectronPhiHist_FR_pythiaVsMadgraph"+additionalLabel, "hFakeElectronPhi", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");

  //Muons
  makeComparison("MuonDenominatorPtHist_FR_pythiaVsMadgraph"+additionalLabel, "hMuonDenominatorPt", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MuonDenominatorEtaHist_FR_pythiaVsMadgraph"+additionalLabel, "hMuonDenominatorEta", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
   makeComparison("MuonDenominatorPhiHist_FR_pythiaVsMadgraph"+additionalLabel, "hMuonDenominatorPhi", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("FakeMuonPtHist_FR_pythiaVsMadgraph"+additionalLabel, "hFakeMuonPt", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("FakeMuonEtaHist_FR_pythiaVsMadgraph"+additionalLabel, "hFakeMuonEta", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("FakeMuonPhiHist_FR_pythiaVsMadgraph"+additionalLabel, "hFakeMuonPhi", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");

  //***********************************************************************************************
  // Distributions with no Cuts
  //***********************************************************************************************  
  makeComparison("LeptonPtMax_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtMax", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("LeptonPtMin_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtMin", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MetPtHist_FR_pythiaVsMadgraph"+additionalLabel, "hMetPtHist", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MetPhiHist_FR_pythiaVsMadgraph"+additionalLabel, "hMetPhiHist", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("UncorrMetPtHist_FR_pythiaVsMadgraph"+additionalLabel, "hUncorrMetPtHist", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("UncorrMetPhiHist_FR_pythiaVsMadgraph"+additionalLabel, "hUncorrMetPhiHist", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DeltaPhiLeptons_FR_pythiaVsMadgraph"+additionalLabel, "hDeltaPhiLeptons", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DeltaEtaLeptons_FR_pythiaVsMadgraph"+additionalLabel, "hDeltaEtaLeptons", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DileptonMass_FR_pythiaVsMadgraph"+additionalLabel, "hDileptonMass", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  

  makeComparison("LeptonPtMax_ee_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtMax_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("LeptonPtMin_ee_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtMin_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MetPtHist_ee_FR_pythiaVsMadgraph"+additionalLabel, "hMetPtHist_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MetPhiHist_ee_FR_pythiaVsMadgraph"+additionalLabel, "hMetPhiHist_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("UncorrMetPtHist_ee_FR_pythiaVsMadgraph"+additionalLabel, "hUncorrMetPtHist_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("UncorrMetPhiHist_ee_FR_pythiaVsMadgraph"+additionalLabel, "hUncorrMetPhiHist_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DeltaPhiLeptons_ee_FR_pythiaVsMadgraph"+additionalLabel, "hDeltaPhiLeptons_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DeltaEtaLeptons_ee_FR_pythiaVsMadgraph"+additionalLabel, "hDeltaEtaLeptons_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DileptonMass_ee_FR_pythiaVsMadgraph"+additionalLabel, "hDileptonMass_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
 

  makeComparison("LeptonPtMax_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtMax_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("LeptonPtMin_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtMin_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MetPtHist_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hMetPtHist_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MetPhiHist_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hMetPhiHist_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("UncorrMetPtHist_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hUncorrMetPtHist_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("UncorrMetPhiHist_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hUncorrMetPhiHist_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DeltaPhiLeptons_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hDeltaPhiLeptons_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DeltaEtaLeptons_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hDeltaEtaLeptons_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DileptonMass_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hDileptonMass_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
 
  makeComparison("LeptonPtMax_emu_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtMax_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("LeptonPtMin_emu_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtMin_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MetPtHist_emu_FR_pythiaVsMadgraph"+additionalLabel, "hMetPtHist_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MetPhiHist_emu_FR_pythiaVsMadgraph"+additionalLabel, "hMetPhiHist_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("UncorrMetPtHist_emu_FR_pythiaVsMadgraph"+additionalLabel, "hUncorrMetPtHist_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("UncorrMetPhiHist_emu_FR_pythiaVsMadgraph"+additionalLabel, "hUncorrMetPhiHist_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DeltaPhiLeptons_emu_FR_pythiaVsMadgraph"+additionalLabel, "hDeltaPhiLeptons_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DeltaEtaLeptons_emu_FR_pythiaVsMadgraph"+additionalLabel, "hDeltaEtaLeptons_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DileptonMass_emu_FR_pythiaVsMadgraph"+additionalLabel, "hDileptonMass_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
 

  //***********************************************************************************************
  // N-1 cut Distributions 
  //***********************************************************************************************  
  makeComparison("LeptonPtMax_NMinusOne_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtMax_NMinusOne", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("LeptonPtMin_NMinusOne_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtMin_NMinusOne", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MetPtHist_NMinusOne_FR_pythiaVsMadgraph"+additionalLabel, "hMetPtHist_NMinusOne", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MetPhiHist_NMinusOne_FR_pythiaVsMadgraph"+additionalLabel, "hMetPhiHist_NMinusOne", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("METdeltaPhilEtHist_NMinusOne_FR_pythiaVsMadgraph"+additionalLabel, "hMETdeltaPhilEtHist_NMinusOne", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("NCentralJets_NMinusOne_FR_pythiaVsMadgraph"+additionalLabel, "hNCentralJets_NMinusOne", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("NDirtyMuonsHist_NMinusOne_FR_pythiaVsMadgraph"+additionalLabel, "hNDirtyMuonsHist_NMinusOne", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("NCleanExtraTracksHist_NMinusOne_FR_pythiaVsMadgraph"+additionalLabel, "hNCleanExtraTracksHist_NMinusOne", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DeltaPhiLeptons_NMinusOne_FR_pythiaVsMadgraph"+additionalLabel, "hDeltaPhiLeptons_NMinusOne", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DeltaEtaLeptons_NMinusOne_FR_pythiaVsMadgraph"+additionalLabel, "hDeltaEtaLeptons_NMinusOne", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DileptonMass_NMinusOne_FR_pythiaVsMadgraph"+additionalLabel, "hDileptonMass_NMinusOne", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MinDeltaPhiLeptonMet_NMinusOne_FR_pythiaVsMadgraph"+additionalLabel, "hMinDeltaPhiLeptonMet_NMinusOne", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
 
  makeComparison("LeptonPtMax_NMinusOne_ee_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtMax_NMinusOne_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("LeptonPtMin_NMinusOne_ee_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtMin_NMinusOne_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MetPtHist_NMinusOne_ee_FR_pythiaVsMadgraph"+additionalLabel, "hMetPtHist_NMinusOne_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MetPhiHist_NMinusOne_ee_FR_pythiaVsMadgraph"+additionalLabel, "hMetPhiHist_NMinusOne_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("METdeltaPhilEtHist_NMinusOne_ee_FR_pythiaVsMadgraph"+additionalLabel, "hMETdeltaPhilEtHist_NMinusOne_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("NCentralJets_NMinusOne_ee_FR_pythiaVsMadgraph"+additionalLabel, "hNCentralJets_NMinusOne_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("NDirtyMuonsHist_NMinusOne_ee_FR_pythiaVsMadgraph"+additionalLabel, "hNDirtyMuonsHist_NMinusOne_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("NCleanExtraTracksHist_NMinusOne_ee_FR_pythiaVsMadgraph"+additionalLabel, "hNCleanExtraTracksHist_NMinusOne_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DeltaPhiLeptons_NMinusOne_ee_FR_pythiaVsMadgraph"+additionalLabel, "hDeltaPhiLeptons_NMinusOne_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DeltaEtaLeptons_NMinusOne_ee_FR_pythiaVsMadgraph"+additionalLabel, "hDeltaEtaLeptons_NMinusOne_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DileptonMass_NMinusOne_ee_FR_pythiaVsMadgraph"+additionalLabel, "hDileptonMass_NMinusOne_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MinDeltaPhiLeptonMet_NMinusOne_ee_FR_pythiaVsMadgraph"+additionalLabel, "hMinDeltaPhiLeptonMet_NMinusOne_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");

  makeComparison("LeptonPtMax_NMinusOne_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtMax_NMinusOne_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("LeptonPtMin_NMinusOne_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtMin_NMinusOne_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MetPtHist_NMinusOne_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hMetPtHist_NMinusOne_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MetPhiHist_NMinusOne_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hMetPhiHist_NMinusOne_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  //makeComparison("METdeltaPhilEtHist_NMinusOne_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hMETdeltaPhilEtHist_NMinusOne_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("NCentralJets_NMinusOne_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hNCentralJets_NMinusOne_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("NDirtyMuonsHist_NMinusOne_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hNDirtyMuonsHist_NMinusOne_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("NCleanExtraTracksHist_NMinusOne_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hNCleanExtraTracksHist_NMinusOne_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DeltaPhiLeptons_NMinusOne_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hDeltaPhiLeptons_NMinusOne_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DeltaEtaLeptons_NMinusOne_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hDeltaEtaLeptons_NMinusOne_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DileptonMass_NMinusOne_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hDileptonMass_NMinusOne_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MinDeltaPhiLeptonMet_NMinusOne_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hMinDeltaPhiLeptonMet_NMinusOne_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");

  makeComparison("LeptonPtMax_NMinusOne_emu_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtMax_NMinusOne_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("LeptonPtMin_NMinusOne_emu_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtMin_NMinusOne_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MetPtHist_NMinusOne_emu_FR_pythiaVsMadgraph"+additionalLabel, "hMetPtHist_NMinusOne_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MetPhiHist_NMinusOne_emu_FR_pythiaVsMadgraph"+additionalLabel, "hMetPhiHist_NMinusOne_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("METdeltaPhilEtHist_NMinusOne_emu_FR_pythiaVsMadgraph"+additionalLabel, "hMETdeltaPhilEtHist_NMinusOne_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("NCentralJets_NMinusOne_emu_FR_pythiaVsMadgraph"+additionalLabel, "hNCentralJets_NMinusOne_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("NDirtyMuonsHist_NMinusOne_emu_FR_pythiaVsMadgraph"+additionalLabel, "hNDirtyMuonsHist_NMinusOne_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("NCleanExtraTracksHist_NMinusOne_emu_FR_pythiaVsMadgraph"+additionalLabel, "hNCleanExtraTracksHist_NMinusOne_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DeltaPhiLeptons_NMinusOne_emu_FR_pythiaVsMadgraph"+additionalLabel, "hDeltaPhiLeptons_NMinusOne_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  //makeComparison("DeltaEtaLeptons_NMinusOne_emu_FR_pythiaVsMadgraph"+additionalLabel, "hDeltaEtaLeptons_NMinusOne_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("DileptonMass_NMinusOne_emu_FR_pythiaVsMadgraph"+additionalLabel, "hDileptonMass_NMinusOne_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MinDeltaPhiLeptonMet_NMinusOne_emu_FR_pythiaVsMadgraph"+additionalLabel, "hMinDeltaPhiLeptonMet_NMinusOne_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");

  //***********************************************************************************************
  // Distributions with no Cuts
  //***********************************************************************************************  
  makeComparison("MinDeltaPhiLeptonMet_afterCuts_FR_pythiaVsMadgraph"+additionalLabel, "hMinDeltaPhiLeptonMet_afterCuts", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("MtLepton1_afterCuts_FR_pythiaVsMadgraph"+additionalLabel, "hMtLepton1_afterCuts", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("MtLepton2_afterCuts_FR_pythiaVsMadgraph"+additionalLabel, "hMtLepton2_afterCuts", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("MtHiggs_afterCuts_FR_pythiaVsMadgraph"+additionalLabel, "hMtHiggs_afterCuts", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("LeptonPtPlusMet_afterCuts_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtPlusMet_afterCuts", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 

  makeComparison("MinDeltaPhiLeptonMet_afterCuts_ee_FR_pythiaVsMadgraph"+additionalLabel, "hMinDeltaPhiLeptonMet_afterCuts_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("MtLepton1_afterCuts_ee_FR_pythiaVsMadgraph"+additionalLabel, "hMtLepton1_afterCuts_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("MtLepton2_afterCuts_ee_FR_pythiaVsMadgraph"+additionalLabel, "hMtLepton2_afterCuts_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("MtHiggs_afterCuts_ee_FR_pythiaVsMadgraph"+additionalLabel, "hMtHiggs_afterCuts_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("LeptonPtPlusMet_afterCuts_ee_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtPlusMet_afterCuts_ee", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 

  makeComparison("MinDeltaPhiLeptonMet_afterCuts_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hMinDeltaPhiLeptonMet_afterCuts_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR");
  makeComparison("MtLepton1_afterCuts_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hMtLepton1_afterCuts_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("MtLepton2_afterCuts_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hMtLepton2_afterCuts_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("MtHiggs_afterCuts_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hMtHiggs_afterCuts_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("LeptonPtPlusMet_afterCuts_mumu_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtPlusMet_afterCuts_mumu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 

  makeComparison("MinDeltaPhiLeptonMet_afterCuts_emu_FR_pythiaVsMadgraph"+additionalLabel, "hMinDeltaPhiLeptonMet_afterCuts_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("MtLepton1_afterCuts_emu_FR_pythiaVsMadgraph"+additionalLabel, "hMtLepton1_afterCuts_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("MtLepton2_afterCuts_emu_FR_pythiaVsMadgraph"+additionalLabel, "hMtLepton2_afterCuts_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("MtHiggs_afterCuts_emu_FR_pythiaVsMadgraph"+additionalLabel, "hMtHiggs_afterCuts_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 
  makeComparison("LeptonPtPlusMet_afterCuts_emu_FR_pythiaVsMadgraph"+additionalLabel, "hLeptonPtPlusMet_afterCuts_emu", fakeHistogramFiles_pythia,fakeDatasetNames_pythia,"Pythia FR", fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph, "Madgraph FR"); 


}

//--------------------------------------------------------------------------------------------------
// make plots comparing FR method with MC Fakes
//--------------------------------------------------------------------------------------------------
void makeComparisonFRVsMCFake ( vector<string> fakeHistogramFiles, 
                                vector<string> fakeDatasetNames,
                                vector<string> simulationFakeHistogramFiles, 
                                vector<string> simulationFakeDatasetNames,
                                string additionalLabel = "") {
  
  
  //***********************************************************************************************
  // Distributions with no Cuts
  //***********************************************************************************************  
  makeComparison("LeptonPtMax_FRVsMCFake"+additionalLabel, "hLeptonPtMax", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("LeptonPtMin_FRVsMCFake"+additionalLabel, "hLeptonPtMin", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("MetPtHist_FRVsMCFake"+additionalLabel, "hMetPtHist", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("MetPhiHist_FRVsMCFake"+additionalLabel, "hMetPhiHist", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("UncorrMetPtHist_FRVsMCFake"+additionalLabel, "hUncorrMetPtHist", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("UncorrMetPhiHist_FRVsMCFake"+additionalLabel, "hUncorrMetPhiHist", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("DeltaPhiLeptons_FRVsMCFake"+additionalLabel, "hDeltaPhiLeptons", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("DeltaEtaLeptons_FRVsMCFake"+additionalLabel, "hDeltaEtaLeptons", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("DileptonMass_FRVsMCFake"+additionalLabel, "hDileptonMass", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  

  makeComparison("LeptonPtMax_ee_FRVsMCFake"+additionalLabel, "hLeptonPtMax_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("LeptonPtMin_ee_FRVsMCFake"+additionalLabel, "hLeptonPtMin_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("MetPtHist_ee_FRVsMCFake"+additionalLabel, "hMetPtHist_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("MetPhiHist_ee_FRVsMCFake"+additionalLabel, "hMetPhiHist_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("UncorrMetPtHist_ee_FRVsMCFake"+additionalLabel, "hUncorrMetPtHist_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("UncorrMetPhiHist_ee_FRVsMCFake"+additionalLabel, "hUncorrMetPhiHist_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("DeltaPhiLeptons_ee_FRVsMCFake"+additionalLabel, "hDeltaPhiLeptons_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("DeltaEtaLeptons_ee_FRVsMCFake"+additionalLabel, "hDeltaEtaLeptons_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("DileptonMass_ee_FRVsMCFake"+additionalLabel, "hDileptonMass_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
 

  makeComparison("LeptonPtMax_mumu_FRVsMCFake"+additionalLabel, "hLeptonPtMax_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("LeptonPtMin_mumu_FRVsMCFake"+additionalLabel, "hLeptonPtMin_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("MetPtHist_mumu_FRVsMCFake"+additionalLabel, "hMetPtHist_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("MetPhiHist_mumu_FRVsMCFake"+additionalLabel, "hMetPhiHist_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("UncorrMetPtHist_mumu_FRVsMCFake"+additionalLabel, "hUncorrMetPtHist_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("UncorrMetPhiHist_mumu_FRVsMCFake"+additionalLabel, "hUncorrMetPhiHist_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("DeltaPhiLeptons_mumu_FRVsMCFake"+additionalLabel, "hDeltaPhiLeptons_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("DeltaEtaLeptons_mumu_FRVsMCFake"+additionalLabel, "hDeltaEtaLeptons_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("DileptonMass_mumu_FRVsMCFake"+additionalLabel, "hDileptonMass_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
 
  makeComparison("LeptonPtMax_emu_FRVsMCFake"+additionalLabel, "hLeptonPtMax_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("LeptonPtMin_emu_FRVsMCFake"+additionalLabel, "hLeptonPtMin_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("MetPtHist_emu_FRVsMCFake"+additionalLabel, "hMetPtHist_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("MetPhiHist_emu_FRVsMCFake"+additionalLabel, "hMetPhiHist_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("UncorrMetPtHist_emu_FRVsMCFake"+additionalLabel, "hUncorrMetPtHist_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("UncorrMetPhiHist_emu_FRVsMCFake"+additionalLabel, "hUncorrMetPhiHist_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("DeltaPhiLeptons_emu_FRVsMCFake"+additionalLabel, "hDeltaPhiLeptons_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("DeltaEtaLeptons_emu_FRVsMCFake"+additionalLabel, "hDeltaEtaLeptons_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
  makeComparison("DileptonMass_emu_FRVsMCFake"+additionalLabel, "hDileptonMass_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
 

  //***********************************************************************************************
  // N-1 cut Distributions 
  //***********************************************************************************************  
  // makeComparison("LeptonPtMax_NMinusOne_FRVsMCFake"+additionalLabel, "hLeptonPtMax_NMinusOne", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("LeptonPtMin_NMinusOne_FRVsMCFake"+additionalLabel, "hLeptonPtMin_NMinusOne", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("MetPtHist_NMinusOne_FRVsMCFake"+additionalLabel, "hMetPtHist_NMinusOne", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("MetPhiHist_NMinusOne_FRVsMCFake"+additionalLabel, "hMetPhiHist_NMinusOne", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("METdeltaPhilEtHist_NMinusOne_FRVsMCFake"+additionalLabel, "hMETdeltaPhilEtHist_NMinusOne", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("NCentralJets_NMinusOne_FRVsMCFake"+additionalLabel, "hNCentralJets_NMinusOne", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("NDirtyMuonsHist_NMinusOne_FRVsMCFake"+additionalLabel, "hNDirtyMuonsHist_NMinusOne", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("NCleanExtraTracksHist_NMinusOne_FRVsMCFake"+additionalLabel, "hNCleanExtraTracksHist_NMinusOne", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("DeltaPhiLeptons_NMinusOne_FRVsMCFake"+additionalLabel, "hDeltaPhiLeptons_NMinusOne", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("DeltaEtaLeptons_NMinusOne_FRVsMCFake"+additionalLabel, "hDeltaEtaLeptons_NMinusOne", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("DileptonMass_NMinusOne_FRVsMCFake"+additionalLabel, "hDileptonMass_NMinusOne", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("MinDeltaPhiLeptonMet_NMinusOne_FRVsMCFake"+additionalLabel, "hMinDeltaPhiLeptonMet_NMinusOne", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
 
//   makeComparison("LeptonPtMax_NMinusOne_ee_FRVsMCFake"+additionalLabel, "hLeptonPtMax_NMinusOne_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("LeptonPtMin_NMinusOne_ee_FRVsMCFake"+additionalLabel, "hLeptonPtMin_NMinusOne_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("MetPtHist_NMinusOne_ee_FRVsMCFake"+additionalLabel, "hMetPtHist_NMinusOne_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("MetPhiHist_NMinusOne_ee_FRVsMCFake"+additionalLabel, "hMetPhiHist_NMinusOne_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("METdeltaPhilEtHist_NMinusOne_ee_FRVsMCFake"+additionalLabel, "hMETdeltaPhilEtHist_NMinusOne_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("NCentralJets_NMinusOne_ee_FRVsMCFake"+additionalLabel, "hNCentralJets_NMinusOne_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("NDirtyMuonsHist_NMinusOne_ee_FRVsMCFake"+additionalLabel, "hNDirtyMuonsHist_NMinusOne_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("NCleanExtraTracksHist_NMinusOne_ee_FRVsMCFake"+additionalLabel, "hNCleanExtraTracksHist_NMinusOne_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("DeltaPhiLeptons_NMinusOne_ee_FRVsMCFake"+additionalLabel, "hDeltaPhiLeptons_NMinusOne_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("DeltaEtaLeptons_NMinusOne_ee_FRVsMCFake"+additionalLabel, "hDeltaEtaLeptons_NMinusOne_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("DileptonMass_NMinusOne_ee_FRVsMCFake"+additionalLabel, "hDileptonMass_NMinusOne_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("MinDeltaPhiLeptonMet_NMinusOne_ee_FRVsMCFake"+additionalLabel, "hMinDeltaPhiLeptonMet_NMinusOne_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");

//   makeComparison("LeptonPtMax_NMinusOne_mumu_FRVsMCFake"+additionalLabel, "hLeptonPtMax_NMinusOne_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("LeptonPtMin_NMinusOne_mumu_FRVsMCFake"+additionalLabel, "hLeptonPtMin_NMinusOne_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("MetPtHist_NMinusOne_mumu_FRVsMCFake"+additionalLabel, "hMetPtHist_NMinusOne_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("MetPhiHist_NMinusOne_mumu_FRVsMCFake"+additionalLabel, "hMetPhiHist_NMinusOne_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   //makeComparison("METdeltaPhilEtHist_NMinusOne_mumu_FRVsMCFake"+additionalLabel, "hMETdeltaPhilEtHist_NMinusOne_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("NCentralJets_NMinusOne_mumu_FRVsMCFake"+additionalLabel, "hNCentralJets_NMinusOne_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("NDirtyMuonsHist_NMinusOne_mumu_FRVsMCFake"+additionalLabel, "hNDirtyMuonsHist_NMinusOne_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("NCleanExtraTracksHist_NMinusOne_mumu_FRVsMCFake"+additionalLabel, "hNCleanExtraTracksHist_NMinusOne_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("DeltaPhiLeptons_NMinusOne_mumu_FRVsMCFake"+additionalLabel, "hDeltaPhiLeptons_NMinusOne_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("DeltaEtaLeptons_NMinusOne_mumu_FRVsMCFake"+additionalLabel, "hDeltaEtaLeptons_NMinusOne_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("DileptonMass_NMinusOne_mumu_FRVsMCFake"+additionalLabel, "hDileptonMass_NMinusOne_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("MinDeltaPhiLeptonMet_NMinusOne_mumu_FRVsMCFake"+additionalLabel, "hMinDeltaPhiLeptonMet_NMinusOne_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");

//   makeComparison("LeptonPtMax_NMinusOne_emu_FRVsMCFake"+additionalLabel, "hLeptonPtMax_NMinusOne_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("LeptonPtMin_NMinusOne_emu_FRVsMCFake"+additionalLabel, "hLeptonPtMin_NMinusOne_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("MetPtHist_NMinusOne_emu_FRVsMCFake"+additionalLabel, "hMetPtHist_NMinusOne_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("MetPhiHist_NMinusOne_emu_FRVsMCFake"+additionalLabel, "hMetPhiHist_NMinusOne_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("METdeltaPhilEtHist_NMinusOne_emu_FRVsMCFake"+additionalLabel, "hMETdeltaPhilEtHist_NMinusOne_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("NCentralJets_NMinusOne_emu_FRVsMCFake"+additionalLabel, "hNCentralJets_NMinusOne_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("NDirtyMuonsHist_NMinusOne_emu_FRVsMCFake"+additionalLabel, "hNDirtyMuonsHist_NMinusOne_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("NCleanExtraTracksHist_NMinusOne_emu_FRVsMCFake"+additionalLabel, "hNCleanExtraTracksHist_NMinusOne_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("DeltaPhiLeptons_NMinusOne_emu_FRVsMCFake"+additionalLabel, "hDeltaPhiLeptons_NMinusOne_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   //makeComparison("DeltaEtaLeptons_NMinusOne_emu_FRVsMCFake"+additionalLabel, "hDeltaEtaLeptons_NMinusOne_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("DileptonMass_NMinusOne_emu_FRVsMCFake"+additionalLabel, "hDileptonMass_NMinusOne_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("MinDeltaPhiLeptonMet_NMinusOne_emu_FRVsMCFake"+additionalLabel, "hMinDeltaPhiLeptonMet_NMinusOne_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");

//   //***********************************************************************************************
//   // Distributions After all Cuts
//   //***********************************************************************************************  
//   makeComparison("MinDeltaPhiLeptonMet_afterCuts_FRVsMCFake"+additionalLabel, "hMinDeltaPhiLeptonMet_afterCuts", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("MtLepton1_afterCuts_FRVsMCFake"+additionalLabel, "hMtLepton1_afterCuts", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("MtLepton2_afterCuts_FRVsMCFake"+additionalLabel, "hMtLepton2_afterCuts", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("MtHiggs_afterCuts_FRVsMCFake"+additionalLabel, "hMtHiggs_afterCuts", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("LeptonPtPlusMet_afterCuts_FRVsMCFake"+additionalLabel, "hLeptonPtPlusMet_afterCuts", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 

//   makeComparison("MinDeltaPhiLeptonMet_afterCuts_ee_FRVsMCFake"+additionalLabel, "hMinDeltaPhiLeptonMet_afterCuts_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("MtLepton1_afterCuts_ee_FRVsMCFake"+additionalLabel, "hMtLepton1_afterCuts_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("MtLepton2_afterCuts_ee_FRVsMCFake"+additionalLabel, "hMtLepton2_afterCuts_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("MtHiggs_afterCuts_ee_FRVsMCFake"+additionalLabel, "hMtHiggs_afterCuts_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("LeptonPtPlusMet_afterCuts_ee_FRVsMCFake"+additionalLabel, "hLeptonPtPlusMet_afterCuts_ee", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 

//   makeComparison("MinDeltaPhiLeptonMet_afterCuts_mumu_FRVsMCFake"+additionalLabel, "hMinDeltaPhiLeptonMet_afterCuts_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake");
//   makeComparison("MtLepton1_afterCuts_mumu_FRVsMCFake"+additionalLabel, "hMtLepton1_afterCuts_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("MtLepton2_afterCuts_mumu_FRVsMCFake"+additionalLabel, "hMtLepton2_afterCuts_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("MtHiggs_afterCuts_mumu_FRVsMCFake"+additionalLabel, "hMtHiggs_afterCuts_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("LeptonPtPlusMet_afterCuts_mumu_FRVsMCFake"+additionalLabel, "hLeptonPtPlusMet_afterCuts_mumu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 

//   makeComparison("MinDeltaPhiLeptonMet_afterCuts_emu_FRVsMCFake"+additionalLabel, "hMinDeltaPhiLeptonMet_afterCuts_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("MtLepton1_afterCuts_emu_FRVsMCFake"+additionalLabel, "hMtLepton1_afterCuts_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("MtLepton2_afterCuts_emu_FRVsMCFake"+additionalLabel, "hMtLepton2_afterCuts_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("MtHiggs_afterCuts_emu_FRVsMCFake"+additionalLabel, "hMtHiggs_afterCuts_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 
//   makeComparison("LeptonPtPlusMet_afterCuts_emu_FRVsMCFake"+additionalLabel, "hLeptonPtPlusMet_afterCuts_emu", fakeHistogramFiles,fakeDatasetNames,"FR Method", simulationFakeHistogramFiles,simulationFakeDatasetNames, "MC Fake"); 


}
//--------------------------------------------------------------------------------------------------
// Main of the macro
//--------------------------------------------------------------------------------------------------
void makeWJetsFakeComparisonPlots()
{

  //***********************************************************************************************
  // Definition of Fake rate method Samples
  //***********************************************************************************************
  vector<string> fakeHistogramFiles_pythia;
  vector<string> fakeDatasetNames_pythia;
  fakeHistogramFiles_pythia.push_back("/home/sixie/hist/WJetsFakeValidationStudy/filler/006/WJetsFakeValidationStudy_s8-we-id9.root");
  fakeDatasetNames_pythia.push_back("s8-we-id9");
  //fakeHistogramFiles_pythia.push_back("/home/sixie/hist/WJetsFakeValidationStudy/filler/006/WJetsFakeValidationStudy_s8-wm-id9.root");
  //fakeDatasetNames_pythia.push_back("s8-wm-id9");
  //fakeHistogramFiles_pythia.push_back("/home/sixie/hist/WJetsFakeValidationStudy/filler/006/WJetsFakeValidationStudy_s8-wtau-id9.root");
  //fakeDatasetNames_pythia.push_back("s8-wtau-id9");

  vector<string> fakeHistogramFiles_madgraph;
  vector<string> fakeDatasetNames_madgraph;
  fakeHistogramFiles_madgraph.push_back("/home/sixie/hist/WJetsFakeValidationStudy/filler/006/WJetsFakeValidationStudy_f8-wjets-mg-id9.root");
  fakeDatasetNames_madgraph.push_back("f8-wjets-mg-id9");

  //***********************************************************************************************
  // Definition of Simulation Fake Samples
  //***********************************************************************************************
  //Samples
  vector<string> simulationFakeHistogramFiles_pythia;
  vector<string> simulationFakeDatasetNames_pythia;
  simulationFakeHistogramFiles_pythia.push_back("/home/sixie/hist/WJetsFakeValidationStudy_MCFake/filler/006/WJetsFakeValidationStudy_MCFake_s8-we-id9.root");
  simulationFakeDatasetNames_pythia.push_back("s8-we-id9");
  simulationFakeHistogramFiles_pythia.push_back("/home/sixie/hist/WJetsFakeValidationStudy_MCFake/filler/006/WJetsFakeValidationStudy_MCFake_s8-wm-id9.root");
  simulationFakeDatasetNames_pythia.push_back("s8-wm-id9");
  simulationFakeHistogramFiles_pythia.push_back("/home/sixie/hist/WJetsFakeValidationStudy_MCFake/filler/006/WJetsFakeValidationStudy_MCFake_s8-wtau-id9.root");
  simulationFakeDatasetNames_pythia.push_back("s8-wtau-id9");
  
  vector<string> simulationFakeHistogramFiles_madgraph;
  vector<string> simulationFakeDatasetNames_madgraph;
  simulationFakeHistogramFiles_madgraph.push_back("/home/sixie/hist/WJetsFakeValidationStudy_MCFake/filler/006/WJetsFakeValidationStudy_MCFake_f8-wjets-mg-id9.root");
  simulationFakeDatasetNames_madgraph.push_back("f8-wjets-mg-id9");
  
  
  //***********************************************************************************************
  // Make comparison plots of pythia vs madraph
  //***********************************************************************************************
  //makeComparisonPythiaVsMadgraph(fakeHistogramFiles_pythia,fakeDatasetNames_pythia,fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph);
  makeComparisonPythiaVsMadgraph(simulationFakeHistogramFiles_pythia,simulationFakeDatasetNames_pythia,simulationFakeHistogramFiles_madgraph,simulationFakeDatasetNames_madgraph);
  
  //***********************************************************************************************
  // Make comparison plots of FR vs Simulation Fakes
  //***********************************************************************************************
  //makeComparisonFRVsMCFake(fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph,simulationFakeHistogramFiles_madgraph, simulationFakeDatasetNames_madgraph, "_madgraph");
  //makeComparisonFRVsMCFake(fakeHistogramFiles_pythia,fakeDatasetNames_pythia,simulationFakeHistogramFiles_pythia, simulationFakeDatasetNames_pythia, "_pythia");
  //makeComparisonFRVsMCFake(fakeHistogramFiles_madgraph,fakeDatasetNames_madgraph,simulationFakeHistogramFiles_madgraph, simulationFakeDatasetNames_madgraph, "_pythia");
  
   return;
   
}
