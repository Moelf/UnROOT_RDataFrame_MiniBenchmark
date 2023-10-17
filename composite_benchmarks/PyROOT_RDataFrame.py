import ROOT, time

ROOT.gInterpreter.Declare('#include "df103_NanoAODHiggsAnalysis_python.h"')

def selection_4mu(df):
    """Select interesting events with four muons"""
    df_ge4m = df.Filter("nMuon==4", "Exactly four muons")
 
    df_kin = df_ge4m.Filter("All(Muon_pt>5) && All(abs(Muon_eta)<2.4)", "Good muon kinematics")
    df_2p2n = df_kin.Filter("Sum(Muon_charge)==0",
                           "Two positive and two negative muons")
 
    return df_2p2n

def filter_z_candidates(df):
    """Apply selection on reconstructed Z candidates"""
    df_z1_cut = df.Filter("Z_mass[0] > 40 && Z_mass[0] < 120", "Mass of first Z candidate in [40, 120]")
    df_z2_cut = df_z1_cut.Filter("Z_mass[1] > 12 && Z_mass[1] < 120", "Mass of second Z candidate in [12, 120]")
    return df_z2_cut

def reco_higgs_to_4mu(df):
    """Reconstruct Higgs from four muons"""
    # Filter interesting events
    df_base = selection_4mu(df)
 
    # Reconstruct Z systems
    df_z_idx = df_base.Define("Z_idx", "reco_zz_to_4l(Muon_pt, Muon_eta, Muon_phi, Muon_mass, Muon_charge)")
 
    # Cut on distance between muons building Z systems
    df_z_dr = df_z_idx.Filter("filter_z_dr(Z_idx, Muon_eta, Muon_phi)", "Delta R separation of muons building Z system")
 
    # Compute masses of Z systems
    df_z_mass = df_z_dr.Define("Z_mass", "compute_z_masses_4l(Z_idx, Muon_pt, Muon_eta, Muon_phi, Muon_mass)")
 
    # Cut on mass of Z candidates
    df_z_cut = filter_z_candidates(df_z_mass)
 
    # Reconstruct H mass
    df_h_mass = df_z_cut.Define("H_mass", "compute_higgs_mass_4l(Z_idx, Muon_pt, Muon_eta, Muon_phi, Muon_mass)")
 
    return df_h_mass

df_bkg_4mu = ROOT.RDataFrame("Events", "./Run2012BC_DoubleMuParked_Muons.root")
df_bkg_4mu_reco = reco_higgs_to_4mu(df_bkg_4mu)
df_h_bkg_4mu = df_bkg_4mu_reco.Histo1D(("h_bkg_4mu", "", 36, 70, 180), "H_mass")
start_time = time.time()
df_h_bkg_4mu.GetValue()
print("1-thread:  %s seconds" % (time.time() - start_time))

ROOT.ROOT.EnableImplicitMT(4)
df_bkg_4mu = ROOT.RDataFrame("Events", "./Run2012BC_DoubleMuParked_Muons.root")
df_bkg_4mu_reco = reco_higgs_to_4mu(df_bkg_4mu)
df_h_bkg_4mu = df_bkg_4mu_reco.Histo1D(("h_bkg_4mu", "", 36, 70, 180), "H_mass")
start_time = time.time()
df_h_bkg_4mu.GetValue()
print("4-threads: %s seconds" % (time.time() - start_time))
