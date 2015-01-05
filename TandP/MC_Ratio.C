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

int MC_Ratio(int leptonId, double par_low, double par_upp, int nbins, TString sel_den , TString sel_num, double cut_den = 0., double cut_num = 0., TString par_x = "Pt", TString option = ""){

	setTDRStyle();

	///////////////
	//Get the TTree
	///////////////

	//Location of the .root file
	TString location = "/Users/GLP/Desktop/CERN_data/2014-11-13_skim2ll-mva-softbtag/postprocessed/";

	//Reading the tree 
	//
	TChain* tree = new TChain("treeProducerSusyMultilepton");

	//DY events
	tree->Add(location+"DYJetsToLLM50_PU_S14_POSTLS170.root");

	//Plot the result

	Long64_t n = tree->GetEntries();

	//Path for input and output file. Written in FitDataPath.txt
	TString _path = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_ratio/";

	//////////////////////
	//Name for the plots//
	//////////////////////

	TString pname;
	TString _pname;
	TString _par;
	TString _sel_num;
	TString _sel_den;
	TString _cut_den;
	TString _option;

	//Writing string
	if (leptonId == 11) {pname = "e";_pname = "e";}
	else if (leptonId == 13){pname = " #mu";_pname = "mu";}
	if(par_x == "Pt"){_par = "P_{t}";}
	else if(par_x == "eta"){_par = "#eta";}
	else if(par_x == "phi"){_par = "#phi";}
	else{cout<<"ERROR: wrong parameter name !";return 1;}
	if((sel_num == "tightmva")&&(leptonId == 13)){cout<<"ERROR: no tightId MVA defined for the muon !";return 1;}
	if(sel_num == ""){_sel_num = "unsel";}
	else if(sel_num == "tightcut"){_sel_num = "tightcut";}
	else if(sel_num == "tightmva"){_sel_num = "tightmva";}
	//else if(sel_num == "mvaid"){_sel_num = Form("tightmva%0.3lf",cut_num) ;}
	else if(sel_num == "loose"){_sel_num = "loose";}
	else if(sel_num == "reliso3"){_sel_num = Form("reliso3_%0.3lf",cut_num);}
	else if(sel_num == "reliso4"){_sel_num = Form("reliso4_%0.3lf",cut_num);}
	else if(sel_num == "chiso3"){_sel_num = Form("chiso3_%0.3lf",cut_num);}
	else if(sel_num == "chiso4"){_sel_num = Form("chiso4_%0.3lf",cut_num);}
	else if(sel_num == "dxy"){_sel_num = Form("dxy_%0.3lf",cut_num);}
	else if(sel_num == "dz"){_sel_num = Form("dz_%0.3lf",cut_num);}
	else{cout<<"ERROR: wrong numerator name !";return 1;}
	//Selection on the denominator
	if((sel_den == "tightmva")&&(leptonId == 13)){cout<<"ERROR: no tightId MVA defined for the muon !";return 1;}
	if(sel_den == ""){_sel_den = "unsel";}
	else if(sel_den == "tightcut"){_sel_den = "tightcut";}
	else if(sel_den == "tightmva"){_sel_den = "tightmva";}
	//else if(sel_den == "mvaid"){_sel_den = Form("tightmva%0.3lf",cut_den) ;}
	else if(sel_den == "loose"){_sel_den = "loose";}
	else{cout<<"ERROR: wrong denominator selection name !";return 1;}
	option.Append(" ");
	option.Prepend(" ");
	if(option.Contains(" ll ")){_option += "_ll";}
	if(option.Contains(" unmatched ")){_option += "_unmatched";}
	if(option.Contains(" alleta ")){_option += "_alleta";}
	_option += "_";

	//Name of the output

	//TString _fname = file_name("eff3",leptonId, par_low,par_upp,sel_den,sel_num,cut_den,cut_num,par_x,option);
	//TString _output= _path+_fname + ".root";
	TString _output = _path+"eff4test"+_option+_pname+"_den_"+_sel_den+"_num_"+_sel_num+"_"+par_x+".root";

	//Declaration of histogram


	//Preparation for general range in eta/pt

	const int nrange = 3;
	double par2[nrange] = {0,0.8,1.5};

	TH1D **histo_num = new TH1D*[nrange];
	TH1D **histo_den = new TH1D*[nrange];
	TH1D **eff = new TH1D*[nrange];

	for(int _i = 0; _i < nrange; ++_i){ 

		//Barrel
		histo_num[_i] = new TH1D("histo_num","Pt",nbins,min(par_low,0.),par_upp);
		histo_den[_i] = new TH1D("histo_den","Pt",nbins,min(par_low,0.),par_upp);
		eff[_i] = new TH1D("eff","Pt",nbins,min(par_low,0.),par_upp);

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
	tree->SetBranchAddress("LepOther_looseIdSusy",Oloose);
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
	tree->SetBranchAddress("LepGood_looseIdSusy",&Gloose);
	tree->SetBranchAddress("LepGood_relIso03",&Giso3);
	tree->SetBranchAddress("LepGood_relIso04",&Giso4);
	tree->SetBranchAddress("LepGood_chargedHadRelIso03",&Gchiso3);
	tree->SetBranchAddress("LepGood_chargedHadRelIso04",&Gchiso4);
	tree->SetBranchAddress("LepGood_dxy",&Gdxy);
	tree->SetBranchAddress("LepGood_dz",&Gdz);

	int count = 0;

	//Start loop over all events
	for (int k = 0; k < 100000; ++k) {

		if( 100*(double)k/n> count){cout<<count<<endl;++count;}

		tree->GetEntry(k);

		//loop on other (not loose) 
		//Selection on denominator
		if(sel_den != "loose"){
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

								//Fill Pt only for matched events
								if(((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5))||option.Contains("unmat")){
									//Filling the den


									for(int ii = 0; ii < nrange; ++ii){

										if((ii < nrange-1)&&(abs(Oeta[j]) >= par2[ii])&&(abs(Oeta[j]) < par2[ii+1])){histo_den[ii]->Fill(par);}
										else if((ii == nrange-1)&&(abs(Oeta[j]) >= par2[ii])){histo_den[ii]->Fill(par);}//Put large par2 into large bin
										else{}

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
									if((sel_num == "tightmva")&&(abs(Oid[j] == 11))&&(Otight[j] == 1)){a = 9;}
									//Never filled here
									if(sel_num == "loose"){a = 8;}


									//Find the corresponding histogram for par2
									TH1D* hist;
									bool notfund = true;
									int ii = 0;
									while(notfund){

										if((ii < nrange-1)&&(abs(Oeta[j]) > par2[ii])&&(abs(Oeta[j]) <= par2[ii+1])){hist = histo_num[ii]; notfund = false;}
										else if((ii == nrange-1)&&(abs(Oeta[j]) >= par2[ii])){hist = histo_num[ii]; notfund = false;}
										if(ii == nrange){cout<<"Error!"; return 1;}
										++ii;

									}

									switch(a){

										case 0:

											break;

										case 1:
											hist->Fill(par);
											break;

										case 2:
											hist->Fill(par);
											break;
										case 3:
											hist->Fill(par);
											break;
										case 4:
											hist->Fill(par);
											break;
										case 5:
											hist->Fill(par);
											break;
										case 6:
											hist->Fill(par);
											break;

										case 7:
											hist->Fill(par);
											break;

										case 8:
											//Nothing to fill
											break;

										case 9:
											hist->Fill(par);
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

							//Fill Pt only for matched events
							if(((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5))||option.Contains("unmat")){
								//Filling the den
								//
								//Old Code
								//////////
								//if(option.Contains(" alleta ")){
								//	histo_den->Fill(par);
								//}else{
								//	if(abs(Geta[j]) < 1.2){histo_denB->Fill(par);}
								//	if(abs(Geta[j]) >= 1.2){histo_denE->Fill(par);}
								//}

								//New Code
								//////////

								for(int ii = 0; ii < nrange; ++ii){

									if((ii < nrange-1)&&(abs(Geta[j]) >= par2[ii])&&(abs(Geta[j]) < par2[ii+1])){histo_den[ii]->Fill(par);}
									else if((ii == nrange-1)&&(abs(Geta[j]) >= par2[ii])){histo_den[ii]->Fill(par);}//Put large par2 into large bin

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
								if((sel_num == "tightmva")&&(abs(Gid[j] == 11))&&(Gtight[j] == 1)){a = 9;}
								//Only loose leptons here, so this is filled anyway
								if((sel_num == "loose")){a = 8;}
								//if((sel_num == "mvaid")&&(abs(Gmvaid[j]) >= cut_num)){a = 9;}


								//Find the corresponding histogram for par2
								TH1D* hist;
								bool notfund = true;
								int ii = 0;
								while(notfund){

									if((ii < nrange-1)&&(abs(Geta[j]) > par2[ii])&&(abs(Geta[j]) <= par2[ii+1])){hist = histo_num[ii]; notfund = false;}
									else if((ii == nrange-1)&&(abs(Geta[j]) >= par2[ii])){hist = histo_num[ii]; notfund = false;}
									if(ii == nrange){cout<<"Error!"; return 1;}
									++ii;

								}

								switch(a){

									case 0:

										break;

									case 1:
										hist->Fill(par);
										break;

									case 2:
										hist->Fill(par);
										break;
									case 3:
										hist->Fill(par);
										break;
									case 4:
										hist->Fill(par);
										break;
									case 5:
										hist->Fill(par);
										break;
									case 6:
										hist->Fill(par);
										break;

									case 7:
										hist->Fill(par);
										break;

									case 8:
										hist->Fill(par);
										break;

									case 9:
										hist->Fill(par);
										break;
								}
							}
						}
					}
				}
			}
		}
	}



	//Canvas declaration

	TCanvas** c = new TCanvas*[nrange];

	for(int i = 0; i < nrange; ++i){

		c[i] = new TCanvas(Form("c%i",i),"c");
	}


	for(int i = 0; i < nrange; ++i){

		histo_num[i]->Sumw2();
		histo_den[i]->Sumw2();

		eff[i]->Divide(histo_num[i],histo_den[i],1,1,"B");

		//Name of the par2 range
		TString _par2;

		if(i < nrange-1){_par2 = Form("%0.3lf<",par2[i])+par_x+Form("<%0.3lf",par2[i+1]);}
		else if(i == nrange-1){_par2 = Form("%0.3lf<",par2[i])+par_x;}

		//Efficiency of the iso cut.
		c[i]->cd();
		eff[i]->Draw();
		eff[i]->GetYaxis()->SetTitle("#epsilon");
		eff[i]->GetXaxis()->SetTitle(_par);
		//eff->GetXaxis()->SetRangeUser(0,250);
		eff[i]->SetMarkerStyle(20);
		eff[i]->SetMarkerSize(1);
		eff[i]->SetMarkerColor(4);
		eff[i]->SetLineColor(4);
		eff[i]->SetTitle(_sel_num+" for "+_sel_den+" "+pname+" "+_par2);

		//Define the name of the canvas
		TString cname = "eff4test"+_option+_pname+"_den_"+_sel_den+"_num_"+_sel_num+"_"+_par2;

		c[i]->SaveAs(_path+"PDF/"+cname+".pdf");

		TFile* output = new TFile(_output,"recreate");
		eff[i]->Write("eff");
		output->Close();

	}


	return 0;

}
