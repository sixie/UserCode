// $Id $

#ifndef FAKERATEMODS_LINKDEF_H
#define FAKERATEMODS_LINKDEF_H
#include "MitHiggs/FakeRateMods/interface/MuonFakeRateAnalysisMod.h"
#include "MitHiggs/FakeRateMods/interface/ComputeElectronFakeRateMod.h"
#include "MitHiggs/FakeRateMods/interface/ComputeMuonFakeRateMod.h"
#include "MitHiggs/FakeRateMods/interface/JetChargeCorrelationStudyMod.h"
#include "MitHiggs/FakeRateMods/interface/WJetChargeCorrelationStudyMod.h"
#include "MitHiggs/FakeRateMods/interface/ElectronIDChargeCorrelationStudyMod.h"
#include "MitHiggs/FakeRateMods/interface/WJetsFakeElectronAnalysisMod.h"
#include "MitHiggs/FakeRateMods/interface/FakeMuonAnalysisMod.h"
#include "MitHiggs/FakeRateMods/interface/FakeElectronAnalysisMod.h"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::MuonFakeRateAnalysisMod+;
#pragma link C++ class mithep::ComputeElectronFakeRateMod+;
#pragma link C++ class mithep::ComputeMuonFakeRateMod+;
#pragma link C++ class mithep::JetChargeCorrelationStudyMod+;
#pragma link C++ class mithep::WJetChargeCorrelationStudyMod+;
#pragma link C++ class mithep::ElectronIDChargeCorrelationStudyMod+;
#pragma link C++ class mithep::WJetsFakeElectronAnalysisMod+;
#pragma link C++ class mithep::FakeMuonAnalysisMod+;
#pragma link C++ class mithep::FakeElectronAnalysisMod+;
#endif
 
