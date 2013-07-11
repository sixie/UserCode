//--------------------------------------------------------------------------------------------------
// $Id: ElectronIsolationStudyMod.h,v 1.3 2008/12/04 19:19:50 sixie Exp $
//
// ElectronIsolationStudyMod
//
// This Module applies electron ID criteria and exports a pointer to a collection
// of Good Electrons according to some specified ID scheme
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITHIGGS_ISOLATIONSTUDYMODS_ELECTRONISOLATIONSTUDYMOD_H
#define MITHIGGS_ISOLATIONSTUDYMODS_ELECTRONISOLATIONSTUDYMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;

namespace mithep 
{
  class ElectronIsolationStudyMod : public BaseMod
  {
    public:
      ElectronIsolationStudyMod(const char *name="ElectronIsolationStudyMod", 
                     const char *title="Example analysis module with all branches");
      ~ElectronIsolationStudyMod() {}
      void        SetPrintDebug(bool b)                       { fPrintDebug               = b;    }
      void        SetMatchToRealElectrons(bool b)             { fMatchToRealElectrons     = b;    } 
      void        SetImposeElectronID(bool b)                 { fImposeElectronID         = b;    }
      void        SetElectronPtMin(double min)                { fElectronPtMin            = min;  }
      void        SetElectronPtMax(double max)                { fElectronPtMax            = max;  }

    protected:
      bool                     fPrintDebug;               //flag for printing debug output
      TString                  fElectronName;             //name of electron collection
      TString                  fMCLeptonsName ;           // new lepton coll
      const ElectronCol	      *fElectrons;                //!Electron branch
      int                      fNEventsProcessed;         // Number of events processed
      bool                     fMatchToRealElectrons;    
      bool                     fImposeElectronID;    

      double                   fIDLikelihoodCut;          //Cut value for ID likelihood
      double                   fElectronPtMin;            //defined the range of
      double                   fElectronPtMax;            //pt's for electrons under study

      int                      fNElectrons;
      int                      fNElectronsPassDefaultIsolation;

      TH1D                    *fElectronPtHist;
      TH1D                    *fElectronEtaHist;
      TH2D                    *fElectronTrackIsoVsCaloIso;
      TH2D                    *fElectronTrackIsoVsCaloTowerIso;
      TH2D                    *fElectronTrackIsoVsEcalJurassicIso;
      TH2D                    *fElectronTrackIsoVsHcalIso;
      TH2D                    *fElectronTrackAndCaloIsoCutEff;
      TH2D                    *fNElectronPassTrackAndCaloIso;
      TH2D                    *fElectronTrackAndCaloTowerIsoCutEff;
      TH2D                    *fNElectronPassTrackAndCaloTowerIso;
      TH1D                    *fElectronTrackIso;
      TH1D                    *fElectronCaloIso;
      TH1D                    *fElectronCaloTowerIso;
      TH1D                    *fElectronEcalJurassicIso;
      TH1D                    *fElectronHcalIso;
      TH1D                    *fElectronTotalJurassicIso;
      TH1D                    *fElectronTrackIsoPlusCaloIso;
      TH1D                    *fElectronTrackIsoPlusCaloTowerIso;
      TH1D                    *fElectronTrackIsoPlusJurassicIso;
      TH1D                    *fElectronTwoTimesTrackIsoPlusCaloTowerIso;
      TH1D                    *fElectronTwoTimesTrackIsoPlusJurassicIso;
      TH1D                    *fElectronTrackIsoOverPt;
      TH1D                    *fElectronCaloIsoOverPt;
      TH1D                    *fElectronCaloTowerIsoOverPt;
      TH1D                    *fElectronEcalJurassicIsoOverPt;
      TH1D                    *fElectronHcalIsoOverPt;
      TH1D                    *fElectronTotalJurassicIsoOverPt;
      TH1D                    *fElectronTrackIsoPlusCaloIsoOverPt;
      TH1D                    *fElectronTrackIsoPlusCaloTowerIsoOverPt;
      TH1D                    *fElectronTrackIsoPlusJurassicIsoOverPt;
      TH1D                    *fElectronTwoTimesTrackIsoPlusCaloTowerIsoOverPt;
      TH1D                    *fElectronTwoTimesTrackIsoPlusJurassicIsoOverPt;
      TH1D                    *fNElectronPassTrackIsoCut;
      TH1D                    *fNElectronPassCaloIsoCut;
      TH1D                    *fNElectronPassCaloTowerIsoCut;
      TH1D                    *fNElectronPassEcalJurassicIsoCut;
      TH1D                    *fNElectronPassHcalIsoCut;
      TH1D                    *fNElectronPassTotalJurassicIsoCut;
      TH1D                    *fNElectronPassTrackIsoPlusCaloIsoCut;
      TH1D                    *fNElectronPassTrackIsoPlusCaloTowerIsoCut;
      TH1D                    *fNElectronPassTrackIsoPlusJurassicIsoCut;
      TH1D                    *fNElectronPassTwoTimesTrackIsoPlusCaloTowerIsoCut;
      TH1D                    *fNElectronPassTwoTimesTrackIsoPlusJurassicIsoCut;
      TH1D                    *fNElectronPassTrackIsoOverPtCut;
      TH1D                    *fNElectronPassCaloIsoOverPtCut;
      TH1D                    *fNElectronPassCaloTowerIsoOverPtCut;
      TH1D                    *fNElectronPassEcalJurassicIsoOverPtCut;
      TH1D                    *fNElectronPassHcalIsoOverPtCut;
      TH1D                    *fNElectronPassTotalJurassicIsoOverPtCut;
      TH1D                    *fNElectronPassTrackIsoPlusCaloIsoOverPtCut;
      TH1D                    *fNElectronPassTrackIsoPlusCaloTowerIsoOverPtCut;
      TH1D                    *fNElectronPassTrackIsoPlusJurassicIsoOverPtCut;
      TH1D                    *fNElectronPassTwoTimesTrackIsoPlusCaloTowerIsoOverPtCut;
      TH1D                    *fNElectronPassTwoTimesTrackIsoPlusJurassicIsoOverPtCut;
      TH1D                    *fElectronTrackIsoCutEff;
      TH1D                    *fElectronCaloIsoCutEff;
      TH1D                    *fElectronCaloTowerIsoCutEff;
      TH1D                    *fElectronEcalJurassicIsoCutEff;
      TH1D                    *fElectronHcalIsoCutEff;         
      TH1D                    *fElectronTotalJurassicIsoCutEff;         
      TH1D                    *fElectronTrackIsoPlusCaloIsoCutEff;
      TH1D                    *fElectronTrackIsoPlusCaloTowerIsoCutEff;
      TH1D                    *fElectronTrackIsoPlusJurassicIsoCutEff;
      TH1D                    *fElectronTwoTimesTrackIsoPlusCaloTowerIsoCutEff;
      TH1D                    *fElectronTwoTimesTrackIsoPlusJurassicIsoCutEff;
      TH1D                    *fElectronTrackIsoOverPtCutEff;
      TH1D                    *fElectronCaloIsoOverPtCutEff;
      TH1D                    *fElectronCaloTowerIsoOverPtCutEff;
      TH1D                    *fElectronEcalJurassicIsoOverPtCutEff;
      TH1D                    *fElectronHcalIsoOverPtCutEff;         
      TH1D                    *fElectronTotalJurassicIsoOverPtCutEff;         
      TH1D                    *fElectronTrackIsoPlusCaloIsoOverPtCutEff;
      TH1D                    *fElectronTrackIsoPlusCaloTowerIsoOverPtCutEff;
      TH1D                    *fElectronTrackIsoPlusJurassicIsoOverPtCutEff;
      TH1D                    *fElectronTwoTimesTrackIsoPlusCaloTowerIsoOverPtCutEff;
      TH1D                    *fElectronTwoTimesTrackIsoPlusJurassicIsoOverPtCutEff;
      TH2D                    *fElectronTrackIsoVsPt;
      TH2D                    *fElectronCaloIsoVsPt;
      TH2D                    *fElectronCaloTowerIsoVsPt;
      TH2D                    *fElectronJurassicEcalIsoVsPt;
      TH2D                    *fElectronHcalIsoVsPt; 
      TH2D                    *fElectronJurassicIsoVsPt;
      TH2D                    *fElectronTrackPlusJurassicIsoVsPt;

      void        Begin();
      void        Process();
      void        SlaveBegin();
      void        SlaveTerminate();
      void        Terminate();
      
    
      ClassDef(ElectronIsolationStudyMod,1) // TAM example analysis module
  };
}
#endif
