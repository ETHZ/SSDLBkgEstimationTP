/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : FitIncMassBkg.C

* Purpose : Does a fit of the invariant mass+bkg. Returns the T&P efficiency of the "histo" bin.
            Improvement w.r.t. v3: Fits the bkg separatly to use as starting parameters for the fit.
 
* Used by:  FitData.C

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By : Gaël L. Perrin

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
#include "FitBkg.C"
#include "DrawInvMassBkg3.C"
#include <iostream>
#include <fstream>

using namespace RooFit;

double FitInvMassBkg_v3(TH1D* histo, TH1D* histo_bkg, TString signal = "CBxBW",TString _bkg = "Cheb", TString option = ""){


	//Set Style
	setTDRStyle();

	if(option.Contains("nentries")){return histo->GetEntries()-histo_bkg->GetEntries();}
	else{

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
	x.setRange("D",55,120) ;

        /////////////////////
	//Define fit function 
	/////////////////////
	cout<<"Debug4"<<endl;
	
	//fsig for adding two funciton i.e. F(x) = fsig*sig(x) + (1-fsig)*bkg(x)
	//RooRealVar fsig("fsig","sigal fraction",0.5, 0., 1.);
	RooRealVar nsig("nsig","signal events",histo->GetEntries()/2., 1,histo->GetEntries());
	RooRealVar nbkg("nbkg","background events",histo_bkg->GetEntries()/2.,1,histo_bkg->GetEntries());
	RooArgList pdfval(nsig,nbkg);

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
	//Gauss used for convolution
	RooRealVar gau_bias("gau_bias","alpha",0, -3., 3.);
	RooRealVar gau_sigma("gau_sigma","bias",1, 0., 7.);
	cout<<"Debug5"<<endl;

	mean.setRange(88,94);
	width.setRange(0,20);
	sigma.setRange(0.5,10);
	//fsig.setConstant(kTRUE);
	//alpha.setConstant(kTRUE);

	RooVoigtian sig_voigtian("sig_voigtian","Voigtian",x,mean,width,sigma);
	RooBreitWigner sig_bw("sig_bw","BW",x,mean,width);
	//RooGaussian sig_gau("sig_gau","gauss",x,mean,sigma);
	RooCBShape sig_cb("sig_cb", "Crystal Ball",x,mean,sigma,alpha,ncb);
	RooCBShape sig_cb_resp("sig_cb_resp", "Crystal Ball for conv.",x,cb_bias,cb_sigma,cb_alpha,cb_ncb);
	RooGaussian sig_gau_resp("sig_gau_resp", "Gaussian for conv.",x,gau_bias,gau_sigma);

	x.setBins(10000,"cache");
	RooFFTConvPdf sig_cbbw("sig_cbbw","CBxBW",x,sig_cb_resp,sig_bw);
	RooFFTConvPdf sig_bwgau("sig_bwgau","BWxGau",x,sig_bw,sig_gau_resp);

	//NB: The CrystalBall shape is Gaussian that is 'connected' to an exponential taill at 'alpha' sigma of the Gaussian. The sign determines if it happens on the left or right side. The 'n' parameter control the slope of the exponential part. 
	cout<<"Debug6"<<endl;

	RooAbsPdf* sig;
	if(signal == "Vo"){sig = &sig_bwgau;}
	else if(signal == "BW"){sig = &sig_bw;}
	//else if(signal == "Gau"){sig = &sig_gau;}
	else if(signal == "CB"){sig = &sig_cb;}
	else if(signal == "CBxBW"){ sig = &sig_cbbw;}
	else if(signal == "BWxGau"){ sig = &sig_bwgau;}
	else{ cout<<"Wrong signal function name"<<endl;
		return 1;
	}

	/////////////////////////////
	//Background fitting function
	/////////////////////////////
	
	//Get the initial parameter of the background
	//


	cout<<"Debug7"<<endl;
	vector<double> vec = FitBkg(histo_bkg,_bkg);

	//Chebychev
	RooRealVar a0("a0","a0",vec[0],-5.,0.) ;
	RooRealVar a1("a1","a1",vec[1],-2.5,1.2) ;
	RooRealVar a2("a2","a2",vec[2],-1.5,1.) ;
	RooRealVar a3("a3","a3",vec[3],-3,1.) ;
	RooRealVar a4("a4","a4",vec[4],-1.5,1.) ;
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
	
	//RooAddPdf model("model","Signal+Background", RooArgList(*sig,*bkg),fsig);
	RooAddPdf model("model","Signal+Background", RooArgList(*sig,*bkg),pdfval);
	 
	//Perform the fit
	RooAbsPdf* fit_func;
	fit_func = &model;
	
	RooFitResult* filters = fit_func->fitTo(dh,Range("R1"),"qr");
	fit_func->plotOn(frame);
	fit_func->plotOn(frame,Components(*sig),LineStyle(kDashed),LineColor(kRed));
	fit_func->plotOn(frame,Components(*bkg),LineStyle(kDashed),LineColor(kGreen));
	frame->SetAxisRange(50,120);
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

	///////////////////////
	//Plot the efficiency//
	///////////////////////
	
	//Old stuff
	
	//ofstream myfile;
	//myfile.open("/Users/GLP/Desktop/integrals.txt");
	//This one doesn't take the normalisation into account !
	//myfile<<"integral 1 "<<histo->GetName()<<endl;
	////Integral of sig
	//RooAbsReal* integral_sig = sig->createIntegral(x,x,"D") ;
	//myfile<<fsig.getVal()*integral_sig->getVal()<<endl;
	//Integral of sig+bkg
	//RooAbsReal* total = fit_func->createIntegral(x, NormSet(x), Range("D")) ;
	//Integral of sig only
	//RooAbsReal* background = bkg->createIntegral(x, NormSet(x), Range("D"));
	//cout<<"The total integral is"<<n*total->getVal();
	//cout<<"The bkg integral is"<<n*bkg->getVal();
	//cout<<"The bkg with the fsig is"<<fsig.getVal()*n*bkg->getVal();
	//cout<<"The returned value is"<<n*(total->getVal()-fsig.getVal()*background->getVal());
	//myfile<<"integral 2 "<<endl;
	//myfile<<n*(total->getVal()-(1-fsig.getVal())*background->getVal())<<endl;
	//myfile.close();
	//return n*(total->getVal()-(1-fsig.getVal())*background->getVal());
	
	return nsig.getVal();
	
	}
}

