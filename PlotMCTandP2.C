/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : PlotMCTandP.C 

* Purpose : Same a previous version but only for the real TandProb this time.

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
#include "setTDRStyle.C"
#include "TPaveText.h"

int PlotMCTandP2(){

	setTDRStyle();

	//Files from TandP
	TFile *f1 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3_mu__den_unsel_num_tightId_Pt.root","read");
	//Files from MC
	TFile *f2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/InvM2_Pt10_Pt250_tight_for__mu_FIT.root","read");
	//f2->GetListOfKeys()->ls();


	TGraphErrors* graph1 = (TGraphErrors*)f2->Get("Eff_tight_mu_eta>1.2;1");

	TCanvas *c1 = (TCanvas*)f1->Get("c2");
	TH1D* histo1 = (TH1D*)c1->FindObject("eff");

	
	//TCanvas* c = new TCanvas("c","c",1200,800);
	TCanvas* c = new TCanvas("c1","c1");
	graph1->Draw("AP");
	graph1->SetLineWidth(3);
	graph1->SetLineColor(4);
	graph1->SetMarkerStyle(1);
	//graph1->SetTitle("#epsilon for e. from Z, isocut"+_DR+"  #leq 0.2");
        //graph1->SetTitleSize(0.06);
	graph1->GetYaxis()->SetRangeUser(0,1.1);
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

	//TFile* file_out = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/efficiencys/DeleteMe.root","recreate");
	////c->Write();
	//c1->Write();
	//file_out->Close();
	////Save PDF
	////
	//c1->SaveAs("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/ratio_eff/tightId_e"+_q+"_isocut"+_DR+"<0.2.pdf");

	return 0;

}
