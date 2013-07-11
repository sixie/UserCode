//--------------------------------------------------------------------------------------------------
// $Id: GeneratorTools.h,v 1.2 2009/03/17 08:37:02 sixie Exp $
//
// GeneratorTools
//
// Isolation functions to compute various kinds of isolation.
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_UTILS_GENERATORTOOLS_H
#define MITANA_UTILS_GENERATORTOOLS_H

#include "TString.h"
#include <TMath.h>
#include "MitAna/DataTree/interface/MCParticle.h"
#include "MitAna/DataTree/interface/Electron.h"

namespace mithep
{
  class GeneratorTools {
    public:
      
      static void PrintHepMCTable(const mithep::Collection<mithep::MCParticle> *particles, 
                           Bool_t showDaughters, int suppressEntriesAfterThisIndex);

      static void PrintNearbyParticles(const mithep::Collection<mithep::MCParticle> *particles, 
                                       Double_t eta, Double_t phi, Double_t deltaR);

      static TString ConvertPdgIdToName(Int_t pdgId);

      static const mithep::MCParticle* MatchElectronToSimParticle(const mithep::Collection<mithep::MCParticle> *particles,
                                                                  const mithep::Track *eleTrack, Bool_t isGsfTrack, 
                                                                  Int_t printDebugLevel, Bool_t printHepMCTable);

      static Int_t CategorizeFakeElectron(const mithep::MCParticle *ele);

//       static const mithep::MCParticle* MatchMuonToSimParticle(const mithep::Collection<mithep::MCParticle> *particles,
//                                                               const mithep::Muon *mu);

//       static Int_t CategorizeFakeMuon(const mithep::MCParticle *mu);

  };
}
#endif
