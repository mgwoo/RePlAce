##### RePlAce run
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

# make sure we erase all outputs before running RePlAce
rm -rf ./outputs

# run RePlAce binary
for i in {01..18}; do
  python3 run_binary.py 1.03 ibm${i} |& tee ibm${i}_pcof.1.03.log;
done


###### RePlAce evaluation
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

# set up the evaluation environment folder (eval)
# keep original benchmarks and overwrite the ibm%02d.ntup.pl as ibm%02d.pl.
for i in {01..18}; do
  mkdir eval/ibm${i};
  cp -va ./outputs/IBM/ibm${i}/experiment0/tiers/0/ibm${i}.ntup.pl eval/ibm${i}/;
  cp -va IBM/ibm${i}/* eval/ibm${i}/;
  rm -rf eval/ibm${i}/ibm${i}.pl;
  ln -snf ibm${i}.ntup.pl eval/ibm${i}/ibm${i}.pl;
  ln -snf ../bin/CongestionMaps-Lnx64.exe eval/ibm${i}/;
  ln -snf ../bin/WLCalc-Lnx64.exe eval/ibm${i}/;
done

# extract hpwl and congestion info
python3 eval.py
