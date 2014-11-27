/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

 * File Name : FitBkg.C

 * Purpose : This file do a fit of the background.

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
#include "RooLandau.h"
#include "RooGaussian.h"
#include "RooFFTConvPdf.h"
#include "RooNumConvPdf.h"
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

using namespace RooFit;

void FitBkg(TH1D* histo, TString _write, int I = 1){

	int n = histo->GetEntries();
	double w = histo->GetXaxis()->GetBinWidth(1);
	int ndf;

	RooPlot* frame;

	double hmin0 = histo->GetXaxis()->GetXmin();
	double hmax0 = histo->GetXaxis()->GetXmax();

	histo->GetXaxis()->SetRangeUser(hmin0,hmax0);

	// Declare observable x
	RooRealVar x("x","x",hmin0,hmax0) ;
	RooDataHist dh("dh","dh",x,Import(*histo)) ;

	frame = x.frame(Title(histo->GetName())) ;
	dh.plotOn(frame,DataError(RooAbsData::SumW2), MarkerColor(1),MarkerSize(0.9),MarkerStyle(7)); 
	dh.statOn(frame);  

	x.setRange("R0",0,200) ;

	//Defining the fitting functions

	if(I == 1){
		//This doesn't work because FFTW3 has be be installed and working with root

		//Landau (X) Gauss
		RooRealVar resp_mean("resp_mean","resp_mean",1,0,20) ;
		RooRealVar resp_sigma("resp_sigma","resp_sigma",1,0,30) ;
		RooGaussian resp("resp","gauss",x,resp_mean,resp_sigma) ;

		RooRealVar mean("mean","mean",40,0,200) ;
		RooRealVar sigma("sigma","sigma",5,0,50) ;
		RooLandau Land("Land","Background",x,mean,sigma);
		x.setBins(10000,"cache") ;

		RooFFTConvPdf bkg("bkg","Background",x,Land,resp);

		//Fitting
		RooFitResult* filters = bkg.fitTo(dh,Range("R0"),"qr");
		bkg.plotOn(frame,LineColor(2));
		bkg.paramOn(frame); 
	}

	else if(I == 2){
		//Landau

		RooRealVar mean("mean","mean",40,10,70) ;
		RooRealVar sigma("sigma","sigma",10,0,20) ;

		RooLandau bkg("bkg","Background",x,mean,sigma);

		//Fitting
		RooFitResult* filters = bkg.fitTo(dh,Range("R0"),"qr");
		bkg.plotOn(frame,LineColor(2));
		bkg.paramOn(frame);}

	else if(I == 3){
		//Chebychev
		RooRealVar a0("a0","a0",1,-2,1) ;
		RooRealVar a1("a1","a1",0.1,-1,1) ;
		RooRealVar a2("a2","a2",-0.1,-1,1) ;

		a0.setRange(-1.5,1.);
		a1.setRange(-1.,1.);
		a2.setRange(-1.,1.);

		RooChebychev bkg("bkg","Background",x,RooArgSet(a0,a1));

		//Fitting
		RooFitResult* filters = bkg.fitTo(dh,Range("R0"),"qr");
		bkg.plotOn(frame,LineColor(2));
		bkg.paramOn(frame); 
	}

	frame->GetXaxis()->SetTitle("Z mass (in GeV/c^{2})");  
	frame->GetXaxis()->SetTitleOffset(1.2);
	float binsize = histo->GetBinWidth(1); 

	//Store result in .root file
	frame->Write(_write);

}
