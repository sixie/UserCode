//--------------------------------------------------------------------------------------------------
// $Id: SignalFilterForJettinessMod.h,v 1.1 2011/01/18 14:11:07 mzanetti Exp $
//
// SignalFilterForJettinessMod
//
// Authors: Marco Zanetti
//--------------------------------------------------------------------------------------------------

#ifndef EWKMODS_SIGNALFILTERFORJETTINESS_H
#define EWKMODS_SIGNALFILTERFORJETTINESS_H

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

class TH1D;
class TH2D;

namespace mithep 
{

  enum AnalysisSet{DrellYan, WW, hWW, DrellYan1J, WW1J, hWW1J};

  class SignalFilterForJettinessMod : public BaseMod
  {
  public:
    SignalFilterForJettinessMod(const char *name="SignalFilterForJettinessMod", 
		const char *title="Example analysis module with all branches");
    ~SignalFilterForJettinessMod() {}

    // define the signal
    void  SetAnalysisMode(int signal)  { analysis = AnalysisSet(signal); }
    
    // setters for defining input collections by name
    void  SetMuonsCollectionName(const char *name) { fMuonsCollectionName = name; }    
    void  SetElectronsCollectionName(const char *name) { fElectronsCollectionName = name; }
    void  SetLeptonCollectionName(const char *name) { fLeptonsCollectionName = name; }
    void  SetPFJetCollectionName(const char *name) { fPFJetCollectionName = name; } 
    void  SetMetCollectionName(const char *name) { fMetCollectionName = name; }
    
    // setters for naming the output collections
    void  SetSignalsNams(const char *name) { fSignalJetsName = name; }
    void  SetSignalJetsName(const char *name) { fSignalsName = name; }
    void  SetRadiationJetsName(const char *name) { fRadiationJetsName = name; }
    void  SetRadiationTracksName(const char *name) { fRadiationTracksName = name; }
    void  SetRadiationPFCandidatesName(const char *name) { fRadiationPFCandidatesName = name; }

    // getters for the output collections
    const char *GetSignalsName() const { return fSignalsName;}
    const char *GetSignalJetsName() const { return fSignalJetsName;}
    const char *GetRadiationJetsName() const { return fRadiationJetsName;}
    const char *GetRadiationTrackssName() const { return fRadiationTracksName;}
    const char *GetRadiationPFCandidatessName() const { return fRadiationPFCandidatesName;}

    // setters for analysis cuts
    void  SetOutputJetsPtCut(float cut) { outputJetsPtCut = cut; }
    void  SetOutputJetsDeltaPhiCut(float cut) { outputJetsDeltaPhiCut = cut; }
    void  SetOutputTrackPtCut(float cut) { outputTrackPtCut = cut; }
    void  SetOutputPFCandidatePtCut(float cut) { outputPFCandidatePtCut = cut; }

  protected:

    AnalysisSet analysis;

    // edm objects
    const TrackCol *fTracks;
    const CaloTowerCol *fCaloTowers;
    const PFCandidateCol *fPFCandidates;

    // input collections names
    TString fMCParticlesCollectionName;
    TString fTracksCollectionName;
    TString fCaloTowersCollectionName;
    TString fPFCandidatesCollectionName;
    TString fMuonsCollectionName;
    TString fElectronsCollectionName;
    TString fLeptonsCollectionName;
    TString fPFJetCollectionName;
    TString fMetCollectionName;


    // output collections names
    TString fSignalsName;
    TString fSignalJetsName;
    TString fRadiationJetsName;
    TString fRadiationTracksName;
    TString fRadiationPFCandidatesName;

    // cuts
    float outputJetsPtCut;
    float outputJetsDeltaPhiCut;
    float outputTrackPtCut;
    float outputPFCandidatePtCut;


    TH1D *hJetCleaningDR;

    void  Begin();
    void  Process();
    void  SlaveBegin();
    void  SlaveTerminate();
    void  Terminate();
    
    ClassDef(SignalFilterForJettinessMod,1) // TAM example analysis module
      };
}
#endif
