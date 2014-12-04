/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : PlotZbkg.C 

* Purpose : The Z invariant mass has been plotted separately for the Z and background (TTJets) events. This file plots the two of them on the same canvas.

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By : Gaël L. Perrin

_._._._._._._._._._._._._._._._._._._._._.*/

#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include <string>
#include <functional>
#include <iomanip>
#include <TROOT.h>
#include "TStyle.h"
#include <TH1F.h>
#include <TFile.h>
#include <TLegend.h>
#include <TCanvas.h>

int PlotZbkg(){

//	gStyle->SetOptStat(0);

//Files from TandP
TFile *f1 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots/Figure_133.root","read");
TFile *f2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots/Figure_134.root","read");
//Files from MC
TFile *f3 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots/Figure_135.root","read");
TFile *f4 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots/Figure_136.root","read");

TCanvas *ca = (TCanvas*)f1->Get("c1");
TCanvas *cb = (TCanvas*)f2->Get("c1");
TCanvas *cc = (TCanvas*)f3->Get("c1");
TCanvas *cd = (TCanvas*)f4->Get("c1");

TH1D* histo1 = (TH1D*)ca->FindObject("histo_M");
TH1D* histo2 = (TH1D*)cb->FindObject("histo_M");
TH1D* histo3 = (TH1D*)cc->FindObject("histo_M");
TH1D* histo4 = (TH1D*)cd->FindObject("histo_M");

ca->GetListOfPrimitives()->ls();

TCanvas* c1 = new TCanvas("c1","c1");
histo3->Draw();
histo3->SetTitle("Z Invariant mass for e.");
histo1->Draw("same");
histo1->SetLineColor(2);
histo1->SetFillColor(2);
histo1->SetFillStyle(3004);
histo1->SetMarkerColor(2);
TLegend* leg = new TLegend(0.2, 0.6, 0.45 , 0.8);//(0.8, 0.9, .2, .4);
leg->AddEntry(histo3, "Z + TTJets");
leg->AddEntry(histo1, "TTJets");
leg->Draw();

TCanvas* c2 = new TCanvas("c2","c2");
histo4->Draw();
histo4->SetTitle("Z Invariant mass for   #mu.");
histo2->Draw("same");
histo2->SetLineColor(2);
histo2->SetFillColor(2);
histo2->SetMarkerColor(2);
histo2->SetFillStyle(3004);
leg->Draw();


	TFile* file_out = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/PlotZbkg.root","recreate");
	c1->Write();
	c2->Write();
	file_out->Close();

	return 0;

}
