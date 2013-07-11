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
		"MSEL=0             !User defined processes",
                "MSUB(23)=1              !ffbar->Z/gamma W",
		# Z decays
		"MDME(174,1)=0            !Z decay into d dbar",
		"MDME(175,1)=0            !Z decay into u ubar",
		"MDME(176,1)=0            !Z decay into s sbar",
		"MDME(177,1)=0            !Z decay into c cbar",
		"MDME(178,1)=0            !Z decay into b bbar",
		"MDME(179,1)=0            !Z decay into t tbar",
		"MDME(180,1)=-1           !Z decay into b' b'bar",
		"MDME(181,1)=-1           !Z decay into t' t'bar",
		"MDME(182,1)=1            !Z decay into e- e+",
		"MDME(183,1)=0            !Z decay into nu_e nu_ebar",
		"MDME(184,1)=1            !Z decay into mu- mu+",
		"MDME(185,1)=0            !Z decay into nu_mu nu_mubar",
		"MDME(186,1)=1            !Z decay into tau- tau+",
		"MDME(187,1)=0            !Z decay into nu_tau nu_taubar",
		"MDME(188,1)=-1           !Z decay into tau'- tau'+",
		"MDME(189,1)=-1           !Z decay into nu'_tau nu'_taubar",
		# W decays
		"MDME(190,1)=0      !W decay into dbar u",
		"MDME(191,1)=0      !W decay into dbar c",
		"MDME(192,1)=0      !W decay into dbar t",
		"MDME(193,1)= -1    !W decay into dbar t'",
		"MDME(194,1)=0      !W decay into sbar u",
		"MDME(195,1)=0      !W decay into sbar c",
		"MDME(196,1)=0      !W decay into sbar t",
		"MDME(197,1)=-1     !W decay into sbar t'",
		"MDME(198,1)=0      !W decay into bbar u",
		"MDME(199,1)=0      !W decay into bbar c",
		"MDME(200,1)=0      !W decay into bbar t",
		"MDME(201,1)=-1     !W decay into bbar t'",
		"MDME(202,1)=-1     !W decay into b'bar u",
		"MDME(203,1)=-1     !W decay into b'bar c",
		"MDME(204,1)=-1     !W decay into b'bar t",
		"MDME(205,1)=-1     !W decay into bbar t'",
		"MDME(206,1)=1      !W decay into e+ nu_e",
		"MDME(207,1)=1      !W decay into mu+ nu_mu",
		"MDME(208,1)=1      !W decay into tau+ nu_tau",
		"MDME(209,1)=-1     !W decay into tau'+ nu'_tau"),
        # This is a vector of ParameterSet names to be read, in this order
        parameterSets = cms.vstring('pythiaUESettings', 
            'processParameters')
    )
)
