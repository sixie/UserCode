#!/usr/bin/env python

###########Example to run
#python RunOptimization.py --ntupleName=WWEventSelectionOptimizationTree --crossSectionFile=/home/sixie/Tools/data/DataSampleCrossSections.txt --runDirectory=/home/sixie/temp/ --signalSample=s8-ww2l-id9 --integratedLuminosity=1000
###########Example to run

import sys, getopt, os, fnmatch, commands
import string

sys.path.append('/home/sixie/Tools/python/')
import RootFileUtilities

###declare 
setupcmsswcommand = 'cd /home/sixie/CMSSW_2_1_11/src/; eval `scramv1 runtime -sh`; cd -;'

def usage():
    print "possible options are: --help, --ntupleName=<myNtupleName> , --crossSectionFile=<myXSecFile>, --runDirectory=<myRunDirectory>, --signalSample=<signalSampleName>, --integratedLuminosity=<myIntegratedLuminosityInPb^-1>"


###declare global variables
runDirectory = ''
ntupleName = ''
crossSectionFile = ''
signalSample = ''
intLuminosity = '1.0'
datasetNameList = list()
crossSectionList = list()
doMerge = ''
doOptimization = ''

if len(sys.argv[1:]) < 1:
    print "No input file specified."
    usage()
    sys.exit()

try:
    opts, args = getopt.getopt(sys.argv[1:], "hn:x:r:s:l:mo", ["help", "ntupleName=", "crossSectionFile=", "runDirectory=", "signalSample=", "integratedLuminosity="])
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit()
        elif o in ("-n", "--ntupleName"):
            ntupleName = a
        elif o in ("-x", "--crossSectionFile"):
            crossSectionFile = a
        elif o in ("-x", "--runDirectory"):
            runDirectory = a
        elif o in ("-s", "--signalSample"):
            signalSample = a
        elif o in ("-s", "--integratedLuminosity"):
            intLuminosity = a
        elif o in ("-m"):
            doMerge = 'yes'
        elif o in ("-o"):
            doOptimization = 'yes'     
        else:
            usage()
            sys.exit()
except getopt.GetoptError:
    usage()
    sys.exit(2)

if (ntupleName == ''):
    print "Error: No Ntuple name specified."
    sys.exit()

if (crossSectionFile == ''):
    print "Error: No cross section file specified."
    sys.exit()

if (runDirectory == ''):
    print "Error: No run directory specified."
    sys.exit()

if (signalSample == ''):
    print "Error: No signal sample specified."
    sys.exit()

try:
    inputFile = open(crossSectionFile,"r")
except IOError:
    print "Error: The specified crossSectionFile " + crossSectionFile + " could not be opened."
    sys.exit()


########################################################################################
#Read in List of Datasets and CrossSections
########################################################################################
lineNumber = 1
templine = inputFile.readline()
while len(templine.split()) > 0:
    if (templine[0] == '#'):
        templine = inputFile.readline()
        lineNumber += 1
        continue;
    if len(templine.split()) != 2:
        print "error: incorrect format for cross section file. Check line %s" % lineNumber
        sys.exit()
    else:
        tempInputList = templine.split()        
        datasetNameList.append(tempInputList[0])
        crossSectionList.append(tempInputList[1])
        lineNumber += 1
        #print numberOfVariables
    templine = inputFile.readline()
else:
    #print "done reading input"
    print ''
inputFile.close()

#Check the list of variables
#count = 0
#for l in datasetNameList:
#    print l+" "+crossSectionList[count]
#    count += 1


########################################################################################
##Merge Filesets
########################################################################################
print "*****************************************************************"
print "Merge Filesets"
print "*****************************************************************"

ntuplePath = '/home/sixie/ntuples/' + ntupleName + '/filler/006/'
if (doMerge == 'yes'):
    RootFileUtilities.MergeFilesets(datasetNameList, ntuplePath, ntuplePath, ntupleName)
print ''

########################################################################################
##Properly Reweight
########################################################################################
print "*****************************************************************"
print "Do Reweighting according to crosssections"
print "*****************************************************************"

if (RootFileUtilities.filesExist(runDirectory,ntupleName+'_reweightTree.C') == False):
    print 'The Reweighting Macro ' + ntupleName+'_reweightTree.C' + ' is missing from the run Directory ' + runDirectory
    sys.exit()

n = 0
for dataset in datasetNameList:    
    ntupleFilename = ntupleName + '_' + dataset + '.root'
    if (RootFileUtilities.filesExist(ntuplePath,ntupleFilename) == True):
        print "Reweighting dataset " + dataset + " with cross section " + crossSectionList[n] + " and Integrated Luminosity " + intLuminosity
        #reweightCommand = setupcmsswcommand + 'cd ' + runDirectory + ';' + 'root -l -q ' + ntupleName + '_reweightTree.C+\(\\"' + ntuplePath+ntupleFilename + '\\",' + crossSectionList[n] + ',' + intLuminosity + '\)'
        reweightCommand = setupcmsswcommand + 'cd ' + runDirectory + ';' + 'root -l -q $CMSSW_BASE/src/rootlogon.C ' + ntupleName + '_reweightTree.C+\(\\"' + ntuplePath+ntupleFilename + '\\",\\"' + dataset + '\\",' + intLuminosity + '\)'
        print reweightCommand
        os.system(reweightCommand)
    else:
        print "The ntuple file for dataset " + dataset + " was not found. filename: " + ntuplePath + ntupleFilename

    print ''
    n += 1

########################################################################################
##Merge backgrounds
########################################################################################
print "Merge Background Files"
bkgFilesToMerge = ''
for dataset in datasetNameList:
    ntupleFilename = ntupleName + '_' + dataset + '.root.reweighted.root'
    if (dataset == signalSample):
        if (RootFileUtilities.filesExist(ntuplePath,ntupleFilename) == True):
            os.system('cp ' + ntuplePath+ntupleFilename + ' ' + ntuplePath+ntupleName+'_sig.root')
        else:
            print "Error: specified signal sample file (' + ntuplePath+ntupleFilename + ') doesn't exist."
            sys.exit()
    else:
        if (RootFileUtilities.filesExist(ntuplePath,ntupleFilename) == True):
            bkgFilesToMerge += ntuplePath + ntupleFilename + ' '
        else:
            print 'Background file ' + ntuplePath + ntupleFilename + ' does not exist. It will not be added to the background file.'

mergeBkgCommand = setupcmsswcommand + 'hadd ' + ntuplePath + ntupleName + '_bkg.root ' + bkgFilesToMerge
print mergeBkgCommand
if (RootFileUtilities.filesExist(ntuplePath,ntupleName + '_bkg.root') == True):
    print "Background file already exists. It will be erased."
    os.system('rm ' + ntuplePath + ntupleName + '_bkg.root')
os.system(mergeBkgCommand )
print ''


########################################################################################
##Run Optimization Code
########################################################################################
print "Run Optimization Code"
if (RootFileUtilities.filesExist(runDirectory,ntupleName+'_Optimization.C') == False):
    print 'The Optimization Macro ' + ntupleName+'_Optimization.C' + ' is missing from the run Directory ' + runDirectory
    sys.exit()
else:
    optimizationCommand = setupcmsswcommand + 'cd ' + runDirectory + ';' + 'root -l -q -b ' + ntupleName + '_Optimization.C+\(\\"' + ntuplePath + ntupleName + '_sig.root\\",\\"' + ntuplePath + ntupleName + '_bkg.root\\",-1\)'
    print optimizationCommand
    if (doOptimization == 'yes'):
        os.system(optimizationCommand)

print ''

