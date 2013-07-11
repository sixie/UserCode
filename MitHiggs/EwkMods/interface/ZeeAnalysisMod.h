//--------------------------------------------------------------------------------------------------
// $Id: ZeeAnalysisMod.h,v 1.2 2008/12/04 19:19:49 sixie Exp $
//
// ZeeAnalysisMod
//
// This Module performs cleaning of objects including, removal of
// duplicate objects, applying object ID cuts, correction of MET
// The output will be collections of objects deemed as "good" and
// exported to other modules.
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef EWKMODS_ZEEANALYSISMOD_H
#define EWKMODS_ZEEANALYSISMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;

namespace mithep 
{
  class ZeeAnalysisMod : public BaseMod
  {
    public:
      ZeeAnalysisMod(const char *name="ZeeAnalysisMod", 
                     const char *title="Example analysis module with all branches");
      ~ZeeAnalysisMod() {}

    protected:
      TString                  fMCPartName;           //name of particle collection
      TString                  fTrackName;            //name of track collection
      TString                  fMuonName;             //name of muon collection
      TString                  fElectronName;         //name of electron collection
      TString                  fJetName;              //name of jet collection
      TString                  fMetName;              //name of met collection
     
      string                   fGoodElectronsName ;        
      string                   fGoodMuonsName ;        
      string                   fGoodCentralJetsName;        
      string                   fGoodForwardJetsName;        
      string                   fMuonCorrectedMetName ; 
      string                   fGenLeptonsName;  

      const MCParticleCol     *fParticles;            //!GenParticle branch
      const TrackCol	      *fTracks;	              //!Track branch
      const MuonCol	      *fMuons;                //!Muon branch
      const ElectronCol	      *fElectrons;            //!Electron branch
      const MetCol            *fMet;                  //!Missing Et
      const JetCol            *fJets;                 //!Jet branch
      
      TH1D                    *fGenZMass;
      TH1D                    *fZMass;
      TH1D                    *fZMassBB;
      TH1D                    *fZMassEB;
      TH1D                    *fZMassEE;
      TH2D                    *fZMassVsZEta;
      TH2D                    *fZMassVsLeptonEta;
      TH2D                    *fZMassVsLeptonPhi;
      TH1D                    *fDileptonDeltaEta;
      TH1D                    *fDileptonDeltaPhi;
      TH1D                    *fLepton1Pt;
      TH1D                    *fLepton2Pt;
      TH1D                    *fLeptonEta;
      TH1D                    *fLeptonPhi;
      TH1D                    *fMetHist;
      TH1D                    *fNJets;
      TH1D                    *fJetPt;
      TH1D                    *fJetEta;
      TH1D                    *fJetPhi;
      TH1D                    *fMinDeltaRLeptonJet;


      int                      fNEventsProcessed;
      int                      fNEventsPassedSkim;
      int                      fNEventsPassedLeptonSelection;
      int                      fNEventsPassedKinematicPreselection;

      void                     Begin();
      void                     Process();
      void                     SlaveBegin();
      void                     SlaveTerminate();
      void                     Terminate();      

      ClassDef(ZeeAnalysisMod,1) // TAM example analysis module
  };
}
#endif
