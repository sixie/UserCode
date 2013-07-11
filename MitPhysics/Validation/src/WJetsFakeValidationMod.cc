 // $Id: WJetsFakeValidationMod.cc,v 1.5 2009/06/30 08:58:58 phedex Exp $

#include "MitPhysics/Validation/interface/WJetsFakeValidationMod.h"
#include <TH1D.h>
#include <TH2D.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitPhysics/Utils/interface/GeneratorTools.h"
#include "MitPhysics/Utils/interface/IsolationTools.h"

#include "MitPhysics/Init/interface/FakeObject.h"
#include "MitPhysics/Init/interface/FakeEventHeader.h"

using namespace mithep;
ClassImp(mithep::WJetsFakeValidationMod)

//--------------------------------------------------------------------------------------------------
WJetsFakeValidationMod::WJetsFakeValidationMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fUseMCFake(false),
  fPerformFakeMuonMetCorrection(true),
  fOSFakerateCorrection(1.0),
  fSSFakerateCorrection(1.0),
  fSampleName("NotSet"),
  fFakeEventHeaderName(ModNames::gkFakeEventHeadersName),
  fElectronFakeableObjectsName(ModNames::gkElectronFakeableObjectsName),
  fMuonFakeableObjectsName(ModNames::gkMuonFakeableObjectsName),
  fMCPartBranchName(Names::gkMCPartBrn),
  fGenJetBranchName(Names::gkSC5GenJetBrn),
  fTrackBranchName(Names::gkTrackBrn),
  fMuonBranchName(Names::gkMuonBrn),
  fMetName("NotSet"),
  fCleanJetsName(ModNames::gkCleanJetsName),
  fTriggerObjectsName("NotSpecified"),
  fParticles(0),
  fGenJets(0),
  fTracks(0),
  fMuons(0),
  fMet(0)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void WJetsFakeValidationMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void WJetsFakeValidationMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and  
  LoadBranch(fMCPartBranchName);
  LoadBranch(fGenJetBranchName);
  LoadBranch(fTrackBranchName);
  LoadBranch(fMuonBranchName);

  bool printdebug = false;
//***********************************************************************************************
//Import Collections
//***********************************************************************************************

  //Obtain all the good objects from the event cleaning module
  const ElectronCol *CleanElectrons = GetObjThisEvt<ElectronCol>(ModNames::gkCleanElectronsName);
  ObjArray<Muon> *CleanMuons = dynamic_cast<ObjArray<Muon>* >
    (FindObjThisEvt(ModNames::gkCleanMuonsName));
  mithep::ParticleOArr *CleanLeptons = dynamic_cast<mithep::ParticleOArr*>
    (FindObjThisEvt(ModNames::gkMergedLeptonsName));

  //Get Met
  if (!fMetName.IsNull()) {
    fMet = GetObjThisEvt<MetCol>(fMetName);
  } 
  const Met *originalCaloMet = 0;
  if (fMet) {
    originalCaloMet = fMet->At(0);
  } else {
    cout << "Error: Met Collection " << fMetName << " could not be loaded.\n";
  }
  ObjArray<Jet> *OriginalCleanJets = dynamic_cast<ObjArray<Jet>* > 
    (FindObjThisEvt(fCleanJetsName.Data()));

  //Obtain the collection of fake objects
  ElectronCol *ElectronFakeableObjects = 0;
  if(!fElectronFakeableObjectsName.IsNull())
    ElectronFakeableObjects = dynamic_cast<ElectronCol* >
      (FindObjThisEvt(fElectronFakeableObjectsName.Data()));
  MuonCol *MuonFakeableObjects = 0;
  if (!fMuonFakeableObjectsName.IsNull())
    MuonFakeableObjects = dynamic_cast<MuonCol* >
      (FindObjThisEvt(fMuonFakeableObjectsName.Data()));
  ChargedParticleOArr *FakeableObjects = new ChargedParticleOArr;
  if (ElectronFakeableObjects) {
    for (UInt_t i=0; i<ElectronFakeableObjects->GetEntries(); i++)
      FakeableObjects->Add(ElectronFakeableObjects->At(i));
  }
  if (MuonFakeableObjects) {
    for (UInt_t i=0; i<MuonFakeableObjects->GetEntries(); i++)
      FakeableObjects->Add(MuonFakeableObjects->At(i));  
  }
  Collection<FakeEventHeader> *FakeEventHeaders = 0;
  if (!fUseMCFake) {
    if (!fFakeEventHeaderName.IsNull()) {
      FakeEventHeaders = dynamic_cast<Collection<FakeEventHeader>* >(FindObjThisEvt(fFakeEventHeaderName.Data()));
      if (!FakeEventHeaders) {
        cout << "Error: FakeEventHeader with name  " << fFakeEventHeaderName.Data() << " could not be loaded.\n";
        assert(false);
      }
    } else 
      cout << "Error: FakeEventHeaders  " << fFakeEventHeaderName.Data() << " could not be loaded.\n";
  }  

  //Load Trigger Objects
  const TriggerObjectCol *triggerObjects = GetHLTObjects(fTriggerObjectsName);
  if (!triggerObjects) {
    cout << "Error: Could not load Trigger Object Collection with name " << fTriggerObjectsName << endl;
  }

  //get monte carlo collections
  const MCParticleCol *GenLeptons = 0;
  GenLeptons = GetObjThisEvt<MCParticleCol>(ModNames::gkMCLeptonsName);
  const MCParticleCol *GenPhotons = 0;
  GenPhotons = GetObjThisEvt<MCParticleCol>(ModNames::gkMCPhotonsName);
  const MCParticleCol *GenTaus = 0;  
  GenTaus = GetObjThisEvt<MCParticleCol>(ModNames::gkMCTausName);
  ObjArray<MCParticle> *GenLeptonsAndTaus = new ObjArray<MCParticle>;
  for (UInt_t i=0; i<GenLeptons->GetEntries(); i++)
    GenLeptonsAndTaus->Add(GenLeptons->At(i));
  for (UInt_t i=0; i<GenTaus->GetEntries(); i++)
    GenLeptonsAndTaus->Add(GenTaus->At(i));


  //***********************************************************************************************
  //Reject W+gamma / Z+gamma Events
  //***********************************************************************************************
  if(string(fSampleName.Data()) == "s8-wjets-mg-id11" 
     || string(fSampleName.Data()) == "s8-vqq-mg-id11"
     || string(fSampleName.Data()) == "s8-zjets-mg-id11"
    ) {
    Bool_t FoundWGammaEvent = false;
    for (UInt_t i=0; i<GenPhotons->GetEntries(); i++) {  
      if (GenPhotons->At(i)->Pt() > 10.0) {
        
        //ISR Photon
        if (GenPhotons->At(i)->Mother() && GenPhotons->At(i)->Mother()->IsParton()
            || (GenPhotons->At(i)->Mother()->AbsPdgId() == 22 
                && GenPhotons->At(i)->Mother()->Status() == 3  
                && GenPhotons->At(i)->Mother()->Mother() 
                && GenPhotons->At(i)->Mother()->Mother()->IsParton())
          ) {
          FoundWGammaEvent = true;    
        }
        
        //WWgamma vertex  
        if ((GenPhotons->At(i)->Mother() && GenPhotons->At(i)->Mother()->AbsPdgId() == 24) 
            || 
            (GenPhotons->At(i)->Mother()->AbsPdgId() == 22 
             && GenPhotons->At(i)->Mother()->Status() == 3
             && GenPhotons->At(i)->Mother()->Mother()
             && GenPhotons->At(i)->Mother()->Mother()->AbsPdgId() == 24
              )
          ) {
          FoundWGammaEvent = true;
        }
        
        //Pythia FSR
        if (GenPhotons->At(i)->Mother() && GenPhotons->At(i)->Mother()->Status() == 3
            && (GenPhotons->At(i)->Mother()->AbsPdgId() == 11 
                || GenPhotons->At(i)->Mother()->AbsPdgId() == 13
                || GenPhotons->At(i)->Mother()->AbsPdgId() == 15)
          ) {
          FoundWGammaEvent = true;
        }      
      } 
    }
    if (FoundWGammaEvent) {
      delete FakeableObjects;
      delete GenLeptonsAndTaus;
      return;
    }
  }
  
  //***********************************************************************************************
  //Do Madgraph Vs pythia Normalization study
  //***********************************************************************************************
  Int_t NCleanElectrons = 0;
  Int_t NDenominators = 0;
  for (UInt_t i=0; i<CleanElectrons->GetEntries(); i++) {    
    if (CleanElectrons->At(i)->Pt() > 10) {
      NCleanElectrons++;
      //Veto numerators matching to real electrons      
      Bool_t IsMCLepton = false;
      for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
        if (MathUtils::DeltaR(CleanElectrons->At(i)->Phi(), CleanElectrons->At(i)->Eta(),
                              GenLeptonsAndTaus->At(l)->Phi(), 
                              GenLeptonsAndTaus->At(l)->Eta()) < 0.1) {
          IsMCLepton = true;
        }
      }    
      if (IsMCLepton) {
        fRealElectronPt->Fill(CleanElectrons->At(i)->Pt());
        fRealElectronEta->Fill(CleanElectrons->At(i)->Eta());
        fRealElectronPhi->Fill(CleanElectrons->At(i)->Phi());
      }
      if (i==0) {
        fElectron1Pt->Fill(CleanElectrons->At(i)->Pt());
        fElectron1Eta->Fill(CleanElectrons->At(i)->Eta());
        fElectron1Phi->Fill(CleanElectrons->At(i)->Phi());
      }
    }
  }
  for (UInt_t i=0;i<ElectronFakeableObjects->GetEntries() ; i++) {
    if (ElectronFakeableObjects->At(i)->Pt() > 10.0) {
      NDenominators++;
    }
  }
  fNCleanElectrons->Fill(NCleanElectrons);
  fNDenominators->Fill(NDenominators);
  fNCleanElectronsVsNDenominators->Fill(NCleanElectrons,NDenominators);

  //***********************************************************************************************
  //Find Numerator Objects for Electrons
  //Numerators are basically Clean Electrons
  //***********************************************************************************************
  for (UInt_t i=0; i<CleanElectrons->GetEntries(); i++) {  
    
    //Veto numerators matching to real electrons      
    Bool_t IsMCLepton = false;
    for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
      if (MathUtils::DeltaR(CleanElectrons->At(i)->Phi(), CleanElectrons->At(i)->Eta(),
                            GenLeptonsAndTaus->At(l)->Phi(), 
                            GenLeptonsAndTaus->At(l)->Eta()) < 0.1) {
        IsMCLepton = true;
      }
    }    
    
    if (CleanElectrons->At(i)->Pt() > 10.0
        && !IsMCLepton) {
      fElectronNumeratorPt->Fill(CleanElectrons->At(i)->Pt());
      fElectronNumeratorEta->Fill(CleanElectrons->At(i)->Eta());
      fElectronNumeratorPhi->Fill(CleanElectrons->At(i)->Phi());        
    }
  }
  
  for (UInt_t i=0; i<CleanMuons->GetEntries(); i++) {  
    
    const Track *muonTrack =       muonTrack = CleanMuons->At(i)->Trk();
    if (CleanMuons->At(i)->HasTrackerTrk())
      muonTrack = CleanMuons->At(i)->TrackerTrk();
    
    //Veto denominators matching to real muons
    Bool_t IsMCLepton = false;
    for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
      if (MathUtils::DeltaR(muonTrack->Phi(), 
                            muonTrack->Eta(),
                            GenLeptonsAndTaus->At(l)->Phi(), 
                            GenLeptonsAndTaus->At(l)->Eta()) < 0.1) {
        IsMCLepton = true;
      }
    }
    
    if (CleanMuons->At(i)->Pt() > 10.0 && !IsMCLepton) {
      fMuonNumeratorPt->Fill(CleanMuons->At(i)->Pt());
      fMuonNumeratorEta->Fill(CleanMuons->At(i)->Eta());
      fMuonNumeratorPhi->Fill(CleanMuons->At(i)->Phi());
    }
  }
  
//   MDB(kAnalysis, 7) {
//     if (FakeEventHeaders->GetEntries() > 1) {

//       //**********************************************************************************************
//       //Show all Leptons and Jets
//       //**********************************************************************************************
//       cout << "************************************************************************\n";
//       cout << "************************************************************************\n";
//       cout << "Check Leptons: " << CleanLeptons->GetEntries() << endl;
//       for(UInt_t i=0; i<CleanLeptons->GetEntries(); i++)
//         cout << CleanLeptons->At(i)->Pt() <<  " "  
//              << CleanLeptons->At(i)->Eta() << " "  
//              << CleanLeptons->At(i)->Phi() <<  " " 
//              <<endl;
//       cout << "Check Jets: " << OriginalCleanJets->GetEntries() << endl;
//       for(UInt_t i=0; i<OriginalCleanJets->GetEntries(); i++)
//         cout << OriginalCleanJets->At(i)->Pt() <<  " "  
//              << OriginalCleanJets->At(i)->Eta() << " "  
//              << OriginalCleanJets->At(i)->Phi() <<  " " 
//              <<endl;
    
//       //**********************************************************************************************
//       //Show all Fakes
//       //**********************************************************************************************
//       cout << "******************************************************************\n";
//       cout << "Show all Electron FakeableObjects\n";
//       cout << "Number of Electron FakeableObjects : " << ElectronFakeableObjects->GetEntries() 
//            << endl;
//       cout << "Number of Muon FakeableObjects : " << MuonFakeableObjects->GetEntries() << endl;
//       for (UInt_t i=0;i<ElectronFakeableObjects->GetEntries() ; i++) {
//         cout << "ElectronFakeableObject " << i << " : " << ElectronFakeableObjects->At(i)->Pt() << " " 
//              << ElectronFakeableObjects->At(i)->Eta() << " " 
//              << ElectronFakeableObjects->At(i)->Phi() << endl;    
//       }
//       for (UInt_t i=0;i<MuonFakeableObjects->GetEntries() ; i++) {
//         cout << "MuonFakeableObject " << i << " : " << MuonFakeableObjects->At(i)->Pt() << " " 
//              << MuonFakeableObjects->At(i)->Eta() << " " 
//              << MuonFakeableObjects->At(i)->Phi() << endl;    
//       }
      
//       cout << "******************************************************************\n";
//       cout << "Show all FakeEventHeaders\n";
//       cout << "Number of FakeEventHeaders : " << FakeEventHeaders->GetEntries() << endl;
//       for (UInt_t i=0;i<FakeEventHeaders->GetEntries() ; i++) {
//         cout << "FakeEvent " << i << " : weight = " << FakeEventHeaders->At(i)->Weight() << " NJets = " 
//              << FakeEventHeaders->At(i)->NJets() << " NFakes = " << FakeEventHeaders->At(i)->FakeObjectsSize() << endl;
        
//         for (UInt_t j=0;j<FakeEventHeaders->At(i)->FakeObjectsSize();j++) {
//           cout << "FakeObject " << j << " ";
//           if (FakeEventHeaders->At(i)->FakeObject(j)->ObjType() == kElectron) 
//             cout << "e";
//           else if (FakeEventHeaders->At(i)->FakeObject(j)->ObjType() == kMuon)
//             cout << "mu";
//           else 
//             cout << "unknown";
          
//           cout << " " << FakeEventHeaders->At(i)->FakeObject(j)->Pt() << " "
//                << FakeEventHeaders->At(i)->FakeObject(j)->Eta() << " " 
//                << FakeEventHeaders->At(i)->FakeObject(j)->Phi() << endl;
//         }      
//         for (UInt_t j=0;j<FakeEventHeaders->At(i)->NJets();j++) {
//           cout << "Unfaked Jet " << j << " ";
//           cout << " " << FakeEventHeaders->At(i)->UnfakedJet(j)->Pt() << " "
//                << FakeEventHeaders->At(i)->UnfakedJet(j)->Eta() << " " 
//                << FakeEventHeaders->At(i)->UnfakedJet(j)->Phi() << endl;
//         }
//         cout << "-----------------------------------------\n";
//       }
//     }  
//   }

  //***********************************************************************************************
  //Denominator distributions
  //***********************************************************************************************
  if (CleanLeptons->GetEntries() == 1 ) {
    for (UInt_t i=0;i<ElectronFakeableObjects->GetEntries() ; i++) {
      fElectronDenominatorPt->Fill(ElectronFakeableObjects->At(i)->Pt());
      fElectronDenominatorEta->Fill(ElectronFakeableObjects->At(i)->Eta());
      fElectronDenominatorPhi->Fill(ElectronFakeableObjects->At(i)->Phi());
    }
    for (UInt_t i=0;i<MuonFakeableObjects->GetEntries() ; i++) {
      fMuonDenominatorPt->Fill(MuonFakeableObjects->At(i)->Pt());
      fMuonDenominatorEta->Fill(MuonFakeableObjects->At(i)->Eta());
      fMuonDenominatorPhi->Fill(MuonFakeableObjects->At(i)->Phi());
    }
  }



  //***********************************************************************************************
  //If we use MC Fakes, then create a new FakeEventHeader containing no fakes with weight = 1
  //This ensures that in the loop over FakeEventHeaders we do the correct thing.
  //***********************************************************************************************
  if (fUseMCFake) {
    ObjArray <FakeEventHeader> *tmpFakeEventHeaders = new  ObjArray <FakeEventHeader> ;
    FakeEventHeader *initialFakeEvent = new FakeEventHeader();
    for (UInt_t j=0;j<OriginalCleanJets->GetEntries();j++)
      initialFakeEvent->AddJets(OriginalCleanJets->At(j));

    tmpFakeEventHeaders->Add(initialFakeEvent);
    FakeEventHeaders = dynamic_cast<Collection<FakeEventHeader>* > (tmpFakeEventHeaders);
  }

  //***********************************************************************************************
  //For Debugging
  //***********************************************************************************************

  //For All events: simulate fake rate computation environment
  for (UInt_t i=0;i<ElectronFakeableObjects->GetEntries() ; i++) {
    Bool_t IsMCLepton = false;
    for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
      if (MathUtils::DeltaR(ElectronFakeableObjects->At(i)->Phi(), 
                            ElectronFakeableObjects->At(i)->Eta(),
                            GenLeptonsAndTaus->At(l)->Phi(), 
                            GenLeptonsAndTaus->At(l)->Eta()) < 0.1) {
        IsMCLepton = true;
      }
    }

    if (!IsMCLepton && ElectronFakeableObjects->At(i)->Pt() > 10.0) {
      fAllElectronDenominatorPtEta->Fill(ElectronFakeableObjects->At(i)->Pt(),
                                         ElectronFakeableObjects->At(i)->Eta());  

      //categorize Fake
      Int_t printDebugLevel = 0;
      const MCParticle *matchedSimParticle = GeneratorTools::MatchElectronToSimParticle(fParticles, ElectronFakeableObjects->At(i)->Trk(), false, printDebugLevel, false);           
      Int_t FakeCategory = GeneratorTools::CategorizeFakeElectron(matchedSimParticle);
      fAllFakeElectronDenominatorCategory->Fill(FakeCategory);      
    }
  }

  //For All events: simulate fake rate computation environment
  for (UInt_t i=0;i<MuonFakeableObjects->GetEntries() ; i++) {
    Bool_t IsMCLepton = false;
    for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
      if (MathUtils::DeltaR(MuonFakeableObjects->At(i)->Phi(), 
                            MuonFakeableObjects->At(i)->Eta(),
                            GenLeptonsAndTaus->At(l)->Phi(), 
                            GenLeptonsAndTaus->At(l)->Eta()) < 0.1) {
        IsMCLepton = true;
      }
    }

    if (!IsMCLepton && MuonFakeableObjects->At(i)->Pt() > 10.0) {
      //categorize Fake

      Int_t printDebugLevel = 0;
      Bool_t isTrackerTrack = false;
      const MCParticle *matchedSimParticle = 0;
      if (dynamic_cast<Muon*>(MuonFakeableObjects->At(i))->TrackerTrk()) {
        isTrackerTrack = true;
      }
      if (isTrackerTrack) {
        matchedSimParticle = GeneratorTools::MatchMuonToSimParticle(fParticles, dynamic_cast<Muon*>(MuonFakeableObjects->At(i))->TrackerTrk(), isTrackerTrack, printDebugLevel, false);
      } else {
        matchedSimParticle = GeneratorTools::MatchMuonToSimParticle(fParticles, dynamic_cast<Muon*>(MuonFakeableObjects->At(i))->Trk(), isTrackerTrack, printDebugLevel, false);
      }
      Int_t FakeCategory = GeneratorTools::CategorizeFakeMuon(matchedSimParticle);              
      fAllFakeMuonDenominatorCategory->Fill(FakeCategory);      
    }
  }


  for (UInt_t i=0;i<CleanElectrons->GetEntries() ; i++) {
    Bool_t IsMCLepton = false;
    for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
      if (MathUtils::DeltaR(CleanElectrons->At(i)->Phi(), 
                            CleanElectrons->At(i)->Eta(),
                            GenLeptonsAndTaus->At(l)->Phi(), 
                            GenLeptonsAndTaus->At(l)->Eta()) < 0.1) {
        IsMCLepton = true;
      }
    }

    Double_t Electron1PUsingSCEnergy = 0;
    Double_t Electron1PtUsingSCEnergy = 0;
    Electron1PUsingSCEnergy = 
      TMath::Sqrt(CleanElectrons->At(i)->SCluster()->Energy()*CleanElectrons->At(i)->SCluster()->Energy() - 
                  CleanElectrons->At(i)->Mass()*CleanElectrons->At(i)->Mass());
    Electron1PtUsingSCEnergy = 
      TMath::Abs(Electron1PUsingSCEnergy*
                 TMath::Cos(CleanElectrons->At(i)->Trk()->Lambda()));
    
    if (!IsMCLepton && Electron1PtUsingSCEnergy > 10.0) {
      fAllElectronNumeratorPtEta->Fill(Electron1PtUsingSCEnergy,
                                       CleanElectrons->At(i)->Eta());  

      //categorize Fake
      Int_t printDebugLevel = 0;
      const MCParticle *matchedSimParticle = GeneratorTools::MatchElectronToSimParticle(fParticles, CleanElectrons->At(i)->Trk(), false, printDebugLevel, false);           
      Int_t FakeCategory = GeneratorTools::CategorizeFakeElectron(matchedSimParticle);
      fAllFakeElectronCategory->Fill(FakeCategory);  
    }
  }

  //For Fake modelling events: simulate fake rate application environment
  if (CleanLeptons->GetEntries() == 1 ) {

    //require Pt > 10.0 for the clean lepton
    Double_t Lepton1Pt = CleanLeptons->At(0)->Pt();
    Double_t Electron1PUsingSCEnergy = 0;
    Double_t Electron1PtUsingSCEnergy = 0;
    if (CleanLeptons->At(0)->ObjType() == kElectron) {
      Electron* tmpEle = dynamic_cast<Electron*>(CleanLeptons->At(0));
      Electron1PUsingSCEnergy = 
        TMath::Sqrt(tmpEle->SCluster()->Energy()*tmpEle->SCluster()->Energy() - 
                    tmpEle->Mass()*tmpEle->Mass());
      Electron1PtUsingSCEnergy = 
        TMath::Abs(Electron1PUsingSCEnergy*
                   TMath::Cos(tmpEle->Trk()->Lambda()));
      Lepton1Pt = Electron1PtUsingSCEnergy;
    }   

    if (Lepton1Pt > 10.0) {
      for (UInt_t i=0;i<ElectronFakeableObjects->GetEntries() ; i++) {
        Bool_t IsMCLepton = false;
        for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
          if (MathUtils::DeltaR(ElectronFakeableObjects->At(i)->Phi(), 
                                ElectronFakeableObjects->At(i)->Eta(),
                                GenLeptonsAndTaus->At(l)->Phi(), 
                                GenLeptonsAndTaus->At(l)->Eta()) < 0.1) {
            IsMCLepton = true;
          }
        }
        
        Bool_t isClean = false;
        if (MathUtils::DeltaR(ElectronFakeableObjects->At(i)->Phi(), 
                              ElectronFakeableObjects->At(i)->Eta(),
                              CleanLeptons->At(0)->Phi(), 
                              CleanLeptons->At(0)->Eta()) < 0.3) {
          isClean = true;
        }
        
        if (!isClean && !IsMCLepton && ElectronFakeableObjects->GetEntries() == 1 && ElectronFakeableObjects->At(i)->Pt() > 10.0) {
          fFakeSelectionElectronDenominatorPtEta->Fill(ElectronFakeableObjects->At(i)->Pt(),
                                                       ElectronFakeableObjects->At(i)->Eta());    
        }
      }
    }
  }
  
  if (CleanLeptons->GetEntries() == 2) {    

    Double_t Lepton1Pt = CleanLeptons->At(0)->Pt();
    Double_t Lepton2Pt = CleanLeptons->At(1)->Pt();
    
    Double_t Electron1PUsingSCEnergy = 0;
    Double_t Electron1PtUsingSCEnergy = 0;
    Double_t Electron2PUsingSCEnergy = 0;
    Double_t Electron2PtUsingSCEnergy = 0;
    if (CleanLeptons->At(0)->ObjType() == kElectron) {
      Electron* tmpEle = dynamic_cast<Electron*>(CleanLeptons->At(0));
      Electron1PUsingSCEnergy = 
        TMath::Sqrt(tmpEle->SCluster()->Energy()*tmpEle->SCluster()->Energy() - 
                    tmpEle->Mass()*tmpEle->Mass());
      Electron1PtUsingSCEnergy = 
        TMath::Abs(Electron1PUsingSCEnergy*
                   TMath::Cos(tmpEle->Trk()->Lambda()));
      Lepton1Pt = Electron1PtUsingSCEnergy;
    }   
    if (CleanLeptons->At(1)->ObjType() == kElectron) {
      Electron* tmpEle = dynamic_cast<Electron*>(CleanLeptons->At(1));
      Electron2PUsingSCEnergy = 
        TMath::Sqrt(tmpEle->SCluster()->Energy()*tmpEle->SCluster()->Energy() - 
                    tmpEle->Mass()*tmpEle->Mass());
      Electron2PtUsingSCEnergy = 
        TMath::Abs(Electron2PUsingSCEnergy*
                   TMath::Cos(tmpEle->Trk()->Lambda()));
      Lepton2Pt = Electron2PtUsingSCEnergy;
    }    

    Bool_t IsMCLepton1 = false;
    for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
      if (MathUtils::DeltaR(CleanLeptons->At(0)->Phi(), CleanLeptons->At(0)->Eta(),
                            GenLeptonsAndTaus->At(l)->Phi(), 
                            GenLeptonsAndTaus->At(l)->Eta()) < 0.1) {
        IsMCLepton1 = true;
      }
    }
    Bool_t IsMCLepton2 = false;
    for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
      if (MathUtils::DeltaR(CleanLeptons->At(1)->Phi(), CleanLeptons->At(1)->Eta(),
                            GenLeptonsAndTaus->At(l)->Phi(), 
                            GenLeptonsAndTaus->At(l)->Eta()) < 0.1) {
        IsMCLepton2 = true;
      }
    }
       
    if (Lepton1Pt > 10.0 && Lepton2Pt > 10.0) {
      if (!IsMCLepton1 && IsMCLepton2 && CleanLeptons->At(0)->ObjType() == kElectron) {    
        fFakeSelectionElectronNumeratorPtEta->Fill(Lepton1Pt, CleanLeptons->At(0)->Eta());                
      } else if (IsMCLepton1 && !IsMCLepton2 && CleanLeptons->At(1)->ObjType() == kElectron) {
        fFakeSelectionElectronNumeratorPtEta->Fill(Lepton2Pt, CleanLeptons->At(1)->Eta());
      } else {
        //both matched gen lepton
        //print out info.
      }
    }

    //Print out what happened to the bins with 0 numerator entries. Eta < 1.0, Pt < 20.
    if ( !(Lepton1Pt > 10.0 && Lepton2Pt > 10.0) &&
         ((!IsMCLepton1 && IsMCLepton2 && CleanLeptons->At(0)->ObjType() == kElectron) || (IsMCLepton1 && !IsMCLepton2 && CleanLeptons->At(1)->ObjType() == kElectron))      
      ) {
      printdebug = true;
      cout << "Debug: " << Lepton1Pt << " " << Lepton2Pt << " " << IsMCLepton1 << " " << IsMCLepton2 << endl;
    }
  }
 


  //***********************************************************************************************
  //Do Study on Lepton Trigger
  //Consider events with 2 clean leptons with pt>20 ,10.
  //Classify into events where real lepton passed trigger, where fake passed trigger, 
  //both passed trigger, none passed trigger (volunteer)
  //***********************************************************************************************
  if (CleanLeptons->GetEntries() >= 2 && CleanLeptons->At(0)->Pt() > 20.0 
      && CleanLeptons->At(1)->Pt() > 10.0) {
    Bool_t RealElectronPassTrigger = false;
    Bool_t FakeElectronPassTrigger = false;
    Bool_t VolunteerElectronPassTrigger = false;
    Bool_t RealMuonPassTrigger = false;
    Bool_t FakeMuonPassTrigger = false;
    Bool_t VolunteerMuonPassTrigger = false;

    for (UInt_t l=0; l<triggerObjects->GetEntries(); ++l) {
      
      //***********************************************************************************************
      //Electron Trigger
      //***********************************************************************************************
      if (triggerObjects->At(l)->TrigName() == string("HLT_IsoEle18_L1R")
          && triggerObjects->At(l)->Type() == TriggerObject::TriggerElectron) {
        
        fElectronTriggerObjectPt->Fill(triggerObjects->At(l)->Pt());
        fElectronTriggerObjectEta->Fill(triggerObjects->At(l)->Eta());
        
        Double_t minDR;
        Int_t electronTriggerObjectType = 0;
        
        //match to gen electron      
        minDR = 5000.0;
        for (UInt_t e=0;e<GenLeptons->GetEntries() ; ++e) {
          if (GenLeptons->At(e)->AbsPdgId() == 11) {
            Double_t deltaR = MathUtils::DeltaR(GenLeptons->At(e)->Phi(), GenLeptons->At(e)->Eta(),
                                                triggerObjects->At(l)->Phi(), 
                                                triggerObjects->At(l)->Eta());
            if (deltaR < minDR) {
              minDR = deltaR;
            }
          }
        }
        if (minDR < 0.3) {
          RealElectronPassTrigger = true;
          electronTriggerObjectType = 1;
          fElectronTriggerObjectPt_Real->Fill(triggerObjects->At(l)->Pt());
          fElectronTriggerObjectEta_Real->Fill(triggerObjects->At(l)->Eta());
        } else {                
          //match to  electron fakeable object
          minDR = 5000.0;
          for (UInt_t e=0;e<ElectronFakeableObjects->GetEntries() ; ++e) {
            if (ElectronFakeableObjects->At(e)->ObjType() == kElectron) {
              Double_t deltaR = MathUtils::DeltaR(ElectronFakeableObjects->At(e)->Phi(), 
                                                  ElectronFakeableObjects->At(e)->Eta(),
                                                  triggerObjects->At(l)->Phi(), 
                                                  triggerObjects->At(l)->Eta());
              if (deltaR < minDR) {
                minDR = deltaR;
              }
            }
          }
          if (minDR < 0.3) {
            FakeElectronPassTrigger = true;
            electronTriggerObjectType = 2;
            fElectronTriggerObjectPt_Fake->Fill(triggerObjects->At(l)->Pt());        
            fElectronTriggerObjectEta_Fake->Fill(triggerObjects->At(l)->Eta());
          } else {          
            VolunteerElectronPassTrigger = true;;      
            fElectronTriggerObjectPt_Volunteer->Fill(triggerObjects->At(l)->Pt());
            fElectronTriggerObjectEta_Volunteer->Fill(triggerObjects->At(l)->Eta());
          } // not matched to denominator
        } // not matched to gen electron
      }
      
      //***********************************************************************************************
      //Muon Trigger
      //***********************************************************************************************
      if (triggerObjects->At(l)->TrigName() ==  string("HLT_Mu15")
          && triggerObjects->At(l)->Type() == TriggerObject::TriggerMuon) {
        
        fMuonTriggerObjectPt->Fill(triggerObjects->At(l)->Pt());
        fMuonTriggerObjectEta->Fill(triggerObjects->At(l)->Eta());
        
        Double_t minDR;
        Int_t muonTriggerObjectType = 0;
        
        //match to gen muon
        minDR = 5000.0;
        for (UInt_t e=0;e<GenLeptons->GetEntries() ; ++e) {
          if (GenLeptons->At(e)->AbsPdgId() == 13) {
            Double_t deltaR = MathUtils::DeltaR(GenLeptons->At(e)->Phi(), GenLeptons->At(e)->Eta(),
                                                triggerObjects->At(l)->Phi(), 
                                                triggerObjects->At(l)->Eta());
            if (deltaR < minDR) {
              minDR = deltaR;
            }
          }
        }
        if (minDR < 0.3) {
          RealMuonPassTrigger = true;
          muonTriggerObjectType = 1;
          fMuonTriggerObjectPt_Real->Fill(triggerObjects->At(l)->Pt());
          fMuonTriggerObjectEta_Real->Fill(triggerObjects->At(l)->Eta());
        } else {
          //match to muon fakeable object
          minDR = 5000.0;
          for (UInt_t e=0;e<MuonFakeableObjects->GetEntries() ; ++e) {
            if (MuonFakeableObjects->At(e)->ObjType() == kMuon) {
              Double_t deltaR = MathUtils::DeltaR(MuonFakeableObjects->At(e)->Phi(), 
                                                  MuonFakeableObjects->At(e)->Eta(),
                                                  triggerObjects->At(l)->Phi(), 
                                                  triggerObjects->At(l)->Eta());
              if (deltaR < minDR) {
                minDR = deltaR;
              }
            }
          }
          if (minDR < 0.3) {
            FakeMuonPassTrigger = true;
            muonTriggerObjectType = 2;
            fMuonTriggerObjectPt_Fake->Fill(triggerObjects->At(l)->Pt());
            fMuonTriggerObjectEta_Fake->Fill(triggerObjects->At(l)->Eta());
          } else {          
            VolunteerMuonPassTrigger = true;
            fMuonTriggerObjectPt_Volunteer->Fill(triggerObjects->At(l)->Pt());
            fMuonTriggerObjectEta_Volunteer->Fill(triggerObjects->At(l)->Eta());
          }//if not matched to denominator
        }//if not matched to gen muon
      }
    }  
    
    //Find real lepton;
    Particle *realLepton = 0;
    for (UInt_t e=0;e<CleanLeptons->GetEntries() ; ++e) {
      double min_dr = 5000.0;
      for (UInt_t g=0;g<GenLeptons->GetEntries() ; ++g) {
        Double_t deltaR = MathUtils::DeltaR(GenLeptons->At(g)->Phi(), GenLeptons->At(g)->Eta(),
                                            CleanLeptons->At(e)->Phi(), 
                                            CleanLeptons->At(e)->Eta());
        if (deltaR < min_dr) {
          min_dr = deltaR;
        }
      }
      if (min_dr < 0.3) {
        realLepton = CleanLeptons->At(e);
        break;
      }
    }

    //Classify event by trigger type
    if ((RealMuonPassTrigger || RealElectronPassTrigger)
        && !(FakeMuonPassTrigger || FakeElectronPassTrigger)
        && !(VolunteerMuonPassTrigger || VolunteerElectronPassTrigger)
      ) {
      fTriggerType->Fill(1);
      if (RealMuonPassTrigger) fMuonTriggerType->Fill(1);
      if (RealElectronPassTrigger) fElectronTriggerType->Fill(1);

      //plot pt, eta of the clean lepton (fake) which failed trigger
      for (UInt_t e=0;e<CleanLeptons->GetEntries() ; ++e) {
        if (CleanLeptons->At(e) != realLepton) {
          fFailedTriggerLeptonPt_RealTriggerOnly->Fill(CleanLeptons->At(e)->Pt());
          fFailedTriggerLeptonEta_RealTriggerOnly->Fill(CleanLeptons->At(e)->Eta());
          if (CleanLeptons->At(e)->ObjType() == kElectron) {
            fFailedTriggerElectronPt_RealTriggerOnly->Fill(CleanLeptons->At(e)->Pt());
            fFailedTriggerElectronEta_RealTriggerOnly->Fill(CleanLeptons->At(e)->Eta());
          }
          if (CleanLeptons->At(e)->ObjType() == kMuon) {
            fFailedTriggerMuonPt_RealTriggerOnly->Fill(CleanLeptons->At(e)->Pt());
            fFailedTriggerMuonEta_RealTriggerOnly->Fill(CleanLeptons->At(e)->Eta());
          }
        }
      }

    } else if ((RealMuonPassTrigger || RealElectronPassTrigger)
               && (FakeMuonPassTrigger || FakeElectronPassTrigger)
               && !(VolunteerMuonPassTrigger || VolunteerElectronPassTrigger)
      ) {
      fTriggerType->Fill(2);
      if (RealMuonPassTrigger || FakeMuonPassTrigger) fMuonTriggerType->Fill(2);
      if (RealElectronPassTrigger || FakeElectronPassTrigger) fElectronTriggerType->Fill(2);
    } else if (!(RealMuonPassTrigger || RealElectronPassTrigger)
               && (FakeMuonPassTrigger || FakeElectronPassTrigger)
               && !(VolunteerMuonPassTrigger || VolunteerElectronPassTrigger)
      ) {
      fTriggerType->Fill(3);
      if (FakeMuonPassTrigger) fMuonTriggerType->Fill(3);
      if (FakeElectronPassTrigger) fElectronTriggerType->Fill(3);
      //plot pt, eta of the clean lepton (real) which failed trigger
      for (UInt_t e=0;e<CleanLeptons->GetEntries() ; ++e) {
        if (CleanLeptons->At(e) == realLepton) {
          fFailedTriggerLeptonPt_FakeTriggerOnly->Fill(CleanLeptons->At(e)->Pt());
          fFailedTriggerLeptonEta_FakeTriggerOnly->Fill(CleanLeptons->At(e)->Eta());
          if (CleanLeptons->At(e)->ObjType() == kElectron) {
            fFailedTriggerElectronPt_FakeTriggerOnly->Fill(CleanLeptons->At(e)->Pt());
            fFailedTriggerElectronEta_FakeTriggerOnly->Fill(CleanLeptons->At(e)->Eta());
          }
          if (CleanLeptons->At(e)->ObjType() == kMuon) {
            fFailedTriggerMuonPt_FakeTriggerOnly->Fill(CleanLeptons->At(e)->Pt());
            fFailedTriggerMuonEta_FakeTriggerOnly->Fill(CleanLeptons->At(e)->Eta());
          }          
        }
      }      
    } else if (!(RealMuonPassTrigger || RealElectronPassTrigger)
               && !(FakeMuonPassTrigger || FakeElectronPassTrigger)
               && (VolunteerMuonPassTrigger || VolunteerElectronPassTrigger)
      ) {
      fTriggerType->Fill(4);
    } else {
      fTriggerType->Fill(5);
    }
  }


  //***********************************************************************************************
  //Distributions from applying fake rates
  //***********************************************************************************************
  for (UInt_t i=0;i<FakeEventHeaders->GetEntries() ; i++) {
    //only consider events with one and only one fake. (A fake WW event)
    ObjArray<Particle> *leptons = NULL;
    if (fUseMCFake) {
      leptons = CleanLeptons;
    } else {
      leptons = new ObjArray<Particle>;

      //here we really expect CleanLeptons to contain only 1 entry
      for (UInt_t j=0;j<CleanLeptons->GetEntries() ; j++) {
        leptons->Add(CleanLeptons->At(j));
      }
      for (UInt_t j=0;j<FakeEventHeaders->At(i)->FakeObjectsSize() ; j++) {
        leptons->Add(FakeEventHeaders->At(i)->FakeObject(j)->FakeParticle());
      }
    }

    //Construct the Clean Jet collection.
    ObjArray<Jet> *CleanJets = NULL;
    if (fUseMCFake) {
      CleanJets = OriginalCleanJets;
    } else {
      CleanJets = new ObjArray<Jet>;
      for (UInt_t j=0;j<FakeEventHeaders->At(i)->NJets() ; j++) {
        CleanJets->Add(FakeEventHeaders->At(i)->UnfakedJet(j));
      }
    }

    //Perform correction for potential fake muons
    //have to add fake muon momentum to originalCaloMet;
    const Met *caloMet = originalCaloMet;
    Double_t FakeMuonMetCorrection_X = 0.0;
    Double_t FakeMuonMetCorrection_Y = 0.0;
    for (UInt_t j=0;j<FakeEventHeaders->At(i)->FakeObjectsSize() ; j++) {
      if (FakeEventHeaders->At(i)->FakeObject(j)->ObjType() == kMuon) {
        FakeMuonMetCorrection_X += FakeEventHeaders->At(i)->FakeObject(j)->Px();
        FakeMuonMetCorrection_Y += FakeEventHeaders->At(i)->FakeObject(j)->Py();
      }
    }
    
    if (!fUseMCFake && fPerformFakeMuonMetCorrection) {
      caloMet = new Met(originalCaloMet->Px()+FakeMuonMetCorrection_X,
                        originalCaloMet->Py()+FakeMuonMetCorrection_Y);
    }

    //we have to sort leptons here
    leptons->Sort();

    //check lepton sort
    MDB(kAnalysis, 7) {
      if (FakeEventHeaders->GetEntries() > 1) {
        
        cout << "**********************************************************\n";
        cout << "Check Lepton sorting\n";
        cout << "**********************************************************\n";
        for (UInt_t l=0;l<leptons->GetEntries();l++) {
          cout << "lepton " << l << " ";
          if (leptons->At(l)->ObjType() == kElectron)
            cout << " e ";
          else if (leptons->At(l)->ObjType() == kMuon)
            cout << " mu ";
          else 
            cout << " unk ";
          
          cout << " " << leptons->At(l)->Pt() << " " << leptons->At(l)->Eta()
               << " " << leptons->At(l)->Phi() << endl;
        }
      }
    }


    //*********************************************************************************************
    //Construct the event weight using fake rate and corrections
    //*********************************************************************************************
    //fake rate has to be corrected by the amount lost when those denominators 
    //became fakes in data. If a denominator fakes a lepton in data, it goes in the 2lepton
    //final state, and we don't count it in this prediction. So we have to add it back.
    Double_t eventweight = FakeEventHeaders->At(i)->Weight();
    if (FakeEventHeaders->At(i)->FakeObjectsSize() > 0 && FakeEventHeaders->At(i)->Weight() < 1) {
      eventweight = eventweight / (1.0 - FakeEventHeaders->At(i)->Weight());
    }

    //error on the weight and correction to the error. 
    //Use standard error propagation with function f = e / (1-e) ; df = de / (1-e)^2
    Double_t eventweightError = FakeEventHeaders->At(i)->WeightLowError();
    eventweightError = eventweightError / ((1 - FakeEventHeaders->At(i)->Weight())*
                                           (1 - FakeEventHeaders->At(i)->Weight()));
    //*********************************************************************************************
    //another correction to account for events lost due to only the fake lepton firing the trigger    
    //*********************************************************************************************
    if (CleanLeptons->GetEntries() >= 1 && FakeEventHeaders->At(i)->FakeObjectsSize() >= 1) {
      if (CleanLeptons->At(0)->ObjType() == kElectron && 
          FakeEventHeaders->At(i)->FakeObject(0)->FakeParticle()->ObjType() == kElectron) {
        eventweight = eventweight * 1.06;
        eventweightError = eventweightError * 1.06;
      } else if (CleanLeptons->At(0)->ObjType() == kMuon && 
                 FakeEventHeaders->At(i)->FakeObject(0)->FakeParticle()->ObjType() == kMuon) {
        eventweight = eventweight * 1.12;
        eventweightError = eventweightError * 1.12;
      } else if (CleanLeptons->At(0)->ObjType() == kElectron && 
                 FakeEventHeaders->At(i)->FakeObject(0)->FakeParticle()->ObjType() == kMuon) {
        eventweight = eventweight * 1.17;
        eventweightError = eventweightError * 1.17;
      } else if (CleanLeptons->At(0)->ObjType() == kMuon && 
                FakeEventHeaders->At(i)->FakeObject(0)->FakeParticle()->ObjType() == kElectron) {
        eventweight = eventweight * 1.17;        
        eventweightError = eventweightError * 1.17;
      }
    }

    //correction on the error
    

    //can get type by doing leptons->At(i)->ObjType()
    //from now on everything else is the same as in HWW module.
    //And all collections made above should be same as in HWW module if fUseMCFake == true!!



    //*********************************************************************************************
    //Do not count events where both leptons came from a real lepton.
    //In data: we subtract events where the denominator was a real lepton that failed electron ID
    //         using MC, in the 1lepton+denominator sample.
    //*********************************************************************************************
      
    if (leptons->GetEntries() >= 2) {
      Bool_t Lepton1IsMCLepton = false;
      Bool_t Lepton2IsMCLepton = false;
      
      for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
        if (MathUtils::DeltaR(leptons->At(0)->Phi(), 
                              leptons->At(0)->Eta(),
                              GenLeptonsAndTaus->At(l)->Phi(), 
                              GenLeptonsAndTaus->At(l)->Eta()) < 0.1) {
          Lepton1IsMCLepton = true;
        }       
      }
      for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
        if (MathUtils::DeltaR(leptons->At(1)->Phi(), 
                              leptons->At(1)->Eta(),
                              GenLeptonsAndTaus->At(l)->Phi(), 
                              GenLeptonsAndTaus->At(l)->Eta()) < 0.1) {
          Lepton2IsMCLepton = true;
        }       
      }
        
      if (Lepton1IsMCLepton && Lepton2IsMCLepton) {
        continue;
      }
    }
    
    //***********************************************************************************************
    //For FR method (fUseMCFake == false)
    //Select events containing one and only one clean lepton with pt > 10 GeV
    //This will be how it is done in data. 
    //***********************************************************************************************
    if (!fUseMCFake)
      if (CleanLeptons->GetEntries() != 1 || CleanLeptons->At(0)->Pt() <= 10.0)
        continue;




    //*********************************************************************************************
    //Fake Histograms
    //*********************************************************************************************
    if (!fUseMCFake) {
      if (CleanLeptons->GetEntries() == 1 && FakeEventHeaders->At(i)->FakeObjectsSize() == 1 ) {

        int Charge1;
        int Charge2;
        if (CleanLeptons->At(0)->Pt() > FakeEventHeaders->At(i)->FakeObject(0)->Pt()) {
          Charge1 = Int_t(CleanLeptons->At(0)->Charge());
          Charge2 = Int_t(FakeEventHeaders->At(i)->FakeObject(0)->Charge());
        } else {
          Charge1 = Int_t(FakeEventHeaders->At(i)->FakeObject(0)->Charge());
          Charge2 = Int_t(CleanLeptons->At(0)->Charge());
        }

        Double_t Lepton1Pt = CleanLeptons->At(0)->Pt();
        Double_t Electron1PUsingSCEnergy = 0;
        Double_t Electron1PtUsingSCEnergy = 0;
        if (CleanLeptons->At(0)->ObjType() == kElectron) {
          Electron* tmpEle = dynamic_cast<Electron*>(CleanLeptons->At(0));
          Electron1PUsingSCEnergy = 
            TMath::Sqrt(tmpEle->SCluster()->Energy()*tmpEle->SCluster()->Energy() - 
                        tmpEle->Mass()*tmpEle->Mass());
          Electron1PtUsingSCEnergy = 
            TMath::Abs(Electron1PUsingSCEnergy*
                       TMath::Cos(tmpEle->Trk()->Lambda()));
          Lepton1Pt = Electron1PtUsingSCEnergy;
        }

        //only fill hist if lepton1 pt > 10
        if (Lepton1Pt > 10.0) {
          if (FakeEventHeaders->At(i)->FakeObject(0)->ObjType() == kElectron) {
            
            //categorize Fake
            Int_t printDebugLevel = 0;
            const MCParticle *matchedSimParticle = GeneratorTools::MatchElectronToSimParticle(fParticles, dynamic_cast<const Electron*>(FakeEventHeaders->At(i)->FakeObject(0)->FakeParticle())->Trk(), false, printDebugLevel, false);           
            Int_t FakeCategory = GeneratorTools::CategorizeFakeElectron(matchedSimParticle);

            fFakeElectronPt_weighted->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Pt(),
                                           eventweight);
            fFakeElectronEta_weighted->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Eta(),
                                            eventweight);
            fFakeElectronPhi_weighted->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Phi(),
                                            eventweight);
            fFakeElectronPt->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Pt());
            fFakeElectronEta->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Eta());
            fFakeElectronPhi->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Phi());
            fFakeElectronCategory->Fill(FakeCategory, eventweight);

            if (Charge1 == 1 && Charge2 == -1) {
              fFakeElectronPt_weighted_PlusMinus->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Pt(),
                                                       eventweight);
              fFakeElectronEta_weighted_PlusMinus->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Eta(),
                                                        eventweight);
              fFakeElectronPhi_weighted_PlusMinus->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Phi(),
                                                        eventweight);
              fFakeElectronCategoryPlusMinus->Fill(FakeCategory, eventweight);
            }
          }

          if (FakeEventHeaders->At(i)->FakeObject(0)->ObjType() == kMuon) {
            Int_t printDebugLevel = 0;
            Bool_t isTrackerTrack = false;
            const MCParticle *matchedSimParticle = 0;            
            if (dynamic_cast<const Muon*>(FakeEventHeaders->At(i)->FakeObject(0)->FakeParticle())->TrackerTrk()) {
              isTrackerTrack = true;
            }
            if (isTrackerTrack) {
              matchedSimParticle = GeneratorTools::MatchMuonToSimParticle(fParticles, dynamic_cast<const Muon*>(FakeEventHeaders->At(i)->FakeObject(0)->FakeParticle())->TrackerTrk(), isTrackerTrack, printDebugLevel, false);
            } else {
              matchedSimParticle = GeneratorTools::MatchMuonToSimParticle(fParticles, dynamic_cast<const Muon*>(FakeEventHeaders->At(i)->FakeObject(0)->FakeParticle())->Trk(), isTrackerTrack, printDebugLevel, false);
            }
            Int_t FakeCategory = GeneratorTools::CategorizeFakeMuon(matchedSimParticle);  
                        
            fFakeMuonPt_weighted->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Pt(),
                                       eventweight);
            fFakeMuonEta_weighted->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Eta(),
                                        eventweight);
            fFakeMuonPhi_weighted->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Phi(),
                                        eventweight);
            fFakeMuonPt->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Pt());
            fFakeMuonEta->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Eta());
            fFakeMuonPhi->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Phi());
            fFakeMuonCategory->Fill(FakeCategory, eventweight);  
            printdebug = true;
          }
        }   
      }
    } else {
      if(CleanLeptons->GetEntries() >= 2) {
        //Before we remove events with 2 clean leptons we do this study
        //Fill dilepton charge using denominator on events where denominator matches to a clean lepton      
        //fDileptonChargeUsingDenominator
        //choose events which have 2 clean leptons
        //Do this only for MC Fake case.
        int fakeindex = -1;
        int WCharge;
        int FakeCharge;
        
        for (UInt_t c=0;c<2;c++) {
          int fakeMatchIndex = -1;
          double minDR = 5000;
          
          for (UInt_t f=0;f<FakeableObjects->GetEntries() ; f++) {
            double DR = MathUtils::DeltaR(FakeableObjects->At(f)->Phi(),
                                          FakeableObjects->At(f)->Eta(),
                                          CleanLeptons->At(c)->Phi(),
                                          CleanLeptons->At(c)->Eta());
            if (DR < minDR) {
              fakeMatchIndex = f;
              minDR = DR;
            }                                       
          }                    
          //found match
          if (minDR < 0.3) {
            fakeindex = c;
            FakeCharge = int(FakeableObjects->At(i)->Charge());
            if (fakeindex == 0)
              WCharge = int(CleanLeptons->At(1)->Charge());
            else 
              WCharge = int(CleanLeptons->At(0)->Charge());
            break;
          }
        }
        
        if (fakeindex >= 0) {
          if (WCharge + FakeCharge == 0) {
            if (fakeindex == 0) {
              fDileptonChargeUsingDenominator->Fill(2*FakeCharge + WCharge);
            } else {
              fDileptonChargeUsingDenominator->Fill(2*WCharge + FakeCharge);
            }
          } else {
            fDileptonChargeUsingDenominator->Fill(WCharge + FakeCharge);
          }
        } {
          //not matched to anything
          if (CleanLeptons->At(0)->Charge()+CleanLeptons->At(1)->Charge() == 0) {
            if (CleanLeptons->At(0)->Charge() == 1)
              fDileptonChargeNotMatchedToDenominator->Fill(1);
            else 
              fDileptonChargeNotMatchedToDenominator->Fill(-1);
          } else {
            fDileptonChargeNotMatchedToDenominator->Fill(CleanLeptons->At(0)->Charge()+CleanLeptons->At(1)->Charge());
          }
        }
        
        //make the histogram for the lepton that did not match to the gen lepton      
        Bool_t IsMCLepton1 = false;
        for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
          if (MathUtils::DeltaR(CleanLeptons->At(0)->Phi(), CleanLeptons->At(0)->Eta(),
                                GenLeptonsAndTaus->At(l)->Phi(), 
                                GenLeptonsAndTaus->At(l)->Eta()) < 0.1) {
          IsMCLepton1 = true;
          }
        }
        Bool_t IsMCLepton2 = false;
        for (UInt_t l=0; l<GenLeptonsAndTaus->GetEntries(); l++) {
          if (MathUtils::DeltaR(CleanLeptons->At(1)->Phi(), CleanLeptons->At(1)->Eta(),
                                GenLeptonsAndTaus->At(l)->Phi(), 
                                GenLeptonsAndTaus->At(l)->Eta()) < 0.1) {
          IsMCLepton2 = true;
          }
        }
       
        Double_t Lepton1Pt = CleanLeptons->At(0)->Pt();
        Double_t Lepton2Pt = CleanLeptons->At(1)->Pt();

        Double_t Electron1PUsingSCEnergy = 0;
        Double_t Electron1PtUsingSCEnergy = 0;
        Double_t Electron2PUsingSCEnergy = 0;
        Double_t Electron2PtUsingSCEnergy = 0;
        if (CleanLeptons->At(0)->ObjType() == kElectron) {
          Electron* tmpEle = dynamic_cast<Electron*>(CleanLeptons->At(0));
          Electron1PUsingSCEnergy = 
            TMath::Sqrt(tmpEle->SCluster()->Energy()*tmpEle->SCluster()->Energy() - 
                        tmpEle->Mass()*tmpEle->Mass());
          Electron1PtUsingSCEnergy = 
            TMath::Abs(Electron1PUsingSCEnergy*
                       TMath::Cos(tmpEle->Trk()->Lambda()));
          Lepton1Pt = Electron1PtUsingSCEnergy;
        }   
        if (CleanLeptons->At(1)->ObjType() == kElectron) {
          Electron* tmpEle = dynamic_cast<Electron*>(CleanLeptons->At(1));
          Electron2PUsingSCEnergy = 
            TMath::Sqrt(tmpEle->SCluster()->Energy()*tmpEle->SCluster()->Energy() - 
                        tmpEle->Mass()*tmpEle->Mass());
          Electron2PtUsingSCEnergy = 
            TMath::Abs(Electron2PUsingSCEnergy*
                       TMath::Cos(tmpEle->Trk()->Lambda()));
          Lepton2Pt = Electron2PtUsingSCEnergy;
        }
        
        if (Lepton1Pt > 10.0 && Lepton2Pt > 10.0) {
          if (!IsMCLepton1 && IsMCLepton2) {       
            if (CleanLeptons->At(0)->ObjType() == kElectron) {
              Int_t printDebugLevel = 0;
              const MCParticle *matchedSimParticle = GeneratorTools::MatchElectronToSimParticle(fParticles, dynamic_cast<Electron*>(CleanLeptons->At(0))->Trk(), false, printDebugLevel, false);
              Int_t FakeCategory = GeneratorTools::CategorizeFakeElectron(matchedSimParticle);
              
              fFakeElectronPt_weighted->Fill(Lepton1Pt);
              fFakeElectronEta_weighted->Fill(CleanLeptons->At(0)->Eta());
              fFakeElectronPhi_weighted->Fill(CleanLeptons->At(0)->Phi());
              fFakeElectronPt->Fill(Lepton1Pt);
              fFakeElectronEta->Fill(CleanLeptons->At(0)->Eta());
              fFakeElectronPhi->Fill(CleanLeptons->At(0)->Phi());    
              fFakeElectronCategory->Fill(FakeCategory);
              
              if (CleanLeptons->At(0)->Charge() == 1 && CleanLeptons->At(1)->Charge() == -1) {
                fFakeElectronPt_weighted_PlusMinus->Fill(Lepton1Pt);
                fFakeElectronEta_weighted_PlusMinus->Fill(CleanLeptons->At(0)->Eta());
                fFakeElectronPhi_weighted_PlusMinus->Fill(CleanLeptons->At(0)->Phi());
                fFakeElectronCategoryPlusMinus->Fill(FakeCategory);
                //printdebug = true;
              }
            } else if (CleanLeptons->At(0)->ObjType() == kMuon) {
              Int_t printDebugLevel = 0;
              Bool_t isTrackerTrack = false;
              const MCParticle *matchedSimParticle = 0;
              if (dynamic_cast<Muon*>(CleanLeptons->At(0))->TrackerTrk()) {
                isTrackerTrack = true;
              }
              if (isTrackerTrack) {
                matchedSimParticle = GeneratorTools::MatchMuonToSimParticle(fParticles, dynamic_cast<Muon*>(CleanLeptons->At(0))->TrackerTrk(), isTrackerTrack, printDebugLevel, false);
              } else {
                matchedSimParticle = GeneratorTools::MatchMuonToSimParticle(fParticles, dynamic_cast<Muon*>(CleanLeptons->At(0))->Trk(), isTrackerTrack, printDebugLevel, false);
              }
              Int_t FakeCategory = GeneratorTools::CategorizeFakeMuon(matchedSimParticle);
              
              fFakeMuonPt_weighted->Fill(Lepton1Pt);
              fFakeMuonEta_weighted->Fill(CleanLeptons->At(0)->Eta());
              fFakeMuonPhi_weighted->Fill(CleanLeptons->At(0)->Phi());
              fFakeMuonPt->Fill(Lepton1Pt);
              fFakeMuonEta->Fill(CleanLeptons->At(0)->Eta());
              fFakeMuonPhi->Fill(CleanLeptons->At(0)->Phi());    
              fFakeMuonCategory->Fill(FakeCategory);              
            }      
          } else if (IsMCLepton1 && !IsMCLepton2) {
            if (CleanLeptons->At(1)->ObjType() == kElectron) {
              Int_t printDebugLevel = 0;
              const MCParticle *matchedSimParticle = GeneratorTools::MatchElectronToSimParticle(fParticles, dynamic_cast<Electron*>(CleanLeptons->At(1))->Trk(), false, printDebugLevel, false);
              Int_t FakeCategory = GeneratorTools::CategorizeFakeElectron(matchedSimParticle);
              
              fFakeElectronPt_weighted->Fill(Lepton2Pt);
              fFakeElectronEta_weighted->Fill(CleanLeptons->At(1)->Eta());
              fFakeElectronPhi_weighted->Fill(CleanLeptons->At(1)->Phi());
              fFakeElectronPt->Fill(Lepton2Pt);
              fFakeElectronEta->Fill(CleanLeptons->At(1)->Eta());
              fFakeElectronPhi->Fill(CleanLeptons->At(1)->Phi());
              fFakeElectronCategory->Fill(FakeCategory);
              
              if (CleanLeptons->At(0)->Charge() == 1 && CleanLeptons->At(1)->Charge() == -1) {
                fFakeElectronPt_weighted_PlusMinus->Fill(Lepton2Pt);
                fFakeElectronEta_weighted_PlusMinus->Fill(CleanLeptons->At(1)->Eta());
                fFakeElectronPhi_weighted_PlusMinus->Fill(CleanLeptons->At(1)->Phi());
                fFakeElectronCategoryPlusMinus->Fill(FakeCategory);
                printdebug = true;
              }
            } else if (CleanLeptons->At(1)->ObjType() == kMuon) {
              Int_t printDebugLevel = 0;
              Bool_t isTrackerTrack = false;
              const MCParticle *matchedSimParticle = 0;
              if (dynamic_cast<Muon*>(CleanLeptons->At(1))->TrackerTrk()) {
                isTrackerTrack = true;
              }
              if (isTrackerTrack) {
                matchedSimParticle = GeneratorTools::MatchMuonToSimParticle(fParticles, dynamic_cast<Muon*>(CleanLeptons->At(1))->TrackerTrk(), isTrackerTrack, printDebugLevel, false);
              } else {
                matchedSimParticle = GeneratorTools::MatchMuonToSimParticle(fParticles, dynamic_cast<Muon*>(CleanLeptons->At(1))->Trk(), isTrackerTrack, printDebugLevel, false);
              }
              Int_t FakeCategory = GeneratorTools::CategorizeFakeMuon(matchedSimParticle);
              
              fFakeMuonPt_weighted->Fill(Lepton2Pt);
              fFakeMuonEta_weighted->Fill(CleanLeptons->At(1)->Eta());
              fFakeMuonPhi_weighted->Fill(CleanLeptons->At(1)->Phi());
              fFakeMuonPt->Fill(Lepton2Pt);
              fFakeMuonEta->Fill(CleanLeptons->At(1)->Eta());
              fFakeMuonPhi->Fill(CleanLeptons->At(1)->Phi());    
              fFakeMuonCategory->Fill(FakeCategory);              
            }
          } else {
            //both matched.
            //print out info.
          }
        }

//         cout << "************************************************************************\n";
//         cout << "Fake Event \n";
//         cout << "************************************************************************\n";
//         cout << "Lepton1 isMC: " << IsMCLepton1 << "    Lepton2 isMC: " << IsMCLepton2 << endl;        
//         //print Event Info
//         //**********************************************************************************************
//         //Show all Leptons and Jets
//         //**********************************************************************************************
//         cout << "************************************************************************\n";
//         cout << "************************************************************************\n";
//         cout << "Check Leptons: " << CleanLeptons->GetEntries() << endl;
//         for(UInt_t k=0; k<CleanLeptons->GetEntries(); ++k)
//           cout << CleanLeptons->At(k)->Pt() <<  " "  
//                << CleanLeptons->At(k)->Eta() << " "  
//                << CleanLeptons->At(k)->Phi() <<  " " 
//                <<endl;
//         cout << "Check Jets: " << OriginalCleanJets->GetEntries() << endl;
//         for(UInt_t k=0; k<OriginalCleanJets->GetEntries(); ++k)
//           cout << OriginalCleanJets->At(k)->Pt() <<  " "  
//                << OriginalCleanJets->At(k)->Eta() << " "  
//                << OriginalCleanJets->At(k)->Phi() <<  " " 
//                <<endl;
        
//         //**********************************************************************************************
//         //Show all Fakes
//         //**********************************************************************************************
//         cout << "******************************************************************\n";
//         cout << "Show all Electron FakeableObjects\n";
//         cout << "Number of Electron FakeableObjects : " << ElectronFakeableObjects->GetEntries() 
//              << endl;
//         cout << "Number of Muon FakeableObjects : " << MuonFakeableObjects->GetEntries() << endl;
//         for (UInt_t k=0;k<ElectronFakeableObjects->GetEntries() ; ++k) {
//           cout << "ElectronFakeableObject " << k << " : " << ElectronFakeableObjects->At(k)->Pt() << " " 
//                << ElectronFakeableObjects->At(k)->Eta() << " " 
//                << ElectronFakeableObjects->At(k)->Phi() << endl;    
//         }
//         for (UInt_t k=0;k<MuonFakeableObjects->GetEntries() ; ++k) {
//           cout << "MuonFakeableObject " << k << " : " << MuonFakeableObjects->At(k)->Pt() << " " 
//                << MuonFakeableObjects->At(k)->Eta() << " " 
//                << MuonFakeableObjects->At(k)->Phi() << endl;    
//         }

//         //**********************************************************************************************
//         //Show Generator objects
//         //**********************************************************************************************
//         for (UInt_t d=0; d<GenLeptonsAndTaus->GetEntries(); d++) {
//         cout << "GenLeptonsAndTaus: " << d << " : " << GenLeptonsAndTaus->At(d)->PdgId() << " " 
//              <<GenLeptonsAndTaus->At(d)->Pt() 
//              << " " <<  GenLeptonsAndTaus->At(d)->Eta()  << " " 
//              <<  GenLeptonsAndTaus->At(d)->Phi();
//         cout << endl;
//         }      
//         //print out genjets
//         for (UInt_t d=0; d<fGenJets->GetEntries(); d++) {    
//           if (fGenJets->At(d)->Pt() > 10.0) {
//             cout << "GenJets: " << d << " : " << fGenJets->At(d)->Pt() << " " 
//                  << fGenJets->At(d)->Eta() << " " << fGenJets->At(d)->Phi()  
//                  << " | flavor" << fGenJets->At(d)->MatchedMCFlavor() << endl;
//           }
//         }
//         //Print HepMC table
//         GeneratorTools::PrintHepMCTable(fParticles, true, -1);



      }
    }
    
    //***********************************************************************************************
    //Debug
    //***********************************************************************************************
    
    //Make pt eta distribution for predicted fake electron
    if (CleanLeptons->GetEntries() == 1 && FakeEventHeaders->At(i)->FakeObjectsSize() == 1) {
      if (FakeEventHeaders->At(i)->FakeObject(0)->ObjType() == kElectron) {
          
        fPredictionElectronNumeratorPtEta->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Pt(),
                                                FakeEventHeaders->At(i)->FakeObject(0)->Eta(),
                                                FakeEventHeaders->At(i)->Weight());

        fPredictionElectronDenominatorPtEta->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Pt(),
                                                  FakeEventHeaders->At(i)->FakeObject(0)->Eta());

      }
    }


//       if (!fUseMCFake && leptons->GetEntries()>=2 )  {

//       //**********************************************************************************************
//       //Show all Leptons and Jets
//       //**********************************************************************************************
//       cout << "ModuleName: " << GetName() << endl;
//       cout << "************************************************************************\n";
//       cout << "************************************************************************\n";
//       cout << "Check Leptons: " << CleanLeptons->GetEntries() << endl;
//       for(UInt_t q=0; q<CleanLeptons->GetEntries(); q++)
//         cout << CleanLeptons->At(q)->Pt() <<  " "  
//              << CleanLeptons->At(q)->Eta() << " "  
//              << CleanLeptons->At(q)->Phi() <<  " " 
//              <<endl;
//       cout << "Check Jets: " << OriginalCleanJets->GetEntries() << endl;
//       for(UInt_t q=0; q<OriginalCleanJets->GetEntries(); q++)
//         cout << OriginalCleanJets->At(q)->Pt() <<  " "  
//              << OriginalCleanJets->At(q)->Eta() << " "  
//              << OriginalCleanJets->At(q)->Phi() <<  " " 
//              <<endl;
//       cout << "Check Fakes: " << FakeEventHeaders->At(i)->FakeObjectsSize() << endl;
//       for(UInt_t q=0; q<FakeEventHeaders->At(i)->FakeObjectsSize(); q++)
//         cout << FakeEventHeaders->At(i)->FakeObject(q)->Pt() <<  " "  
//              << FakeEventHeaders->At(i)->FakeObject(q)->Eta() << " "  
//              << FakeEventHeaders->At(i)->FakeObject(q)->Phi() <<  " " 
//              <<endl;
//       cout << "EventWeight: " << FakeEventHeaders->At(i)->Weight() << endl;

//       }





   //*********************************************************************************************
    //Fill some distributions before preselection
    //*********************************************************************************************   
    
    CompositeParticle *dilepton = NULL;

    fNLeptons->Fill(leptons->GetEntries());
    fNLeptons_weighted->Fill(leptons->GetEntries(),eventweight);

    if (leptons->GetEntries()>=2) {

      fLepton1Pt->Fill(leptons->At(0)->Pt());
      fLepton1Pt_weighted->Fill(leptons->At(0)->Pt(),eventweight);
      fLepton2Pt->Fill(leptons->At(1)->Pt());
      fLepton2Pt_weighted->Fill(leptons->At(1)->Pt(),eventweight);
      fLepton1Pt_sysError->Fill(leptons->At(0)->Pt(),eventweightError);
      fLepton2Pt_sysError->Fill(leptons->At(1)->Pt(),eventweightError);

      if (leptons->At(0)->ObjType() == kElectron) {
        fLepton1Pt_e->Fill(leptons->At(0)->Pt());
        fLepton1Pt_e_weighted->Fill(leptons->At(0)->Pt(),eventweight);
      } else if (leptons->At(0)->ObjType() == kMuon) {
        fLepton1Pt_mu->Fill(leptons->At(0)->Pt());
        fLepton1Pt_mu_weighted->Fill(leptons->At(0)->Pt(),eventweight);
      }

      if (leptons->At(1)->ObjType() == kElectron) {
        fLepton2Pt_e->Fill(leptons->At(1)->Pt());
        fLepton2Pt_e_weighted->Fill(leptons->At(1)->Pt(),eventweight);
      } else if (leptons->At(1)->ObjType() == kMuon) {
        fLepton2Pt_mu->Fill(leptons->At(1)->Pt());
        fLepton2Pt_mu_weighted->Fill(leptons->At(1)->Pt(),eventweight);
      }

      dilepton = new CompositeParticle();
      dilepton->AddDaughter(leptons->At(0));
      dilepton->AddDaughter(leptons->At(1));

      //Dilepton Charge will be filled like this
      // -2: -- , -1: -+, 1: +-, 2:++
      if (dilepton->Charge() == 0) {
        if (leptons->At(0)->Charge() == 1) {
          fDileptonCharge->Fill(1.0);
          fDileptonCharge_weighted->Fill(1.0,eventweight);
          fDileptonCharge_sysError->Fill(1.0,eventweightError);
          fDileptonChargeWithoutCorrection_weighted->Fill(1.0,FakeEventHeaders->At(i)->Weight());
        } else {
          fDileptonCharge->Fill(-1.0);
          fDileptonCharge_weighted->Fill(-1.0,eventweight);
          fDileptonCharge_sysError->Fill(-1.0,eventweightError);
          fDileptonChargeWithoutCorrection_weighted->Fill(-1.0,FakeEventHeaders->At(i)->Weight());
        }
      } else {
        fDileptonCharge->Fill(dilepton->Charge());
        fDileptonCharge_weighted->Fill(dilepton->Charge(),eventweight);
        fDileptonCharge_sysError->Fill(dilepton->Charge(),eventweightError);
        fDileptonChargeWithoutCorrection_weighted->Fill(dilepton->Charge(),FakeEventHeaders->At(i)->Weight());
      }
    }

    if (leptons->GetEntries() >= 3) {
      fLepton3Pt->Fill(leptons->At(2)->Pt());
      fLepton3Pt_weighted->Fill(leptons->At(2)->Pt(),eventweight);
    }


    //*********************************************************************************************
    //Kinematic PreSelection
    //*********************************************************************************************
    //make sure 2nd highest pt lepton has Pt > 10
    if (leptons->GetEntries() < 2 || leptons->At(1)->Pt() <= 10) continue;
    
    //make sure the 3rd highest pt lepton has pt <= 10.
    if (leptons->GetEntries() >= 3 && leptons->At(2)->Pt() > 10) continue;
   
    //charge of the leptons should be opposite
    if (dilepton->Charge() != 0) continue;

    //*********************************************************************************************
    //Get Dirty Muons
    //*********************************************************************************************
    ObjArray<Muon> *DirtyMuons = new ObjArray<Muon>;
    for (UInt_t m=0; m<fMuons->GetEntries(); ++m) {
      const Muon *mu = fMuons->At(m);
      if(!mu->GlobalTrk()) continue;
      if(mu->Pt() < 5.0)   continue;

      //remove the fake
       bool isFakedMuon = false;
       for (UInt_t f=0;f<FakeEventHeaders->At(i)->FakeObjectsSize() ; f++) {
         if (mu->HasTrackerTrk() && 
             (dynamic_cast<const mithep::ChargedParticle*>
              (FakeEventHeaders->At(i)->FakeObject(f)->FakeParticle()))->TrackerTrk() &&
             (dynamic_cast<const mithep::ChargedParticle*>
              (FakeEventHeaders->At(i)->FakeObject(f)->FakeParticle()))->TrackerTrk() ==
             mu->TrackerTrk()
           )
           isFakedMuon = true;
       }
      
      //remove clean muons
      bool isCleanMuon = false;
      for (UInt_t j=0; j<CleanMuons->GetEntries(); j++) {
        if(fMuons->At(m) == CleanMuons->At(j)) isCleanMuon = true;
      }

      if(!isCleanMuon 
         && !(isFakedMuon && !fUseMCFake)
        ) DirtyMuons->Add(mu);
    }

    //*********************************************************************************************
    //Get Clean Tracks excluding the good leptons
    //*********************************************************************************************
    ObjArray<Track> *CleanExtraTracks = new ObjArray<Track>;
    int nTracks = 0;

    double z0Average = ( (dynamic_cast<const mithep::ChargedParticle*>(leptons->At(0)))->Trk()->Z0()
      + (dynamic_cast<const mithep::ChargedParticle*>(leptons->At(1)))->Trk()->Z0()) /2 ;
    
    for (UInt_t t=0; t<fTracks->GetEntries(); ++t) {
      bool isLepton = false;
      
      if (MathUtils::DeltaR(fTracks->At(t)->Phi(),fTracks->At(t)->Eta(),leptons->At(0)->Phi(),
                            leptons->At(0)->Eta()) > 0.01 &&
          MathUtils::DeltaR(fTracks->At(t)->Phi(),fTracks->At(t)->Eta(),leptons->At(1)->Phi(),
                            leptons->At(1)->Eta()) > 0.01
        ) {
      } else {
        isLepton = true;
      }
      
      MDB(kAnalysis, 8) {
        cout << "Track " << t << " : "  << fTracks->At(t)->Pt() << " " << fTracks->At(t)->Eta() 
             << " " << fTracks->At(t)->Phi() << " islepton=" << isLepton << endl;              
      }             
      
      if ( !isLepton && fTracks->At(t)->Pt() > 3.0 
           && fTracks->At(t)->NHits() >= 8 
           && fabs(z0Average - fTracks->At(t)->Z0()) < 0.5 ) {
        CleanExtraTracks->Add(fTracks->At(t));
        nTracks++;
      }
    }

    //*********************************************************************************************
    //Define Event Variables
    //*********************************************************************************************
    //delta phi between the 2 leptons in degrees
    double deltaPhiLeptons = MathUtils::DeltaPhi(leptons->At(0)->Phi(), 
                                                 leptons->At(1)->Phi())* 360.0 / 2 / TMath::Pi();
    
    double deltaEtaLeptons = leptons->At(0)->Eta() - leptons->At(1)->Eta();
    
    double deltaPhiDileptonMet = MathUtils::DeltaPhi(caloMet->Phi(), 
                                                     dilepton->Phi())*360.0 / 2 / TMath::Pi();
    
    double mtHiggs = TMath::Sqrt(2.0*dilepton->Pt() * caloMet->Pt()*
                                 (1.0 - cos(deltaPhiDileptonMet * 2 * TMath::Pi() / 360.0)));
    
    //angle between MET and closest lepton
    double deltaPhiMetLepton[2] = {MathUtils::DeltaPhi(caloMet->Phi(), leptons->At(0)->Phi()),
                                   MathUtils::DeltaPhi(caloMet->Phi(), leptons->At(1)->Phi())};
    
    double mTW[2] = {TMath::Sqrt(2.0*leptons->At(0)->Pt()*caloMet->Pt()*
                                 (1.0 - cos(deltaPhiMetLepton[0]))),
                     TMath::Sqrt(2.0*leptons->At(1)->Pt()*caloMet->Pt()*
                                 (1.0 - cos(deltaPhiMetLepton[1])))};
    
    double minDeltaPhiMetLepton = (deltaPhiMetLepton[0] < deltaPhiMetLepton[1])?
      deltaPhiMetLepton[0]:deltaPhiMetLepton[1];
    minDeltaPhiMetLepton = minDeltaPhiMetLepton * 360.0 / 2 / TMath::Pi();
    
    //count the number of central Jets for vetoing
    int nCentralJets = 0;
    for (UInt_t j=0; j<CleanJets->GetEntries(); j++) {
      if (fabs(CleanJets->At(j)->Eta()) < 2.5)
        nCentralJets++;
    }
    
    //Lepton Type
    int finalstateType = -1;
    if (leptons->At(0)->ObjType() == kMuon && leptons->At(1)->ObjType() == kMuon ){ // mumu
      finalstateType = 10;
    } else if(leptons->At(0)->ObjType() == kElectron && leptons->At(1)->ObjType() == kElectron ){ // ee
      finalstateType = 11;
    } else if((leptons->At(0)->ObjType() == kElectron && leptons->At(1)->ObjType() == kMuon) || 
              (leptons->At(0)->ObjType() == kMuon && leptons->At(1)->ObjType() == kElectron)) {
      finalstateType = 12;
    } else {
      cerr << "Error: finalstate lepton type not supported\n";
    }

    //*********************************************************************************************
    //Define Cuts
    //*********************************************************************************************
    const int nCuts = 9;
    bool passCut[nCuts] = {false, false, false, false,
                         false, false, false, false, false};
    
    if(leptons->At(0)->Pt() > 20.0 &&
       leptons->At(1)->Pt() > 10.0 &&
       caloMet->Pt()    > 30.0 &&
       dilepton->Mass() > 12.0
      )                              passCut[0] = true;
    //above cuts are for preselction to be fed into TMVA
    
    if(nCentralJets < 1)     passCut[1] = true;
    
    if (finalstateType == 10){ // mumu
      if(caloMet->Pt()	> 50.0 &&
         caloMet->Pt()	< 200.0)         passCut[2] = true;
      if(deltaPhiLeptons	< 45.0)          passCut[3] = true;
      if(dilepton->Mass()	< 50.0)          passCut[4] = true;
      if(leptons->At(0)->Pt()	> 35.0   &&
         leptons->At(0)->Pt()	< 55.0)          passCut[5] = true;
      if(leptons->At(1)->Pt()	> 25.0)          passCut[6] = true;
    }
    else if(finalstateType == 11 ){ // ee
      if(caloMet->Pt()	  > 51.0   &&
         caloMet->Pt()	  < 200.0)       passCut[2] = true;
      if(deltaPhiLeptons	  < 45.0)        passCut[3] = true;
      if(dilepton->Mass()	  < 40.0)        passCut[4] = true;
      if(leptons->At(0)->Pt() > 25.0   &&
	 leptons->At(0)->Pt() < 49.0)        passCut[5] = true;
      if(leptons->At(1)->Pt() > 25.0)        passCut[6] = true;
    }      
    else if(finalstateType == 12) { //emu
      if(caloMet->Pt()			> 45.0 &&
         caloMet->Pt()			< 105.0) passCut[2] = true;
      if(deltaPhiLeptons		< 70.0)  passCut[3] = true;
      if(dilepton->Mass()		< 45.0)  passCut[4] = true;
      if(leptons->At(0)->Pt()	> 25.0   &&
         leptons->At(0)->Pt()	< 50.0)  passCut[5] = true;
      if(leptons->At(1)->Pt()	> 25.0)  passCut[6] = true;
    }
    
    if (DirtyMuons->GetEntries() < 1)      passCut[7] = true;
    if (CleanExtraTracks->GetEntries() < 4)     passCut[8] = true;

    //*********************************************************************************************
    //Final Decision
    //*********************************************************************************************
    bool passAllCuts = true;
    for(int c=0; c<nCuts; c++) passAllCuts = passAllCuts & passCut[c];
    
    //Cut Selection Histograms
    fHWWSelection->Fill(-1,eventweight);
    fHWWSelection_sysError->Fill(-1,eventweightError);

    if (finalstateType == 10 )
      fHWWToMuMuSelection->Fill(-1,eventweight);
    else if(finalstateType == 11 )
      fHWWToEESelection->Fill(-1,eventweight);
    else if(finalstateType == 12 )
      fHWWToEMuSelection->Fill(-1,eventweight);
    
    
    for (int k=0;k<nCuts;k++) {
      bool pass = true;
      bool passPreviousCut = true;
      for (int p=0;p<=k;p++) {
        pass = (pass && passCut[p]);
        if (p<k)
          passPreviousCut = (passPreviousCut&& passCut[p]);
      }
      
      if (pass) {
        fHWWSelection->Fill(k,eventweight);
        fHWWSelection_sysError->Fill(k,eventweightError);

        if (finalstateType == 10 )
          fHWWToMuMuSelection->Fill(k,eventweight);
        else if(finalstateType == 11)
          fHWWToEESelection->Fill(k,eventweight);
        else if(finalstateType == 12)
          fHWWToEMuSelection->Fill(k,eventweight);
      }
    }

    //*****************************************************************************************
    //Histograms after no cuts
    //*****************************************************************************************
    fLeptonEta->Fill(leptons->At(0)->Eta(),eventweight); 
    fLeptonEta->Fill(leptons->At(1)->Eta(),eventweight);
    fLeptonPtMax->Fill(leptons->At(0)->Pt(),eventweight);
    fLeptonPtMin->Fill(leptons->At(1)->Pt(),eventweight);
    fMetPtHist->Fill(caloMet->Pt(),eventweight);                             
    fMetPhiHist->Fill(caloMet->Phi(),eventweight);                            
    fUncorrMetPtHist->Fill(originalCaloMet->Pt(),eventweight); 
    fUncorrMetPhiHist->Fill(originalCaloMet->Phi(),eventweight);
    fDeltaPhiLeptons->Fill(deltaPhiLeptons,eventweight);
    fDeltaEtaLeptons->Fill(deltaEtaLeptons,eventweight);
    fDileptonMass->Fill(dilepton->Mass(),eventweight);   

    fLeptonEta_sysError->Fill(leptons->At(0)->Eta(),eventweightError); 
    fLeptonEta_sysError->Fill(leptons->At(1)->Eta(),eventweightError);
    fLeptonPtMax_sysError->Fill(leptons->At(0)->Pt(),eventweightError);
    fLeptonPtMin_sysError->Fill(leptons->At(1)->Pt(),eventweightError);
    fMetPtHist_sysError->Fill(caloMet->Pt(),eventweightError);                             
    fMetPhiHist_sysError->Fill(caloMet->Phi(),eventweightError);                            
    fUncorrMetPtHist_sysError->Fill(originalCaloMet->Pt(),eventweightError); 
    fUncorrMetPhiHist_sysError->Fill(originalCaloMet->Phi(),eventweightError);
    fDeltaPhiLeptons_sysError->Fill(deltaPhiLeptons,eventweightError);
    fDeltaEtaLeptons_sysError->Fill(deltaEtaLeptons,eventweightError);
    fDileptonMass_sysError->Fill(dilepton->Mass(),eventweightError);   
 
    if (finalstateType == 10 ) {
      fLeptonEta_mumu->Fill(leptons->At(0)->Pt(),eventweight); 
      fLeptonEta_mumu->Fill(leptons->At(1)->Pt(),eventweight);
      fLeptonPtMax_mumu->Fill(leptons->At(0)->Pt(),eventweight);
      fLeptonPtMin_mumu->Fill(leptons->At(1)->Pt(),eventweight);
      fMetPtHist_mumu->Fill(caloMet->Pt(),eventweight);     
      fMetPhiHist_mumu->Fill(caloMet->Phi(),eventweight);  
      fUncorrMetPtHist_mumu->Fill(originalCaloMet->Pt(),eventweight);
      fUncorrMetPhiHist_mumu->Fill(originalCaloMet->Phi(),eventweight); 
      fDeltaPhiLeptons_mumu->Fill(deltaPhiLeptons,eventweight);
      fDeltaEtaLeptons_mumu->Fill(deltaEtaLeptons,eventweight);
      fDileptonMass_mumu->Fill(dilepton->Mass(),eventweight);
    } else if(finalstateType == 11 ) {
      fLeptonEta_ee->Fill(leptons->At(0)->Pt(),eventweight); 
      fLeptonEta_ee->Fill(leptons->At(1)->Pt(),eventweight);
      fLeptonPtMax_ee->Fill(leptons->At(0)->Pt(),eventweight);
      fLeptonPtMin_ee->Fill(leptons->At(1)->Pt(),eventweight);
      fMetPtHist_ee->Fill(caloMet->Pt(),eventweight); 
      fMetPhiHist_ee->Fill(caloMet->Phi(),eventweight); 
      fUncorrMetPtHist_ee->Fill(originalCaloMet->Pt(),eventweight);
      fUncorrMetPhiHist_ee->Fill(originalCaloMet->Phi(),eventweight);  
      fDeltaPhiLeptons_ee->Fill(deltaPhiLeptons,eventweight);
      fDeltaEtaLeptons_ee->Fill(deltaEtaLeptons,eventweight);
      fDileptonMass_ee->Fill(dilepton->Mass(),eventweight);
    } else if(finalstateType == 12 ) {
      fLeptonEta_emu->Fill(leptons->At(0)->Pt(),eventweight); 
      fLeptonEta_emu->Fill(leptons->At(1)->Pt(),eventweight);
      fLeptonPtMax_emu->Fill(leptons->At(0)->Pt(),eventweight);
      fLeptonPtMin_emu->Fill(leptons->At(1)->Pt(),eventweight);
      fMetPtHist_emu->Fill(caloMet->Pt(),eventweight);
      fMetPhiHist_emu->Fill(caloMet->Phi(),eventweight);
      fUncorrMetPtHist_emu->Fill(originalCaloMet->Pt(),eventweight);
      fUncorrMetPhiHist_emu->Fill(originalCaloMet->Phi(),eventweight);
      fDeltaPhiLeptons_emu->Fill(deltaPhiLeptons,eventweight);
      fDeltaEtaLeptons_emu->Fill(deltaEtaLeptons,eventweight);
      fDileptonMass_emu->Fill(dilepton->Mass(),eventweight);
    }

    //*********************************************************************************************
    // N-1 Histograms
    //*********************************************************************************************

    //N Jet Veto  
    bool pass = true;
    for (int k=0;k<nCuts;k++) {
      if (k != 1) {
        pass = (pass && passCut[k]);      
      }
    }
    if (pass) {
      fNCentralJets_NMinusOne->Fill(nCentralJets,eventweight);
      if (finalstateType == 10) {
        fNCentralJets_NMinusOne_mumu->Fill(nCentralJets,eventweight);
      } else if (finalstateType == 11) {
        fNCentralJets_NMinusOne_ee->Fill(nCentralJets,eventweight);
      } else if (finalstateType == 12) {
        fNCentralJets_NMinusOne_emu->Fill(nCentralJets,eventweight);
      }
    }     

    //Met Cut
    pass = true;
    for (int k=0;k<nCuts;k++) {
      if (k != 2) {
        pass = (pass && passCut[k]);      
      }
    }
    if (pass) {
      fMetPtHist_NMinusOne->Fill(caloMet->Pt(),eventweight);  
      if (finalstateType == 10) {
         fMetPtHist_NMinusOne_mumu->Fill(caloMet->Pt(),eventweight);  
      } else if (finalstateType == 11) {
        fMetPtHist_NMinusOne_ee->Fill(caloMet->Pt(),eventweight);  
      } else if (finalstateType == 12) {
        fMetPtHist_NMinusOne_emu->Fill(caloMet->Pt(),eventweight);  
      }      
    }
    
    //DeltaPhiLeptons
    pass = true;
    for (int k=0;k<nCuts;k++) {
      if (k != 3) {
        pass = (pass && passCut[k]);      
      }
    }
    if (pass) {
      fDeltaPhiLeptons_NMinusOne->Fill(deltaPhiLeptons,eventweight); 
      if (finalstateType == 10) {
        fDeltaPhiLeptons_NMinusOne_mumu->Fill(deltaPhiLeptons,eventweight); 
      } else if (finalstateType == 11) {
        fDeltaPhiLeptons_NMinusOne_ee->Fill(deltaPhiLeptons,eventweight); 
      } else if (finalstateType == 12) {
        fDeltaPhiLeptons_NMinusOne_emu->Fill(deltaPhiLeptons,eventweight); 
      }  
    }
    
    //dilepton mass
    pass = true;
    for (int k=0;k<nCuts;k++) {
      if (k != 4)
        pass = (pass && passCut[k]);    
    }
    if (pass) {
      fDileptonMass_NMinusOne->Fill(dilepton->Mass(),eventweight);
      if (finalstateType == 10) {
        fDileptonMass_NMinusOne_mumu->Fill(dilepton->Mass(),eventweight);
      } else if (finalstateType == 11) {
        fDileptonMass_NMinusOne_ee->Fill(dilepton->Mass(),eventweight);
      } else if (finalstateType == 12) {
        fDileptonMass_NMinusOne_emu->Fill(dilepton->Mass(),eventweight);
      }         
    }
    
    //Lepton Pt Max
    pass = true;
    for (int k=0;k<nCuts;k++) {
      if (k != 5) {
        pass = (pass && passCut[k]);      
      }
    }
    if (pass) {
      fLeptonPtMax_NMinusOne->Fill(leptons->At(0)->Pt(),eventweight);
      if (finalstateType == 10) {
        fLeptonPtMax_NMinusOne_mumu->Fill(leptons->At(0)->Pt(),eventweight);
      } else if (finalstateType == 11) {
        fLeptonPtMax_NMinusOne_ee->Fill(leptons->At(0)->Pt(),eventweight);
      } else if (finalstateType == 12) {
        fLeptonPtMax_NMinusOne_emu->Fill(leptons->At(0)->Pt(),eventweight);
      }
    }
    
    //Lepton Pt Min
    pass = true;
    for (int k=0;k<nCuts;k++) {
      if (k != 6) {
        pass = (pass && passCut[k]);      
      }
    }
    if (pass) {
      fLeptonPtMin_NMinusOne->Fill(leptons->At(1)->Pt(),eventweight);
      if (finalstateType == 10) {
        fLeptonPtMin_NMinusOne_mumu->Fill(leptons->At(1)->Pt(),eventweight);
      } else if (finalstateType == 11) {
        fLeptonPtMin_NMinusOne_ee->Fill(leptons->At(1)->Pt(),eventweight);
      } else if (finalstateType == 12) {
        fLeptonPtMin_NMinusOne_emu->Fill(leptons->At(1)->Pt(),eventweight);
      }
    }
    
    //NDirtyMuons
    pass = true;
    for (int k=0;k<nCuts;k++) {
      if (k != 7)
        pass = (pass && passCut[k]);    
    }
    if (pass) {
      fNDirtyMuonsHist_NMinusOne->Fill(DirtyMuons->GetEntries(),eventweight);
      if (finalstateType == 10) {
        fNDirtyMuonsHist_NMinusOne_mumu->Fill(DirtyMuons->GetEntries(),
                                              eventweight);
      } else if (finalstateType == 11) {
        fNDirtyMuonsHist_NMinusOne_ee->Fill(DirtyMuons->GetEntries(),
                                            eventweight);
      } else if (finalstateType == 12) {
        fNDirtyMuonsHist_NMinusOne_emu->Fill(DirtyMuons->GetEntries(),
                                             eventweight);
      } 
    }
    
    //NCleanExtraTracks
    pass = true;
    for (int k=0;k<nCuts;k++) {
      if (k != 8)
        pass = (pass && passCut[k]);    
    }
    if (pass) {
      fNCleanExtraTracksHist_NMinusOne->Fill(CleanExtraTracks->GetEntries(),
                                             eventweight);
      if (finalstateType == 10) {
        fNCleanExtraTracksHist_NMinusOne_mumu->Fill(CleanExtraTracks->GetEntries(),
                                                    eventweight);
      } else if (finalstateType == 11) {
        fNCleanExtraTracksHist_NMinusOne_ee->Fill(CleanExtraTracks->GetEntries(),
                                                  eventweight);
      } else if (finalstateType == 12) {
        fNCleanExtraTracksHist_NMinusOne_emu->Fill(CleanExtraTracks->GetEntries(),
                                                   eventweight);
      } 
    }     

    //*********************************************************************************************
    //Plots after all Cuts
    //*********************************************************************************************
    if (passAllCuts) {
      fMinDeltaPhiLeptonMet_afterCuts->Fill(minDeltaPhiMetLepton,eventweight);
      fMtLepton1_afterCuts->Fill(mTW[0],eventweight);
      fMtLepton2_afterCuts->Fill(mTW[1],eventweight);
      fMtHiggs_afterCuts->Fill(mtHiggs,eventweight);
      fLeptonPtPlusMet_afterCuts->Fill(leptons->At(0)->Pt()+leptons->At(1)->Pt()+caloMet->Pt(),
                                       eventweight);
      if (finalstateType == 10) {
        fMinDeltaPhiLeptonMet_afterCuts_mumu->Fill(minDeltaPhiMetLepton,
                                                   eventweight);         
        fMtLepton1_afterCuts_mumu->Fill(mTW[0],eventweight);
        fMtLepton2_afterCuts_mumu->Fill(mTW[1],eventweight);
        fMtHiggs_afterCuts_mumu->Fill(mtHiggs,eventweight);
        fLeptonPtPlusMet_afterCuts_mumu->Fill(leptons->At(0)->Pt()+leptons->At(1)->Pt()+
                                              caloMet->Pt(),eventweight);
      } else if (finalstateType == 11) {
        fMinDeltaPhiLeptonMet_afterCuts_ee->Fill(minDeltaPhiMetLepton,
                                                 eventweight); 
        fMtLepton1_afterCuts_ee->Fill(mTW[0],eventweight);
        fMtLepton2_afterCuts_ee->Fill(mTW[1],eventweight);
        fMtHiggs_afterCuts_ee->Fill(mtHiggs,eventweight);
        fLeptonPtPlusMet_afterCuts_ee->Fill(leptons->At(0)->Pt()+leptons->At(1)->Pt()+
                                            caloMet->Pt(),eventweight);
      } else if (finalstateType == 12) {
        fMinDeltaPhiLeptonMet_afterCuts_emu->Fill(minDeltaPhiMetLepton,
                                                  eventweight);    
        fMtLepton1_afterCuts_emu->Fill(mTW[0],eventweight);
        fMtLepton2_afterCuts_emu->Fill(mTW[1],eventweight);
        fMtHiggs_afterCuts_emu->Fill(mtHiggs,eventweight);
        fLeptonPtPlusMet_afterCuts_emu->Fill(leptons->At(0)->Pt()+leptons->At(1)->Pt()+
                                             caloMet->Pt(),eventweight);
      }
    }
    if (!fUseMCFake) {
      delete leptons;
      delete CleanJets;
      delete caloMet;
    }
    delete dilepton;
    delete DirtyMuons;
    delete CleanExtraTracks;
  }

  //***********************************************************************************************
  //Print Debug Info
  //***********************************************************************************************
  MDB(kAnalysis, 4) {
    if (printdebug) {
      cout << "LumiSec: " << GetEventHeader()->LumiSec() << " RunNum: " << GetEventHeader()->RunNum()
           << " EventNum: " << GetEventHeader()->EvtNum() << endl;
      cout << "***********************************************************************\n";
      cout << "Numerators\n";
      cout << "***********************************************************************\n";
      for (UInt_t i=0; i<CleanMuons->GetEntries(); i++) {        
        cout << "\nMuon " << i << " : " << CleanMuons->At(i)->Pt() << " " 
             << CleanMuons->At(i)->Eta() << " " << CleanMuons->At(i)->Phi() 
             << endl;     
      }
      
      for (UInt_t i=0; i<CleanElectrons->GetEntries(); i++) {  
        
        Double_t ElectronPUsingSCEnergy = 
          TMath::Sqrt(CleanElectrons->At(i)->SCluster()->Energy()*
                      CleanElectrons->At(i)->SCluster()->Energy() - 
                      CleanElectrons->At(i)->Mass()*
                      CleanElectrons->At(i)->Mass());
        Double_t ElectronPtUsingSCEnergy = 
          TMath::Abs(ElectronPUsingSCEnergy*
                     TMath::Cos(CleanElectrons->At(i)->Trk()->Lambda()));
        
        cout << "\nElectron " << i << " : " << CleanElectrons->At(i)->Pt() << "/" 
             << ElectronPtUsingSCEnergy << " " 
             << CleanElectrons->At(i)->Eta() << " " << CleanElectrons->At(i)->Phi() << " " 
             << CleanElectrons->At(i)->GsfTrk() << endl; 
        if (CleanElectrons->At(i)->GsfTrk()) {
          cout << "Electron GsfTrack : " << CleanElectrons->At(i)->GsfTrk()->Pt() << " "
               << CleanElectrons->At(i)->GsfTrk()->Eta() << " " 
               << CleanElectrons->At(i)->GsfTrk()->Phi() << endl;
        }
        if (CleanElectrons->At(i)->SCluster()) {
          cout << "Electron SCluster : " << CleanElectrons->At(i)->SCluster()->Energy() << " "
               << CleanElectrons->At(i)->SCluster()->Eta() << " " 
               << CleanElectrons->At(i)->SCluster()->Phi() << endl;
        }
        cout << "Isolation : " << CleanElectrons->At(i)->TrackIsolation() << " "
             << CleanElectrons->At(i)->EcalJurassicIsolation() << " "
             << CleanElectrons->At(i)->HcalIsolation() << " "
             << endl;
        double iso = IsolationTools::TrackIsolation(CleanElectrons->At(i)->Trk(), 
                                                    0.3, 0.015, 1.0, 0.2,
                                                    fTracks);
        cout << iso << endl;
      }

      for (UInt_t i=0; i<CleanMuons->GetEntries(); i++) {  
        cout << "CleanMuon " << i << " ";
        if (CleanMuons->At(i)->HasGlobalTrk())
          cout << " global ";
        else if (CleanMuons->At(i)->HasTrackerTrk())
          cout << " trackeronly ";
        else if (CleanMuons->At(i)->HasStandaloneTrk())
          cout << " standalone ";
        else
          cout << " none ";
        
        cout << " : " << CleanMuons->At(i)->Pt() << " " 
             << CleanMuons->At(i)->Eta() << " " << CleanMuons->At(i)->Phi() << " " 
             << " tracker track: ";
        if (CleanMuons->At(i)->HasTrackerTrk()) {
          cout << CleanMuons->At(i)->TrackerTrk()->Pt() << " "  
               << CleanMuons->At(i)->TrackerTrk()->Eta() << " " 
               << CleanMuons->At(i)->TrackerTrk()->Phi() << " : ";
        } 
        if (CleanMuons->At(i)->HasStandaloneTrk()) {
          cout << " standalone track : " << CleanMuons->At(i)->StandaloneTrk()->Pt() << " "  
               << CleanMuons->At(i)->StandaloneTrk()->Eta() << " " 
               << CleanMuons->At(i)->StandaloneTrk()->Phi() << " : ";
        } 
        if (CleanMuons->At(i)->HasGlobalTrk()) {
          cout << " global track : " << CleanMuons->At(i)->GlobalTrk()->Pt() << " "  
               << CleanMuons->At(i)->GlobalTrk()->Eta() << " " 
               << CleanMuons->At(i)->GlobalTrk()->Phi() << " : ";
        }
        cout << "Isolation : " << CleanMuons->At(i)->IsoR03SumPt() << " "
             << CleanMuons->At(i)->IsoR03EmEt() << " "
             << CleanMuons->At(i)->IsoR03HadEt() << " "
             << " ";
        double iso = IsolationTools::TrackIsolation(CleanMuons->At(i)->Trk(), 0.3, 0.015, 1.0, 0.2,
                                                    fTracks);
        cout << iso << endl;
        cout << endl;
      }
      
      cout << "***********************************************************************\n";
      cout << "Denominators\n";
      cout << "***********************************************************************\n";
      for (UInt_t d=0; d<ElectronFakeableObjects->GetEntries(); d++) {
        cout << "ElectronFakeableObjects  " << d << " : " << ElectronFakeableObjects->At(d)->Pt() 
             << " " <<  ElectronFakeableObjects->At(d)->Eta()  << " " 
             <<  ElectronFakeableObjects->At(d)->Phi() << " ";
        
        if (ElectronFakeableObjects->At(d)->GsfTrk()) {
          cout << "   GsfTrack: " << ElectronFakeableObjects->At(d)->GsfTrk()->Pt() << " "
               << ElectronFakeableObjects->At(d)->GsfTrk()->Eta() << " " 
               << ElectronFakeableObjects->At(d)->GsfTrk()->Phi() << " ";
        }
        if (ElectronFakeableObjects->At(d)->SCluster()) {
          
          cout <<  " --- SCluster: ";          
          cout << ElectronFakeableObjects->At(d)->SCluster()->Energy() << " "
               << ElectronFakeableObjects->At(d)->SCluster()->Eta() << " "
               << ElectronFakeableObjects->At(d)->SCluster()->Phi();
        }
        cout << endl;
      }

    for (UInt_t d=0; d<MuonFakeableObjects->GetEntries(); d++) {
      cout << " : " << MuonFakeableObjects->At(d)->Pt() << " " 
           << MuonFakeableObjects->At(d)->Eta() << " " << MuonFakeableObjects->At(d)->Phi() << " " 
           << " tracker track: ";
      if (MuonFakeableObjects->At(d)->HasTrackerTrk()) {
        cout << MuonFakeableObjects->At(d)->TrackerTrk()->Pt() << " "  
             << MuonFakeableObjects->At(d)->TrackerTrk()->Eta() << " " 
             << MuonFakeableObjects->At(d)->TrackerTrk()->Phi() << " : ";
      } 
      if (MuonFakeableObjects->At(d)->HasStandaloneTrk()) {
        cout << " standalone track : " << MuonFakeableObjects->At(d)->StandaloneTrk()->Pt() << " "  
             << MuonFakeableObjects->At(d)->StandaloneTrk()->Eta() << " " 
             << MuonFakeableObjects->At(d)->StandaloneTrk()->Phi() << " : ";
      } 
      if (MuonFakeableObjects->At(d)->HasGlobalTrk()) {
        cout << " global track : " << MuonFakeableObjects->At(d)->GlobalTrk()->Pt() << " "  
             << MuonFakeableObjects->At(d)->GlobalTrk()->Eta() << " " 
             << MuonFakeableObjects->At(d)->GlobalTrk()->Phi() << " : ";
      }
      cout << "Isolation : " << MuonFakeableObjects->At(d)->IsoR03SumPt() << " "
           << MuonFakeableObjects->At(d)->IsoR03EmEt() << " "
           << MuonFakeableObjects->At(d)->IsoR03HadEt() << " "
           << " ";
      double iso = IsolationTools::TrackIsolation(MuonFakeableObjects->At(d)->Trk(), 
                                                  0.3, 0.015, 1.0, 0.2,fTracks);
      cout << iso << endl;
      cout << endl;
    }

       
      for (UInt_t l=0; l<OriginalCleanJets->GetEntries(); l++) {
        cout << "OriginalCleanJets " << l << " : " 
             << OriginalCleanJets->At(l)->Pt() << " " 
             << OriginalCleanJets->At(l)->Eta() << " " 
             << OriginalCleanJets->At(l)->Phi() << " " 
             << endl;
      }
      
      for (UInt_t d=0; d<GenLeptonsAndTaus->GetEntries(); d++) {
        cout << "GenLeptonsAndTaus: " << d << " : " << GenLeptonsAndTaus->At(d)->Status() << " " 
             << GenLeptonsAndTaus->At(d)->PdgId() << " " 
             <<GenLeptonsAndTaus->At(d)->Pt() 
             << " " <<  GenLeptonsAndTaus->At(d)->Eta()  << " " 
             <<  GenLeptonsAndTaus->At(d)->Phi();
        cout << endl;
      }        
      for (UInt_t d=0; d<fGenJets->GetEntries(); d++) {        
        cout << "fGenJets " << d << " : " << fGenJets->At(d)->Pt() << " " 
             << fGenJets->At(d)->Eta() << " " << fGenJets->At(d)->Phi()  
             << " flavor" << fGenJets->At(d)->MatchedMCFlavor() << endl;
      }
      GeneratorTools::PrintHepMCTable(fParticles, true, -1);
      
    }
  }
  delete FakeableObjects;
  delete GenLeptonsAndTaus;
  if (fUseMCFake) {
    delete FakeEventHeaders->At(0);
    delete FakeEventHeaders;
  }
  return;
}
//--------------------------------------------------------------------------------------------------
void WJetsFakeValidationMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fMCPartBranchName, fParticles);
  ReqBranch(fTrackBranchName,  fTracks);
  ReqBranch(fMuonBranchName,   fMuons);
  ReqBranch(fGenJetBranchName, fGenJets);

  //***********************************************************************************************
  // Fake Histograms
  //*********************************************************************************************** 
  AddTH1(fElectronDenominatorPt, "hElectronDenominatorPt", 
         ";ElectronDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronDenominatorEta, "hElectronDenominatorEta", 
         ";ElectronDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronDenominatorPhi, "hElectronDenominatorPhi", 
         ";ElectronDenominatorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fMuonDenominatorPt, "hMuonDenominatorPt", 
         ";MuonDenominatorPt;Number of Events", 50, 0, 250);
  AddTH1(fMuonDenominatorEta, "hMuonDenominatorEta", 
         ";MuonDenominatorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonDenominatorPhi, "hMuonDenominatorPhi", 
         ";MuonDenominatorPhi;Number of Events", 100, -3.2, 3.2);

  AddTH1(fFakeElectronPt, "hFakeElectronPt", ";FakeElectronPt;Number of Events", 50, 0, 250);
  AddTH1(fFakeElectronEta, "hFakeElectronEta", ";FakeElectronEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fFakeElectronPhi, "hFakeElectronPhi", ";FakeElectronPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fFakeMuonPt, "hFakeMuonPt", ";FakeMuonPt;Number of Events", 50, 0, 250);
  AddTH1(fFakeMuonEta, "hFakeMuonEta", ";FakeMuonEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fFakeMuonPhi, "hFakeMuonPhi", ";FakeMuonPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fFakeElectronPt_weighted, "hFakeElectronPt_weighted", ";FakeElectronPt;Number of Events", 50, 0, 250);
  AddTH1(fFakeElectronEta_weighted, "hFakeElectronEta_weighted", ";FakeElectronEta;Number of Events", 
         100, -5.0, 5.0);
  AddTH1(fFakeElectronPhi_weighted, "hFakeElectronPhi_weighted", ";FakeElectronPhi;Number of Events", 
         100, -3.2, 3.2);
  AddTH1(fFakeMuonPt_weighted, "hFakeMuonPt_weighted", ";FakeMuonPt;Number of Events", 50, 0, 250);
  AddTH1(fFakeMuonEta_weighted, "hFakeMuonEta_weighted", ";FakeMuonEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fFakeMuonPhi_weighted, "hFakeMuonPhi_weighted", ";FakeMuonPhi;Number of Events", 100, -3.2, 3.2);

  AddTH1(fFakeElectronPt_weighted_PlusMinus, "hFakeElectronPt_weighted_PlusMinus", ";FakeElectronPt;Number of Events", 50, 0, 250);
  AddTH1(fFakeElectronEta_weighted_PlusMinus, "hFakeElectronEta_weighted_PlusMinus", ";FakeElectronEta;Number of Events", 
         100, -5.0, 5.0);
  AddTH1(fFakeElectronPhi_weighted_PlusMinus, "hFakeElectronPhi_weighted_PlusMinus", ";FakeElectronPhi;Number of Events", 
         100, -3.2, 3.2);


  AddTH1(fFakeElectronCategory,
         "hFakeElectronCategory", 
         ";FakeElectronCategory;Number of Events", 8,-0.5,7.5);
  TAxis *xa = fFakeElectronCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");

  AddTH1(fFakeElectronCategoryPlusMinus,
         "hFakeElectronCategoryPlusMinus", 
         ";FakeElectronCategory;Number of Events", 8,-0.5,7.5);
  xa = fFakeElectronCategoryPlusMinus->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");


  AddTH1(fAllFakeElectronCategory,
         "hAllFakeElectronCategory", 
         ";FakeElectronCategory;Number of Events", 8,-0.5,7.5);
  xa = fAllFakeElectronCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");

  AddTH1(fAllFakeElectronDenominatorCategory,
         "hAllFakeElectronDenominatorCategory", 
         ";FakeElectronCategory;Number of Events", 8,-0.5,7.5);
  xa = fAllFakeElectronDenominatorCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"Conversion");
  xa->SetBinLabel(3,"Charged Pi");
  xa->SetBinLabel(4,"Charged K");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"Brem");
  xa->SetBinLabel(7,"PromptPhoton");
  xa->SetBinLabel(8,"Other");


  AddTH1(fFakeMuonCategory,
         "hFakeMuonCategory", 
         ";FakeMuonCategory;Number of Events", 8,-0.5,7.5);
  xa = fFakeMuonCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"PionDecayInFlight");
  xa->SetBinLabel(3,"KaonDecayInFlight");
  xa->SetBinLabel(4,"MuonFromOtherProcess");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"PionPunchThru");
  xa->SetBinLabel(7,"OtherNonMuon");
  xa->SetBinLabel(8,"NoAncestorFound");

  AddTH1(fAllFakeMuonCategory,
         "hAllFakeMuonCategory", 
         ";FakeMuonCategory;Number of Events", 8,-0.5,7.5);
  xa = fAllFakeMuonCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"PionDecayInFlight");
  xa->SetBinLabel(3,"KaonDecayInFlight");
  xa->SetBinLabel(4,"MuonFromOtherProcess");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"PionPunchThru");
  xa->SetBinLabel(7,"OtherNonMuon");
  xa->SetBinLabel(8,"NoAncestorFound");

  AddTH1(fAllFakeMuonDenominatorCategory,
         "hAllFakeMuonDenominatorCategory", 
         ";FakeMuonCategory;Number of Events", 8,-0.5,7.5);
  xa = fAllFakeMuonDenominatorCategory->GetXaxis();
  xa->SetBinLabel(1,"NoMatch");
  xa->SetBinLabel(2,"PionDecayInFlight");
  xa->SetBinLabel(3,"KaonDecayInFlight");
  xa->SetBinLabel(4,"MuonFromOtherProcess");
  xa->SetBinLabel(5,"HeavyFlavor");
  xa->SetBinLabel(6,"PionPunchThru");
  xa->SetBinLabel(7,"OtherNonMuon");
  xa->SetBinLabel(8,"NoAncestorFound");


  //***********************************************************************************************
  // For debugging purposes
  //***********************************************************************************************
  AddTH1(fRealElectronPt, "hRealElectronPt", 
         ";ElectronPt;Number of Events", 50, 0, 250);
  AddTH1(fRealElectronEta, "hRealElectronEta", 
         ";ElectronEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fRealElectronPhi, "hRealElectronPhi", 
         ";ElectronPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fElectron1Pt, "hElectron1Pt", 
         ";ElectronPt;Number of Events", 50, 0, 250);
  AddTH1(fElectron1Eta, "hElectron1Eta", 
         ";ElectronEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectron1Phi, "hElectron1Phi", 
         ";ElectronPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fNCleanElectrons, "hNCleanElectrons", 
         ";Number of Clean Electrons;Number of Events", 5, -0.5,4.5);
  AddTH1(fNDenominators, "hNDenominators", 
         ";Number of Electron Denominators;Number of Events", 5, -0.5,4.5);
  AddTH2(fNCleanElectronsVsNDenominators, "hNCleanElectronsVsNDenominators", 
         ";Number of Clean Electrons;Number of Electron Denominators;Number of Events", 
         5, -0.5,4.5, 5, -0.5,4.5);


  AddTH1(fElectronNumeratorPt, "hElectronNumeratorPt", 
         ";ElectronNumeratorPt;Number of Events", 50, 0, 250);
  AddTH1(fElectronNumeratorEta, "hElectronNumeratorEta", 
         ";ElectronNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronNumeratorPhi, "hElectronNumeratorPhi", 
         ";ElectronNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fMuonNumeratorPt, "hMuonNumeratorPt", 
         ";MuonNumeratorEt;Number of Events", 50, 0, 250);
  AddTH1(fMuonNumeratorEta, "hMuonNumeratorEta", 
         ";MuonNumeratorEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonNumeratorPhi, "hMuonNumeratorPhi", 
         ";MuonNumeratorPhi;Number of Events", 100, -3.2, 3.2);
  AddTH2(fAllElectronNumeratorPtEta, "hAllElectronNumeratorPtEta", 
         ";ElectronNumeratorPt;ElectronNumeratorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);
  AddTH2(fAllElectronDenominatorPtEta, "hAllElectronDenominatorPtEta", 
         ";ElectronDenominatorPt;ElectronDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);
  AddTH2(fFakeSelectionElectronNumeratorPtEta, "hFakeSelectionElectronNumeratorPtEta", 
         ";ElectronNumeratorPt;ElectronNumeratorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);
  AddTH2(fFakeSelectionElectronDenominatorPtEta, "hFakeSelectionElectronDenominatorPtEta", 
         ";ElectronDenominatorPt;ElectronDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  AddTH2(fPredictionElectronDenominatorPtEta, "hPredictionElectronDenominatorPtEta", 
         ";ElectronDenominatorPt;ElectronDenominatorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);
  AddTH2(fPredictionElectronNumeratorPtEta, "hPredictionElectronNumeratorPtEta", 
         ";ElectronNumeratorPt;ElectronNumeratorEta;Number of Events", 
         50, 0, 250, 100,-5.0,5.0);

  //***********************************************************************************************
  // Before Preselection Histograms
  //***********************************************************************************************
  AddTH1(fDileptonCharge, "hDileptonCharge", ";DileptonCharge;Number of Events", 5, -2.5, 2.5);
  xa = fDileptonCharge->GetXaxis();
  xa->SetBinLabel(1,"--");
  xa->SetBinLabel(2,"-+");
  xa->SetBinLabel(4,"+-");
  xa->SetBinLabel(5,"++");

  AddTH1(fDileptonChargeUsingDenominator, "hDileptonChargeUsingDenominator", ";DileptonChargeUsingDenominator;Number of Events", 5, -2.5, 2.5);
  xa = fDileptonChargeUsingDenominator->GetXaxis();
  xa->SetBinLabel(1,"--");
  xa->SetBinLabel(2,"-+");
  xa->SetBinLabel(4,"+-");
  xa->SetBinLabel(5,"++");

  AddTH1(fDileptonChargeNotMatchedToDenominator, "hDileptonChargeNotMatchedToDenominator", ";DileptonChargeNotMatchedToDenominator;Number of Events", 5, -2.5, 2.5);
  xa = fDileptonChargeNotMatchedToDenominator->GetXaxis();
  xa->SetBinLabel(1,"--");
  xa->SetBinLabel(2,"-+");
  xa->SetBinLabel(4,"+-");
  xa->SetBinLabel(5,"++");

  AddTH1(fDileptonCharge_weighted, "hDileptonCharge_weighted", ";DileptonCharge;Number of Events", 5, -2.5, 2.5);
  xa = fDileptonCharge_weighted->GetXaxis();
  xa->SetBinLabel(1,"--");
  xa->SetBinLabel(2,"-+");
  xa->SetBinLabel(4,"+-");
  xa->SetBinLabel(5,"++");

  AddTH1(fDileptonChargeWithoutCorrection_weighted, "hDileptonChargeWithoutCorrection_weighted", ";DileptonChargeWithoutCorrection;Number of Events", 5, -2.5, 2.5);
  xa = fDileptonChargeWithoutCorrection_weighted->GetXaxis();
  xa->SetBinLabel(1,"--");
  xa->SetBinLabel(2,"-+");
  xa->SetBinLabel(4,"+-");
  xa->SetBinLabel(5,"++");

  AddTH1(fNLeptons, "hNLeptons", ";NLeptons;Number of Events", 6, -0.5, 5.5);
  AddTH1(fLepton1Pt, "hLepton1Pt", ";Lepton1 Pt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fLepton1Pt_e, "hLepton1Pt_e", ";Lepton1 (e) Pt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fLepton1Pt_mu, "hLepton1Pt_mu", ";Lepton1 (mu) Pt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fLepton2Pt, "hLepton2Pt", ";Lepton2 Pt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fLepton2Pt_e, "hLepton2Pt_e", ";Lepton2 (e) Pt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fLepton2Pt_mu, "hLepton2Pt_mu", ";Lepton2 (mu) Pt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fLepton3Pt, "hLepton3Pt", ";Lepton3 Pt;Number of Events", 100, 0.0, 50.0);
  AddTH1(fNLeptons_weighted, "hNLeptons_weighted", ";NLeptons;Number of Events", 6, -0.5, 5.5);
  AddTH1(fLepton1Pt_weighted, "hLepton1Pt_weighted", ";Lepton1 Pt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fLepton1Pt_e_weighted, "hLepton1Pt_e_weighted", ";Lepton1 (e) Pt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fLepton1Pt_mu_weighted, "hLepton1Pt_mu_weighted", ";Lepton1 (mu) Pt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fLepton2Pt_weighted, "hLepton2Pt_weighted", ";Lepton2 Pt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fLepton2Pt_e_weighted, "hLepton2Pt_e_weighted", ";Lepton2 (e) Pt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fLepton2Pt_mu_weighted, "hLepton2Pt_mu_weighted", ";Lepton2 (mu) Pt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fLepton3Pt_weighted, "hLepton3Pt_weighted", ";Lepton3 Pt;Number of Events", 100, 0.0, 50.0);

  //Histogram to record the systematic errors
  AddTH1(fDileptonCharge_sysError, "hDileptonCharge_sysError", ";DileptonCharge;Number of Events", 5, -2.5, 2.5);
  xa = fDileptonCharge_sysError->GetXaxis();
  xa->SetBinLabel(1,"--");
  xa->SetBinLabel(2,"-+");
  xa->SetBinLabel(4,"+-");
  xa->SetBinLabel(5,"++");

  AddTH1(fNLeptons_sysError, "hNLeptons_sysError", ";NLeptons;Number of Events", 6, -0.5, 5.5);
  AddTH1(fLepton1Pt_sysError, "hLepton1Pt_sysError", ";Lepton1 Pt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fLepton2Pt_sysError, "hLepton2Pt_sysError", ";Lepton2 Pt;Number of Events", 100, 0.0, 100.0);

  //***********************************************************************************************
  // Trigger Histograms
  //***********************************************************************************************
  AddTH1(fTriggerType, "hTriggerType", 
         ";TriggerType;Number of Events", 10, -0.5, 9.5);
  AddTH1(fElectronTriggerType, "hElectronTriggerType", 
         ";TriggerType;Number of Events", 10, -0.5, 9.5);
  AddTH1(fMuonTriggerType, "hMuonTriggerType", 
         ";TriggerType;Number of Events", 10, -0.5, 9.5);

  AddTH1(fElectronTriggerObjectPt, "hElectronTriggerObjectPt", 
         ";TriggerObjectPt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fElectronTriggerObjectEta, "hElectronTriggerObjectEta", 
         ";TriggerObjectEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronTriggerObjectPt_Real, "hElectronTriggerObjectPt_Real", 
         ";TriggerObjectPt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fElectronTriggerObjectEta_Real, "hElectronTriggerObjectEta_Real", 
         ";TriggerObjectEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronTriggerObjectPt_Fake, "hElectronTriggerObjectPt_Fake", 
         ";TriggerObjectPt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fElectronTriggerObjectEta_Fake, "hElectronTriggerObjectEta_Fake", 
         ";TriggerObjectEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fElectronTriggerObjectPt_Volunteer, "hElectronTriggerObjectPt_Volunteer", 
         ";TriggerObjectPt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fElectronTriggerObjectEta_Volunteer, "hElectronTriggerObjectEta_Volunteer", 
         ";TriggerObjectEta;Number of Events", 100, -5.0, 5.0);

  AddTH1(fMuonTriggerObjectPt, "hMuonTriggerObjectPt", 
         ";TriggerObjectPt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fMuonTriggerObjectEta, "hMuonTriggerObjectEta", 
         ";TriggerObjectEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonTriggerObjectPt_Real, "hMuonTriggerObjectPt_Real", 
         ";TriggerObjectPt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fMuonTriggerObjectEta_Real, "hMuonTriggerObjectEta_Real", 
         ";TriggerObjectEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonTriggerObjectPt_Fake, "hMuonTriggerObjectPt_Fake", 
         ";TriggerObjectPt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fMuonTriggerObjectEta_Fake, "hMuonTriggerObjectEta_Fake", 
         ";TriggerObjectEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMuonTriggerObjectPt_Volunteer, "hMuonTriggerObjectPt_Volunteer", 
         ";TriggerObjectPt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fMuonTriggerObjectEta_Volunteer, "hMuonTriggerObjectEta_Volunteer", 
         ";TriggerObjectEta;Number of Events", 100, -5.0, 5.0);

  AddTH1(fFailedTriggerLeptonPt_RealTriggerOnly, "hFailedTriggerLeptonPt_RealTriggerOnly", 
         ";TriggerObjectPt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fFailedTriggerLeptonEta_RealTriggerOnly, "hFailedTriggerLeptonEta_RealTriggerOnly", 
         ";TriggerObjectEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fFailedTriggerElectronPt_RealTriggerOnly, "hFailedTriggerElectronPt_RealTriggerOnly", 
         ";TriggerObjectPt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fFailedTriggerElectronEta_RealTriggerOnly, "hFailedTriggerElectronEta_RealTriggerOnly", 
         ";TriggerObjectEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fFailedTriggerMuonPt_RealTriggerOnly, "hFailedTriggerMuonPt_RealTriggerOnly", 
         ";TriggerObjectPt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fFailedTriggerMuonEta_RealTriggerOnly, "hFailedTriggerMuonEta_RealTriggerOnly", 
         ";TriggerObjectEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fFailedTriggerLeptonPt_FakeTriggerOnly, "hFailedTriggerLeptonPt_FakeTriggerOnly", 
         ";TriggerObjectPt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fFailedTriggerLeptonEta_FakeTriggerOnly, "hFailedTriggerLeptonEta_FakeTriggerOnly", 
         ";TriggerObjectEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fFailedTriggerElectronPt_FakeTriggerOnly, "hFailedTriggerElectronPt_FakeTriggerOnly", 
         ";TriggerObjectPt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fFailedTriggerElectronEta_FakeTriggerOnly, "hFailedTriggerElectronEta_FakeTriggerOnly", 
         ";TriggerObjectEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fFailedTriggerMuonPt_FakeTriggerOnly, "hFailedTriggerMuonPt_FakeTriggerOnly", 
         ";TriggerObjectPt;Number of Events", 100, 0.0, 100.0);
  AddTH1(fFailedTriggerMuonEta_FakeTriggerOnly, "hFailedTriggerMuonEta_FakeTriggerOnly", 
         ";TriggerObjectEta;Number of Events", 100, -5.0, 5.0);



  //***********************************************************************************************
  // Selection Histograms
  //***********************************************************************************************

  AddTH1(fHWWSelection,"hHWWSelection", ";Cut Number;Number of Events", 11, -1.5, 9.5);
  xa = fHWWSelection->GetXaxis();
  xa->SetBinLabel(1,"TwoLeptons");
  xa->SetBinLabel(2,"PreSel");
  xa->SetBinLabel(3,"CJVeto");
  xa->SetBinLabel(4,"MetCut");
  xa->SetBinLabel(5,"#Delta#phi Cut");
  xa->SetBinLabel(6,"MassCut");
  xa->SetBinLabel(7,"Pt1Cut");
  xa->SetBinLabel(8,"Pt2Cut");
  xa->SetBinLabel(9,"MuonVeto");
  xa->SetBinLabel(10,"NTracksCut");

  AddTH1(fHWWToEESelection ,"hHWWToEESelection", ";Cut Number;Number of Events", 11, -1.5, 9.5);
  xa = fHWWToEESelection->GetXaxis();
  xa->SetBinLabel(1,"TwoLeptons");
  xa->SetBinLabel(2,"PreSel");
  xa->SetBinLabel(3,"CJVeto");
  xa->SetBinLabel(4,"MetCut");
  xa->SetBinLabel(5,"#Delta#phi Cut");
  xa->SetBinLabel(6,"MassCut");
  xa->SetBinLabel(7,"Pt1Cut");
  xa->SetBinLabel(8,"Pt2Cut");
  xa->SetBinLabel(9,"MuonVeto");
  xa->SetBinLabel(10,"NTracksCut");
  AddTH1(fHWWToMuMuSelection ,"hHWWToMuMuSelection", ";Cut Number;Number of Events",11, -1.5, 9.5);
  xa = fHWWToMuMuSelection->GetXaxis();
  xa->SetBinLabel(1,"TwoLeptons");
  xa->SetBinLabel(2,"PreSel");
  xa->SetBinLabel(3,"CJVeto");
  xa->SetBinLabel(4,"MetCut");
  xa->SetBinLabel(5,"#Delta#phi Cut");
  xa->SetBinLabel(6,"MassCut");
  xa->SetBinLabel(7,"Pt1Cut");
  xa->SetBinLabel(8,"Pt2Cut");
  xa->SetBinLabel(9,"MuonVeto");
  xa->SetBinLabel(10,"NTracksCut");
  AddTH1(fHWWToEMuSelection ,"hHWWToEMuSelection", ";Cut Number;Number of Events", 11, -1.5, 9.5);
  xa = fHWWToEMuSelection->GetXaxis();
  xa->SetBinLabel(1,"TwoLeptons");
  xa->SetBinLabel(2,"PreSel");
  xa->SetBinLabel(3,"CJVeto");
  xa->SetBinLabel(4,"MetCut");
  xa->SetBinLabel(5,"#Delta#phi Cut");
  xa->SetBinLabel(6,"MassCut");
  xa->SetBinLabel(7,"Pt1Cut");
  xa->SetBinLabel(8,"Pt2Cut");
  xa->SetBinLabel(9,"MuonVeto");
  xa->SetBinLabel(10,"NTracksCut");

  AddTH1(fHWWSelection_sysError,"hHWWSelection_sysError", ";Cut Number;Number of Events", 11, -1.5, 9.5);
  xa = fHWWSelection_sysError->GetXaxis();
  xa->SetBinLabel(1,"TwoLeptons");
  xa->SetBinLabel(2,"PreSel");
  xa->SetBinLabel(3,"CJVeto");
  xa->SetBinLabel(4,"MetCut");
  xa->SetBinLabel(5,"#Delta#phi Cut");
  xa->SetBinLabel(6,"MassCut");
  xa->SetBinLabel(7,"Pt1Cut");
  xa->SetBinLabel(8,"Pt2Cut");
  xa->SetBinLabel(9,"MuonVeto");
  xa->SetBinLabel(10,"NTracksCut");

  //***********************************************************************************************
  // NoCuts Histograms
  //***********************************************************************************************
  //All events
  AddTH1(fLeptonEta              ,"hLeptonEta",";LeptonEta;Number of Events",100,-5.,5.0);
  AddTH1(fLeptonPtMax            ,"hLeptonPtMax",";Lepton P_t Max;Number of Events",150,0.,150.);
  AddTH1(fLeptonPtMin            ,"hLeptonPtMin",";Lepton P_t Min;Number of Events",150,0.,150.);
  AddTH1(fMetPtHist              ,"hMetPtHist",";Met;Number of Events",150,0.,300.);  
  AddTH1(fMetPhiHist             ,"hMetPhiHist",";#phi;Number of Events",28,-3.5,3.5);
  AddTH1(fUncorrMetPtHist        ,"hUncorrMetPtHist",";Met;Number of Events",150,0.,300.);  
  AddTH1(fUncorrMetPhiHist       ,"hUncorrMetPhiHist",";#phi;Number of Events",28,-3.5,3.5);
  AddTH1(fDeltaPhiLeptons        ,"hDeltaPhiLeptons",";#Delta#phi_{ll};Number of Events",90,0,180);
  AddTH1(fDeltaEtaLeptons        ,"hDeltaEtaLeptons",";#Delta#eta_{ll};Number of Events",100,-5.0,5.0);
  AddTH1(fDileptonMass           ,"hDileptonMass",";Mass_{ll};Number of Events",150,0.,300.);

  AddTH1(fLeptonEta_sysError     ,"hLeptonEta_sysError",";LeptonEta;Number of Events",100,-5.,5.0);
  AddTH1(fLeptonPtMax_sysError     ,"hLeptonPtMax_sysError",";Lepton P_t Max;Number of Events",150,0.,150.);
  AddTH1(fLeptonPtMin_sysError     ,"hLeptonPtMin_sysError",";Lepton P_t Min;Number of Events",150,0.,150.);
  AddTH1(fMetPtHist_sysError       ,"hMetPtHist_sysError",";Met;Number of Events",150,0.,300.);  
  AddTH1(fMetPhiHist_sysError      ,"hMetPhiHist_sysError",";#phi;Number of Events",28,-3.5,3.5);
  AddTH1(fUncorrMetPtHist_sysError ,"hUncorrMetPtHist_sysError",";Met;Number of Events",150,0.,300.);  
  AddTH1(fUncorrMetPhiHist_sysError,"hUncorrMetPhiHist_sysError",";#phi;Number of Events",28,-3.5,3.5);
  AddTH1(fDeltaPhiLeptons_sysError ,"hDeltaPhiLeptons_sysError",";#Delta#phi_{ll};Number of Events",90,0,180);
  AddTH1(fDeltaEtaLeptons_sysError ,"hDeltaEtaLeptons_sysError",";#Delta#eta_{ll};Number of Events",100,-5.0,5.0);
  AddTH1(fDileptonMass_sysError    ,"hDileptonMass_sysError",";Mass_{ll};Number of Events",150,0.,300.);


  //ee events
  AddTH1(fLeptonEta_ee           ,"hLeptonEta_ee",";LeptonEta;Number of Events",100,-5.,5.0);
  AddTH1(fLeptonPtMax_ee         ,"hLeptonPtMax_ee",";Lepton P_t Max;Number of Events",150,0.,150.);
  AddTH1(fLeptonPtMin_ee         ,"hLeptonPtMin_ee",";Lepton P_t Min;Number of Events",150,0.,150.);
  AddTH1(fMetPtHist_ee           ,"hMetPtHist_ee",";Met;Number of Events",150,0.,300.);  
  AddTH1(fMetPhiHist_ee          ,"hMetPhiHist_ee",";#phi;Number of Events",28,-3.5,3.5);
  AddTH1(fUncorrMetPtHist_ee     ,"hUncorrMetPtHist_ee",";Met;Number of Events",150,0.,300.);  
  AddTH1(fUncorrMetPhiHist_ee    ,"hUncorrMetPhiHist_ee",";#phi;Number of Events",28,-3.5,3.5);
  AddTH1(fDeltaPhiLeptons_ee     ,"hDeltaPhiLeptons_ee",";#Delta#phi_{ll};Number of Events",
                                  90,0,180);
  AddTH1(fDeltaEtaLeptons_ee     ,"hDeltaEtaLeptons_ee",";#Delta#eta_{ll};Number of Events",
                                  100,-5.0,5.0);
  AddTH1(fDileptonMass_ee        ,"hDileptonMass_ee",";Mass_{ll};Number of Events",150,0.,300.);
  
  //mumu events
  AddTH1(fLeptonEta_mumu         ,"hLeptonEta_mumu",";LeptonEta;Number of Events",100,-5.,5.0);
  AddTH1(fLeptonPtMax_mumu       ,"hLeptonPtMax_mumu",";Lepton P_t Max;Number of Events",
                                  150,0.,150.); 
  AddTH1(fLeptonPtMin_mumu       ,"hLeptonPtMin_mumu",";Lepton P_t Min;Number of Events",
                                  150,0.,150.);
  AddTH1(fMetPtHist_mumu         ,"hMetPtHist_mumu",";Met;Number of Events",150,0.,300.);  
  AddTH1(fMetPhiHist_mumu        ,"hMetPhiHist_mumu",";#phi;Number of Events",28,-3.5,3.5);
  AddTH1(fUncorrMetPtHist_mumu   ,"hUncorrMetPtHist_mumu",";Met;Number of Events",150,0.,300.);  
  AddTH1(fUncorrMetPhiHist_mumu  ,"hUncorrMetPhiHist_mumu",";#phi;Number of Events",28,-3.5,3.5);
  AddTH1(fDeltaPhiLeptons_mumu   ,"hDeltaPhiLeptons_mumu",";#Delta#phi_{ll};Number of Events",
                                  90,0,180);
  AddTH1(fDeltaEtaLeptons_mumu   ,"hDeltaEtaLeptons_mumu",";#Delta#eta_{ll};Number of Events",
                                  100,-5.0,5.);
  AddTH1(fDileptonMass_mumu      ,"hDileptonMass_mumu",";Mass_{ll};Number of Events",150,0.,300.);
  
  //emu events
  AddTH1(fLeptonEta_emu          ,"hLeptonEta_emu",";LeptonEta;Number of Events",100,-5.,5.0);
  AddTH1(fLeptonPtMax_emu        ,"hLeptonPtMax_emu",";Lepton P_t Max;Number of Events",
                                  150,0.,150.); 
  AddTH1(fLeptonPtMin_emu        ,"hLeptonPtMin_emu",";Lepton P_t Min;Number of Events",
                                  150,0.,150.);
  AddTH1(fMetPtHist_emu          ,"hMetPtHist_emu",";Met;Number of Events",150,0.,300.);  
  AddTH1(fMetPhiHist_emu         ,"hMetPhiHist_emu",";#phi;Number of Events",28,-3.5,3.5);
  AddTH1(fUncorrMetPtHist_emu    ,"hUncorrMetPtHist_emu",";Met;Number of Events",150,0.,300.);  
  AddTH1(fUncorrMetPhiHist_emu   ,"hUncorrMetPhiHist_emu",";#phi;Number of Events",28,-3.5,3.5);
  AddTH1(fDeltaPhiLeptons_emu    ,"hDeltaPhiLeptons_emu",";#Delta#phi_{ll};Number of Events",
                                  90,0,180);
  AddTH1(fDeltaEtaLeptons_emu    ,"hDeltaEtaLeptons_emu",";#Delta#eta_{ll};Number of Events",
                                  100,-5.0,5.0);
  AddTH1(fDileptonMass_emu       ,"hDileptonMass_emu",";Mass_{ll};Number of Events",150,0.,300.);
  
  //***********************************************************************************************
  // N-1 Histograms
  //***********************************************************************************************
  //All events
  AddTH1(fLeptonPtMax_NMinusOne            ,"hLeptonPtMax_NMinusOne",
                                            ";Lepton P_t Max;Number of Events",150,0.,150.);
  AddTH1(fLeptonPtMin_NMinusOne            ,"hLeptonPtMin_NMinusOne",
                                            ";Lepton P_t Min;Number of Events",150,0.,150.);
  AddTH1(fMetPtHist_NMinusOne              ,"hMetPtHist_NMinusOne",
                                            ";Met;Number of Events",150,0.,300.);  
  AddTH1(fMetPhiHist_NMinusOne             ,"hMetPhiHist_NMinusOne",
                                            ";#phi;Number of Events",28,-3.5,3.5);
  AddTH1(fMETdeltaPhilEtHist_NMinusOne     ,"hMETdeltaPhilEtHist_NMinusOne",
                                            ";METdeltaPhilEtHist;Number of Events",150,0.,300.);
  AddTH1(fNCentralJets_NMinusOne           ,"hNCentralJets_NMinusOne",
                                            ";Number of Central Jets;Number of Events",6,-0.5,5.5);
  AddTH1(fNDirtyMuonsHist_NMinusOne        ,"hNDirtyMuonsHist_NMinusOne",
                                            ";Number of Dirty Muons; Number of Events",6,-0.5,5.5);
  AddTH1(fNCleanExtraTracksHist_NMinusOne  ,"hNCleanExtraTracksHist_NMinusOne",
                                            ";Number of Clean Extra Tracks; Number of Events",
                                            15,-0.5,14.5);
  AddTH1(fDeltaPhiLeptons_NMinusOne        ,"hDeltaPhiLeptons_NMinusOne",
                                            ";#Delta#phi_{ll};Number of Events",90,0,180);
  AddTH1(fDeltaEtaLeptons_NMinusOne        ,"hDeltaEtaLeptons_NMinusOne",
                                            ";#Delta#eta_{ll};Number of Events",100,-5.0,5.0);
  AddTH1(fDileptonMass_NMinusOne           ,"hDileptonMass_NMinusOne",
                                            ";Mass_{ll};Number of Events",150,0.,300.);
  AddTH1(fMinDeltaPhiLeptonMet_NMinusOne   ,"hMinDeltaPhiLeptonMet_NMinusOne", 
                                            ";Min #Delta#phi_{l,Met};Number of Events",90,0.,180);

  //ee events
  AddTH1(fLeptonEta_NMinusOne_ee             ,"hLeptonEta_NMinusOne_ee",
                                              ";LeptonEta;Number of Events",100,-5.,5.0);
  AddTH1(fLeptonPtMax_NMinusOne_ee           ,"hLeptonPtMax_NMinusOne_ee",
                                              ";Lepton P_t Max;Number of Events",150,0.,150.);
  AddTH1(fLeptonPtMin_NMinusOne_ee           ,"hLeptonPtMin_NMinusOne_ee",
                                              ";Lepton P_t Min;Number of Events",150,0.,150.);
  AddTH1(fMetPtHist_NMinusOne_ee             ,"hMetPtHist_NMinusOne_ee",";Met;Number of Events",
                                              150,0.,300.);  
  AddTH1(fMetPhiHist_NMinusOne_ee            ,"hMetPhiHist_NMinusOne_ee",";#phi;Number of Events",
                                              28,-3.5,3.5);
  AddTH1(fMETdeltaPhilEtHist_NMinusOne_ee    ,"hMETdeltaPhilEtHist_NMinusOne_ee",
                                              ";METdeltaPhilEtHist;Number of Events",150,0.,300.);  
  AddTH1(fNCentralJets_NMinusOne_ee          ,"hNCentralJets_NMinusOne_ee",
                                              ";Number of Central Jets;Number of Events",
                                              6,-0.5,5.5);
  AddTH1(fNDirtyMuonsHist_NMinusOne_ee       ,"hNDirtyMuonsHist_NMinusOne_ee",
                                              ";Number of Dirty Muons; Number of Events",
                                              6,-0.5,5.5);
  AddTH1(fNCleanExtraTracksHist_NMinusOne_ee ,"hNCleanExtraTracksHist_NMinusOne_ee",
                                              ";Number of Clean Extra Tracks; Number of Events",
                                              15,-0.5,14.5);
  AddTH1(fDeltaPhiLeptons_NMinusOne_ee       ,"hDeltaPhiLeptons_NMinusOne_ee",
                                              ";#Delta#phi_{ll};Number of Events",90,0,180);
  AddTH1(fDeltaEtaLeptons_NMinusOne_ee       ,"hDeltaEtaLeptons_NMinusOne_ee",
                                              ";#Delta#eta_{ll};Number of Events",100,-5.0,5.0);
  AddTH1(fDileptonMass_NMinusOne_ee          ,"hDileptonMass_NMinusOne_ee",
                                              ";Mass_{ll};Number of Events",150,0.,300.);
  AddTH1(fMinDeltaPhiLeptonMet_NMinusOne_ee  ,"hMinDeltaPhiLeptonMet_NMinusOne_ee", 
                                              ";Min #Delta#phi_{l,Met};Number of Events",90,0.,180);

  //mumu events
  AddTH1(fLeptonEta_NMinusOne_mumu             ,"hLeptonEta_NMinusOne_mumu",
                                                ";LeptonEta;Number of Events",100,-5.,5.0);
  AddTH1(fLeptonPtMax_NMinusOne_mumu           ,"hLeptonPtMax_NMinusOne_mumu",
                                                ";Lepton P_t Max;Number of Events",150,0.,150.);
  AddTH1(fLeptonPtMin_NMinusOne_mumu           ,"hLeptonPtMin_NMinusOne_mumu",
                                                ";Lepton P_t Min;Number of Events",150,0.,150.);
  AddTH1(fMetPtHist_NMinusOne_mumu             ,"hMetPtHist_NMinusOne_mumu",
                                                ";Met;Number of Events",150,0.,300.);  
  AddTH1(fMetPhiHist_NMinusOne_mumu            ,"hMetPhiHist_NMinusOne_mumu",
                                                ";#phi;Number of Events",28,-3.5,3.5);
  AddTH1(fMETdeltaPhilEtHist_NMinusOne_mumu    ,"hMETdeltaPhilEtHist_NMinusOne_mumu",
                                                ";METdeltaPhilEtHist;Number of Events",150,0.,300.);
  AddTH1(fNCentralJets_NMinusOne_mumu          ,"hNCentralJets_NMinusOne_mumu",
                                                ";Number of Central Jets;Number of Events",
                                                6,-0.5,5.5);
  AddTH1(fNDirtyMuonsHist_NMinusOne_mumu       ,"hNDirtyMuonsHist_NMinusOne_mumu",
                                                ";Number of Dirty Muons; Number of Events",
                                                6,-0.5,5.5);
  AddTH1(fNCleanExtraTracksHist_NMinusOne_mumu ,"hNCleanExtraTracksHist_NMinusOne_mumu",
                                                ";Number of Clean Extra Tracks;Number of Events",
                                                15,-0.5,14.5);
  AddTH1(fDeltaPhiLeptons_NMinusOne_mumu       ,"hDeltaPhiLeptons_NMinusOne_mumu",
                                                ";#Delta#phi_{ll};Number of Events",
                                                90,0,180);
  AddTH1(fDeltaEtaLeptons_NMinusOne_mumu       ,"hDeltaEtaLeptons_NMinusOne_mumu",
                                                ";#Delta#eta_{ll};Number of Events",
                                                100,-5.0,5.0);
  AddTH1(fDileptonMass_NMinusOne_mumu          ,"hDileptonMass_NMinusOne_mumu",
                                                ";Mass_{ll};Number of Events",
                                                150,0.,300.);
  AddTH1(fMinDeltaPhiLeptonMet_NMinusOne_mumu  ,"hMinDeltaPhiLeptonMet_NMinusOne_mumu", 
                                                ";Min #Delta#phi_{l,Met};Number of Events",
                                                90,0.,180);
  //emu events
  AddTH1(fLeptonEta_NMinusOne_emu             ,"hLeptonEta_NMinusOne_emu",
                                               ";LeptonEta;Number of Events",100,-5.,5.0);
  AddTH1(fLeptonPtMax_NMinusOne_emu           ,"hLeptonPtMax_NMinusOne_emu",
                                               ";Lepton P_t Max;Number of Events",150,0.,150.);
  AddTH1(fLeptonPtMin_NMinusOne_emu           ,"hLeptonPtMin_NMinusOne_emu",
                                               ";Lepton P_t Min;Number of Events",150,0.,150.);
  AddTH1(fMetPtHist_NMinusOne_emu             ,"hMetPtHist_NMinusOne_emu",
                                               ";Met;Number of Events",150,0.,300.);  
  AddTH1(fMetPhiHist_NMinusOne_emu            ,"hMetPhiHist_NMinusOne_emu",
                                               ";#phi;Number of Events",28,-3.5,3.5);
  AddTH1(fMETdeltaPhilEtHist_NMinusOne_emu    ,"hMETdeltaPhilEtHist_NMinusOne_emu",
                                               ";METdeltaPhilEtHist;Number of Events",150,0.,300.);
  AddTH1(fNCentralJets_NMinusOne_emu          ,"hNCentralJets_NMinusOne_emu",
                                               ";Number of Central Jets;Number of Events",
                                                6,-0.5,5.5);
  AddTH1(fNDirtyMuonsHist_NMinusOne_emu       ,"hNDirtyMuonsHist_NMinusOne_emu",
                                               ";Number of Dirty Muons; Number of Events",
                                                6,-0.5,5.5);
  AddTH1(fNCleanExtraTracksHist_NMinusOne_emu ,"hNCleanExtraTracksHist_NMinusOne_emu",
                                               ";Number of Clean Extra Tracks; Number of Events",
                                                15,-0.5,14.5);
  AddTH1(fDeltaPhiLeptons_NMinusOne_emu       ,"hDeltaPhiLeptons_NMinusOne_emu",
                                               ";#Delta#phi_{ll};Number of Events",90,0,180);
  AddTH1(fDeltaEtaLeptons_NMinusOne_emu       ,"hDeltaEtaLeptons_NMinusOne_emu",
                                               ";#Delta#eta_{ll};Number of Events",100,-5.0,5.0);
  AddTH1(fDileptonMass_NMinusOne_emu          ,"hDileptonMass_NMinusOne_emu",
                                               ";Mass_{ll};Number of Events",150,0.,300.);
  AddTH1(fMinDeltaPhiLeptonMet_NMinusOne_emu  ,"hMinDeltaPhiLeptonMet_NMinusOne_emu", 
                                               ";Min #Delta#phi_{l,Met};Number of Events",
                                                90,0.,180);

  //***********************************************************************************************
  // After all cuts Histograms
  //***********************************************************************************************

  AddTH1(fMinDeltaPhiLeptonMet_afterCuts    ,"hMinDeltaPhiLeptonMet_afterCuts", 
                                             ";Min #Delta#phi_{l,Met};Number of Events",90,0.,180);
  AddTH1(fMtLepton1_afterCuts               ,"hMtLepton1_afterCuts",
                                             ";M_t (Lepton1,Met);Number of Events",100,0.,200.);
  AddTH1(fMtLepton2_afterCuts               ,"hMtLepton2_afterCuts",
                                             ";M_t (Lepton2,Met);Number of Events",100,0.,200.);
  AddTH1(fMtHiggs_afterCuts                 ,"hMtHiggs_afterCuts",
                                             ";M_t (l1+l2+Met);Number of Events",150,0.,300.);
  AddTH1(fLeptonPtPlusMet_afterCuts         ,"hLeptonPtPlusMet_afterCuts",
                                             ";LeptonPtPlusMet;Number of Events",150,0., 300.);
  AddTH1(fMinDeltaPhiLeptonMet_afterCuts_ee ,"hMinDeltaPhiLeptonMet_afterCuts_ee", 
                                             ";Min #Delta#phi_{l,Met};Number of Events",90,0.,180);
  AddTH1(fMtLepton1_afterCuts_ee            ,"hMtLepton1_afterCuts_ee",
                                             ";M_t (Lepton1,Met);Number of Events",100,0.,200.);
  AddTH1(fMtLepton2_afterCuts_ee            ,"hMtLepton2_afterCuts_ee",
                                             ";M_t (Lepton2,Met);Number of Events",100,0.,200.);
  AddTH1(fMtHiggs_afterCuts_ee              ,"hMtHiggs_afterCuts_ee",
                                             ";M_t (l1+l2+Met);Number of Events",150,0.,300.);
  AddTH1(fLeptonPtPlusMet_afterCuts_ee      ,"hLeptonPtPlusMet_afterCuts_ee",
                                             ";LeptonPtPlusMet;Number of Events",150,0., 300.);
  AddTH1(fMinDeltaPhiLeptonMet_afterCuts_mumu,"hMinDeltaPhiLeptonMet_afterCuts_mumu", 
                                             ";Min #Delta#phi_{l,Met};Number of Events",90,0.,180);
  AddTH1(fMtLepton1_afterCuts_mumu          ,"hMtLepton1_afterCuts_mumu",
                                             ";M_t (Lepton1,Met);Number of Events",100,0.,200.);
  AddTH1(fMtLepton2_afterCuts_mumu          ,"hMtLepton2_afterCuts_mumu",
                                             ";M_t (Lepton2,Met);Number of Events",100,0.,200.);
  AddTH1(fMtHiggs_afterCuts_mumu            ,"hMtHiggs_afterCuts_mumu",
                                             ";M_t (l1+l2+Met);Number of Events",150,0.,300.);
  AddTH1(fLeptonPtPlusMet_afterCuts_mumu    ,"hLeptonPtPlusMet_afterCuts_mumu",
                                             ";LeptonPtPlusMet;Number of Events",150,0., 300.);
  AddTH1(fMinDeltaPhiLeptonMet_afterCuts_emu,"hMinDeltaPhiLeptonMet_afterCuts_emu", 
                                             ";Min #Delta#phi_{l,Met};Number of Events",90,0.,180);
  AddTH1(fMtLepton1_afterCuts_emu           ,"hMtLepton1_afterCuts_emu",
                                             ";M_t (Lepton1,Met);Number of Events",100,0.,200.);
  AddTH1(fMtLepton2_afterCuts_emu           ,"hMtLepton2_afterCuts_emu",
                                             ";M_t (Lepton2,Met);Number of Events",100,0.,200.);
  AddTH1(fMtHiggs_afterCuts_emu             ,"hMtHiggs_afterCuts_emu",
                                             ";M_t (l1+l2+Met);Number of Events",150,0.,300.);
  AddTH1(fLeptonPtPlusMet_afterCuts_emu     ,"hLeptonPtPlusMet_afterCuts_emu",
                                             ";LeptonPtPlusMet;Number of Events",150,0., 300.);

}

//--------------------------------------------------------------------------------------------------
void WJetsFakeValidationMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void WJetsFakeValidationMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
