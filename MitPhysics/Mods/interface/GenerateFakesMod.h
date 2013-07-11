//--------------------------------------------------------------------------------------------------
// $Id: GenerateFakesMod.h,v 1.4 2009/06/30 08:54:10 phedex Exp $
//
// GenerateFakesMod
//
// This Module generates a collection of FakeEventHeaders containing information
// about possible fakes and their weight. The collection generated takes into account all possible
// faking combinatorics from the given set of fakable objects.
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITPHYSICS_MODS_GENERATEFAKESMOD_H
#define MITPHYSICS_MODS_GENERATEFAKESMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"
#include "MitPhysics/Init/interface/FakeRate.h"

namespace mithep 
{
  class GenerateFakesMod : public BaseMod
  {
    public:
      GenerateFakesMod(const char *name="GenerateFakesMod", 
                     const char *title="Fake Object Generation Module");
      ~GenerateFakesMod() {}

      const char   *GetCleanElectronsName()          const { return fCleanElectronsName;          }
      const char   *GetCleanMuonsName()              const { return fCleanMuonsName;              }
      const char   *GetCleanPhotonsName()            const { return fCleanPhotonsName;            }
      const char   *GetCleanJetsName()               const { return fCleanJetsName;               }
      const char   *GetElectronFakeableObjectsName() const { return fElectronFakeableObjectsName; }
      const char   *GetMuonFakeableObjectsName()     const { return fMuonFakeableObjectsName;     }
      const char   *GetMCLeptonsName()               const { return fMCLeptonsName;               }
      const char   *GetMCTausName()                  const { return fMCTausName;                  }
      const char   *GetFakeEventHeadersName()        const { return fFakeEventHeadersName;        }
      const char   *GetOutputName()                  const { return GetFakeEventHeadersName();    }
      const Bool_t  GetUse2DFakeRate()               const { return fUse2DFakeRate;               }
      const Bool_t  GetUseFitFunction()              const { return fUseFitFunction;              }

      void SetCleanElectronsName(const char *name)          { fCleanElectronsName          = name; }
      void SetCleanMuonssName(const char *name)             { fCleanMuonsName              = name; }
      void SetCleanPhotonsName(const char *name)            { fCleanPhotonsName            = name; }
      void SetCleanJetsName(const char *name)               { fCleanJetsName               = name; }
      void SetElectronFakeableObjectsName(const char *name) { fElectronFakeableObjectsName = name; }
      void SetMuonFakeableObjectsName(const char *name)     { fMuonFakeableObjectsName     = name; }
      void SetMCLeptonsName(const char *name)               { fMCLeptonsName               = name; }
      void SetMCTausName(const char *name)                  { fMCTausName                  = name; }
      void SetFakeEventHeadersName(const char *name)        { fFakeEventHeadersName        = name; }
      void SetOutputName(const char *name)                  { SetFakeEventHeadersName(name);       }
      void SetElectronFRFilename(const char *name)          { fElectronFRFilename          = name; }
      void SetMuonFRFilename(const char *name)              { fMuonFRFilename              = name; }
      void SetUse2DFakeRate(Bool_t b)                       { fUse2DFakeRate               = b;    }
      void SetUseFitFunction(Bool_t b)                      { fUseFitFunction              = b;    }
      void SetElectronFRFunctionName(const char *name)      { fElectronFRFunctionName      = name; }
      void SetMuonFRFunctionName(const char *name)          { fMuonFRFunctionName          = name; }
      void SetElectronFRHistName(const char *name)          { fElectronFRHistName          = name; }
      void SetMuonFRHistName(const char *name)              { fMuonFRHistName              = name; }

      void LoadFakeRate();
      
    protected:
      void               Process();

      FakeRate          *fFakeRate;                     //holds the fake probabilities
      TString            fElectronFRFilename;           //file containing electron fake rate
      TString            fMuonFRFilename;               //file containing muon fake rate
      Bool_t             fUse2DFakeRate;                //whether to use fit function or not
      Bool_t             fUseFitFunction;               //whether to use fit function or not
      TString            fElectronFRFunctionName;       //fit function containing electron fake rate
      TString            fMuonFRFunctionName;           //fit function containing muon fake rate
      TString            fElectronFRHistName;           //hist containing electron fake rate
      TString            fMuonFRHistName;               //hist containing muon fake rate
      TString            fCleanElectronsName;           //name of clean electrons           (input)
      TString            fCleanMuonsName;               //name of clean muons               (input)
      TString            fCleanPhotonsName;             //name of clean photons             (input)
      TString            fCleanJetsName;                //name of clean jets                (input)
      TString            fMCLeptonsName;                //name of MC leptons
      TString            fMCTausName;                   //name of MC taus
      TString            fElectronFakeableObjectsName;  //name of electron fakeable objects (input)
      TString            fMuonFakeableObjectsName;      //name of muon fakeable objects     (input)

      TString            fFakeEventHeadersName; //name of collection of FakeEventHeaders    (output)
   
    ClassDef(GenerateFakesMod, 1) // Jet cleaning module
  };
}
#endif
