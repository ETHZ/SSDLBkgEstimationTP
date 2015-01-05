#include "iostream"
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"

TF1* FitBreitWigner(TH1D histo, float bin_min, float bin_max, int color = 1){

//Definition of Fitting functions

	TString fitting_funct = "[0]/(pow((x*x - [1]*[1]),2) + [1]*[1]*[2]*[2])"; 

	TF1* f1 = new TF1("f1", fitting_funct, bin_min, bin_max);
	
	f1->SetParameter(1,histo.GetMean());
	//Get FWHM
    	int bin1 = histo.FindFirstBinAbove(histo.GetMaximum()/2);
        int bin2 = histo.FindLastBinAbove(histo.GetMaximum()/2);
        double fwhm = histo.GetBinCenter(bin2) - histo.GetBinCenter(bin1);
		 
	f1->SetParameter(2,fwhm);

//Declaration of function to fit

histo.Fit(f1,"N");

	Double_t Chi2 = f1->GetChisquare();
        Double_t ndf = f1->GetNDF();
	Double_t prob = f1->GetProb();
	
        cout<<"Chi2 ="<<Chi2<<endl;
        cout<<"ndf = "<<ndf<<endl; 
	cout<<"The prob is = "<<prob<<endl;
	cout<<"Chi2/ndf = "<<Chi2/ndf<<endl;

//cout<<"P- value"<<f1->GetChisquare()<<endl;

histo.Draw();

f1->SetLineColor(1);
f1->Draw("same");

return f1;


}
