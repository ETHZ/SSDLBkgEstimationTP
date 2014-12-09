/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name :GoodOfFit.C 

* Purpose : Compare the original histogram to the fitting. The integral of the integral of the Inv mass, background and differences are compared. Plots are also generated where both fitted function and histogram are on the canvas at the same time.

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By :

_._._._._._._._._._._._._._._._._._._._._.*/

#include "cmath"
#include "vector"
#include "iostream"
#include <stdio.h>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TString.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TLorentzVector.h"
#include "RooPlot.h"

#include "setfreestyle.C"
#include "FitInvMass.C"
#include "InvMass.C"
#include "DeltaR.C"
#include "FitInvMassBkg.C"
#include "FitBkg.C"
#include "Readfile.C"

int GoodOfFit(){

	//The fitting function. Can be 
	//BWxGau, BW, Gau, CB
	TString _f = "CB";
	//File containing the histograms
	string input = "DYinclusive.txt";

	vector<string> vec = readfile(input);

	//Folder containing the fit and the GooOfFit output
	TString _input = (TString) input;
	_input.Remove(input.length()-4);
	TString _folder = "/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/ZBkgInvM/"+_input+"/";

	//Open the file containing the GooOfFit output
	TFile* f_out = new TFile(_folder+"GoodOfFit"+_f+".root","recreate");

	for (int i = 0; i <vec.size(); i+=2){ 

		TString s1 = vec[i];
		TString s2 = vec[i+1];
		
		//recover histogram
//		TString _hist_input = s1+"_"+s2;
		TFile* f_hist = new TFile(s1,"read");
		TH1D* histo = (TH1D*)f_hist->Get(s2); 
		
		//recover fitted function
		s1.Remove(0,92);
		s1.Remove(s1.Length()-5);
		s1.ReplaceAll("1.2","1,2");
		TString _fit = s1+"_"+s2;
		TFile* f_fit = new TFile(_folder+"ZBkgRead_Znbkg"+_f+".root");
		//cout<<f_fit->GetListOfKeys()<<endl;
		RooPlot* frame = (RooPlot*) f_fit->Get(_fit+";1");
//		RooAddPdf* fit_f= (RooAddPdf*) frame->getObject(3);
		cout<<"information about object "<<frame->GetObjectInfo(0,1)<<endl;
		
		//RooPlot* frame2;
		//fit_f->plotOn(frame2);
		

		//cout<<"TFile is "<<_folder+"ZBkgRead"+_f+".root";
		//cout<<" _fit is "<<_fit<<endl;

		//histo->DrawCopy();
		//frame->DrawClone();
		
		f_out->cd();
		//frame2->Write();


		//FitBkg(histo,_write,1); 

		f_hist->Close(); 
		f_fit->Close();
		delete f_hist;
		delete f_fit;

	}

	f_out->Close();
		

}
