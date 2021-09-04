## Benchmark Summary
### Hardware:
- AMD Ryzen 9 3900X (24) @ 3.800GHz
- Samsung SSD 860 NVMe

```julia
(JuliaHEPBenchmarkRDF) pkg> st
     Project JuliaHEPBenchmarkRDF v0.1.0
      Status `~/Documents/github/JuliaHEPBenchmarkRDF/Project.toml`
  [68837c9b] FHist v0.6.0
  [7073ff75] IJulia v1.23.2
  [3a55db76] LVCyl v0.1.0 `https://github.com/JuliaHEP/LVCyl.jl#master`
  [3cd96dde] UnROOT v0.4.0
```

## Results:

### Single-threaded (cold run, first Julia, then RDF):
- Julia: 22.3 s
- ROOT: 41.2 s

### 4 threads (same condition):
- Julia: 9.6 s
- ROOT: 11.8 s
