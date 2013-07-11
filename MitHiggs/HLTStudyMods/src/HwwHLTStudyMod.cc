 // $Id: HwwHLTStudyMod.cc,v 1.2 2008/12/04 19:19:49 sixie Exp $

#include "MitHiggs/HLTStudyMods/interface/HwwHLTStudyMod.h"
#include <TH1D.h>
#include <TH2D.h>
#include "MitAna/DataTree/interface/Names.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitCommon/MathTools/interface/MathUtils.h"

using namespace mithep;
ClassImp(mithep::HwwHLTStudyMod)

//--------------------------------------------------------------------------------------------------
HwwHLTStudyMod::HwwHLTStudyMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fPreSelectionType("Loose"),
  fDoHLTStudy(false),
  fPrintDebug(false),
  fMuonName(Names::gkMuonBrn),
  fElectronName(Names::gkElectronBrn),
  fMuons(0),
  fElectrons(0),
  fNEventsProcessed(0),
  fNEventsPassed(0)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void HwwHLTStudyMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.

}

//--------------------------------------------------------------------------------------------------
void HwwHLTStudyMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches an
  fNEventsProcessed++;

  if (fNEventsProcessed % 1000 == 0 || fPrintDebug) {
    time_t systime;
    systime = time(NULL);
    cerr << endl << "HwwHLTStudyMod : Process Event " << fNEventsProcessed << "  Time: " << ctime(&systime) << endl;
  }
  
  //Make some plots for events failing HLT
  const TriggerObjectCol *objs = GetHLTObjects("myhltobjs");
  //cerr << "trigobj " << objs << endl;
  
  ObjArray<Electron> *CleanElectrons = dynamic_cast<ObjArray<Electron>* >
    (FindObjThisEvt(ModNames::gkCleanElectronsName));
  ObjArray<Muon> *CleanMuons = dynamic_cast<ObjArray<Muon>* >(FindObjThisEvt(ModNames::gkCleanMuonsName));
  //make lepton vector from muons and electrons
  vector<ChargedParticle*> leptons;
  for (UInt_t j=0; j<CleanMuons->GetEntries(); j++) {
    leptons.push_back(CleanMuons->At(j));
  }
  for (UInt_t j=0; j<CleanElectrons->GetEntries(); j++) {   
    leptons.push_back(CleanElectrons->At(j));
  }
  //We should sort the Leptons by Pt   
  for(UInt_t i=0; i<leptons.size(); i++)
    for(UInt_t j=i+1; j<leptons.size(); j++)
      if(leptons[i]->Pt() < leptons[j]->Pt()) {
	//swap i and j
	ChargedParticle* templepton = leptons[i];
	leptons[i] = leptons[j];
	leptons[j] = templepton;
      }


//find trigger object closest to leading lepton
  const TriggerObject *leadingTriggerObject;
  if (leptons.size() >= 1  && (objs)) {
    double minDR = 5000;
    for(UInt_t i=0;i<objs->GetEntries();++i) {
      const TriggerObject *to = objs->At(i);
      double tmpDR = MathUtils::DeltaR(to->Phi(),to->Eta(),leptons[0]->Phi(), leptons[0]->Eta());
        if (tmpDR < minDR) {
          minDR = tmpDR; 
          leadingTriggerObject = to;
        }          
    }  
    fHLTToRecoObjectDeltaR->Fill(minDR);
    if (leptons[0]->Mass() == 0.51099892e-3) {
      fHLTToRecoElectronDeltaR->Fill(minDR);
    } 
    if (leptons[0]->Mass() == 105.658369e-3) {
      fHLTToRecoMuonDeltaR->Fill(minDR);
    }
  }

  double maxLeptonPt = 0;
  double maxLeptonEta = -999.0;
  string maxLeptonType = "";
  for (UInt_t i=0; i<CleanMuons->GetEntries(); ++i) {
    if (CleanMuons->At(i)->Pt() > maxLeptonPt) {
      maxLeptonPt = CleanMuons->At(i)->Pt();
      maxLeptonEta = CleanMuons->At(i)->Eta();
      maxLeptonType = "mu";
    }
  }
  for (UInt_t i=0; i<CleanElectrons->GetEntries(); ++i) {
    if (CleanElectrons->At(i)->Pt() > maxLeptonPt) {
      maxLeptonPt = CleanElectrons->At(i)->Pt();
      maxLeptonEta = CleanElectrons->At(i)->Eta();
      maxLeptonType = "e";
    }
  }
  if (maxLeptonPt > 0) {
    if (!objs ) { //no trig objects means it failed HLT
      
      fFailedPtHist->Fill(maxLeptonPt);
      fFailedEtaHist->Fill(maxLeptonEta);
      
      

      if (maxLeptonType == "e") {
        fFailedElectronPtHist->Fill(maxLeptonPt);
        fFailedElectronEtaHist->Fill(maxLeptonEta);
      } else if (maxLeptonType == "mu") {
        fFailedMuonPtHist ->Fill(maxLeptonPt);
        fFailedMuonEtaHist->Fill(maxLeptonEta);
      }    
    } else {
      fPassedPtHist->Fill(maxLeptonPt);
      fPassedEtaHist->Fill(maxLeptonEta);
      
      if (maxLeptonType == "e") {
        fPassedElectronPtHist->Fill(maxLeptonPt);
        fPassedElectronEtaHist->Fill(maxLeptonEta);
      } else if (maxLeptonType == "mu") {
        fPassedMuonPtHist ->Fill(maxLeptonPt);
        fPassedMuonEtaHist->Fill(maxLeptonEta);
      }
    }
  }  
}
//--------------------------------------------------------------------------------------------------
void HwwHLTStudyMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fMuonName,                  fMuons);
  ReqBranch(fElectronName,              fElectrons);

  fFailedPtHist  = new TH1D("hFailedPtHist",";p_{t};#",250,0.,500.);
  fFailedEtaHist = new TH1D("hFailedEtaHist",";#eta;#",160,-8.,8.);
  fFailedElectronPtHist  = new TH1D("hFailedElectronPtHist",";p_{t};#",250,0.,500.);
  fFailedElectronEtaHist = new TH1D("hFailedElectronEtaHist",";#eta;#",160,-8.,8.);
  fFailedMuonPtHist  = new TH1D("hFailedMuonPtHist",";p_{t};#",250,0.,500.);
  fFailedMuonEtaHist = new TH1D("hFailedMuonEtaHist",";#eta;#",160,-8.,8.);

  fPassedPtHist  = new TH1D("hPassedPtHist",";p_{t};#",250,0.,500.);
  fPassedEtaHist = new TH1D("hPassedEtaHist",";#eta;#",160,-8.,8.);
  fPassedElectronPtHist  = new TH1D("hPassedElectronPtHist",";p_{t};#",250,0.,500.);
  fPassedElectronEtaHist = new TH1D("hPassedElectronEtaHist",";#eta;#",160,-8.,8.);
  fPassedMuonPtHist  = new TH1D("hPassedMuonPtHist",";p_{t};#",250,0.,500.);
  fPassedMuonEtaHist = new TH1D("hPassedMuonEtaHist",";#eta;#",160,-8.,8.);

  fHLTToRecoObjectDeltaR  = new TH1D("hHLTToRecoObjectDeltaR",";DeltaR;Number of Events",100,0.,0.05);
  fHLTToRecoElectronDeltaR  = new TH1D("hHLTToRecoElectronDeltaR",";DeltaR;Number of Events",100,0.,0.5);
  fHLTToRecoMuonDeltaR  = new TH1D("hHLTToRecoMuonDeltaR",";DeltaR;Number of Events",100,0.,0.5);


  AddOutput(fFailedPtHist);
  AddOutput(fFailedEtaHist);
  AddOutput(fFailedElectronPtHist);
  AddOutput(fFailedElectronEtaHist);
  AddOutput(fFailedMuonPtHist);
  AddOutput(fFailedMuonEtaHist);
  AddOutput(fPassedPtHist);
  AddOutput(fPassedEtaHist);
  AddOutput(fPassedElectronPtHist);
  AddOutput(fPassedElectronEtaHist);
  AddOutput(fPassedMuonPtHist);
  AddOutput(fPassedMuonEtaHist);
  AddOutput(fHLTToRecoObjectDeltaR);
  AddOutput(fHLTToRecoElectronDeltaR);
  AddOutput(fHLTToRecoMuonDeltaR);


}

//--------------------------------------------------------------------------------------------------
void HwwHLTStudyMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.

  cerr << "Events Processed: " << fNEventsProcessed << endl;
  cerr << "Events Passed PreSelection: " << fNEventsPassed << endl;

}

//--------------------------------------------------------------------------------------------------
void HwwHLTStudyMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
