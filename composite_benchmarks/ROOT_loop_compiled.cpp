#include "df103_NanoAODHiggsAnalysis_python.h"
#include "TFile.h"
#include <ROOT/RVec.hxx>
#include "TStopwatch.h"
#include <iostream>
#include "TTreePerfStats.h"

int main() {
    TFile *f = TFile::Open("./Run2012BC_DoubleMuParked_Muons.root");
    TTree *t;
    f->GetObject("Events",t);

    TH1D *hist = new TH1D("hist","hist",36,70,180);

    constexpr size_t N = 4;
    UInt_t nMuon;
    Float_t pMuon_pt[N];
    Float_t pMuon_eta[N];
    Float_t pMuon_phi[N];
    Float_t pMuon_mass[N];
    Int_t pMuon_charge[N];

    TBranch *b_nMuon = t->GetBranch("nMuon");
    TBranch *b_Muon_pt = t->GetBranch("Muon_pt");
    TBranch *b_Muon_eta = t->GetBranch("Muon_eta");
    TBranch *b_Muon_phi = t->GetBranch("Muon_phi");
    TBranch *b_Muon_mass = t->GetBranch("Muon_mass");
    TBranch *b_Muon_charge = t->GetBranch("Muon_charge");

    b_nMuon->SetAddress(&nMuon);
    b_Muon_pt->SetAddress(&pMuon_pt);
    b_Muon_eta->SetAddress(&pMuon_eta);
    b_Muon_phi->SetAddress(&pMuon_phi);
    b_Muon_mass->SetAddress(&pMuon_mass);
    b_Muon_charge->SetAddress(&pMuon_charge);

    // If you want additional performance printouts
    // TTreePerfStats *ps = new TTreePerfStats("ioperf",t);

    TStopwatch timer;
    Int_t noe = t->GetEntries();
    timer.Start();
    for(Int_t entry = 0; entry != noe; ++entry ){
        b_nMuon->      GetEntry(entry);
        if(nMuon != 4)
            continue;

        b_Muon_charge->GetEntry(entry);
        RVec<Int_t> Muon_charge(pMuon_charge, nMuon);

        if(Sum(Muon_charge) != 0)
            continue;

        b_Muon_pt->    GetEntry(entry);
        b_Muon_eta->   GetEntry(entry);
        RVec<Float_t> Muon_pt(pMuon_pt, nMuon);
        RVec<Float_t> Muon_eta(pMuon_eta, nMuon);

        if(!( All(Muon_pt > 5) && All(abs(Muon_eta < 2.4)) ))
            continue;

        b_Muon_phi->   GetEntry(entry);
        b_Muon_mass->  GetEntry(entry);
        RVec<Float_t> Muon_phi(pMuon_phi, nMuon);
        RVec<Float_t> Muon_mass(pMuon_mass, nMuon);
        auto Z_idx = reco_zz_to_4l(Muon_pt, Muon_eta, Muon_phi, Muon_mass, Muon_charge);

        if (!filter_z_dr(Z_idx, Muon_eta, Muon_phi))
            continue;

        auto zmass = compute_z_masses_4l(Z_idx, Muon_pt, Muon_eta, Muon_phi, Muon_mass);

        if(zmass[0] <= 40 || zmass[0] >= 120)
            continue;

        if(zmass[1] <= 12 || zmass[1] >= 120)
            continue;

        auto H_mass = compute_higgs_mass_4l(Z_idx, Muon_pt, Muon_eta, Muon_phi, Muon_mass);
        hist->Fill(H_mass);

    }
    timer.Stop();
    timer.Print("m");
    std::cout<<hist->Integral()<<std::endl;
    std::cout<<hist->GetBinContent(0)<<std::endl;
    std::cout<<hist->GetBinContent(1)<<std::endl;
    return 0;
}
