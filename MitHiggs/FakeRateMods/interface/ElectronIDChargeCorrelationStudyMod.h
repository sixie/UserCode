//--------------------------------------------------------------------------------------------------
// $Id: ElectronIDChargeCorrelationStudyMod.h,v 1.3 2009/03/17 08:33:15 sixie Exp $
//
// ElectronIDChargeCorrelationStudyMod
//
// This Module performs cleaning of objects including, removal of
// duplicate objects, applying object ID cuts, correction of MET
// The output will be collections of objects deemed as "good" and
// exported to other modules.
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef FAKERATEMODS_ELECTRONIDCHARGECORRELATIONSTUDYMOD_H
#define FAKERATEMODS_ELECTRONIDCHARGECORRELATIONSTUDYMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;
class TH3D;

namespace mithep 
{
  class ElectronIDChargeCorrelationStudyMod : public BaseMod
  {
    public:
      ElectronIDChargeCorrelationStudyMod(const char *name="ElectronIDChargeCorrelationStudyMod", 
                     const char *title="Example analysis module with all branches");
      ~ElectronIDChargeCorrelationStudyMod() {}

      Bool_t      GetVetoGeneratorLeptons()          const { return fVetoGeneratorLeptons;         }
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
      const GenJetCol         *fGenJets;              //!GenJet branch
      const SuperClusterCol *fBarrelSuperClusters;    //!Barrel SC branch
      const SuperClusterCol *fEndcapSuperClusters;    //!Endcap SC branch
      const TrackCol	    *fTracks;	              //!Track branch
      const TrackCol	    *fGsfTracks;	      //!GsfTrack branch
      const ElectronCol	    *fElectrons;              //!Electron branch
      const JetCol          *fJets;                   //!Jet branch            

      TH1D *fSameSignFakeElectronHOverE;
      TH1D *fSameSignFakeElectronESuperClusterOverP;
      TH1D *fSameSignFakeElectronSigmaEtaEta;
      TH1D *fSameSignFakeElectronSigmaiEtaiEta;
      TH1D *fSameSignFakeElectronDeltaPhi;
      TH1D *fSameSignFakeElectronDeltaEta;
      TH1D *fSameSignFakeElectronfBrem;
      TH1D *fSameSignFakeElectronClassification;
      TH1D *fSameSignFakeElectronEta;
      TH1D *fSameSignFakeElectronPhi;
      TH1D *fOppositeSignFakeElectronHOverE;
      TH1D *fOppositeSignFakeElectronESuperClusterOverP;
      TH1D *fOppositeSignFakeElectronSigmaEtaEta;
      TH1D *fOppositeSignFakeElectronSigmaiEtaiEta;
      TH1D *fOppositeSignFakeElectronDeltaPhi;
      TH1D *fOppositeSignFakeElectronDeltaEta;
      TH1D *fOppositeSignFakeElectronfBrem;
      TH1D *fOppositeSignFakeElectronClassification;
      TH1D *fOppositeSignFakeElectronEta;
      TH1D *fOppositeSignFakeElectronPhi;

      TH1D *fSameSignFakeIsolatedElectronHOverE;
      TH1D *fSameSignFakeIsolatedElectronESuperClusterOverP;
      TH1D *fSameSignFakeIsolatedElectronSigmaEtaEta;
      TH1D *fSameSignFakeIsolatedElectronSigmaiEtaiEta;
      TH1D *fSameSignFakeIsolatedElectronDeltaPhi;
      TH1D *fSameSignFakeIsolatedElectronDeltaEta;
      TH1D *fSameSignFakeIsolatedElectronfBrem;
      TH1D *fSameSignFakeIsolatedElectronClassification;
      TH1D *fSameSignFakeIsolatedElectronEta;
      TH1D *fSameSignFakeIsolatedElectronPhi;
      TH1D *fOppositeSignFakeIsolatedElectronHOverE;
      TH1D *fOppositeSignFakeIsolatedElectronESuperClusterOverP;
      TH1D *fOppositeSignFakeIsolatedElectronSigmaEtaEta;
      TH1D *fOppositeSignFakeIsolatedElectronSigmaiEtaiEta;
      TH1D *fOppositeSignFakeIsolatedElectronDeltaPhi;
      TH1D *fOppositeSignFakeIsolatedElectronDeltaEta;
      TH1D *fOppositeSignFakeIsolatedElectronfBrem;
      TH1D *fOppositeSignFakeIsolatedElectronClassification;
      TH1D *fOppositeSignFakeIsolatedElectronEta;
      TH1D *fOppositeSignFakeIsolatedElectronPhi;

      TH1D *fSameSignFakeCleanElectronHOverE;
      TH1D *fSameSignFakeCleanElectronESuperClusterOverP;
      TH1D *fSameSignFakeCleanElectronSigmaEtaEta;
      TH1D *fSameSignFakeCleanElectronSigmaiEtaiEta;
      TH1D *fSameSignFakeCleanElectronDeltaPhi;
      TH1D *fSameSignFakeCleanElectronDeltaEta;
      TH1D *fSameSignFakeCleanElectronfBrem;
      TH1D *fSameSignFakeCleanElectronClassification;
      TH1D *fSameSignFakeCleanElectronEta;
      TH1D *fSameSignFakeCleanElectronPhi;
      TH1D *fOppositeSignFakeCleanElectronHOverE;
      TH1D *fOppositeSignFakeCleanElectronESuperClusterOverP;
      TH1D *fOppositeSignFakeCleanElectronSigmaEtaEta;
      TH1D *fOppositeSignFakeCleanElectronSigmaiEtaiEta;
      TH1D *fOppositeSignFakeCleanElectronDeltaPhi;
      TH1D *fOppositeSignFakeCleanElectronDeltaEta;
      TH1D *fOppositeSignFakeCleanElectronfBrem;
      TH1D *fOppositeSignFakeCleanElectronClassification;
      TH1D *fOppositeSignFakeCleanElectronEta;
      TH1D *fOppositeSignFakeCleanElectronPhi;

      void                     Begin();
      void                     Process();
      void                     SlaveBegin();
      void                     SlaveTerminate();
      void                     Terminate();

      ClassDef(ElectronIDChargeCorrelationStudyMod,1) // TAM example analysis module
  };
}
#endif
