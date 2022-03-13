#!/bin/bash
##### RePlAce run
# make sure we erase all outputs before running RePlAce
rm -rf ./outputs

# run RePlAce binary
for i in {01..18}; do
  python3 run_binary.py 1.03 ibm${i} |& tee ibm${i}_pcof.1.03.log;
done


###### RePlAce evaluation
# set up the evaluation environment folder (eval)
# keep original benchmarks and overwrite the ibm%02d.ntup.pl as ibm%02d.pl.
for i in {01..18}; do
  mkdir -p eval/ibm${i};
  cp -va ./outputs/IBM/ibm${i}/experiment0/tiers/0/ibm${i}.ntup.pl eval/ibm${i}/;
  cp -va IBM/ibm${i}/* eval/ibm${i}/;
  rm -rf eval/ibm${i}/ibm${i}.pl;
  ln -snf ibm${i}.ntup.pl eval/ibm${i}/ibm${i}.pl;
  ln -snf $(readlink -f .)/eval/bin/CongestionMaps-Lnx64.exe eval/ibm${i}/;
  ln -snf $(readlink -f .)/eval/bin/WLCalc-Lnx64.exe eval/ibm${i}/;
done

# extract hpwl and congestion info
python3 eval.py
