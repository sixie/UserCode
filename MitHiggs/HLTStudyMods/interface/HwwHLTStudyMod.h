//--------------------------------------------------------------------------------------------------
// $Id: HwwHLTStudyMod.h,v 1.2 2008/12/04 19:19:49 sixie Exp $
//
// HwwEvtSelMod
//
// A Module for Preselection of H->WW events
// Apply very loose cuts in order to reduce the number of
// irrelevent events that we have to process
//
// Authors: Si Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITHIGGS_HLTSTUDYMODS_HWWHLTSTUDYMOD_H
#define MITHIGGS_HLTSTUDYMODS_HWWHLTSTUDYMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;

namespace mithep 
{
  class HwwHLTStudyMod : public BaseMod
  {
    public:
      HwwHLTStudyMod(const char *name="HwwHLTStudyMod", 
		 const char *title="Example analysis module with all branches");
      ~HwwHLTStudyMod() {}

      void         SetPrintDebug(bool b)            { fPrintDebug       = b;     }   
      void         SetDoHLTStudy(bool b)            { fDoHLTStudy       = b;     }   
      void         SetPreSelectionType(TString s)   { fPreSelectionType = s;     }   

    protected:
      bool                     fDoHLTStudy;
      bool                     fPrintDebug;
      TString                  fPreSelectionType;     
      TString                  fMuonName;             //name of muon collection
      TString                  fElectronName;         //name of electron collection
      const MuonCol           *fMuons;                //!Muon branch
      const ElectronCol	      *fElectrons;            //!Electron branch

      TH1D        *fFailedPtHist;     //!pt histogram
      TH1D        *fFailedEtaHist;    //!eta histogram  
      TH1D        *fFailedElectronPtHist;
      TH1D        *fFailedElectronEtaHist;
      TH1D        *fFailedMuonPtHist;
      TH1D        *fFailedMuonEtaHist;
      TH1D        *fPassedPtHist;     //!pt histogram
      TH1D        *fPassedEtaHist;    //!eta histogram  
      TH1D        *fPassedElectronPtHist;
      TH1D        *fPassedElectronEtaHist;
      TH1D        *fPassedMuonPtHist;
      TH1D        *fPassedMuonEtaHist;
      TH1D        *fHLTToRecoObjectDeltaR;
      TH1D        *fHLTToRecoElectronDeltaR;
      TH1D        *fHLTToRecoMuonDeltaR;
      
      int                      fNEventsProcessed;
      int                      fNEventsPassed;     

      void                     Begin();
      void                     Process();
      void                     SlaveBegin();
      void                     SlaveTerminate();
      void                     Terminate();      

      ClassDef(HwwHLTStudyMod,1) // TAM example analysis module
  };
}
#endif
