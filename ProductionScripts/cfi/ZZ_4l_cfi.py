import FWCore.ParameterSet.Config as cms

from Configuration.Generator.PythiaUESettings_cfi import *
source = cms.Source("PythiaSource",
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(0),
    pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(1.0),
    comEnergy = cms.untracked.double(10000.0),
    PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
        processParameters = cms.vstring(
	    'MSEL=0                  !0=full user control (then use MSUB)',
            'MSUB(22)=1              !ffbar->Z/gamma Z/gamma'
            'MDME( 174,1) = 0    !Z decay into d dbar', 
            'MDME( 175,1) = 0    !Z decay into u ubar', 
            'MDME( 176,1) = 0    !Z decay into s sbar', 
            'MDME( 177,1) = 0    !Z decay into c cbar', 
            'MDME( 178,1) = 0    !Z decay into b bbar', 
            'MDME( 179,1) = 0    !Z decay into t tbar', 
            'MDME( 182,1) = 1    !Z decay into e- e+', 
            'MDME( 183,1) = 0    !Z decay into nu_e nu_ebar', 
            'MDME( 184,1) = 1    !Z decay into mu- mu+', 
            'MDME( 185,1) = 0    !Z decay into nu_mu nu_mubar', 
            'MDME( 186,1) = 1    !Z decay into tau- tau+', 
            'MDME( 187,1) = 0    !Z decay into nu_tau nu_taubar'),
        # This is a vector of ParameterSet names to be read, in this order
        parameterSets = cms.vstring('pythiaUESettings', 
            'processParameters')
    )
)