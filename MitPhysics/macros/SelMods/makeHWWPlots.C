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
void makeHWWPlots()
{
  //------------------------------------------------------------------------------------------------
  // Process the full set of dimuon mass histograms which come in predefined bins of the dimuon pT
  // to obtain in the end a distribution showing the measured dimuon mass versus its pT.
  //------------------------------------------------------------------------------------------------
  // access the histogram file and load TAMTopModule to load into memory
  gStyle->SetPalette(1);

  bool     doLeg   = true;
  double   legx[2] = { 0.5, 0.7 };
  double   legy[2] = { 0.6, 0.8 };
  double   legTextSize = 0.03;
  TString legTitle = "";

  string plotname;
  string filename;
  string specialLabel = "_SC5";
  
  string filename_signal = "mit-HWWSelection-003-hist_HWWSignal" + specialLabel + ".root";
  vector<string> filenames;
  vector<string> datasetnames;
vector<double> scaleFactors;
//   filenames.push_back("mit-HWWSelection-003-hist_HWWSignal" + specialLabel + ".root");
//   datasetnames.push_back("H165WW");
//   scaleFactors.push_back(0.325);
  filenames.push_back("mit-HWWSelection-003-hist_WWBkg" + specialLabel + ".root");
  datasetnames.push_back("WW");
  scaleFactors.push_back(7.517/9500);
  filenames.push_back("mit-HWWSelection-003-hist_ttbarBkg" + specialLabel + ".root");
  datasetnames.push_back("TTBAR");
  scaleFactors.push_back(836.5/108927);
  filenames.push_back("mit-HWWSelection-003-hist_WZBkg" + specialLabel + ".root");
  datasetnames.push_back("WZ");
  scaleFactors.push_back(0.585/9100);
  filenames.push_back("mit-HWWSelection-003-hist_ZZBkg" + specialLabel + ".root");
  datasetnames.push_back("ZZ");
  scaleFactors.push_back(0.22/8500);

  double scaleFactorSignal = 0.325/23300;

  assert(filenames.size() == datasetnames.size());
  assert(filenames.size() == scaleFactors.size());
  
  
  plotname = "HWWSelectionStacked";
  makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, 
                  "HWW Signal", "hHWWSelection", plotname+specialLabel);

//*************************************************************************************************
//Sequential Cuts Plots
//*************************************************************************************************

  plotname = "LeptonEta";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonEta", plotname+specialLabel);

  plotname = "NJets";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCentralJets", plotname+specialLabel);

  plotname = "Met";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPtHist", plotname+specialLabel);

  plotname = "DeltaPhiLeptons";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaPhiLeptons", plotname+specialLabel);

  plotname = "DileptonMass";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hDileptonMass", plotname+specialLabel);

  plotname = "LeptonPtMax";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonPtMax", plotname+specialLabel);

  plotname = "LeptonPtMin";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hLeptonPtMin", plotname+specialLabel);

  plotname = "NDirtyMuons";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNDirtyMuonsHist", plotname+specialLabel);

  plotname = "NCleanExtraTracksHist";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCleanExtraTracksHist", plotname+specialLabel);

//*************************************************************************************************
//N-1 Plots
//*************************************************************************************************

  plotname = "NJets_NMinusOne";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCentralJets_NMinusOne", plotname+specialLabel);

  plotname = "Met_NMinusOne";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPtHist_NMinusOne", plotname+specialLabel);

  plotname = "DeltaPhiLeptons_NMinusOne";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaPhiLeptons_NMinusOne", plotname+specialLabel);

  plotname = "DileptonMass_NMinusOne";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hDileptonMass_NMinusOne", plotname+specialLabel);

  plotname = "LeptonPtMax_NMinusOne";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonPtMax_NMinusOne", plotname+specialLabel);

  plotname = "LeptonPtMin_NMinusOne";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hLeptonPtMin_NMinusOne", plotname+specialLabel);

  plotname = "NDirtyMuons_NMinusOne";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNDirtyMuonsHist_NMinusOne", plotname+specialLabel);

  plotname = "NCleanExtraTracksHist_NMinusOne";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCleanExtraTracksHist_NMinusOne", plotname+specialLabel);

//*************************************************************************************************
//After all cuts Plots
//*************************************************************************************************

  plotname = "LeptonEta_afterCuts";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonEta_afterCuts", plotname+specialLabel);

  plotname = "LeptonPtMax_afterCuts";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonPtMax_afterCuts", plotname+specialLabel);

  plotname = "LeptonPtMin_afterCuts";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hLeptonPtMin_afterCuts", plotname+specialLabel);

  plotname = "Met_afterCuts";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPtHist_afterCuts", plotname+specialLabel);

  plotname = "MetPhi_afterCuts";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPhiHist_afterCuts", plotname+specialLabel);

  plotname = "DeltaPhiLeptons_afterCuts";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaPhiLeptons_afterCuts", plotname+specialLabel);

  plotname = "DeltaEtaLeptons_afterCuts";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaEtaLeptons_afterCuts", plotname+specialLabel);

  plotname = "DileptonMass_afterCuts";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hDileptonMass_afterCuts", plotname+specialLabel);

  plotname = "MinDeltaPhiLeptonMet_afterCuts";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMinDeltaPhiLeptonMet_afterCuts", plotname+specialLabel);

  plotname = "MetSignificance_afterCuts";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMetSignificance_afterCuts", plotname+specialLabel);

  plotname = "SumEt_afterCuts";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hSumEt_afterCuts", plotname+specialLabel);

  plotname = "MtLepton1_afterCuts";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMtLepton1_afterCuts", plotname+specialLabel);

  plotname = "MtLepton2_afterCuts";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMtLepton2_afterCuts", plotname+specialLabel);

  plotname = "MtHiggs_afterCuts";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMtHiggs_afterCuts", plotname+specialLabel);

  plotname = "LeptonPtPlusMet_afterCuts";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hLeptonPtPlusMet_afterCuts", plotname+specialLabel);


//*************************************************************************************************
//
//ee final state Plots
//
//*************************************************************************************************
//   plotname = "HWWSelectionStacked_ee";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hHWWSelection_ee", plotname+specialLabel);

//*************************************************************************************************
//Sequential Cuts Plots
//*************************************************************************************************

  plotname = "LeptonEta_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonEta_ee", plotname+specialLabel);

  plotname = "NJets_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCentralJets_ee", plotname+specialLabel);

  plotname = "Met_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPtHist_ee", plotname+specialLabel);

  plotname = "DeltaPhiLeptons_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaPhiLeptons_ee", plotname+specialLabel);

  plotname = "DileptonMass_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hDileptonMass_ee", plotname+specialLabel);

  plotname = "LeptonPtMax_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonPtMax_ee", plotname+specialLabel);

  plotname = "LeptonPtMin_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hLeptonPtMin_ee", plotname+specialLabel);

  plotname = "NDirtyMuons_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNDirtyMuonsHist_ee", plotname+specialLabel);

  plotname = "NCleanExtraTracksHist_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCleanExtraTracksHist_ee", plotname+specialLabel);

//*************************************************************************************************
//N-1 Plots
//*************************************************************************************************

  plotname = "NJets_NMinusOne_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCentralJets_NMinusOne_ee", plotname+specialLabel);

  plotname = "Met_NMinusOne_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPtHist_NMinusOne_ee", plotname+specialLabel);

  plotname = "DeltaPhiLeptons_NMinusOne_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaPhiLeptons_NMinusOne_ee", plotname+specialLabel);

  plotname = "DileptonMass_NMinusOne_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hDileptonMass_NMinusOne_ee", plotname+specialLabel);

  plotname = "LeptonPtMax_NMinusOne_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonPtMax_NMinusOne_ee", plotname+specialLabel);

  plotname = "LeptonPtMin_NMinusOne_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hLeptonPtMin_NMinusOne_ee", plotname+specialLabel);

  plotname = "NDirtyMuons_NMinusOne_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNDirtyMuonsHist_NMinusOne_ee", plotname+specialLabel);

  plotname = "NCleanExtraTracksHist_NMinusOne_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCleanExtraTracksHist_NMinusOne_ee", plotname+specialLabel);

//*************************************************************************************************
//After all cuts Plots
//*************************************************************************************************

  plotname = "LeptonEta_afterCuts_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonEta_afterCuts_ee", plotname+specialLabel);

  plotname = "LeptonPtMax_afterCuts_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonPtMax_afterCuts_ee", plotname+specialLabel);

  plotname = "LeptonPtMin_afterCuts_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hLeptonPtMin_afterCuts_ee", plotname+specialLabel);

  plotname = "Met_afterCuts_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPtHist_afterCuts_ee", plotname+specialLabel);

  plotname = "MetPhi_afterCuts_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPhiHist_afterCuts_ee", plotname+specialLabel);

  plotname = "DeltaPhiLeptons_afterCuts_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaPhiLeptons_afterCuts_ee", plotname+specialLabel);

  plotname = "DeltaEtaLeptons_afterCuts_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaEtaLeptons_afterCuts_ee", plotname+specialLabel);

  plotname = "DileptonMass_afterCuts_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hDileptonMass_afterCuts_ee", plotname+specialLabel);

  plotname = "MinDeltaPhiLeptonMet_afterCuts_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMinDeltaPhiLeptonMet_afterCuts_ee", plotname+specialLabel);

  plotname = "MetSignificance_afterCuts_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMetSignificance_afterCuts_ee", plotname+specialLabel);

  plotname = "SumEt_afterCuts_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hSumEt_afterCuts_ee", plotname+specialLabel);

  plotname = "MtLepton1_afterCuts_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMtLepton1_afterCuts_ee", plotname+specialLabel);

  plotname = "MtLepton2_afterCuts_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMtLepton2_afterCuts_ee", plotname+specialLabel);

  plotname = "MtHiggs_afterCuts_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMtHiggs_afterCuts_ee", plotname+specialLabel);

  plotname = "LeptonPtPlusMet_afterCuts_ee";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hLeptonPtPlusMet_afterCuts_ee", plotname+specialLabel);


//*************************************************************************************************
//
//mumu final state Plots
//
//*************************************************************************************************
//   plotname = "HWWSelectionStacked_mumu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hHWWSelection_mumu", plotname+specialLabel);

//*************************************************************************************************
//Sequential Cuts Plots
//*************************************************************************************************

  plotname = "LeptonEta_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonEta_mumu", plotname+specialLabel);

  plotname = "NJets_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCentralJets_mumu", plotname+specialLabel);

  plotname = "Met_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPtHist_mumu", plotname+specialLabel);

  plotname = "DeltaPhiLeptons_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaPhiLeptons_mumu", plotname+specialLabel);

  plotname = "DileptonMass_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hDileptonMass_mumu", plotname+specialLabel);

  plotname = "LeptonPtMax_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonPtMax_mumu", plotname+specialLabel);

  plotname = "LeptonPtMin_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hLeptonPtMin_mumu", plotname+specialLabel);

  plotname = "NDirtyMuons_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNDirtyMuonsHist_mumu", plotname+specialLabel);

  plotname = "NCleanExtraTracksHist_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCleanExtraTracksHist_mumu", plotname+specialLabel);

//*************************************************************************************************
//N-1 Plots
//*************************************************************************************************

  plotname = "NJets_NMinusOne_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCentralJets_NMinusOne_mumu", plotname+specialLabel);

  plotname = "Met_NMinusOne_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPtHist_NMinusOne_mumu", plotname+specialLabel);

  plotname = "DeltaPhiLeptons_NMinusOne_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaPhiLeptons_NMinusOne_mumu", plotname+specialLabel);

  plotname = "DileptonMass_NMinusOne_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hDileptonMass_NMinusOne_mumu", plotname+specialLabel);

  plotname = "LeptonPtMax_NMinusOne_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonPtMax_NMinusOne_mumu", plotname+specialLabel);

  plotname = "LeptonPtMin_NMinusOne_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hLeptonPtMin_NMinusOne_mumu", plotname+specialLabel);

  plotname = "NDirtyMuons_NMinusOne_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNDirtyMuonsHist_NMinusOne_mumu", plotname+specialLabel);

  plotname = "NCleanExtraTracksHist_NMinusOne_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCleanExtraTracksHist_NMinusOne_mumu", plotname+specialLabel);

//*************************************************************************************************
//After all cuts Plots
//*************************************************************************************************

  plotname = "LeptonEta_afterCuts_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonEta_afterCuts_mumu", plotname+specialLabel);

  plotname = "LeptonPtMax_afterCuts_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonPtMax_afterCuts_mumu", plotname+specialLabel);

  plotname = "LeptonPtMin_afterCuts_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hLeptonPtMin_afterCuts_mumu", plotname+specialLabel);

  plotname = "Met_afterCuts_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPtHist_afterCuts_mumu", plotname+specialLabel);

  plotname = "MetPhi_afterCuts_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPhiHist_afterCuts_mumu", plotname+specialLabel);

  plotname = "DeltaPhiLeptons_afterCuts_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaPhiLeptons_afterCuts_mumu", plotname+specialLabel);

  plotname = "DeltaEtaLeptons_afterCuts_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaEtaLeptons_afterCuts_mumu", plotname+specialLabel);

  plotname = "DileptonMass_afterCuts_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hDileptonMass_afterCuts_mumu", plotname+specialLabel);

  plotname = "MinDeltaPhiLeptonMet_afterCuts_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMinDeltaPhiLeptonMet_afterCuts_mumu", plotname+specialLabel);

  plotname = "MetSignificance_afterCuts_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMetSignificance_afterCuts_mumu", plotname+specialLabel);

  plotname = "SumEt_afterCuts_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hSumEt_afterCuts_mumu", plotname+specialLabel);

  plotname = "MtLepton1_afterCuts_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMtLepton1_afterCuts_mumu", plotname+specialLabel);

  plotname = "MtLepton2_afterCuts_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMtLepton2_afterCuts_mumu", plotname+specialLabel);

  plotname = "MtHiggs_afterCuts_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMtHiggs_afterCuts_mumu", plotname+specialLabel);

  plotname = "LeptonPtPlusMet_afterCuts_mumu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hLeptonPtPlusMet_afterCuts_mumu", plotname+specialLabel);
//*************************************************************************************************
//
//emu final state Plots
//
//*************************************************************************************************
//   plotname = "HWWSelectionStacked_emu";
//   makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hHWWSelection_emu", plotname+specialLabel);

//*************************************************************************************************
//Sequential Cuts Plots
//*************************************************************************************************

  plotname = "LeptonEta_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonEta_emu", plotname+specialLabel);

  plotname = "NJets_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCentralJets_emu", plotname+specialLabel);

  plotname = "Met_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPtHist_emu", plotname+specialLabel);

  plotname = "DeltaPhiLeptons_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaPhiLeptons_emu", plotname+specialLabel);

  plotname = "DileptonMass_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hDileptonMass_emu", plotname+specialLabel);

  plotname = "LeptonPtMax_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonPtMax_emu", plotname+specialLabel);

  plotname = "LeptonPtMin_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hLeptonPtMin_emu", plotname+specialLabel);

  plotname = "NDirtyMuons_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNDirtyMuonsHist_emu", plotname+specialLabel);

  plotname = "NCleanExtraTracksHist_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCleanExtraTracksHist_emu", plotname+specialLabel);

//*************************************************************************************************
//N-1 Plots
//*************************************************************************************************

  plotname = "NJets_NMinusOne_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCentralJets_NMinusOne_emu", plotname+specialLabel);

  plotname = "Met_NMinusOne_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPtHist_NMinusOne_emu", plotname+specialLabel);

  plotname = "DeltaPhiLeptons_NMinusOne_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaPhiLeptons_NMinusOne_emu", plotname+specialLabel);

  plotname = "DileptonMass_NMinusOne_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hDileptonMass_NMinusOne_emu", plotname+specialLabel);

  plotname = "LeptonPtMax_NMinusOne_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonPtMax_NMinusOne_emu", plotname+specialLabel);

  plotname = "LeptonPtMin_NMinusOne_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hLeptonPtMin_NMinusOne_emu", plotname+specialLabel);

  plotname = "NDirtyMuons_NMinusOne_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNDirtyMuonsHist_NMinusOne_emu", plotname+specialLabel);

  plotname = "NCleanExtraTracksHist_NMinusOne_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hNCleanExtraTracksHist_NMinusOne_emu", plotname+specialLabel);

//*************************************************************************************************
//After all cuts Plots
//*************************************************************************************************

  plotname = "LeptonEta_afterCuts_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonEta_afterCuts_emu", plotname+specialLabel);

  plotname = "LeptonPtMax_afterCuts_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hLeptonPtMax_afterCuts_emu", plotname+specialLabel);

  plotname = "LeptonPtMin_afterCuts_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hLeptonPtMin_afterCuts_emu", plotname+specialLabel);

  plotname = "Met_afterCuts_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPtHist_afterCuts_emu", plotname+specialLabel);

  plotname = "MetPhi_afterCuts_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal, "WW Signal", "hMetPhiHist_afterCuts_emu", plotname+specialLabel);

  plotname = "DeltaPhiLeptons_afterCuts_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaPhiLeptons_afterCuts_emu", plotname+specialLabel);

  plotname = "DeltaEtaLeptons_afterCuts_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,  "WW Signal", "hDeltaEtaLeptons_afterCuts_emu", plotname+specialLabel);

  plotname = "DileptonMass_afterCuts_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hDileptonMass_afterCuts_emu", plotname+specialLabel);

  plotname = "MinDeltaPhiLeptonMet_afterCuts_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMinDeltaPhiLeptonMet_afterCuts_emu", plotname+specialLabel);

  plotname = "MetSignificance_afterCuts_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMetSignificance_afterCuts_emu", plotname+specialLabel);

  plotname = "SumEt_afterCuts_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hSumEt_afterCuts_emu", plotname+specialLabel);

  plotname = "MtLepton1_afterCuts_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMtLepton1_afterCuts_emu", plotname+specialLabel);

  plotname = "MtLepton2_afterCuts_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMtLepton2_afterCuts_emu", plotname+specialLabel);

  plotname = "MtHiggs_afterCuts_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hMtHiggs_afterCuts_emu", plotname+specialLabel);

  plotname = "LeptonPtPlusMet_afterCuts_emu";
  //makeStackedPlot(filenames, scaleFactors, datasetnames, filename_signal, scaleFactorSignal,   "WW Signal", "hLeptonPtPlusMet_afterCuts_emu", plotname+specialLabel);


  //************************************************************************************************
  //
  //Optimize Cuts
  //
  //************************************************************************************************
  plotname = "MetCutOptimization";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hMetPtHist_NMinusOne", "Met Cut", "below", plotname+specialLabel);
  plotname = "MetCutOptimizationVsSigEff";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hMetPtHist_NMinusOne", "below", plotname+specialLabel);

  plotname = "DeltaPhiLeptonsCutOptimization";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hDeltaPhiLeptons_NMinusOne", "DeltaPhiLeptons Cut", "above", plotname+specialLabel);
  plotname = "DeltaPhiLeptonsCutOptimizationVsSigEff";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hDeltaPhiLeptons_NMinusOne", "above", plotname+specialLabel);

  plotname = "DileptonMassCutOptimization";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hDileptonMass_NMinusOne", "DileptonMass Cut", "above", plotname+specialLabel);
  plotname = "DileptonMassCutOptimizationVsSigEff";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hDileptonMass_NMinusOne", "above", plotname+specialLabel);

  plotname = "LeptonPtMaxCutOptimization";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hLeptonPtMax_NMinusOne", "LeptonPtMax Cut", "below", plotname+specialLabel);
  plotname = "LeptonPtMaxCutOptimizationVsSigEff";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hLeptonPtMax_NMinusOne", "below", plotname+specialLabel);

  plotname = "LeptonPtMinCutOptimization";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hLeptonPtMin_NMinusOne", "LeptonPtMin Cut", "below", plotname+specialLabel);
  plotname = "LeptonPtMinCutOptimizationVsSigEff";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hLeptonPtMin_NMinusOne", "below", plotname+specialLabel);

  plotname = "NCleanExtraTracksHistCutOptimization";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hNCleanExtraTracksHist_NMinusOne", "NCleanExtraTracksHist Cut", "above", plotname+specialLabel);
  plotname = "NCleanExtraTracksHistCutOptimizationVsSigEff";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hNCleanExtraTracksHist_NMinusOne",  "above", plotname+specialLabel);

  //************************************************************************************************
  //For ee
  //************************************************************************************************
  plotname = "MetCutOptimization_ee";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hMetPtHist_NMinusOne_ee", "Met Cut", "below", plotname+specialLabel);
  plotname = "MetCutOptimizationVsSigEff_ee";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hMetPtHist_NMinusOne_ee", "below", plotname+specialLabel);

  plotname = "DeltaPhiLeptonsCutOptimization_ee";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hDeltaPhiLeptons_NMinusOne_ee", "DeltaPhiLeptons Cut", "above", plotname+specialLabel);
  plotname = "DeltaPhiLeptonsCutOptimizationVsSigEff_ee";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hDeltaPhiLeptons_NMinusOne_ee", "above", plotname+specialLabel);

  plotname = "DileptonMassCutOptimization_ee";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hDileptonMass_NMinusOne_ee", "DileptonMass Cut", "above", plotname+specialLabel);
  plotname = "DileptonMassCutOptimizationVsSigEff_ee";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hDileptonMass_NMinusOne_ee", "above", plotname+specialLabel);

  plotname = "LeptonPtMaxCutOptimization_ee";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hLeptonPtMax_NMinusOne_ee", "LeptonPtMax Cut", "below", plotname+specialLabel);
  plotname = "LeptonPtMaxCutOptimizationVsSigEff_ee";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hLeptonPtMax_NMinusOne_ee", "below", plotname+specialLabel);

  plotname = "LeptonPtMinCutOptimization_ee";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hLeptonPtMin_NMinusOne_ee", "LeptonPtMin Cut", "below", plotname+specialLabel);
  plotname = "LeptonPtMinCutOptimizationVsSigEff_ee";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hLeptonPtMin_NMinusOne_ee", "below", plotname+specialLabel);

  plotname = "NCleanExtraTracksHistCutOptimization_ee";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hNCleanExtraTracksHist_NMinusOne_ee", "NCleanExtraTracksHist Cut", "above", plotname+specialLabel);
  plotname = "NCleanExtraTracksHistCutOptimizationVsSigEff_ee";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hNCleanExtraTracksHist_NMinusOne_ee",  "above", plotname+specialLabel);

  //************************************************************************************************
  //For mumu
  //************************************************************************************************
  plotname = "MetCutOptimization_mumu";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hMetPtHist_NMinusOne_mumu", "Met Cut", "below", plotname+specialLabel);
  plotname = "MetCutOptimizationVsSigEff_mumu";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hMetPtHist_NMinusOne_mumu", "below", plotname+specialLabel);

  plotname = "DeltaPhiLeptonsCutOptimization_mumu";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hDeltaPhiLeptons_NMinusOne_mumu", "DeltaPhiLeptons Cut", "above", plotname+specialLabel);
  plotname = "DeltaPhiLeptonsCutOptimizationVsSigEff_mumu";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hDeltaPhiLeptons_NMinusOne_mumu", "above", plotname+specialLabel);

  plotname = "DileptonMassCutOptimization_mumu";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hDileptonMass_NMinusOne_mumu", "DileptonMass Cut", "above", plotname+specialLabel);
  plotname = "DileptonMassCutOptimizationVsSigEff_mumu";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hDileptonMass_NMinusOne_mumu", "above", plotname+specialLabel);

  plotname = "LeptonPtMaxCutOptimization_mumu";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hLeptonPtMax_NMinusOne_mumu", "LeptonPtMax Cut", "below", plotname+specialLabel);
  plotname = "LeptonPtMaxCutOptimizationVsSigEff_mumu";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hLeptonPtMax_NMinusOne_mumu", "below", plotname+specialLabel);

  plotname = "LeptonPtMinCutOptimization_mumu";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hLeptonPtMin_NMinusOne_mumu", "LeptonPtMin Cut", "below", plotname+specialLabel);
  plotname = "LeptonPtMinCutOptimizationVsSigEff_mumu";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hLeptonPtMin_NMinusOne_mumu", "below", plotname+specialLabel);

  plotname = "NCleanExtraTracksHistCutOptimization_mumu";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hNCleanExtraTracksHist_NMinusOne_mumu", "NCleanExtraTracksHist Cut", "above", plotname+specialLabel);
  plotname = "NCleanExtraTracksHistCutOptimizationVsSigEff_mumu";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hNCleanExtraTracksHist_NMinusOne_mumu",  "above", plotname+specialLabel);

  //************************************************************************************************
  //For emu
  //************************************************************************************************
  plotname = "MetCutOptimization_emu";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hMetPtHist_NMinusOne_emu", "Met Cut", "below", plotname+specialLabel);
  plotname = "MetCutOptimizationVsSigEff_emu";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hMetPtHist_NMinusOne_emu", "below", plotname+specialLabel);

  plotname = "DeltaPhiLeptonsCutOptimization_emu";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hDeltaPhiLeptons_NMinusOne_emu", "DeltaPhiLeptons Cut", "above", plotname+specialLabel);
  plotname = "DeltaPhiLeptonsCutOptimizationVsSigEff_emu";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hDeltaPhiLeptons_NMinusOne_emu", "above", plotname+specialLabel);

  plotname = "DileptonMassCutOptimization_emu";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hDileptonMass_NMinusOne_emu", "DileptonMass Cut", "above", plotname+specialLabel);
  plotname = "DileptonMassCutOptimizationVsSigEff_emu";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hDileptonMass_NMinusOne_emu", "above", plotname+specialLabel);

  plotname = "LeptonPtMaxCutOptimization_emu";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hLeptonPtMax_NMinusOne_emu", "LeptonPtMax Cut", "below", plotname+specialLabel);
  plotname = "LeptonPtMaxCutOptimizationVsSigEff_emu";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hLeptonPtMax_NMinusOne_emu", "below", plotname+specialLabel);

  plotname = "LeptonPtMinCutOptimization_emu";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hLeptonPtMin_NMinusOne_emu", "LeptonPtMin Cut", "below", plotname+specialLabel);
  plotname = "LeptonPtMinCutOptimizationVsSigEff_emu";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hLeptonPtMin_NMinusOne_emu", "below", plotname+specialLabel);

  plotname = "NCleanExtraTracksHistCutOptimization_emu";
  optimizeCut(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hNCleanExtraTracksHist_NMinusOne_emu", "NCleanExtraTracksHist Cut", "above", plotname+specialLabel);
  plotname = "NCleanExtraTracksHistCutOptimizationVsSigEff_emu";
  makeOptimizationVsSigEff(filenames, scaleFactors, filename_signal, scaleFactorSignal, "hNCleanExtraTracksHist_NMinusOne_emu",  "above", plotname+specialLabel);

  return;

}

TH1F* getHisto(string filename, string histoname) {
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


void makeStackedPlot(vector<string> bkgFilenames, vector<double> bkgScaleFactors, vector<string> bkgLegendNames, string signalFilename, double signalScaleFactor, string signalLegendName, string histName, string plotname) {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  THStack *hBkgStack = new THStack(plotname.c_str(),plotname.c_str());
  for (int i=0;i<bkgFilenames.size();i++) {   
    TH1F *tmp = getHisto( bkgFilenames[i],histName);
    if (tmp != NULL) {
      InitHist(tmp, "Xtitle", "Number of Events", 3+i)
      tmp->Scale(bkgScaleFactors[i]);
      tmp->SetFillStyle(1001);
      tmp->SetFillColor(3+i);
      tmp->SetLineWidth(1);  
      tmp->SetMinimum(0);
      hBkgStack->Add(tmp);
      cout << i << " " << bkgFilenames[i] << " " << tmp << " " << " " << hBkgStack->GetStack()->At(i) << endl;
    } else {
      cerr << "could not get histogram " << bkgFilenames[i] << ":hHWWSelection\n";
    }
    tmp = 0;
  }
  TH1F* hHWWSignal = getHisto( signalFilename,histName) ;
  hHWWSignal->Scale(signalScaleFactor);
  hHWWSignal->SetFillStyle(0);
  hHWWSignal->SetLineColor(2);
  hHWWSignal->SetLineWidth(2); 

  //make the plot
  //TCanvas *cv = new TCanvas("cv", plotname.c_str() , 800,600); 
  hBkgStack->Draw();
  hHWWSignal->Draw("hist, same"); 
  TLegend *leg1=0;
  leg1 = new TLegend(0.5,0.6,0.7,0.8);   
  leg1->SetBorderSize(1);
  leg1->SetTextSize(0.03);
  leg1->AddEntry(hHWWSignal, signalLegendName.c_str(), "LP"); 
  for (int i=bkgFilenames.size()-1;i>= 0;i--)
    leg1->AddEntry(hBkgStack->GetStack()->At(i),bkgLegendNames[i].c_str(), "FP"); 
  leg1->Draw();
  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

}


void optimizeCut(vector<string> bkgFilenames, vector<double> bkgScaleFactors, string signalFilename, double signalScaleFactor, string histName, string xaxislabel, string cutDirection, string plotname) {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  //build the background
  TH1F* hTempBkg = getHisto( bkgFilenames[0],histName);
  TH1F* bkg = hTempBkg->Clone();
  bkg->Scale(bkgScaleFactors[0]);
  for (int i=1;i<bkgFilenames.size();i++) {   
    TH1F *tmp = getHisto( bkgFilenames[i],histName);
    if (tmp != NULL) {
      tmp->Scale(bkgScaleFactors[i]);
      bkg->Add(tmp);
    } else {
      cerr << "could not get histogram " << bkgFilenames[i] << ":hHWWSelection\n";
    }
    tmp = 0;
  }

  //get the signal
  TH1F* signal = getHisto( signalFilename,histName) ;
  signal->Scale(signalScaleFactor);

  vector<double> xaxis;
  double NSignal;
  double NBkg;
  vector<double> Significance;
  xaxis.clear();
  Significance.clear();

  for (int i=1;i<=signal->GetXaxis()->GetNbins();i++) {

    xaxis.push_back(signal->GetXaxis()->GetBinCenter(i));
    if (cutDirection == "below") {
      NSignal = signal->Integral(i,signal->GetXaxis()->GetNbins()+1);
      NBkg = bkg->Integral(i,bkg->GetXaxis()->GetNbins()+1);
      if (NSignal + NBkg > 0) 
        Significance.push_back(NSignal / TMath::Sqrt(NSignal + NBkg));
      else 
        Significance.push_back(0.0);
      
      //cerr << i << " " << NSignal << " " << NBkg << " " << NSignal / TMath::Sqrt(NSignal + NBkg) << endl;

    } else if (cutDirection == "above") {
      NSignal = signal->Integral(0,i);
      NBkg = bkg->Integral(0,i);
      if (NSignal + NBkg > 0) 
        Significance.push_back(NSignal / TMath::Sqrt(NSignal + NBkg));
      else 
        Significance.push_back(0.0);
    } else {
      cerr << "cutDirection is invalid" << endl;
      return;
    }
  }

  TGraph *graph = new TGraph(signal->GetNbinsX(), (Double_t *)&(xaxis[0]), (Double_t *)&(Significance[0]));
  graph->SetTitle(plotname.c_str()); 
  graph->SetMarkerStyle(kFullSquare);
  graph->GetYaxis()->SetTitle("S/Sqrt(S+B)");
  graph->GetXaxis()->SetTitle(xaxislabel.c_str());
  graph->SetMarkerSize(0.75);
  graph->Draw("AP");

  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}


void makeOptimizationVsSigEff(vector<string> bkgFilenames, vector<double> bkgScaleFactors, string signalFilename, double signalScaleFactor, string histName,  string cutDirection, string plotname) {

  TCanvas *cv = MakeCanvas("cv", plotname.c_str(), 800, 600);

  //build the background
  TH1F* hTempBkg = getHisto( bkgFilenames[0],histName);
  TH1F* bkg = hTempBkg->Clone();
  bkg->Scale(bkgScaleFactors[0]);
  for (int i=1;i<bkgFilenames.size();i++) {   
    TH1F *tmp = getHisto( bkgFilenames[i],histName);
    if (tmp != NULL) {
      tmp->Scale(bkgScaleFactors[i]);
      bkg->Add(tmp);
    } else {
      cerr << "could not get histogram " << bkgFilenames[i] << ":hHWWSelection\n";
    }
    tmp = 0;
  }

  //get the signal
  TH1F* signal = getHisto( signalFilename,histName) ;
  signal->Scale(signalScaleFactor);

  vector<double> xaxis;
  double NSignal;
  double NBkg;
  vector<double> Significance;
  xaxis.clear();
  Significance.clear();

  double NSignalTotal = signal->Integral(0,signal->GetXaxis()->GetNbins()+1);

  assert(NSignalTotal > 0);

  for (int i=1;i<=signal->GetXaxis()->GetNbins();i++) {
    
    if (cutDirection == "below") {
      NSignal = signal->Integral(i,signal->GetXaxis()->GetNbins()+1);
      NBkg = bkg->Integral(i,bkg->GetXaxis()->GetNbins()+1);
      xaxis.push_back(NSignal / NSignalTotal);
      if (NSignal + NBkg > 0) 
        Significance.push_back(NSignal / TMath::Sqrt(NSignal + NBkg));
      else 
        Significance.push_back(0.0);
      
      //cerr << i << " " << NSignal << " " << NBkg << " " << NSignal / TMath::Sqrt(NSignal + NBkg) << endl;

    } else if (cutDirection == "above") {
      NSignal = signal->Integral(0,i);
      NBkg = bkg->Integral(0,i);
      xaxis.push_back(NSignal / NSignalTotal);
      if (NSignal + NBkg > 0) 
        Significance.push_back(NSignal / TMath::Sqrt(NSignal + NBkg));
      else 
        Significance.push_back(0.0);
    } else {
      cerr << "cutDirection is invalid" << endl;
      return;
    }
  }

  TGraph *graph = new TGraph(signal->GetNbinsX(), (Double_t *)&(xaxis[0]), (Double_t *)&(Significance[0]));
  graph->SetTitle(plotname.c_str());
  graph->SetMarkerStyle(kFullSquare);
  graph->GetYaxis()->SetTitle("S/Sqrt(S+B)");
  graph->GetXaxis()->SetTitle("Signal Efficiency");
  graph->SetMarkerSize(0.75);
  graph->Draw("AP");

  filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();
}
