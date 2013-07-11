// $Id $

#ifndef ISOLATIONSTUDYMODS_LINKDEF_H
#define ISOLATIONSTUDYMODS_LINKDEF_H
#include "MitHiggs/IsolationStudyMods/interface/ElectronIsolationStudyMod.h"
#include "MitHiggs/IsolationStudyMods/interface/MuonIsolationStudyMod.h"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::ElectronIsolationStudyMod+;
#pragma link C++ class mithep::MuonIsolationStudyMod+;
#endif
