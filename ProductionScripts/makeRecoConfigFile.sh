#!/bin/bash

dataset=$1
echo $dataset

rfdir /castor/cern.ch/user/s/sixie/RAW/$dataset | awk '{print "\"castor:/castor/cern.ch/user/s/sixie/RAW/XXX/" $9 "\"," }' | sed -e "s/XXX/$dataset/g" > recoInputfiles

