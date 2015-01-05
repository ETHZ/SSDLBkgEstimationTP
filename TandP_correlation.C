/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : TandP_correlation.C

* Purpose : This file study how the selection on the tag is correlated with the selection on the prob

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

int TandP_correlationMain(TTree* tree, int leptonId, double Pt_low = 10, double Pt_upp = 50,int nptbins = 10, TString select = "tight", TString effcut = "reliso3", double cut = 0.2, TString option ="");

int TandP_correlation(int leptonId, double Pt_low = 10 , double Pt_upp = 50 ,int nptbins = 10,TString select = "tight", TString effcut = "reliso3", double cut = 0.2, TString option = ""){

	//Location of the .root file
	TString location = "/Users/GLP/Desktop/CERN_data/2014-11-13_skim2ll-mva-softbtag/postprocessed/matched/";

	//Reading the tree 
	//
	TChain* tree = new TChain("treeProducerSusyMultilepton");

	//DY events
 	tree->Add(location+"DYJetsToLLM50_PU_S14_POSTLS170.root");
	

	//Plot the result
	//
	DrawInvMassBkgMain(tree,leptonId ,Pt_low ,Pt_upp ,nptbins,select,effcut, cut, option);
	
	return 0;

}

int     TandP_correlationMain(TTree* tree, int leptonId, double Pt_low , double Pt_upp ,int nptbins , TString select, TString effcut , double cut, TString option){

	TString _path= "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/";

	setTDRStyle();

	//Some variables
	Long64_t n = tree->GetEntries();

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

			//Store the values from both the loose and tight in the same variable

			//Prob selection cut
			if(abs(Gid[i]) == leptonId){
			if((select != "tight")||((select == "tight")&&(Gtight[i] == 1))){ 
			if((option.Contains("matching"))||((option == "matching")&&(matched[i] == 1))){ 

			//Prob1
			if(prob[0] == 9999){prob[0] = i;}
			//Prob2
			if((prob[0] != 9999)&&(prob[0] != i)&&(prob[1] == 9999)){prob[1] = i;}

			//Selection cut for Tag only
			if((effcut != "reliso3")||((effcut == "reliso3")&&(Giso3[i] < cut ))){
			if((effcut != "reliso4")||((effcut == "reliso4")&&(Giso4[i] < cut ))){
			if((effcut != "chiso3")||((effcut == "chiso3")&&(Gchiso3[i] < cut ))){
			if((effcut != "chiso3")||((effcut == "chiso3")&&(Gchiso4[i] < cut ))){
			if((effcut != "dxy")||((effcut == "dxy")&&(abs(Gdxy[i])< cut ))){
			if((effcut != "dz")||((effcut == "dz")&&(abs(Gdz[i])< cut ))){
			if((effcut != "tight")||((effcut == "tight")&&(Gtight[i] == 1))){

							if(prob[0] == i){tag[0] = i; }
							if(prob[1] == i){tag[1] = i; }

			}
			}
			}
			}
			}
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



					return 0;

					}
