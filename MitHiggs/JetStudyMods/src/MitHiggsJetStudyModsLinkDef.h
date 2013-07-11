// $Id $

#ifndef JETSTUDYMODS_LINKDEF_H
#define JETSTUDYMODS_LINKDEF_H
#include "MitHiggs/JetStudyMods/interface/JetVetoStudyMod.h"
#include "MitHiggs/JetStudyMods/interface/VBFJetTagStudyMod.h"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::JetVetoStudyMod+;
#pragma link C++ class mithep::VBFJetTagStudyMod+;
#endif
