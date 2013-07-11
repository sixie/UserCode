//--------------------------------------------------------------------------------------------------
// $Id: JetIDMod.h,v 1.3 2009/03/06 16:48:04 phedex Exp $
//
// JetIDMod
//
// This module applies jet identification criteria and exports a pointer to a collection
// of "good jet" according to the specified identification scheme.
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITPHYSICS_MODS_JETIDMOD_H
#define MITPHYSICS_MODS_JETIDMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

namespace mithep 
{
  class JetIDMod : public BaseMod
  {
    public:
      JetIDMod(const char *name="JetIDMod", 
               const char *title="Jed identification module");
      ~JetIDMod() {}

      const char       *GetInputName()                 const { return fJetBranchName;     }   
      const char       *GetGoodName()                  const { return GetGoodJetsName();  }     
      const char       *GetGoodJetsName()              const { return fGoodJetsName;      }     
      const char       *GetOutputName()                const { return GetGoodJetsName();  }     
      Double_t          GetPtCut()                     const { return fJetPtCut;          }     
      Bool_t            GetUseCorrection()             const { return fUseJetCorrection;  }     
      void              SetGoodJetsName(const char *name)    { fGoodJetsName = name;      }     
      void              SetGoodName(const char *name)        { SetGoodJetsName(name);     }     
      void              SetInputName(const char *name)       { fJetBranchName = name;     }  
      void              SetOutputName(const char *name)      { SetGoodJetsName(name);     }     
      void              SetPtCut(Double_t cut)               { fJetPtCut = cut;           }     
      void              SetUseCorrection(Bool_t b)           { fUseJetCorrection = b;     }     

    protected:
      void              Process();
      void              SlaveBegin();

      TString           fJetBranchName;         //name of jet collection (input)
      TString           fGoodJetsName;          //name of good jets collection (output)
      Bool_t            fUseJetCorrection;      //=true then use corrected energy
      Double_t          fJetPtCut;              //jet pt cut
      const JetCol     *fJets;                  //!jet collection

      ClassDef(JetIDMod, 1) // Jet identification module
  };
}
#endif
