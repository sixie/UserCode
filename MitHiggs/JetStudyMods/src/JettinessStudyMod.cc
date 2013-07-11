// $Id: JettinessStudyMod.cc,v 1.1 2011/01/18 14:13:13 mzanetti Exp $

#include "MitHiggs/JetStudyMods/interface/JettinessStudyMod.h"
#include <TH1D.h>
#include <TH2D.h>
#include "MitAna/DataTree/interface/Names.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitAna/DataTree/interface/ParticleCol.h"
#include "MitAna/DataTree/interface/MCParticleCol.h"
#include "MitPhysics/Utils/interface/MatchingTools.h"
#include "MitPhysics/Utils/interface/JetTools.h"


using namespace std;
using namespace mithep;

ClassImp(mithep::JettinessStudyMod)

//--------------------------------------------------------------------------------------------------
JettinessStudyMod::JettinessStudyMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fVertexes(0),
  fMCParticlesCollectionName(Names::gkMCPartBrn),
  fVertexesName(Names::gkPVBrn),
  fMetCollectionName(Names::gkCaloMetBrn), // default is NOT cleaned met
  fSignalsName("signalJetsForJettiness"),
  fSignalJetsName("signalJetsForJettiness"),
  fRadiationJetsName("radiationJetsForJettiness"),
  fRadiationTracksName("radiationTracksForJettiness"),
  fRadiationPFCandidatesName("radiationPFCandidatesForJettiness"),
  outputJetsPtCut(30),
  outputJetsDeltaPhiCut(1)
{
  // Default analysis is DrellYan
  analysis = AnalysisSet(0); 

  // zero-ing the counters
  for (int i=0; i<10; i++) {
    jetVetoCounters[i]=0;
    jetJettinessCounters[i]=0;
    pfCandJettinessCounters[i]=0;
  }

}

//--------------------------------------------------------------------------------------------------
void JettinessStudyMod::Begin()
{
}

//--------------------------------------------------------------------------------------------------
void JettinessStudyMod::Process()
{

  // check for the pileup
  LoadBranch(fVertexesName);


  // MET (after correction/cleaning)
  MetOArr *pfMetColl = dynamic_cast<MetOArr*> (FindObjThisEvt(fMetCollectionName));
  const Met *pfMet = pfMetColl->At(0);

  // signal 
  CompositeParticleOArr *Signals = dynamic_cast<CompositeParticleOArr* >(FindObjThisEvt(fSignalsName));
  // signal jets
  JetOArr *signalJets = dynamic_cast<JetOArr* >(FindObjThisEvt(fSignalJetsName));
  // radiation jets
  JetOArr *radiationJets = dynamic_cast<JetOArr* >(FindObjThisEvt(fRadiationJetsName));
  // radiation tracks
  TrackOArr *radiationTracks = dynamic_cast<TrackOArr* > (FindObjThisEvt(fRadiationTracksName));
  // pf candidates tracks
  PFCandidateOArr *radiationPFCandidates = dynamic_cast<PFCandidateOArr* > (FindObjThisEvt(fRadiationPFCandidatesName));

  if ( Signals->Entries() > 0 ) {

    if (fVertexes->GetEntries() > 1) return;

    CompositeParticle * signal = Signals->At(0);
    hSignalMass->Fill(signal->Mass());

    hVerticesMulti->Fill(fVertexes->GetEntries());
    
    double beta = sqrt(1-signal->Mass()*signal->Mass()/(signal->E()*signal->E()));
    double costheta = signal->Pz()/signal->P();
    double rapidity = 0.5*log( (1+beta*costheta) / (1-beta*costheta) );
    double Q = sqrt(signal->Mass()*signal->Mass()+signal->Pt()*signal->Pt());
    double jettinessJets = JetTools::NJettiness(radiationJets,signalJets, Q,rapidity);
    double jettinessTracks = JetTools::NJettiness(radiationTracks,signalJets, Q,rapidity);
    double jettinessPFCands = JetTools::NJettiness(radiationPFCandidates,signalJets, Q,rapidity);
    
    // choose..
    double jettiness = jettinessJets;
    
    // signal features
    hSignalRapidity->Fill(rapidity);
    hSignalPt->Fill(signal->Pt());
    hSignalEta->Fill(signal->Eta());
    
    // basic jettiness distribution
    hNJettiness1->Fill(jettiness);
    hNJettiness2->Fill(JetTools::NJettiness(radiationJets,signalJets, Q,0));
    hNJettiness3->Fill(JetTools::NJettiness(radiationJets,signalJets, Q,signal->Eta()));
    hNJettinessTracks1->Fill(jettinessTracks);
    hNJettinessPFCands1->Fill(jettinessPFCands);
    
    // radiation jets
    hRadJetsMulti->Fill(radiationJets->GetEntries());
    for (UInt_t i=0; i<radiationJets->GetEntries(); ++i) {
      const Jet *jet = radiationJets->At(i);
      if (i==0) {
	hLeadingRadJetPt->Fill(jet->Pt());
	hLeadingRadJetEta->Fill(jet->Eta());
      }
      hRadJetsPt->Fill(jet->Pt());
      hRadJetsEta->Fill(jet->Eta());	
    }

    // counting the events that would pass a given cut
    jetVetoCounters[0]++;
    jetJettinessCounters[0]++;
    pfCandJettinessCounters[0]++;
    for (int i=1; i<10; i++) {
      if (radiationJets->GetEntries() > 0)
	if (radiationJets->At(0)->Pt() < (10 + i*5) ) jetVetoCounters[i]++;
      if (jettinessJets < i*0.1) jetJettinessCounters[i]++;
      if (jettinessPFCands < i*0.1) pfCandJettinessCounters[i]++;
    }
    
    // signal jets
    hSignJetsMulti->Fill(signalJets->GetEntries());
    for (UInt_t i=0; i<signalJets->GetEntries(); ++i) {
      const Jet *jet = signalJets->At(i);
      if (i==0) {
	hLeadingSignJetPt->Fill(jet->Pt());
	hLeadingSignJetEta->Fill(jet->Eta());
      }
      hSignJetsPt->Fill(jet->Pt());
      hSignJetsEta->Fill(jet->Eta());	
    }
    
    // tracks
    hTracksMulti->Fill(radiationTracks->GetEntries());
    for (UInt_t i=0; i<radiationTracks->GetEntries(); ++i) {
      if (i==0) {
	hLeadingTrackPt->Fill(radiationTracks->At(0)->Pt());
	hLeadingTrackEta->Fill(radiationTracks->At(0)->Eta());
      }
      hTracksPt->Fill(radiationTracks->At(i)->Pt());
      hTracksEta->Fill(radiationTracks->At(i)->Eta());
    }
    
    // PF candidates
    hPFCandidatesMulti->Fill(radiationPFCandidates->GetEntries());
    for (UInt_t i=0; i<radiationPFCandidates->GetEntries(); ++i) {
      if (i==0) {
	  hLeadingPFCandidatePt->Fill(radiationPFCandidates->At(0)->Pt());
	  hLeadingPFCandidateEta->Fill(radiationPFCandidates->At(0)->Eta());
      }
      hPFCandidatesPt->Fill(radiationPFCandidates->At(i)->Pt());
      hPFCandidatesEta->Fill(radiationPFCandidates->At(i)->Eta());
    }
    
    
    hMEt_Jettiness->Fill(pfMet->Pt(),jettiness);
    hMEt_SignalPt->Fill(pfMet->Pt(),signal->Pt());
    hMEtSignalDPhi->Fill(MathUtils::DeltaPhi(pfMet->Mom(),signal->Mom()));
    
    // correlation
    hSignalEta_Jettiness->Fill(signal->Eta(),jettiness);
    hSignalPt_Jettiness->Fill(signal->Pt(),jettiness);
    
    if (radiationJets->GetEntries()>0) {
      hLeadingRadJetPt_SignPt->Fill(signal->Pt(),radiationJets->At(0)->Pt());
      hLeadingRadJetSignalDPhi->Fill(MathUtils::DeltaPhi(signal->Mom(),radiationJets->At(0)->Mom()));  
      hLeadingRadJetPt_Jettiness->Fill(radiationJets->At(0)->Pt(), jettiness);
      hLeadingRadJetPt_JettinessTrack->Fill(radiationJets->At(0)->Pt(), jettinessTracks);
      hLeadingRadJetPt_JettinessPFC->Fill(radiationJets->At(0)->Pt(), jettinessPFCands);
      hLeadingRadJetEta_Jettiness->Fill(radiationJets->At(0)->Eta(), jettiness);
      hMEt_LeadingRadJetPt->Fill(pfMet->Pt(),radiationJets->At(0)->Pt());
      hMEtLeadingRadJetDPhi->Fill(MathUtils::DeltaPhi(pfMet->Mom(),radiationJets->At(0)->Mom()));
    }
    if (signalJets->GetEntries()>0) {
      hLeadingSignJetPt_SignPt->Fill(signal->Pt(),signalJets->At(0)->Pt());
      hLeadingSignJetSignalDPhi->Fill(MathUtils::DeltaPhi(signal->Mom(),signalJets->At(0)->Mom()));  
      hLeadingSignJetPt_Jettiness->Fill(signalJets->At(0)->Pt(), jettiness);
      hLeadingSignJetEta_Jettiness->Fill(signalJets->At(0)->Eta(), jettiness);
      hMEt_LeadingSignJetPt->Fill(pfMet->Pt(),signalJets->At(0)->Pt());
      hMEtLeadingSignJetDPhi->Fill(MathUtils::DeltaPhi(pfMet->Mom(),signalJets->At(0)->Mom()));
    }
    
  }

}

//--------------------------------------------------------------------------------------------------
void JettinessStudyMod::SlaveBegin()
{

  ReqBranch(fVertexesName, fVertexes);

  AddTH1(hVerticesMulti ,"hVerticesMulti",";vertex multiplicity;",5,-0.5,4.5);  

  // signal histos
  AddTH1(hSignalMass ,"hSignalMass",";SignalMass;a.u.",100,0.,200.);
  AddTH1(hSignalPt ,"hSignalPt",";SignalPt;a.u.",50,0.,100.);
  AddTH1(hSignalEta ,"hSignalEta",";SignalEta;a.u.",50,-5,5.);
  AddTH1(hSignalRapidity ,"hSignalRapidity",";SignalRapidity;a.u.",50,-5,5.);

  // jet histos
  AddTH1(hRadJetsMulti ,"hRadJetsMulti",";jet multiplicity;",21,-0.5,20.5);  
  AddTH1(hRadJetsPt ,"hRadJetsPt",";jet p_{T};",100,0.,200.);  
  AddTH1(hLeadingRadJetPt ,"hLeadingRadJetPt",";leading jet p_{T};",100,0.,200.);  
  AddTH1(hRadJetsEta ,"hRadJetsEta",";jet #eta;",50,-5.,5.);  
  AddTH1(hLeadingRadJetEta ,"hLeadingRadJetEta",";leading jet #eta;",50,-5.,5.);
  AddTH1(hSignJetsMulti ,"hSignJetsMulti",";jet multiplicity;",21,-0.5,20.5);  
  AddTH1(hSignJetsPt ,"hSignJetsPt",";jet p_{T};",100,0.,200.);  
  AddTH1(hLeadingSignJetPt ,"hLeadingSignJetPt",";leading jet p_{T};",100,0.,200.);  
  AddTH1(hSignJetsEta ,"hSignJetsEta",";jet #eta;",50,-5.,5.);  
  AddTH1(hLeadingSignJetEta ,"hLeadingSignJetEta",";leading jet #eta;",50,-5.,5.);

  AddTH2(hLeadingRadJetPt_SignPt ,"hLeadingRadJetPt_SignPt",";p_{T}^{Sign};jet p_{T}",50,0.,100.,100,0.,200.);
  AddTH1(hLeadingRadJetSignalDPhi ,"hLeadingRadJetSignalDPhi",";#Delta#phi(jet;Sign);",40,0.,4.);
  AddTH2(hLeadingSignJetPt_SignPt ,"hLeadingSignJetPt_SignPt",";p_{T}^{Sign};jet p_{T}",50,0.,100.,100,0.,200.);
  AddTH1(hLeadingSignJetSignalDPhi ,"hLeadingSignJetSignalDPhi",";#Delta#phi(jet;Sign);",40,0.,4.);

  AddTH2(hSignalEta_Jettiness ,"hSignalEta_Jettiness",";#eta^{Sign};jettines",50,-5,5.,50,0,2);
  AddTH2(hSignalPt_Jettiness ,"hSignalPt_Jettiness",";p_{T}^{Sign};jettines",50,0,100.,50,0,2);
  AddTH2(hLeadingRadJetPt_Jettiness ,"hLeadingRadJetPt_Jettiness",";p_{T}^{jet};jettines",50,0,100.,50,0,2);
  AddTH2(hLeadingSignJetPt_Jettiness ,"hLeadingSignJetPt_Jettiness",";p_{T}^{jet};jettines",50,0,100,50,0,2);
  AddTH2(hLeadingRadJetEta_Jettiness ,"hLeadingRadJetEta_Jettiness",";#eta^{jet};jettines",50,-5,5.,50,0,2);
  AddTH2(hLeadingSignJetEta_Jettiness ,"hLeadingSignJetEta_Jettiness",";#eta^{jet};jettines",50,-5,5.,50,0,2);

  AddTH1(hTracksMulti ,"hTracksMulti",";track multiplicity;",201,-0.5,200.5);  
  AddTH1(hTracksPt ,"hTracksPt","; p_{T};",100,0.,100.);  
  AddTH1(hLeadingTrackPt ,"hLeadingTrackPt","; p_{T};",100,0.,100.);  
  AddTH1(hTracksEta ,"hTracksEta","; #eta;",30,-3.,3.);  
  AddTH1(hLeadingTrackEta ,"hLeadingTrackEta","; #eta;",30,-3.,3.);

  AddTH1(hPFCandidatesMulti ,"hPFCandidatesMulti",";PF cands multiplicity;",201,-0.5,200.5);  
  AddTH1(hPFCandidatesPt ,"hPFCandidatesPt","; p_{T};",100,0.,100.);  
  AddTH1(hLeadingPFCandidatePt ,"hLeadingPFCandidatePt","; p_{T};",100,0.,100.);  
  AddTH1(hPFCandidatesEta ,"hPFCandidatesEta","; #eta;",50,-5.,5.);  
  AddTH1(hLeadingPFCandidateEta ,"hLeadingPFCandidateEta","; #eta;",50,-5.,5.);


  // each type of jettines
  AddTH2(hLeadingRadJetPt_JettinessJet ,"hLeadingRadJetPt_JettinessJet",";p_{T}^{jet};jettines",50,0,100.,50,0,2);
  AddTH2(hLeadingRadJetPt_JettinessTrack ,"hLeadingRadJetPt_JettinessTrack",";p_{T}^{jet};jettines",50,0,100.,50,0,2);
  AddTH2(hLeadingRadJetPt_JettinessPFC ,"hLeadingRadJetPt_JettinessPFC",";p_{T}^{jet};jettines",50,0,100.,50,0,2);


  AddTH2(hMEt_Jettiness ,"hMEt_Jettiness",";MEt;jettines",50,0,100.,50,0,2);
  AddTH2(hMEt_SignalPt ,"hMEt_SignalPt",";MEt;p_{T}^{Sign}",50,0,100.,50,0,100);
  AddTH2(hMEt_LeadingRadJetPt ,"hMEt_LeadingRadJetPt",";MEt;jet p_{T}",50,0,100.,50,0,100);
  AddTH2(hMEt_LeadingSignJetPt ,"hMEt_LeadingSignJetPt",";MEt;jet p_{T}",50,0,100.,50,0,100);
  AddTH1(hMEtSignalDPhi ,"hMEtSignalDPhi",";#Delta#phi(MEt;Sign);",40,0.,4.);
  AddTH1(hMEtLeadingRadJetDPhi ,"hMEtLeadingRadJetDPhi",";#Delta#phi(MEt;jet);",40,0.,4.);
  AddTH1(hMEtLeadingSignJetDPhi ,"hMEtLeadingSignJetDPhi",";#Delta#phi(MEt;jet);",40,0.,4.);

  AddTH1(hNJettiness1 ,"hNJettiness1",";0Jettiness;a.u.",50,0,2);
  AddTH1(hNJettiness2 ,"hNJettiness2",";0Jettiness;a.u.",50,0,2);
  AddTH1(hNJettiness3 ,"hNJettiness3",";0Jettiness;a.u.",50,0,2);

  AddTH1(hNJettinessTracks1 ,"hNJettinessTracks1",";0Jettiness;a.u.",50,0,2);
  AddTH1(hNJettinessTracks2 ,"hNJettinessTracks2",";0Jettiness;a.u.",50,0,2);
  AddTH1(hNJettinessTracks3 ,"hNJettinessTracks3",";0Jettiness;a.u.",50,0,2);

  AddTH1(hNJettinessCaloTowers1 ,"hNJettinessCaloTowers1",";0Jettiness;a.u.",50,0,2);
  AddTH1(hNJettinessCaloTowers2 ,"hNJettinessCaloTowers2",";0Jettiness;a.u.",50,0,2);
  AddTH1(hNJettinessCaloTowers3 ,"hNJettinessCaloTowers3",";0Jettiness;a.u.",50,0,2);

  AddTH1(hNJettinessPFCands1 ,"hNJettinessPFCands1",";0Jettiness;a.u.",50,0,2);
  AddTH1(hNJettinessPFCands2 ,"hNJettinessPFCands2",";0Jettiness;a.u.",50,0,2);
  AddTH1(hNJettinessPFCands3 ,"hNJettinessPFCands3",";0Jettiness;a.u.",50,0,2);

  AddTH1(hEffJetVeto,"hEffJetVeto",";;",9,0,90);
  AddTH1(hEffJetJettiness,"hEffJetJettiness",";;",9,0,0.9);
  AddTH1(hEffPFCandJettiness,"hEffPFCandJettiness",";;",9,0,0.9);
}

//--------------------------------------------------------------------------------------------------
void JettinessStudyMod::SlaveTerminate()
{
  for (int i=1; i<10; i++) {
    hEffJetVeto->SetBinContent(i,jetVetoCounters[i]/jetVetoCounters[0]);
    hEffJetJettiness->SetBinContent(i,jetJettinessCounters[i]/jetJettinessCounters[0]);
    hEffPFCandJettiness->SetBinContent(i,pfCandJettinessCounters[i]/pfCandJettinessCounters[0]);
  }
}

//--------------------------------------------------------------------------------------------------
void JettinessStudyMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
