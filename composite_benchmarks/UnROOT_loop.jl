@assert Threads.nthreads() == 4

using Pkg
Pkg.activate(@__DIR__)
Pkg.instantiate()

using UnROOT, FHist, LorentzVectorHEP
include("./UnROOT_benchmark_utils.jl")

_t() = LazyTree(ROOTFile("./Run2012BC_DoubleMuParked_Muons.root"), "Events")
function main(t)
    H = Hist1D(Float64; bins = range(70, 180; length=36), overflow=true)
    @time for evt in t
        evt.nMuon != 4 && continue

        pts, etas  = evt.Muon_pt, evt.Muon_eta

        (all(pts .> 5) && all(abs.(etas) .< 2.4)) || continue

        phis, masses, charges = evt.Muon_phi, evt.Muon_mass, evt.Muon_charge

        sum(charges) != 0 && continue

        Z_idx = reco_zz_to_4l(pts, etas, phis, masses, charges)

        filter_z_dr(Z_idx, etas, phis) || continue

        Z_mass = compute_z_masses_4l(Z_idx, pts, etas, phis, masses)

        filter_z_candidates(Z_mass) || continue
        h_mass = compute_higgs_mass_4l(Z_idx, pts, etas, phis, masses)
        push!(H, h_mass)
    end
    H
end

@info "1st run"
main(_t())
@info "2nd run"
main(_t())

function MultiThread_main(t)
    H = Hist1D(Float64; bins = range(70, 180; length=37), overflow=true)
    @time Threads.@threads for evt in t
        evt.nMuon != 4 && continue

        pts, etas  = evt.Muon_pt, evt.Muon_eta

        (all(pts .> 5) && all(abs.(etas) .< 2.4)) || continue

        phis, masses, charges = evt.Muon_phi, evt.Muon_mass, evt.Muon_charge

        sum(charges) != 0 && continue

        Z_idx = reco_zz_to_4l(pts, etas, phis, masses, charges)

        filter_z_dr(Z_idx, etas, phis) || continue

        Z_mass = compute_z_masses_4l(Z_idx, pts, etas, phis, masses)

        filter_z_candidates(Z_mass) || continue
        h_mass = compute_higgs_mass_4l(Z_idx, pts, etas, phis, masses)
        atomic_push!(H, h_mass)
    end
    H
end

@info "4-threads 1st run"
MultiThread_main(_t())
@info "4-threads 2nd run"
MultiThread_main(_t())
