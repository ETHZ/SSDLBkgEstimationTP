/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

 * File Name : myfile.c

 * Purpose :

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

#include "setfreestyle.C"
#include "FitInvMass.C"
#include "InvMass.C"
#include "DeltaR.C"
#include "FitInvMassBkg.C"
#include "FitBkg.C"
#include "Readfile.C"

void ZBkgFit(){

	//The fitting function. Can be 
	//BWxGau, BW, Gau, CB
	TString _f = "CB";
	//Next file contains the list of histo
	string input = "DYinclusive.txt";

	vector<string> vec = readfile(input);

	cout<<"The size is "<<vec.size()<<endl;

	TString _input = (TString) input;
	_input.Remove(input.length()-4);

	TFile* f_out = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/ZBkgInvM/"+_input+"/"+"ZBkgRead_Znbkg"+_f+".root","recreate");
	cout<<"The input is "<<_input<<endl;

	for (int i = 0; i <vec.size(); i+=2){ 

		TString s1 = vec[i];
		TString s2 = vec[i+1];

		cout<<"i is "<<i<<endl;
		cout<<vec[i]<<endl;
		cout<<vec[i+1]<<endl;
		TFile* f = new TFile(s1,"read");
		TH1D* histo = (TH1D*)f->Get(s2); 

		f_out->cd();
		gDirectory->pwd();
		s1.Remove(0,92);
		s1.Remove(s1.Length()-5);
		s1.ReplaceAll("1.2","1,2");
		cout<<"s1 is now "<<s1<<endl;
		TString _write = s1+"_"+s2;
		FitInvMassBkg(histo,_write,_f,"");
		//FitBkg(histo,_write,1); 

		f->Close(); 
		delete f;

	}

	f_out->Close();



}
