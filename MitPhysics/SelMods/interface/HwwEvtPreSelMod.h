//--------------------------------------------------------------------------------------------------
// $Id: HwwEvtPreSelMod.h,v 1.1 2008/11/12 15:04:49 sixie Exp $
//
// HwwEvtSelMod
//
// A Module for Preselection of H->WW events
// Apply very loose cuts in order to reduce the number of
// irrelevent events that we have to process
//
// Authors: Si Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_PHYSICSMOD_HWWEVTPRESELMOD_H
#define MITANA_PHYSICSMOD_HWWEVTPRESELMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;

namespace mithep 
{
  class HwwEvtPreSelMod : public BaseMod
  {
    public:
      HwwEvtPreSelMod(const char *name="HwwEvtPreSelMod", 
		 const char *title="Example analysis module with all branches");
      ~HwwEvtPreSelMod() {}

      void         SetPrintDebug(bool b)            { fPrintDebug       = b;     }   
      void         SetDoHLTStudy(bool b)            { fDoHLTStudy       = b;     }   
      void         SetPreSelectionType(TString s)   { fPreSelectionType = s;     }   

    protected:
      bool                     fDoHLTStudy;
      bool                     fPrintDebug;
      TString                  fPreSelectionType;     
      TString                  fMuonName;             //name of muon collection
      TString                  fElectronName;         //name of electron collection
      MuonCol		      *fMuons;                //!Muon branch
      ElectronCol	      *fElectrons;            //!Electron branch

      TH1D        *fFailedPtHist;     //!pt histogram
      TH1D        *fFailedEtaHist;    //!eta histogram  
      TH1D        *fFailedElectronPtHist;
      TH1D        *fFailedElectronEtaHist;
      TH1D        *fFailedMuonPtHist;
      TH1D        *fFailedMuonEtaHist;

      int                      fNEventsProcessed;
      int                      fNEventsPassed;     

      void                     Begin();
      void                     Process();
      void                     SlaveBegin();
      void                     SlaveTerminate();
      void                     Terminate();      

      ClassDef(HwwEvtPreSelMod,1) // TAM example analysis module
  };
}
#endif
