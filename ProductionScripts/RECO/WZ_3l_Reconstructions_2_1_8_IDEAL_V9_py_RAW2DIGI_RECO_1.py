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
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_1.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_10.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_100.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_11.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_12.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_14.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_15.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_16.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_17.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_18.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_19.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_2.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_20.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_21.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_22.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_23.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_24.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_25.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_26.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_28.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_29.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_3.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_30.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_31.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_32.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_33.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_34.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_35.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_36.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_37.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_38.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_39.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_4.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_40.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_41.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_42.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_44.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_45.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_46.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_47.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_48.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_49.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_5.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_50.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_51.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_52.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_53.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_55.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_56.root",
"castor:/castor/cern.ch/user/s/sixie/RAW/WZ_3l_0/WZ_3l_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_59.root"
    )
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    #outputCommands = process.RECOEventContent.outputCommands,
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('/server/02a/sixie/RECO/WZ_3l_IDEAL_V9_py_RAW2DIGI_RECO_1.root'),
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
