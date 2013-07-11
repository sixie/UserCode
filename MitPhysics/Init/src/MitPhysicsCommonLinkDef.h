// $Id: MitPhysicsCommonLinkDef.h,v 1.2 2009/02/21 13:04:09 sixie Exp $

#ifndef MITPHYSICS_INIT_LINKDEF_H
#define MITPHYSICS_INIT_LINKDEF_H
#include "MitPhysics/Init/interface/ModNames.h"
#include "MitPhysics/Init/interface/FakeObject.h"
#include "MitPhysics/Init/interface/FakeRate.h"
#include "MitPhysics/Init/interface/FakeEventHeader.h"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::ModNames+;

#pragma link C++ class mithep::FakeObject+;
#pragma link C++ class mithep::Collection<mithep::FakeObject>+;
#pragma link C++ class mithep::Array<mithep::FakeObject>+;
#pragma link C++ class mithep::ObjArray<mithep::FakeObject>+;

#pragma link C++ class mithep::FakeEventHeader+;
#pragma link C++ class mithep::Collection<mithep::FakeEventHeader>+;
#pragma link C++ class mithep::Array<mithep::FakeEventHeader>+;
#pragma link C++ class mithep::ObjArray<mithep::FakeEventHeader>+;

#pragma link C++ class mithep::FakeRate+;

#endif
