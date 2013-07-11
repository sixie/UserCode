// $Id: MitHiggsHWWModsLinkDef.h,v 1.2 2008/11/23 15:59:11 sixie Exp $

#ifndef HWWMODS_LINKDEF_H
#define HWWMODS_LINKDEF_H
#include "MitHiggs/HwwMods/interface/HwwMCEvtSelMod.h"
#include "MitHiggs/HwwMods/interface/HwwEvtSelMod.h"
#include "MitHiggs/HwwMods/interface/VBFHwwEvtSelMod.h"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::VBFHwwEvtSelMod+;
#pragma link C++ class mithep::HwwMCEvtSelMod+;
#pragma link C++ class mithep::HwwEvtSelMod+;
#endif
