cmsDriver.py Configuration/GenProduction/python/WW_2l_cfi.py \
	     -s GEN,SIM,DIGI,L1,DIGI2RAW,HLT \
	     --eventcontent RAWSIM \
	     --datatier GEN-SIM-RAW \
	     --conditions FrontierConditions_GlobalTag,IDEAL_V9::All \
	     -n 200 \
             --no_exec
	     
cmsDriver.py Configuration/GenProduction/python/WZ_3l_cfi.py \
	     -s GEN,SIM,DIGI,L1,DIGI2RAW,HLT \
	     --eventcontent RAWSIM \
	     --datatier GEN-SIM-RAW \
	     --conditions FrontierConditions_GlobalTag,IDEAL_V9::All \
	     -n 200 \
             --no_exec
	     
cmsDriver.py Configuration/GenProduction/python/ZZ_4l_cfi.py \
	     -s GEN,SIM,DIGI,L1,DIGI2RAW,HLT \
	     --eventcontent RAWSIM \
	     --datatier GEN-SIM-RAW \
	     --conditions FrontierConditions_GlobalTag,IDEAL_V9::All \
	     -n 200 \
             --no_exec
	     

cmsDriver.py Configuration/GenProduction/python/H200_ZZ_qqll_cfi.py \
	     -s GEN,SIM,DIGI,L1,DIGI2RAW,HLT \
	     --eventcontent RAWSIM \
	     --datatier GEN-SIM-RAW \
	     --conditions FrontierConditions_GlobalTag,IDEAL_V9::All \
	     -n 200 \
             --no_exec
	     

cmsDriver.py Configuration/GenProduction/python/Reconstruction_2_1_8_IDEAL_V9.py \
	     -s RAW2DIGI,RECO \
	     --filein file:GEN-SIM-RAW.root \
	     --eventcontent RECO \
	     --datatier RECO \
	     --conditions FrontierConditions_GlobalTag,IDEAL_V9::All \
             -n -1 \
             --no_exec
