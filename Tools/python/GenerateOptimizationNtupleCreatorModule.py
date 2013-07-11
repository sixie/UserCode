#!/usr/bin/env python

######################################################################
#Example to run
#python GenerateOptimizationNtupleCreatorModule.py --inputFile=input.cfg --DatasetListFile=/home/sixie/Tools/data/DataSampleCrossSections.txt

#root -l WWEventSelectionOptimizationTree_Optimization.C+\(\"/home/sixie/ntuples/WWEventSelectionOptimizationTree/filler/006/WWEventSelectionOptimizationTree_sig.root\",\"/home/sixie/ntuples/WWEventSelectionOptimizationTree/filler/006/WWEventSelectionOptimizationTree_bkg.root\"\)
######################################################################

import sys, getopt
import string

def usage():
    print "possible options are: --help, --inputFile=<myinputfile>, --DatasetListFile=<datasetListFile>"

inputFilename = ''
datasetListFilename = ''

if len(sys.argv[1:]) < 1:
    print "No input file specified."
    usage()
    sys.exit()

try:
    opts, args = getopt.getopt(sys.argv[1:], "hi:d:", ["help", "inputFile=", "DatasetListFile="])
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit()
        elif o in ("-i", "--inputFile"):
            inputFilename = a
        elif o in ("-d", "--DatasetListFile"):
            datasetListFilename = a    
        else:
            usage()
            sys.exit()
except getopt.GetoptError:
    usage()
    sys.exit(2)

if (datasetListFilename == ''):
    print "Error: No dataset list file specified."
    sys.exit()

########################################################################################
#Open Input file and read in information
########################################################################################
try:
    inputFile = open(inputFilename,"r")
except IOError:
    print "Error: The specified inputFile " + inputFilename + " could not be opened."
    sys.exit()

templine = inputFile.readline()
moduleName = ''
ntupleName = ''

########################################################################################
#Read in Ntuple Name, and Module Name
########################################################################################
#print"length %s" % len(templine.split())
if len(templine.split()) != 1:
    print "error: incorrect format for module name definition. Check Line 1 of the input file"
    sys.exit()
else:
    for word in templine.split():
        ntupleName = ntupleName + word
moduleName = ntupleName + "CreatorMod"

########################################################################################
#Read in Global Definitions
########################################################################################
#Global Definition Variables
FinalStates = list()
NumberOfOptimizationPoints = 200

templine = inputFile.readline()
for word in templine.split():
    if word != "##GlobalDefinitions" :
        print "Error: input file has incorrect format. Cannot find Global Definitions. Check Line 2 of the input file"
        sys.exit()

readGlobalDefinitionsComplete = False
templine = inputFile.readline()
lineNumber = 3
if len(templine.split()) == 1:
    tempInputList = templine.split()
    if tempInputList[0] == "##EndGlobalDefinitions":
        readGlobalDefinitionsComplete = True
        
while (readGlobalDefinitionsComplete == False):
    tempInputList = templine.split()
    if tempInputList[0] == 'FinalStates':
        wordList = templine.split()
        if (len(wordList) <= 5):
            print "Error: Incorrect format for definition of final states. check line %s" % (lineNumber)
            sys.exit()
        if (wordList[1] != '=' or wordList[2] != '{' or wordList[len(wordList)-1] != '}'):
            print "Error: incorrect format for definition of final states. check line %s" % (lineNumber)
            sys.exit()        
        wordcount = 3       
        while (wordcount < len(wordList)-1):
            if ((wordList[wordcount+1] != "," and (wordcount+1 != len(wordList)-1)) or ((wordList[wordcount+1] == "," and wordcount+2 == len(wordList)-1))):
                print "Error: incorrect format for definition of final states. check line %s" % (lineNumber)
                sys.exit()            
            if (wordList[wordcount] != ","):
                FinalStates.append(wordList[wordcount])
            wordcount += 2
        
    elif tempInputList[0] == 'NumberOfOptimizationPoints':
        if len(templine.split()) != 2:
            print "Error: incorrect format for definition of OptimizationPoints. check line %s" % (lineNumber)
            sys.exit()
        NumberOfOptimizationPoints = int(tempInputList[1])
    templine = inputFile.readline()
    lineNumber += 1        
    if len(templine.split()) == 1:
        tempInputList = templine.split()        
        if (tempInputList[0] == '##EndGlobalDefinitions'):
            readGlobalDefinitionsComplete = True


if len(FinalStates) <= 0:
    print "Error: number of final states is 0 or badly defined."
    sys.exit()

#for fs in FinalStates:
#    print fs
#sys.exit()

########################################################################################
#Read in List of Variables
########################################################################################
#variable list definitions
numberOfVariables = 0
variableNames = list()
variableTypes = list()
variableCutDirection = list()
variableCutInitialValue = list()
variableRangeMin = list()
variableRangeMax = list()
for i in range (0, len(FinalStates)):
    variableCutInitialValue.append(list())

templine = inputFile.readline()
lineNumber += 1
for word in templine.split():
    if word != "##CutVariables" :
        print "error: incorrect format Variable list. Ensure that the list of variables are inserted between lines with the string '##Variables'. Check Line %s of the input file" % (lineNumber)
        sys.exit()

readVariableListComplete = False
templine = inputFile.readline()
lineNumber += 1
if len(templine.split()) == 1:
    tempInputList = templine.split()
    if tempInputList[0] == "##EndCutVariables":
        readVariableListComplete = True
while (readVariableListComplete == False):
    if (templine[0] == "#"):
        templine = inputFile.readline()
        lineNumber += 1
        continue
    if len(templine.split()) != 3+len(FinalStates)+2:
        print "error: incorrect format for variable definition. Check line %s" % (lineNumber)
        sys.exit()
    else:
        tempInputList = templine.split()
        variableNames.append(tempInputList[0])
        variableTypes.append(tempInputList[1])        
        if (tempInputList[2] != "CutAbove") and (tempInputList[2] != "CutBelow"):
            print "Error: Incorrect Format. Cut direction input not recognized. Check Line %s" % (lineNumber)
            sys.exit()
        else:
            variableCutDirection.append(tempInputList[2])        
        for i in range(0,len(FinalStates)):
            variableCutInitialValue[i].append(tempInputList[3+i])
        numberOfVariables = numberOfVariables + 1
        variableRangeMin.append(tempInputList[3+len(FinalStates)])
        variableRangeMax.append(tempInputList[3+len(FinalStates)+1])
        #print numberOfVariables
    templine = inputFile.readline()
    lineNumber += 1
    if len(templine.split()) == 1:
        tempInputList = templine.split()        
        if (tempInputList[0] == '##EndCutVariables'):
            readVariableListComplete = True


########################################################################################
#Read in List of Datasets
########################################################################################
datasetNameList = list()
try:
    datasetListFile = open(datasetListFilename,"r")
except IOError:
    print "Error: The specified dataset list file " + datasetListFilename + " could not be opened."
    sys.exit()

lineNumber = 1
templine = datasetListFile.readline()
while len(templine.split()) > 0:
    if (len(templine.split()) != 2) and len(templine.split()) != 1:
        print "error: incorrect format for Dataset list file. Check line %s" % lineNumber
        sys.exit()
    else:
        if (templine[0] == "#"):
            templine = datasetListFile.readline()
            lineNumber += 1
            continue

        tempInputList = templine.split()
        datasetNameList.append(tempInputList[0])
        lineNumber += 1
    templine = datasetListFile.readline()
else:
    #print "done reading input"
    print ''
inputFile.close()

#Check the list of variables
count = 0
for l in datasetNameList:
    #print l
    count += 1


inputFile.close()
print "done reading input"
########################################################################################
#Check Input 
########################################################################################
#Check the list of variables
if 0 == 1:
    print "Number of Final States: " + str(len(FinalStates))
    print "Number of Optimization Points: " + str(NumberOfOptimizationPoints)
    count = 0
    for l in variableNames:
        tempstring = l+" "+variableTypes[count]+" " + variableCutDirection[count] + " : "
        for i in range (0,len(FinalStates)):
            tempstring += variableCutInitialValue[i][count] + " "
        tempstring += " : " + variableRangeMin[count] + " " + variableRangeMax[count]
        print tempstring    
        count += 1
    sys.exit()



########################################################################################
#Write the Header File
########################################################################################
outputModuleHeaderFile = open(moduleName + ".h", "w")
outputModuleHeaderFile.write('//--------------------------------------------------------------------------------------------------\n')
outputModuleHeaderFile.write('// $Id: GenerateOptimizationNtupleCreatorModule.py,v 1.1 2009/02/21 13:09:20 sixie Exp $\n')
outputModuleHeaderFile.write('//\n')
outputModuleHeaderFile.write('// ' + moduleName + '\n')
outputModuleHeaderFile.write('//\n')
outputModuleHeaderFile.write('// A Module to Create An ntuple for Optimization of Selection Cuts\n')
outputModuleHeaderFile.write('//\n')
outputModuleHeaderFile.write('//--------------------------------------------------------------------------------------------------\n')
outputModuleHeaderFile.write('#ifndef MITHIGGS_OPTIMIZATION_' + str.upper(moduleName) + '_H\n')
outputModuleHeaderFile.write('#define MITHIGGS_OPTIMIZATION_' + str.upper(moduleName) + '_H\n')
outputModuleHeaderFile.write('\n')
outputModuleHeaderFile.write('#include "MitAna/TreeMod/interface/BaseMod.h"\n')
outputModuleHeaderFile.write('#include "MitAna/DataTree/interface/Collections.h"\n')
outputModuleHeaderFile.write('\n')    
outputModuleHeaderFile.write('namespace mithep\n')
outputModuleHeaderFile.write('{\n')
outputModuleHeaderFile.write('  class ' + moduleName + ' : public BaseMod \n')
outputModuleHeaderFile.write('  {\n')
outputModuleHeaderFile.write('    public:\n')
outputModuleHeaderFile.write('    ' + moduleName + '(\n')
outputModuleHeaderFile.write('    const char *name="' + moduleName + '",\n')
outputModuleHeaderFile.write('    const char *title="module to create optimization ntuple");\n')
outputModuleHeaderFile.write('    ~'+ moduleName + '() {}\n')
outputModuleHeaderFile.write('\n')
outputModuleHeaderFile.write('    void         SetCleanJetsName (TString s)          { fCleanJetsName = s;            }\n')
outputModuleHeaderFile.write('    void         SetJetName (TString s)                { fJetName = s;                  }\n')
outputModuleHeaderFile.write('    void         SetOutputFilename (TString s)         { fOutputFilename = s;           }\n')
outputModuleHeaderFile.write('\n')
outputModuleHeaderFile.write('    protected:\n')
outputModuleHeaderFile.write('\n')
outputModuleHeaderFile.write('      TString                  fMCPartName;\n')
outputModuleHeaderFile.write('      TString                  fMetName;\n')
outputModuleHeaderFile.write('      TString                  fTrackName;\n')
outputModuleHeaderFile.write('      TString                  fJetName;\n')
outputModuleHeaderFile.write('      TString                  fCleanJetsName;\n')
outputModuleHeaderFile.write('      TString                  fMuonName;\n')
outputModuleHeaderFile.write('      const MCParticleCol     *fParticles;\n')
outputModuleHeaderFile.write('      const MetCol            *fMet;\n')
outputModuleHeaderFile.write('      const TrackCol          *fTracks;\n')
outputModuleHeaderFile.write('      const JetCol            *fJets;\n')
outputModuleHeaderFile.write('      const MuonCol           *fMuons;\n')
outputModuleHeaderFile.write('\n')
outputModuleHeaderFile.write('      float                    fOptimizationVariables[' + str(len(variableNames)+2) + '];\n')
outputModuleHeaderFile.write('      TString                  fOutputFilename;\n')
outputModuleHeaderFile.write('      TFile                   *fOutputFile;\n')
outputModuleHeaderFile.write('      TTree                   *fOptimizationTree;\n')
outputModuleHeaderFile.write('\n')
outputModuleHeaderFile.write('      void         Begin();\n')
outputModuleHeaderFile.write('      void         Process();\n')
outputModuleHeaderFile.write('      void         SlaveBegin();\n')
outputModuleHeaderFile.write('      void         SlaveTerminate();\n')
outputModuleHeaderFile.write('      void         Terminate();\n')
outputModuleHeaderFile.write('\n')    
outputModuleHeaderFile.write('    ClassDef(' + moduleName + ',1) // TAM analysis module \n')
outputModuleHeaderFile.write('  };\n')
outputModuleHeaderFile.write('}\n')
outputModuleHeaderFile.write('#endif\n')
outputModuleHeaderFile.close()

########################################################################################
#Write the cc File
########################################################################################
outputModuleCCFile = open(moduleName + ".cc", "w")
outputModuleCCFile.write('//--------------------------------------------------------------------------------------------------\n')
outputModuleCCFile.write('// $Id: GenerateOptimizationNtupleCreatorModule.py,v 1.1 2009/02/21 13:09:20 sixie Exp $\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('#include "MitHiggs/Optimization/interface/' + moduleName + '.h"\n')
outputModuleCCFile.write('#include "MitAna/DataUtil/interface/Debug.h"\n')
outputModuleCCFile.write('#include "MitAna/DataTree/interface/Names.h"\n')
outputModuleCCFile.write('#include "MitPhysics/Init/interface/ModNames.h"\n')
outputModuleCCFile.write('#include "MitAna/DataCont/interface/ObjArray.h"\n')
outputModuleCCFile.write('#include "MitCommon/MathTools/interface/MathUtils.h"\n')
outputModuleCCFile.write('#include "TFile.h"\n')
outputModuleCCFile.write('#include "TTree.h"\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('using namespace mithep;\n')
outputModuleCCFile.write('ClassImp(mithep::' + moduleName + ')\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('//--------------------------------------------------------------------------------------------------\n')
outputModuleCCFile.write(moduleName + '::' + moduleName + '\n')
outputModuleCCFile.write('(const char *name, const char *title) : \n')
outputModuleCCFile.write('  BaseMod(name,title),\n')
outputModuleCCFile.write('  fMCPartName(Names::gkMCPartBrn),\n')
outputModuleCCFile.write('  fMetName(Names::gkCaloMetBrn),\n')
outputModuleCCFile.write('  fTrackName(Names::gkTrackBrn),\n')
outputModuleCCFile.write('  fJetName(Names::gkCaloJetBrn),\n')
outputModuleCCFile.write('  fCleanJetsName(ModNames::gkCleanJetsName), \n')
outputModuleCCFile.write('  fMuonName(Names::gkMuonBrn),\n')
outputModuleCCFile.write('  fParticles(0),\n')
outputModuleCCFile.write('  fMet(0),\n')
outputModuleCCFile.write('  fTracks(0),\n')
outputModuleCCFile.write('  fJets(0),\n')
outputModuleCCFile.write('  fMuons(0),\n')
outputModuleCCFile.write('  fOutputFilename("'+ moduleName + '.ntuple.root")\n')
outputModuleCCFile.write('{\n')
outputModuleCCFile.write('  // Constructor.\n')
outputModuleCCFile.write('}\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('//--------------------------------------------------------------------------------------------------\n')
outputModuleCCFile.write('void ' + moduleName + '::Begin()\n')
outputModuleCCFile.write('{\n')
outputModuleCCFile.write('  // Run startup code on the client machine. For this module, we dont do\n')
outputModuleCCFile.write('  // anything here.\n')
outputModuleCCFile.write('}\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('//--------------------------------------------------------------------------------------------------\n')
outputModuleCCFile.write('void ' + moduleName + '::SlaveBegin()\n')
outputModuleCCFile.write('{\n')
outputModuleCCFile.write('  // Run startup code on the computer (slave) doing the actual analysis. Here,\n')
outputModuleCCFile.write('  // we typically initialize histograms and other analysis objects and request\n')
outputModuleCCFile.write('  // branches. For this module, we request a branch of the MitTree.\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('  ReqBranch(fMetName, fMet);\n')
outputModuleCCFile.write('  ReqBranch(fMCPartName, fParticles);\n')
outputModuleCCFile.write('  ReqBranch(fTrackName,  fTracks);\n')
outputModuleCCFile.write('  ReqBranch(fJetName,    fJets);\n')
outputModuleCCFile.write('  ReqBranch(fMuonName,   fMuons);\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('  fOptimizationTree = new TTree("' + ntupleName + '", \n')
outputModuleCCFile.write('                                "' + ntupleName + '");\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('  char* TreeFormat;\n')

outputModuleCCFile.write('  TreeFormat = "weight/F:finalstatetype/I:')
#Loop over variables
n = 0
for varname in variableNames:
    if n < len(variableNames)-1:
        outputModuleCCFile.write(varname + '/' + variableTypes[n] + ':')
    else:
        outputModuleCCFile.write(varname + '/' + variableTypes[n])
    n += 1
#Loop over variables
outputModuleCCFile.write('";\n')

outputModuleCCFile.write('  fOptimizationTree->Branch("optimization_branch", &fOptimizationVariables,TreeFormat);\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('  AddOutput(fOptimizationTree);\n')
outputModuleCCFile.write('}\n')
outputModuleCCFile.write('\n')

outputModuleCCFile.write('//--------------------------------------------------------------------------------------------------\n')
outputModuleCCFile.write('void ' + moduleName + '::Process()\n')
outputModuleCCFile.write('{\n')
outputModuleCCFile.write('  // Process entries of the tree. For this module, we just load the branches and  \n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('//***********************************************************************************************\n')
outputModuleCCFile.write('//Import Collections\n')
outputModuleCCFile.write('//***********************************************************************************************\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('  ObjArray<Muon> *CleanMuons = dynamic_cast<ObjArray<Muon>* >\n')
outputModuleCCFile.write('    (FindObjThisEvt(ModNames::gkCleanMuonsName));\n')
outputModuleCCFile.write('  ObjArray<Jet> *CleanJets = dynamic_cast<ObjArray<Jet>* >\n')
outputModuleCCFile.write('    (FindObjThisEvt(fCleanJetsName.Data()));\n')
outputModuleCCFile.write('  mithep::ParticleOArr *CleanLeptons = dynamic_cast<mithep::ParticleOArr*>\n')
outputModuleCCFile.write('    (FindObjThisEvt(ModNames::gkMergedLeptonsName));\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('  LoadBranch(fMetName);\n')
outputModuleCCFile.write('  const Met *caloMet = fMet->At(0);\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('  MDB(kAnalysis, 4) {\n')
outputModuleCCFile.write('    cerr << "Check Lepton Sort: " << CleanLeptons->GetEntries() << endl;\n')
outputModuleCCFile.write('    for(UInt_t i=0; i<CleanLeptons->GetEntries(); i++)\n')
outputModuleCCFile.write('      cerr << CleanLeptons->At(i)->Pt() << endl;\n')
outputModuleCCFile.write('  }\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('//***********************************************************************************************\n')
outputModuleCCFile.write('//Kinematic PreSelection\n')
outputModuleCCFile.write('//***********************************************************************************************\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('  MDB(kAnalysis, 4) {\n')
outputModuleCCFile.write('    cerr << "Event Passes PreSelection" << endl;  \n')
outputModuleCCFile.write('  }\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('//***********************************************************************************************\n')
outputModuleCCFile.write('//Define Event Variables\n')
outputModuleCCFile.write('//***********************************************************************************************\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('//***********************************************************************************************\n')
outputModuleCCFile.write('//Fill Optimization Variables\n')
outputModuleCCFile.write('//***********************************************************************************************\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.write('   for (int i=0;i<' + str(len(variableNames)+2) + ';i++) {\n')
outputModuleCCFile.write('     fOptimizationVariables[i] = 0.0;\n')
outputModuleCCFile.write('   }\n')
outputModuleCCFile.write('\n')

outputModuleCCFile.write('  fOptimizationVariables[0] = 1.0; //weight\n')
outputModuleCCFile.write('  fOptimizationVariables[1] = 0.0; //finalstatetype\n')
#Loop over variables
n = 2
for varname in variableNames:
    outputModuleCCFile.write('  fOptimizationVariables[' + str(n) + '] = 0.0; //' + varname + '\n')
    n += 1
#Loop over variables

outputModuleCCFile.write('  fOptimizationTree->Fill();\n')
outputModuleCCFile.write('}\n')
outputModuleCCFile.write('\n')

outputModuleCCFile.write('//--------------------------------------------------------------------------------------------------\n')
outputModuleCCFile.write('void ' + moduleName + '::SlaveTerminate()\n')
outputModuleCCFile.write('{\n')
outputModuleCCFile.write('  // Run finishing code on the computer (slave) that did the analysis. For this\n')
outputModuleCCFile.write('  // module, we dont do anything here.\n')
outputModuleCCFile.write('}\n')
outputModuleCCFile.write('\n')

outputModuleCCFile.write('//--------------------------------------------------------------------------------------------------\n')
outputModuleCCFile.write('void ' + moduleName + '::Terminate()\n')
outputModuleCCFile.write('{\n')
outputModuleCCFile.write('  // Run finishing code on the client computer. For this module, we dont do\n')
outputModuleCCFile.write('  // anything here.\n')
outputModuleCCFile.write('}\n')
outputModuleCCFile.write('\n')
outputModuleCCFile.close()




########################################################################################
#Write run Macro
########################################################################################
outputModuleRunMacroFile = open("run" + moduleName + ".C", "w")
outputModuleRunMacroFile.write('// $Id: GenerateOptimizationNtupleCreatorModule.py,v 1.1 2009/02/21 13:09:20 sixie Exp $\n')
outputModuleRunMacroFile.write('\n')
outputModuleRunMacroFile.write('#if !defined(__CINT__) || defined(__MAKECINT__)\n')
outputModuleRunMacroFile.write('#include <TROOT.h>\n')
outputModuleRunMacroFile.write('#include <exception>\n')
outputModuleRunMacroFile.write('#include "MitAna/DataUtil/interface/Debug.h"\n')
outputModuleRunMacroFile.write('#include "MitAna/Catalog/interface/Catalog.h"\n')
outputModuleRunMacroFile.write('#include "MitAna/TreeMod/interface/Analysis.h"\n')
outputModuleRunMacroFile.write('#include "MitAna/TreeMod/interface/HLTMod.h"\n')
outputModuleRunMacroFile.write('#include "MitPhysics/Mods/interface/MuonIDMod.h"\n')
outputModuleRunMacroFile.write('#include "MitPhysics/Mods/interface/ElectronIDMod.h"\n')
outputModuleRunMacroFile.write('#include "MitPhysics/Mods/interface/PhotonIDMod.h"\n')
outputModuleRunMacroFile.write('#include "MitPhysics/Mods/interface/JetIDMod.h"\n')
outputModuleRunMacroFile.write('#include "MitPhysics/Mods/interface/ElectronCleaningMod.h"\n')
outputModuleRunMacroFile.write('#include "MitPhysics/Mods/interface/PhotonCleaningMod.h"\n')
outputModuleRunMacroFile.write('#include "MitPhysics/Mods/interface/JetCleaningMod.h"\n')
outputModuleRunMacroFile.write('#include "MitPhysics/Mods/interface/MergeLeptonsMod.h"\n')
outputModuleRunMacroFile.write('#include "MitHiggs/Optimization/interface/' + moduleName + '.h"\n')
outputModuleRunMacroFile.write('#endif\n')
outputModuleRunMacroFile.write('\n')
outputModuleRunMacroFile.write('//--------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('void execute' + moduleName + '(const char *fileset  = "",\n')
outputModuleRunMacroFile.write('                        const char *dataset    = "s8-ttbar-id9",\n')
outputModuleRunMacroFile.write('                        const char *book       = "mit/filler/006",\n')
outputModuleRunMacroFile.write('                        const char *catalogDir = "/home/mitprod/catalog",\n')
outputModuleRunMacroFile.write('                        const char *outputName = "' + moduleName + '",\n')
outputModuleRunMacroFile.write('                        int         nEvents    = -1)\n')
outputModuleRunMacroFile.write('{\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  // some global setups\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  using namespace mithep;\n')
outputModuleRunMacroFile.write('  gDebugMask  = Debug::kAnalysis;\n')
outputModuleRunMacroFile.write('  gDebugLevel = 3;\n')
outputModuleRunMacroFile.write('\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  // HLT information\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  HLTMod *hltmod = new HLTMod;\n')
outputModuleRunMacroFile.write('  hltmod->SetPrintTable(kTRUE);\n')
outputModuleRunMacroFile.write('  hltmod->AddTrigger("HLT_IsoEle15_L1I");\n')
outputModuleRunMacroFile.write('  hltmod->AddTrigger("HLT_Ele15_SW_L1R");\n')
outputModuleRunMacroFile.write('  hltmod->AddTrigger("HLT_IsoEle15_LW_L1I");\n')
outputModuleRunMacroFile.write('  hltmod->AddTrigger("HLT_IsoEle18_L1R");\n')
outputModuleRunMacroFile.write('  hltmod->AddTrigger("HLT_Ele15_LW_L1R");\n')
outputModuleRunMacroFile.write('  hltmod->AddTrigger("HLT_LooseIsoEle15_LW_L1R");\n')
outputModuleRunMacroFile.write('  hltmod->AddTrigger("HLT_IsoMu15");\n')
outputModuleRunMacroFile.write('  hltmod->AddTrigger("HLT_Mu15");\n')
outputModuleRunMacroFile.write('  hltmod->SetTrigObjsName("myhltobjs");\n')
outputModuleRunMacroFile.write('\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  // object id and cleaning sequence\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  MuonIDMod           *muonID           = new MuonIDMod;  \n')
outputModuleRunMacroFile.write('  muonID->SetIDType("Loose");\n')
outputModuleRunMacroFile.write('  muonID->SetIsoType("TrackCaloSliding"); \n')
outputModuleRunMacroFile.write('  ElectronIDMod       *electronID       = new ElectronIDMod;\n')
outputModuleRunMacroFile.write('  electronID->SetIDType(TString("Tight"));\n')
outputModuleRunMacroFile.write('  electronID->SetIsoType(TString("TrackJuraSliding"));\n')
outputModuleRunMacroFile.write('  PhotonIDMod         *photonID       = new PhotonIDMod;\n')
outputModuleRunMacroFile.write('  photonID->SetIDType(TString("Tight"));\n')
outputModuleRunMacroFile.write('  photonID->SetPtMin(20.0);\n')
outputModuleRunMacroFile.write('  JetIDMod            *jetID            = new JetIDMod;\n')
outputModuleRunMacroFile.write('  jetID->SetInputName(TString("SisCone5Jets"));\n')
outputModuleRunMacroFile.write('  jetID->SetGoodName(TString("GoodJets"));\n')
outputModuleRunMacroFile.write('  jetID->SetUseCorrection(kTRUE);\n')
outputModuleRunMacroFile.write('  jetID->SetPtCut(35.0);\n')
outputModuleRunMacroFile.write('  ElectronCleaningMod *electronCleaning = new ElectronCleaningMod;\n')
outputModuleRunMacroFile.write('  PhotonCleaningMod   *photonCleaning   = new PhotonCleaningMod;\n')
outputModuleRunMacroFile.write('  JetCleaningMod      *jetCleaning      = new JetCleaningMod;\n')
outputModuleRunMacroFile.write('  jetCleaning->SetGoodJetsName (TString("GoodJets"));\n')
outputModuleRunMacroFile.write('  jetCleaning->SetCleanJetsName(TString("CleanJets"));\n')
outputModuleRunMacroFile.write('\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  // merge modules\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  MergeLeptonsMod *mergeLeptonsMod = new MergeLeptonsMod;\n')
outputModuleRunMacroFile.write('\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  // analyses modules\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  ' + moduleName + '  *analysisMod = \n')
outputModuleRunMacroFile.write('    new ' + moduleName + ';\n')
outputModuleRunMacroFile.write('  analysisMod->SetCleanJetsName(TString("CleanJets"));\n')
outputModuleRunMacroFile.write('  analysisMod->SetJetName(TString("SisCone5Jets"));\n')
outputModuleRunMacroFile.write('\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  // making analysis chain\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  hltmod->Add(muonID);\n')
outputModuleRunMacroFile.write('  muonID->Add(electronID);\n')
outputModuleRunMacroFile.write('  electronID->Add(photonID);\n')
outputModuleRunMacroFile.write('  photonID->Add(jetID);\n')
outputModuleRunMacroFile.write('  jetID->Add(electronCleaning);\n')
outputModuleRunMacroFile.write('  electronCleaning->Add(photonCleaning);\n')
outputModuleRunMacroFile.write('  photonCleaning->Add(jetCleaning);\n')
outputModuleRunMacroFile.write('  jetCleaning->Add(mergeLeptonsMod);\n')
outputModuleRunMacroFile.write('  mergeLeptonsMod->Add(analysisMod);\n')
outputModuleRunMacroFile.write('\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  // setup analysis\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  Analysis *ana = new Analysis;\n')
outputModuleRunMacroFile.write('  ana->SetUseHLT(kFALSE);\n')
outputModuleRunMacroFile.write('  ana->SetKeepHierarchy(kTRUE);\n')
outputModuleRunMacroFile.write('  if (nEvents >= 0)\n')
outputModuleRunMacroFile.write('    ana->SetProcessNEvents(nEvents);\n')
outputModuleRunMacroFile.write('  ana->SetSuperModule(muonID);\n')
outputModuleRunMacroFile.write('\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  // organize input\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  printf("\\nRely on Catalog: %s\\n",catalogDir);\n')
outputModuleRunMacroFile.write('  printf("  -> Book: %s  Dataset: %s  Fileset: %s <-\\n\\n",book,dataset,fileset);\n')
outputModuleRunMacroFile.write('  Catalog *c = new Catalog(catalogDir);\n')
outputModuleRunMacroFile.write('  Dataset *d = c->FindDataset(book,dataset,fileset);\n')
outputModuleRunMacroFile.write('  ana->AddDataset(d);\n')
outputModuleRunMacroFile.write('\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  // organize output\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  TString rootFile = TString(outputName);\n')
outputModuleRunMacroFile.write('  rootFile += TString("_") + TString(dataset);\n')
outputModuleRunMacroFile.write('  if (TString(fileset) != TString(""))\n')
outputModuleRunMacroFile.write('    rootFile += TString("_") + TString(fileset);\n')
outputModuleRunMacroFile.write('  rootFile += TString(".root");\n')
outputModuleRunMacroFile.write('  printf("\\nRoot output: %s\\n\\n",rootFile.Data());  \n')
outputModuleRunMacroFile.write('  ana->SetOutputName(rootFile.Data());\n')
outputModuleRunMacroFile.write('\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  // run the analysis after successful initialisation\n')
outputModuleRunMacroFile.write('  //------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('  ana->Run(!gROOT->IsBatch());  \n')
outputModuleRunMacroFile.write('\n')
outputModuleRunMacroFile.write('  return;\n')
outputModuleRunMacroFile.write('}\n')
outputModuleRunMacroFile.write('\n')
outputModuleRunMacroFile.write('//--------------------------------------------------------------------------------------------------\n')
outputModuleRunMacroFile.write('void run' + moduleName + '(const char *fileset      = "",\n')
outputModuleRunMacroFile.write('                        const char *dataset      = "s8-ttbar-id9",\n')
outputModuleRunMacroFile.write('                        const char *book         = "mit/filler/006",\n')
outputModuleRunMacroFile.write('                        const char *catalogDir   = "/home/mitprod/catalog",\n')
outputModuleRunMacroFile.write('                        const char *outputName   = "WWSelection",\n')
outputModuleRunMacroFile.write('                        int         nEvents      = -1, \n')
outputModuleRunMacroFile.write('                        int         runTypeIndex = -1)\n')
outputModuleRunMacroFile.write('{\n')
outputModuleRunMacroFile.write('\n')
outputModuleRunMacroFile.write('  TString outfileName = TString(outputName);\n')
outputModuleRunMacroFile.write('\n')
outputModuleRunMacroFile.write('  if (runTypeIndex == -1) {\n')
outputModuleRunMacroFile.write('    execute'+moduleName+'(fileset,dataset,book,catalogDir,outfileName,nEvents);\n')
outputModuleRunMacroFile.write('  }\n')
outputModuleRunMacroFile.write('}\n')
outputModuleRunMacroFile.write('\n')
outputModuleRunMacroFile.close()

########################################################################################
#Write the condor submit script
########################################################################################
outputModuleCondorSubmitScript = open("submitAll_" + ntupleName + ".sh", "w")

for dataset in datasetNameList:
    outputModuleCondorSubmitScript.write('./submit.csh /home/sixie/CMSSW_2_1_11/src/MitHiggs/macros/runMacros/' + 'run' + moduleName + '.C+ /home/mitprod/catalog/mit filler/006 ' + dataset + ' ' + ntupleName + ' /server/02a/sixie/ntuples -1 \n')
outputModuleCondorSubmitScript.write('\n')
    

########################################################################################
#Write the Reweighting Macro
########################################################################################
outputModuleReweightMacroFile = open(ntupleName + "_reweightTree.C", "w")
outputModuleReweightMacroFile.write('#include <string.h>\n')
outputModuleReweightMacroFile.write('#include "TFile.h"\n')
outputModuleReweightMacroFile.write('#include "TDirectory.h"\n')
outputModuleReweightMacroFile.write('#include "TTree.h"\n')
outputModuleReweightMacroFile.write('#include "TH1F.h"\n')
outputModuleReweightMacroFile.write('\n')
outputModuleReweightMacroFile.write('\n')
outputModuleReweightMacroFile.write('void ' + ntupleName + '_reweightTree(const string inputfile, string datasetName, double IntegratedLuminosity) {\n')
outputModuleReweightMacroFile.write('  mithep::SimpleTable xstab("$CMSSW_BASE/src/MitPhysics/data/xs.dat");\n')
outputModuleReweightMacroFile.write('  double CrossSection = xstab.Get(datasetName.c_str());\n')

outputModuleReweightMacroFile.write('  TFile* tmpfile = new TFile(inputfile.c_str(),"READ");\n')
outputModuleReweightMacroFile.write('  string outputfile = inputfile + ".reweighted.root";\n')
outputModuleReweightMacroFile.write('  TFile* outfile = new TFile(outputfile.c_str(),"RECREATE");\n')
outputModuleReweightMacroFile.write('  TDirectory *analysisDir = tmpfile->GetDirectory("MuonIDMod/ElectronIDMod/PhotonIDMod/JetIDMod/ElectronCleaningMod/PhotonCleaningMod/JetCleaningMod/MergeLeptonsMod/' + moduleName + '");\n')
outputModuleReweightMacroFile.write('  TDirectory *FWDir = tmpfile->GetDirectory("AnaFwkMod");\n')
outputModuleReweightMacroFile.write('  TTree* tmpTree = (TTree*)analysisDir->Get("' + ntupleName + '");\n')
outputModuleReweightMacroFile.write('  TH1F* tmpHist = (TH1F*)FWDir->Get("hDEvents");  \n')
outputModuleReweightMacroFile.write('\n')
outputModuleReweightMacroFile.write('  double NEvents = tmpHist->Integral();\n')
outputModuleReweightMacroFile.write('\n')
outputModuleReweightMacroFile.write('  Float_t fOptimizationVariables[' + str(len(variableNames)+2) + '];\n')
outputModuleReweightMacroFile.write('  tmpTree->SetBranchAddress("optimization_branch",&fOptimizationVariables );\n')
outputModuleReweightMacroFile.write('\n')
outputModuleReweightMacroFile.write('  Int_t nentries = (Int_t)tmpTree->GetEntries();\n')
outputModuleReweightMacroFile.write('\n')
outputModuleReweightMacroFile.write('  TTree *fOptimizationTree = new TTree("' + ntupleName + '", \n')
outputModuleReweightMacroFile.write('                                       "' + ntupleName + '");\n')
outputModuleReweightMacroFile.write('  char* TreeFormat;\n')

outputModuleReweightMacroFile.write('  TreeFormat = "weight/F:finalstatetype/I:')
#Loop over variables
n = 0
for varname in variableNames:
    if n < len(variableNames)-1:
        outputModuleReweightMacroFile.write(varname + '/' + variableTypes[n] + ':')
    else:
        outputModuleReweightMacroFile.write(varname + '/' + variableTypes[n])
    n += 1
#Loop over variables
outputModuleReweightMacroFile.write('";\n')

outputModuleReweightMacroFile.write('  fOptimizationTree->Branch("optimization_branch", &fOptimizationVariables,TreeFormat);\n')
outputModuleReweightMacroFile.write('\n')
outputModuleReweightMacroFile.write('  for (int n=0;n<nentries;n++) {    \n')
outputModuleReweightMacroFile.write('    tmpTree->GetEntry(n);\n')
outputModuleReweightMacroFile.write('    fOptimizationVariables[0] = CrossSection*IntegratedLuminosity / NEvents;\n')
outputModuleReweightMacroFile.write('    fOptimizationTree->Fill();\n')
outputModuleReweightMacroFile.write('  }\n')
outputModuleReweightMacroFile.write('\n')
outputModuleReweightMacroFile.write('  outfile->cd();\n')
outputModuleReweightMacroFile.write('  fOptimizationTree->Write();\n')
outputModuleReweightMacroFile.write('  outfile->Close();\n')
outputModuleReweightMacroFile.write('  tmpfile->Close();\n')
outputModuleReweightMacroFile.write('}\n')



########################################################################################
#Write the Ntuple Analysis template
########################################################################################
outputModuleOptimizationMacroFile = open(ntupleName + "_Optimization.C", "w")
outputModuleOptimizationMacroFile.write('#include <iostream>\n')
outputModuleOptimizationMacroFile.write('#include <stdlib.h>\n')
outputModuleOptimizationMacroFile.write('#include <string.h>\n')
outputModuleOptimizationMacroFile.write('#include <vector>\n')
outputModuleOptimizationMacroFile.write('#include <utility>\n')
outputModuleOptimizationMacroFile.write('#include "TFile.h"\n')
outputModuleOptimizationMacroFile.write('#include "TMath.h"\n')
outputModuleOptimizationMacroFile.write('#include "TDirectory.h"\n')
outputModuleOptimizationMacroFile.write('#include "TTree.h"\n')
outputModuleOptimizationMacroFile.write('#include "TH1F.h"\n')
outputModuleOptimizationMacroFile.write('#include "TCanvas.h"\n')
outputModuleOptimizationMacroFile.write('#include "TLegend.h"\n')


outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('\n')

outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('//Define Global Variables for the macro\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('string fOptimizationVariableNames[' + str(len(variableNames)) + '] = { ')
for j in range(0,len(variableNames)):
        if j < len(variableNames)-1:
            outputModuleOptimizationMacroFile.write('"' + variableNames[j] + '", ')
        else:
            outputModuleOptimizationMacroFile.write('"' + variableNames[j] + '" ')
outputModuleOptimizationMacroFile.write('};\n')
outputModuleOptimizationMacroFile.write('string fOptimizationVariableCutDirection[' + str(len(variableNames)) + '] = { ')
for j in range(0,len(variableNames)):
        if j < len(variableNames)-1:
            outputModuleOptimizationMacroFile.write('"' + variableCutDirection[j] + '", ')
        else:
            outputModuleOptimizationMacroFile.write('"' + variableCutDirection[j] + '" ')
outputModuleOptimizationMacroFile.write('};\n')
outputModuleOptimizationMacroFile.write('Float_t fOptimizationVariableInitialValue[' + str(len(FinalStates)) + '][' + str(len(variableNames)) + '] = { ')

for i in range(0,len(FinalStates)):
    outputModuleOptimizationMacroFile.write('{')
    for j in range(0,len(variableNames)):
        if j < len(variableNames)-1:
            outputModuleOptimizationMacroFile.write(variableCutInitialValue[i][j] + ', ')
        else:
            outputModuleOptimizationMacroFile.write(variableCutInitialValue[i][j])            
    if (i < len(FinalStates)-1):
        outputModuleOptimizationMacroFile.write('}, ')
    else:
        outputModuleOptimizationMacroFile.write('}')

outputModuleOptimizationMacroFile.write('};\n')

outputModuleOptimizationMacroFile.write('Float_t fOptimizationVariableRangeMin[' + str(len(variableNames)) + '] = { ')
for j in range(0,len(variableNames)):
    if j < len(variableNames)-1:
        outputModuleOptimizationMacroFile.write(variableRangeMin[j] + ', ')
    else:
        outputModuleOptimizationMacroFile.write(variableRangeMin[j])
outputModuleOptimizationMacroFile.write('};\n')
outputModuleOptimizationMacroFile.write('Float_t fOptimizationVariableRangeMax[' + str(len(variableNames)) + '] = { ')
for j in range(0,len(variableNames)):
    if j < len(variableNames)-1:
        outputModuleOptimizationMacroFile.write(variableRangeMax[j] + ',')
    else:
        outputModuleOptimizationMacroFile.write(variableRangeMax[j])
outputModuleOptimizationMacroFile.write('};\n')
outputModuleOptimizationMacroFile.write('bool fOptimizeThisVariable[' + str(len(variableNames)) + '] = { ')
for j in range(0,len(variableNames)):
    if j < len(variableNames)-1:
        outputModuleOptimizationMacroFile.write('true, ')
    else:
        outputModuleOptimizationMacroFile.write('true ')
outputModuleOptimizationMacroFile.write('};\n')
    
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('\n')

outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('//Convert int to string\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('string IntToString(int i) {\n')
outputModuleOptimizationMacroFile.write('  char temp[100];\n')
outputModuleOptimizationMacroFile.write('  sprintf(temp, "%d", i);\n')
outputModuleOptimizationMacroFile.write('  string str = temp;\n')
outputModuleOptimizationMacroFile.write('  return str;\n')
outputModuleOptimizationMacroFile.write('}\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('//Function to determine whether the event passes the defined cuts or not\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('bool passCut(Float_t fOptimizationVariables[' + str(len(variableNames)) + '], Float_t fCutValues[' + str(len(variableNames)) + ']  , int cutExceptionVariableIndex = -1) {\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  bool pass = true;\n')
for j in range (0,len(variableNames)):
    outputModuleOptimizationMacroFile.write('  pass = pass && (fOptimizationVariables[' + str(j) + '] ')
    if variableCutDirection[j] == "CutAbove":
        outputModuleOptimizationMacroFile.write( '<' )
    elif variableCutDirection[j] == "CutBelow":
        outputModuleOptimizationMacroFile.write( '>' )
    else:
        print "Error. variableCutDirection[" + str(j) + "] = " + variableCutDirection[j] + " is not recognized."
    outputModuleOptimizationMacroFile.write(' fCutValues[' + str(j) + '] || cutExceptionVariableIndex == ' + str(j) + ' );\n')

outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('return pass;\n')
outputModuleOptimizationMacroFile.write('}\n')
outputModuleOptimizationMacroFile.write('\n')

outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('//Function to create a distribution histogram of the variable given\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('TH1F* makeDistributionHistogram ( int variableIndex, TTree* tree , bool isSignal, int OnlyThisFinalState = -1) {\n')
outputModuleOptimizationMacroFile.write('  assert(variableIndex >= 0 && variableIndex < ' + str(len(variableNames))+ ');\n')
outputModuleOptimizationMacroFile.write('  string label = (isSignal)?"NSignalEventsPass":"NBkgEventsPass";\n')
outputModuleOptimizationMacroFile.write('  string histName = "h" + fOptimizationVariableNames[variableIndex] + "_" + label;\n')
outputModuleOptimizationMacroFile.write('  string axisLabel = ";" + fOptimizationVariableNames[variableIndex] + ";Number of Events";\n')
outputModuleOptimizationMacroFile.write('  TH1F *histogram = new TH1F(histName.c_str(),axisLabel.c_str(),' + str(NumberOfOptimizationPoints) + ', fOptimizationVariableRangeMin[variableIndex], fOptimizationVariableRangeMax[variableIndex]);\n')
outputModuleOptimizationMacroFile.write('  histogram->SetDirectory(0);\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  //define array to hold optimization branch information\n')
outputModuleOptimizationMacroFile.write('  Float_t fOptimizationVariables[' + str(len(variableNames)+2) + '];\n')
outputModuleOptimizationMacroFile.write('  tree->SetBranchAddress("optimization_branch",&fOptimizationVariables );\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  Int_t nentries = (Int_t)tree->GetEntries();\n')
outputModuleOptimizationMacroFile.write('  for (int n=0;n<nentries;n++) {    \n')
outputModuleOptimizationMacroFile.write('    tree->GetEntry(n);\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('    float eventweight = fOptimizationVariables[0];\n')
outputModuleOptimizationMacroFile.write('    int finalstatetype = (int)fOptimizationVariables[1];\n')
outputModuleOptimizationMacroFile.write('    if (!(finalstatetype >= 1 && finalstatetype <= ' + str(len(FinalStates)) + ')) {\n')
outputModuleOptimizationMacroFile.write('      cout << "finalstatetype = " << finalstatetype << " unrecognized" << endl;\n')
outputModuleOptimizationMacroFile.write('      assert(!(finalstatetype >= 1 && finalstatetype <= ' + str(len(FinalStates)) + '));\n')
outputModuleOptimizationMacroFile.write('    }\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('    //only look at events with the given finalstatetype\n')
outputModuleOptimizationMacroFile.write('    if (OnlyThisFinalState > 0) {\n')
outputModuleOptimizationMacroFile.write('      if (OnlyThisFinalState != finalstatetype)\n')
outputModuleOptimizationMacroFile.write('        continue;\n')
outputModuleOptimizationMacroFile.write('    }\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('    Float_t cutValues[' + str(len(variableNames)) + '];\n')
outputModuleOptimizationMacroFile.write('    Float_t variableValues[' + str(len(variableNames)) + '];\n')
outputModuleOptimizationMacroFile.write('    for (int j=0;j<' + str(len(variableNames)) + ';j++) {\n')
outputModuleOptimizationMacroFile.write('      cutValues[j] = fOptimizationVariableInitialValue[finalstatetype-1][j];\n')
outputModuleOptimizationMacroFile.write('      variableValues[j] = fOptimizationVariables[2+j];\n')
outputModuleOptimizationMacroFile.write('    }\n')
outputModuleOptimizationMacroFile.write('    if (passCut(variableValues, cutValues, variableIndex)) {\n')
outputModuleOptimizationMacroFile.write('      histogram->Fill(fOptimizationVariables[2+variableIndex], eventweight);\n')
outputModuleOptimizationMacroFile.write('    }  \n')
outputModuleOptimizationMacroFile.write('  }\n')
outputModuleOptimizationMacroFile.write('  return histogram;\n')
outputModuleOptimizationMacroFile.write('}\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('//Function to create a histogram of the number of events passing the given cut value\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('TH1F* makeNMinusOneHistogram ( int variableIndex, TH1F *distributionHist , bool isSignal) {\n')
outputModuleOptimizationMacroFile.write('  string label = (isSignal)?"NSignalEventsPass":"NBkgEventsPass";\n')
outputModuleOptimizationMacroFile.write('  string histName = "h" + fOptimizationVariableNames[variableIndex] + "_" + label;\n')
outputModuleOptimizationMacroFile.write('  string axisLabel = ";" + fOptimizationVariableNames[variableIndex] + ";Number of Events";\n')
outputModuleOptimizationMacroFile.write('  TH1F *histogram = new TH1F(histName.c_str(),axisLabel.c_str(),' + str(NumberOfOptimizationPoints) + ', fOptimizationVariableRangeMin[variableIndex], fOptimizationVariableRangeMax[variableIndex]);\n')
outputModuleOptimizationMacroFile.write('  histogram->SetDirectory(0);\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  assert(variableIndex >= 0 && variableIndex < ' + str(len(variableNames)) + ');\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  for (int i=0;i<=histogram->GetXaxis()->GetNbins()+1;i++) {\n')
outputModuleOptimizationMacroFile.write('    float tempCutValue = 0.0;\n')
outputModuleOptimizationMacroFile.write('    if (i <= histogram->GetXaxis()->GetNbins())\n')
outputModuleOptimizationMacroFile.write('      tempCutValue = histogram->GetXaxis()->GetBinUpEdge(i);\n')
outputModuleOptimizationMacroFile.write('    else\n')
outputModuleOptimizationMacroFile.write('      tempCutValue = histogram->GetXaxis()->GetBinLowEdge(i);\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('    double eventsPassed = 0.0;\n')
outputModuleOptimizationMacroFile.write('    if (fOptimizationVariableCutDirection[variableIndex] == "CutBelow")\n')
outputModuleOptimizationMacroFile.write('      eventsPassed = distributionHist->Integral(i,distributionHist->GetXaxis()->GetNbins()+1);\n')
outputModuleOptimizationMacroFile.write('    else if (fOptimizationVariableCutDirection[variableIndex] == "CutAbove")\n')
outputModuleOptimizationMacroFile.write('      eventsPassed = distributionHist->Integral(0,i);\n')

outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('    histogram->SetBinContent(i,eventsPassed);\n')
outputModuleOptimizationMacroFile.write('  }\n')
outputModuleOptimizationMacroFile.write('  return histogram;\n')
outputModuleOptimizationMacroFile.write('}\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('//Function to create a histogram of the significance as a function of the cut\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('TH1F* makeSignificanceHistogram ( int variableIndex, TH1F *signalHist ,TH1F *bkgHist, int significanceType) {\n')
outputModuleOptimizationMacroFile.write('  string histName = "hSignificance" + IntToString(significanceType) + "_" + fOptimizationVariableNames[variableIndex];\n')
outputModuleOptimizationMacroFile.write('  string axisLabel = ";" + fOptimizationVariableNames[variableIndex] + ";Significance" + IntToString(significanceType);\n')
outputModuleOptimizationMacroFile.write('  TH1F *histogram = new TH1F(histName.c_str(),axisLabel.c_str(),' + str(NumberOfOptimizationPoints) + ', fOptimizationVariableRangeMin[variableIndex], fOptimizationVariableRangeMax[variableIndex]);\n')
outputModuleOptimizationMacroFile.write('  histogram->SetDirectory(0);\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  assert(variableIndex >= 0 && variableIndex < ' + str(len(variableNames)) + ');\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  for (int i=0;i<=histogram->GetXaxis()->GetNbins()+1;i++) {\n')
outputModuleOptimizationMacroFile.write('    double NSig = signalHist->GetBinContent(i);\n')
outputModuleOptimizationMacroFile.write('    double NBkg = bkgHist->GetBinContent(i);\n')
outputModuleOptimizationMacroFile.write('    double Significance = 0.0;\n')
outputModuleOptimizationMacroFile.write('    if (significanceType == 0) {\n')
outputModuleOptimizationMacroFile.write('      if (NSig+NBkg > 0)\n')
outputModuleOptimizationMacroFile.write('        Significance = NSig/TMath::Sqrt(NSig+NBkg);\n')
outputModuleOptimizationMacroFile.write('    } else if (significanceType == 1) {\n')
outputModuleOptimizationMacroFile.write('      if (NBkg > 0)\n')
outputModuleOptimizationMacroFile.write('        Significance = NSig/NBkg;\n')
outputModuleOptimizationMacroFile.write('    } else if (significanceType == 2) {\n')
outputModuleOptimizationMacroFile.write('      if (NSig+NBkg+(0.2*NBkg)*(0.2*NBkg) > 0)\n')
outputModuleOptimizationMacroFile.write('        Significance = NSig/TMath::Sqrt(NSig+NBkg+(0.2*NBkg)*(0.2*NBkg));\n')
outputModuleOptimizationMacroFile.write('    }\n')
outputModuleOptimizationMacroFile.write('    histogram->SetBinContent(i,Significance);\n')
outputModuleOptimizationMacroFile.write('  }\n')
outputModuleOptimizationMacroFile.write('  return histogram;\n')
outputModuleOptimizationMacroFile.write('}\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('//Draws the signal and background histograms together and makes gif file\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('void DrawSigBkgHistogram(TH1F* sig, TH1F* bkg, string histname) {\n')
outputModuleOptimizationMacroFile.write('  string filename = histname + ".gif";\n')
outputModuleOptimizationMacroFile.write('  TCanvas *cv = new TCanvas(histname.c_str(), histname.c_str(), 0,0,800,600);\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  sig->SetLineColor(2);\n')
outputModuleOptimizationMacroFile.write('  bkg->SetLineColor(4);\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  TLegend *leg= new TLegend(0.5,0.7,0.75,0.8);\n')
outputModuleOptimizationMacroFile.write('  leg->AddEntry(sig, "signal", "LP"); \n')
outputModuleOptimizationMacroFile.write('  leg->AddEntry(bkg, "bkg", "LP"); \n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  //set best Y-range\n')
outputModuleOptimizationMacroFile.write('  double tempMaxY1 = sig->GetMaximum();\n')
outputModuleOptimizationMacroFile.write('  double tempMaxY2 = bkg->GetMaximum();\n')
outputModuleOptimizationMacroFile.write('  double MaxY = max(tempMaxY1, tempMaxY2);\n')
outputModuleOptimizationMacroFile.write('  sig->SetMinimum(0.0);\n')
outputModuleOptimizationMacroFile.write('  sig->SetMaximum(1.1*MaxY);\n')
outputModuleOptimizationMacroFile.write('  bkg->SetMinimum(0.0);\n')
outputModuleOptimizationMacroFile.write('  bkg->SetMaximum(1.1*MaxY);\n')
outputModuleOptimizationMacroFile.write('  sig->Draw("");\n')
outputModuleOptimizationMacroFile.write('  bkg->Draw("same");\n')
outputModuleOptimizationMacroFile.write('  leg->Draw();\n')
outputModuleOptimizationMacroFile.write('  cv->SaveAs(filename.c_str());\n')
outputModuleOptimizationMacroFile.write('  delete cv;\n')
outputModuleOptimizationMacroFile.write('}\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('//Draws the significance histograms and saves to gif file\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('void DrawSignificanceHistogram(TH1F* hist, string histname) {\n')
outputModuleOptimizationMacroFile.write('  string filename = histname + ".gif";\n')
outputModuleOptimizationMacroFile.write('  TCanvas *cv = new TCanvas(histname.c_str(), histname.c_str(), 0,0,800,600);\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  hist->Draw("");\n')
outputModuleOptimizationMacroFile.write('  cv->SaveAs(filename.c_str());\n')
outputModuleOptimizationMacroFile.write('  delete cv;\n')
outputModuleOptimizationMacroFile.write('}\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('\n')

outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('//Finds the optimal cuts value from the significance histogram\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('Float_t FindOptimalValue ( int variableIndex, TH1F *significance , float significanceThreshold = 0.2 ) {\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  assert(variableIndex >= 0 && variableIndex < ' + str(len(variableNames)) + ');\n')
outputModuleOptimizationMacroFile.write('  vector <pair<float, float> > localMaxima;\n')
outputModuleOptimizationMacroFile.write('  pair<float,float> lowerBoundary;\n')
outputModuleOptimizationMacroFile.write('  pair<float,float> upperBoundary;\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  for (int i=1;i<=significance->GetXaxis()->GetNbins();i++) {\n')
outputModuleOptimizationMacroFile.write('    double xvalue = 0.0;\n')
outputModuleOptimizationMacroFile.write('    if (i <= significance->GetXaxis()->GetNbins())\n')
outputModuleOptimizationMacroFile.write('      xvalue = significance->GetXaxis()->GetBinUpEdge(i);\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('    float yvalue = significance->GetBinContent(i);\n')
outputModuleOptimizationMacroFile.write('    if (i > 1 && i < significance->GetXaxis()->GetNbins()) {\n')
outputModuleOptimizationMacroFile.write('      float yvalue_left = significance->GetBinContent(i-1);\n')
outputModuleOptimizationMacroFile.write('      float yvalue_right = significance->GetBinContent(i+1);\n')
outputModuleOptimizationMacroFile.write('      if (yvalue > yvalue_left && yvalue > yvalue_right) {\n')
outputModuleOptimizationMacroFile.write('        pair<float,float> maximum(xvalue, yvalue);\n')
outputModuleOptimizationMacroFile.write('        localMaxima.push_back(maximum);\n')
outputModuleOptimizationMacroFile.write('      }\n')
outputModuleOptimizationMacroFile.write('    } else if (i==1) {\n')
outputModuleOptimizationMacroFile.write('      lowerBoundary = pair<float,float>(xvalue,yvalue);\n')
outputModuleOptimizationMacroFile.write('    } else if (i==significance->GetXaxis()->GetNbins()) {\n')
outputModuleOptimizationMacroFile.write('      upperBoundary = pair<float,float>(xvalue,yvalue);\n')
outputModuleOptimizationMacroFile.write('    }\n')
outputModuleOptimizationMacroFile.write('  }\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  //sort the maxima by yvalue\n')
outputModuleOptimizationMacroFile.write('  for(UInt_t i=0; i<localMaxima.size();i++) {\n')
outputModuleOptimizationMacroFile.write('    for(UInt_t j=i+1; j<localMaxima.size();j++) {\n')
outputModuleOptimizationMacroFile.write('      if(localMaxima[i].second < localMaxima[j].second) {\n')
outputModuleOptimizationMacroFile.write('        pair<float,float> temp = localMaxima[i];\n')
outputModuleOptimizationMacroFile.write('        localMaxima[i] = localMaxima[j];\n')
outputModuleOptimizationMacroFile.write('        localMaxima[j] = temp;\n')
outputModuleOptimizationMacroFile.write('      }\n')
outputModuleOptimizationMacroFile.write('    }\n')
outputModuleOptimizationMacroFile.write('  }\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  //check the sort\n')
outputModuleOptimizationMacroFile.write('  cout << "Check Maxima sorting : variable = " << fOptimizationVariableNames[variableIndex] << endl;\n')
outputModuleOptimizationMacroFile.write('  for(UInt_t i=0; i<localMaxima.size();i++)\n')
outputModuleOptimizationMacroFile.write('    cout << localMaxima[i].first << " " <<  localMaxima[i].second << endl;\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  //find the global maximum\n')
outputModuleOptimizationMacroFile.write('  pair<float, float> optimalPoint;\n')
outputModuleOptimizationMacroFile.write('  if (localMaxima.size() > 0)\n')
outputModuleOptimizationMacroFile.write('    optimalPoint = localMaxima[0];\n')
outputModuleOptimizationMacroFile.write('  for(UInt_t i=0; i<localMaxima.size();i++) {\n')
outputModuleOptimizationMacroFile.write('    if (i==0)\n')
outputModuleOptimizationMacroFile.write('      continue;\n')

variableListCutBelow = list()
for i in range (0,len(variableCutDirection)):
    if (variableCutDirection[i] == "CutBelow"):
        variableListCutBelow.append(i)

outputModuleOptimizationMacroFile.write('    if ( ')
for i in range (0,len(variableListCutBelow)):
    if (i < len(variableListCutBelow)-1):
        outputModuleOptimizationMacroFile.write(' variableIndex == ' + str(variableListCutBelow[i]) + ' || ')
    else:
        outputModuleOptimizationMacroFile.write(' variableIndex == ' + str(variableListCutBelow[i]) + ' ')
outputModuleOptimizationMacroFile.write(') { //list of variables for which we cut below\n')

outputModuleOptimizationMacroFile.write('      if (optimalPoint.second - localMaxima[i].second < significanceThreshold && localMaxima[i].first < optimalPoint.first ) {\n')
outputModuleOptimizationMacroFile.write('        optimalPoint = localMaxima[i];\n')
outputModuleOptimizationMacroFile.write('      }\n')
outputModuleOptimizationMacroFile.write('    } else { //list of variables for which we cut above\n')
outputModuleOptimizationMacroFile.write('      if (optimalPoint.second - localMaxima[i].second < significanceThreshold && localMaxima[i].first > optimalPoint.first ) {\n')
outputModuleOptimizationMacroFile.write('        optimalPoint = localMaxima[i];\n')
outputModuleOptimizationMacroFile.write('      }\n')
outputModuleOptimizationMacroFile.write('    }\n')
outputModuleOptimizationMacroFile.write('  }\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  cout << "Maximum out of local maxima : " << optimalPoint.first << " " << optimalPoint.second << endl;\n')
outputModuleOptimizationMacroFile.write('\n')

outputModuleOptimizationMacroFile.write('  if ( ')
for i in range (0,len(variableListCutBelow)):
    if (i < len(variableListCutBelow)-1):
        outputModuleOptimizationMacroFile.write(' variableIndex == ' + str(variableListCutBelow[i]) + ' || ')
    else:
        outputModuleOptimizationMacroFile.write(' variableIndex == ' + str(variableListCutBelow[i]) + ' ')
outputModuleOptimizationMacroFile.write(') { //list of variables for which we cut below\n')

outputModuleOptimizationMacroFile.write('    if (upperBoundary.second - optimalPoint.second > significanceThreshold)\n')
outputModuleOptimizationMacroFile.write('      optimalPoint = upperBoundary;\n')
outputModuleOptimizationMacroFile.write('    if (optimalPoint.second - lowerBoundary.second <= significanceThreshold)\n')
outputModuleOptimizationMacroFile.write('      optimalPoint = lowerBoundary;\n')
outputModuleOptimizationMacroFile.write('  } else {//list of variables for which we cut above\n')
outputModuleOptimizationMacroFile.write('    if (lowerBoundary.second - optimalPoint.second > significanceThreshold)\n')
outputModuleOptimizationMacroFile.write('      optimalPoint = lowerBoundary;\n')
outputModuleOptimizationMacroFile.write('    if (optimalPoint.second - upperBoundary.second <= significanceThreshold)\n')
outputModuleOptimizationMacroFile.write('      optimalPoint = upperBoundary;\n')
outputModuleOptimizationMacroFile.write('  }\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  cout << "lowerBoundary : " << lowerBoundary.first << " " << lowerBoundary.second << endl;\n')
outputModuleOptimizationMacroFile.write('  cout << "upperBoundary : " << upperBoundary.first << " " << upperBoundary.second << endl;\n')
outputModuleOptimizationMacroFile.write('  cout << "Global maxima : " << optimalPoint.first << " " << optimalPoint.second << endl;\n')
outputModuleOptimizationMacroFile.write('  return optimalPoint.first;\n')
outputModuleOptimizationMacroFile.write('}\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('//Perform the Optimization\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('void RunOptimization( TTree* signalTree , TTree* bkgTree , int maxIterations, int finalstate , bool saveIntermediatePlots) {\n')
outputModuleOptimizationMacroFile.write('  string finalstatelabel = "";\n')

fsCount = 1
for fs in FinalStates:
    if (fsCount == 1):
        outputModuleOptimizationMacroFile.write('  if (finalstate == ' + str(fsCount) + ')\n')
        outputModuleOptimizationMacroFile.write('    finalstatelabel = "_' + fs + '";\n')
    else:
        outputModuleOptimizationMacroFile.write('  else if (finalstate == ' + str(fsCount) + ')\n')
        outputModuleOptimizationMacroFile.write('    finalstatelabel = "_' + fs + '";\n')
    fsCount += 1
outputModuleOptimizationMacroFile.write('\n')

outputModuleOptimizationMacroFile.write('  //Run Optimization Scheme\n')
outputModuleOptimizationMacroFile.write('  bool continueOptimization = true;\n')
outputModuleOptimizationMacroFile.write('  int NIterations = 0;\n')
outputModuleOptimizationMacroFile.write('  cout << "*************************************************************************" << endl;\n')
outputModuleOptimizationMacroFile.write('  cout << "Run Optimization for final state " << finalstate << endl;\n')
outputModuleOptimizationMacroFile.write('  cout << "*************************************************************************" << endl;\n')
outputModuleOptimizationMacroFile.write('  while (continueOptimization && NIterations < maxIterations) {\n')
outputModuleOptimizationMacroFile.write('    cout << "Iteration #" << NIterations << endl;\n')
outputModuleOptimizationMacroFile.write('    for (int v = 0; v < ' + str(len(variableNames)) + ';v++) {\n')
outputModuleOptimizationMacroFile.write('      if (fOptimizeThisVariable[v] == false)\n')
outputModuleOptimizationMacroFile.write('        continue;\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('      TH1F* signalDistribution = makeDistributionHistogram(v, signalTree, true, finalstate);\n')
outputModuleOptimizationMacroFile.write('      TH1F* bkgDistribution = makeDistributionHistogram(v, bkgTree, false, finalstate);\n')
outputModuleOptimizationMacroFile.write('      TH1F* signalHist = makeNMinusOneHistogram(v, signalDistribution, true);\n')
outputModuleOptimizationMacroFile.write('      TH1F* bkgHist = makeNMinusOneHistogram(v, bkgDistribution, false);\n')
outputModuleOptimizationMacroFile.write('      TH1F* significanceHist = makeSignificanceHistogram(v, signalHist, bkgHist, 2);\n')
outputModuleOptimizationMacroFile.write('      if (saveIntermediatePlots) {\n')
outputModuleOptimizationMacroFile.write('        DrawSigBkgHistogram(signalDistribution, bkgDistribution, "Distribution_" + fOptimizationVariableNames[v] + finalstatelabel + "_Iteration" + IntToString(NIterations));\n')
outputModuleOptimizationMacroFile.write('        DrawSigBkgHistogram(signalHist, bkgHist, "NEventsPassed_" + fOptimizationVariableNames[v] + finalstatelabel + "_Iteration" + IntToString(NIterations));\n')
outputModuleOptimizationMacroFile.write('        DrawSignificanceHistogram(significanceHist, "Significance2_" + fOptimizationVariableNames[v] + finalstatelabel + "_Iteration" + IntToString(NIterations));\n')
outputModuleOptimizationMacroFile.write('      }\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('      float optimalValue = FindOptimalValue(v,significanceHist);\n')
outputModuleOptimizationMacroFile.write('      if (finalstate > 0) {\n')
outputModuleOptimizationMacroFile.write('        fOptimizationVariableInitialValue[finalstate-1][v] = optimalValue;\n')
outputModuleOptimizationMacroFile.write('      } else {\n')
outputModuleOptimizationMacroFile.write('        for (int f=0; f<' + str(len(FinalStates)) +';f++) \n')
outputModuleOptimizationMacroFile.write('          fOptimizationVariableInitialValue[f][v] = optimalValue;\n')
outputModuleOptimizationMacroFile.write('      }\n')
outputModuleOptimizationMacroFile.write('    }\n')
outputModuleOptimizationMacroFile.write('    NIterations++;\n')
outputModuleOptimizationMacroFile.write('  }\n')
outputModuleOptimizationMacroFile.write('}\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('//Make Final Plots\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('void MakeFinalOptimizationPlots( TTree* signalTree , TTree* bkgTree , int finalstate) {\n')
outputModuleOptimizationMacroFile.write('  string finalstatelabel = "";\n')
fsCount = 1
for fs in FinalStates:
    if (fsCount == 1):
        outputModuleOptimizationMacroFile.write('  if (finalstate == ' + str(fsCount) + ')\n')
        outputModuleOptimizationMacroFile.write('    finalstatelabel = "_' + fs + '";\n')
    else:
        outputModuleOptimizationMacroFile.write('  else if (finalstate == ' + str(fsCount) + ')\n')
        outputModuleOptimizationMacroFile.write('    finalstatelabel = "_' + fs + '";\n')
    fsCount += 1
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  for (int v = 0; v < ' + str(len(variableNames)) + ';v++) {\n')
outputModuleOptimizationMacroFile.write('      //make N-1 distribution histograms\n')
outputModuleOptimizationMacroFile.write('      TH1F* signalDistribution = makeDistributionHistogram(v, signalTree, true, finalstate);\n')
outputModuleOptimizationMacroFile.write('      TH1F* bkgDistribution = makeDistributionHistogram(v, bkgTree, false, finalstate);\n')
outputModuleOptimizationMacroFile.write('      DrawSigBkgHistogram(signalDistribution, bkgDistribution, "Distribution_" + fOptimizationVariableNames[v] + finalstatelabel);\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('      //make N-1 number of events passed histograms\n')
outputModuleOptimizationMacroFile.write('      TH1F* signalHist = makeNMinusOneHistogram(v, signalDistribution, true);\n')
outputModuleOptimizationMacroFile.write('      TH1F* bkgHist = makeNMinusOneHistogram(v, bkgDistribution, false);\n')
outputModuleOptimizationMacroFile.write('      DrawSigBkgHistogram(signalHist, bkgHist, "NEventsPassed_" + fOptimizationVariableNames[v] + finalstatelabel);\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('      //make Significance histograms\n')
outputModuleOptimizationMacroFile.write('      TH1F* significanceHist = makeSignificanceHistogram(v, signalHist, bkgHist, 2);\n')
outputModuleOptimizationMacroFile.write('      DrawSignificanceHistogram(significanceHist, "Significance2_" + fOptimizationVariableNames[v] + finalstatelabel);\n')
outputModuleOptimizationMacroFile.write('    }\n')
outputModuleOptimizationMacroFile.write('}\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('//Main part of the macro\n')
outputModuleOptimizationMacroFile.write('//*************************************************************************************************\n')
outputModuleOptimizationMacroFile.write('void ' + ntupleName + '_Optimization(const string signalInputFilename, const string bkgInputFilename, int OnlyThisFinalState = -1, bool runOptimization = false, int maxIterations = 1, bool saveIntermediatePlots = false) {\n')
outputModuleOptimizationMacroFile.write('  TFile* signalFile = new TFile(signalInputFilename.c_str(),"READ");\n')
outputModuleOptimizationMacroFile.write('  TTree* signalTree = (TTree*)signalFile->Get("' + ntupleName + '");\n')
outputModuleOptimizationMacroFile.write('  TFile* bkgFile = new TFile(bkgInputFilename.c_str(),"READ");\n')
outputModuleOptimizationMacroFile.write('  TTree* bkgTree = (TTree*)bkgFile->Get("' + ntupleName + '");\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  Float_t fSignalOptimizationVariables[' + str(len(variableNames)+2) + '];\n')
outputModuleOptimizationMacroFile.write('  signalTree->SetBranchAddress("optimization_branch",&fSignalOptimizationVariables );\n')
outputModuleOptimizationMacroFile.write('  Float_t fBkgOptimizationVariables[' + str(len(variableNames)+2) + '];\n')
outputModuleOptimizationMacroFile.write('  bkgTree->SetBranchAddress("optimization_branch",&fBkgOptimizationVariables );\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  Int_t SignalEntries = (Int_t)signalTree->GetEntries();\n')
outputModuleOptimizationMacroFile.write('  Int_t BkgEntries = (Int_t)signalTree->GetEntries();\n')
outputModuleOptimizationMacroFile.write('\n')

outputModuleOptimizationMacroFile.write('  if (runOptimization) {\n')
outputModuleOptimizationMacroFile.write('    if (OnlyThisFinalState == -1) {\n')
outputModuleOptimizationMacroFile.write('      //Optimize Each Final State separately\n')
outputModuleOptimizationMacroFile.write('      for (int f=1; f<=' + str(len(FinalStates)) + '; f++) {\n')
outputModuleOptimizationMacroFile.write('        RunOptimization( signalTree, bkgTree, maxIterations, f , saveIntermediatePlots);\n')
outputModuleOptimizationMacroFile.write('      }\n')
outputModuleOptimizationMacroFile.write('    } else { //OnlyThisFinalState = 0 means we optimize all final states together\n')
outputModuleOptimizationMacroFile.write('      RunOptimization( signalTree, bkgTree, maxIterations, OnlyThisFinalState , saveIntermediatePlots);\n')
outputModuleOptimizationMacroFile.write('    }\n')
outputModuleOptimizationMacroFile.write('  }\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('\n')

outputModuleOptimizationMacroFile.write('  cout << "*************************************************************************" << endl;\n')
outputModuleOptimizationMacroFile.write('  cout << "Final Optimal Cuts" << endl;\n')
outputModuleOptimizationMacroFile.write('  cout << "*************************************************************************" << endl;\n')
outputModuleOptimizationMacroFile.write('  for (int f=0; f<' + str(len(FinalStates)) + ';f++) {\n')
outputModuleOptimizationMacroFile.write('    cout << "Final State" << f+1 << endl;\n')
outputModuleOptimizationMacroFile.write('    for (int v = 0; v < ' + str(len(variableNames)) + ';v++) {\n')
outputModuleOptimizationMacroFile.write('      cout << fOptimizationVariableNames[v] << " : " << fOptimizationVariableInitialValue[f][v] << endl;\n')
outputModuleOptimizationMacroFile.write('    }\n')
outputModuleOptimizationMacroFile.write('    cout << "----------------------------------------------------------------------" << endl;\n')
outputModuleOptimizationMacroFile.write('  }\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  cout << "*************************************************************************" << endl;\n')
outputModuleOptimizationMacroFile.write('  cout << "Making Final Plots" << endl;\n')
outputModuleOptimizationMacroFile.write('  cout << "*************************************************************************" << endl;\n')
outputModuleOptimizationMacroFile.write('  if (OnlyThisFinalState == -1) {\n')
outputModuleOptimizationMacroFile.write('    for (int f=0; f<=' + str(len(FinalStates)) + ';f++) {\n')
outputModuleOptimizationMacroFile.write('      MakeFinalOptimizationPlots(signalTree , bkgTree , f);\n')
outputModuleOptimizationMacroFile.write('    }\n')
outputModuleOptimizationMacroFile.write('  } else {\n')
outputModuleOptimizationMacroFile.write('    MakeFinalOptimizationPlots(signalTree , bkgTree , OnlyThisFinalState);\n')
outputModuleOptimizationMacroFile.write('  }\n')
outputModuleOptimizationMacroFile.write('\n')
outputModuleOptimizationMacroFile.write('  signalFile->Close();\n')
outputModuleOptimizationMacroFile.write('  bkgFile->Close();\n')
outputModuleOptimizationMacroFile.write('}\n')
