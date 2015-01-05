/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : PlotMCTandP.C 

* Purpose : The eff. for the rel. of the Z tree have been estimated using MC and TandP. The plots coming from both methods have been stored separately. The objective of this code is to put all the curve on the same canvas to do a comparison.

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By :

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

int PlotMCTandP(){

	gStyle->SetOptStat(0);

//Files from TandP
TFile *f1 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeff_beta_e isocut04=0.20.root","read");
TFile *f2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeff_beta_e isocut04=0.10.root","read");
TFile *f3= new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeff_beta_e isocut04=0.05.root","read");
TFile *f4 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeff_beta_mu isocut04=0.20.root","read");
TFile *f5 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeff_beta_mu isocut04=0.10.root","read");
TFile *f6 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeff_beta_mu isocut04=0.05.root","read");
//Files from MC
TFile *f7 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots/Figure_86.root");
TFile *f8 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots/Figure_87.root");

TGraphErrors* graph1 = (TGraphErrors*)f1->Get("Graph;1");
TGraphErrors* graph2 = (TGraphErrors*)f2->Get("Graph;1");
TGraphErrors* graph3 = (TGraphErrors*)f3->Get("Graph;1");
TGraphErrors* graph4 = (TGraphErrors*)f4->Get("Graph;1");
TGraphErrors* graph5 = (TGraphErrors*)f5->Get("Graph;1");
TGraphErrors* graph6 = (TGraphErrors*)f6->Get("Graph;1");

TCanvas *c7 = (TCanvas*)f7->Get("c1");
TCanvas *c8 = (TCanvas*)f8->Get("c1");

TH1D* histo1 = (TH1D*)c7->FindObject("eff_isocut_10");
TH1D* histo2 = (TH1D*)c7->FindObject("eff_isocut_7");
TH1D* histo3 = (TH1D*)c7->FindObject("eff_isocut_5");
TH1D* histo4 = (TH1D*)c8->FindObject("eff_isocut_10");
TH1D* histo5 = (TH1D*)c8->FindObject("eff_isocut_7");
TH1D* histo6 = (TH1D*)c8->FindObject("eff_isocut_5");

TCanvas* c1 = new TCanvas("c1","c1");
c1->cd();
graph1->ResetAttLine();
graph1->ResetAttMarker();
graph1->Draw();
graph1->SetTitle("#epsilon for e. from Z, isocu04 #leq 0.2");
graph1->GetYaxis()->SetRangeUser(0.,1.);
histo1->Draw("same");
histo1->SetLineColor(2);
histo1->SetMarkerColor(2);
TLegend* leg = new TLegend(0.4, 0.2, 0.7 , 0.4);//(0.8, 0.9, .2, .4);
leg->AddEntry(graph1, "T&P");
leg->AddEntry(histo1, "MC");
leg->Draw();
TCanvas* c2 = new TCanvas("c2","c2");
c2->cd();
graph2->ResetAttLine();
graph2->ResetAttMarker();
graph2->Draw();
graph2->SetTitle("#epsilon for e. from Z isocu04 #leq 0.1");
graph2->GetYaxis()->SetRangeUser(0.,1.);
histo2->Draw("same");
histo2->SetLineColor(2);
histo2->SetMarkerColor(2);
leg->Draw();
TCanvas* c3 = new TCanvas("c3","c3");
c3->cd();
graph3->ResetAttLine();
graph3->ResetAttMarker();
graph3->Draw("AC");
graph3->SetTitle("#epsilon for e. from Z isocu04 #leq 0.05");
graph3->GetYaxis()->SetRangeUser(0.,1.);
histo3->Draw("same");
histo3->SetLineColor(2);
histo3->SetMarkerColor(2);
leg->Draw();


TCanvas* c4 = new TCanvas("c4","c4");
c4->cd();
graph4->ResetAttLine();
graph4->ResetAttMarker();
graph4->Draw("AC");
graph4->SetTitle("#epsilon for #mu from Z, isocu04 #leq 0.2");
graph4->GetYaxis()->SetRangeUser(0.,1.);
histo4->Draw("same");
histo4->SetLineColor(2);
histo4->SetMarkerColor(2);
TLegend* leg2 = new TLegend(0.4, 0.2, 0.7 , 0.4);//(0.8, 0.9, .2, .4);
leg2->AddEntry(graph4, "T&P");
leg2->AddEntry(histo4, "MC");
leg2->Draw();
TCanvas* c5 = new TCanvas("c5","c5");
c5->cd();
graph5->ResetAttLine();
graph5->ResetAttMarker();
graph5->Draw("AC");
graph5->SetTitle("#epsilon for #mu from Z isocu04 #leq 0.1");
graph5->GetYaxis()->SetRangeUser(0.,1.);
histo5->Draw("same");
histo5->SetLineColor(2);
histo5->SetMarkerColor(2);
leg2->Draw();
TCanvas* c6 = new TCanvas("c6","c6");
c6->cd();
graph6->ResetAttLine();
graph6->ResetAttMarker();
graph6->Draw("AC");
graph6->SetTitle("#epsilon for #mu from Z isocu04 #leq 0.05");
graph6->GetYaxis()->SetRangeUser(0.,1.);
histo6->Draw("same");
histo6->SetLineColor(2);
histo6->SetMarkerColor(2);
leg2->Draw();

	TFile* file_out = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/PlotMCTandP.root","recreate");
	c1->Write();
	c2->Write();
	c3->Write();
	c4->Write();
	c5->Write();
	c6->Write();
	file_out->Close();


	
return 0;

}
