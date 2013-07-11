//--------------------------------------------------------------------------------------------------
// $Id: WJetsFakeElectronAnalysisMod.h,v 1.3 2009/03/17 08:33:15 sixie Exp $
//
// WJetsFakeElectronAnalysisMod
//
// This Module performs cleaning of objects including, removal of
// duplicate objects, applying object ID cuts, correction of MET
// The output will be collections of objects deemed as "good" and
// exported to other modules.
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef FAKERATEMODS_WJETSFAKEELECTRONANALYSISMOD_H
#define FAKERATEMODS_WJETSFAKEELECTRONANALYSISMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;
class TH3D;

namespace mithep 
{
  class WJetsFakeElectronAnalysisMod : public BaseMod
  {
    public:
      WJetsFakeElectronAnalysisMod(const char *name="WJetsFakeElectronAnalysisMod", 
                     const char *title="Example analysis module with all branches");
      ~WJetsFakeElectronAnalysisMod() {}

      const Bool_t GetVetoGeneratorLeptons()         const { return fVetoGeneratorLeptons;         }
      const Bool_t GetRequireFirstLepton()           const { return fRequireFirstLepton;           }
      const Bool_t GetPrintLogs()                    const { return fPrintLogs;                    }
      const char *GetMCPartBranchName()              const { return fMCPartBranchName;             }
      const char *GetGenJetBranchName()              const { return fGenJetBranchName;             }
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
      const GenJetCol       *fGenJets;              //!GenJet branch
      const SuperClusterCol *fBarrelSuperClusters;    //!Barrel SC branch
      const SuperClusterCol *fEndcapSuperClusters;    //!Endcap SC branch      
      const TrackCol	    *fTracks;	              //!Track branch
      const TrackCol	    *fGsfTracks;	      //!GsfTrack branch
      const ElectronCol	    *fElectrons;              //!Electron branch
      const JetCol          *fJets;                   //!Jet branch            


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


      void                     Begin();
      void                     Process();
      void                     SlaveBegin();
      void                     SlaveTerminate();
      void                     Terminate();

      int count;

      ClassDef(WJetsFakeElectronAnalysisMod,1) // TAM example analysis module
  };
}
#endif
