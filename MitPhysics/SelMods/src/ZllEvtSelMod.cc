// $Id: ZllEvtSelMod.cc,v 1.1 2008/11/12 15:04:50 sixie Exp $

#include "MitPhysics/SelMods/interface/ZllEvtSelMod.h"
#include <TH1D.h>
#include <TH2D.h>
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitCommon/MathTools/interface/MathUtils.h"

using namespace mithep;
ClassImp(mithep::ZllEvtSelMod)

//--------------------------------------------------------------------------------------------------
ZllEvtSelMod::ZllEvtSelMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fPrintDebug(false),
  fMetName(Names::gkCaloMetBrn),
  fTrackName(Names::gkTrackBrn),
  fCaloTowerName(Names::gkCaloTowerBrn),
  fCleanJetsName(Names::gkCleanJetsName),
  fMCLeptonsName(Names::gkMCLeptonsName),
  fMet(0),
  fNEventsProcessed(0)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void ZllEvtSelMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void ZllEvtSelMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and  
  fNEventsProcessed++;

  if (fNEventsProcessed % 1000000 == 0 || fPrintDebug) {
    time_t systime;
    systime = time(NULL);
    cerr << endl << "ZllEvtSelMod : Process Event " << fNEventsProcessed << "  Time: " << ctime(&systime) << endl;
  }

  //Obtain all the good objects from the event cleaning module
  ObjArray<Electron> *CleanElectrons = dynamic_cast<ObjArray<Electron>* >
    (FindObjThisEvt(Names::gkCleanElectronsName));
  ObjArray<Muon> *CleanMuons = dynamic_cast<ObjArray<Muon>* >
    (FindObjThisEvt(Names::gkCleanMuonsName));
  ObjArray<Jet> *CleanJets = dynamic_cast<ObjArray<Jet>* >
    (FindObjThisEvt(fCleanJetsName.Data()));

  //Get Generator Level information for matching
  //ObjArray<MCParticle> *GenLeptons = 
  //  dynamic_cast<ObjArray<MCParticle>*> (FindObjThisEvt(fMCLeptonsName.Data()));

  const TriggerObjectCol *objs = GetHLTObjects(fObjsName);

  if (!objs){
    //printf("TriggerObjectCol not found\n");
    //return;
  }

  LoadBranch(fMetName);
  Met *caloMet = fMet->At(0);

  vector<ChargedParticle*> leptons;
  vector<string> leptonType; 
  double zAverage = 0.0;

  // Make lepton vector from muons and electrons
  for (UInt_t j=0; j<CleanMuons->GetEntries(); j++) {
    if(CleanMuons->At(j)->Pt() <= 15) continue;
    zAverage = zAverage + CleanMuons->At(j)->BestTrk()->Z0();
    leptons.push_back(CleanMuons->At(j));
    leptonType.push_back("mu");
  }
  for (UInt_t j=0; j<CleanElectrons->GetEntries(); j++) {   
    if(CleanElectrons->At(j)->Pt() <= 15) continue;
    zAverage = zAverage + CleanElectrons->At(j)->BestTrk()->Z0();
    leptons.push_back(CleanElectrons->At(j));
    leptonType.push_back("e");
  }

  // Computing Z average (our primary vertex)
  if(leptons.size() > 0) zAverage = zAverage / leptons.size();

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

  // Minimun Pt, Nleptons>=2 requirements
  if (leptons.size() >= 2 &&
      leptons[0]->Pt() > 20 && leptons[1]->Pt() > 15){

    CompositeParticle *dilepton = new CompositeParticle();
    dilepton->AddDaughter(leptons[0]);
    dilepton->AddDaughter(leptons[1]);

    // Sort and count the number of central Jets for vetoing
    int nCentralJets = 0;
    for(UInt_t i=0; i<CleanJets->GetEntries(); i++){
      if(fabs(CleanJets->At(i)->Eta()) < 2.5){
        nCentralJets++;
      }
    }

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

    // HLT study
    if(objs){
      Int_t ents=objs->GetEntries();
      double deltaRleptonHLT[2] = {999., 999.};
      for(Int_t i=0;i<ents;++i) {
         const TriggerObject* to = objs->At(i);

         if(strcmp(to->TrigName(),"HLT_IsoEle15_L1I")==0)	 hDZllHLT[0+100*pairType]->Fill(0.);
         if(strcmp(to->TrigName(),"HLT_Ele15_SW_L1R")==0)	 hDZllHLT[0+100*pairType]->Fill(1.);
         if(strcmp(to->TrigName(),"HLT_IsoEle15_LW_L1I")==0)	 hDZllHLT[0+100*pairType]->Fill(2.);
         if(strcmp(to->TrigName(),"HLT_IsoEle18_L1R")==0)	 hDZllHLT[0+100*pairType]->Fill(3.);
         if(strcmp(to->TrigName(),"HLT_Ele15_LW_L1R")==0)	 hDZllHLT[0+100*pairType]->Fill(4.);
         if(strcmp(to->TrigName(),"HLT_LooseIsoEle15_LW_L1R")==0)hDZllHLT[0+100*pairType]->Fill(5.);
         if(strcmp(to->TrigName(),"HLT_IsoMu15")==0)		 hDZllHLT[0+100*pairType]->Fill(6.);
         if(strcmp(to->TrigName(),"HLT_Mu15")==0)		 hDZllHLT[0+100*pairType]->Fill(7.);
         if(strcmp(to->TrigName(),"HLT_IsoEle15_L1I")!=0 &&
    	    strcmp(to->TrigName(),"HLT_Ele15_SW_L1R")!=0 &&
            strcmp(to->TrigName(),"HLT_IsoEle15_LW_L1I")!=0 &&
            strcmp(to->TrigName(),"HLT_IsoEle18_L1R")!=0 &&
            strcmp(to->TrigName(),"HLT_Ele15_LW_L1R")!=0 &&
            strcmp(to->TrigName(),"HLT_LooseIsoEle15_LW_L1R")!=0 &&
            strcmp(to->TrigName(),"HLT_IsoMu15")!=0 &&
            strcmp(to->TrigName(),"HLT_Mu15")!=0)		 hDZllHLT[0+100*pairType]->Fill(8.);

        for(int nl=0; nl<2; nl++){
          if     (leptonType[nl] == "mu" && to->Type() == TriggerObject::TriggerMuon){
            double DeltaRAux = MathUtils::DeltaR(leptons[nl]->Eta(), leptons[nl]->Phi(),
        					 to->Eta(),to->Phi());
    	    if(DeltaRAux < deltaRleptonHLT[nl]) deltaRleptonHLT[nl] = DeltaRAux;
          }
          else if(leptonType[nl] == "e" && to->Type() == TriggerObject::TriggerElectron){
            double DeltaRAux = MathUtils::DeltaR(leptons[nl]->Eta(), leptons[nl]->Phi(),
        					 to->Eta(),to->Phi());
    	    if(DeltaRAux < deltaRleptonHLT[nl]) deltaRleptonHLT[nl] = DeltaRAux;
          }
        }
      } // Loop over HLT objects
      for(int nl=0; nl<2; nl++){
        if     (leptonType[nl] == "mu")
          hDZllHLT[1+100*pairType]->Fill(TMath::Min(deltaRleptonHLT[nl],0.999));
        else if(leptonType[nl] == "e")
          hDZllHLT[2+100*pairType]->Fill(TMath::Min(deltaRleptonHLT[nl],0.999));
      }
      if      (deltaRleptonHLT[0] < 0.1 && deltaRleptonHLT[1] < 0.1)
        hDZllHLT[3+100*pairType]->Fill(0.0);
      else if(deltaRleptonHLT[0] < 0.1 || deltaRleptonHLT[1] < 0.1)
        hDZllHLT[3+100*pairType]->Fill(1.0);
      else
        hDZllHLT[3+100*pairType]->Fill(2.0);
    } // if !objs

    hDZllMET[ 0+100*pairType]->Fill((double)leptons.size());
    if(leptons.size() == 2 && dilepton->Charge() == 0)
          hDZllMET[ 1+100*pairType]->Fill(TMath::Min(dilepton->Mass(),299.999));
    // No more than 2 isolated good leptons
    if(leptons.size() == 2 && dilepton->Charge() == 0 &&
       fabs(dilepton->Mass()-91.1876) < 30){

      // MET study for nCentralJets == 0
      if(nCentralJets == 0){
        double metDif[2] = {caloMet->Px() + dilepton->Px(),
	                    caloMet->Py() + dilepton->Py()};
        hDZllMET[ 2+100*pairType]->Fill(metDif[0]);
        hDZllMET[ 3+100*pairType]->Fill(metDif[1]);
        hDZllMET[ 4+100*pairType]->Fill(caloMet->Pt() - dilepton->Pt());
        hDZllMET[ 5+100*pairType]->Fill(metDif[0]/caloMet->Px());
        hDZllMET[ 6+100*pairType]->Fill(metDif[1]/caloMet->Py());
        hDZllMET[ 7+100*pairType]->Fill((caloMet->Pt() - dilepton->Pt())/caloMet->Pt());
        hDZllMET[ 8+100*pairType]->Fill(MathUtils::DeltaPhi(caloMet->Phi(), 
  				   atan2(-dilepton->Py(), -dilepton->Px())) 
				   * 180./TMath::Pi());
        hDZllMET[ 9+100*pairType]->Fill(caloMet->Pt());
        hDZllMET[10+100*pairType]->Fill(dilepton->Pt());
      }

      hDZllMET[11+100*pairType]->Fill((double)nCentralJets);
      hDZllMET[12+100*pairType]->Fill(leptons[0]->Pt());
      hDZllMET[13+100*pairType]->Fill(leptons[1]->Pt());
      hDZllMET[14+100*pairType]->Fill(caloMet->MetSig());
      hDZllMET[15+100*pairType]->Fill(caloMet->SumEt());
      
      // Isolation studies
      LoadBranch(fTrackName);
      LoadBranch(fCaloTowerName);
      double theEta = RND.Rndm()*5.-2.5;
      double thePhi = RND.Rndm()*2.0*TMath::Pi()-TMath::Pi();
      if(theEta < -2.5         || theEta > 2.5 ||
         thePhi < -TMath::Pi() || thePhi > TMath::Pi())
	   printf("Problem, eta(%f), phi (%f)\n",theEta,thePhi);

      double sumPt = 0;
      int nTracks = 0;
      getTrackIsolation(theEta, thePhi, leptons, fTracks, sumPt, nTracks);
      double lIso[4] = {0.0, 0.0, 0.0, 0.0};
      getCaloIsolation(theEta, thePhi, leptons, fCaloTowers, lIso);

      if(nCentralJets == 0){
        hDZllIso[ 0+100*pairType]->Fill(TMath::Min(sumPt,9.999));
        hDZllIso[ 1+100*pairType]->Fill((double)nTracks);
        hDZllIso[ 2+100*pairType]->Fill(TMath::Min(lIso[0],9.999));
        hDZllIso[ 3+100*pairType]->Fill(TMath::Min(lIso[1],9.999));
        hDZllIso[ 4+100*pairType]->Fill(TMath::Min(lIso[2],9.999));
        hDZllIso[ 5+100*pairType]->Fill(TMath::Min(lIso[3],9.999));
        hDZllIsoEtaIsoTrack0->Fill(theEta,sumPt);
        hDZllIsoEtaIsoNTrack0->Fill(theEta,(double)nTracks);
        hDZllIsoEtaIsoCaloAll0->Fill(theEta,lIso[0]);
        hDZllIsoEtaIsoEem0->Fill(theEta,lIso[1]);
        hDZllIsoEtaIsoHcal0->Fill(theEta,lIso[2]);   
        hDZllIsoEtaIsoHo0->Fill(theEta,lIso[3]);
      }    
      hDZllIso[ 6+100*pairType]->Fill(TMath::Min(sumPt,9.999));
      hDZllIso[ 7+100*pairType]->Fill((double)nTracks);
      hDZllIso[ 8+100*pairType]->Fill(TMath::Min(lIso[0],9.999));
      hDZllIso[ 9+100*pairType]->Fill(TMath::Min(lIso[1],9.999));
      hDZllIso[10+100*pairType]->Fill(TMath::Min(lIso[2],9.999));
      hDZllIso[11+100*pairType]->Fill(TMath::Min(lIso[3],9.999));
      hDZllIsoEtaIsoTrack->Fill(theEta,sumPt);
      hDZllIsoEtaIsoNTrack->Fill(theEta,(double)nTracks);
      hDZllIsoEtaIsoCaloAll->Fill(theEta,lIso[0]);
      hDZllIsoEtaIsoEem->Fill(theEta,lIso[1]);
      hDZllIsoEtaIsoHcal->Fill(theEta,lIso[2]);   
      hDZllIsoEtaIsoHo->Fill(theEta,lIso[3]);

    } // Nleptons == 2 , Ncharge == 0 and fabs(mll-mz)<30 requirements
    delete dilepton;
  } // Minimun Pt and Nleptons >= 2 requirements 
  leptons.clear();
  delete objs;
}
//--------------------------------------------------------------------------------------------------
void ZllEvtSelMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fMetName,       fMet);
  ReqBranch(fTrackName,     fTracks);
  ReqBranch(fCaloTowerName, fCaloTowers);

  char sb[200];
  for(int j=0; j<3; j++){
    int ind = 100 * j;
    sprintf(sb,"hDZllMET_%d",ind+0);  hDZllMET[ind+0]  = new TH1D(sb,sb,10,-0.5,9.5);
    sprintf(sb,"hDZllMET_%d",ind+1);  hDZllMET[ind+1]  = new TH1D(sb,sb,150,0.0,300.);
    sprintf(sb,"hDZllMET_%d",ind+2);  hDZllMET[ind+2]  = new TH1D(sb,sb,100,-50.0,50.);
    sprintf(sb,"hDZllMET_%d",ind+3);  hDZllMET[ind+3]  = new TH1D(sb,sb,100,-50.0,50.);
    sprintf(sb,"hDZllMET_%d",ind+4);  hDZllMET[ind+4]  = new TH1D(sb,sb,100,-50.0,50.);
    sprintf(sb,"hDZllMET_%d",ind+5);  hDZllMET[ind+5]  = new TH1D(sb,sb,100,-2.0,2.);
    sprintf(sb,"hDZllMET_%d",ind+6);  hDZllMET[ind+6]  = new TH1D(sb,sb,100,-2.0,2.);
    sprintf(sb,"hDZllMET_%d",ind+7);  hDZllMET[ind+7]  = new TH1D(sb,sb,100,-2.0,2.);
    sprintf(sb,"hDZllMET_%d",ind+8);  hDZllMET[ind+8]  = new TH1D(sb,sb,90,0.0,180.);
    sprintf(sb,"hDZllMET_%d",ind+9);  hDZllMET[ind+9]  = new TH1D(sb,sb,150,0.0,300.);
    sprintf(sb,"hDZllMET_%d",ind+10); hDZllMET[ind+10] = new TH1D(sb,sb,150,0.0,300.); 
    sprintf(sb,"hDZllMET_%d",ind+11); hDZllMET[ind+11] = new TH1D(sb,sb,10,-0.5,9.5);
    sprintf(sb,"hDZllMET_%d",ind+12); hDZllMET[ind+12] = new TH1D(sb,sb,150,0.0,300.);
    sprintf(sb,"hDZllMET_%d",ind+13); hDZllMET[ind+13] = new TH1D(sb,sb,150,0.0,300.); 
    sprintf(sb,"hDZllMET_%d",ind+14); hDZllMET[ind+14] = new TH1D(sb,sb,150,0.0,30.);
    sprintf(sb,"hDZllMET_%d",ind+15); hDZllMET[ind+15] = new TH1D(sb,sb,200,0.0,800.);
  }

  for(int i=0; i<16; i++){
    for(int j=0; j<3; j++){
      AddOutput(hDZllMET[i+j*100]);
    }
  }

  for(int j=0; j<3; j++){
    int ind = 100 * j;
    sprintf(sb,"hDZllIso_%d",ind+ 0); hDZllIso[ind+ 0] = new TH1D(sb,sb,80,0.0,10.);
    sprintf(sb,"hDZllIso_%d",ind+ 1); hDZllIso[ind+ 1] = new TH1D(sb,sb,20,-0.5,19.5);
    sprintf(sb,"hDZllIso_%d",ind+ 2); hDZllIso[ind+ 2] = new TH1D(sb,sb,80,0.0,10.);
    sprintf(sb,"hDZllIso_%d",ind+ 3); hDZllIso[ind+ 3] = new TH1D(sb,sb,80,0.0,10.);
    sprintf(sb,"hDZllIso_%d",ind+ 4); hDZllIso[ind+ 4] = new TH1D(sb,sb,80,0.0,10.);
    sprintf(sb,"hDZllIso_%d",ind+ 5); hDZllIso[ind+ 5] = new TH1D(sb,sb,80,0.0,10.);
    sprintf(sb,"hDZllIso_%d",ind+ 6); hDZllIso[ind+ 6] = new TH1D(sb,sb,80,0.0,10.);
    sprintf(sb,"hDZllIso_%d",ind+ 7); hDZllIso[ind+ 7] = new TH1D(sb,sb,20,-0.5,19.5);
    sprintf(sb,"hDZllIso_%d",ind+ 8); hDZllIso[ind+ 8] = new TH1D(sb,sb,80,0.0,10.);
    sprintf(sb,"hDZllIso_%d",ind+ 9); hDZllIso[ind+ 9] = new TH1D(sb,sb,80,0.0,10.);
    sprintf(sb,"hDZllIso_%d",ind+10); hDZllIso[ind+10] = new TH1D(sb,sb,80,0.0,10.);
    sprintf(sb,"hDZllIso_%d",ind+11); hDZllIso[ind+11] = new TH1D(sb,sb,80,0.0,10.);
  }

  for(int i=0; i<12; i++){
    for(int j=0; j<3; j++){
      AddOutput(hDZllIso[i+j*100]);
    }
  }

  sprintf(sb,"hDZllIsoEtaIsoTrack0");   hDZllIsoEtaIsoTrack0   = new TH2D(sb,sb,20,-2.5,2.5,80,0,10);  
  sprintf(sb,"hDZllIsoEtaIsoNTrack0");  hDZllIsoEtaIsoNTrack0  = new TH2D(sb,sb,20,-2.5,2.5,20,-0.5,19.5);  
  sprintf(sb,"hDZllIsoEtaIsoCaloAll0"); hDZllIsoEtaIsoCaloAll0 = new TH2D(sb,sb,20,-2.5,2.5,80,0,10);  
  sprintf(sb,"hDZllIsoEtaIsoEem0");     hDZllIsoEtaIsoEem0     = new TH2D(sb,sb,20,-2.5,2.5,80,0,10);  
  sprintf(sb,"hDZllIsoEtaIsoHcal0");    hDZllIsoEtaIsoHcal0    = new TH2D(sb,sb,20,-2.5,2.5,80,0,10);  
  sprintf(sb,"hDZllIsoEtaIsoHo0");      hDZllIsoEtaIsoHo0      = new TH2D(sb,sb,20,-2.5,2.5,80,0,10);  
  AddOutput(hDZllIsoEtaIsoTrack0);
  AddOutput(hDZllIsoEtaIsoNTrack0);
  AddOutput(hDZllIsoEtaIsoCaloAll0);
  AddOutput(hDZllIsoEtaIsoEem0);
  AddOutput(hDZllIsoEtaIsoHcal0);
  AddOutput(hDZllIsoEtaIsoHo0);
  sprintf(sb,"hDZllIsoEtaIsoTrack");   hDZllIsoEtaIsoTrack   = new TH2D(sb,sb,20,-2.5,2.5,80,0,10);  
  sprintf(sb,"hDZllIsoEtaIsoNTrack");  hDZllIsoEtaIsoNTrack  = new TH2D(sb,sb,20,-2.5,2.5,20,-0.5,19.5);  
  sprintf(sb,"hDZllIsoEtaIsoCaloAll"); hDZllIsoEtaIsoCaloAll = new TH2D(sb,sb,20,-2.5,2.5,80,0,10);  
  sprintf(sb,"hDZllIsoEtaIsoEem");     hDZllIsoEtaIsoEem     = new TH2D(sb,sb,20,-2.5,2.5,80,0,10);  
  sprintf(sb,"hDZllIsoEtaIsoHcal");    hDZllIsoEtaIsoHcal    = new TH2D(sb,sb,20,-2.5,2.5,80,0,10);  
  sprintf(sb,"hDZllIsoEtaIsoHo");      hDZllIsoEtaIsoHo      = new TH2D(sb,sb,20,-2.5,2.5,80,0,10);  
  AddOutput(hDZllIsoEtaIsoTrack);
  AddOutput(hDZllIsoEtaIsoNTrack);
  AddOutput(hDZllIsoEtaIsoCaloAll);
  AddOutput(hDZllIsoEtaIsoEem);
  AddOutput(hDZllIsoEtaIsoHcal);
  AddOutput(hDZllIsoEtaIsoHo);

  for(int j=0; j<3; j++){
    int ind = 100 * j;
    sprintf(sb,"hDZllHLT_%d",ind+0);  hDZllHLT[ind+0]  = new TH1D(sb,sb,9,-0.5,8.5);
    sprintf(sb,"hDZllHLT_%d",ind+1);  hDZllHLT[ind+1]  = new TH1D(sb,sb,100,0.0,1.0);
    sprintf(sb,"hDZllHLT_%d",ind+2);  hDZllHLT[ind+2]  = new TH1D(sb,sb,100,0.0,1.0);
    sprintf(sb,"hDZllHLT_%d",ind+3);  hDZllHLT[ind+3]  = new TH1D(sb,sb,3,-0.5,2.5);
  }

  for(int i=0; i<4; i++){
    for(int j=0; j<3; j++){
      AddOutput(hDZllHLT[i+j*100]);
    }
  }
}

//--------------------------------------------------------------------------------------------------
void ZllEvtSelMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis
}

//--------------------------------------------------------------------------------------------------
void ZllEvtSelMod::Terminate()
{
  // Run finishing code on the client computer
}

//--------------------------------------------------------------------------------------------------
void ZllEvtSelMod::getTrackIsolation(double theEta, double thePhi,
                                     vector<ChargedParticle*> leptons,
                                     mithep::TrackCol *iTracks,
				     double &sumPt, int &nTracks) {
  sumPt   = 0.0; nTracks = 0;
  if(leptons.size() < 2) return;

  for(unsigned int i = 0; i < iTracks->GetEntries(); i++) {
    mithep::Track* pTrack = iTracks->At(i);
    if(MathUtils::DeltaR(pTrack->Eta(), pTrack->Phi(),
    		         leptons[0]->Eta(), leptons[0]->Phi()) < 0.05 ||
       MathUtils::DeltaR(pTrack->Eta(), pTrack->Phi(),
    		         leptons[1]->Eta(), leptons[1]->Phi()) < 0.05 ||
       MathUtils::DeltaR(pTrack->Eta(), pTrack->Phi(),
    		         theEta, thePhi) > 0.3) continue;

    if(pTrack->Pt() <= 0.0) continue;
    sumPt = sumPt + pTrack->Pt(); nTracks++;
  }
}

//--------------------------------------------------------------------------------------------------
void ZllEvtSelMod::getCaloIsolation(double theEta, double thePhi,
                                    vector<ChargedParticle*> leptons,
                                    mithep::CaloTowerCol *iTowers,
				    double lIso[4]) {

  for(int i=0; i<4; i++) lIso[i] = 0.0;
  if(leptons.size() < 2) return;

  // input (to be included somewhere else)
  double fEth    = 0.2;
  double fHth    = 0.5;
  double fHoh    = 0.5;
    
  double fNoiseEB = 0.04;
  double fNoiseEE = 0.15;
  double fNoiseHB = 0.2;
  double fNoiseHO = 0.2;

  double fDRMax  = 0.3;
  double fVetoEm = 0.07;
  double fVetoHa = 0.1;
  double fVetoHo = 0.1;

  for (unsigned int i0 = 0; i0 < iTowers->GetEntries(); i0++){
    mithep::CaloTower *pTower = iTowers->At(i0);

    if(MathUtils::DeltaR(pTower->Eta(), pTower->Phi(),
    		         leptons[0]->Eta(), leptons[0]->Phi()) < 0.1 ||
       MathUtils::DeltaR(pTower->Eta(), pTower->Phi(),
    		         leptons[1]->Eta(), leptons[1]->Phi()) < 0.1) continue;

    double pDR = MathUtils::DeltaR(pTower->Eta(), pTower->Phi(), theEta, thePhi);

    double pEmEt = pTower->EmEt();
    double pEmE  = pTower->EmEnergy();
    bool   pEcal = pEmEt > fEth && pEmE > 3 * (fabs(theEta) > 1.479 ? fNoiseEE : fNoiseEB);
    
    double pHaEt = pTower->HadEt();
    double pHaE  = pTower->HadEnergy();
    bool   pHcal = pHaEt > fHth && pHaE > 3 * fNoiseHB;
    
    double pHoEt = pTower->OuterEt();
    double pHoE  = pTower->OuterEnergy();
    bool   pHoal = pHoEt > fHoh && pHoE > 3 * fNoiseHO;
    
    if (pDR > fDRMax) continue;
    
    if ((!pEcal) && (!pHcal) && (!pHoal)) continue;

    bool pEmVeto = false; bool pHaVeto = false; bool pHoVeto = false;
    if (pDR < fVetoEm ) pEmVeto = true;
    if (pDR < fVetoHa ) pHaVeto = true;
    if (pDR < fVetoHo ) pHoVeto = true;
    
    if (pEcal && pEmVeto == false) {lIso[1] += pEmEt; lIso[0] += pEmEt;}
    if (pHcal && pHaVeto == false) {lIso[2] += pHaEt; lIso[0] += pHaEt;} 
    if (pHoal && pHoVeto == false) {lIso[3] += pHoEt; lIso[0] += pHoEt;} 
  }
}
