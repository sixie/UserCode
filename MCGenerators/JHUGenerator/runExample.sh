# Spin2 X -> WW -> lvlvL
./JHUGen Collider=1 Process=2 DecayMode1=4 DecayMode2=4 PChannel=2 PDFSet=1 Unweighted=1 OffXVV=011 VegasNc1=300000

# Spin2 X -> ZZ -> llll
./JHUGen Collider=1 Process=0 DecayMode1=0 DecayMode2=0 PChannel=2 PDFSet=1 Unweighted=1 OffXVV=011

# Spin2 X -> gamma gamma
./JHUGen Collider=1 Process=2 DecayMode1=7 DecayMode2=7 PChannel=0 PDFSet=1 Unweighted=1 OffXVV=100

~/condor/bin/runJHU.csh 300000 1 JHU_WWlvlv_Spin2_PChannel2_PDFSet1 2 4 4 2 011

~/condor/bin/runJHU.csh 10000 1 JHU_gg_Spin2_PChannel2_PDFSet1 2 7 7 2 000

