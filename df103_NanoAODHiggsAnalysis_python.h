/// \file
/// \ingroup tutorial_dataframe
/// Header file with functions needed to execute the Python version
/// of the NanoAOD Higgs tutorial. The header is declared to the
/// ROOT C++ interpreter prior to the start of the analysis via the
/// `ROOT.gInterpreter.Declare()` function.
///
/// \date July 2019
/// \authors Stefan Wunsch (KIT, CERN), Vincenzo Eduardo Padulano (UniMiB, CERN)
 
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLatex.h"
#include "Math/Vector4D.h"
#include "TStyle.h"
 
using namespace ROOT::VecOps;
using RNode = ROOT::RDF::RNode;
using rvec_f = const RVec<float> &;
using rvec_i = const RVec<int> &;
const auto z_mass = 91.2;
 
// Reconstruct two Z candidates from four leptons of the same kind
RVec<RVec<size_t>> reco_zz_to_4l(rvec_f pt, rvec_f eta, rvec_f phi, rvec_f mass, rvec_i charge)
{
   RVec<RVec<size_t>> idx(2);
   idx[0].reserve(2); idx[1].reserve(2);
 
   // Find first lepton pair with invariant mass closest to Z mass
   auto idx_cmb = Combinations(pt, 2);
   auto best_mass = -9999;
   size_t best_i1 = 0; size_t best_i2 = 0;
   for (size_t i = 0; i < idx_cmb[0].size(); i++) {
      const auto i1 = idx_cmb[0][i];
      const auto i2 = idx_cmb[1][i];
      if (charge[i1] != charge[i2]) {
         ROOT::Math::PtEtaPhiMVector p1(pt[i1], eta[i1], phi[i1], mass[i1]);
         ROOT::Math::PtEtaPhiMVector p2(pt[i2], eta[i2], phi[i2], mass[i2]);
         const auto this_mass = (p1 + p2).M();
         if (std::abs(z_mass - this_mass) < std::abs(z_mass - best_mass)) {
            best_mass = this_mass;
            best_i1 = i1;
            best_i2 = i2;
         }
      }
   }
   idx[0].emplace_back(best_i1);
   idx[0].emplace_back(best_i2);
 
   // Reconstruct second Z from remaining lepton pair
   for (size_t i = 0; i < 4; i++) {
      if (i != best_i1 && i != best_i2) {
         idx[1].emplace_back(i);
      }
   }
 
   // Return indices of the pairs building two Z bosons
   return idx;
}
 
// Compute Z masses from four leptons of the same kind and sort ascending in distance to Z mass
RVec<float> compute_z_masses_4l(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f mass)
{
   RVec<float> z_masses(2);
   for (size_t i = 0; i < 2; i++) {
      const auto i1 = idx[i][0]; const auto i2 = idx[i][1];
      ROOT::Math::PtEtaPhiMVector p1(pt[i1], eta[i1], phi[i1], mass[i1]);
      ROOT::Math::PtEtaPhiMVector p2(pt[i2], eta[i2], phi[i2], mass[i2]);
      z_masses[i] = (p1 + p2).M();
   }
   if (std::abs(z_masses[0] - z_mass) < std::abs(z_masses[1] - z_mass)) {
      return z_masses;
   } else {
      return Reverse(z_masses);
   }
}
 
// Compute mass of Higgs from four leptons of the same kind
float compute_higgs_mass_4l(const RVec<RVec<size_t>> &idx, rvec_f pt, rvec_f eta, rvec_f phi, rvec_f mass)
{
   const auto i1 = idx[0][0]; const auto i2 = idx[0][1];
   const auto i3 = idx[1][0]; const auto i4 = idx[1][1];
   ROOT::Math::PtEtaPhiMVector p1(pt[i1], eta[i1], phi[i1], mass[i1]);
   ROOT::Math::PtEtaPhiMVector p2(pt[i2], eta[i2], phi[i2], mass[i2]);
   ROOT::Math::PtEtaPhiMVector p3(pt[i3], eta[i3], phi[i3], mass[i3]);
   ROOT::Math::PtEtaPhiMVector p4(pt[i4], eta[i4], phi[i4], mass[i4]);
   return (p1 + p2 + p3 + p4).M();
}
 
 
bool filter_z_dr(const RVec<RVec<size_t>> &idx, rvec_f eta, rvec_f phi)
{
   for (size_t i = 0; i < 2; i++) {
      const auto i1 = idx[i][0];
      const auto i2 = idx[i][1];
      const auto dr = DeltaR(eta[i1], eta[i2], phi[i1], phi[i2]);
      if (dr < 0.02) {
         return false;
      }
   }
   return true;
};