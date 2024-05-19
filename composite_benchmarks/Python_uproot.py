import uproot as up
import awkward as ak
import hist
from hist import Hist
from coffea.nanoevents.methods import candidate

#ref: https://github.com/cmstas/ewkcoffea/blob/main/ewkcoffea/modules/selection_wwz.py#L309
def get_z_candidate_mask(lep_collection):

    # Attach the local index to the lepton objects
    lep_collection['idx'] = ak.local_index(lep_collection, axis=1)

    # Make all pairs of leptons
    ll_pairs = ak.combinations(lep_collection, 2, fields=["l0","l1"])
    ll_pairs_idx = ak.argcombinations(lep_collection, 2, fields=["l0","l1"])

    # Check each pair to see how far it is from the Z
    dist_from_z_all_pairs = abs((ll_pairs.l0+ll_pairs.l1).mass - 91.2)

    # Mask out the pairs that are not SFOS (so that we don't include them when finding the one that's closest to Z)
    # And then of the SFOS pairs, get the index of the one that's cosest to the Z
    sfos_mask = (ll_pairs.l0.charge == -ll_pairs.l1.charge)
    dist_from_z_sfos_pairs = ak.mask(dist_from_z_all_pairs,sfos_mask)
    sfos_pair_closest_to_z_idx = ak.argmin(dist_from_z_sfos_pairs,axis=-1,keepdims=True)

    # Construct a mask (of the shape of the original lep array) corresponding to the leps that are part of the Z candidate
    mask = (lep_collection.idx == ak.flatten(ll_pairs_idx.l0[sfos_pair_closest_to_z_idx]))
    mask = (mask | (lep_collection.idx == ak.flatten(ll_pairs_idx.l1[sfos_pair_closest_to_z_idx])))
    mask = ak.fill_none(mask, False)

    return mask

tree = up.open("./Run2012BC_DoubleMuParked_Muons.root:Events")

from datetime import datetime
h = Hist.new.Regular(36, 70, 180, name="m4l").Double()

startTime = datetime.now()

for a in tree.iterate(step_size=10**6):
    mask_g4m = a["nMuon"] == 4
    Muon_pt = a["Muon_pt"]
    mask_pt = ak.all(Muon_pt > 5, axis=1)
    Muon_eta = a["Muon_eta"]
    mask_eta = ak.all(abs(Muon_eta) < 2.4, axis=1)
    mask_charge = ak.sum(a["Muon_charge"], axis=1) == 0

    mask_kinematics = mask_g4m & mask_pt & mask_eta & mask_charge
    df1 = a[mask_kinematics]

    muons = ak.zip({"pt":     df1.Muon_pt,
                    "eta":    df1.Muon_eta,
                    "phi":    df1.Muon_phi,
                    "mass":   df1.Muon_mass,
                    "charge": df1.Muon_charge}, with_name="PtEtaPhiMCandidate", behavior=candidate.behavior)

    z_candidate_mask = get_z_candidate_mask(muons)

    muons_pair1 = muons[z_candidate_mask]
    muons_pair1_dR = muons_pair1[:, 0].delta_r(muons_pair1[:, 1])
    muons_pair1_mass = (muons_pair1[:, 0] + muons_pair1[:, 1]).mass

    mask_pair1_dR = muons_pair1_dR > 0.02
    mask_pair1_mass = (40 < muons_pair1_mass) & (muons_pair1_mass < 120)

    muons_pair2 = muons[~z_candidate_mask]
    muons_pair2_dR = muons_pair2[:, 0].delta_r(muons_pair2[:, 1])
    muons_pair2_mass = (muons_pair2[:, 0] + muons_pair2[:, 1]).mass

    mask_pair2_dR = muons_pair2_dR > 0.02
    mask_pair2_mass = (12 < muons_pair2_mass) & (muons_pair2_mass < 120)

    mask_muon = mask_pair1_dR & mask_pair1_mass & mask_pair2_dR & mask_pair2_mass

    df2 = muons[mask_muon]
    masses = (df2[:, 0] + df2[:, 1] + df2[:, 2] + df2[:, 3]).mass
    h.fill(masses)

print("1-thread:")
print(datetime.now() - startTime)
