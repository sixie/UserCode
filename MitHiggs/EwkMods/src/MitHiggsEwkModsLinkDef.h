// $Id $

#ifndef EWKMODS_LINKDEF_H
#define EWKMODS_LINKDEF_H
#include "MitHiggs/EwkMods/interface/ZeeAnalysisMod.h"
#include "MitHiggs/EwkMods/interface/ZmumuAnalysisMod.h"
#include "MitHiggs/EwkMods/interface/WenuAnalysisMod.h"
#include "MitHiggs/EwkMods/interface/WmunuAnalysisMod.h"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::ZeeAnalysisMod+;
#pragma link C++ class mithep::ZmumuAnalysisMod+;
#pragma link C++ class mithep::WenuAnalysisMod+;
#pragma link C++ class mithep::WmunuAnalysisMod+;
#endif
