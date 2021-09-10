import awkward1 as ak
import time
import uproot # uproot4

f = uproot.open("uncompressed_Run2012BC_DoubleMuParked_Muons.root")
t = f["Events"]

t0 = time.time()
print(ak.sum(t["Muon_pt"].array()))
t1 = time.time()
print(t1-t0)
