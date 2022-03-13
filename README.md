# RePlAce
RePlAce: Advancing Solution Quality and Routability Validation in Global Placement

This repo is to distribute the bug-fixed and deterministic binaries that are targeted for Bookshelf formats (ICCAD04, ISPD05, ISPD06, MMS09, DAC12, ICCAD12) as TCAD19 paper described.
The difference between this binary and [archived RePlAce standalone](https://github.com/The-OpenROAD-Project/RePlAce/tree/standalone) is, these binaries are targeted only for academic Bookshelf formats. 

The original TCAD19-published binary had the following problems. 
- Memory violations on the hash map structure. Also, some parts of the Nesterov loop had memory violations because of non-zero-filled structures. 
- Non-deterministic problems due to MKL dependencies. (e.g., mkl_malloc, etc).
- Non-deterministic problems due to dynamically linked libraries.

There are two RePlAce binaries in this repo.
1) [RePlAce](./RePlAce) 
    1) Rewriting hash map structure with std::map. 
    2) Make sure all of the Nesterov loop-related structure has been correctly zero-filled.
2) [RePlAce-static](./RePlAce-static) - A static binary version from [RePlAce](./RePlAce)
    1) Replacing mkl_malloc with malloc and removing MKL dependency.
    2) Generate as a static binary by removing other redundant libraries.

If you need an open-sourced version of RePlAce, please consider using the [OpenROAD integrated RePlAce repo](https://github.com/The-OpenROAD-Project/OpenROAD/tree/master/src/gpl). (more clean C++11 structures).

## [Modified ICCAD04 Benchmarks](./IBM/)
We also provide the modified version of ICCAD04 benchmarks.
- Periphery pins are moved to the nearest vertical or horizontal location on the edges of the placement area.
- Locations of the .pl files were zeroed out so that placement tools can start from scratch.

## An example script to run the whole modified ICCAD04 benchmarks (ibm01~ibm18)
    $ pip3 install -r requirements.txt
    $ ./setup_and_run.sh

## Detailed Binary Usages 
### Prerequisite
1. Python >= 3.9.0
2. [NTUPlace3](http://eda.ee.ntu.edu.tw/research.htm) - Detailed placer
3. [NTUPlace4h](http://eda.ee.ntu.edu.tw/research.htm) - Detailed placer
4. [NCTUgr.ICCAD12](https://people.cs.nctu.edu.tw/~whliu/NCTU-GR.htm) - Global router - (optional: for the routability driven mode)
5. [WLCalc-Lnx64.exe](http://vlsicad.eecs.umich.edu/BK/PlaceUtils) - HPWL evaluation
6. [CongestionMaps-Lnx64.exe](http://vlsicad.eecs.umich.edu/BK/PlaceUtils) - Congestion evaluation
- Note that [setup.sh](setup.sh) will download 2, 3, 5, and 6.

### Directory Structures
All binaries should be located as in the following:

    ./RePlAce
    ./ntuplace3
    ./ntuplace4h
    ./NCTUgr.ICCAD12

Case 1. ICCAD04 Testcases:

    ./IBM/ibm01/ibm01.aux
    ./IBM/ibm02/ibm02.aux
    ... (from ibm01 to ibm18)

An Example of Running ICCAD04 Commands (inside ./run):

    ./RePlAce -bmflag ibm -bmname ibm01
    
Case 2. ISPD05/ISPD06 Testcases:

    ./ISPD/adaptec1/adaptec1.aux
    ./ISPD/adaptec2/adaptec2.aux
    ... (from adaptec1 to newblue7)

An Example of Running ISPD05/ISPD06 Commands:

    ./RePlAce -bmflag ispd -bmname adaptec1

Case 3. MMS09 testcases:

    ./MMS/adaptec1/adaptec1.aux
    ./MMS/adaptec2/adaptec2.aux
    ... (from adaptec1 to newblue7)

An Example of Running MMS09 Commands:

    ./RePlAce -bmflag mms -bmname adaptec1

Case 4. DAC12/ICCAD12 testcases:

    ./SB/superblue1/superblue1.aux
    ./SB/superblue1/superblue2.aux
    ... (from superblue1 to superblue19)

An Example of Running DAC12/ICCAD12 Commands:

    ./RePlAce -bmflag sb -bmname superblue1 
    
Optional parameters: 
- `-pcofmin`: set `pcof_min(µ_k Lower Bound)`. Default is 0.95. Allowed values are `[0.95-1.05, float]`.
- `-pcofmax`: set `pcof_max(µ_k Upper Bound)`. Default is 1.05. Allowed values are `[1.00-1.20, float]`.
- `-DP`: set Detailed Placer (DP). Default is NTUplace3. Allowed values are `[NTUplace3|NTUplace4h, string]`.
- `-LD`: enable local density mode from TCAD19 paper. No values are required. (flag)
- `-DS`: enable dynamic step size adaption mode from TCAD19 paper. No values are required. (flag)
- `-R`: enable routability-driven mode from TCAD19 paper. No values are required. (flag)
- `-max_ip_iter`: the number of maximum iterations in the initial placement (BiCGSTAB placer). Default is 30. Allowed values are `[0-MAX_INT, int]`. (Only [RePlAce-static](./RePlAce-static) supports)

## References
- TCAD19 Paper reference: C.-K. Cheng, A. B. Kahng, I. Kang and L. Wang, "RePlAce: Advancing Solution Quality and Routability Validation in Global Placement", IEEE Transactions on Computer-Aided Design of Integrated Circuits and Systems, (2019), pp. 1717-1730. (Digital Object Identifier: 10.1109/TCAD.2018.2859220)
