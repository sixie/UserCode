//--------------------------------------------------------------------------------------------------
// $Id: JettinessStudyMod.h,v 1.1 2011/01/18 14:12:51 mzanetti Exp $
//
// JettinessStudyMod
//
// Authors: Marco Zanetti
//--------------------------------------------------------------------------------------------------

#ifndef JETSTUDYMODS_JETTINESSSTUDYMOD_H
#define JETSTUDYMODS_JETTINESSSTUDYMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/MetCol.h"
#include "MitAna/DataTree/interface/ElectronCol.h"
#include "MitAna/DataTree/interface/MuonCol.h"
#include "MitAna/DataTree/interface/TrackCol.h"
#include "MitAna/DataTree/interface/CaloTowerCol.h"
#include "MitAna/DataTree/interface/JetCol.h"
#include "MitAna/DataTree/interface/GenJetCol.h"
#include "MitAna/DataTree/interface/MCParticleCol.h"
#include "MitAna/DataTree/interface/SuperClusterCol.h"
#include "MitAna/DataTree/interface/CaloTowerCol.h"
#include "MitAna/DataTree/interface/DecayParticleCol.h"
#include "MitAna/DataTree/interface/VertexCol.h"
#include "MitAna/DataTree/interface/CompositeParticleCol.h"
#include "MitAna/DataTree/interface/PFCandidateCol.h"

// for the analysis enum
#include "MitHiggs/EwkMods/interface/SignalFilterForJettinessMod.h"

class TH1D;
class TH2D;
class JetTools;

namespace mithep 
{

  class JettinessStudyMod : public BaseMod
  {
  public:
    JettinessStudyMod(const char *name="JettinessStudyMod", 
		      const char *title="Jettines studies");
    ~JettinessStudyMod() {}

    // define the signal
    void  SetAnalysisMode(int signal)  { analysis = AnalysisSet(signal); }
    
    // setters for defining input collections by name
    void  SetMetCollectionName(const char *name) { fMetCollectionName = name; } // coming from the event
    void  SetSignalsName(const char *name) { fSignalsName = name; } // coming from SignalFilterForJettinessMod
    void  SetSignalJetsName(const char *name) { fSignalJetsName = name; } // coming from SignalFilterForJettinessMod
    void  SetRadiationJetsName(const char *name) { fRadiationJetsName = name; } // coming from SignalFilterForJettinessMod 
    void  SetRadiationTracksName(const char *name) { fRadiationTracksName = name; }
    void  SetRadiationPFCandidatesName(const char *name) { fRadiationPFCandidatesName = name; }

    
    // setters for analysis cuts
    void  SetOutputJetsPtCut(float cut) { outputJetsPtCut = cut; }
    void  SetOutputJetsDeltaPhiCut(float cut) { outputJetsDeltaPhiCut = cut; }

  protected:

    AnalysisSet analysis;

    const VertexCol * fVertexes;

    // input collections names
    TString fMCParticlesCollectionName;
    TString fVertexesName;
    TString fMetCollectionName;
    TString fSignalsName;
    TString fSignalJetsName;
    TString fRadiationJetsName;
    TString fRadiationTracksName;
    TString fRadiationPFCandidatesName;

    // cuts
    float outputJetsPtCut;
    float outputJetsDeltaPhiCut;

    // histograms
    TH1D *hVerticesMulti;
    TH1D *hSignalMass;
    TH1D *hSignalPt;
    TH1D *hSignalEta;
    TH1D *hSignalRapidity;

    TH1D *hRadJetsMulti; 
    TH1D *hRadJetsPt;
    TH1D *hLeadingRadJetPt;
    TH1D *hRadJetsEta;
    TH1D *hLeadingRadJetEta;
    TH1D *hSignJetsMulti;
    TH1D *hSignJetsPt;
    TH1D *hLeadingSignJetPt;
    TH1D *hSignJetsEta;
    TH1D *hLeadingSignJetEta;

    TH1D *hTracksMulti; 
    TH1D *hTracksPt;
    TH1D *hLeadingTrackPt;
    TH1D *hTracksEta;
    TH1D *hLeadingTrackEta;

    TH1D *hPFCandidatesMulti; 
    TH1D *hPFCandidatesPt;
    TH1D *hLeadingPFCandidatePt;
    TH1D *hPFCandidatesEta;
    TH1D *hLeadingPFCandidateEta;

    TH2D *hLeadingRadJetPt_SignPt;
    TH1D *hLeadingRadJetSignalDPhi;
    TH2D *hLeadingSignJetPt_SignPt;
    TH1D *hLeadingSignJetSignalDPhi;

    TH2D *hSignalPt_Jettiness;
    TH2D *hSignalEta_Jettiness;
    TH2D *hLeadingRadJetPt_Jettiness;
    TH2D *hLeadingSignJetPt_Jettiness;
    TH2D *hLeadingRadJetEta_Jettiness;
    TH2D *hLeadingSignJetEta_Jettiness;

    TH2D *hLeadingRadJetPt_JettinessJet;
    TH2D *hLeadingRadJetPt_JettinessTrack;
    TH2D *hLeadingRadJetPt_JettinessPFC;

    TH2D *hMEt_Jettiness;
    TH2D *hMEt_SignalPt; 
    TH2D *hMEt_LeadingRadJetPt; 
    TH2D *hMEt_LeadingSignJetPt; 
    TH1D *hMEtSignalDPhi; 
    TH1D *hMEtLeadingRadJetDPhi; 
    TH1D *hMEtLeadingSignJetDPhi; 

    TH1D *hNJettiness1;
    TH1D *hNJettiness2;
    TH1D *hNJettiness3;

    TH1D *hNJettinessTracks1;
    TH1D *hNJettinessTracks2;
    TH1D *hNJettinessTracks3;

    TH1D *hNJettinessCaloTowers1;
    TH1D *hNJettinessCaloTowers2;
    TH1D *hNJettinessCaloTowers3;

    TH1D *hNJettinessPFCands1;
    TH1D *hNJettinessPFCands2;
    TH1D *hNJettinessPFCands3;

    TH1D *hEffJetVeto;
    TH1D *hEffJetJettiness;
    TH1D *hEffPFCandJettiness;

    map<int, int> jetVetoCounters;
    map<int, int> jetJettinessCounters;
    map<int, int> pfCandJettinessCounters;

    void  Begin();
    void  Process();
    void  SlaveBegin();
    void  SlaveTerminate();
    void  Terminate();
    
    ClassDef(JettinessStudyMod,1) // TAM example analysis module
      };
}
#endif
