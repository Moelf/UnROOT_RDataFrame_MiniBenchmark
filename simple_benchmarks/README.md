
### Simple benchmark

Compute the sum of floats in a single jagged branch over many events.
Compare julia, C++, and Python using files with no compression, ZLIB, LZ4, and LZMA.
The short scripts are shown below.

### Tree information

[Open data DoubleMuon dataset](http://opendata.web.cern.ch/record/12341): ~62M events, 150M muons, ~2GB with zlib

Converted to uncompressed, LZ4, LZMA versions with

```bash
hadd -O -f0 uncompressed_Run2012BC_DoubleMuParked_Muons.root Run2012BC_DoubleMuParked_Muons.root
hadd -O -f404 lz4_Run2012BC_DoubleMuParked_Muons.root Run2012BC_DoubleMuParked_Muons.root
hadd -O -f209 lzma_Run2012BC_DoubleMuParked_Muons.root Run2012BC_DoubleMuParked_Muons.root
```

### julia (`UnROOT`)

```julia
julia> using UnROOT

julia> const t_lz4 = LazyTree(ROOTFile("lz4_Run2012BC_DoubleMuParked_Muons.root"),"Events");

julia> function foo(t)
    s = 0.0f0
    for evt in t
        s += sum(evt.Muon_pt)
    end
    s
end

julia> @time foo(t_lz4)
```

### C++ (ROOT with `TTreeReader`)

```cpp
#include "TFile.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TStopwatch.h"
#include <iostream>

void simple() {
   TFile *f = TFile::Open("uncompressed_Run2012BC_DoubleMuParked_Muons.root");
   TTreeReader reader("Events", f);
   TTreeReaderArray<float> Muon_pt(reader, "Muon_pt");
   TStopwatch timer;
   timer.Start();
   float s = 0.0;
   while (reader.Next()) {
       for (size_t i = 0; i < Muon_pt.GetSize(); i++) {
           s += Muon_pt[i];
       }
   }
   timer.Stop();
   timer.Print();
   std::cout << s << std::endl;
}
```

Run with `root -b -q -l -n simple.C` (interpreted) or `root -b -q -l -n simple.C+` (compiled)

Note that changing `void simple()` to `int main()` and then compiling with 
```bash
time g++ simple.cpp -O2 `root-config --cflags --glibs` && ./a.out
```
gives the same times as running with `root simple.C+`

### C++ (ROOT with `SetAddress`)

```cpp
#include "TFile.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TStopwatch.h"
#include <iostream>

void simple() {
   TFile *f = TFile::Open("../uncompressed_Run2012BC_DoubleMuParked_Muons.root");
   TTree *tree = (TTree*)(f->Get("Events"));
   tree->SetBranchStatus("*", 0);           // limit read branches
   tree->SetBranchStatus("nMuon", 1);       // though I didn't notice a difference
   tree->SetBranchStatus("Muon_pt", 1);     // ...but just in case
   float Muon_pt_[30];
   TBranch *b_Muon_pt_ = tree->GetBranch("Muon_pt");
   b_Muon_pt_->SetAddress(&Muon_pt_);
   float s = 0.0;

   TStopwatch timer;
   timer.Start();
   for (unsigned int ievt=0; ievt<tree->GetEntries(); ievt++) {
       size_t nbytes = b_Muon_pt_->GetEntry(ievt);
       size_t nmu = nbytes/sizeof(Muon_pt_[0]);
       for (size_t imu=0; imu<nmu; imu++) {
           s += Muon_pt_[imu];
       }
   }
   timer.Stop();
   timer.Print();
   std::cout << s << std::endl;
}
```

Run with`root -b -q -l -n simple.C+` (compiled)

### C++ (ROOT with `RDataFrame`)

```cpp
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RVec.hxx>
#include <TFile.h>
#include <TStopwatch.h>
int main() {
   // ROOT::EnableImplicitMT();
   auto df = ROOT::RDataFrame("Events", "uncompressed_Run2012BC_DoubleMuParked_Muons.root");
   auto out = (df
           .Define("sumpt", [](ROOT::RVec<float> &pts) { return Sum(pts); }, {"Muon_pt"})
           .Sum("sumpt")
           );
  TStopwatch sw;
  sw.Start();
  std::cout << out.GetValue() << std::endl;
  sw.Stop();
  sw.Print("m");
}
```

Compiled with 

```bash
time g++ simple.cpp -O2 `root-config --cflags --glibs`
```

### Python (`uproot`)

```python
import awkward1 as ak
import time
import uproot

f = uproot.open("Run2012BC_DoubleMuParked_Muons.root")
t = f["Events"]

t0 = time.time()
print(ak.sum(t["Muon_pt"].array()))
t1 = time.time()
print(t1-t0)
```

### Results

Times are in seconds and were collected warm/after a couple of sequential runs. Variation of roughly 5-10% on the timings.

|      | julia  | C++ `TTreeReader` interpreted | C++ `TTreeReader` compiled | C++ `SetAddress` compiled | C++ `RDataFrame` compiled | uproot |
| ---- | ------ | ----------------------------- | -------------------------- | ------------------------- | ------------------------- | ------ |
| none | 2.084  | 11.200                        | 7.260                      | 3.500                     | 9.150                     | 1.958  |
| zlib | 7.075  | 15.560                        | 11.730                     | 8.110                     | 13.640                    | 6.569  |
| lz4  | 3.056  | 11.300                        | 7.620                      | 3.740                     | 9.498                     | 2.797  |
| lzma | 44.718 | 52.660                        | 49.550                     | 45.520                    | 51.655                    | 42.148 |

