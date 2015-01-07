/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

 * File Name : FitBkg.C

 * Purpose : This file do a fit of the background. The parameter of this fit are then returned in a vector to be used by FitIntMassBkg_v3.C function
 
 * Used by: FitInvMassBkg

 * Creation Date : 20-12-2008

 * Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

 * Created By :

 _._._._._._._._._._._._._._._._._._._._._.*/

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooBreitWigner.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooChebychev.h"
#include "RooNovosibirsk.h"
#include "RooLandau.h"
#include "RooGaussian.h"
#include "RooFFTConvPdf.h"
#include "RooNumConvPdf.h"
#include "RooBinning.h"
#include "RooPlot.h"
#include "vector"
#include "cmath"
#include "iostream"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TString.h"
#include "TLegend.h"
#include "TLorentzVector.h"

#include "../setTDRStyle.C"


using namespace RooFit;

vector<double> FitBkg(TH1D* histo, TString _bkg = "Novo"){

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

	x.setRange("R0",10,200) ;

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
		RooRealVar a1("a1","a1",0,-2.5,1.2); 
		RooRealVar a2("a2","a2",0,-1.5,1.) ;
		RooRealVar a3("a3","a3",0,-3,1.) ;
		RooRealVar a4("a4","a4",0,-1.5,1.) ;
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


