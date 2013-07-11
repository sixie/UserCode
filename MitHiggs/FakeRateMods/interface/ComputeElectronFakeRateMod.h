//--------------------------------------------------------------------------------------------------
// $Id: ComputeElectronFakeRateMod.h,v 1.7 2009/06/30 08:58:50 phedex Exp $
//
// ComputeElectronFakeRateMod
//
// This Module performs cleaning of objects including, removal of
// duplicate objects, applying object ID cuts, correction of MET
// The output will be collections of objects deemed as "good" and
// exported to other modules.
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef FAKERATEMODS_COMPUTEELECTRONFAKERATEMOD_H
#define FAKERATEMODS_COMPUTEELECTRONFAKERATEMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;
class TH3D;

namespace mithep 
{
  class ComputeElectronFakeRateMod : public BaseMod
  {
    public:
      ComputeElectronFakeRateMod(const char *name="ComputeElectronFakeRateMod", 
                     const char *title="Example analysis module with all branches");
      ~ComputeElectronFakeRateMod() {}

      Bool_t      GetVetoTriggerJet()                const { return fVetoTriggerJet;               }
      Bool_t      GetVetoGeneratorLeptons()          const { return fVetoGeneratorLeptons;         }
      Int_t       GetSampleType()                    const { return fSampleType;                   }
      const char *GetMCPartBranchName()              const { return fMCPartBranchName;             }
      const char *GetGenJetBranchName()              const { return fGenJetBranchName;             }
      const char *GetTriggerName()                   const { return fTriggerName;                  }
      const char *GetTriggerObjectsName()            const { return fTriggerObjectsName;           }
      const char *GetElectronsBranchName()           const { return fElectronBranchName;           }
      const char *GetTrackBranchName()               const { return fTrackBranchName;              }
      const char *GetGsfTrackBranchName()            const { return fGsfTrackBranchName;           }
      const char *GetBarrelSuperClustersBranchName() const { return fBarrelSuperClusterBranchName; }
      const char *GetEndcapSuperClustersBranchName() const { return fEndcapSuperClusterBranchName; }
      const char *GetMetName()                       const { return fMetName;                      }
      const char *GetCleanElectronsName()            const { return fCleanElectronsName;           }
      const char *GetGoodJetsName()                  const { return fGoodJetsName;                 }
      const char *GetCleanJetsName()                 const { return fCleanJetsName;                }
      const char *GetMCLeptonsName()                 const { return fMCLeptonsName;                }
      const char *GetMCTausName()                    const { return fMCTausName;                   }
      const char *GetElectronFakeableObjectsName()   const { return fElectronFakeableObjectsName;  }

      void SetVetoTriggerJet(Bool_t b)                       { fVetoTriggerJet             = b;    }
      void SetVetoGeneratorLeptons(Bool_t b)                 { fVetoGeneratorLeptons       = b;    }
      void SetSampleType(Int_t type)                         { fSampleType                 = type; }
      void SetMCPartBranchName(const char *name)             { fMCPartBranchName           = name; }
      void SetGenJetBranchName(const char *name)             { fGenJetBranchName           = name; }
      void SetTriggerName(const char *name)                  { fTriggerName                = name; }
      void SetTriggerObjectsName(const char *name)           { fTriggerObjectsName         = name; }
      void SetElectronBranchName(const char *name)           { fElectronBranchName         = name; }
      void SetTrackBranchName(const char *name)              { fTrackBranchName            = name; }
      void SetGsfTrackBranchName(const char *name)           { fGsfTrackBranchName         = name; }
      void SetBarrelSuperClusterBranchName(const char *name) { fBarrelSuperClusterBranchName=name; }
      void SetEndcapSuperClusterBranchName(const char *name) { fEndcapSuperClusterBranchName=name; }
      void SetMetName (TString name)                         { fMetName                    = name; }
      void SetCleanElectronsName(const char *name)           { fCleanElectronsName         = name; }
      void SetGoodJetsName(const char *name)                 { fGoodJetsName               = name; }
      void SetCleanJetsName(const char *name)                { fCleanJetsName              = name; }
      void SetMCLeptonsName(const char *name)                { fMCLeptonsName              = name; }
      void SetMCTausName(const char *name)                   { fMCTausName                 = name; }
      void SetElectronFakeableObjectsName(const char *name)  { fElectronFakeableObjectsName= name; }

    protected:
      Bool_t           fVetoTriggerJet;               //whether to veto on the leading jet
      Bool_t           fVetoGeneratorLeptons;         //whether to veto on gen electrons
      Int_t            fSampleType;                   //Integer to decide what sample it is
      TString          fMCPartBranchName;             //name of particle collection
      TString          fGenJetBranchName;             //name of genjet collection
      TString          fTriggerName;                  //name of trigger 
      TString          fTriggerObjectsName;           //name of trigger objects
      TString          fTrackBranchName;              //name of track collection
      TString          fGsfTrackBranchName;           //name of track collection
      TString          fElectronBranchName;           //name of electron collection
      TString          fBarrelSuperClusterBranchName; //name of barrel supercluster branch  (input)
      TString          fEndcapSuperClusterBranchName; //name of endcap supercluster branch  (input)
      TString          fMetName;                      //name of met collection              (input)
      TString          fCleanElectronsName;           //name of clean electrons             (input)
      TString          fGoodJetsName;                 //name of Good jets collection
      TString          fCleanJetsName;                //name of Clean jets collection
      TString          fMCLeptonsName;                //name of MC leptons
      TString          fMCTausName;                   //name of MC taus
      TString          fElectronFakeableObjectsName;  //name of fakeable objects           (input)

      const MCParticleCol   *fParticles;              //!GenParticle branch
      const GenJetCol       *fGenJets;                //!GenJet branch
      const SuperClusterCol *fBarrelSuperClusters;    //!Barrel SC branch
      const SuperClusterCol *fEndcapSuperClusters;    //!Endcap SC branch
      const TrackCol	    *fTracks;	              //!Track branch
      const TrackCol	    *fGsfTracks;	      //!GsfTrack branch
      const ElectronCol	    *fElectrons;              //!Electron branch
      const MetCol          *fMet;                    //!Missing Et

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

      TH1D *fElectronDenominatorPt;
      TH1D *fElectronDenominatorEta;
      TH1D *fElectronDenominatorPhi;
      TH2D *fElectronDenominatorPtEta;
      TH3D *fElectronDenominatorPtEtaPhi;
      TH1D *fPlusElectronDenominatorPt;
      TH1D *fPlusElectronDenominatorEta;
      TH1D *fPlusElectronDenominatorPhi;
      TH1D *fMinusElectronDenominatorPt;
      TH1D *fMinusElectronDenominatorEta;
      TH1D *fMinusElectronDenominatorPhi;
      TH1D *fPlusElectronPlusWDenominatorPt;
      TH1D *fPlusElectronPlusWDenominatorEta;
      TH1D *fPlusElectronPlusWDenominatorPhi;
      TH1D *fMinusElectronPlusWDenominatorPt;
      TH1D *fMinusElectronPlusWDenominatorEta;
      TH1D *fMinusElectronPlusWDenominatorPhi;
      TH1D *fPlusElectronMinusWDenominatorPt;
      TH1D *fPlusElectronMinusWDenominatorEta;
      TH1D *fPlusElectronMinusWDenominatorPhi;
      TH1D *fMinusElectronMinusWDenominatorPt;
      TH1D *fMinusElectronMinusWDenominatorEta;
      TH1D *fMinusElectronMinusWDenominatorPhi;
      TH2D *fPredictionElectronDenominatorPtEta;
      TH1D *fElectronDenominatorFakeCategory;
      TH1D *fElectronDenominatorJetFlavor;
      TH1D *fElectronDenominatorFragmentationX;

      TH1D *fElectronNumeratorPt;
      TH1D *fElectronNumeratorEta;
      TH1D *fElectronNumeratorPhi;
      TH2D *fElectronNumeratorPtEta;
      TH3D *fElectronNumeratorPtEtaPhi;
      TH1D *fPlusElectronNumeratorPt;
      TH1D *fPlusElectronNumeratorEta;
      TH1D *fPlusElectronNumeratorPhi;
      TH1D *fMinusElectronNumeratorPt;
      TH1D *fMinusElectronNumeratorEta;
      TH1D *fMinusElectronNumeratorPhi;
      TH1D *fPlusElectronPlusWNumeratorPt;
      TH1D *fPlusElectronPlusWNumeratorEta;
      TH1D *fPlusElectronPlusWNumeratorPhi;
      TH1D *fMinusElectronPlusWNumeratorPt;
      TH1D *fMinusElectronPlusWNumeratorEta;
      TH1D *fMinusElectronPlusWNumeratorPhi;
      TH1D *fPlusElectronMinusWNumeratorPt;
      TH1D *fPlusElectronMinusWNumeratorEta;
      TH1D *fPlusElectronMinusWNumeratorPhi;
      TH1D *fMinusElectronMinusWNumeratorPt;
      TH1D *fMinusElectronMinusWNumeratorEta;
      TH1D *fMinusElectronMinusWNumeratorPhi;
      TH2D *fPredictionElectronNumeratorPtEta;
      TH1D *fElectronNumeratorFakeCategory;
      TH1D *fElectronNumeratorJetFlavor;
      TH1D *fElectronNumeratorFragmentationX;

      TH1D *fElectronDenominatorPt_NoMatch;
      TH1D *fElectronDenominatorEta_NoMatch;
      TH1D *fElectronDenominatorPhi_NoMatch;
      TH2D *fElectronDenominatorPtEta_NoMatch;
      TH1D *fElectronDenominatorPt_Conversion;
      TH1D *fElectronDenominatorEta_Conversion;
      TH1D *fElectronDenominatorPhi_Conversion;
      TH2D *fElectronDenominatorPtEta_Conversion;
      TH1D *fElectronDenominatorPt_ChargedPion;
      TH1D *fElectronDenominatorEta_ChargedPion;
      TH1D *fElectronDenominatorPhi_ChargedPion;
      TH2D *fElectronDenominatorPtEta_ChargedPion;
      TH1D *fElectronDenominatorPt_ChargedKaon;
      TH1D *fElectronDenominatorEta_ChargedKaon;
      TH1D *fElectronDenominatorPhi_ChargedKaon;
      TH2D *fElectronDenominatorPtEta_ChargedKaon;
      TH1D *fElectronDenominatorPt_HeavyFlavor;
      TH1D *fElectronDenominatorEta_HeavyFlavor;
      TH1D *fElectronDenominatorPhi_HeavyFlavor;
      TH2D *fElectronDenominatorPtEta_HeavyFlavor;
      TH1D *fElectronDenominatorPt_Brem;
      TH1D *fElectronDenominatorEta_Brem;
      TH1D *fElectronDenominatorPhi_Brem;
      TH2D *fElectronDenominatorPtEta_Brem;
      TH1D *fElectronDenominatorPt_PromptPhoton;
      TH1D *fElectronDenominatorEta_PromptPhoton;
      TH1D *fElectronDenominatorPhi_PromptPhoton;
      TH2D *fElectronDenominatorPtEta_PromptPhoton;
      TH1D *fElectronDenominatorPt_Other;
      TH1D *fElectronDenominatorEta_Other;
      TH1D *fElectronDenominatorPhi_Other;
      TH2D *fElectronDenominatorPtEta_Other;
      TH1D *fElectronNumeratorPt_NoMatch;
      TH1D *fElectronNumeratorEta_NoMatch;
      TH1D *fElectronNumeratorPhi_NoMatch;
      TH2D *fElectronNumeratorPtEta_NoMatch;
      TH1D *fElectronNumeratorPt_Conversion;
      TH1D *fElectronNumeratorEta_Conversion;
      TH1D *fElectronNumeratorPhi_Conversion;
      TH2D *fElectronNumeratorPtEta_Conversion;
      TH1D *fElectronNumeratorPt_ChargedPion;
      TH1D *fElectronNumeratorEta_ChargedPion;
      TH1D *fElectronNumeratorPhi_ChargedPion;
      TH2D *fElectronNumeratorPtEta_ChargedPion;
      TH1D *fElectronNumeratorPt_ChargedKaon;
      TH1D *fElectronNumeratorEta_ChargedKaon;
      TH1D *fElectronNumeratorPhi_ChargedKaon;
      TH2D *fElectronNumeratorPtEta_ChargedKaon;
      TH1D *fElectronNumeratorPt_HeavyFlavor;
      TH1D *fElectronNumeratorEta_HeavyFlavor;
      TH1D *fElectronNumeratorPhi_HeavyFlavor;
      TH2D *fElectronNumeratorPtEta_HeavyFlavor;
      TH1D *fElectronNumeratorPt_Brem;
      TH1D *fElectronNumeratorEta_Brem;
      TH1D *fElectronNumeratorPhi_Brem;
      TH2D *fElectronNumeratorPtEta_Brem;
      TH1D *fElectronNumeratorPt_PromptPhoton;
      TH1D *fElectronNumeratorEta_PromptPhoton;
      TH1D *fElectronNumeratorPhi_PromptPhoton;
      TH2D *fElectronNumeratorPtEta_PromptPhoton;
      TH1D *fElectronNumeratorPt_Other;
      TH1D *fElectronNumeratorEta_Other;
      TH1D *fElectronNumeratorPhi_Other;
      TH2D *fElectronNumeratorPtEta_Other;

      TH1D *fElectronNumeratorJetFlavor_Conversion;
      TH1D *fElectronNumeratorJetFlavor_ChargedPion;
      TH1D *fElectronNumeratorJetFlavor_ChargedKaon;
      TH1D *fElectronNumeratorJetFlavor_HeavyFlavor;
      TH1D *fElectronNumeratorFragmentationX_Conversion;
      TH1D *fElectronNumeratorFragmentationX_ChargedPion;
      TH1D *fElectronNumeratorFragmentationX_ChargedKaon;
      TH1D *fElectronNumeratorFragmentationX_HeavyFlavor;
      TH1D *fElectronDenominatorJetFlavor_Conversion;
      TH1D *fElectronDenominatorJetFlavor_ChargedPion;
      TH1D *fElectronDenominatorJetFlavor_ChargedKaon;
      TH1D *fElectronDenominatorJetFlavor_HeavyFlavor;
      TH1D *fElectronDenominatorFragmentationX_Conversion;
      TH1D *fElectronDenominatorFragmentationX_ChargedPion;
      TH1D *fElectronDenominatorFragmentationX_ChargedKaon;
      TH1D *fElectronDenominatorFragmentationX_HeavyFlavor;

      TH1D *fSCTrackMatchDeltaR_All;
      TH1D *fSCTrackMatchDeltaR_Numerator;
      TH1D *fSCTrackMatchDeltaR_FrameworkDenominator;
      TH1D *fSCTrackMatchDeltaR_EOverPDenominator;
      TH1D *fSCTrackMatchDeltaPt_DRMatched;
      TH1D *fSCTrackMatchDeltaPt_FrameworkDenominator;
      TH1D *fSCTrackMatchDeltaPt_EOverPDenominator;
      TH1D *fSCTrackMatchEOverP_EOverPDenominator;
      TH1D *fGsfTrackToRegularTrackDeltaR;
      TH1D *fGsfTrackToRegularTrackDeltaRWithPtMatch;
      TH1D *fGsfTrackToRegularTrackDeltaPt;
      TH1D *fEOverP_RejectedGsfTracksWithinCone;
      TH1D *fDeltaR_RejectedGsfTracks;

      TH1D *fElectronDenominatorPt_WithCuts;
      TH1D *fElectronDenominatorEta_WithCuts;
      TH1D *fElectronDenominatorPhi_WithCuts;
      TH2D *fElectronDenominatorPtEta_WithCuts;
      TH3D *fElectronDenominatorPtEtaPhi_WithCuts;
      TH1D *fElectronDenominatorFakeCategory_WithCuts;
      TH1D *fElectronDenominatorJetFlavor_WithCuts;
      TH1D *fElectronDenominatorFragmentationX_WithCuts;
      TH1D *fElectronNumeratorPt_WithCuts;
      TH1D *fElectronNumeratorEta_WithCuts;
      TH1D *fElectronNumeratorPhi_WithCuts;
      TH2D *fElectronNumeratorPtEta_WithCuts;
      TH3D *fElectronNumeratorPtEtaPhi_WithCuts;
      TH1D *fElectronNumeratorFakeCategory_WithCuts;
      TH1D *fElectronNumeratorJetFlavor_WithCuts;
      TH1D *fElectronNumeratorFragmentationX_WithCuts;

      void                     Begin();
      void                     Process();
      void                     SlaveBegin();
      void                     SlaveTerminate();
      void                     Terminate();      

      ClassDef(ComputeElectronFakeRateMod,1) // TAM example analysis module
  };
}
#endif
