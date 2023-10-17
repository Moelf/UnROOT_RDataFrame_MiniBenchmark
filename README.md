# Benchmark Summary

See also: the [simple benchmark](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/tree/main/simple_benchmarks) for comparing mostly the read/interpretation speed between ROOT and Julia.

## Results:
### Single-thread composite benchmark
| Language | Run Time |
| -------- | -------- |
| [Julia](https://nbviewer.jupyter.org/github/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/UnROOT_benchmark.ipynb) | 15.48 s |
| [PyROOT RDF](https://nbviewer.jupyter.org/github/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/RDataFrame_benchmark.ipynb) | 40.22 s |
| [Compiled C++ ROOT Loop](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/tree/main/composite_benchmarks#root-rdataframe-g-compiled) | 19.96 s |
| [Compiled RDF](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/composite_benchmarks/RDataFrame_benchmark_compiled_single.cpp) | 24.97 s |

### 4-threads composite benchmark
| Language | Run Time |
| -------- | -------- |
| [Julia](https://nbviewer.jupyter.org/github/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/UnROOT_benchmark.ipynb) | 4.60 s |
| PyROOT RDF | 10.94 s |
| Compiled C++ ROOT Loop | Not impl. |
| [Compiled RDF](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/main/composite_benchmarks/RDataFrame_benchmark_compiled_MT.cpp) | 10.23 s |

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
minimized based on: https://root.cern/doc/main/df103__NanoAODHiggsAnalysis_8py.html
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
ROOT Version: 6.28/09

julia> versioninfo()
Julia Version 1.9.2
Commit e4ee485e909 (2023-07-05 09:39 UTC)
Platform Info:
  OS: Linux (x86_64-linux-gnu)
  CPU: 96 × AMD EPYC 7402 24-Core Processor
  WORD_SIZE: 64
  LIBM: libopenlibm
  LLVM: libLLVM-14.0.6 (ORCJIT, znver2)
  Threads: 4 on 96 virtual cores
```

```julia
(composite_benchmarks) pkg> st
Status `~/UnROOT_RDataFrame_MiniBenchmark/composite_benchmarks/Project.toml`
  [68837c9b] FHist v0.10.2
  [f612022c] LorentzVectorHEP v0.1.6
  [3cd96dde] UnROOT v0.10.18
```
