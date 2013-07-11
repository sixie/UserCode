 // $Id: HwwEvtPreSelMod.cc,v 1.1 2008/11/12 15:04:49 sixie Exp $

#include "MitPhysics/SelMods/interface/HwwEvtPreSelMod.h"
#include <TH1D.h>
#include <TH2D.h>
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitCommon/MathTools/interface/MathUtils.h"

using namespace mithep;
ClassImp(mithep::HwwEvtPreSelMod)

//--------------------------------------------------------------------------------------------------
HwwEvtPreSelMod::HwwEvtPreSelMod(const char *name, const char *title) : 
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
void HwwEvtPreSelMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.

}

//--------------------------------------------------------------------------------------------------
void HwwEvtPreSelMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches an
  fNEventsProcessed++;

  if (fNEventsProcessed % 1000 == 0 || fPrintDebug) {
    time_t systime;
    systime = time(NULL);
    cerr << endl << "HwwEvtPreSelMod : Process Event " << fNEventsProcessed << "  Time: " << ctime(&systime) << endl;
  }



  if (fDoHLTStudy) {
    //Make some plots for events failing HLT
    const TriggerObjectCol *objs = GetHLTObjects("myhltobjs");
    //cerr << "trigobj " << objs << endl;
    
    if (!objs ) { //no trig objects means it failed HLT
      LoadBranch(fMuonName);
      LoadBranch(fElectronName);
      double maxLeptonPt = 0;
      double maxLeptonEta = -999.0;
      string maxLeptonType = "";
      for (UInt_t i=0; i<fMuons->GetEntries(); ++i) {
        if (fMuons->At(i)->Pt() > maxLeptonPt) {
          maxLeptonPt = fMuons->At(i)->Pt();
          maxLeptonEta = fMuons->At(i)->Eta();
          maxLeptonType = "mu";
        }
      }
      for (UInt_t i=0; i<fElectrons->GetEntries(); ++i) {
        if (fElectrons->At(i)->Pt() > maxLeptonPt) {
          maxLeptonPt = fElectrons->At(i)->Pt();
          maxLeptonEta = fElectrons->At(i)->Eta();
          maxLeptonType = "e";
        }
      }
      if (maxLeptonPt > 20.0) {
        fFailedPtHist->Fill(maxLeptonPt);
        fFailedEtaHist->Fill(maxLeptonEta);
        
        if (maxLeptonType == "e") {
          fFailedElectronPtHist->Fill(maxLeptonPt);
          fFailedElectronEtaHist->Fill(maxLeptonEta);
        } else if (maxLeptonType == "mu") {
          fFailedMuonPtHist ->Fill(maxLeptonPt);
          fFailedMuonEtaHist->Fill(maxLeptonEta);
        }
      }
    }
  }

  if(fPreSelectionType.CompareTo("Loose") == 0) {
    int nLeptons = 0;
    LoadBranch(fMuonName);
    double maxLeptonPt = 0;
    for (UInt_t i=0; i<fMuons->GetEntries(); ++i) {
      if (fPrintDebug) cerr << "mu " << i << " " << fMuons->At(i)->Pt() << endl;
      
      if (fMuons->At(i)->Pt() > 5) 
        nLeptons++;
      if (fMuons->At(i)->Pt() > maxLeptonPt)       
        maxLeptonPt = fMuons->At(i)->Pt();    
    }
    
    if (maxLeptonPt > 20 &&  nLeptons >= 2) {
      fNEventsPassed++;
      return;
    }
    
    LoadBranch(fElectronName);
    for (UInt_t i=0; i<fElectrons->GetEntries(); ++i) {
      if (fPrintDebug) cerr << "ele " << i << " " << fElectrons->At(i)->Pt() << endl;
      if (fElectrons->At(i)->Pt() > 5)
        nLeptons++;
      if (fElectrons->At(i)->Pt() > maxLeptonPt)
        maxLeptonPt = fElectrons->At(i)->Pt();
    }
    
    if (maxLeptonPt > 20 &&  nLeptons >= 2) {
      fNEventsPassed++;
      return;
    }
    
    //By now the event fails preselection. If nLeptons < 2 or if maxLeptonPt
    //is less than 20
    if (fPrintDebug) cerr << nLeptons << " " << maxLeptonPt << endl;  
    if (fPrintDebug) cerr << "fail preselection - skip event" << endl;
    SkipEvent();
    return;
  } else if (fPreSelectionType.CompareTo("Tight") == 0) {
    ObjArray<Electron> *CleanElectrons = dynamic_cast<ObjArray<Electron>* >
      (FindObjThisEvt(Names::gkCleanElectronsName));
    ObjArray<Muon> *CleanMuons = dynamic_cast<ObjArray<Muon>* >(FindObjThisEvt(Names::gkCleanMuonsName));
    
    double maxLeptonPt = 0;
    double secondLeptonPt = 0;
    int nLeptons = 0;
    if (CleanMuons) {
      for (UInt_t i=0; i<CleanMuons->GetEntries(); ++i) {
        if (CleanMuons->At(i)->Pt() > 5) 
          nLeptons++;
        if (CleanMuons->At(i)->Pt() > maxLeptonPt) {
          secondLeptonPt = maxLeptonPt;
          maxLeptonPt = CleanMuons->At(i)->Pt();   
        } else if (CleanMuons->At(i)->Pt() >  secondLeptonPt)
          secondLeptonPt = CleanMuons->At(i)->Pt();         
      }
    }
    if (CleanElectrons) {
      for (UInt_t i=0; i<fElectrons->GetEntries(); ++i) {
        if (fElectrons->At(i)->Pt() > 5)
          nLeptons++;
        if (fElectrons->At(i)->Pt() > maxLeptonPt) {
          secondLeptonPt = maxLeptonPt;
          maxLeptonPt = fElectrons->At(i)->Pt();
        } else if (fElectrons->At(i)->Pt() > secondLeptonPt)
          secondLeptonPt = fElectrons->At(i)->Pt();
      }
    }    
    if (!(nLeptons >= 2 && maxLeptonPt > 20 && secondLeptonPt > 15)) {
      SkipEvent();
      return;  
    }
  }
}
//--------------------------------------------------------------------------------------------------
void HwwEvtPreSelMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fMuonName,                  fMuons);
  ReqBranch(fElectronName,              fElectrons);

//   fFailedPtHist  = new TH1D("hFailedPtHist",";p_{t};#",250,0.,500.);
//   fFailedEtaHist = new TH1D("hFailedEtaHist",";#eta;#",160,-8.,8.);
//   fFailedElectronPtHist  = new TH1D("hFailedElectronPtHist",";p_{t};#",250,0.,500.);
//   fFailedElectronEtaHist = new TH1D("hFailedElectronEtaHist",";#eta;#",160,-8.,8.);
//   fFailedMuonPtHist  = new TH1D("hFailedMuonPtHist",";p_{t};#",250,0.,500.);
//   fFailedMuonEtaHist = new TH1D("hFailedMuonEtaHist",";#eta;#",160,-8.,8.);

//   AddOutput(fFailedPtHist);
//   AddOutput(fFailedEtaHist);
//   AddOutput(fFailedElectronPtHist);
//   AddOutput(fFailedElectronEtaHist);
//   AddOutput(fFailedMuonPtHist);
//   AddOutput(fFailedMuonEtaHist);

}

//--------------------------------------------------------------------------------------------------
void HwwEvtPreSelMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.

  cerr << "Events Processed: " << fNEventsProcessed << endl;
  cerr << "Events Passed PreSelection: " << fNEventsPassed << endl;

}

//--------------------------------------------------------------------------------------------------
void HwwEvtPreSelMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
