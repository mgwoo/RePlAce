#!/bin/bash
# download all required binary (ntuplace3, ntuplace4h)
wget https://github.com/The-OpenROAD-Project/RePlAce/blob/master/ntuplace/ntuplace3?raw=true
wget https://github.com/The-OpenROAD-Project/RePlAce/blob/master/ntuplace/ntuplace4h?raw=true

# rename binaries
mv ntuplace3?raw=true ntuplace3
mv ntuplace4h?raw=true ntuplace4h

# set permission (executable)
chmod a+x ntuplace*

# gunzip all IBM benchmarks
gunzip -r IBM/

# for RePlAce evaluation
rm -rf eval
mkdir -p eval/bin/

# download all evaluation binaries
# 1. Wirelength Calculator
wget http://vlsicad.eecs.umich.edu/BK/PlaceUtils/bin/Lnx64/WLCalc-Lnx64.exe.gz 
mv WLCalc-Lnx64.exe.gz eval/bin/

# 2. Congestion Map Plotter 
wget http://vlsicad.eecs.umich.edu/BK/PlaceUtils/bin/Lnx64/CongestionMaps-Lnx64.exe.gz
mv CongestionMaps-Lnx64.exe.gz eval/bin/

# gunzip all evaluation binaries
gunzip -r eval/bin/

# set permission (executable)
chmod a+x eval/bin/*
