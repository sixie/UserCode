//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Jun  6 21:47:12 2010 by ROOT version 5.22/00d
// from TTree ZeeTree/ZeeTree
// found on file: EwkAnalysis_p10-zee-powhep-cteq66-v26_noskim.root
//////////////////////////////////////////////////////////

#ifndef MitZeeEvent_h
#define MitZeeEvent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class MitZeeEvent {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         event_branch_weight;
   Float_t         event_branch_weightErrorLow;
   Float_t         event_branch_weightErrorHigh;
   Float_t         event_branch_sampleType;
   Float_t         event_branch_runNumber;
   Float_t         event_branch_lumiNumber;
   Float_t         event_branch_eventNumber;
   Float_t         event_branch_caloMetX;
   Float_t         event_branch_caloMetY;
   Float_t         event_branch_tcMetX;
   Float_t         event_branch_tcMetY;
   Float_t         event_branch_pfMetX;
   Float_t         event_branch_pfMetY;
   Int_t           nEle;
   Int_t           electronType[7];   //[nEle]
   Int_t           electronLocation[7];   //[nEle]
   Int_t           electronTriggerMatch[7];   //[nEle]
   Float_t         electronPt[7];   //[nEle]
   Float_t         electronEta[7];   //[nEle]
   Float_t         electronPhi[7];   //[nEle]
   Float_t         electronCoviEtaiEta[7];   //[nEle]
   Float_t         electronDeltaEta[7];   //[nEle]
   Float_t         electronDeltaPhi[7];   //[nEle]
   Float_t         electronEOverP[7];   //[nEle]
   Float_t         electronHadOverEm[7];   //[nEle]
   Float_t         electronEcalRecHitIsoDr04[7];   //[nEle]
   Float_t         electronTrackIsolationDr03[7];   //[nEle]
   Float_t         electronHcalTowerSumEtDr04[7];   //[nEle]
   Float_t         electronD0[7];   //[nEle]
   Float_t         electronD0ToBeamspot[7];   //[nEle]
   Int_t           electronPassedConversionVeto[7];   //[nEle]
   Int_t           electronPassedChargeFilter[7];   //[nEle]
   Int_t           electronFakeFlag[7];   //[nEle]
   Int_t           electronConvPartnerDist[7];   //[nEle]
   Int_t           electronConvPartnerDCotTheta[7];   //[nEle]
   Int_t           electronNMissingHits[7];   //[nEle]

   // List of branches
   TBranch        *b_event_branch;   //!
   TBranch        *b_nEle;   //!
   TBranch        *b_electronType;   //!
   TBranch        *b_electronLocation;   //!
   TBranch        *b_electronTriggerMatch;   //!
   TBranch        *b_electronPt;   //!
   TBranch        *b_electronEta;   //!
   TBranch        *b_electronPhi;   //!
   TBranch        *b_electronCoviEtaiEta;   //!
   TBranch        *b_electronDeltaEta;   //!
   TBranch        *b_electronDeltaPhi;   //!
   TBranch        *b_electronEOverP;   //!
   TBranch        *b_electronHadOverEm;   //!
   TBranch        *b_electronEcalRecHitIsoDr04;   //!
   TBranch        *b_electronTrackIsolationDr03;   //!
   TBranch        *b_electronHcalTowerSumEtDr04;   //!
   TBranch        *b_electronD0;   //!
   TBranch        *b_electronD0ToBeamspot;   //!
   TBranch        *b_electronPassedConversionVeto;   //!
   TBranch        *b_electronPassedChargeFilter;   //!
   TBranch        *b_electronFakeFlag;   //!
   TBranch        *b_electronConvPartnerDist;   //!
   TBranch        *b_electronConvPartnerDCotTheta;   //!
   TBranch        *b_electronNMissingHits;   //!

   MitZeeEvent(TTree *tree=0);
   virtual ~MitZeeEvent();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef MitZeeEvent_cxx
MitZeeEvent::MitZeeEvent(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("EwkAnalysis_p10-zee-powhep-cteq66-v26_noskim.root");
      if (!f) {
         f = new TFile("EwkAnalysis_p10-zee-powhep-cteq66-v26_noskim.root");
      }
      tree = (TTree*)gDirectory->Get("ZeeTree");

   }
   Init(tree);
}

MitZeeEvent::~MitZeeEvent()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MitZeeEvent::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MitZeeEvent::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void MitZeeEvent::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event_branch", &event_branch_weight, &b_event_branch);
   fChain->SetBranchAddress("nEle", &nEle, &b_nEle);
   fChain->SetBranchAddress("electronType", electronType, &b_electronType);
   fChain->SetBranchAddress("electronLocation", electronLocation, &b_electronLocation);
   fChain->SetBranchAddress("electronTriggerMatch", electronTriggerMatch, &b_electronTriggerMatch);
   fChain->SetBranchAddress("electronPt", electronPt, &b_electronPt);
   fChain->SetBranchAddress("electronEta", electronEta, &b_electronEta);
   fChain->SetBranchAddress("electronPhi", electronPhi, &b_electronPhi);
   fChain->SetBranchAddress("electronCoviEtaiEta", electronCoviEtaiEta, &b_electronCoviEtaiEta);
   fChain->SetBranchAddress("electronDeltaEta", electronDeltaEta, &b_electronDeltaEta);
   fChain->SetBranchAddress("electronDeltaPhi", electronDeltaPhi, &b_electronDeltaPhi);
   fChain->SetBranchAddress("electronEOverP", electronEOverP, &b_electronEOverP);
   fChain->SetBranchAddress("electronHadOverEm", electronHadOverEm, &b_electronHadOverEm);
   fChain->SetBranchAddress("electronEcalRecHitIsoDr04", electronEcalRecHitIsoDr04, &b_electronEcalRecHitIsoDr04);
   fChain->SetBranchAddress("electronTrackIsolationDr03", electronTrackIsolationDr03, &b_electronTrackIsolationDr03);
   fChain->SetBranchAddress("electronHcalTowerSumEtDr04", electronHcalTowerSumEtDr04, &b_electronHcalTowerSumEtDr04);
   fChain->SetBranchAddress("electronD0", electronD0, &b_electronD0);
   fChain->SetBranchAddress("electronD0ToBeamspot", electronD0ToBeamspot, &b_electronD0ToBeamspot);
   fChain->SetBranchAddress("electronPassedConversionVeto", electronPassedConversionVeto, &b_electronPassedConversionVeto);
   fChain->SetBranchAddress("electronPassedChargeFilter", electronPassedChargeFilter, &b_electronPassedChargeFilter);
   fChain->SetBranchAddress("electronFakeFlag", electronFakeFlag, &b_electronFakeFlag);
   fChain->SetBranchAddress("electronConvPartnerDist", electronConvPartnerDist, &b_electronConvPartnerDist);
   fChain->SetBranchAddress("electronConvPartnerDCotTheta", electronConvPartnerDCotTheta, &b_electronConvPartnerDCotTheta);
   fChain->SetBranchAddress("electronNMissingHits", electronNMissingHits, &b_electronNMissingHits);
   Notify();
}

Bool_t MitZeeEvent::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MitZeeEvent::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MitZeeEvent::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MitZeeEvent_cxx
