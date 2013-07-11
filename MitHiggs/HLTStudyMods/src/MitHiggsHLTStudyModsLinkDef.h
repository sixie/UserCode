// $Id $

#ifndef HLTSTUDYMODS_LINKDEF_H
#define HLTSTUDYMODS_LINKDEF_H
#include "MitHiggs/HLTStudyMods/interface/HwwHLTStudyMod.h"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::HwwHLTStudyMod+;
#endif
