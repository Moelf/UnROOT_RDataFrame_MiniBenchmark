using UnROOT

const t_lz4 = LazyTree(ROOTFile("lz4_Run2012BC_DoubleMuParked_Muons.root"),"Events");
function foo(t)
    s = 0.0f0
    for evt in t
        s += sum(evt.Muon_pt)
    end
    s
end

@time foo(t_lz4)
