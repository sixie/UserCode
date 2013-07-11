//--------------------------------------------------------------------------------------------------
// $Id: FakeMuonAnalysisMod.h,v 1.2 2009/03/17 08:33:15 sixie Exp $
//
// FakeMuonAnalysisMod
//
// This Module performs cleaning of objects including, removal of
// duplicate objects, applying object ID cuts, correction of MET
// The output will be collections of objects deemed as "good" and
// exported to other modules.
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef FAKERATEMODS_FAKEMUONANALYSISMOD_H
#define FAKERATEMODS_FAKEMUONANALYSISMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;
class TH3D;

namespace mithep 
{
  class FakeMuonAnalysisMod : public BaseMod
  {
    public:
      FakeMuonAnalysisMod(const char *name="FakeMuonAnalysisMod", 
                     const char *title="Example analysis module with all branches");
      ~FakeMuonAnalysisMod() {}

      const Bool_t GetVetoGeneratorLeptons()         const { return fVetoGeneratorLeptons;         }
      const Bool_t GetRequireFirstLepton()           const { return fRequireFirstLepton;           }
      const Bool_t GetPrintLogs()                    const { return fPrintLogs;                    }
      const char *GetMCPartBranchName()              const { return fMCPartBranchName;             }
      const char *GetGenJetBranchName()              const { return fGenJetBranchName;             }
      const char *GetMuonsBranchName()               const { return fMuonBranchName;               }
      const char *GetTrackBranchName()               const { return fTrackBranchName;              }
      const char *GetJetBranchName()                 const { return fJetBranchName;                }
      const char *GetCleanMuonsName()                const { return fCleanMuonsName;               }
      const char *GetGoodJetsName()                  const { return fGoodJetsName;                 }
      const char *GetMCLeptonsName()                 const { return fMCLeptonsName;                }
      const char *GetMCTausName()                    const { return fMCTausName;                   }

      void SetVetoGeneratorLeptons(Bool_t b)                 { fVetoGeneratorLeptons       = b;    }
      void SetRequireFirstLepton(Bool_t b)                   { fRequireFirstLepton         = b;    }
      void SetPrintLogs(Bool_t b)                            { fPrintLogs                  = b;    }
      void SetMCPartBranchName(const char *name)             { fMCPartBranchName           = name; }
      void SetGenJetBranchName(const char *name)             { fGenJetBranchName           = name; }
      void SetMuonBranchName(const char *name)               { fMuonBranchName             = name; }
      void SetTrackBranchName(const char *name)              { fTrackBranchName            = name; }
      void SetJetBranchName(const char *name)                { fJetBranchName              = name; }
      void SetCleanMuonsName(const char *name)               { fCleanMuonsName             = name; }
      void SetGoodJetsName(const char *name)                 { fGoodJetsName               = name; }
      void SetMCLeptonsName(const char *name)                { fMCLeptonsName              = name; }
      void SetMCTausName(const char *name)                   { fMCTausName                 = name; }
 
    protected:
      Bool_t           fVetoGeneratorLeptons;         //whether to veto on gen electrons
      Bool_t           fRequireFirstLepton;           //whether we require another real lepton
      Bool_t           fPrintLogs;                    //whether we print the logs
      TString          fMCPartBranchName;             //name of particle collection
      TString          fGenJetBranchName;             //name of genjet collection
      TString          fTrackBranchName;              //name of track collection
      TString          fMuonBranchName;               //name of electron collection
      TString          fJetBranchName;                //name of jet collection
      TString          fCleanMuonsName;               //name of clean electrons             (input)
      TString          fGoodJetsName;                 //name of Good jets collection
      TString          fMCLeptonsName;                //name of MC leptons
      TString          fMCTausName;                   //name of MC taus

      const MCParticleCol   *fParticles;              //!GenParticle branch
      const GenJetCol       *fGenJets;                //!GenJet branch
      const TrackCol	    *fTracks;	              //!Track branch
      const MuonCol	    *fMuons;                  //!Muon branch
      const JetCol          *fJets;                   //!Jet branch            

      TH1D *fCleanCentralMuonTrackerGlobalTrackChargeMatch;
      TH1D *fCleanForwardMuonTrackerGlobalTrackChargeMatch;

      TH1D *fCleanPlusCentralMuonMatchedJetFlavor;
      TH1D *fCleanMinusCentralMuonMatchedJetFlavor;      
      TH1D *fCleanPlusCentralMuonMatchedGenJetPt;
      TH1D *fCleanMinusCentralMuonMatchedGenJetPt;
      TH1D *fCleanPlusCentralMuonMatchedGenJetDeltaR;
      TH1D *fCleanMinusCentralMuonMatchedGenJetDeltaR;
      TH1D *fCleanPlusCentralMuonMatchedGenJetDeltaPtOverPt;
      TH1D *fCleanMinusCentralMuonMatchedGenJetDeltaPtOverPt;
      TH1D *fCleanPlusForwardMuonMatchedJetFlavor;
      TH1D *fCleanMinusForwardMuonMatchedJetFlavor;      
      TH1D *fCleanPlusForwardMuonMatchedGenJetPt;
      TH1D *fCleanMinusForwardMuonMatchedGenJetPt;
      TH1D *fCleanPlusForwardMuonMatchedGenJetDeltaR;
      TH1D *fCleanMinusForwardMuonMatchedGenJetDeltaR;
      TH1D *fCleanPlusForwardMuonMatchedGenJetDeltaPtOverPt;
      TH1D *fCleanMinusForwardMuonMatchedGenJetDeltaPtOverPt;
      
      TH1D *fCleanPlusCentralMuonMatchedGenParticleID;
      TH1D *fCleanMinusCentralMuonMatchedGenParticleID;
      TH1D *fCleanPlusCentralMuonMatchedGenParticlePt;
      TH1D *fCleanMinusCentralMuonMatchedGenParticlePt;
      TH1D *fCleanPlusCentralMuonMatchedGenParticleDeltaR;
      TH1D *fCleanMinusCentralMuonMatchedGenParticleDeltaR;
      TH1D *fCleanPlusCentralMuonMatchedGenParticleDeltaPtOverPt;
      TH1D *fCleanMinusCentralMuonMatchedGenParticleDeltaPtOverPt;
      TH1D *fCleanPlusForwardMuonMatchedGenParticleID;
      TH1D *fCleanMinusForwardMuonMatchedGenParticleID;
      TH1D *fCleanPlusForwardMuonMatchedGenParticlePt;
      TH1D *fCleanMinusForwardMuonMatchedGenParticlePt;
      TH1D *fCleanPlusForwardMuonMatchedGenParticleDeltaR;
      TH1D *fCleanMinusForwardMuonMatchedGenParticleDeltaR;
      TH1D *fCleanPlusForwardMuonMatchedGenParticleDeltaPtOverPt;
      TH1D *fCleanMinusForwardMuonMatchedGenParticleDeltaPtOverPt;

      TH1D *fCleanPlusCentralMuonMatchedSimChargedParticleID;
      TH1D *fCleanMinusCentralMuonMatchedSimChargedParticleID;
      TH1D *fCleanPlusCentralMuonMatchedSimChargedParticlePt;
      TH1D *fCleanMinusCentralMuonMatchedSimChargedParticlePt;
      TH1D *fCleanPlusCentralMuonMatchedSimChargedParticleDeltaR;
      TH1D *fCleanMinusCentralMuonMatchedSimChargedParticleDeltaR;
      TH1D *fCleanPlusCentralMuonMatchedSimChargedParticleDeltaPtOverPt;
      TH1D *fCleanMinusCentralMuonMatchedSimChargedParticleDeltaPtOverPt;
      TH1D *fCleanPlusForwardMuonMatchedSimChargedParticleID;
      TH1D *fCleanMinusForwardMuonMatchedSimChargedParticleID;
      TH1D *fCleanPlusForwardMuonMatchedSimChargedParticlePt;
      TH1D *fCleanMinusForwardMuonMatchedSimChargedParticlePt;
      TH1D *fCleanPlusForwardMuonMatchedSimChargedParticleDeltaR;
      TH1D *fCleanMinusForwardMuonMatchedSimChargedParticleDeltaR;
      TH1D *fCleanPlusForwardMuonMatchedSimChargedParticleDeltaPtOverPt;
      TH1D *fCleanMinusForwardMuonMatchedSimChargedParticleDeltaPtOverPt;

      TH1D *fCleanPlusCentralMuonMatchedAncestorParticleID;
      TH1D *fCleanMinusCentralMuonMatchedAncestorParticleID;
      TH1D *fCleanPlusCentralMuonMatchedAncestorParticlePt;
      TH1D *fCleanMinusCentralMuonMatchedAncestorParticlePt;
      TH1D *fCleanPlusCentralMuonMatchedAncestorParticleDeltaR;
      TH1D *fCleanMinusCentralMuonMatchedAncestorParticleDeltaR;
      TH1D *fCleanPlusCentralMuonMatchedAncestorParticleDeltaPtOverPt;
      TH1D *fCleanMinusCentralMuonMatchedAncestorParticleDeltaPtOverPt;
      TH1D *fCleanPlusForwardMuonMatchedAncestorParticleID;
      TH1D *fCleanMinusForwardMuonMatchedAncestorParticleID;
      TH1D *fCleanPlusForwardMuonMatchedAncestorParticlePt;
      TH1D *fCleanMinusForwardMuonMatchedAncestorParticlePt;
      TH1D *fCleanPlusForwardMuonMatchedAncestorParticleDeltaR;
      TH1D *fCleanMinusForwardMuonMatchedAncestorParticleDeltaR;
      TH1D *fCleanPlusForwardMuonMatchedAncestorParticleDeltaPtOverPt;
      TH1D *fCleanMinusForwardMuonMatchedAncestorParticleDeltaPtOverPt;

      TH1D *fCleanPlusCentralMuonUnmatchedSimChargedParticleBestDeltaPtOverPt;
      TH1D *fCleanMinusCentralMuonUnmatchedSimChargedParticleBestDeltaPtOverPt;
      TH1D *fCleanPlusForwardMuonUnmatchedSimChargedParticleBestDeltaPtOverPt;
      TH1D *fCleanMinusForwardMuonUnmatchedSimChargedParticleBestDeltaPtOverPt;

      void                     Begin();
      void                     Process();
      void                     SlaveBegin();
      void                     SlaveTerminate();
      void                     Terminate();

      ClassDef(FakeMuonAnalysisMod,1) // TAM example analysis module
  };
}
#endif
