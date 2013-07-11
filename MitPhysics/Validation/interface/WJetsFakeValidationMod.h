//--------------------------------------------------------------------------------------------------
// $Id: WJetsFakeValidationMod.h,v 1.4 2009/06/30 08:13:51 phedex Exp $
//
// WJetsFakeValidationMod
//
// A Module for Selecting H->WW events
// and produces some distributions.
//
//
// Authors: Si Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITPHYSICS_VALIDATION_WJETSFAKEVALIDATION_H
#define MITPHYSICS_VALIDATION_WJETSFAKEVALIDATION_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;

namespace mithep 
{
  class WJetsFakeValidationMod : public BaseMod
  {
    public:
    WJetsFakeValidationMod(const char *name="WJetsFakeValidationMod", 
		 const char *title="Example analysis module with all branches");
      ~WJetsFakeValidationMod() {}

      Bool_t      GetUseMCFake()                         { return fUseMCFake;                    }
      Bool_t      GetPerformFakeMuonMetCorrection()      { return fPerformFakeMuonMetCorrection; }
      Double_t    GetOSFakerateCorrection()              { return fOSFakerateCorrection;         }
      Double_t    GetSSFakerateCorrection()              { return fSSFakerateCorrection;         }
      const char *GetSampleName()                  const { return fSampleName;                   }
      const char *GetFakeEventHeaderName()         const { return fFakeEventHeaderName;          }
      const char *GetElectronFakeableObjectsName() const { return fElectronFakeableObjectsName;  }
      const char *GetMuonFakeableObjectsName()     const { return fMuonFakeableObjectsName;      }
      const char *GetMCPartBranchName()            const { return fMCPartBranchName;             }
      const char *GetGenJetBranchName()            const { return fGenJetBranchName;             }
      const char *GetTrackBranchName()             const { return fTrackBranchName;              }
      const char *GetMuonBranchName()              const { return fMuonBranchName;               }
      const char *GetMetName()                     const { return fMetName;                      }
      const char *GetTriggerObjectsName()          const { return fTriggerObjectsName;           }

      void   SetUseMCFake(Bool_t b)                        { fUseMCFake                    = b;  }
      void   SetPerformFakeMuonMetCorrection(Bool_t b)     { fPerformFakeMuonMetCorrection = b;  }
      void   SetOSFakerateCorrection(Double_t x)           { fOSFakerateCorrection         = x;  }
      void   SetSSFakerateCorrection(Double_t x)           { fSSFakerateCorrection         = x;  }
      void   SetSampleName(const char *s)                  { fSampleName                   = s;  }
      void   SetFakeEventHeaderName (const char *s)        { fFakeEventHeaderName          = s;  }
      void   SetElectronFakeableObjectsName(const char *s) { fElectronFakeableObjectsName  = s;  }
      void   SetMuonFakeableObjectsName(const char *s)     { fMuonFakeableObjectsName      = s;  }
      void   SetCleanJetsName (TString s)                  { fCleanJetsName                = s;  }
      void   SetMCPartBranchName (TString s)               { fMCPartBranchName             = s;  }
      void   SetGenJetBranchName (TString s)               { fGenJetBranchName             = s;  }
      void   SetTrackBranchName (TString s)                { fTrackBranchName              = s;  }
      void   SetMuonBranchName (TString s)                 { fMuonBranchName               = s;  }
      void   SetMetName (TString s)                        { fMetName                      = s;  }
      void   SetTriggerObjectsName(const char *name)       { fTriggerObjectsName         = name; }

    protected:
      Bool_t                fUseMCFake;                    //whether to use MC simulation fakes
      Bool_t                fPerformFakeMuonMetCorrection; //whether to perform fake muon 
                                                           //met correction
      Double_t              fOSFakerateCorrection;         //OS fake rate correction factor
      Double_t              fSSFakerateCorrection;         //OS fake rate correction factor
      TString               fSampleName;                   //name of Sample
      TString               fFakeEventHeaderName;          //name of fake event header         (input)
      TString               fElectronFakeableObjectsName;  //name of electron fakeable objects (input)
      TString               fMuonFakeableObjectsName;      //name of muon fakeable objects     (input)
      TString               fMCPartBranchName;             //name of particle collection
      TString               fGenJetBranchName;             //name of genjet collection
      TString               fTrackBranchName;            
      TString               fMuonBranchName;	           // name of muon collection
      TString               fMetName;                      //name of met collection
      TString               fCleanJetsName;                //name of clean central jets collection
      TString               fTriggerObjectsName;           //name of trigger objects
      const MCParticleCol  *fParticles;                    //!GenParticle branch
      const GenJetCol      *fGenJets;                      //!GenJet branch
      const TrackCol	   *fTracks;                       //!Track branch      
      const MuonCol	   *fMuons;	                   //!Muon branch
      const MetCol         *fMet;                          //!Missing Et
  
      TH1D *fElectronDenominatorPt;
      TH1D *fElectronDenominatorEta;
      TH1D *fElectronDenominatorPhi;
      TH1D *fFakeElectronPt;
      TH1D *fFakeElectronEta;
      TH1D *fFakeElectronPhi;
      TH1D *fFakeElectronPt_weighted;
      TH1D *fFakeElectronEta_weighted;
      TH1D *fFakeElectronPhi_weighted;
      TH1D *fFakeElectronPt_weighted_PlusMinus;
      TH1D *fFakeElectronEta_weighted_PlusMinus;
      TH1D *fFakeElectronPhi_weighted_PlusMinus;
      TH1D *fFakeElectronCategory;
      TH1D *fFakeElectronCategoryPlusMinus;
      TH1D *fAllFakeElectronCategory;
      TH1D *fAllFakeElectronDenominatorCategory;

      TH1D *fMuonDenominatorPt;
      TH1D *fMuonDenominatorEta;
      TH1D *fMuonDenominatorPhi;
      TH1D *fFakeMuonPt;
      TH1D *fFakeMuonEta;
      TH1D *fFakeMuonPhi;
      TH1D *fFakeMuonPt_weighted;
      TH1D *fFakeMuonEta_weighted;
      TH1D *fFakeMuonPhi_weighted;
      TH1D *fFakeMuonCategory;
      TH1D *fAllFakeMuonCategory;
      TH1D *fAllFakeMuonDenominatorCategory;

      TH1D *fElectronNumeratorPt;
      TH1D *fElectronNumeratorEta;
      TH1D *fElectronNumeratorPhi;
      TH1D *fMuonNumeratorPt;
      TH1D *fMuonNumeratorEta;
      TH1D *fMuonNumeratorPhi;
      TH2D *fAllElectronNumeratorPtEta;
      TH2D *fAllElectronDenominatorPtEta;
      TH2D *fFakeSelectionElectronNumeratorPtEta;
      TH2D *fFakeSelectionElectronDenominatorPtEta;
      TH2D *fPredictionElectronDenominatorPtEta;
      TH2D *fPredictionElectronNumeratorPtEta;

      TH1D *fRealElectronPt;      
      TH1D *fRealElectronEta;
      TH1D *fRealElectronPhi;
      TH1D *fElectron1Pt;
      TH1D *fElectron1Eta;
      TH1D *fElectron1Phi;
      TH1D *fNCleanElectrons;
      TH1D *fNDenominators;
      TH2D *fNCleanElectronsVsNDenominators;


      TH1D *fDileptonCharge;
      TH1D *fDileptonChargeUsingDenominator;
      TH1D *fDileptonChargeNotMatchedToDenominator;
      TH1D *fDileptonCharge_weighted;
      TH1D *fDileptonChargeWithoutCorrection_weighted;
      TH1D *fNLeptons;
      TH1D *fLepton1Pt;
      TH1D *fLepton1Pt_e;
      TH1D *fLepton1Pt_mu;
      TH1D *fLepton2Pt;
      TH1D *fLepton2Pt_e;
      TH1D *fLepton2Pt_mu;
      TH1D *fLepton3Pt;
      TH1D *fNLeptons_weighted;
      TH1D *fLepton1Pt_weighted;
      TH1D *fLepton1Pt_e_weighted;
      TH1D *fLepton1Pt_mu_weighted;
      TH1D *fLepton2Pt_weighted;
      TH1D *fLepton2Pt_e_weighted;
      TH1D *fLepton2Pt_mu_weighted;
      TH1D *fLepton3Pt_weighted;

      TH1D *fDileptonCharge_sysError;
      TH1D *fNLeptons_sysError;
      TH1D *fLepton1Pt_sysError;
      TH1D *fLepton2Pt_sysError;
      
      TH1D *fTriggerType;
      TH1D *fElectronTriggerType;
      TH1D *fMuonTriggerType;
      TH1D *fElectronTriggerObjectPt;
      TH1D *fElectronTriggerObjectEta;
      TH1D *fElectronTriggerObjectPt_Real;
      TH1D *fElectronTriggerObjectEta_Real;
      TH1D *fElectronTriggerObjectPt_Fake;
      TH1D *fElectronTriggerObjectEta_Fake;
      TH1D *fElectronTriggerObjectPt_Volunteer;
      TH1D *fElectronTriggerObjectEta_Volunteer;
      TH1D *fMuonTriggerObjectPt;
      TH1D *fMuonTriggerObjectEta;
      TH1D *fMuonTriggerObjectPt_Real;
      TH1D *fMuonTriggerObjectEta_Real;
      TH1D *fMuonTriggerObjectPt_Fake;
      TH1D *fMuonTriggerObjectEta_Fake;
      TH1D *fMuonTriggerObjectPt_Volunteer;
      TH1D *fMuonTriggerObjectEta_Volunteer;
      TH1D *fFailedTriggerLeptonPt_RealTriggerOnly;
      TH1D *fFailedTriggerLeptonEta_RealTriggerOnly;
      TH1D *fFailedTriggerElectronPt_RealTriggerOnly;
      TH1D *fFailedTriggerElectronEta_RealTriggerOnly;
      TH1D *fFailedTriggerMuonPt_RealTriggerOnly;
      TH1D *fFailedTriggerMuonEta_RealTriggerOnly;
      TH1D *fFailedTriggerLeptonPt_FakeTriggerOnly;
      TH1D *fFailedTriggerLeptonEta_FakeTriggerOnly;
      TH1D *fFailedTriggerElectronPt_FakeTriggerOnly;
      TH1D *fFailedTriggerElectronEta_FakeTriggerOnly;
      TH1D *fFailedTriggerMuonPt_FakeTriggerOnly;
      TH1D *fFailedTriggerMuonEta_FakeTriggerOnly;

      TH1D                    *fHWWSelection;
      TH1D                    *fHWWToEESelection;
      TH1D                    *fHWWToMuMuSelection;
      TH1D                    *fHWWToEMuSelection;
      TH1D                    *fHWWSelection_sysError;

      TH1D                    *fLeptonEta;
      TH1D                    *fLeptonPtMax;
      TH1D                    *fLeptonPtMin;
      TH1D                    *fMetPtHist;
      TH1D                    *fMetPhiHist;
      TH1D                    *fUncorrMetPtHist;
      TH1D                    *fUncorrMetPhiHist;
      TH1D                    *fDeltaPhiLeptons;
      TH1D                    *fDeltaEtaLeptons;
      TH1D                    *fDileptonMass;
      TH1D                    *fLeptonEta_ee;
      TH1D                    *fLeptonPtMax_ee;
      TH1D                    *fLeptonPtMin_ee;
      TH1D                    *fMetPtHist_ee;
      TH1D                    *fMetPhiHist_ee;
      TH1D                    *fUncorrMetPtHist_ee;
      TH1D                    *fUncorrMetPhiHist_ee;
      TH1D                    *fDeltaPhiLeptons_ee;
      TH1D                    *fDeltaEtaLeptons_ee;
      TH1D                    *fDileptonMass_ee;
      TH1D                    *fLeptonEta_mumu;
      TH1D                    *fLeptonPtMax_mumu;
      TH1D                    *fLeptonPtMin_mumu;
      TH1D                    *fMetPtHist_mumu;
      TH1D                    *fMetPhiHist_mumu;
      TH1D                    *fUncorrMetPtHist_mumu;
      TH1D                    *fUncorrMetPhiHist_mumu;
      TH1D                    *fDeltaPhiLeptons_mumu;
      TH1D                    *fDeltaEtaLeptons_mumu;
      TH1D                    *fDileptonMass_mumu;
      TH1D                    *fLeptonEta_emu;
      TH1D                    *fLeptonPtMax_emu;
      TH1D                    *fLeptonPtMin_emu;
      TH1D                    *fMetPtHist_emu;
      TH1D                    *fMetPhiHist_emu;
      TH1D                    *fUncorrMetPtHist_emu;
      TH1D                    *fUncorrMetPhiHist_emu;
      TH1D                    *fDeltaPhiLeptons_emu;
      TH1D                    *fDeltaEtaLeptons_emu;
      TH1D                    *fDileptonMass_emu;
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
      TH1D                    *fMinDeltaPhiLeptonMet_afterCuts;                                     
      TH1D                    *fMtLepton1_afterCuts;
      TH1D                    *fMtLepton2_afterCuts;
      TH1D                    *fMtHiggs_afterCuts;
      TH1D                    *fLeptonPtPlusMet_afterCuts;      
      TH1D                    *fMinDeltaPhiLeptonMet_afterCuts_ee;   
      TH1D                    *fMtLepton1_afterCuts_ee;
      TH1D                    *fMtLepton2_afterCuts_ee;
      TH1D                    *fMtHiggs_afterCuts_ee;
      TH1D                    *fLeptonPtPlusMet_afterCuts_ee;
      TH1D                    *fMinDeltaPhiLeptonMet_afterCuts_mumu; 
      TH1D                    *fMtLepton1_afterCuts_mumu;
      TH1D                    *fMtLepton2_afterCuts_mumu;
      TH1D                    *fMtHiggs_afterCuts_mumu;
      TH1D                    *fLeptonPtPlusMet_afterCuts_mumu;
      TH1D                    *fMinDeltaPhiLeptonMet_afterCuts_emu;
      TH1D                    *fMtLepton1_afterCuts_emu;
      TH1D                    *fMtLepton2_afterCuts_emu;
      TH1D                    *fMtHiggs_afterCuts_emu;
      TH1D                    *fLeptonPtPlusMet_afterCuts_emu;

      TH1D                    *fLeptonEta_sysError;
      TH1D                    *fLeptonPtMax_sysError;
      TH1D                    *fLeptonPtMin_sysError;
      TH1D                    *fMetPtHist_sysError;
      TH1D                    *fMetPhiHist_sysError;
      TH1D                    *fUncorrMetPtHist_sysError;
      TH1D                    *fUncorrMetPhiHist_sysError;
      TH1D                    *fDeltaPhiLeptons_sysError;
      TH1D                    *fDeltaEtaLeptons_sysError;
      TH1D                    *fDileptonMass_sysError;


      void         Begin();
      void         Process();
      void         SlaveBegin();
      void         SlaveTerminate();
      void         Terminate();      

      ClassDef(WJetsFakeValidationMod,1) // TAM example analysis module
  };
}
#endif
