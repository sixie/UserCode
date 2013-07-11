// $Id: ElectronIDMod.cc,v 1.4 2009/03/17 08:37:02 sixie Exp $

#include "MitPhysics/Mods/interface/ElectronIDMod.h"
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitPhysics/Init/interface/ModNames.h"

using namespace mithep;

ClassImp(mithep::ElectronIDMod)

//--------------------------------------------------------------------------------------------------
ElectronIDMod::ElectronIDMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fVertexBranchName(Names::gkPVBeamSpotBrn),
  fElectronBranchName(Names::gkElectronBrn),
  fConversionBranchName(Names::gkMvfConversionBrn),
  fGoodElectronsName(ModNames::gkGoodElectronsName),  
  fElectronIDType("Tight"),
  fElectronIsoType("TrackJuraSliding"),
  fElectronPtMin(10),
  fIDLikelihoodCut(0.9),
  fTrackIsolationCut(5.0),
  fCaloIsolationCut(5.0),
  fEcalJuraIsoCut(5.0),
  fHcalIsolationCut(5.0),
  fEOverPCut(5.0),
  fD0Cut(0.03),
  fElectrons(0),
  fElIdType(kIdUndef),
  fElIsoType(kIsoUndef),
  fApplyConversionFilter(kFALSE)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void ElectronIDMod::Process()
{
  // Process entries of the tree. 

  LoadBranch(fVertexBranchName);
  LoadBranch(fElectronBranchName);
  LoadBranch(fConversionBranchName);

  ElectronOArr *GoodElectrons = new ElectronOArr;
  GoodElectrons->SetName(fGoodElectronsName);

  for (UInt_t i=0; i<fElectrons->GetEntries(); ++i) {    
    const Electron *e = fElectrons->At(i);        

    if (e->Pt() <= fElectronPtMin) 
      continue;
    
    Bool_t idcut = kFALSE;
    switch (fElIdType) {
      case kTight:
        idcut = e->PassTightID();
        idcut = idcut && (e->ESuperClusterOverP() < fEOverPCut);
        break;
      case kLoose:
        idcut = e->PassLooseID();
       break;
      case kLikelihood:
        idcut = (e->IDLikelihood() > fIDLikelihoodCut);
        break;
      case kNoId:
        idcut = kTRUE;
        break;
      case kCustomId:
      default:
        break;
    }

    if (!idcut) 
      continue;

    Bool_t isocut = kFALSE;
    switch (fElIsoType) {
      case kTrackCalo:
        isocut = (e->TrackIsolation() < fTrackIsolationCut) &&
                 (e->CaloIsolation() < fCaloIsolationCut);
        break;
      case kTrackJura:
        isocut = (e->TrackIsolation() < fTrackIsolationCut) &&
                 (e->EcalJurassicIsolation() < fEcalJuraIsoCut) &&
                 (e->HcalIsolation() < fHcalIsolationCut);
        break;
      case kTrackJuraSliding:
        { 
          Double_t totalIso = e->TrackIsolation() + e->EcalJurassicIsolation() - 1.5;
          if ((totalIso < (e->Pt()-10.0)*6.0/15.0 && e->Pt() <= 25) ||
              (totalIso < 6.0 && e->Pt() > 25) ||
	       totalIso <= 0)
            isocut = kTRUE;
        }
        break;
      case kNoIso:
        isocut = kTRUE;
        break;
      case kCustomIso:
      default:
        break;
    }

    if (!isocut) 
      continue;

    // Apply Conversion Filter
    if (fApplyConversionFilter) {
      Bool_t ConversionMatchFound = false;
      

      MDB(kAnalysis, 6) {
        cout << "************************* Electron ID ****************************\n";
        cout << "Check for Conversions" << endl;
        cout << "Electron Candidate: " << e->Pt() << " " << e->Eta() << " "
             << e->Phi() << "  GsfTrack: " << e->GsfTrk()->Pt() << " "
             << e->GsfTrk()->Eta() << " " << e->GsfTrk()->Phi() << " TrackerTrack: ";
        if (e->TrackerTrk())
          cout << e->TrackerTrk()->Pt() << " " << e->TrackerTrk()->Eta() << " "
               << e->TrackerTrk()->Phi();
        cout << endl;             
      }

      for (int c=0;c<fConversions->GetEntries();c++) {
        Bool_t isGoodConversion = true;

        MDB(kAnalysis, 6) {
          cout << "************************* Electron ID ****************************\n";
          cout << "Conversion Candidate: " << c << " : "  
               << fConversions->At(c)->Prob() << "  " <<  fConversions->At(c)->Lxy()
               << " " <<  fConversions->At(c)->Lz() << " " 
               << fConversions->At(c)->NDaughters() << " DaughterTracks | ";         
        }

        isGoodConversion = isGoodConversion && 
          (fConversions->At(c)->Prob() > 0.005) &&
          (fConversions->At(c)->Lxy() > 0) &&
          (fConversions->At(c)->Lz() > 0);

        for (int d=0; d < fConversions->At(c)->NDaughters();d++) {
          const Track *trk = dynamic_cast<const ChargedParticle*>(fConversions->At(c)->Daughter(d))->Trk();
                    
          if (trk) {
            if (!(trk->NHits() > 8 && trk->Prob() > 0.005))
              isGoodConversion = false;
            
            const StableData *sd = dynamic_cast<const StableData*>
              (fConversions->At(c)->DaughterDat(d));
            
            if (sd->NWrongHits() != 0)
              isGoodConversion = false;
            
            MDB(kAnalysis, 6) {
              cout << "Daughter " << d << ":" << trk->Pt() << " " << trk->Eta() << " " 
                   << trk->Phi() << " " << trk->NHits() << " " << trk->Prob() << " "
                   << sd->NWrongHits() << " | ";
            }
          } else {
            isGoodConversion = false;
          }
        }

        MDB(kAnalysis, 6) cout << " isGoodConversion: " << isGoodConversion << endl;

        if (isGoodConversion) {
          for (int d=0;d < fConversions->At(c)->NDaughters();d++) {
            const Track *trk = dynamic_cast<const ChargedParticle*>(fConversions->At(c)->Daughter(d))->Trk();

            if (e->TrackerTrk() == trk) {
              ConversionMatchFound = true;
              MDB(kAnalysis, 6) {
                cout << "Found Conversion Match: " << trk->Pt() << " " << trk->Eta() << " " 
                     << trk->Phi() << endl;
              }              
              break;
            }
          }
          
          if (ConversionMatchFound)
            break;
        }//if it's a good conversion
      }//loop over all conversions
      
      //if electorn matches to a conversion we skip it.
      if (ConversionMatchFound) {
        continue;        
        //we can make some plots of the events that was vetoed by the conversion filter here
      }

      //find Primary Vertex
      const Vertex *primaryVertex = NULL;
      Double_t bestVertexProb = -1.0;
      for (UInt_t i=0; i<fVertices->GetEntries(); i++) {
        if (fVertices->At(i)->Prob() > bestVertexProb) {
          bestVertexProb = fVertices->At(i)->Prob();
          primaryVertex = fVertices->At(i);
        }
      }
      
      //add impact parameter cut to conversion filter
      if (fabs(e->Trk()->D0Corrected(primaryVertex)) >= fD0Cut)
        continue;
      
    }

    // add good electron
    GoodElectrons->Add(fElectrons->At(i));
  }

  // sort according to pt
  GoodElectrons->Sort();

  // add to event for other modules to use
  AddObjThisEvt(GoodElectrons);  
}

//--------------------------------------------------------------------------------------------------
void ElectronIDMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we just request the electron collection branch.

  ReqBranch(fVertexBranchName,   fVertices);
  ReqBranch(fElectronBranchName, fElectrons);
  ReqBranch(fConversionBranchName, fConversions);

  if (fElectronIDType.CompareTo("Tight") == 0) 
    fElIdType = kTight;
  else if (fElectronIDType.CompareTo("Loose") == 0) 
    fElIdType = kLoose;
  else if (fElectronIDType.CompareTo("Likelihood") == 0) 
    fElIdType = kLikelihood;
  else if (fElectronIDType.CompareTo("NoId") == 0) 
    fElIdType = kNoId;
  else if (fElectronIDType.CompareTo("Custom") == 0) {
    fElIdType = kCustomId;
    SendError(kWarning, "SlaveBegin",
              "Custom electron identification is not yet implemented.");
  } else {
    SendError(kAbortAnalysis, "SlaveBegin",
              "The specified electron identification %s is not defined.",
              fElectronIDType.Data());
    return;
  }

  if (fElectronIsoType.CompareTo("TrackCalo") == 0 )
    fElIsoType = kTrackCalo;
  else if (fElectronIsoType.CompareTo("TrackJura") == 0) 
    fElIsoType = kTrackJura;
  else if(fElectronIsoType.CompareTo("TrackJuraSliding") == 0)
    fElIsoType = kTrackJuraSliding;
  else if (fElectronIsoType.CompareTo("NoIso") == 0 )
    fElIsoType = kNoIso;
  else if (fElectronIsoType.CompareTo("Custom") == 0 ) {
    fElIsoType = kCustomIso;
    SendError(kWarning, "SlaveBegin",
              "Custom electron isolation is not yet implemented.");
  } else {
    SendError(kAbortAnalysis, "SlaveBegin",
              "The specified electron isolation %s is not defined.",
              fElectronIsoType.Data());
    return;
  }
}
