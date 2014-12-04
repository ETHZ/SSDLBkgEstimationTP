/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : AddHisto.C++

* Purpose : This files takes two .root files in as specific location, extract the histograms and then add them on the same canvas. 

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

int AddHisto(){

	setTDRStyle();

	//Files from TandP bin
	TFile* f1 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/cutstudy/eff_mu_fromDY_den_tight_chiso3.root","read");
	TFile* f2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/cutstudy/eff_mu_fromWTT_den_tight_chiso3.root","read");

	TCanvas* c1 = (TCanvas*)f1->Get("c1");
	TCanvas* c2 = (TCanvas*)f2->Get("c1");

	TH1D* histo1 = (TH1D*)c1->FindObject("histo");
	TH1D* histo2 = (TH1D*)c2->FindObject("histo");

	TCanvas* b = new TCanvas("b","b");
	b->SetLogy();
	histo1->Draw();
	histo1->SetLineColor(4);
	histo1->SetLineWidth(2);
	histo1->SetMarkerStyle(1);
	histo1->SetMarkerColor(4);
	histo1->SetTitle("tight #mu DY and bkg");
	histo1->GetXaxis()->SetNdivisions(509);
	histo2->Draw("same");
	histo2->SetLineColor(2);
	histo2->SetLineWidth(2);
	histo2->SetMarkerStyle(1);
	histo2->SetMarkerColor(2);
	TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
	leg->AddEntry(histo1,"DYJets");
	leg->AddEntry(histo2,"WJets+TTJets");
	leg->SetTextFont(43);
	leg->SetTextSize(25);
	leg->SetBorderSize(0);
	leg->Draw();


	return 0;


}
