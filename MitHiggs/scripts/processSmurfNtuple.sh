#!/bin/sh

########################################################
#Merge ntuples
########################################################

#May 10 Rereco
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-m10-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-m10-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-dmu-m10-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-m10-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-m10-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-dmu-m10-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-dmu-m10-v1_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-m10-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-m10-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-del-m10-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-m10-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-m10-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-del-m10-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-del-m10-v1_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-m10-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-m10-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-mueg-m10-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-m10-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-m10-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-mueg-m10-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-mueg-m10-v1_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-m10-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-m10-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-sel-m10-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-m10-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-m10-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-sel-m10-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-sel-m10-v1_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-m10-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-m10-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-smu-m10-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-m10-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-m10-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-smu-m10-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-smu-m10-v1_*.root 



#Prompt Reco V4

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-pr-v4_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-pr-v4_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-dmu-pr-v4_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-pr-v4_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-pr-v4_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-dmu-pr-v4_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-dmu-pr-v4_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-pr-v4_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-pr-v4_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-del-pr-v4_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-pr-v4_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-pr-v4_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-del-pr-v4_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-del-pr-v4_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-pr-v4_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-pr-v4_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-mueg-pr-v4_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-pr-v4_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-pr-v4_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-mueg-pr-v4_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-mueg-pr-v4_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-pr-v4_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-pr-v4_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-sel-pr-v4_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-pr-v4_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-pr-v4_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-sel-pr-v4_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-sel-pr-v4_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-pr-v4_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-pr-v4_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-smu-pr-v4_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-pr-v4_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-pr-v4_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-smu-pr-v4_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-smu-pr-v4_*.root 


#Aug 05 Rereco
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-a05-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-a05-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-dmu-a05-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-a05-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-a05-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-dmu-a05-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-dmu-a05-v1_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-a05-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-a05-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-del-a05-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-a05-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-a05-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-del-a05-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-del-a05-v1_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-a05-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-a05-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-mueg-a05-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-a05-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-a05-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-mueg-a05-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-mueg-a05-v1_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-a05-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-a05-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-sel-a05-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-a05-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-a05-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-sel-a05-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-sel-a05-v1_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-a05-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-a05-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-smu-a05-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-a05-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-a05-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-smu-a05-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-smu-a05-v1_*.root 



#Oct03 Rereco
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-o03-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-o03-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-dmu-o03-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-o03-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-o03-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-dmu-o03-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-dmu-o03-v1_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-o03-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-o03-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-del-o03-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-o03-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-o03-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-del-o03-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-del-o03-v1_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-o03-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-o03-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-mueg-o03-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-o03-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-o03-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-mueg-o03-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-mueg-o03-v1_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-o03-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-o03-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-sel-o03-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-o03-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-o03-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-sel-o03-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-sel-o03-v1_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-o03-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-o03-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11a-smu-o03-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-o03-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-o03-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11a-smu-o03-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11a-smu-o03-v1_*.root 



#Prompt Reco V1

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-dmu-pr-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-dmu-pr-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11b-dmu-pr-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-dmu-pr-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-dmu-pr-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11b-dmu-pr-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11b-dmu-pr-v1_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-del-pr-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-del-pr-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11b-del-pr-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-del-pr-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-del-pr-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11b-del-pr-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11b-del-pr-v1_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-mueg-pr-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-mueg-pr-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11b-mueg-pr-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-mueg-pr-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-mueg-pr-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11b-mueg-pr-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11b-mueg-pr-v1_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-sel-pr-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-sel-pr-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11b-sel-pr-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-sel-pr-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-sel-pr-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11b-sel-pr-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11b-sel-pr-v1_*.root 

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-smu-pr-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-smu-pr-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf0_r11b-smu-pr-v1_*.root 
hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-smu-pr-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-smu-pr-v1_noskim_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf1_r11b-smu-pr-v1_*.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_smurf2_r11b-smu-pr-v1_*.root 



########################################################
#Merge all data into a single file
########################################################

hadd -f /home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_data_all_noskim.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-m10-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-m10-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-m10-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-m10-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-m10-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-pr-v4_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-pr-v4_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-pr-v4_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-pr-v4_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-pr-v4_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-m10-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-m10-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-m10-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-m10-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-m10-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-pr-v4_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-pr-v4_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-pr-v4_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-pr-v4_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-pr-v4_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-a05-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-a05-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-a05-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-a05-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-a05-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-o03-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-o03-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-o03-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-o03-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-o03-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-a05-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-a05-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-a05-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-a05-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-a05-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-dmu-o03-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-del-o03-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-mueg-o03-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-sel-o03-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11a-smu-o03-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-dmu-pr-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-del-pr-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-mueg-pr-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-sel-pr-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-smu-pr-v1_noskim.0.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-dmu-pr-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-del-pr-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-mueg-pr-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-sel-pr-v1_noskim.fakes.root \
/home/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_r11b-smu-pr-v1_noskim.fakes.root \

########################################################
#Split them into 2l, l+fake, fake+fake
########################################################

python $CMSSW_BASE/src/MitHiggs/scripts/NormalizeHiggsNtuple.py \
  --InputPath=/data/blue/sixie/hist/AllNtuple/smurfIDIsoCombined/data/ \
  --OutputPath=/data/blue/sixie/hist/AllNtuple/smurfIDIsoCombined/data/ \
  --FilenameHeader=AllNtuple \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/data_smurf.txt


mkdir -p /data/smurf/sixie/data/Run2011_Summer11_MVAIDIsoCombinedHalfBkgWP/
cp /data/blue/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_data_all_noskim_normalized.root /data/smurf/sixie/data/Run2011_Summer11_MVAIDIsoCombinedHalfBkgWP/data_2l.root
cp /data/blue/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_data_fake_all_noskim_normalized.root /data/smurf/sixie/data/Run2011_Summer11_MVAIDIsoCombinedHalfBkgWP/data_lfake.root
cp /data/blue/sixie/hist/AllNtuple/smurfIDIsoCombined/data/AllNtuple_data_2fake_all_noskim_normalized.root /data/smurf/sixie/data/Run2011_Summer11_MVAIDIsoCombinedHalfBkgWP/data_fakefake.root


########################################################
# Monte Carlo
########################################################
mkdir -p /data/blue/sixie/Smurf/MVAIDIsoCombined/mc/ 

python $CMSSW_BASE/src/MitHiggs/scripts/MergeFilesets.py --InputPath=/home/sixie/hist/AllNtuple/smurfIDIsoCombined/mc/   --OutputPath=/data/blue/sixie/Smurf/MVAIDIsoCombined/mc/   --FilenameHeader=AllNtuple --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/mc_smurf.txt

python $CMSSW_BASE/src/MitHiggs/scripts/NormalizeHiggsNtuple.py \
  --InputPath=/data/blue/sixie/Smurf/MVAIDIsoCombined/mc/ \
  --OutputPath=/data/blue/sixie/Smurf/MVAIDIsoCombined/mc/ \
  --FilenameHeader=AllNtuple \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/mc_smurf.txt


###############################################
#HWW signal
###############################################

hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/qqww.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-wwj-v11-pu*normalized*.root
hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/ttbar.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-tt2l-powheg-v11-pu*normalized*.root
hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/wtop.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-wtop-powheg-v11*normalized*.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-wtopb-powheg-v11*normalized*.root
hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/ggww.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-ggww2l-v11-pu*normalized*.root
hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/wz.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-wz-v11-pu*normalized*.root
hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/zz.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-zz-v11-pu*normalized*.root
hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/wgamma.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-w?g-v11-pu*normalized*.root

hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/zee.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-zeem20-powheg-v11-pu*normalized*.root
hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/zmm.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-zmmm20-powheg-v11-pu*normalized*.root
hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/ztt.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-zttm20-powheg-v11-pu*normalized*.root

hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/hww115.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-h115*normalized.root
hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/hww120.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-h120*normalized.root
hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/hww130.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-h130*normalized.root
hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/hww140.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-h140*normalized.root
hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/hww150.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-h150*normalized.root
hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/hww160.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-h160*normalized.root
hadd -f  /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/V2/mitf-alljets/hww200.root /data/blue/sixie/Smurf/MVAElectrons/V2/AllNtuple_s11-h200*normalized.root


###############################################
#HZZ signal
###############################################


mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h130zz2l-gf-v1g1-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/gfhzz130.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h140zz2l-gf-v1g1-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/gfhzz140.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h150zz2l-gf-v1g1-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/gfhzz150.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h160zz2l-gf-v1g1-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/gfhzz160.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h170zz2l-gf-v1g1-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/gfhzz170.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h180zz2l-gf-v1g1-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/gfhzz180.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h190zz2l-gf-v1g1-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/gfhzz190.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h220zz2l-gf-v1g1-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/gfhzz220.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h230zz2l-gf-v1g1-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/gfhzz230.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h250zz2l-gf-v1g1-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/gfhzz250.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h275zz2l-gf-v1g1-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/gfhzz275.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h300zz2l-gf-v1g1-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/gfhzz300.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h325zz2l-gf-v1g1-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/gfhzz325.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h450zz2l-gf-v1g1-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/gfhzz450.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h130zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz130.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h140zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz140.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h150zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz150.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h160zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz160.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h170zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz170.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h190zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz190.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h200zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz200.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h210zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz210.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h220zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz220.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h250zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz250.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h275zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz275.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h300zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz300.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h325zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz325.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h500zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz500.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h525zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz525.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h550zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz550.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h575zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz575.root
mv /data/blue/sixie/Smurf/V6/smurf0/AllNtuple_s11-h600zz2l-vbf-v1g1-pu_noskim_normalized.root   /data/smurf/sixie/data/Run2011_Summer11_SmurfV6/mitf-alljets/vbfhzz600.root


mv /data/blue/sixie/Smurf/MVAElectrons/smurf0/AllNtuple_s11-h115ww2l-gf-v11-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/mitf-alljets/gfhww115.root
mv /data/blue/sixie/Smurf/MVAElectrons/smurf0/AllNtuple_s11-h120ww2l-gf-v11-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/mitf-alljets/gfhww120.root
mv /data/blue/sixie/Smurf/MVAElectrons/smurf0/AllNtuple_s11-h130ww2l-gf-v11-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/mitf-alljets/gfhww130.root
mv /data/blue/sixie/Smurf/MVAElectrons/smurf0/AllNtuple_s11-h140ww2l-gf-v11-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/mitf-alljets/gfhww140.root
mv /data/blue/sixie/Smurf/MVAElectrons/smurf0/AllNtuple_s11-h150ww2l-gf-v11-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/mitf-alljets/gfhww150.root
mv /data/blue/sixie/Smurf/MVAElectrons/smurf0/AllNtuple_s11-h160ww2l-gf-v11-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/mitf-alljets/gfhww160.root
mv /data/blue/sixie/Smurf/MVAElectrons/smurf0/AllNtuple_s11-h200ww2l-gf-v11-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/mitf-alljets/gfhww200.root

mv /data/blue/sixie/Smurf/MVAElectrons/smurf0/AllNtuple_s11-h115ww2l-vbf-v11-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/mitf-alljets/vbfhww115.root
mv /data/blue/sixie/Smurf/MVAElectrons/smurf0/AllNtuple_s11-h120ww2l-vbf-v11-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/mitf-alljets/vbfhww120.root
mv /data/blue/sixie/Smurf/MVAElectrons/smurf0/AllNtuple_s11-h130ww2l-vbf-v11-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/mitf-alljets/vbfhww130.root
mv /data/blue/sixie/Smurf/MVAElectrons/smurf0/AllNtuple_s11-h140ww2l-vbf-v11-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/mitf-alljets/vbfhww140.root
mv /data/blue/sixie/Smurf/MVAElectrons/smurf0/AllNtuple_s11-h150ww2l-vbf-v11-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/mitf-alljets/vbfhww150.root
mv /data/blue/sixie/Smurf/MVAElectrons/smurf0/AllNtuple_s11-h160ww2l-vbf-v11-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/mitf-alljets/vbfhww160.root
mv /data/blue/sixie/Smurf/MVAElectrons/smurf0/AllNtuple_s11-h200ww2l-vbf-v11-pu_noskim_normalized.root    /data/smurf/sixie/data/Run2011_Summer11_SmurfMVAElectrons/mitf-alljets/vbfhww200.root




########################################################
# Data
########################################################

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=/data/blue/sixie/hist/SmurfNtuple/ \
  --FilenameHeader=SmurfNtuple \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/data_41x.txt \
  --OutputLabel=data \
  --TestSampleFraction=0.0									   


python $CMSSW_BASE/src/MitHiggs/scripts/NormalizeHiggsNtuple.py \
  --InputPath=/home/sixie/hist/SmurfNtuple/mc/020/ \
  --OutputPath=/data/blue/sixie/Smurf/V6/ \
  --FilenameHeader=SmurfNtuple \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HZZAnalysis_42x.txt



python $CMSSW_BASE/src/MitHiggs/scripts/NormalizeHiggsNtuple.py \
  --InputPath=/data/blue/sixie/temp/ \
  --OutputPath=/data/blue/sixie/Smurf/V6/ \
  --FilenameHeader=HwwNtuple \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HiggsAnalysisFullSampleList_41x.txt






python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=/data/blue/sixie/Smurf/V6/ \
  --FilenameHeader=HwwNtuple \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/wjets_41x.txt \
  --OutputLabel=wjets \
  --TestSampleFraction=0.0									   




export OUTPUT=ntuples_41x;

#python $CMSSW_BASE/src/MitHiggs/scripts/NormalizeHiggsNtuple.py \
#  --InputPath=/home/ceballos/condor/old \
#  --OutputPath=$CMSSW_BASE/src/ \
#  --FilenameHeader=histo \
#  --DatasetListFile=$CMSSW_BASE/src/test.txt

python $CMSSW_BASE/src/MitHiggs/scripts/NormalizeHiggsNtuple.py \
  --InputPath=/home/ceballos/condor/old \
  --OutputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HiggsAnalysisFullSampleList_41x.txt

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW115_7TeV.txt \
  --OutputLabel=hww115_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW120_7TeV.txt \
  --OutputLabel=hww120_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW130_7TeV.txt \
  --OutputLabel=hww130_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW140_7TeV.txt \
  --OutputLabel=hww140_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW150_7TeV.txt \
  --OutputLabel=hww150_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW160_7TeV.txt \
  --OutputLabel=hww160_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW170_7TeV.txt \
  --OutputLabel=hww170_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW180_7TeV.txt \
  --OutputLabel=hww180_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW190_7TeV.txt \
  --OutputLabel=hww190_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW200_7TeV.txt \
  --OutputLabel=hww200_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW250_7TeV.txt \
  --OutputLabel=hww250_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW300_7TeV.txt \
  --OutputLabel=hww300_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW350_7TeV.txt \
  --OutputLabel=hww350_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW400_7TeV.txt \
  --OutputLabel=hww400_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW450_7TeV.txt \
  --OutputLabel=hww450_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW500_7TeV.txt \
  --OutputLabel=hww500_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW550_7TeV.txt \
  --OutputLabel=hww550_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HWW600_7TeV.txt \
  --OutputLabel=hww600_std_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/MC_LFO_7TeV_41x.txt \
  --OutputLabel=mc_l_fake_pu \
  --TestSampleFraction=0.0

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HiggsAnalysisStandardPileUpList_7TeV_41x.txt \
  --OutputLabel=hww_std_pu \
  --TestSampleFraction=0.5									   

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HiggsAnalysisStandardDataPileUpList_7TeV_41x.txt \
  --OutputLabel=hww_std_data_pu \
  --TestSampleFraction=0.5									   

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/HiggsAnalysisTrainingPileUpList_7TeV_41x.txt \
  --OutputLabel=hww_train_pu \
  --TestSampleFraction=0.5									   

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/data_41x.txt \
  --OutputLabel=data \
  --TestSampleFraction=0.0									   

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/dymm_41x.txt \
  --OutputLabel=dymm \
  --TestSampleFraction=0.0									   

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/dyee_41x.txt \
  --OutputLabel=dyee \
  --TestSampleFraction=0.0									   

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/dytt_41x.txt \
  --OutputLabel=dytt \
  --TestSampleFraction=0.0									   

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/wz_41x.txt \
  --OutputLabel=wz \
  --TestSampleFraction=0.0									   

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/zz_41x.txt \
  --OutputLabel=zz \
  --TestSampleFraction=0.0									   

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/wjets_41x.txt \
  --OutputLabel=wjets \
  --TestSampleFraction=0.0									   

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/qqww_41x.txt \
  --OutputLabel=qqww \
  --TestSampleFraction=0.0									   

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/ggww_41x.txt \
  --OutputLabel=ggww \
  --TestSampleFraction=0.0									   

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/tw_41x.txt \
  --OutputLabel=tw \
  --TestSampleFraction=0.0									   

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/ttbar_41x.txt \
  --OutputLabel=ttbar \
  --TestSampleFraction=0.0									   

python $CMSSW_BASE/src/MitHiggs/scripts/CreateTrainingAndTestSamples.py \
  --InputPath=$CMSSW_BASE/src/$OUTPUT \
  --FilenameHeader=histo \
  --DatasetListFile=$CMSSW_BASE/src/MitHiggs/scripts/wgamma_41x.txt \
  --OutputLabel=wgamma \
  --TestSampleFraction=0.0
