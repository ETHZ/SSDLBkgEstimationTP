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
#include <iostream>
#include <iomanip>
#include <TROOT.h>
#include "TStyle.h"
#include <TH1F.h>
#include <TFile.h>
#include <TLegend.h>
#include <TCanvas.h>
//#include "../setTDRStyle.C"
#include "MC_Ratio.C"
//#include "MC_Ratio2.C"
#include "TandP.C"
#include "TPaveText.h"

int CompareMCvsTandP(int leptonId, double par_low, double par_upp,int nbins, TString sel_den , TString sel_num, double cut_den = 0., double cut_num = 0.,TString option = "", TString par_x = "Pt"){

	setTDRStyle();

	//output of the code
	TString _output ="Compare";
	TString _outpath ="/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MCvsTandP/";

	//////////////////////////
	//Retrieve MC ratio file//
	//////////////////////////
	
	TString path_MC = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_ratio/eff_beta_newdy";
	
	//Name for storing and final plots
	TString pname;
	TString _pname;
	TString _par;
	TString _sel_num;
	TString _sel_den;
	TString _cut_num;
	TString _cut_den;
	TString _eta;
	TString _option;

	//Writing string
	if (leptonId == 11) {pname = "e";_pname = "e";}
	else if (leptonId == 13){pname = " #mu";_pname = "mu";}
	if(par_x == "Pt"){_par = "P_{t}";}
	else if(par_x == "eta"){_par = "#eta";}
	else if(par_x == "phi"){_par = "#phi";}
	else{cout<<"ERROR: wrong parameter name !";return 1;}
	if(sel_num == ""){_sel_num = "unsel";}
	else if(sel_num == "tight"){_sel_num = "tight";}
	else if(sel_num == "loose"){_sel_num = "loose";}
	else if(sel_num == "reliso3"){_sel_num = Form("reliso3_%0.3lf",cut_num);}
	else if(sel_num == "reliso4"){_sel_num = Form("reliso4_%0.3lf",cut_num);}
	else if(sel_num == "chiso3"){_sel_num = Form("chiso3_%0.3lf",cut_num);}

	else if(sel_num == "chiso4"){_sel_num = Form("chiso4_%0.3lf",cut_num);}
	else if(sel_num == "dxy"){_sel_num = Form("dxy_%0.3lf",cut_num);}
	else if(sel_num == "dz"){_sel_num = Form("dz_%0.3lf",cut_num);}
	else{cout<<"ERROR: wrong numerator name !";return 1;}
	if(sel_den == ""){_sel_den = "unsel";}
	else if(sel_den == "tight"){_sel_den = "tight";}
	else{cout<<"ERROR: wrong denominator selection name !";return 1;}
	if(option.Contains("ll")){_option += "ll";}
	_option += "_";

	//TFile  f(path_MC+_option+_eta+_pname+"_den_"+_sel_den+"_num_"+_sel_num+"_"+par_x+".root");
	TFile  f(path_MC+_option+_pname+"_den_"+_sel_den+"_num_"+_sel_num+"_"+par_x+".root");
	if(f.IsZombie()){
		cout<<"File does not exists"<<endl;
		cout<<"Creating file, please wait..."<<endl;
		MC_Ratio(leptonId,par_low,par_upp,nbins,sel_den,sel_num,cut_num,par_x,option);
		cout<<"Done !"<<endl;
	}

	//TFile* fMC = new TFile(path_MC+_option+_eta+_pname+"_den_"+_sel_den+"_num_"+_sel_num+"_"+par_x+".root", "read");
	TFile* fMC = new TFile(path_MC+_option+_pname+"_den_"+_sel_den+"_num_"+_sel_num+"_"+par_x+".root", "read");
	delete fMC;
	fMC = new TFile(path_MC+_option+_eta+_pname+"_den_"+_sel_den+"_num_"+_sel_num+"_"+par_x+".root", "read");

	//////////
	//output// 
	//////////
	
	TFile* fout = new TFile(_outpath+_output+"_"+option+"_"+_pname+"_den_"+_sel_den+"_num_"+_sel_num+"_"+par_x+".root", "recreate");

	////////////////
	//Retrieve T&P//
	////////////////

	//selection string
	TString _sel;

	if(sel_den == "tight"){_sel = "tight";}
	else if(sel_den == "loose"){_sel = "loose";}
	else if(sel_den == ""){_sel = "";}
	else{cout<<"ERROR: wrong selection !";return 1;}

	//cut string
	
	TString path_TP = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/";
	
	TString _effcut;

	if(sel_num == "tight"){_effcut = "tight";}
	else if(sel_num == ""){_effcut = "";}
	else if(sel_num == "loose"){_effcut = "loose";}
	else if(sel_num == "reliso3"){_effcut = Form("reliso3_%0.3lf",cut_num);}
	else if(sel_num == "reliso4"){_effcut = Form("reliso4_%0.3lf",cut_num);}
	else if(sel_num == "chiso3"){_effcut = Form("chiso3_%0.3lf",cut_num);}
	else if(sel_num == "chiso4"){_effcut = Form("chiso4_%0.3lf",cut_num);}
	else if(sel_num == "dxy"){_effcut = Form("dxy_%0.3lf",cut_num);}
	else if(sel_num == "dz"){_effcut = Form("dz_%0.3lf",cut_num);}
	else{cout<<"ERROR: wrong numerator name !";return 1;}

	TString _fname = "InvM_beta_newdy";
	if(option.Contains("matching")){_fname += "_Matched";}

	TString _ptrange;
	_ptrange = Form("Pt%0.f_Pt%0.f",par_low,par_upp);
	_fname += "_"+_ptrange;
	_fname += "_"+_effcut+"_for_"+_sel+"_"+_pname; 
	if(option.Contains("nentries")){_fname += "_nentries";}

	TFile  f2(path_TP+_fname+"_FIT"+".root");
	if(f2.IsZombie()){

		cout<<"File does not exists"<<endl;
		cout<<"Creating file, please wait..."<<endl;
		TandP(leptonId,par_low,par_upp,nbins,sel_den,sel_num,cut_num,"CBxBW",option);
		cout<<"Done !"<<endl;
		
	}

	//Name of the input file
	TFile* f3 = new TFile(path_TP+_fname+"_FIT"+".root","read");
	if(!(f3->GetListOfKeys()->Contains("Eff_histo_"+sel_num+"_"+_pname+"_eta<1.2")||f3->GetListOfKeys()->Contains("eff_eta>1.2"))){
		f3->Close();

		cout<<"The files doesn't contain the requiered object"<<endl;
		cout<<"Doing the fit again"<<endl;
		TandP(leptonId,par_low,par_upp,nbins,sel_den,sel_num,cut_num,"CBxBW",option);
		cout<<"Done !"<<endl;
	}
	f3->Close();
	TFile* fTP = new TFile(path_TP+_fname+"_FIT"+".root","read");

	///////////////////
	//Save the output//
	///////////////////

	///////////////
	//Get the plots
	///////////////
	
	TH1D* eff_MC_B;
	TH1D* eff_MC_E;
	
	TH1D* eff_TP_B;
	TH1D* eff_TP_E;


	eff_TP_B = (TH1D*)fTP->Get("Eff_histo_"+sel_num+"_"+_pname+"_eta<1.2");
	eff_TP_E = (TH1D*)fTP->Get("Eff_histo_"+sel_num+"_"+_pname+"_eta>1.2");

	eff_MC_B = (TH1D*)fMC->Get("eff_eta<1.2");
	eff_MC_E = (TH1D*)fMC->Get("eff_eta>1.2");

	TH1D* ratio_B = (TH1D*) eff_TP_B->Clone("ratio_B");
	TH1D* ratio_E = (TH1D*) eff_TP_E->Clone("ratio_E");

	ratio_B->Divide(eff_MC_B,eff_TP_B,1,1,"B");
	ratio_E->Divide(eff_MC_E,eff_TP_E,1,1,"B");

	////////////////
	//Do the Plots//
	////////////////

	TCanvas* c1 = new TCanvas("c1","c1");
	c1->Divide(1,2);
	c1->cd(1);
	c1->GetPad(1)->SetPad(0,0.21,1,1);
	eff_MC_B->Draw();
	eff_MC_B->SetLineColor(4);
	eff_MC_B->SetLineWidth(3);
	eff_MC_B->SetMarkerColor(4);
	eff_MC_B->SetMarkerStyle(1);
	eff_MC_B->GetYaxis()->SetRangeUser(0,1.1);
	eff_MC_B->GetYaxis()->SetTitle(" #epsilon ");
	eff_MC_B->GetYaxis()->SetRangeUser(0,1.1);
	eff_MC_B->GetYaxis()->SetTitle(" #epsilon ");
	eff_MC_B->SetTitle(_sel_num+" for "+_sel_den+" "+pname+" #||{#eta}<1.2");

	eff_TP_B->Draw("same");
	eff_TP_B->SetLineWidth(3);
	eff_TP_B->SetLineColor(2);
	eff_TP_B->SetMarkerStyle(1);
	eff_TP_B->SetMarkerColor(2);
	TLegend* leg_B = new TLegend(0.3, 0.3, 0.6 , 0.5);
	leg_B->AddEntry(eff_MC_B, "MC ratio");
	leg_B->AddEntry(eff_TP_B, "T&P","LP");
	leg_B->SetBorderSize(0.);
	leg_B->SetTextFont(43);
	leg_B->SetTextSize(25);
	leg_B->Draw();

	c1->cd(2);
	c1->GetPad(2)->SetPad(0,0.02,1,0.22);
	c1->GetPad(2)->SetGrid(2);
	ratio_B->SetLineColor(1);
	ratio_B->SetTitle("");
	ratio_B->SetLineWidth(1);
	ratio_B->SetMarkerStyle(20);
	ratio_B->GetYaxis()->SetRangeUser(0.8,1.2);
	ratio_B->GetYaxis()->SetRangeUser(0.95,1.05);
	ratio_B->GetYaxis()->SetLabelSize(0.2);
	ratio_B->GetYaxis()->SetNdivisions(503);
	ratio_B->GetYaxis()->SetTitle("");
	ratio_B->GetXaxis()->SetLabelSize(0.2);
	ratio_B->GetXaxis()->SetRangeUser(0,200);
	ratio_B->GetXaxis()->SetTitle("");
	ratio_B->Draw();

	TCanvas* c2 = new TCanvas("c2","c2");
	c2->Divide(1,2);
	c2->cd(1);
	c2->GetPad(1)->SetPad(0,0.21,1,1);
	
	eff_MC_E->Draw();
	eff_MC_E->SetLineColor(4);
	eff_MC_E->SetLineWidth(3);
	eff_MC_E->SetMarkerColor(4);
	eff_MC_E->SetMarkerStyle(1);
	eff_MC_E->GetYaxis()->SetRangeUser(0,1.1);
	eff_MC_E->GetYaxis()->SetTitle(" #epsilon ");
	eff_MC_E->GetYaxis()->SetRangeUser(0,1.1);
	eff_MC_E->GetYaxis()->SetTitle(" #epsilon ");

	eff_TP_E->Draw("same");
	eff_TP_E->SetLineWidth(3);
	eff_TP_E->SetLineColor(2);
	eff_TP_E->SetMarkerStyle(1);
	eff_TP_E->SetMarkerColor(2);
	TLegend* leg_E = new TLegend(0.3, 0.3, 0.6 , 0.5);
	leg_E->AddEntry(eff_MC_E, "MC ratio");
	leg_E->AddEntry(eff_TP_E, "T&P","LP");
	leg_E->SetBorderSize(0.);
	leg_E->SetTextFont(43);
	leg_E->SetTextSize(25);
	leg_E->Draw();

	c2->cd(2);
	c2->GetPad(2)->SetPad(0,0.02,1,0.22);
	c2->GetPad(2)->SetGrid(2);
	ratio_E->SetLineColor(1);
	ratio_E->SetTitle("");
	ratio_E->SetLineWidth(1);
	ratio_E->SetMarkerStyle(20);
	ratio_E->GetYaxis()->SetRangeUser(0.8,1.2);
	ratio_E->GetYaxis()->SetRangeUser(0.95,1.05);
	ratio_E->GetYaxis()->SetLabelSize(0.2);
	ratio_E->GetYaxis()->SetNdivisions(503);
	ratio_E->GetYaxis()->SetTitle("");
	ratio_E->GetXaxis()->SetLabelSize(0.2);
	ratio_E->GetXaxis()->SetRangeUser(0,200);
	ratio_E->Draw();


	fout->cd();
	c1->Write("Barrel");
	c2->Write("EndCape");
	c1->SaveAs(_outpath+"PDF/"+_output+"_"+option+"_"+_pname+"_den_"+_sel_den+"_num_"+_sel_num+"_"+par_x+"_eta<1.2.pdf");
	c2->SaveAs(_outpath+"PDF/"+_output+"_"+option+"_"+_pname+"_den_"+_sel_den+"_num_"+_sel_num+"_"+par_x+"_eta>1.2.pdf");
	fout->Close();

	return 0;


}
