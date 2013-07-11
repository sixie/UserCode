// $Id: MuonIsolationStudyMod.cc,v 1.2 2008/12/04 19:19:50 sixie Exp $

#include "MitHiggs/IsolationStudyMods/interface/MuonIsolationStudyMod.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitPhysics/Utils/interface/IsolationTools.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include <TH1D.h>
#include <TH2D.h>

using namespace mithep;

ClassImp(mithep::MuonIsolationStudyMod)

//--------------------------------------------------------------------------------------------------
  MuonIsolationStudyMod::MuonIsolationStudyMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fPrintDebug(false),
  fMuonName(Names::gkMuonBrn),
  fMCLeptonsName(ModNames::gkMCLeptonsName),
  fMuons(0),
  fNEventsProcessed(0),
  fMatchToRealMuons(false),
  fImposeMuonID(false),
  fIDLikelihoodCut(0.9),
  fMuonPtMin(0.0),
  fMuonPtMax(10000.0),
  fNMuons(0),
  fNMuonsPassDefaultIsolation(0)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void MuonIsolationStudyMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont do
  // anything here.
}

//--------------------------------------------------------------------------------------------------
void MuonIsolationStudyMod::Process()
{
  // Process entries of the tree. 

  fNEventsProcessed++;
  fTotalEvents->Fill(0);

  if (fNEventsProcessed % 1000 == 0 || fPrintDebug) {
    time_t systime;
    systime = time(NULL);

    cerr << endl << "MuonIsolationStudyMod : Process Event " << fNEventsProcessed << "  Time: " << ctime(&systime) << endl;  
  }  
  



  ObjArray<MCParticle> *GenLeptons = 0;
  if (fMatchToRealMuons)
    GenLeptons = dynamic_cast<ObjArray<MCParticle>* > (FindObjThisEvt(fMCLeptonsName.Data()));

  //Get Muons
  LoadBranch(fMuonName);

  for (UInt_t i=0; i<fMuons->GetEntries(); ++i) {    
    const Muon *mu = fMuons->At(i);        
    
    bool allCuts = ( mu->GlobalTrk() && fabs(mu->Eta()) < 2.5);
  
    if (!allCuts)
      continue;

    if (fMatchToRealMuons) {
      bool matched = false;
      for (UInt_t j=0; j<GenLeptons->GetEntries(); ++j) { 
        if (GenLeptons->At(j)->AbsPdgId() == 13 && MathUtils::DeltaR(GenLeptons->At(j)->Mom(),
                                                                     mu->Mom()) < 0.1) {
          matched = true;
          break;
        }
      }

      if (!matched)
        continue;
    }

    //get eta and pt distribution
    fMuonPtHist->Fill(mu->Pt());
    fMuonEtaHist->Fill(mu->Eta());


    //2D distribution of Isolation Versus Pt
    //here i want all pt range
    fMuonTrackIso03VsPt->Fill(mu->IsoR03SumPt(), mu->Pt());
    fMuonEcalIso03VsPt->Fill(mu->IsoR03EmEt(), mu->Pt());
    fMuonHcalIso03VsPt->Fill(mu->IsoR05HadEt(),mu->Pt());
    fMuonTrackPlusEcalPlusHcalIso03VsPt->Fill(mu->IsoR03SumPt()+mu->IsoR03EmEt()+mu->IsoR05HadEt(),
                                              mu->Pt());

    //only look at muons in this pt range
    if (!(mu->Pt() > fMuonPtMin && mu->Pt() < fMuonPtMax))
      continue;

    fNMuons++;
    
    if (mu->IsoR03SumPt() < 3.0 && mu->IsoR03EmEt() + mu->IsoR03HadEt() < 3.0)
      fNMuonsPassDefaultIsolation++;

    //1D distributions
    fMuonTrackIsoR03->Fill(mu->IsoR03SumPt());
    fMuonEcalIsoR03->Fill(mu->IsoR03EmEt());
    fMuonHcalIsoR03->Fill(mu->IsoR03HadEt());
    fMuonEcalPlusHcalIsoR03->Fill(mu->IsoR03EmEt() + mu->IsoR03HadEt());
    fMuonOnePointFiveEcalPlusHcalIsoR03->Fill(1.5*mu->IsoR03EmEt()+mu->IsoR03HadEt());
    fMuonTrackPlusEcalPlusHcalIsoR03->Fill(mu->IsoR03SumPt()+mu->IsoR03EmEt()+mu->IsoR03HadEt());
    fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03->Fill(
      mu->IsoR03SumPt()+1.5*mu->IsoR03EmEt()+mu->IsoR03HadEt());
    fMuonTwoTrackPlusEcalPlusHcalIsoR03->
      Fill(2*mu->IsoR03SumPt()+mu->IsoR03EmEt()+mu->IsoR03HadEt());
    fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03->Fill( 
      2*mu->IsoR03SumPt()+1.5*mu->IsoR03EmEt()+mu->IsoR03HadEt());
    fMuonTrackIsoR05->Fill(mu->IsoR05SumPt());
    fMuonEcalIsoR05->Fill(mu->IsoR05EmEt());
    fMuonHcalIsoR05->Fill(mu->IsoR05HadEt());
    fMuonEcalPlusHcalIsoR05->Fill(mu->IsoR05EmEt() + mu->IsoR05HadEt());
    fMuonOnePointFiveEcalPlusHcalIsoR05->Fill(1.5*mu->IsoR05EmEt()+mu->IsoR05HadEt());
    fMuonTrackPlusEcalPlusHcalIsoR05->Fill(mu->IsoR05SumPt()+mu->IsoR05EmEt()+mu->IsoR05HadEt());
    fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05->Fill(
      mu->IsoR05SumPt()+1.5*mu->IsoR05EmEt()+mu->IsoR05HadEt());
    fMuonTwoTrackPlusEcalPlusHcalIsoR05->
      Fill(2*mu->IsoR05SumPt()+mu->IsoR05EmEt()+mu->IsoR05HadEt());
    fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05->Fill( 
      2*mu->IsoR05SumPt()+1.5*mu->IsoR05EmEt()+mu->IsoR05HadEt());
    fMuonTrackIsoR03OverPt->Fill(mu->IsoR03SumPt()/mu->Pt());
    fMuonEcalIsoR03OverPt->Fill(mu->IsoR03EmEt()/mu->Pt());
    fMuonHcalIsoR03OverPt->Fill(mu->IsoR03HadEt()/mu->Pt());
    fMuonEcalPlusHcalIsoR03OverPt->Fill((mu->IsoR03EmEt() + mu->IsoR03HadEt())/ mu->Pt());
    fMuonOnePointFiveEcalPlusHcalIsoR03OverPt->Fill((1.5*mu->IsoR03EmEt()+
                                                     mu->IsoR03HadEt())/mu->Pt());
    fMuonTrackPlusEcalPlusHcalIsoR03OverPt->Fill((mu->IsoR03SumPt()+mu->IsoR03EmEt()+
                                                  mu->IsoR03HadEt())/mu->Pt());
    fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt->Fill(
      (mu->IsoR03SumPt()+1.5*mu->IsoR03EmEt()+mu->IsoR03HadEt())/mu->Pt());
    fMuonTwoTrackPlusEcalPlusHcalIsoR03OverPt->
      Fill((2*mu->IsoR03SumPt()+mu->IsoR03EmEt()+mu->IsoR03HadEt())/mu->Pt());
    fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt->Fill( 
      (2*mu->IsoR03SumPt()+1.5*mu->IsoR03EmEt()+mu->IsoR03HadEt())/mu->Pt());
    fMuonTrackIsoR05OverPt->Fill(mu->IsoR05SumPt()/mu->Pt());
    fMuonEcalIsoR05OverPt->Fill(mu->IsoR05EmEt()/mu->Pt());
    fMuonHcalIsoR05OverPt->Fill(mu->IsoR05HadEt()/mu->Pt());
    fMuonEcalPlusHcalIsoR05OverPt->Fill((mu->IsoR05EmEt() + mu->IsoR05HadEt())/mu->Pt());
    fMuonOnePointFiveEcalPlusHcalIsoR05OverPt->Fill((1.5*mu->IsoR05EmEt()+
                                                     mu->IsoR05HadEt())/mu->Pt());
    fMuonTrackPlusEcalPlusHcalIsoR05OverPt->Fill((mu->IsoR05SumPt()+mu->IsoR05EmEt()+
                                                  mu->IsoR05HadEt())/mu->Pt());
    fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt->Fill(
      (mu->IsoR05SumPt()+1.5*mu->IsoR05EmEt()+mu->IsoR05HadEt())/mu->Pt());
    fMuonTwoTrackPlusEcalPlusHcalIsoR05OverPt->Fill(
      (2*mu->IsoR05SumPt()+mu->IsoR05EmEt()+mu->IsoR05HadEt())/mu->Pt());
    fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt->Fill( 
      (2*mu->IsoR05SumPt()+1.5*mu->IsoR05EmEt()+mu->IsoR05HadEt())/mu->Pt());

    //2D distributions
    fMuonTrackIsoVsEcalIsoR03->Fill(mu->IsoR03SumPt(),mu->IsoR03EmEt());
    fMuonTrackIsoVsHcalIsoR03->Fill(mu->IsoR03SumPt(),mu->IsoR05HadEt());
    fMuonTrackIsoVsEcalPlusHcalIsoR03->Fill(mu->IsoR03SumPt(),mu->IsoR03EmEt()+mu->IsoR05HadEt());
    fMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoR03->Fill(mu->IsoR03SumPt(),
                                                        1.5*mu->IsoR03EmEt()+mu->IsoR05HadEt());
    fMuonTrackIsoVsEcalIsoR05->Fill(mu->IsoR05SumPt(),mu->IsoR05EmEt());
    fMuonTrackIsoVsHcalIsoR05->Fill(mu->IsoR05SumPt(),mu->IsoR05HadEt());
    fMuonTrackIsoVsEcalPlusHcalIsoR05->Fill(mu->IsoR05SumPt(),mu->IsoR05EmEt()+mu->IsoR05HadEt());
    fMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoR05->Fill(mu->IsoR05SumPt(),
                                                        1.5*mu->IsoR05EmEt()+mu->IsoR05HadEt());
    
    
    //do 2D matrix of Number of Muons Passing trackIso and caloIso Cuts
    for (int t = 0;t < 101; t++) {
      double trackIsoCut = 0.0 + t*0.2;
      for (int c = 0; c < 101 ; c++ ) {
        double caloIsoCut = 0.0 + c*0.2;        
        if (mu->IsoR03SumPt() < trackIsoCut && mu->IsoR03EmEt() < caloIsoCut)
          fNMuonPassTrackIsoVsEcalIsoR03->Fill(trackIsoCut, caloIsoCut) ;
        if (mu->IsoR03SumPt() < trackIsoCut && mu->IsoR03HadEt() < caloIsoCut)
          fNMuonPassTrackIsoVsHcalIsoR03->Fill(trackIsoCut, caloIsoCut) ;
        if (mu->IsoR03SumPt() < trackIsoCut && mu->IsoR03EmEt()+mu->IsoR03HadEt() < caloIsoCut)
          fNMuonPassTrackIsoVsEcalPlusHcalIsoR03->Fill(trackIsoCut, caloIsoCut) ;
        if (mu->IsoR03SumPt() < trackIsoCut && 1.5*mu->IsoR03EmEt()+mu->IsoR03HadEt() < caloIsoCut)
          fNMuonPassTrackIsoVsOnePointFiveEcalPlusHcalIsoR03->Fill(trackIsoCut, caloIsoCut);
        if (mu->IsoR05SumPt() < trackIsoCut && mu->IsoR05EmEt() < caloIsoCut)
          fNMuonPassTrackIsoVsEcalIsoR05->Fill(trackIsoCut, caloIsoCut) ;
        if (mu->IsoR05SumPt() < trackIsoCut && mu->IsoR05HadEt() < caloIsoCut)
          fNMuonPassTrackIsoVsHcalIsoR05->Fill(trackIsoCut, caloIsoCut) ;
        if (mu->IsoR05SumPt() < trackIsoCut && mu->IsoR05EmEt()+mu->IsoR05HadEt() < caloIsoCut)
          fNMuonPassTrackIsoVsEcalPlusHcalIsoR05->Fill(trackIsoCut, caloIsoCut) ;
        if (mu->IsoR05SumPt() < trackIsoCut && 1.5*mu->IsoR05EmEt()+mu->IsoR05HadEt() < caloIsoCut)
          fNMuonPassTrackIsoVsOnePointFiveEcalPlusHcalIsoR05->Fill(trackIsoCut, caloIsoCut);
      }            
    }   
  }                                            
  
  //Final Summary Debug Output   
  if ( fPrintDebug ) {
    cerr << "Event Dump: " << fNEventsProcessed << endl;    
  }   
  
}


//--------------------------------------------------------------------------------------------------
void MuonIsolationStudyMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.

  ReqBranch(fMuonName,              fMuons);
  fMuonPtHist                     = new TH1D("hMuonPtHist",";p_{t};#",100,0.,200.);  
  fMuonEtaHist                    = new TH1D("hMuonEtaHist",";#eta;#",100,-5.,5.);

  fTotalEvents  = new TH1D("hTotalEvents",";XAxis;Number of Events",1,0.0,1.0);  
  AddOutput(fTotalEvents);

//*************************************************************************************************
// Isolation Cone 0.5 Histograms
//*************************************************************************************************


  fMuonTrackIsoVsEcalIsoR03  = 
    new TH2D("hMuonTrackIsoVsEcalIsoR03",";TrackIsolation;EcalIsolation",
             101,0,20.0,101,0,20.0);  
  fMuonTrackIsoVsHcalIsoR03  = 
    new TH2D("hMuonTrackIsoVsHcalIsoR03",";TrackIsolation;HcalIsolation",
             101,0,20.0,101,0,20.0);  
  fMuonTrackIsoVsEcalPlusHcalIsoR03  = 
    new TH2D("hMuonTrackIsoVsEcalPlusHcalIsoR03",";TrackIsolation;Ecal + Hcal Isolation",
             101,0,20.0,101,0,20.0);  
  fMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoR03  = 
    new TH2D("hMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoR03",
             ";TrackIsolation;1.5*Ecal Iso + Hcal Iso", 101,0,20.0,101,0,20.0);  

  fNMuonPassTrackIsoVsEcalIsoR03  = 
    new TH2D("hNMuonPassTrackIsoVsEcalIsoR03",";TrackIsolation;EcalIsolation",
             101,0,20.0,101,0,20.0);  
  fNMuonPassTrackIsoVsHcalIsoR03  = 
    new TH2D("hNMuonPassTrackIsoVsHcalIsoR03",";TrackIsolation;HcalIsolation",
             101,0,20.0,101,0,20.0);  
  fNMuonPassTrackIsoVsEcalPlusHcalIsoR03  = 
    new TH2D("hNMuonPassTrackIsoVsEcalPlusHcalIsoR03",";TrackIsolation;Ecal + Hcal Isolation",
             101,0,20.0,101,0,20.0);  
  fNMuonPassTrackIsoVsOnePointFiveEcalPlusHcalIsoR03  = 
    new TH2D("hNMuonPassTrackIsoVsOnePointFiveEcalPlusHcalIsoR03",
             ";TrackIsolation;1.5*Ecal Iso + Hcal Iso", 101,0,20.0,101,0,20.0);  

  fMuonTrackIsoVsEcalIsoEffR03  = 
    new TH2D("hMuonTrackIsoVsEcalIsoEffR03",";TrackIsolation;EcalIsolation",
             101,0,20.0,101,0,20.0);  
  fMuonTrackIsoVsHcalIsoEffR03  = 
    new TH2D("hMuonTrackIsoVsHcalIsoEffR03",";TrackIsolation;HcalIsolation",
             101,0,20.0,101,0,20.0);  
  fMuonTrackIsoVsEcalPlusHcalIsoEffR03  = 
    new TH2D("hMuonTrackIsoVsEcalPlusHcalIsoEffR03",";TrackIsolation;Ecal + Hcal Isolation",
             101,0,20.0,101,0,20.0);  
  fMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoEffR03  = 
    new TH2D("hMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoEffR03",
             ";TrackIsolation;1.5*Ecal Iso + Hcal Iso",101,0,20.0,101,0,20.0);  


  fMuonTrackIsoR03  = 
    new TH1D("hMuonTrackIsoR03",";TrackIso ;Number of Events", 101,0,20.0); 
  fMuonEcalIsoR03  = 
    new TH1D("hMuonEcalIsoR03",";EcalIso;Number of Events", 101,0,20.0); 
  fMuonHcalIsoR03  = 
    new TH1D("hMuonHcalIsoR03",";HcalIso;Number of Events", 101,0,20.0); 
  fMuonEcalPlusHcalIsoR03  = 
    new TH1D("hMuonEcalPlusHcalIsoR03",";Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonOnePointFiveEcalPlusHcalIsoR03  = 
    new TH1D("hMuonOnePointFiveEcalPlusHcalIsoR03",
             ";1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTrackPlusEcalPlusHcalIsoR03  = 
    new TH1D("hMuonTrackPlusEcalPlusHcalIsoR03",
             ";Track + Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03  = 
    new TH1D("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03",
             ";Track + 1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTwoTrackPlusEcalPlusHcalIsoR03  = 
    new TH1D("hMuonTwoTrackPlusEcalPlusHcalIsoR03",
             ";2*Track + Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03  = 
    new TH1D("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03",
             ";2*Track + 1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTrackIsoR03OverPt  = 
    new TH1D("hMuonTrackIsoR03OverPt",";TrackIso / Pt ;Number of Events", 201,0,4.0); 
  fMuonEcalIsoR03OverPt  = 
    new TH1D("hMuonEcalIsoR03OverPt",";EcalIso / Pt;Number of Events", 201,0,4.0); 
  fMuonHcalIsoR03OverPt  = 
    new TH1D("hMuonHcalIsoR03OverPt",";HcalIso / Pt;Number of Events", 201,0,4.0); 
  fMuonEcalPlusHcalIsoR03OverPt  = 
    new TH1D("hMuonEcalPlusHcalIsoR03OverPt",";Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonOnePointFiveEcalPlusHcalIsoR03OverPt  = 
    new TH1D("hMuonOnePointFiveEcalPlusHcalIsoR03OverPt",
             ";1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonTrackPlusEcalPlusHcalIsoR03OverPt  = 
    new TH1D("hMuonTrackPlusEcalPlusHcalIsoR03OverPt",
             ";Track + Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt  = 
    new TH1D("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt",
             ";Track + 1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonTwoTrackPlusEcalPlusHcalIsoR03OverPt  = 
    new TH1D("hMuonTwoTrackPlusEcalPlusHcalIsoR03OverPt",
             ";2*Track + Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt  = 
    new TH1D("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt",
             ";2*Track + 1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 


  fNMuonPassTrackIsoR03  = 
    new TH1D("hNMuonPassTrackIsoR03",";TrackIso ;Number of Events", 101,0,20.0); 
  fNMuonPassEcalIsoR03  = 
    new TH1D("hNMuonPassEcalIsoR03",";EcalIso;Number of Events", 101,0,20.0); 
  fNMuonPassHcalIsoR03  = 
    new TH1D("hNMuonPassHcalIsoR03",";HcalIso;Number of Events", 101,0,20.0); 
  fNMuonPassEcalPlusHcalIsoR03  = 
    new TH1D("hNMuonPassEcalPlusHcalIsoR03",";Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fNMuonPassOnePointFiveEcalPlusHcalIsoR03  = 
    new TH1D("hNMuonPassOnePointFiveEcalPlusHcalIsoR03",
             ";1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fNMuonPassTrackPlusEcalPlusHcalIsoR03  = 
    new TH1D("hNMuonPassTrackPlusEcalPlusHcalIsoR03",
             ";Track + Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR03  = 
    new TH1D("hNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR03",
             ";Track + 1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fNMuonPassTwoTrackPlusEcalPlusHcalIsoR03  = 
    new TH1D("hNMuonPassTwoTrackPlusEcalPlusHcalIsoR03",
             ";2*Track + Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03  = 
    new TH1D("hNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03",
             ";2*Track + 1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fNMuonPassTrackIsoR03OverPt  = 
    new TH1D("hNMuonPassTrackIsoR03OverPt",";TrackIso / Pt ;Number of Events", 201,0,4.0); 
  fNMuonPassEcalIsoR03OverPt  = 
    new TH1D("hNMuonPassEcalIsoR03OverPt",";EcalIso / Pt;Number of Events", 201,0,4.0); 
  fNMuonPassHcalIsoR03OverPt  = 
    new TH1D("hNMuonPassHcalIsoR03OverPt",";HcalIso / Pt;Number of Events", 201,0,4.0); 
  fNMuonPassEcalPlusHcalIsoR03OverPt  = 
    new TH1D("hNMuonPassEcalPlusHcalIsoR03OverPt",";Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fNMuonPassOnePointFiveEcalPlusHcalIsoR03OverPt  = 
    new TH1D("hNMuonPassOnePointFiveEcalPlusHcalIsoR03OverPt",
             ";1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fNMuonPassTrackPlusEcalPlusHcalIsoR03OverPt  = 
    new TH1D("hNMuonPassTrackPlusEcalPlusHcalIsoR03OverPt",
             ";Track + Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt  = 
    new TH1D("hNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt",
             ";Track + 1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fNMuonPassTwoTrackPlusEcalPlusHcalIsoR03OverPt  = 
    new TH1D("hNMuonPassTwoTrackPlusEcalPlusHcalIsoR03OverPt",
             ";2*Track + Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt  = 
    new TH1D("hNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt",
             ";2*Track + 1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 

  fMuonTrackIsoR03Eff  = 
    new TH1D("hMuonTrackIsoR03Eff",";TrackIso ;Number of Events", 101,0,20.0); 
  fMuonEcalIsoR03Eff  = 
    new TH1D("hMuonEcalIsoR03Eff",";EcalIso;Number of Events", 101,0,20.0); 
  fMuonHcalIsoR03Eff  = 
    new TH1D("hMuonHcalIsoR03Eff",";HcalIso;Number of Events", 101,0,20.0); 
  fMuonEcalPlusHcalIsoR03Eff  = 
    new TH1D("hMuonEcalPlusHcalIsoR03Eff",";Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonOnePointFiveEcalPlusHcalIsoR03Eff  = 
    new TH1D("hMuonOnePointFiveEcalPlusHcalIsoR03Eff",
             ";1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTrackPlusEcalPlusHcalIsoR03Eff  = 
    new TH1D("hMuonTrackPlusEcalPlusHcalIsoR03Eff",
             ";Track + Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03Eff  = 
    new TH1D("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03Eff",
             ";Track + 1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTwoTrackPlusEcalPlusHcalIsoR03Eff  = 
    new TH1D("hMuonTwoTrackPlusEcalPlusHcalIsoR03Eff",
             ";2*Track + Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03Eff  = 
    new TH1D("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03Eff",
             ";2*Track + 1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTrackIsoR03OverPtEff  = 
    new TH1D("hMuonTrackIsoR03OverPtEff",";TrackIso / Pt ;Number of Events", 201,0,4.0); 
  fMuonEcalIsoR03OverPtEff  = 
    new TH1D("hMuonEcalIsoR03OverPtEff",";EcalIso / Pt;Number of Events", 201,0,4.0); 
  fMuonHcalIsoR03OverPtEff  = 
    new TH1D("hMuonHcalIsoR03OverPtEff",";HcalIso / Pt;Number of Events", 201,0,4.0); 
  fMuonEcalPlusHcalIsoR03OverPtEff  = 
    new TH1D("hMuonEcalPlusHcalIsoR03OverPtEff",";Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonOnePointFiveEcalPlusHcalIsoR03OverPtEff  = 
    new TH1D("hMuonOnePointFiveEcalPlusHcalIsoR03OverPtEff",
             ";1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonTrackPlusEcalPlusHcalIsoR03OverPtEff  = 
    new TH1D("hMuonTrackPlusEcalPlusHcalIsoR03OverPtEff",
             ";Track + Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPtEff  = 
    new TH1D("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPtEff",
             ";Track + 1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonTwoTrackPlusEcalPlusHcalIsoR03OverPtEff  = 
    new TH1D("hMuonTwoTrackPlusEcalPlusHcalIsoR03OverPtEff",
             ";2*Track + Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPtEff  = 
    new TH1D("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPtEff",
             ";2*Track + 1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 

//*************************************************************************************************
// Isolation Cone 0.5 Histograms
//*************************************************************************************************

  fMuonTrackIsoVsEcalIsoR05  = 
    new TH2D("hMuonTrackIsoVsEcalIsoR05",";TrackIsolation;EcalIsolation",
             101,0,20.0,101,0,20.0);  
  fMuonTrackIsoVsHcalIsoR05  = 
    new TH2D("hMuonTrackIsoVsHcalIsoR05",";TrackIsolation;HcalIsolation",
             101,0,20.0,101,0,20.0);  
  fMuonTrackIsoVsEcalPlusHcalIsoR05  = 
    new TH2D("hMuonTrackIsoVsEcalPlusHcalIsoR05",";TrackIsolation;Ecal + Hcal Isolation",
             101,0,20.0,101,0,20.0);  
  fMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoR05  = 
    new TH2D("hMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoR05",
             ";TrackIsolation;1.5*Ecal Iso + Hcal Iso", 101,0,20.0,101,0,20.0);  

  fNMuonPassTrackIsoVsEcalIsoR05  = 
    new TH2D("hNMuonPassTrackIsoVsEcalIsoR05",";TrackIsolation;EcalIsolation",
             101,0,20.0,101,0,20.0);  
  fNMuonPassTrackIsoVsHcalIsoR05  = 
    new TH2D("hNMuonPassTrackIsoVsHcalIsoR05",";TrackIsolation;HcalIsolation",
             101,0,20.0,101,0,20.0);  
  fNMuonPassTrackIsoVsEcalPlusHcalIsoR05  = 
    new TH2D("hNMuonPassTrackIsoVsEcalPlusHcalIsoR05",";TrackIsolation;Ecal + Hcal Isolation",
             101,0,20.0,101,0,20.0);  
  fNMuonPassTrackIsoVsOnePointFiveEcalPlusHcalIsoR05  = 
    new TH2D("hNMuonPassTrackIsoVsOnePointFiveEcalPlusHcalIsoR05",
             ";TrackIsolation;1.5*Ecal Iso + Hcal Iso", 101,0,20.0,101,0,20.0);  

  fMuonTrackIsoVsEcalIsoEffR05  = 
    new TH2D("hMuonTrackIsoVsEcalIsoEffR05",";TrackIsolation;EcalIsolation",
             101,0,20.0,101,0,20.0);  
  fMuonTrackIsoVsHcalIsoEffR05  = 
    new TH2D("hMuonTrackIsoVsHcalIsoEffR05",";TrackIsolation;HcalIsolation",
             101,0,20.0,101,0,20.0);  
  fMuonTrackIsoVsEcalPlusHcalIsoEffR05  = 
    new TH2D("hMuonTrackIsoVsEcalPlusHcalIsoEffR05",";TrackIsolation;Ecal + Hcal Isolation",
             101,0,20.0,101,0,20.0);  
  fMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoEffR05  = 
    new TH2D("hMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoEffR05",
             ";TrackIsolation;1.5*Ecal Iso + Hcal Iso",101,0,20.0,101,0,20.0);  


  fMuonTrackIsoR05  = 
    new TH1D("hMuonTrackIsoR05",";TrackIso ;Number of Events", 101,0,20.0); 
  fMuonEcalIsoR05  = 
    new TH1D("hMuonEcalIsoR05",";EcalIso;Number of Events", 101,0,20.0); 
  fMuonHcalIsoR05  = 
    new TH1D("hMuonHcalIsoR05",";HcalIso;Number of Events", 101,0,20.0); 
  fMuonEcalPlusHcalIsoR05  = 
    new TH1D("hMuonEcalPlusHcalIsoR05",";Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonOnePointFiveEcalPlusHcalIsoR05  = 
    new TH1D("hMuonOnePointFiveEcalPlusHcalIsoR05",
             ";1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTrackPlusEcalPlusHcalIsoR05  = 
    new TH1D("hMuonTrackPlusEcalPlusHcalIsoR05",
             ";Track + Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05  = 
    new TH1D("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05",
             ";Track + 1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTwoTrackPlusEcalPlusHcalIsoR05  = 
    new TH1D("hMuonTwoTrackPlusEcalPlusHcalIsoR05",
             ";2*Track + Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05  = 
    new TH1D("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05",
             ";2*Track + 1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTrackIsoR05OverPt  = 
    new TH1D("hMuonTrackIsoR05OverPt",";TrackIso / Pt ;Number of Events", 201,0,4.0); 
  fMuonEcalIsoR05OverPt  = 
    new TH1D("hMuonEcalIsoR05OverPt",";EcalIso / Pt;Number of Events", 201,0,4.0); 
  fMuonHcalIsoR05OverPt  = 
    new TH1D("hMuonHcalIsoR05OverPt",";HcalIso / Pt;Number of Events", 201,0,4.0); 
  fMuonEcalPlusHcalIsoR05OverPt  = 
    new TH1D("hMuonEcalPlusHcalIsoR05OverPt",";Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonOnePointFiveEcalPlusHcalIsoR05OverPt  = 
    new TH1D("hMuonOnePointFiveEcalPlusHcalIsoR05OverPt",
             ";1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonTrackPlusEcalPlusHcalIsoR05OverPt  = 
    new TH1D("hMuonTrackPlusEcalPlusHcalIsoR05OverPt",
             ";Track + Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt  = 
    new TH1D("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt",
             ";Track + 1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonTwoTrackPlusEcalPlusHcalIsoR05OverPt  = 
    new TH1D("hMuonTwoTrackPlusEcalPlusHcalIsoR05OverPt",
             ";2*Track + Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt  = 
    new TH1D("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt",
             ";2*Track + 1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 

  fNMuonPassTrackIsoR05  = 
    new TH1D("hNMuonPassTrackIsoR05",";TrackIso ;Number of Events", 101,0,20.0); 
  fNMuonPassEcalIsoR05  = 
    new TH1D("hNMuonPassEcalIsoR05",";EcalIso;Number of Events", 101,0,20.0); 
  fNMuonPassHcalIsoR05  = 
    new TH1D("hNMuonPassHcalIsoR05",";HcalIso;Number of Events", 101,0,20.0); 
  fNMuonPassEcalPlusHcalIsoR05  = 
    new TH1D("hNMuonPassEcalPlusHcalIsoR05",";Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fNMuonPassOnePointFiveEcalPlusHcalIsoR05  = 
    new TH1D("hNMuonPassOnePointFiveEcalPlusHcalIsoR05",
             ";1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fNMuonPassTrackPlusEcalPlusHcalIsoR05  = 
    new TH1D("hNMuonPassTrackPlusEcalPlusHcalIsoR05",
             ";Track + Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR05  = 
    new TH1D("hNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR05",
             ";Track + 1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fNMuonPassTwoTrackPlusEcalPlusHcalIsoR05  = 
    new TH1D("hNMuonPassTwoTrackPlusEcalPlusHcalIsoR05",
             ";2*Track + Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05  = 
    new TH1D("hNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05",
             ";2*Track + 1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fNMuonPassTrackIsoR05OverPt  = 
    new TH1D("hNMuonPassTrackIsoR05OverPt",";TrackIso / Pt ;Number of Events", 201,0,4.0); 
  fNMuonPassEcalIsoR05OverPt  = 
    new TH1D("hNMuonPassEcalIsoR05OverPt",";EcalIso / Pt;Number of Events", 201,0,4.0); 
  fNMuonPassHcalIsoR05OverPt  = 
    new TH1D("hNMuonPassHcalIsoR05OverPt",";HcalIso / Pt;Number of Events", 201,0,4.0); 
  fNMuonPassEcalPlusHcalIsoR05OverPt  = 
    new TH1D("hNMuonPassEcalPlusHcalIsoR05OverPt",";Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fNMuonPassOnePointFiveEcalPlusHcalIsoR05OverPt  = 
    new TH1D("hNMuonPassOnePointFiveEcalPlusHcalIsoR05OverPt",
             ";1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fNMuonPassTrackPlusEcalPlusHcalIsoR05OverPt  = 
    new TH1D("hNMuonPassTrackPlusEcalPlusHcalIsoR05OverPt",
             ";Track + Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt  = 
    new TH1D("hNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt",
             ";Track + 1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fNMuonPassTwoTrackPlusEcalPlusHcalIsoR05OverPt  = 
    new TH1D("hNMuonPassTwoTrackPlusEcalPlusHcalIsoR05OverPt",
             ";2*Track + Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt  = 
    new TH1D("hNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt",
             ";2*Track + 1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 


  fMuonTrackIsoR05Eff  = 
    new TH1D("hMuonTrackIsoR05Eff",";TrackIso ;Number of Events", 101,0,20.0); 
  fMuonEcalIsoR05Eff  = 
    new TH1D("hMuonEcalIsoR05Eff",";EcalIso;Number of Events", 101,0,20.0); 
  fMuonHcalIsoR05Eff  = 
    new TH1D("hMuonHcalIsoR05Eff",";HcalIso;Number of Events", 101,0,20.0); 
  fMuonEcalPlusHcalIsoR05Eff  = 
    new TH1D("hMuonEcalPlusHcalIsoR05Eff",";Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonOnePointFiveEcalPlusHcalIsoR05Eff  = 
    new TH1D("hMuonOnePointFiveEcalPlusHcalIsoR05Eff",
             ";1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTrackPlusEcalPlusHcalIsoR05Eff  = 
    new TH1D("hMuonTrackPlusEcalPlusHcalIsoR05Eff",
             ";Track + Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05Eff  = 
    new TH1D("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05Eff",
             ";Track + 1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTwoTrackPlusEcalPlusHcalIsoR05Eff  = 
    new TH1D("hMuonTwoTrackPlusEcalPlusHcalIsoR05Eff",
             ";2*Track + Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05Eff  = 
    new TH1D("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05Eff",
             ";2*Track + 1.5*Ecal + Hcal Iso;Number of Events", 101,0,20.0); 
  fMuonTrackIsoR05OverPtEff  = 
    new TH1D("hMuonTrackIsoR05OverPtEff",";TrackIso / Pt ;Number of Events", 201,0,4.0); 
  fMuonEcalIsoR05OverPtEff  = 
    new TH1D("hMuonEcalIsoR05OverPtEff",";EcalIso / Pt;Number of Events", 201,0,4.0); 
  fMuonHcalIsoR05OverPtEff  = 
    new TH1D("hMuonHcalIsoR05OverPtEff",";HcalIso / Pt;Number of Events", 201,0,4.0); 
  fMuonEcalPlusHcalIsoR05OverPtEff  = 
    new TH1D("hMuonEcalPlusHcalIsoR05OverPtEff",";Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonOnePointFiveEcalPlusHcalIsoR05OverPtEff  = 
    new TH1D("hMuonOnePointFiveEcalPlusHcalIsoR05OverPtEff",
             ";1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonTrackPlusEcalPlusHcalIsoR05OverPtEff  = 
    new TH1D("hMuonTrackPlusEcalPlusHcalIsoR05OverPtEff",
             ";Track + Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPtEff  = 
    new TH1D("hMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPtEff",
             ";Track + 1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonTwoTrackPlusEcalPlusHcalIsoR05OverPtEff  = 
    new TH1D("hMuonTwoTrackPlusEcalPlusHcalIsoR05OverPtEff",
             ";2*Track + Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 
  fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPtEff  = 
    new TH1D("hMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPtEff",
             ";2*Track + 1.5*Ecal + Hcal Iso / Pt;Number of Events", 201,0,4.0); 

//*************************************************************************************************
// Isolation Vs Pt Plots
//*************************************************************************************************
  fMuonTrackIso03VsPt = 
    new TH2D("hMuonTrackIso03VsPt",";TrackIsolation;Pt",
             101,0,20.0,101,0,100.0);  
  fMuonEcalIso03VsPt = 
    new TH2D("hMuonEcalIso03VsPt",";Ecal Isolation;Pt",
             101,0,20.0,101,0,100.0);  
  fMuonHcalIso03VsPt = 
    new TH2D("hMuonHcalIso03VsPt",";Hcal Isolation;Pt",
             101,0,20.0,101,0,100.0);  
  fMuonTrackPlusEcalPlusHcalIso03VsPt = 
    new TH2D("hMuonTrackPlusEcalPlusHcalIso03VsPt",";Track + Ecal + Hcal Isolation;Pt",
             101,0,20.0,101,0,100.0);  


  AddOutput(fMuonPtHist); 
  AddOutput(fMuonEtaHist); 
  AddOutput(fMuonTrackIsoVsEcalIsoR03 ); 
  AddOutput(fMuonTrackIsoVsHcalIsoR03 ); 
  AddOutput(fMuonTrackIsoVsEcalPlusHcalIsoR03 ); 
  AddOutput(fMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoR03 ); 
  AddOutput(fNMuonPassTrackIsoVsEcalIsoR03 ); 
  AddOutput(fNMuonPassTrackIsoVsHcalIsoR03 ); 
  AddOutput(fNMuonPassTrackIsoVsEcalPlusHcalIsoR03 ); 
  AddOutput(fNMuonPassTrackIsoVsOnePointFiveEcalPlusHcalIsoR03 ); 
  AddOutput(fMuonTrackIsoVsEcalIsoEffR03 ); 
  AddOutput(fMuonTrackIsoVsHcalIsoEffR03 ); 
  AddOutput(fMuonTrackIsoVsEcalPlusHcalIsoEffR03 ); 
  AddOutput(fMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoEffR03 ); 
  AddOutput(fMuonTrackIsoR03 ); 
  AddOutput(fMuonEcalIsoR03 ); 
  AddOutput(fMuonHcalIsoR03 ); 
  AddOutput(fMuonEcalPlusHcalIsoR03 ); 
  AddOutput(fMuonOnePointFiveEcalPlusHcalIsoR03 ); 
  AddOutput(fMuonTrackPlusEcalPlusHcalIsoR03 ); 
  AddOutput(fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03 ); 
  AddOutput(fMuonTwoTrackPlusEcalPlusHcalIsoR03 ); 
  AddOutput(fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03 ); 
  AddOutput(fNMuonPassTrackIsoR03 ); 
  AddOutput(fNMuonPassEcalIsoR03 ); 
  AddOutput(fNMuonPassHcalIsoR03 ); 
  AddOutput(fNMuonPassEcalPlusHcalIsoR03 ); 
  AddOutput(fNMuonPassOnePointFiveEcalPlusHcalIsoR03 ); 
  AddOutput(fNMuonPassTrackPlusEcalPlusHcalIsoR03 ); 
  AddOutput(fNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR03 ); 
  AddOutput(fNMuonPassTwoTrackPlusEcalPlusHcalIsoR03 ); 
  AddOutput(fNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03 ); 
  AddOutput(fMuonTrackIsoR03Eff ); 
  AddOutput(fMuonEcalIsoR03Eff ); 
  AddOutput(fMuonHcalIsoR03Eff ); 
  AddOutput(fMuonEcalPlusHcalIsoR03Eff ); 
  AddOutput(fMuonOnePointFiveEcalPlusHcalIsoR03Eff ); 
  AddOutput(fMuonTrackPlusEcalPlusHcalIsoR03Eff ); 
  AddOutput(fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03Eff ); 
  AddOutput(fMuonTwoTrackPlusEcalPlusHcalIsoR03Eff ); 
  AddOutput(fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03Eff ); 
  AddOutput(fMuonTrackIsoR03OverPt ); 
  AddOutput(fMuonEcalIsoR03OverPt ); 
  AddOutput(fMuonHcalIsoR03OverPt ); 
  AddOutput(fMuonEcalPlusHcalIsoR03OverPt ); 
  AddOutput(fMuonOnePointFiveEcalPlusHcalIsoR03OverPt ); 
  AddOutput(fMuonTrackPlusEcalPlusHcalIsoR03OverPt ); 
  AddOutput(fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt ); 
  AddOutput(fMuonTwoTrackPlusEcalPlusHcalIsoR03OverPt ); 
  AddOutput(fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt ); 
  AddOutput(fNMuonPassTrackIsoR03OverPt ); 
  AddOutput(fNMuonPassEcalIsoR03OverPt ); 
  AddOutput(fNMuonPassHcalIsoR03OverPt ); 
  AddOutput(fNMuonPassEcalPlusHcalIsoR03OverPt ); 
  AddOutput(fNMuonPassOnePointFiveEcalPlusHcalIsoR03OverPt ); 
  AddOutput(fNMuonPassTrackPlusEcalPlusHcalIsoR03OverPt ); 
  AddOutput(fNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt ); 
  AddOutput(fNMuonPassTwoTrackPlusEcalPlusHcalIsoR03OverPt ); 
  AddOutput(fNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt ); 
  AddOutput(fMuonTrackIsoR03OverPtEff ); 
  AddOutput(fMuonEcalIsoR03OverPtEff ); 
  AddOutput(fMuonHcalIsoR03OverPtEff ); 
  AddOutput(fMuonEcalPlusHcalIsoR03OverPtEff ); 
  AddOutput(fMuonOnePointFiveEcalPlusHcalIsoR03OverPtEff ); 
  AddOutput(fMuonTrackPlusEcalPlusHcalIsoR03OverPtEff ); 
  AddOutput(fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPtEff ); 
  AddOutput(fMuonTwoTrackPlusEcalPlusHcalIsoR03OverPtEff ); 
  AddOutput(fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPtEff ); 
  AddOutput(fMuonTrackIsoVsEcalIsoR05 ); 
  AddOutput(fMuonTrackIsoVsHcalIsoR05 ); 
  AddOutput(fMuonTrackIsoVsEcalPlusHcalIsoR05 ); 
  AddOutput(fMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoR05 ); 
  AddOutput(fNMuonPassTrackIsoVsEcalIsoR05 ); 
  AddOutput(fNMuonPassTrackIsoVsHcalIsoR05 ); 
  AddOutput(fNMuonPassTrackIsoVsEcalPlusHcalIsoR05 ); 
  AddOutput(fNMuonPassTrackIsoVsOnePointFiveEcalPlusHcalIsoR05 ); 
  AddOutput(fMuonTrackIsoVsEcalIsoEffR05 ); 
  AddOutput(fMuonTrackIsoVsHcalIsoEffR05 ); 
  AddOutput(fMuonTrackIsoVsEcalPlusHcalIsoEffR05 ); 
  AddOutput(fMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoEffR05 ); 
  AddOutput(fMuonTrackIsoR05 ); 
  AddOutput(fMuonEcalIsoR05 ); 
  AddOutput(fMuonHcalIsoR05 ); 
  AddOutput(fMuonEcalPlusHcalIsoR05 ); 
  AddOutput(fMuonOnePointFiveEcalPlusHcalIsoR05 ); 
  AddOutput(fMuonTrackPlusEcalPlusHcalIsoR05 ); 
  AddOutput(fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05 ); 
  AddOutput(fMuonTwoTrackPlusEcalPlusHcalIsoR05 ); 
  AddOutput(fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05 ); 
  AddOutput(fNMuonPassTrackIsoR05 ); 
  AddOutput(fNMuonPassEcalIsoR05 ); 
  AddOutput(fNMuonPassHcalIsoR05 ); 
  AddOutput(fNMuonPassEcalPlusHcalIsoR05 ); 
  AddOutput(fNMuonPassOnePointFiveEcalPlusHcalIsoR05 ); 
  AddOutput(fNMuonPassTrackPlusEcalPlusHcalIsoR05 ); 
  AddOutput(fNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR05 ); 
  AddOutput(fNMuonPassTwoTrackPlusEcalPlusHcalIsoR05 ); 
  AddOutput(fNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05 ); 
  AddOutput(fMuonTrackIsoR05Eff ); 
  AddOutput(fMuonEcalIsoR05Eff ); 
  AddOutput(fMuonHcalIsoR05Eff ); 
  AddOutput(fMuonEcalPlusHcalIsoR05Eff ); 
  AddOutput(fMuonOnePointFiveEcalPlusHcalIsoR05Eff ); 
  AddOutput(fMuonTrackPlusEcalPlusHcalIsoR05Eff ); 
  AddOutput(fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05Eff ); 
  AddOutput(fMuonTwoTrackPlusEcalPlusHcalIsoR05Eff ); 
  AddOutput(fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05Eff ); 
  AddOutput(fMuonTrackIsoR05OverPt ); 
  AddOutput(fMuonEcalIsoR05OverPt ); 
  AddOutput(fMuonHcalIsoR05OverPt ); 
  AddOutput(fMuonEcalPlusHcalIsoR05OverPt ); 
  AddOutput(fMuonOnePointFiveEcalPlusHcalIsoR05OverPt ); 
  AddOutput(fMuonTrackPlusEcalPlusHcalIsoR05OverPt ); 
  AddOutput(fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt ); 
  AddOutput(fMuonTwoTrackPlusEcalPlusHcalIsoR05OverPt ); 
  AddOutput(fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt ); 
  AddOutput(fNMuonPassTrackIsoR05OverPt ); 
  AddOutput(fNMuonPassEcalIsoR05OverPt ); 
  AddOutput(fNMuonPassHcalIsoR05OverPt ); 
  AddOutput(fNMuonPassEcalPlusHcalIsoR05OverPt ); 
  AddOutput(fNMuonPassOnePointFiveEcalPlusHcalIsoR05OverPt ); 
  AddOutput(fNMuonPassTrackPlusEcalPlusHcalIsoR05OverPt ); 
  AddOutput(fNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt ); 
  AddOutput(fNMuonPassTwoTrackPlusEcalPlusHcalIsoR05OverPt ); 
  AddOutput(fNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt ); 
  AddOutput(fMuonTrackIsoR05OverPtEff ); 
  AddOutput(fMuonEcalIsoR05OverPtEff ); 
  AddOutput(fMuonHcalIsoR05OverPtEff ); 
  AddOutput(fMuonEcalPlusHcalIsoR05OverPtEff ); 
  AddOutput(fMuonOnePointFiveEcalPlusHcalIsoR05OverPtEff ); 
  AddOutput(fMuonTrackPlusEcalPlusHcalIsoR05OverPtEff ); 
  AddOutput(fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPtEff ); 
  AddOutput(fMuonTwoTrackPlusEcalPlusHcalIsoR05OverPtEff ); 
  AddOutput(fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPtEff );
  AddOutput(fMuonTrackIso03VsPt); 
  AddOutput(fMuonEcalIso03VsPt); 
  AddOutput(fMuonHcalIso03VsPt); 
  AddOutput(fMuonTrackPlusEcalPlusHcalIso03VsPt);


}

//--------------------------------------------------------------------------------------------------
void MuonIsolationStudyMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.

  cerr << "Number of Events" << fNEventsProcessed  << endl;


  double muonWeight = 1 / (fNMuons + 0.0);
  cerr << "weight = " << muonWeight << endl;

  //1D iso variables
  for (int i=1;i<=101;i++) {    
    double isolation = 0.0 + i*0.2;   
    //Number of Muons Passed
    fNMuonPassTrackIsoR03->Fill(isolation,fMuonTrackIsoR03->Integral(0,i));
    fNMuonPassEcalIsoR03->Fill(isolation,fMuonEcalIsoR03->Integral(0,i));
    fNMuonPassHcalIsoR03->Fill(isolation,  fMuonHcalIsoR03->Integral(0,i));
    fNMuonPassEcalPlusHcalIsoR03->Fill(isolation,fMuonEcalPlusHcalIsoR03->Integral(0,i));
    fNMuonPassOnePointFiveEcalPlusHcalIsoR03->Fill(
      isolation,fMuonOnePointFiveEcalPlusHcalIsoR03->Integral(0,i));   
    fNMuonPassTrackPlusEcalPlusHcalIsoR03->Fill(
      isolation,fMuonTrackPlusEcalPlusHcalIsoR03->Integral(0,i));   
    fNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR03->Fill(
      isolation,fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03->Integral(0,i));
    fNMuonPassTwoTrackPlusEcalPlusHcalIsoR03->Fill(
      isolation,fMuonTwoTrackPlusEcalPlusHcalIsoR03->Integral(0,i));   
    fNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03->Fill(
      isolation,fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03->Integral(0,i));   
    fNMuonPassTrackIsoR05->Fill(isolation,fMuonTrackIsoR05->Integral(0,i));
    fNMuonPassEcalIsoR05->Fill(isolation,fMuonEcalIsoR05->Integral(0,i));
    fNMuonPassHcalIsoR05->Fill(isolation,  fMuonHcalIsoR05->Integral(0,i));
    fNMuonPassEcalPlusHcalIsoR05->Fill(isolation,fMuonEcalPlusHcalIsoR05->Integral(0,i));
    fNMuonPassOnePointFiveEcalPlusHcalIsoR05->Fill(
      isolation,fMuonOnePointFiveEcalPlusHcalIsoR05->Integral(0,i));   
    fNMuonPassTrackPlusEcalPlusHcalIsoR05->Fill(
      isolation,fMuonTrackPlusEcalPlusHcalIsoR05->Integral(0,i));   
    fNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR05->Fill(
      isolation,fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05->Integral(0,i));
    fNMuonPassTwoTrackPlusEcalPlusHcalIsoR05->Fill(
      isolation,fMuonTwoTrackPlusEcalPlusHcalIsoR05->Integral(0,i));   
    fNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05->Fill(
      isolation,fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05->Integral(0,i));   
    //Efficiency
    fMuonTrackIsoR03Eff->Fill(isolation,muonWeight*fMuonTrackIsoR03->Integral(0,i));
    fMuonEcalIsoR03Eff->Fill(isolation,muonWeight*fMuonEcalIsoR03->Integral(0,i));
    fMuonHcalIsoR03Eff->Fill(isolation,muonWeight*fMuonHcalIsoR03->Integral(0,i));
    fMuonEcalPlusHcalIsoR03Eff->Fill(isolation,muonWeight*fMuonEcalPlusHcalIsoR03->Integral(0,i));
    fMuonOnePointFiveEcalPlusHcalIsoR03Eff->Fill(
      isolation,muonWeight*fMuonOnePointFiveEcalPlusHcalIsoR03->Integral(0,i));   
    fMuonTrackPlusEcalPlusHcalIsoR03Eff->Fill(
      isolation,muonWeight*fMuonTrackPlusEcalPlusHcalIsoR03->Integral(0,i));   
    fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03Eff->Fill(
      isolation,muonWeight*fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03->Integral(0,i));
    fMuonTwoTrackPlusEcalPlusHcalIsoR03Eff->Fill(
      isolation,muonWeight*fMuonTwoTrackPlusEcalPlusHcalIsoR03->Integral(0,i));   
    fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03Eff->Fill(
      isolation,muonWeight*fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03->Integral(0,i));   
    fMuonTrackIsoR05Eff->Fill(isolation,muonWeight*fMuonTrackIsoR05->Integral(0,i));
    fMuonEcalIsoR05Eff->Fill(isolation,muonWeight*fMuonEcalIsoR05->Integral(0,i));
    fMuonHcalIsoR05Eff->Fill(isolation,muonWeight*fMuonHcalIsoR05->Integral(0,i));
    fMuonEcalPlusHcalIsoR05Eff->Fill(isolation,muonWeight*fMuonEcalPlusHcalIsoR05->Integral(0,i));
    fMuonOnePointFiveEcalPlusHcalIsoR05Eff->Fill(
      isolation,muonWeight*fMuonOnePointFiveEcalPlusHcalIsoR05->Integral(0,i));   
    fMuonTrackPlusEcalPlusHcalIsoR05Eff->Fill(
      isolation,muonWeight*fMuonTrackPlusEcalPlusHcalIsoR05->Integral(0,i));   
    fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05Eff->Fill(
      isolation,muonWeight*fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05->Integral(0,i));
    fMuonTwoTrackPlusEcalPlusHcalIsoR05Eff->Fill(
      isolation,muonWeight*fMuonTwoTrackPlusEcalPlusHcalIsoR05->Integral(0,i));   
    fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05Eff->Fill(
      isolation,muonWeight*fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05->Integral(0,i));   
  }

  //1D iso/Pt variables
  for (int i=1;i<=201;i++) {    
    double relativeIsolation = 0.0 + i*0.02;
    //Number of Muons Passed    
    fNMuonPassTrackIsoR03OverPt->Fill(relativeIsolation,fMuonTrackIsoR03OverPt->Integral(0,i));
    fNMuonPassEcalIsoR03OverPt->Fill(relativeIsolation,fMuonEcalIsoR03OverPt->Integral(0,i));
    fNMuonPassHcalIsoR03OverPt->Fill(relativeIsolation,  fMuonHcalIsoR03OverPt->Integral(0,i));
    fNMuonPassEcalPlusHcalIsoR03OverPt->Fill(relativeIsolation,fMuonEcalPlusHcalIsoR03OverPt->Integral(0,i));
    fNMuonPassOnePointFiveEcalPlusHcalIsoR03OverPt->Fill(
      relativeIsolation,fMuonOnePointFiveEcalPlusHcalIsoR03OverPt->Integral(0,i));   
    fNMuonPassTrackPlusEcalPlusHcalIsoR03OverPt->Fill(
      relativeIsolation,fMuonTrackPlusEcalPlusHcalIsoR03OverPt->Integral(0,i));   
    fNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt->Fill(
      relativeIsolation,fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt->Integral(0,i));
    fNMuonPassTwoTrackPlusEcalPlusHcalIsoR03OverPt->Fill(
      relativeIsolation,fMuonTwoTrackPlusEcalPlusHcalIsoR03OverPt->Integral(0,i));   
    fNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt->Fill(
      relativeIsolation,fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt->Integral(0,i));   
    fNMuonPassTrackIsoR05OverPt->Fill(relativeIsolation,fMuonTrackIsoR05OverPt->Integral(0,i));
    fNMuonPassEcalIsoR05OverPt->Fill(relativeIsolation,fMuonEcalIsoR05OverPt->Integral(0,i));
    fNMuonPassHcalIsoR05OverPt->Fill(relativeIsolation,  fMuonHcalIsoR05OverPt->Integral(0,i));
    fNMuonPassEcalPlusHcalIsoR05OverPt->Fill(relativeIsolation,fMuonEcalPlusHcalIsoR05OverPt->Integral(0,i));
    fNMuonPassOnePointFiveEcalPlusHcalIsoR05OverPt->Fill(
      relativeIsolation,fMuonOnePointFiveEcalPlusHcalIsoR05OverPt->Integral(0,i));   
    fNMuonPassTrackPlusEcalPlusHcalIsoR05OverPt->Fill(
      relativeIsolation,fMuonTrackPlusEcalPlusHcalIsoR05OverPt->Integral(0,i));   
    fNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt->Fill(
      relativeIsolation,fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt->Integral(0,i));
    fNMuonPassTwoTrackPlusEcalPlusHcalIsoR05OverPt->Fill(
      relativeIsolation,fMuonTwoTrackPlusEcalPlusHcalIsoR05OverPt->Integral(0,i));   
    fNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt->Fill(
      relativeIsolation,fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt->Integral(0,i));   
    //Efficiency  
    fMuonTrackIsoR03OverPtEff->Fill(relativeIsolation,muonWeight*fMuonTrackIsoR03OverPt->Integral(0,i));
    fMuonEcalIsoR03OverPtEff->Fill(relativeIsolation,muonWeight*fMuonEcalIsoR03OverPt->Integral(0,i));
    fMuonHcalIsoR03OverPtEff->Fill(relativeIsolation,muonWeight*fMuonHcalIsoR03OverPt->Integral(0,i));
    fMuonEcalPlusHcalIsoR03OverPtEff->Fill(relativeIsolation,muonWeight*fMuonEcalPlusHcalIsoR03OverPt->Integral(0,i));
    fMuonOnePointFiveEcalPlusHcalIsoR03OverPtEff->Fill(
      relativeIsolation,muonWeight*fMuonOnePointFiveEcalPlusHcalIsoR03OverPt->Integral(0,i));   
    fMuonTrackPlusEcalPlusHcalIsoR03OverPtEff->Fill(
      relativeIsolation,muonWeight*fMuonTrackPlusEcalPlusHcalIsoR03OverPt->Integral(0,i));   
    fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPtEff->Fill(
      relativeIsolation,muonWeight*fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt->Integral(0,i));
    fMuonTwoTrackPlusEcalPlusHcalIsoR03OverPtEff->Fill(
      relativeIsolation,muonWeight*fMuonTwoTrackPlusEcalPlusHcalIsoR03OverPt->Integral(0,i));   
    fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPtEff->Fill(
      relativeIsolation,muonWeight*fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt->Integral(0,i));   
    fMuonTrackIsoR05OverPtEff->Fill(relativeIsolation,muonWeight*fMuonTrackIsoR05OverPt->Integral(0,i));
    fMuonEcalIsoR05OverPtEff->Fill(relativeIsolation,muonWeight*fMuonEcalIsoR05OverPt->Integral(0,i));
    fMuonHcalIsoR05OverPtEff->Fill(relativeIsolation,muonWeight*fMuonHcalIsoR05OverPt->Integral(0,i));
    fMuonEcalPlusHcalIsoR05OverPtEff->Fill(relativeIsolation,muonWeight*fMuonEcalPlusHcalIsoR05OverPt->Integral(0,i));
    fMuonOnePointFiveEcalPlusHcalIsoR05OverPtEff->Fill(
      relativeIsolation,muonWeight*fMuonOnePointFiveEcalPlusHcalIsoR05OverPt->Integral(0,i));   
    fMuonTrackPlusEcalPlusHcalIsoR05OverPtEff->Fill(
      relativeIsolation,muonWeight*fMuonTrackPlusEcalPlusHcalIsoR05OverPt->Integral(0,i));   
    fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPtEff->Fill(
      relativeIsolation,muonWeight*fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt->Integral(0,i));
    fMuonTwoTrackPlusEcalPlusHcalIsoR05OverPtEff->Fill(
      relativeIsolation,muonWeight*fMuonTwoTrackPlusEcalPlusHcalIsoR05OverPt->Integral(0,i));   
    fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPtEff->Fill(
      relativeIsolation,muonWeight*fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt->Integral(0,i));   
  }

  //2D Efficiencies
  for (int t = 0;t <= 101; t++) {
    double trackIsoCut = 0.0 + t*0.2;
    for (int c = 0; c <= 101 ; c++ ) {      
      double caloIsoCut = 0.0 + c*0.2;
      fMuonTrackIsoVsEcalIsoEffR03->Fill(trackIsoCut,caloIsoCut,muonWeight*fNMuonPassTrackIsoVsEcalIsoR03->GetBinContent(t+1,c+1));
      fMuonTrackIsoVsHcalIsoEffR03->Fill(trackIsoCut,caloIsoCut,muonWeight*fNMuonPassTrackIsoVsHcalIsoR03->GetBinContent(t+1,c+1));
      fMuonTrackIsoVsEcalPlusHcalIsoEffR03->Fill(trackIsoCut,caloIsoCut,muonWeight*fNMuonPassTrackIsoVsEcalPlusHcalIsoR03->GetBinContent(t+1,c+1));
      fMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoEffR03->Fill(trackIsoCut,caloIsoCut,muonWeight*fNMuonPassTrackIsoVsOnePointFiveEcalPlusHcalIsoR03->GetBinContent(t+1,c+1));
      fMuonTrackIsoVsEcalIsoEffR05->Fill(trackIsoCut,caloIsoCut,muonWeight*fNMuonPassTrackIsoVsEcalIsoR05->GetBinContent(t+1,c+1));
      fMuonTrackIsoVsHcalIsoEffR05->Fill(trackIsoCut,caloIsoCut,muonWeight*fNMuonPassTrackIsoVsHcalIsoR05->GetBinContent(t+1,c+1));
      fMuonTrackIsoVsEcalPlusHcalIsoEffR05->Fill(trackIsoCut,caloIsoCut,muonWeight*fNMuonPassTrackIsoVsEcalPlusHcalIsoR05->GetBinContent(t+1,c+1));
      fMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoEffR05->Fill(trackIsoCut,caloIsoCut,muonWeight*fNMuonPassTrackIsoVsOnePointFiveEcalPlusHcalIsoR05->GetBinContent(t+1,c+1));
    }       
  }

  cerr << "efficiency of default cuts : " << fNMuonsPassDefaultIsolation << " / " << fNMuons << " = " << fNMuonsPassDefaultIsolation / (0.0+fNMuons) << endl;

}

//--------------------------------------------------------------------------------------------------
void MuonIsolationStudyMod::Terminate()
{
  // Run finishing code on the client computer. For this module, we dont do
  // anything here.
}
