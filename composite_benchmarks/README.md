## Detailed Results:
### Single-thread composite benchmark
| Language | Compile | 1st Run (including JIT time) | 2nd Run |
| -------- | ------------ | ------- | ---------- |
| [Julia](https://nbviewer.jupyter.org/github/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/UnROOT_benchmark.ipynb) | -- | 16.27 s | 15.48 s |
| [PyROOT RDF](https://nbviewer.jupyter.org/github/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/RDataFrame_benchmark.ipynb) | -- | 44.66 s | 40.22 s |
| [Compiled C++ ROOT Loop](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/tree/main/composite_benchmarks#root-rdataframe-g-compiled) | 3.58 s | -- | 19.96 s |
| [Compiled RDF](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/composite_benchmarks/RDataFrame_benchmark_compiled_single.cpp) | 8.35 s | -- | 24.97 s |

### 4-threads composite benchmark
| Language | Compile | 1st Run (including JIT time) | 2nd Run |
| -------- | ------- | ------- | ---------- |
| [Julia](https://nbviewer.jupyter.org/github/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/UnROOT_benchmark.ipynb) | -- | 4.71 s | 4.60 s |
| PyROOT RDF | -- | 11.02 s | 10.94 s |
| Compiled C++ ROOT Loop |  Not impl. | -- | -- |
| [Compiled RDF](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/composite_benchmarks/RDataFrame_benchmark_compiled_MT.cpp) | 8.68 s | -- | 10.23 s |

## How-to reproduce

Putting the `.root` from http://opendata.web.cern.ch/record/12341/files/Run2012BC_DoubleMuParked_Muons.root inside the `composite_benchmarks` directory:

### Julia
```
$ julia -t 4 --project=. --startup-file=no ./UnROOT_loop.jl
[ Info: 1st run
 16.277249 seconds (18.52 M allocations: 16.409 GiB, 1.58% gc time, 3.31% compilation time)
[ Info: 2nd run
 15.484084 seconds (17.27 M allocations: 16.327 GiB, 1.15% gc time)
[ Info: 4-threads 1st run
  4.706499 seconds (17.63 M allocations: 16.956 GiB, 2.39% gc time, 15.78% compilation time)
[ Info: 4-threads 2nd run
  4.597590 seconds (17.27 M allocations: 16.933 GiB, 1.80% gc time)
```

### PyROOT RDataFrame
```
$ python PyROOT_RDataFrame.py
1-thread:  44.6568386554718 seconds
1-thread:  40.22056269645691 seconds
4-threads: 11.021458864212036 seconds
4-threads: 10.93662428855896 seconds
```

### ROOT RDataFrame (g++ compiled)
```
$ time g++ RDataFrame_benchmark_compiled_single.cpp -O2 `root-config --cflags --glibs` -o run;
real	0m8.359s

#warm up once
$ g++ RDataFrame_benchmark_compiled_single.cpp -O2 `root-config --cflags --glibs` -o run; ./run
Real time 0:00:24.972, CP time 24.970
65807
9809
3991

$ time g++ RDataFrame_benchmark_compiled_MT.cpp -O2 `root-config --cflags --glibs` -o run;
real	0m8.675s

#warm up once
$ g++ RDataFrame_benchmark_compiled_MT.cpp -O2 `root-config --cflags --glibs` -o run; ./run
Real time 0:00:08.731, CP time 34.460
65807
9809
3991
```

### ROOT for-loop (g++ compiled)
```
$ time g++ ROOT_loop_compiled.cpp -O2 `root-config --cflags --glibs` -o run
real	0m3.582s

$ g++ ROOT_loop_compiled.cpp -O2 `root-config --cflags --glibs` -o run; ./run
Real time 0:00:19.975, CP time 19.970
67537
9942
4046
```
