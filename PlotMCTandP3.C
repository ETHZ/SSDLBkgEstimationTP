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
#include <iostream>
#include <iomanip>
#include <TROOT.h>
#include "TStyle.h"
#include <TH1F.h>
#include <TFile.h>
#include <TLegend.h>
#include <TCanvas.h>
#include "setTDRStyle.C"
#include "TPaveText.h"

int PlotMCTandP3(){

	setTDRStyle();
	gROOT->SetBatch(kTRUE); 


	int sel_array[4] = {1,2,1,1};
	int lep_array[2] = {11,13};
	bool barrel_array[2] = {true,false};

	TString location = "/Users/GLP/Desktop/EFFPLOTS/";
	TFile* file_out = new TFile(location+"eff_MCvsTandP.root","recreate");

	for(int i = 0; i<4 ; ++i){
		for(int j = 0; j<2 ; ++j){
			for(int k = 0; k<2 ; ++k){

				//Name of the canvas in the stored file
				TString _cname;

				int sel = sel_array[i];//1 for tight, 2 for rel. iso, 3 for dxy and 4 for dz
				int lep = lep_array[j];
				bool barrel = barrel_array[k];

				TString _lep;
				TString _sel;
				TString _eta;

				if(lep == 11){_lep = "e";}
				else if(lep == 13){_lep = "mu";}

				if(barrel == true){_eta = "eta<1.2";}
				else if(barrel == false){_eta = "eta>1.2";}


				TFile *f1;
				TFile *f2;
				//TFile *f3;
				TGraphErrors* graph1;
				TGraphErrors* graph2;

				TH1D* hist1;

				TCanvas *c1;

				switch(sel){

					case 1:
						f1 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_v3_"+_lep+"__den_unsel_num_tightId_Pt.root","read");
						f2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/InvM2_Pt10_Pt250_tight_for__"+_lep+"_test_FIT.root","read");
						//f3 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/InvM2_Matched_Pt10_Pt250_tight_for__"+_lep+"_FIT.root","read");

						if(barrel == true){
							graph1 = (TGraphErrors*)f2->Get("Eff_tight_"+_lep+"_eta<1.2;1");
							//graph2 = (TGraphErrors*)f3->Get("Eff_tight_"+_lep+"_eta<1.2;1");

							hist1 = (TH1D*)f2->Get("Eff_histo_tight_"+_lep+"_eta<1.2;1");
						}else{
							graph1 = (TGraphErrors*)f2->Get("Eff_tight_"+_lep+"_eta>1.2;1");
							//graph2 = (TGraphErrors*)f3->Get("Eff_tight_"+_lep+"_eta>1.2;1");

							hist1 = (TH1D*)f2->Get("Eff_histo_tight_"+_lep+"_eta>1.2;1");
						}

						_sel = "den_unsel_num_tight";

						break;

					case 2:
						f1 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_v3_"+_lep+"__den_tight_num_reliso3_0.200_Pt.root","read");
						f2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/InvM2_Pt10_Pt250_reliso3_0.200_for_tight_"+_lep+"_test_FIT.root","read");
						//f3 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/InvM2_Matched_Pt10_Pt250_reliso3_0.200_for_tight_"+_lep+"_FIT.root","read");
						if(barrel == true){
							graph1 = (TGraphErrors*)f2->Get("Eff_reliso3_"+_lep+"_eta<1.2;1");
							//graph2 = (TGraphErrors*)f3->Get("Eff_reliso3_"+_lep+"_eta<1.2;1");
							
							hist1 = (TH1D*)f2->Get("Eff_histo_reliso3_"+_lep+"_eta<1.2;1");
						}else{
							graph1 = (TGraphErrors*)f2->Get("Eff_reliso3_"+_lep+"_eta>1.2;1");
							//graph2 = (TGraphErrors*)f3->Get("Eff_reliso3_"+_lep+"_eta>1.2;1");
							//
							hist1 = (TH1D*)f2->Get("Eff_histo_reliso3_"+_lep+"_eta>1.2;1");
							
						}

						_sel = "den_tight_num_reliso3<0.2";

						break;

					case 3:
						f1 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_v3_"+_lep+"__den_tight_num_dxy_0.010_Pt.root","read");
						f2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/InvM2_Pt10_Pt250_dxy_0.010_for_tight_"+_lep+"_FIT.root","read");
						//f3 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/InvM2_Matched_Pt10_Pt250_dxy_0.010_for_tight_"+_lep+"_FIT.root","read");
						if(barrel == true){
							graph1 = (TGraphErrors*)f2->Get("Eff_dxy_"+_lep+"_eta<1.2;1");
							//graph2 = (TGraphErrors*)f3->Get("Eff_dxy_"+_lep+"_eta<1.2;1");
						}else{
							graph1 = (TGraphErrors*)f2->Get("Eff_dxy_"+_lep+"_eta>1.2;1");
							//graph2 = (TGraphErrors*)f3->Get("Eff_dxy_"+_lep+"_eta>1.2;1");
						}

						_sel = "den_tight_num_dxy<0.01";

						break;

					case 4:
						f1 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/eff3ll_v3_"+_lep+"__den_tight_num_dz_0.020_Pt.root","read");
						f2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/InvM2_Pt10_Pt250_dz_0.020_for_tight_"+_lep+"_FIT.root","read");
						//f3 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/InvM2_Matched_Pt10_Pt250_dz_0.020_for_tight_"+_lep+"_FIT.root","read");

						if(barrel == true){
							graph1 = (TGraphErrors*)f2->Get("Eff_dz_"+_lep+"_eta<1.2;1");
							//graph2 = (TGraphErrors*)f3->Get("Eff_dz_"+_lep+"_eta<1.2;1");
						}else{
							graph1 = (TGraphErrors*)f2->Get("Eff_dz_"+_lep+"_eta>1.2;1");
							//graph2 = (TGraphErrors*)f3->Get("Eff_dz_"+_lep+"_eta>1.2;1");
						}

						_sel = "den_tight_num_dz<0.02";

						break;

				}

				//Name of output canvas
				_cname = "eff_"+_sel+"_"+_lep+"_"+_eta;

				if(barrel == true){c1 = (TCanvas*)f1->Get("c1");}//Barrel
				else{c1 = (TCanvas*)f1->Get("c2");}
				TH1D* histo1 = (TH1D*)c1->FindObject("eff");
				histo1->SetBinContent(0,-1);
				TH1D* ratio = (TH1D*) histo1->Clone("ratio"); 

				//histo1->Sumw2();
				ratio->Divide(histo1,hist1,1,1,"B");

				TCanvas* c = new TCanvas("c1","c1");
				c->Divide(1,2);
				c->cd(1);
				c->GetPad(1)->SetPad(0,0.21,1,1);
				histo1->Draw();
				histo1->SetLineColor(4);
				histo1->SetLineWidth(3);
				histo1->SetMarkerColor(4);
				histo1->SetMarkerStyle(1);
				histo1->GetYaxis()->SetRangeUser(0,1.1);
				histo1->GetYaxis()->SetTitle(" #epsilon ");
				histo1->GetXaxis()->SetRangeUser(0,200);
				hist1->Draw("same");
				hist1->SetLineWidth(3);
				hist1->SetLineColor(2);
				hist1->SetMarkerStyle(1);
				hist1->SetMarkerColor(2);
				hist1->GetYaxis()->SetRangeUser(0,1.1);
				hist1->GetYaxis()->SetTitle(" #epsilon ");

				TLegend* leg = new TLegend(0.6, 0.3, 0.9 , 0.5);
				leg->AddEntry(histo1, "MC ratio");
				leg->AddEntry(hist1, "T&P","LP");
				//leg->AddEntry(graph2, "T&P Matched","LP");
				leg->SetBorderSize(0.);
				leg->SetTextFont(43);
				leg->SetTextSize(25);
				leg->Draw();

				c->cd(2);
				c->GetPad(2)->SetPad(0,0.02,1,0.22);
				c->GetPad(2)->SetGrid(2);
				ratio->SetLineColor(1);
				ratio->SetTitle("");
				ratio->SetLineWidth(3);
				ratio->SetMarkerStyle(1);
				ratio->SetMarkerStyle(1);
				ratio->GetYaxis()->SetRangeUser(0.8,1.2);
				ratio->GetYaxis()->SetRangeUser(0.95,1.05);
				ratio->GetYaxis()->SetLabelSize(0.2);
				ratio->GetYaxis()->SetNdivisions(503);
				ratio->GetYaxis()->SetTitle("");
				ratio->GetXaxis()->SetLabelSize(0.2);
				ratio->GetXaxis()->SetRangeUser(0,200);
				ratio->GetXaxis()->SetTitle("");
				ratio->Draw();

				file_out->cd();
				c->Write(_cname);

				//Save PDF
				c->SaveAs(location+_cname+".pdf");

			}
		}
	}

	file_out->Close();

	return 0;

}
