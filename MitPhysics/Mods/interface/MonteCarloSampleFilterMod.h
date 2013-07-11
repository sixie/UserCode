//--------------------------------------------------------------------------------------------------
// $Id $
//
// MonteCarloSampleFilterMod
//
// This module removes certain physics processes from processing based on the generator information
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITPHYSICS_MODS_MONTECARLOSAMPLEFILTERMOD_H
#define MITPHYSICS_MODS_MONTECARLOSAMPLEFILTERMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;

namespace mithep 
{
  class MonteCarloSampleFilterMod : public BaseMod
  {
    public:
      MonteCarloSampleFilterMod(const char *name="MonteCarloSampleFilterMod", 
                   const char *title="Filters certain physics processes");

      const char   *GetMCPartName()                 const { return fMCPartName;                }
      const char   *GetMCSampleName()               const { return fMCSampleName;              }
      const Bool_t  GetRemoveWGammaProcess()        const { return fRemoveWGammaProcess;       }

      void          SetMCPartName(const char *s)	  { fMCPartName                = s;    }
      void          SetMCSampleName(const char *s)	  { fMCSampleName              = s;    }
      void          SetRemoveWGammaProcess(Bool_t b)	  { fRemoveWGammaProcess       = b;    }
       
    protected:
      void                 Process();
      void                 SlaveBegin();

      TString              fMCPartName;               //name of MCParticle branch
      TString              fMCSampleName;             //name of MCSampleType
      Bool_t               fRemoveWGammaProcess;      //!whether to remove W+gamma events

      const MCParticleCol *fParticles;	          //!MCParticle branch

    ClassDef(MonteCarloSampleFilterMod, 1) // Module to gather generator information
  };
}
#endif
