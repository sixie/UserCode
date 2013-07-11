
#############################
# Workspaces
#############################
#create workspace for data
./scripts/generateWorkspace.sh -m lambdaWZ -t  HCPFullComb -M 125.8 -d 1 
./scripts/generateWorkspace.sh -m lambdadu -t  HCPFullComb -M 125.8 -d 1 
./scripts/generateWorkspace.sh -m lambdalq -t  HCPFullComb -M 125.8 -d 1 


#create workspace & generate asimov toy
./scripts/generateWorkspace.sh -m cVcF -t  HCPhgg -M 125.8 -s 1 
./scripts/generateWorkspace.sh -m cVcF -t  HCPhww2l -M 125.8 -s 1
./scripts/generateWorkspace.sh -m cVcF -t  HCPhzz4l -M 125.8 -s 1
./scripts/generateWorkspace.sh -m cVcF -t  HCPvhbb -M 125.8 -s 1 
./scripts/generateWorkspace.sh -m cVcF -t  HCPhires -M 125.8 -s 1
./scripts/generateWorkspace.sh -m cVcF -t  HCPFullComb -M 125.8 -s 1 

./scripts/generateWorkspace.sh -m lambdaWZ -t  HCPhgg -M 125.8 -s 1
./scripts/generateWorkspace.sh -m lambdaWZ -t  HCPhww2l -M 125.8 -s 1
./scripts/generateWorkspace.sh -m lambdaWZ -t  HCPhzz4l -M 125.8 -s 1
./scripts/generateWorkspace.sh -m lambdaWZ -t  HCPvhbb -M 125.8 -s 1 
./scripts/generateWorkspace.sh -m lambdaWZ -t  HCPhires -M 125.8 -s 1
./scripts/generateWorkspace.sh -m lambdaWZ -t  HCPFullComb -M 125.8 -s 1 

./scripts/generateWorkspace.sh -m lambdadu -t  HCPhgg -M 125.8 -s 1
./scripts/generateWorkspace.sh -m lambdadu -t  HCPhww2l -M 125.8 -s 1
./scripts/generateWorkspace.sh -m lambdadu -t  HCPhzz4l -M 125.8 -s 1
./scripts/generateWorkspace.sh -m lambdadu -t  HCPvhbb -M 125.8 -s 1 
./scripts/generateWorkspace.sh -m lambdadu -t  HCPhires -M 125.8 -s 1
./scripts/generateWorkspace.sh -m lambdadu -t  HCPFullComb -M 125.8 -s 1 

./scripts/generateWorkspace.sh -m lambdalq -t  HCPhgg -M 125.8 -s 1
./scripts/generateWorkspace.sh -m lambdalq -t  HCPhww2l -M 125.8 -s 1
./scripts/generateWorkspace.sh -m lambdalq -t  HCPhzz4l -M 125.8 -s 1
./scripts/generateWorkspace.sh -m lambdalq -t  HCPvhbb -M 125.8 -s 1 
./scripts/generateWorkspace.sh -m lambdalq -t  HCPhires -M 125.8 -s 1
./scripts/generateWorkspace.sh -m lambdalq -t  HCPFullComb -M 125.8 -s 1 



#create workspaces for real toys
./scripts/generateWorkspace.sh -m twoHypothesisHiggs -t  hcpFullComb -M 125 -s 1 -P muFloating -p "x=0.0" -S 1000
#do it on the batch
./batch/submitGenerateToys.sh -m twoHypothesisHiggs -t hzz4lpseudoscalar -S 1000 -n 10


#############################
# Run fits
#############################


#asimov toy 1D
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcphgg_125.8_1_Asimov.root -f grid1D -P CV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcphgg_125.8_1_Asimov.root -f grid1D -P CF
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcphww2l_125.8_1_Asimov.root -f grid1D -P CV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcphww2l_125.8_1_Asimov.root -f grid1D -P CF
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcphzz4l_125.8_1_Asimov.root -f grid1D -P CV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcphzz4l_125.8_1_Asimov.root -f grid1D -P CF
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcpvhbb_125.8_1_Asimov.root -f grid1D -P CV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcpvhbb_125.8_1_Asimov.root -f grid1D -P CF
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcphires_125.8_1_Asimov.root -f grid1D -P CV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcphires_125.8_1_Asimov.root -f grid1D -P CF
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcpFullComb_125.8_1_Asimov.root -f grid1D -P CV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcpFullComb_125.8_1_Asimov.root -f grid1D -P CF

./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphgg_125.8_1_Asimov.root -f grid1D -P kZ
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphgg_125.8_1_Asimov.root -f grid1D -P lambdaWZ
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphgg_125.8_1_Asimov.root -f grid1D -P kf
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphww2l_125.8_1_Asimov.root -f grid1D -P kZ
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphww2l_125.8_1_Asimov.root -f grid1D -P lambdaWZ
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphww2l_125.8_1_Asimov.root -f grid1D -P kf
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphzz4l_125.8_1_Asimov.root -f grid1D -P kZ
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphzz4l_125.8_1_Asimov.root -f grid1D -P lambdaWZ
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphzz4l_125.8_1_Asimov.root -f grid1D -P kf
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcpvhbb_125.8_1_Asimov.root -f grid1D -P kZ
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcpvhbb_125.8_1_Asimov.root -f grid1D -P lambdaWZ
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcpvhbb_125.8_1_Asimov.root -f grid1D -P kf
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hires_125.8_1_Asimov.root -f grid1D -P kZ
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hires_125.8_1_Asimov.root -f grid1D -P lambdaWZ
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hires_125.8_1_Asimov.root -f grid1D -P kf
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_FullComb_125.8_1_Asimov.root -f grid1D -P kZ
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_FullComb_125.8_1_Asimov.root -f grid1D -P lambdaWZ
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_FullComb_125.8_1_Asimov.root -f grid1D -P kf


./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphgg_125.8_1_Asimov.root -f grid1D -P kV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphgg_125.8_1_Asimov.root -f grid1D -P lambdadu
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphgg_125.8_1_Asimov.root -f grid1D -P ku
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphww2l_125.8_1_Asimov.root -f grid1D -P kV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphww2l_125.8_1_Asimov.root -f grid1D -P lambdadu
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphww2l_125.8_1_Asimov.root -f grid1D -P ku
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphzz4l_125.8_1_Asimov.root -f grid1D -P kV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphzz4l_125.8_1_Asimov.root -f grid1D -P lambdadu
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphzz4l_125.8_1_Asimov.root -f grid1D -P ku
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcpvhbb_125.8_1_Asimov.root -f grid1D -P kV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcpvhbb_125.8_1_Asimov.root -f grid1D -P lambdadu
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcpvhbb_125.8_1_Asimov.root -f grid1D -P ku
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphires_125.8_1_Asimov.root -f grid1D -P kV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphires_125.8_1_Asimov.root -f grid1D -P lambdadu
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphires_125.8_1_Asimov.root -f grid1D -P ku
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcpFullComb_125.8_1_Asimov.root -f grid1D -P kV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcpFullComb_125.8_1_Asimov.root -f grid1D -P lambdadu
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcpFullComb_125.8_1_Asimov.root -f grid1D -P ku

./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphgg_125.8_1_Asimov.root -f grid1D -P kV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphgg_125.8_1_Asimov.root -f grid1D -P lambdalq
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphgg_125.8_1_Asimov.root -f grid1D -P kq
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphww2l_125.8_1_Asimov.root -f grid1D -P kV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphww2l_125.8_1_Asimov.root -f grid1D -P lambdalq
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphww2l_125.8_1_Asimov.root -f grid1D -P kq
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphzz4l_125.8_1_Asimov.root -f grid1D -P kV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphzz4l_125.8_1_Asimov.root -f grid1D -P lambdalq
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphzz4l_125.8_1_Asimov.root -f grid1D -P kq
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcpvhbb_125.8_1_Asimov.root -f grid1D -P kV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcpvhbb_125.8_1_Asimov.root -f grid1D -P lambdalq
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcpvhbb_125.8_1_Asimov.root -f grid1D -P kq
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphires_125.8_1_Asimov.root -f grid1D -P kV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphires_125.8_1_Asimov.root -f grid1D -P lambdalq
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphires_125.8_1_Asimov.root -f grid1D -P kq
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcpFullComb_125.8_1_Asimov.root -f grid1D -P kV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcpFullComb_125.8_1_Asimov.root -f grid1D -P lambdalq
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcpFullComb_125.8_1_Asimov.root -f grid1D -P kq



#asimov toy with systematics 1D
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_HCPFullComb_125.8_1_Asimov.root -f grid1D_multiJob  -j 10 -P kZ
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_HCPFullComb_125.8_1_Asimov.root -f grid1D_multiJob  -j 10 -P lambdaWZ
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_HCPFullComb_125.8_1_Asimov.root -f grid1D_multiJob  -j 10 -P kf
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_HCPFullComb_125.8_1_Asimov.root -f grid1D_multiJob  -j 10 -P kV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_HCPFullComb_125.8_1_Asimov.root -f grid1D_multiJob  -j 10 -P lambdadu
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_HCPFullComb_125.8_1_Asimov.root -f grid1D_multiJob  -j 10 -P ku
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_HCPFullComb_125.8_1_Asimov.root -f grid1D_multiJob  -j 10 -P kV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_HCPFullComb_125.8_1_Asimov.root -f grid1D_multiJob  -j 10 -P lambdalq
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_HCPFullComb_125.8_1_Asimov.root -f grid1D_multiJob  -j 10 -P kq

./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_HCPFullComb.root -f grid1D_multiJob  -j 10 -P kZ
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_HCPFullComb.root -f grid1D_multiJob  -j 10 -P lambdaWZ
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_HCPFullComb.root -f grid1D_multiJob  -j 10 -P kf
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_HCPFullComb.root -f grid1D_multiJob  -j 10 -P kV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_HCPFullComb.root -f grid1D_multiJob  -j 10 -P lambdadu
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_HCPFullComb.root -f grid1D_multiJob  -j 10 -P ku
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_HCPFullComb.root -f grid1D_multiJob  -j 10 -P kV
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_HCPFullComb.root -f grid1D_multiJob  -j 10 -P lambdalq
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_HCPFullComb.root -f grid1D_multiJob  -j 10 -P kq


#asimov toy 2D
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcphgg_125.8_1_Asimov.root -f grid2D 
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcphww2l_125.8_1_Asimov.root -f grid2D 
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcphzz4l_125.8_1_Asimov.root -f grid2D 
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcpvhbb_125.8_1_Asimov.root -f grid2D 
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcphires_125.8_1_Asimov.root -f grid2D 
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcpFullComb_125.8_1_Asimov.root -f grid2D 


./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphgg_125.8_1_Asimov.root -f grid2D_forHighDim -P "kZ lambdaWZ"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphww2l_125.8_1_Asimov.root -f grid2D_forHighDim -P "kZ lambdaWZ"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphzz4l_125.8_1_Asimov.root -f grid2D_forHighDim -P "kZ lambdaWZ"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcpvhbb_125.8_1_Asimov.root -f grid2D_forHighDim -P "kZ lambdaWZ"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphires_125.8_1_Asimov.root -f grid2D_forHighDim -P "kZ lambdaWZ"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcpFullComb_125.8_1_Asimov.root -f grid2D_forHighDim -P "kZ lambdaWZ"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphgg_125.8_1_Asimov.root -f grid2D_forHighDim -P "kZ kf"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphww2l_125.8_1_Asimov.root -f grid2D_forHighDim -P "kZ kf"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphzz4l_125.8_1_Asimov.root -f grid2D_forHighDim -P "kZ kf"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcpvhbb_125.8_1_Asimov.root -f grid2D_forHighDim -P "kZ kf"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphires_125.8_1_Asimov.root -f grid2D_forHighDim -P "kZ kf"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcpFullComb_125.8_1_Asimov.root -f grid2D_forHighDim -P "kZ kf"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphgg_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdaWZ kf"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphww2l_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdaWZ kf"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphzz4l_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdaWZ kf"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcpvhbb_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdaWZ kf"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcphires_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdaWZ kf"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_hcpFullComb_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdaWZ kf"



./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphgg_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV lambdadu"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphgg_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV ku"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphgg_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdadu ku"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphww2l_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV lambdadu"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphww2l_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV ku"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphww2l_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdadu ku"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphzz4l_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV lambdadu"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphzz4l_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV ku"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphzz4l_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdadu ku"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcpvhbb_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV lambdadu"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcpvhbb_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV ku"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcpvhbb_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdadu ku"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphires_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV lambdadu"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphires_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV ku"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcphires_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdadu ku"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcpFullComb_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV lambdadu"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcpFullComb_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV ku"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_hcpFullComb_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdadu ku"


./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphgg_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV lambdalq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphgg_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV kq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphgg_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdalq kq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphww2l_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV lambdalq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphww2l_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV kq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphww2l_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdalq kq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphzz4l_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV lambdalq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphzz4l_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV kq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphzz4l_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdalq kq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcpvhbb_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV lambdalq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcpvhbb_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV kq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcpvhbb_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdalq kq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphires_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV lambdalq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphires_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV kq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcphires_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdalq kq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcpFullComb_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV lambdalq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcpFullComb_125.8_1_Asimov.root -f grid2D_forHighDim -P "kV kq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_hcpFullComb_125.8_1_Asimov.root -f grid2D_forHighDim -P "lambdalq kq"



#asimov toy 2D with systematics
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcphgg_125.8_1_Asimov.root -f grid2D_multiJob  -j 100 
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcphww2l_125.8_1_Asimov.root -f grid2D_multiJob -j 100 
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcphzz4l_125.8_1_Asimov.root -f grid2D_multiJob -j 100 
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcpvhbb_125.8_1_Asimov.root -f grid2D_multiJob -j 100 
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcphires_125.8_1_Asimov.root -f grid2D_multiJob -j 100 
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/cVcF/cVcF_hcpFullComb_125.8_1_Asimov.root -f grid2D_multiJob -j 100 


./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_HCPFullComb_125.8_1_Asimov.root -f grid2D_forHighDim_multiJob -j 100 -P "kZ lambdaWZ"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_HCPFullComb_125.8_1_Asimov.root -f grid2D_forHighDim_multiJob -j 100 -P "kZ kf"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_HCPFullComb_125.8_1_Asimov.root -f grid2D_forHighDim_multiJob -j 100 -P "lambdaWZ kf"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_HCPFullComb_125.8_1_Asimov.root -f grid2D_forHighDim_multiJob -j 100 -P "kV lambdadu"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_HCPFullComb_125.8_1_Asimov.root -f grid2D_forHighDim_multiJob -j 100 -P "kV ku"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_HCPFullComb_125.8_1_Asimov.root -f grid2D_forHighDim_multiJob -j 100 -P "lambdadu ku"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_HCPFullComb_125.8_1_Asimov.root -f grid2D_forHighDim_multiJob -j 100 -P "kV lambdalq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_HCPFullComb_125.8_1_Asimov.root -f grid2D_forHighDim_multiJob -j 100 -P "kV kq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_HCPFullComb_125.8_1_Asimov.root -f grid2D_forHighDim_multiJob -j 100 -P "lambdalq kq"

./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_HCPFullComb.root -f grid2D_forHighDim_multiJob -j 100 -P "kZ lambdaWZ"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_HCPFullComb.root -f grid2D_forHighDim_multiJob -j 100 -P "kZ kf"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdaWZ/lambdaWZ_HCPFullComb.root -f grid2D_forHighDim_multiJob -j 100 -P "lambdaWZ kf"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_HCPFullComb.root -f grid2D_forHighDim_multiJob -j 100 -P "kV lambdadu"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_HCPFullComb.root -f grid2D_forHighDim_multiJob -j 100 -P "kV ku"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdadu/lambdadu_HCPFullComb.root -f grid2D_forHighDim_multiJob -j 100 -P "lambdadu ku"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_HCPFullComb.root -f grid2D_forHighDim_multiJob -j 100 -P "kV lambdalq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_HCPFullComb.root -f grid2D_forHighDim_multiJob -j 100 -P "kV kq"
./batch/submitGrid.sh -w /afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/workspaces/lambdalq/lambdalq_HCPFullComb.root -f grid2D_forHighDim_multiJob -j 100 -P "lambdalq kq"



#check jobs 
bjobs -w | grep "kZ lambdaWZ" | grep "Nov  8" | wc 
bjobs -w | grep "kZ kf" | grep "Nov  8" | wc 
bjobs -w | grep "lambdaWZ kf" | grep "Nov  8" | wc 
bjobs -w | grep "kV lambdadu" | grep "Nov  8" | wc 
bjobs -w | grep "kV ku" | grep "Nov  8" | wc 
bjobs -w | grep "lambdadu ku" | grep "Nov  8" | wc 
bjobs -w | grep "kV lambdalq" | grep "Nov  8" | wc 
bjobs -w | grep "kV kq" | grep "Nov  8" | wc 
bjobs -w | grep "lambdalq kq" | grep "Nov  8" | wc 


#count them
ls higgsCombinecVcF_hcphgg_125.8_1_Asimov_* | wc 
ls higgsCombinecVcF_hcphww2l_125.8_1_Asimov_* | wc 
ls higgsCombinecVcF_hcphzz4l_125.8_1_Asimov_* | wc 
ls higgsCombinecVcF_hcpvhbb_125.8_1_Asimov_* | wc 
ls higgsCombinecVcF_hcphires_125.8_1_Asimov_* | wc 
ls higgsCombinecVcF_hcpFullComb_125.8_1_Asimov_* | wc 


#############################
#hadd them
#############################
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphgg_125.8_1_Asimov.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphgg_125.8_1_Asimov_*
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphww2l_125.8_1_Asimov.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphww2l_125.8_1_Asimov_*
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphzz4l_125.8_1_Asimov.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphzz4l_125.8_1_Asimov_*
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcpvhbb_125.8_1_Asimov.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcpvhbb_125.8_1_Asimov_*
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphires_125.8_1_Asimov.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphires_125.8_1_Asimov_*
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcpFullComb_125.8_1_Asimov.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcpFullComb_125.8_1_Asimov_*


## LambdaWZ ###################
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_lambdaWZ.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_lambdaWZ_*
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_kf.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_kf_*
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_kZ.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_kZ_*

hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_kZ_lambdaWZ.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_????_kZ_lambdaWZ.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_???_kZ_lambdaWZ.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_0_kZ_lambdaWZ.MultiDimFit.mH125.8.root
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_kZ_kf.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_????_kZ_kf.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_???_kZ_kf.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_0_kZ_kf.MultiDimFit.mH125.8.root
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_lambdaWZ_kf.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_????_lambdaWZ_kf.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_???_lambdaWZ_kf.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_0_lambdaWZ_kf.MultiDimFit.mH125.8.root


hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_lambdaWZ.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_lambdaWZ_*
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_kf.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_kf_*
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_kZ.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_kZ_*

hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_kZ_lambdaWZ.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_????_kZ_lambdaWZ.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_???_kZ_lambdaWZ.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_0_kZ_lambdaWZ.MultiDimFit.mH125.8.root
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_kZ_kf.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_????_kZ_kf.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_???_kZ_kf.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_0_kZ_kf.MultiDimFit.mH125.8.root
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_lambdaWZ_kf.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_????_lambdaWZ_kf.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_???_lambdaWZ_kf.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_0_lambdaWZ_kf.MultiDimFit.mH125.8.root
6800(running) 7300(running)



## lambdadu ###################
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_lambdadu.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_lambdadu_*
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_kV.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_kV_*
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_ku.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_ku_*

hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_lambdadu_ku.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_????_lambdadu_ku.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_???_lambdadu_ku.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_0_lambdadu_ku.MultiDimFit.mH125.8.root
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_kV_lambdadu.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_????_kV_lambdadu.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_???_kV_lambdadu.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_0_kV_lambdadu.MultiDimFit.mH125.8.root
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_kV_ku.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_????_kV_ku.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_???_kV_ku.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_0_kV_ku.MultiDimFit.mH125.8.root



hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_lambdadu.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_lambdadu_*
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_kV.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_kV_*
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_ku.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_ku_*

hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_lambdadu_ku.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_????_lambdadu_ku.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_???_lambdadu_ku.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_0_lambdadu_ku.MultiDimFit.mH125.8.root
4200(running) 4700(running) 5500(running) 2600(running)
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_kV_lambdadu.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_????_kV_lambdadu.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_???_kV_lambdadu.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_0_kV_lambdadu.MultiDimFit.mH125.8.root
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_kV_ku.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_????_kV_ku.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_???_kV_ku.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_0_kV_ku.MultiDimFit.mH125.8.root
7300(running) 8700(running) 9100(running)




## lambdalq ###################
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_lambdalq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_lambdalq_*
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_kV.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_kV_*
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_kq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_kq_*

hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_lambdalq_kq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_????_lambdalq_kq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_???_lambdalq_kq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_0_lambdalq_kq.MultiDimFit.mH125.8.root
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_kV_lambdalq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_????_kV_lambdalq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_???_kV_lambdalq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_0_kV_lambdalq.MultiDimFit.mH125.8.root
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_kV_kq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_????_kV_kq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_???_kV_kq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_0_kV_kq.MultiDimFit.mH125.8.root


hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_lambdalq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_lambdalq_* #missing
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_kV.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_kV_*
hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_kq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_kq_*

hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_lambdalq_kq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_????_lambdalq_kq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_???_lambdalq_kq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_0_lambdalq_kq.MultiDimFit.mH125.8.root
4500(running) 8200(running) 0(running)

hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_kV_lambdalq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_????_kV_lambdalq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_???_kV_lambdalq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_0_kV_lambdalq.MultiDimFit.mH125.8.root
400(running) 9800(running) 

hadd -f /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_kV_kq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_????_kV_kq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_???_kV_kq.MultiDimFit.mH125.8.root /afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_0_kV_kq.MultiDimFit.mH125.8.root



#############################
#make plot
#############################

#asimov toy
root -l -b -q plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphgg_125.8_1_Asimov_CV.MultiDimFit.mH125.8.root","cVcF_hcphgg_CV","CV",0,2,100)'
root -l -b -q plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphgg_125.8_1_Asimov_CF.MultiDimFit.mH125.8.root","cVcF_hcphgg_CF","CF",-2,2,100)'
root -l -b -q plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphww2l_125.8_1_Asimov_CV.MultiDimFit.mH125.8.root","cVcF_hcphww2l_CV","CV",0,2,100)'
root -l -b -q plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphww2l_125.8_1_Asimov_CF.MultiDimFit.mH125.8.root","cVcF_hcphww2l_CF","CF",-2,2,100)'
root -l -b -q plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphzz4l_125.8_1_Asimov_CV.MultiDimFit.mH125.8.root","cVcF_hcphzz4l_CV","CV",0,2,100)'
root -l -b -q plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphzz4l_125.8_1_Asimov_CF.MultiDimFit.mH125.8.root","cVcF_hcphzz4l_CF","CF",-2,2,100)'
root -l -b -q plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcpvhbb_125.8_1_Asimov_CV.MultiDimFit.mH125.8.root","cVcF_hcpvhbb_CV","CV",0,2,100)'
root -l -b -q plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcpvhbb_125.8_1_Asimov_CF.MultiDimFit.mH125.8.root","cVcF_hcpvhbb_CF","CF",-2,2,100)'
root -l -b -q plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphires_125.8_1_Asimov_CV.MultiDimFit.mH125.8.root","cVcF_hcphires_CV","CV",0,2,100)'
root -l -b -q plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphires_125.8_1_Asimov_CF.MultiDimFit.mH125.8.root","cVcF_hcphires_CF","CF",-2,2,100)'
root -l -b -q plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcpFullComb_125.8_1_Asimov_CV.MultiDimFit.mH125.8.root","cVcF_hcpFullComb_CV","CV",0,2,100)'
root -l -b -q plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcpFullComb_125.8_1_Asimov_CF.MultiDimFit.mH125.8.root","cVcF_hcpFullComb_CF","CF",-2,2,100)'




#asimov toy with systematics, fixed Mu



#########2D asimov toy with systematics########D
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphgg_125.8_1_Asimov.MultiDimFit.mH125.8.root","Contour_CVCF_hcphgg","CV","CF",0.0,2.0,-2.0,2.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphww2l_125.8_1_Asimov.MultiDimFit.mH125.8.root","Contour_CVCF_hcphww2l","CV","CF",0.0,2.0,-2.0,2.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphzz4l_125.8_1_Asimov.MultiDimFit.mH125.8.root","Contour_CVCF_hcphzz4l","CV","CF",0.0,2.0,-2.0,2.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcpvhbb_125.8_1_Asimov.MultiDimFit.mH125.8.root","Contour_CVCF_hcpvhbb","CV","CF",0.0,2.0,-2.0,2.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcphires_125.8_1_Asimov.MultiDimFit.mH125.8.root","Contour_CVCF_hcphires","CV","CF",0.0,2.0,-2.0,2.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/cVcF/higgsCombinecVcF_hcpFullComb_125.8_1_Asimov.MultiDimFit.mH125.8.root","Contour_CVCF_hcpFullComb","CV","CF",0.0,2.0,-2.0,2.0)'


#lambdaWZ
root -l plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_lambdaWZ.MultiDimFit.mH125.8.root","lambdaWZ_HCPFullComb_125.8_1_Asimov_lambdaWZ","lambdaWZ",0,2,100)'
root -l plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_kZ.MultiDimFit.mH125.8.root","lambdaWZ_HCPFullComb_125.8_1_Asimov_kZ","kZ",0,2,100)'
root -l plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_kf.MultiDimFit.mH125.8.root","lambdaWZ_HCPFullComb_125.8_1_Asimov_kf","kf",0,2,100)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_kZ_lambdaWZ.MultiDimFit.mH125.8.root","Contour_lambdaWZ_HCPFullComb_125.8_1_Asimov_kZlambdaWZ","kZ","lambdaWZ",0.0,2.0,-2.0,2.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_kZ_kf.MultiDimFit.mH125.8.root","Contour_lambdaWZ_HCPFullComb_125.8_1_Asimov_kZkf","kZ","kf",0.0,2.0,-2.0,2.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_125.8_1_Asimov_lambdaWZ_kf.MultiDimFit.mH125.8.root","Contour_lambdaWZ_HCPFullComb_125.8_1_Asimov_lambdaWZkf","lambdaWZ","kf",0.0,2.0,-2.0,2.0)'

#lambdadu
root -l plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_lambdadu.MultiDimFit.mH125.8.root","lambdadu_HCPFullComb_125.8_1_Asimov_lambdadu","lambdadu",-3,3,100)'
root -l plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_kV.MultiDimFit.mH125.8.root","lambdadu_HCPFullComb_125.8_1_Asimov_kV","kV",0,2,100)'
root -l plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_ku.MultiDimFit.mH125.8.root","lambdadu_HCPFullComb_125.8_1_Asimov_ku","ku",0,2,100)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_lambdadu_ku.MultiDimFit.mH125.8.root","Contour_lambdadu_HCPFullComb_125.8_1_Asimov_lambdaduku","lambdadu","ku",-3.0,3.0,-2.0,2.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_kV_lambdadu.MultiDimFit.mH125.8.root","Contour_lambdadu_HCPFullComb_125.8_1_AsimovkVlambdadu","kV","lambdadu",0.0,2.0,-3.0,3.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_125.8_1_Asimov_kV_ku.MultiDimFit.mH125.8.root","Contour_lambdadu_HCPFullComb_125.8_1_Asimov_kVku","kV","ku",0.0,2.0,-2.0,2.0)'

#lambdalq
root -l plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_lambdalq.MultiDimFit.mH125.8.root","lambdalq_HCPFullComb_125.8_1_Asimov_lambdalq","lambdalq",-3,3,100)'
root -l plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_kV.MultiDimFit.mH125.8.root","lambdalq_HCPFullComb_125.8_1_Asimov_kV","kV",0,2,100)'
root -l plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_kq.MultiDimFit.mH125.8.root","lambdalq_HCPFullComb_125.8_1_Asimov_kq","kq",0,2,100)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_lambdalq_kq.MultiDimFit.mH125.8.root","Contour_lambdalq_HCPFullComb_125.8_1_Asimov_lambdalqkq","lambdalq","kq",-3.0,3.0,-2.0,2.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_kV_lambdalq.MultiDimFit.mH125.8.root","Contour_lambdalq_HCPFullComb_125.8_1_AsimovkVlambdalq","kV","lambdalq",0.0,2.0,-3.0,3.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_125.8_1_Asimov_kV_kq.MultiDimFit.mH125.8.root","Contour_lambdalq_HCPFullComb_125.8_1_Asimov_kVkq","kV","kq",0.0,2.0,-2.0,2.0)'



########Data##############
#lambdaWZ
root -l plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_lambdaWZ.MultiDimFit.mH125.8.root","lambdaWZ_HCPFullComb_lambdaWZ","lambdaWZ",0,2,100)'
root -l plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_kZ.MultiDimFit.mH125.8.root","lambdaWZ_HCPFullComb_kZ","kZ",0,2,100)'
root -l plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_kf.MultiDimFit.mH125.8.root","lambdaWZ_HCPFullComb_kf","kf",0,2,100)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_kZ_lambdaWZ.MultiDimFit.mH125.8.root","Contour_lambdaWZ_HCPFullComb_kZlambdaWZ","kZ","lambdaWZ",0.0,2.0,-2.0,2.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_kZ_kf.MultiDimFit.mH125.8.root","Contour_lambdaWZ_HCPFullComb_kZkf","kZ","kf",0.0,2.0,-2.0,2.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_lambdaWZ_kf.MultiDimFit.mH125.8.root","Contour_lambdaWZ_HCPFullComb_lambdaWZkf","lambdaWZ","kf",0.0,2.0,-2.0,2.0)'
root -l plotting/plotSelectedContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_kZ_lambdaWZ.MultiDimFit.mH125.8.root","lambdaWZ-kZ",999,999)'
root -l plotting/plotSelectedContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_kZ_kf.MultiDimFit.mH125.8.root","lambdaWZ-kZ",999,999)'
root -l plotting/plotSelectedContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdaWZ/higgsCombinelambdaWZ_HCPFullComb_lambdaWZ_kf.MultiDimFit.mH125.8.root","Contour_lambdaWZ_HCPFullComb_lambdaWZkf","lambdaWZ","kf",0.0,2.0,-2.0,2.0)'


#lambdadu
root -l  plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_lambdadu.MultiDimFit.mH125.8.root","lambdadu_HCPFullComb_lambdadu","lambdadu",-3,3,100)'
root -l  plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_kV.MultiDimFit.mH125.8.root","lambdadu_HCPFullComb_kV","kV",0,2,100)'
root -l  plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_ku.MultiDimFit.mH125.8.root","lambdadu_HCPFullComb_ku","ku",0,2,100)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_lambdadu_ku.MultiDimFit.mH125.8.root","Contour_lambdadu_HCPFullComb_lambdaduku","lambdadu","ku",-3.0,3.0,-2.0,2.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_kV_lambdadu.MultiDimFit.mH125.8.root","Contour_lambdadu_HCPFullCombkVlambdadu","kV","lambdadu",0.0,2.0,-3.0,3.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdadu/higgsCombinelambdadu_HCPFullComb_kV_ku.MultiDimFit.mH125.8.root","Contour_lambdadu_HCPFullComb_kVku","kV","ku",0.0,2.0,-2.0,2.0)'



#lambdalq
root -l plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_lambdalq.MultiDimFit.mH125.8.root","lambdalq_HCPFullComb_lambdalq","lambdalq",-3,3,100)'
root -l plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_kV.MultiDimFit.mH125.8.root","lambdalq_HCPFullComb_kV","kV",0,2,100)'
root -l plotting/plot1DNLL.C'("/afs/cern.ch/user/s/sixie/CMSSW_analysis/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_kq.MultiDimFit.mH125.8.root","lambdalq_HCPFullComb_kq","kq",0,2,100)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_lambdalq_kq.MultiDimFit.mH125.8.root","Contour_lambdalq_HCPFullComb_lambdalqkq","lambdalq","kq",-3.0,3.0,-2.0,2.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_kV_lambdalq.MultiDimFit.mH125.8.root","Contour_lambdalq_HCPFullCombkVlambdalq","kV","lambdalq",0.0,2.0,-3.0,3.0)'
root -l plotting/plotContours.C'("/afs/cern.ch/work/s/sixie/public/releases/analysis/CMSSW_5_3_3_patch3/src/HiggsProperties/results/lambdalq/higgsCombinelambdalq_HCPFullComb_kV_kq.MultiDimFit.mH125.8.root","Contour_lambdalq_HCPFullComb_kVkq","kV","kq",0.0,2.0,-2.0,2.0)'

