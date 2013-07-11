#!/bin/tcsh

echo "Arguments: $*"
set neventsPerJob=$1
set jobNumber=$2
set label=$3
set Spin=$4
set DecayModel1=$5
set DecayModel2=$6
set ProductionChannel=$7
set OffXVV=$8

#install it in local area

if ( "$Spin" == "0" && "$DecayModel1" == "7" && "$DecayModel2" == "7") then
  echo "use /afs/cern.ch/work/s/sixie/public/MCGenerators/JHUGenerator/JHUGenHggSpin0Plus.tgz"
  tar vxzf /afs/cern.ch/work/s/sixie/public/MCGenerators/JHUGenerator/JHUGenHggSpin0Plus.tgz
else if ( "$Spin" == "2" && "$DecayModel1" == "7" && "$DecayModel2" == "7") then
  echo "use /afs/cern.ch/work/s/sixie/public/MCGenerators/JHUGenerator/JHUGenHggSpin2Plus.tgz"
  tar vxzf /afs/cern.ch/work/s/sixie/public/MCGenerators/JHUGenerator/JHUGenHggSpin2Plus.tgz
else 
  echo "use /afs/cern.ch/work/s/sixie/public/MCGenerators/JHUGenerator/JHUGen.tgz"
  tar vxzf /afs/cern.ch/work/s/sixie/public/MCGenerators/JHUGenerator/JHUGen.tgz
endif

./JHUGen Collider=1 Process=${Spin} DecayMode1=${DecayModel1} DecayMode2=${DecayModel2} PChannel=${ProductionChannel} PDFSet=1 Unweighted=1 OffXVV=${OffXVV} VegasNc1=${neventsPerJob}


setenv SCRAM_ARCH slc5_amd64_gcc462
cd /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/
eval `scramv1 runtime -csh`
cd -

cmsMkdir /store/group/phys_higgs/sixie/MCGenerators/JHUGenerator/${label}/
cmsStage -f data/output.lhe /store/group/phys_higgs/sixie/MCGenerators/JHUGenerator/${label}/${label}_${jobNumber}.lhe

echo "DONE"
hostname

exit $status
