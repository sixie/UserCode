 // $Id: BambuToTextMod.cc,v 1.1 2008/11/29 18:51:07 sixie Exp $

#include "MitPhysics/Validation/interface/BambuToTextMod.h"
#include <TH1D.h>
#include <TH2D.h>
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace mithep;
ClassImp(mithep::BambuToTextMod)

//--------------------------------------------------------------------------------------------------
BambuToTextMod::BambuToTextMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fPrintDebug(false),
  fOutputFilename(TString("VistaOutputFile.txt")),
  fProcessName(TString("DefaultMCProcess")),
  fMCPartName(Names::gkMCPartBrn),
  fCleanMuonName(Names::gkMuonBrn),
  fCleanElectronName(Names::gkElectronBrn),
  fCleanPhotonName(Names::gkPhotonBrn),
  fCleanJetName(TString("CleanJets")),   
  fMetName(Names::gkCaloMetBrn),
  fParticles(0),
  fMuons(0),	             
  fElectrons(0),	     
  fPhotons(0),	     
  fJets(0),             
  fMet(0),
  fNEventsProcessed(0)
{
  // Constructor.
  
}

//--------------------------------------------------------------------------------------------------
void BambuToTextMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void BambuToTextMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and  
  fNEventsProcessed++;

  if (fNEventsProcessed % 1000 == 0 || fPrintDebug) {
    time_t systime;
    systime = time(NULL);
    cerr << endl << "BambuToTextMod : Process Event " << fNEventsProcessed << "  Time: " << ctime(&systime) << endl;
  }

  //string to accumulate Vista information
  std::stringstream hepEvtStream;

  hepEvtStream << fProcessName << " " << GetEventHeader()->RunNum() << "." 
               << GetEventHeader()->EvtNum() << " " << "1" << " " 
               << "pp 10000 0.0 0.0 ";


  //Obtain all the good objects from the event cleaning module
  ObjArray<Muon> *CleanMuons = dynamic_cast<ObjArray<Muon>* >(FindObjThisEvt(fCleanMuonName));
  ObjArray<Electron> *CleanElectrons = dynamic_cast<ObjArray<Electron>* >
    (FindObjThisEvt(fCleanElectronName));
  ObjArray<Photon> *CleanPhotons = dynamic_cast<ObjArray<Photon>* >(FindObjThisEvt(fCleanPhotonName));
  ObjArray<Jet> *CleanJets = dynamic_cast<ObjArray<Jet>* >
    (FindObjThisEvt(fCleanJetName));

  LoadBranch(fMetName);
  Met *caloMet = fMet->At(0);

  //clustered objects px, py
  double cl_px = 0.0;
  double cl_py = 0.0;

  //***********************************************************************************************
  //Muons
  //***********************************************************************************************

  if(CleanMuons) {    
    for (UInt_t j=0; j<CleanMuons->GetEntries(); j++) {
      string charge;
      if (CleanMuons->At(j)->Charge() > 0)
      charge = "+";
      else 
        charge = "-";
      hepEvtStream << "mu" << charge << " " << CleanMuons->At(j)->Pt() << " " 
                   << CleanMuons->At(j)->Eta() << " " << CleanMuons->At(j)->Phi() 
                   << " ";
      cl_px += CleanMuons->At(j)->Px();
      cl_py += CleanMuons->At(j)->Py();
    }
  }

  //***********************************************************************************************
  //Electrons
  //***********************************************************************************************
  if (CleanElectrons) {
    for (UInt_t j=0; j<CleanElectrons->GetEntries(); j++) {       
      string charge;
      if (CleanElectrons->At(j)->Charge() > 0)
        charge = "+";
      else 
        charge = "-";
      hepEvtStream << "e" << charge << " " << CleanElectrons->At(j)->Pt() << " " 
                   << CleanElectrons->At(j)->Eta() << " " << CleanElectrons->At(j)->Phi() 
                   << " ";    
      cl_px += CleanElectrons->At(j)->Px();
      cl_py += CleanElectrons->At(j)->Py();
    }
  }

  //***********************************************************************************************
  //Photons
  //***********************************************************************************************
  if (CleanPhotons) {
    for (UInt_t j=0; j<CleanPhotons->GetEntries(); j++) {       
      hepEvtStream << "ph" << " " << CleanPhotons->At(j)->Pt() << " " 
                   << CleanPhotons->At(j)->Eta() << " " << CleanPhotons->At(j)->Phi() 
                   << " ";    
      cl_px += CleanPhotons->At(j)->Px();
      cl_py += CleanPhotons->At(j)->Py();
    }
  }
  //***********************************************************************************************
  //Jets
  //***********************************************************************************************
  if (CleanJets) {
    for (UInt_t j=0; j<CleanJets->GetEntries(); j++) {       
      hepEvtStream << "j" << " " << CleanJets->At(j)->Mass() << " " << CleanJets->At(j)->Pt() << " " 
                   << CleanJets->At(j)->Eta() << " " << CleanJets->At(j)->Phi() 
                   << " ";    
    cl_px += CleanJets->At(j)->Px();
    cl_py += CleanJets->At(j)->Py();
    }
  }

  //***********************************************************************************************
  //Make Uncl energy = - (all particles momentum + met)
  //***********************************************************************************************
  double uncl_px = 0.0 - cl_px - caloMet->Px();
  double uncl_py = 0.0 - cl_py - caloMet->Py();

  FourVector *UnclEnergy = new FourVector(uncl_px, uncl_py,0, 
                                     TMath::Sqrt(uncl_px*uncl_px+uncl_py*uncl_py));
  hepEvtStream  << "uncl  0 " << UnclEnergy->Pt() << " 0 "  
                << UnclEnergy->Phi() << " ";

  hepEvtStream << ";" << endl;
  fOutputFile << hepEvtStream.str();

}
//--------------------------------------------------------------------------------------------------
void BambuToTextMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fMetName, fMet);
  ReqBranch(fMCPartName, fParticles);
  fOutputFile.open(fOutputFilename.Data());
}

//--------------------------------------------------------------------------------------------------
void BambuToTextMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.

  fOutputFile.close();

}

//--------------------------------------------------------------------------------------------------
void BambuToTextMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
  
}
