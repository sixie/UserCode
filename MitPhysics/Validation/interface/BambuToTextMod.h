//--------------------------------------------------------------------------------------------------
// $Id: BambuToTextMod.h,v 1.1 2008/11/29 18:51:07 sixie Exp $
//
// BAMBU2TXT
//
// A Module for Selecting H->WW events
// and produces some distributions.
//
//
// Authors: Si Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITPHYSICS_VALIDATION_BAMBUTOTEXTMOD_H
#define MITPHYSICS_VALIDATION_BAMBUTOTEXTMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;

namespace mithep 
{
  class BambuToTextMod : public BaseMod
  {
    public:
      BambuToTextMod(const char *name="HwwEvtSelMod", 
                   const char *title="Example analysis module with all branches");
      ~BambuToTextMod() {}
      void         SetOutputFilename (TString s)         { fOutputFilename = s;           }   
      void         SetProcessName (TString s)            { fProcessName = s;              }   
      void         SetCleanMuonName (TString s)          { fCleanMuonName = s;            }   
      void         SetCleanElectronName (TString s)      { fCleanElectronName = s;        }   
      void         SetCleanPhotonName (TString s)        { fCleanPhotonName = s;          }   
      void         SetCleanJetName (TString s)           { fCleanJetName = s;             }   
      void         SetMetName (TString s)                { fMetName = s;                  }   

    protected:
      bool                     fPrintDebug;
      TString                  fOutputFilename;
      TString                  fProcessName;
      TString                  fMCPartName;           //name of particle collection
      TString                  fCleanMuonName;	      // name of muon collection
      TString                  fCleanElectronName;    // name of muon collection
      TString                  fCleanPhotonName;      // name of muon collection
      TString                  fCleanJetName;         //name of clean central jets collection
      TString                  fMetName;              //name of met collection          
      MCParticleCol           *fParticles;           
      MuonCol	              *fMuons;	             
      ElectronCol	      *fElectrons;	     
      PhotonCol	              *fPhotons;	     
      JetCol                  *fJets;                
      MetCol                  *fMet;                 
      std::ofstream            fOutputFile;

      int                      fNEventsProcessed;

      void         Begin();
      void         Process();
      void         SlaveBegin();
      void         SlaveTerminate();
      void         Terminate();      

      ClassDef(BambuToTextMod,1) // TAM example analysis module
  };
}
#endif
