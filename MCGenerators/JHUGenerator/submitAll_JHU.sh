#HWW Spin2
foreach f(`seq 1 1 400`)
 echo $f
 bsub -q 1nd -o /afs/cern.ch/user/s/sixie/work/private/condor/res/JHUGenerator/JHU_WWlvlv_Spin2_PChannel2_PDFSet1_${f}.out -J JHU_WWlvlv_Spin2_PChannel2_PDFSet1_${f} /afs/cern.ch/work/s/sixie/public/condor/bin/runJHU.csh 300000 ${f} JHU_WWlvlv_Spin2_PChannel2_PDFSet1 2 4 4 2 011
end

#HWW Spin0
foreach f(`seq 1 1 400`)
 echo $f
 bsub -q 1nd -o /afs/cern.ch/user/s/sixie/work/private/condor/res/JHUGenerator/JHU_WWlvlv_Spin0_PChannel2_PDFSet1_${f}.out -J JHU_WWlvlv_Spin0_PChannel2_PDFSet1_${f} /afs/cern.ch/work/s/sixie/public/condor/bin/runJHU.csh 300000 ${f} JHU_WWlvlv_Spin0_PChannel2_PDFSet1 0 4 4 2 011
end


#Hgg Spin2
foreach f(`seq 1 1 400`)
 echo $f
 bsub -q 1nd -o /afs/cern.ch/user/s/sixie/work/private/condor/res/JHUGenerator/JHU_gg_Spin2_PChannel2_PDFSet1_${f}.out -J JHU_gg_Spin2_PChannel2_PDFSet1_${f} /afs/cern.ch/work/s/sixie/public/condor/bin/runJHU.csh 300000 ${f} JHU_gg_Spin2_PChannel2_PDFSet1 2 7 7 2 000
end

#Hgg Spin0
foreach f(`seq 1 1 50`)
 echo $f
 bsub -q 1nd -o /afs/cern.ch/user/s/sixie/work/private/condor/res/JHUGenerator/JHU_gg_Spin0_PChannel2_PDFSet1_${f}.out -J JHU_gg_Spin0_PChannel2_PDFSet1_${f} /afs/cern.ch/work/s/sixie/public/condor/bin/runJHU.csh 100000 ${f} JHU_gg_Spin0_PChannel2_PDFSet1 0 7 7 2 000
end


