/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : FitIncMassBkg.C

* Purpose : Does the same thing as the FitInvMass, which means that it fit the invariant mass spectra of a lepton pair. The difference is that here the background is considered as well.

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By : Gaël L. Perrin

_._._._._._._._._._._._._._._._._._._._._.*/

#include "RooRealVar.h"
#include "RooLandau.h"
#include "RooBreitWigner.h"
#include "RooFFTConvPdf.h"
#include "RooCBShape.h"
#include "RooAddPdf.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooVoigtian.h"
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

int FitInvMassBkg(TH1D* histo, TString _write, TString signal = "BWxGau", TString option = ""){

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
	dh.plotOn(frame,DataError(RooAbsData::SumW2), MarkerColor(1),MarkerSize(0.9),MarkerStyle(7));  //this will show histogram data points on canvas
	dh.statOn(frame);  //this will display hist stat on canvas

	x.setRange("R0",0,200) ;
	//Define fit function 
	//
	
	//fsig for adding two funciton i.e. F(x) = fsig*sig(x) + (1-fsig)*bkg(x)
	RooRealVar fsig("fsig","sigal fraction",0., 0., 1.);//before 0.9

	//Signal fitting function
	RooRealVar mean("mean","mean",91.186, 70.0, 120.0);
	RooRealVar width("width","width",7.5, 0, 30.0);
	RooRealVar sigma("sigma","sigma",1, 0., 120.0);
	RooRealVar alpha("alpha","alpha",5, 0., 7);
	RooRealVar ncb("ncb","ncb",2, 0, 10);

	mean.setRange(88,94);
	width.setRange(0,20);
	sigma.setRange(0.5,10);
	//fsig.setConstant(kTRUE);
	//alpha.setConstant(kTRUE);

	RooVoigtian sig_bwgau("sig_bwgau","BWxgauss",x,mean,width,sigma);
	RooBreitWigner sig_bw("sig_bw","BW",x,mean,width);
	RooGaussian sig_gau("sig_gau","gauss",x,mean,sigma);
	RooCBShape sig_cb("sig_CB", "Crystal Ball",x,mean,sigma,alpha,ncb);

	//NB: The CrystalBall shape is Gaussian that is 'connected' to an exponential taill at 'alpha' sigma of the Gaussian. The sign determines if it happens on the left or right side. The 'n' parameter control the slope of the exponential part. 

	RooAbsPdf* sig;
	if(signal == "BWxGau"){sig = &sig_bwgau;}
	else if(signal == "BW"){sig = &sig_bw;}
	else if(signal == "Gau"){sig = &sig_gau;}
	else if(signal == "CB"){sig = &sig_cb;}
	else{ cout<<"Wrong signal function name"<<endl;
		return 1;
	}

	//Background fitting function
	//Landau (X) Gauss
	RooRealVar resp_mean("resp_mean","resp_mean",0.,0.,20) ;
	RooRealVar resp_sigma("resp_sigma","resp_sigma",20,5.,30) ;
	RooGaussian resp_bkg("resp","gauss",x,resp_mean,resp_sigma) ;

	resp_mean.setConstant(kTRUE);
	resp_sigma.setRange(5,30);
	
	RooRealVar mean_bkg("mean_bkg","mean",40,30,70) ;
	RooRealVar sigma_bkg("sigma_bkg","sigma",10,0,20) ;

	sigma_bkg.setRange(0.,20);

	RooLandau Land_bkg("Land","Background",x,mean_bkg,sigma_bkg);


	x.setBins(10000,"cache") ;

	RooFFTConvPdf bkg("bkg","Background",x,Land_bkg,resp_bkg);

	//Adding the two functions
	RooAddPdf model("model","Signal+Background", RooArgList(*sig,bkg),fsig);

	setfreestyle();
	 
	//Perform the fit
	RooAbsPdf* fit_func;
	if(option == ""){fit_func = &model;}
	else if(option == "sigonly"){fit_func = sig;}
	
	RooFitResult* filters = fit_func->fitTo(dh,Range("R1"),"qr");
	fit_func->plotOn(frame);
	if(option == ""){fit_func->plotOn(frame,Components(bkg),LineStyle(kDashed));}
	fit_func->paramOn(frame); 

	frame->SetTitle("Invariant mass for DY, TTJets and Wjets");  
	frame->GetXaxis()->SetTitle("m (in GeV/c^{2})");  
	frame->GetXaxis()->SetTitleOffset(1.2);
	float binsize = histo->GetBinWidth(1); 

	//Store result in .root file
	frame->SetName(_write);
	frame->Write();
	
	return 0;

}
