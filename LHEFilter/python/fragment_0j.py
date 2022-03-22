import FWCore.ParameterSet.Config as cms

externalLHEProducer = cms.EDProducer("ExternalLHEProducer",
    args = cms.vstring('/nfs/dust/cms/user/afiqaize/cms/toponium_210421/genproductions/bin/MadGraph5_aMCatNLO/EtaTToLL_0j_madgraph_4f_NNPDF31nnlo_13TeV_slc7_amd64_gcc700_CMSSW_10_6_19_tarball.tar.xz'),
    nEvents = cms.untracked.uint32(5000),
    numberOfParameters = cms.uint32(1),
    outputFile = cms.string('cmsgrid_final.lhe'),
    scriptName = cms.FileInPath('GeneratorInterface/LHEInterface/data/run_generic_tarball_cvmfs.sh')
)

# link to cards: 
# FIXME

import FWCore.ParameterSet.Config as cms
from Configuration.Generator.Pythia8CommonSettings_cfi import *
from Configuration.Generator.MCTunes2017.PythiaCP5Settings_cfi import *
from Configuration.Generator.PSweightsPythia.PythiaPSweightsSettings_cfi import *

generator = cms.EDFilter("Pythia8HadronizerFilter",
                         maxEventsToPrint = cms.untracked.int32(1),
                         pythiaPylistVerbosity = cms.untracked.int32(1),
                         filterEfficiency = cms.untracked.double(1.0),
                         pythiaHepMCVerbosity = cms.untracked.bool(False),
                         comEnergy = cms.double(13000.),
                         PythiaParameters = cms.PSet(
                             pythia8CommonSettingsBlock,
                             pythia8CP5SettingsBlock,
                             #pythia8PSweightsSettingsBlock,
                             parameterSets = cms.vstring('pythia8CommonSettings',
                                                         'pythia8CP5Settings',
                                                         #'pythia8PSweightsSettings',
                             )
                         )
)

etatMassFilter = cms.EDFilter("LHEInvariantMassFilter",
                              ParticleID = cms.vint32(24, 5, -24, -5),
                              minMass = cms.double(337.),
                              maxMass = cms.double(349.),
                              src = cms.InputTag("externalLHEProducer")
)

ProductionFilterSequence = cms.Sequence(etatMassFilter + generator)
