/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name :DrawInvMassBkg_v2.C 

* Purpose : This file plots the histograms for Z+bkg. Reads the first file sample (the one storing electron and muon separatly.)

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

int DrawInvMassBkgMain(TTree* tree, int leptonId, double Pt_low = 10, double Pt_upp = 50,int nptbins = 10, TString effcut = "");

int DrawInvMassBkg2(TString _tree, int leptonId, double Pt_low = 10 , double Pt_upp = 50 ,int nptbins = 10, TString effcut = "", double cut){

	//Location of the .root file
	TString location = "/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/";

	//Reading the tree 
	//
	TChain* tree = new TChain("treeProducerSusySSDL");

	//DY events
	//Inclusive
	if(_tree.Contains("Inc")){tree->Add(location+"DYJetsToLLM50_PU_S14_POSTLS170.root");}
	//From 100 to Inf HT
	else if(_tree.Contains("NotInc")){tree->Add(location+"/DYJetsM50*.root");}
	else{cout<<"ERROR: Wrong tree name!"<<endl; return 1;}
	
	//WJet events
 	tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/WJets*.root");

	//TTJets events
	tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/TTJets_MS*.root");

	//Plot the result
	//
	DrawInvMassBkgMain(tree,leptonId ,Pt_low ,Pt_upp ,nptbins ,effcut);
	
	return 0;

}

int     DrawInvMassBkgMain(TTree* tree, int leptonId, double Pt_low , double Pt_upp ,int nptbins , TString effcut ){

	TString _path= "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/";

	setTDRStyle();

	TString pname;
	if(leptonId == 11){pname = "e";}
	if(leptonId == 13){pname = "mu";}

	//Some variables
	Long64_t n = tree->GetEntries();
	int nbins = 200;
	double Dpt = (Pt_upp-Pt_low)/nptbins;

	TString _fname = Form("TESTInvM_Pt_DYnWnonInc_Pt%0.f_Pt%0.f_",Pt_low,Pt_upp);
	_fname += effcut+"_"+pname; 

	//Name of the output file
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
	//Generated
	Int_t evt_id;
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
	//Reconstructed
	//Electrons
	Int_t nrecel;
	Int_t receltightid[200];
	Float_t recelPt[200];
	Float_t recelm[200];
	Float_t recel_eta[200];
	Float_t recel_phi[200];
	Int_t recelcharge[200];
	Float_t receliso03[200];
	Float_t receliso04[200];
	Float_t reccheliso03[200];
	Float_t reccheliso04[200];
	//Muons
	Int_t nrecmu;
	Int_t recmutightid[200];
	Float_t recmuPt[200];
	Float_t scale;
	Float_t recmum[200];
	Float_t recmu_eta[200];
	Float_t recmu_phi[200];
	Int_t recmucharge[200];
	Float_t recmuiso03[200];
	Float_t recmuiso04[200];
	Float_t recchmuiso03[200];
	Float_t recchmuiso04[200];

	//Assigne branches
	tree->SetBranchAddress("evt_scale1fb", &scale);
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
	//reconstructed
	//electron
	tree->SetBranchAddress("nel", &nrecel);
	tree->SetBranchAddress("el_tightId", &receltightid);
	tree->SetBranchAddress("el_pt", &recelPt);
	tree->SetBranchAddress("el_mass", &recelm);
	tree->SetBranchAddress("el_eta", &recel_eta); 
	tree->SetBranchAddress("el_phi", &recel_phi);
	tree->SetBranchAddress("el_charge", &recelcharge);
        tree->SetBranchAddress("el_relIso03", &receliso03);
        tree->SetBranchAddress("el_relIso04", &receliso04);
	tree->SetBranchAddress("el_chargedHadRelIso03", &reccheliso03);
	tree->SetBranchAddress("el_chargedHadRelIso04", &reccheliso04);

	//muons
	tree->SetBranchAddress("nmu", &nrecmu);
	tree->SetBranchAddress("mu_tightId", &recmutightid);
	tree->SetBranchAddress("mu_pt", &recmuPt);
	tree->SetBranchAddress("mu_mass", &recmum);
	tree->SetBranchAddress("mu_eta", &recmu_eta);
	tree->SetBranchAddress("mu_phi", &recmu_phi);
	tree->SetBranchAddress("mu_charge", &recmucharge);
	tree->SetBranchAddress("mu_relIso03", &recmuiso03);
        tree->SetBranchAddress("mu_relIso04", &recmuiso04);
	tree->SetBranchAddress("mu_chargedHadRelIso03", &recchmuiso03);
	tree->SetBranchAddress("mu_chargedHadRelIso04", &recchmuiso04);

	//Start loop over all events
	for (int k = 0; k < n; ++k) {

		int tag[2];
		int prob[2];
		tag[0] = 9999;
		tag[1] = 9999;
		prob[0] = 9999;
		prob[1] = 9999;

		tree->GetEntry(k);

				if(leptonId == 11){		

					for (int i = 0; i < nrecel; ++i) {

						//Prob selection cut
						//if(receltightid[i] == 1){ 

						//Prob1
						if(prob[0] == 9999){prob[0] = i;}
						//Prob2
						if((prob[0] != 9999)&&(prob[0] != i)&&(prob[1] == 9999)){prob[1] = i;}
						//Selection cut for Tag only
						//if(receliso03[i] < iso_cut ){
						if((effcut != "tightId")&&((effcut == "tightId")&&(receltightid[i] == 1))){ 

							if(prob[0] == i){tag[0] = i; }
							if(prob[1] == i){tag[1] = i; }

						}
						//}
					}

					//loop over all the rec particles to find the isolation
					//We requiere one tag at least 
					while((tag[0] != 9999)||(tag[1] != 9999)){

						int l1;
						int l2;

						if(tag[0] != 9999){l1 = prob[1]; l2 = tag[0]; tag[0] = 9999;}
						else if(tag[1] != 9999){l1 = prob[0]; l2 = tag[1]; tag[1] = 9999;}

						double M = InvMass(recelPt[l1],recel_eta[l1],recel_phi[l1],m[l1],recelPt[l2],recel_eta[l2],recel_phi[l2],m[l2]);

						if(l1 != 9999){
							for(int ii = 0; ii < nptbins+1; ++ii){
								//Add all the signal to the Z
								double Pt1 = 0;
								double Pt2 = 0;
								if(ii*Dpt+Pt_low < Pt_upp){Pt1 = ii*Dpt+Pt_low; Pt2 = (ii+1)*Dpt+Pt_low;}
								else if (ii*Dpt+Pt_low >= Pt_upp){Pt1 = ii*Dpt+Pt_low; Pt2 = 10000;}
								if((recelPt[l1] > Pt1)&&(recelPt[l1]< Pt2)){

									//Efficiency cut
									//if(receliso03[l1] < iso_cut){
									if((effcut == "tightId")&&(receltightid[l1] == 1)){ 
										if(abs(recel_eta[l1]) < 1.2){histo_M_DYJets_bkg_loweta[ii]->Fill(M,scale);}
										if(abs(recel_eta[l1]) >= 1.2){histo_M_DYJets_bkg_higheta[ii]->Fill(M,scale);}

										if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){
											if(recel_eta[l1] < 1.2){histo_M_bkg_loweta[ii]->Fill(M,scale);}
											if(recel_eta[l1] >= 1.2){histo_M_bkg_higheta[ii]->Fill(M,scale);}

											//Just the Wjets bkg
											if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){
												if(recel_eta[l1] < 1.2){histo_M_WJets_loweta[ii]->Fill(M,scale);}
												if(recel_eta[l1] >= 1.2){histo_M_WJets_higheta[ii]->Fill(M,scale);}
											}
										}else if((evt_id == 702)||(evt_id == 703)||(evt_id == 704)||(evt_id == 705)||(evt_id == 701)){

											if(recel_eta[l1] < 1.2){histo_M_DYJets_loweta[ii]->Fill(M,scale);}
											if(recel_eta[l1] >= 1.2){histo_M_DYJets_higheta[ii]->Fill(M,scale);}

										}
									}

									//if(receliso03[l1] >= iso_cut){
									if((effcut == "tightId")&&(receltightid[l1] != 1)){ 
										if(recel_eta[l1] < 1.2)histo_M_DYJets_bkg_fail_loweta[ii]->Fill(M,scale);
										if(recel_eta[l1] >= 1.2)histo_M_DYJets_bkg_fail_higheta[ii]->Fill(M,scale);

										if ((evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){
											if(recel_eta[l1] < 1.2)histo_M_bkg_fail_loweta[ii]->Fill(M,scale);
											if(recel_eta[l1] >= 1.2)histo_M_bkg_fail_higheta[ii]->Fill(M,scale);

											//Just the Wjets bkg
											if ((evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){
												if(recel_eta[l1] < 1.2)histo_M_WJets_fail_loweta[ii]->Fill(M,scale);
												if(recel_eta[l1] >= 1.2)histo_M_WJets_fail_higheta[ii]->Fill(M,scale);
											}
										}else if((evt_id == 702)||(evt_id == 703)||(evt_id == 704)||(evt_id == 705)||(evt_id == 701)){

											if(recel_eta[l1] < 1.2)histo_M_DYJets_fail_loweta[ii]->Fill(M,scale);
										if(recel_eta[l1] >= 1.2)histo_M_DYJets_fail_higheta[ii]->Fill(M,scale);

										}
									}

								}
								}	
								}
							}
						}					

				if(leptonId == 13){		

					for (int i = 0; i < nrecmu; ++i) {

						//Prob selection cut
						//if(recmutightid[i] == 1){ 

						//Prob1
						if(prob[0] == 9999){prob[0] = i;}
						//Prob2
						if((prob[0] != 9999)&&(prob[0] != i)&&(prob[1] == 9999)){prob[1] = i;}
						//Selection cut for Tag only
						//if(recmuiso03[i] < iso_cut ){
						if((effcut == "tightId")&&(recmutightid[i] == 1)){ 

							if(prob[0] == i){tag[0] = i; }
							if(prob[1] == i){tag[1] = i; }

						}
						//}
					}

					//loop over all the rec particles to find the isolation
					//We requiere one tag at least 
					while((tag[0] != 9999)||(tag[1] != 9999)){

						int l1;
						int l2;

						if(tag[0] != 9999){l1 = prob[1]; l2 = tag[0]; tag[0] = 9999;}
						else if(tag[1] != 9999){l1 = prob[0]; l2 = tag[1]; tag[1] = 9999;}

						double M = InvMass(recmuPt[l1],recmu_eta[l1],recmu_phi[l1],m[l1],recmuPt[l2],recmu_eta[l2],recmu_phi[l2],m[l2]);

						if(l1 != 9999){
							for(int ii = 0; ii < nptbins+1; ++ii){
								//Add all the signal to the Z
								double Pt1 = 0;
								double Pt2 = 0;
								if(ii*Dpt+Pt_low < Pt_upp){Pt1 = ii*Dpt+Pt_low; Pt2 = (ii+1)*Dpt+Pt_low;}
								else if (ii*Dpt+Pt_low >= Pt_upp){Pt1 = ii*Dpt+Pt_low; Pt2 = 10000;}
								if((recmuPt[l1] > Pt1)&&(recmuPt[l1]< Pt2)){

									//Efficiency cut
									//if(receliso03[l1] < iso_cut){
									if((effcut == "tightId")&&(recmutightid[l1] == 1)){ 
										if(abs(recmu_eta[l1]) < 1.2){histo_M_DYJets_bkg_loweta[ii]->Fill(M,scale);}
										if(abs(recmu_eta[l1]) >= 1.2){histo_M_DYJets_bkg_higheta[ii]->Fill(M,scale);}

										if ((evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){
											if(recmu_eta[l1] < 1.2){histo_M_bkg_loweta[ii]->Fill(M,scale);}
											if(recmu_eta[l1] >= 1.2){histo_M_bkg_higheta[ii]->Fill(M,scale);}

											//Just the Wjets bkg
											if ((evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){
												if(recmu_eta[l1] < 1.2){histo_M_WJets_loweta[ii]->Fill(M,scale);}
												if(recmu_eta[l1] >= 1.2){histo_M_WJets_higheta[ii]->Fill(M,scale);}
											}
										}else if((evt_id == 702)||(evt_id == 703)||(evt_id == 704)||(evt_id == 705)||(evt_id == 701)){

											if(recmu_eta[l1] < 1.2){histo_M_DYJets_loweta[ii]->Fill(M,scale);}
											if(recmu_eta[l1] >= 1.2){histo_M_DYJets_higheta[ii]->Fill(M,scale);}

										}
									}

									//if(receliso03[l1] >= iso_cut){
									if((effcut == "tightId")&&(recmutightid[l1] != 1)){ 
										if(recmu_eta[l1] < 1.2)histo_M_DYJets_bkg_fail_loweta[ii]->Fill(M,scale);
										if(recmu_eta[l1] >= 1.2)histo_M_DYJets_bkg_fail_higheta[ii]->Fill(M,scale);

										if ((evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){
											if(recmu_eta[l1] < 1.2)histo_M_bkg_fail_loweta[ii]->Fill(M,scale);
											if(recmu_eta[l1] >= 1.2)histo_M_bkg_fail_higheta[ii]->Fill(M,scale);

											//Just the Wjets bkg
											if ((evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){
												if(recmu_eta[l1] < 1.2)histo_M_WJets_fail_loweta[ii]->Fill(M,scale);
												if(recmu_eta[l1] >= 1.2)histo_M_WJets_fail_higheta[ii]->Fill(M,scale);
											}
										}else if((evt_id == 702)||(evt_id == 703)||(evt_id == 704)||(evt_id == 705)||(evt_id == 701)){

											if(recmu_eta[l1] < 1.2)histo_M_DYJets_fail_loweta[ii]->Fill(M,scale);
											if(recmu_eta[l1] >= 1.2)histo_M_DYJets_fail_higheta[ii]->Fill(M,scale);

										}
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
						c2->Write(Form("Zbkg_Pt%0.f_Pt%0.f_eta<1.2",Pt1,Pt2));
						c3->Write(Form("Z_Pt%0.f_Pt%0.f_eta<1.2",Pt1,Pt2));
						c4->Write(Form("Z_Pt%0.f_Pt%0.f_eta<1.2",Pt1,Pt2));

						
	 					mkdir(_path+_fname+"_PDF/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	                                        TString _cname = Form(_path+_fname+"_PDF/InvM_Pt%0.f_Pt%0.f",Pt1,Pt2);
						_cname += "_"+effcut+"_"+pname;
						TString _c1name = _cname+ "_Zbkg_bkg_eta<1.2.pdf";
						TString _c2name = _cname+ "_Zbkg_bkg_eta<1.2.pdf";
						TString _c3name = _cname+ "_Z_bkg_eta>1.2.pdf";
						TString _c4name = _cname+ "_Z_bkg_eta>1.2.pdf";

						c1->SaveAs(_c1name);
						c2->SaveAs(_c2name);
						c3->SaveAs(_c3name);
						c4->SaveAs(_c4name);

						histo_M_DYJets_bkg_loweta[i]->Write(Form("histo_M_DYJets_bkg_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2));
						histo_M_DYJets_loweta[i]->Write(Form("histo_M_DYJets_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
						histo_M_bkg_loweta[i]->Write(Form("histo_M_bkg_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
						histo_M_DYJets_bkg_fail_loweta[i]->Write(Form("histo_M_DYJets_bkg_fail_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
						histo_M_DYJets_fail_loweta[i]->Write(Form("histo_M_DYJets_fail_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
						histo_M_bkg_fail_loweta[i]->Write(Form("histo_M_bkg_fail_loweta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
						//Endcape
						histo_M_DYJets_bkg_higheta[i]->Write(Form("histo_M_DYJets_bkg_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2));
						histo_M_DYJets_higheta[i]->Write(Form("histo_M_DYJets_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
						histo_M_bkg_higheta[i]->Write(Form("histo_M_bkg_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
						histo_M_DYJets_bkg_fail_higheta[i]->Write(Form("histo_M_DYJets_bkg_fail_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
						histo_M_DYJets_fail_higheta[i]->Write(Form("histo_M_DYJets_fail_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 
						histo_M_bkg_fail_higheta[i]->Write(Form("histo_M_bkg_fail_higheta_Pt%0.f_Pt%0.f",Pt1,Pt2)); 



					}

					file_out->Close();

					return 0;

					}
