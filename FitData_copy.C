/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : FitData.C 

* Purpose : Performs a fit of the invriant mass mass + background and store the result using path.
            Additionnaly, the T&P ratio efficiency is calculated and stored.

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By :

_._._._._._._._._._._._._._._._._._._._._.*/

#include "TString.h"
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include <stdio.h>
#include <sys/stat.h>
#include <iostream>
#include <cmath>

#include "FitInvMassBkg_v3.C"

double BinomError(double Nt, double eff) {

	double error=0;
	if(Nt==0) return 1;
	error = sqrt(eff*(1-eff)/Nt) ;
	return error;

}


int FitData(int leptonId, double Pt_low, double Pt_upp = 9999, int nptbins = 10, TString select = "tight", TString effcut = "", double cut = 0.2, TString _sig = "CBxBW",TString option = ""){
	//setTDRStyle();
	gROOT->SetBatch(kTRUE); 
	
	//Path for input and output file. Written in FitDataPath.txt
	ifstream file("FitDataPath.txt");
	string str;
	getline(file,str);
	TString _path = str;

	//Some variables
	double Dpt = (Pt_upp-Pt_low)/nptbins;

	/////////////////////
	//Name of the input//
	/////////////////////
	
	TString pname;
	if(leptonId == 11){pname = "e";}
	if(leptonId == 13){pname = "mu";}

	//selection string
	TString _sel;

	//cout<<"select is "<<select<<endl;
	if(select == "tight"){_sel = "tight";}
	else if(select == "loose"){_sel = "loose";}
	else if(select == ""){_sel = "";}
	else{cout<<"ERROR: wrong selection !";return 1;}

	//cut string
	TString _effcut;

	if(effcut == "tight"){_effcut = "tight";}
	else if(effcut == ""){_effcut = "";}
	else if(effcut == "loose"){_effcut = "loose";}
	else if(effcut == "reliso3"){_effcut = Form("reliso3_%0.3lf",cut);}
	else if(effcut == "reliso4"){_effcut = Form("reliso4_%0.3lf",cut);}
	else if(effcut == "chiso3"){_effcut = Form("chiso3_%0.3lf",cut);}
	else if(effcut == "chiso4"){_effcut = Form("chiso4_%0.3lf",cut);}
	else if(effcut == "dxy"){_effcut = Form("dxy_%0.3lf",cut);}
	else if(effcut == "dz"){_effcut = Form("dz_%0.3lf",cut);}
	else{cout<<"ERROR: wrong numerator name !";return 1;}

	TString _fname;
	if(option == ""){_fname = Form("InvM2_Pt%0.f_Pt%0.f_",Pt_low,Pt_upp);}
	else if(option == "matching"){_fname = Form("InvM2_Matched_Pt%0.f_Pt%0.f_",Pt_low,Pt_upp);}
	else{cout<<"ERROR: wrong option !";return 1;}

	_fname += _effcut+"_for_"+_sel+"_"+pname; 
	//_fname += effcut+"_"+pname; 
	
	//Store the result


	//Create folder to store background fitting
	mkdir(_path+_fname+"test_FIT_PDF/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	//Name of the input file
	TFile* f = new TFile(_path+_fname+".root","read");

	TFile* file_out = new TFile(_path+_fname+"_test_FIT"+".root","recreate");
			
	//Declaration of the histograms
	//eta < 1.2
	TH1D **histo_DY = new TH1D*[nptbins+1];
	TH1D **histo_BKG = new TH1D*[nptbins+1];
	TH1D **histo_DY_fail = new TH1D*[nptbins+1];
	TH1D **histo_BKG_fail = new TH1D*[nptbins+1];

	TH1D *eff = new TH1D("eff_BKG_fail","eff",nptbins,Pt_low,Pt_upp);
	//eta > 1.2
	TH1D **histo_DY_ = new TH1D*[nptbins+1];
	TH1D **histo_BKG_ = new TH1D*[nptbins+1];
	TH1D **histo_DY_fail_ = new TH1D*[nptbins+1];
	TH1D **histo_BKG_fail_ = new TH1D*[nptbins+1];

	TH1D *eff_ = new TH1D("eff_BKG_fail_","eff",nptbins,Pt_low,Pt_upp);

	//Efficiency
	//In order to compute the efficiency, one needs to fill the following variables
	double EffB[100];
	double EffE[100];
	double error_effB[100];
	double error_effE[100];
	double PT[100];
	double error_pt[100];

	
	//Starting the loop that read the hisograms and do the fit
	for(int i =0; i<nptbins;++i){

		//eta < 1.2
		char _DY[200];
		char _BKG[200];
		char _DY_fail[200];
		char _BKG_fail[200];
		//eta > 1.2
		char _DY_[200];
		char _BKG_[200];
		char _DY_fail_[200];
		char _BKG_fail_[200];

		//Define the Pt range of each histogram
		double Pt1;
		double Pt2;
		if(i*Dpt+Pt_low < Pt_upp){Pt1 = i*Dpt+Pt_low; Pt2 = (i+1)*Dpt+Pt_low;}
		else if (i*Dpt+Pt_low >= Pt_upp){Pt1 = i*Dpt+Pt_low; Pt2 = 10000;}

		//Fill the eff variables
		
	        PT[i] = (Pt2+Pt1)/2.;
		error_pt[i] = (Pt2-Pt1)/2.;

		//eta < 1.2
		sprintf(_DY,"histo_M_DYJets_bkg_loweta_Pt%0.f_Pt%0.f;1",Pt1,Pt2);
		sprintf(_BKG,"histo_M_bkg_loweta_Pt%0.f_Pt%0.f;1",Pt1,Pt2);
		sprintf(_DY_fail,"histo_M_DYJets_bkg_fail_loweta_Pt%0.f_Pt%0.f;1",Pt1,Pt2);
		sprintf(_BKG_fail,"histo_M_bkg_fail_loweta_Pt%0.f_Pt%0.f;1",Pt1,Pt2);
		//eta > 1.2
		sprintf(_DY_,"histo_M_DYJets_bkg_higheta_Pt%0.f_Pt%0.f;1",Pt1,Pt2);
		sprintf(_BKG_,"histo_M_bkg_higheta_Pt%0.f_Pt%0.f;1",Pt1,Pt2);
		sprintf(_DY_fail_,"histo_M_DYJets_bkg_fail_higheta_Pt%0.f_Pt%0.f;1",Pt1,Pt2);
		sprintf(_BKG_fail_,"histo_M_bkg_fail_higheta_Pt%0.f_Pt%0.f;1",Pt1,Pt2);

		//eta > 1.2
		histo_DY[i] = (TH1D*)f->Get(_DY);
		histo_BKG[i] = (TH1D*)f->Get(_BKG);
		histo_DY_fail[i] = (TH1D*)f->Get(_DY_fail);
		histo_BKG_fail[i] = (TH1D*)f->Get(_BKG_fail);
		//eta > 1.2
		histo_DY_[i] = (TH1D*)f->Get(_DY_);
		histo_BKG_[i] = (TH1D*)f->Get(_BKG_);
		histo_DY_fail_[i] = (TH1D*)f->Get(_DY_fail_);
		histo_BKG_fail_[i] = (TH1D*)f->Get(_BKG_fail_);

		//eta >1.2
		histo_DY[i]->SetName(_DY);
		histo_BKG[i]->SetName(_BKG);
		histo_DY_fail[i]->SetName(_DY_fail);
		histo_BKG_fail[i]->SetName(_BKG_fail);
		//eta <1.2
  	 	histo_DY_[i]->SetName(_DY_);
		histo_BKG_[i]->SetName(_BKG_);
		histo_DY_fail_[i]->SetName(_DY_fail_);
		histo_BKG_fail_[i]->SetName(_BKG_fail_);
		
		cout<<"This is the bin from "<<Pt1<<" to "<<Pt2<<endl;
	        TCanvas* c1 = new TCanvas("c1","c1");
		c1->Divide(1,2);
		c1->cd(1);
		cout<<"Barrel "<<endl;
		double passB = FitInvMassBkg_v3(histo_DY[i],histo_BKG[i],_sig);
		c1->cd(2);
		double failB = FitInvMassBkg_v3(histo_DY_fail[i],histo_BKG_fail[i],_sig);
	        TCanvas* c2 = new TCanvas("c2","c2");
		c2->Divide(1,2);
		c2->cd(1);
		cout<<"Endcape"<<endl;
		double passE = FitInvMassBkg_v3(histo_DY_[i],histo_BKG_[i],_sig);
		c2->cd(2);
		double failE = FitInvMassBkg_v3(histo_DY_fail_[i],histo_BKG_fail_[i],_sig);

		//Compute and fill the efficiency

		EffB[i] = passB/(passB+failB);
		error_effB[i] = BinomError(passB+failB,EffB[i]);
		EffE[i] = passE/(passE+failE);
		error_effE[i] = BinomError(passE+failE,EffE[i]);

		eff->Fill(PT[i],EffB[i]);
		eff->SetBinError(i,error_effB[i]);
		eff_->Fill(PT[i],EffE[i]);
		eff_->SetBinError(i,error_effE[i]);

		mkdir(_path+_fname+"_test_FIT_PDF/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		TString _cname = Form(_path+_fname+"_test_FIT_PDF/InvM_Pt%0.f_Pt%0.f",Pt1,Pt2);
		_cname += "_"+effcut+"_"+pname;
		TString _c1name = _cname+ "_eta<1.2.pdf";
		TString _c2name = _cname+ "_eta>1.2.pdf";
		c1->SaveAs(_c1name);
		c2->SaveAs(_c2name);

	}

	//Declaration of graph for the efficiency
	//Barrel
	TGraphErrors* effB = new TGraphErrors(nptbins,PT,EffB,error_pt,error_effB);
	effB->Draw("A");
	effB->SetMarkerStyle(4);
	effB->SetMarkerSize(0.4);
	effB->SetMarkerColor(4);
	//effB->SetLineColor(4);
	effB->GetXaxis()->SetTitle("P_{t}");
	effB->GetXaxis()->SetRangeUser(0,250);
	effB->GetYaxis()->SetRangeUser(0,1.1);
	effB->GetYaxis()->SetTitle(" #epsilon ");
	effB->GetXaxis()->SetTitle("P_{t}");
	//Endcape
	TGraphErrors* effE = new TGraphErrors(nptbins,PT,EffE,error_pt,error_effE);
	effE->Draw("A");
	effE->SetMarkerStyle(4);
	effE->SetMarkerSize(0.4);
	effE->SetMarkerColor(4);
	//effE->SetLineColor(4);
	effE->GetXaxis()->SetTitle("P_{t}");
	effE->GetYaxis()->SetRangeUser(0,1.1);
	effE->GetYaxis()->SetTitle(" #epsilon ");
	effE->GetXaxis()->SetTitle("P_{t}");

	TString _g1name = _path+_fname+"_test_FIT_PDF/Eff";
	TString _g2name = _path+_fname+"_test_FIT_PDF/Eff";

	TString _g3name = _path+_fname+"_test_FIT_PDF/Eff_histo";
	TString _g4name = _path+_fname+"_test_FIT_PDF/Eff_histo";

	_g1name += "_"+effcut+"_"+pname+"_eta<1.2.pdf";
	_g2name += "_"+effcut+"_"+pname+"_eta>1.2.pdf";

	_g3name += "_"+effcut+"_"+pname+"_eta<1.2.pdf";
	_g4name += "_"+effcut+"_"+pname+"_eta>1.2.pdf";


	TCanvas* b1 = new TCanvas("b1","b1");
	TCanvas* b2 = new TCanvas("b2","b2");
	TCanvas* d1 = new TCanvas("d1","d1");
	TCanvas* d2 = new TCanvas("d2","d2");

	b1->cd();
	effB->Draw();
	b1->SaveAs(_g1name);
	b2->cd();
	effE->Draw();
	b2->SaveAs(_g2name);

	d1->cd();
	eff->Draw();
	d1->SaveAs(_g3name);
	d2->cd();
	eff_->Draw();
	d2->SaveAs(_g4name);


	effB->Write("Eff_"+effcut+"_"+pname+"_eta<1.2");
	effE->Write("Eff_"+effcut+"_"+pname+"_eta>1.2");

	eff->Write("Eff_histo_"+effcut+"_"+pname+"_eta<1.2");
	eff_->Write("Eff_histo_"+effcut+"_"+pname+"_eta>1.2");

	f->Close();
	file_out->Close();
}
