#include "RooRealVar.h"
#include "RooBreitWigner.h"
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

vector<double> FitInvMass(TH1D* histo){

	vector<double> vec;

	gROOT->ProcessLine(".x ~/rootlogon.C");

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


	x.setRange("R0",90.5,91) ;
	x.setRange("R1",70,110) ;
	x.setRange("R2",60,120) ;
	x.setRange("R3",50,130) ;

	RooRealVar mean("mean","mean",91.186/*histo->GetMean()*/, 70.0, 120.0);
	RooRealVar width("width","width",7.5, 0, 30.0);
	RooRealVar sigma("sigma","sigma",0, 0.0, 120.0);


	mean.setRange(88,94);
	width.setRange(0,20);
	sigma.setRange(0,10);

	//Choose the fitting here
	//RooGaussian gauss("gauss","gauss",x,mean,sigma);ndf = 2;
	RooBreitWigner gauss("gauss","gauss",x,mean,width);ndf = 2;
	//RooVoigtian gauss("gauss","gauss",x,mean,width,sigma); ndf = 3;
	
	RooFitResult* filters = gauss.fitTo(dh,Range("R1"),"qr");
	gauss.plotOn(frame,LineColor(4));//this will show fit overlay on canvas
	gauss.paramOn(frame); //this will display the fit parameters on canvas

	//TCanvas* b1 = new TCanvas("b1","b1",1200,800);

	//gPad->SetLeftMargin(0.15);

	//frame->GetXaxis()->SetTitle("Z mass (in GeV/c^{2})");  
	//frame->GetXaxis()->SetTitleOffset(1.2);
	//float binsize = histo->GetBinWidth(1); 
	//frame->Draw() ;
	cout<<"The chi2 is:"<<endl;
	cout<<frame->chiSquare(ndf)<<endl; 
	cout<<" "<<endl;

	//Do the integral

	//Store result in .root file
	frame->Write(histo->GetTitle());

	RooAbsReal* integral = gauss.createIntegral(x, NormSet(x), Range("R1")) ;

	vec.push_back(n*integral->getVal());
	//vec.push_back((double)n);
	vec.push_back((double)frame->chiSquare(ndf));
	
	return vec;
}
