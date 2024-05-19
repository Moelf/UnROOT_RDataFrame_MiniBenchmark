## Detailed Results:
### Single-thread composite benchmark
| Language | Compile | 1st Run (including JIT time) | 2nd Run |
| -------- | ------------ | ------- | ---------- |
| [Julia](https://nbviewer.jupyter.org/github/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/UnROOT_benchmark.ipynb) | -- | 20.74 s | 19.67 s |
| [PyROOT RDF](https://nbviewer.jupyter.org/github/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/RDataFrame_benchmark.ipynb) | -- | 43.77 s | 38.97 s |
| [Compiled RDF](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/composite_benchmarks/RDataFrame_benchmark_compiled_single.cpp) | 8.23 s | -- | 25.04 s |
| [Compiled C++ ROOT Loop](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/tree/main/composite_benchmarks#root-rdataframe-g-compiled) | 3.65 s | -- | 19.63 s |
| [Python Uproot loop-less](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/tree/main/composite_benchmarks#uproot-loop-less) | -- | -- | 29.13 s |

### 4-threads composite benchmark
| Language | Compile | 1st Run (including JIT time) | 2nd Run |
| -------- | ------- | ------- | ---------- |
| [Julia](https://nbviewer.jupyter.org/github/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/UnROOT_benchmark.ipynb) | -- | 5.49 s | 5.25 s |
| PyROOT RDF | -- | 11.07 s | 11.01 s |
| [Compiled RDF](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/composite_benchmarks/RDataFrame_benchmark_compiled_MT.cpp) | 8.27 s | -- | 8.67 s |
| Compiled C++ ROOT Loop |  Not impl. | -- | -- |
| Python Uproot loop-less | Not impl. | -- | -- |

## How-to reproduce

Putting the `.root` from http://opendata.web.cern.ch/record/12341/files/Run2012BC_DoubleMuParked_Muons.root inside the `composite_benchmarks` directory:

### Julia
```
$ julia -t 4 --project=. --startup-file=no ./UnROOT_loop.jl
[ Info: 1st run
 20.744484 seconds (17.79 M allocations: 16.363 GiB, 19.86% gc time, 1.62% compilation time)
[ Info: 2nd run
 19.673402 seconds (17.27 M allocations: 16.327 GiB, 19.31% gc time, 0.03% compilation time)
[ Info: 4-threads 1st run
  5.492121 seconds (17.60 M allocations: 16.954 GiB, 7.79% gc time, 12.61% compilation time)
[ Info: 4-threads 2nd run
  5.248575 seconds (17.27 M allocations: 16.932 GiB, 6.81% gc time)
```

### PyROOT RDataFrame
```
$ python PyROOT_RDataFrame.py
1-thread:  43.7752480506897 seconds
1-thread:  38.96797323226929 seconds
4-threads: 11.071122407913208 seconds
4-threads: 11.008059740066528 seconds
```

### ROOT RDataFrame (g++ compiled)
```
$ time g++ RDataFrame_benchmark_compiled_single.cpp -O3 `root-config --cflags --glibs` -o run;
real	0m8.227s

$ ./run
Real time 0:00:25.036, CP time 25.020
$ ./run
Real time 0:00:25.038, CP time 25.030

# 4-threads

$ time g++ RDataFrame_benchmark_compiled_MT.cpp -O3 `root-config --cflags --glibs` -o run_mt
real	0m8.270s

$ ./run_mt
Real time 0:00:08.676, CP time 33.480
$ ./run_mt
Real time 0:00:09.158, CP time 35.370
```

### ROOT for-loop (g++ compiled)
```
$ time g++ ROOT_loop_compiled.cpp -O3 `root-config --cflags --glibs` -o run
real	0m3.648s

$ ./run
Real time 0:00:19.636, CP time 19.630
$ ./run
Real time 0:00:19.664, CP time 19.660
```

### Uproot loop less
```
# also see requirements.txt
login04.af.uchicago.edu:~/UnROOT_RDataFrame_MiniBenchmark/composite_benchmarks $ python Python_uproot.py
/home/jiling/UnROOT_RDataFrame_MiniBenchmark/composite_benchmarks/uproot_env/lib64/python3.9/site-packages/coffea/nanoevents/methods/candidate.py:11: FutureWarning: In version 2024.7.0 (target date: 2024-06-30 11:59:59-05:00), this will be an error.
...

1-thread:
0:00:29.135526
```
