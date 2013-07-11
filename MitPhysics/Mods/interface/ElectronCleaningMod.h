//--------------------------------------------------------------------------------------------------
// $Id: ElectronCleaningMod.h,v 1.3 2009/03/06 16:48:04 phedex Exp $
//
// ElectronCleaningMod
//
// This Module performs cleaning of electrons, ie. it removes duplicate objects and good muons 
// from the good electrons.
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITPHYSICS_MODS_ELECTRONCLEANINGMOD_H
#define MITPHYSICS_MODS_ELECTRONCLEANINGMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

namespace mithep 
{
  class ElectronCleaningMod : public BaseMod
  {
    public:
      ElectronCleaningMod(const char *name="ElectronCleaningMod", 
                          const char *title="Electron cleaning module");
      ~ElectronCleaningMod() {}

      const char        *GetCleanElectronsName() const { return fCleanElectronsName;     }
      const char        *GetCleanName()          const { return GetCleanElectronsName(); }
      const char        *GetCleanMuonsName()     const { return fCleanMuonsName;         }
      const char        *GetGoodElectronsName()  const { return fGoodElectronsName;      }
      const char        *GetOutputName()         const { return GetCleanElectronsName(); }
      void               SetCleanElectronsName(const char *name) { fCleanElectronsName = name;  }
      void               SetCleanName(const char *name)          { SetCleanElectronsName(name); }
      void               SetCleanMuonsName(const char *name)     { fCleanMuonsName     = name;  }
      void               SetGoodElectronsName(const char *name)  { fGoodElectronsName  = name;  }
      void               SetOutputName(const char *name)         { SetCleanElectronsName(name); }

    protected:
      void               Process();

      TString            fGoodElectronsName;  //name of good electrons (input)
      TString            fCleanMuonsName;     //name of clean muons (input)
      TString            fCleanElectronsName; //name of clean electrons (output)
    
      ClassDef(ElectronCleaningMod, 1) // Electron cleaning module
  };
}
#endif
