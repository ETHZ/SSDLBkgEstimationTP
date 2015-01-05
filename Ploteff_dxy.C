/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : myfile.c

* Purpose : To plot 3*times reliso3 for electron and muon

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

int Ploteff_dxy(){

	TString location = "/Users/GLP/Desktop/EFFPLOTS/";
	TFile* file_out = new TFile(location+"eff_dxy.root","recreate");

	setTDRStyle();

	TFile *e1pt = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_e__den_tight_num_dxy_0.020_Pt.root","read");
	TFile *e2pt = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_e__den_tight_num_dxy_0.010_Pt.root","read");
	TFile *e3pt = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_e__den_tight_num_dxy_0.005_Pt.root","read");

	TFile *mu1pt = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_mu__den_tight_num_dxy_0.020_Pt.root","read");
	TFile *mu2pt = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_mu__den_tight_num_dxy_0.010_Pt.root","read");
	TFile *mu3pt = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_mu__den_tight_num_dxy_0.005_Pt.root","read");

	TFile *e1eta = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_e__den_tight_num_dxy_0.020_eta.root","read");
	TFile *e2eta = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_e__den_tight_num_dxy_0.010_eta.root","read");
	TFile *e3eta = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_e__den_tight_num_dxy_0.005_eta.root","read");

	TFile *mu1eta = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_mu__den_tight_num_dxy_0.020_eta.root","read");
	TFile *mu2eta = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_mu__den_tight_num_dxy_0.010_eta.root","read");
	TFile *mu3eta = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_alleta_v3_mu__den_tight_num_dxy_0.005_eta.root","read");


	TCanvas *c_e1pt = (TCanvas*)e1pt->Get("c1");
	TCanvas *c_e2pt = (TCanvas*)e2pt->Get("c1");
	TCanvas *c_e3pt = (TCanvas*)e3pt->Get("c1");
	TCanvas *c_mu1pt = (TCanvas*)mu1pt->Get("c1");
	TCanvas *c_mu2pt = (TCanvas*)mu2pt->Get("c1");
	TCanvas *c_mu3pt = (TCanvas*)mu3pt->Get("c1");

	TCanvas *c_e1eta = (TCanvas*)e1eta->Get("c1");
	TCanvas *c_e2eta = (TCanvas*)e2eta->Get("c1");
	TCanvas *c_e3eta = (TCanvas*)e3eta->Get("c1");
	TCanvas *c_mu1eta = (TCanvas*)mu1eta->Get("c1");
	TCanvas *c_mu2eta = (TCanvas*)mu2eta->Get("c1");
	TCanvas *c_mu3eta = (TCanvas*)mu3eta->Get("c1");

	TH1D* he1pt =  (TH1D*)c_e1pt ->FindObject("eff");
	TH1D* he2pt =  (TH1D*)c_e2pt ->FindObject("eff");
	TH1D* he3pt =  (TH1D*)c_e3pt ->FindObject("eff");
	TH1D* hmu1pt = (TH1D*)c_mu1pt->FindObject("eff");
	TH1D* hmu2pt = (TH1D*)c_mu2pt->FindObject("eff");
	TH1D* hmu3pt = (TH1D*)c_mu3pt->FindObject("eff");

	TH1D* he1eta =  (TH1D*)c_e1eta ->FindObject("eff");
	TH1D* he2eta =  (TH1D*)c_e2eta ->FindObject("eff");
	TH1D* he3eta =  (TH1D*)c_e3eta ->FindObject("eff");
	TH1D* hmu1eta = (TH1D*)c_mu1eta->FindObject("eff");
	TH1D* hmu2eta = (TH1D*)c_mu2eta->FindObject("eff");
	TH1D* hmu3eta = (TH1D*)c_mu3eta->FindObject("eff");

	TCanvas* b1 = new TCanvas("b1","b1");
	he1pt->Draw();
	he1pt->SetLineColor(4);
	he1pt->SetLineWidth(1);
	he1pt->SetMarkerColor(4);
	//he1pt->SetMarkerStyle(1);
	he1pt->GetYaxis()->SetRangeUser(0,1.1);
	he2pt->Draw("same");
	he2pt->SetLineColor(2);
	he2pt->SetLineWidth(1);
	he2pt->SetMarkerColor(2);
	he3pt->Draw("same");
	he3pt->SetLineColor(3);
	he3pt->SetLineWidth(1);
	he3pt->SetMarkerColor(3);
	//he2pt->SetMarkerStyle(1);
	TLegend* leg = new TLegend(0.6, 0.3, 0.9 , 0.5);
	leg->AddEntry(he1pt, "dxy #leq 0.2 mm");
	leg->AddEntry(he2pt, "dxy #leq 0.1 mm");
	leg->AddEntry(he3pt, "dxy #leq 0.05 mm");
	leg->SetBorderSize(0.);
	leg->SetTextFont(43);
	leg->SetTextSize(25);
	leg->Draw();

	TCanvas* b2 = new TCanvas("b2","b2");
	hmu1pt->Draw();
	hmu1pt->SetLineColor(4);
	hmu1pt->SetLineWidth(1);
	hmu1pt->SetMarkerColor(4);
	//hmu1pt->SetMarkerStyle(1);
	hmu1pt->GetYaxis()->SetRangeUser(0,1.1);
	hmu2pt->Draw("same");
	hmu2pt->SetLineColor(2);
	hmu2pt->SetLineWidth(1);
	hmu2pt->SetMarkerColor(2);
	hmu3pt->Draw("same");
	hmu3pt->SetLineColor(3);
	hmu3pt->SetLineWidth(1);
	hmu3pt->SetMarkerColor(3);
	//hmu2pt->SetMarkerStyle(1);
	TLegend* leg2 = new TLegend(0.6, 0.3, 0.9 , 0.5);
	leg2->AddEntry(hmu1pt, "dxy #leq 0.2 mm");
	leg2->AddEntry(hmu2pt, "dxy #leq 0.1 mm");
	leg2->AddEntry(hmu3pt, "dxy #leq 0.05 mm");
	leg2->SetBorderSize(0.);
	leg2->SetTextFont(43);
	leg2->SetTextSize(25);
	leg2->Draw();

	TCanvas* b3 = new TCanvas("b3","b3");
	he1eta->Draw();
	he1eta->SetLineColor(4);
	he1eta->SetLineWidth(1);
	he1eta->SetMarkerColor(4);
	//he1eta->SetMarkerStyle(1);
	he1eta->GetYaxis()->SetRangeUser(0,1.1);
	he2eta->Draw("same");
	he2eta->SetLineColor(2);
	he2eta->SetLineWidth(1);
	he2eta->SetMarkerColor(2);
	he3eta->Draw("same");
	he3eta->SetLineColor(3);
	he3eta->SetLineWidth(1);
	he3eta->SetMarkerColor(3);
	//he2eta->SetMarkerStyle(1);
	TLegend* leg3 = new TLegend(0.6, 0.3, 0.9 , 0.5);
	leg3->AddEntry(he1eta, "dxy #leq 0.2 mm");
	leg3->AddEntry(he2eta, "dxy #leq 0.1 mm");
	leg3->AddEntry(he3eta, "dxy #leq 0.05 mm");
	leg3->SetBorderSize(0.);
	leg3->SetTextFont(43);
	leg3->SetTextSize(25);
	leg3->Draw();

	TCanvas* b4 = new TCanvas("b4","b4");
	hmu1eta->Draw();
	hmu1eta->SetLineColor(4);
	hmu1eta->SetLineWidth(1);
	hmu1eta->SetMarkerColor(4);
	//hmu1eta->SetMarkerStyle(1);
	hmu1eta->GetYaxis()->SetRangeUser(0,1.1);
	hmu2eta->Draw("same");
	hmu2eta->SetLineColor(2);
	hmu2eta->SetLineWidth(1);
	hmu2eta->SetMarkerColor(2);
	hmu3eta->Draw("same");
	hmu3eta->SetLineColor(3);
	hmu3eta->SetLineWidth(1);
	hmu3eta->SetMarkerColor(3);
	//hmu2eta->SetMarkerStyle(1);
	TLegend* leg4 = new TLegend(0.6, 0.3, 0.9 , 0.5);
	leg4->AddEntry(hmu1eta, "dxy #leq 0.2 mm");
	leg4->AddEntry(hmu2eta, "dxy #leq 0.1 mm");
	leg4->AddEntry(hmu3eta, "dxy #leq 0.05 mm");
	leg4->SetBorderSize(0.);
	leg4->SetTextFont(43);
	leg4->SetTextSize(25);
	leg4->Draw();

	TCanvas* d1 = new TCanvas("d1","d1");
	he1pt->Draw();
	he1pt->SetLineColor(4);
	he1pt->SetLineWidth(1);
	he1pt->SetMarkerColor(4);
	//he1pt->SetMarkerStyle(1);
	he1pt->GetYaxis()->SetRangeUser(0,1.1);
	hmu1pt->Draw("same");
	hmu1pt->SetLineColor(2);
	hmu1pt->SetLineWidth(1);
	hmu1pt->SetMarkerColor(2);
	//hmu1pt->SetMarkerStyle(1);
	TLegend* dleg = new TLegend(0.6, 0.3, 0.9 , 0.5);
	dleg->AddEntry(he1pt, "e");
	dleg->AddEntry(hmu1pt, "#mu");
	dleg->SetBorderSize(0.);
	dleg->SetTextFont(43);
	dleg->SetTextSize(25);
	dleg->Draw();

	TCanvas* d2 = new TCanvas("d2","d2");
	he2pt->Draw();
	he2pt->SetLineColor(2);
	he2pt->SetLineWidth(1);
	he2pt->SetMarkerColor(2);
	//he2pt->SetMarkerStyle(1);
	he2pt->GetYaxis()->SetRangeUser(0,1.1);
	hmu2pt->Draw("same");
	hmu2pt->SetLineColor(4);
	hmu2pt->SetLineWidth(1);
	hmu2pt->SetMarkerColor(4);
	//hmu2pt->SetMarkerStyle(1);
	TLegend* dleg2 = new TLegend(0.6, 0.3, 0.9 , 0.5);
	dleg2->AddEntry(he2pt, "e");
	dleg2->AddEntry(hmu2pt, "#mu");
	dleg2->SetBorderSize(0.);
	dleg2->SetTextFont(43);
	dleg2->SetTextSize(25);
	dleg2->Draw();

	TCanvas* d3 = new TCanvas("d3","d3");
	he1eta->Draw();
	he1eta->SetLineColor(4);
	he1eta->SetLineWidth(1);
	he1eta->SetMarkerColor(4);
//	he1eta->SetMarkerStyle(1);
	he1eta->GetYaxis()->SetRangeUser(0,1.1);
	hmu1eta->Draw("same");
	hmu1eta->SetLineColor(2);
	hmu1eta->SetLineWidth(1);
	hmu1eta->SetMarkerColor(2);
//	hmu1eta->SetMarkerStyle(1);
	
	TLegend* dleg3 = new TLegend(0.6, 0.3, 0.9 , 0.5);
	dleg3->AddEntry(he1eta, "e");
	dleg3->AddEntry(hmu1eta, "#mu");
	dleg3->SetBorderSize(0.);
	dleg3->SetTextFont(43);
	dleg3->SetTextSize(25);
	dleg3->Draw();

	TCanvas* d4 = new TCanvas("d4","d4");
	he2eta->Draw();
	he2eta->SetLineColor(2);
	he2eta->SetLineWidth(1);
	he2eta->SetMarkerColor(2);
	//he2eta->SetMarkerStyle(1);
	he2eta->GetYaxis()->SetRangeUser(0,1.1);
	hmu2eta->Draw("same");
	hmu2eta->SetLineColor(4);
	hmu2eta->SetLineWidth(1);
	hmu2eta->SetMarkerColor(4);
	//hmu2eta->SetMarkerStyle(1);
	TLegend* dleg4 = new TLegend(0.6, 0.3, 0.9 , 0.5);
	dleg4->AddEntry(he2eta, "e");
	dleg4->AddEntry(hmu2eta, "#mu");
	dleg4->SetBorderSize(0.);
	dleg4->SetTextFont(43);
	dleg4->SetTextSize(25);
	dleg4->Draw();

	file_out->cd();

	b1->Write("e_pt");
	b2->Write("mu_pt");
	b3->Write("e_eta");
	b4->Write("mu_eta");

	d1->Write("e_mu_pt0.02");
	d2->Write("e_mu_pt0.01");
	d3->Write("e_mu_eta0.02");
	d4->Write("e_mu_eta0.01");

	file_out->Close();




	return 0;

}
