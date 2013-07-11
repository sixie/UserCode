//-----------------------------------------------------------------------------
// $Id  $
//
// VBFJetTagStudyMod
//
// A Module for Selecting forward tagging jets
// and produces some distributions
//
//
// Authors: ceballos
//-----------------------------------------------------------------------------

#ifndef HWWMODS_VBFJETTAGSTUDYMOD_H
#define HWWMODS_VBFJETTAGSTUDYMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

class TH1D;
class TH2D;

namespace mithep 
{
  class VBFJetTagStudyMod : public BaseMod
  {
    public:
    VBFJetTagStudyMod(const char *name="VBFJetTagStudyMod", 
		 const char *title="Example analysis module with all branches");
      ~VBFJetTagStudyMod() {}
      void         SetPrintDebug(bool b)            { fPrintDebug = b;	      }
      void         SetFillHist(bool b)              { fFillHist = b;	      }
      void         SetCleanJetsName (TString s)     { fCleanJetsName = s;     }
      void         SetCleanFwdJetsName (TString s)  { fCleanFwdJetsName = s;  }
      void         SetCleanNoFwdJetsName (TString s){ fCleanNoFwdJetsName = s;}
      void         SetMCqqHsName (TString s)        { fMCqqHsName = s;        }
      void         SetUseANN (bool b)               { fUseANN = b;            }
      void         SetUseHighestPtJets (bool b)     { fUseHighestPtJets= b;   }
      void         SetJetPtMax (double x)           { fJetPtMax = x;	      }
      void         SetJetPtMin (double x)           { fJetPtMin = x;	      }
      void         SetDeltaEtaMin (double x)        { fDeltaEtaMin = x;       }
      void         SetDiJetMassMin (double x)       { fDiJetMassMin = x;      }
      void         SetANNOutputMin (double x)       { fANNOutputMin = x;      }
 
    protected:
      bool         fPrintDebug;
      bool         fFillHist;
      TString      fMetName;
      TString      fCleanJetsName; 
      TString      fCleanFwdJetsName; 
      TString      fCleanNoFwdJetsName;
      TString      fMCqqHsName;
  
      TH1D        *fFwdQuarkPt;
      TH1D        *fFwdQuarkEta;
      TH1D        *fFwdQuarkDeltaEta;
      TH1D        *fFwdQuarkDijetMass;
      TH1D        *fMCEventClassification;
      TH1D        *fCleanJetEta_VBF;
      TH1D        *fCleanJetPt_VBF;
      TH1D        *fNCleanJets_VBF;
      TH1D        *fCleanJet1Pt_VBF;
      TH1D        *fCleanJet2Pt_VBF;
      TH1D        *fCleanJetEta_nonVBF;
      TH1D        *fCleanJetPt_nonVBF;
      TH1D        *fNCleanJets_nonVBF;
      TH1D        *fCleanJet1Pt_nonVBF;
      TH1D        *fCleanJet2Pt_nonVBF;
      TH1D        *fVBFJet_SameEtaSide;
      TH1D        *fVBFJet_DeltaEta;
      TH1D        *fVBFJet_DijetMass;
      TH1D        *fVBFJet_NNOutput;
      TH1D        *fVBFJet_SameEtaSide_NMinusOne;
      TH1D        *fVBFJet_DeltaEta_NMinusOne;
      TH1D        *fVBFJet_DijetMass_NMinusOne;
      TH1D        *fVBFJet_SameEtaSide_matchedVBF;
      TH1D        *fVBFJet_DeltaEta_matchedVBF;
      TH1D        *fVBFJet_DijetMass_matchedVBF;
      TH1D        *fVBFJet_NNOutput_matchedVBF;
      TH1D        *fVBFJet_SameEtaSide_NMinusOne_matchedVBF;
      TH1D        *fVBFJet_DeltaEta_NMinusOne_matchedVBF;
      TH1D        *fVBFJet_DijetMass_NMinusOne_matchedVBF;  
      TH1D        *fVBFJet_SameEtaSide_nonVBF;
      TH1D        *fVBFJet_DeltaEta_nonVBF;
      TH1D        *fVBFJet_DijetMass_nonVBF;
      TH1D        *fVBFJet_NNOutput_nonVBF;    
      TH1D        *fVBFJet_SameEtaSide_NMinusOne_nonVBF;
      TH1D        *fVBFJet_DeltaEta_NMinusOne_nonVBF;
      TH1D        *fVBFJet_DijetMass_NMinusOne_nonVBF;
      TH1D         *hDFwdJetSel[300];

      const MetCol *fMet;

      bool         fUseANN;
      bool         fUseHighestPtJets;
      double       fJetPtMax;
      double       fJetPtMin;
      double       fDeltaEtaMin;
      double       fDiJetMassMin;
      double       fANNOutputMin;
      int          fNEventsProcessed;

      void         Begin();
      void         Process();
      void         SlaveBegin();
      void         SlaveTerminate();
      void         Terminate();      
      double       Testmlp_qqH(double var[6]);
      double       Sigmoid(double x);

      ClassDef(VBFJetTagStudyMod,1) // TAM example analysis module
  };
}
#endif
