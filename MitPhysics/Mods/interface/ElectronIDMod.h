//--------------------------------------------------------------------------------------------------
// $Id: ElectronIDMod.h,v 1.4 2009/03/17 08:37:02 sixie Exp $
//
// ElectronIDMod
//
// This module applies electron identification criteria and exports a pointer to a collection
// of "good electrons" according to the specified identification scheme.
//
// Authors: S.Xie, C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef MITPHYSICS_MODS_ELECTRONIDMOD_H
#define MITPHYSICS_MODS_ELECTRONIDMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

namespace mithep 
{
  class ElectronIDMod : public BaseMod
  {
    public:
      ElectronIDMod(const char *name="ElectronIDMod", 
                    const char *title="Electron identification module");
      ~ElectronIDMod() {}

      Bool_t         GetApplyConversionFilter()      const { return fApplyConversionFilter;  }
      Double_t       GetCaloIsoCut()                 const { return fCaloIsolationCut;       }
      Double_t       GetEcalJurIsoCut()              const { return fEcalJuraIsoCut;         }
      const char    *GetGoodName()                   const { return GetGoodElectronsName();  }   
      const char    *GetGoodElectronsName()          const { return fGoodElectronsName;      }   
      Double_t       GetHcalIsoCut()                 const { return fHcalIsolationCut;       }
      Double_t       GetIDLikelihoodCut()            const { return fIDLikelihoodCut;        }
      const char    *GetIDType()                     const { return fElectronIDType;         }
      const char    *GetInputName()                  const { return fElectronBranchName;     }   
      const char    *GetIsoType()                    const { return fElectronIsoType;        }
      const char    *GetOutputName()                 const { return GetGoodElectronsName();  }
      Double_t       GetPtMin()                      const { return fElectronPtMin;          }
      Double_t       GetTrackIsoCut()                const { return fTrackIsolationCut;      }
      Double_t       GetEOverPCut()                  const { return fEOverPCut;              }
      Double_t       GetD0Cut()                      const { return fD0Cut;                  }
      const char    *GetVertexBranchName()           const { return fVertexBranchName;       }
      void           SetApplyConversionFilter(Bool_t b)    { fApplyConversionFilter  = b;    }
      void           SetCaloIsoCut(Double_t cut)           { fCaloIsolationCut       = cut;  }
      void           SetEcalJurIsoCut(Double_t cut)        { fEcalJuraIsoCut         = cut;  }
      void           SetGoodName(const char *n)            { SetGoodElectronsName(n);        }   
      void           SetGoodElectronsName(const char *n)   { fGoodElectronsName      = n;    }   
      void           SetHcalIsoCut(Double_t cut)           { fHcalIsolationCut       = cut;  }
      void           SetIDLikelihoodCut(Double_t cut)      { fIDLikelihoodCut        = cut;  }
      void           SetIDType(const char *type)           { fElectronIDType         = type; }
      void           SetInputName(const char *n)           { fElectronBranchName     = n;    }   
      void           SetIsoType(const char *type)          { fElectronIsoType        = type; }
      void           SetOutputName(const char *n)          { SetGoodElectronsName(n);        }   
      void           SetPtMin(Double_t pt)                 { fElectronPtMin          = pt;   }
      void           SetTrackIsoCut(Double_t cut)          { fTrackIsolationCut      = cut;  }
      void           SetEOverPCut(Double_t cut)            { fEOverPCut              = cut;  }
      void           SetD0Cut(Double_t cut)                { fD0Cut                  = cut;  }
      void           SetVertexBranchName(const char *name) { fVertexBranchName       = name; }

      enum EElIdType {
        kIdUndef = 0,       //not defined
        kTight,             //"Tight"
        kLoose,             //"Loose"
        kLikelihood,        //"Likelihood"
        kNoId,              //"NoId"
        kCustomId           //"Custom"
      };
      enum EElIsoType {
        kIsoUndef = 0,      //not defined
        kTrackCalo,         //"TrackCalo"
        kTrackJura,         //"TrackJura"
        kTrackJuraSliding,  //"TrackJuraSliding"
        kNoIso,             //"NoIso"
        kCustomIso          //"Custom"
      };

    protected:
      void                    Process();
      void                    SlaveBegin();

      TString                 fVertexBranchName;     //name of vertex collection
      TString                 fElectronBranchName;   //name of electron collection (input)
      TString                 fConversionBranchName; //name of electron collection (input)
      TString                 fGoodElectronsName;    //name of exported "good electrons" collection
      TString                 fElectronIDType;       //type of electron ID we impose
      TString                 fElectronIsoType;      //type of electron Isolation we impose
      Double_t                fElectronPtMin;        //min pt cut
      Double_t                fIDLikelihoodCut;      //cut value for ID likelihood
      Double_t                fTrackIsolationCut;    //cut value for track isolation
      Double_t                fCaloIsolationCut;     //cut value for calo isolation
      Double_t                fEcalJuraIsoCut;       //cut value for ecal jurassic isolation
      Double_t                fHcalIsolationCut;     //cut value for hcal isolation
      Double_t                fEOverPCut;            //cut value for E/P
      Double_t                fD0Cut;                //cut value for E/P
      const VertexCol        *fVertices;             //!Vertex branch
      const ElectronCol      *fElectrons;            //!electron collection
      const DecayParticleCol *fConversions;          //!conversion collection
      EElIdType               fElIdType;             //!identification scheme
      EElIsoType              fElIsoType;            //!isolation scheme
      Bool_t                  fApplyConversionFilter;  //!whether remove conversions
    
    ClassDef(ElectronIDMod, 1) // Electron identification module
  };
}
#endif
