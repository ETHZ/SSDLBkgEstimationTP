#include "iostream"
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"
#include "FitBreitWigner.C"
#include "FitGauss.C"

void Fit_Z_InvMass(){

//Getting histo
TFile* file = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/Z_InvMass.root");

TH1D* histo[4];

histo[0] = (TH1D*)file->Get("histo_GenInvMass_e");
histo[1] = (TH1D*)file->Get("histo_RecInvMass_e");
histo[2] = (TH1D*)file->Get("histo_GenInvMass_mu");
histo[3] = (TH1D*)file->Get("histo_RecInvMass_mu");

//Declaration of the fitting functions

TF1* f[4];
TF1* fg[4];

for(int i = 0; i < 4; ++i){
        
        f[i] = FitBreitWigner(*histo[i], 70,110);
        fg[i] = FitGauss(*histo[i], 70,110);

}

TCanvas* c1 = new TCanvas("c1","c1");
c1->Divide(1,2);
c1->cd(1);

        histo[0]->Draw();
        f[0]->Draw("same");
        histo[1]->Draw("same");
        f[1]->Draw("same");
c1->cd(2);	
        histo[2]->Draw();
        f[2]->Draw("same");
        histo[3]->Draw("same");
        f[3]->Draw("same");

TCanvas* c2 = new TCanvas("c2","c2");
c2->Divide(1,2);
c2->cd(1);

        histo[0]->Draw();
        fg[0]->Draw("same");
        histo[1]->Draw("same");
        fg[1]->Draw("same");
c2->cd(2);	
        histo[2]->Draw();
        fg[2]->Draw("same");
        histo[3]->Draw("same");
        fg[3]->Draw("same");


}
