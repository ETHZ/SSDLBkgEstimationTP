/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name :DrawInvMassBkg_v2.C 

* Purpose : This file take each TTree individually. Usefull if the TTree have different branches

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
#include "FitInvMass.C"
#include "setTDRStyle.C"
#include "InvMass.C"
#include "DeltaR.C"

vector< vector<TH1D*> > DrawInvMassBkgMain(int tv,TTree* tree, int leptonId, double Pt_low = 10, double Pt_upp = 50,int nptbins = 10, TString select = "tight", TString effcut = "reliso3", double cut = 0.2);

int DrawInvMassBkg3(int leptonId, double Pt_low = 10 , double Pt_upp = 50 ,int nptbins = 10,TString select = "tight", TString effcut = "reliso3", double cut = 0.2){

	double Dpt = (Pt_upp-Pt_low)/nptbins;
	
	/////////////////////////
	//Name of the output file
	/////////////////////////

	TString _path= "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/VERSION3";

	//particle string
	TString pname;
	if(leptonId == 11){pname = "e";}
	if(leptonId == 13){pname = "mu";}

	//selection string
	TString _sel;

	//cout<<"select is "<<select<<endl;
	if(select == "tight"){_sel = "tight";}
	else if(select == "loose"){_sel = "loose";}
	else{cout<<"ERROR: wrong selection !";return 1;}

	//cut string
	TString _effcut;

	if(effcut == "tight"){_effcut = "tight";}
	else if(effcut == "loose"){_effcut = "loose";}
	else if(effcut == "reliso3"){_effcut = Form("reliso3_%0.3lf ",cut);}
	else if(effcut == "reliso4"){_effcut = Form("reliso4_%0.3lf ",cut);}
	else if(effcut == "chiso3"){_effcut = Form("chiso3_%0.3lf ",cut);}
	else if(effcut == "chiso4"){_effcut = Form("chiso4_%0.3lf ",cut);}
	else if(effcut == "dxy"){_effcut = Form("dxy_%0.3lf ",cut);}
	else if(effcut == "dz"){_effcut = Form("dz_%0.3lf ",cut);}
	else{cout<<"ERROR: wrong numerator name !";return 1;}

	TString _fname = Form("NEWCODEInvM2_Pt%0.f_Pt%0.f_",Pt_low,Pt_upp);
	_fname += _effcut+"_for_"+_sel+"_"+pname; 

	TFile* file_out = new TFile(_path+_fname+".root","recreate");

	///////////////////////////
	//Build the array of canvas
	///////////////////////////
	
	//TCanvas**  c1 = new TCanvas*[nptbins+1];
	//TCanvas**  c2 = new TCanvas*[nptbins+1];

	//for(int i = 0; i < nptbins+1; ++i){
	//	c1[i] = new TCanvas("c1","c1");
	//	c2[i] = new TCanvas("c2","c2");

	//	c1[i]->Divide(1,2);
	//	c2[i]->Divide(1,2);
	//}

        //////////////
	//Histograms//
	//////////////
	
	//Barrel
	TH1D** hBDY = new TH1D*[nptbins+1];
	TH1D** hBTT = new TH1D*[nptbins+1];
	TH1D** hBWJ = new TH1D*[nptbins+1];
	TH1D** hBBkg = new TH1D*[nptbins+1];
	TH1D** hBfDY = new TH1D*[nptbins+1];
	TH1D** hBfTT = new TH1D*[nptbins+1];
	TH1D** hBfWJ = new TH1D*[nptbins+1];
	TH1D** hBfBkg = new TH1D*[nptbins+1];
	////Endcape
	TH1D** hEDY = new TH1D*[nptbins+1];
	TH1D** hETT = new TH1D*[nptbins+1];
	TH1D** hEWJ = new TH1D*[nptbins+1];
	TH1D** hEBkg = new TH1D*[nptbins+1];
	TH1D** hEfDY = new TH1D*[nptbins+1];
	TH1D** hEfTT = new TH1D*[nptbins+1];
	TH1D** hEfWJ = new TH1D*[nptbins+1];
	TH1D** hEfBkg = new TH1D*[nptbins+1];
	
	//////////
	//TTrees//
	//////////

	//Location of the .root file
	//First data file
	TString location1 = "/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/";
	//Second data file
	TString location2 = "/Users/GLP/Desktop/CERN_data/2014-11-13_skim2ll-mva-softbtag/postprocessed/";

	//Reading the tree 
	//
	//TChain* treeDY = new TChain("treeProducerSusySSDL");
        //treeDY->Add(location1+"DYJetsToLLM50_PU_S14_POSTLS170.root");

	TChain* treeBKG = new TChain("treeProducerSusyMultilepton");
        treeBKG->Add(location2+"WJetsToLNu_13TeV-madgraph-pythia8-tauola.root");
        treeBKG->Add(location2+"TTJets_MS*.root");

	//TChain* treeWJ = new TChain("treeProducerSusyMultilepton");
        //treeWJ->Add(location2+"WJetsToLNu_13TeV-madgraph-pythia8-tauola.root");
	//TChain* treeTT = new TChain("treeProducerSusyMultilepton");
        //treeTT->Add(location2+"TTJets_MS*.root");

	//////////////////
	//Fill the histo//
	//////////////////

	TCanvas* b1 = new TCanvas("b1","b1");

	//DY
	//vector< vector<TH1D*> > vecDY = DrawInvMassBkgMain(1,treeDY,leptonId ,Pt_low ,Pt_upp ,nptbins,select,effcut, cut);

	//For thr background
	vector< vector<TH1D*> > vecBKG = DrawInvMassBkgMain(2,treeBKG,leptonId ,Pt_low ,Pt_upp ,nptbins,select,effcut, cut);

	//Bck separated
	//vector< vector<TH1D*> > vecWJ = DrawInvMassBkgMain(2,treeWJ,leptonId ,Pt_low ,Pt_upp ,nptbins,select,effcut, cut);
	//vector< vector<TH1D*> > vecTT = DrawInvMassBkgMain(2,treeTT,leptonId ,Pt_low ,Pt_upp ,nptbins,select,effcut, cut);


	for(int i = 0; i < nptbins+1; ++i){
		//cout<<"int the i loop"<<endl;

	//DY
	//hBDY[i]  = vecDY[0][i];
	//hBfDY[i] = vecDY[1][i];
	//hEDY[i]  = vecDY[2][i];
	//hEfDY[i] = vecDY[3][i];
	//WJ
	//hBWJ[i]  = vecWJ[0][i];
	//hBfWJ[i] = vecWJ[1][i];
	//hEWJ[i]  = vecWJ[2][i];
	//hEfWJ[i] = vecWJ[3][i];
	////TT
	//hBTT[i]  = vecTT[0][i];
	//hBfTT[i] = vecTT[1][i];
	//hETT[i]  = vecTT[2][i];
	//hEfTT[i] = vecTT[3][i];

	hBBkg[i]  = vecBKG[0][i];
	hBfBkg[i] = vecBKG[1][i];
	hEBkg[i]  = vecBKG[2][i];
	hEfBkg[i] = vecBKG[3][i];

	}

	//////////////////
	//Save the histo//
	//////////////////

	for(int i = 0; i < nptbins+1; ++i){
		double Pt1;
		double Pt2;
		if(i*Dpt+Pt_low < Pt_upp){Pt1 = i*Dpt+Pt_low; Pt2 = (i+1)*Dpt+Pt_low;}
		else if (i*Dpt+Pt_low >= Pt_upp){Pt1 = i*Dpt+Pt_low; Pt2 = 10000;}

		//hBDY[i]->Write(Form("histo_M_DYJets_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
		//hBfDY[i]->Write(Form("histo_M_DYJets_fail_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
		hBBkg[i]->Write(Form("histo_M_bkg_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
		hBfBkg[i]->Write(Form("histo_M_bkg_fail_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
		//hEDY[i]->Write(Form("histo_M_DYJets_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
		//hEfDY[i]->Write(Form("histo_M_DYJets_fail_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
	        hEBkg[i]->Write(Form("histo_M_bkg_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
		hEfBkg[i]->Write(Form("histo_M_bkg_fail_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 

	}

        //file_out->Close();

	return 0;

}

vector< vector<TH1D*> > DrawInvMassBkgMain(int tv, TTree* tree, int leptonId, double Pt_low , double Pt_upp ,int nptbins , TString select, TString effcut , double cut){


	//Return vector
	
	vector< vector<TH1D*> > vec;

	vector<TH1D*> vecB;
	vector<TH1D*> vecBf;
	vector<TH1D*> vecE;
	vector<TH1D*> vecEf;


	//Some variables
	Long64_t n = tree->GetEntries();
	int nbins = 200;
	double Dpt = (Pt_upp-Pt_low)/nptbins;

	//cout<<"n is "<<n<<endl;


	//Histograms
	//Barrel
	TH1D* hB  = new TH1D[nptbins+1];
	TH1D* hBf = new TH1D[nptbins+1];
	//Endcape
	TH1D* hE  = new TH1D[nptbins+1];
	TH1D* hEf = new TH1D[nptbins+1];

	for(int _i = 0; _i < nptbins+1; ++_i){ 

		double Pt1;
		double Pt2;
		if(_i*Dpt+Pt_low < Pt_upp){Pt1 = _i*Dpt+Pt_low; Pt2 = (_i+1)*Dpt;}
		else if (_i*Dpt+Pt_low >= Pt_upp){Pt1 = _i*Dpt+Pt_low; Pt2 = 10000;}

		//Barrel
		hB[_i] = TH1D("nB","M",nbins,0,250);
		hBf[_i] = TH1D("hBf","M",nbins,0,250);
		//Endcape
		hE[_i] = TH1D("hE","M",nbins,0,250);
		hEf[_i] = TH1D("hEf","M",nbins,0,250);

	}

	//"Convert" effcut to int in order to do the switch
	int c;
	if(effcut == "reliso3"){c = 1;}
	if(effcut == "reliso4"){c = 2;}
	if(effcut == "chiso3"){c = 3;}
	if(effcut == "chiso4"){c = 4;}
	if(effcut == "dxy"){c = 5;}
	if(effcut == "dz"){c = 6;}
	if(effcut == "tight"){c = 6;}
	//cout<<"2"<<endl;

	//Switch to define the TanP according to the branches

//	switch(tv){

//		case 2:

		//////////////
		//Branch def//
		//////////////

		Int_t evt_id;
		Float_t scale;
		//tight
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
		tree->SetBranchAddress("LepOther_looseIdSusy",Oloose);//!!! same for the two sample
		tree->SetBranchAddress("LepOther_relIso03",&Oiso3);
		tree->SetBranchAddress("LepOther_relIso04",&Oiso4);
		tree->SetBranchAddress("LepOther_chargedHadRelIso03",&Ochiso3);
		tree->SetBranchAddress("LepOther_chargedHadRelIso04",&Ochiso4);
		tree->SetBranchAddress("LepOther_edxy",&Odxy);
		tree->SetBranchAddress("LepOther_edz",&Odz);
		//Loose
		tree->SetBranchAddress("nLepGood",&Gn);
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
		tree->SetBranchAddress("LepGood_edxy",&Gdxy);
		tree->SetBranchAddress("LepGood_edz",&Gdz);

		//cout<<"3"<<endl;

		//Start loop over all events
		for (int k = 0; k < n; ++k) {
			//cout<<"event number "<<k<<endl;

			//Definitions to use T&P
			int tag[2];
			int prob[2];
			tag[0] = 9999;
			tag[1] = 9999;
			prob[0] = 9999;
			prob[1] = 9999;

//	cout<<"4"<<endl;
			tree->GetEntry(k);
			//cout<<"nleptongood is"<<Gn<<endl;


			//cout<<"DEBUG 1"<<endl;
			for (int i = 0; i < Gn; ++i) {

				//Store the values from both the loose and tight in the same variable

				//Prob selection cut
				if(abs(Gid[i]) == leptonId){
					if((select != "tight")||((select = "tight")&&(Gtight[i] == 1))){ 

//	cout<<"5"<<endl;

						//Prob1
						if(prob[0] == 9999){prob[0] = i;}
						//Prob2
						if((prob[0] != 9999)&&(prob[0] != i)&&(prob[1] == 9999)){prob[1] = i;}
						//Selection cut for Tag only
						switch(c){

							case 1:
								if((prob[0] == i)&&(Giso3[i] < cut )){tag[0] = i;}
								if((prob[1] == i)&&(Giso3[i] < cut )){tag[1] = i;}
							case 2:
								if((prob[0] == i)&&(Giso4[i] < cut )){tag[0] = i;}
								if((prob[1] == i)&&(Giso4[i] < cut )){tag[1] = i;}
							case 3:
								if((prob[0] == i)&&(Gchiso3[i] < cut )){tag[0] = i;}
								if((prob[1] == i)&&(Gchiso3[i] < cut )){tag[1] = i;}
							case 4:
								if((prob[0] == i)&&(Gchiso4[i] < cut )){tag[0] = i;}
								if((prob[1] == i)&&(Gchiso3[i] < cut )){tag[1] = i;}
							case 5:
								if((prob[0] == i)&&(Gdxy[i] < cut )){tag[0] = i;}
								if((prob[1] == i)&&(Gdxy[i] < cut )){tag[1] = i;}
							case 6:
								if((prob[0] == i)&&(Gdz[i] < cut )){tag[0] = i;}
								if((prob[1] == i)&&(Gdz[i] < cut )){tag[1] = i;}
							case 7:
								if((prob[0] == i)&&(Gtight[i] < cut )){tag[0] = i;}
								if((prob[1] == i)&&(Gtight[i] < cut )){tag[1] = i;}


						}
					}
				}
			}

	//cout<<"6"<<endl;
			//loop over all the rec particles to find the isolation
			//We requiere one tag at least 
			while((tag[0] != 9999)||(tag[1] != 9999)){

				int l1;
				int l2;

				if(tag[0] != 9999){l1 = prob[1]; l2 = tag[0]; tag[0] = 9999;}
				else if(tag[1] != 9999){l1 = prob[0]; l2 = tag[1]; tag[1] = 9999;}
				//cout<<"l1 is "<<l1<<endl;
				//cout<<"l2 is "<<l2<<endl;

				double M = InvMass(Gpt[l1],Geta[l1],Gphi[l1],Gm[l1],Gpt[l2],Geta[l2],Gphi[l2],Gm[l2]);

				if(l1 != 9999){
					for(int ii = 0; ii < nptbins+1; ++ii){
						//Add all the signal to the Z
						double Pt1 = 0;
						double Pt2 = 0;
						if(ii*Dpt+Pt_low < Pt_upp){Pt1 = ii*Dpt+Pt_low; Pt2 = (ii+1)*Dpt+Pt_low;}
						else if (ii*Dpt+Pt_low >= Pt_upp){Pt1 = ii*Dpt+Pt_low; Pt2 = 10000;}
						if((Gpt[l1] > Pt1)&&(Gpt[l1]< Pt2)){

							switch(c){

								case 1:
									if((abs(Geta[l1]) < 1.2)&&(Giso3[l1] < cut)){hB[ii].Fill(M,scale);}
									if((abs(Geta[l1]) > 1.2)&&(Giso3[l1] < cut)){hE[ii].Fill(M,scale);}
									if((abs(Geta[l1]) < 1.2)&&(Giso3[l1] >= cut)){hBf[ii].Fill(M,scale);}
									if((abs(Geta[l1]) > 1.2)&&(Giso3[l1] >= cut)){hEf[ii].Fill(M,scale);}
								case 2:
									if((abs(Geta[l1]) < 1.2)&&(Giso4[l1] < cut)){hB[ii].Fill(M,scale);}
									if((abs(Geta[l1]) > 1.2)&&(Giso4[l1] < cut)){hE[ii].Fill(M,scale);}
									if((abs(Geta[l1]) < 1.2)&&(Giso4[l1] >= cut)){hBf[ii].Fill(M,scale);}
									if((abs(Geta[l1]) > 1.2)&&(Giso4[l1] >= cut)){hEf[ii].Fill(M,scale);}
								case 3: 
									if((abs(Geta[l1]) < 1.2)&&(Gchiso3[l1] < cut)){hB[ii].Fill(M,scale);}
									if((abs(Geta[l1]) > 1.2)&&(Gchiso3[l1] < cut)){hE[ii].Fill(M,scale);}
									if((abs(Geta[l1]) < 1.2)&&(Gchiso3[l1] >= cut)){hBf[ii].Fill(M,scale);}
									if((abs(Geta[l1]) > 1.2)&&(Gchiso3[l1] >= cut)){hEf[ii].Fill(M,scale);}
								case 4:
									if((abs(Geta[l1]) < 1.2)&&(Gchiso4[l1] < cut)){hB[ii].Fill(M,scale);}
									if((abs(Geta[l1]) > 1.2)&&(Gchiso4[l1] < cut)){hE[ii].Fill(M,scale);}
									if((abs(Geta[l1]) < 1.2)&&(Gchiso4[l1] >= cut)){hBf[ii].Fill(M,scale);}
									if((abs(Geta[l1]) > 1.2)&&(Gchiso4[l1] >= cut)){hEf[ii].Fill(M,scale);}
								case 5:
									if((abs(Geta[l1]) < 1.2)&&(Gdxy[l1] < cut)){hB[ii].Fill(M,scale);}
									if((abs(Geta[l1]) > 1.2)&&(Gdxy[l1] < cut)){hE[ii].Fill(M,scale);}
									if((abs(Geta[l1]) < 1.2)&&(Gdxy[l1] >= cut)){hBf[ii].Fill(M,scale);}
									if((abs(Geta[l1]) > 1.2)&&(Gdxy[l1] >= cut)){hEf[ii].Fill(M,scale);}
								case 6:
									if((abs(Geta[l1]) < 1.2)&&(Gdz[l1] < cut)){hB[ii].Fill(M,scale);}
									if((abs(Geta[l1]) > 1.2)&&(Gdz[l1] < cut)){hE[ii].Fill(M,scale);}
									if((abs(Geta[l1]) < 1.2)&&(Gdz[l1] >= cut)){hBf[ii].Fill(M,scale);}
									if((abs(Geta[l1]) > 1.2)&&(Gdz[l1] >= cut)){hEf[ii].Fill(M,scale);}
							}
						}

					}	
				}
			}
		}

		for(int ii = 0; ii < nptbins+1;++ii){
			//cout<<"7"<<endl;
			//Push in the vector
			vecB.push_back(&hB[ii]);
			vecBf.push_back(&hBf[ii]);
			vecE.push_back(&hE[ii]);
			vecEf.push_back(&hEf[ii]);
		}

//	}

	//cout<<"8"<<endl;
	vec.push_back(vecB);
	vec.push_back(vecBf);
	vec.push_back(vecE);
	vec.push_back(vecEf);
	//cout<<"9"<<endl;

	return vec;

}

