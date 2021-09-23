const z_mass = 91.2
const LV32 = LorentzVectorCyl{Float32}
function reco_zz_to_4l(pts, etas, phis, masses, charges)
    # Find first lepton pair with invariant mass closest to Z mass
    best_mass = -Inf
    best_i1 = best_i2 = 1
    for i1 in eachindex(pts), i2 in (i1 + 1):lastindex(pts)
        charges[i1] == charges[i2] && continue
        p1 = LV32(pts[i1], etas[i1], phis[i1], masses[i1])
        p2 = LV32(pts[i2], etas[i2], phis[i2], masses[i2])
        this_mass = (p1 + p2).mass
        if (abs(z_mass - this_mass) < abs(z_mass - best_mass))
            best_mass = this_mass
            best_i1 = i1
            best_i2 = i2
        end
    end
    idx = [[best_i1, best_i2], Int[]]

    # Reconstruct second Z from remaining lepton pair
    for i in 1:4
        if (i != best_i1 && i != best_i2)
            push!(idx[2], i)
        end
    end

    # Return indices of the pairs building two Z bosons
    return idx
end

function filter_z_dr(idx, etas, phis)
    for pair in idx
        i1, i2 = pair
        dr = sqrt((etas[i1] - etas[i2])^2 + (phis[i1] - phis[i2])^2)
        dr < 0.02 && return false;
    end
    return true
end

function filter_z_candidates(Z_mass)
    40 < Z_mass[1] < 120 || return false
    12 < Z_mass[2] < 120 || return false
    return true
end

function compute_z_masses_4l(idx, pts, etas, phis, masses)
    z_masses = zeros(2)
    for (i, pair) in enumerate(idx)
        i1, i2 = pair
        p1 = LV32(pts[i1], etas[i1], phis[i1], masses[i1])
        p2 = LV32(pts[i2], etas[i2], phis[i2], masses[i2])
        z_masses[i] = (p1 + p2).mass
    end
    return abs(z_masses[1] - z_mass) < abs(z_masses[2] - z_mass) ?
      z_masses : reverse!(z_masses);
end

function compute_higgs_mass_4l(idx, pts, etas, phis, masses)
    ((i1, i2), (i3, i4)) = idx
    p1 = LV32(pts[i1], etas[i1], phis[i1], masses[i1]);
    p2 = LV32(pts[i2], etas[i2], phis[i2], masses[i2]);
    p3 = LV32(pts[i3], etas[i3], phis[i3], masses[i3]);
    p4 = LV32(pts[i4], etas[i4], phis[i4], masses[i4]);
    return (p1 + p2 + p3 + p4).mass
end
