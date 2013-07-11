// Generator level info data object
struct TGenData
{
  UInt_t  npho;                   // number of photons
  Float_t scalePdf;               // pdf scale sqrt(Q^2)  
  Float_t weight;                 // event weight
  Float_t vmass, vpt, vy, vphi;   // boson kinematics
  Float_t mass, pt, y, phi, mt;   // dilepton kinematics
  Float_t pt_1, eta_1, phi_1;     // lepton kinematics
  Float_t pt_2, eta_2, phi_2;     // lepton kinematics
  Int_t   q_1, q_2;               // lepton charge 
  Float_t phopt, phoeta, phophi;  // leading photon kinematics
  Float_t x_1, x_2;               // parton momentum fraction
  Int_t   id_1, id_2;             // parton PDG ID
  
  Float_t aqcd, aqed;             // QCD, QED coupling constants
  Float_t jetpt, jeteta, jetphi;  // "jet" kinematics
  Int_t   jetid;                  // "jet" PDG ID
};

struct TGenWGammastarData
{
  UInt_t  npho;                   // number of photons
  Float_t scalePdf;               // pdf scale sqrt(Q^2)  
  Float_t weight;                 // event weight
  Float_t wmass, wpt, wy, wphi;   // boson kinematics
  Float_t zmass, zpt, zy, zphi;   // boson kinematics
  Float_t mass, pt, y, phi, mt;   // dilepton kinematics
  Float_t pt_1, eta_1, phi_1;     // lepton kinematics
  Float_t pt_2, eta_2, phi_2;     // lepton kinematics
  Int_t   q_1, q_2;               // lepton charge 

  Float_t pt_zlepton1, eta_zlepton1, phi_zlepton1 ;    // lepton kinematics
  Int_t   pdgid_zlepton1;
  Float_t pt_zlepton2, eta_zlepton2, phi_zlepton2;     // lepton kinematics
  Int_t   pdgid_zlepton2;
  Float_t pt_wlepton, eta_wlepton, phi_wlepton;     // lepton kinematics
  Int_t   pdgid_wlepton;
  Float_t pt_wneutrino, eta_wneutrino, phi_wneutrino;     // lepton kinematics
  Float_t fourLeptonMass;
  Float_t wlnumass;
  Float_t zllmass;

  Float_t x_1, x_2;               // parton momentum fraction
  Int_t   id_1, id_2;             // parton PDG ID
  
  Float_t aqcd, aqed;             // QCD, QED coupling constants
};


struct TGenWWData
{

  Float_t scalePdf;               // pdf scale sqrt(Q^2)  
  Float_t weight;                 // event weight
  Float_t higgsmass, higgspt, higgsy, higgsphi;   // boson kinematics
  Float_t w1mass, w1pt, w1y, w1phi;   // boson kinematics
  Float_t w2mass, w2pt, w2y, w2phi;   // boson kinematics
  Float_t nu1mass, nu1pt, nu1y, nu1phi;   // boson kinematics
  Float_t nu2mass, nu2pt, nu2y, nu2phi;   // boson kinematics
  Float_t lep1mass, lep1pt, lep1y, lep1phi;   // boson kinematics
  Float_t lep2mass, lep2pt, lep2y, lep2phi;   // boson kinematics
  Float_t x_1, x_2;               // parton momentum fraction
  Int_t   id_1, id_2;             // parton PDG ID
  Float_t aqcd, aqed;             // QCD, QED coupling constants
};


struct TGenGammaGammaData
{
  Float_t scalePdf;               // pdf scale sqrt(Q^2)  
  Float_t weight;                 // event weight
  Float_t higgsmass, higgspt, higgsy, higgstheta, higgsphi;   // boson kinematics
  Float_t ph1mass, ph1pt, ph1y, ph1theta, ph1phi;   // boson kinematics
  Float_t ph2mass, ph2pt, ph2y, ph2theta, ph2phi;   // boson kinematics
  Float_t x_1, x_2;               // parton momentum fraction
  Int_t   id_1, id_2;             // parton PDG ID
  Float_t aqcd, aqed;             // QCD, QED coupling constants
};


