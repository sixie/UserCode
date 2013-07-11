 // $Id: JetVetoStudyMod.cc,v 1.1 2008/11/12 15:04:49 sixie Exp $

#include "MitPhysics/SelMods/interface/JetVetoStudyMod.h"
#include <TH1D.h>
#include <TH2D.h>
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitCommon/MathTools/interface/MathUtils.h"

using namespace mithep;
ClassImp(mithep::JetVetoStudyMod)

//--------------------------------------------------------------------------------------------------
JetVetoStudyMod::JetVetoStudyMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fPrintDebug(false),
  fImposeSelectionCuts(false),
  fDoJetCorrections(false),
  fImposeDirtyMuonVeto(false),
  fPlotType("N-1"),
  fMetName(Names::gkCaloMetBrn),
  fMuonName(Names::gkMuonBrn),
  fCleanJetsName(Names::gkCleanJetsName),
  fTrackName(Names::gkTrackBrn),
  fJetName(Names::gkCaloJetBrn), 
  fGenJetName(Names::gkIC5GenJetBrn), 
  fMCPartName(Names::gkMCPartBrn),
  fMCLeptonsName(Names::gkMCLeptonsName),
  fMCTausName(Names::gkMCTausName),
  fMCNeutrinosName(Names::gkMCNeutrinosName),
  fMCQuarksName(Names::gkMCQuarksName),
  fMCqqHsName(Names::gkMCqqHsName),
  fMCBosonsName(Names::gkMCBosonsName),
  fLoadGenParticles(false),
  fMet(0),
  fMuons(0),
  fTracks(0),
  fJets(0),
  fGenJets(0),
  fParticles(0),
  fNEventsPassPreSelection(0),
  fNEventsPassSelection(0),
  fNEventsPassJetVeto(0),
  fNEventsProcessed(0)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void JetVetoStudyMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void JetVetoStudyMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and  
  fNEventsProcessed++;
  
  if (fNEventsProcessed % 1000 == 0 || fPrintDebug) {
    time_t systime;
    systime = time(NULL);
    cerr << endl << "JetVetoStudyMod : Process Event " << fNEventsProcessed << "  Time: " << ctime(&systime) << endl;
  }

  //Get Generator Level information for matching
  //  ObjArray<MCParticle> *GenLeptons   = dynamic_cast<ObjArray<MCParticle>* > (FindObjThisEvt(fMCLeptonsName.Data()));
  //  ObjArray<MCParticle> *GenTaus      = dynamic_cast<ObjArray<MCParticle>* > (FindObjThisEvt(fMCTausName.Data()));
//   ObjArray<MCParticle> *GenNeutrinos = dynamic_cast<ObjArray<MCParticle>* > (FindObjThisEvt(fMCNeutrinosName.Data()));
   //  ObjArray<MCParticle> *GenQuarks    = dynamic_cast<ObjArray<MCParticle>* > (FindObjThisEvt(fMCQuarksName.Data()));
  //ObjArray<MCParticle> *GenqqHs      = dynamic_cast<ObjArray<MCParticle>* > (FindObjThisEvt(fMCqqHsName.Data()));
  //ObjArray<MCParticle> *GenBosons    = dynamic_cast<ObjArray<MCParticle>* > (FindObjThisEvt(fMCBosonsName.Data()));

  //Obtain all the good objects from the event cleaning module
  ObjArray<Electron> *CleanElectrons = dynamic_cast<ObjArray<Electron>* >
    (FindObjThisEvt(Names::gkCleanElectronsName));
  ObjArray<Muon> *CleanMuons = dynamic_cast<ObjArray<Muon>* >
    (FindObjThisEvt(Names::gkCleanMuonsName));
  ObjArray<Jet> *CleanJets = dynamic_cast<ObjArray<Jet>* >
    (FindObjThisEvt(fCleanJetsName.Data()));

  LoadBranch(fMetName);
  Met *caloMet = fMet->At(0);

  vector<ChargedParticle*> leptons;
  vector<string> leptonType; 

  LoadBranch(fMuonName);
  ObjArray<Muon> *DirtyMuons = new ObjArray<Muon>;
  for (UInt_t i=0; i<fMuons->GetEntries(); ++i) {
    Muon *mu = fMuons->At(i);
    if(!mu->GlobalTrk()) continue;
    if(mu->Pt() < 5.0)   continue;

    bool isCleanMuon = false;
    for (UInt_t j=0; j<CleanMuons->GetEntries(); j++) {
      if(fMuons->At(i) == CleanMuons->At(j)) isCleanMuon = true;
    }
    if(isCleanMuon == false) DirtyMuons->Add(mu);
  }

  // Make lepton vector from muons and electrons
  for (UInt_t j=0; j<CleanMuons->GetEntries(); j++) {
    leptons.push_back(CleanMuons->At(j));
    leptonType.push_back("mu");
  }
  for (UInt_t j=0; j<CleanElectrons->GetEntries(); j++) {   
    leptons.push_back(CleanElectrons->At(j));
    leptonType.push_back("e");
  }

  // Sort the Leptons by Pt   
  for(UInt_t i=0; i<leptons.size(); i++){
    for(UInt_t j=i+1; j<leptons.size(); j++){
      if(leptons[i]->Pt() < leptons[j]->Pt()) {
	//swap i and j
	ChargedParticle* templepton = leptons[i];
	leptons[i] = leptons[j];
	leptons[j] = templepton;
	string templeptonType = leptonType[i];
	leptonType[i] = leptonType[j];
	leptonType[j] = templeptonType;	 
      }
    }
  }
  if (fPrintDebug) {
    cout << "Check Lepton Sort\n";
    for(UInt_t i=0; i<leptons.size(); i++)
      cout << leptons[i]->Pt() << endl;
  }

  // Pt requirements
  if (leptons.size() < 2 || leptons[0]->Pt() < 20 || leptons[1]->Pt() < 15) return;

  // MET requirement
  if (caloMet->Pt() < 0) return;

  CompositeParticle *dilepton = new CompositeParticle();
  dilepton->AddDaughter(leptons[0]);
  dilepton->AddDaughter(leptons[1]);
  // Charge of the leptons should be opposite
  if (dilepton->Charge() != 0) return;

  // Minimum mass requirement
  if (dilepton->Mass() < 12) return;

  // Sort and count the number of central Jets for vetoing
  vector<Jet*> sortedJets;
  int nCentralJets = 0;
  for(UInt_t i=0; i<CleanJets->GetEntries(); i++){
    if(fabs(CleanJets->At(i)->Eta()) < 2.5){
      nCentralJets++;
      Jet* jet_f = new Jet(CleanJets->At(i)->Px()*CleanJets->At(i)->L2RelativeCorrectionScale()*CleanJets->At(i)->L3AbsoluteCorrectionScale(),
                           CleanJets->At(i)->Py()*CleanJets->At(i)->L2RelativeCorrectionScale()*CleanJets->At(i)->L3AbsoluteCorrectionScale(),
                           CleanJets->At(i)->Pz()*CleanJets->At(i)->L2RelativeCorrectionScale()*CleanJets->At(i)->L3AbsoluteCorrectionScale(),
			   CleanJets->At(i)->E() *CleanJets->At(i)->L2RelativeCorrectionScale()*CleanJets->At(i)->L3AbsoluteCorrectionScale());
      sortedJets.push_back(jet_f);
    }
  }

  for(UInt_t i=0; i<sortedJets.size(); i++){
    for(UInt_t j=i+1; j<sortedJets.size(); j++){
      if(sortedJets[i]->Pt() < sortedJets[j]->Pt()) {
	//swap i and j
        Jet* tempjet = sortedJets[i];
        sortedJets[i] = sortedJets[j];
        sortedJets[j] = tempjet;	
      }
    }
  }

  // Delta phi between the 2 leptons in degrees
  double deltaPhiLeptons = MathUtils::DeltaPhi(leptons[0]->Phi(), 
                                               leptons[1]->Phi())* 180. / TMath::Pi();

  double deltaEtaLeptons = abs(leptons[0]->Eta() - leptons[1]->Eta());

  double deltaPhiDileptonMet = MathUtils::DeltaPhi(caloMet->Phi(), 
                                                   dilepton->Phi())* 180. / TMath::Pi();

  double mtHiggs = TMath::Sqrt(2.0*dilepton->Pt() * caloMet->Pt()*
			       (1.0 - cos(deltaPhiDileptonMet * TMath::Pi() / 180.0)));

  // Angle between MET and closest lepton
  double deltaPhiMetLepton[2] = {MathUtils::DeltaPhi(caloMet->Phi(), leptons[0]->Phi()),
                                 MathUtils::DeltaPhi(caloMet->Phi(), leptons[1]->Phi())};
  
  double minDeltaPhiMetLepton = (deltaPhiMetLepton[0] < deltaPhiMetLepton[1])?
    deltaPhiMetLepton[0]:deltaPhiMetLepton[1];
  minDeltaPhiMetLepton = minDeltaPhiMetLepton * 180. / TMath::Pi();

  double METdeltaPhilEt = caloMet->Pt();
  if(minDeltaPhiMetLepton < 90.0) 
    METdeltaPhilEt = METdeltaPhilEt * sin(minDeltaPhiMetLepton * TMath::Pi() / 180.);

  double mTW[2] = {TMath::Sqrt(2.0*leptons[0]->Pt()*caloMet->Pt()*
                               (1.0 - cos(deltaPhiMetLepton[0]))),
		   TMath::Sqrt(2.0*leptons[1]->Pt()*caloMet->Pt()*
                               (1.0 - cos(deltaPhiMetLepton[1])))};

  int pairType = -1;
  if (leptonType[0] == "mu" && leptonType[1] == "mu" )
    pairType = 0;
  else if(leptonType[0] == "e" && leptonType[1] == "e")
    pairType = 1;
  else if((leptonType[0] == "e" && leptonType[1] == "mu") || 
	  (leptonType[0] == "mu" && leptonType[1] == "e"))
    pairType = 2;
  else {
    cout << "Hey, this is not possible, leptonTypes: "
         << leptonType[0] << " - " 
	 << leptonType[1] << endl;
  }

  // No more than 2 isolated good leptons
  if (leptons.size() > 2) return;

  //event passes preselection
  fNEventsPassPreSelection++;

  //HWW Selection Cuts
    const int nCuts = 8;
  bool passCut[nCuts] = {false, false, false, false,
			   false, false, false, false};
  if(leptons[0]->Pt() > 7.0 &&
       leptons[1]->Pt() > 7.0)   passCut[0] = true;
  
  if(caloMet->Pt() > 30.0 &&
     dilepton->Mass()         > 12.0 &&
       //nCleanJets           < 3    &&
     leptons[0]->Pt()       > 20.0 &&
     leptons[1]->Pt()       > 10.0)  passCut[1] = true;
  
  if(nCentralJets < 1)     passCut[2] = true;

  if (pairType == 0 ){ // mumu
    if(caloMet->Pt()			> 50.0 &&
       caloMet->Pt()			< 200.0) passCut[3] = true;
    if(deltaPhiLeptons		< 45.0)  passCut[4] = true;
    if(dilepton->Mass()		< 50.0)  passCut[5] = true;
    if(leptons[0]->Pt()	> 35.0   &&
       leptons[0]->Pt()	< 55.0)  passCut[6] = true;
    if(leptons[1]->Pt()	> 25.0)  passCut[7] = true;
  }
  else if(pairType == 1){ // ee
    if(caloMet->Pt()			> 51.0 &&
       caloMet->Pt()			< 200.0) passCut[3] = true;
    if(deltaPhiLeptons		< 45.0)  passCut[4] = true;
    if(dilepton->Mass()		< 40.0)  passCut[5] = true;
      if(leptons[0]->Pt()	> 25.0   &&
	 leptons[0]->Pt()	< 49.0)  passCut[6] = true;
      if(leptons[1]->Pt()	> 25.0)  passCut[7] = true;
  }      
  else if(pairType == 2 ){ //emu
    if(caloMet->Pt()			> 45.0 &&
       caloMet->Pt()			< 105.0) passCut[3] = true;
    if(deltaPhiLeptons		< 70.0)  passCut[4] = true;
    if(dilepton->Mass()		< 45.0)  passCut[5] = true;
    if(leptons[0]->Pt()	> 25.0   &&
       leptons[0]->Pt()	< 50.0)  passCut[6] = true;
    if(leptons[1]->Pt()	> 25.0)  passCut[7] = true;
  }

  //impose all cuts except for jet veto
  //bool allCuts = (passCut[0] && passCut[1] &&passCut[3] &&passCut[4] &&passCut[5] &&passCut[6] &&passCut[7]);
  bool allCuts = (passCut[0] && passCut[1] &&passCut[3] &&passCut[5] );
  if (fImposeSelectionCuts && !allCuts) return;
  fNEventsPassSelection++;

  //load branches

  //study zero jet events AND pass Dirty Muon Veto
  LoadBranch(fTrackName);
  if(nCentralJets == 0 && (DirtyMuons->GetEntries() == 0 || !fImposeDirtyMuonVeto)) {
    fNEventsPassJetVeto++;
    if (fPrintDebug) {
      cerr << "Lepton 1: " << leptons[0]->Pt() << " " << leptons[0]->Eta() << " "
           << leptons[0]->Phi() << endl;
      cerr << "Lepton 2: " << leptons[1]->Pt() << " " << leptons[1]->Eta() << " "
           << leptons[1]->Phi() << endl;   
    } 

    //find GenJets    
    LoadBranch(fGenJetName);
    
    int NGenJets = 0;
    for (UInt_t j=0; j<fGenJets->GetEntries();j++) {
      if (fGenJets->At(j)->Pt() > 10.0 && fGenJets->At(j)->Eta() < 2.5)
        NGenJets++;
    }

    
    //try to find tracks.
    ObjArray<Track> *CleanTracks = new ObjArray<Track>;
    int nTracks = 0;
    //some information about the leptons
//     cerr << (dynamic_cast<Electron*>(leptons[0])) << endl;
//     cerr << (dynamic_cast<Muon*>(leptons[0])) << endl;
//     cerr << (dynamic_cast<Electron*>(leptons[1])) << endl;
//     cerr << (dynamic_cast<Muon*>(leptons[1])) << endl;

//     const Track *lepton0Track;
//     if ((dynamic_cast<Electron*>(leptons[0]))) lepton0Track = ((Electron*)leptons[0])->TrackerTrk();
//     else if ((dynamic_cast<Muon*>(leptons[0]))) lepton0Track = ((Muon*)leptons[0])->TrackerTrk();
//     else cerr << "lepton 0 is not electron or muon" << endl;
//     const Track *lepton1Track;
//     if ((dynamic_cast<Electron*>(leptons[1]))) lepton1Track = ((Electron*)leptons[1])->TrackerTrk();
//     else if ((dynamic_cast<Muon*>(leptons[1]))) lepton1Track = ((Muon*)leptons[1])->TrackerTrk();  
//     else cerr << "lepton 1 is not electron or muon" << endl;

    double z0Average = (leptons[0]->Trk()->Z0() + leptons[1]->Trk()->Z0())/2 ;
//     cerr << "here" <<  " " << lepton0Track << " " << lepton1Track << endl ;


    for (UInt_t i=0; i<fTracks->GetEntries(); ++i) {
      bool isLepton = false;

//      if (fTracks->At(i) != lepton0Track && fTracks->At(i) != lepton1Track) {
      
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
        CleanTracks->Add(fTracks->At(i));
        nTracks++;
      } 
    }

    //Fill histograms
    if (NGenJets == 0) {
      fNTracksNoGenJets->Fill(nTracks);
    } else {
      fNTracksWithGenJets->Fill(nTracks);
    }
  }

  LoadBranch(fJetName);
  ObjArray<Jet> *studyJets = new ObjArray<Jet>;
  for (UInt_t i=0; i<fJets->GetEntries(); ++i) {            
    if (fabs(fJets->At(i)->Eta()) < 2.5) {
      
      //remove overlaps with electrons
      bool isElectronOverlap = false;
      for (UInt_t j=0; j<CleanElectrons->GetEntries(); j++) {
        double deltaR = MathUtils::DeltaR(CleanElectrons->At(j)->Mom(),fJets->At(i)->Mom());  
        if (deltaR < 0.3) {
          isElectronOverlap = true;	 	 	
          break;	 	 
        }
      }
      if (isElectronOverlap) continue; //skip if it overlaps with an electron            
      if (fJets->At(i)->Et() < 5) continue; //skip jets below 10 GeV      
      studyJets->Add(fJets->At(i));
    }
  }

  //count the number of tracks;
  int NumberOfTracks = 0;    

  for (UInt_t i=0; i<fTracks->GetEntries(); ++i) {
    bool TrackIsLepton = false;

    if (MathUtils::DeltaR(fTracks->At(i)->Eta(),fTracks->At(i)->Phi(),leptons[0]->Eta(),
                          leptons[0]->Phi()) > 0.01 &&
        MathUtils::DeltaR(fTracks->At(i)->Eta(),fTracks->At(i)->Phi(),leptons[1]->Eta(),
                          leptons[1]->Phi()) > 0.01
      ) {
    } else {
      TrackIsLepton = true;
    }
    //exclude tracks matched to well identified leptons
    if (fTracks->At(i)->Pt() > 3.0 && !TrackIsLepton)
      NumberOfTracks++;
  }

  //make sure we don't exceed the matrix limits
  if (NumberOfTracks > 29) {
    cerr << "More than 29 Tracks above 3.0GeV!!!" << endl;
    //assert(nTracks <= 29);
  }

  for (int t=0; t<30; t++) {   
    if (NumberOfTracks <= t) {
      fNEventsPassTrackVetoHist->Fill(t);
      if (nCentralJets == 0  && (DirtyMuons->GetEntries() == 0 || !fImposeDirtyMuonVeto))
        fNEventsPassTrackVetoAfterJetVetoHist->Fill(t);
    }
  }

  for (UInt_t k=0; k< 100;k++) {
    double JetEtCut = 5.0 + k*1.0;
    bool JetPass = false;
    for (UInt_t l=0; l<studyJets->GetEntries(); ++l) { 
      double jetEt = studyJets->At(l)->Et();
      if (fDoJetCorrections)
        jetEt = jetEt * studyJets->At(l)->L2RelativeCorrectionScale()*studyJets->At(l)->L3AbsoluteCorrectionScale();      
      if ( jetEt > JetEtCut) {
        JetPass = true;
        break;
      }
    }    

    //Number of events which had no jets above jetEtCut
    if (!JetPass)
      fNEventsPassJetVetoVsJetEtCutHist->Fill(JetEtCut);

    for (int t=0; t<30; t++) {
      //Number of events which had no jets above jetEtCut OR with NTracks less than t          
      if (!JetPass || NumberOfTracks <= t) {
        fNEventsPassJetOrTrackVeto[t][k]++;
        fNEventsPassJetOrTrackVetoHist->Fill(t,JetEtCut);
      }
      //Number of events which had no jets above jetEtCut And with NTracks less than t          
      if (!JetPass &&  NumberOfTracks <= t) {
        fNEventsPassJetAndTrackVeto[t][k]++; 
        fNEventsPassJetAndTrackVetoHist->Fill(t,JetEtCut);
      }
    }
  }        
}
//--------------------------------------------------------------------------------------------------
void JetVetoStudyMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fMetName,    fMet);
  ReqBranch(fMuonName,   fMuons);
  ReqBranch(fTrackName,  fTracks);
  ReqBranch(fJetName,    fJets);
  ReqBranch(fGenJetName, fGenJets);
  ReqBranch(fMCPartName, fParticles);
  //initialize the 2D matrix
  for (UInt_t k=0; k< 30;k++) {   
    for (UInt_t l=0; l < 100;l++) {      
      fNEventsPassJetAndTrackVeto[k][l] = 0;
      fNEventsPassJetOrTrackVeto[k][l] = 0;
    }
  }

//Si's histograms
  fJetAndTrackVetoEff  = new TH2D("hJetAndTrackVetoEff",";NTracks Cut;JetEt Cut",31,-0.5,30.5,101,4.5,105.5);  
  fJetOrTrackVetoEff  = new TH2D("hJetOrTrackVetoEff",";NTracks Cut;JetEt Cut",31,-0.5,30.5,101,4.5,105.5);  
  fJetVetoEffVsJetEtCut  = new TH1D("hJetVetoEffVsJetEtCut",";JetEt Cut;Efficiency",101,4.5,105.5);  
  fTrackVetoEff  = new TH1D("hTrackVetoEff",";NTracksCut;Efficiency",31,-0.5,30.5);  
  fTrackVetoEffAfterJetVeto  = new TH1D("hTrackVetoEffAfterJetVeto",";NTracksCut;Efficiency",31,-0.5,30.5);  
  fNEventsPassJetAndTrackVetoHist  = new TH2D("hNEventsPassJetAndTrackVeto",";NTracks Cut;JetEt Cut",31,-0.5,30.5,101,4.5,105.5);  
  fNEventsPassJetOrTrackVetoHist  = new TH2D("hNEventsPassJetOrTrackVeto",";NTracks Cut;JetEt Cut",31,-0.5,30.5,101,4.5,105.5);  
  fNEventsPassJetVetoVsJetEtCutHist  = new TH1D("hNEventsPassJetVetoVsJetEtCutHist",";JetEtCut;Number of Events",101,4.5,105.5);  
  fNEventsPassTrackVetoHist  = new TH1D("hNEventsPassTrackVetoHist",";NTracks Cut;Number of Events",31,-0.5,30.5);  
  fNEventsPassTrackVetoAfterJetVetoHist  = new TH1D("hNEventsPassTrackVetoAfterJetVetoHist",";NTracks Cut;Number of Events",31,-0.5,30.5);  

  fNTracksNoGenJets = new TH1D("hNTracksNoGenJets",";NTracks;Number of Events",31,-0.5,30.5);  
  fNTracksWithGenJets = new TH1D("hNTracksWithGenJets",";NTracks;Number of Events",31,-0.5,30.5);  

  AddOutput(fJetAndTrackVetoEff);
  AddOutput(fJetOrTrackVetoEff);
  AddOutput(fJetVetoEffVsJetEtCut);
  AddOutput(fTrackVetoEff);
  AddOutput(fTrackVetoEffAfterJetVeto);
  AddOutput(fNEventsPassJetAndTrackVetoHist);
  AddOutput(fNEventsPassJetOrTrackVetoHist);
  AddOutput(fNEventsPassJetVetoVsJetEtCutHist);
  AddOutput(fNEventsPassTrackVetoHist);
  AddOutput(fNEventsPassTrackVetoAfterJetVetoHist);
  AddOutput(fNTracksNoGenJets);
  AddOutput(fNTracksWithGenJets);
}
  
//--------------------------------------------------------------------------------------------------
void JetVetoStudyMod::SlaveTerminate()
{
  double weight = 1/(fNEventsPassSelection + 0.0);
double JetVetoWeight = 1/(fNEventsPassJetVeto + 0.0);
  cerr << "NEventsProcessed" << fNEventsProcessed << endl;
  cerr << "NEventsPassSelection" << fNEventsPassSelection << "  weight=" << weight << endl;
  for (UInt_t l=0; l< 30;l++) {  
    fTrackVetoEff->Fill(l,weight*fNEventsPassTrackVetoHist->GetBinContent(l+1));
    fTrackVetoEffAfterJetVeto->Fill(l,JetVetoWeight*fNEventsPassTrackVetoAfterJetVetoHist->GetBinContent(l+1));
    for (UInt_t k=0; k< 100;k++) {
      double JetEtCut = 5.0 + k*1.0;
      fJetAndTrackVetoEff->Fill(l,JetEtCut,weight*fNEventsPassJetAndTrackVetoHist->GetBinContent(l+1,k+1));
      fJetOrTrackVetoEff->Fill(l,JetEtCut,weight*fNEventsPassJetOrTrackVetoHist->GetBinContent(l+1,k+1));   
//         fJetAndTrackVetoEff->Fill(l,JetEtCut,weight*fNEventsPassJetAndTrackVeto[l][k]);
//         fJetOrTrackVetoEff->Fill(l,JetEtCut,weight*fNEventsPassJetOrTrackVeto[l][k]);  
//        cout << fNEventsPassJetAndTrackVeto[l][k] << "   " << fNEventsPassJetAndTrackVetoHist->GetBinContent(l+1,k+1) << endl;
    }    
  }

  for (UInt_t k=0; k< 100;k++) {
    double JetEtCut = 5.0 + k*1.0;
    fJetVetoEffVsJetEtCut->Fill(JetEtCut, weight*fNEventsPassJetVetoVsJetEtCutHist->GetBinContent(k+1));
  }

  // Run finishing code on the computer (slave) that did the analysis
}

//--------------------------------------------------------------------------------------------------
void JetVetoStudyMod::Terminate()
{
  // Run finishing code on the client computer
}
