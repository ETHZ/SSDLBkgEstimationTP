/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : FitHelper.C 

* Purpose : This file contain various functions that helps to perform the fitting. The goal of those function is mainly to fit the best value and rang of the parameter to fit.

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By :
_._._._._._._._._._._._._._._._._._._._._.*/

#include "RooBinning.h"


double MaxFinder(TH1D* h){

	int binmax =h->GetMaximumBin();
	int nbins = h->GetNbinsX();
 	double max =h->GetXaxis()->GetXmax();
 	double min =h->GetXaxis()->GetXmin();

	double width = (max-min)/(double)nbins;

	return binmax*width;

}
TH1* NormToWidth(TH1* h){


	for(int i = 1;i<=h->GetNbinsX();++i){

		h->SetBinContent(i,h->GetBinContent(i)/h->GetBinWidth(i));
	}

	return h;

}
RooBinning Rebin2(TH1* h){

	double n = h->GetEntries();
	double* xbins = new double[h->GetNbinsX()];
	TAxis* axis = h->GetXaxis();

	int k = 0;
	int width = 0;
	for(int i = 1; i<=h->GetNbinsX();i+=width){

		width = 1;	

		double content = h->GetBinContent(i);

		for(int j = i+1; j < i+11; ++j){
			if(abs((double) ((double)content/width - h->GetBinContent(j))/(double) content) > 0.1){}
			else{
				content +=h->GetBinContent(j);
				++width;
			}
		}

		xbins[k] = axis->GetBinLowEdge(i);
		++k;
	}

	xbins[k] = axis->GetXmax();

	double* xbins2 = new double[k+1];
	for(int i= 0; i<=k;++i){
		xbins2[i] = xbins[i];
	}

	RooBinning roobin = RooBinning(k,xbins);
	

	return roobin;
	delete[] xbins;
	delete[] xbins2;

}

TH1* Rebin(TH1* h){

	double n = h->GetEntries();
	double* xbins = new double[h->GetNbinsX()];
	TAxis* axis = h->GetXaxis();

	int k = 0;
	int width = 0;
	for(int i = 1; i<=h->GetNbinsX();i+=width){

		width = 1;	

		double content = h->GetBinContent(i);

		for(int j = i+1; j < i+11; ++j){
			if(abs((double) ((double)content/width - h->GetBinContent(j))/(double) content) > 0.1){}
			else{
				content +=h->GetBinContent(j);
				++width;
			}
		}

		xbins[k] = axis->GetBinLowEdge(i);
		++k;
	}

	xbins[k] = axis->GetXmax();

	double* xbins2 = new double[k+1];
	for(int i= 0; i<=k;++i){
		xbins2[i] = xbins[i];
	}

	h = h->Rebin(k,h->GetName(),xbins2);

	for(int i = 0;i<=k;++i){

		h->SetBinContent(i,h->GetBinContent(i)/h->GetBinWidth(i));
	}

	//TH1* hnew = new TH1("hnew",h->GetName(),k,xbins2);
	//TAxis* axisnew = hnew->GetXaxis();

	//for(int i = 1;i <=h->GetNbinsX();++i){

	//	//double weight = 1./axisnew->GetBinWidth(hnew->FindBin(axis->GetBinCenter(i)));
	//	double content = 3;//h->GetBinContent(i);

	//	hnew->Fill(100);//,weight);
	//}


	//return hnew;
	return h;
	delete[] xbins;
	delete[] xbins2;

}



double PicFinder(TH1D h, double xpic = 91.186 , double widthpic = 2.4952){

	int integral = h.GetEntries();

	//Find the bin of the Z peak max (stored in binmax)
	int x_upp = h.FindBin(xpic+widthpic);
	int x_low = h.FindBin(xpic+widthpic);
	h.GetXaxis()->SetRange(x_low,x_upp);
	int peak_bin = h.GetMaximumBin();

	//Find the inf and sup such that the value drops below 20% (stored in peak_inf and peak_supp)
	int peak_inf = h.FindFirstBinAbove(h.GetBinContent(peak_bin)/5.);
	int peak_supp= h.FindLastBinAbove(h.GetBinContent(peak_bin)/5.);

	//Compute the number of entries corresponding to the z peak
	h.GetXaxis()->SetRange(peak_inf,peak_supp);
	int peak_integral = h.GetEntries();

	return (double)peak_integral/ (double) integral;

}

double FWHMFinder(TH1D* h){

	return h->GetMaximum();

}
