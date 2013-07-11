// $Id: JetChargeCorrelationStudyMod.cc,v 1.3 2009/03/17 08:33:15 sixie Exp $

#include "MitHiggs/FakeRateMods/interface/JetChargeCorrelationStudyMod.h"
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

ClassImp(mithep::JetChargeCorrelationStudyMod)

//--------------------------------------------------------------------------------------------------
  JetChargeCorrelationStudyMod::JetChargeCorrelationStudyMod(const char *name, const char *title) : 
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
void JetChargeCorrelationStudyMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void JetChargeCorrelationStudyMod::SlaveBegin()
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
  AddTH1(fUpJetFlavorMatchedTrackCharge, "hUpJetFlavorMatchedTrackCharge", 
         ";UpJetFlavorCharge+TrackCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fUpJetFlavorMatchedLooseIsolatedTrackCharge, "hUpJetFlavorMatchedLooseIsolatedTrackCharge", 
         ";UpJetFlavorCharge+TrackCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fUpJetFlavorMatchedTightIsolatedTrackCharge, "hUpJetFlavorMatchedTightIsolatedTrackCharge", 
         ";UpJetFlavorCharge+TrackCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fUpJetFlavorMatchedRecoElectronCharge, "hUpJetFlavorMatchedRecoElectronCharge", 
         ";UpJetFlavorCharge+ElectronCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fUpJetFlavorMatchedLooseElectronCharge, "hUpJetFlavorMatchedLooseElectronCharge", 
         ";UpJetFlavorCharge+ElectronCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fUpJetFlavorMatchedTightElectronCharge, "hUpJetFlavorMatchedTightElectronCharge", 
         ";UpJetFlavorCharge+ElectronCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fUpJetFlavorMatchedCleanElectronCharge, "hUpJetFlavorMatchedCleanElectronCharge", 
         ";UpJetFlavorCharge+ElectronCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fDownJetFlavorMatchedTrackCharge, "hDownJetFlavorMatchedTrackCharge", 
         ";DownJetFlavorCharge+TrackCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fDownJetFlavorMatchedLooseIsolatedTrackCharge, "hDownJetFlavorMatchedLooseIsolatedTrackCharge", 
         ";DownJetFlavorCharge+TrackCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fDownJetFlavorMatchedTightIsolatedTrackCharge, "hDownJetFlavorMatchedTightIsolatedTrackCharge", 
         ";DownJetFlavorCharge+TrackCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fDownJetFlavorMatchedRecoElectronCharge, "hDownJetFlavorMatchedRecoElectronCharge", 
         ";DownJetFlavorCharge+ElectronCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fDownJetFlavorMatchedLooseElectronCharge, "hDownJetFlavorMatchedLooseElectronCharge", 
         ";DownJetFlavorCharge+ElectronCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fDownJetFlavorMatchedTightElectronCharge, "hDownJetFlavorMatchedTightElectronCharge", 
         ";DownJetFlavorCharge+ElectronCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fDownJetFlavorMatchedCleanElectronCharge, "hDownJetFlavorMatchedCleanElectronCharge", 
         ";DownJetFlavorCharge+ElectronCharge;Number of Events", 5, -2.5,2.5);

  AddTH1(fGluonJetMatchedTrackCharge, "hGluonJetMatchedTrackCharge", 
         ";GluonJetCharge+TrackCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fGluonJetMatchedLooseIsolatedTrackCharge, "hGluonJetMatchedLooseIsolatedTrackCharge", 
         ";GluonJetCharge+TrackCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fGluonJetMatchedTightIsolatedTrackCharge, "hGluonJetMatchedTightIsolatedTrackCharge", 
         ";GluonJetCharge+TrackCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fGluonJetMatchedRecoElectronCharge, "hGluonJetMatchedRecoElectronCharge", 
         ";GluonJetCharge+ElectronCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fGluonJetMatchedLooseElectronCharge, "hGluonJetMatchedLooseElectronCharge", 
         ";GluonJetCharge+ElectronCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fGluonJetMatchedTightElectronCharge, "hGluonJetMatchedTightElectronCharge", 
         ";GluonJetCharge+ElectronCharge;Number of Events", 5, -2.5,2.5);
  AddTH1(fGluonJetMatchedCleanElectronCharge, "hGluonJetMatchedCleanElectronCharge", 
         ";GluonJetCharge+ElectronCharge;Number of Events", 5, -2.5,2.5);


  //*******************************************************************************************
  //Charge Correlations between W and jets
  //*******************************************************************************************
  AddTH1(fNGenJets, "hNGenJets", ";NGenJets;Number of Events", 10, -0.5,9.5);

  AddTH1(fPlusWJetFlavor, "hPlusWJetFlavor", 
         ";PlusWJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fPlusWTrackMatchedJetFlavor, "hPlusWTrackMatchedJetFlavor", 
         ";PlusWTrackMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fPlusWLooseIsolatedTrackMatchedJetFlavor, "hPlusWLooseIsolatedTrackMatchedJetFlavor", 
         ";PlusWLooseIsolatedTrackMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fPlusWTightIsolatedTrackMatchedJetFlavor, "hPlusWTightIsolatedTrackMatchedJetFlavor", 
         ";PlusWTightIsolatedTrackMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fPlusWRecoElectronMatchedJetFlavor, "hPlusWRecoElectronMatchedJetFlavor", 
         ";PlusWRecoElectronMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fPlusWLooseElectronMatchedJetFlavor, "hPlusWLooseElectronMatchedJetFlavor", 
         ";PlusWLooseElectronMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fPlusWTightElectronMatchedJetFlavor, "hPlusWTightElectronMatchedJetFlavor", 
         ";PlusWTightElectronMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fPlusWCleanElectronMatchedJetFlavor, "hPlusWCleanElectronMatchedJetFlavor", 
         ";PlusWCleanElectronMatchedJetFlavor;Number of Events", 32, -9.5,22.5);

  AddTH1(fMinusWJetFlavor, "hMinusWJetFlavor", 
         ";MinusWJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fMinusWTrackMatchedJetFlavor, "hMinusWTrackMatchedJetFlavor", 
         ";MinusWTrackMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fMinusWLooseIsolatedTrackMatchedJetFlavor, "hMinusWLooseIsolatedTrackMatchedJetFlavor", 
         ";MinusWLooseIsolatedTrackMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fMinusWTightIsolatedTrackMatchedJetFlavor, "hMinusWTightIsolatedTrackMatchedJetFlavor", 
         ";MinusWTightIsolatedTrackMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fMinusWRecoElectronMatchedJetFlavor, "hMinusWRecoElectronMatchedJetFlavor", 
         ";MinusWRecoElectronMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fMinusWLooseElectronMatchedJetFlavor, "hMinusWLooseElectronMatchedJetFlavor", 
         ";MinusWLooseElectronMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fMinusWTightElectronMatchedJetFlavor, "hMinusWTightElectronMatchedJetFlavor", 
         ";MinusWTightElectronMatchedJetFlavor;Number of Events", 32, -9.5,22.5);
  AddTH1(fMinusWCleanElectronMatchedJetFlavor, "hMinusWCleanElectronMatchedJetFlavor", 
         ";MinusWCleanElectronMatchedJetFlavor;Number of Events", 32, -9.5,22.5);



}

//--------------------------------------------------------------------------------------------------
void JetChargeCorrelationStudyMod::Process()
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
  fNGenJets->Fill(NGenJets);
  
  //***********************************************************************************************
  //Charge CorrelationStudy Between W and the jet
  //***********************************************************************************************  
  if (GenLeptons->GetEntries() == 1 && NGenJets == 1) {
    
    if (MathUtils::DeltaR(fGenJets->At(0)->Phi(), fGenJets->At(0)->Eta(),
                          GenLeptons->At(0)->Phi(), 
                          GenLeptons->At(0)->Eta())>= 0.3) {
      if (GenLeptons->At(0)->Charge() > 0)
        fPlusWJetFlavor->Fill(fGenJets->At(0)->MatchedMCFlavor());
      else 
        fMinusWJetFlavor->Fill(fGenJets->At(0)->MatchedMCFlavor());
    }

    //***********************************************************************************************
    //Find Track TrackDenominator Objects for Electrons
    //Isolated Tracks
    //***********************************************************************************************  
    for (UInt_t i=0; i<fTracks->GetEntries(); i++) {
      const Track *track = fTracks->At(i);
      
      Double_t trackIsolation = IsolationTools::TrackIsolation(track, 0.3, 0.015, 1.0, 
                                                               1000.0, fTracks);
      //Veto denominators matching to real genleptons
      Bool_t IsMCLepton = false;
      for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
        if (MathUtils::DeltaR(track->Phi(), track->Eta(),
                              GenLeptonsAndTaus->At(l)->Phi(), 
                              GenLeptonsAndTaus->At(l)->Eta()) < 0.3) {
          IsMCLepton = true;
        }
      }
      
      //define denominator cuts
      if (track->Pt() > 10.0 && !(fVetoGeneratorLeptons && IsMCLepton)
        ) {
        
        //try to match it to a genjet
        Double_t minDR = 5000.0;
        const GenJet *matchedGenJet = NULL;
        for(UInt_t j=0;j<fGenJets->GetEntries();j++) {
          Double_t DR = MathUtils::DeltaR(track->Phi(), track->Eta(), fGenJets->At(j)->Phi(),
                                          fGenJets->At(j)->Eta());
          if (DR < minDR && DR < 0.5) {
            minDR = DR;
            matchedGenJet = fGenJets->At(j);
          }
        }
        
        //if it matched
        if (matchedGenJet) {
          
          MDB(kAnalysis, 5) {
            cout << "track matched jet flavor: " << matchedGenJet->MatchedMCFlavor()  << endl;
          }
          
          if (GenLeptons->At(0)->Charge() > 0)
            fPlusWTrackMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
          else 
            fMinusWTrackMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
          
          if (trackIsolation < 10.0) {
            if (GenLeptons->At(0)->Charge() > 0)
              fPlusWLooseIsolatedTrackMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
            else 
              fMinusWLooseIsolatedTrackMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());          
          }
          if (trackIsolation < 5.0) {
            if (GenLeptons->At(0)->Charge() > 0)
              fPlusWTightIsolatedTrackMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
            else 
              fMinusWTightIsolatedTrackMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());  
          }
        }
      } //passes track cuts    
    } //loop over all tracks
    
    
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
        
        //try to match it to a genjet
        Double_t minDR = 5000.0;
        const GenJet *matchedGenJet = NULL;
        for(UInt_t j=0;j<fGenJets->GetEntries();j++) {
          Double_t DR = MathUtils::DeltaR(ele->Phi(), ele->Eta(), fGenJets->At(j)->Phi(),
                                          fGenJets->At(j)->Eta());
          if (DR < minDR && DR < 0.5) {
            minDR = DR;
            matchedGenJet = fGenJets->At(j);
          }
        }
         
        //if it matched
        if (matchedGenJet) {
          
          MDB(kAnalysis, 5) {
            cout << "reco electron matched jet flavor: " << matchedGenJet->MatchedMCFlavor()  << endl;
          }
          
          if (GenLeptons->At(0)->Charge() > 0)
            fPlusWRecoElectronMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
          else 
            fMinusWRecoElectronMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
          
          if (ele->PassLooseID()) {
            if (GenLeptons->At(0)->Charge() > 0)
              fPlusWLooseElectronMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
            else 
              fMinusWLooseElectronMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
          }
          if (ele->PassTightID()) {              
            if (GenLeptons->At(0)->Charge() > 0)
              fPlusWTightElectronMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
            else 
              fMinusWTightElectronMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());          
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
        
        //try to match it to a genjet
        Double_t minDR = 5000.0;
        const GenJet *matchedGenJet = NULL;
        for(UInt_t j=0;j<fGenJets->GetEntries();j++) {
          Double_t DR = MathUtils::DeltaR(ele->Phi(), ele->Eta(), fGenJets->At(j)->Phi(),
                                          fGenJets->At(j)->Eta());
          if (DR < minDR && DR < 0.5) {
            minDR = DR;
            matchedGenJet = fGenJets->At(j);
          }
        }
        
        //if it matched
        if (matchedGenJet) {
          
          MDB(kAnalysis, 5) {
            cout << "clean electron matched jet flavor: " << matchedGenJet->MatchedMCFlavor()  << endl;
          }
          if (GenLeptons->At(0)->Charge() > 0)
            fPlusWCleanElectronMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());
          else 
            fMinusWCleanElectronMatchedJetFlavor->Fill(matchedGenJet->MatchedMCFlavor());       
        }
      }
    } //loop over RECO electrons.
  }


  //***********************************************************************************************
  //Charge Correlation Study Between quark and leading track
  //***********************************************************************************************  

  //***********************************************************************************************
  //Find Track TrackDenominator Objects for Electrons
  //Isolated Tracks
  //***********************************************************************************************  
  for (UInt_t i=0; i<fTracks->GetEntries(); i++) {
    const Track *track = fTracks->At(i);

    Double_t trackIsolation = IsolationTools::TrackIsolation(track, 0.3, 0.015, 1.0, 
                                                             1000.0, fTracks);
    //Veto denominators matching to real genleptons
    Bool_t IsMCLepton = false;
    for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
      if (MathUtils::DeltaR(track->Phi(), track->Eta(),
                            GenLeptonsAndTaus->At(l)->Phi(), 
                            GenLeptonsAndTaus->At(l)->Eta()) < 0.3) {
        IsMCLepton = true;
      }
    }
    
    //define denominator cuts
    if (track->Pt() > 10.0 && !(fVetoGeneratorLeptons && IsMCLepton)
      ) {
     
      //try to match it to a genjet
      Double_t minDR = 5000.0;
      const GenJet *matchedGenJet = NULL;
      for(UInt_t j=0;j<fGenJets->GetEntries();j++) {
        Double_t DR = MathUtils::DeltaR(track->Phi(), track->Eta(), fGenJets->At(j)->Phi(),
                                        fGenJets->At(j)->Eta());
        if (DR < minDR && DR < 0.5) {
          minDR = DR;
          matchedGenJet = fGenJets->At(j);
        }
      }

      //if it matched
      if (matchedGenJet) {
        
        MDB(kAnalysis, 5) {
          cout << "track matched jet flavor: " << matchedGenJet->MatchedMCFlavor()  << endl;
        }
        Int_t JetChargeSign = 0;
        Int_t ChargeCode = 0;
        if (matchedGenJet->MatchedMCFlavor() == 21) {
          fGluonJetMatchedTrackCharge->Fill(track->Charge());
          if (trackIsolation < 10.0)
            fGluonJetMatchedLooseIsolatedTrackCharge->Fill(track->Charge());
          if (trackIsolation < 5.0)
            fGluonJetMatchedTightIsolatedTrackCharge->Fill(track->Charge());    
        }  else if (matchedGenJet->MatchedMCFlavor() != 0 ) {
          JetChargeSign = (matchedGenJet->MatchedMCFlavor() > 0) ? 1 : -1 ;
          if (track->Charge() + JetChargeSign == 0) {
            ChargeCode = 2*JetChargeSign + track->Charge();
          } else {
            ChargeCode = track->Charge() + JetChargeSign;
          }

          if (fabs(matchedGenJet->MatchedMCFlavor()) == 2 || fabs(matchedGenJet->MatchedMCFlavor()) == 4) {
            fUpJetFlavorMatchedTrackCharge->Fill(ChargeCode);
          } else if (fabs(matchedGenJet->MatchedMCFlavor()) == 1 ||
                     fabs(matchedGenJet->MatchedMCFlavor()) == 3 || 
                     fabs(matchedGenJet->MatchedMCFlavor()) == 5 ) {
            fDownJetFlavorMatchedTrackCharge->Fill(ChargeCode);
          }
          
          if (trackIsolation < 10.0) {
            if (fabs(matchedGenJet->MatchedMCFlavor()) == 2 || fabs(matchedGenJet->MatchedMCFlavor()) == 4) {
              fUpJetFlavorMatchedLooseIsolatedTrackCharge->Fill(ChargeCode);
            } else if (fabs(matchedGenJet->MatchedMCFlavor()) == 1 ||
                       fabs(matchedGenJet->MatchedMCFlavor()) == 3 || 
                     fabs(matchedGenJet->MatchedMCFlavor()) == 5 ) {
              fDownJetFlavorMatchedLooseIsolatedTrackCharge->Fill(ChargeCode);
            }
          }
          if (trackIsolation < 5.0) {
            if (fabs(matchedGenJet->MatchedMCFlavor()) == 2 || fabs(matchedGenJet->MatchedMCFlavor()) == 4) {
              fUpJetFlavorMatchedTightIsolatedTrackCharge->Fill(ChargeCode);       
            } else if (fabs(matchedGenJet->MatchedMCFlavor()) == 1 ||
                       fabs(matchedGenJet->MatchedMCFlavor()) == 3 || 
                       fabs(matchedGenJet->MatchedMCFlavor()) == 5 ) {
              fDownJetFlavorMatchedTightIsolatedTrackCharge->Fill(ChargeCode);
            }
          }
        }
      }

    } //pass track denominator cuts
  } //loop over all tracks


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

      //try to match it to a genjet
      Double_t minDR = 5000.0;
      const GenJet *matchedGenJet = NULL;
      for(UInt_t j=0;j<fGenJets->GetEntries();j++) {
        Double_t DR = MathUtils::DeltaR(ele->Phi(), ele->Eta(), fGenJets->At(j)->Phi(),
                                        fGenJets->At(j)->Eta());
        if (DR < minDR && DR < 0.5) {
          minDR = DR;
          matchedGenJet = fGenJets->At(j);
        }
      }

      //if it matched
      if (matchedGenJet) {
        
        MDB(kAnalysis, 5) {
          cout << "reco electron matched jet flavor: " << matchedGenJet->MatchedMCFlavor()  << endl;
        }
        Int_t JetChargeSign = 0;
        Int_t ChargeCode = 0;
        if (matchedGenJet->MatchedMCFlavor() == 21) {
          fGluonJetMatchedRecoElectronCharge->Fill(ele->Charge());
          if (ele->PassLooseID())
            fGluonJetMatchedLooseElectronCharge->Fill(ele->Charge());
          if (ele->PassTightID())
            fGluonJetMatchedTightElectronCharge->Fill(ele->Charge());       
        }  else if (matchedGenJet->MatchedMCFlavor() != 0 ) {
          JetChargeSign = (matchedGenJet->MatchedMCFlavor() > 0) ? 1 : -1 ;
          if (ele->Charge() + JetChargeSign == 0) {
            ChargeCode = 2*JetChargeSign + ele->Charge();
          } else {
            ChargeCode = ele->Charge() + JetChargeSign;
          }
          if (fabs(matchedGenJet->MatchedMCFlavor()) == 2 || fabs(matchedGenJet->MatchedMCFlavor()) == 4) {
            fUpJetFlavorMatchedRecoElectronCharge->Fill(ChargeCode);
          } else if (fabs(matchedGenJet->MatchedMCFlavor()) == 1 ||
                     fabs(matchedGenJet->MatchedMCFlavor()) == 3 || 
                     fabs(matchedGenJet->MatchedMCFlavor()) == 5 ) {
            fDownJetFlavorMatchedRecoElectronCharge->Fill(ChargeCode);
          }
                    
          if (ele->PassLooseID()) {
            if (fabs(matchedGenJet->MatchedMCFlavor()) == 2 || fabs(matchedGenJet->MatchedMCFlavor()) == 4) {
              fUpJetFlavorMatchedLooseElectronCharge->Fill(ChargeCode);
            } else if (fabs(matchedGenJet->MatchedMCFlavor()) == 1 ||
                       fabs(matchedGenJet->MatchedMCFlavor()) == 3 || 
                       fabs(matchedGenJet->MatchedMCFlavor()) == 5 ) {
              fDownJetFlavorMatchedLooseElectronCharge->Fill(ChargeCode);
            }
          }
          if (ele->PassTightID()) {              
            if (fabs(matchedGenJet->MatchedMCFlavor()) == 2 || fabs(matchedGenJet->MatchedMCFlavor()) == 4) {
              fUpJetFlavorMatchedTightElectronCharge->Fill(ChargeCode); 
            } else if (fabs(matchedGenJet->MatchedMCFlavor()) == 1 ||
                       fabs(matchedGenJet->MatchedMCFlavor()) == 3 || 
                       fabs(matchedGenJet->MatchedMCFlavor()) == 5 ) {
              fDownJetFlavorMatchedTightElectronCharge->Fill(ChargeCode); 
            }
          }
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

      //try to match it to a genjet
      Double_t minDR = 5000.0;
      const GenJet *matchedGenJet = NULL;
      for(UInt_t j=0;j<fGenJets->GetEntries();j++) {
        Double_t DR = MathUtils::DeltaR(ele->Phi(), ele->Eta(), fGenJets->At(j)->Phi(),
                                        fGenJets->At(j)->Eta());
        if (DR < minDR && DR < 0.5) {
          minDR = DR;
          matchedGenJet = fGenJets->At(j);
        }
      }

      //if it matched
      if (matchedGenJet) {
        
        MDB(kAnalysis, 5) {
          cout << "clean electron matched jet flavor: " << matchedGenJet->MatchedMCFlavor()  << endl;
        }
        Int_t JetChargeSign = 0;
        Int_t ChargeCode = 0;
        if (matchedGenJet->MatchedMCFlavor() == 21) {
          fGluonJetMatchedCleanElectronCharge->Fill(ele->Charge());          
        }  else if (matchedGenJet->MatchedMCFlavor() != 0 ) {
          JetChargeSign = (matchedGenJet->MatchedMCFlavor() > 0) ? 1 : -1 ;
          if (ele->Charge() + JetChargeSign == 0) {
            ChargeCode = 2*JetChargeSign + ele->Charge();
          } else {
            ChargeCode = ele->Charge() + JetChargeSign;
          }

          if (fabs(matchedGenJet->MatchedMCFlavor()) == 2 || fabs(matchedGenJet->MatchedMCFlavor()) == 4) {
            fUpJetFlavorMatchedCleanElectronCharge->Fill(ChargeCode);                                   
          } else if (fabs(matchedGenJet->MatchedMCFlavor()) == 1 ||
                     fabs(matchedGenJet->MatchedMCFlavor()) == 3 || 
                     fabs(matchedGenJet->MatchedMCFlavor()) == 5 ) {
            fDownJetFlavorMatchedCleanElectronCharge->Fill(ChargeCode);                                   
          }
        }
      }
    }
  } //loop over RECO electrons.



}

//--------------------------------------------------------------------------------------------------
void JetChargeCorrelationStudyMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.

}

//--------------------------------------------------------------------------------------------------
void JetChargeCorrelationStudyMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
