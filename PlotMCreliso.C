/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : myfile.c

* Purpose :

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
#include "setTDRStyle.C"
#include "TPaveText.h"

int PlotMCreliso(){

	setTDRStyle();

	TFile *f1 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_e__den_tight_num_reliso3_0.200_Pt.root","read");
	TFile *f2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_e__den_tight_num_reliso3_0.010_Pt.root","read");
	TFile *f3 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_e__den_tight_num_reliso3_0.007_Pt.root","read");

	TFile *g1 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_mu__den_tight_num_reliso3_0.200_Pt.root","read");
	TFile *g2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_mu__den_tight_num_reliso3_0.010_Pt.root","read");
	TFile *g3 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_mu__den_tight_num_reliso3_0.007_Pt.root","read");


	TCanvas *c1 = (TCanvas*)f1->Get("c1");
	TCanvas *c2 = (TCanvas*)f2->Get("c1");
	TCanvas *c3 = (TCanvas*)f3->Get("c1");
	TCanvas *d1 = (TCanvas*)g1->Get("c1");
	TCanvas *d2 = (TCanvas*)g2->Get("c1");
	TCanvas *d3 = (TCanvas*)g3->Get("c1");

	TH1D* he1 = (TH1D*)c1->FindObject("eff");
	TH1D* he2 = (TH1D*)c2->FindObject("eff");
	TH1D* he3 = (TH1D*)c3->FindObject("eff");
	TH1D* hmu1 = (TH1D*)d1->FindObject("eff");
	TH1D* hmu2 = (TH1D*)d2->FindObject("eff");
	TH1D* hmu3 = (TH1D*)d3->FindObject("eff");

	TCanvas* c = new TCanvas("c1","c1");
	he1->Draw();
	he1->SetLineColor(2);
	he1->SetLineWidth(3);
	he1->SetMarkerColor(2);
	he1->SetMarkerStyle(1);
	he1->GetYaxis()->SetRangeUser(0,1);
	he2->Draw("same");
	//he2->SetLineColor(2);
	//he2->SetLineWidth(3);
	//he2->SetMarkerColor(2);
	//he2->SetMarkerStyle(1);
	//he3->Draw("same");
	//he3->SetLineColor(2);
	//he3->SetLineWidth(3);
	//he3->SetMarkerColor(2);
	//he3->SetMarkerStyle(1);

	//TCanvas* b = new TCanvas("c1","c1");
	//hmu1->Draw();
	//hmu1->SetLineColor(2);
	//hmu1->SetLineWidth(3);
	//hmu1->SetMarkerColor(2);
	//hmu1->SetMarkerStyle(1);
	//hmu2->Draw("same");
	//hmu2->SetLineColor(2);
	//hmu2->SetLineWidth(3);
	//hmu2->SetMarkerColor(2);
	//hmu2->SetMarkerStyle(1);
	//hmu3->Draw("same");
	//hmu3->SetLineColor(2);
	//hmu3->SetLineWidth(3);
	//hmu3->SetMarkerColor(2);
	//hmu3->SetMarkerStyle(1);

	TLegend* leg = new TLegend(0.6, 0.2, 0.9 , 0.4);
	leg->AddEntry(he1, "MC ratio");
	leg->SetBorderSize(0.);
	leg->Draw();

	//TFile* file_out = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/efficiencys/DeleteMe.root","recreate");
	////c->Write();
	//c1->Write();
	//file_out->Close();
	////Save PDF
	////
	//c1->SaveAs("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/ratio_eff/tightId_e"+_q+"_isocut"+_DR+"<0.2.pdf");

	return 0;

}
