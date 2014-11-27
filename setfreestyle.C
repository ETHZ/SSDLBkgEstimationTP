/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : myfile.c

* Purpose :

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By :

_._._._._._._._._._._._._._._._._._._._._.*/

void setfreestyle(){

	gStyle->SetTitleSize(0.5,"XYZ");
	gStyle->SetLabelFont(42,"XYZ");
        gStyle->SetTitleFont(42);
        gStyle->SetTitleFontSize(0.1);
        gStyle->SetLabelSize(0.05, "XYZ");
	gStyle->SetOptStat(0);
	gStyle->SetFrameBorderSize(1);
	gROOT->ForceStyle();

}
