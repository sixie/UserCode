// $Id: ElectronIDChargeCorrelationStudyMod.cc,v 1.3 2009/03/17 08:33:15 sixie Exp $

#include "MitHiggs/FakeRateMods/interface/ElectronIDChargeCorrelationStudyMod.h"
#include "MitAna/DataUtil/interface/Debug.h"
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include "MitAna/DataTree/interface/Names.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitPhysics/Utils/interface/IsolationTools.h"
#include <map>

using namespace mithep;

ClassImp(mithep::ElectronIDChargeCorrelationStudyMod)

//--------------------------------------------------------------------------------------------------
  ElectronIDChargeCorrelationStudyMod::ElectronIDChargeCorrelationStudyMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fVetoGeneratorLeptons(true),
  fMCPartBranchName(Names::gkMCPartBrn),
  fGenJetBranchName(Names::gkSC5GenJetBrn),
  fTrackBranchName(Names::gkTrackBrn),
  fGsfTrackBranchName(Names::gkGsfTrackBrn),
  fElectronBranchName(Names::gkElectronBrn),
  fJetBranchName(Names::gkCaloJetBrn),
  fBarrelSuperClusterBranchName(Names::gkBarrelSuperClusterBrn),
  fEndcapSuperClusterBranchName(Names::gkEndcapSuperClusterBrn),
  fCleanElectronsName(ModNames::gkCleanElectronsName),
  fGoodJetsName(ModNames::gkGoodJetsName),
  fMCLeptonsName(ModNames::gkMCLeptonsName),
  fMCTausName(ModNames::gkMCTausName),
  fParticles(0),
  fBarrelSuperClusters(0),
  fEndcapSuperClusters(0),
  fTracks(0),
  fGsfTracks(0),
  fElectrons(0),
  fJets(0)
{ 
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void ElectronIDChargeCorrelationStudyMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void ElectronIDChargeCorrelationStudyMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fMCPartBranchName,                fParticles);
  ReqBranch(fGenJetBranchName,                fGenJets);
  ReqBranch(fTrackBranchName,                 fTracks);
  ReqBranch(fGsfTrackBranchName,              fGsfTracks);
  ReqBranch(fElectronBranchName,              fElectrons);
  ReqBranch(fJetBranchName,                   fJets);
  ReqBranch(fBarrelSuperClusterBranchName,    fBarrelSuperClusters);
  ReqBranch(fEndcapSuperClusterBranchName,    fEndcapSuperClusters);

  //*******************************************************************************************
  //Charge Correlations between the quark and leading track
  //*******************************************************************************************
  AddTH1(fSameSignFakeElectronHOverE, "hSameSignFakeElectronHOverE", 
         ";SameSignFakeElectronHOverE;Number of Events", 100, 0,0.5);
  AddTH1(fSameSignFakeElectronESuperClusterOverP, "hSameSignFakeElectronESuperClusterOverP", 
         ";SameSignFakeElectronESuperClusterOverP;Number of Events", 100, 0,2.0);
  AddTH1(fSameSignFakeElectronSigmaEtaEta, "hSameSignFakeElectronSigmaEtaEta", 
         ";SameSignFakeElectronSigmaEtaEta;Number of Events", 100, 0,0.1);
  AddTH1(fSameSignFakeElectronSigmaiEtaiEta, "hSameSignFakeElectronSigmaiEtaiEta", 
         ";SameSignFakeElectronSigmaiEtaiEta;Number of Events", 100, 0,0.01);
  AddTH1(fSameSignFakeElectronDeltaPhi, "hSameSignFakeElectronDeltaPhi", 
         ";SameSignFakeElectronDeltaPhi;Number of Events", 100, 0,0.3);
  AddTH1(fSameSignFakeElectronDeltaEta, "hSameSignFakeElectronDeltaEta", 
         ";SameSignFakeElectronDeltaEta;Number of Events", 100, 0,0.1);
  AddTH1(fSameSignFakeElectronfBrem, "hSameSignFakeElectronfBrem", 
         ";SameSignFakeElectronfBrem;Number of Events",  100, 0.0,1.0);
  AddTH1(fSameSignFakeElectronClassification, "hSameSignFakeElectronClassification", 
         ";SameSignFakeElectronClassification;Number of Events",  141, -0.5,140.5);
  AddTH1(fSameSignFakeElectronEta, "hSameSignFakeElectronEta", 
         ";SameSignFakeElectronEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fSameSignFakeElectronPhi, "hSameSignFakeElectronPhi", 
         ";SameSignFakeElectronPhi;Number of Events",  25, -3.2,3.2);

  AddTH1(fOppositeSignFakeElectronHOverE, "hOppositeSignFakeElectronHOverE", 
         ";OppositeSignFakeElectronHOverE;Number of Events", 100, 0,0.5);
  AddTH1(fOppositeSignFakeElectronESuperClusterOverP, "hOppositeSignFakeElectronESuperClusterOverP", 
         ";OppositeSignFakeElectronESuperClusterOverP;Number of Events", 100, 0,2.0);
  AddTH1(fOppositeSignFakeElectronSigmaEtaEta, "hOppositeSignFakeElectronSigmaEtaEta", 
         ";OppositeSignFakeElectronSigmaEtaEta;Number of Events", 100, 0,0.1);
  AddTH1(fOppositeSignFakeElectronSigmaiEtaiEta, "hOppositeSignFakeElectronSigmaiEtaiEta", 
         ";OppositeSignFakeElectronSigmaiEtaiEta;Number of Events", 100, 0,0.01);
  AddTH1(fOppositeSignFakeElectronDeltaPhi, "hOppositeSignFakeElectronDeltaPhi", 
         ";OppositeSignFakeElectronDeltaPhi;Number of Events", 100, 0,0.3);
  AddTH1(fOppositeSignFakeElectronDeltaEta, "hOppositeSignFakeElectronDeltaEta", 
         ";OppositeSignFakeElectronDeltaEta;Number of Events", 100, 0,0.1);
  AddTH1(fOppositeSignFakeElectronfBrem, "hOppositeSignFakeElectronfBrem", 
         ";OppositeSignFakeElectronfBrem;Number of Events",  100, 0.0,1.0);
  AddTH1(fOppositeSignFakeElectronClassification, "hOppositeSignFakeElectronClassification", 
         ";OppositeSignFakeElectronClassification;Number of Events",  141, -0.5,140.5);
  AddTH1(fOppositeSignFakeElectronEta, "hOppositeSignFakeElectronEta", 
         ";OppositeSignFakeElectronEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fOppositeSignFakeElectronPhi, "hOppositeSignFakeElectronPhi", 
         ";SameSignFakeElectronPhi;Number of Events",  25, -3.2,3.2);

  AddTH1(fSameSignFakeIsolatedElectronHOverE, "hSameSignFakeIsolatedElectronHOverE", 
         ";SameSignFakeIsolatedElectronHOverE;Number of Events", 100, 0,0.5);
  AddTH1(fSameSignFakeIsolatedElectronESuperClusterOverP, "hSameSignFakeIsolatedElectronESuperClusterOverP", 
         ";SameSignFakeIsolatedElectronESuperClusterOverP;Number of Events", 100, 0,2.0);
  AddTH1(fSameSignFakeIsolatedElectronSigmaEtaEta, "hSameSignFakeIsolatedElectronSigmaEtaEta", 
         ";SameSignFakeIsolatedElectronSigmaEtaEta;Number of Events", 100, 0,0.1);
  AddTH1(fSameSignFakeIsolatedElectronSigmaiEtaiEta, "hSameSignFakeIsolatedElectronSigmaiEtaiEta", 
         ";SameSignFakeIsolatedElectronSigmaiEtaiEta;Number of Events", 100, 0,0.01);
  AddTH1(fSameSignFakeIsolatedElectronDeltaPhi, "hSameSignFakeIsolatedElectronDeltaPhi", 
         ";SameSignFakeIsolatedElectronDeltaPhi;Number of Events", 100, 0,0.3);
  AddTH1(fSameSignFakeIsolatedElectronDeltaEta, "hSameSignFakeIsolatedElectronDeltaEta", 
         ";SameSignFakeIsolatedElectronDeltaEta;Number of Events", 100, 0,0.1);
  AddTH1(fSameSignFakeIsolatedElectronfBrem, "hSameSignFakeIsolatedElectronfBrem", 
         ";SameSignFakeIsolatedElectronfBrem;Number of Events",  100, 0.0,1.0);
  AddTH1(fSameSignFakeIsolatedElectronClassification, "hSameSignFakeIsolatedElectronClassification", 
         ";SameSignFakeIsolatedElectronClassification;Number of Events", 141, -0.5,140.5);
  AddTH1(fSameSignFakeIsolatedElectronEta, "hSameSignFakeIsolatedElectronEta", 
         ";SameSignFakeIsolatedElectronEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fSameSignFakeIsolatedElectronPhi, "hSameSignFakeIsolatedElectronPhi", 
         ";SameSignFakeIsolatedElectronPhi;Number of Events",  25, -3.2,3.2);

  AddTH1(fOppositeSignFakeIsolatedElectronHOverE, "hOppositeSignFakeIsolatedElectronHOverE", 
         ";OppositeSignFakeIsolatedElectronHOverE;Number of Events", 100, 0,0.5);
  AddTH1(fOppositeSignFakeIsolatedElectronESuperClusterOverP, "hOppositeSignFakeIsolatedElectronESuperClusterOverP", 
         ";OppositeSignFakeIsolatedElectronESuperClusterOverP;Number of Events", 100, 0,2.0);
  AddTH1(fOppositeSignFakeIsolatedElectronSigmaEtaEta, "hOppositeSignFakeIsolatedElectronSigmaEtaEta", 
         ";OppositeSignFakeIsolatedElectronSigmaEtaEta;Number of Events", 100, 0,0.1);
  AddTH1(fOppositeSignFakeIsolatedElectronSigmaiEtaiEta, "hOppositeSignFakeIsolatedElectronSigmaiEtaiEta", 
         ";OppositeSignFakeIsolatedElectronSigmaiEtaiEta;Number of Events", 100, 0,0.01);
  AddTH1(fOppositeSignFakeIsolatedElectronDeltaPhi, "hOppositeSignFakeIsolatedElectronDeltaPhi", 
         ";OppositeSignFakeIsolatedElectronDeltaPhi;Number of Events", 100, 0,0.3);
  AddTH1(fOppositeSignFakeIsolatedElectronDeltaEta, "hOppositeSignFakeIsolatedElectronDeltaEta", 
         ";OppositeSignFakeIsolatedElectronDeltaEta;Number of Events", 100, 0,0.1);
  AddTH1(fOppositeSignFakeIsolatedElectronfBrem, "hOppositeSignFakeIsolatedElectronfBrem", 
         ";OppositeSignFakeIsolatedElectronfBrem;Number of Events",  100, 0.0,1.0);
  AddTH1(fOppositeSignFakeIsolatedElectronClassification, "hOppositeSignFakeIsolatedElectronClassification", 
         ";OppositeSignFakeIsolatedElectronClassification;Number of Events",  141, -0.5,140.5);
  AddTH1(fOppositeSignFakeIsolatedElectronEta, "hOppositeSignFakeIsolatedElectronEta", 
         ";OppositeSignFakeIsolatedElectronEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fOppositeSignFakeIsolatedElectronPhi, "hOppositeSignFakeIsolatedElectronPhi", 
         ";OppositeSignFakeIsolatedElectronPhi;Number of Events",  25, -3.2,3.2);


  AddTH1(fSameSignFakeCleanElectronHOverE, "hSameSignFakeCleanElectronHOverE", 
         ";SameSignFakeCleanElectronHOverE;Number of Events", 100, 0,0.5);
  AddTH1(fSameSignFakeCleanElectronESuperClusterOverP, "hSameSignFakeCleanElectronESuperClusterOverP", 
         ";SameSignFakeCleanElectronESuperClusterOverP;Number of Events", 100, 0,2.0);
  AddTH1(fSameSignFakeCleanElectronSigmaEtaEta, "hSameSignFakeCleanElectronSigmaEtaEta", 
         ";SameSignFakeCleanElectronSigmaEtaEta;Number of Events", 100, 0,0.1);
  AddTH1(fSameSignFakeCleanElectronSigmaiEtaiEta, "hSameSignFakeCleanElectronSigmaiEtaiEta", 
         ";SameSignFakeCleanElectronSigmaiEtaiEta;Number of Events", 100, 0,0.01);
  AddTH1(fSameSignFakeCleanElectronDeltaPhi, "hSameSignFakeCleanElectronDeltaPhi", 
         ";SameSignFakeCleanElectronDeltaPhi;Number of Events", 100, 0,0.3);
  AddTH1(fSameSignFakeCleanElectronDeltaEta, "hSameSignFakeCleanElectronDeltaEta", 
         ";SameSignFakeCleanElectronDeltaEta;Number of Events", 100, 0,0.1);
  AddTH1(fSameSignFakeCleanElectronfBrem, "hSameSignFakeCleanElectronfBrem", 
         ";SameSignFakeCleanElectronfBrem;Number of Events",  100, 0.0,1.0);
  AddTH1(fSameSignFakeCleanElectronClassification, "hSameSignFakeCleanElectronClassification", 
         ";SameSignFakeCleanElectronClassification;Number of Events",  141, -0.5,140.5);
  AddTH1(fSameSignFakeCleanElectronEta, "hSameSignFakeCleanElectronEta", 
         ";SameSignFakeCleanElectronEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fSameSignFakeCleanElectronPhi, "hSameSignFakeCleanElectronPhi", 
         ";SameSignFakeCleanElectronPhi;Number of Events",  25, -3.2,3.2);

  AddTH1(fOppositeSignFakeCleanElectronHOverE, "hOppositeSignFakeCleanElectronHOverE", 
         ";OppositeSignFakeCleanElectronHOverE;Number of Events", 100, 0,0.5);
  AddTH1(fOppositeSignFakeCleanElectronESuperClusterOverP, "hOppositeSignFakeCleanElectronESuperClusterOverP", 
         ";OppositeSignFakeCleanElectronESuperClusterOverP;Number of Events", 100, 0,2.0);
  AddTH1(fOppositeSignFakeCleanElectronSigmaEtaEta, "hOppositeSignFakeCleanElectronSigmaEtaEta", 
         ";OppositeSignFakeCleanElectronSigmaEtaEta;Number of Events", 100, 0,0.1);
  AddTH1(fOppositeSignFakeCleanElectronSigmaiEtaiEta, "hOppositeSignFakeCleanElectronSigmaiEtaiEta", 
         ";OppositeSignFakeCleanElectronSigmaiEtaiEta;Number of Events", 100, 0,0.01);
  AddTH1(fOppositeSignFakeCleanElectronDeltaPhi, "hOppositeSignFakeCleanElectronDeltaPhi", 
         ";OppositeSignFakeCleanElectronDeltaPhi;Number of Events", 100, 0,0.3);
  AddTH1(fOppositeSignFakeCleanElectronDeltaEta, "hOppositeSignFakeCleanElectronDeltaEta", 
         ";OppositeSignFakeCleanElectronDeltaEta;Number of Events", 100, 0,0.1);
  AddTH1(fOppositeSignFakeCleanElectronfBrem, "hOppositeSignFakeCleanElectronfBrem", 
         ";OppositeSignFakeCleanElectronfBrem;Number of Events",  100, 0.0,1.0);
  AddTH1(fOppositeSignFakeCleanElectronClassification, "hOppositeSignFakeCleanElectronClassification", 
         ";OppositeSignFakeCleanElectronClassification;Number of Events",  141, -0.5,140.5);
  AddTH1(fOppositeSignFakeCleanElectronEta, "hOppositeSignFakeCleanElectronEta", 
         ";OppositeSignFakeCleanElectronEta;Number of Events",  25, -5.0,5.0);
  AddTH1(fOppositeSignFakeCleanElectronPhi, "hOppositeSignFakeCleanElectronPhi", 
         ";OppositeSignFakeCleanElectronPhi;Number of Events",  25, -3.2,3.2);



}

//--------------------------------------------------------------------------------------------------
void ElectronIDChargeCorrelationStudyMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and
  //output debug info or not   

  LoadBranch(fMCPartBranchName);
  LoadBranch(fGenJetBranchName);
  LoadBranch(fElectronBranchName);
  LoadBranch(fTrackBranchName);
  LoadBranch(fGsfTrackBranchName);
  LoadBranch(fJetBranchName);
  LoadBranch(fBarrelSuperClusterBranchName);
  LoadBranch(fEndcapSuperClusterBranchName);

  //get monte carlo collections
  const MCParticleCol *GenLeptons = 0;
  if (!fMCLeptonsName.IsNull())
    GenLeptons = GetObjThisEvt<MCParticleCol>(fMCLeptonsName);
  const MCParticleCol *GenTaus = 0;
  if (!fMCTausName.IsNull())
    GenTaus = GetObjThisEvt<MCParticleCol>(fMCTausName);
  ObjArray<MCParticle> *GenLeptonsAndTaus = new ObjArray<MCParticle>;
  for (UInt_t i=0; i<GenLeptons->GetEntries(); i++)
    GenLeptonsAndTaus->Add(GenLeptons->At(i));
  for (UInt_t i=0; i<GenTaus->GetEntries(); i++)
    GenLeptonsAndTaus->Add(GenTaus->At(i));

  // get input clean object collections
  const ElectronCol *CleanElectrons = 0;
  if (!fCleanElectronsName.IsNull())
    CleanElectrons = GetObjThisEvt<ElectronCol>(fCleanElectronsName);
  const JetCol *GoodJets = 0;
  if (!fGoodJetsName.IsNull())
    GoodJets = GetObjThisEvt<JetCol>(fGoodJetsName);


  //Combine Barrel and Endcap superclusters into the same ObjArray
  ObjArray <SuperCluster> *SuperClusters = new ObjArray <SuperCluster>;
  for (UInt_t i=0; i<fBarrelSuperClusters->GetEntries(); i++) {
    SuperClusters->Add(fBarrelSuperClusters->At(i));
  }
  for (UInt_t i=0; i<fEndcapSuperClusters->GetEntries(); i++) {
    SuperClusters->Add(fEndcapSuperClusters->At(i));
  }

  ObjArray<Track> *DenominatorGsfTracks = new ObjArray<Track>;
  ObjArray<Track> *FrameworkMatchedGsfTracks = new ObjArray<Track>;
  ObjArray<Track> *DeltaRMatchedGsfTracks = new ObjArray<Track>;



  //count GenJets above pt > 20GeV.
  Int_t NGenJets = 0;
  for (UInt_t i=0;i<fGenJets->GetEntries();i++) {
    if (fGenJets->At(i)->Pt() >= 20.0) 
      NGenJets++;
  }
  
  //***********************************************************************************************
  //Charge CorrelationStudy Between W and the jet
  //***********************************************************************************************  
  if (GenLeptons->GetEntries() == 1 && NGenJets == 1) {
                
    //***********************************************************************************************
    //Find Reco Electrons Matching to GenJets
    //***********************************************************************************************  
    for (UInt_t i=0; i<fElectrons->GetEntries(); i++) {
      const Electron *ele = fElectrons->At(i);
      
      //Veto denominators matching to real genleptons
      Bool_t IsMCLepton = false;
      for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
        if (MathUtils::DeltaR(ele->Phi(), ele->Eta(),
                              GenLeptonsAndTaus->At(l)->Phi(), 
                              GenLeptonsAndTaus->At(l)->Eta()) < 0.3) {
          IsMCLepton = true;
        }
      }
      
      if (ele->Pt() > 10.0 && !(fVetoGeneratorLeptons && IsMCLepton)
        ) {
        double sigmaetaeta = ele->CovEtaEta();
        if (ele->Eta() < 1.479) sigmaetaeta = sigmaetaeta - 0.02*(fabs(ele->Eta()) - 2.3);
        double fBrem = (ele->PIn() - ele->POut()) / ele->PIn();

        if (GenLeptons->At(0)->Charge() == ele->Charge()) {
          fSameSignFakeElectronHOverE->Fill(ele->HadronicOverEm());
          fSameSignFakeElectronESuperClusterOverP->Fill(ele->ESuperClusterOverP());
          fSameSignFakeElectronSigmaEtaEta->Fill(sigmaetaeta);
          fSameSignFakeElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
          fSameSignFakeElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
          fSameSignFakeElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
          fSameSignFakeElectronClassification->Fill(ele->Classification());
          fSameSignFakeElectronfBrem->Fill(fBrem);
          fSameSignFakeElectronEta->Fill(ele->Eta());
          fSameSignFakeElectronPhi->Fill(ele->Phi());

        } else {
          fOppositeSignFakeElectronHOverE->Fill(ele->HadronicOverEm());
          fOppositeSignFakeElectronESuperClusterOverP->Fill(ele->ESuperClusterOverP());
          fOppositeSignFakeElectronSigmaEtaEta->Fill(sigmaetaeta);
          fOppositeSignFakeElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
          fOppositeSignFakeElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
          fOppositeSignFakeElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
          fOppositeSignFakeElectronClassification->Fill(ele->Classification());
          fOppositeSignFakeElectronfBrem->Fill(fBrem);
          fOppositeSignFakeElectronEta->Fill(ele->Eta());
          fOppositeSignFakeElectronPhi->Fill(ele->Phi());

        }    

        if (ele->TrackIsolation() < 5.0) {
          if (GenLeptons->At(0)->Charge() == ele->Charge()) {
            fSameSignFakeIsolatedElectronHOverE->Fill(ele->HadronicOverEm());
            fSameSignFakeIsolatedElectronESuperClusterOverP->Fill(ele->ESuperClusterOverP());
            fSameSignFakeIsolatedElectronSigmaEtaEta->Fill(sigmaetaeta);
            fSameSignFakeIsolatedElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
            fSameSignFakeIsolatedElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
            fSameSignFakeIsolatedElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
            fSameSignFakeIsolatedElectronClassification->Fill(ele->Classification());
            fSameSignFakeIsolatedElectronfBrem->Fill(fBrem);
            fSameSignFakeIsolatedElectronEta->Fill(ele->Eta());
            fSameSignFakeIsolatedElectronPhi->Fill(ele->Phi());
          } else {
            fOppositeSignFakeIsolatedElectronHOverE->Fill(ele->HadronicOverEm());
            fOppositeSignFakeIsolatedElectronESuperClusterOverP->Fill(ele->ESuperClusterOverP());
            fOppositeSignFakeIsolatedElectronSigmaEtaEta->Fill(sigmaetaeta);
            fOppositeSignFakeIsolatedElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
            fOppositeSignFakeIsolatedElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
            fOppositeSignFakeIsolatedElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
            fOppositeSignFakeIsolatedElectronClassification->Fill(ele->Classification());
            fOppositeSignFakeIsolatedElectronfBrem->Fill(fBrem);
            fOppositeSignFakeIsolatedElectronEta->Fill(ele->Eta());
            fOppositeSignFakeIsolatedElectronPhi->Fill(ele->Phi());
          }    
        }    
      }
    } //loop over RECO electrons.    


  //***********************************************************************************************
  //Find Clean Electrons Matching to GenJets
  //***********************************************************************************************  
  for (UInt_t i=0; i<CleanElectrons->GetEntries(); i++) {
    const Electron *ele = CleanElectrons->At(i);

    //Veto denominators matching to real genleptons
    Bool_t IsMCLepton = false;
    for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
      if (MathUtils::DeltaR(ele->Phi(), ele->Eta(),
                            GenLeptonsAndTaus->At(l)->Phi(), 
                            GenLeptonsAndTaus->At(l)->Eta()) < 0.3) {
        IsMCLepton = true;
      }
    }
    
    if (ele->Pt() > 10.0 && !(fVetoGeneratorLeptons && IsMCLepton)
      ) {
      double sigmaetaeta = ele->CovEtaEta();
      if (ele->Eta() < 1.479) sigmaetaeta = sigmaetaeta - 0.02*(fabs(ele->Eta()) - 2.3);
      double fBrem = (ele->PIn() - ele->POut()) / ele->PIn();
       if (GenLeptons->At(0)->Charge() == ele->Charge()) {
         fSameSignFakeCleanElectronHOverE->Fill(ele->HadronicOverEm());
         fSameSignFakeCleanElectronESuperClusterOverP->Fill(ele->ESuperClusterOverP());
         fSameSignFakeCleanElectronSigmaEtaEta->Fill(sigmaetaeta);
         fSameSignFakeCleanElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
         fSameSignFakeCleanElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
         fSameSignFakeCleanElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
         fSameSignFakeCleanElectronClassification->Fill(ele->Classification());
         fSameSignFakeCleanElectronfBrem->Fill(fBrem);
         fSameSignFakeCleanElectronEta->Fill(ele->Eta());
         fSameSignFakeCleanElectronPhi->Fill(ele->Phi());
       } else {
         fOppositeSignFakeCleanElectronHOverE->Fill(ele->HadronicOverEm());
         fOppositeSignFakeCleanElectronESuperClusterOverP->Fill(ele->ESuperClusterOverP());
         fOppositeSignFakeCleanElectronSigmaEtaEta->Fill(sigmaetaeta);
         fOppositeSignFakeCleanElectronSigmaiEtaiEta->Fill(ele->CoviEtaiEta());
         fOppositeSignFakeCleanElectronDeltaPhi->Fill(ele->DeltaPhiSuperClusterTrackAtVtx());
         fOppositeSignFakeCleanElectronDeltaEta->Fill(ele->DeltaEtaSuperClusterTrackAtVtx());
         fOppositeSignFakeCleanElectronClassification->Fill(ele->Classification());
         fOppositeSignFakeCleanElectronfBrem->Fill(fBrem);
         fOppositeSignFakeCleanElectronEta->Fill(ele->Eta());
         fOppositeSignFakeCleanElectronPhi->Fill(ele->Phi());
       }    
    }
  } //loop over RECO electrons.


 
  } //if njet == 1, nlepton == 1
}

//--------------------------------------------------------------------------------------------------
void ElectronIDChargeCorrelationStudyMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.

}

//--------------------------------------------------------------------------------------------------
void ElectronIDChargeCorrelationStudyMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
