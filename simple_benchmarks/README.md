
### Simple benchmark

Compute the sum of floats in a single jagged branch over many events.
Compare julia, C++, and Python an files with no compression, ZLIB, LZ4, and LZMA.
The scripts are pasted below and included in this directory as well.

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

Numbers are warm/after a couple of sequential runs.

|              | julia (s) | ROOT interpreted (s) | ROOT compiled (s) | uproot (s) |
| ------------ | --------- | -------------------- | ----------------- | ---------- |
| uncompressed | 2.585     | 11.200               | 7.260             | 2.062      |
| zlib         | 7.634     | 15.560               | 11.730            | 6.789      |
| lzma         | 45.045    | 52.660               | 49.550            | 43.612     |
| lz4          | 3.987     | 11.300               | 7.620             | 2.917      |

