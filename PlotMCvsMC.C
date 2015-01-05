/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : PlotMCTandP.C 

* Purpose : Used to combine plots from ratio_eff

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

int PlotMCvsMC(){

	setTDRStyle();


	bool reliso03 = false;
	bool charged = false;

	double lowy = 0.5;

	TString _DR = "03";
	TString _DRMC = "0.3";
	if(reliso03 == false){_DR = "04";_DRMC = "0.4";lowy = 0.2;}
	TString _q = "";
	TString _iso;
	_iso = "isocut"+_DR;
	if(charged == true) _q = "_charged";_iso = _q+"isocut"+_DR;

	//Files from MC
	//TFile *f1 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ratio_eff/eff_plots2_v3_tightId_e"+_q+"_isocut"+_DRMC+".root","read");
	//TFile *f2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ratio_eff/eff_plots2_v3_tightId_mu"+_q+"_isocut"+_DRMC+".root","read");
	TFile *f1 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ratio_eff/eff_plots2_v3_tightId_e_charged_isocut0.3.root","read");
	TFile *f2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ratio_eff/eff_plots2_v3_tightId_mu_charged_isocut0.3.root","read");
	TFile *f3 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ratio_eff/eff_plots2_v3_tightId_e_charged_isocut0.4.root","read");
	TFile *f4 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ratio_eff/eff_plots2_v3_tightId_mu_charged_isocut0.4.root","read");

	TCanvas *c1 = (TCanvas*)f1->Get("c1");
	TCanvas *c2 = (TCanvas*)f2->Get("c1");
	TCanvas *c3 = (TCanvas*)f3->Get("c1");
	TCanvas *c4 = (TCanvas*)f4->Get("c1");

	TH1D* histo1 = (TH1D*)c1->FindObject("eff_isocut_10");
	//TH1D* histo2 = (TH1D*)c7->FindObject("eff_isocut_7");
	//TH1D* histo3 = (TH1D*)c7->FindObject("eff_isocut_5");
	TH1D* histo2 = (TH1D*)c2->FindObject("eff_isocut_10");
	//TH1D* histo5 = (TH1D*)c8->FindObject("eff_isocut_7");
	//TH1D* histo6 = (TH1D*)c8->FindObject("eff_isocut_5");
	TH1D* histo3 = (TH1D*)c3->FindObject("eff_isocut_10");
	TH1D* histo4 = (TH1D*)c4->FindObject("eff_isocut_10");

	
	//TCanvas* c = new TCanvas("c","c",1200,800);
	//c->UseCurrentStyle();
	//c->Divide(3,2);
	TCanvas* c = new TCanvas("c1","c1");
	c1->cd();
	//c1->UseCurrentStyle();
	//c->cd(1);
	histo1->Draw();
	histo1->SetLineColor(1);
	histo1->SetLineWidth(4);
	histo1->SetMarkerColor(1);
	histo1->SetMarkerStyle(1);
	histo1->GetYaxis()->SetRangeUser(0.5,1.05);
	histo1->SetTitle("");
	histo2->Draw("same");
	histo2->SetLineColor(3);
	histo2->SetLineStyle(2);
	histo2->SetLineWidth(4);
	histo2->SetMarkerColor(3);
	histo2->SetMarkerStyle(1);
	histo3->Draw("same");
	histo3->SetLineColor(2);
	histo3->SetLineWidth(4);
	histo3->SetMarkerColor(2);
	histo3->SetMarkerStyle(1);
	histo4->Draw("same");
	histo4->SetLineColor(4);
	histo4->SetLineWidth(4);
	histo4->SetLineStyle(2);
	histo4->SetMarkerColor(4);
	histo4->SetMarkerStyle(1);
	TLegend* leg = new TLegend(0.6, 0.3, 0.9 , 0.5);
	leg->AddEntry(histo2, "#mu, #Delta R = 0.3");
	leg->AddEntry(histo1, "e, #Delta R = 0.3 ");
	leg->AddEntry(histo4, "#mu, #Delta R = 0.4");
	leg->AddEntry(histo3, "e, #Delta R = 0.4");
	leg->SetTextFont(43);
	leg->SetTextSize(25);
	leg->SetBorderSize(0.);
	leg->Draw();

	//Save PDF
	//
	c->SaveAs("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ratio_eff/chiso4_mu_vs_e.pdf");

	TFile* file_out = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ratio_eff/chiso4_mu_vs_e.root","recreate");
	c->Write();
	file_out->Close();

	return 0;

}
