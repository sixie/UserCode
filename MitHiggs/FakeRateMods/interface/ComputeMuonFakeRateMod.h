//--------------------------------------------------------------------------------------------------
// $Id: ComputeMuonFakeRateMod.h,v 1.7 2009/06/30 08:58:50 phedex Exp $
//
// ComputeMuonFakeRateMod
//
// This Module performs cleaning of objects including, removal of
// duplicate objects, applying object ID cuts, correction of MET
// The output will be collections of objects deemed as "good" and
// exported to other modules.
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef FAKERATEMODS_COMPUTEMUONFAKERATEMOD_H
#define FAKERATEMODS_COMPUTEMUONFAKERATEMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;
class TH3D;

namespace mithep 
{
  class ComputeMuonFakeRateMod : public BaseMod
  {
    public:
      ComputeMuonFakeRateMod(const char *name="ComputeMuonFakeRateMod", 
                     const char *title="Example analysis module with all branches");
      ~ComputeMuonFakeRateMod() {}

      Bool_t      GetVetoTriggerJet()                const { return fVetoTriggerJet;               }
      Bool_t      GetVetoGeneratorLeptons()          const { return fVetoGeneratorLeptons;         }
      Int_t       GetSampleType()                    const { return fSampleType;                   }
      const char *GetMCPartBranchName()              const { return fMCPartBranchName;             }
      const char *GetGenJetBranchName()              const { return fGenJetBranchName;             }
      const char *GetTriggerName()                   const { return fTriggerName;                  }
      const char *GetTriggerObjectsName()            const { return fTriggerObjectsName;           }
      const char *GetMuonsBranchName()               const { return fMuonBranchName;               }
      const char *GetTrackBranchName()               const { return fTrackBranchName;              }
      const char *GetMetName()                       const { return fMetName;                      }
      const char *GetCleanMuonsName()                const { return fCleanMuonsName;               }
      const char *GetGoodJetsName()                  const { return fGoodJetsName;                 }
      const char *GetCleanJetsName()                 const { return fCleanJetsName;                }
      const char *GetMCLeptonsName()                 const { return fMCLeptonsName;                }
      const char *GetMCTausName()                    const { return fMCTausName;                   }
      const char *GetMuonFakeableObjectsName()       const { return fMuonFakeableObjectsName;  }

      void SetVetoTriggerJet(Bool_t b)                       { fVetoTriggerJet             = b;    }
      void SetVetoGeneratorLeptons(Bool_t b)                 { fVetoGeneratorLeptons       = b;    }
      void SetSampleType(Int_t type)                         { fSampleType                 = type; }
      void SetMCPartBranchName(const char *name)             { fMCPartBranchName           = name; }
      void SetGenJetBranchName(const char *name)             { fGenJetBranchName           = name; }
      void SetTriggerName(const char *name)                  { fTriggerName                = name; }
      void SetTriggerObjectsName(const char *name)           { fTriggerObjectsName         = name; }
      void SetMuonBranchName(const char *name)               { fMuonBranchName             = name; }
      void SetTrackBranchName(const char *name)              { fTrackBranchName            = name; }
      void SetMetName (TString name)                         { fMetName                    = name; }
      void SetCleanMuonsName(const char *name)               { fCleanMuonsName             = name; }
      void SetGoodJetsName(const char *name)                 { fGoodJetsName               = name; }
      void SetCleanJetsName(const char *name)                { fCleanJetsName              = name; }
      void SetMCLeptonsName(const char *name)                { fMCLeptonsName              = name; }
      void SetMCTausName(const char *name)                   { fMCTausName                 = name; }
      void SetMuonFakeableObjectsName(const char *name)      { fMuonFakeableObjectsName    = name; }

    protected:
      Bool_t           fVetoTriggerJet;               //whether to veto on the leading jet
      Bool_t           fVetoGeneratorLeptons;         //whether to veto on gen electrons
      Int_t            fSampleType;                   //Integer to decide what sample it is
      TString          fMCPartBranchName;             //name of particle collection
      TString          fGenJetBranchName;             //name of genjet collection
      TString          fTriggerName;                  //name of trigger 
      TString          fTriggerObjectsName;           //name of trigger objects
      TString          fTrackBranchName;              //name of track collection
      TString          fMuonBranchName;               //name of electron collection
      TString          fMetName;                      //name of met collection              (input)
      TString          fCleanMuonsName;               //name of clean electrons            (input)
      TString          fGoodJetsName;                 //name of Good jets collection
      TString          fCleanJetsName;                //name of Clean jets collection
      TString          fMCLeptonsName;                //name of MC leptons
      TString          fMCTausName;                   //name of MC taus
      TString          fMuonFakeableObjectsName;      //name of fakeable objects           (input)

      const MCParticleCol     *fParticles;              //!GenParticle branch
      const GenJetCol         *fGenJets;                //!GenJet branch
      const TrackCol	      *fTracks;	                //!Track branch
      const MuonCol	      *fMuons;                  //!Muon branch
      const MetCol            *fMet;                    //!Missing Et
           
      TH1D *fTriggerJetPt;
      TH1D *fTriggerJetEta;
      TH1D *fTriggerJetPhi;
      TH1D *fTriggerPhotonPt;
      TH1D *fTriggerPhotonEta;
      TH1D *fTriggerPhotonPhi;
      
      TH1D *fNCleanLeptons;
      TH1D *fMetHist;
      TH1D *fNumeratorJetMass;
      TH1D *fDenominatorJetMass;
      TH1D *fNumeratorPhotonMass;
      TH1D *fDenominatorPhotonMass;
      TH1D *fDeltaPhiTriggerPhotonToLeadingJet;

      TH1D *fMuonDenominatorPt;
      TH1D *fMuonDenominatorEta;
      TH1D *fMuonDenominatorPhi;
      TH2D *fMuonDenominatorPtEta;
      TH3D *fMuonDenominatorPtEtaPhi;
      TH1D *fPlusMuonDenominatorPt;
      TH1D *fPlusMuonDenominatorEta;
      TH1D *fPlusMuonDenominatorPhi;
      TH1D *fMinusMuonDenominatorPt;
      TH1D *fMinusMuonDenominatorEta;
      TH1D *fMinusMuonDenominatorPhi;
      TH1D *fPlusMuonPlusWDenominatorPt;
      TH1D *fPlusMuonPlusWDenominatorEta;
      TH1D *fPlusMuonPlusWDenominatorPhi;
      TH1D *fMinusMuonPlusWDenominatorPt;
      TH1D *fMinusMuonPlusWDenominatorEta;
      TH1D *fMinusMuonPlusWDenominatorPhi;
      TH1D *fPlusMuonMinusWDenominatorPt;
      TH1D *fPlusMuonMinusWDenominatorEta;
      TH1D *fPlusMuonMinusWDenominatorPhi;
      TH1D *fMinusMuonMinusWDenominatorPt;
      TH1D *fMinusMuonMinusWDenominatorEta;
      TH1D *fMinusMuonMinusWDenominatorPhi;
      TH1D *fMuonDenominatorFakeCategory;      
      TH1D *fMuonDenominatorJetFlavor;      
      TH1D *fMuonDenominatorFragmentationX;
         

      TH1D *fMuonNumeratorPt;
      TH1D *fMuonNumeratorEta;
      TH1D *fMuonNumeratorPhi;
      TH2D *fMuonNumeratorPtEta;
      TH3D *fMuonNumeratorPtEtaPhi;
      TH1D *fPlusMuonNumeratorPt;
      TH1D *fPlusMuonNumeratorEta;
      TH1D *fPlusMuonNumeratorPhi;
      TH1D *fMinusMuonNumeratorPt;
      TH1D *fMinusMuonNumeratorEta;
      TH1D *fMinusMuonNumeratorPhi;
      TH1D *fPlusMuonPlusWNumeratorPt;
      TH1D *fPlusMuonPlusWNumeratorEta;
      TH1D *fPlusMuonPlusWNumeratorPhi;
      TH1D *fMinusMuonPlusWNumeratorPt;
      TH1D *fMinusMuonPlusWNumeratorEta;
      TH1D *fMinusMuonPlusWNumeratorPhi;
      TH1D *fPlusMuonMinusWNumeratorPt;
      TH1D *fPlusMuonMinusWNumeratorEta;
      TH1D *fPlusMuonMinusWNumeratorPhi;
      TH1D *fMinusMuonMinusWNumeratorPt;
      TH1D *fMinusMuonMinusWNumeratorEta;
      TH1D *fMinusMuonMinusWNumeratorPhi;
      TH1D *fMuonNumeratorFakeCategory;
      TH1D *fMuonNumeratorJetFlavor;        
      TH1D *fMuonNumeratorFragmentationX;
         
      TH1D *fMuonDenominatorPt_NoMatch;         
      TH1D *fMuonDenominatorEta_NoMatch;         
      TH1D *fMuonDenominatorPhi_NoMatch;         
      TH2D *fMuonDenominatorPtEta_NoMatch;                  
      TH1D *fMuonDenominatorPt_ChargedPion;         
      TH1D *fMuonDenominatorEta_ChargedPion;         
      TH1D *fMuonDenominatorPhi_ChargedPion;         
      TH2D *fMuonDenominatorPtEta_ChargedPion;                 
      TH1D *fMuonDenominatorPt_ChargedKaon;         
      TH1D *fMuonDenominatorEta_ChargedKaon;         
      TH1D *fMuonDenominatorPhi_ChargedKaon;         
      TH2D *fMuonDenominatorPtEta_ChargedKaon;                  
      TH1D *fMuonDenominatorPt_MuonFromOtherProcess;         
      TH1D *fMuonDenominatorEta_MuonFromOtherProcess;         
      TH1D *fMuonDenominatorPhi_MuonFromOtherProcess;         
      TH2D *fMuonDenominatorPtEta_MuonFromOtherProcess;                  
      TH1D *fMuonDenominatorPt_HeavyFlavor;         
      TH1D *fMuonDenominatorEta_HeavyFlavor;         
      TH1D *fMuonDenominatorPhi_HeavyFlavor;         
      TH2D *fMuonDenominatorPtEta_HeavyFlavor;                  
      TH1D *fMuonDenominatorPt_PionPunchThru;         
      TH1D *fMuonDenominatorEta_PionPunchThru;        
      TH1D *fMuonDenominatorPhi_PionPunchThru;         
      TH2D *fMuonDenominatorPtEta_PionPunchThru;                  
      TH1D *fMuonDenominatorPt_OtherNonMuon;         
      TH1D *fMuonDenominatorEta_OtherNonMuon;         
      TH1D *fMuonDenominatorPhi_OtherNonMuon;         
      TH2D *fMuonDenominatorPtEta_OtherNonMuon;                  
      TH1D *fMuonDenominatorPt_NoAncestor;         
      TH1D *fMuonDenominatorEta_NoAncestor;         
      TH1D *fMuonDenominatorPhi_NoAncestor;         
      TH2D *fMuonDenominatorPtEta_NoAncestor;
      
      TH1D *fMuonNumeratorPt_NoMatch;         
      TH1D *fMuonNumeratorEta_NoMatch;         
      TH1D *fMuonNumeratorPhi_NoMatch;         
      TH2D *fMuonNumeratorPtEta_NoMatch;         
      TH1D *fMuonNumeratorPt_ChargedPion;         
      TH1D *fMuonNumeratorEta_ChargedPion;         
      TH1D *fMuonNumeratorPhi_ChargedPion;         
      TH2D *fMuonNumeratorPtEta_ChargedPion;         
      TH1D *fMuonNumeratorPt_ChargedKaon;         
      TH1D *fMuonNumeratorEta_ChargedKaon;         
      TH1D *fMuonNumeratorPhi_ChargedKaon;         
      TH2D *fMuonNumeratorPtEta_ChargedKaon;         
      TH1D *fMuonNumeratorPt_MuonFromOtherProcess;         
      TH1D *fMuonNumeratorEta_MuonFromOtherProcess;         
      TH1D *fMuonNumeratorPhi_MuonFromOtherProcess;         
      TH2D *fMuonNumeratorPtEta_MuonFromOtherProcess;         
      TH1D *fMuonNumeratorPt_HeavyFlavor;         
      TH1D *fMuonNumeratorEta_HeavyFlavor;         
      TH1D *fMuonNumeratorPhi_HeavyFlavor;         
      TH2D *fMuonNumeratorPtEta_HeavyFlavor;         
      TH1D *fMuonNumeratorPt_PionPunchThru;         
      TH1D *fMuonNumeratorEta_PionPunchThru;         
      TH1D *fMuonNumeratorPhi_PionPunchThru;         
      TH2D *fMuonNumeratorPtEta_PionPunchThru;         
      TH1D *fMuonNumeratorPt_OtherNonMuon;         
      TH1D *fMuonNumeratorEta_OtherNonMuon;         
      TH1D *fMuonNumeratorPhi_OtherNonMuon;         
      TH2D *fMuonNumeratorPtEta_OtherNonMuon;         
      TH1D *fMuonNumeratorPt_NoAncestor;         
      TH1D *fMuonNumeratorEta_NoAncestor;         
      TH1D *fMuonNumeratorPhi_NoAncestor;         
      TH2D *fMuonNumeratorPtEta_NoAncestor;         
      
      TH1D *fMuonDenominatorPt_WithCuts;
      TH1D *fMuonDenominatorEta_WithCuts;
      TH1D *fMuonDenominatorPhi_WithCuts;
      TH2D *fMuonDenominatorPtEta_WithCuts;
      TH3D *fMuonDenominatorPtEtaPhi_WithCuts;
      TH1D *fMuonDenominatorFakeCategory_WithCuts;
      TH1D *fMuonDenominatorJetFlavor_WithCuts;
      TH1D *fMuonDenominatorFragmentationX_WithCuts;
      TH1D *fMuonNumeratorPt_WithCuts;
      TH1D *fMuonNumeratorEta_WithCuts;
      TH1D *fMuonNumeratorPhi_WithCuts;
      TH2D *fMuonNumeratorPtEta_WithCuts;
      TH3D *fMuonNumeratorPtEtaPhi_WithCuts;
      TH1D *fMuonNumeratorFakeCategory_WithCuts;
      TH1D *fMuonNumeratorJetFlavor_WithCuts;
      TH1D *fMuonNumeratorFragmentationX_WithCuts;


      void                     Begin();
      void                     Process();
      void                     SlaveBegin();
      void                     SlaveTerminate();
      void                     Terminate();      

      ClassDef(ComputeMuonFakeRateMod,1) // TAM example analysis module
  };
}
#endif
