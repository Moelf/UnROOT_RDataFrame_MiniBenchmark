# Benchmark Summary

## Results:
### Single-threaded (cold run, first Julia, then RDF):
- [Julia](https://nbviewer.jupyter.org/github/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/master/UnROOT_benchmark.ipynb): 22.3 s
- [ROOT](https://nbviewer.jupyter.org/github/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/master/RDataFrame_benchmark.ipynb): 41.2 s

### 4 threads (same condition):
- Julia: 9.6 s
- ROOT: 11.8 s

(all heavy lifting functions used by RDataFrame are written in C++ inside [header file](https://github.com/Moelf/UnROOT_RDataFrame_MiniBenchmark/blob/master/df103_NanoAODHiggsAnalysis_python.h))

## Hardware:
- AMD Ryzen 9 3900X (24) @ 3.800GHz
- Samsung SSD 860 NVMe
- 64GB Main Memory

```julia
(JuliaHEPBenchmarkRDF) pkg> st
     Project JuliaHEPBenchmarkRDF v0.1.0
      Status `~/Documents/github/JuliaHEPBenchmarkRDF/Project.toml`
  [68837c9b] FHist v0.6.0
  [7073ff75] IJulia v1.23.2
  [3a55db76] LVCyl v0.1.0 `https://github.com/JuliaHEP/LVCyl.jl#master`
  [3cd96dde] UnROOT v0.4.0
```
## Physics Task:
Make a histogram of 4-lepton invariant mass (higgs candidate in real analysis)

## Data:
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

## Procedure
minimized based on: https://root.cern/doc/master/df103__NanoAODHiggsAnalysis_8py.html
1. `nMuon == 4`.
2. All muon has `pt > 5` and `abs(eta) < 2.4`.
3. Sum of `Muon_charge` equals 0.
4. Get the `Z_idx = [[idx1, idx2], [idx3, idx4]]` which represent two pairs of Z-candidate muons.
5. Each Z-candidate muon pair must **not** have `dR < 0.02`.
6. Compute the two Z bosons' masses and require first to be between 40,120 GeV, the second between 12,120 GeV.
7. Compute Higgs' mass and fill Histogram.
