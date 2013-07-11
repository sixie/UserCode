//------------------------------------------------------------------------------
// $Id: ttEvtSelMod.h,v 1.1 2008/11/12 15:04:49 sixie Exp $
//
// ttEvtSelMod
//
// A Module for Selecting ttbar events
// and produces some distributions
//
//
// Authors: ceballos
//------------------------------------------------------------------------------

#ifndef HWWMODS_TTEVTSELMOD_H
#define HWWMODS_TTEVTSELMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;

namespace mithep 
{
  class ttEvtSelMod : public BaseMod
  {
    public:
    ttEvtSelMod(const char *name="ttEvtSelMod", 
		 const char *title="Example analysis module with all branches");
      ~ttEvtSelMod() {}
      void      SetPrintDebug(bool b)	       { fPrintDebug = b;      }   
      void      SetCleanJetsName (TString s)   { fCleanJetsName = s;   }   
      void      SetMCLeptonsName(TString s)    { fMCLeptonsName = s;   }     
      void      SetMCAllLeptonsName(TString s) { fMCAllLeptonsName = s;}     
      void      SetMCQuarksName(TString s)     { fMCQuarksName = s;    }     

    protected:
      bool      fPrintDebug;	   // Debug info
      TString   fPlotType;	   // Type of histograms to make
      TString   fMetName;	   // name of met collection
      TString   fMuonName;	   // name of muon collection
      TString   fTrackName;	   // name of track collection
      TString   fVertexName;	   // name of vertex collection
      TString   fCleanJetsName;    // name of clean central jets collection
      TString   fMCLeptonsName;    // new lepton coll (from W)
      TString   fMCAllLeptonsName; // new lepton coll (all)
      TString   fMCQuarksName;     // new Quarks coll
      MetCol    *fMet;  	   // Missing Et
      MuonCol	*fMuons;	   // Muon branch
      TrackCol  *fTracks;	   // Track branch     
      VertexCol *fVertices;	   // Vertices branches

      TH1D      *hDttPresel[300];
      TH1D      *hDttDiJetSel[300];
      TH1D      *hDttNoJetsSel[300];
      TH1D      *hDttJetVeto[10];
      TH2D      *hDttPreselAlphaEP;
      TH1D      *hDttVert[5];
      TH2D      *hDttSelD0Phi;

      int       fNEventsProcessed;

      void      Begin();
      void      Process();
      void      SlaveBegin();
      void      SlaveTerminate();
      void      Terminate();	  

      double    DecayXY(const mithep::Track *lTrack, mithep::Vertex *iVertex);
      double    DecayXY(const mithep::Track *lTrack, mithep::VertexCol *iVertices);

      ClassDef(ttEvtSelMod,1) // TAM example analysis module
  };
}
#endif
