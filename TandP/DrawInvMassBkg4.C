/*********************************************
 * Description - This file plots the histograms for Z+bkg. THis used the files contained in 2014-11-13_skim2ll-mva-softbtag. 
 Update w.r.t. previous code: all lepton are now considered (loose and tight). 


 * Author - Gaël L. Perrin

 * Date - Jan 06 2015

 * *******************************************/

#include "cmath"
#include "TChain.h"
#include <sys/stat.h>
#include "iostream"
#include "fstream"
#include "vector"
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
#include "../setTDRStyle.C"
#include "../InvMass.C"
#include "../DeltaR.C"

////////////////////////
//Variable description//
////////////////////////

//tree:		TTree to be analysed
//leptonId:	pdgId of the lepton
//par_low	low boundary of Pt bins
//par_upp	high boundary of Pt bins
//nparbins	number of Pt bins
//select	selection of Tag AND Probe. Can take parameters such as: "loose", "tightcut", "tightmva" (for electron only).
//effcut	additional requirement on Tag (the selection on tag is select + effcut). Can take the parameter: "loose", "tightcut", "tightmva", "dxy", "dz", "reliso3", "reliso4", "chiso3", "chiso4".
//cut		cut parameter of effcut is possible.
//option	can take: matching (a matching between the Tag and Probe is required).

int DrawInvMassBkgMain(TTree* tree, int leptonId, double par_low = 10, double par_upp = 50,int nparbins = 10, TString select = "tightcut", TString effcut = "reliso3", double cut = 0.2,TString par_x = "Pt", TString par_y = "eta", TString option ="");

int DrawInvMassBkg(int leptonId, double par_low = 10 , double par_upp = 50 ,int nparbins = 10,TString select = "tightcut", TString effcut = "reliso3", double cut = 0.2, TString par_x = "Pt", TString par_y = "eta", TString option = ""){

	//Location of the .root file
	TString location = "/Users/GLP/Desktop/CERN_data/2014-11-13_skim2ll-mva-softbtag/postprocessed/matched/";

	//Reading the tree 
	//
	TChain* tree = new TChain("treeProducerSusyMultilepton");

	//DY events
	tree->Add(location+"DYJetsToLLM50_PU_S14_POSTLS170.root");

	//WJet events
	tree->Add(location+"WJetsToLNu_13TeV-madgraph-pythia8-tauola.root");

	//TTJets events
	tree->Add(location+"TTJets_MSDecaysCKM_central_PU_S14_POSTLS170.root");

	//Plot the result
	//
	DrawInvMassBkgMain(tree,leptonId ,par_low ,par_upp ,nparbins,select,effcut, cut,par_x, option);

	return 0;

}

int     DrawInvMassBkgMain(TTree* tree, int leptonId, double par_low , double par_upp ,int nparbins , TString select, TString effcut , double cut, TString par_x, TString par_y, TString option){

	TString _path= "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/";

	setTDRStyle();

	//Some variables
	Long64_t n = tree->GetEntries();
	int nbins = 200;
	double Dpar = (par_upp-par_low)/nparbins;

	/////////////////////////
	//Name of the output file
	/////////////////////////

	//particle string
	TString pname;
	if(abs(leptonId) == 11){pname = "e";}
	if(abs(leptonId) == 13){pname = "mu";}

	//Parameter string
	TString _par;
	TString _par_2;
	if(par_x == "Pt"){_par = "P_{t}";}
	else if(par_x == "eta"){_par = "#eta";}
	else if(par_x == "phi"){_par = "#phi";}
	if(par_y == "Pt"){_par_2 = "P_{t}";}
	else if(par_y == "eta"){_par_2 = "#eta";}
	else if(par_y == "phi"){_par_2 = "#phi";}

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

	TString _fname = "InvM4test";
	if(option.Contains("matching")){_fname += "_Matched";}

	TString _ptrange;
	_ptrange = Form("Pt%0.f_Pt%0.f",par_low,par_upp);
	_fname += "_"+_ptrange;
	_fname += "_"+_effcut+"_for_"+_sel+"_"+pname; 

	TFile* file_out = new TFile(_path+_fname+".root","recreate");

	//Parameter 2

	const int nrange = 2;
	double par2[nrange] = {0,1.2};

	//cout<<"debug0"<<endl;

	TH1D*** histo_M_DYJets_bkg  = new TH1D**[nrange];
	TH1D*** histo_M_DYJets  = new TH1D**[nrange];
	TH1D*** histo_M_bkg  = new TH1D**[nrange];
	TH1D*** histo_M_WJets  = new TH1D**[nrange];
	TH1D*** histo_M_TTJets  = new TH1D**[nrange];
	TH1D*** histo_M_DYJets_bkg_fail  = new TH1D**[nrange];
	TH1D*** histo_M_DYJets_fail  = new TH1D**[nrange];
	TH1D*** histo_M_bkg_fail  = new TH1D**[nrange];
	TH1D*** histo_M_WJets_fail  = new TH1D**[nrange];
	TH1D*** histo_M_TTJets_fail  = new TH1D**[nrange];

	for(int i = 0; i < nrange; ++i){ 

		histo_M_DYJets_bkg[i]  		= new TH1D*[nparbins+1];
		histo_M_DYJets[i]  		= new TH1D*[nparbins+1];
		histo_M_bkg[i]  		= new TH1D*[nparbins+1];
		histo_M_WJets[i]  		= new TH1D*[nparbins+1];
		histo_M_TTJets[i]  		= new TH1D*[nparbins+1];
		histo_M_DYJets_bkg_fail[i] 	= new TH1D*[nparbins+1];
		histo_M_DYJets_fail[i]     	= new TH1D*[nparbins+1];
		histo_M_bkg_fail[i]  		= new TH1D*[nparbins+1];
		histo_M_WJets_fail[i]  		= new TH1D*[nparbins+1];
		histo_M_TTJets_fail[i]  	= new TH1D*[nparbins+1];

		for(int j = 0; j < nparbins+1; ++j){ 

			histo_M_DYJets_bkg[i][j]  		=  new TH1D("histo_M_DYJets_bkg","M",nbins,0,250);
			histo_M_DYJets[i][j]  			=  new TH1D("histo_M_DYJets","M",nbins,0,250);
			histo_M_bkg[i][j]  			=  new TH1D("histo_M_bkg","M",nbins,0,250);
			histo_M_WJets[i][j]  			=  new TH1D("histo_M_WYJets","M",nbins,0,250);
			histo_M_TTJets[i][j]  			=  new TH1D("histo_M_TTJets","M",nbins,0,250);
			histo_M_DYJets_bkg_fail[i][j]	 	=  new TH1D("histo_M_DYJets_bkg_fail","M",nbins,0,250);
			histo_M_DYJets_fail[i][j]     		=  new TH1D("histo_M_DYJets_fail","M",nbins,0,250);
			histo_M_bkg_fail[i][j]  		=  new TH1D("histo_M_bkg_fail","M",nbins,0,250);
			histo_M_WJets_fail[i][j]  		=  new TH1D("histo_M_WJets_fail","M",nbins,0,250);
			histo_M_TTJets_fail[i][j]  		=  new TH1D("histo_M_TTJets_fail","M",nbins,0,250);


		}

	}

	//cout<<"debug1"<<endl;

	//Old

	//for(int _i = 0; _i < nparbins+1; ++_i){ 

	//	double _par1;
	//	double _par2;
	//	if(_i*Dpar+par_low < par_upp){_par1 = _i*Dpar+par_low; _par2 = (_i+1)*Dpar;}
	//	else if (_i*Dpar+par_low >= par_upp){_par1 = _i*Dpar+par_low; _par2 = 10000;}

	//	//Barrel
	//	histo_M_DYJets_bkg[_i] 		= new TH1D("histo_M_DYJets_bkg","M",nbins,0,250);
	//	histo_M_DYJets[_i] 		= new TH1D("histo_M_DYJets","M",nbins,0,250);
	//	histo_M_bkg[_i] 			= new TH1D("histo_M_bkg","M",nbins,0,250);
	//	histo_M_WJets[_i] 		= new TH1D("histo_M_WYJets","M",nbins,0,250);
	//	histo_M_DYJets_bkg_fail[_i] 	= new TH1D("histo_M_DYJets_bkg_fail","M",nbins,0,250);
	//	histo_M_DYJets_fail[_i] 		= new TH1D("histo_M_DYJets_fail","M",nbins,0,250);
	//	histo_M_bkg_fail[_i] 		= new TH1D("histo_M_bkg_fail","M",nbins,0,250);
	//	histo_M_WJets_fail[_i] 		= new TH1D("histo_M_WJets_fail","M",nbins,0,250);
	//	//Endcape
	//	histo_M_DYJets_bkg_higheta[_i] 		= new TH1D("histo_M_DYJets_bkg_higheta","M",nbins,0,250);
	//	histo_M_DYJets_higheta[_i] 		= new TH1D("histo_M_DYJets_higheta","M",nbins,0,250);
	//	histo_M_bkg_higheta[_i] 		= new TH1D("histo_M_bkg_higheta","M",nbins,0,250);
	//	histo_M_WJets_higheta[_i] 		= new TH1D("histo_M_WYJets_higheta","M",nbins,0,250);
	//	histo_M_DYJets_bkg_fail_higheta[_i] 	= new TH1D("histo_M_DYJets_bkg_fail_higheta","M",nbins,0,250);
	//	histo_M_DYJets_fail_higheta[_i] 	= new TH1D("histo_M_DYJets_fail_higheta","M",nbins,0,250);
	//	histo_M_bkg_fail_higheta[_i] 		= new TH1D("histo_M_bkg_fail_higheta","M",nbins,0,250);
	//	histo_M_WJets_fail_higheta[_i] = new TH1D("histo_M_WJets_fail_higheta","M",nbins,0,250);

	//}

	//End Old

	//Event variables
	////Generated
	Int_t evt_id;
	Float_t scale;
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
	Int_t Oloose[200];
	Float_t Oiso3[200];
	Float_t Oiso4[200];
	Float_t Ochiso3[200];
	Float_t Ochiso4[200];
	Float_t Odxy[200];
	Float_t Odz[200];
	//loose
	Int_t Gn;
	Int_t matched[200];
	Int_t Gid[200];
	Float_t Gpt[200];
	Float_t Gm[200];
	Float_t Geta[200];
	Float_t Gphi[200];
	Int_t   Gq[200];
	Int_t Gtight[200];
	Int_t Gtighte[200];
	Int_t Gloose[200];
	Float_t Giso3[200];
	Float_t Giso4[200];
	Float_t Gchiso3[200];
	Float_t Gchiso4[200];
	Float_t Gdxy[200];
	Float_t Gdz[200];

	//Assigne branches
	tree->SetBranchAddress("evt_scale1fb", &scale);
	tree->SetBranchAddress("evt_id", &evt_id);
	//not loose
	tree->SetBranchAddress("nLepOther",&On);
	tree->SetBranchAddress("LepOther_pdgId",&Oid);
	tree->SetBranchAddress("LepOther_pt",&Opt);
	tree->SetBranchAddress("LepOther_mass",&Om);
	tree->SetBranchAddress("LepOther_eta",&Oeta);
	tree->SetBranchAddress("LepOther_phi",&Ophi);
	tree->SetBranchAddress("LepOther_charge",&Oq);
	tree->SetBranchAddress("LepOther_tightId",&Otight);
	tree->SetBranchAddress("LepGood_eleCutIdCSA14_50ns_v1",&Otighte);
	tree->SetBranchAddress("LepOther_relIso03",&Oiso3);
	tree->SetBranchAddress("LepOther_relIso04",&Oiso4);
	tree->SetBranchAddress("LepOther_chargedHadRelIso03",&Ochiso3);
	tree->SetBranchAddress("LepOther_chargedHadRelIso04",&Ochiso4);
	tree->SetBranchAddress("LepOther_dxy",&Odxy);
	tree->SetBranchAddress("LepOther_dz",&Odz);
	//Loose
	tree->SetBranchAddress("nLepGood",&Gn);
	tree->SetBranchAddress("LepGood_matched",&matched);
	tree->SetBranchAddress("LepGood_pdgId",&Gid);
	tree->SetBranchAddress("LepGood_pt",&Gpt);
	tree->SetBranchAddress("LepGood_mass",&Gm);
	tree->SetBranchAddress("LepGood_eta",&Geta);
	tree->SetBranchAddress("LepGood_phi",&Gphi);
	tree->SetBranchAddress("LepGood_charge",&Gq);
	tree->SetBranchAddress("LepGood_tightId",&Gtight);
	tree->SetBranchAddress("LepGood_eleCutIdCSA14_50ns_v1",&Gtighte);
	tree->SetBranchAddress("LepGood_relIso03",&Giso3);
	tree->SetBranchAddress("LepGood_relIso04",&Giso4);
	tree->SetBranchAddress("LepGood_chargedHadRelIso03",&Gchiso3);
	tree->SetBranchAddress("LepGood_chargedHadRelIso04",&Gchiso4);
	tree->SetBranchAddress("LepGood_dxy",&Gdxy);
	tree->SetBranchAddress("LepGood_dz",&Gdz);

	int count = 0;

	//Start loop over all events
	for (int k = 0; k < n; ++k) {

	if( 100*(double)k/n> count){cout<<count<<endl;++count;}

	//cout<<"debug2"<<endl;

		//Definitions to use T&P
		int tag[2];
		int prob[2];
		tag[0] = 9999;
		tag[1] = 9999;
		prob[0] = 9999;
		prob[1] = 9999;

		tree->GetEntry(k);

		//looseId leptons
		if(select != "loose"){
			for (int i = 0; i < On; ++i) {
	//cout<<"debug3"<<endl;

				//define selections using bools
				bool reliso3((effcut != "reliso3")||((effcut == "reliso3")&&(Oiso3[i] < cut )));
				bool reliso4((effcut != "reliso4")||((effcut == "reliso4")&&(Oiso4[i] < cut )));
				bool chiso3((effcut != "chiso3")||((effcut == "chiso3")&&(Ochiso3[i] < cut )));
				bool chiso4((effcut != "chiso4")||((effcut == "chiso4")&&(Ochiso4[i] < cut )));
				bool dxy((effcut != "dxy")||((effcut == "dxy")&&(abs(Odxy[i])< cut )));
				bool dz((effcut != "dz")||((effcut == "dz")&&(abs(Odz[i])< cut )));
				bool tight((effcut != "tightcut")||(((abs(Oid[i]) == 13)&&(effcut == "tightcut")&&(Otight[i] == 1))||((abs(Oid[i]) == 11)&&(effcut == "tightcut")&&(Otighte[i] >= 3))));
				bool tightmva((effcut != "tightmva")||((abs(Oid[i]) == 11)&&(effcut == "tightmva")&&(Otight[i] == 1)));


				//Store the values from both the loose and tight in the same variable

				//Prob selection cut
				if(abs(Oid[i]) == leptonId){
					if((select != "tightcut")||(((abs(Oid[i]) == 13)&&(select == "tightcut")&&(Otight[i] == 1))||((abs(Oid[i]) == 11)&&(select == "tightcut")&&(Otighte[i] >= 3)))){ 
						if((select != "tightmva")||((abs(Oid[i]) == 11)&&(select == "tightmva")&&(Otight[i] == 1))){ 
							if((!option.Contains("matching"))||((option.Contains("matching"))&&(matched[i] == 1))){ 

								//Prob1
								if(prob[0] == 9999){prob[0] = i;}
								//Prob2
								if((prob[0] != 9999)&&(prob[0] != i)&&(prob[1] == 9999)){prob[1] = i;}

								//Selection cut for Tag only
								if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightmva){

									if(prob[0] == i){tag[0] = i; }
									if(prob[1] == i){tag[1] = i; }

								}
							}
						}
					}
				}
			}

			//loop over all the rec particles to find the isolation
			//We requiere one tag at least 
			while((tag[0] != 9999)||(tag[1] != 9999)){

				int l1;
				int l2;

				if(tag[0] != 9999){l1 = prob[1]; l2 = tag[0]; tag[0] = 9999;}
				else if(tag[1] != 9999){l1 = prob[0]; l2 = tag[1]; tag[1] = 9999;}

				double M = InvMass(Opt[l1],Oeta[l1],Ophi[l1],Om[l1],Opt[l2],Oeta[l2],Ophi[l2],Om[l2]);

				if(l1 != 9999){

					//Parameter on the xaxis

					double par;
					double par_2;

					//Choose the parameter to be filled for the eff.
					if(par_x == "Pt"){par = Opt[l1];}
					else if(par_x == "eta"){par = Oeta[l1];}
					else if(par_x == "phi"){par = Ophi[l1];}
					if(par_y == "Pt"){par_2 = Opt[l1];}
					else if(par_y == "eta"){par_2 = abs(Oeta[l1]);}
					else if(par_y == "phi"){par_2 = abs(Ophi[l1]);}

					for(int ii = 0; ii < nparbins+1; ++ii){


						//Add all the signal to the Z
						double _par1 = 0;
						double _par2 = 0;
						if(ii*Dpar+par_low < par_upp){_par1 = ii*Dpar+par_low; _par2 = (ii+1)*Dpar+par_low;}
						else if (ii*Dpar+par_low >= par_upp){_par1 = ii*Dpar+par_low; _par2 = 10000;}
						if((par > _par1)&&(par< _par2)){

							bool reliso3((effcut != "reliso3")||((effcut == "reliso3")&&(Oiso3[l1] < cut )));
							bool reliso4((effcut != "reliso4")||((effcut == "reliso4")&&(Oiso4[l1] < cut )));
							bool chiso3((effcut != "chiso3")||((effcut == "chiso3")&&(Ochiso3[l1] < cut )));
							bool chiso4((effcut != "chiso4")||((effcut == "chiso4")&&(Ochiso4[l1] < cut )));
							bool dxy((effcut != "dxy")||((effcut == "dxy")&&(abs(Odxy[l1])< cut )));
							bool dz((effcut != "dz")||((effcut == "dz")&&(abs(Odz[l1])< cut )));
							bool tight((effcut != "tightcut")||(((abs(Oid[l1]) == 13)&&(effcut == "tightcut")&&(Otight[l1] == 1))||((abs(Oid[l1]) == 11)&&(effcut == "tightcut")&&(Otighte[l1] >= 3))));
							bool tightmva((effcut != "tightmva")||((abs(Oid[l1]) == 11)&&(effcut == "tightmva")&&(Otight[l1] == 1)));

							//Find the corresponding histogram for par2
							bool notfund = true;
							int kk = 0;
							while(notfund){

								if((kk < nrange-1)&&(par_2 > par2[kk])&&(par_2 <= par2[kk+1])){notfund = false;}
								else if((kk == nrange-1)&&(par_2 >= par2[kk])){notfund = false;}
								++kk;
								if(kk == nrange){notfund = false; kk = -1;}//Doesn't match the binning. Don't fill histogram
							}

							//Efficiency cut
							if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightmva && (kk!= -1)){

								histo_M_DYJets_bkg[kk][ii]->Fill(M,scale);

								if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){
									histo_M_bkg[kk][ii]->Fill(M,scale);

									//Just the Wjets bkg
									if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){
										histo_M_WJets[kk][ii]->Fill(M,scale);
									}else if (evt_id == 300){

										histo_M_TTJets[kk][ii]->Fill(M,scale);

									}else if((evt_id == 702)||(evt_id == 703)||(evt_id == 704)||(evt_id == 705)||(evt_id == 701)){

										histo_M_DYJets[kk][ii]->Fill(M,scale);

									}
								}

							}

							reliso3 = ((effcut != "reliso3")||((effcut == "reliso3")&&(Oiso3[l1] >= cut )));
							reliso4 = ((effcut != "reliso4")||((effcut == "reliso4")&&(Oiso4[l1] >= cut )));
							chiso3 = ((effcut != "chiso3")||((effcut == "chiso3")&&(Ochiso3[l1] >= cut )));
							chiso4 = ((effcut != "chiso4")||((effcut == "chiso4")&&(Ochiso4[l1] >= cut )));
							dxy = ((effcut != "dxy")||((effcut == "dxy")&&(abs(Odxy[l1]) >= cut )));
							dz = ((effcut != "dz")||((effcut == "dz")&&(abs(Odz[l1]) >= cut )));
							tight = ((effcut != "tightcut")||(((abs(Oid[l1]) == 13)&&(effcut == "tightcut")&&(Otight[l1] != 1))||((abs(Oid[l1]) == 11)&&(effcut == "tightcut")&&(Otighte[l1] < 3))));
							tightmva = ((effcut != "tightmva")||((abs(Oid[l1]) == 11)&&(effcut == "tightmva")&&(Otight[l1] != 1)));

							if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightmva && (kk!= -1)){

								histo_M_DYJets_bkg_fail[kk][ii]->Fill(M,scale);

								if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){
									histo_M_bkg_fail[kk][ii]->Fill(M,scale);

									if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){

										histo_M_WJets_fail[kk][ii]->Fill(M,scale);

									}else if (evt_id == 300){

										histo_M_TTJets_fail[kk][ii]->Fill(M,scale);

									}else if((evt_id == 702)||(evt_id == 703)||(evt_id == 704)||(evt_id == 705)||(evt_id == 701)){

										histo_M_DYJets_fail[kk][ii]->Fill(M,scale);

									}
								}
							}
						}
					}
				}
			}
		}


		for (int i = 0; i < Gn; ++i) {

	//cout<<"debug4"<<endl;


			//define selections using bools

			bool reliso3((effcut != "reliso3")||((effcut == "reliso3")&&(Giso3[i] < cut )));
			bool reliso4((effcut != "reliso4")||((effcut == "reliso4")&&(Giso4[i] < cut )));
			bool chiso3((effcut != "chiso3")||((effcut == "chiso3")&&(Gchiso3[i] < cut )));
			bool chiso4((effcut != "chiso4")||((effcut == "chiso4")&&(Gchiso4[i] < cut )));
			bool dxy((effcut != "dxy")||((effcut == "dxy")&&(abs(Gdxy[i])< cut )));
			bool dz((effcut != "dz")||((effcut == "dz")&&(abs(Gdz[i])< cut )));
			bool tight((effcut != "tightcut")||(((abs(Gid[i]) == 13)&&(effcut == "tightcut")&&(Gtight[i] == 1))||((abs(Gid[i]) == 11)&&(effcut == "tightcut")&&(Gtighte[i] >= 3))));
			bool tightmva((effcut != "tightmva")||((abs(Gid[i]) == 11)&&(effcut == "tightmva")&&(Gtight[i] == 1)));


			//Store the values from both the loose and tight in the same variable

			//Prob selection cut
			if(abs(Gid[i]) == leptonId){
				if((select != "tightcut")||(((abs(Gid[i]) == 13)&&(select == "tightcut")&&(Gtight[i] == 1))||((abs(Gid[i]) == 11)&&(select == "tightcut")&&(Gtighte[i] >= 3)))){ 
					if((select != "tightmva")||((abs(Gid[i]) == 11)&&(select == "tightmva")&&(Gtight[i] == 1))){ 
						if((!option.Contains("matching"))||((option.Contains("matching"))&&(matched[i] == 1))){ 

							//Prob1
							if(prob[0] == 9999){prob[0] = i;}
							//Prob2
							if((prob[0] != 9999)&&(prob[0] != i)&&(prob[1] == 9999)){prob[1] = i;}

							//Selection cut for Tag only
							if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightmva){

								if(prob[0] == i){tag[0] = i; }
								if(prob[1] == i){tag[1] = i; }

							}
						}
					}
				}
			}
		}

		//loop over all the rec particles to find the isolation
		//We requiere one tag at least 
		while((tag[0] != 9999)||(tag[1] != 9999)){

			int l1;
			int l2;

			if(tag[0] != 9999){l1 = prob[1]; l2 = tag[0]; tag[0] = 9999;}
			else if(tag[1] != 9999){l1 = prob[0]; l2 = tag[1]; tag[1] = 9999;}

			double M = InvMass(Gpt[l1],Geta[l1],Gphi[l1],Gm[l1],Gpt[l2],Geta[l2],Gphi[l2],Gm[l2]);

			if(l1 != 9999){

				//Parameter on the xaxis

				double par;
				double par_2;

				//Choose the parameter to be filled for the eff.
				if(par_x == "Pt"){par = Gpt[l1];}
				else if(par_x == "eta"){par = Geta[l1];}
				else if(par_x == "phi"){par = Gphi[l1];}
				if(par_y == "Pt"){par_2 = Gpt[l1];}
				else if(par_y == "eta"){par_2 = abs(Geta[l1]);}
				else if(par_y == "phi"){par_2 = abs(Gphi[l1]);}

				for(int ii = 0; ii < nparbins+1; ++ii){
					//Add all the signal to the Z
					double _par1 = 0;
					double _par2 = 0;
					if(ii*Dpar+par_low < par_upp){_par1 = ii*Dpar+par_low; _par2 = (ii+1)*Dpar+par_low;}
					else if (ii*Dpar+par_low >= par_upp){_par1 = ii*Dpar+par_low; _par2 = 10000;}
					if((Gpt[l1] > _par1)&&(Gpt[l1]< _par2)){


						bool reliso3((effcut != "reliso3")||((effcut == "reliso3")&&(Giso3[l1] < cut )));
						bool reliso4((effcut != "reliso4")||((effcut == "reliso4")&&(Giso4[l1] < cut )));
						bool chiso3((effcut != "chiso3")||((effcut == "chiso3")&&(Gchiso3[l1] < cut )));
						bool chiso4((effcut != "chiso4")||((effcut == "chiso4")&&(Gchiso4[l1] < cut )));
						bool dxy((effcut != "dxy")||((effcut == "dxy")&&(abs(Gdxy[l1])< cut )));
						bool dz((effcut != "dz")||((effcut == "dz")&&(abs(Gdz[l1])< cut )));
						bool tight((effcut != "tightcut")||(((abs(Gid[l1]) == 13)&&(effcut == "tightcut")&&(Gtight[l1] == 1))||((abs(Gid[l1]) == 11)&&(effcut == "tightcut")&&(Gtighte[l1] >= 3))));
						bool tightmva((effcut != "tightmva")||((abs(Gid[l1]) == 11)&&(effcut == "tightmva")&&(Gtight[l1] == 1)));

						//Find the corresponding histogram for par2
						bool notfund = true;
						int kk = 0;
						while(notfund){

							if((kk < nrange-1)&&(par_2 > par2[kk])&&(par_2 <= par2[kk+1])){notfund = false;}
							else if((kk == nrange-1)&&(par_2 >= par2[kk])){notfund = false;}
							++kk;
							if(kk == nrange){notfund = false; kk = -1;}//Doesn't match the binning. Don't fill histogram
						}


						//Efficiency cut
						if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightmva && (kk != -1)){

							histo_M_DYJets_bkg[kk][ii]->Fill(M,scale);

							if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){

								histo_M_bkg[kk][ii]->Fill(M,scale);

								//Just the Wjets bkg
								if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){

									histo_M_WJets[kk][ii]->Fill(M,scale);

								}else if (evt_id == 300){

									histo_M_TTJets[kk][ii]->Fill(M,scale);


								}else if((evt_id == 702)||(evt_id == 703)||(evt_id == 704)||(evt_id == 705)||(evt_id == 701)){

									histo_M_DYJets[kk][ii]->Fill(M,scale);

								}
							}
						}


						reliso3 = ((effcut != "reliso3")||((effcut == "reliso3")&&(Giso3[l1] >= cut )));
						reliso4 = ((effcut != "reliso4")||((effcut == "reliso4")&&(Giso4[l1] >= cut )));
						chiso3 = ((effcut != "chiso3")||((effcut == "chiso3")&&(Gchiso3[l1] >= cut )));
						chiso4 = ((effcut != "chiso4")||((effcut == "chiso4")&&(Gchiso4[l1] >= cut )));
						dxy = ((effcut != "dxy")||((effcut == "dxy")&&(abs(Gdxy[l1]) >= cut )));
						dz = ((effcut != "dz")||((effcut == "dz")&&(abs(Gdz[l1]) >= cut )));
						tight = ((effcut != "tightcut")||(((abs(Gid[l1]) == 13)&&(effcut == "tightcut")&&(Gtight[l1] != 1))||((abs(Gid[l1]) == 11)&&(effcut == "tightcut")&&(Gtighte[l1] < 3))));
						tightmva = ((effcut != "tightmva")||((abs(Gid[l1]) == 11)&&(effcut == "tightmva")&&(Gtight[l1] != 1)));

						if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightmva && (kk != -1)){

							histo_M_DYJets_bkg_fail[kk][ii]->Fill(M,scale);

							if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){

								histo_M_bkg_fail[kk][ii]->Fill(M,scale);

								if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){

									histo_M_WJets_fail[kk][ii]->Fill(M,scale);

								}else if (evt_id == 300){

									histo_M_TTJets_fail[kk][ii]->Fill(M,scale);


								}else if((evt_id == 702)||(evt_id == 703)||(evt_id == 704)||(evt_id == 705)||(evt_id == 701)){

									histo_M_DYJets_fail[kk][ii]->Fill(M,scale);
								}
							}
						}
					}
				}
			}
		}
	}

	//cout<<"debug5"<<endl;

	TCanvas** c1 = new TCanvas*[nrange];
	TCanvas** c2 = new TCanvas*[nrange];

	for(int i = 0; i < nrange; ++i){ 

		c1[i] = new TCanvas(Form("c1_%i",i),Form("c1_%i",i));
		c1[i]->Divide(1,2);
		c2[i] = new TCanvas(Form("c2_%i",i),Form("c2_%i",i));
		c2[i]->Divide(1,2);

		for(int j = 0; j < nparbins+1; ++j){ 

		//Plot the canvas
		double _par1;
		double _par2;
		if(i*Dpar+par_low < par_upp){_par1 = i*Dpar+par_low; _par2 = (i+1)*Dpar+par_low;}
		else if (i*Dpar+par_low >= par_upp){_par1 = i*Dpar+par_low; _par2 = 10000;}

		c1[i]->cd(1);

		TString _title; 
		_title = Form(", %0.f #leq P_{t} #leq %0.f",_par1,_par2);
		if(_par2==10000)_title = Form(", %0.f  #leq P_{t}",_par1);
		TString _stitle = (TString)"Invariant mass for "+pname+_title+", #||{#eta}<1.2, "+effcut+" pass";
		TString _stitlefail = (TString)"Invariant mass for "+pname+_title+", #||{#eta}<1.2, "+effcut+" fail";
		TString _stitle2 = (TString)"Invariant mass for "+pname+_title+", #||{#eta}>1.2, "+effcut+" pass";
		TString _stitle2fail = (TString)"Invariant mass for "+pname+_title+", #||{#eta}>1.2, "+effcut+" fail";

		histo_M_DYJets_bkg[i][j]->Draw();
		histo_M_DYJets_bkg[i][j]->SetTitle(_stitle);
		histo_M_DYJets_bkg[i][j]->GetXaxis()->SetTitle("m [GeV]");
		histo_M_DYJets_bkg[i][j]->SetLineWidth(2);
		histo_M_DYJets_bkg[i][j]->SetLineColor(4);
		histo_M_DYJets_bkg[i][j]->SetMarkerColor(4);
		histo_M_bkg[i][j]->Draw("same");
		histo_M_bkg[i][j]->SetLineWidth(2);
		histo_M_bkg[i][j]->SetMarkerColor(2);
		histo_M_bkg[i][j]->SetLineColor(2);
		histo_M_WJets[i][j]->Draw("same");
		histo_M_WJets[i][j]->SetLineWidth(3);
		histo_M_WJets[i][j]->SetLineColor(3);
		histo_M_WJets[i][j]->SetMarkerColor(3);
		TLegend* leg = new TLegend(0.6, 0.7,0.89,0.89);
		leg->AddEntry(histo_M_DYJets_bkg[i][j], "Z + bkg");
		leg->SetTextFont(43);
		leg->SetTextSize(25);
		leg->AddEntry(histo_M_bkg[i][j], "TTJets + WJets");
		leg->AddEntry(histo_M_WJets[i][j], "WJets");
		leg->SetBorderSize(0);
		leg->Draw();

		//Zbkg fail
		c1[i]->cd(2);
		histo_M_DYJets_bkg_fail[i][j]->Draw();
		histo_M_DYJets_bkg_fail[i][j]->SetTitle(_stitlefail);
		histo_M_DYJets_bkg_fail[i][j]->GetXaxis()->SetTitle("m [GeV]");
		histo_M_DYJets_bkg_fail[i][j]->SetLineWidth(2);
		histo_M_DYJets_bkg_fail[i][j]->SetLineColor(4);
		histo_M_DYJets_bkg_fail[i][j]->SetMarkerColor(4);
		histo_M_bkg_fail[i][j]->Draw("same");
		histo_M_bkg_fail[i][j]->SetLineWidth(2);
		histo_M_bkg_fail[i][j]->SetMarkerColor(2);
		histo_M_bkg_fail[i][j]->SetLineColor(2);
		histo_M_WJets_fail[i][j]->Draw("same");
		histo_M_WJets_fail[i][j]->SetLineWidth(3);
		histo_M_WJets_fail[i][j]->SetLineColor(3);
		histo_M_WJets_fail[i][j]->SetMarkerColor(3);


		//Z pass eta<1.2
		c2[i]->cd(1);
		histo_M_DYJets[i][j]->Draw();
		histo_M_DYJets[i][j]->SetTitle(_stitle);
		histo_M_DYJets[i][j]->GetXaxis()->SetTitle("m [GeV]");
		histo_M_DYJets[i][j]->SetLineWidth(2);
		histo_M_DYJets[i][j]->SetLineColor(4);
		histo_M_DYJets[i][j]->SetMarkerColor(4);
		histo_M_bkg[i][j]->Draw("same");
		histo_M_bkg[i][j]->SetLineWidth(2);
		histo_M_bkg[i][j]->SetMarkerColor(2);
		histo_M_bkg[i][j]->SetLineColor(2);
		TLegend* leg2 = new TLegend(0.6, 0.7,0.89,0.89);
		leg2->SetTextFont(43);
		leg2->SetTextSize(25);
		leg2->AddEntry(histo_M_DYJets[i][j], "Z");
		leg2->AddEntry(histo_M_bkg[i][j], "TTJets + WJets");
		leg2->SetBorderSize(0);
		leg2->Draw();

		//Z fail eta<1.2
		c2[i]->cd(2);
		histo_M_DYJets_fail[i][j]->Draw();
		histo_M_DYJets_fail[i][j]->SetTitle(_stitlefail);
		histo_M_DYJets_fail[i][j]->GetXaxis()->SetTitle("m [GeV]");
		histo_M_DYJets_fail[i][j]->SetLineWidth(2);
		histo_M_DYJets_fail[i][j]->SetLineColor(4);
		histo_M_DYJets_fail[i][j]->SetMarkerColor(4);
		histo_M_bkg_fail[i][j]->Draw("same");
		histo_M_bkg_fail[i][j]->SetLineWidth(2);
		histo_M_bkg_fail[i][j]->SetMarkerColor(2);
		histo_M_bkg_fail[i][j]->SetLineColor(2);

		TString _parxbin;
		TString _parybin;

		//Parameter string
		TString _par;
		if(par_x == "Pt"){_parxbin = Form("%0.f_Pt%0.f_",_par1,_par2);}
		else if(par_x == "eta"){_parxbin = Form("%0.f_eta%0.f_",_par1,_par2);}
		else if(par_x == "phi"){_parxbin = Form("%0.f_phi%0.f_",_par1,_par2);}
		if(par_y == "Pt"){_parybin = Form("%0.f_Pt%0.f_",_par1,_par2);}
		else if(par_y == "eta"){_parybin = Form("%0.f_eta%0.f_",_par1,_par2);}
		else if(par_y == "phi"){_parybin = Form("%0.f_phi%0.f_",_par1,_par2);}

		c1[i]->Write("Zbkg_"+_parxbin+"_"+_parybin);
		c2[i]->Write("Z_"+_parxbin+"_"+_parybin);

		mkdir(_path+_fname+"_PDF/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		TString _cname = _path+_fname+"_PDF/InvM4test_"+_parxbin;
		_cname += "_"+_effcut+"_"+pname;

		TString _c1name = _cname+ "_Zbkg_bkg_"+_parybin;
		TString _c2name = _cname+ "_Z_bkg_"+_parybin;

		c1[i]->SaveAs(_c1name);
		c2[i]->SaveAs(_c2name);

		histo_M_DYJets_bkg[i][j]->Write("histo_M_DYJets_bkg_Pt"+_parxbin+"_"+_parybin);
		histo_M_DYJets[i][j]->Write("histo_M_DYJets_Pt"+_parxbin+"_"+_parybin); 
		histo_M_bkg[i][j]->Write("histo_M_bkg_Pt"+_parxbin+"_"+_parybin); 
		histo_M_WJets[i][j]->Write("histo_M_WJets_Pt"+_parxbin+"_"+_parybin); 
		histo_M_TTJets[i][j]->Write("histo_M_TTJets_Pt"+_parxbin+"_"+_parybin); 
		histo_M_DYJets_bkg_fail[i][j]->Write("histo_M_DYJets_bkg_fail_Pt"+_parxbin+"_"+_parybin); 
		histo_M_DYJets_fail[i][j]->Write("histo_M_DYJets_fail_Pt"+_parxbin+"_"+_parybin); 
		histo_M_bkg_fail[i][j]->Write("histo_M_bkg_fail_Pt"+_parxbin+"_"+_parybin); 
		histo_M_WJets_fail[i][j]->Write("histo_M_WJets_fail_Pt"+_parxbin+"_"+_parybin); 
		histo_M_TTJets_fail[i][j]->Write("histo_M_TTJets_fail_Pt"+_parxbin+"_"+_parybin); 
                                     
		}                    

	}

	file_out->Close();

	return 0;

}
