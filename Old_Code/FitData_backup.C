/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : FitData.C 

* Purpose :

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By :

_._._._._._._._._._._._._._._._._._._._._.*/

#include "RooRealVar.h"
#include "RooLandau.h"
#include "RooBreitWigner.h"
#include "RooFFTConvPdf.h"
#include "RooNovosibirsk.h"
#include "RooCBShape.h"
#include "RooAddPdf.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooChebychev.h"
#include "RooGaussian.h"
#include "RooVoigtian.h"
#include "RooPlot.h"
#include "fstream"
#include "TString.h"
#include "TROOT.h"
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "vector"
#include <stdio.h>
#include <sys/stat.h>
#include <iostream>

#include "setTDRStyle.C"

//#include "FitHelper.C"
//#include "FitInvMassBkg_v3.C"

using namespace RooFit;


vector<double> FitBkg(TH1D* histo, TString _bkg = "Novo");

int FitInvMassBkg(TH1D* histo, TH1D* histo_bkg, TString signal = "CBxBW",TString _bkg = "Cheb");

void FitData(int leptonId, double Pt_low, double Pt_upp = 9999, int nptbins =10, TString effcut = "", TString _sig = "CB"){

	//setTDRStyle();
	
	//Path for input and output file. Written in FitDataPath.txt
	ifstream file("FitDataPath.txt");
	string str;
	getline(file,str);
	TString _path = str;

	//Some variables
	double Dpt = (Pt_upp-Pt_low)/nptbins;

	cout<<"Hello"<<endl;

	TString pname;
	if(leptonId == 11){pname = "e";}
	if(leptonId == 13){pname = "mu";}

	TString _fname = Form("InvM_Pt_DYnWnonInc_Pt%0.f_Pt%0.f_",Pt_low,Pt_upp);
	_fname += effcut+"_"+pname; 


	//Create folder to store background fitting
	mkdir(_path+_fname+"_FIT_PDF/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	cout<<"1"<<endl;

	//Name of the input file
	TFile* f = new TFile(_path+_fname+".root","read");

	cout<<"2"<<endl;
			
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

	cout<<"3"<<endl;

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

		f->GetListOfKeys()->ls();

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

	        cout<<"3.5"<<endl;
		//eta >1.2
		histo_DY[i]->SetName(_DY);
	        cout<<"3.75"<<endl;
		histo_BKG[i]->SetName(_BKG);
		histo_DY_fail[i]->SetName(_DY_fail);
		histo_BKG_fail[i]->SetName(_BKG_fail);
		//eta <1.2
  	 	histo_DY_[i]->SetName(_DY_);
		histo_BKG_[i]->SetName(_BKG_);
		histo_DY_fail_[i]->SetName(_DY_fail_);
		histo_BKG_fail_[i]->SetName(_BKG_fail_);

	        cout<<"4"<<endl;

	        TCanvas* c1 = new TCanvas("c1","c1");
		c1->Divide(1,2);
		c1->cd(1);
		FitInvMassBkg(histo_DY[i],histo_BKG[i],_sig);
		cout<<"5"<<endl;
		c1->cd(2);
		FitInvMassBkg(histo_DY_fail[i],histo_BKG_fail[i],_sig);
	        TCanvas* c2 = new TCanvas("c2","c2");
		c2->Divide(1,2);
		c2->cd(1);
		FitInvMassBkg(histo_DY_[i],histo_BKG_[i],_sig);
		c2->cd(2);
		FitInvMassBkg(histo_DY_fail_[i],histo_BKG_fail_[i],_sig);


		mkdir(_path+_fname+"_FIT_PDF/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		TString _cname = Form(_path+_fname+"_FIT_PDF/InvM_Pt%0.f_Pt%0.f",Pt1,Pt2);
		_cname += "_"+effcut+"_"+pname;
		TString _c1name = _cname+ "_eta<1.2.pdf";
		TString _c2name = _cname+ "_eta>1.2.pdf";
		c1->SaveAs(_c1name);
		c2->SaveAs(_c2name);

	        cout<<"5"<<endl;

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

int FitInvMassBkg(TH1D* histo, TH1D* histo_bkg, TString signal,TString _bkg){

	//Set Style
	setTDRStyle();

	cout<<"21"<<endl;

	//Path for input and output file. Written in FitDataPath.txt
	ifstream file("FitDataPath.txt");
	string str;
	getline(file,str);
	TString _path = str;

	//Rebin(histo);

	//Getting info about the histogram to fit
	int n = histo->GetEntries();
	double w = histo->GetXaxis()->GetBinWidth(1);
	int ndf;
	double hmin0 = histo->GetXaxis()->GetXmin();
	double hmax0 = histo->GetXaxis()->GetXmax();
	histo->GetXaxis()->SetRangeUser(hmin0,hmax0);

	//Declare observable x
	//Try to rebin using this. Doesn't work for now
        //RooBinning xbins = Rebin2(histo);
	RooRealVar x("x","x",hmin0,hmax0) ;
	RooDataHist dh("dh","dh",x,Import(*histo)) ;

	//Define the frame
	RooPlot* frame;
	frame = x.frame();
	dh.plotOn(frame,DataError(RooAbsData::SumW2), MarkerColor(1),MarkerSize(0.9),MarkerStyle(7));  //this will show histogram data points on canvas

	//x.setRange("R0",0,200) ;
	x.setRange("R1",55,200) ;

        /////////////////////
	//Define fit function 
	/////////////////////
	
	//fsig for adding two funciton i.e. F(x) = fsig*sig(x) + (1-fsig)*bkg(x)
	RooRealVar fsig("fsig","sigal fraction",0.5, 0., 1.);//before 0.9

	//Various parameters
	
	//True mean
	RooRealVar mean("mean","PDG mean of Z",91.186);//, 70.0, 120.0);
	//For the BW
	RooRealVar width("width","PDG width of Z",2.4952);//, 0., 5.);
	//For the Gauss and the CB alone 
	RooRealVar sigma("sigma","sigma",1, 0., 10.);
	RooRealVar alpha("alpha","alpha",0.7, 0., 7);
	RooRealVar ncb("ncb","ncb",7, 0, 150);
	//For the CB used for convolution, i.e. CBxBW
	RooRealVar cb_bias("cb_bias","bias",0, -3.,3.);
	RooRealVar cb_sigma("cb_sigma","response",1, 0.,5);
	RooRealVar cb_alpha("cb_alpha","alpha",1.,0.,7);
	RooRealVar cb_ncb("cb_ncb","ncb",2, 0, 10);

	mean.setRange(88,94);
	width.setRange(0,20);
	sigma.setRange(0.5,10);
	//fsig.setConstant(kTRUE);
	//alpha.setConstant(kTRUE);

	RooVoigtian sig_bwgau("sig_bwgau","BWxgauss",x,mean,width,sigma);
	RooBreitWigner sig_bw("sig_bw","BW",x,mean,width);
	RooGaussian sig_gau("sig_gau","gauss",x,mean,sigma);
	RooCBShape sig_cb("sig_cb", "Crystal Ball",x,mean,sigma,alpha,ncb);
	RooCBShape sig_cb_resp("sig_cb_resp", "Crystal Ball for conv.",x,cb_bias,cb_sigma,cb_alpha,cb_ncb);

	x.setBins(10000,"cache");
	RooFFTConvPdf sig_cbbw("sig_cbbw","CBxBW",x,sig_cb_resp,sig_bw);

	//NB: The CrystalBall shape is Gaussian that is 'connected' to an exponential taill at 'alpha' sigma of the Gaussian. The sign determines if it happens on the left or right side. The 'n' parameter control the slope of the exponential part. 

	RooAbsPdf* sig;
	if(signal == "BWxGau"){sig = &sig_bwgau;}
	else if(signal == "BW"){sig = &sig_bw;}
	else if(signal == "Gau"){sig = &sig_gau;}
	else if(signal == "CB"){sig = &sig_cb;}
	else if(signal == "CBxBW"){ sig = &sig_cbbw;}
	else{ cout<<"Wrong signal function name"<<endl;
		return 1;
	}

	/////////////////////////////
	//Background fitting function
	/////////////////////////////
	
	//Get the initial parameter of the background
	//


	vector<double> vec = FitBkg(histo_bkg,_bkg);

	//Chebychev
	RooRealVar a0("a0","a0",vec[0],-5.,0.) ;
	RooRealVar a1("a1","a1",vec[1],-2,1.2) ;
	RooRealVar a2("a2","a2",vec[2],-1.,1.) ;
	RooRealVar a3("a3","a3",vec[3],-2.5,0.) ;
	RooRealVar a4("a4","a4",vec[4],-1.,1.) ;
	RooRealVar a5("a5","a5",vec[5],-1.,1.) ;
	RooRealVar a6("a6","a6",vec[6],-1.,1.) ;

	RooChebychev bkg_cheb("bkg","Background",x,RooArgSet(a0,a1,a2,a3,a4,a5,a6));

	//Novo

	RooRealVar peak_bkg("peak_bkg","peak",vec[0],0,250);
	RooRealVar width_bkg("width_bkg","width",vec[1],0,1) ;
	RooRealVar tail_bkg("tail_bkg","tail",vec[2],0,10) ;

	RooNovosibirsk bkg_nov("bkg","Background",x,peak_bkg,width_bkg,tail_bkg);

	RooAbsPdf* bkg;

	if(_bkg == "Cheb"){bkg = &bkg_cheb;}
	if(_bkg == "Novo"){bkg = &bkg_nov;}


	//////////////////////////
	//Adding the two functions
	//////////////////////////
	
	RooAddPdf model("model","Signal+Background", RooArgList(*sig,*bkg),fsig);
	 
	//Perform the fit
	RooAbsPdf* fit_func;
	fit_func = &model;
	
	RooFitResult* filters = fit_func->fitTo(dh,Range("R1"),"qr");
	fit_func->plotOn(frame);
	fit_func->plotOn(frame,Components(*sig),LineStyle(kDashed),LineColor(kRed));
	fit_func->plotOn(frame,Components(*bkg),LineStyle(kDashed),LineColor(kGreen));
	frame->Draw();
	fit_func->paramOn(frame); 
	dh.statOn(frame);  //this will display hist stat on canvas

	frame->SetTitle(histo->GetTitle());  
	frame->GetXaxis()->SetTitle("m (in GeV/c^{2})");  
	frame->GetXaxis()->SetTitleOffset(1.2);
	float binsize = histo->GetBinWidth(1); 

	//Store result in .root file
	frame->SetName(histo->GetName());
	frame->Write();
	
	return 0;

}

vector<double> FitBkg(TH1D* histo, TString _bkg ){

	setTDRStyle();

	vector<double> vec;

	int n = histo->GetEntries();
	double w = histo->GetXaxis()->GetBinWidth(1);
	int ndf;

	RooPlot* frame;

	double hmin0 = histo->GetXaxis()->GetXmin();
	double hmax0 = histo->GetXaxis()->GetXmax();

	histo->GetXaxis()->SetRangeUser(hmin0,hmax0);

	// Declare observable x
	RooRealVar x("x","x",hmin0,hmax0) ;
	RooDataHist dh("dh","dh",x,Import(*histo,kFALSE)) ;

	frame = x.frame(Title(histo->GetName())) ;
	dh.plotOn(frame,DataError(RooAbsData::SumW2), MarkerColor(1),MarkerSize(0.9),MarkerStyle(7)); 
	dh.statOn(frame);  

	x.setRange("R0",0,200) ;

	//Defining the fitting functions
	if(_bkg == "Novo"){
		//Novo

		RooRealVar peak("peak","peak",45.,0.,100.);
		RooRealVar width("width","width",20.,0.,40.) ;
		RooRealVar tail("tail","tail",0.01,0.,1.) ;


		RooNovosibirsk bkg("bkg","Background",x,peak,width,tail);

		//Fitting
		RooFitResult* filters = bkg.fitTo(dh/*,Range("R0"),"qr"*/);
		bkg.plotOn(frame,LineColor(2));
		bkg.paramOn(frame); 

		vec.push_back(peak.getVal());
		vec.push_back(width.getVal());
		vec.push_back(tail.getVal());
	}

	if(_bkg == "Cheb"){
		//Chebychev


		RooRealVar a0("a0","a0",-1,-5.,0.) ;
		RooRealVar a1("a1","a1",0,-2,1.2) ;
		RooRealVar a2("a2","a2",0,-1.,1.) ;
		RooRealVar a3("a3","a3",0,-2.5,0.) ;
		RooRealVar a4("a4","a4",0,-1.,1.) ;
		RooRealVar a5("a5","a5",0,-1.,1.) ;
		RooRealVar a6("a6","a6",0,-1.,1.) ;

		RooChebychev bkg("bkg","Background",x,RooArgSet(a0,a1,a2,a3,a4,a5,a6));

		//Fitting
		RooFitResult* filters = bkg.fitTo(dh,Range("R0"),"qr");
		bkg.plotOn(frame,LineColor(2));
		bkg.paramOn(frame); 

		vec.push_back(a0.getVal());
		vec.push_back(a1.getVal());
		vec.push_back(a2.getVal());
		vec.push_back(a3.getVal());
		vec.push_back(a4.getVal());
		vec.push_back(a5.getVal());
		vec.push_back(a6.getVal());

	}

	if(_bkg == "Land"){
		//Background fitting function
		//Landau (X) Gauss
		RooRealVar resp_mean("resp_mean","resp_mean",1,0.,20) ;
		RooRealVar resp_sigma("resp_sigma","resp_sigma",20,5,30) ;
		RooGaussian resp_bkg("resp","gauss",x,resp_mean,resp_sigma) ;

		RooRealVar mean_bkg("mean_bkg","mean",40,30,70) ;
		RooRealVar sigma_bkg("sigma_bkg","sigma",10,0,20) ;

		RooLandau Land_bkg("Land","Background",x,mean_bkg,sigma_bkg);

		sigma_bkg.setRange(30,50);
		resp_sigma.setRange(5,10);

		x.setBins(10000,"cache") ;

		RooFFTConvPdf bkg("bkg","Background",x,Land_bkg,resp_bkg);

		//Fitting
		RooFitResult* filters = bkg.fitTo(dh,Range("R0"),"qr");
	        frame->Draw();
		bkg.plotOn(frame,LineColor(2));
		bkg.paramOn(frame); 

		vec.push_back(0);
		vec.push_back(0);
		vec.push_back(0);

	}


	frame->GetXaxis()->SetTitle("Z mass (in GeV/c^{2})");  
	frame->GetXaxis()->SetTitleOffset(1.2);
	float binsize = histo->GetBinWidth(1); 

	//Store result in .root file
	frame->Write(histo->GetName());

	return vec;

}



