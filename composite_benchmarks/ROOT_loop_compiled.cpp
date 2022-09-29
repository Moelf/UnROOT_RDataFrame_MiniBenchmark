#include "df103_NanoAODHiggsAnalysis_python.h"
#include "TFile.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TStopwatch.h"
#include <iostream>
#include "TTreePerfStats.h"

int main() {
    TFile *f = TFile::Open("./Run2012BC_DoubleMuParked_Muons.root");
    TTree *t = (TTree*)(f->Get("Events"));
    TH1F *hist = new TH1F("hist","hist",36,70,180);
    Int_t nMuon;
    t->Branch("nMuon",&nMuon);

    RVec<Float_t> Muon_pt, Muon_eta, Muon_phi, Muon_mass;
    t->SetBranchAddress("Muon_pt",  &Muon_pt);
    t->SetBranchAddress("Muon_eta", &Muon_eta);
    t->SetBranchAddress("Muon_phi", &Muon_phi);
    t->SetBranchAddress("Muon_mass",&Muon_mass);

    RVec<Int_t> Muon_charge;
    t->Branch("Muon_charge",&Muon_charge);

    // If you want additional performance printouts
    // TTreePerfStats *ps = new TTreePerfStats("ioperf",t);

    TStopwatch timer;
    Float_t s = 0.0;
    Int_t noe = t->GetEntriesFast();
    timer.Start();
    for(Int_t entry = 0; entry != noe; ++entry ){
        t->GetEntry(entry);
        if(nMuon != 4)
            continue;
        if(!( All(Muon_pt > 5) && All(abs(Muon_eta < 2.4)) ))
            continue;

        auto Z_idx = reco_zz_to_4l(Muon_pt, Muon_eta, Muon_phi, Muon_mass, Muon_charge);
        if (!(filter_z_dr(Z_idx, Muon_eta, Muon_phi)))
            continue;

        auto zmass = compute_z_masses_4l(Z_idx, Muon_pt, Muon_eta, Muon_phi, Muon_mass);

        if(zmass[0] < 40 || zmass[0] > 120)
            continue;
        if(zmass[1] < 12 || zmass[1] > 120)
            continue;

        auto H_mass = compute_higgs_mass_4l(Z_idx, Muon_pt, Muon_eta, Muon_phi, Muon_mass);
        hist->Fill(H_mass);

    }
    timer.Stop();
    timer.Print("m");
    return 0;
}
