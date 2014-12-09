/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : myfile.c

* Purpose :

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By :

_._._._._._._._._._._._._._._._._._._._._.*/

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooLandau.h"
#include "RooFFTConvPdf.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TH1.h"
using namespace RooFit ;

void convolution()
{

	// Construct observable
	RooRealVar t("t","t",-10,30) ;

	// Construct landau(t,ml,sl) ;
	RooRealVar ml("ml","mean landau",5.,-20,20) ;
	RooRealVar sl("sl","sigma landau",1,0.1,10) ;
	RooLandau landau("lx","lx",t,ml,sl) ;

	// Construct gauss(t,mg,sg)
	RooRealVar mg("mg","mg",0) ;
	RooRealVar sg("sg","sg",2,0.1,10) ;
	RooGaussian gauss("gauss","gauss",t,mg,sg) ;



	// Set #bins to be used for FFT sampling to 10000
	t.setBins(10000,"cache") ; 

	// Construct landau (x) gauss
	RooFFTConvPdf lxg("lxg","landau (X) gauss",t,landau,gauss) ;

	// Sample 1000 events in x from gxlx
	RooDataSet* data = lxg.generate(t,10000) ;

	// Fit gxlx to data
	lxg.fitTo(*data) ;

	// Plot data, landau pdf, landau (X) gauss pdf
	RooPlot* frame = t.frame(Title("landau (x) gauss convolution")) ;
	data->plotOn(frame) ;
	lxg.plotOn(frame) ;
	landau.plotOn(frame,LineStyle(kDashed)) ;


	// Draw frame on canvas
	new TCanvas("rf208_convolution","rf208_convolution",600,600) ;
	gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.4) ; frame->Draw() ;
}
