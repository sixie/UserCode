// $Id: WmunuAnalysisMod.cc,v 1.4 2008/12/04 19:19:49 sixie Exp $

#include "MitHiggs/EwkMods/interface/WmunuAnalysisMod.h"
#include <TH1D.h>
#include <TH2D.h>
#include "MitAna/DataTree/interface/Names.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitCommon/MathTools/interface/MathUtils.h"

using namespace mithep;

ClassImp(mithep::WmunuAnalysisMod)

//--------------------------------------------------------------------------------------------------
WmunuAnalysisMod::WmunuAnalysisMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fMCPartName(Names::gkMCPartBrn),
  fTrackName(Names::gkTrackBrn),
  fMuonName(Names::gkMuonBrn),
  fElectronName(Names::gkElectronBrn),
  fJetName(Names::gkCaloJetBrn),
  fMetName(Names::gkCaloMetBrn),
  fParticles(0),
  fTracks(0),
  fMuons(0),
  fElectrons(0),
  fNEventsProcessed(0),
  fNEventsPassedSkim(0),
  fNEventsPassedLeptonSelection(0),
  fNEventsPassedKinematicPreselection(0)

{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void WmunuAnalysisMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
  fGoodElectronsName = "GoodElectrons" ;        
  fGoodMuonsName = "GoodMuons" ;        
  fGoodCentralJetsName = "GoodCentralJets";        
  fGoodForwardJetsName = "GoodForwardJets" ;        
  fMuonCorrectedMetName = "MET" ;  
  fGenLeptonsName = "GenLeptons";
}

//--------------------------------------------------------------------------------------------------
void WmunuAnalysisMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and
  //output debug info or not  
  bool printDebug = false; 

  //Obtain generator objects
  ObjArray<MCParticle> *GenLeptons = dynamic_cast<ObjArray<MCParticle>* >(FindObjThisEvt(fGenLeptonsName.c_str()));

 //Obtain all the good objects from the event cleaning module
  ObjArray<Electron> *GoodElectrons = dynamic_cast<ObjArray<Electron>* >(FindObjThisEvt(fGoodElectronsName.c_str()));
  ObjArray<Muon> *GoodMuons = dynamic_cast<ObjArray<Muon>* >(FindObjThisEvt(fGoodMuonsName.c_str()));
  ObjArray<Jet> *GoodCentralJets = dynamic_cast<ObjArray<Jet>* >(FindObjThisEvt(fGoodCentralJetsName.c_str()));
  ObjArray<Jet> *GoodForwardJets = dynamic_cast<ObjArray<Jet>* >(FindObjThisEvt(fGoodForwardJetsName.c_str()));
  Met *muonCorrectedMet = dynamic_cast<Met*> (FindObjThisEvt(fMuonCorrectedMetName.c_str()));

  fNEventsProcessed++;
 
  if (fNEventsProcessed % 1000 == 0 || printDebug)
    cerr << endl << "Process Event " << fNEventsProcessed << endl;

  vector<Muon*> muons; 
  for (UInt_t j=0; j<GoodMuons->GetEntries(); j++)
    muons.push_back(GoodMuons->At(j));

  //sort them by pt
  for(UInt_t i=0; i<muons.size(); i++)
    for(UInt_t j=i+1; j<muons.size(); j++)
      if(muons[i]->Pt() < muons[j]->Pt()) {
	//swap i and j
	Muon* templepton = muons[i];
	muons[i] = muons[j];
	muons[j] = templepton;
      }
  
  //make sure there's at least one lepton with 5GeV
  if (muons.size() < 1 || muons[0]->Pt() < 5) return;
  fNEventsPassedLeptonSelection++;
    
  if (!(GenLeptons->GetEntries() == 2 && 
        ((GenLeptons->At(0)->AbsPdgId()==13 && GenLeptons->At(1)->AbsPdgId()==14) 
         || (GenLeptons->At(1)->AbsPdgId()==13 && GenLeptons->At(0)->AbsPdgId()==14))
        )
    ) {
    cerr << "NOT WENU EVENT!!!! WEIRD!!!!" << endl;
    return;
  }
  
  Int_t GenMuonIndex = 0;
  Int_t GenNeutrinoIndex = 0;
  if (GenLeptons->At(0)->AbsPdgId()==13) {
    GenMuonIndex = 0;
    GenNeutrinoIndex = 1;
  } else {
    GenMuonIndex = 1;
    GenNeutrinoIndex = 0;
  }
  
  
  MCParticle *GenMuon = new MCParticle(GenLeptons->At(GenMuonIndex)->Px(),GenLeptons->At(GenMuonIndex)->Py(),GenLeptons->At(GenMuonIndex)->Pz(),GenLeptons->At(GenMuonIndex)->E(),13,1);
  MCParticle *GenNeutrino = new MCParticle(GenLeptons->At(GenNeutrinoIndex)->Px(),GenLeptons->At(GenNeutrinoIndex)->Py(),GenLeptons->At(GenNeutrinoIndex)->Pz(),GenLeptons->At(GenNeutrinoIndex)->E(),14,1);

  CompositeParticle *W = new CompositeParticle();
  CompositeParticle *GenW = new CompositeParticle();
 
  W->AddDaughter(muons[0]);
  W->AddDaughter(muonCorrectedMet);
  GenW->AddDaughter(GenMuon);
  GenW->AddDaughter(GenNeutrino);
  fGenWMt->Fill(GenW->Mt());  
  fWMt->Fill(W->Mass());
    
  if(muons[0]->Eta() < 1.5) 
    fWMtBarrel->Fill(W->Mt());
  else 
    fWMtEndcap->Fill(W->Mt());

  fWMtVsLeptonEta->Fill(W->Mt(), muons[0]->Eta());
  fWMtVsLeptonPhi->Fill(W->Mt(), muons[0]->Phi());

  double minDeltaRLeptons = 5000;
  if (muons.size() > 1)
    for(UInt_t i=1; i<muons.size(); i++) {
      if (MathUtils::DeltaR(muons[0]->Mom(),muons[i]->Mom()) < minDeltaRLeptons) 
        minDeltaRLeptons = MathUtils::DeltaR(muons[0]->Mom(),muons[i]->Mom());
    }

  fMinDeltaRLeptons->Fill(minDeltaRLeptons);

  fLepton1Pt->Fill(muons[0]->Pt());
  fLepton1Eta->Fill(muons[0]->Eta()); 
  fLepton1Phi->Fill(muons[0]->Phi()); 

  if (muons.size()>1)
    fLepton2Pt->Fill(muons[1]->Pt());

  fMetHist->Fill(muonCorrectedMet->Pt());
  fNJets->Fill(GoodCentralJets->GetEntries()+GoodForwardJets->GetEntries());

  double minDRLeptonJet = 5000.0;
  for (UInt_t j=0; j<GoodCentralJets->GetEntries(); j++) {
    fJetPt->Fill(GoodCentralJets->At(j)->Pt());
    fJetEta->Fill(GoodCentralJets->At(j)->Eta());
    fJetPhi->Fill(GoodCentralJets->At(j)->Phi());
    if (MathUtils::DeltaR(GoodCentralJets->At(j)->Mom(),muons[0]->Mom()) < minDRLeptonJet)
      minDRLeptonJet = MathUtils::DeltaR(GoodCentralJets->At(j)->Mom(),muons[0]->Mom());   
  }
  for (UInt_t j=0; j<GoodForwardJets->GetEntries(); j++) {
    fJetPt->Fill(GoodForwardJets->At(j)->Pt());
    fJetEta->Fill(GoodForwardJets->At(j)->Eta());
    fJetPhi->Fill(GoodForwardJets->At(j)->Phi());
    if (MathUtils::DeltaR(GoodForwardJets->At(j)->Mom(),muons[0]->Mom()) < minDRLeptonJet)
       minDRLeptonJet = MathUtils::DeltaR(GoodForwardJets->At(j)->Mom(),muons[0]->Mom());   
  }

  if (minDRLeptonJet < 5000.0)
    fMinDeltaRLeptonJet->Fill(minDRLeptonJet);
  
  if ( 0 ==1 ) {
    //print out Generator BLock       
    LoadBranch(fMCPartName);
    cout << "Event Dump: " << fNEventsProcessed << endl;
    
    //print out event content to text
    cout << "Electrons" << endl;
    for (UInt_t i = 0; i < GoodElectrons->GetEntries(); i++) {
      cout << i << " " << GoodElectrons->At(i)->Pt() << " " << GoodElectrons->At(i)->Eta() 
           << " " << GoodElectrons->At(i)->Phi() << endl;
    }   
    
    cout << "Muons" << endl;
       for (UInt_t i = 0; i < GoodMuons->GetEntries(); i++) {
	 cout << i << " " << GoodMuons->At(i)->Pt() << " " << GoodMuons->At(i)->Eta() 
              << " " << GoodMuons->At(i)->Phi() << endl;    
       }
       
       cout << "Central Jets" << endl;
       for (UInt_t i = 0; i < GoodCentralJets->GetEntries(); i++) {
	 cout << i << " " << GoodCentralJets->At(i)->Pt() << " " 
              << GoodCentralJets->At(i)->Eta() << " " << GoodCentralJets->At(i)->Phi() << endl;    
       }
       
       cout << "CorrectedMET" << endl;	  
       cout << muonCorrectedMet->Pt() << " " << muonCorrectedMet->Eta() << " " 
            << muonCorrectedMet->Phi() << endl; 

       cout << "Generator Block" << endl;
       for (UInt_t i=0; i<fParticles->GetEntries(); ++i) {
	 const MCParticle* p = fParticles->At(i);	 
         cout << i << " " << p->PdgId() << " " << p->Status() << " " << p->Pt() 
                << " " << p->Eta() << " " << p->Phi() << endl;	    	 
       }
      
       

   }
  
}


//--------------------------------------------------------------------------------------------------
void WmunuAnalysisMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fMCPartName,                fParticles);
  ReqBranch(fTrackName,                 fTracks);
  ReqBranch(fMuonName,                  fMuons);
  ReqBranch(fElectronName,              fElectrons);
  ReqBranch(fJetName,                   fJets);
  ReqBranch(fMetName,                   fMet);

  fGenWMt                  = new TH1D("hGenGenWMt",";GenGenWMt;#",200,0.,200.);
  fWMt                     = new TH1D("hWMt",";WMt;#",200,0.,200.);
  fWMtBarrel               = new TH1D("hWMtBarrel",";WMtBarrel;#",200,0.,200.);
  fWMtEndcap              = new TH1D("hWMtEndcap",";WMtEndcap;#",200,0.,200.);
  fWMtVsLeptonEta          = new TH2D("hWMtVsLeptonEta",
                                      ";W Mt vs electron #eta;#",200,0.,200.,80,-8.,8.); 
  fWMtVsLeptonPhi          = new TH2D("hWMtVsLeptonPhi",
                                      ";W Mt vs electron1 #phi;#",200,0.,200.,80,-3.2,3.2);
  fMinDeltaRLeptons        = new TH1D("fMinDeltaRLeptons",";MinDeltaRLeptons;#",100,0.,5);
  fLepton1Pt               = new TH1D("hfLepton1Pt",";Lepton1Pt;#",100,0.,200);
  fLepton1Eta               = new TH1D("hLeptonEta",";LeptonEta;#",160,-8,8);
  fLepton1Phi               = new TH1D("hLeptonPhi",";LeptonPhi;#",100,-3.2,3.2);
  fLepton2Pt               = new TH1D("hfLepton2Pt",";Lepton2Pt;#",100,0.,200);
  fMetHist                 = new TH1D("hMet",";Met;#",100,0.,100);
  fNJets                   = new TH1D("hNJets",";NJets;#",16,0.,15);
  fJetPt                   = new TH1D("hJetPt",";JetPt;#",150,0.,300);
  fJetEta                  = new TH1D("hJetEta",";JetEta;#",160,-8.0,8.0);
  fJetPhi                  = new TH1D("hJetPhi",";JetPhi;#",100,-3.2,3.2);
  fMinDeltaRLeptonJet      = new TH1D("hMinDeltaRLeptonJet",";MinDeltaRLeptonJet;#",100,0.,5);

  AddOutput(fGenWMt);
  AddOutput(fWMt);
  AddOutput(fWMtBarrel);
  AddOutput(fWMtEndcap);
  AddOutput(fWMtVsLeptonEta);
  AddOutput(fWMtVsLeptonPhi);
  AddOutput(fMinDeltaRLeptons);
  AddOutput(fLepton1Pt);
  AddOutput(fLepton1Eta);
  AddOutput(fLepton1Phi);
  AddOutput(fLepton2Pt);
  AddOutput(fMetHist);
  AddOutput(fNJets);
  AddOutput(fJetPt);
  AddOutput(fJetEta);
  AddOutput(fJetPhi);
  AddOutput(fMinDeltaRLeptonJet);

}

//--------------------------------------------------------------------------------------------------
void WmunuAnalysisMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.
  cout << "Events Processed: " << fNEventsProcessed << endl;
  cout << "Events Passed Skim: " << fNEventsPassedSkim << endl;
  cout << "Events Passed LeptonSelection: " << fNEventsPassedLeptonSelection << endl;
  cout << "Events Passed KinematicPreselection: " << fNEventsPassedKinematicPreselection << endl;

}

//--------------------------------------------------------------------------------------------------
void WmunuAnalysisMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
