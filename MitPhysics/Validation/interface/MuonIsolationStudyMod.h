//--------------------------------------------------------------------------------------------------
// $Id: MuonIsolationStudyMod.h,v 1.1 2008/11/12 15:04:50 sixie Exp $
//
// MuonIsolationStudyMod
//
// This Module applies electron ID criteria and exports a pointer to a collection
// of Good Muons according to some specified ID scheme
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_VALIDATION_MUONISOLATIONSTUDYMOD_H
#define MITANA_VALIDATION_MUONISOLATIONSTUDYMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;

namespace mithep 
{
  class MuonIsolationStudyMod : public BaseMod
  {
    public:
      MuonIsolationStudyMod(const char *name="MuonIsolationStudyMod", 
                     const char *title="Example analysis module with all branches");
      ~MuonIsolationStudyMod() {}
      void        SetPrintDebug(bool b)                   { fPrintDebug           = b;    }
      void        SetMatchToRealMuons(bool b)             { fMatchToRealMuons     = b;    }        
      void        SetImposeMuonID(bool b)                 { fImposeMuonID         = b;    }        
      void        SetMuonPtMin(double min)                { fMuonPtMin            = min;  }        
      void        SetMuonPtMax(double max)                { fMuonPtMax            = max;  }        

    protected:
      bool                     fPrintDebug;            //flag for printing debug output
      TString                  fMuonName;              //name of electron collection
      TString                  fMCLeptonsName ;        // new lepton coll
      MuonCol	               *fMuons;                //!Muon branch
      int                      fNEventsProcessed;      // Number of events processed
      bool                     fMatchToRealMuons;    
      bool                     fImposeMuonID;    

      double                   fIDLikelihoodCut;       //Cut value for ID likelihood
      double                   fMuonPtMin;             //defined the range of
      double                   fMuonPtMax;             //pts for muons under study

      int                      fNMuons;
      int                      fNMuonsPassDefaultIsolation;

      TH1D                    *fMuonPtHist;
      TH1D                    *fMuonEtaHist;
      TH2D                    *fMuonTrackIsoVsEcalIsoR03 ; 
      TH2D                    *fMuonTrackIsoVsHcalIsoR03 ; 
      TH2D                    *fMuonTrackIsoVsEcalPlusHcalIsoR03 ; 
      TH2D                    *fMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoR03 ; 
      TH2D                    *fNMuonPassTrackIsoVsEcalIsoR03 ; 
      TH2D                    *fNMuonPassTrackIsoVsHcalIsoR03 ; 
      TH2D                    *fNMuonPassTrackIsoVsEcalPlusHcalIsoR03 ; 
      TH2D                    *fNMuonPassTrackIsoVsOnePointFiveEcalPlusHcalIsoR03 ; 
      TH2D                    *fMuonTrackIsoVsEcalIsoEffR03 ; 
      TH2D                    *fMuonTrackIsoVsHcalIsoEffR03 ; 
      TH2D                    *fMuonTrackIsoVsEcalPlusHcalIsoEffR03 ; 
      TH2D                    *fMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoEffR03 ; 
      TH1D                    *fMuonTrackIsoR03 ; 
      TH1D                    *fMuonEcalIsoR03 ; 
      TH1D                    *fMuonHcalIsoR03 ; 
      TH1D                    *fMuonEcalPlusHcalIsoR03 ; 
      TH1D                    *fMuonOnePointFiveEcalPlusHcalIsoR03 ; 
      TH1D                    *fMuonTrackPlusEcalPlusHcalIsoR03 ; 
      TH1D                    *fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03 ; 
      TH1D                    *fMuonTwoTrackPlusEcalPlusHcalIsoR03 ; 
      TH1D                    *fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03 ; 
      TH1D                    *fMuonTrackIsoR03OverPt ; 
      TH1D                    *fMuonEcalIsoR03OverPt ; 
      TH1D                    *fMuonHcalIsoR03OverPt ; 
      TH1D                    *fMuonEcalPlusHcalIsoR03OverPt ; 
      TH1D                    *fMuonOnePointFiveEcalPlusHcalIsoR03OverPt ; 
      TH1D                    *fMuonTrackPlusEcalPlusHcalIsoR03OverPt ; 
      TH1D                    *fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt ; 
      TH1D                    *fMuonTwoTrackPlusEcalPlusHcalIsoR03OverPt ; 
      TH1D                    *fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt ; 
      TH1D                    *fNMuonPassTrackIsoR03 ; 
      TH1D                    *fNMuonPassEcalIsoR03 ; 
      TH1D                    *fNMuonPassHcalIsoR03 ; 
      TH1D                    *fNMuonPassEcalPlusHcalIsoR03 ; 
      TH1D                    *fNMuonPassOnePointFiveEcalPlusHcalIsoR03 ; 
      TH1D                    *fNMuonPassTrackPlusEcalPlusHcalIsoR03 ; 
      TH1D                    *fNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR03 ; 
      TH1D                    *fNMuonPassTwoTrackPlusEcalPlusHcalIsoR03 ; 
      TH1D                    *fNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03 ; 
      TH1D                    *fNMuonPassTrackIsoR03OverPt ; 
      TH1D                    *fNMuonPassEcalIsoR03OverPt ; 
      TH1D                    *fNMuonPassHcalIsoR03OverPt ; 
      TH1D                    *fNMuonPassEcalPlusHcalIsoR03OverPt ; 
      TH1D                    *fNMuonPassOnePointFiveEcalPlusHcalIsoR03OverPt ; 
      TH1D                    *fNMuonPassTrackPlusEcalPlusHcalIsoR03OverPt ; 
      TH1D                    *fNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt ; 
      TH1D                    *fNMuonPassTwoTrackPlusEcalPlusHcalIsoR03OverPt ; 
      TH1D                    *fNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPt ; 
      TH1D                    *fMuonTrackIsoR03Eff ; 
      TH1D                    *fMuonEcalIsoR03Eff ; 
      TH1D                    *fMuonHcalIsoR03Eff ; 
      TH1D                    *fMuonEcalPlusHcalIsoR03Eff ; 
      TH1D                    *fMuonOnePointFiveEcalPlusHcalIsoR03Eff ; 
      TH1D                    *fMuonTrackPlusEcalPlusHcalIsoR03Eff ; 
      TH1D                    *fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03Eff ; 
      TH1D                    *fMuonTwoTrackPlusEcalPlusHcalIsoR03Eff ; 
      TH1D                    *fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03Eff ; 
      TH1D                    *fMuonTrackIsoR03OverPtEff ; 
      TH1D                    *fMuonEcalIsoR03OverPtEff ; 
      TH1D                    *fMuonHcalIsoR03OverPtEff ; 
      TH1D                    *fMuonEcalPlusHcalIsoR03OverPtEff ; 
      TH1D                    *fMuonOnePointFiveEcalPlusHcalIsoR03OverPtEff ; 
      TH1D                    *fMuonTrackPlusEcalPlusHcalIsoR03OverPtEff ; 
      TH1D                    *fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPtEff ; 
      TH1D                    *fMuonTwoTrackPlusEcalPlusHcalIsoR03OverPtEff ; 
      TH1D                    *fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR03OverPtEff ; 
      TH2D                    *fMuonTrackIsoVsEcalIsoR05 ; 
      TH2D                    *fMuonTrackIsoVsHcalIsoR05 ; 
      TH2D                    *fMuonTrackIsoVsEcalPlusHcalIsoR05 ; 
      TH2D                    *fMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoR05 ; 
      TH2D                    *fNMuonPassTrackIsoVsEcalIsoR05 ; 
      TH2D                    *fNMuonPassTrackIsoVsHcalIsoR05 ; 
      TH2D                    *fNMuonPassTrackIsoVsEcalPlusHcalIsoR05 ; 
      TH2D                    *fNMuonPassTrackIsoVsOnePointFiveEcalPlusHcalIsoR05 ; 
      TH2D                    *fMuonTrackIsoVsEcalIsoEffR05 ; 
      TH2D                    *fMuonTrackIsoVsHcalIsoEffR05 ; 
      TH2D                    *fMuonTrackIsoVsEcalPlusHcalIsoEffR05 ; 
      TH2D                    *fMuonTrackIsoVsOnePointFiveEcalPlusHcalIsoEffR05 ; 
      TH1D                    *fMuonTrackIsoR05 ; 
      TH1D                    *fMuonEcalIsoR05 ; 
      TH1D                    *fMuonHcalIsoR05 ; 
      TH1D                    *fMuonEcalPlusHcalIsoR05 ; 
      TH1D                    *fMuonOnePointFiveEcalPlusHcalIsoR05 ; 
      TH1D                    *fMuonTrackPlusEcalPlusHcalIsoR05 ; 
      TH1D                    *fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05 ; 
      TH1D                    *fMuonTwoTrackPlusEcalPlusHcalIsoR05 ; 
      TH1D                    *fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05 ; 
      TH1D                    *fMuonTrackIsoR05OverPt ; 
      TH1D                    *fMuonEcalIsoR05OverPt ; 
      TH1D                    *fMuonHcalIsoR05OverPt ; 
      TH1D                    *fMuonEcalPlusHcalIsoR05OverPt ; 
      TH1D                    *fMuonOnePointFiveEcalPlusHcalIsoR05OverPt ; 
      TH1D                    *fMuonTrackPlusEcalPlusHcalIsoR05OverPt ; 
      TH1D                    *fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt ; 
      TH1D                    *fMuonTwoTrackPlusEcalPlusHcalIsoR05OverPt ; 
      TH1D                    *fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt ; 
      TH1D                    *fNMuonPassTrackIsoR05 ; 
      TH1D                    *fNMuonPassEcalIsoR05 ; 
      TH1D                    *fNMuonPassHcalIsoR05 ; 
      TH1D                    *fNMuonPassEcalPlusHcalIsoR05 ; 
      TH1D                    *fNMuonPassOnePointFiveEcalPlusHcalIsoR05 ; 
      TH1D                    *fNMuonPassTrackPlusEcalPlusHcalIsoR05 ; 
      TH1D                    *fNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR05 ; 
      TH1D                    *fNMuonPassTwoTrackPlusEcalPlusHcalIsoR05 ; 
      TH1D                    *fNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05 ; 
      TH1D                    *fNMuonPassTrackIsoR05OverPt ; 
      TH1D                    *fNMuonPassEcalIsoR05OverPt ; 
      TH1D                    *fNMuonPassHcalIsoR05OverPt ; 
      TH1D                    *fNMuonPassEcalPlusHcalIsoR05OverPt ; 
      TH1D                    *fNMuonPassOnePointFiveEcalPlusHcalIsoR05OverPt ; 
      TH1D                    *fNMuonPassTrackPlusEcalPlusHcalIsoR05OverPt ; 
      TH1D                    *fNMuonPassTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt ; 
      TH1D                    *fNMuonPassTwoTrackPlusEcalPlusHcalIsoR05OverPt ; 
      TH1D                    *fNMuonPassTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPt ; 
      TH1D                    *fMuonTrackIsoR05Eff ; 
      TH1D                    *fMuonEcalIsoR05Eff ; 
      TH1D                    *fMuonHcalIsoR05Eff ; 
      TH1D                    *fMuonEcalPlusHcalIsoR05Eff ; 
      TH1D                    *fMuonOnePointFiveEcalPlusHcalIsoR05Eff ; 
      TH1D                    *fMuonTrackPlusEcalPlusHcalIsoR05Eff ; 
      TH1D                    *fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05Eff ; 
      TH1D                    *fMuonTwoTrackPlusEcalPlusHcalIsoR05Eff ; 
      TH1D                    *fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05Eff ; 
      TH1D                    *fMuonTrackIsoR05OverPtEff ; 
      TH1D                    *fMuonEcalIsoR05OverPtEff ; 
      TH1D                    *fMuonHcalIsoR05OverPtEff ; 
      TH1D                    *fMuonEcalPlusHcalIsoR05OverPtEff ; 
      TH1D                    *fMuonOnePointFiveEcalPlusHcalIsoR05OverPtEff ; 
      TH1D                    *fMuonTrackPlusEcalPlusHcalIsoR05OverPtEff ; 
      TH1D                    *fMuonTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPtEff ; 
      TH1D                    *fMuonTwoTrackPlusEcalPlusHcalIsoR05OverPtEff ; 
      TH1D                    *fMuonTwoTrackPlusOnePointFiveEcalPlusHcalIsoR05OverPtEff ; 
      TH2D                    *fMuonTrackIso03VsPt ;
      TH2D                    *fMuonEcalIso03VsPt ;
      TH2D                    *fMuonHcalIso03VsPt ;
      TH2D                    *fMuonTrackPlusEcalPlusHcalIso03VsPt ;

      void        Begin();
      void        Process();
      void        SlaveBegin();
      void        SlaveTerminate();
      void        Terminate();
      
    
      ClassDef(MuonIsolationStudyMod,1) // TAM example analysis module
  };
}
#endif
