// $Id: rootlogon.C,v 1.1 2008/08/22 19:32:22 sixie Exp $
{
  gROOT->Macro("$CMSSW_BASE/src/MitAna/macros/setRootEnv.C+");

  loadmylib("MitHiggs","HwwMods");
}
