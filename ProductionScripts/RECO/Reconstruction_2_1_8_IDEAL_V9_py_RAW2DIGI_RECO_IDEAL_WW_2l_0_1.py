# Auto generated configuration file
# using: 
# Revision: 1.77.2.1 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/Reconstruction_2_1_8_IDEAL_V9.py -s RAW2DIGI,RECO --filein file:GEN-SIM-RAW.root --eventcontent RECO --datatier RECO --conditions FrontierConditions_GlobalTag,IDEAL_V9::All -n -1 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/RawToDigi_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('Configuration/GenProduction/python/Reconstruction_2_1_8_IDEAL_V9.py nevts:-1'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    "castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_1.root"
    )
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    #outputCommands = process.RECOEventContent.outputCommands,
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('/server/02a/sixie/RECO/WW_2l_IDEAL_V9_py_RAW2DIGI_RECO.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-RECO'),        
        filterName = cms.untracked.string('')
    )
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'IDEAL_V9::All'

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.out_step)
