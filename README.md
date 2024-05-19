# Benchmark Summary

See also: the [simple benchmark](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/tree/main/simple_benchmarks) for comparing mostly the read/interpretation speed between ROOT and Julia.

## Results:
### Single-thread composite benchmark
| Language | Run Time |
| -------- | -------- |
| [Julia](https://nbviewer.jupyter.org/github/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/UnROOT_benchmark.ipynb) | 19.67 s |
| [PyROOT RDF](https://nbviewer.jupyter.org/github/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/RDataFrame_benchmark.ipynb) | 38.97 s |
| [Compiled RDF](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/composite_benchmarks/RDataFrame_benchmark_compiled_single.cpp) | 25.04 s |
| [Compiled C++ ROOT Loop](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/tree/main/composite_benchmarks#root-rdataframe-g-compiled) | 19.63 s |
| [Python Uproot loop-less](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/tree/main/composite_benchmarks#uproot-loop-less) | 29.13 s |


### 4-threads composite benchmark
| Language | Run Time |
| -------- | -------- |
| [Julia](https://nbviewer.jupyter.org/github/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/UnROOT_benchmark.ipynb) | 5.25 s |
| PyROOT RDF | 11.01 s |
| [Compiled RDF](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/composite_benchmarks/RDataFrame_benchmark_compiled_MT.cpp) | 8.67 s |
| Compiled C++ ROOT Loop | Not impl. |
| Python Uproot loop-less | Not impl. |

For more timing (compile, JIT etc.), navigate to [composite_benchmark](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/tree/main/composite_benchmarks) directory.

(all heavy lifting functions used by RDataFrame are written in C++ inside [header file](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/composite_benchmarks/df103_NanoAODHiggsAnalysis_python.h))

## Physics Task:
Make a histogram of 4-lepton invariant mass (higgs candidate in real analysis)

### Data:
http://opendata.web.cern.ch/record/12341
```julia
julia> mytree
 Row │ nMuon   Muon_pt           Muon_eta          Muon_phi          Muon_mass         Muon_charge     
     │ UInt32  SubArray{Float3   SubArray{Float3   SubArray{Float3   SubArray{Float3   SubArray{Int32, 
─────┼─────────────────────────────────────────────────────────────────────────────────────────────────
 1   │ 2       Float32[10.76369  Float32[1.066827  Float32[-0.03427  Float32[0.105658  Int32[-1, -1]
 2   │ 2       Float32[10.53849  Float32[-0.42778  Float32[-0.27479  Float32[0.105658  Int32[1, -1]
 3   │ 1       Float32[3.275326  Float32[2.210855  Float32[-1.22341  Float32[0.105658  Int32[1]
```

### Steps
minimized based on: https://root.cern/doc/v632/df103__NanoAODHiggsAnalysis_8py.html
1. `nMuon == 4`.
2. All muon has `pt > 5` and `abs(eta) < 2.4`.
3. Sum of `Muon_charge` equals 0.
4. Get the `Z_idx = [[idx1, idx2], [idx3, idx4]]` which represent two pairs of Z-candidate muons.
5. Each Z-candidate muon pair must **not** have `dR < 0.02`.
6. Compute the two Z bosons' masses and require first to be between 40,120 GeV, the second between 12,120 GeV.
7. Compute Higgs' mass and fill Histogram.

## Specs and library versions:
```
$ root --version
ROOT Version: 6.30/05
Built for linuxx8664gcc on Mar 01 2024, 00:06:39
From heads/v6-30-00-patches@v6-30-04-28-g4462606


julia> versioninfo()
Julia Version 1.10.3
Commit 0b4590a5507 (2024-04-30 10:59 UTC)
Build Info:
  Official https://julialang.org/ release
Platform Info:
  OS: Linux (x86_64-linux-gnu)
  CPU: 96 × AMD EPYC 7402 24-Core Processor
  WORD_SIZE: 64
  LIBM: libopenlibm
  LLVM: libLLVM-15.0.7 (ORCJIT, znver2)
Threads: 4 default, 0 interactive, 2 GC (on 96 virtual cores)
```

```julia
(composite_benchmarks) pkg> st
Status `~/UnROOT_RDataFrame_MiniBenchmark/composite_benchmarks/Project.toml`
  [68837c9b] FHist v0.11.4
  [f612022c] LorentzVectorHEP v0.1.6
  [811555cd] ThreadPinning v0.7.22
  [3cd96dde] UnROOT v0.10.30
```
