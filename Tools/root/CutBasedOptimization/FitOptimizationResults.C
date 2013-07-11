
//Do Optimization
//root -l -b -q FitOptimizationResults.C+\(\"/home/sixie/Tools/root/CutBasedOptimization/HWWCutOptimizationResults.root\",10\)



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
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TArrayF.h"
#include "MitNtupleEvent.C"
#include <vector>
//#include <pair>

#define NSIGNAL    6
#define NVARIABLES 10
#define NPROCESSES 32
#define NBINS      200
//*************************************************************************************************
//Define Global Variables for the macro
//*************************************************************************************************

string fSignalName[NSIGNAL] = {"HWW130", "HWW150","HWW160", "HWW170", "HWW200","HWW500" };
double fSignalMass[NSIGNAL] = {130, 150,160, 170, 200,500 };



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

TF1 *performFit(const string inputFile, int variableIndex, string cutDirection, int OnlyThisFinalState = -1 ) {

  string finalstateString = "";
  if (OnlyThisFinalState == 10) {
    finalstateString = "_mumu";
  } else if (OnlyThisFinalState == 11) {
    finalstateString = "_ee";
  } else if (OnlyThisFinalState == 12) {
    finalstateString = "_emu";
  } else {
    cerr << "Error: Finalstate = " << OnlyThisFinalState << " is not supported\n";
    assert(false);
  }
  
  string cutDirectionString = "";
  if (cutDirection == "cutabove") {
    cutDirectionString = "CutAbove";
  } else if (cutDirection == "cutbelow") {
    cutDirectionString = "CutBelow";
  } else {
    cerr << "Error: cutDirection == " << cutDirection << " is not supported.\n";
    assert(false);
  }

  string plotname = fOptimizationVariableNames[variableIndex]+"Optimal" +cutDirectionString+"ValuesVsMass"+finalstateString;
  TCanvas *cv = new TCanvas("cv", plotname.c_str(), 800, 600);

  TFile *file = new TFile(inputFile.c_str(), "READ");
  file->cd();  

  TH1F *tmp = new TH1F( (fOptimizationVariableNames[variableIndex]+"Optimal" +cutDirectionString+"ValuesVsMass"+finalstateString).c_str(), (";Signal Mass;"+fOptimizationVariableNames[variableIndex]).c_str(),
                        (Int_t)(fSignalMass[NSIGNAL-1]-fSignalMass[0]+21), fSignalMass[0]-10.0, fSignalMass[NSIGNAL-1]+10);

  for (int s=0;s<NSIGNAL;s++) {
    
    TH1F *hist = (TH1F*)file->FindObjectAny(("Optimal" +cutDirectionString+"Values_"+fSignalName[s]+finalstateString).c_str());
    
    if (hist) {
      tmp->Fill(fSignalMass[s],hist->GetBinContent(variableIndex+1));
      cerr << hist << " " << s << " " << variableIndex << " " << hist->GetBinContent(variableIndex+1) << endl;
    } else {
      cerr << "cannot find hist " << ("Optimal" +cutDirectionString+"Values_"+fSignalName[s]+finalstateString) << " " << hist << endl;
    }
  }

  TF1 *fitResult = new TF1("fitFunction", "[0]*x + [1]" ,-1000 ,1000);
  tmp->Fit("fitFunction");
  tmp->SetMarkerStyle(20);  
  tmp->SetMarkerSize(1.0);  
  tmp->Draw("E1");
  //fitResult->Draw("same");  
  
  string filename = plotname + ".gif";
  cv->SaveAs(filename.c_str());
  cv->Delete();

  fitResult->SetName((fOptimizationVariableNames[variableIndex]+"Optimal" +cutDirectionString+"ValuesVsMassFit"+finalstateString).c_str());
  return fitResult;

}
                     


//*************************************************************************************************
//Main part of the macro
//*************************************************************************************************
void FitOptimizationResults(const string inputFile, int OnlyThisFinalState = -1 
                     ) {


  for (int v=0; v<NVARIABLES;v++) {
    if (OnlyThisFinalState == -1) {
      for (int f=10;f<=12;f++) {
        if (fOptimizeThisVariable[0][v] && fCutAbove[v]) {
          TF1 *cutAboveValuesFit = performFit(inputFile,v,"cutabove",f);
        }
        if (fOptimizeThisVariable[1][v] && fCutBelow[v]) {
          TF1 *cutBelowValuesFit = performFit(inputFile,v,"cutbelow",f);
        }
      }
    } else {     
      if (fOptimizeThisVariable[0][v] && fCutAbove[v]) {
        TF1 *cutAboveValuesFit = performFit(inputFile,v,"cutabove", OnlyThisFinalState);
      }
      if (fOptimizeThisVariable[1][v] && fCutBelow[v]) {
        TF1 *cutBelowValuesFit = performFit(inputFile,v,"cutbelow",OnlyThisFinalState);
      }
    }
  }

  return;

}

