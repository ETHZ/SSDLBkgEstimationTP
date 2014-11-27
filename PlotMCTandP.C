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
#include "setTDRStyle.C"
#include "TPaveText.h"

int PlotMCTandP(){


	bool reliso03 = false;
	bool charged = true;

	double lowy = 0.5;

	TString _DR = "03";
	TString _DRMC = "0.3";
	if(reliso03 == false){_DR = "04";_DRMC = "0.4";lowy = 0.2;}
	TString _q = "";
	TString _iso;
	_iso = "isocut"+_DR;
	if(charged == true) _q = "_charged";_iso = _q+"isocut"+_DR;

	//Files from TandP
	TFile *f1 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP_nobkg/TandP2_v3_Zeffs_tightId_e"+_q+"_isocut"+_DR+"=0.20.root","read");
	TFile *f2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP_nobkg/TandP2_v3_Zeffs_tightId_e"+_q+"_isocut"+_DR+"=0.10.root","read");
	TFile *f3 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP_nobkg/TandP2_v3_Zeffs_tightId_e"+_q+"_isocut"+_DR+"=0.05.root","read");
	TFile *f4 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP_nobkg/TandP2_v3_Zeffs_tightId_mu"+_q+"_isocut"+_DR+"=0.20.root","read");
	TFile *f5 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP_nobkg/TandP2_v3_Zeffs_tightId_mu"+_q+"_isocut"+_DR+"=0.10.root","read");
	TFile *f6 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/TandP_nobkg/TandP2_v3_Zeffs_tightId_mu"+_q+"_isocut"+_DR+"=0.05.root","read");
	//Files from MC
	TFile *f7 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/ratio_eff/eff_plots2_v3_tightId_e"+_q+"_isocut"+_DRMC+".root","read");
	TFile *f8 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/ratio_eff/eff_plots2_v3_tightId_mu"+_q+"_isocut"+_DRMC+".root","read");


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

	
	//TCanvas* c = new TCanvas("c","c",1200,800);
	//c->UseCurrentStyle();
	//c->Divide(3,2);
	TCanvas* c1 = new TCanvas("c1","c1");
	c1->cd();
	//c1->UseCurrentStyle();
	//c->cd(1);
	graph1->Draw("AP");
	graph1->SetLineWidth(3);
	graph1->SetLineColor(4);
	graph1->SetMarkerStyle(1);
	graph1->SetTitle("#epsilon for e. from Z, isocut"+_DR+"  #leq 0.2");
        //graph1->SetTitleSize(0.06);
	graph1->GetYaxis()->SetRangeUser(lowy,1.);
	graph1->GetYaxis()->SetTitle(" #epsilon ");
	histo1->Draw("same");
	histo1->SetLineColor(2);
	histo1->SetLineWidth(3);
	histo1->SetMarkerColor(2);
	histo1->SetMarkerStyle(1);
	TLegend* leg = new TLegend(0.6, 0.2, 0.9 , 0.4);
	leg->AddEntry(graph1, "T&P","LP");
	leg->AddEntry(histo1, "MC ratio");
	leg->SetBorderSize(0.);
	leg->Draw();
	TCanvas* c2 = new TCanvas("c2","c2");
	c2->cd();
	//c->cd(2);
	graph2->Draw("AP");
	graph2->SetLineWidth(3);
	graph2->SetLineColor(4);
	graph2->SetMarkerStyle(1);
	graph2->SetTitle("#epsilon for e. from Z isocut"+_DR+"  #leq 0.1");
	graph2->GetYaxis()->SetRangeUser(lowy,1.);
	graph2->GetYaxis()->SetTitle(" #epsilon ");
	histo2->Draw("same");
	histo2->SetLineColor(2);
	histo2->SetLineWidth(3);
	histo2->SetMarkerColor(2);
	histo2->SetMarkerStyle(1);
	leg->Draw();
	TCanvas* c3 = new TCanvas("c3","c3");
	c3->cd();
	//c->cd(3);
	graph3->Draw("AP");
	graph3->SetLineWidth(3);
	graph3->SetLineColor(4);
	graph3->SetMarkerStyle(1);
	graph3->SetTitle("#epsilon for e. from Z isocut"+_DR+"  #leq 0.05");
	graph3->GetYaxis()->SetRangeUser(lowy,1.);
	graph3->GetYaxis()->SetTitle(" #epsilon ");
	histo3->Draw("same");
	histo3->SetLineColor(2);
	histo3->SetLineWidth(3);
	histo3->SetMarkerColor(2);
	histo3->SetMarkerStyle(1);
	leg->Draw();


	TCanvas* c4 = new TCanvas("c4","c4");
	c4->cd();
	//c->cd(4);
	graph4->Draw("AP");
	graph4->SetLineWidth(3);
	graph4->SetLineColor(4);
	graph4->SetMarkerStyle(1);
	graph4->SetTitle("#epsilon for #mu from Z, isocut"+_DR+"  #leq 0.2");
	graph4->GetYaxis()->SetRangeUser(lowy,1.);
	graph4->GetYaxis()->SetTitle(" #epsilon ");
	histo4->Draw("same");
	histo4->SetLineColor(2);
	histo4->SetLineWidth(3);
	histo4->SetMarkerColor(2);
	histo4->SetMarkerStyle(1);
	//TLegend* leg2 = new TLegend(0.4, 0.2, 0.7 , 0.4);//(0.8, 0.9, .2, .4);
	//leg2->AddEntry(graph4, "T&P","LP");
	//leg2->AddEntry(histo4, "MC ratio");
	//leg2->Draw();
	leg->Draw();
	TCanvas* c5 = new TCanvas("c5","c5");
	c5->cd();
	//c->cd(5);
	graph5->Draw("AP");
	graph5->SetLineWidth(3);
	graph5->SetLineColor(4);
	graph5->SetMarkerStyle(1);
	graph5->SetTitle("#epsilon for #mu from Z isocut"+_DR+"  #leq 0.1");
	graph5->GetYaxis()->SetRangeUser(lowy,1.);
	graph5->GetYaxis()->SetTitle(" #epsilon ");
	histo5->Draw("same");
	histo5->SetLineColor(2);
	histo5->SetLineWidth(3);
	histo5->SetMarkerColor(2);
	histo5->SetMarkerStyle(1);
	//leg2->Draw();
	leg->Draw();
	TCanvas* c6 = new TCanvas("c6","c6");
	c6->cd();
	//c->cd(6);
	graph6->Draw("AP");
	graph6->SetLineWidth(3);
	graph6->SetLineColor(4);
	graph6->SetMarkerStyle(1);
	graph6->SetTitle("#epsilon for #mu from Z isocut"+_DR+"  #leq 0.05");
	graph6->GetYaxis()->SetRangeUser(lowy,1.);
	graph6->GetYaxis()->SetTitle(" #epsilon ");
	histo6->Draw("same");
	histo6->SetLineColor(2);
	histo6->SetLineWidth(3);
	histo6->SetMarkerColor(2);
	histo6->SetMarkerStyle(1);
	//leg2->Draw();
	leg->Draw();

	TFile* file_out = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/efficiencys/PlotMCTandP_tight_"+_DR+".root","recreate");
	//c->Write();
	c1->Write();
	c2->Write();
	c3->Write();
	c4->Write();
	c5->Write();
	c6->Write();
	file_out->Close();
	//Save PDF
	//
	c1->SaveAs("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/ratio_eff/tightId_e"+_q+"_isocut"+_DR+"<0.2.pdf");
	c2->SaveAs("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/ratio_eff/tightId_e"+_q+"_isocut"+_DR+"<0.1.pdf");
	c3->SaveAs("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/ratio_eff/tightId_e"+_q+"_isocut"+_DR+"<0.05.pdf");
	c4->SaveAs("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/ratio_eff/tightId_mu"+_q+"_isocut"+_DR+"<0.2.pdf");
	c5->SaveAs("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/ratio_eff/tightId_mu"+_q+"_isocut"+_DR+"<0.1.pdf");
	c6->SaveAs("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/ratio_eff/tightId_mu"+_q+"_isocut"+_DR+"<0.05.pdf");

	return 0;

}
