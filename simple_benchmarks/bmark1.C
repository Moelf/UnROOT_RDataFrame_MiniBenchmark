#include "TFile.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TStopwatch.h"
#include <iostream>
#include "TTreePerfStats.h"
 
void simple() {
   TFile *f = TFile::Open("uncompressed_Run2012BC_DoubleMuParked_Muons.root");
   TTree *t = (TTree*)(f->Get("Events"));
   TTreeReader reader(t);
   TTreeReaderArray<float> Muon_pt(reader, "Muon_pt");

   // If you want additional performance printouts
   // TTreePerfStats *ps = new TTreePerfStats("ioperf",t);
 
   TStopwatch timer;
   timer.Start();
   float s = 0.0;
   while (reader.Next()) {
       for (size_t i = 0; i < Muon_pt.GetSize(); i++) {
           s += Muon_pt[i];
       }
   }
   timer.Stop();
   timer.Print();
   std::cout << s << std::endl;

   // TCanvas* c1 = new TCanvas();
   // ps->Draw();
   // c1->SaveAs("test.png");
   // ps->Print("unzip");

}
