import FWCore.ParameterSet.Config as cms
from Configuration.GenProduction.PythiaUESettings_cfi import *

source = cms.Source("PythiaSource",
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(0),
    pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(1.0),
    comEnergy = cms.untracked.double(10000.0),
    PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
        processParameters = cms.vstring(
      'MSEL=0                 ! User defined processes',
      'MSUB(25)=1             ! W+W- production from fermions',
      'MDME(190,1)=0          ! W decay into dbar u',
      'MDME(191,1)=0          ! W decay into dbar c',
      'MDME(192,1)=0          ! W decay into dbar t',
      'MDME(193,1)=0          ! W decay into dbar tprime',
      'MDME(194,1)=0          ! W decay into sbar u',
      'MDME(195,1)=0          ! W decay into sbar c',
      'MDME(196,1)=0          ! W decay into sbar t',
      'MDME(197,1)=0          ! W decay into sbar tprime',
      'MDME(198,1)=0          ! W decay into bbar u',
      'MDME(199,1)=0          ! W decay into bbar c',
      'MDME(200,1)=0          ! W decay into bbar t',
      'MDME(201,1)=0          ! W decay into bbar tprime',
      'MDME(202,1)=0          ! W decay into bprimebar u',
      'MDME(203,1)=0          ! W decay into bprimebar c',
      'MDME(204,1)=0          ! W decay into bprimebar t',
      'MDME(205,1)=0          ! W decay into bbar tprime',
      'MDME(206,1)=1          ! W decay into e+ nu_e',
      'MDME(207,1)=1          ! W decay into mu+ nu_mu',
      'MDME(208,1)=1          ! W decay into tau+ nu_tau',
      'MDME(209,1)=0          ! W decay into tauprime+ nuprime_tau'),
        # This is a vector of ParameterSet names to be read, in this order
        parameterSets = cms.vstring('pythiaUESettings', 
            'processParameters')
    )
)
