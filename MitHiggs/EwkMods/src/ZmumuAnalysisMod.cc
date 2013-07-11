// $Id: ZmumuAnalysisMod.cc,v 1.4 2008/12/04 19:19:49 sixie Exp $

#include "MitHiggs/EwkMods/interface/ZmumuAnalysisMod.h"
#include <TH1D.h>
#include <TH2D.h>
#include "MitAna/DataTree/interface/Names.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitCommon/MathTools/interface/MathUtils.h"

using namespace mithep;

ClassImp(mithep::ZmumuAnalysisMod)

//--------------------------------------------------------------------------------------------------
ZmumuAnalysisMod::ZmumuAnalysisMod(const char *name, const char *title) : 
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
void ZmumuAnalysisMod::Begin()
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
void ZmumuAnalysisMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and
  //output debug info or not  
  bool printDebug = false; 

  //Obtain generator objects
  ObjArray<MCParticle> *GenLeptons = dynamic_cast<ObjArray<MCParticle>* >(FindObjThisEvt(fGenLeptonsName.c_str()));

 //Obtain all the good objects from the event cleaning module
  //ObjArray<Electron> *GoodElectrons = dynamic_cast<ObjArray<Electron>* >(FindObjThisEvt(fGoodElectronsName.c_str()));
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
  
  //make sure 2nd highest pt lepton has Pt > 5
  if (muons.size() < 2 || muons[1]->Pt() <= 5 ) return;
  fNEventsPassedLeptonSelection++;

  //make sure the 3rd highest pt lepton has pt <= 10.
  //if (muons.size() >= 3 && muons[2]->Pt() > 10) return;
  fNEventsPassedKinematicPreselection++;   
    
  if (!(GenLeptons->GetEntries() >= 2 && GenLeptons->At(0)->AbsPdgId() == 13 && GenLeptons->At(1)->AbsPdgId() == 13)) {
    cerr << "NOT ZEE EVENT!!!!!! WEIRD!!!!" << endl;
    return;
  }

  MCParticle *GenMuon1 = new MCParticle(GenLeptons->At(0)->Px(),GenLeptons->At(0)->Py(),GenLeptons->At(0)->Pz(),GenLeptons->At(0)->E(),11,1);
  MCParticle *GenMuon2 = new MCParticle(GenLeptons->At(1)->Px(),GenLeptons->At(1)->Py(),GenLeptons->At(1)->Pz(),GenLeptons->At(1)->E(),11,1);

  CompositeParticle *Z = new CompositeParticle();
  CompositeParticle *GenZ = new CompositeParticle();
 
   Z->AddDaughter(muons[0]);
   Z->AddDaughter(muons[1]);
   GenZ->AddDaughter(GenMuon1);
   GenZ->AddDaughter(GenMuon2);
   fGenZMass->Fill(GenZ->Mass());

   double p = TMath::Sqrt( muons[0]->E()*muons[0]->E() - muons[0]->Mass()*muons[0]->Mass());
   FourVector e1( p*sin(muons[0]->Trk()->Theta())*cos(muons[0]->Trk()->Phi()), p*sin(muons[0]->Trk()->Theta())*sin(muons[0]->Trk()->Phi()), p*cos(muons[0]->Trk()->Theta()), muons[0]->E() );
   
   p = TMath::Sqrt( muons[1]->E()*muons[1]->E() - muons[1]->Mass()*muons[1]->Mass());
   FourVector e2( p*sin(muons[1]->Trk()->Theta())*cos(muons[1]->Trk()->Phi()), p*sin(muons[1]->Trk()->Theta())*sin(muons[1]->Trk()->Phi()), p*cos(muons[1]->Trk()->Theta()), muons[1]->E() );
 
   fZMass->Fill(Z->Mass());
   //fZMass->Fill(TMath::Sqrt((e1+e2).M2()));


   if(muons[0]->Eta() < 1.5 && muons[1]->Eta() < 1.5) fZMassBB->Fill(Z->Mass());
   else if(muons[0]->Eta() > 1.5 && muons[1]->Eta() > 1.5) fZMassEE->Fill(Z->Mass());
   else fZMassEB->Fill(Z->Mass());

   fZMassVsZEta->Fill(Z->Mass(), Z->Eta());
   fZMassVsLeptonEta->Fill(Z->Mass(), muons[0]->Eta());
   fZMassVsLeptonPhi->Fill(Z->Mass(), muons[0]->Phi());
   fDileptonDeltaEta->Fill(fabs(muons[0]->Eta() - muons[1]->Eta()));
   fDileptonDeltaPhi->Fill(MathUtils::DeltaPhi(muons[0]->Phi(),muons[1]->Phi()));
   fLepton1Pt->Fill(muons[0]->Pt());
   fLepton2Pt->Fill(muons[1]->Pt());
   fLeptonEta->Fill(muons[0]->Eta()); fLeptonEta->Fill(muons[1]->Eta());
   fLeptonPhi->Fill(muons[0]->Phi()); fLeptonPhi->Fill(muons[1]->Phi());
   fMetHist->Fill(muonCorrectedMet->Pt());
   fNJets->Fill(GoodCentralJets->GetEntries()+GoodForwardJets->GetEntries());

   double minDRLeptonJet = 5000.0;
   for (UInt_t j=0; j<GoodCentralJets->GetEntries(); j++) {
     fJetPt->Fill(GoodCentralJets->At(j)->Pt());
     fJetEta->Fill(GoodCentralJets->At(j)->Eta());
     fJetPhi->Fill(GoodCentralJets->At(j)->Phi());
     if (MathUtils::DeltaR(GoodCentralJets->At(j)->Mom(),muons[0]->Mom()) < minDRLeptonJet)
       minDRLeptonJet = MathUtils::DeltaR(GoodCentralJets->At(j)->Mom(),muons[0]->Mom());
     if (MathUtils::DeltaR(GoodCentralJets->At(j)->Mom(),muons[1]->Mom()) < minDRLeptonJet)
       minDRLeptonJet = MathUtils::DeltaR(GoodCentralJets->At(j)->Mom(),muons[1]->Mom());
   }
   for (UInt_t j=0; j<GoodForwardJets->GetEntries(); j++) {
     fJetPt->Fill(GoodForwardJets->At(j)->Pt());
     fJetEta->Fill(GoodForwardJets->At(j)->Eta());
     fJetPhi->Fill(GoodForwardJets->At(j)->Phi());
     if (MathUtils::DeltaR(GoodForwardJets->At(j)->Mom(),muons[0]->Mom()) < minDRLeptonJet)
       minDRLeptonJet = MathUtils::DeltaR(GoodForwardJets->At(j)->Mom(),muons[0]->Mom());
     if (MathUtils::DeltaR(GoodForwardJets->At(j)->Mom(),muons[1]->Mom()) < minDRLeptonJet)
       minDRLeptonJet = MathUtils::DeltaR(GoodForwardJets->At(j)->Mom(),muons[1]->Mom());
   }

   if (minDRLeptonJet < 5000.0)
     fMinDeltaRLeptonJet->Fill(minDRLeptonJet);

   if (Z->Mass() < 10 ) {
    //print out Generator BLock       
       LoadBranch(fMCPartName);
       cout << "Event Dump: " << fNEventsProcessed << endl;
       
       //print out event content to text
       cout << "Muons" << endl;
       for (UInt_t i = 0; i < GoodMuons->GetEntries(); i++) {
	 cout << i << " " << GoodMuons->At(i)->Pt() << " " << GoodMuons->At(i)->Eta() 
              << " " << GoodMuons->At(i)->Phi() << endl;
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
void ZmumuAnalysisMod::SlaveBegin()
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

  fGenZMass                = new TH1D("hGenZmass",";GenZmass;#",200,0.,200.);
  fZMass                   = new TH1D("hZmass",";Zmass;#",200,0.,200.);
  fZMassBB                 = new TH1D("hZmassBB",";ZmassBB;#",200,0.,200.);
  fZMassEB                 = new TH1D("hZmassEB",";ZmassEB;#",200,0.,200.);
  fZMassEE                 = new TH1D("hZmassEE",";ZmassEE;#",200,0.,200.);
  fZMassVsZEta             = new TH2D("hZmassVsZEta",";Zmass vs Z #eta;#",200,0.,200.,80,-8.,8.); 
  fZMassVsLeptonEta        = new TH2D("hZmassVsLeptonEta",
                                      ";Zmass vs muon1 #eta;#",200,0.,200.,80,-8.,8.); 
  fZMassVsLeptonPhi        = new TH2D("hZmassVsLeptonPhi",
                                      ";Zmass vs muon1 #phi;#",200,0.,200.,80,-3.2,3.2);
  fDileptonDeltaEta        = new TH1D("hDileptonDeltaEta",";DileptonDeltaEta;#",100,0.,5);
  fDileptonDeltaPhi        = new TH1D("hDileptonDeltaPhi",";DileptonDeltaPhi;#",100,0.,3.2);
  fLepton1Pt               = new TH1D("hfLepton1Pt",";Lepton1Pt;#",100,0.,200);
  fLepton2Pt               = new TH1D("hfLepton2Pt",";Lepton2Pt;#",100,0.,200);
  fLeptonEta               = new TH1D("hLeptonEta",";LeptonEta;#",160,-8,8);
  fLeptonPhi               = new TH1D("hLeptonPhi",";LeptonPhi;#",100,-3.2,3.2);
  fMetHist                 = new TH1D("hMet",";Met;#",100,0.,50);
  fNJets                   = new TH1D("hNJets",";NJets;#",16,0.,15);
  fJetPt                   = new TH1D("hJetPt",";JetPt;#",150,0.,300);
  fJetEta                  = new TH1D("hJetEta",";JetEta;#",160,-8.0,8.0);
  fJetPhi                  = new TH1D("hJetPhi",";JetPhi;#",100,-3.2,3.2);
  fMinDeltaRLeptonJet      = new TH1D("hMinDeltaRLeptonJet",";MinDeltaRLeptonJet;#",100,0.,5);

  AddOutput(fGenZMass);
  AddOutput(fZMass);
  AddOutput(fZMassBB);
  AddOutput(fZMassEB);
  AddOutput(fZMassEE);
  AddOutput(fZMassVsZEta);
  AddOutput(fZMassVsLeptonEta);
  AddOutput(fZMassVsLeptonPhi);
  AddOutput(fDileptonDeltaEta);
  AddOutput(fDileptonDeltaPhi);
  AddOutput(fLepton1Pt);
  AddOutput(fLepton2Pt);
  AddOutput(fLeptonEta);
  AddOutput(fLeptonPhi);
  AddOutput(fMetHist);
  AddOutput(fNJets);
  AddOutput(fJetPt);
  AddOutput(fJetEta);
  AddOutput(fJetPhi);
  AddOutput(fMinDeltaRLeptonJet);
}

//--------------------------------------------------------------------------------------------------
void ZmumuAnalysisMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.
  cout << "Events Processed: " << fNEventsProcessed << endl;
  cout << "Events Passed Skim: " << fNEventsPassedSkim << endl;
  cout << "Events Passed LeptonSelection: " << fNEventsPassedLeptonSelection << endl;
  cout << "Events Passed KinematicPreselection: " << fNEventsPassedKinematicPreselection << endl;

}

//--------------------------------------------------------------------------------------------------
void ZmumuAnalysisMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
