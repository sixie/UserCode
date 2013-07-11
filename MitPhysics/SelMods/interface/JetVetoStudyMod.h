//--------------------------------------------------------------------------------------------------
// $Id: JetVetoStudyMod.h,v 1.1 2008/11/12 15:04:49 sixie Exp $
//
// NJetStudyMod
//
// A Module for Selecting ttbar events
// and produces some distributions
//
//
// Authors: ceballos
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_PHYSICSMOD_JETVETOSTUDYMOD_H
#define MITANA_PHYSICSMOD_JETVETOSTUDYMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;

namespace mithep 
{
  class JetVetoStudyMod : public BaseMod
  {
    public:
    JetVetoStudyMod(const char *name="JetVetoStudyMod", 
		 const char *title="Example analysis module with all branches");
      ~JetVetoStudyMod() {}
      void         SetLoadGenParticles(bool b)           { fLoadGenParticles = b;         }        
      void         SetPrintDebug(bool b)                 { fPrintDebug = b;               }   
      void         SetPlotType(TString type)             { fPlotType = type;              }   
      void         SetCleanJetsName (TString s)          { fCleanJetsName = s;            }   
      void         SetJetName (TString s)                { fJetName = s;                  }   
      void         SetGenJetName (TString s)             { fGenJetName = s;               }   
      void         SetImposeSelectionCuts (bool b)       { fImposeSelectionCuts = b;      }   
      void         SetDoJetCorrections (bool b)          { fDoJetCorrections = b;         }   
      void         SetImposeDirtyMuonVeto (bool b)       { fImposeDirtyMuonVeto = b;      }   

    protected:
      bool                     fPrintDebug;
      bool                     fImposeSelectionCuts;
      bool                     fDoJetCorrections;
      bool                     fImposeDirtyMuonVeto;
      TString                  fPlotType;              //Type of histograms to make
      TString                  fMetName;               //name of met collection
      TString                  fMuonName;              //name of muon collection
      TString                  fCleanJetsName;         //name of clean central jets collection
      TString                  fTrackName;          
      TString                  fJetName;          
      TString                  fGenJetName;   
      TString                  fMCPartName;               // generator coll
      TString                  fMCLeptonsName ;           // new lepton coll
      TString                  fMCTausName;               // new tau coll
      TString                  fMCNeutrinosName;          // new Neutrinos coll
      TString                  fMCQuarksName;             // new Quarks coll
      TString                  fMCqqHsName;               // new qqH coll
      TString                  fMCBosonsName;             // new Bosons coll

      bool                     fLoadGenParticles;     //bool to determine if we want gen info
      MetCol                  *fMet;                  //!Missing Et
      MuonCol		      *fMuons;                //!Muon branch
      TrackCol		      *fTracks;               //!Track branch     
      JetCol                  *fJets;                 //!Jet branch
      GenJetCol               *fGenJets;                 //!Jet branch
      MCParticleCol           *fParticles;                // GenParticle branch
      
      TH2D                    *fJetAndTrackVetoEff;
      TH2D                    *fJetOrTrackVetoEff;
      TH2D                    *fNEventsPassJetAndTrackVetoHist;
      TH2D                    *fNEventsPassJetOrTrackVetoHist;
      TH1D                    *fJetVetoEffVsJetEtCut;
      TH1D                    *fTrackVetoEff;
      TH1D                    *fTrackVetoEffAfterJetVeto;
      TH1D                    *fNEventsPassJetVetoVsJetEtCutHist;
      TH1D                    *fNEventsPassTrackVetoHist;
      TH1D                    *fNEventsPassTrackVetoAfterJetVetoHist;
      TH1D                    *fNTracksNoGenJets;
      TH1D                    *fNTracksWithGenJets;

      int                      fNEventsPassPreSelection;
      int                      fNEventsPassSelection;
      int                      fNEventsPassJetVeto;
      int                      fNEventsProcessed;
      int                      fNEventsPassJetAndTrackVeto[30][100];
      int                      fNEventsPassJetOrTrackVeto[30][100];

      void         Begin();
      void         Process();
      void         SlaveBegin();
      void         SlaveTerminate();
      void         Terminate();      

      ClassDef(JetVetoStudyMod,1) // TAM example analysis module
  };
}
#endif
