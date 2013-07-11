//--------------------------------------------------------------------------------------------------
// $Id: FakeElectronAnalysisMod.h,v 1.2 2009/03/17 08:33:15 sixie Exp $
//
// FakeElectronAnalysisMod
//
// This Module performs cleaning of objects including, removal of
// duplicate objects, applying object ID cuts, correction of MET
// The output will be collections of objects deemed as "good" and
// exported to other modules.
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef FAKERATEMODS_FAKEELECTRONANALYSISMOD_H
#define FAKERATEMODS_FAKEELECTRONANALYSISMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;
class TH3D;

namespace mithep 
{
  class FakeElectronAnalysisMod : public BaseMod
  {
    public:
      FakeElectronAnalysisMod(const char *name="FakeElectronAnalysisMod", 
                     const char *title="Example analysis module with all branches");
      ~FakeElectronAnalysisMod() {}

      const Bool_t GetVetoGeneratorLeptons()         const { return fVetoGeneratorLeptons;         }
      const Bool_t GetRequireFirstLepton()           const { return fRequireFirstLepton;           }
      const Bool_t GetPrintLogs()                    const { return fPrintLogs;                    }
      const char *GetMCPartBranchName()              const { return fMCPartBranchName;             }
      const char *GetGenJetBranchName()              const { return fGenJetBranchName;             }
      const char *GetVertexBranchName()              const { return fVertexBranchName;             }
      const char *GetElectronsBranchName()           const { return fElectronBranchName;           }
      const char *GetTrackBranchName()               const { return fTrackBranchName;              }
      const char *GetGsfTrackBranchName()            const { return fGsfTrackBranchName;           }
      const char *GetBarrelSuperClustersBranchName() const { return fBarrelSuperClusterBranchName; }
      const char *GetEndcapSuperClustersBranchName() const { return fEndcapSuperClusterBranchName; }
      const char *GetJetBranchName()                 const { return fJetBranchName;                }
      const char *GetCleanElectronsName()            const { return fCleanElectronsName;           }
      const char *GetGoodJetsName()                  const { return fGoodJetsName;                 }
      const char *GetMCLeptonsName()                 const { return fMCLeptonsName;                }
      const char *GetMCTausName()                    const { return fMCTausName;                   }

      void SetVetoGeneratorLeptons(Bool_t b)                 { fVetoGeneratorLeptons       = b;    }
      void SetRequireFirstLepton(Bool_t b)                   { fRequireFirstLepton         = b;    }
      void SetPrintLogs(Bool_t b)                            { fPrintLogs                  = b;    }
      void SetMCPartBranchName(const char *name)             { fMCPartBranchName           = name; }
      void SetGenJetBranchName(const char *name)             { fGenJetBranchName           = name; }
      void SetVertexBranchName(const char *name)             { fVertexBranchName           = name; }
      void SetElectronBranchName(const char *name)           { fElectronBranchName         = name; }
      void SetTrackBranchName(const char *name)              { fTrackBranchName            = name; }
      void SetGsfTrackBranchName(const char *name)           { fGsfTrackBranchName         = name; }
      void SetBarrelSuperClusterBranchName(const char *name) { fBarrelSuperClusterBranchName=name; }
      void SetEndcapSuperClusterBranchName(const char *name) { fEndcapSuperClusterBranchName=name; }
      void SetJetBranchName(const char *name)                { fJetBranchName              = name; }
      void SetCleanElectronsName(const char *name)           { fCleanElectronsName         = name; }
      void SetGoodJetsName(const char *name)                 { fGoodJetsName               = name; }
      void SetMCLeptonsName(const char *name)                { fMCLeptonsName              = name; }
      void SetMCTausName(const char *name)                   { fMCTausName                 = name; }
 
    protected:
      Bool_t           fVetoGeneratorLeptons;         //whether to veto on gen electrons
      Bool_t           fRequireFirstLepton;           //whether we require another real lepton
      Bool_t           fPrintLogs;                    //whether we print the logs
      TString          fMCPartBranchName;             //name of particle collection
      TString          fGenJetBranchName;             //name of genjet collection
      TString          fVertexBranchName;             //name of vertex collection
      TString          fTrackBranchName;              //name of track collection
      TString          fGsfTrackBranchName;           //name of track collection
      TString          fElectronBranchName;           //name of electron collection
      TString          fJetBranchName;                //name of jet collection
      TString          fBarrelSuperClusterBranchName; //name of barrel supercluster branch  (input)
      TString          fEndcapSuperClusterBranchName; //name of endcap supercluster branch  (input)
      TString          fCleanElectronsName;           //name of clean electrons             (input)
      TString          fGoodJetsName;                 //name of Good jets collection
      TString          fMCLeptonsName;                //name of MC leptons
      TString          fMCTausName;                   //name of MC taus

      const MCParticleCol   *fParticles;              //!GenParticle branch
      const VertexCol       *fVertices;               //!Vertex branch
      const GenJetCol       *fGenJets;                //!GenJet branch
      const SuperClusterCol *fBarrelSuperClusters;    //!Barrel SC branch
      const SuperClusterCol *fEndcapSuperClusters;    //!Endcap SC branch      
      const TrackCol	    *fTracks;	              //!Track branch
      const TrackCol	    *fGsfTracks;	      //!GsfTrack branch
      const ElectronCol	    *fElectrons;              //!Electron branch
      const JetCol          *fJets;                   //!Jet branch            

      TH1D *fNGenJets;
      TH1D *fNCleanLeptons;
      TH1D *fNGenLeptons;

      TH1D *fCleanBarrelElectronTrackerGsfTrackChargeMatch;
      TH1D *fCleanEndcapElectronTrackerGsfTrackChargeMatch;

      TH1D *fCleanPlusBarrelElectronMatchedJetFlavor;
      TH1D *fCleanMinusBarrelElectronMatchedJetFlavor;      
      TH1D *fCleanPlusBarrelElectronMatchedGenJetPt;
      TH1D *fCleanMinusBarrelElectronMatchedGenJetPt;
      TH1D *fCleanPlusBarrelElectronMatchedGenJetDeltaR;
      TH1D *fCleanMinusBarrelElectronMatchedGenJetDeltaR;
      TH1D *fCleanPlusBarrelElectronMatchedGenJetDeltaPtOverPt;
      TH1D *fCleanMinusBarrelElectronMatchedGenJetDeltaPtOverPt;
      TH1D *fCleanPlusEndcapElectronMatchedJetFlavor;
      TH1D *fCleanMinusEndcapElectronMatchedJetFlavor;      
      TH1D *fCleanPlusEndcapElectronMatchedGenJetPt;
      TH1D *fCleanMinusEndcapElectronMatchedGenJetPt;
      TH1D *fCleanPlusEndcapElectronMatchedGenJetDeltaR;
      TH1D *fCleanMinusEndcapElectronMatchedGenJetDeltaR;
      TH1D *fCleanPlusEndcapElectronMatchedGenJetDeltaPtOverPt;
      TH1D *fCleanMinusEndcapElectronMatchedGenJetDeltaPtOverPt;
      
      TH1D *fCleanPlusBarrelElectronMatchedGenParticleID;
      TH1D *fCleanMinusBarrelElectronMatchedGenParticleID;
      TH1D *fCleanPlusBarrelElectronMatchedGenParticlePt;
      TH1D *fCleanMinusBarrelElectronMatchedGenParticlePt;
      TH1D *fCleanPlusBarrelElectronMatchedGenParticleDeltaR;
      TH1D *fCleanMinusBarrelElectronMatchedGenParticleDeltaR;
      TH1D *fCleanPlusBarrelElectronMatchedGenParticleDeltaPtOverPt;
      TH1D *fCleanMinusBarrelElectronMatchedGenParticleDeltaPtOverPt;
      TH1D *fCleanPlusEndcapElectronMatchedGenParticleID;
      TH1D *fCleanMinusEndcapElectronMatchedGenParticleID;
      TH1D *fCleanPlusEndcapElectronMatchedGenParticlePt;
      TH1D *fCleanMinusEndcapElectronMatchedGenParticlePt;
      TH1D *fCleanPlusEndcapElectronMatchedGenParticleDeltaR;
      TH1D *fCleanMinusEndcapElectronMatchedGenParticleDeltaR;
      TH1D *fCleanPlusEndcapElectronMatchedGenParticleDeltaPtOverPt;
      TH1D *fCleanMinusEndcapElectronMatchedGenParticleDeltaPtOverPt;

      TH1D *fCleanPlusBarrelElectronMatchedSimChargedParticleID;
      TH1D *fCleanMinusBarrelElectronMatchedSimChargedParticleID;
      TH1D *fCleanPlusBarrelElectronMatchedSimChargedParticlePt;
      TH1D *fCleanMinusBarrelElectronMatchedSimChargedParticlePt;
      TH1D *fCleanPlusBarrelElectronMatchedSimChargedParticleDeltaR;
      TH1D *fCleanMinusBarrelElectronMatchedSimChargedParticleDeltaR;
      TH1D *fCleanPlusBarrelElectronMatchedSimChargedParticleDeltaPtOverPt;
      TH1D *fCleanMinusBarrelElectronMatchedSimChargedParticleDeltaPtOverPt;
      TH1D *fCleanPlusEndcapElectronMatchedSimChargedParticleID;
      TH1D *fCleanMinusEndcapElectronMatchedSimChargedParticleID;
      TH1D *fCleanPlusEndcapElectronMatchedSimChargedParticlePt;
      TH1D *fCleanMinusEndcapElectronMatchedSimChargedParticlePt;
      TH1D *fCleanPlusEndcapElectronMatchedSimChargedParticleDeltaR;
      TH1D *fCleanMinusEndcapElectronMatchedSimChargedParticleDeltaR;
      TH1D *fCleanPlusEndcapElectronMatchedSimChargedParticleDeltaPtOverPt;
      TH1D *fCleanMinusEndcapElectronMatchedSimChargedParticleDeltaPtOverPt;

      TH1D *fCleanPlusBarrelElectronMatchedAncestorParticleID;
      TH1D *fCleanMinusBarrelElectronMatchedAncestorParticleID;
      TH1D *fCleanPlusBarrelElectronMatchedAncestorParticlePt;
      TH1D *fCleanMinusBarrelElectronMatchedAncestorParticlePt;
      TH1D *fCleanPlusBarrelElectronMatchedAncestorParticleDeltaR;
      TH1D *fCleanMinusBarrelElectronMatchedAncestorParticleDeltaR;
      TH1D *fCleanPlusBarrelElectronMatchedAncestorParticleDeltaPtOverPt;
      TH1D *fCleanMinusBarrelElectronMatchedAncestorParticleDeltaPtOverPt;
      TH1D *fCleanPlusEndcapElectronMatchedAncestorParticleID;
      TH1D *fCleanMinusEndcapElectronMatchedAncestorParticleID;
      TH1D *fCleanPlusEndcapElectronMatchedAncestorParticlePt;
      TH1D *fCleanMinusEndcapElectronMatchedAncestorParticlePt;
      TH1D *fCleanPlusEndcapElectronMatchedAncestorParticleDeltaR;
      TH1D *fCleanMinusEndcapElectronMatchedAncestorParticleDeltaR;
      TH1D *fCleanPlusEndcapElectronMatchedAncestorParticleDeltaPtOverPt;
      TH1D *fCleanMinusEndcapElectronMatchedAncestorParticleDeltaPtOverPt;

      TH1D *fCleanPlusBarrelElectronFakeCategory;
      TH1D *fCleanMinusBarrelElectronFakeCategory;
      TH1D *fCleanPlusEndcapElectronFakeCategory;
      TH1D *fCleanMinusEndcapElectronFakeCategory;

      TH1D *fCleanPlusBarrelElectronUnmatchedSimChargedParticleBestDeltaPtOverPt;
      TH1D *fCleanMinusBarrelElectronUnmatchedSimChargedParticleBestDeltaPtOverPt;
      TH1D *fCleanPlusEndcapElectronUnmatchedSimChargedParticleBestDeltaPtOverPt;
      TH1D *fCleanMinusEndcapElectronUnmatchedSimChargedParticleBestDeltaPtOverPt;

      TH1D *fConversionFakeElectronPt;
      TH1D *fConversionFakeElectronEta;
      TH1D *fConversionFakeElectronPhi;
      TH1D *fConversionFakeElectronTrkIso;
      TH1D *fConversionFakeElectronJurassicIso;
      TH1D *fConversionFakeElectronHOverE;
      TH1D *fConversionFakeElectronEOverP;
      TH1D *fConversionFakeElectronSigmaEtaEta;
      TH1D *fConversionFakeElectronSigmaiEtaiEta;
      TH1D *fConversionFakeElectronDeltaPhi;
      TH1D *fConversionFakeElectronDeltaEta;
      TH1D *fConversionFakeElectronfBrem;
      TH1D *fConversionFakeElectronClassification;
      TH1D *fChargedPionFakeElectronPt;
      TH1D *fChargedPionFakeElectronEta;
      TH1D *fChargedPionFakeElectronPhi;
      TH1D *fChargedPionFakeElectronTrkIso;
      TH1D *fChargedPionFakeElectronJurassicIso;
      TH1D *fChargedPionFakeElectronHOverE;
      TH1D *fChargedPionFakeElectronEOverP;
      TH1D *fChargedPionFakeElectronSigmaEtaEta;
      TH1D *fChargedPionFakeElectronSigmaiEtaiEta;
      TH1D *fChargedPionFakeElectronDeltaPhi;
      TH1D *fChargedPionFakeElectronDeltaEta;
      TH1D *fChargedPionFakeElectronfBrem;
      TH1D *fChargedPionFakeElectronClassification;      
      TH1D *fChargedKaonFakeElectronPt;
      TH1D *fChargedKaonFakeElectronEta;
      TH1D *fChargedKaonFakeElectronPhi;
      TH1D *fChargedKaonFakeElectronTrkIso;
      TH1D *fChargedKaonFakeElectronJurassicIso;
      TH1D *fChargedKaonFakeElectronHOverE;
      TH1D *fChargedKaonFakeElectronEOverP;
      TH1D *fChargedKaonFakeElectronSigmaEtaEta;
      TH1D *fChargedKaonFakeElectronSigmaiEtaiEta;
      TH1D *fChargedKaonFakeElectronDeltaPhi;
      TH1D *fChargedKaonFakeElectronDeltaEta;
      TH1D *fChargedKaonFakeElectronfBrem;
      TH1D *fChargedKaonFakeElectronClassification;      
      TH1D *fHeavyFlavorFakeElectronPt;
      TH1D *fHeavyFlavorFakeElectronEta;
      TH1D *fHeavyFlavorFakeElectronPhi;
      TH1D *fHeavyFlavorFakeElectronTrkIso;
      TH1D *fHeavyFlavorFakeElectronJurassicIso;
      TH1D *fHeavyFlavorFakeElectronHOverE;
      TH1D *fHeavyFlavorFakeElectronEOverP;
      TH1D *fHeavyFlavorFakeElectronSigmaEtaEta;
      TH1D *fHeavyFlavorFakeElectronSigmaiEtaiEta;
      TH1D *fHeavyFlavorFakeElectronDeltaPhi;
      TH1D *fHeavyFlavorFakeElectronDeltaEta;
      TH1D *fHeavyFlavorFakeElectronfBrem;
      TH1D *fHeavyFlavorFakeElectronClassification;      
      TH1D *fBremFakeElectronPt;
      TH1D *fBremFakeElectronEta;
      TH1D *fBremFakeElectronPhi;
      TH1D *fBremFakeElectronTrkIso;
      TH1D *fBremFakeElectronJurassicIso;
      TH1D *fBremFakeElectronHOverE;
      TH1D *fBremFakeElectronEOverP;
      TH1D *fBremFakeElectronSigmaEtaEta;
      TH1D *fBremFakeElectronSigmaiEtaiEta;
      TH1D *fBremFakeElectronDeltaPhi;
      TH1D *fBremFakeElectronDeltaEta;
      TH1D *fBremFakeElectronfBrem;
      TH1D *fBremFakeElectronClassification;      
      TH1D *fPromptPhotonFakeElectronPt;
      TH1D *fPromptPhotonFakeElectronEta;
      TH1D *fPromptPhotonFakeElectronPhi;
      TH1D *fPromptPhotonFakeElectronTrkIso;
      TH1D *fPromptPhotonFakeElectronJurassicIso;
      TH1D *fPromptPhotonFakeElectronHOverE;
      TH1D *fPromptPhotonFakeElectronEOverP;
      TH1D *fPromptPhotonFakeElectronSigmaEtaEta;
      TH1D *fPromptPhotonFakeElectronSigmaiEtaiEta;
      TH1D *fPromptPhotonFakeElectronDeltaPhi;
      TH1D *fPromptPhotonFakeElectronDeltaEta;
      TH1D *fPromptPhotonFakeElectronfBrem;
      TH1D *fPromptPhotonFakeElectronClassification;
      TH1D *fUncategorizedFakeElectronPt;
      TH1D *fUncategorizedFakeElectronEta;
      TH1D *fUncategorizedFakeElectronPhi;
      TH1D *fUncategorizedFakeElectronTrkIso;
      TH1D *fUncategorizedFakeElectronJurassicIso;
      TH1D *fUncategorizedFakeElectronHOverE;
      TH1D *fUncategorizedFakeElectronEOverP;
      TH1D *fUncategorizedFakeElectronSigmaEtaEta;
      TH1D *fUncategorizedFakeElectronSigmaiEtaiEta;
      TH1D *fUncategorizedFakeElectronDeltaPhi;
      TH1D *fUncategorizedFakeElectronDeltaEta;
      TH1D *fUncategorizedFakeElectronfBrem;
      TH1D *fUncategorizedFakeElectronClassification;
      
      TH1D *fDileptonChargeConversionFakes;         
      TH1D *fDileptonChargeChargedPionFakes;
      TH1D *fDileptonChargeChargedKaonFakes;         
      TH1D *fDileptonChargeHeavyFlavorFakes;         
      TH1D *fDileptonChargeBremFakes;         
      TH1D *fDileptonChargePromptPhotonFakes;
      TH1D *fDileptonChargeUncategorizedFakes;
      TH1D *fDileptonChargeNoSimMatchFakes;    
  
      TH1D *fSameSignConversionCategorySimParticleToIsolatedTrackChargeMatch;
      TH1D *fSameSignConversionCategorySimParticleToElectronChargeMatch;
      TH1D *fSameSignConversionCategorySimParticleToIsolatedElectronChargeMatch;
      TH1D *fSameSignConversionCategorySimParticleToIsolatedLooseElectronChargeMatch;
      TH1D *fSameSignConversionCategorySimParticleToIsolatedTightElectronChargeMatch;
      TH1D *fSameSignConversionCategorySimParticleToCleanElectronChargeMatch;
      TH1D *fOppositeSignConversionCategorySimParticleToIsolatedTrackChargeMatch;
      TH1D *fOppositeSignConversionCategorySimParticleToElectronChargeMatch;
      TH1D *fOppositeSignConversionCategorySimParticleToIsolatedElectronChargeMatch;
      TH1D *fOppositeSignConversionCategorySimParticleToIsolatedLooseElectronChargeMatch;
      TH1D *fOppositeSignConversionCategorySimParticleToIsolatedTightElectronChargeMatch;
      TH1D *fOppositeSignConversionCategorySimParticleToCleanElectronChargeMatch;

      TH1D *fPlusWJetFlavor;         
      TH1D *fPlusWTrackMatchedJetFlavor;         
      TH1D *fPlusWLooseIsolatedTrackMatchedJetFlavor;         
      TH1D *fPlusWTightIsolatedTrackMatchedJetFlavor;         
      TH1D *fPlusWRecoElectronMatchedJetFlavor;
      TH1D *fPlusWLooseElectronMatchedJetFlavor;
      TH1D *fPlusWTightElectronMatchedJetFlavor;
      TH1D *fPlusWCleanElectronMatchedJetFlavor;
      TH1D *fMinusWJetFlavor;
      TH1D *fMinusWTrackMatchedJetFlavor;
      TH1D *fMinusWLooseIsolatedTrackMatchedJetFlavor;
      TH1D *fMinusWTightIsolatedTrackMatchedJetFlavor;
      TH1D *fMinusWRecoElectronMatchedJetFlavor;
      TH1D *fMinusWLooseElectronMatchedJetFlavor;
      TH1D *fMinusWTightElectronMatchedJetFlavor;
      TH1D *fMinusWCleanElectronMatchedJetFlavor;

      TH1D *fSameSignIsolatedTrackTrkIso;
      TH1D *fSameSignIsolatedTrackD0;
      TH1D *fSameSignIsolatedTrackEta;
      TH1D *fSameSignIsolatedTrackPhi;
      TH1D *fSameSignIsolatedTrackPt;
      TH1D *fSameSignRealToIsolatedTrackDeltaEta;
      TH1D *fSameSignRealToIsolatedTrackDeltaPhi;
      TH1D *fSameSignRealToIsolatedTrackDeltaR;
      TH1D *fOppositeSignIsolatedTrackTrkIso;
      TH1D *fOppositeSignIsolatedTrackD0;
      TH1D *fOppositeSignIsolatedTrackEta;
      TH1D *fOppositeSignIsolatedTrackPhi;
      TH1D *fOppositeSignIsolatedTrackPt;
      TH1D *fOppositeSignRealToIsolatedTrackDeltaEta;
      TH1D *fOppositeSignRealToIsolatedTrackDeltaPhi;
      TH1D *fOppositeSignRealToIsolatedTrackDeltaR;
      TH1D *fSameSignFakeElectronHOverE;
      TH1D *fSameSignFakeElectronESuperClusterOverP;
      TH1D *fSameSignFakeElectronSigmaEtaEta;
      TH1D *fSameSignFakeElectronSigmaiEtaiEta;
      TH1D *fSameSignFakeElectronDeltaPhi;
      TH1D *fSameSignFakeElectronDeltaEta;
      TH1D *fSameSignFakeElectronfBrem;
      TH1D *fSameSignFakeElectronClassification;
      TH1D *fSameSignFakeElectronTrkIso;
      TH1D *fSameSignFakeElectronJurassicIso;
      TH1D *fSameSignFakeElectronD0;
      TH1D *fSameSignFakeElectronEta;
      TH1D *fSameSignFakeElectronPhi;
      TH1D *fSameSignFakeElectronPt;
      TH1D *fSameSignRealToFakeElectronDeltaEta;
      TH1D *fSameSignRealToFakeElectronDeltaPhi;
      TH1D *fSameSignRealToFakeElectronDeltaR;
      TH1D *fOppositeSignFakeElectronHOverE;
      TH1D *fOppositeSignFakeElectronESuperClusterOverP;
      TH1D *fOppositeSignFakeElectronSigmaEtaEta;
      TH1D *fOppositeSignFakeElectronSigmaiEtaiEta;
      TH1D *fOppositeSignFakeElectronDeltaPhi;
      TH1D *fOppositeSignFakeElectronDeltaEta;
      TH1D *fOppositeSignFakeElectronfBrem;
      TH1D *fOppositeSignFakeElectronClassification;
      TH1D *fOppositeSignFakeElectronTrkIso;
      TH1D *fOppositeSignFakeElectronJurassicIso;
      TH1D *fOppositeSignFakeElectronD0;
      TH1D *fOppositeSignFakeElectronEta;
      TH1D *fOppositeSignFakeElectronPhi;
      TH1D *fOppositeSignFakeElectronPt;
      TH1D *fOppositeSignRealToFakeElectronDeltaEta;
      TH1D *fOppositeSignRealToFakeElectronDeltaPhi;
      TH1D *fOppositeSignRealToFakeElectronDeltaR;
      TH1D *fSameSignFakeIsolatedElectronHOverE;
      TH1D *fSameSignFakeIsolatedElectronESuperClusterOverP;
      TH1D *fSameSignFakeIsolatedElectronSigmaEtaEta;
      TH1D *fSameSignFakeIsolatedElectronSigmaiEtaiEta;
      TH1D *fSameSignFakeIsolatedElectronDeltaPhi;
      TH1D *fSameSignFakeIsolatedElectronDeltaEta;
      TH1D *fSameSignFakeIsolatedElectronfBrem;
      TH1D *fSameSignFakeIsolatedElectronClassification;
      TH1D *fSameSignFakeIsolatedElectronTrkIso;
      TH1D *fSameSignFakeIsolatedElectronJurassicIso;
      TH1D *fSameSignFakeIsolatedElectronD0;
      TH1D *fSameSignFakeIsolatedElectronEta;
      TH1D *fSameSignFakeIsolatedElectronPhi;
      TH1D *fSameSignFakeIsolatedElectronPt;
      TH1D *fSameSignRealToFakeIsolatedElectronDeltaEta;
      TH1D *fSameSignRealToFakeIsolatedElectronDeltaPhi;
      TH1D *fSameSignRealToFakeIsolatedElectronDeltaR;
      TH1D *fOppositeSignFakeIsolatedElectronHOverE;
      TH1D *fOppositeSignFakeIsolatedElectronESuperClusterOverP;
      TH1D *fOppositeSignFakeIsolatedElectronSigmaEtaEta;
      TH1D *fOppositeSignFakeIsolatedElectronSigmaiEtaiEta;
      TH1D *fOppositeSignFakeIsolatedElectronDeltaPhi;
      TH1D *fOppositeSignFakeIsolatedElectronDeltaEta;
      TH1D *fOppositeSignFakeIsolatedElectronfBrem;
      TH1D *fOppositeSignFakeIsolatedElectronClassification;
      TH1D *fOppositeSignFakeIsolatedElectronTrkIso;
      TH1D *fOppositeSignFakeIsolatedElectronJurassicIso;
      TH1D *fOppositeSignFakeIsolatedElectronD0;
      TH1D *fOppositeSignFakeIsolatedElectronEta;
      TH1D *fOppositeSignFakeIsolatedElectronPhi;
      TH1D *fOppositeSignFakeIsolatedElectronPt;
      TH1D *fOppositeSignRealToFakeIsolatedElectronDeltaEta;
      TH1D *fOppositeSignRealToFakeIsolatedElectronDeltaPhi;
      TH1D *fOppositeSignRealToFakeIsolatedElectronDeltaR;
      TH1D *fSameSignFakeIsolatedLooseElectronHOverE;
      TH1D *fSameSignFakeIsolatedLooseElectronESuperClusterOverP;
      TH1D *fSameSignFakeIsolatedLooseElectronSigmaEtaEta;
      TH1D *fSameSignFakeIsolatedLooseElectronSigmaiEtaiEta;
      TH1D *fSameSignFakeIsolatedLooseElectronDeltaPhi;
      TH1D *fSameSignFakeIsolatedLooseElectronDeltaEta;
      TH1D *fSameSignFakeIsolatedLooseElectronfBrem;
      TH1D *fSameSignFakeIsolatedLooseElectronClassification;
      TH1D *fSameSignFakeIsolatedLooseElectronTrkIso;
      TH1D *fSameSignFakeIsolatedLooseElectronJurassicIso;
      TH1D *fSameSignFakeIsolatedLooseElectronD0;
      TH1D *fSameSignFakeIsolatedLooseElectronEta;
      TH1D *fSameSignFakeIsolatedLooseElectronPhi;
      TH1D *fSameSignFakeIsolatedLooseElectronPt;
      TH1D *fSameSignRealToFakeIsolatedLooseElectronDeltaEta;
      TH1D *fSameSignRealToFakeIsolatedLooseElectronDeltaPhi;
      TH1D *fSameSignRealToFakeIsolatedLooseElectronDeltaR;
      TH1D *fOppositeSignFakeIsolatedLooseElectronHOverE;
      TH1D *fOppositeSignFakeIsolatedLooseElectronESuperClusterOverP;
      TH1D *fOppositeSignFakeIsolatedLooseElectronSigmaEtaEta;
      TH1D *fOppositeSignFakeIsolatedLooseElectronSigmaiEtaiEta;
      TH1D *fOppositeSignFakeIsolatedLooseElectronDeltaPhi;
      TH1D *fOppositeSignFakeIsolatedLooseElectronDeltaEta;
      TH1D *fOppositeSignFakeIsolatedLooseElectronfBrem;
      TH1D *fOppositeSignFakeIsolatedLooseElectronClassification;
      TH1D *fOppositeSignFakeIsolatedLooseElectronTrkIso;
      TH1D *fOppositeSignFakeIsolatedLooseElectronJurassicIso;
      TH1D *fOppositeSignFakeIsolatedLooseElectronD0;
      TH1D *fOppositeSignFakeIsolatedLooseElectronEta;
      TH1D *fOppositeSignFakeIsolatedLooseElectronPhi;
      TH1D *fOppositeSignFakeIsolatedLooseElectronPt;
      TH1D *fOppositeSignRealToFakeIsolatedLooseElectronDeltaEta;
      TH1D *fOppositeSignRealToFakeIsolatedLooseElectronDeltaPhi;
      TH1D *fOppositeSignRealToFakeIsolatedLooseElectronDeltaR;
      TH1D *fSameSignFakeIsolatedTightElectronHOverE;
      TH1D *fSameSignFakeIsolatedTightElectronESuperClusterOverP;
      TH1D *fSameSignFakeIsolatedTightElectronSigmaEtaEta;
      TH1D *fSameSignFakeIsolatedTightElectronSigmaiEtaiEta;
      TH1D *fSameSignFakeIsolatedTightElectronDeltaPhi;
      TH1D *fSameSignFakeIsolatedTightElectronDeltaEta;
      TH1D *fSameSignFakeIsolatedTightElectronfBrem;
      TH1D *fSameSignFakeIsolatedTightElectronClassification;
      TH1D *fSameSignFakeIsolatedTightElectronTrkIso;
      TH1D *fSameSignFakeIsolatedTightElectronJurassicIso;
      TH1D *fSameSignFakeIsolatedTightElectronD0;
      TH1D *fSameSignFakeIsolatedTightElectronEta;
      TH1D *fSameSignFakeIsolatedTightElectronPhi;
      TH1D *fSameSignFakeIsolatedTightElectronPt;
      TH1D *fSameSignRealToFakeIsolatedTightElectronDeltaEta;
      TH1D *fSameSignRealToFakeIsolatedTightElectronDeltaPhi;
      TH1D *fSameSignRealToFakeIsolatedTightElectronDeltaR;
      TH1D *fOppositeSignFakeIsolatedTightElectronHOverE;
      TH1D *fOppositeSignFakeIsolatedTightElectronESuperClusterOverP;
      TH1D *fOppositeSignFakeIsolatedTightElectronSigmaEtaEta;
      TH1D *fOppositeSignFakeIsolatedTightElectronSigmaiEtaiEta;
      TH1D *fOppositeSignFakeIsolatedTightElectronDeltaPhi;
      TH1D *fOppositeSignFakeIsolatedTightElectronDeltaEta;
      TH1D *fOppositeSignFakeIsolatedTightElectronfBrem;
      TH1D *fOppositeSignFakeIsolatedTightElectronClassification;
      TH1D *fOppositeSignFakeIsolatedTightElectronTrkIso;
      TH1D *fOppositeSignFakeIsolatedTightElectronJurassicIso;
      TH1D *fOppositeSignFakeIsolatedTightElectronD0;
      TH1D *fOppositeSignFakeIsolatedTightElectronEta;
      TH1D *fOppositeSignFakeIsolatedTightElectronPhi;
      TH1D *fOppositeSignFakeIsolatedTightElectronPt;
      TH1D *fOppositeSignRealToFakeIsolatedTightElectronDeltaEta;
      TH1D *fOppositeSignRealToFakeIsolatedTightElectronDeltaPhi;
      TH1D *fOppositeSignRealToFakeIsolatedTightElectronDeltaR; 
      TH1D *fSameSignFakeCleanElectronHOverE;
      TH1D *fSameSignFakeCleanElectronESuperClusterOverP;
      TH1D *fSameSignFakeCleanElectronSigmaEtaEta;
      TH1D *fSameSignFakeCleanElectronSigmaiEtaiEta;
      TH1D *fSameSignFakeCleanElectronDeltaPhi;
      TH1D *fSameSignFakeCleanElectronDeltaEta;
      TH1D *fSameSignFakeCleanElectronfBrem;
      TH1D *fSameSignFakeCleanElectronClassification;
      TH1D *fSameSignFakeCleanElectronTrkIso;
      TH1D *fSameSignFakeCleanElectronJurassicIso;
      TH1D *fSameSignFakeCleanElectronD0;
      TH1D *fSameSignFakeCleanElectronEta;
      TH1D *fSameSignFakeCleanElectronPhi;
      TH1D *fSameSignFakeCleanElectronPt;
      TH1D *fSameSignRealToFakeCleanElectronDeltaEta;
      TH1D *fSameSignRealToFakeCleanElectronDeltaPhi;
      TH1D *fSameSignRealToFakeCleanElectronDeltaR;
      TH1D *fOppositeSignFakeCleanElectronHOverE;
      TH1D *fOppositeSignFakeCleanElectronESuperClusterOverP;
      TH1D *fOppositeSignFakeCleanElectronSigmaEtaEta;
      TH1D *fOppositeSignFakeCleanElectronSigmaiEtaiEta;
      TH1D *fOppositeSignFakeCleanElectronDeltaPhi;
      TH1D *fOppositeSignFakeCleanElectronDeltaEta;
      TH1D *fOppositeSignFakeCleanElectronfBrem;
      TH1D *fOppositeSignFakeCleanElectronClassification;
      TH1D *fOppositeSignFakeCleanElectronTrkIso;
      TH1D *fOppositeSignFakeCleanElectronJurassicIso;
      TH1D *fOppositeSignFakeCleanElectronD0;
      TH1D *fOppositeSignFakeCleanElectronEta;
      TH1D *fOppositeSignFakeCleanElectronPhi;
      TH1D *fOppositeSignFakeCleanElectronPt;
      TH1D *fOppositeSignRealToFakeCleanElectronDeltaEta;
      TH1D *fOppositeSignRealToFakeCleanElectronDeltaPhi;
      TH1D *fOppositeSignRealToFakeCleanElectronDeltaR;
         
      TH1D *fSameSignIsolatedTrackCategory;
      TH1D *fOppositeSignIsolatedTrackCategory;
      TH1D *fSameSignFakeElectronCategory;
      TH1D *fOppositeSignFakeElectronCategory;
      TH1D *fSameSignFakeIsolatedElectronCategory;
      TH1D *fOppositeSignFakeIsolatedElectronCategory;
      TH1D *fSameSignFakeIsolatedLooseElectronCategory;
      TH1D *fOppositeSignFakeIsolatedLooseElectronCategory;
      TH1D *fSameSignFakeIsolatedTightElectronCategory;
      TH1D *fOppositeSignFakeIsolatedTightElectronCategory;
      TH1D *fSameSignFakeCleanElectronCategory;
      TH1D *fOppositeSignFakeCleanElectronCategory;

      void                     Begin();
      void                     Process();
      void                     SlaveBegin();
      void                     SlaveTerminate();
      void                     Terminate();

      int count;

      ClassDef(FakeElectronAnalysisMod,1) // TAM example analysis module
  };
}
#endif
