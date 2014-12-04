/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : FitData.C 

* Purpose :

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By :

_._._._._._._._._._._._._._._._._._._._._.*/

#include "TString.h"
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include <stdio.h>
#include <sys/stat.h>
#include <iostream>

#include "FitHelper.C"
#include "FitInvMassBkg_v3.C"




void FitData(int leptonId, double Pt_low, double Pt_upp = 9999, int nptbins =10, TString effcut = "", TString _sig = "CB"){

	//setTDRStyle();
	
	//Path for input and output file. Written in FitDataPath.txt
	ifstream file("FitDataPath.txt");
	string str;
	getline(file,str);
	TString _path = str;

	//Some variables
	double Dpt = (Pt_upp-Pt_low)/nptbins;

	TString pname;
	if(leptonId == 11){pname = "e";}
	if(leptonId == 13){pname = "mu";}

	TString _fname = Form("InvM_Pt_DYnWnonInc_Pt%0.f_Pt%0.f_",Pt_low,Pt_upp);
	_fname += effcut+"_"+pname; 


	//Create folder to store background fitting
	mkdir(_path+_fname+"_FIT_PDF/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	//Name of the input file
	TFile* f = new TFile(_path+_fname+".root","read");
			
	//Declaration of the histograms
	//eta < 1.2
	TH1D **histo_DY = new TH1D*[nptbins+1];
	TH1D **histo_BKG = new TH1D*[nptbins+1];
	TH1D **histo_DY_fail = new TH1D*[nptbins+1];
	TH1D **histo_BKG_fail = new TH1D*[nptbins+1];
	//eta > 1.2
	TH1D **histo_DY_ = new TH1D*[nptbins+1];
	TH1D **histo_BKG_ = new TH1D*[nptbins+1];
	TH1D **histo_DY_fail_ = new TH1D*[nptbins+1];
	TH1D **histo_BKG_fail_ = new TH1D*[nptbins+1];

	TFile* file_out = new TFile(_path+_fname+"_FIT"+".root","recreate");

	//Starting the loop that read the hisograms and do the fit
	for(int i =0; i<nptbins+1;++i){

		//Read the input root file and retrieve histograms
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


		//Do the rebinning


		////eta > 1.2
		//histo_DY[i] =(TH1D*) Rebin(histo_DY[i]);
		//histo_BKG[i]=(TH1D*) Rebin(histo_BKG[i]);
		//histo_DY_fail[i] =(TH1D*) Rebin(histo_DY_fail[i]);
		//histo_BKG_fail[i] =(TH1D*) Rebin(histo_BKG_fail[i]);
		////eta > 1.2
		//histo_DY_[i] =(TH1D*) Rebin(histo_DY_[i]);
		//histo_BKG_[i] =(TH1D*) Rebin(histo_BKG_[i]);
		//histo_DY_fail_[i] =(TH1D*) Rebin(histo_DY_fail_[i]);
		//histo_BKG_fail_[i] =(TH1D*) Rebin(histo_BKG_fail_[i]);

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

	        TCanvas* c1 = new TCanvas("c1","c1");
		c1->Divide(1,2);
		c1->cd(1);
		FitInvMassBkg_v3(histo_DY[i],histo_BKG[i],_sig);
		c1->cd(2);
		FitInvMassBkg_v3(histo_DY_fail[i],histo_BKG_fail[i],_sig);
	        TCanvas* c2 = new TCanvas("c2","c2");
		c2->Divide(1,2);
		c2->cd(1);
		FitInvMassBkg_v3(histo_DY_[i],histo_BKG_[i],_sig);
		c2->cd(2);
		FitInvMassBkg_v3(histo_DY_fail_[i],histo_BKG_fail_[i],_sig);

		mkdir(_path+_fname+"_FIT_PDF/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		TString _cname = Form(_path+_fname+"_FIT_PDF/InvM_Pt%0.f_Pt%0.f",Pt1,Pt2);
		_cname += "_"+effcut+"_"+pname;
		TString _c1name = _cname+ "_eta<1.2.pdf";
		TString _c2name = _cname+ "_eta>1.2.pdf";
		c1->SaveAs(_c1name);
		c2->SaveAs(_c2name);

		//Uncomment the following to check if the files are plotted correctly
		//mkdir(_path+_fname+"_TEST/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		//TString _cname = Form(_path+_fname+"_TEST/NOREBINInvM_Pt%0.f_Pt%0.f",Pt1,Pt2);

		//TCanvas* c1 = new TCanvas("c1","c1");
		//c1->Divide(1,2);
		//TCanvas* c2 = new TCanvas("c2","c2");
		//c2->Divide(1,2);
		//TCanvas* c3 = new TCanvas("c3","c3");
		//c3->Divide(1,2);
		//TCanvas* c4 = new TCanvas("c4","c4");
		//c4->Divide(1,2);

		//Rebin(histo_BKG[i]);
		//Rebin(histo_BKG_fail[i]);
		//Rebin(histo_DY_[i]);
		//Rebin(histo_DY_fail_[i]);
		//Rebin(histo_BKG_[i]);
		//Rebin(histo_BKG_fail_[i]);
		//eta > 1.2
		//c1->cd(1);
	        //Rebin(histo_DY[i])->Draw();
		//c1->cd(2);
		//NormToWidth(histo_DY[i])->Draw();
		//histo_DY_fail[i]->Draw("same");
		//c2->cd(1);
		//histo_BKG[i]->Draw();
		//c2->cd(2);
		//histo_BKG_fail[i]->Draw("same");
		////eta > 1.2
		//c3->cd(1);
		//histo_DY_[i]->Draw();
		//c3->cd(2);
		//histo_DY_fail_[i]->Draw("same");
		//c4->cd(1);
		//histo_BKG_[i]->Draw();
		//c4->cd(2);
		//histo_BKG_fail_[i]->Draw("same");	

		//_cname += "_"+effcut+"_"+pname;
		//TString _c1name = _cname+ "_Zbkg_eta<1.2.pdf";
		//TString _c2name = _cname+ "_bkg_eta<1.2.pdf";
		//TString _c3name = _cname+ "_Zbkg_eta>1.2.pdf";
		//TString _c4name = _cname+ "_bkg_eta>1.2.pdf";

		//c1->SaveAs(_c1name);
		//c2->SaveAs(_c2name);
		//c3->SaveAs(_c3name);
		//c4->SaveAs(_c4name);

	}

	f->Close();
	file_out->Close();
}
