/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

 * File Name :DrawInvMassBkg3.C 

 * Purpose : This file plots the histograms for Z+bkg. THis used the files contained in 2014-11-13_skim2ll-mva-softbtag. For now, we only use the lepton passing the loose identification are used.

 * DataSet:  3

 * Creation Date : 20-12-2008

 * Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

 * Created By : Gaël L. Perrin

 _._._._._._._._._._._._._._._._._._._._._.*/

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
//#include "FitInvMass.C"
//#include "InvMass.C"
//#include "DeltaR.C"

int DrawInvMassBkgMain(TTree* tree, int leptonId, double Pt_low = 10, double Pt_upp = 50,int nptbins = 10, TString select = "tight", TString effcut = "reliso3", double cut = 0.2, TString option ="");

int DrawInvMassBkg(int leptonId, double Pt_low = 10 , double Pt_upp = 50 ,int nptbins = 10,TString select = "tight", TString effcut = "reliso3", double cut = 0.2, TString option = ""){

	//Location of the .root file
	TString location = "/Users/GLP/Desktop/CERN_data/2014-11-13_skim2ll-mva-softbtag/postprocessed/matched/";
	TString locationdy = "/Users/GLP/Desktop/CERN_data/dyjetsnew/postprocessed/";

	//Reading the tree 
	//
	TChain* tree = new TChain("treeProducerSusyMultilepton");

	//DY events
	//tree->Add(location+"DYJetsToLLM50_PU_S14_POSTLS170.root");
	tree->Add(locationdy+"dyjetsnew.root");

	//WJet events
	tree->Add(location+"WJetsToLNu_13TeV-madgraph-pythia8-tauola.root");

	//TTJets events
	tree->Add(location+"TTJets_MSDecaysCKM_central_PU_S14_POSTLS170.root");

	//Plot the result
	//
	DrawInvMassBkgMain(tree,leptonId ,Pt_low ,Pt_upp ,nptbins,select,effcut, cut, option);

	return 0;

}

int     DrawInvMassBkgMain(TTree* tree, int leptonId, double Pt_low , double Pt_upp ,int nptbins , TString select, TString effcut , double cut, TString option){

	TString _path= "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/";

	setTDRStyle();

	//Some variables
	Long64_t n = tree->GetEntries();
	int nbins = 200;
	double Dpt = (Pt_upp-Pt_low)/nptbins;

	/////////////////////////
	//Name of the output file
	/////////////////////////

	//particle string
	TString pname;
	if(abs(leptonId) == 11){pname = "e";}
	if(abs(leptonId) == 13){pname = "mu";}

	//selection string
	TString _sel;

	if(select == "tight"){_sel = "tight";}
	else if(select == "loose"){_sel = "loose";}
	else if(select == ""){_sel = "";}
	else{cout<<"ERROR: wrong selection !";return 1;}

	//cut string
	TString _effcut;
	if(effcut == "tight"){_effcut = "tight";}
	else if(effcut == ""){_effcut = "";}
	else if(effcut == "loose"){_effcut = "loose";}
	else if(effcut == "reliso3"){_effcut = Form("reliso3_%0.3lf",cut);}
	else if(effcut == "reliso4"){_effcut = Form("reliso4_%0.3lf",cut);}
	else if(effcut == "chiso3"){_effcut = Form("chiso3_%0.3lf",cut);}
	else if(effcut == "chiso4"){_effcut = Form("chiso4_%0.3lf",cut);}
	else if(effcut == "dxy"){_effcut = Form("dxy_%0.3lf",cut);}
	else if(effcut == "dz"){_effcut = Form("dz_%0.3lf",cut);}
	else{cout<<"ERROR: wrong numerator name !";return 1;}

	TString _fname = "InvM2";
	if(option.Contains("matching")){_fname += "_Matched";}

	TString _ptrange;
	_ptrange = Form("Pt%0.f_Pt%0.f",Pt_low,Pt_upp);
	_fname += "_"+_ptrange;
	_fname += "_"+_effcut+"_for_"+_sel+"_"+pname; 

	TFile* file_out = new TFile(_path+_fname+".root","recreate");

	//Histograms
	//Barrel
	TH1D** histo_M_DYJets_bkg_loweta = new TH1D*[nptbins+1];
	TH1D** histo_M_DYJets_loweta = new TH1D*[nptbins+1];
	TH1D** histo_M_bkg_loweta = new TH1D*[nptbins+1];
	TH1D** histo_M_WJets_loweta = new TH1D*[nptbins+1];
	TH1D** histo_M_DYJets_bkg_fail_loweta = new TH1D*[nptbins+1];
	TH1D** histo_M_DYJets_fail_loweta = new TH1D*[nptbins+1];
	TH1D** histo_M_bkg_fail_loweta = new TH1D*[nptbins+1];
	TH1D** histo_M_WJets_fail_loweta = new TH1D*[nptbins+1];
	//Endcape
	TH1D** histo_M_DYJets_bkg_higheta = new TH1D*[nptbins+1];
	TH1D** histo_M_DYJets_higheta = new TH1D*[nptbins+1];
	TH1D** histo_M_bkg_higheta = new TH1D*[nptbins+1];
	TH1D** histo_M_WJets_higheta = new TH1D*[nptbins+1];
	TH1D** histo_M_DYJets_bkg_fail_higheta = new TH1D*[nptbins+1];
	TH1D** histo_M_DYJets_fail_higheta = new TH1D*[nptbins+1];
	TH1D** histo_M_bkg_fail_higheta = new TH1D*[nptbins+1];
	TH1D** histo_M_WJets_fail_higheta = new TH1D*[nptbins+1];

	for(int _i = 0; _i < nptbins+1; ++_i){ 

		double Pt1;
		double Pt2;
		if(_i*Dpt+Pt_low < Pt_upp){Pt1 = _i*Dpt+Pt_low; Pt2 = (_i+1)*Dpt;}
		else if (_i*Dpt+Pt_low >= Pt_upp){Pt1 = _i*Dpt+Pt_low; Pt2 = 10000;}

		//Barrel
		histo_M_DYJets_bkg_loweta[_i] = new TH1D("histo_M_DYJets_bkg_loweta","M",nbins,0,250);
		histo_M_DYJets_loweta[_i] = new TH1D("histo_M_DYJets_loweta","M",nbins,0,250);
		histo_M_bkg_loweta[_i] = new TH1D("histo_M_bkg_loweta","M",nbins,0,250);
		histo_M_WJets_loweta[_i] = new TH1D("histo_M_WYJets_loweta","M",nbins,0,250);
		histo_M_DYJets_bkg_fail_loweta[_i] = new TH1D("histo_M_DYJets_bkg_fail_loweta","M",nbins,0,250);
		histo_M_DYJets_fail_loweta[_i] = new TH1D("histo_M_DYJets_fail_loweta","M",nbins,0,250);
		histo_M_bkg_fail_loweta[_i] = new TH1D("histo_M_bkg_fail_loweta","M",nbins,0,250);
		histo_M_WJets_fail_loweta[_i] = new TH1D("histo_M_WJets_fail_loweta","M",nbins,0,250);
		//Endcape
		histo_M_DYJets_bkg_higheta[_i] = new TH1D("histo_M_DYJets_bkg_higheta","M",nbins,0,250);
		histo_M_DYJets_higheta[_i] = new TH1D("histo_M_DYJets_higheta","M",nbins,0,250);
		histo_M_bkg_higheta[_i] = new TH1D("histo_M_bkg_higheta","M",nbins,0,250);
		histo_M_WJets_higheta[_i] = new TH1D("histo_M_WYJets_higheta","M",nbins,0,250);
		histo_M_DYJets_bkg_fail_higheta[_i] = new TH1D("histo_M_DYJets_bkg_fail_higheta","M",nbins,0,250);
		histo_M_DYJets_fail_higheta[_i] = new TH1D("histo_M_DYJets_fail_higheta","M",nbins,0,250);
		histo_M_bkg_fail_higheta[_i] = new TH1D("histo_M_bkg_fail_higheta","M",nbins,0,250);
		histo_M_WJets_fail_higheta[_i] = new TH1D("histo_M_WJets_fail_higheta","M",nbins,0,250);

	}

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
	tree->SetBranchAddress("LepOther_looseIdSusy",Oloose);
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
	tree->SetBranchAddress("LepGood_looseIdSusy",&Gloose);
	tree->SetBranchAddress("LepGood_relIso03",&Giso3);
	tree->SetBranchAddress("LepGood_relIso04",&Giso4);
	tree->SetBranchAddress("LepGood_chargedHadRelIso03",&Gchiso3);
	tree->SetBranchAddress("LepGood_chargedHadRelIso04",&Gchiso4);
	tree->SetBranchAddress("LepGood_dxy",&Gdxy);
	tree->SetBranchAddress("LepGood_dz",&Gdz);

	//Start loop over all events
	for (int k = 0; k < n; ++k) {

		//Definitions to use T&P
		int tag[2];
		int prob[2];
		tag[0] = 9999;
		tag[1] = 9999;
		prob[0] = 9999;
		prob[1] = 9999;

		tree->GetEntry(k);

		for (int i = 0; i < Gn; ++i) {

			//define selections using bools

			bool reliso3((effcut != "reliso3")||((effcut == "reliso3")&&(Giso3[i] < cut )));
			bool reliso4((effcut != "reliso4")||((effcut == "reliso4")&&(Giso4[i] < cut )));
			bool chiso3((effcut != "chiso3")||((effcut == "chiso3")&&(Gchiso3[i] < cut )));
			bool chiso4((effcut != "chiso4")||((effcut == "chiso4")&&(Gchiso4[i] < cut )));
			bool dxy((effcut != "dxy")||((effcut == "dxy")&&(abs(Gdxy[i])< cut )));
			bool dz((effcut != "dz")||((effcut == "dz")&&(abs(Gdz[i])< cut )));
			bool tight((effcut != "tight")||((effcut == "tight")&&(Gtight[i] == 1)));


			//Store the values from both the loose and tight in the same variable

			//Prob selection cut
			if(abs(Gid[i]) == leptonId){
				if((select != "tight")||((select == "tight")&&(Gtight[i] == 1))){ 
					if((!option.Contains("matching"))||((option.Contains("matching"))&&(matched[i] == 1))){ 

						//Prob1
						if(prob[0] == 9999){prob[0] = i;}
						//Prob2
						if((prob[0] != 9999)&&(prob[0] != i)&&(prob[1] == 9999)){prob[1] = i;}

						//Selection cut for Tag only
						if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight){

							if(prob[0] == i){tag[0] = i; }
							if(prob[1] == i){tag[1] = i; }

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

				for(int ii = 0; ii < nptbins+1; ++ii){
					//Add all the signal to the Z
					double Pt1 = 0;
					double Pt2 = 0;
					if(ii*Dpt+Pt_low < Pt_upp){Pt1 = ii*Dpt+Pt_low; Pt2 = (ii+1)*Dpt+Pt_low;}
					else if (ii*Dpt+Pt_low >= Pt_upp){Pt1 = ii*Dpt+Pt_low; Pt2 = 10000;}
					if((Gpt[l1] > Pt1)&&(Gpt[l1]< Pt2)){

						bool reliso3((effcut != "reliso3")||((effcut == "reliso3")&&(Giso3[l1] < cut )));
						bool reliso4((effcut != "reliso4")||((effcut == "reliso4")&&(Giso4[l1] < cut )));
						bool chiso3((effcut != "chiso3")||((effcut == "chiso3")&&(Gchiso3[l1] < cut )));
						bool chiso4((effcut != "chiso4")||((effcut == "chiso4")&&(Gchiso4[l1] < cut )));
						bool dxy((effcut != "dxy")||((effcut == "dxy")&&(abs(Gdxy[l1])< cut )));
						bool dz((effcut != "dz")||((effcut == "dz")&&(abs(Gdz[l1])< cut )));
						bool tight((effcut != "tight")||((effcut == "tight")&&(Gtight[l1] == 1)));

						//Efficiency cut
						if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight){

							//if((effcut != "reliso3")||((effcut == "reliso3")&&(Giso3[l1] < cut ))){
							//if((effcut != "reliso4")||((effcut == "reliso4")&&(Giso4[l1] < cut ))){
							//if((effcut != "chiso3")||((effcut == "chiso3")&&(Gchiso3[l1] < cut ))){
							//if((effcut != "chiso3")||((effcut == "chiso3")&&(Gchiso3[l1] < cut ))){
							//if((effcut != "dxy")||((effcut == "dxy")&&(abs(Gdxy[l1])< cut ))){
							//if((effcut != "dz")||((effcut == "dz")&&(abs(Gdz[l1])< cut ))){
							//if((effcut != "tight")||((effcut == "tight")&&(Gtight[l1] == 1))){

							if(abs(Geta[l1]) < 1.2){histo_M_DYJets_bkg_loweta[ii]->Fill(M,scale);}
							if(abs(Geta[l1]) >= 1.2){histo_M_DYJets_bkg_higheta[ii]->Fill(M,scale);}

							if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){
								if(abs(Geta[l1]) < 1.2){histo_M_bkg_loweta[ii]->Fill(M,scale);}
								if(abs(Geta[l1]) >= 1.2){histo_M_bkg_higheta[ii]->Fill(M,scale);}

								//Just the Wjets bkg
								if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){
									if(abs(Geta[l1]) < 1.2){histo_M_WJets_loweta[ii]->Fill(M,scale);}
									if(abs(Geta[l1]) >= 1.2){histo_M_WJets_higheta[ii]->Fill(M,scale);}
								}
							}else if((evt_id == 702)||(evt_id == 703)||(evt_id == 704)||(evt_id == 705)||(evt_id == 701)){

								if(abs(Geta[l1]) < 1.2){histo_M_DYJets_loweta[ii]->Fill(M,scale);}
								if(abs(Geta[l1]) >= 1.2){histo_M_DYJets_higheta[ii]->Fill(M,scale);}

							}
						}

						reliso3 = ((effcut != "reliso3")||((effcut == "reliso3")&&(Giso3[l1] >= cut )));
						reliso4 = ((effcut != "reliso4")||((effcut == "reliso4")&&(Giso4[l1] >= cut )));
						chiso3 = ((effcut != "chiso3")||((effcut == "chiso3")&&(Gchiso3[l1] >= cut )));
						chiso4 = ((effcut != "chiso4")||((effcut == "chiso4")&&(Gchiso4[l1] >= cut )));
						dxy = ((effcut != "dxy")||((effcut == "dxy")&&(abs(Gdxy[l1]) >= cut )));
						dz = ((effcut != "dz")||((effcut == "dz")&&(abs(Gdz[l1]) >= cut )));
						tight = ((effcut != "tight")||((effcut == "tight")&&(Gtight[l1] != 1)));

						//if((effcut != "reliso3")||((effcut == "reliso3")&&(Giso3[l1] >= cut ))){
						//if((effcut != "reliso4")||((effcut == "reliso4")&&(Giso4[l1] >= cut ))){
						//if((effcut != "chiso3")||((effcut == "chiso3")&&(Gchiso3[l1] >= cut ))){
						//if((effcut != "chiso3")||((effcut == "chiso3")&&(Gchiso3[l1] >= cut ))){
						//if((effcut != "dxy")||((effcut == "dxy")&&(abs(Gdxy[l1]) >= cut ))){
						//if((effcut != "dz")||((effcut == "dz")&&(abs(Gdz[l1]) >= cut ))){
						//if((effcut != "tight")||((effcut == "tight")&&(Gtight[l1] != 1))){

						if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight){

							if(abs(Geta[l1]) < 1.2)histo_M_DYJets_bkg_fail_loweta[ii]->Fill(M,scale);
							if(abs(Geta[l1]) >= 1.2)histo_M_DYJets_bkg_fail_higheta[ii]->Fill(M,scale);

							if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){
								if(abs(Geta[l1]) < 1.2)histo_M_bkg_fail_loweta[ii]->Fill(M,scale);
								if(abs(Geta[l1]) >= 1.2)histo_M_bkg_fail_higheta[ii]->Fill(M,scale);

								if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){
									if(abs(Geta[l1]) < 1.2)histo_M_WJets_fail_loweta[ii]->Fill(M,scale);
									if(abs(Geta[l1]) >= 1.2)histo_M_WJets_fail_higheta[ii]->Fill(M,scale);
								}

							}else if((evt_id == 702)||(evt_id == 703)||(evt_id == 704)||(evt_id == 705)||(evt_id == 701)){

								if(abs(Geta[l1]) < 1.2)histo_M_DYJets_fail_loweta[ii]->Fill(M,scale);
								if(abs(Geta[l1]) >= 1.2)histo_M_DYJets_fail_higheta[ii]->Fill(M,scale);

							}

						}

						}
						}
						}
						}
						}

						for(int i = 0; i < nptbins+1; ++i){

							//Plot the canvas
							double Pt1;
							double Pt2;
							if(i*Dpt+Pt_low < Pt_upp){Pt1 = i*Dpt+Pt_low; Pt2 = (i+1)*Dpt+Pt_low;}
							else if (i*Dpt+Pt_low >= Pt_upp){Pt1 = i*Dpt+Pt_low; Pt2 = 10000;}

							TCanvas* c1 = new TCanvas("c1","c1");
							c1->Divide(1,2);
							TCanvas* c2 = new TCanvas("c2","c2");
							c2->Divide(1,2);
							TCanvas* c3 = new TCanvas("c3","c3");
							c3->Divide(1,2);
							TCanvas* c4 = new TCanvas("c4","c4");
							c4->Divide(1,2);

							//Zbkg pass eta<1.2
							c1->cd(1);
							histo_M_DYJets_bkg_loweta[i]->Draw();

							TString _title; 
							_title = Form(", %0.f #leq P_{t} #leq %0.f",Pt1,Pt2);
							if(Pt2==10000)_title = Form(", %0.f  #leq P_{t}",Pt1);
							TString _stitle = (TString)"Invariant mass for "+pname+_title+", #||{#eta}<1.2, "+effcut+" pass";
							TString _stitlefail = (TString)"Invariant mass for "+pname+_title+", #||{#eta}<1.2, "+effcut+" fail";
							TString _stitle2 = (TString)"Invariant mass for "+pname+_title+", #||{#eta}>1.2, "+effcut+" pass";
							TString _stitle2fail = (TString)"Invariant mass for "+pname+_title+", #||{#eta}>1.2, "+effcut+" fail";

							histo_M_DYJets_bkg_loweta[i]->SetTitle(_stitle);
							histo_M_DYJets_bkg_loweta[i]->GetXaxis()->SetTitle("m [GeV]");
							histo_M_DYJets_bkg_loweta[i]->SetLineWidth(2);
							histo_M_DYJets_bkg_loweta[i]->SetLineColor(4);
							histo_M_DYJets_bkg_loweta[i]->SetMarkerColor(4);
							histo_M_bkg_loweta[i]->Draw("same");
							histo_M_bkg_loweta[i]->SetLineWidth(2);
							histo_M_bkg_loweta[i]->SetMarkerColor(2);
							histo_M_bkg_loweta[i]->SetLineColor(2);
							histo_M_WJets_loweta[i]->Draw("same");
							histo_M_WJets_loweta[i]->SetLineWidth(3);
							histo_M_WJets_loweta[i]->SetLineColor(3);
							histo_M_WJets_loweta[i]->SetMarkerColor(3);
							TLegend* leg = new TLegend(0.6, 0.7,0.89,0.89);
							leg->AddEntry(histo_M_DYJets_bkg_loweta[i], "Z + bkg");
							leg->SetTextFont(43);
							leg->SetTextSize(25);
							leg->AddEntry(histo_M_bkg_loweta[i], "TTJets + WJets");
							leg->AddEntry(histo_M_WJets_loweta[i], "WJets");
							leg->SetBorderSize(0);
							leg->Draw();

							//Zbkg fail eta<1.2
							c1->cd(2);
							histo_M_DYJets_bkg_fail_loweta[i]->Draw();
							histo_M_DYJets_bkg_fail_loweta[i]->SetTitle(_stitlefail);
							histo_M_DYJets_bkg_fail_loweta[i]->GetXaxis()->SetTitle("m [GeV]");
							histo_M_DYJets_bkg_fail_loweta[i]->SetLineWidth(2);
							histo_M_DYJets_bkg_fail_loweta[i]->SetLineColor(4);
							histo_M_DYJets_bkg_fail_loweta[i]->SetMarkerColor(4);
							histo_M_bkg_fail_loweta[i]->Draw("same");
							histo_M_bkg_fail_loweta[i]->SetLineWidth(2);
							histo_M_bkg_fail_loweta[i]->SetMarkerColor(2);
							histo_M_bkg_fail_loweta[i]->SetLineColor(2);
							histo_M_WJets_fail_loweta[i]->Draw("same");
							histo_M_WJets_fail_loweta[i]->SetLineWidth(3);
							histo_M_WJets_fail_loweta[i]->SetLineColor(3);
							histo_M_WJets_fail_loweta[i]->SetMarkerColor(3);

							//Z pass eta<1.2
							c2->cd(1);
							histo_M_DYJets_loweta[i]->Draw();
							histo_M_DYJets_loweta[i]->SetTitle(_stitle);
							histo_M_DYJets_loweta[i]->GetXaxis()->SetTitle("m [GeV]");
							histo_M_DYJets_loweta[i]->SetLineWidth(2);
							histo_M_DYJets_loweta[i]->SetLineColor(4);
							histo_M_DYJets_loweta[i]->SetMarkerColor(4);
							histo_M_bkg_loweta[i]->Draw("same");
							histo_M_bkg_loweta[i]->SetLineWidth(2);
							histo_M_bkg_loweta[i]->SetMarkerColor(2);
							histo_M_bkg_loweta[i]->SetLineColor(2);
							TLegend* leg2 = new TLegend(0.6, 0.7,0.89,0.89);
							leg2->SetTextFont(43);
							leg2->SetTextSize(25);
							leg2->AddEntry(histo_M_DYJets_loweta[i], "Z");
							leg2->AddEntry(histo_M_bkg_loweta[i], "TTJets + WJets");
							leg2->SetBorderSize(0);
							leg2->Draw();

							//Z fail eta<1.2
							c2->cd(2);
							histo_M_DYJets_fail_loweta[i]->Draw();
							histo_M_DYJets_fail_loweta[i]->SetTitle(_stitlefail);
							histo_M_DYJets_fail_loweta[i]->GetXaxis()->SetTitle("m [GeV]");
							histo_M_DYJets_fail_loweta[i]->SetLineWidth(2);
							histo_M_DYJets_fail_loweta[i]->SetLineColor(4);
							histo_M_DYJets_fail_loweta[i]->SetMarkerColor(4);
							histo_M_bkg_fail_loweta[i]->Draw("same");
							histo_M_bkg_fail_loweta[i]->SetLineWidth(2);
							histo_M_bkg_fail_loweta[i]->SetMarkerColor(2);
							histo_M_bkg_fail_loweta[i]->SetLineColor(2);

							//Zbkg pass eta>1.2
							c3->cd(1);
							histo_M_DYJets_bkg_higheta[i]->Draw();
							histo_M_DYJets_bkg_higheta[i]->SetTitle(_stitle2);
							histo_M_DYJets_bkg_higheta[i]->GetXaxis()->SetTitle("m [GeV]");
							histo_M_DYJets_bkg_higheta[i]->SetLineWidth(2);
							histo_M_DYJets_bkg_higheta[i]->SetLineColor(4);
							histo_M_DYJets_bkg_higheta[i]->SetMarkerColor(4);
							histo_M_bkg_higheta[i]->Draw("same");
							histo_M_bkg_higheta[i]->SetLineWidth(2);
							histo_M_bkg_higheta[i]->SetMarkerColor(2);
							histo_M_bkg_higheta[i]->SetLineColor(2);
							histo_M_WJets_higheta[i]->Draw("same");
							histo_M_WJets_higheta[i]->SetLineWidth(3);
							histo_M_WJets_higheta[i]->SetLineColor(3);
							histo_M_WJets_higheta[i]->SetMarkerColor(3);
							leg->Draw();

							//Zbkg fail eta>1.2
							c3->cd(2);
							histo_M_DYJets_bkg_fail_higheta[i]->Draw();
							histo_M_DYJets_bkg_fail_higheta[i]->SetTitle(_stitle2fail);
							histo_M_DYJets_bkg_fail_higheta[i]->SetLineWidth(2);
							histo_M_DYJets_bkg_fail_higheta[i]->SetLineColor(4);
							histo_M_DYJets_bkg_fail_higheta[i]->SetMarkerColor(4);
							histo_M_bkg_fail_higheta[i]->Draw("same");
							histo_M_bkg_fail_higheta[i]->SetLineWidth(2);
							histo_M_bkg_fail_higheta[i]->SetMarkerColor(2);
							histo_M_bkg_fail_higheta[i]->SetLineColor(2);
							histo_M_WJets_fail_higheta[i]->Draw("same");
							histo_M_WJets_fail_higheta[i]->SetLineWidth(3);
							histo_M_WJets_fail_higheta[i]->SetLineColor(3);
							histo_M_WJets_fail_higheta[i]->SetMarkerColor(3);

							//Z pass eta >1.2
							TCanvas* c7 = new TCanvas("c7","c7");
							c4->cd(1);
							histo_M_DYJets_higheta[i]->Draw();
							histo_M_DYJets_higheta[i]->SetTitle(_stitle2);
							histo_M_DYJets_higheta[i]->SetLineWidth(2);
							histo_M_DYJets_higheta[i]->SetLineColor(4);
							histo_M_DYJets_higheta[i]->SetMarkerColor(4);
							histo_M_bkg_higheta[i]->Draw("same");
							histo_M_bkg_higheta[i]->SetLineWidth(2);
							histo_M_bkg_higheta[i]->SetMarkerColor(2);
							histo_M_bkg_higheta[i]->SetLineColor(2);
							leg2->Draw();

							//Z fail eta >1.2
							c4->cd(2);
							histo_M_DYJets_fail_higheta[i]->Draw();
							histo_M_DYJets_fail_higheta[i]->SetTitle(_stitle2fail);
							histo_M_DYJets_fail_higheta[i]->SetLineWidth(2);
							histo_M_DYJets_fail_higheta[i]->SetLineColor(4);
							histo_M_DYJets_fail_higheta[i]->SetMarkerColor(4);
							histo_M_bkg_fail_higheta[i]->Draw("same");
							histo_M_bkg_fail_higheta[i]->SetLineWidth(2);
							histo_M_bkg_fail_higheta[i]->SetMarkerColor(2);
							histo_M_bkg_fail_higheta[i]->SetLineColor(2);

							c1->Write(Form("Zbkg_Pt%0.f_Pt%0.f_eta<1.2",Pt1,Pt2));
							c2->Write(Form("Z_Pt%0.f_Pt%0.f_eta<1.2",Pt1,Pt2));
							c3->Write(Form("Zbkg_Pt%0.f_Pt%0.f_eta>1.2",Pt1,Pt2));
							c4->Write(Form("Z_Pt%0.f_Pt%0.f_eta>1.2",Pt1,Pt2));


							mkdir(_path+_fname+"_PDF/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
							TString _cname = Form(_path+_fname+"_PDF/InvM_Pt%0.f_Pt%0.f",Pt1,Pt2);
							_cname += "_"+_effcut+"_"+pname;
							TString _c1name = _cname+ "_Zbkg_bkg_eta<1.2.pdf";
							TString _c2name = _cname+ "_Z_bkg_eta<1.2.pdf";
							TString _c3name = _cname+ "_Zbkg_bkg_eta>1.2.pdf";
							TString _c4name = _cname+ "_Z_bkg_eta>1.2.pdf";

							c1->SaveAs(_c1name);
							c2->SaveAs(_c2name);
							c3->SaveAs(_c3name);
							c4->SaveAs(_c4name);

							histo_M_DYJets_bkg_loweta[i]->Write(Form("histo_M_DYJets_bkg_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2));
							histo_M_DYJets_loweta[i]->Write(Form("histo_M_DYJets_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
							histo_M_bkg_loweta[i]->Write(Form("histo_M_bkg_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
							histo_M_WJets_loweta[i]->Write(Form("histo_M_WJets_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
							histo_M_DYJets_bkg_fail_loweta[i]->Write(Form("histo_M_DYJets_bkg_fail_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
							histo_M_DYJets_fail_loweta[i]->Write(Form("histo_M_DYJets_fail_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
							histo_M_bkg_fail_loweta[i]->Write(Form("histo_M_bkg_fail_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
							histo_M_WJets_fail_loweta[i]->Write(Form("histo_M_WJets_fail_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 

							//Endcape
							histo_M_DYJets_bkg_higheta[i]->Write(Form("histo_M_DYJets_bkg_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2));
							histo_M_DYJets_higheta[i]->Write(Form("histo_M_DYJets_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
							histo_M_bkg_higheta[i]->Write(Form("histo_M_bkg_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
							histo_M_WJets_higheta[i]->Write(Form("histo_M_WJets_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2));
							histo_M_DYJets_bkg_fail_higheta[i]->Write(Form("histo_M_DYJets_bkg_fail_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
							histo_M_DYJets_fail_higheta[i]->Write(Form("histo_M_DYJets_fail_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
							histo_M_bkg_fail_higheta[i]->Write(Form("histo_M_bkg_fail_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
							histo_M_WJets_fail_higheta[i]->Write(Form("histo_M_WJets_fail_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2));



						}

						file_out->Close();

						return 0;

						}
