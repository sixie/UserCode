//--------------------------------------------------------------------------------------------------
// $Id: WWEvtSelMod.h,v 1.1 2008/11/12 15:04:49 sixie Exp $
//
// WWEvtSelMod
//
// A Module for Selecting H->WW events
// and produces some distributions.
//
//
// Authors: Si Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_PHYSICSMOD_WWEVTSELMOD_H
#define MITANA_PHYSICSMOD_WWEVTSELMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;

namespace mithep 
{
  class WWEvtSelMod : public BaseMod
  {
    public:
    WWEvtSelMod(const char *name="WWEvtSelMod", 
		 const char *title="Example analysis module with all branches");
      ~WWEvtSelMod() {}
      void         SetLoadGenParticles(bool b)           { fLoadGenParticles = b;         }        
      void         SetPrintDebug(bool b)                 { fPrintDebug = b;               }   
      void         SetPlotType(TString type)             { fPlotType = type;              }   
      void         SetCleanJetsName (TString s)          { fCleanJetsName = s;            }   
      void         SetJetName (TString s)                { fJetName = s;                  }   

    protected:
      bool                     fPrintDebug;
      TString                  fPlotType;             //Type of histograms to make
      TString                  fMCPartName;           //name of particle collection
      TString                  fMetName;              //name of met collection
      TString                  fTrackName;            //name of track collection
      TString                  fJetName;              //name of jet collection
      TString                  fCleanJetsName;        //name of clean central jets collection
      TString                  fMuonName;	      // name of muon collection
      bool                     fLoadGenParticles;     //bool to determine if we want gen info
      MCParticleCol           *fParticles;            //!GenParticle branch
      MetCol                  *fMet;                  //!Missing Et
      TrackCol		      *fTracks;               //!Track branch      
      JetCol                  *fJets;                 //!Jet branch
      MuonCol	              *fMuons;	              //!Muon branch

      TH1D                    *fWWSelection;
      TH1D                    *fWWToEESelection;
      TH1D                    *fWWToMuMuSelection;
      TH1D                    *fWWToEMuSelection;

      TH1D                    *fLeptonEta;   
      TH1D                    *fLeptonPtMax;          
      TH1D                    *fLeptonPtMin;          
      TH1D                    *fMetPtHist;	      
      TH1D                    *fMetPhiHist;        
      TH1D                    *fMETdeltaPhilEtHist;     
      TH1D                    *fNCentralJets;        
      TH1D                    *fNDirtyMuonsHist;
      TH1D                    *fNCleanExtraTracksHist;
      TH1D                    *fDeltaPhiLeptons;    
      TH1D                    *fDeltaEtaLeptons;    
      TH1D                    *fDileptonMass;        
      TH1D                    *fMinDeltaPhiLeptonMet;
      TH1D                    *fLeptonEta_ee;   
      TH1D                    *fLeptonPtMax_ee;     
      TH1D                    *fLeptonPtMin_ee; 
      TH1D                    *fMetPtHist_ee;	     
      TH1D                    *fMetPhiHist_ee;         
      TH1D                    *fMETdeltaPhilEtHist_ee;     
      TH1D                    *fNCentralJets_ee;       
      TH1D                    *fNDirtyMuonsHist_ee;
      TH1D                    *fNCleanExtraTracksHist_ee;
      TH1D                    *fDeltaPhiLeptons_ee;    
      TH1D                    *fDeltaEtaLeptons_ee;   
      TH1D                    *fDileptonMass_ee;       
      TH1D                    *fMinDeltaPhiLeptonMet_ee;
      TH1D                    *fLeptonEta_mumu;   
      TH1D                    *fLeptonPtMax_mumu;     
      TH1D                    *fLeptonPtMin_mumu;
      TH1D                    *fMetPtHist_mumu;	
      TH1D                    *fMetPhiHist_mumu;  
      TH1D                    *fMETdeltaPhilEtHist_mumu;     
      TH1D                    *fNCentralJets_mumu;  
      TH1D                    *fNDirtyMuonsHist_mumu;
      TH1D                    *fNCleanExtraTracksHist_mumu;
      TH1D                    *fDeltaPhiLeptons_mumu;  
      TH1D                    *fDeltaEtaLeptons_mumu; 
      TH1D                    *fDileptonMass_mumu;
      TH1D                    *fMinDeltaPhiLeptonMet_mumu;
      TH1D                    *fLeptonEta_emu;   
      TH1D                    *fLeptonPtMax_emu;     
      TH1D                    *fLeptonPtMin_emu;        
      TH1D                    *fMetPtHist_emu;	     
      TH1D                    *fMetPhiHist_emu;           
      TH1D                    *fMETdeltaPhilEtHist_emu;     
      TH1D                    *fNCentralJets_emu;         
      TH1D                    *fNDirtyMuonsHist_emu;
      TH1D                    *fNCleanExtraTracksHist_emu;
      TH1D                    *fDeltaPhiLeptons_emu;      
      TH1D                    *fDeltaEtaLeptons_emu;      
      TH1D                    *fDileptonMass_emu;         
      TH1D                    *fMinDeltaPhiLeptonMet_emu; 


      TH1D                    *fLeptonEta_NMinusOne;   
      TH1D                    *fLeptonPtMax_NMinusOne;    
      TH1D                    *fLeptonPtMin_NMinusOne;    
      TH1D                    *fMetPtHist_NMinusOne;	      
      TH1D                    *fMetPhiHist_NMinusOne;        
      TH1D                    *fMETdeltaPhilEtHist_NMinusOne;     
      TH1D                    *fNCentralJets_NMinusOne;   
      TH1D                    *fNDirtyMuonsHist_NMinusOne;
      TH1D                    *fNCleanExtraTracksHist_NMinusOne;
      TH1D                    *fDeltaPhiLeptons_NMinusOne;
      TH1D                    *fDeltaEtaLeptons_NMinusOne;
      TH1D                    *fDileptonMass_NMinusOne;   
      TH1D                    *fMinDeltaPhiLeptonMet_NMinusOne; 
      TH1D                    *fLeptonEta_NMinusOne_ee;   
      TH1D                    *fLeptonPtMax_NMinusOne_ee;       
      TH1D                    *fLeptonPtMin_NMinusOne_ee;       
      TH1D                    *fMetPtHist_NMinusOne_ee;	     
      TH1D                    *fMetPhiHist_NMinusOne_ee;         
      TH1D                    *fMETdeltaPhilEtHist_NMinusOne_ee;     
      TH1D                    *fNCentralJets_NMinusOne_ee;      
      TH1D                    *fNDirtyMuonsHist_NMinusOne_ee;
      TH1D                    *fNCleanExtraTracksHist_NMinusOne_ee;
      TH1D                    *fDeltaPhiLeptons_NMinusOne_ee;   
      TH1D                    *fDeltaEtaLeptons_NMinusOne_ee;   
      TH1D                    *fDileptonMass_NMinusOne_ee;      
      TH1D                    *fMinDeltaPhiLeptonMet_NMinusOne_ee;
      TH1D                    *fLeptonEta_NMinusOne_mumu;   
      TH1D                    *fLeptonPtMax_NMinusOne_mumu;       
      TH1D                    *fLeptonPtMin_NMinusOne_mumu;       
      TH1D                    *fMetPtHist_NMinusOne_mumu;	  
      TH1D                    *fMetPhiHist_NMinusOne_mumu;        
      TH1D                    *fMETdeltaPhilEtHist_NMinusOne_mumu;     
      TH1D                    *fNCentralJets_NMinusOne_mumu;      
      TH1D                    *fNDirtyMuonsHist_NMinusOne_mumu;
      TH1D                    *fNCleanExtraTracksHist_NMinusOne_mumu;
      TH1D                    *fDeltaPhiLeptons_NMinusOne_mumu;   
      TH1D                    *fDeltaEtaLeptons_NMinusOne_mumu;   
      TH1D                    *fDileptonMass_NMinusOne_mumu;      
      TH1D                    *fMinDeltaPhiLeptonMet_NMinusOne_mumu;
      TH1D                    *fLeptonEta_NMinusOne_emu;   
      TH1D                    *fLeptonPtMax_NMinusOne_emu;          
      TH1D                    *fLeptonPtMin_NMinusOne_emu;          
      TH1D                    *fMetPtHist_NMinusOne_emu;	    
      TH1D                    *fMetPhiHist_NMinusOne_emu;           
      TH1D                    *fMETdeltaPhilEtHist_NMinusOne_emu;   
      TH1D                    *fNCentralJets_NMinusOne_emu;        
      TH1D                    *fNDirtyMuonsHist_NMinusOne_emu;
      TH1D                    *fNCleanExtraTracksHist_NMinusOne_emu;
      TH1D                    *fDeltaPhiLeptons_NMinusOne_emu;    
      TH1D                    *fDeltaEtaLeptons_NMinusOne_emu;   
      TH1D                    *fDileptonMass_NMinusOne_emu;       
      TH1D                    *fMinDeltaPhiLeptonMet_NMinusOne_emu;


      TH1D                    *fLeptonEta_afterCuts;
      TH1D                    *fLeptonPtMax_afterCuts;
      TH1D                    *fLeptonPtMin_afterCuts;
      TH1D                    *fMetPtHist_afterCuts;                             
      TH1D                    *fMetPhiHist_afterCuts;                           
      TH1D                    *fMETdeltaPhilEtHist_afterCuts;                                       
      TH1D                    *fDeltaPhiLeptons_afterCuts;
      TH1D                    *fDeltaEtaLeptons_afterCuts;
      TH1D                    *fDileptonMass_afterCuts;
      TH1D                    *fMinDeltaPhiLeptonMet_afterCuts;                                     
      TH1D                    *fMetSignificance_afterCuts;                               
      TH1D                    *fSumEt_afterCuts;
      TH1D                    *fMtLepton1_afterCuts;
      TH1D                    *fMtLepton2_afterCuts;
      TH1D                    *fMtWW_afterCuts;
      TH1D                    *fLeptonPtPlusMet_afterCuts;      
      TH1D                    *fLeptonEta_afterCuts_ee;
      TH1D                    *fLeptonPtMax_afterCuts_ee;
      TH1D                    *fLeptonPtMin_afterCuts_ee;
      TH1D                    *fMetPtHist_afterCuts_ee;                             
      TH1D                    *fMetPhiHist_afterCuts_ee;                           
      TH1D                    *fMETdeltaPhilEtHist_afterCuts_ee;     
      TH1D                    *fDeltaPhiLeptons_afterCuts_ee;
      TH1D                    *fDeltaEtaLeptons_afterCuts_ee;
      TH1D                    *fDileptonMass_afterCuts_ee;
      TH1D                    *fMinDeltaPhiLeptonMet_afterCuts_ee;   
      TH1D                    *fMetSignificance_afterCuts_ee;                               
      TH1D                    *fSumEt_afterCuts_ee;
      TH1D                    *fMtLepton1_afterCuts_ee;
      TH1D                    *fMtLepton2_afterCuts_ee;
      TH1D                    *fMtWW_afterCuts_ee;
      TH1D                    *fLeptonPtPlusMet_afterCuts_ee;
      TH1D                    *fLeptonEta_afterCuts_mumu;
      TH1D                    *fLeptonPtMax_afterCuts_mumu;
      TH1D                    *fLeptonPtMin_afterCuts_mumu;
      TH1D                    *fMetPtHist_afterCuts_mumu;                             
      TH1D                    *fMetPhiHist_afterCuts_mumu;                           
      TH1D                    *fMETdeltaPhilEtHist_afterCuts_mumu;     
      TH1D                    *fDeltaPhiLeptons_afterCuts_mumu;
      TH1D                    *fDeltaEtaLeptons_afterCuts_mumu;
      TH1D                    *fDileptonMass_afterCuts_mumu;
      TH1D                    *fMinDeltaPhiLeptonMet_afterCuts_mumu;    
      TH1D                    *fMetSignificance_afterCuts_mumu;                               
      TH1D                    *fSumEt_afterCuts_mumu;
      TH1D                    *fMtLepton1_afterCuts_mumu;
      TH1D                    *fMtLepton2_afterCuts_mumu;
      TH1D                    *fMtWW_afterCuts_mumu;
      TH1D                    *fLeptonPtPlusMet_afterCuts_mumu;
      TH1D                    *fLeptonEta_afterCuts_emu;
      TH1D                    *fLeptonPtMax_afterCuts_emu;
      TH1D                    *fLeptonPtMin_afterCuts_emu;
      TH1D                    *fMetPtHist_afterCuts_emu;                             
      TH1D                    *fMetPhiHist_afterCuts_emu;                           
      TH1D                    *fMETdeltaPhilEtHist_afterCuts_emu;     
      TH1D                    *fDeltaPhiLeptons_afterCuts_emu;
      TH1D                    *fDeltaEtaLeptons_afterCuts_emu;
      TH1D                    *fDileptonMass_afterCuts_emu;
      TH1D                    *fMinDeltaPhiLeptonMet_afterCuts_emu;     
      TH1D                    *fMetSignificance_afterCuts_emu;                               
      TH1D                    *fSumEt_afterCuts_emu;
      TH1D                    *fMtLepton1_afterCuts_emu;
      TH1D                    *fMtLepton2_afterCuts_emu;
      TH1D                    *fMtWW_afterCuts_emu;
      TH1D                    *fLeptonPtPlusMet_afterCuts_emu;

      int                      fNEventsProcessed;
      int                      fNEventsPassedSkim;
      int                      fNEventsPassedLeptonSelection;
      int                      fNEventsPassedKinematicPreselection;
      int                      fNEventsInsideAcceptance;

      void         Begin();
      void         Process();
      void         SlaveBegin();
      void         SlaveTerminate();
      void         Terminate();      

      ClassDef(WWEvtSelMod,1) // TAM example analysis module
  };
}
#endif
