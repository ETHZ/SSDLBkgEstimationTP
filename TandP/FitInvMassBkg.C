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
#include "RooExponential.h"
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
//#include "DrawInvMassBkg4.C"
#include <iostream>
#include <fstream>

using namespace RooFit;

double FitInvMassBkg(TH1D* histo, TString signal = "CBxBW", TString _bkg = "Exp", TString option = ""){

  option.Append(" ");
  option.Prepend(" ");

  cout<<"start the fitting"<<endl;
  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);


  //Set Style
  setTDRStyle();

  //Getting info about the histogram to fit
  int n = histo->GetEntries();
  double w = histo->GetXaxis()->GetBinWidth(1);
  int ndf;
  //double hmin0 = histo->GetXaxis()->GetXmin();
  //double hmax0 = histo->GetXaxis()->GetXmax();
  double xmin = 70;
  double xmax = 130;
  histo->GetXaxis()->SetRangeUser(xmin,xmax);

  //Declare observable x
  RooRealVar x("x","x", xmin, xmax) ;
  RooDataHist dh("dh","dh",x,Import(*histo)) ;

  //Define the frame
  RooPlot* frame;
  frame = x.frame();
  dh.plotOn(frame,DataError(RooAbsData::SumW2), MarkerColor(1),MarkerSize(0.9),MarkerStyle(7));  //this will show histogram data points on canvas

  x.setRange("R", xmin, xmax) ;

  /////////////////////////
  //Signal fitting function 
  /////////////////////////

  //fsig for adding two funciton i.e. F(x) = fsig*sig(x) + (1-fsig)*bkg(x)
  RooRealVar nsig("nsig","signal events",histo->GetEntries()/2., 1,histo->GetEntries());
  RooRealVar nbkg("nbkg","background events",histo->GetEntries()/2.,1,histo->GetEntries());
  RooArgList pdfval(nsig,nbkg);
  cout<<"Debug4"<<endl;

  //Parameters
  //

  //True mean
  RooRealVar mean("mean","PDG mean of Z",91.186);
  mean.setRange(88,94);
  //BW PDG width
  RooRealVar width("width","PDG width of Z",2.4952);
  width.setRange(0,20);
  //
  RooRealVar ncb("ncb","ncb",7, 0, 150);
  RooRealVar cb_ncb("cb_ncb","ncb",2, 1, 5);
  RooRealVar cb_bias("cb_bias","bias",0, -3.,3.);
  RooRealVar cb_sigma("cb_sigma","response",1, 0.,5);
  RooRealVar cb_alpha("cb_alpha","alpha",1.,0.,7);

  RooBreitWigner sig_bw("sig_bw","BW",x,mean,width);
  RooCBShape sig_cb_resp("sig_cb_resp", "Crystal Ball for conv.",x,cb_bias,cb_sigma,cb_alpha,cb_ncb);

  x.setBins(10000,"cache");
  RooFFTConvPdf sig_cbbw("sig_cbbw","CBxBW",x,sig_cb_resp,sig_bw);

  cout<<"Debug5"<<endl;

  //NB: The CrystalBall shape is Gaussian that is 'connected' to an exponential taill at 'alpha' sigma of the Gaussian. The sign determines if it happens on the left or right side. The 'n' parameter control the slope of the exponential part. 

  RooAbsPdf* sig;

  cout<<"Debug5"<<endl;

  /////////////////////////////
  //Background fitting function
  /////////////////////////////

  //Get the initial parameter of the background
  //Bkg function
  RooAbsPdf* bkg;

  //Chebychev newfit
  RooRealVar a0("a0","a0",0,-5.,0.) ;
  RooRealVar a1("a1","a1",0,-2.5,3) ;
  RooRealVar a2("a2","a2",0,-1.5,3) ;
  RooRealVar a3("a3","a3",0,-4,2.) ;
  RooRealVar a4("a4","a4",0,-1.5,3.) ;
  RooRealVar a5("a5","a5",0,-3.,1.) ;
  RooRealVar a6("a6","a6",0,-1.,1.) ;
  RooChebychev bkg_cheb("bkg","Background",x,RooArgSet(a0,a1,a2,a3,a4,a5,a6));

  bkg = &bkg_cheb;
  cout<<"Debug6"<<endl;

  //////////////////////////
  //Adding the two functions
  //////////////////////////

  RooAddPdf model("model","Signal+Background", RooArgList(*sig,*bkg),pdfval);

  cout<<"Debug6a"<<endl;
  //Perform the fit
  RooAbsPdf* fit_func;
  cout<<"Debug6b"<<endl;
  fit_func = &model;
  cout<<"Debug7"<<endl;

  cout<<"The range of x is "<<xmin<<"-"<<xmax<<endl; 
  RooFitResult* filters = fit_func->fitTo(dh,Range("R"),"qr");
  fit_func->plotOn(frame);
  fit_func->plotOn(frame,Components(*sig),LineStyle(kDashed),LineColor(kRed));
  fit_func->plotOn(frame,Components(*bkg),LineStyle(kDashed),LineColor(kGreen));
  frame->SetAxisRange(xmin, xmax);
  frame->Draw();
  fit_func->paramOn(frame); 
  dh.statOn(frame);  //this will display hist stat on canvas

  frame->SetTitle(histo->GetTitle());  
  frame->GetXaxis()->SetTitle("m (in GeV/c^{2})");  
  frame->GetXaxis()->SetTitleOffset(1.2);
  float binsize = histo->GetBinWidth(1); 
  cout<<"Debug8"<<endl;

  //Store result in .root file
  frame->SetName(histo->GetName());
  frame->Write();

  ///////////////////////
  //Plot the efficiency//
  ///////////////////////

  return nsig.getVal();

  }

