import FWCore.ParameterSet.Config as cms

process = cms.Process('Test')

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

process.load('Configuration/StandardSequences/Generator_cff')
process.load('Configuration/StandardSequences/VtxSmearedNoSmear_cff')
process.load('RecoJets/Configuration/GenJetParticles_cff')
process.load('RecoJets/Configuration/RecoGenJets_cff')

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10000)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.RandomNumberGeneratorService.generator = cms.PSet(
    initialSeed = cms.untracked.uint32(1)
)

process.source = cms.Source("LHESource",
                            fileNames = cms.untracked.vstring('file:/data/blue/sixie/MCGenerators/MCAtNLO/CT10/MCAtNLOHerwig_WWToENuENu_XXLABELXX/XX_JOBNUMBER_XX/writer.lhe',
                                                              'file:/data/blue/sixie/MCGenerators/MCAtNLO/CT10/MCAtNLOHerwig_WWToENuMuNu_XXLABELXX/XX_JOBNUMBER_XX/writer.lhe',
                                                               'file:/data/blue/sixie/MCGenerators/MCAtNLO/CT10/MCAtNLOHerwig_WWToENuTauNu_XXLABELXX/XX_JOBNUMBER_XX/writer.lhe',
                                                               'file:/data/blue/sixie/MCGenerators/MCAtNLO/CT10/MCAtNLOHerwig_WWToMuNuENu_XXLABELXX/XX_JOBNUMBER_XX/writer.lhe',
                                                               'file:/data/blue/sixie/MCGenerators/MCAtNLO/CT10/MCAtNLOHerwig_WWToMuNuMuNu_XXLABELXX/XX_JOBNUMBER_XX/writer.lhe',
                                                               'file:/data/blue/sixie/MCGenerators/MCAtNLO/CT10/MCAtNLOHerwig_WWToMuNuTauNu_XXLABELXX/XX_JOBNUMBER_XX/writer.lhe',
                                                               'file:/data/blue/sixie/MCGenerators/MCAtNLO/CT10/MCAtNLOHerwig_WWToTauNuENu_XXLABELXX/XX_JOBNUMBER_XX/writer.lhe',
                                                               'file:/data/blue/sixie/MCGenerators/MCAtNLO/CT10/MCAtNLOHerwig_WWToTauNuMuNu_XXLABELXX/XX_JOBNUMBER_XX/writer.lhe',
                                                               'file:/data/blue/sixie/MCGenerators/MCAtNLO/CT10/MCAtNLOHerwig_WWToTauNuTauNu_XXLABELXX/XX_JOBNUMBER_XX/writer.lhe'
                                                              )
)


process.writer = cms.EDAnalyzer("LHEWriter")


process.outpath = cms.EndPath(process.writer)
