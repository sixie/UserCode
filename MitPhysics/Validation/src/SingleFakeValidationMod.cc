 // $Id: SingleFakeValidationMod.cc,v 1.1 2009/02/15 18:11:40 sixie Exp $

#include "MitPhysics/Validation/interface/SingleFakeValidationMod.h"
#include <TH1D.h>
#include <TH2D.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitCommon/MathTools/interface/MathUtils.h"

#include "MitPhysics/Init/interface/FakeObject.h"
#include "MitPhysics/Init/interface/FakeEventHeader.h"

using namespace mithep;
ClassImp(mithep::SingleFakeValidationMod)

//--------------------------------------------------------------------------------------------------
SingleFakeValidationMod::SingleFakeValidationMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fPrintDebug(false),
  fMCPartName(Names::gkMCPartBrn),
  fMetName(Names::gkCaloMetBrn),
  fTrackName(Names::gkTrackBrn),
  fJetName(Names::gkCaloJetBrn),
  fVertexBranchName(TString("PrimaryVertexes")),
  fCleanJetsName(ModNames::gkCleanJetsName), 
  fParticles(0),
  fMet(0),
  fTracks(0),
  fJets(0)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void SingleFakeValidationMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void SingleFakeValidationMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and  

//***********************************************************************************************
//Import Collections
//***********************************************************************************************

  //Obtain all the good objects from the event cleaning module
  ObjArray<Muon> *CleanMuons = dynamic_cast<ObjArray<Muon>* >
    (FindObjThisEvt(ModNames::gkCleanMuonsName));
  ObjArray<Electron> *CleanElectrons = dynamic_cast<ObjArray<Electron>* >
    (FindObjThisEvt(ModNames::gkCleanElectronsName));
  ObjArray<Jet> *CleanJets = dynamic_cast<ObjArray<Jet>* >
    (FindObjThisEvt(fCleanJetsName.Data()));
  mithep::ParticleOArr *CleanLeptons = dynamic_cast<mithep::ParticleOArr*>
    (FindObjThisEvt(ModNames::gkMergedLeptonsName));
  LoadBranch(fMetName);
  const Met *caloMet = fMet->At(0);

  //Obtain the collection of fake objects
  ObjArray<Particle> *ElectronFakeableObjects = dynamic_cast<ObjArray<Particle>* >
    (FindObjThisEvt(ModNames::gkElectronFakeableObjectsName));
  ObjArray<Particle> *MuonFakeableObjects = dynamic_cast<ObjArray<Particle>* >
    (FindObjThisEvt(ModNames::gkMuonFakeableObjectsName));
  ObjArray<FakeEventHeader> *FakeEventHeaders = dynamic_cast<ObjArray<FakeEventHeader>* >
    (FindObjThisEvt(ModNames::gkFakeEventHeadersName));
 
 
  //***********************************************************************************************
  //Show all Leptons and Jets
  //**********************************************************************************************
  MDB(kAnalysis, 5) {
    cout << "************************************************************************\n";
    cout << "************************************************************************\n";
    cout << "Check Leptons: " << CleanLeptons->GetEntries() << endl;
    for(UInt_t i=0; i<CleanLeptons->GetEntries(); i++)
      cout << CleanLeptons->At(i)->Pt() <<  " "  
           << CleanLeptons->At(i)->Eta() << " "  
           << CleanLeptons->At(i)->Phi() <<  " " 
           <<endl;
    cout << "Check Jets: " << CleanJets->GetEntries() << endl;
    for(UInt_t i=0; i<CleanJets->GetEntries(); i++)
      cout << CleanJets->At(i)->Pt() <<  " "  
           << CleanJets->At(i)->Eta() << " "  
           << CleanJets->At(i)->Phi() <<  " " 
           <<endl;
  }


  //***********************************************************************************************
  //Show all Fakes
  //**********************************************************************************************
  MDB(kAnalysis, 5) {
    cout << "******************************************************************\n";
    cout << "Show all Electron FakeableObjects\n";
    cout << "Number of Electron FakeableObjects : " << ElectronFakeableObjects->GetEntries() 
         << endl;
    cout << "Number of Muon FakeableObjects : " << MuonFakeableObjects->GetEntries() << endl;
    for (UInt_t i=0;i<ElectronFakeableObjects->GetEntries() ; i++) {
      cout << "ElectronFakeableObject " << i << " : " << ElectronFakeableObjects->At(i)->Pt() << " " 
           << ElectronFakeableObjects->At(i)->Eta() << " " 
           << ElectronFakeableObjects->At(i)->Phi() << endl;    
    }
    for (UInt_t i=0;i<MuonFakeableObjects->GetEntries() ; i++) {
      cout << "MuonFakeableObject " << i << " : " << MuonFakeableObjects->At(i)->Pt() << " " 
           << MuonFakeableObjects->At(i)->Eta() << " " 
           << MuonFakeableObjects->At(i)->Phi() << endl;    
    }

    cout << "******************************************************************\n";
    cout << "Show all FakeEventHeaders\n";
    cout << "Number of FakeEventHeaders : " << FakeEventHeaders->GetEntries() << endl;
    for (UInt_t i=0;i<FakeEventHeaders->GetEntries() ; i++) {
      cout << "FakeEvent " << i << " : " << FakeEventHeaders->At(i)->Weight() << " " 
           << FakeEventHeaders->At(i)->NJets() << " " << FakeEventHeaders->At(i)->FakeObjectsSize() << endl;
      
      for (UInt_t j=0;j<FakeEventHeaders->At(i)->FakeObjectsSize();j++) {
        cout << "FakeObject " << j << " ";
        if (FakeEventHeaders->At(i)->FakeObject(j)->ObjType() == kElectron) 
          cout << "e";
        else if (FakeEventHeaders->At(i)->FakeObject(j)->ObjType() == kMuon)
          cout << "mu";
        else 
          cout << "unknown";
        
        cout << " " << FakeEventHeaders->At(i)->FakeObject(j)->Pt() << " "
             << FakeEventHeaders->At(i)->FakeObject(j)->Eta() << " " 
             << FakeEventHeaders->At(i)->FakeObject(j)->Phi() << endl;
      }      
      for (UInt_t j=0;j<FakeEventHeaders->At(i)->NJets();j++) {
        cout << "Unfaked Jet " << j << " ";
        cout << " " << FakeEventHeaders->At(i)->UnfakedJet(j)->Pt() << " "
             << FakeEventHeaders->At(i)->UnfakedJet(j)->Eta() << " " 
             << FakeEventHeaders->At(i)->UnfakedJet(j)->Phi() << endl;
      }
      cout << "-----------------------------------------\n";
    }       
  }

  //reject events with already one good lepton in it. means the fake is the only lepton.
  if (CleanLeptons->GetEntries() > 0) 
    return;


  //***********************************************************************************************
  //Denominator distributions
  //***********************************************************************************************
  for (UInt_t i=0;i<ElectronFakeableObjects->GetEntries() ; i++) {

    //define transverse mass
    //use same definition as kristian
    float sin_theta = 
      ElectronFakeableObjects->At(i)->Pt() / ElectronFakeableObjects->At(i)->P();
    float e_term = ( ElectronFakeableObjects->At(i)->E()*sin_theta + caloMet->Pt() );
    float x_term = ( ElectronFakeableObjects->At(i)->Px() + caloMet->Px() );
    float y_term = ( ElectronFakeableObjects->At(i)->Py() + caloMet->Py() );
    float mt = sqrt( e_term*e_term - ( x_term*x_term + y_term*y_term));      

    //*****************************************************************************************
    //This is the framework definition. we should check that we get the same answer.
    CompositeParticle *WBoson = new CompositeParticle;
    WBoson->AddDaughter(ElectronFakeableObjects->At(i));
    WBoson->AddDaughter(caloMet);
    float WTransverseMass = WBoson->TMass();
    //*****************************************************************************************

    fElectronDenominatorEt->Fill(ElectronFakeableObjects->At(i)->Et());
    fElectronDenominatorEta->Fill(ElectronFakeableObjects->At(i)->Eta());
    fElectronDenominatorPhi->Fill(ElectronFakeableObjects->At(i)->Phi());
    fDenominatorMet->Fill(caloMet->Pt());
    fDenominatorMt->Fill(mt);
  }
  for (UInt_t i=0;i<MuonFakeableObjects->GetEntries() ; i++) {

    //define transverse mass
    //use same definition as kristian
    float sin_theta = 
      MuonFakeableObjects->At(i)->Pt() / MuonFakeableObjects->At(i)->P();
    float e_term = ( MuonFakeableObjects->At(i)->E()*sin_theta + caloMet->Pt() );
    float x_term = ( MuonFakeableObjects->At(i)->Px() + caloMet->Px() );
    float y_term = ( MuonFakeableObjects->At(i)->Py() + caloMet->Py() );
    float mt = sqrt( e_term*e_term - ( x_term*x_term + y_term*y_term));      

    //*****************************************************************************************
    //This is the framework definition. we should check that we get the same answer.
    CompositeParticle *WBoson = new CompositeParticle;
    WBoson->AddDaughter(MuonFakeableObjects->At(i));
    WBoson->AddDaughter(caloMet);
    float WTransverseMass = WBoson->TMass();
    //*****************************************************************************************

    fMuonDenominatorPt->Fill(MuonFakeableObjects->At(i)->Pt());
    fMuonDenominatorEta->Fill(MuonFakeableObjects->At(i)->Eta());
    fMuonDenominatorPhi->Fill(MuonFakeableObjects->At(i)->Phi());
  }

  //***********************************************************************************************
  //MonteCarlo Numerator distributions
  //***********************************************************************************************
  if (CleanElectrons->GetEntries() == 1) {
    for (UInt_t i=0;i<CleanElectrons->GetEntries() ; i++) {
      const Electron *ele = CleanElectrons->At(i);
      //compute d0          
      double lXM = -sin(ele->BestTrk()->Phi())*(ele->BestTrk()->D0());
      double lYM =  cos(ele->BestTrk()->Phi())*(ele->BestTrk()->D0());
      double lDX = (lXM + fVertex->At(0)->X());
      double lDY = (lYM + fVertex->At(0)->Y());
      double d0 =  (ele->BestTrk()->Px()*lDY - ele->BestTrk()->Py()*lDX)/ele->BestTrk()->Pt();
      
      //define transverse mass
      //use same definition as kristian
      float sin_theta = ele->Pt()/ele->P();
      float e_term = ( ele->E()*sin_theta + caloMet->Pt() );
      float x_term = ( ele->Px() + caloMet->Px() );
      float y_term = ( ele->Py() + caloMet->Py() );
      float mt = sqrt( e_term*e_term - ( x_term*x_term + y_term*y_term));      
      
      //*****************************************************************************************
      //This is the framework definition. we should check that we get the same answer.
      CompositeParticle *WBoson = new CompositeParticle;
      WBoson->AddDaughter(ele);
      WBoson->AddDaughter(caloMet);
      float WTransverseMass = WBoson->TMass();
      //*****************************************************************************************
            
      //apply this cut here
      if (fabs(d0) < 0.02) {
        fMCFakeMuonPt->Fill(ele->Pt());
        fMCFakeMuonEta->Fill(ele->Eta());
        fMCFakeMuonPhi->Fill(ele->Phi());
        fMCFakeMet->Fill(caloMet->Pt());
        fMCFakeMt->Fill(mt);
      }            
    }
  }


  //***********************************************************************************************
  //Distributions from applying fake rates
  //***********************************************************************************************
  for (UInt_t i=0;i<FakeEventHeaders->GetEntries() ; i++) {
    
    //only consider events with one and only one fake. (A fake W event)
    if (FakeEventHeaders->At(i)->FakeObjectsSize() == 1) {
      if (FakeEventHeaders->At(i)->FakeObject(0)->ObjType() == kElectron) {
        //*****************************************************************************************
        //Make basic W event selection
        //*****************************************************************************************
        //Add Cuts here

        //define transverse mass
        //use same definition as kristian
        float sin_theta = 
          FakeEventHeaders->At(i)->FakeObject(0)->Pt()/FakeEventHeaders->At(i)->FakeObject(0)->P();
        float e_term = ( FakeEventHeaders->At(i)->FakeObject(0)->E()*sin_theta + caloMet->Pt() );
        float x_term = ( FakeEventHeaders->At(i)->FakeObject(0)->Px() + caloMet->Px() );
        float y_term = ( FakeEventHeaders->At(i)->FakeObject(0)->Py() + caloMet->Py() );
        float mt = sqrt( e_term*e_term - ( x_term*x_term + y_term*y_term));      

        //*****************************************************************************************
        //This is the framework definition. we should check that we get the same answer.
        CompositeParticle *WBoson = new CompositeParticle;
        WBoson->AddDaughter(FakeEventHeaders->At(i)->FakeObject(0));
        WBoson->AddDaughter(caloMet);
        float WTransverseMass = WBoson->TMass();
        //*****************************************************************************************
        fFakeElectronEt->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Et(),
                              FakeEventHeaders->At(i)->Weight());
        fFakeElectronEta->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Eta(),
                              FakeEventHeaders->At(i)->Weight());
        fFakeElectronPhi->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Phi(),
                              FakeEventHeaders->At(i)->Weight());
        fFakeMet->Fill(caloMet->Pt(), FakeEventHeaders->At(i)->Weight());
        fFakeMt->Fill(mt,  FakeEventHeaders->At(i)->Weight());
      }
      if (FakeEventHeaders->At(i)->FakeObject(0)->ObjType() == kMuon) {
        //*****************************************************************************************
        //Make basic W event selection
        //*****************************************************************************************
        //Add Cuts here

        //define transverse mass
        //use same definition as kristian
        float sin_theta = 
          FakeEventHeaders->At(i)->FakeObject(0)->Pt()/FakeEventHeaders->At(i)->FakeObject(0)->P();
        float e_term = ( FakeEventHeaders->At(i)->FakeObject(0)->E()*sin_theta + caloMet->Pt() );
        float x_term = ( FakeEventHeaders->At(i)->FakeObject(0)->Px() + caloMet->Px() );
        float y_term = ( FakeEventHeaders->At(i)->FakeObject(0)->Py() + caloMet->Py() );
        float mt = sqrt( e_term*e_term - ( x_term*x_term + y_term*y_term));      

        //*****************************************************************************************
        //This is the framework definition. we should check that we get the same answer.
        CompositeParticle *WBoson = new CompositeParticle;
        WBoson->AddDaughter(FakeEventHeaders->At(i)->FakeObject(0));
        WBoson->AddDaughter(caloMet);
        float WTransverseMass = WBoson->TMass();
        //*****************************************************************************************
        fFakeMuonPt->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Pt(),
                              FakeEventHeaders->At(i)->Weight());
        fFakeMuonEta->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Eta(),
                              FakeEventHeaders->At(i)->Weight());
        fFakeMuonPhi->Fill(FakeEventHeaders->At(i)->FakeObject(0)->Phi(),
                              FakeEventHeaders->At(i)->Weight());
        fFakeMet->Fill(caloMet->Pt(), FakeEventHeaders->At(i)->Weight());
        fFakeMt->Fill(mt,  FakeEventHeaders->At(i)->Weight());
      }
    
    }
  }

  return;

}
//--------------------------------------------------------------------------------------------------
void SingleFakeValidationMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fMetName, fMet);
  ReqBranch(fMCPartName, fParticles);
  ReqBranch(fTrackName,  fTracks);
  ReqBranch(fJetName,    fJets);
  ReqBranch(fMuonName,   fMuons);
  ReqBranch(fVertexBranchName, fVertex);


//*************************************************************************************************
// Selection Histograms
//*************************************************************************************************  
  AddTH1(fElectronDenominatorEt, "hElectronDenominatorEt", 
         ";ElectronDenominatorEt;Number of Events", 50, 0, 250);
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
  AddTH1(fDenominatorMt,"hDenominatorMt", ";DenominatorMt;Number of Events", 50, 0, 250);
  AddTH1(fDenominatorMet,"hDenominatorMet", ";DenominatorMet;Number of Events", 50, 0, 250);


  AddTH1(fFakeElectronEt, "hFakeElectronEt", ";FakeElectronEt;Number of Events", 50, 0, 250);
  AddTH1(fFakeElectronEta, "hFakeElectronEta", ";FakeElectronEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fFakeElectronPhi, "hFakeElectronPhi", ";FakeElectronPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fFakeMuonPt, "hFakeMuonPt", ";FakeMuonPt;Number of Events", 50, 0, 250);
  AddTH1(fFakeMuonEta, "hFakeMuonEta", ";FakeMuonEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fFakeMuonPhi, "hFakeMuonPhi", ";FakeMuonPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fFakeMt,"hFakeMt", ";FakeMt;Number of Events", 50, 0, 250);
  AddTH1(fFakeMet,"hFakeMet", ";FakeMet;Number of Events", 50, 0, 250);


  AddTH1(fMCFakeElectronEt, "hMCFakeElectronEt", 
         ";MCFakeElectronEt;Number of Events", 50, 0, 250);
  AddTH1(fMCFakeElectronEta, "hMCFakeElectronEta", 
         ";MCFakeElectronEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMCFakeElectronPhi, "hMCFakeElectronPhi", 
         ";MCFakeElectronPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fMCFakeMuonPt, "hMCFakeMuonPt", ";MCFakeMuonPt;Number of Events", 50, 0, 250);
  AddTH1(fMCFakeMuonEta, "hMCFakeMuonEta", ";MCFakeMuonEta;Number of Events", 100, -5.0, 5.0);
  AddTH1(fMCFakeMuonPhi, "hMCFakeMuonPhi", ";MCFakeMuonPhi;Number of Events", 100, -3.2, 3.2);
  AddTH1(fMCFakeMt,"hMCFakeMt", ";MCFake_Mt;Number of Events", 50, 0, 250);
  AddTH1(fMCFakeMet,"hMCFakeMet", ";MCFake_Met;Number of Events", 50, 0, 250);
}

//--------------------------------------------------------------------------------------------------
void SingleFakeValidationMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void SingleFakeValidationMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
