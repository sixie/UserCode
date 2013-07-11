#! /bin/csh -f

cd $CMSSW_BASE/src/
root -l $CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/makePlotsForNote.C+
mv *.gif *.eps $CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/plots/
mv *.tex $CMSSW_BASE/src/UserCode/sixie/doc/20090429_HWWFakeRateNote/
cd -

