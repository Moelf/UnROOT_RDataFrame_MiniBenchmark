# How-to

Putting the `.root` from http://opendata.web.cern.ch/record/12341/files/Run2012BC_DoubleMuParked_Muons.root inside the `composite_benchmarks` directory:

### Julia
```
[00:34] login01.af.uchicago.edu:~/UnROOT_RDataFrame_MiniBenchmark/composite_benchmarks $ julia -t 4 --project=. --startup-file=no ./UnROOT_loop.jl 
  Activating project at `~/UnROOT_RDataFrame_MiniBenchmark/composite_benchmarks`
[ Info: 1st run
 15.991890 seconds (20.06 M allocations: 16.459 GiB, 4.23% gc time, 4.09% compilation time)
[ Info: 2nd run
 15.053849 seconds (17.40 M allocations: 16.328 GiB, 3.06% gc time)
[ Info: 4-threads 1st run
  4.724717 seconds (18.31 M allocations: 16.974 GiB, 6.77% gc time, 4.61% compilation time)
[ Info: 4-threads 2nd run
  4.612587 seconds (17.40 M allocations: 16.934 GiB, 9.48% gc time)
```

### PyROOT RDataFrame
```
~/UnROOT_RDataFrame_MiniBenchmark/composite_benchmarks $ python PyROOT_RDataFrame.py 
43.55262041091919 seconds
```

### ROOT RDataFrame (g++ compiled)
```
[00:36] login01.af.uchicago.edu:~/UnROOT_RDataFrame_MiniBenchmark/composite_benchmarks $ g++ RDataFrame_benchmark_compiled
_single.cpp -O2 `root-config --cflags --glibs` -o run; ./run
Warning in <UnknownClass::SetDisplay>: DISPLAY not set, setting it to c-24-61-185-63.hsd1.ma.comcast.net:0.0
./runReal time 0:00:24.940, CP time 24.940
65807
9809
3991

[00:39] login01.af.uchicago.edu:~/UnROOT_RDataFrame_MiniBenchmark/composite_benchmarks $ g++ RDataFrame_benchmark_compiled
_MT.cpp -O2 `root-config --cflags --glibs` -o run; ./run
Warning in <UnknownClass::SetDisplay>: DISPLAY not set, setting it to c-24-61-185-63.hsd1.ma.comcast.net:0.0
Real time 0:00:10.237, CP time 39.850
65807
9809
3991
```

### ROOT for-loop (g++ compiled)
```
[00:36] login01.af.uchicago.edu:~/UnROOT_RDataFrame_MiniBenchmark/composite_benchmarks $ g++ ROOT_loop_compiled.cpp -O2 `root-config --cflags --glibs` -o run; ./run
Warning in <UnknownClass::SetDisplay>: DISPLAY not set, setting it to c-24-61-185-63.hsd1.ma.comcast.net:0.0
Real time 0:00:19.677, CP time 19.690
67537
9942
4046
```
