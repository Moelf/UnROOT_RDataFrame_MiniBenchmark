# How-to

Putting the `.root` from http://opendata.web.cern.ch/record/12341/files/Run2012BC_DoubleMuParked_Muons.root inside the `composite_benchmarks` directory:

### Julia
```
~/UnROOT_RDataFrame_MiniBenchmark/composite_benchmarks $ julia -t 4 ./UnROOT_loop.jl 
  Activating project at `~/UnROOT_RDataFrame_MiniBenchmark/composite_benchmarks`
[ Info: 1st run
 15.836218 seconds (20.07 M allocations: 16.459 GiB, 3.12% gc time, 4.29% compilation time)
[ Info: 2nd run
 14.876444 seconds (17.40 M allocations: 16.328 GiB, 1.71% gc time)
[ Info: 4-threads 1st run
  5.997804 seconds (18.28 M allocations: 16.973 GiB, 8.68% gc time, 3.81% compilation time)
[ Info: 4-threads 2nd run
  5.102895 seconds (17.40 M allocations: 16.934 GiB, 1.74% gc time)
```

### PyROOT RDataFrame
```
~/UnROOT_RDataFrame_MiniBenchmark/composite_benchmarks $ python PyROOT_RDataFrame.py 
43.55262041091919 seconds
```

### ROOT RDataFrame (g++ compiled)
```
g++ RDataFrame_benchmark_compiled_single.cpp -O2 `root-config --cflags --glibs` -o 
run; ./run
Real time 0:00:25.038, CP time 25.040

~/UnROOT_RDataFrame_MiniBenchmark/composite_benchmarks $ g++ RDataFrame_benchmark_compiled_MT.cpp -O2 `root-config --cflags --glibs` -o run;
 ./run
Real time 0:00:16.099, CP time 51.210
```

### ROOT for-loop (g++ compiled)
```
~/UnROOT_RDataFrame_MiniBenchmark/composite_benchmarks $ g++ ROOT_loop_compiled.cpp -O2 `root-config --cflags --glibs` -o run; ./run
Real time 0:00:33.321, CP time 33.320
```
