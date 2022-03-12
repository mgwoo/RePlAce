import psutil
import subprocess
import sys

# this script will run RePlAce with affinity=0
def set_affinity():
  p = psutil.Process()
  print(f'-I- current cpu affinity = {p.cpu_affinity()}; setting cpu affinity to [0]')
  p.cpu_affinity([0])

# argv[1] == pcofmax
pcofmax = sys.argv[1]
# argv[2] == benchmark name (e.g., ibm01, ibm02, ..., ibm18)
benchname = sys.argv[2]

result = subprocess.run([f'./RePlAce', '-pcofmax', pcofmax, 
                                 '-bmflag', 'ibm', '-bmname', benchname, 
                                 '-DP', 'NTUplace3'], 
												check=False,
												preexec_fn=set_affinity)
