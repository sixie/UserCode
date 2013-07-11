//--------------------------------------------------------------------------------------------------
// $Id: FakeEventHeader.h,v 1.3 2009/06/30 08:20:07 phedex Exp $
//
// FakeEventHeader
//
// Class to hold information specific for events where some fakable object has been promoted 
// to a (faked) lepton. 
//
// Authors: S. Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_FAKEEVENTHEADER_H
#define MITANA_DATATREE_FAKEEVENTHEADER_H
 
#include "MitAna/DataTree/interface/DataObject.h"
#include "MitAna/DataTree/interface/Jet.h"
#include "MitAna/DataTree/interface/Collections.h"
#include "MitPhysics/Init/interface/FakeObject.h"

namespace mithep 
{
  class FakeEventHeader : public DataObject
  {
    public:
      FakeEventHeader() : fWeight(1.0), fWeightLowError(0.0), fWeightHighError(0.0) {} 
      ~FakeEventHeader() {}

      Double_t                   Weight()              const { return  fWeight;                   }
      Double_t                   WeightLowError()      const { return  fWeightLowError;           }
      Double_t                   WeightHighError()     const { return  fWeightHighError;          }
      const mithep::FakeObject  *FakeObject(UInt_t i)  const { return  fFakeObjects.At(i);        }
      UInt_t                     FakeObjectsSize()     const { return  fFakeObjects.GetEntries(); }
      const Jet                 *UnfakedJet(UInt_t i)  const { return  fJets.At(i);               }
      UInt_t                     NJets()               const { return  fJets.GetEntries();        }

      void      SetWeight(Double_t w)                        { fWeight = w;                       }
      void      SetWeightLowError(Double_t error)            { fWeightLowError = error;           }
      void      SetWeightHighError(Double_t error)           { fWeightHighError = error;          }
      void      AddJets(const Jet *jet)                      { fJets.Add(jet);                    }
      void      AddFakeObject(const Particle *p, EObjType faketype, Bool_t fakeTag, Bool_t mcTag);
      void      AddFakeObject(const mithep::FakeObject *f);

    protected:
      Double_t                  fWeight;          //!fake event weight
      Double_t                  fWeightLowError;  //!fake event weight low error
      Double_t                  fWeightHighError; //!fake event weight high error
      Array<mithep::FakeObject> fFakeObjects;     //!fake objects
      ObjArray<Jet>             fJets;            //!collection of jets after some have been 
                                                  //!potentially promoted to fakes

    ClassDef(FakeEventHeader, 1) // Event header class
  };
}

//--------------------------------------------------------------------------------------------------
inline void mithep::FakeEventHeader::AddFakeObject(const Particle *p, EObjType faketype, 
                                                   Bool_t fakeTag, Bool_t mcTag)  
{
   // Add new fake object
  mithep::FakeObject *newFake = fFakeObjects.AddNew();
  newFake->SetParticle(p);
  newFake->SetFakeType(faketype);
  newFake->SetFakeTag(fakeTag);
  newFake->SetMCTag(mcTag);  
}

//--------------------------------------------------------------------------------------------------
inline void mithep::FakeEventHeader::AddFakeObject(const mithep::FakeObject *f)  
{
   // Add new fake object
  mithep::FakeObject *newFake = fFakeObjects.AddNew();
  newFake->SetParticle(f->FakeParticle());
  newFake->SetFakeType(f->ObjType());
  newFake->SetFakeTag(f->FakeTag());
  newFake->SetMCTag(f->MCTag());  
}

#endif
