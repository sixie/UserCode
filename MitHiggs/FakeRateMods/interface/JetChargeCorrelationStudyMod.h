//--------------------------------------------------------------------------------------------------
// $Id: JetChargeCorrelationStudyMod.h,v 1.3 2009/03/17 08:33:15 sixie Exp $
//
// JetChargeCorrelationStudyMod
//
// This Module performs cleaning of objects including, removal of
// duplicate objects, applying object ID cuts, correction of MET
// The output will be collections of objects deemed as "good" and
// exported to other modules.
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef FAKERATEMODS_JETCHARGECORRELATIONSTUDYMOD_H
#define FAKERATEMODS_JETCHARGECORRELATIONSTUDYMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;
class TH3D;

namespace mithep 
{
  class JetChargeCorrelationStudyMod : public BaseMod
  {
    public:
      JetChargeCorrelationStudyMod(const char *name="JetChargeCorrelationStudyMod", 
                     const char *title="Example analysis module with all branches");
      ~JetChargeCorrelationStudyMod() {}

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

      TH1D   *fUpJetFlavorMatchedTrackCharge;
      TH1D   *fUpJetFlavorMatchedLooseIsolatedTrackCharge;
      TH1D   *fUpJetFlavorMatchedTightIsolatedTrackCharge;
      TH1D   *fUpJetFlavorMatchedRecoElectronCharge;
      TH1D   *fUpJetFlavorMatchedLooseElectronCharge;
      TH1D   *fUpJetFlavorMatchedTightElectronCharge;
      TH1D   *fUpJetFlavorMatchedCleanElectronCharge;
      TH1D   *fDownJetFlavorMatchedTrackCharge;
      TH1D   *fDownJetFlavorMatchedLooseIsolatedTrackCharge;
      TH1D   *fDownJetFlavorMatchedTightIsolatedTrackCharge;
      TH1D   *fDownJetFlavorMatchedRecoElectronCharge;
      TH1D   *fDownJetFlavorMatchedLooseElectronCharge;
      TH1D   *fDownJetFlavorMatchedTightElectronCharge;
      TH1D   *fDownJetFlavorMatchedCleanElectronCharge;

      TH1D   *fGluonJetMatchedTrackCharge;
      TH1D   *fGluonJetMatchedLooseIsolatedTrackCharge;
      TH1D   *fGluonJetMatchedTightIsolatedTrackCharge;
      TH1D   *fGluonJetMatchedRecoElectronCharge;
      TH1D   *fGluonJetMatchedLooseElectronCharge;
      TH1D   *fGluonJetMatchedTightElectronCharge;
      TH1D   *fGluonJetMatchedCleanElectronCharge;

      TH1D   *fNGenJets;
      TH1D   *fPlusWJetFlavor;
      TH1D   *fPlusWTrackMatchedJetFlavor;
      TH1D   *fPlusWLooseIsolatedTrackMatchedJetFlavor;
      TH1D   *fPlusWTightIsolatedTrackMatchedJetFlavor;
      TH1D   *fPlusWRecoElectronMatchedJetFlavor;
      TH1D   *fPlusWLooseElectronMatchedJetFlavor;
      TH1D   *fPlusWTightElectronMatchedJetFlavor;
      TH1D   *fPlusWCleanElectronMatchedJetFlavor;
      TH1D   *fMinusWJetFlavor;
      TH1D   *fMinusWTrackMatchedJetFlavor;
      TH1D   *fMinusWLooseIsolatedTrackMatchedJetFlavor;
      TH1D   *fMinusWTightIsolatedTrackMatchedJetFlavor;
      TH1D   *fMinusWRecoElectronMatchedJetFlavor;
      TH1D   *fMinusWLooseElectronMatchedJetFlavor;
      TH1D   *fMinusWTightElectronMatchedJetFlavor;
      TH1D   *fMinusWCleanElectronMatchedJetFlavor;

      void                     Begin();
      void                     Process();
      void                     SlaveBegin();
      void                     SlaveTerminate();
      void                     Terminate();      

      ClassDef(JetChargeCorrelationStudyMod,1) // TAM example analysis module
  };
}
#endif
