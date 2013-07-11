// $Id: MuonFakeRateAnalysisMod.cc,v 1.3 2008/12/04 19:19:49 sixie Exp $

#include "MitHiggs/FakeRateMods/interface/MuonFakeRateAnalysisMod.h"
#include <TH1D.h>
#include <TH2D.h>
#include "MitAna/DataTree/interface/Names.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include <map>

using namespace mithep;

ClassImp(mithep::MuonFakeRateAnalysisMod)

//--------------------------------------------------------------------------------------------------
  MuonFakeRateAnalysisMod::MuonFakeRateAnalysisMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fMCPartName(Names::gkMCPartBrn),
  fTrackName(Names::gkTrackBrn),
  fMuonName(Names::gkMuonBrn),
  fElectronName(Names::gkElectronBrn),
  fJetName(Names::gkCaloJetBrn),
  fMetName(Names::gkCaloMetBrn),
  fGoodElectronsName("GoodElectrons"),        
  fGoodMuonsName("GoodMuons"),        
  fGoodCentralJetsName("GoodCentralJets" ),        
  fGoodForwardJetsName("GoodForwardJets" ),        
  fMuonCorrectedMetName("muonCorrectedMet" ),
  fGenLeptonsName("GenLeptons"),
  fParticles(0),
  fTracks(0),
  fMuons(0),
  fElectrons(0),
  fNEventsProcessed(0)
{

  cerr << "Constructor\n";
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void MuonFakeRateAnalysisMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void MuonFakeRateAnalysisMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and
  //output debug info or not   
  bool printDebug = false; 

  LoadBranch(fMCPartName);

 //Obtain all the good objects from the event cleaning module
  // ObjArray<Electron> *GoodElectrons = dynamic_cast<ObjArray<Electron>* >(FindObjThisEvt(fGoodElectronsName.Data()));
  ObjArray<Muon> *GoodMuons = dynamic_cast<ObjArray<Muon>* >(FindObjThisEvt(fGoodMuonsName.Data()));
  // ObjArray<Jet> *GoodCentralJets = dynamic_cast<ObjArray<Jet>* >(FindObjThisEvt(fGoodCentralJetsName.Data()));
  // ObjArray<Jet> *GoodForwardJets = dynamic_cast<ObjArray<Jet>* >(FindObjThisEvt(fGoodForwardJetsName.Data()));
  // Met *muonCorrectedMet = dynamic_cast<Met*> (FindObjThisEvt(fMuonCorrectedMetName.Data()));

  fNEventsProcessed++;
 
  if (fNEventsProcessed % 1000 == 0 || printDebug)
    cerr << endl << "Process Event " << fNEventsProcessed << endl;
  
  //Try to find the quarks from the hard interactions.
  
  ObjArray<Jet> *GoodJets = new ObjArray<Jet>;
  double LeadingJetPt = 0;
  LoadBranch(fJetName);
  for (UInt_t i=0; i<fJets->GetEntries(); ++i) {
    if(fabs(fJets->At(i)->Eta()) >= 2.5) continue; // minimum eta requirement
    if(fJets->At(i)->Pt() <= 10) continue; // minimum Pt requirement
    GoodJets->Add(fJets->At(i));        
    if (fJets->At(i)->Pt() > LeadingJetPt)
      LeadingJetPt = fJets->At(i)->Pt();
  }

  ObjArray<Track> *IsolatedTrackDenominatorObject = new ObjArray<Track>;

  map<const Track*, Jet*> IsolatedTrackToJetMap;
  //vector<int> IsolatedTrackToJetDenominatorObjectMatch;

  double LeadingIsolatedTrackPt = 0;
  LoadBranch(fTrackName);

  for (UInt_t i=0; i<fTracks->GetEntries(); ++i) {
    const Track* track = fTracks->At(i);
    fTrackPtHist->Fill(track->Pt());
    fTrackThetaHist->Fill(track->Theta());
    fTrackPhiHist->Fill(track->Phi());    

    if (track->Pt() <= 10.0) continue;
    if (fabs(track->Mom().Eta()) >= 2.5) continue;
//    if (fabs(track->NHits()) < 8) continue; 
    if (fabs(track->Chi2()/track->Ndof()) >= 40) continue;    

    //apply isolation
    double trackIsoSumPt = 0.0;    
    for (UInt_t j=0; j<fTracks->GetEntries(); ++j) {
      double deltaR2=MathUtils::DeltaR(track->Mom().Phi(), track->Mom().Eta(),
                                           fTracks->At(j)->Mom().Phi(), 
                                           fTracks->At(j)->Mom().Eta());
      if(fTracks->At(j)->Pt() > 1.0 && 
         fabs(track->Chi2()/track->Ndof()) < 10 &&
         //fTracks->At(j)->hits() >= 5 
         fabs(fTracks->At(j)->Mom().Eta()) < 2.5 &&
         deltaR2 > 0.01 && deltaR2 < 0.3 &&
         fabs(fTracks->At(j)->Z0() - track->Z0()) < 0.2
        ) {
        trackIsoSumPt += fTracks->At(j)->Pt();
      }
    }
    

    if (trackIsoSumPt > 10.0) continue;

    if (track->Pt() > LeadingIsolatedTrackPt)
      LeadingIsolatedTrackPt = track->Pt();
    IsolatedTrackDenominatorObject->Add(track);

    //match isolated tracks to jets
    double minDR = 5000;
    int matchindex = -1;
    for(UInt_t j=0;j<GoodJets->GetEntries();j++) {
      if (MathUtils::DeltaR(track->Mom().Phi(), track->Mom().Eta(),
                                GoodJets->At(j)->Phi(),
                                GoodJets->At(j)->Eta()) < minDR) {
        minDR = MathUtils::DeltaR(track->Mom().Phi(), track->Mom().Eta(),
                                      GoodJets->At(j)->Phi(),
                                      GoodJets->At(j)->Eta());
        matchindex = j;
      }
    }
    if (minDR < 0.1)
      IsolatedTrackToJetMap[track] = GoodJets->At(matchindex);
  }

 
  for(UInt_t i=0; i < IsolatedTrackDenominatorObject->GetEntries();i++) {
    if (IsolatedTrackDenominatorObject->At(i)->Pt() < LeadingIsolatedTrackPt) {
      fIsolatedTrackPtHist->Fill(IsolatedTrackDenominatorObject->At(i)->Pt());
      fIsolatedTrackEtaHist->Fill(IsolatedTrackDenominatorObject->At(i)->Mom().Eta());
      fIsolatedTrackPhiHist->Fill(IsolatedTrackDenominatorObject->At(i)->Phi()); 

      map<const Track*, Jet*>::iterator iter = 
        IsolatedTrackToJetMap.find(IsolatedTrackDenominatorObject->At(i));      
      double flavor = 0;
      if (iter != IsolatedTrackToJetMap.end()) {    
        flavor = abs(iter->second->MatchedMCFlavor());
      } else {
        
        double minDR = 5000;
        int ClosestPartonFlavor = 0;
        for (UInt_t p=0; p<fParticles->GetEntries(); ++p) {
          if (fParticles->At(p)->Status() == 3 && 
              (fParticles->At(p)->AbsPdgId() >= 1 && fParticles->At(p)->AbsPdgId() <= 5 ||
               fParticles->At(p)->AbsPdgId() == 21)
            ) {
            if (MathUtils::DeltaR(fParticles->At(p)->Phi(),fParticles->At(p)->Eta(),
                                  IsolatedTrackDenominatorObject->At(i)->Mom().Phi(),
                                  IsolatedTrackDenominatorObject->At(i)->Mom().Eta()) < minDR) {
              minDR = MathUtils::DeltaR(fParticles->At(p)->Phi(),fParticles->At(p)->Eta(),
                                        IsolatedTrackDenominatorObject->At(i)->Mom().Phi(),
                                        IsolatedTrackDenominatorObject->At(i)->Mom().Eta());
              ClosestPartonFlavor = fParticles->At(p)->AbsPdgId();
            }
          }
        }
          
        if (minDR < 1.0)
          flavor = ClosestPartonFlavor;
      }
         
      if (flavor >= 1 && flavor <= 3 || flavor == 21 ) {
        fLightFlavorIsolatedTrackPtHist->Fill(IsolatedTrackDenominatorObject->At(i)->Pt());
        fLightFlavorIsolatedTrackEtaHist->Fill(
          IsolatedTrackDenominatorObject->At(i)->Mom().Eta());
        fLightFlavorIsolatedTrackPhiHist->Fill(IsolatedTrackDenominatorObject->At(i)->Phi()); 
      } else if (flavor == 4) {
        fCharmFlavorIsolatedTrackPtHist->Fill(IsolatedTrackDenominatorObject->At(i)->Pt());
        fCharmFlavorIsolatedTrackEtaHist->Fill(
          IsolatedTrackDenominatorObject->At(i)->Mom().Eta());
        fCharmFlavorIsolatedTrackPhiHist->Fill(IsolatedTrackDenominatorObject->At(i)->Phi()); 
      } else if (flavor == 5) {
        fBottomFlavorIsolatedTrackPtHist->Fill(IsolatedTrackDenominatorObject->At(i)->Pt());
        fBottomFlavorIsolatedTrackEtaHist->Fill(
          IsolatedTrackDenominatorObject->At(i)->Mom().Eta());
        fBottomFlavorIsolatedTrackPhiHist->Fill(IsolatedTrackDenominatorObject->At(i)->Phi());
      } else {
        fUnmatchedFlavorIsolatedTrackPtHist->Fill(IsolatedTrackDenominatorObject->At(i)->Pt());
        fUnmatchedFlavorIsolatedTrackEtaHist->Fill(
          IsolatedTrackDenominatorObject->At(i)->Mom().Eta());
        fUnmatchedFlavorIsolatedTrackPhiHist->Fill(IsolatedTrackDenominatorObject->At(i)->Phi());  
      }
        
      
      
    }
  }


  map<Muon*,Jet*> GoodMuonToJetMap;
  map<Muon*,Track*> GoodMuonToIsolatedTrackMap;
  for (UInt_t i=0; i<GoodMuons->GetEntries(); i++) {        
    
    if (fabs(GoodMuons->At(i)->Eta()) > 2.5) 
      continue;
    if (GoodMuons->At(i)->Pt() < 10)
      continue;

    bool isTriggerJet = false;

    double closestDR = 5000;
    double closestFOPt = -1;
    int closestIndex = -1;
    //Checks that the muon is not near the higest pt jet
    for (UInt_t j=0; j<GoodJets->GetEntries(); ++j) {
      if (MathUtils::DeltaR(GoodJets->At(j)->Mom(), GoodMuons->At(i)->Mom()) < closestDR) {
        closestDR = MathUtils::DeltaR(GoodJets->At(j)->Mom(), GoodMuons->At(i)->Mom());
        closestFOPt = GoodJets->At(j)->Pt();
        closestIndex = j;
      }    
    }
  
    if (closestDR < 0.3 && closestFOPt == LeadingJetPt)
      isTriggerJet = true;

    //match the muon to a jet
    if (closestDR < 0.3)
      GoodMuonToJetMap[GoodMuons->At(i)] = GoodJets->At(closestIndex);
    else {
      //cout << "!!!!!!!!!   Muon Match to no jet   !!!!!!\n";
    }
      
    //Checks that the muon is not from the highest pt isolated track
    closestDR = 5000;
    closestFOPt = -1;
    closestIndex = -1;
    for (UInt_t j=0; j<IsolatedTrackDenominatorObject->GetEntries(); ++j) {
      if (MathUtils::DeltaR(GoodMuons->At(i)->Mom().Phi(), GoodMuons->At(i)->Mom().Eta(),
                            IsolatedTrackDenominatorObject->At(j)->Mom().Phi(),
                            IsolatedTrackDenominatorObject->At(j)->Mom().Eta()) < closestDR) {
        closestDR = MathUtils::DeltaR(GoodMuons->At(i)->Mom().Phi(), GoodMuons->At(i)->Mom().Eta(),
                                      IsolatedTrackDenominatorObject->At(j)->Mom().Phi(),
                                      IsolatedTrackDenominatorObject->At(j)->Mom().Eta());
        closestFOPt = IsolatedTrackDenominatorObject->At(j)->Pt();
        closestIndex = j;
      }
    }

    // if (closestDR < 0.3 && closestFOPt == LeadingIsolatedTrackPt)
//       isTriggerJet = true;
    
    if (closestDR < 0.1)
      GoodMuonToIsolatedTrackMap[GoodMuons->At(i)] = 
        IsolatedTrackDenominatorObject->At(closestIndex);
    else {    
      cout << "!!!!!!!!!   Muon Match to no isolated track   !!!!!!\n";
    }                  
    
    if (!isTriggerJet) {
      fMuonPtHist->Fill(GoodMuons->At(i)->Pt());
      fMuonEtaHist->Fill(GoodMuons->At(i)->Eta());
      fMuonPhiHist->Fill(GoodMuons->At(i)->Phi());


      map<Muon*, Jet*>::iterator iter = 
        GoodMuonToJetMap.find(GoodMuons->At(i));      
      double flavor = 0;
      if (iter != GoodMuonToJetMap.end()) {    
        flavor = abs(iter->second->MatchedMCFlavor());
      } else {        
        double minDR = 5000;
        int ClosestPartonFlavor = 0;
        for (UInt_t p=0; p<fParticles->GetEntries(); ++p) {
          if (fParticles->At(p)->Status() == 3 && 
              (fParticles->At(p)->AbsPdgId() >= 1 && fParticles->At(p)->AbsPdgId() <= 5 ||
               fParticles->At(p)->AbsPdgId() == 21)
            ) {
            if (MathUtils::DeltaR(fParticles->At(p)->Phi(),fParticles->At(p)->Eta(),
                                  GoodMuons->At(i)->Phi(),
                                  GoodMuons->At(i)->Eta()) < minDR) {
              minDR = MathUtils::DeltaR(fParticles->At(p)->Phi(),fParticles->At(p)->Eta(),
                                        GoodMuons->At(i)->Phi(),
                                        GoodMuons->At(i)->Eta());
              ClosestPartonFlavor = fParticles->At(p)->AbsPdgId();
            }
          }
        }
          
        if (minDR < 1.0)
          flavor = ClosestPartonFlavor;
      }
         
      if (flavor >= 1 && flavor <= 3 || flavor == 21 ) {
        fLightFlavorMuonPtHist->Fill(GoodMuons->At(i)->Pt());
        fLightFlavorMuonEtaHist->Fill(GoodMuons->At(i)->Eta());
        fLightFlavorMuonPhiHist->Fill(GoodMuons->At(i)->Phi());
      } else if (flavor == 4) {
        fCharmFlavorMuonPtHist->Fill(GoodMuons->At(i)->Pt());
        fCharmFlavorMuonEtaHist->Fill(GoodMuons->At(i)->Eta());
        fCharmFlavorMuonPhiHist->Fill(GoodMuons->At(i)->Phi());
       } else if (flavor == 5) {
         fBottomFlavorMuonPtHist->Fill(GoodMuons->At(i)->Pt());
        fBottomFlavorMuonEtaHist->Fill(GoodMuons->At(i)->Eta());
        fBottomFlavorMuonPhiHist->Fill(GoodMuons->At(i)->Phi());
      } else {
        fUnmatchedFlavorMuonPtHist->Fill(GoodMuons->At(i)->Pt());
        fUnmatchedFlavorMuonEtaHist->Fill(GoodMuons->At(i)->Eta());
        fUnmatchedFlavorMuonPhiHist->Fill(GoodMuons->At(i)->Phi());
      }        
    }    
  }  
}


//--------------------------------------------------------------------------------------------------
void MuonFakeRateAnalysisMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fMCPartName,                fParticles);
  ReqBranch(fTrackName,                 fTracks);
  ReqBranch(fMuonName,                  fMuons);
  ReqBranch(fElectronName,              fElectrons);
  ReqBranch(fJetName,                   fJets);
  ReqBranch(fMetName,                   fMet);
 
  fTrackPtHist                     = new TH1D("hTrackPtHist",";p_{t};#",100,0.,100);
  fTrackThetaHist                  = new TH1D("hTrackThetaHist",";#theta;#",100,0.0,3.2);
  fTrackPhiHist                    = new TH1D("hTrackPhiHist",";p_{t};#",100,-3.5,3.5);
  fIsolatedTrackPtHist             = new TH1D("hIsolatedTrackPtHist",";p_{t};#",100,0.,100.);
  fIsolatedTrackEtaHist            = new TH1D("hIsolatedTrackEtaHist",";#theta;#",100,-3.5,3.5);
  fIsolatedTrackPhiHist            = new TH1D("hIsolatedTrackPhiHist",";p_{t};#",100,-3.5,3.5);
  fMuonPtHist                      = new TH1D("hMuonPtHist",";p_{t};#",100,0.,100.);
  fMuonEtaHist                     = new TH1D("hMuonEtaHist",";#eta;#",100,-3.5,3.5);
  fMuonPhiHist                     = new TH1D("hMuonPhiHist",";p_{t};#",100,-3.5,3.5);
  fLightFlavorIsolatedTrackPtHist  = new TH1D("hLightFlavorIsolatedTrackPtHist",
                                              ";p_{t};#",100,0.,100.);
  fLightFlavorIsolatedTrackEtaHist = new TH1D("hLightFlavorIsolatedTrackEtaHist",
                                              ";#eta;#",100,-3.5,3.5);
  fLightFlavorIsolatedTrackPhiHist = new TH1D("hLightFlavorIsolatedTrackPhiHist",
                                              ";p_{t};#",100,-3.5,3.5);
  fLightFlavorMuonPtHist           = new TH1D("hLightFlavorMuonPtHist",";p_{t};#",100,0.,100.);
  fLightFlavorMuonEtaHist          = new TH1D("hLightFlavorMuonEtaHist",";#theta;#",100,-3.5,3.5);
  fLightFlavorMuonPhiHist          = new TH1D("hLightFlavorMuonPhiHist",";p_{t};#",100,-3.5,3.5);
  fCharmFlavorIsolatedTrackPtHist  = new TH1D("hCharmFlavorIsolatedTrackPtHist",
                                              ";p_{t};#",100,0.,100.);
  fCharmFlavorIsolatedTrackEtaHist = new TH1D("hCharmFlavorIsolatedTrackEtaHist",
                                              ";#eta;#",100,-3.5,3.5);
  fCharmFlavorIsolatedTrackPhiHist = new TH1D("hCharmFlavorIsolatedTrackPhiHist",
                                              ";p_{t};#",100,-3.5,3.5);
  fCharmFlavorMuonPtHist           = new TH1D("hCharmFlavorMuonPtHist",";p_{t};#",100,0.,100.);
  fCharmFlavorMuonEtaHist          = new TH1D("hCharmFlavorMuonEtaHist",";#theta;#",100,-3.5,3.5);
  fCharmFlavorMuonPhiHist          = new TH1D("hCharmFlavorMuonPhiHist",";p_{t};#",100,-3.5,3.5);
  fBottomFlavorIsolatedTrackPtHist  = new TH1D("hBottomFlavorIsolatedTrackPtHist",
                                               ";p_{t};#",100,0.,100.);
  fBottomFlavorIsolatedTrackEtaHist = new TH1D("hBottomFlavorIsolatedTrackEtaHist",
                                               ";#eta;#",100,-3.5,3.5);
  fBottomFlavorIsolatedTrackPhiHist = new TH1D("hBottomFlavorIsolatedTrackPhiHist",
                                               ";p_{t};#",100,-3.5,3.5);
  fBottomFlavorMuonPtHist           = new TH1D("hBottomFlavorMuonPtHist",";p_{t};#",100,0.,100.);
  fBottomFlavorMuonEtaHist          = new TH1D("hBottomFlavorMuonEtaHist",";#theta;#",100,-3.5,3.5);
  fBottomFlavorMuonPhiHist          = new TH1D("hBottomFlavorMuonPhiHist",";p_{t};#",100,-3.5,3.5); 
  fUnmatchedFlavorIsolatedTrackPtHist  = new TH1D("hUnmatchedFlavorIsolatedTrackPtHist",
                                                  ";p_{t};#",100,0.,100.);
  fUnmatchedFlavorIsolatedTrackEtaHist = new TH1D("hUnmatchedFlavorIsolatedTrackEtaHist",
                                                  ";#eta;#",100,-3.5,3.5);
  fUnmatchedFlavorIsolatedTrackPhiHist = new TH1D("hUnmatchedFlavorIsolatedTrackPhiHist",
                                                  ";p_{t};#",100,-3.5,3.5);
  fUnmatchedFlavorMuonPtHist           = new TH1D("hUnmatchedFlavorMuonPtHist",
                                                  ";p_{t};#",100,0.,100.);
  fUnmatchedFlavorMuonEtaHist          = new TH1D("hUnmatchedFlavorMuonEtaHist",
                                                  ";#theta;#",100,-3.5,3.5);
  fUnmatchedFlavorMuonPhiHist          = new TH1D("hUnmatchedFlavorMuonPhiHist",
                                                  ";p_{t};#",100,-3.5,3.5); 
  
  AddOutput(fTrackPhiHist);
  AddOutput(fTrackPtHist);  
  AddOutput(fTrackThetaHist);
  AddOutput(fIsolatedTrackPhiHist);
  AddOutput(fIsolatedTrackPtHist);  
  AddOutput(fIsolatedTrackEtaHist);
  AddOutput(fMuonPhiHist);
  AddOutput(fMuonPtHist);  
  AddOutput(fMuonEtaHist);
  AddOutput(fLightFlavorIsolatedTrackPhiHist);
  AddOutput(fLightFlavorIsolatedTrackPtHist);  
  AddOutput(fLightFlavorIsolatedTrackEtaHist);
  AddOutput(fLightFlavorMuonPhiHist);
  AddOutput(fLightFlavorMuonPtHist);  
  AddOutput(fLightFlavorMuonEtaHist);
  AddOutput(fCharmFlavorIsolatedTrackPhiHist);
  AddOutput(fCharmFlavorIsolatedTrackPtHist);  
  AddOutput(fCharmFlavorIsolatedTrackEtaHist);
  AddOutput(fCharmFlavorMuonPhiHist);
  AddOutput(fCharmFlavorMuonPtHist);  
  AddOutput(fCharmFlavorMuonEtaHist);
  AddOutput(fBottomFlavorIsolatedTrackPhiHist);
  AddOutput(fBottomFlavorIsolatedTrackPtHist);  
  AddOutput(fBottomFlavorIsolatedTrackEtaHist);
  AddOutput(fBottomFlavorMuonPhiHist);
  AddOutput(fBottomFlavorMuonPtHist);  
  AddOutput(fBottomFlavorMuonEtaHist);
  AddOutput(fUnmatchedFlavorIsolatedTrackPhiHist);
  AddOutput(fUnmatchedFlavorIsolatedTrackPtHist);  
  AddOutput(fUnmatchedFlavorIsolatedTrackEtaHist);
  AddOutput(fUnmatchedFlavorMuonPhiHist);
  AddOutput(fUnmatchedFlavorMuonPtHist);  
  AddOutput(fUnmatchedFlavorMuonEtaHist);

}

//--------------------------------------------------------------------------------------------------
void MuonFakeRateAnalysisMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.
  cout << "Events Processed: " << fNEventsProcessed << endl;

}

//--------------------------------------------------------------------------------------------------
void MuonFakeRateAnalysisMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
