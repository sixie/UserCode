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
    input = cms.untracked.int32(-1)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    "castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_56.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_58.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_59.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_6.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_60.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_61.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_62.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_63.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_65.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_66.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_67.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_68.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_69.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_7.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_70.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_71.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_72.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_74.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_75.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_76.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_77.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_78.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_79.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_8.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_80.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_81.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_82.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_83.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_84.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_85.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_86.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_87.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_88.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_89.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_9.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_90.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_91.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_92.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_93.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_94.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_95.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_96.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_97.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_98.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WW_2l_0/WW_2l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_99.root",

    )
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    #outputCommands = process.RECOEventContent.outputCommands,
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('/server/02a/sixie/RECO/WW_2l_IDEAL_V9_py_RAW2DIGI_RECO_2.root'),
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
