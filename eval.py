import pandas as pd
import re
import subprocess

def measure_congestion(benchmark_aux, benchmark_name, run_dir):
  # This utility for measuring congestion was downloaded from: 
  # http://vlsicad.eecs.umich.edu/BK/PlaceUtils/
  result = subprocess.run([f'./CongestionMaps-Lnx64.exe', '-f', benchmark_aux], 
                     check=True, capture_output=True, cwd=run_dir)
  out = result.stdout.decode('utf-8')
  median  = float(re.search('Median Congestion : (([0-9]*[.])?[0-9]+)', out)[1])
  average = float(re.search('Average Congestion : (([0-9]*[.])?[0-9]+)', out)[1])
  peak    = float(re.search('Peak Congestion : (([0-9]*[.])?[0-9]+)', out)[1])
  print(f'-I- congestion median = {median}, average = {average}, peak = {peak}')
  df = pd.DataFrame([dict(benchmark=benchmark_name, median=median, average=average, peak=peak)])
  return df[['benchmark', 'median', 'average', 'peak']]


def measure_hpwl(benchmark_aux, benchmark_name, run_dir):
  # This utility for measuring congestion was downloaded from: 
  # http://vlsicad.eecs.umich.edu/BK/PlaceUtils/
  result = subprocess.run([f'./WLCalc-Lnx64.exe', '-f', benchmark_aux], 
                     check=True, capture_output=True, cwd=run_dir)
  out = result.stdout.decode('utf-8')

  hpwl  = float(re.search('Pin-to-pin HalfPerim WL      : (.*?) \(', out)[1])
  print(f'-I- pin-to-pin hpwl = {hpwl}')
  return hpwl

# run evaluator for all designs 
df = pd.DataFrame(columns=['benchmark', 'hpwl', 'median', 'average', 'peak'])
for i in range(1,19):
  res = measure_congestion("ibm%02d.aux" % (i), "ibm%02d" % (i), "eval/ibm%02d" % (i))
  res['hpwl'] = measure_hpwl("ibm%02d.aux" % (i), "ibm%02d" % (i), "eval/ibm%02d" % (i))
  df = df.append(res)

print(df)
