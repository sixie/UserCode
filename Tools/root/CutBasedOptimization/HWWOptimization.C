//don't do optimization
//root -l -b -q HWWOptimization.C+\(\"/home/ceballos/HiggsMVA/NeuralNetworkMaker-21x/ntuples/inputNtuple-data-standard-histo_H160_WW2l_all.root\",\"/home/ceballos/HiggsMVA/NeuralNetworkMaker-21x/ntuples/inputNtuple-data-standard-histo_HBCK2.root\",\"HWW160\",\"HWWCutOptimizationResults.root\",10\)
//
//Do Optimization
//root -l -b -q HWWOptimization.C+\(\"/home/ceballos/HiggsMVA/NeuralNetworkMaker-21x/ntuples/inputNtuple-data-standard-histo_H160_WW2l_all.root\",\"/home/ceballos/HiggsMVA/NeuralNetworkMaker-21x/ntuples/inputNtuple-data-standard-histo_HBCK2.root\",\"HWW160\",\"HWWCutOptimizationResults.root\",10,true,5,false\)

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
#include "TCanvas.h"
#include "TLegend.h"
#include "TArrayF.h"
#include "MitNtupleEvent.C"
#include <vector>
//#include <pair>

#define NVARIABLES 10
#define NPROCESSES 32
#define NBINS      200
//*************************************************************************************************
//Define Global Variables for the macro
//*************************************************************************************************

int fProcessCode[NPROCESSES] = { 120, 130, 150, 160, 170, 200, 500, 1130, 1140, 1150, 0, 1, 2, 
                                 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 25, 26,
                                 27, 28, 29 };
string fProcessName[NPROCESSES] = { "H120", "H130", "H150","H160", "H170", "H200","H500", "WH130", 
                                    "WH140", "WH150", "Wmunu", "Wenu","Wtaunu", "WMadgraph", 
                                    "ttbar","ttbarMadgraph", "Zmumu", "Zee","Ztautau","ZMadgraph", 
                                    "VQQmadgraph", "Zmumu_M20","Zee_M20", "Ztautau_M20", 
                                    "SingleTop_s" , "SingleTop_t","SingleTop_Wt", "WW_2l", "WZ_3l", 
                                    "WZ_incl", "ZZ_2l2n", "ZZ_4l" };

string fOptimizationVariableNames[NVARIABLES] = { "ptmax", "ptmin", "met", "dileptonmass", "dileptonmassZVeto", 
                                          "deltaphiLeptons", "metdeltaphilmet", "ncentraljets", "ndirtymuons", 
                                          "nextratracks" };

//OLD CUTS
// bool fCutAbove[NVARIABLES] = { true, false, true, true, false,  true, false,  true,  true,  true};
// bool fCutBelow[NVARIABLES] = { true,  true, true, true,  true, false, false, false, false, false};
// bool fixCutValueAcrossFinalstates[2][NVARIABLES] = {{false,false,false,false,false,false,false,true,true,true},
//                                                     {false,false,false,false,true,false,false,false,false,false}};
// Float_t fCutAboveInitialValue[3][NVARIABLES] = { {55.0, 5000.0, 200.0, 50.0, 5000.0, 45.0, 5000.0, 0.0, 0.0, 4.0}, 
//                                                  {49.0, 5000.0, 200.0, 40.0, 5000.0, 45.0, 5000.0, 0.0, 0.0, 4.0}, 
//                                                  {50.0, 5000.0, 105.0, 45.0, 5000.0, 70.0, 5000.0, 0.0, 0.0, 4.0}};
// Float_t fCutBelowInitialValue[3][NVARIABLES] = { {35.0,   25.0,  50.0, 12.0,   15.0,  0.0,   0.0,  0.0, 0.0, 0.0}, 
//                                                  {25.0,   25.0,  51.0, 12.0,   15.0,  0.0,   0.0,  0.0, 0.0, 0.0}, 
//                                                  {25.0,   25.0,  45.0, 12.0,   15.0,  0.0,   0.0,  0.0, 0.0, 0.0}};

//Optimization Result (Only mumu is optimized for now)
// bool fCutAbove[NVARIABLES] = { true, false, true, true, false,  true, false,  true,  true,  true};
// bool fCutBelow[NVARIABLES] = { true,  true, true, true,  true, false, false, false, false, false};
// bool fixCutValueAcrossFinalstates[2][NVARIABLES] = {{false,false,false,false,false,false,false,true,true,true},
//                                                     {false,false,false,false,true,false,false,false,false,false}};
// Float_t fCutAboveInitialValue[3][NVARIABLES] = { {400.0, 400.0, 400.0, 50.0, 5000.0,180.0, 400.0, 0.0, 0.0,15.0}, 
//                                                  {49.0, 5000.0, 200.0, 40.0, 5000.0, 45.0, 400.0, 0.0, 0.0, 4.0}, 
//                                                  {50.0, 5000.0, 105.0, 45.0, 5000.0, 70.0, 400.0, 0.0, 0.0, 4.0}};
// Float_t fCutBelowInitialValue[3][NVARIABLES] = { {0.0,   30.0,   32.0, 12.0,    0.0,  0.0,   0.0, 0.0, 0.0, 0.0}, 
//                                                  {25.0,   25.0,  51.0, 12.0,   15.0,  0.0,   0.0, 0.0, 0.0, 0.0}, 
//                                                  {25.0,   25.0,  45.0, 12.0,   15.0,  0.0,   0.0, 0.0, 0.0, 0.0}};



//Let's start with these from Guillelmo
bool fCutAbove[NVARIABLES] = { true, true, true, true, false, true, true, true, true, true};
bool fCutBelow[NVARIABLES] = { true, true, true, true, true, true, true, false, false, false};

//Two arrays are for [cutabove, cutbelow]
bool fixCutValueAcrossFinalstates[2][NVARIABLES] = {{false,false,false,false,false,false,false,true,true,true},
                                                    {false,false,false,false,true,false,false,false,false,false}};
//Three arrays are for [mumu,ee,emu]
Float_t fCutAboveInitialValue[3][NVARIABLES] = { {50.0, 47.0, 400.0, 50.0, 5000.0, 57.0, 5000.0,  0.0, 0.0, 4.0}, 
                                                 {50.0, 30.0, 400.0, 50.0, 5000.0, 57.0, 5000.0,  0.0, 0.0, 4.0}, 
                                                 {70.0, 80.0, 400.0, 50.0, 5000.0, 57.0, 5000.0,  0.0, 0.0, 4.0}};
Float_t fCutBelowInitialValue[3][NVARIABLES] = { {28.0, 25.0,  48.0, 12.0,   15.0,  0.0,    0.0,  0.0, 0.0, 0.0}, 
                                                 {28.0, 25.0,  48.0, 12.0,   15.0,  0.0,    0.0,  0.0, 0.0, 0.0}, 
                                                 {36.0, 25.0,  55.0, 12.0,   15.0,  0.0,    0.0,  0.0, 0.0, 0.0}};

Float_t fOptimizationVariableRangeMin[NVARIABLES] = { 0.0,     0.0,   0.0,   0.0,  0.0,   0.0,   0.0, 0.0, 0.0,  0.0};
Float_t fOptimizationVariableRangeMax[NVARIABLES] = { 400.0, 400.0, 400.0, 200.0, 200.0, 180.0, 400.0, 5.0, 5.0, 15.0};
bool fOptimizeThisVariable[2][NVARIABLES] = { {true, true, true, true, false, true, true, false, false, true},
                                              {true, true, true, false, true, true, true, false, false,false} };

bool fPlotThisVariable[NVARIABLES] = { true, true, true, true, true, true, true, true, true, true };




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
  assert(t);

  if (verbose) {
    cout << "---\tRecovered tree " << t->GetName()
	 << " with "<< t->GetEntries() << " entries" << endl;
  }
  
  return t;
}


//*************************************************************************************************
//Check that given initial conditions satisfy requirements
//*************************************************************************************************
void CheckInitialValueConsistency() {

  //check that cutabove values are above cutbelow values
  for (int i=0;i<NVARIABLES;i++) {
    for (int f=0;f<3;f++) {
      if (fCutAboveInitialValue[f][i] < fCutBelowInitialValue[f][i]) {
        cout << "cutAbove initial value if less than cutBelow initial value for Variable " << i
             << " in final state " << f << endl;
        assert (0==1);
      }        
    }
  }

  //check that values which should be fixed across final states are fixed
  for (int i=0;i<NVARIABLES;i++) {
    if (fixCutValueAcrossFinalstates[0][i] && 
        (fCutAboveInitialValue[0][i] != fCutAboveInitialValue[1][i] ||
         fCutAboveInitialValue[2][i] != fCutAboveInitialValue[2][i])
      ) {
      cout << "CutAbove initial values for Variable " << i << " are not fixed across final states." 
           << endl;
        assert (0==1);
    }
    if (fixCutValueAcrossFinalstates[1][i] && 
        (fCutBelowInitialValue[0][i] != fCutBelowInitialValue[1][i] ||
         fCutBelowInitialValue[2][i] != fCutBelowInitialValue[2][i])
      ) {
      cout << "CutBelow initial values for Variable " << i << " are not fixed across final states." 
           << endl;
        assert (0==1);
    }      
  }

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
//Function to determine whether the event passes the defined cuts or not
//*************************************************************************************************
bool passCut(MitNtupleEvent *event , 
             Float_t fOptimizationVariables[NVARIABLES], Float_t fCutAboveValues[NVARIABLES],
             Float_t fCutBelowValues[NVARIABLES], int cutExceptionVariableIndex = -1, string cutDirection = "none") {

  bool pass = true;
  int cutindex = -2;

  //make preselection requirements
  int charge = (int)(event->H_q1 + event->H_q2);
  pass = pass && (event->H_nlep == 2);
  pass = pass && (charge == 0);
  pass = pass && (event->H_pt1 > 20.0/400.0);
  pass = pass && (event->H_pt2 > 10.0/400.0);

  if (pass == false) 
    cutindex = -1;

  for (int i=0;i<NVARIABLES;i++) {

    pass = pass && (fOptimizationVariables[i] <= fCutAboveValues[i] || !fCutAbove[i] 
                    || cutExceptionVariableIndex == i );
    pass = pass && (fOptimizationVariables[i] >= fCutBelowValues[i] || !fCutBelow[i] 
                    || cutExceptionVariableIndex == i );

    //cutDirection indicates the cut that we will be scanning. 
    //So we should impose all cuts except the specified one.
    //eg. if cutDirection == "cutabove", then we should impose the "cutbelow" condition
    //    for the cutExceptionVariable
    if (i == cutExceptionVariableIndex) {
      if (cutDirection == "cutabove")
        pass = pass && ( fOptimizationVariables[i] >= fCutBelowValues[i] || !fCutBelow[i]);
      else if (cutDirection == "cutbelow")
        pass = pass && (fOptimizationVariables[i] <= fCutAboveValues[i] || !fCutAbove[i]);
      else
        pass = pass;
    }

    if (pass == false && cutindex < 0)
      cutindex = i;       
  }
  return pass;
}


//*************************************************************************************************
//Convert Variable Index to Variable Value by retrieving it from Guillelmo's ntuple
//My list of variables have to be defined by hand. This function serves as the definition.
//*************************************************************************************************
Double_t GetVariableValue (int variableIndex, MitNtupleEvent *event) {

  Double_t value = 0;
  if (variableIndex == 0) {
    value = event->H_pt1 * 400;
  } else if (variableIndex == 1) {
    value = event->H_pt2 * 400;
  } else if (variableIndex == 2) {
    value = event->H_met * 400;
  } else if (variableIndex == 3) {
    value = event->H_dim01*500;
  }  else if (variableIndex == 4) {
    value = fabs(event->H_dim01*500 - 91.1876);
  }  else if (variableIndex == 5) {
    value = event->H_delphil * 180;
  }  else if (variableIndex == 6) {
    value = event->H_metdeltaphilmet * 400 ;
  }  else if (variableIndex == 7) {
    value = event->H_njets;
  }  else if (variableIndex == 8) {
    value = event->H_nmuons;
  }  else if (variableIndex == 9) {
    value = event->H_ntracks;
  } else {
    cout << "variableIndex = " << variableIndex << " not recognized!" << endl;
    value = 0;
  }  
  return value;
}


//*************************************************************************************************
//Function to create a distribution histogram of the variable given
//*************************************************************************************************
TH1F* makeDistributionHistogram ( int variableIndex, string cutDirection, MitNtupleEvent* event , Int_t nentries, bool isSignal, int OnlyThisFinalState = -1) {
  assert(variableIndex >= 0 && variableIndex < NVARIABLES);
  string label = (isSignal)?"NSignalEventsPass":"NBkgEventsPass";
  string histName = "h" + fOptimizationVariableNames[variableIndex] + "_" + label;
  string axisLabel = ";" + fOptimizationVariableNames[variableIndex] + ";Number of Events";
  TH1F *histogram = new TH1F(histName.c_str(),axisLabel.c_str(),NBINS, fOptimizationVariableRangeMin[variableIndex], fOptimizationVariableRangeMax[variableIndex]);
  histogram->Sumw2();
  histogram->SetDirectory(0);

  for (int n=0;n<nentries;n++) { 
    event->GetEntry(n);
    float eventweight = event->H_weight;
    int finalstatetype = (int)event->H_ltype;

   //only look at events with the given finalstatetype
    if (OnlyThisFinalState >= 0 && OnlyThisFinalState != finalstatetype)
      continue;

    Float_t cutAboveValues[NVARIABLES];
    Float_t cutBelowValues[NVARIABLES];
    Float_t variableValues[NVARIABLES];   

    //only look at events with the given finalstatetype
    //convert finalstatetype into the array index
    int fs;
    if (finalstatetype == 10)
      fs = 0;
    else if (finalstatetype == 11)
      fs = 1;
    else if (finalstatetype == 12)
      fs = 2;
    else {
      fs = -1;
      continue;
    }

    for (int j=0;j<NVARIABLES;j++) {
      cutAboveValues[j] = fCutAboveInitialValue[fs][j];
      cutBelowValues[j] = fCutBelowInitialValue[fs][j];
      variableValues[j] = GetVariableValue(j, event);      
    }

    if (passCut(event ,variableValues, cutAboveValues, cutBelowValues, variableIndex, cutDirection)) {
      histogram->Fill(variableValues[variableIndex], eventweight);
    }  
  }
  return histogram;
}


//*************************************************************************************************
//Function to create a histogram of the number of events passing the given cut value and all
//other cuts. (N-1 histogram plot)
//*************************************************************************************************
TH1F* makeNMinusOneHistogram ( int variableIndex, TH1F *distributionHist , bool isSignal , 
                               string cutDirection) {
  string label = (isSignal)?"NSignalEventsPass":"NBkgEventsPass";
  string histName = "h" + fOptimizationVariableNames[variableIndex] + "_" + label;
  string axisLabel = ";" + fOptimizationVariableNames[variableIndex] + ";Number of Events";
  TH1F *histogram = new TH1F(histName.c_str(),axisLabel.c_str(),NBINS, fOptimizationVariableRangeMin[variableIndex], fOptimizationVariableRangeMax[variableIndex]);
  histogram->Sumw2();
  histogram->SetDirectory(0);

  assert(variableIndex >= 0 && variableIndex < NVARIABLES);

  for (int i=0;i<=histogram->GetXaxis()->GetNbins()+1;i++) {
    float tempCutValue = 0.0;
    if (i <= histogram->GetXaxis()->GetNbins())
      tempCutValue = histogram->GetXaxis()->GetBinUpEdge(i);
    else
      tempCutValue = histogram->GetXaxis()->GetBinLowEdge(i);

    double eventsPassed = 0.0;
    double sumErrorSquared = 0.0;
    if (cutDirection == "cutbelow") {
      eventsPassed = distributionHist->Integral(i,distributionHist->GetXaxis()->GetNbins()+1);
      for (int n=i;n<=distributionHist->GetXaxis()->GetNbins()+1;n++) {
        sumErrorSquared += distributionHist->GetBinError(n)*distributionHist->GetBinError(n);
      }     
    } else if (cutDirection == "cutabove") {
      eventsPassed = distributionHist->Integral(0,i);
      for (int n=0;n<=i;n++) {
        sumErrorSquared += distributionHist->GetBinError(n)*distributionHist->GetBinError(n);
      }  
    }    
    histogram->SetBinContent(i,eventsPassed);
    histogram->SetBinError(i,TMath::Sqrt(sumErrorSquared));    
  }
  return histogram;
}


//*************************************************************************************************
//Function to create a histogram of the significance as a function of the cut
//*************************************************************************************************
TH1F* makeSignificanceHistogram ( int variableIndex, TH1F *signalHist ,TH1F *bkgHist, int significanceType) {
  string histName = "hSignificance" + IntToString(significanceType) + "_" + fOptimizationVariableNames[variableIndex];
  string axisLabel = ";" + fOptimizationVariableNames[variableIndex] + ";Significance" + IntToString(significanceType);

  TH1F *histogram = (TH1F*)signalHist->Clone("Significance");
  //TH1F *histogram = new TH1F(histName.c_str(),axisLabel.c_str(),NBINS, fOptimizationVariableRangeMin[variableIndex], fOptimizationVariableRangeMax[variableIndex]);
  //histogram->Sumw2();
  histogram->SetDirectory(0);

  assert(variableIndex >= 0 && variableIndex < NVARIABLES);    
  for (int i=0;i<=histogram->GetXaxis()->GetNbins()+1;i++) {
    double NSig = signalHist->GetBinContent(i);
    double NBkg = bkgHist->GetBinContent(i);
    double Significance = 0.0;
    if (significanceType == 0) {
      if (NSig+NBkg > 0)
        Significance = NSig/TMath::Sqrt(NSig+NBkg);
    } else if (significanceType == 1) {
      if (NBkg > 0)
        Significance = NSig/NBkg;
    } else if (significanceType == 2) {
      if (NSig+NBkg+(0.2*NBkg)*(0.2*NBkg) > 0)
        Significance = NSig/TMath::Sqrt(NSig+NBkg+(0.2*NBkg)*(0.2*NBkg));
    }
    double binError;
    double dNSig = signalHist->GetBinError(i);
    double dNBkg = bkgHist->GetBinError(i);
    
    if (Significance == 0)
      binError = 0;
    else 
      binError = Significance * TMath::Sqrt( (dNSig / NSig)*(dNSig / NSig) + 0.25*(1.0/(NSig + NBkg + 0.2*0.2*NBkg*NBkg))*(1.0/(NSig + NBkg + 0.2*0.2*NBkg*NBkg))*(dNSig + dNBkg*(1+0.08*NBkg))*(dNSig + dNBkg*(1+0.08*NBkg)) );

    histogram->SetBinContent(i,Significance);
    histogram->SetBinError(i,binError);
  }
  return histogram;
}


//*************************************************************************************************
//Draws the signal and background histograms together and makes gif file
//*************************************************************************************************
void DrawSigBkgHistogram(TH1F* sig, TH1F* bkg, string histname) {
  string filename = histname + ".gif";
  TCanvas *cv = new TCanvas(histname.c_str(), histname.c_str(), 0,0,800,600);

  sig->SetLineColor(2);
  bkg->SetLineColor(4);

  TLegend *leg= new TLegend(0.5,0.7,0.75,0.8);
  leg->AddEntry(sig, "signal", "LP"); 
  leg->AddEntry(bkg, "bkg", "LP"); 

  //set best Y-range
  double tempMaxY1 = sig->GetMaximum();
  double tempMaxY2 = bkg->GetMaximum();
  double MaxY = max(tempMaxY1, tempMaxY2);
  sig->SetMinimum(0.0);
  sig->SetMaximum(1.1*MaxY);
  bkg->SetMinimum(0.0);
  bkg->SetMaximum(1.1*MaxY);
  sig->Draw("");
  bkg->Draw("same");
  leg->Draw();
  cv->SaveAs(filename.c_str());
  delete cv;
}


//*************************************************************************************************
//Draws the significance histograms and saves to gif file
//*************************************************************************************************
void DrawSignificanceHistogram(TH1F* hist, string histname) {
  string filename = histname + ".gif";
  TCanvas *cv = new TCanvas(histname.c_str(), histname.c_str(), 0,0,800,600);
  double MaxY = hist->GetMaximum();
  hist->SetMinimum(0.0);
  hist->SetMaximum(5.0);
  hist->Draw("");
  cv->SaveAs(filename.c_str());
  delete cv;
}


//*************************************************************************************************
//Finds the optimal cuts value from the significance histogram
//*************************************************************************************************
Float_t FindOptimalValue ( int variableIndex, string cutDirection, TH1F *significance ) {

  assert(variableIndex >= 0 && variableIndex < NVARIABLES);
  vector <pair<float, pair <float,float> > > localMaxima;
  pair<float, pair<float,float> > lowerBoundary;
  pair<float, pair<float, float> > upperBoundary;

  for (int i=1;i<=significance->GetXaxis()->GetNbins();i++) {
    double xvalue = 0.0;
    if (i <= significance->GetXaxis()->GetNbins()) {
      if (cutDirection == "cutabove") 
        xvalue = significance->GetXaxis()->GetBinUpEdge(i);
      else if (cutDirection == "cutbelow") 
        xvalue = significance->GetXaxis()->GetBinLowEdge(i);
      else {
        cerr << "Error : cutDirection = " << cutDirection << " is not supported. \n";
        assert(false);
      }
    }

    float yvalue = significance->GetBinContent(i);
    float yvalueError = significance->GetBinError(i);
    if (i > 1 && i < significance->GetXaxis()->GetNbins()) {
      float yvalue_left = significance->GetBinContent(i-1);
      float yvalue_right = significance->GetBinContent(i+1);

      if (yvalue >= yvalue_left && yvalue >= yvalue_right) {
        pair<float,float> maximum_yvalue(yvalue,yvalueError);
        pair<float,pair<float,float> > maximum(xvalue, maximum_yvalue);
        localMaxima.push_back(maximum);
      }
    } else if (i==1) {
      pair<float,float> lowerBoundary_yvalue(yvalue,yvalueError);
      lowerBoundary = pair<float,pair<float,float> >(xvalue,lowerBoundary_yvalue);
    } else if (i==significance->GetXaxis()->GetNbins()) {
      pair<float,float> upperBoundary_yvalue(yvalue,yvalueError);
      upperBoundary = pair<float,pair<float,float> >(xvalue,upperBoundary_yvalue);
    }
  }

  //sort the maxima by yvalue
  for(UInt_t i=0; i<localMaxima.size();i++) {
    for(UInt_t j=i+1; j<localMaxima.size();j++) {
      if(localMaxima[i].second.first < localMaxima[j].second.first) {
        pair<float,pair<float,float> > temp = localMaxima[i];
        localMaxima[i] = localMaxima[j];
        localMaxima[j] = temp;
      }
    }
  }

  //check the sort
  //cout << "Check Maxima sorting : variable = " << fOptimizationVariableNames[variableIndex] <<  " cutDirection = " << cutDirection << endl;
  //for(UInt_t i=0; i<localMaxima.size();i++)
    //cout << localMaxima[i].first << " " <<  localMaxima[i].second.first << endl;

  //threshold will be equal to the error on the best localMaximum. 
  //I think this doesn't cause problems if one of the boundaries is higher than the local maximum.
  
  Double_t significanceThreshold;
  if (localMaxima.size()>0)
    significanceThreshold = localMaxima[0].second.second;

  //find the global maximum
  pair<float, pair<float,float> > optimalPoint;
  pair<float, pair<float,float> > originalMaximum;
  if (localMaxima.size() > 0) {
    optimalPoint = localMaxima[0];
    originalMaximum = localMaxima[0];
  }
  for(UInt_t i=0; i<localMaxima.size();i++) {
    if (i==0)
      continue;
    if ( cutDirection == "cutbelow" ) { //list of variables for which we cut below
      if (originalMaximum.second.first - localMaxima[i].second.first < significanceThreshold && localMaxima[i].first < optimalPoint.first ) {
        optimalPoint = localMaxima[i];
      }
    } else if (cutDirection == "cutabove") { //list of variables for which we cut above
      if (originalMaximum.second.first - localMaxima[i].second.first < significanceThreshold && localMaxima[i].first > optimalPoint.first ) {
        optimalPoint = localMaxima[i];
      }
    } else {
      cerr << "Error : cutDirection = " << cutDirection << " is not supported. \n";
      assert(false);
    }
  }

  //cout << "Maximum out of local maxima : " << optimalPoint.first << " " << optimalPoint.second.first << endl;

  if ( cutDirection == "cutbelow" ) { 
    if (upperBoundary.second.first - originalMaximum.second.first > significanceThreshold)
      optimalPoint = upperBoundary;
    if (originalMaximum.second.first - lowerBoundary.second.first <= significanceThreshold)
      optimalPoint = lowerBoundary;
  } else if (cutDirection == "cutabove") {
    if (lowerBoundary.second.first - originalMaximum.second.first > significanceThreshold)
      optimalPoint = lowerBoundary;
    if (originalMaximum.second.first - upperBoundary.second.first <= significanceThreshold)
      optimalPoint = upperBoundary;
  } else {
    cerr << "Error : cutDirection = " << cutDirection << " is not supported. \n";
    assert(false);
  }

  //cout << "lowerBoundary : " << lowerBoundary.first << " " << lowerBoundary.second.first << endl;
  //cout << "upperBoundary : " << upperBoundary.first << " " << upperBoundary.second.first << endl;
  cout << "Global maxima : " << optimalPoint.first << " " << optimalPoint.second.first << endl;
  return optimalPoint.first;
}

//*************************************************************************************************
//Perform the Optimization
//*************************************************************************************************
void RunOptimization( MitNtupleEvent* signalEvent , MitNtupleEvent* bkgEvent, Int_t NSignalEntries, 
                      Int_t NBkgEntries,  string SignalSampleName , int maxIterations, int finalstate , bool saveIntermediatePlots) {
  string finalstatelabel = SignalSampleName;
  int finalstateIndex = -1;
  if (finalstate == 10) {
    finalstatelabel += "_mumu";
    finalstateIndex = 0;
  } else if (finalstate == 11) {
    finalstatelabel += "_ee";
    finalstateIndex = 1;
  } else if (finalstate == 12) {
    finalstatelabel += "_emu";
    finalstateIndex = 2;
  } else {
    cerr << "Error: final state = " << finalstate << " is not supported!\n";
    assert(false);
  }

  //Run Optimization Scheme
  bool continueOptimization = true;
  int NIterations = 0;
  cout << "*************************************************************************" << endl;
  cout << "Run Optimization for final state " << finalstate << endl;
  cout << "*************************************************************************" << endl;
  while (continueOptimization && NIterations < maxIterations) {
    cout << "Iteration #" << NIterations << endl;
    for (int v = 0; v < NVARIABLES;v++) {

      //First Optimize cutabove
      if (fCutAbove[v] && fOptimizeThisVariable[0][v]) {
        TH1F* signalDistribution = makeDistributionHistogram(v, "cutabove" , signalEvent, NSignalEntries, true, finalstate);
        TH1F* bkgDistribution = makeDistributionHistogram(v, "cutabove", bkgEvent, NBkgEntries, false, finalstate);
        TH1F* signalHist = makeNMinusOneHistogram(v, signalDistribution, true, "cutabove");
        TH1F* bkgHist = makeNMinusOneHistogram(v, bkgDistribution, false, "cutabove");
        TH1F* significanceHist = makeSignificanceHistogram(v, signalHist, bkgHist, 2);
        if (saveIntermediatePlots) {
          DrawSigBkgHistogram(signalDistribution, bkgDistribution, "Distribution_" + fOptimizationVariableNames[v] + finalstatelabel + "_cutabove_Iteration" + IntToString(NIterations));
          DrawSigBkgHistogram(signalHist, bkgHist, "NEventsPassed_" + fOptimizationVariableNames[v] + finalstatelabel + "_cutabove_Iteration" + IntToString(NIterations));
          DrawSignificanceHistogram(significanceHist, "Significance2_" + fOptimizationVariableNames[v] + finalstatelabel + "_cutabove_Iteration" + IntToString(NIterations));
        }
        
        float optimalValue = FindOptimalValue(v,"cutabove",significanceHist);
        fCutAboveInitialValue[finalstateIndex][v] = optimalValue;        
      }

      //Now Optimize cutbelow
      if (fCutBelow[v] && fOptimizeThisVariable[1][v]) {
        TH1F* signalDistribution = makeDistributionHistogram(v, "cutbelow", signalEvent, NSignalEntries, true, finalstate);
        TH1F* bkgDistribution = makeDistributionHistogram(v, "cutbelow", bkgEvent, NBkgEntries, false, finalstate);
        TH1F* signalHist = makeNMinusOneHistogram(v, signalDistribution, true, "cutbelow");
        TH1F* bkgHist = makeNMinusOneHistogram(v, bkgDistribution, false, "cutbelow");
        TH1F* significanceHist = makeSignificanceHistogram(v, signalHist, bkgHist, 2);
        if (saveIntermediatePlots) {
          DrawSigBkgHistogram(signalDistribution, bkgDistribution, "Distribution_" + fOptimizationVariableNames[v] + finalstatelabel + "_cutbelow_Iteration" + IntToString(NIterations));
          DrawSigBkgHistogram(signalHist, bkgHist, "NEventsPassed_" + fOptimizationVariableNames[v] + finalstatelabel + "_cutbelow_Iteration" + IntToString(NIterations));
          DrawSignificanceHistogram(significanceHist, "Significance2_" + fOptimizationVariableNames[v] + finalstatelabel + "_cutbelow_Iteration" + IntToString(NIterations));
        }

        float optimalValue = FindOptimalValue(v, "cutbelow", significanceHist);
        fCutBelowInitialValue[finalstateIndex][v] = optimalValue;       
      }

    } // loop over variables
    NIterations++;
  } //Optimization Iterations
}


//*************************************************************************************************
//Make Final Plots
//*************************************************************************************************
void MakeFinalOptimizationPlots( MitNtupleEvent* signalEvent , MitNtupleEvent* bkgEvent , Int_t NSignalEntries, 
                                 Int_t NBkgEntries, string SignalSampleName, int finalstate) {
  string finalstatelabel = SignalSampleName;
  if (finalstate == 10)
    finalstatelabel += "_mumu";
  else if (finalstate == 11)
    finalstatelabel += "_ee";
  else if (finalstate == 12)
    finalstatelabel += "_emu";

  for (int v = 0; v < NVARIABLES;v++) {

    //printf("begin %d\n", v);

    //for cutabove
    if (fCutAbove[v]) {
      TH1F* signalDistribution_cutabove = makeDistributionHistogram(v, "cutabove", signalEvent, NSignalEntries, true, finalstate);
      TH1F* bkgDistribution_cutabove = makeDistributionHistogram(v, "cutabove", bkgEvent, NBkgEntries, false, finalstate);
      DrawSigBkgHistogram(signalDistribution_cutabove, bkgDistribution_cutabove, "Distribution_" + fOptimizationVariableNames[v] + finalstatelabel + "_cutabove");
            
      TH1F *signalHist_cutabove = makeNMinusOneHistogram(v, signalDistribution_cutabove, true, "cutabove");
      TH1F *bkgHist_cutabove = makeNMinusOneHistogram(v, bkgDistribution_cutabove, false, "cutabove");
      DrawSigBkgHistogram(signalHist_cutabove, bkgHist_cutabove, 
                          "NEventsPassed_" + fOptimizationVariableNames[v] + finalstatelabel + "_cutabove");
      
      TH1F *significanceHist_cutabove = makeSignificanceHistogram(v, signalHist_cutabove, bkgHist_cutabove, 2);
      DrawSignificanceHistogram(significanceHist_cutabove , 
                                "Significance2_" + fOptimizationVariableNames[v] + finalstatelabel + "_cutabove");
      
    }
    //for cutbelow
    if (fCutBelow[v]) {
      TH1F* signalDistribution_cutbelow = makeDistributionHistogram(v, "cutbelow", signalEvent, NSignalEntries, true, finalstate);
      TH1F* bkgDistribution_cutbelow = makeDistributionHistogram(v, "cutbelow", bkgEvent, NBkgEntries, false, finalstate);
      DrawSigBkgHistogram(signalDistribution_cutbelow, bkgDistribution_cutbelow, "Distribution_" + fOptimizationVariableNames[v] + finalstatelabel + "_cutbelow");
      
      
      TH1F *signalHist_cutbelow = makeNMinusOneHistogram(v, signalDistribution_cutbelow, true,  "cutbelow");
      TH1F *bkgHist_cutbelow = makeNMinusOneHistogram(v, bkgDistribution_cutbelow, false, "cutbelow");
      DrawSigBkgHistogram(signalHist_cutbelow, bkgHist_cutbelow, 
                          "NEventsPassed_" + fOptimizationVariableNames[v] + finalstatelabel+ "_cutbelow");
      
      TH1F* significanceHist_cutbelow = makeSignificanceHistogram(v, signalHist_cutbelow, bkgHist_cutbelow, 2);
      DrawSignificanceHistogram(significanceHist_cutbelow, 
                                "Significance2_" + fOptimizationVariableNames[v] + finalstatelabel + "_cutbelow");
      
    }   
  }
}

//*************************************************************************************************
//Print Final Results
//*************************************************************************************************
void PrintFinalResults( MitNtupleEvent* signalEvent , MitNtupleEvent* bkgEvent , Int_t NSignalEntries, 
                                 Int_t NBkgEntries, string SignalSampleName, int OnlyThisFinalState) {
  string finalstatelabel = SignalSampleName;
  if (OnlyThisFinalState == 10)
    finalstatelabel += "_mumu";
  else if (OnlyThisFinalState == 11)
    finalstatelabel += "_ee";
  else if (OnlyThisFinalState == 12)
    finalstatelabel += "_emu";


  //add up signal
  Double_t TotalSignalCount = 0.0;
  for (int n=0;n<NSignalEntries;n++) { 

    signalEvent->GetEntry(n);
    float eventweight = signalEvent->H_weight;
    int finalstatetype = (int)signalEvent->H_ltype;
    if (!(finalstatetype >= 10 && finalstatetype <= 12)) {
      //cout << "finalstatetype = " << finalstatetype << " unrecognized" << endl;
      //assert(!(finalstatetype >= 10 && finalstatetype <= 12));
    }
    //only look at events with the given finalstatetype
    if (OnlyThisFinalState >= 0 && OnlyThisFinalState != finalstatetype)
      continue;

    Float_t cutAboveValues[NVARIABLES];
    Float_t cutBelowValues[NVARIABLES];
    Float_t variableValues[NVARIABLES];   

    //only look at events with the given finalstatetype
    //convert finalstatetype into the array index
    int fs;
    if (finalstatetype == 10)
      fs = 0;
    else if (finalstatetype == 11)
      fs = 1;
    else if (finalstatetype == 12)
      fs = 2;
    else {
      fs = -1;
      continue;
    }

    for (int j=0;j<NVARIABLES;j++) {
      cutAboveValues[j] = fCutAboveInitialValue[fs][j];
      cutBelowValues[j] = fCutBelowInitialValue[fs][j];
      variableValues[j] = GetVariableValue(j, signalEvent);      
    }
    if (passCut(signalEvent,variableValues, cutAboveValues, cutBelowValues)) {
      TotalSignalCount += eventweight;
    }
  }

  //add up bkg
  Double_t TotalBkg = 0.0;
  vector < pair<int , double> > TotalBkgCount;
  
  for (int n=0;n<NBkgEntries;n++) { 

    bkgEvent->GetEntry(n);
    float eventweight = bkgEvent->H_weight;
    int finalstatetype = (int)bkgEvent->H_ltype;
    int decay = (int)bkgEvent->H_decay;

    //only look at events with the given finalstatetype
    if (OnlyThisFinalState >= 0 && OnlyThisFinalState != finalstatetype)
      continue;

    Float_t cutAboveValues[NVARIABLES];
    Float_t cutBelowValues[NVARIABLES];
    Float_t variableValues[NVARIABLES];   

    //only look at events with the given finalstatetype
    //convert finalstatetype into the array index
    int fs;
    if (finalstatetype == 10)
      fs = 0;
    else if (finalstatetype == 11)
      fs = 1;
    else if (finalstatetype == 12)
      fs = 2;
    else {
      fs = -1;
      continue;
    }

    for (int j=0;j<NVARIABLES;j++) {
      cutAboveValues[j] = fCutAboveInitialValue[fs][j];
      cutBelowValues[j] = fCutBelowInitialValue[fs][j];
      variableValues[j] = GetVariableValue(j, bkgEvent);      
    }
    if (passCut(bkgEvent,variableValues, cutAboveValues, cutBelowValues)) {
      TotalBkg += eventweight;

      bool foundProcess = false;
      int foundIndex = -1;
      
      for (int p=0;p<TotalBkgCount.size();p++) {

        //cout << "Check TotalBkgCount " << p << " " << TotalBkgCount[p].first << " " <<  TotalBkgCount[p].second << endl;
        if (decay == TotalBkgCount[p].first) {
          foundProcess = true;
          foundIndex = p;
        }
      }
      if (!foundProcess) {
        pair< int, double > newBkg(decay,eventweight);
        TotalBkgCount.push_back(newBkg);
      } else {
        TotalBkgCount[foundIndex].second = TotalBkgCount[foundIndex].second + eventweight;
      }
    }
  }

  cout << "For Final State : " << finalstatelabel << endl;
  cout << "Total Signal : " << TotalSignalCount << endl;
  cout << "Total Bkg : " << TotalBkg << endl;
  for (int p=0;p<TotalBkgCount.size();p++) {      
    //find the process
    int decayIndex = -1;
    for (int k=0;k<NPROCESSES;k++) {
      if (fProcessCode[k] == TotalBkgCount[p].first)
        decayIndex = k;
    }
    
    if (decayIndex >= 0) {
      cout << "Bkg " << fProcessName[decayIndex] << " " << TotalBkgCount[p].second << endl;
    }       
  }
}

//*************************************************************************************************
//To save the optimal cut values into an TArray and save to file
//*************************************************************************************************
void saveOptimalValues(string outputFilename, int finalstate, string SignalName) {
  int fs;
  string finalstateString = "";
  if (finalstate == 10) {
    fs = 0;
    finalstateString = "mumu";
  } else if (finalstate == 11) {
    fs = 1;
    finalstateString = "ee";
  } else if (finalstate == 12) {
    fs = 2;
    finalstateString = "emu";
  } else {
    cerr << "Error: Finalstate = " << finalstate << " is not supported\n";
    assert(false);
  }

  string cutAboveHistName = "OptimalCutAboveValues_" + SignalName + "_" + finalstateString;
  string cutBelowHistName = "OptimalCutBelowValues_" + SignalName + "_" + finalstateString;

  TFile *file = new TFile(outputFilename.c_str(), "UPDATE");
  file->cd();  

  //Try to find histograms with same name in the output file.
  file->Delete((cutAboveHistName+";*").c_str());
  file->Delete((cutBelowHistName+";*").c_str());

  TH1F *OptimalCutAboveValues = new TH1F(cutAboveHistName.c_str(),
                                         "Cut Index",NVARIABLES, 0, NVARIABLES-1);
  TH1F *OptimalCutBelowValues = new TH1F(cutBelowHistName.c_str(),
                                         "Cut Index",NVARIABLES, 0, NVARIABLES-1);
  for (int v=1;v<=NVARIABLES;v++) {
    OptimalCutAboveValues->SetBinContent(v,fCutAboveInitialValue[fs][v-1]);
    OptimalCutBelowValues->SetBinContent(v,fCutBelowInitialValue[fs][v-1]);      
  }    

  //Create root file to store fake rates
  file->WriteTObject(OptimalCutAboveValues);
  file->WriteTObject(OptimalCutBelowValues);
  file->Write();
  file->Close();
}

//*************************************************************************************************
//Main part of the macro
//*************************************************************************************************
void HWWOptimization(const string signalInputFilename, const string bkgInputFilename, 
                     string SignalSampleName, string ResultsFilename, int OnlyThisFinalState = -1, 
                     bool runOptimization = false, int maxIterations = 1, 
                     bool saveIntermediatePlots = false) {
  TTree* signalTree = getTreeFromFile(signalInputFilename.c_str(),"all");
  TTree* bkgTree = getTreeFromFile(bkgInputFilename.c_str(),"all");
  assert(signalTree);
  assert(bkgTree);

  CheckInitialValueConsistency();


  MitNtupleEvent signalEvent(signalTree);
  MitNtupleEvent bkgEvent(bkgTree);
  
  if (runOptimization) {
    if (OnlyThisFinalState == 0) {
      RunOptimization( &signalEvent, &bkgEvent, signalTree->GetEntries(),  bkgTree->GetEntries(), SignalSampleName,
                       maxIterations, OnlyThisFinalState , saveIntermediatePlots);
    } else {
      //Optimize Each Final State separately
      for (int f=10; f<=12; f++) {
        if (OnlyThisFinalState == -1 || OnlyThisFinalState == f)
          RunOptimization( &signalEvent, &bkgEvent, signalTree->GetEntries(),  bkgTree->GetEntries(), SignalSampleName,
                           maxIterations, f , saveIntermediatePlots);
      }
    } 
  }

  cout << "*************************************************************************" << endl;
  cout << "Final Optimal Cuts" << endl;
  cout << "*************************************************************************" << endl;
  for (int f=10; f<=12;f++) {
    int finalstateIndex;
    if (f == 10) {
      finalstateIndex = 0;
    } else if (f == 11) {
      finalstateIndex = 1;
    } else if (f == 12) {
      finalstateIndex = 2;
    } else {
      cerr << "finalstate is wrong\n";
      assert(false);
    }
    
    cout << "Final State : " << f << endl;
    for (int v = 0; v < NVARIABLES;v++) {
      cout << fOptimizationVariableNames[v] << " : ";
      if (fCutBelow[v])
        cout << fCutBelowInitialValue[finalstateIndex][v];
      else 
        cout << "-Infinity ";
      cout << " -> ";
      if (fCutAbove[v])
        cout << fCutAboveInitialValue[finalstateIndex][v];      
      else 
        cout << " Infinity ";
      cout << endl;
    }
    cout << "----------------------------------------------------------------------" << endl;
  }


  cout << "*************************************************************************" << endl;
  cout << "Making Final Plots" << endl;
  cout << "*************************************************************************" << endl;
  if (OnlyThisFinalState == -1) {
    for (int f=10; f<=12;f++) {
      MakeFinalOptimizationPlots(&signalEvent , &bkgEvent , signalTree->GetEntries(), 
                                 bkgTree->GetEntries(), SignalSampleName, f);
    }
  } else {
    MakeFinalOptimizationPlots(&signalEvent , &bkgEvent , signalTree->GetEntries(), 
                               bkgTree->GetEntries(), SignalSampleName, OnlyThisFinalState);
  }


  cout << "*************************************************************************" << endl;
  cout << "Final Results" << endl;
  cout << "*************************************************************************" << endl;
  if (OnlyThisFinalState == -1) {
    for (int f=10; f<=12;f++) {
      PrintFinalResults(&signalEvent , &bkgEvent , signalTree->GetEntries(), 
                        bkgTree->GetEntries(), SignalSampleName, f);
    }
  } else {
    PrintFinalResults(&signalEvent , &bkgEvent , signalTree->GetEntries(), 
                      bkgTree->GetEntries(), SignalSampleName, OnlyThisFinalState);
  }
  

  cout << "*************************************************************************" << endl;
  cout << "Saving Results" << endl;
  cout << "*************************************************************************" << endl;  
  if (OnlyThisFinalState == -1) {
    for (int f=10; f<=12;f++) {
      saveOptimalValues(ResultsFilename, f, SignalSampleName);     
    } 
  }else {    
    saveOptimalValues(ResultsFilename, OnlyThisFinalState, SignalSampleName);
  }

}

