// $Id: FakeRate.cc,v 1.3 2009/06/30 08:20:07 phedex Exp $

#include "MitPhysics/Init/interface/FakeRate.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TF2.h"

using namespace mithep;
using namespace std;

ClassImp(mithep::FakeRate)

//--------------------------------------------------------------------------------------------------
Bool_t FakeRate::Init()
{
  //Load all histogram files.

  //Get the root file which is storing the fake rates
  TFile *ElectronFRFile = new TFile(fElectronFRFilename);  
  if (!ElectronFRFile) {
    cerr << "The Electron FakeRate file : " << fElectronFRFilename << " could not be opened!\n";
    return false;
  }
  TFile *MuonFRFile = new TFile(fMuonFRFilename);  
  if (!MuonFRFile) {
    cerr << "The Muon FakeRate file : " << fMuonFRFilename << " could not be opened!\n";
    return false;
  }
  
  // *****************************************************************************************
  // Load Histogram / Fit Functions from the file
  // *****************************************************************************************
  if (fUse2DFakeRate) {

    fElectronFakeRateHist_PtEta = (TH2F*)(ElectronFRFile->Get(fElectronFRHistName));
    fMuonFakeRateHist_PtEta = (TH2F*)(MuonFRFile->Get(fMuonFRHistName));
    if (!fElectronFakeRateHist_PtEta) {
      cout << "Error: Histogram " << fElectronFRHistName << " cannot be loaded from file "
           << fElectronFRFilename << endl;
    }
    if (!fMuonFakeRateHist_PtEta) {
      cout << "Error: Histogram " << fMuonFRHistName << " cannot be loaded. from file"
           << fMuonFRFilename << endl;
    }

    fElectronFakeRateHist_PtEta->SetDirectory(0);
    fMuonFakeRateHist_PtEta->SetDirectory(0);

    if (fUseFitFunction) {
      //Currently unsupported
      cerr << "Error: Using 2D Fake Rates with Fit Function is not currently supported.\n";
      return false;

      //Once supported, below code will load the functions.
      TF2 *ElectronFakeRateFit_PtEta_temp = (TF2*)(ElectronFRFile->Get(fElectronFRFunctionName));
      TF2 *MuonFakeRateFit_PtEta_temp = (TF2*)(MuonFRFile->Get(fMuonFRFunctionName));
      if (!ElectronFakeRateFit_PtEta_temp) {
        cout << "Error: Function " << fElectronFRFunctionName << " cannot be loaded from file "
             << fElectronFRFilename << endl;
      }
      if (!MuonFakeRateFit_PtEta_temp) {
      cout << "Error: Function " << fMuonFRFunctionName << " cannot be loaded. from file"
           << fMuonFRFilename << endl;
      }
      fElectronFakeRateFit_PtEta = (TF2*)(ElectronFakeRateFit_PtEta_temp->Clone());
      fMuonFakeRateFit_PtEta = (TF2*)(MuonFakeRateFit_PtEta_temp->Clone());
    }

  } else {

    fElectronFakeRateHist_Pt = (TH1F*)(ElectronFRFile->Get(fElectronFRHistName)); 
    fMuonFakeRateHist_Pt = (TH1F*)(MuonFRFile->Get(fMuonFRHistName)); 
    if (!fElectronFakeRateHist_Pt) {
      cout << "Error: Histogram " << fElectronFRHistName << " cannot be loaded from file "
           << fElectronFRFilename << endl;
    }
    if (!fMuonFakeRateHist_Pt) {
      cout << "Error: Histogram " << fMuonFRHistName << " cannot be loaded. from file"
           << fMuonFRFilename << endl;
    }
    fElectronFakeRateHist_Pt->SetDirectory(0);
    fMuonFakeRateHist_Pt->SetDirectory(0);

    if (fUseFitFunction) {
      TF1 *ElectronFakeRateFit_Pt_temp = (TF1*)(ElectronFRFile->Get(fElectronFRFunctionName));
      TF1 *MuonFakeRateFit_Pt_temp = (TF1*)(MuonFRFile->Get(fMuonFRFunctionName));
      if (!ElectronFakeRateFit_Pt_temp) {
        cout << "Error: Function " << fElectronFRFunctionName << " cannot be loaded from file "
             << fElectronFRFilename << endl;
      }
      if (!MuonFakeRateFit_Pt_temp) {
      cout << "Error: Function " << fMuonFRFunctionName << " cannot be loaded. from file"
           << fMuonFRFilename << endl;
      }
      fElectronFakeRateFit_Pt = (TF1*)(ElectronFakeRateFit_Pt_temp->Clone());
      fMuonFakeRateFit_Pt = (TF1*)(MuonFakeRateFit_Pt_temp->Clone());
    }
  }


  fElectronFakeRateHist_PtEta_sysError = (TH2F*)(ElectronFRFile->Get("RecoElectronFakeRate_PtEta_Madgraph_SysErrors"));
  fMuonFakeRateHist_PtEta_sysError = (TH2F*)(MuonFRFile->Get("TrackerMuonFakeRate_PtEta_Madgraph_SysErrors"));
  if (!fElectronFakeRateHist_PtEta_sysError) {
    cout << "Error: Histogram RecoElectronFakeRate_PtEta_Madgraph_SysErrors cannot be loaded from file "
         << endl;
  }
  if (!fMuonFakeRateHist_PtEta_sysError) {
    cout << "Error: Histogram TrackerMuonFakeRate_PtEta_Madgraph_SysErrors cannot be loaded. from file"
         << endl;
  }
  fElectronFakeRateHist_PtEta_sysError->SetDirectory(0);
  fMuonFakeRateHist_PtEta_sysError->SetDirectory(0);


  ElectronFRFile->Close();
  MuonFRFile->Close();
  delete ElectronFRFile;
  delete MuonFRFile;

  return true;
}


//--------------------------------------------------------------------------------------------------
Double_t FakeRate::ElectronFakeRate(Double_t pt, Double_t eta, Double_t phi)
{
  // Calculate the electron fake rate given pt, eta, and phi
  Double_t prob = 0.0;

  if (fIsInit) {
    if (fUse2DFakeRate) {
      if (fUseFitFunction) {
        cerr << "Error: Using 2D Fake Rates with Fit Function is not currently supported.\n";      
      } else {
        if (fElectronFakeRateHist_PtEta) {
          Int_t ptbin = fElectronFakeRateHist_PtEta->GetXaxis()->FindFixBin(pt);
          Int_t etabin = fElectronFakeRateHist_PtEta->GetYaxis()->FindFixBin(eta);    
          prob = fElectronFakeRateHist_PtEta->GetBinContent(ptbin,etabin);
        } else {
          cerr << "Error: fElectronFakeRateHist_PtEta was not loaded properly.\n";
          assert(false);
        }
      }
    } else {
      if (fUseFitFunction) {
        if (fElectronFakeRateFit_Pt) {
          prob = fElectronFakeRateFit_Pt->Eval(pt);
        } else {
          cerr << "Error: fElectronFakeRateFit_Pt was not loaded properly.\n";
          assert(false);
        }
      } else {
        if (fElectronFakeRateHist_Pt) {
          Int_t ptbin = fElectronFakeRateHist_Pt->GetXaxis()->FindFixBin(pt);
          prob = fElectronFakeRateHist_Pt->GetBinContent(ptbin);
        } else {
          cerr << "Error: fElectronFakeRateHist_Pt was not loaded properly.\n";
          assert(false);
        }
      }
    }
  } else {
    cerr << "Error: FakeRate was not properly initialized. \n";
    assert(false);
  }
  return prob;
}

//--------------------------------------------------------------------------------------------------
Double_t FakeRate::ElectronFakeRateError(Double_t pt, Double_t eta, Double_t phi)
{
  // Calculate the electron fake rate given pt, eta, and phi
  Double_t error = 0.0;

  if (fIsInit) {
    if (fUse2DFakeRate) {
      if (fUseFitFunction) {
        cerr << "Error: Using 2D Fake Rates with Fit Function is not currently supported.\n";      
      } else {
        if (fElectronFakeRateHist_PtEta_sysError) {
          Int_t ptbin = fElectronFakeRateHist_PtEta_sysError->GetXaxis()->FindFixBin(pt);
          Int_t etabin = fElectronFakeRateHist_PtEta_sysError->GetYaxis()->FindFixBin(eta);    
          error = fElectronFakeRateHist_PtEta_sysError->GetBinContent(ptbin,etabin);

          if (isnan(error)) {
            cerr << "Error: ElectronFakeRateError(" << ptbin << "," << etabin << ") = NAN.\n";
            error = 0;              
          }
        } else {
          cerr << "Error: fElectronFakeRateHist_PtEta_sysError was not loaded properly.\n";
          assert(false);
        }
      }
    } 
  } else {
    cerr << "Error: FakeRate was not properly initialized. \n";
    assert(false);
  }
  return error;
}


//--------------------------------------------------------------------------------------------------
Double_t FakeRate::MuonFakeRate(Double_t pt, Double_t eta, Double_t phi)
{
  // Calculate the muon fake rate given pt, eta, and phi
  Double_t prob = 0.0;

  if (fIsInit) {
    if (fUse2DFakeRate) {
      if (fUseFitFunction) {
        cerr << "Error: Using 2D Fake Rates with Fit Function is not currently supported.\n";      
      } else {
        if (fMuonFakeRateHist_PtEta) {
          Int_t ptbin = fMuonFakeRateHist_PtEta->GetXaxis()->FindFixBin(pt);
          Int_t etabin = fMuonFakeRateHist_PtEta->GetYaxis()->FindFixBin(eta);    
          prob = fMuonFakeRateHist_PtEta->GetBinContent(ptbin,etabin);
        } else {
          cerr << "Error: fMuonFakeRateHist_PtEta was not loaded properly.\n";
          assert(false);
        }
      }
    } else {
      if (fUseFitFunction) {
        if (fMuonFakeRateFit_Pt) {
          prob = fMuonFakeRateFit_Pt->Eval(pt);
        } else {
          cerr << "Error: fMuonFakeRateFit_Pt was not loaded properly.\n";
          assert(false);
        }
      } else {
        if (fMuonFakeRateHist_Pt) {
          Int_t ptbin = fMuonFakeRateHist_Pt->GetXaxis()->FindFixBin(pt);
          prob = fMuonFakeRateHist_Pt->GetBinContent(ptbin);
        } else {
          cerr << "Error: fMuonFakeRateHist_Pt was not loaded properly.\n";
          assert(false);
        }
      }
    }
  } else {
    cerr << "Error: FakeRate was not properly initialized. \n";
    assert(false);
  }
  return prob;
}

//--------------------------------------------------------------------------------------------------
Double_t FakeRate::MuonFakeRateError(Double_t pt, Double_t eta, Double_t phi)
{
  // Calculate the muon fake rate given pt, eta, and phi
  Double_t error = 0.0;

  if (fIsInit) {
    if (fUse2DFakeRate) {
      if (fUseFitFunction) {
        cerr << "Error: Using 2D Fake Rates with Fit Function is not currently supported.\n";      
      } else {
        if (fMuonFakeRateHist_PtEta_sysError) {
          Int_t ptbin = fMuonFakeRateHist_PtEta_sysError->GetXaxis()->FindFixBin(pt);
          Int_t etabin = fMuonFakeRateHist_PtEta_sysError->GetYaxis()->FindFixBin(eta);    
          error = fMuonFakeRateHist_PtEta_sysError->GetBinContent(ptbin,etabin);

          if (isnan(error)) {
            cerr << "Error: ElectronFakeRateError(" << ptbin << "," << etabin << ") = NAN.\n";
            error = 0;              
          }
        } else {
          cerr << "Error: fMuonFakeRateHist_PtEta_sysError was not loaded properly.\n";
          assert(false);
        }
      }
    } 
  } else {
    cerr << "Error: FakeRate was not properly initialized. \n";
    assert(false);
  }
  return error;
}
