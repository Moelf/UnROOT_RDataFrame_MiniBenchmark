# How-to

Putting the `.root` from http://opendata.web.cern.ch/record/12341/files/Run2012BC_DoubleMuParked_Muons.root inside the `composite_benchmarks` directory:

### Julia
```
$> julia -t 4 ./UnROOT_loop.jl
  Activating project at `~/Documents/github/UnROOT_RDataFrame_MiniBenchmark/composite_benchmarks`
[ Info: 1st run
 20.581089 seconds (20.09 M allocations: 21.143 GiB, 14.27% gc time, 1.08% compilation time)
[ Info: 2nd run
 19.808003 seconds (17.41 M allocations: 21.007 GiB, 14.55% gc time)
[ Info: 4-threads 1st run
  5.462921 seconds (17.48 M allocations: 21.795 GiB, 4.64% gc time)
[ Info: 4-threads 2nd run
  5.065520 seconds (17.41 M allocations: 21.791 GiB, 1.20% gc time)
```

### PyROOT RDataFrame (interactive)
```
$> python PyROOT_RDataFrame.py 
40.20841431617737 seconds
```

### ROOT RDataFrame (g++ compiled)
```
$> g++ RDataFrame_benchmark_compiled_single.cpp -O2 `root-config --cflags --glibs` -o run; ./run
Real time 0:00:19.823, CP time 19.800
$> g++ RDataFrame_benchmark_compiled_MT.cpp -O2 `root-config --cflags --glibs` -o run; ./run
Real time 0:00:05.644, CP time 21.940
```

### ROOT for-loop (g++ compiled)
```
$> g++ ROOT_loop_compiled.cpp -O2 `root-config --cflags --glibs` -o run; ./run
Warning in <TStreamerInfo::Build>: Due to some major, backward-incompatible improvements planned for ROOT::RVec, direct I/O of ROOT::RVec objects will break between v6.24 and v6.26. Please use std::vectors instead. See the release notes of v6.24 for more information.
Warning in <TStreamerInfo::Build>: Due to some major, backward-incompatible improvements planned for ROOT::RVec, direct I/O of ROOT::RVec objects will break between v6.24 and v6.26. Please use std::vectors instead. See the release notes of v6.24 for more information.
Real time 0:00:28.157, CP time 28.120
```
