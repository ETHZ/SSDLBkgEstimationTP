/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : PlotTandPbinvsfit.C 

* Purpose : The eff. for the rel. of the Z tree have been estimated using TandP with the total number of bins plotted against the TandP using the area under the fitted function. The plots coming from both methods have been stored separately. The objective of this code is to put all the curve on the same canvas to do a comparison.

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

int PlotTandPbinvsfit(){

	gStyle->SetOptStat(0);

//Files from TandP bin
TFile *f1 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeffs_2l_tightIde_isocut03=0.20.root","read");
TFile *f2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeffs_2l_tightIde_isocut03=0.10.root","read");
TFile *f3 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeffs_2l_tightIde_isocut03=0.05.root","read");
TFile *f4 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeffs_2l_tightIdmu_isocut03=0.20.root","read");
TFile *f5 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeffs_2l_tightIdmu_isocut03=0.10.root","read");
TFile *f6 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeffs_2l_tightIdmu_isocut03=0.05.root","read");
//Files from MC fit
TFile *f7  = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeffs_2l_nentries_tightIde_isocut03=0.20.root","read");
TFile *f8  = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeffs_2l_nentries_tightIde_isocut03=0.10.root","read");
TFile *f9  = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeffs_2l_nentries_tightIde_isocut03=0.05.root","read");
TFile *f10 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeffs_2l_nentries_tightIdmu_isocut03=0.20.root","read");
TFile *f11 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeffs_2l_nentries_tightIdmu_isocut03=0.10.root","read");
TFile *f12 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP2_v2_Zeffs_2l_nentries_tightIdmu_isocut03=0.05.root","read");

TGraphErrors* graph1 = (TGraphErrors*)f1->Get("Graph;1");
TGraphErrors* graph2 = (TGraphErrors*)f2->Get("Graph;1");
TGraphErrors* graph3 = (TGraphErrors*)f3->Get("Graph;1");
TGraphErrors* graph4 = (TGraphErrors*)f4->Get("Graph;1");
TGraphErrors* graph5 = (TGraphErrors*)f5->Get("Graph;1");
TGraphErrors* graph6 = (TGraphErrors*)f6->Get("Graph;1");
TGraphErrors* graph7 = (TGraphErrors*)f7->Get("Graph;1");
TGraphErrors* graph8 = (TGraphErrors*)f8->Get("Graph;1");
TGraphErrors* graph9 = (TGraphErrors*)f9->Get("Graph;1");
TGraphErrors* graph10 = (TGraphErrors*)f10->Get("Graph;1");
TGraphErrors* graph11 = (TGraphErrors*)f11->Get("Graph;1");
TGraphErrors* graph12 = (TGraphErrors*)f12->Get("Graph;1");

TCanvas* c1 = new TCanvas("c1","c1");
c1->cd();
graph1->Draw();
graph1->ResetAttLine();
graph1->ResetAttMarker();
graph1->SetLineColor(2);
graph1->SetLineStyle(2);
graph1->SetLineWidth(2);
graph1->SetTitle("#epsilon for e. from Z, isocu03 #leq 0.2");
graph1->GetYaxis()->SetRangeUser(0.,1.);
graph7->Draw("same");
graph7->SetLineWidth(2);
graph7->ResetAttLine();
graph7->ResetAttMarker();
TLegend* leg = new TLegend(0.4, 0.2, 0.7 , 0.4);//(0.8, 0.9, .2, .4);
leg->AddEntry(graph1, "entries");
leg->AddEntry(graph7, "fit integral");
leg->Draw();
TCanvas* c2 = new TCanvas("c2","c2");
c2->cd();
graph2->Draw();
graph2->ResetAttLine();
graph2->ResetAttMarker();
graph2->SetLineColor(2);
graph2->SetLineStyle(2);
graph2->SetLineWidth(2);
graph2->SetTitle("#epsilon for e. from Z, isocu03 #leq 0.1");
graph2->GetYaxis()->SetRangeUser(0.,1.);
graph8->Draw("same");
graph8->SetLineWidth(2);
graph8->ResetAttLine();
graph8->ResetAttMarker();
leg->Draw();
TCanvas* c3 = new TCanvas("c3","c3");
c3->cd();
graph3->Draw();
graph3->ResetAttLine();
graph3->ResetAttMarker();
graph3->SetLineColor(2);
graph3->SetLineWidth(2);
graph3->SetLineStyle(2);
graph3->SetTitle("#epsilon for e. from Z, isocu03 #leq 0.05");
graph3->GetYaxis()->SetRangeUser(0.,1.);
graph9->Draw("same");
graph9->SetLineWidth(2);
graph9->ResetAttLine();
graph9->ResetAttMarker();
leg->Draw();
TCanvas* c4 = new TCanvas("c4","c4");
c4->cd();
graph4->Draw();
graph4->ResetAttLine();
graph4->ResetAttMarker();
graph4->SetLineWidth(2);
graph4->SetLineColor(2);
graph4->SetLineStyle(2);
graph4->SetTitle("#epsilon for #mu from Z, isocu03 #leq 0.2");
graph4->GetYaxis()->SetRangeUser(0.,1.);
graph10->Draw("same");
graph10->SetLineWidth(2);
graph10->ResetAttLine();
graph10->ResetAttMarker();
leg->Draw();
TCanvas* c5 = new TCanvas("c5","c5");
c5->cd();
graph5->Draw();
graph5->ResetAttLine();
graph5->ResetAttMarker();
graph5->SetLineWidth(2);
graph5->SetLineColor(2);
graph5->SetLineStyle(2);
graph5->SetTitle("#epsilon for #mu from Z, isocu03 #leq 0.1");
graph5->GetYaxis()->SetRangeUser(0.,1.);
graph11->Draw("same");
graph11->SetLineWidth(2);
graph11->ResetAttLine();
graph11->ResetAttMarker();
leg->Draw();
TCanvas* c6 = new TCanvas("c6","c6");
c6->cd();
graph6->Draw();
graph6->ResetAttLine();
graph6->ResetAttMarker();
graph6->SetLineWidth(2);
graph6->SetLineColor(2);
graph6->SetLineStyle(2);
graph6->SetTitle("#epsilon for #mu from Z, isocu03 #leq 0.05");
graph6->GetYaxis()->SetRangeUser(0.,1.);
graph12->Draw("same");
graph12->SetLineWidth(2);
graph12->ResetAttLine();
graph12->ResetAttMarker();
leg->Draw();

	TFile* file_out = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/PlotTandPbinvsfit_2l_tightId_iso03.root","recreate");
	c1->Write();
	c2->Write();
	c3->Write();
	c4->Write();
	c5->Write();
	c6->Write();
	file_out->Close();
	
return 0;

}
