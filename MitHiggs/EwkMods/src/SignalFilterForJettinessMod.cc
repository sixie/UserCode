// $Id: SignalFilterForJettinessMod.cc,v 1.1 2011/01/18 14:11:30 mzanetti Exp $

#include "MitHiggs/EwkMods/interface/SignalFilterForJettinessMod.h"
#include <TH1D.h>
#include <TH2D.h>
#include "MitAna/DataTree/interface/Names.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitAna/DataTree/interface/ParticleCol.h"
#include "MitAna/DataTree/interface/MCParticleCol.h"
#include "MitPhysics/Utils/interface/MatchingTools.h"

using namespace std;
using namespace mithep;

ClassImp(mithep::SignalFilterForJettinessMod)

//--------------------------------------------------------------------------------------------------
SignalFilterForJettinessMod::SignalFilterForJettinessMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fMCParticlesCollectionName(Names::gkMCPartBrn),
  fTracksCollectionName(Names::gkTrackBrn),
  fCaloTowersCollectionName(Names::gkCaloTowerBrn),
  fPFCandidatesCollectionName(Names::gkPFCandidatesBrn),
  fMuonsCollectionName(ModNames::gkCleanMuonsName),
  fElectronsCollectionName(ModNames::gkCleanElectronsName),
  fLeptonsCollectionName(ModNames::gkMergedLeptonsName),
  fPFJetCollectionName(Names::gkPFJetBrn), // default is NOT cleaned jets
  fMetCollectionName(Names::gkCaloMetBrn), // default is NOT cleaned met
  fSignalsName("signalsForJettiness"),
  fSignalJetsName("signalJetsForJettiness"),
  fRadiationJetsName("radiationJetsForJettiness"),
  fRadiationTracksName("radiationTracksForJettiness"),
  fRadiationPFCandidatesName("radiationPFCandidatesForJettiness"),
  outputJetsPtCut(30),
  outputJetsDeltaPhiCut(1),
  outputTrackPtCut(0.3),
  outputPFCandidatePtCut(0.3)
{
  // Default analysis is DrellYan
  analysis = AnalysisSet(0); 

}

//--------------------------------------------------------------------------------------------------
void SignalFilterForJettinessMod::Begin()
{
}

//--------------------------------------------------------------------------------------------------
void SignalFilterForJettinessMod::Process()
{

  /*  I N P U T  */
  // tracks (from edm)
  LoadBranch(fTracksCollectionName);
  // calo towers (from edm)
  LoadBranch(fCaloTowersCollectionName);
  // PF candidates (from edm)
  LoadBranch(fPFCandidatesCollectionName);
  // muons
  MuonOArr *cleanMuons = dynamic_cast<MuonOArr* >(FindObjThisEvt(fMuonsCollectionName));
  // electrons
  ElectronOArr *cleanElectrons = dynamic_cast<ElectronOArr* >(FindObjThisEvt(fElectronsCollectionName));
  // leptons
  ParticleOArr *leptons = dynamic_cast<ParticleOArr*>(FindObjThisEvt(fLeptonsCollectionName));
  // pf jets
  JetOArr *cleanPFJets = dynamic_cast<JetOArr* >(FindObjThisEvt(fPFJetCollectionName));
  // MET
  //Met *pfMet = dynamic_cast<Met*> (FindObjThisEvt(fMetCollectionName));
  MetOArr *pfMetColl = dynamic_cast<MetOArr*> (FindObjThisEvt(fMetCollectionName));
  const Met *pfMet = pfMetColl->At(0);
  
  /*  O U T P U T  */
  // signal
  CompositeParticleOArr *signal = new CompositeParticleOArr;
  signal->SetName(fSignalsName);
  // signal jets
  JetOArr *signalJets = new JetOArr;
  signalJets->SetName(fSignalJetsName);

  // collections for jettiness computation
  JetOArr *radiationJets = new JetOArr;
  radiationJets->SetName(fRadiationJetsName);

  TrackOArr *radiationTracks = new TrackOArr;
  radiationTracks->SetName(fRadiationTracksName);

  PFCandidateOArr *radiationPFCandidates = new PFCandidateOArr;
  radiationPFCandidates->SetName(fRadiationPFCandidatesName);

  //   if (cleanMuons->GetEntries() >= 2) {
  //     diLepton->AddDaughter(cleanMuons->At(0));
  //     diLepton->AddDaughter(cleanMuons->At(1));
  //   }

  // preselection
  bool preSelections = false; 
  if (leptons->GetEntries() >= 2) 
    if (leptons->At(0)->Pt()>=15 && leptons->At(1)->Pt()>=15 ) 
      if (leptons->At(0)->Charge() * leptons->At(1)->Charge() < 0) 
	preSelections = true;

  if (preSelections) {
    // the di-leptonic signal
    CompositeParticle *diLepton = new CompositeParticle;
    diLepton->AddDaughter(leptons->At(0));
    diLepton->AddDaughter(leptons->At(1));

    // select the signal
    if (analysis == DrellYan || analysis ==DrellYan1J ) {
      if (diLepton->Mass() > 50) 
	signal->Add(diLepton);
    }
    if (analysis == WW || analysis == WW1J ) {  
      if (fabs(diLepton->Mass()-91.1876) > 15.0)  
	if (pfMet->Pt() > 30)
	  signal->Add(diLepton);
    }
    if (analysis == hWW || analysis == hWW1J) {
      if (diLepton->Mass() < 50 ) 
	if (pfMet->Pt() > 30)
	  if (MathUtils::DeltaPhi(diLepton->Daughter(0)->Phi(),diLepton->Daughter(1)->Phi() ) < 2)
	    signal->Add(diLepton);
    }

    const Track * signalTrack1;
    const Track * signalTrack2;
    if (leptons->At(0)->ObjType() == kMuon) 
      signalTrack1 = (dynamic_cast<const Muon* > (diLepton->Daughter(0)))->TrackerTrk();
    if (leptons->At(0)->ObjType() == kElectron)
      signalTrack1 = (dynamic_cast<const Electron* > (diLepton->Daughter(0)))->TrackerTrk();
    if (leptons->At(1)->ObjType() == kMuon) 
      signalTrack2 = (dynamic_cast<const Muon* > (diLepton->Daughter(1)))->TrackerTrk();
    if (leptons->At(1)->ObjType() == kElectron)
      signalTrack2 = (dynamic_cast<const Electron* > (diLepton->Daughter(1)))->TrackerTrk();

    // selecting tracks (removing signal tracks)
    for (UInt_t i=0; i<fTracks->GetEntries(); ++i) {
      if (fTracks->At(i) != signalTrack1 && fTracks->At(i) != signalTrack2) {
	if (fTracks->At(i)->Pt() > outputTrackPtCut) radiationTracks->Add(fTracks->At(i));
      }
    }
    // selecting pf candidates (removing signal tracks)
    for (UInt_t i=0; i<fPFCandidates->GetEntries(); ++i) {
      if (fPFCandidates->At(i)->TrackerTrk() != signalTrack1 && fPFCandidates->At(i)->TrackerTrk() != signalTrack2) {
	if (fPFCandidates->At(i)->Pt() > outputPFCandidatePtCut) radiationPFCandidates->Add(fPFCandidates->At(i));
      }
    }
    
    // distinguish analyses only for jets
    // 0 jets cases
    if (analysis == DrellYan || analysis == WW || analysis == hWW ) 
      for (UInt_t i=0; i<cleanPFJets->GetEntries(); ++i) 
	radiationJets->Add(cleanPFJets->At(i));
    
    // 1 jets cases
    if (analysis == DrellYan1J || analysis == WW1J || analysis == hWW1J ) {
      for (UInt_t i=0; i<cleanPFJets->GetEntries(); ++i) {
	const Jet *jet = cleanPFJets->At(i);        
	// defining signal jet
	if (i==0) {
	  if (jet->Pt() > outputJetsPtCut)
	    //MathUtils::DeltaPhi(jet->Phi(), diLepton->Phi()) > outputJetsDeltaPhiCut )
	    signalJets->Add(jet);
	  else radiationJets->Add(jet);
	}
	else radiationJets->Add(jet);
      }
    }
  }
  
  // add to event for other modules to use
  AddObjThisEvt(signal);
  AddObjThisEvt(signalJets);
  AddObjThisEvt(radiationJets);
  //radiationTracks->Sort();
  AddObjThisEvt(radiationTracks);
  //radiationPFCandidates->Sort();
  AddObjThisEvt(radiationPFCandidates);
}


//--------------------------------------------------------------------------------------------------
void SignalFilterForJettinessMod::SlaveBegin()
{
  ReqBranch(fTracksCollectionName, fTracks);
  ReqBranch(fCaloTowersCollectionName, fCaloTowers);
  ReqBranch(fPFCandidatesCollectionName, fPFCandidates);

  AddTH1(hJetCleaningDR ,"hJetCleaningDR",";#DeltaR;Number of Events",100,0.,10.);
}

//--------------------------------------------------------------------------------------------------
void SignalFilterForJettinessMod::SlaveTerminate()
{

}

//--------------------------------------------------------------------------------------------------
void SignalFilterForJettinessMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
