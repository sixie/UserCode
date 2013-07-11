// $Id: MitPhysicsValidationLinkDef.h,v 1.1 2008/11/12 15:04:51 sixie Exp $

#ifndef MITPHYSICS_VALIDATION_LINKDEF_H
#define MITPHYSICS_VALIDATION_LINKDEF_H
#include "MitPhysics/Validation/interface/JetValidationMod.h"
#include "MitPhysics/Validation/interface/ElectronIsolationStudyMod.h"
#include "MitPhysics/Validation/interface/MuonIsolationStudyMod.h"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::JetValidationMod+;
#pragma link C++ class mithep::ElectronIsolationStudyMod+;
#pragma link C++ class mithep::MuonIsolationStudyMod+;
#endif
