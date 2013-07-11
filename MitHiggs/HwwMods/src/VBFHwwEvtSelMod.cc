 // $Id: VBFHwwEvtSelMod.cc,v 1.2 2008/12/04 19:19:50 sixie Exp $

#include "MitHiggs/HwwMods/interface/VBFHwwEvtSelMod.h"
#include <TH1D.h>
#include <TH2D.h>
#include "MitAna/DataTree/interface/Names.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitCommon/MathTools/interface/MathUtils.h"

using namespace mithep;
ClassImp(mithep::VBFHwwEvtSelMod)

//--------------------------------------------------------------------------------------------------
VBFHwwEvtSelMod::VBFHwwEvtSelMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fPrintDebug(false),
  fPlotType("N-1"),
  fMCPartName(Names::gkMCPartBrn),
  fMetName(Names::gkCaloMetBrn),
  fTrackName(Names::gkTrackBrn),
  fJetName(Names::gkCaloJetBrn),
  fCleanFwdJetsName(ModNames::gkCleanFwdJetsName),
  fCleanNoFwdJetsName(ModNames::gkCleanNoFwdJetsName),
  fMuonName(Names::gkMuonBrn),
  fMCLeptonsName(ModNames::gkMCLeptonsName),
  fMCqqHsName(ModNames::gkMCqqHsName),
  fLoadGenParticles(false),
  fParticles(0),
  fMet(0),
  fTracks(0),
  fJets(0),
  fNEventsProcessed(0),
  fNEventsPassedSkim(0),
  fNEventsPassedLeptonSelection(0),
  fNEventsPassedKinematicPreselection(0),
  fNEventsInsideAcceptance(0)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void VBFHwwEvtSelMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void VBFHwwEvtSelMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and  
  fNEventsProcessed++;

  if (fNEventsProcessed % 1000 == 0 || fPrintDebug) {
    time_t systime;
    systime = time(NULL);
    cerr << endl << "VBFHwwEvtSelMod : Process Event " << fNEventsProcessed << "  Time: " << ctime(&systime) << endl;
  }

  //Get Generator Level information for matching
  ObjArray<MCParticle> *GenLeptons =  dynamic_cast<ObjArray<MCParticle>* > (FindObjThisEvt(fMCLeptonsName.Data()));
  ObjArray<MCParticle> *GenVBFQuarks =  dynamic_cast<ObjArray<MCParticle>* > (FindObjThisEvt(fMCqqHsName.Data()));
  
  if (fPrintDebug) {
    cerr << "check generator leptons\n";
    for (UInt_t i=0; i<GenLeptons->GetEntries(); ++i) {    
      cerr << i << " " << GenLeptons->At(i)->PdgId() << " " << GenLeptons->At(i)->Status() 
           << " " << GenLeptons->At(i)->Pt() << " " << GenLeptons->At(i)->Eta() << " " 
           << GenLeptons->At(i)->Phi() << endl;
    }
  } 

  //***********************************************************************************************
  //Make Generator Level Information
  //***********************************************************************************************
  if (GenLeptons) {
    double leptonPtMax = 0;
    double leptonPtMin = 0;
    for (UInt_t i=0; i<GenLeptons->GetEntries();i++) {
      fGenLeptonEta->Fill(GenLeptons->At(i)->Eta());
      if (GenLeptons->At(i)->Pt() > leptonPtMax) {        
        leptonPtMin = leptonPtMax;
        leptonPtMax = GenLeptons->At(i)->Pt();
      }
      if (GenLeptons->At(i)->Pt() > leptonPtMin) {
        leptonPtMin = GenLeptons->At(i)->Pt();
      }
    }
    if (leptonPtMax > 0)
      fGenLeptonPtMax->Fill(leptonPtMax);
    if (leptonPtMin > 0)
      fGenLeptonPtMin->Fill(leptonPtMin);   
  }

  if (GenVBFQuarks) {
    for (UInt_t i=0; i<GenVBFQuarks->GetEntries();i++) {
      fFwdQuarkPt->Fill(GenVBFQuarks->At(i)->Pt());
      fFwdQuarkEta->Fill(GenVBFQuarks->At(i)->Eta());
    }
    if (GenVBFQuarks->GetEntries() == 2) {
      fFwdQuarkDeltaEta->Fill(fabs(GenVBFQuarks->At(0)->Eta() - GenVBFQuarks->At(1)->Eta()));    
      fMCEventClassification->Fill(2); //VBF higgs
    } else {
      fMCEventClassification->Fill(0);
    }
  } else {
    fMCEventClassification->Fill(0); //non VBF higgs
  }
  

  //***********************************************************************************************
  //Obtain all the good objects from the event cleaning module
  //***********************************************************************************************
  ObjArray<Electron> *CleanElectrons = dynamic_cast<ObjArray<Electron>* >
    (FindObjThisEvt(ModNames::gkCleanElectronsName));
  ObjArray<Muon> *CleanMuons = dynamic_cast<ObjArray<Muon>* >
    (FindObjThisEvt(ModNames::gkCleanMuonsName));
  ObjArray<Jet> *CleanCentralJets = dynamic_cast<ObjArray<Jet>* >
    (FindObjThisEvt(fCleanNoFwdJetsName.Data()));
  ObjArray<Jet> *CleanFwdJets = dynamic_cast<ObjArray<Jet>* >
    (FindObjThisEvt(fCleanFwdJetsName.Data()));

  LoadBranch(fMetName);
  const Met *caloMet = fMet->At(0);

  //***********************************************************************************************
  //Make Clean Lepton vector
  //***********************************************************************************************
  vector<ChargedParticle*> leptons;
  vector<string> leptonType; 
  
  //make lepton vector from muons and electrons
  for (UInt_t j=0; j<CleanMuons->GetEntries(); j++) {
    leptons.push_back(CleanMuons->At(j));
    leptonType.push_back("mu");
  }
  for (UInt_t j=0; j<CleanElectrons->GetEntries(); j++) {   
    leptons.push_back(CleanElectrons->At(j));
    leptonType.push_back("e");
  }

  //We should sort the Leptons by Pt   
  for(UInt_t i=0; i<leptons.size(); i++)
    for(UInt_t j=i+1; j<leptons.size(); j++)
      if(leptons[i]->Pt() < leptons[j]->Pt()) {
	//swap i and j
	ChargedParticle* templepton = leptons[i];
	leptons[i] = leptons[j];
	leptons[j] = templepton;
	string templeptonType = leptonType[i];
	leptonType[i] = leptonType[j];
	leptonType[j] = templeptonType;	 
      }
  
  if (fPrintDebug) {
    cerr << "Check Lepton Sort\n";
    for(UInt_t i=0; i<leptons.size(); i++)
      cerr << leptons[i]->Pt() << endl;
  }
  
  if (fPrintDebug)
    cerr << "Analysis begins" << endl;
  
  //***********************************************************************************************
  //Kinematic PreSelection
  //***********************************************************************************************
  //make sure 2nd highest pt lepton has Pt > 0
  if (leptons.size() < 2 || leptons[1]->Pt() <= 0) return;
  
  //make sure the 3rd highest pt lepton has pt <= 10.
  if (leptons.size() >= 3 && leptons[2]->Pt() > 10) return;
  
  //needs to pass HLT  
  fNEventsPassedLeptonSelection++;
    
  CompositeParticle *dilepton = new CompositeParticle();
  dilepton->AddDaughter(leptons[0]);
  dilepton->AddDaughter(leptons[1]);
  
  //charge of the leptons should be opposite
  if (dilepton->Charge() != 0) return;
  
  fNEventsPassedKinematicPreselection++;

  if (fPrintDebug)
    cerr << "Event Passes PreSelection" << endl;  

  //***********************************************************************************************
  //Get Dirty Muons
  //***********************************************************************************************
  LoadBranch(fMuonName);
  ObjArray<Muon> *DirtyMuons = new ObjArray<Muon>;
  for (UInt_t i=0; i<fMuons->GetEntries(); ++i) {
    const Muon *mu = fMuons->At(i);
    if(!mu->GlobalTrk()) continue;
    if(mu->Pt() < 5.0)   continue;

    bool isCleanMuon = false;
    for (UInt_t j=0; j<CleanMuons->GetEntries(); j++) {
      if(fMuons->At(i) == CleanMuons->At(j)) isCleanMuon = true;
    }
    if(isCleanMuon == false) DirtyMuons->Add(mu);
  }

  //***********************************************************************************************
  //Get Clean Tracks excluding the good leptons
  //***********************************************************************************************
  LoadBranch(fTrackName);
  ObjArray<Track> *CleanExtraTracks = new ObjArray<Track>;
  int nTracks = 0;
  double z0Average = (leptons[0]->Trk()->Z0() + leptons[1]->Trk()->Z0())/2 ;

  for (UInt_t i=0; i<fTracks->GetEntries(); ++i) {
    bool isLepton = false;

    if (MathUtils::DeltaR(fTracks->At(i)->Eta(),fTracks->At(i)->Phi(),leptons[0]->Eta(),
                          leptons[0]->Phi()) > 0.01 &&
        MathUtils::DeltaR(fTracks->At(i)->Eta(),fTracks->At(i)->Phi(),leptons[1]->Eta(),
                          leptons[1]->Phi()) > 0.01
      ) {
    } else {
      isLepton = true;
    }
    
    if (fPrintDebug) {
      cerr << "Track " << i << " : "  << fTracks->At(i)->Pt() << " " << fTracks->At(i)->Eta() 
           << " " << fTracks->At(i)->Phi() << " islepton=" << isLepton << endl;              
    } 

    if ( !isLepton && fTracks->At(i)->Pt() > 3.0 
         && fTracks->At(i)->NHits() >= 8 
         && fabs(z0Average - fTracks->At(i)->Z0()) < 0.5 ) {
      CleanExtraTracks->Add(fTracks->At(i));
      nTracks++;
    } 
  }
  
  //***********************************************************************************************
  //Define Event Variabls
  //***********************************************************************************************
  //delta phi between the 2 leptons in degrees
  double deltaPhiLeptons = MathUtils::DeltaPhi(leptons[0]->Phi(), 
                                               leptons[1]->Phi())* 360.0 / 2 / TMath::Pi();

  double deltaEtaLeptons = abs(leptons[0]->Eta() - leptons[1]->Eta()) * 360.0 / 2 / TMath::Pi();

  double deltaPhiDileptonMet = MathUtils::DeltaPhi(caloMet->Phi(), 
                                                   dilepton->Phi())*360.0 / 2 / TMath::Pi();

  double mtHiggs = TMath::Sqrt(2.0*dilepton->Pt() * caloMet->Pt()*
			       (1.0 - cos(deltaPhiDileptonMet * 2 * TMath::Pi() / 360.0)));

  //angle between MET and closest lepton
  double deltaPhiMetLepton[2] = {MathUtils::DeltaPhi(caloMet->Phi(), leptons[0]->Phi()),
                                 MathUtils::DeltaPhi(caloMet->Phi(), leptons[1]->Phi())};
  
  double mTW[2] = {TMath::Sqrt(2.0*leptons[0]->Pt()*caloMet->Pt()*
                               (1.0 - cos(deltaPhiMetLepton[0]))),
		   TMath::Sqrt(2.0*leptons[1]->Pt()*caloMet->Pt()*
                               (1.0 - cos(deltaPhiMetLepton[1])))};

  double minDeltaPhiMetLepton = (deltaPhiMetLepton[0] < deltaPhiMetLepton[1])?
    deltaPhiMetLepton[0]:deltaPhiMetLepton[1];
  minDeltaPhiMetLepton = minDeltaPhiMetLepton * 360.0 / 2 / TMath::Pi();

  //count the number of central Jets for vetoing
  int nCentralJets = CleanCentralJets->GetEntries();
  int nVBFJets = CleanFwdJets->GetEntries();

  //***********************************************************************************************
  //Make preslection histograms
  //***********************************************************************************************
  fNCentralJets_presel->Fill(nCentralJets);
  fNVBFJets_presel->Fill(nVBFJets);

  for (UInt_t j=0; j<CleanCentralJets->GetEntries(); j++) {    
    fCentralJetEta_presel->Fill(CleanCentralJets->At(j)->Eta());
    fCentralJetPt_presel->Fill(CleanCentralJets->At(j)->Pt());
  }

  for (UInt_t j=0; j<CleanFwdJets->GetEntries(); j++) {    
    fVBFJetEta_presel->Fill(CleanFwdJets->At(j)->Eta());
    fVBFJetPt_presel->Fill(CleanFwdJets->At(j)->Pt());
  }
  

  //***********************************************************************************************
  //Define Cuts
  //***********************************************************************************************

  const int nCuts = 10;
  bool passCut[nCuts] = {false, false, false, false, false,
                         false, false, false, false, false};
 
  if(leptons[0]->Pt() > 20.0 &&
     leptons[1]->Pt() > 10.0 &&
     caloMet->Pt()    > 30.0 &&
     dilepton->Mass() > 12.0
    )                              passCut[0] = true;
  //above cuts are for preselction to be fed into TMVA

  if(nCentralJets < 1)     passCut[1] = true;
  if(nVBFJets == 2)        passCut[2] = true;

  if (leptonType[0] == "mu" && leptonType[1] == "mu" ){ // mumu
    if(caloMet->Pt()	> 50.0 &&
       caloMet->Pt()	< 200.0)         passCut[3] = true;
    if(deltaPhiLeptons	< 45.0)          passCut[4] = true;
    if(dilepton->Mass()	< 50.0)          passCut[5] = true;
    if(leptons[0]->Pt()	> 35.0   &&
       leptons[0]->Pt()	< 55.0)          passCut[6] = true;
    if(leptons[1]->Pt()	> 25.0)          passCut[7] = true;
  }
  else if(leptonType[0] == "e" && leptonType[1] == "e"){ // ee
    if(caloMet->Pt()	  > 51.0   &&
       caloMet->Pt()	  < 200.0)       passCut[3] = true;
    if(deltaPhiLeptons	  < 45.0)        passCut[4] = true;
    if(dilepton->Mass()	  < 40.0)        passCut[5] = true;
      if(leptons[0]->Pt() > 25.0   &&
	 leptons[0]->Pt() < 49.0)        passCut[6] = true;
      if(leptons[1]->Pt() > 25.0)        passCut[7] = true;
  }      
  else if((leptonType[0] == "e" && leptonType[1] == "mu") || 
	  (leptonType[0] == "mu" && leptonType[1] == "e")
    ){ //emu
    if(caloMet->Pt()	> 45.0 &&
       caloMet->Pt()	< 105.0)         passCut[3] = true;
    if(deltaPhiLeptons	< 70.0)          passCut[4] = true;
    if(dilepton->Mass()	< 45.0)          passCut[5] = true;
    if(leptons[0]->Pt()	> 25.0   &&
       leptons[0]->Pt()	< 50.0)          passCut[6] = true;
    if(leptons[1]->Pt()	> 25.0)          passCut[7] = true;
  }

  if (DirtyMuons->GetEntries() < 1)      passCut[8] = true;
  if (CleanExtraTracks->GetEntries() < 4)     passCut[9] = true;


  // Final decision
  bool allCuts = true;
  for(int i=0; i<nCuts; i++) allCuts = allCuts & passCut[i];


  //***********************************************************************************************
  //Make Selection Histograms
  //***********************************************************************************************
  
  fHWWSelection->Fill(-1);
  if (leptonType[0] == "mu" && leptonType[1] == "mu" )
    fHWWToMuMuSelection->Fill(-1);
  else if(leptonType[0] == "e" && leptonType[1] == "e")
    fHWWToEESelection->Fill(-1);
  else if((leptonType[0] == "e" && leptonType[1] == "mu") || 
	  (leptonType[0] == "mu" && leptonType[1] == "e"))
    fHWWToEMuSelection->Fill(-1);
  
  for (int k=0;k<nCuts;k++) {
    bool pass = true;
    bool passPreviousCut = true;
    for (int p=0;p<=k;p++) {
      pass = (pass && passCut[p]);
      if (p<k)
        passPreviousCut = (passPreviousCut&& passCut[p]);
    }
    
    if (pass) {
      fHWWSelection->Fill(k);
      if (leptonType[0] == "mu" && leptonType[1] == "mu" )
        fHWWToMuMuSelection->Fill(k);
      else if(leptonType[0] == "e" && leptonType[1] == "e")
        fHWWToEESelection->Fill(k);
      else if((leptonType[0] == "e" && leptonType[1] == "mu") || 
              (leptonType[0] == "mu" && leptonType[1] == "e"))
        fHWWToEMuSelection->Fill(k);
    }

  //***********************************************************************************************
  //Plots with Sequential Cuts
  //***********************************************************************************************

    if (k==0 && passPreviousCut) {
      fLeptonEta->Fill(leptons[0]->Eta());
      fLeptonEta->Fill(leptons[1]->Eta());
      if (leptonType[0] == "e" && leptonType[1] == "e") {
        fLeptonEta_ee->Fill(leptons[0]->Eta());
        fLeptonEta_ee->Fill(leptons[1]->Eta());
      } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
        fLeptonEta_mumu->Fill(leptons[0]->Eta());
        fLeptonEta_mumu->Fill(leptons[1]->Eta());
      } else {
        fLeptonEta_emu->Fill(leptons[0]->Eta());
        fLeptonEta_emu->Fill(leptons[1]->Eta());
      }
    }        
    if (k==1 && passPreviousCut) {
      fNVBFJets->Fill(nCentralJets);
      if (leptonType[0] == "e" && leptonType[1] == "e") {
        fNVBFJets_ee->Fill(nCentralJets);
      } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
        fNVBFJets_mumu->Fill(nCentralJets);
      } else {
        fNVBFJets_emu->Fill(nCentralJets);
      }
    }
    if (k==2 && passPreviousCut) {
      fNCentralJets->Fill(nVBFJets);
      if (leptonType[0] == "e" && leptonType[1] == "e") {
        fNCentralJets_ee->Fill(nVBFJets);
      } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
        fNCentralJets_mumu->Fill(nVBFJets);
      } else {
        fNCentralJets_emu->Fill(nVBFJets);
      }
    }
    if (k==3 && passPreviousCut) {
      fMetPtHist->Fill(caloMet->Pt());
      if (leptonType[0] == "e" && leptonType[1] == "e") {
        fMetPtHist_ee->Fill(caloMet->Pt());
      } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
        fMetPtHist_mumu->Fill(caloMet->Pt());
      } else {
        fMetPtHist_emu->Fill(caloMet->Pt());
      }
    }
    if (k==4 && passPreviousCut) {    
      fDeltaPhiLeptons->Fill(deltaPhiLeptons);  
      if (leptonType[0] == "e" && leptonType[1] == "e") {
        fDeltaPhiLeptons_ee->Fill(deltaPhiLeptons);  
      } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
        fDeltaPhiLeptons_mumu->Fill(deltaPhiLeptons);  
      } else {
        fDeltaPhiLeptons_emu->Fill(deltaPhiLeptons);  
      }
    }
    if (k==5 && passPreviousCut) {
      fDileptonMass->Fill(dilepton->Mass());
      if (leptonType[0] == "e" && leptonType[1] == "e") {
        fDileptonMass_ee->Fill(dilepton->Mass());
      } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
        fDileptonMass_mumu->Fill(dilepton->Mass());
      } else {
        fDileptonMass_emu->Fill(dilepton->Mass());
      }
    }
    if (k==6 && passPreviousCut) {
      fLeptonPtMax->Fill(leptons[0]->Pt());
      if (leptonType[0] == "e" && leptonType[1] == "e") {
        fLeptonPtMax_ee->Fill(leptons[0]->Pt());
      } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
        fLeptonPtMax_mumu->Fill(leptons[0]->Pt());
      } else {
        fLeptonPtMax_emu->Fill(leptons[0]->Pt());
      }
    }        
    if (k==7 && passPreviousCut) {
      fLeptonPtMin->Fill(leptons[1]->Pt());
      if (leptonType[0] == "e" && leptonType[1] == "e") {
        fLeptonPtMin_ee->Fill(leptons[1]->Pt());
      } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
        fLeptonPtMin_mumu->Fill(leptons[1]->Pt());
      } else {
        fLeptonPtMin_emu->Fill(leptons[1]->Pt());
      }
    }        
    if (k==8 && passPreviousCut) {
      fNDirtyMuonsHist->Fill(DirtyMuons->GetEntries());
      if (leptonType[0] == "e" && leptonType[1] == "e") {
        fNDirtyMuonsHist_ee->Fill(DirtyMuons->GetEntries());
      } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
        fNDirtyMuonsHist_mumu->Fill(DirtyMuons->GetEntries());
      } else {
        fNDirtyMuonsHist_emu->Fill(DirtyMuons->GetEntries());
      }
    }
    if (k==9 && passPreviousCut) {
      fNCleanExtraTracksHist->Fill(CleanExtraTracks->GetEntries());
      if (leptonType[0] == "e" && leptonType[1] == "e") {
        fNCleanExtraTracksHist_ee->Fill(CleanExtraTracks->GetEntries());
      } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
        fNCleanExtraTracksHist_mumu->Fill(CleanExtraTracks->GetEntries());
      } else {
        fNCleanExtraTracksHist_emu->Fill(CleanExtraTracks->GetEntries());
      }
    }
  
    
  //***********************************************************************************************
  //Plots after all Cuts
  //***********************************************************************************************
    if (k==9 && pass) {
      fLeptonEta_afterCuts->Fill(leptons[0]->Pt()); 
      fLeptonEta_afterCuts->Fill(leptons[1]->Pt());
      fLeptonPtMax_afterCuts->Fill(leptons[0]->Pt());
      fLeptonPtMin_afterCuts->Fill(leptons[1]->Pt());
      fMetPtHist_afterCuts->Fill(caloMet->Pt());                             
      fMetPhiHist_afterCuts->Fill(caloMet->Phi());                            
      fDeltaPhiLeptons_afterCuts->Fill(deltaPhiLeptons);
      fDeltaEtaLeptons_afterCuts->Fill(deltaEtaLeptons);
      fDileptonMass_afterCuts->Fill(dilepton->Mass());
      fMinDeltaPhiLeptonMet_afterCuts->Fill(minDeltaPhiMetLepton);       
      fMetSignificance_afterCuts->Fill(caloMet->MetSig());                               
      fSumEt_afterCuts->Fill(caloMet->SumEt());
      fMtLepton1_afterCuts->Fill(mTW[0]);
      fMtLepton2_afterCuts->Fill(mTW[1]);
      fMtHiggs_afterCuts->Fill(mtHiggs);
      fLeptonPtPlusMet_afterCuts->Fill(leptons[0]->Pt()+leptons[1]->Pt()+caloMet->Pt());
      if (leptonType[0] == "e" && leptonType[1] == "e") {
        fLeptonEta_afterCuts_ee->Fill(leptons[0]->Pt()); 
        fLeptonEta_afterCuts_ee->Fill(leptons[1]->Pt());
        fLeptonPtMax_afterCuts_ee->Fill(leptons[0]->Pt());
        fLeptonPtMin_afterCuts_ee->Fill(leptons[1]->Pt());
        fMetPtHist_afterCuts_ee->Fill(caloMet->Pt());                             
        fMetPhiHist_afterCuts_ee->Fill(caloMet->Phi());                            
        fDeltaPhiLeptons_afterCuts_ee->Fill(deltaPhiLeptons);
        fDeltaEtaLeptons_afterCuts_ee->Fill(deltaEtaLeptons);
        fDileptonMass_afterCuts_ee->Fill(dilepton->Mass());
        fMinDeltaPhiLeptonMet_afterCuts_ee->Fill(minDeltaPhiMetLepton); 
        fMetSignificance_afterCuts_ee->Fill(caloMet->MetSig());                               
        fSumEt_afterCuts_ee->Fill(caloMet->SumEt());
        fMtLepton1_afterCuts_ee->Fill(mTW[0]);
        fMtLepton2_afterCuts_ee->Fill(mTW[1]);
        fMtHiggs_afterCuts_ee->Fill(mtHiggs);
        fLeptonPtPlusMet_afterCuts_ee->Fill(leptons[0]->Pt()+leptons[1]->Pt()+caloMet->Pt());
      } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
        fLeptonEta_afterCuts_mumu->Fill(leptons[0]->Pt()); 
        fLeptonEta_afterCuts_mumu->Fill(leptons[1]->Pt());
        fLeptonPtMax_afterCuts_mumu->Fill(leptons[0]->Pt());
        fLeptonPtMin_afterCuts_mumu->Fill(leptons[1]->Pt());
        fMetPtHist_afterCuts_mumu->Fill(caloMet->Pt());                             
        fMetPhiHist_afterCuts_mumu->Fill(caloMet->Phi());                            
        fDeltaPhiLeptons_afterCuts_mumu->Fill(deltaPhiLeptons);
        fDeltaEtaLeptons_afterCuts_mumu->Fill(deltaEtaLeptons);
        fDileptonMass_afterCuts_mumu->Fill(dilepton->Mass());
        fMinDeltaPhiLeptonMet_afterCuts_mumu->Fill(minDeltaPhiMetLepton);         
        fMetSignificance_afterCuts_mumu->Fill(caloMet->MetSig());                               
        fSumEt_afterCuts_mumu->Fill(caloMet->SumEt());
        fMtLepton1_afterCuts_mumu->Fill(mTW[0]);
        fMtLepton2_afterCuts_mumu->Fill(mTW[1]);
        fMtHiggs_afterCuts_mumu->Fill(mtHiggs);
        fLeptonPtPlusMet_afterCuts_mumu->Fill(leptons[0]->Pt()+leptons[1]->Pt()+caloMet->Pt());
      } else {
        fLeptonEta_afterCuts_emu->Fill(leptons[0]->Pt()); 
        fLeptonEta_afterCuts_emu->Fill(leptons[1]->Pt());
        fLeptonPtMax_afterCuts_emu->Fill(leptons[0]->Pt());
        fLeptonPtMin_afterCuts_emu->Fill(leptons[1]->Pt());
        fMetPtHist_afterCuts_emu->Fill(caloMet->Pt());                             
        fMetPhiHist_afterCuts_emu->Fill(caloMet->Phi());                            
        fDeltaPhiLeptons_afterCuts_emu->Fill(deltaPhiLeptons);
        fDeltaEtaLeptons_afterCuts_emu->Fill(deltaEtaLeptons);
        fDileptonMass_afterCuts_emu->Fill(dilepton->Mass());
        fMinDeltaPhiLeptonMet_afterCuts_emu->Fill(minDeltaPhiMetLepton);    
        fMetSignificance_afterCuts_emu->Fill(caloMet->MetSig());                               
        fSumEt_afterCuts_emu->Fill(caloMet->SumEt());
        fMtLepton1_afterCuts_emu->Fill(mTW[0]);
        fMtLepton2_afterCuts_emu->Fill(mTW[1]);
        fMtHiggs_afterCuts_emu->Fill(mtHiggs);
        fLeptonPtPlusMet_afterCuts_emu->Fill(leptons[0]->Pt()+leptons[1]->Pt()+caloMet->Pt());
      }
    }
  }
    
  //***********************************************************************************************
  // N-1 Cuts
  //***********************************************************************************************

  //N Jet Veto  
  bool pass = true;
  for (int k=0;k<nCuts;k++) {
    if (k != 1) {
      pass = (pass && passCut[k]);      
    }
  }
  if (pass) {
    fNCentralJets_NMinusOne->Fill(nCentralJets);
    if (leptonType[0] == "e" && leptonType[1] == "e") {
      fNCentralJets_NMinusOne_ee->Fill(nCentralJets);
    } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
      fNCentralJets_NMinusOne_mumu->Fill(nCentralJets);
    } else {
      fNCentralJets_NMinusOne_emu->Fill(nCentralJets);
    }
  }     

  //Met Cut
  pass = true;
  for (int k=0;k<nCuts;k++) {
    if (k != 3) {
      pass = (pass && passCut[k]);      
    }
  }
  if (pass) {
    fMetPtHist_NMinusOne->Fill(caloMet->Pt());  
    if (leptonType[0] == "e" && leptonType[1] == "e") {
      fMetPtHist_NMinusOne_ee->Fill(caloMet->Pt());  
    } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
      fMetPtHist_NMinusOne_mumu->Fill(caloMet->Pt());  
    } else {
      fMetPtHist_NMinusOne_emu->Fill(caloMet->Pt());  
    }
  }
  
  //DeltaPhiLeptons
  pass = true;
  for (int k=0;k<nCuts;k++) {
    if (k != 4) {
      pass = (pass && passCut[k]);      
    }
  }
  if (pass) {
    fDeltaPhiLeptons_NMinusOne->Fill(deltaPhiLeptons); 
    if (leptonType[0] == "e" && leptonType[1] == "e") {
      fDeltaPhiLeptons_NMinusOne_ee->Fill(deltaPhiLeptons); 
    } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
      fDeltaPhiLeptons_NMinusOne_mumu->Fill(deltaPhiLeptons); 
    } else {
      fDeltaPhiLeptons_NMinusOne_emu->Fill(deltaPhiLeptons); 
    }
  }
  
  //dilepton mass
  pass = true;
  for (int k=0;k<nCuts;k++) {
    if (k != 5)
      pass = (pass && passCut[k]);    
  }
  if (pass) {
    fDileptonMass_NMinusOne->Fill(dilepton->Mass());
    if (leptonType[0] == "e" && leptonType[1] == "e") {
      fDileptonMass_NMinusOne_ee->Fill(dilepton->Mass());
    } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
      fDileptonMass_NMinusOne_mumu->Fill(dilepton->Mass());
    } else {
      fDileptonMass_NMinusOne_emu->Fill(dilepton->Mass());
    }
  }
  
  //Lepton Pt Max
  pass = true;
  for (int k=0;k<nCuts;k++) {
    if (k != 6) {
      pass = (pass && passCut[k]);      
    }
  }
  if (pass) {
    fLeptonPtMax_NMinusOne->Fill(leptons[0]->Pt());
    if (leptonType[0] == "e" && leptonType[1] == "e") {
      fLeptonPtMax_NMinusOne_ee->Fill(leptons[0]->Pt());
    } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
      fLeptonPtMax_NMinusOne_mumu->Fill(leptons[0]->Pt());
    } else {
      fLeptonPtMax_NMinusOne_emu->Fill(leptons[0]->Pt());
    }
  }

  //Lepton Pt Min
  pass = true;
  for (int k=0;k<nCuts;k++) {
    if (k != 7) {
      pass = (pass && passCut[k]);      
    }
  }
  if (pass) {
    fLeptonPtMin_NMinusOne->Fill(leptons[1]->Pt());
    if (leptonType[0] == "e" && leptonType[1] == "e") {
      fLeptonPtMin_NMinusOne_ee->Fill(leptons[1]->Pt());
    } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
      fLeptonPtMin_NMinusOne_mumu->Fill(leptons[1]->Pt());
    } else {
      fLeptonPtMin_NMinusOne_emu->Fill(leptons[1]->Pt());
    }
  }

  //NDirtyMuons
  pass = true;
  for (int k=0;k<nCuts;k++) {
    if (k != 8)
      pass = (pass && passCut[k]);    
  }
  if (pass) {
    fNDirtyMuonsHist_NMinusOne->Fill(DirtyMuons->GetEntries());
    if (leptonType[0] == "e" && leptonType[1] == "e") {
      fNDirtyMuonsHist_NMinusOne_ee->Fill(DirtyMuons->GetEntries());
    } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
      fNDirtyMuonsHist_NMinusOne_mumu->Fill(DirtyMuons->GetEntries());
    } else {
      fNDirtyMuonsHist_NMinusOne_emu->Fill(DirtyMuons->GetEntries());
    }
  }
  
  //NCleanExtraTracks
  pass = true;
  for (int k=0;k<nCuts;k++) {
    if (k != 9)
      pass = (pass && passCut[k]);    
  }
  if (pass) {
    fNCleanExtraTracksHist_NMinusOne->Fill(CleanExtraTracks->GetEntries());
    if (leptonType[0] == "e" && leptonType[1] == "e") {
      fNCleanExtraTracksHist_NMinusOne_ee->Fill(CleanExtraTracks->GetEntries());
    } else if (leptonType[0] == "mu" && leptonType[1] == "mu" ) {
      fNCleanExtraTracksHist_NMinusOne_mumu->Fill(CleanExtraTracks->GetEntries());
    } else {
      fNCleanExtraTracksHist_NMinusOne_emu->Fill(CleanExtraTracks->GetEntries());
    }
  }     

  
  if(fPrintDebug) 
  {
    //print out event content to text
    cerr << "Electrons" << endl;
    for (UInt_t i = 0; i < CleanElectrons->GetEntries(); i++) {
      cerr << i << " " << CleanElectrons->At(i)->Pt() << " " 
           << CleanElectrons->At(i)->Eta() << " " << CleanElectrons->At(i)->Phi() 
           << " " << CleanElectrons->At(i)->ESuperClusterOverP() << endl;    
    }  
    cerr << "Muons" << endl;

    for (UInt_t i = 0; i < CleanMuons->GetEntries(); i++) {
      cerr << i << " " << CleanMuons->At(i)->Pt() << " " << CleanMuons->At(i)->Eta() 
           << " " << CleanMuons->At(i)->Phi() << endl;    
    }  
    
    cerr << "CorrectedMET" << endl;	  
    cerr << caloMet->Pt() << " " << caloMet->Eta() << " " << caloMet->Phi() << endl;      
  }
 
  
}
//--------------------------------------------------------------------------------------------------
void VBFHwwEvtSelMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fMetName, fMet);
  ReqBranch(fMCPartName, fParticles);
  ReqBranch(fTrackName,  fTracks);
  ReqBranch(fJetName,    fJets);
  ReqBranch(fMuonName,   fMuons);

//*************************************************************************************************
// Generator Level Information
//*************************************************************************************************
  fGenLeptonEta              = new TH1D("hGenLeptonEta",";LeptonEta;Number of Events",100,-5.,5.0);
  fGenLeptonPtMax            = new TH1D("hGenLeptonPtMax",";Lepton P_t Max;Number of Events",
                                        150,0.,150.);
  fGenLeptonPtMin            = new TH1D("hGenLeptonPtMin",";Lepton P_t Min;Number of Events",
                                        150,0.,150.);
  fFwdQuarkPt                = new TH1D("hFwdQuarkPt",";VBF Forward Quark P_t;Number of Events",
                                        150,0.,300.);
  fFwdQuarkEta               = new TH1D("hFwdQuarkEta",";VBF Forward Quark Eta;Number of Events",
                                        100,-5.,5.0);
  fFwdQuarkDeltaEta          = new TH1D("hFwdQuarkDeltaEta",
                                        ";VBF FwdQuarks Delta Eta;Number of Events",100,0,10.0);
  fMCEventClassification     = new TH1D("hMCEventClassification",
                                        ";MC Event Classification;Number of Events",3,-0.5,2.5);

  AddOutput(fGenLeptonEta );
  AddOutput(fGenLeptonPtMax);
  AddOutput(fGenLeptonPtMin );
  AddOutput(fFwdQuarkPt);
  AddOutput(fFwdQuarkEta);
  AddOutput(fFwdQuarkDeltaEta);
  AddOutput(fMCEventClassification);
//*************************************************************************************************
// Preselection Histograms
//*************************************************************************************************
  fCentralJetEta_presel = new TH1D("hCentralJetEta_presel",";Central Jet Eta;Number of Events",
                                   100,-5.,5.0);
  fCentralJetPt_presel  = new TH1D("hCentralJetPt_presel",";Central Jet P_t;Number of Events",
                                   150,0.,150.);
  fNCentralJets_presel  = new TH1D("hNCentralJets_presel",
                                   ";Number of Central Jets;Number of Events", 6,-0.5,5.5);
  fVBFJetEta_presel     = new TH1D("hVBFJetEta_presel",";VBF Jet Eta;Number of Events",100,-5.,5.0);
  fVBFJetPt_presel      = new TH1D("hVBFJetPt_presel",";VBF Jet P_t;Number of Events",150,0.,150.);
  fNVBFJets_presel      = new TH1D("hNVBFJets_presel",";Number of Central Jets;Number of Events",
                                   6,-0.5,5.5);

  AddOutput(fCentralJetEta_presel);
  AddOutput(fCentralJetPt_presel);
  AddOutput(fNCentralJets_presel);
  AddOutput(fVBFJetEta_presel);
  AddOutput(fVBFJetPt_presel);
  AddOutput(fNVBFJets_presel);

//*************************************************************************************************
// Selection Histograms
//*************************************************************************************************

  fHWWSelection       = new TH1D("hWWSelection", ";Cut Number;Number of Events", 11, -1.5, 9.5);
  fHWWToEESelection   = new TH1D("hWWToEESelection", ";Cut Number;Number of Events", 11, -1.5, 9.5);
  fHWWToMuMuSelection = new TH1D("hWWToMuMuSelection", ";Cut Number;Number of Events", 
                                11, -1.5, 9.5);
  fHWWToEMuSelection  = new TH1D("hWWToEMuSelection", ";Cut Number;Number of Events", 
                                11, -1.5, 9.5);

  AddOutput(fHWWSelection);
  AddOutput(fHWWToEESelection);
  AddOutput(fHWWToMuMuSelection);
  AddOutput(fHWWToEMuSelection);


//*************************************************************************************************
// Sequential Cuts histograms
//*************************************************************************************************
  fLeptonEta              = new TH1D("hLeptonEta",";LeptonEta;Number of Events",100,-5.,5.0);
  fLeptonPtMax            = new TH1D("hLeptonPtMax",";Lepton P_t Max;Number of Events",150,0.,150.);
  fLeptonPtMin            = new TH1D("hLeptonPtMin",";Lepton P_t Min;Number of Events",150,0.,150.);
  fMetPtHist              = new TH1D("hMetPtHist",";Met;Number of Events",150,0.,300.);  
  fMetPhiHist             = new TH1D("hMetPhiHist",";#phi;Number of Events",28,-3.5,3.5);
  fNCentralJets           = new TH1D("hNCentralJets",";Number of Central Jets;Number of Events",
                                     6,-0.5,5.5);
  fNVBFJets               = new TH1D("hNVBFJets",";Number of Central Jets;Number of Events",
                                     6,-0.5,5.5);
  fDeltaPhiLeptons        = new TH1D("hDeltaPhiLeptons",";#Delta#phi_{ll};Number of Events",
                                     90,0,180);
  fDeltaEtaLeptons        = new TH1D("hDeltaEtaLeptons",";#Delta#eta_{ll};Number of Events",
                                     90,0.,180);
  fDileptonMass           = new TH1D("hDileptonMass",";Mass_{ll};Number of Events",150,0.,300.);
  fMinDeltaPhiLeptonMet   = new TH1D("hMinDeltaPhiLeptonMet", 
                                   ";Min #Delta#phi_{l,Met};Number of Events",90,0.,180);
  fNDirtyMuonsHist        = new TH1D("hNDirtyMuonsHist", ";Number of Dirty Muons; Number of Events",
                                     6,-0.5,5.5);
  fNCleanExtraTracksHist  = new TH1D("hNCleanExtraTracksHist",
                                     ";Number of Clean Extra Tracks; Number of Events",
                                     15,-0.5,14.5);

  //ee events
  fLeptonEta_ee              = new TH1D("hLeptonEta_ee",";LeptonEta;Number of Events",100,-5.,5.0);
  fLeptonPtMax_ee            = new TH1D("hLeptonPtMax_ee",";Lepton P_t Max;Number of Events",
                                        150,0.,150.);
  fLeptonPtMin_ee            = new TH1D("hLeptonPtMin_ee",";Lepton P_t Min;Number of Events",
                                        150,0.,150.);
  fMetPtHist_ee              = new TH1D("hMetPtHist_ee",";Met;Number of Events",150,0.,300.);  
  fMetPhiHist_ee             = new TH1D("hMetPhiHist_ee",";#phi;Number of Events",28,-3.5,3.5);
  fNCentralJets_ee           = new TH1D("hNCentralJets_ee",
                                        ";Number of Central Jets;Number of Events",6,-0.5,5.5);
  fNVBFJets_ee               = new TH1D("hNVBFJets_ee",";Number of Central Jets;Number of Events",
                                        6,-0.5,5.5);
  fDeltaPhiLeptons_ee        = new TH1D("hDeltaPhiLeptons_ee",";#Delta#phi_{ll};Number of Events",
                                        90,0,180);
  fDeltaEtaLeptons_ee        = new TH1D("hDeltaEtaLeptons_ee",";#Delta#eta_{ll};Number of Events",
                                        90,0.,180);
  fDileptonMass_ee           = new TH1D("hDileptonMass_ee",";Mass_{ll};Number of Events",
                                        150,0.,300.);
  fMinDeltaPhiLeptonMet_ee   = new TH1D("hMinDeltaPhiLeptonMet_ee", 
                                        ";Min #Delta#phi_{l,Met};Number of Events",90,0.,180);
  fNDirtyMuonsHist_ee        = new TH1D("hNDirtyMuonsHist_ee",
                                        ";Number of Dirty Muons; Number of Events",6,-0.5,5.5);
  fNCleanExtraTracksHist_ee  = new TH1D("hNCleanExtraTracksHist_ee",
                                        ";Number of Clean Extra Tracks; Number of Events",
                                        15,-0.5,14.5);

  //mumu events
  fLeptonEta_mumu             = new TH1D("hLeptonEta_mumu",";LeptonEta;Number of Events",
                                         100,-5.,5.0);
  fLeptonPtMax_mumu           = new TH1D("hLeptonPtMax_mumu",";Lepton P_t Max;Number of Events",
                                         150,0.,150.); 
  fLeptonPtMin_mumu           = new TH1D("hLeptonPtMin_mumu",";Lepton P_t Min;Number of Events",
                                         150,0.,150.);
  fMetPtHist_mumu             = new TH1D("hMetPtHist_mumu",";Met;Number of Events",150,0.,300.);  
  fMetPhiHist_mumu            = new TH1D("hMetPhiHist_mumu",";#phi;Number of Events",28,-3.5,3.5);
  fNCentralJets_mumu          = new TH1D("hNCentralJets_mumu",
                                         ";Number of Central Jets;Number of Events",6,-0.5,5.5);
  fNVBFJets_mumu               = new TH1D("hNVBFJets_mumu",
                                          ";Number of Central Jets;Number of Events", 6,-0.5,5.5);
  fDeltaPhiLeptons_mumu       = new TH1D("hDeltaPhiLeptons_mumu",
                                         ";#Delta#phi_{ll};Number of Events",90,0,180);
  fDeltaEtaLeptons_mumu       = new TH1D("hDeltaEtaLeptons_mumu",
                                         ";#Delta#eta_{ll};Number of Events",90,0.,180);
  fDileptonMass_mumu          = new TH1D("hDileptonMass_mumu",";Mass_{ll};Number of Events",
                                         150,0.,300.);
  fMinDeltaPhiLeptonMet_mumu  = new TH1D("hMinDeltaPhiLeptonMet_mumu", 
                                         ";Min #Delta#phi_{l,Met};Number of Events",90,0.,180);
  fNDirtyMuonsHist_mumu       = new TH1D("hNDirtyMuonsHist_mumu",
                                         ";Number of Dirty Muons; Number of Events",6,-0.5,5.5);
  fNCleanExtraTracksHist_mumu = new TH1D("hNCleanExtraTracksHist_mumu",
                                         ";Number of Clean Extra Tracks; Number of Events",
                                         15,-0.5,14.5);

  //emu events
  fLeptonEta_emu              = new TH1D("hLeptonEta_emu",";LeptonEta;Number of Events",
                                         100,-5.,5.0);
  fLeptonPtMax_emu            = new TH1D("hLeptonPtMax_emu",";Lepton P_t Max;Number of Events",
                                         150,0.,150.); 
  fLeptonPtMin_emu            = new TH1D("hLeptonPtMin_emu",";Lepton P_t Min;Number of Events",
                                         150,0.,150.);
  fMetPtHist_emu              = new TH1D("hMetPtHist_emu",";Met;Number of Events",150,0.,300.);  
  fMetPhiHist_emu             = new TH1D("hMetPhiHist_emu",";#phi;Number of Events",28,-3.5,3.5);
  fNCentralJets_emu           = new TH1D("hNCentralJets_emu",
                                         ";Number of Central Jets;Number of Events",6,-0.5,5.5);
  fNVBFJets_emu               = new TH1D("hNVBFJets_emu",
                                         ";Number of Central Jets;Number of Events", 6,-0.5,5.5);
  fDeltaPhiLeptons_emu        = new TH1D("hDeltaPhiLeptons_emu",";#Delta#phi_{ll};Number of Events",
                                         90,0,180);
  fDeltaEtaLeptons_emu        = new TH1D("hDeltaEtaLeptons_emu",";#Delta#eta_{ll};Number of Events",
                                         90,0.,180);
  fDileptonMass_emu           = new TH1D("hDileptonMass_emu",";Mass_{ll};Number of Events",
                                         150,0.,300.);
  fMinDeltaPhiLeptonMet_emu   = new TH1D("hMinDeltaPhiLeptonMet_emu", 
                                         ";Min #Delta#phi_{l,Met};Number of Events",90,0.,180);
  fNDirtyMuonsHist_emu        = new TH1D("hNDirtyMuonsHist_emu",
                                         ";Number of Dirty Muons; Number of Events",6,-0.5,5.5);
  fNCleanExtraTracksHist_emu  = new TH1D("hNCleanExtraTracksHist_emu",
                                         ";Number of Clean Extra Tracks; Number of Events",
                                         15,-0.5,14.5);

  AddOutput(fLeptonEta);                                       
  AddOutput(fLeptonPtMax);
  AddOutput(fLeptonPtMin);
  AddOutput(fMetPtHist);
  AddOutput(fMetPhiHist);
  AddOutput(fNCentralJets);
  AddOutput(fNVBFJets);
  AddOutput(fNDirtyMuonsHist);
  AddOutput(fNCleanExtraTracksHist);
  AddOutput(fDeltaPhiLeptons);
  AddOutput(fDeltaEtaLeptons);
  AddOutput(fDileptonMass);
  AddOutput(fMinDeltaPhiLeptonMet);
  AddOutput(fMetPhiHist_ee);
  AddOutput(fMetPtHist_ee);
  AddOutput(fNCentralJets_ee);
  AddOutput(fNVBFJets_ee);
  AddOutput(fNDirtyMuonsHist_ee);
  AddOutput(fNCleanExtraTracksHist_ee);
  AddOutput(fDeltaPhiLeptons_ee);
  AddOutput(fDeltaEtaLeptons_ee);
  AddOutput(fDileptonMass_ee);
  AddOutput(fLeptonPtMax_ee);
  AddOutput(fLeptonPtMin_ee);
  AddOutput(fLeptonEta_ee);                                       
  AddOutput(fMinDeltaPhiLeptonMet_ee);
  AddOutput(fMetPhiHist_mumu);
  AddOutput(fMetPtHist_mumu);
  AddOutput(fNCentralJets_mumu);
  AddOutput(fNVBFJets_mumu);
  AddOutput(fNDirtyMuonsHist_mumu);
  AddOutput(fNCleanExtraTracksHist_mumu);
  AddOutput(fDeltaPhiLeptons_mumu);
  AddOutput(fDeltaEtaLeptons_mumu);
  AddOutput(fDileptonMass_mumu);
  AddOutput(fLeptonPtMax_mumu);
  AddOutput(fLeptonPtMin_mumu);
  AddOutput(fLeptonEta_mumu);                                       
  AddOutput(fMinDeltaPhiLeptonMet_mumu);
  AddOutput(fMetPhiHist_emu);
  AddOutput(fMetPtHist_emu);
  AddOutput(fNCentralJets_emu);
  AddOutput(fNVBFJets_emu);
  AddOutput(fNDirtyMuonsHist_emu);
  AddOutput(fNCleanExtraTracksHist_emu);
  AddOutput(fDeltaPhiLeptons_emu);
  AddOutput(fDeltaEtaLeptons_emu);
  AddOutput(fDileptonMass_emu);
  AddOutput(fLeptonPtMax_emu);
  AddOutput(fLeptonPtMin_emu);
  AddOutput(fLeptonEta_emu);                                       
  AddOutput(fMinDeltaPhiLeptonMet_emu);

//*************************************************************************************************
// N-1 Cuts Histograms
//*************************************************************************************************
  //All events
  fLeptonEta_NMinusOne             = new TH1D("hLeptonEta_NMinusOne",";LeptonEta;Number of Events",
                                              100,-5.,5.0);
  fLeptonPtMax_NMinusOne            = new TH1D("hLeptonPtMax_NMinusOne",
                                                ";Lepton P_t Max;Number of Events",150,0.,150.);
  fLeptonPtMin_NMinusOne            = new TH1D("hLeptonPtMin_NMinusOne",
                                               ";Lepton P_t Min;Number of Events",150,0.,150.);
  fMetPtHist_NMinusOne              = new TH1D("hMetPtHist_NMinusOne",
                                               ";Met;Number of Events",150,0.,300.);  
  fMetPhiHist_NMinusOne             = new TH1D("hMetPhiHist_NMinusOne",
                                               ";#phi;Number of Events",28,-3.5,3.5);
  fNCentralJets_NMinusOne           = new TH1D("hNCentralJets_NMinusOne",
                                               ";Number of Central Jets;Number of Events",
                                               6,-0.5,5.5);
  fNDirtyMuonsHist_NMinusOne        = new TH1D("hNDirtyMuonsHist_NMinusOne",
                                               ";Number of Dirty Muons; Number of Events",
                                               6,-0.5,5.5);
  fNCleanExtraTracksHist_NMinusOne  =  new TH1D("hNCleanExtraTracksHist_NMinusOne",
                                                ";Number of Clean Extra Tracks; Number of Events",
                                                15,-0.5,14.5);
  fDeltaPhiLeptons_NMinusOne        = new TH1D("hDeltaPhiLeptons_NMinusOne",
                                               ";#Delta#phi_{ll};Number of Events",90,0,180);
  fDeltaEtaLeptons_NMinusOne        = new TH1D("hDeltaEtaLeptons_NMinusOne",
                                               ";#Delta#eta_{ll};Number of Events",90,0.,180);
  fDileptonMass_NMinusOne           = new TH1D("hDileptonMass_NMinusOne",
                                               ";Mass_{ll};Number of Events",150,0.,300.);
  fMinDeltaPhiLeptonMet_NMinusOne   = new TH1D("hMinDeltaPhiLeptonMet_NMinusOne", 
                                               ";Min #Delta#phi_{l,Met};Number of Events",
                                               90,0.,180);

  //ee events
  fLeptonEta_NMinusOne_ee             = new TH1D("hLeptonEta_NMinusOne_ee",";LeptonEta;Number of Events",100,-5.,5.0);
  fLeptonPtMax_NMinusOne_ee           = new TH1D("hLeptonPtMax_NMinusOne_ee",";Lepton P_t Max;Number of Events",150,0.,150.);
  fLeptonPtMin_NMinusOne_ee           = new TH1D("hLeptonPtMin_NMinusOne_ee",";Lepton P_t Min;Number of Events",150,0.,150.);
  fMetPtHist_NMinusOne_ee             = new TH1D("hMetPtHist_NMinusOne_ee",";Met;Number of Events",150,0.,300.);  
  fMetPhiHist_NMinusOne_ee            = new TH1D("hMetPhiHist_NMinusOne_ee",";#phi;Number of Events",28,-3.5,3.5);
  fNCentralJets_NMinusOne_ee          = new TH1D("hNCentralJets_NMinusOne_ee",
                                                 ";Number of Central Jets;Number of Events",
                                                 6,-0.5,5.5);
  fNDirtyMuonsHist_NMinusOne_ee       = new TH1D("hNDirtyMuonsHist_NMinusOne_ee",
                                                 ";Number of Dirty Muons; Number of Events",
                                                 6,-0.5,5.5);
  fNCleanExtraTracksHist_NMinusOne_ee = new TH1D("hNCleanExtraTracksHist_NMinusOne_ee",
                                                 ";Number of Clean Extra Tracks; Number of Events",
                                                 15,-0.5,14.5);
  fDeltaPhiLeptons_NMinusOne_ee       = new TH1D("hDeltaPhiLeptons_NMinusOne_ee",
                                                 ";#Delta#phi_{ll};Number of Events",90,0,180);
  fDeltaEtaLeptons_NMinusOne_ee       = new TH1D("hDeltaEtaLeptons_NMinusOne_ee",
                                                 ";#Delta#eta_{ll};Number of Events",90,0.,180);
  fDileptonMass_NMinusOne_ee          = new TH1D("hDileptonMass_NMinusOne_ee",
                                                 ";Mass_{ll};Number of Events",150,0.,300.);
  fMinDeltaPhiLeptonMet_NMinusOne_ee  = new TH1D("hMinDeltaPhiLeptonMet_NMinusOne_ee", 
                                                 ";Min #Delta#phi_{l,Met};Number of Events",
                                                 90,0.,180);

  //mumu events
  fLeptonEta_NMinusOne_mumu             = new TH1D("hLeptonEta_NMinusOne_mumu",
                                                   ";LeptonEta;Number of Events",100,-5.,5.0);
  fLeptonPtMax_NMinusOne_mumu           = new TH1D("hLeptonPtMax_NMinusOne_mumu",
                                                   ";Lepton P_t Max;Number of Events",150,0.,150.);
  fLeptonPtMin_NMinusOne_mumu           = new TH1D("hLeptonPtMin_NMinusOne_mumu",
                                                   ";Lepton P_t Min;Number of Events",150,0.,150.);
  fMetPtHist_NMinusOne_mumu             = new TH1D("hMetPtHist_NMinusOne_mumu",
                                                   ";Met;Number of Events",150,0.,300.);  
  fMetPhiHist_NMinusOne_mumu            = new TH1D("hMetPhiHist_NMinusOne_mumu",
                                                   ";#phi;Number of Events",28,-3.5,3.5);
  fNCentralJets_NMinusOne_mumu          = new TH1D("hNCentralJets_NMinusOne_mumu",
                                                   ";Number of Central Jets;Number of Events",
                                                   6,-0.5,5.5);
  fNDirtyMuonsHist_NMinusOne_mumu       = new TH1D("hNDirtyMuonsHist_NMinusOne_mumu",
                                                   ";Number of Dirty Muons; Number of Events",
                                                   6,-0.5,5.5);
  fNCleanExtraTracksHist_NMinusOne_mumu = new TH1D("hNCleanExtraTracksHist_NMinusOne_mumu",
                                                   ";Number of Clean Extra Tracks;Number of Events",
                                                   15,-0.5,14.5);
  fDeltaPhiLeptons_NMinusOne_mumu       = new TH1D("hDeltaPhiLeptons_NMinusOne_mumu",
                                                   ";#Delta#phi_{ll};Number of Events",90,0,180);
  fDeltaEtaLeptons_NMinusOne_mumu       = new TH1D("hDeltaEtaLeptons_NMinusOne_mumu",
                                                   ";#Delta#eta_{ll};Number of Events",90,0.,180);
  fDileptonMass_NMinusOne_mumu          = new TH1D("hDileptonMass_NMinusOne_mumu",
                                                   ";Mass_{ll};Number of Events",150,0.,300.);
  fMinDeltaPhiLeptonMet_NMinusOne_mumu  = new TH1D("hMinDeltaPhiLeptonMet_NMinusOne_mumu", 
                                                   ";Min #Delta#phi_{l,Met};Number of Events",
                                                   90,0.,180);

  //emu events
  fLeptonEta_NMinusOne_emu              = new TH1D("hLeptonEta_NMinusOne_emu",
                                                        ";LeptonEta;Number of Events",100,-5.,5.0);
  fLeptonPtMax_NMinusOne_emu            = new TH1D("hLeptonPtMax_NMinusOne_emu",
                                                        ";Lepton P_t Max;Number of Events",
                                                        150,0.,150.);
  fLeptonPtMin_NMinusOne_emu            = new TH1D("hLeptonPtMin_NMinusOne_emu",
                                                        ";Lepton P_t Min;Number of Events",
                                                        150,0.,150.);
  fMetPtHist_NMinusOne_emu              = new TH1D("hMetPtHist_NMinusOne_emu",
                                                        ";Met;Number of Events",150,0.,300.);  
  fMetPhiHist_NMinusOne_emu             = new TH1D("hMetPhiHist_NMinusOne_emu",
                                                        ";#phi;Number of Events",28,-3.5,3.5);
  fNCentralJets_NMinusOne_emu           = new TH1D("hNCentralJets_NMinusOne_emu",
                                                        ";Number of Central Jets;Number of Events",
                                                        6,-0.5,5.5);
  fNDirtyMuonsHist_NMinusOne_emu        = new TH1D("hNDirtyMuonsHist_NMinusOne_emu",
                                                        ";Number of Dirty Muons; Number of Events",
                                                        6,-0.5,5.5);
  fNCleanExtraTracksHist_NMinusOne_emu  = new TH1D("hNCleanExtraTracksHist_NMinusOne_emu",
                                                  ";Number of Clean Extra Tracks; Number of Events",
                                                        15,-0.5,14.5);
  fDeltaPhiLeptons_NMinusOne_emu        = new TH1D("hDeltaPhiLeptons_NMinusOne_emu",
                                                        ";#Delta#phi_{ll};Number of Events",
                                                        90,0,180);
  fDeltaEtaLeptons_NMinusOne_emu        = new TH1D("hDeltaEtaLeptons_NMinusOne_mumu_emu",
                                                        ";#Delta#eta_{ll};Number of Events",
                                                        90,0.,180);
  fDileptonMass_NMinusOne_emu           = new TH1D("hDileptonMass_NMinusOne_emu",
                                                        ";Mass_{ll};Number of Events",150,0.,300.);
  fMinDeltaPhiLeptonMet_NMinusOne_emu   = new TH1D("hMinDeltaPhiLeptonMet_NMinusOne_emu", 
                                                        ";Min #Delta#phi_{l,Met};Number of Events",
                                                        90,0.,180);

  AddOutput(fLeptonPtMax_NMinusOne);
  AddOutput(fLeptonPtMin_NMinusOne);
  AddOutput(fMetPtHist_NMinusOne);
  AddOutput(fMetPhiHist_NMinusOne);
  AddOutput(fNCentralJets_NMinusOne);
  AddOutput(fNDirtyMuonsHist_NMinusOne);
  AddOutput(fNCleanExtraTracksHist_NMinusOne);
  AddOutput(fDeltaPhiLeptons_NMinusOne);
  AddOutput(fDeltaEtaLeptons_NMinusOne);
  AddOutput(fDileptonMass_NMinusOne);
  AddOutput(fLeptonEta_NMinusOne);                                       
  AddOutput(fMinDeltaPhiLeptonMet_NMinusOne);
  AddOutput(fMetPhiHist_NMinusOne_ee);
  AddOutput(fMetPtHist_NMinusOne_ee);
  AddOutput(fNCentralJets_NMinusOne_ee);
  AddOutput(fNDirtyMuonsHist_NMinusOne_ee);
  AddOutput(fNCleanExtraTracksHist_NMinusOne_ee);
  AddOutput(fDeltaPhiLeptons_NMinusOne_ee);
  AddOutput(fDeltaEtaLeptons_NMinusOne_ee);
  AddOutput(fDileptonMass_NMinusOne_ee);
  AddOutput(fLeptonPtMax_NMinusOne_ee);
  AddOutput(fLeptonPtMin_NMinusOne_ee);
  AddOutput(fLeptonEta_NMinusOne_ee);                                       
  AddOutput(fMinDeltaPhiLeptonMet_NMinusOne_ee);
  AddOutput(fMetPhiHist_NMinusOne_mumu);
  AddOutput(fMetPtHist_NMinusOne_mumu);
  AddOutput(fNCentralJets_NMinusOne_mumu);
  AddOutput(fNDirtyMuonsHist_NMinusOne_mumu);
  AddOutput(fNCleanExtraTracksHist_NMinusOne_mumu);
  AddOutput(fDeltaPhiLeptons_NMinusOne_mumu);
  AddOutput(fDeltaEtaLeptons_NMinusOne_mumu);
  AddOutput(fDileptonMass_NMinusOne_mumu);
  AddOutput(fLeptonPtMax_NMinusOne_mumu);
  AddOutput(fLeptonPtMin_NMinusOne_mumu);
  AddOutput(fLeptonEta_NMinusOne_mumu);                                       
  AddOutput(fMinDeltaPhiLeptonMet_NMinusOne_mumu);
  AddOutput(fMetPhiHist_NMinusOne_emu);
  AddOutput(fMetPtHist_NMinusOne_emu);
  AddOutput(fNCentralJets_NMinusOne_emu);
  AddOutput(fNDirtyMuonsHist_NMinusOne_emu);
  AddOutput(fNCleanExtraTracksHist_NMinusOne_emu);
  AddOutput(fDeltaPhiLeptons_NMinusOne_emu);
  AddOutput(fDeltaEtaLeptons_NMinusOne_emu);
  AddOutput(fDileptonMass_NMinusOne_emu);
  AddOutput(fLeptonPtMax_NMinusOne_emu);
  AddOutput(fLeptonPtMin_NMinusOne_emu);
  AddOutput(fLeptonEta_NMinusOne_emu);                                       
  AddOutput(fMinDeltaPhiLeptonMet_NMinusOne_emu);


//*************************************************************************************************
// After all cuts Histograms
//*************************************************************************************************

  fLeptonEta_afterCuts             = new TH1D("hLeptonEta_afterCuts",";LeptonEta;Number of Events",
                                             100,-5.,5.0);
  fLeptonPtMax_afterCuts           = new TH1D("hLeptonPtMax_afterCuts",
                                             ";Lepton P_t Max;Number of Events",150,0.,150.);
  fLeptonPtMin_afterCuts           = new TH1D("hLeptonPtMin_afterCuts",
                                             ";Lepton P_t Min;Number of Events",150,0.,150.);

  fMetPtHist_afterCuts             = new TH1D("hMetPtHist_afterCuts",";p_{t};Number of Events",
                                             150,0.,300.);  
  fMetPhiHist_afterCuts            = new TH1D("hMetPhiHist_afterCuts",";#phi;Number of Events",
                                             28,-3.5,3.5);
  fDeltaPhiLeptons_afterCuts       = new TH1D("hDeltaPhiLeptons_afterCuts",
                                             ";#Delta#phi_{ll};Number of Events",90,0,180);
  fDeltaEtaLeptons_afterCuts       = new TH1D("hDeltaEtaLeptons_afterCuts",
                                             ";#Delta#eta_{ll};Number of Events",90,0.,180);
  fDileptonMass_afterCuts          = new TH1D("hDileptonMass_afterCuts",
                                             ";Mass_{ll};Number of Events",150,0.,300.);
  fMinDeltaPhiLeptonMet_afterCuts  = new TH1D("hMinDeltaPhiLeptonMet_afterCuts", 
                                             ";Min #Delta#phi_{l,Met};Number of Events",90,0.,180);
  fMetSignificance_afterCuts       = new TH1D("hMetSignificance_afterCuts",
                                             ";MetSignificanceAfterCuts;Number of Events", 
                                             100,0.,20.);
  fSumEt_afterCuts                 = new TH1D("hSumEt_afterCuts",";SumEt;Number of Events",
                                              100,0.,500.);
  fMtLepton1_afterCuts             = new TH1D("hMtLepton1_afterCuts",
                                              ";M_t (Lepton1,Met);Number of Events",100,0.,200.);
  fMtLepton2_afterCuts             = new TH1D("hMtLepton2_afterCuts",
                                              ";M_t (Lepton2,Met);Number of Events",100,0.,200.);
  fMtHiggs_afterCuts                  = new TH1D("hMtHiggs_afterCuts",";M_t (l1+l2+Met);Number of Events",
                                              150,0.,300.);
  fLeptonPtPlusMet_afterCuts       = new TH1D("hLeptonPtPlusMet_afterCuts",
                                              ";LeptonPtPlusMet;Number of Events",150,0., 300.);
  fLeptonEta_afterCuts_ee            = new TH1D("hLeptonEta_afterCuts_ee",
                                                ";LeptonEta;Number of Events",100,-5.,5.0);
  fLeptonPtMax_afterCuts_ee          = new TH1D("hLeptonPtMax_afterCuts_ee",
                                                ";Lepton P_t Max;Number of Events",150,0.,150.);
  fLeptonPtMin_afterCuts_ee          = new TH1D("hLeptonPtMin_afterCuts_ee",
                                                ";Lepton P_t Min;Number of Events",150,0.,150.);
  fMetPtHist_afterCuts_ee            = new TH1D("hMetPtHist_afterCuts_ee",";p_{t};Number of Events",
                                                150,0.,300.);  
  fMetPhiHist_afterCuts_ee           = new TH1D("hMetPhiHist_afterCuts_ee",";#phi;Number of Events",
                                                28,-3.5,3.5);
  fDeltaPhiLeptons_afterCuts_ee      = new TH1D("hDeltaPhiLeptons_afterCuts_ee",
                                                ";#Delta#phi_{ll};Number of Events",90,0,180);
  fDeltaEtaLeptons_afterCuts_ee      = new TH1D("hDeltaEtaLeptons_afterCuts_ee",
                                                ";#Delta#eta_{ll};Number of Events",90,0.,180);
  fDileptonMass_afterCuts_ee         = new TH1D("hDileptonMass_afterCuts_ee",
                                                ";Mass_{ll};Number of Events",150,0.,300.);
  fMinDeltaPhiLeptonMet_afterCuts_ee = new TH1D("hMinDeltaPhiLeptonMet_afterCuts_ee", 
                                                ";Min #Delta#phi_{l,Met};Number of Events",
                                                90,0.,180);
  fMetSignificance_afterCuts_ee      = new TH1D("hMetSignificance_afterCuts_ee",
                                                ";MetSignificanceAfterCuts;Number of Events", 
                                                100,0.,20.);
  fSumEt_afterCuts_ee                = new TH1D("hSumEt_afterCuts_ee",";SumEt;Number of Events",
                                                100,0.,500.);
  fMtLepton1_afterCuts_ee            = new TH1D("hMtLepton1_afterCuts_ee",
                                                ";M_t (Lepton1,Met);Number of Events",100,0.,200.);
  fMtLepton2_afterCuts_ee            = new TH1D("hMtLepton2_afterCuts_ee",
                                                ";M_t (Lepton2,Met);Number of Events",100,0.,200.);
  fMtHiggs_afterCuts_ee                 = new TH1D("hMtHiggs_afterCuts_ee",
                                                ";M_t (l1+l2+Met);Number of Events",150,0.,300.);
  fLeptonPtPlusMet_afterCuts_ee      = new TH1D("hLeptonPtPlusMet_afterCuts_ee",
                                                ";LeptonPtPlusMet;Number of Events",150,0., 300.);

  fLeptonEta_afterCuts_mumu            = new TH1D("hLeptonEta_afterCuts_mumu",
                                                  ";LeptonEta;Number of Events",100,-5.,5.0);
  fLeptonPtMax_afterCuts_mumu          = new TH1D("hLeptonPtMax_afterCuts_mumu",
                                                  ";Lepton P_t Max;Number of Events",150,0.,150.);
  fLeptonPtMin_afterCuts_mumu          = new TH1D("hLeptonPtMin_afterCuts_mumu",
                                                  ";Lepton P_t Min;Number of Events",150,0.,150.);
  fMetPtHist_afterCuts_mumu            = new TH1D("hMetPtHist_afterCuts_mumu",
                                                  ";p_{t};Number of Events", 150,0.,300.);  
  fMetPhiHist_afterCuts_mumu           = new TH1D("hMetPhiHist_afterCuts_mumu",
                                                  ";#phi;Number of Events", 28,-3.5,3.5);
  fDeltaPhiLeptons_afterCuts_mumu      = new TH1D("hDeltaPhiLeptons_afterCuts_mumu",
                                                  ";#Delta#phi_{ll};Number of Events",90,0,180);
  fDeltaEtaLeptons_afterCuts_mumu      = new TH1D("hDeltaEtaLeptons_afterCuts_mumu",
                                                  ";#Delta#eta_{ll};Number of Events",90,0.,180);
  fDileptonMass_afterCuts_mumu         = new TH1D("hDileptonMass_afterCuts_mumu",
                                                  ";Mass_{ll};Number of Events",150,0.,300.);
  fMinDeltaPhiLeptonMet_afterCuts_mumu = new TH1D("hMinDeltaPhiLeptonMet_afterCuts_mumu", 
                                                  ";Min #Delta#phi_{l,Met};Number of Events",
                                                  90,0.,180);
  fMetSignificance_afterCuts_mumu      = new TH1D("hMetSignificance_afterCuts_mumu",
                                                  ";MetSignificanceAfterCuts;Number of Events", 
                                                  100,0.,20.);
  fSumEt_afterCuts_mumu                = new TH1D("hSumEt_afterCuts_mumu",";SumEt;Number of Events",
                                                  100,0.,500.);
  fMtLepton1_afterCuts_mumu            = new TH1D("hMtLepton1_afterCuts_mumu",
                                                  ";M_t (Lepton1,Met);Number of Events",
                                                  100,0.,200.);
  fMtLepton2_afterCuts_mumu            = new TH1D("hMtLepton2_afterCuts_mumu",
                                                  ";M_t (Lepton2,Met);Number of Events",
                                                  100,0.,200.);
  fMtHiggs_afterCuts_mumu                 = new TH1D("hMtHiggs_afterCuts_mumu",
                                                  ";M_t (l1+l2+Met);Number of Events",150,0.,300.);
  fLeptonPtPlusMet_afterCuts_mumu      = new TH1D("hLeptonPtPlusMet_afterCuts_mumu",
                                                  ";LeptonPtPlusMet;Number of Events",150,0., 300.);

  fLeptonEta_afterCuts_emu            = new TH1D("hLeptonEta_afterCuts_emu",
                                                 ";LeptonEta;Number of Events",100,-5.,5.0);
  fLeptonPtMax_afterCuts_emu          = new TH1D("hLeptonPtMax_afterCuts_emu",
                                                 ";Lepton P_t Max;Number of Events",150,0.,150.);
  fLeptonPtMin_afterCuts_emu          = new TH1D("hLeptonPtMin_afterCuts_emu",
                                                 ";Lepton P_t Min;Number of Events",150,0.,150.);
  fMetPtHist_afterCuts_emu            = new TH1D("hMetPtHist_afterCuts_emu",
                                                 ";p_{t};Number of Events", 150,0.,300.);  
  fMetPhiHist_afterCuts_emu           = new TH1D("hMetPhiHist_afterCuts_emu",
                                                 ";#phi;Number of Events", 28,-3.5,3.5);
  fDeltaPhiLeptons_afterCuts_emu      = new TH1D("hDeltaPhiLeptons_afterCuts_emu",
                                                 ";#Delta#phi_{ll};Number of Events",90,0,180);
  fDeltaEtaLeptons_afterCuts_emu      = new TH1D("hDeltaEtaLeptons_afterCuts_emu",
                                                 ";#Delta#eta_{ll};Number of Events",90,0.,180);
  fDileptonMass_afterCuts_emu         = new TH1D("hDileptonMass_afterCuts_emu",
                                                 ";Mass_{ll};Number of Events",150,0.,300.);
  fMinDeltaPhiLeptonMet_afterCuts_emu = new TH1D("hMinDeltaPhiLeptonMet_afterCuts_emu", 
                                                 ";Min #Delta#phi_{l,Met};Number of Events",
                                                 90,0.,180);
  fMetSignificance_afterCuts_emu      = new TH1D("hMetSignificance_afterCuts_emu",
                                                 ";MetSignificanceAfterCuts;Number of Events", 
                                                 100,0.,20.);
  fSumEt_afterCuts_emu                = new TH1D("hSumEt_afterCuts_emu",";SumEt;Number of Events",
                                                 100,0.,500.);
  fMtLepton1_afterCuts_emu            = new TH1D("hMtLepton1_afterCuts_emu",
                                                 ";M_t (Lepton1,Met);Number of Events",100,0.,200.);
  fMtLepton2_afterCuts_emu            = new TH1D("hMtLepton2_afterCuts_emu",
                                                 ";M_t (Lepton2,Met);Number of Events",100,0.,200.);
  fMtHiggs_afterCuts_emu                 = new TH1D("hMtHiggs_afterCuts_emu",
                                                 ";M_t (l1+l2+Met);Number of Events",150,0.,300.);
  fLeptonPtPlusMet_afterCuts_emu      = new TH1D("hLeptonPtPlusMet_afterCuts_emu",
                                                 ";LeptonPtPlusMet;Number of Events",150,0., 300.);

  AddOutput(fLeptonEta_afterCuts);
  AddOutput(fLeptonPtMax_afterCuts);
  AddOutput(fLeptonPtMin_afterCuts);
  AddOutput(fMetPtHist_afterCuts);                             
  AddOutput(fMetPhiHist_afterCuts);                           
  AddOutput(fDeltaPhiLeptons_afterCuts);
  AddOutput(fDeltaEtaLeptons_afterCuts);
  AddOutput(fDileptonMass_afterCuts);
  AddOutput(fMinDeltaPhiLeptonMet_afterCuts);                                     
  AddOutput(fMetSignificance_afterCuts);                               
  AddOutput(fSumEt_afterCuts);
  AddOutput(fMtLepton1_afterCuts);
  AddOutput(fMtLepton2_afterCuts);
  AddOutput(fMtHiggs_afterCuts);
  AddOutput(fLeptonPtPlusMet_afterCuts);
  AddOutput(fLeptonEta_afterCuts_ee);
  AddOutput(fLeptonPtMax_afterCuts_ee);
  AddOutput(fLeptonPtMin_afterCuts_ee);
  AddOutput(fMetPtHist_afterCuts_ee);                             
  AddOutput(fMetPhiHist_afterCuts_ee);                           
  AddOutput(fDeltaPhiLeptons_afterCuts_ee);
  AddOutput(fDeltaEtaLeptons_afterCuts_ee);
  AddOutput(fDileptonMass_afterCuts_ee);
  AddOutput(fMinDeltaPhiLeptonMet_afterCuts_ee);                                     
  AddOutput(fMetSignificance_afterCuts_ee);                               
  AddOutput(fSumEt_afterCuts_ee);
  AddOutput(fMtLepton1_afterCuts_ee);
  AddOutput(fMtLepton2_afterCuts_ee);
  AddOutput(fMtHiggs_afterCuts_ee);
  AddOutput(fLeptonPtPlusMet_afterCuts_ee);
  AddOutput(fLeptonEta_afterCuts_mumu);
  AddOutput(fLeptonPtMax_afterCuts_mumu);
  AddOutput(fLeptonPtMin_afterCuts_mumu);
  AddOutput(fMetPtHist_afterCuts_mumu);                             
  AddOutput(fMetPhiHist_afterCuts_mumu);                           
  AddOutput(fDeltaPhiLeptons_afterCuts_mumu);
  AddOutput(fDeltaEtaLeptons_afterCuts_mumu);
  AddOutput(fDileptonMass_afterCuts_mumu);
  AddOutput(fMinDeltaPhiLeptonMet_afterCuts_mumu);                                     
  AddOutput(fMetSignificance_afterCuts_mumu);                               
  AddOutput(fSumEt_afterCuts_mumu);
  AddOutput(fMtLepton1_afterCuts_mumu);
  AddOutput(fMtLepton2_afterCuts_mumu);
  AddOutput(fMtHiggs_afterCuts_mumu);
  AddOutput(fLeptonPtPlusMet_afterCuts_mumu);
  AddOutput(fLeptonEta_afterCuts_emu);
  AddOutput(fLeptonPtMax_afterCuts_emu);
  AddOutput(fLeptonPtMin_afterCuts_emu);
  AddOutput(fMetPtHist_afterCuts_emu);                             
  AddOutput(fMetPhiHist_afterCuts_emu);                           
  AddOutput(fDeltaPhiLeptons_afterCuts_emu);
  AddOutput(fDeltaEtaLeptons_afterCuts_emu);
  AddOutput(fDileptonMass_afterCuts_emu);
  AddOutput(fMinDeltaPhiLeptonMet_afterCuts_emu);                                     
  AddOutput(fMetSignificance_afterCuts_emu);                               
  AddOutput(fSumEt_afterCuts_emu);
  AddOutput(fMtLepton1_afterCuts_emu);
  AddOutput(fMtLepton2_afterCuts_emu);
  AddOutput(fMtHiggs_afterCuts_emu);
  AddOutput(fLeptonPtPlusMet_afterCuts_emu);

}

//--------------------------------------------------------------------------------------------------
void VBFHwwEvtSelMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.

  cerr << "Events Inside Acceptance: " << fNEventsInsideAcceptance << endl;
  cerr << "Events Processed: " << fNEventsProcessed << endl;
  cerr << "Events Passed Skim: " << fNEventsPassedSkim << endl;
  cerr << "Events Passed LeptonSelection: " << fNEventsPassedLeptonSelection << endl;
  cerr << "Events Passed KinematicPreselection: " << fNEventsPassedKinematicPreselection << endl;

  cerr << "For All" << endl;
  for (int i=1;i<=11;i++) {
    double CutEff = (i>1)?fHWWSelection->GetBinContent(i)/fHWWSelection->GetBinContent(i-1):1.0;
    cerr << "After Cut" << i-2 << " : " << fHWWSelection->GetBinContent(i) 
         << "   Efficiency of this Cut : " << CutEff << endl;
  }

  cerr << "For ee" << endl;
  for (int i=1;i<=11;i++) {
    double CutEff = (i>1)?fHWWToEESelection->GetBinContent(i)/fHWWToEESelection->
      GetBinContent(i-1):1.0;
    cerr << "After Cut" << i-2 << " : " << fHWWToEESelection->GetBinContent(i)
         << "   Efficiency of this Cut : " << CutEff << endl;
  }

  cerr << "For mumu" << endl;
  for (int i=1;i<=11;i++) {
    double CutEff = (i>1)?fHWWToMuMuSelection->GetBinContent(i)/fHWWToMuMuSelection->
      GetBinContent(i-1):1.0;
    cerr << "After Cut" << i-2 << " : " << fHWWToMuMuSelection->GetBinContent(i)
         << "   Efficiency of this Cut : " << CutEff << endl;
  }
  
  cerr << "For emu" << endl;  
  for (int i=1;i<=11;i++) {
    double CutEff = (i>1)?fHWWToEMuSelection->GetBinContent(i)/fHWWToEMuSelection->
      GetBinContent(i-1):1.0;
    cerr << "After Cut" << i-2 << " : " << fHWWToEMuSelection->GetBinContent(i)
         << "   Efficiency of this Cut : " << CutEff << endl;
  }

}

//--------------------------------------------------------------------------------------------------
void VBFHwwEvtSelMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
