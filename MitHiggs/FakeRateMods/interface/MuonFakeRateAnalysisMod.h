//--------------------------------------------------------------------------------------------------
// $Id: MuonFakeRateAnalysisMod.h,v 1.2 2008/12/04 19:19:49 sixie Exp $
//
// MuonFakeRateAnalysisMod
//
// This Module performs cleaning of objects including, removal of
// duplicate objects, applying object ID cuts, correction of MET
// The output will be collections of objects deemed as "good" and
// exported to other modules.
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef EWKMODS_MUONFAKERATEANALYSISMOD_H
#define EWKMODS_MUONFAKERATEANALYSISMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;

namespace mithep 
{
  class MuonFakeRateAnalysisMod : public BaseMod
  {
    public:
      MuonFakeRateAnalysisMod(const char *name="MuonFakeRateAnalysisMod", 
                     const char *title="Example analysis module with all branches");
      ~MuonFakeRateAnalysisMod() {}

    protected:
      TString                  fMCPartName;           //name of particle collection
      TString                  fTrackName;            //name of track collection
      TString                  fMuonName;             //name of muon collection
      TString                  fElectronName;         //name of electron collection
      TString                  fJetName;              //name of jet collection
      TString                  fMetName;              //name of met collection
     
      TString                  fGoodElectronsName ;        
      TString                  fGoodMuonsName ;        
      TString                  fGoodCentralJetsName;        
      TString                  fGoodForwardJetsName;        
      TString                  fMuonCorrectedMetName ; 
      TString                  fGenLeptonsName;  

      const MCParticleCol     *fParticles;            //!GenParticle branch
      const TrackCol	      *fTracks;	              //!Track branch
      const MuonCol	      *fMuons;                //!Muon branch
      const ElectronCol	      *fElectrons;            //!Electron branch
      const MetCol            *fMet;                  //!Missing Et
      const JetCol            *fJets;                 //!Jet branch
      
      TH1D                    *fTrackPtHist;              //!Track pt histogram
      TH1D                    *fTrackThetaHist;           //!Track theta histogram
      TH1D                    *fTrackPhiHist;             //!Track eta histogram
      TH1D                    *fIsolatedTrackPtHist;      
      TH1D                    *fIsolatedTrackEtaHist;     
      TH1D                    *fIsolatedTrackPhiHist;    
      TH1D                    *fMuonPtHist;          
      TH1D                    *fMuonEtaHist;         
      TH1D                    *fMuonPhiHist;         
      TH1D                    *fLightFlavorIsolatedTrackPtHist;      
      TH1D                    *fLightFlavorIsolatedTrackEtaHist;     
      TH1D                    *fLightFlavorIsolatedTrackPhiHist;    
      TH1D                    *fLightFlavorMuonPtHist;          
      TH1D                    *fLightFlavorMuonEtaHist;         
      TH1D                    *fLightFlavorMuonPhiHist;         
      TH1D                    *fCharmFlavorIsolatedTrackPtHist;      
      TH1D                    *fCharmFlavorIsolatedTrackEtaHist;     
      TH1D                    *fCharmFlavorIsolatedTrackPhiHist;    
      TH1D                    *fCharmFlavorMuonPtHist;          
      TH1D                    *fCharmFlavorMuonEtaHist;         
      TH1D                    *fCharmFlavorMuonPhiHist;         
      TH1D                    *fBottomFlavorIsolatedTrackPtHist;      
      TH1D                    *fBottomFlavorIsolatedTrackEtaHist;     
      TH1D                    *fBottomFlavorIsolatedTrackPhiHist;    
      TH1D                    *fBottomFlavorMuonPtHist;          
      TH1D                    *fBottomFlavorMuonEtaHist;         
      TH1D                    *fBottomFlavorMuonPhiHist;         
      TH1D                    *fUnmatchedFlavorIsolatedTrackPtHist;      
      TH1D                    *fUnmatchedFlavorIsolatedTrackEtaHist;     
      TH1D                    *fUnmatchedFlavorIsolatedTrackPhiHist;    
      TH1D                    *fUnmatchedFlavorMuonPtHist;          
      TH1D                    *fUnmatchedFlavorMuonEtaHist;         
      TH1D                    *fUnmatchedFlavorMuonPhiHist;         

      int                      fNEventsProcessed;
 
      void                     Begin();
      void                     Process();
      void                     SlaveBegin();
      void                     SlaveTerminate();
      void                     Terminate();      

      ClassDef(MuonFakeRateAnalysisMod,1) // TAM example analysis module
  };
}
#endif
