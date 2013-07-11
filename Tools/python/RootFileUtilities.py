#!/usr/bin/env python
import sys, getopt, os, fnmatch, commands
import string

###declare CMSSW setup command...
setupcmsswcommand = 'cd /home/sixie/CMSSW_2_1_11/src/; eval `scramv1 runtime -sh`; cd -;'


########################################################################################
##See if files at given path and regular expression exist or not
########################################################################################
def filesExist( path, filenameExpression ):
    exists = False
    for fileName in os.listdir ( path ):
        if fnmatch.fnmatch ( fileName, filenameExpression ):
            exists = True

    return exists

########################################################################################
##Merge Filesets
########################################################################################
def MergeFilesets( datasetNameList, inputPath, outputPath, filenameHeader):
    n = 0
    for dataset in datasetNameList:
        print "Merging files for dataset " + dataset
        
        outputMergedFilename = filenameHeader + '_' + dataset + '.root'
        inputFilenameRegExpression = filenameHeader + '_' + dataset + '_????.root'        
        command = setupcmsswcommand + 'hadd ' + outputPath + outputMergedFilename + ' ' + inputPath + inputFilenameRegExpression
        
        if (filesExist(inputPath,inputFilenameRegExpression) == True):
        #print command
            if (os.path.exists(outputPath+outputMergedFilename)):
                print "Merged file already exists. It will be deleted."
                os.system('rm ' + outputPath+outputMergedFilename)            
            os.system(command)
            print ''
        else:
            print "Warning : No files for dataset " + dataset + " at the location : " + inputPath + inputFilenameRegExpression
        print ''
        n += 1


