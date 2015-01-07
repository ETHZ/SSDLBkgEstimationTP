/*********************************************
 * Description - Performs a fit of the invriant mass mass + background and store the result using path.
 Retrieve then the efficiency using the T&P

 * Author - Gaël L. Perrin

 * Date - Jan 05 2015

 * *******************************************/

#include "TString.h"
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include <stdio.h>
#include <sys/stat.h>
#include <iostream>
#include <cmath>

#include "FitInvMassBkg4.C"

double BinomError(double Nt, double eff) {

	double error=0;
	if(Nt==0) return 1;
	error = sqrt(eff*(1-eff)/Nt) ;
	return error;

}


int TandP(int leptonId, double par_low, double par_upp = 9999, int npar1bins = 10, TString select = "tight", TString effcut = "", double cut = 0.2, TString par_x = "Pt", TString par_y = "eta", TString _sig = "CBxBW",TString option = ""){
	gROOT->SetBatch(kTRUE); 

	cout<<"Debug1"<<endl;

	//Path for input and output file.
	TString _path = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/";

	//Some variables
	double Dpt = (par_upp-par_low)/npar1bins;
	int nbins = 200;

	/////////////////////
	//Name of the input//
	/////////////////////

	TString pname;
	if(leptonId == 11){pname = "e";}
	if(leptonId == 13){pname = "mu";}

	//Parameter string
	TString _par;
	TString _par2;
	if(par_x == "Pt"){_par = "P_{t}";}
	else if(par_x == "eta"){_par = "#eta";}
	else if(par_x == "phi"){_par = "#phi";}
	if(par_y == "Pt"){_par2 = "P_{t}";}
	else if(par_y == "eta"){_par2 = "#eta";}
	else if(par_y == "phi"){_par2 = "#phi";}

	//Parameter 1
	double* par1 = new double[npar1bins+1];
	double Dpar = (double)(par_upp - par_low)/(double)npar1bins;

	for(int i = 0; i < npar1bins+1; ++i){
		par1[i] = par_low + i*Dpar;
	}

	//Parameter 2
	const int npar2bins = 2;
	double par2[npar2bins+1] = {0,1.2,2.5};

	//selection string
	TString _sel;

	if((select == "tightmva")&&(leptonId == 13)){cout<<"ERROR: no tightId MVA defined for the muon !"<<endl;return 1;}
	if(select == "tightcut"){_sel = "tightcut";}
	else if(select == "tightmva"){_sel = "tightmva";}
	else if(select == "loose"){_sel = "loose";}
	else if(select == ""){_sel = "";}
	else{cout<<"ERROR: wrong selection !";return 1;}

	//cut string
	TString _effcut;
	if((effcut == "tightmva")&&(leptonId == 13)){cout<<"ERROR: no tightId MVA defined for the muon !"<<endl;return 1;}
	if(effcut == "tightcut"){_effcut = "tightcut";}
	else if(effcut == "tightmva"){_effcut = "tightmva";}
	else if(effcut == ""){_effcut = "";}
	else if(effcut == "loose"){_effcut = "loose";}
	else if(effcut == "reliso3"){_effcut = Form("reliso3_%0.3lf",cut);}
	else if(effcut == "reliso4"){_effcut = Form("reliso4_%0.3lf",cut);}
	else if(effcut == "chiso3"){_effcut = Form("chiso3_%0.3lf",cut);}
	else if(effcut == "chiso4"){_effcut = Form("chiso4_%0.3lf",cut);}
	else if(effcut == "dxy"){_effcut = Form("dxy_%0.3lf",cut);}
	else if(effcut == "dz"){_effcut = Form("dz_%0.3lf",cut);}
	else{cout<<"ERROR: wrong numerator name !";return 1;}

	///////////////////////////////////////////
	//Write the name of the input/output file//
	///////////////////////////////////////////

	TString _fname_in = "InvM4test";
	TString _fname_out = "InvM4test";
	if(option.Contains("matching")){_fname_in += "_Matched";_fname_out += "_Matched";}

	TString par1range;
	par1range = Form("%0.3f_"+par_x+"%0.3f",par1[0],par1[npar1bins]);
	_fname_in += "_"+par1range;
	_fname_out += "_"+par1range;
	_fname_in += "_"+_effcut+"_for_"+_sel+"_"+pname; 
	_fname_out += "_"+_effcut+"_for_"+_sel+"_"+pname; 

	if(option.Contains("nentries")){_fname_out += "_nentries";}

	//Create folder to store background fitting
	mkdir(_path+_fname_out+"_FIT_PDF/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	//Check if the file exist
	TFile  f_test(_path+_fname_in+".root");
	if(f_test.IsZombie()){

		cout<<"File does not exists"<<endl;

		return 1;
		//cout<<"Creating file, please wait..."<<endl;
		//DrawInvMassBkg(leptonId,par_low,par_upp,npar1bins,select,effcut,cut,option); 
		//cout<<"Done !"<<endl;

	}

	TFile* f = new TFile(_path+_fname_in+".root","read");
	TFile* file_out = new TFile(_path+_fname_out+"_FIT"+".root","recreate");
	TFile* file_out2 = new TFile(_path+_fname_out+"_FIT_eff"+".root","recreate");

	/////////////////////////////////////////
	//Recover the invariant mass histograms//
	/////////////////////////////////////////

	//Declaration of the histograms
	//
	
	//Histo to recover
	TH1D ***histo_DY 	= new TH1D**[npar2bins];
	TH1D ***histo_BKG 	= new TH1D**[npar2bins];
	TH1D ***histo_DY_fail 	= new TH1D**[npar2bins];
	TH1D ***histo_BKG_fail 	= new TH1D**[npar2bins];

	//Output histo
	TH1D **eff 		= new TH1D*[npar2bins];
	//Declaration of graph for the efficiency
	TGraphErrors** _eff 	= new TGraphErrors*[npar2bins];

	//Recover the histo
	for(int i = 0; i < npar2bins; ++i){ 

		histo_DY[i]      	= new TH1D*[npar1bins];
		histo_BKG[i]     	= new TH1D*[npar1bins];
		histo_DY_fail[i] 	= new TH1D*[npar1bins];
		histo_BKG_fail[i]	= new TH1D*[npar1bins];

		eff[i]			= new TH1D("eff_BKG_fail","eff",npar1bins,par1[0],par1[npar1bins+1]);

		for(int j = 0; j < npar1bins; ++j){ 

			histo_DY[i][j]        	=  new TH1D("histo_M_DY","M",nbins,0,250);
			histo_BKG[i][j]     		=  new TH1D("histo_M_BKG","M",nbins,0,250);
			histo_DY_fail[i][j] 		=  new TH1D("histo_M_DY_fail","M",nbins,0,250);
			histo_BKG_fail[i][j]		=  new TH1D("histo_M_BKG_fail","M",nbins,0,250);

		}

	}

	////////////////////////////
	//Calculate the efficiency//
	////////////////////////////

	//Efficiency
	//In order to compute the efficiency, one needs to fill the following variables
	double** Eff = new double*[100];
	double** error_eff = new double*[100];
	double** PT = new double*[100];
	double** error_par1 = new double*[100];

	for(int j = 0; j < npar2bins; ++j){ 

		Eff[j] = new double[100];
		error_eff[j] = new double[100];
		PT[j] = new double[100];
		error_par1[j] = new double[100];  	
	}

	//Folder to save the output 
	mkdir(_path+_fname_out+"_FIT_PDF/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	//Starting the loop that read the hisograms and do the fit
	for(int j = 0; j < npar2bins; ++j){ 

			//name of the bins
			TString _parxbin;
			TString _parybin;

		for(int i = 0; i<npar1bins; ++i){

			//Recover the Histograms
			//
			

			//Parameter string
			TString _par;
			if(par_x == "Pt"){_parxbin = Form("%0.f_Pt%0.f",par1[i],par1[i+1]);}
			else if(par_x == "eta"){_parxbin = Form("%0.3f_eta%0.3f",par1[i],par1[i+1]);}
			else if(par_x == "phi"){_parxbin = Form("%0.3f_phi%0.3f",par1[i],par1[i+1]);}
			if(par_y == "Pt"){_parybin = Form("%0.f_Pt%0.f",par2[j],par2[j+1]);}
			else if(par_y == "eta"){_parybin = Form("%0.3f_eta%0.3f",par2[j],par2[j+1]);cout<<"it works !"<<endl;}
			else if(par_y == "phi"){_parybin = Form("%0.3f_phi%0.3f",par2[j],par2[j+1]);}

			TString _DY;
			TString _BKG;
			TString _DY_fail;
			TString _BKG_fail;

			_DY 		= "histo_M_DYJets_bkg"+_parxbin+"_"+_parybin;
			_BKG 		= "histo_M_bkg"+_parxbin+"_"+_parybin; 
			_DY_fail 	= "histo_M_DYJets_bkg_fail"+_parxbin+"_"+_parybin; 
			_BKG_fail 	= "histo_M_bkg_fail"+_parxbin+"_"+_parybin; 

			histo_DY[j][i] 		 	= (TH1D*)f->Get(_DY);
			histo_BKG[j][i] 		= (TH1D*)f->Get(_BKG);
			histo_DY_fail[j][i] 		= (TH1D*)f->Get(_DY_fail);
			histo_BKG_fail[j][i] 		= (TH1D*)f->Get(_BKG_fail);

			histo_DY[j][i]->SetName(_DY);
			histo_BKG[j][i]->SetName(_BKG);
			histo_DY_fail[j][i]->SetName(_DY_fail);
			histo_BKG_fail[j][i]->SetName(_BKG_fail);

			//Fit the recovered histograms
			//

			TCanvas* c1 = new TCanvas("c1","c1");
			c1->Divide(1,2);
			c1->cd(1);
			double pass = FitInvMassBkg_v3(histo_DY[j][i],histo_BKG[j][i],_sig,"Cheb",option);
			c1->cd(2);
			double fail = FitInvMassBkg_v3(histo_DY_fail[j][i],histo_BKG_fail[j][i],_sig,"Cheb",option);

			//Compute and fill the efficiency
			//

			Eff[j][i] = pass/(pass+fail);
			error_eff[j][i] = BinomError(pass+fail,Eff[j][i]);

			PT[j][i] = (par1[i] + par1[i])/2.;
			error_par1[j][i] = (par1[i] - par1[i])/2.;
			eff[j]->Fill(PT[j][i],Eff[j][i]);
			eff[j]->SetBinError(i,error_eff[j][i]);

			//Save the Fit 

			TString _cname = _path+_fname_out+"_FIT_PDF/InvM_"+_parxbin+"_"+_parybin;
			_cname += "_"+effcut+"_"+pname;
			TString _c1name = _cname+ ".pdf";
			c1->SaveAs(_c1name);
		}

		
	///////////////////////
	//Fill the efficiency//
	///////////////////////
	
	_eff[j] = new TGraphErrors(npar1bins,PT[j],Eff[j],error_par1[j],error_eff[j]);
	
	_eff[j]->Draw("A");
	_eff[j]->SetMarkerStyle(4);
	_eff[j]->SetMarkerSize(0.4);
	_eff[j]->SetMarkerColor(4);
	_eff[j]->GetXaxis()->SetTitle("P_{t}");
	_eff[j]->GetXaxis()->SetRangeUser(0,250);
	_eff[j]->GetYaxis()->SetRangeUser(0,1.1);
	_eff[j]->GetYaxis()->SetTitle(" #epsilon ");
	_eff[j]->GetXaxis()->SetTitle("P_{t}");

	TString _g1name = _path+_fname_out+"_PDF/Eff";
	_g1name += "_"+effcut+"_"+pname+"_"+_parxbin+"_"+_parybin+".pdf";

	TCanvas* b1 = new TCanvas("b1","b1");

	b1->cd();
	eff[j]->Draw();
	b1->SaveAs(_g1name);

	file_out2->cd();
	_eff[j]->Write("Eff_"+effcut+"_"+pname+"_"+_parxbin+"_"+_parybin);
	eff[j]->Write("Eff_histo_"+effcut+"_"+pname+"_"+_parxbin+"_"+_parybin);

	}

	f->Close();
	file_out->Close();
	file_out2->Close();
}
