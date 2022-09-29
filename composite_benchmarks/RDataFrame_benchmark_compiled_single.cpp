#include "df103_NanoAODHiggsAnalysis_python.h"
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RVec.hxx>
#include <TCanvas.h>
#include <TStopwatch.h>

TH1D RDataFrame_benchmark_compiled_single() {
  auto df_bkg_4mu = ROOT::RDataFrame("Events", "Run2012BC_DoubleMuParked_Muons.root");

  // Filter interesting events
  auto df_ge4m = df_bkg_4mu.Filter([](unsigned n) { return n == 4; }, {"nMuon"}, "Exactly four muons");

  auto df_kin = df_ge4m.Filter([](ROOT::RVec<float> &pt, ROOT::RVec<float> &eta) { return All(pt > 5) && All(abs(eta) < 2.4); },
      {"Muon_pt", "Muon_eta"}, "Good muon kinematics");
  auto df_base = df_kin.Filter([](ROOT::RVec<int> &ch) { return Sum(ch) == 0; }, {"Muon_charge"}, "Two positive and two negative muons");

  // Reconstruct Z systems
  auto df_z_idx = df_base.Define("Z_idx", reco_zz_to_4l, {"Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass", "Muon_charge"});

  // Cut on distance between muons building Z systems
  auto df_z_dr = df_z_idx.Filter(filter_z_dr, {"Z_idx", "Muon_eta", "Muon_phi"}, "Delta R separation of muons building Z system");

  // Compute masses of Z systems
  auto df_z_mass = df_z_dr.Define("Z_mass", compute_z_masses_4l, {"Z_idx", "Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass"});

  // Cut on mass of Z candidates
  auto df_z1_cut = df_z_mass.Filter(
      [](ROOT::RVec<float> &zmass) { return zmass[0] > 40 && zmass[0] < 120; },
      {"Z_mass"}, "Mass of first Z candidate in [40, 120]");
  auto df_z_cut = df_z1_cut.Filter(
      [](ROOT::RVec<float> &zmass) { return zmass[1] > 12 && zmass[1] < 120; },
      {"Z_mass"}, "Mass of second Z candidate in [12, 120]");

  // Reconstruct H mass
  auto df_bkg_4mu_reco = df_z_cut.Define("H_mass", compute_higgs_mass_4l, {"Z_idx", "Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass"});

  auto df_h_bkg_4mu = df_bkg_4mu_reco.Histo1D<float>({"h_bkg_4mu", "", 36, 70, 180}, "H_mass");

  TStopwatch sw;
  sw.Start();
  TH1D h = df_h_bkg_4mu.GetValue();
  sw.Stop();
  sw.Print("m");
  std::cout<<h.Integral()<<std::endl;
  std::cout<<h.GetBinContent(0)<<std::endl;
  std::cout<<h.GetBinContent(1)<<std::endl;
  return h;
}

int main(){
    RDataFrame_benchmark_compiled_single();
    return 0;
}
