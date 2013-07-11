//--------------------------------------------------------------------------------------------------
// $Id: SingleFakeValidationMod.h,v 1.1 2009/02/15 18:11:40 sixie Exp $
//
// SingleFakeValidationMod
//
// A Module for Selecting H->WW events
// and produces some distributions.
//
//
// Authors: Si Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITPHYSICS_VALIDATION_SINGLEFAKEVALIDATION_H
#define MITPHYSICS_VALIDATION_SINGLEFAKEVALIDATION_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;

namespace mithep 
{
  class SingleFakeValidationMod : public BaseMod
  {
    public:
    SingleFakeValidationMod(const char *name="SingleFakeValidationMod", 
		 const char *title="Example analysis module with all branches");
      ~SingleFakeValidationMod() {}
      const char   *GetVertexBranchName()          const { return fVertexBranchName;      }

      void         SetCleanJetsName (TString s)          { fCleanJetsName = s;            }
      void         SetJetName (TString s)                { fJetName = s;                  }
      void         SetVertexBranchName(const char *name) { fVertexBranchName      = name; }

    protected:
      bool                     fPrintDebug;
      TString                  fMCPartName;            //name of particle collection
      TString                  fMetName;               //name of met collection
      TString                  fTrackName;          
      TString                  fJetName;          
      TString                  fVertexBranchName;     //name of vertex branch          
      TString                  fCleanJetsName;        //name of clean central jets collection
      TString                  fMuonName;	      // name of muon collection
      const MCParticleCol     *fParticles;            //!GenParticle branch
      const MetCol            *fMet;                  //!Missing Et
      const TrackCol	      *fTracks;               //!Track branch      
      const JetCol            *fJets;                 //!Jet branch
      const MuonCol	      *fMuons;	              //!Muon branch
      const VertexCol         *fVertex;               //!Vertex branch


      TH1D *fElectronDenominatorEt;
      TH1D *fElectronDenominatorEta;
      TH1D *fElectronDenominatorPhi;
      TH1D *fMuonDenominatorPt;
      TH1D *fMuonDenominatorEta;
      TH1D *fMuonDenominatorPhi;
      TH1D *fDenominatorMt;
      TH1D *fDenominatorMet;

      TH1D *fFakeElectronEt;
      TH1D *fFakeElectronEta;
      TH1D *fFakeElectronPhi;
      TH1D *fFakeMuonPt;
      TH1D *fFakeMuonEta;
      TH1D *fFakeMuonPhi;
      TH1D *fFakeMt;
      TH1D *fFakeMet;

      TH1D *fMCFakeElectronEt;
      TH1D *fMCFakeElectronEta;
      TH1D *fMCFakeElectronPhi;
      TH1D *fMCFakeMuonPt;
      TH1D *fMCFakeMuonEta;
      TH1D *fMCFakeMuonPhi;
      TH1D *fMCFakeMt;
      TH1D *fMCFakeMet;

      void         Begin();
      void         Process();
      void         SlaveBegin();
      void         SlaveTerminate();
      void         Terminate();      

      ClassDef(SingleFakeValidationMod,1) // TAM example analysis module
  };
}
#endif
