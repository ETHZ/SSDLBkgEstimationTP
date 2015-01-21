/*********************************************
 * Description - Plot the efficiency for a given cut that is applied to reconstructed data. A matching between the gen. and rec. data is done.
 Upgrade w.r.t. previous version: Can now choose if barrel/endcape separation and includs the loose id as well
 Final version

 * Author - Gaël L. Perrin

 * Date - Jan 05 2015
 * *******************************************/

#include "cmath"
#include "TChain.h"
#include <sys/stat.h>
#include "iostream"
#include "fstream"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TBranch.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TString.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TLorentzVector.h"
#include "../InvMass.C"
#include "../DeltaR.C"
#include "vector"
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TBranch.h"
#include "TCanvas.h"
#include "TLorentzVector.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TString.h"
#include "TLegend.h"
#include "TObject.h"

#include "../setTDRStyle.C"

int MC_Ratio(int leptonId, double* par1, int npar1bins, double* par2, int npar2bins, TString sel_den , TString sel_num, double cut_num = 0., TString par_x = "Pt", TString par_y = "eta", TString option = "");

int MC_Ratio(int leptonId, double par_low, double par_upp, int npar1bins, TString sel_den , TString sel_num, double cut_num = 0., TString par_x = "Pt", TString par_y = "eta", TString option = ""){


	//Parameter 1
	double* par1 = new double[npar1bins+1];
	double Dpar = (double)(par_upp - par_low)/(double)npar1bins;

	for(int i = 0; i < npar1bins+1; ++i){
		par1[i] = par_low + i*Dpar;
	}

	//Parameter 2
	const int npar2bins_eta = 2;
	//const int npar2bins_pt = 19;
  	const int npar2bins_pt = 1;

	double par2_eta[npar2bins_eta+1] = {0,1.2,2.5};
	//double par2_pt[npar2bins_pt+1] = {10,20,30,40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200};
  	double par2_pt[npar2bins_pt+1] = {10,250};

	if(par_y == "eta"){
	return MC_Ratio(leptonId, par1, npar1bins, par2_eta, npar2bins_eta, sel_den, sel_num, cut_num, par_x, par_y, option );
	}else if(par_y == "Pt"){
	return MC_Ratio(leptonId, par1, npar1bins, par2_pt, npar2bins_pt, sel_den, sel_num, cut_num, par_x, par_y, option );
	}else{return 1;}

}

int MC_Ratio(int leptonId, double* par1, int npar1bins, double* par2, int npar2bins, TString sel_den , TString sel_num, double cut_num , TString par_x , TString par_y , TString option ){

	setTDRStyle();

	///////////////
	//Get the TTree
	///////////////

	//Location of the .root file
	TString location = "/Users/GLP/Desktop/CERN_data/2014-11-13_skim2ll-mva-softbtag/postprocessed/";
        TString location2 = "/Users/GLP/Desktop/CERN_data/dyjetsnew/postprocessed/";

	//Reading the tree 
	//
	TChain* tree = new TChain("treeProducerSusyMultilepton");

	//DY events
	//tree->Add(location+"DYJetsToLLM50_PU_S14_POSTLS170.root");
        tree->Add(location2+"dyjetsnew.root");

	//Plot the result

	Long64_t n = tree->GetEntries();

	//Path for input and output file. Written in FitDataPath.txt
	TString _path = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_ratio/";

	//////////////////////
	//Name for the plots//
	//////////////////////
	//
        TString _filetag ="_newloose";

	TString pname;
	TString _pname;
	TString _par;
	TString _sel_num;
	TString _sel_den;
	TString _option;

	////Writing string
	//particle string
	if(abs(leptonId) == 11){pname = "e"; _pname = "e";}
	if(abs(leptonId) == 13){pname = " #mu"; _pname = "mu";}
	//Parameter string
	if(par_x == "Pt"){_par = "P_{t}";}
	else if(par_x == "eta"){_par = "#eta";}
	else if(par_x == "phi"){_par = "#phi";}
	//sel_den string
	if((sel_den == "tightmva")&&(leptonId == 13)){cout<<"ERROR: no tightId MVA defined for the muon !"<<endl;return 1;}
	if(sel_den == "tightcut"){_sel_den = "tightcut";}
	else if(sel_den == "tightmva"){_sel_den = "tightmva";}
	else if(sel_den == "loose"){_sel_den = "loose";}
	else if(sel_den == ""){_sel_den = "";}
	else{cout<<"ERROR: wrong sel_denion !";return 1;}
	//sel_num string
	if((sel_num == "tightmva")&&(leptonId == 13)){cout<<"ERROR: no tightId MVA defined for the muon !"<<endl;return 1;}
	if(sel_num == "tightcut"){_sel_num = "tightcut";}
	else if(sel_num == "tightmva"){_sel_num = "tightmva";}
	else if(sel_num == ""){_sel_num = "";}
	else if(sel_num == "loose"){_sel_num = "loose";}
	else if(sel_num == "reliso3"){_sel_num = Form("reliso3_%0.3lf",cut_num);}
	else if(sel_num == "reliso4"){_sel_num = Form("reliso4_%0.3lf",cut_num);}
	else if(sel_num == "chiso3"){_sel_num = Form("chiso3_%0.3lf",cut_num);}
	else if(sel_num == "chiso4"){_sel_num = Form("chiso4_%0.3lf",cut_num);}
	else if(sel_num == "dxy"){_sel_num = Form("dxy_%0.3lf",cut_num);}
	else if(sel_num == "dz"){_sel_num = Form("dz_%0.3lf",cut_num);}
	else{cout<<"ERROR: wrong numerator name !";return 1;}
	//option string
	option.Append(" ");
	option.Prepend(" ");
	if(option.Contains(" ll ")){_option += "_ll";}
	if(option.Contains(" unmatched ")){_option += "_unmatched";}
	if(option.Contains(" alleta ")){_option += "_alleta";}
	_option += "_";
	//parameter range string
	TString _par1range;
	_par1range = Form("%0.3f_"+par_x+"%0.3f",par1[0],par1[npar1bins]);
	TString _par2range;
	_par2range = Form("%0.3f_"+par_y+"%0.3f",par2[0],par2[npar2bins]);

	/////////////////////////////////////
	//Write the name of the output file//
	/////////////////////////////////////

	TString _fname = "eff"+_filetag+_option+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num;

	//Output file
	TFile* file_out = new TFile(_path+_fname+".root","recreate");

	//Declaration of histogram

	TH1D **histo_num = new TH1D*[npar2bins];
	TH1D **histo_den = new TH1D*[npar2bins];
	TH1D **eff = new TH1D*[npar2bins];


	//Par1 distribution histogram
        TH1D** histo_par1 = new TH1D*[npar2bins];

	//Histo separate in Lep_good Lep_other
	//
	TH1D **histo_num_O = new TH1D*[npar2bins];
	TH1D **histo_den_O = new TH1D*[npar2bins];
	TH1D **eff_O = new TH1D*[npar2bins];
	TH1D **histo_num_G = new TH1D*[npar2bins];
	TH1D **histo_den_G = new TH1D*[npar2bins];
	TH1D **eff_G = new TH1D*[npar2bins];

	//Counter LepGood vs LepOther
	
	TH1D **histo_counter = new TH1D*[npar2bins];
	TH1D **histo_counter_G_par1 = new TH1D*[npar2bins];
	TH1D **histo_counter_O_par1 = new TH1D*[npar2bins];
	TH1D **histo_counter_par1 = new TH1D*[npar2bins];


	for(int _i = 0; _i < npar2bins; ++_i){ 

		histo_num[_i] = new TH1D("histo_num","Pt",npar1bins,par1[0],par1[npar1bins]);
		histo_den[_i] = new TH1D("histo_den","Pt",npar1bins,par1[0],par1[npar1bins]);
		eff[_i] = new TH1D("eff","Pt",npar1bins,par1[0],par1[npar1bins]);

		//
    		histo_par1[_i] = new TH1D("histo_par1","par1",npar1bins*25,par1[0],par1[npar1bins]);

		//
		histo_num_O[_i] = new TH1D("histo_num_O","Pt",npar1bins,par1[0],par1[npar1bins]);
		histo_den_O[_i] = new TH1D("histo_den_O","Pt",npar1bins,par1[0],par1[npar1bins]);
		eff_O[_i] = new TH1D("eff_O","Pt",npar1bins,par1[0],par1[npar1bins]);

		histo_num_G[_i] = new TH1D("histo_num_G","Pt",npar1bins,par1[0],par1[npar1bins]);
		histo_den_G[_i] = new TH1D("histo_den_G","Pt",npar1bins,par1[0],par1[npar1bins]);
		eff_G[_i] = new TH1D("eff_G","Pt",npar1bins,par1[0],par1[npar1bins]);

		histo_counter_G_par1[_i] = new TH1D("histo_counter_G_par1","count",npar1bins,par1[0],par1[npar1bins]);
		histo_counter_O_par1[_i] = new TH1D("histo_counter_O_par1","count",npar1bins,par1[0],par1[npar1bins]);
		histo_counter_par1[_i] = new TH1D("histo_counter_par1","count",npar1bins,par1[0],par1[npar1bins]);

		histo_counter[_i] = new TH1D("histo_counter","count",2,0,2); 

	}

	//Event variables
	Int_t evt_id;
	Float_t scale;
	//Generated
	Float_t gen_phi[200];
	Float_t gen_eta[200];
	Float_t Pt[200];
	Float_t m[200];
	Int_t Id[200];
	Int_t Mo[200];
	Float_t charge[200];
	Int_t status[200];
	Int_t GrMa[200];
	Int_t ngenPart;
	Int_t source[200];
	Int_t pile_up;
	//not loose
	Int_t On;
	Int_t Oid[200];
	Float_t Opt[200];
	Float_t Om[200];
	Float_t Oeta[200];
	Float_t Ophi[200];
	Int_t   Oq[200];
	Int_t Otight[200];
	Int_t Otighte[200];
	//Float_t Omvaid[200];
	Int_t Oloose[200];
	Float_t Oiso3[200];
	Float_t Oiso4[200];
	Float_t Ochiso3[200];
	Float_t Ochiso4[200];
	Float_t Odxy[200];
	Float_t Odz[200];
	//loose
	Int_t Gn;
	Int_t Gid[200];
	Float_t Gpt[200];
	Float_t Gm[200];
	Float_t Geta[200];
	Float_t Gphi[200];
	Int_t   Gq[200];
	Int_t Gtight[200];
	Int_t Gtighte[200];
	//Float_t Gmvaid[200];
	Int_t Gloose[200];
	Float_t Giso3[200];
	Float_t Giso4[200];
	Float_t Gchiso3[200];
	Float_t Gchiso4[200];
	Float_t Gdxy[200];
	Float_t Gdz[200];

	//Assigne branches tree->SetBranchAddress("evt_scale1fb", &scale);
	tree->SetBranchAddress("evt_id", &evt_id);
	//generated
	tree->SetBranchAddress("ngenLep", &ngenPart);
	tree->SetBranchAddress("genLep_pdgId", &Id);
	tree->SetBranchAddress("genLep_sourceId", &source);
	tree->SetBranchAddress("genLep_eta", &gen_eta);
	tree->SetBranchAddress("genLep_phi", &gen_phi);
	tree->SetBranchAddress("genLep_pt", &Pt);
	tree->SetBranchAddress("genLep_mass", &m);
	tree->SetBranchAddress("genLep_charge", &charge);
	tree->SetBranchAddress("genLep_status", &status);
	tree->SetBranchAddress("nTrueInt",&pile_up);
	//not loose
	tree->SetBranchAddress("nLepOther",&On);
	tree->SetBranchAddress("LepOther_pdgId",&Oid);
	tree->SetBranchAddress("LepOther_pt",&Opt);
	tree->SetBranchAddress("LepOther_mass",&Om);
	tree->SetBranchAddress("LepOther_eta",&Oeta);
	tree->SetBranchAddress("LepOther_phi",&Ophi);
	tree->SetBranchAddress("LepOther_charge",&Oq);
	tree->SetBranchAddress("LepOther_tightId",&Otight);
	tree->SetBranchAddress("LepOther_eleCutIdCSA14_50ns_v1",&Otighte);
	//tree->SetBranchAddress("LepOther_mvaId",&Omvaid);
	tree->SetBranchAddress("LepOther_relIso03",&Oiso3);
	tree->SetBranchAddress("LepOther_relIso04",&Oiso4);
	tree->SetBranchAddress("LepOther_chargedHadRelIso03",&Ochiso3);
	tree->SetBranchAddress("LepOther_chargedHadRelIso04",&Ochiso4);
	tree->SetBranchAddress("LepOther_dxy",&Odxy);
	tree->SetBranchAddress("LepOther_dz",&Odz);
	//Loose
	tree->SetBranchAddress("nLepGood",&Gn);
	tree->SetBranchAddress("LepGood_pdgId",&Gid);
	tree->SetBranchAddress("LepGood_pt",&Gpt);
	tree->SetBranchAddress("LepGood_mass",&Gm);
	tree->SetBranchAddress("LepGood_eta",&Geta);
	tree->SetBranchAddress("LepGood_phi",&Gphi);
	tree->SetBranchAddress("LepGood_charge",&Gq);
	tree->SetBranchAddress("LepGood_tightId",&Gtight);
	tree->SetBranchAddress("LepGood_eleCutIdCSA14_50ns_v1",&Gtighte);
	//tree->SetBranchAddress("LepGood_mvaId",&Gmvaid);
	tree->SetBranchAddress("LepGood_relIso03",&Giso3);
	tree->SetBranchAddress("LepGood_relIso04",&Giso4);
	tree->SetBranchAddress("LepGood_chargedHadRelIso03",&Gchiso3);
	tree->SetBranchAddress("LepGood_chargedHadRelIso04",&Gchiso4);
	tree->SetBranchAddress("LepGood_dxy",&Gdxy);
	tree->SetBranchAddress("LepGood_dz",&Gdz);

	int count = 0;

	//Count lepgood/other
	
	int goodcount = 0;
	int othercount = 0;
	

	//n = 100000;

	//Start loop over all events
	for (int k = 0; k < n; ++k) {

		if( 100*(double)k/n> count){cout<<count<<endl;++count;}

		tree->GetEntry(k);

		//loop on other (not loose) 
		//Selection on denominator
		//if(sel_den != "loose"){
		if(false){
			for(int j=0; j<On;++j){
				if((!option.Contains(" ll "))||((option.Contains(" ll "))&&(On == 2)&&(Oid[0] == -Oid[1]))){
					if(abs(Oid[j]) == leptonId){
						if((sel_den != "tightcut")||(((abs(Oid[j]) == 13)&&(sel_den == "tightcut")&&(Otight[j] == 1 ))||((abs(Oid[j]) == 11)&&(sel_den == "tightcut")&&(Otighte[j] >= 3)))){
							if((sel_den != "tightmva")||((abs(Oid[j]) == 11)&&(sel_den == "tightmva")&&(Otight[j] == 1))){

								//Variable for matching
								double R = 999;
								double delta_P = 999;
								double delta_charge = 999;

								//Parameter on the xaxis

								double par;
								double par_2;

								//loop over all generated particles to do the matching
								for (int i = 0; i < ngenPart; ++i) {
									if((abs(Id[i]) == leptonId)&&(status[i]== 1)){ 

										//Electrons selection
										double R2 = DeltaR(gen_eta[i],Oeta[j],gen_phi[i],Ophi[j] );

										//Minimise DeltaR and Fill the other variables
										if (R > R2) {

											R = R2;
											delta_P = abs(Opt[j]-Pt[i])/Pt[i];
											delta_charge = abs(Oq[j] - charge[i]);
										}
									}
								}

								//Choose the parameter to be filled for the eff.
								if(par_x == "Pt"){par = Opt[j];}
								else if(par_x == "eta"){par = Oeta[j];}
								else if(par_x == "phi"){par = Ophi[j];}
								if(par_y == "Pt"){par_2 = Opt[j];}
								else if(par_y == "eta"){par_2 = abs(Oeta[j]);}
								else if(par_y == "phi"){par_2 = abs(Ophi[j]);}

								//Fill Pt only for matched events
								if(((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5))||option.Contains("unmat")){

								  for(int ii = 0; ii < npar2bins; ++ii){
								    if((par_2 > par2[ii])&&(par_2 <= par2[ii+1])){histo_den[ii]->Fill(par); histo_par1[ii]->Fill(par); histo_den_O[ii]->Fill(par); ++othercount; histo_counter_O_par1[ii]->Fill(par);}
									}


									//Additional cut on the numerator
									int a = 0;

									if((sel_num == "tightcut")&&(abs(Oid[j]) == 13)&&(Otight[j] == 1)){a = 1;}
									if((sel_num == "tightcut")&&(abs(Oid[j]) == 11)&&(Otighte[j] >= 3)){a = 1;}
									if((sel_num == "reliso3")&&(Oiso3[j] <= cut_num)){a = 2;}
									if((sel_num == "reliso4")&&(Oiso4[j] <= cut_num)){a = 3;}
									if((sel_num == "chiso3")&&(Ochiso3[j] <= cut_num)){a = 4;}
									if((sel_num == "chiso4")&&(Ochiso4[j] <= cut_num)){a = 5;}
									if((sel_num == "dxy")&&(abs(Odxy[j]) <= cut_num)){a = 6;}
									if((sel_num == "dz")&&(abs(Odz[j]) <= cut_num)){a = 7;}
									if((sel_num == "tightmva")&&(abs(Oid[j]) == 11)&&(Otight[j] == 1)){a = 9;}
									//Never filled here
									if(sel_num == "loose"){a = 8;}


									//Find the corresponding histogram for par2
									TH1D* hist;
									TH1D* hist_O;

									bool found = false;
								  	for(int _i = 0; _i < npar2bins; ++_i){
								  	  if((par_2 > par2[_i])&&(par_2 <= par2[_i+1])){hist = histo_num[_i]; hist_O = histo_num_O[_i]; found = true;}
								  	}
									if(!found){a = 0;}

									switch(a){

										case 0:

											break;

										case 1:
											hist->Fill(par);
											hist_O->Fill(par);
											break;

										case 2:
											hist->Fill(par);
											hist_O->Fill(par);
											break;
										case 3:
											hist->Fill(par);
											hist_O->Fill(par);
											break;
										case 4:
											hist->Fill(par);
											hist_O->Fill(par);
											break;
										case 5:
											hist->Fill(par);
											hist_O->Fill(par);
											break;
										case 6:
											hist->Fill(par);
											hist_O->Fill(par);
											break;

										case 7:
											hist->Fill(par);
											hist_O->Fill(par);
											break;

										case 8:
											//Nothing to fill
											break;

										case 9:
											hist->Fill(par);
											hist_O->Fill(par);
											break;

									}
								}
							}
						}
					}
				}
			}
		}


		//loop on tight 
		for(int j=0; j<Gn;++j){
			if((!option.Contains(" ll "))||((option.Contains(" ll "))&&(Gn == 2)&&(Gid[0] == -Gid[1]))){
				if(abs(Gid[j]) == leptonId){
					//Cut on the denominator
					if((sel_den != "tightcut")||(((abs(Gid[j]) == 13)&&(sel_den == "tightcut")&&(Gtight[j] == 1 ))||((abs(Gid[j]) == 11)&&(sel_den == "tightcut")&&(Gtighte[j] >= 3)))){
						if((sel_den != "tightmva")||((abs(Gid[j]) == 11)&&(sel_den == "tightmva")&&(Gtight[j] == 1))){
							//Veto the EE-EB gape

							//Variable for matching
							double R = 999;
							double delta_P = 999;
							double delta_charge = 999;

							//Parameter on the xaxis

							double par;
							double par_2;

							//loop over all generated particles to do the matching
							for (int i = 0; i < ngenPart; ++i) {
								if((abs(Id[i]) == leptonId)&&(status[i]== 1)){ 

									//Electrons selection
									double R2 = DeltaR(gen_eta[i],Geta[j],gen_phi[i],Gphi[j] );

									//Minimise DeltaR and Fill the other variables
									if (R > R2) {

										R = R2;
										delta_P = abs(Gpt[j]-Pt[i])/Pt[i];
										delta_charge = abs(Gq[j] - charge[i]);
									}
								}
							}

							//Choose the parameter to be filled for the eff.
							if(par_x == "Pt"){par = Gpt[j];}
							else if(par_x == "eta"){par = Geta[j];}
							else if(par_x == "phi"){par = Gphi[j];}
							if(par_y == "Pt"){par_2 = Gpt[j];}
							else if(par_y == "eta"){par_2 = abs(Geta[j]);}
							else if(par_y == "phi"){par_2 = abs(Gphi[j]);}

							//Fill Pt only for matched events
							if(((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5))||option.Contains("unmat")){

								  for(int ii = 0; ii < npar2bins; ++ii){
								    if((par_2 > par2[ii])&&(par_2 <= par2[ii+1])){histo_den[ii]->Fill(par);histo_par1[ii]->Fill(par); histo_den_G[ii]->Fill(par); ++goodcount; histo_counter_G_par1[ii]->Fill(par);}
									}

								//Additional cut on the numerator
								int a = 0;

								if((sel_num == "tightcut")&&(abs(Gid[j]) == 13)&&(Gtight[j] == 1)){a = 1;}
								if((sel_num == "tightcut")&&(abs(Gid[j]) == 11)&&(Gtighte[j] >= 3)){a = 1;}
								if((sel_num == "reliso3")&&(Giso3[j] <= cut_num)){a = 2;}
								if((sel_num == "reliso4")&&(Giso4[j] <= cut_num)){a = 3;}
								if((sel_num == "chiso3")&&(Gchiso3[j] <= cut_num)){a = 4;}
								if((sel_num == "chiso4")&&(Gchiso4[j] <= cut_num)){a = 5;}
								if((sel_num == "dxy")&&(abs(Gdxy[j]) <= cut_num)){a = 6;}
								if((sel_num == "dz")&&(abs(Gdz[j]) <= cut_num)){a = 7;}
								if((sel_num == "tightmva")&&(abs(Gid[j]) == 11)&&(Gtight[j] == 1)){a = 9;}
								//Only loose leptons here, so this is filled anyway
								if((sel_num == "loose")){a = 8;}
								//if((sel_num == "mvaid")&&(abs(Gmvaid[j]) >= cut_num)){a = 9;}


								//Find the corresponding histogram for par2
								TH1D* hist;
								TH1D* hist_G;

								bool found = false;
								for(int _i = 0; _i < npar2bins; ++_i){
								  if((par_2 > par2[_i])&&(par_2 <= par2[_i+1])){hist = histo_num[_i];hist_G = histo_num_G[_i];found = true;}
								}
								if(!found){a = 0;}

								switch(a){

									case 0:

										break;

									case 1:
										hist->Fill(par);
										hist_G->Fill(par);
										break;

									case 2:
										hist->Fill(par);
										hist_G->Fill(par);
										break;
									case 3:
										hist->Fill(par);
										hist_G->Fill(par);
										break;
									case 4:
										hist->Fill(par);
										hist_G->Fill(par);
										break;
									case 5:
										hist->Fill(par);
										hist_G->Fill(par);
										break;
									case 6:
										hist->Fill(par);
										hist_G->Fill(par);
										break;

									case 7:
										hist->Fill(par);
										hist_G->Fill(par);
										break;

									case 8:
										hist->Fill(par);
										hist_G->Fill(par);
										break;

									case 9:
										hist->Fill(par);
										hist_G->Fill(par);
										break;
								}
							}
						}
					}
				}
			}
		}
	}

        mkdir(_path+_fname+"_PDF/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	///////////////////
	//Draw histograms//
	///////////////////

	//Canvas declaration
	for(int i = 0; i < npar2bins; ++i){

	  ////////////////////
	  //Build histograms//
	  ////////////////////

	  histo_num[i]->Sumw2();
	  histo_den[i]->Sumw2();
	  eff[i]->Divide(histo_num[i],histo_den[i],1,1,"B");

	  histo_num_O[i]->Sumw2();
	  histo_den_O[i]->Sumw2();
	  eff_O[i]->Divide(histo_num_O[i],histo_den_O[i],1,1,"B");

	  histo_num_G[i]->Sumw2();
	  histo_den_G[i]->Sumw2();
	  eff_G[i]->Divide(histo_num_G[i],histo_den_G[i],1,1,"B");

	  histo_counter_G_par1[i]->Sumw2();
	  histo_counter_O_par1[i]->Sumw2();
	  histo_counter_par1[i]->Divide(histo_counter_O_par1[i],histo_counter_G_par1[i],1,1,"B");

	  histo_counter[i]->Fill(0.5,goodcount);
	  histo_counter[i]->Fill(1.5,othercount);

	  //String for name of the ouput files and histograms titles
	  //

	  TString _parybin;

	  //Parameter string
	  if(par_y == "Pt"){_parybin = Form("%0.f_Pt%0.f",par2[i],par2[i+1]);}
	  else if(par_y == "eta"){_parybin = Form("%0.3f_eta%0.3f",par2[i],par2[i+1]);cout<<"it works !"<<endl;}
	  else if(par_y == "phi"){_parybin = Form("%0.3f_phi%0.3f",par2[i],par2[i+1]);}

	  TString _parytitle;

	  //Title string
	  if(par_y == "Pt"){_parytitle = Form("%0.f #leq P_{t} #leq %0.f",par2[i],par2[i+1]);}
	  else if(par_y == "eta"){_parytitle = Form("%0.3f #leq #||{#eta}  #leq %0.3f",par2[i],par2[i+1]);cout<<"it works !"<<endl;}
	  else if(par_y == "phi"){_parytitle = Form("%0.3f #leq #||{#phi}  #leq %0.3f",par2[i],par2[i+1]);}

	  //Draw histograms
	  TCanvas* c1 = new TCanvas("c1","c1");
	  c1->cd();
	  eff[i]->Draw();
	  eff[i]->GetYaxis()->SetTitle("#epsilon");
	  eff[i]->GetXaxis()->SetTitle(_par);
	  eff[i]->SetMarkerStyle(20);
	  eff[i]->SetMarkerSize(1);
	  eff[i]->SetMarkerColor(4);
	  eff[i]->SetLineColor(4);
	  eff[i]->SetTitle(_sel_num+" for "+_sel_den+" "+pname+" "+_parytitle);

	  TCanvas* c_par1 = new TCanvas("cpar1","cpar1");
	  TString _partitle = _par + (TString)" distribution for "+pname+", "+_parytitle+", "+sel_num;
	  c_par1->cd();
	  histo_par1[i]->Draw();
	  histo_par1[i]->SetTitle(_partitle);
	  histo_par1[i]->GetXaxis()->SetTitle(_par);
	  histo_par1[i]->SetLineWidth(2);
	  histo_par1[i]->SetLineColor(4);
	  histo_par1[i]->SetMarkerColor(4);

	  TCanvas* cO = new TCanvas("cO","cO");
	  cO->cd();
	  eff_O[i]->Draw();
	  eff_O[i]->GetYaxis()->SetTitle("#epsilon");
	  eff_O[i]->GetXaxis()->SetTitle(_par);
	  eff_O[i]->SetMarkerStyle(20);
	  eff_O[i]->SetMarkerSize(1);
	  eff_O[i]->SetMarkerColor(4);
	  eff_O[i]->SetLineColor(4);
	  eff_O[i]->SetTitle(_sel_num+" for "+_sel_den+" "+pname+" "+_parytitle+" , LepOther");

	  TCanvas* cG = new TCanvas("cG","cG");
	  cG->cd();
	  eff_G[i]->Draw();
	  eff_G[i]->GetYaxis()->SetTitle("#epsilon");
	  eff_G[i]->GetXaxis()->SetTitle(_par);
	  eff_G[i]->SetMarkerStyle(20);
	  eff_G[i]->SetMarkerSize(1);
	  eff_G[i]->SetMarkerColor(4);
	  eff_G[i]->SetLineColor(4);
	  eff_G[i]->SetTitle(_sel_num+" for "+_sel_den+" "+pname+" "+_parytitle+" , LepGood");

	  TCanvas* ccpar = new TCanvas("ccpar","ccpar");
	  ccpar->cd();
	  histo_counter_par1[i]->Draw();

	  TCanvas* cc = new TCanvas("cc","cc");
	  cc->cd();
	  histo_counter[i]->Draw();

	  /////////////////////
	  //Saving the output//
	  /////////////////////

	  //Write pdf
	  TString cname = "eff4test"+_option+_pname+_par1range+"_"+_parybin+"_den_"+_sel_den+"_num_"+_sel_num;
	  c1->SaveAs(_path+_fname+"_PDF/"+cname+".pdf");
	  c_par1->SaveAs(_path+_fname+"_PDF/"+cname+"par_distr.pdf");
	  cO->SaveAs(_path+_fname+"_PDF/"+cname+"_LepOther.pdf");
	  cG->SaveAs(_path+_fname+"_PDF/"+cname+"_LepGood.pdf");
	  ccpar->SaveAs(_path+_fname+"_PDF/"+cname+"_counter_par.pdf");
	  cc->SaveAs(_path+_fname+"_PDF/"+cname+"_counter.pdf");

	  //Write in output file
	  file_out->cd();
	  eff[i]->Write("eff"+_parybin);
	  histo_par1[i]->Write("histo_par1_"+_parybin);
	  eff_O[i]->Write("eff_LepOther"+_parybin);
	  eff_G[i]->Write("eff_LepGood"+_parybin);
	  histo_counter_par1[i]->Write("counter_par"+_parybin);
	  histo_counter[i]->Write("counter"+_parybin);

	}

	file_out->Close();
	return 0;

}
