/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : Efficiency_v3.C 

* Purpose : Plot the efficiency for a given cut that is applied to reconstructed data. A matching between the gen. and rec. data is done.
    Endcape and Barrel are separated here.
 
 
* DataSet:  2

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By : Gaël L. Perrin

_._._._._._._._._._._._._._._._._._._._._.*/
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
#include "FitInvMass.C"
#include "setTDRStyle.C"
#include "setfreestyle.C"
#include "InvMass.C"
#include "DeltaR.C"
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


int Efficiency(int leptonId, double par_low, double par_upp, int nbins, TString sel_den , TString sel_num, double cut_den = 0., double cut_num = 0., TString par_x = "Pt"){

	///////////////
	//Get the TTree
	///////////////

	TChain* tree = new TChain("treeProducerSusySSDL");
	tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/DYJetsToLLM50_PU_S14_POSTLS170.root");

	//

	setTDRStyle();

	Long64_t n = tree->GetEntries();

	//Path for input and output file. Written in FitDataPath.txt
	ifstream file("EfficiencyPath.txt");
	string str;
	getline(file,str);
	TString _path = str;

	//Name for storing and final plots
	TString pname;
	TString _pname;
	TString treename;
	TString _par;
	TString _sel_num;
	TString _sel_den;
	TString _cut_num;
	TString _cut_den;

	//Writing string
	if (leptonId == 11) {pname = "e";_pname = "e";}
	else if (leptonId == 13){pname = " #mu";_pname = "mu";}
	if(par_x == "Pt"){_par = "P_{t}";}
	else if(par_x == "eta"){_par = "#eta";}
	else if(par_x == "phi"){_par = "#phi";}
	else{cout<<"ERROR: wrong parameter name !";return 1;}
	if(sel_num == ""){_sel_num = "unsel";}
	else if(sel_num == "tightId"){_sel_num = "tightId";}
	else if(sel_num == "dz"){_sel_num = Form("IP dz < %0.3lf ",cut_num);}
	else if(sel_num == "dxy"){_sel_num = Form("IP dxy < %0.3lf ", cut_num) ;}
	else{cout<<"ERROR: wrong numeretor selection name !";return 1;}
	if(sel_den == ""){_sel_den = "unsel";}
	else if(sel_den == "tightId"){_sel_den = "tight";}
	else{cout<<"ERROR: wrong denominator selection name !";return 1;}
	if(cut_den == 0.){_cut_den = "";}
	else{_cut_den = Form("%0.3lf",cut_den);}
	if(cut_num == 0.){_cut_num = "";}
	else{_cut_num = Form("%0.3lf",cut_num);}

	//Name of the output
	TString _output = _path+"eff3_"+_pname+"_"+treename+"_den_"+_sel_den+"_num_"+_sel_num+"_"+par_x+".root";

	//Declaration of histogram
	//
	//efficiency of the isolation cut
	TH1D *histo_num= new TH1D("histo_num","Pt",nbins,par_low,par_upp);
	TH1D *histo_den= new TH1D("histo_den","Pt",nbins,par_low,par_upp);
	TH1D *histo_numE= new TH1D("histo_num","Pt",nbins,par_low,par_upp);
	TH1D *histo_denE= new TH1D("histo_den","Pt",nbins,par_low,par_upp);

	//efficiency of the selection
	TH1D* eff = new TH1D ("eff","Pt",nbins,par_low,par_upp);
	TH1D* effE = new TH1D ("eff","Pt",nbins,par_low,par_upp);

	//Event variables
	//
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
	Float_t receldz[200];
	Float_t receldxy[200];
	//Muons
	Int_t nrecmu;
	Int_t recmutightid[200];
	Float_t recmuPt[200];
	Float_t recmum[200];
	Float_t recmu_eta[200];
	Float_t recmu_phi[200];
	Int_t recmucharge[200];
	Float_t recmuiso03[200];
	Float_t recmuiso04[200];
	Float_t recmudz[200];
	Float_t recmudxy[200];

	//Assigne branches
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
	//tree->SetBranchAddress("el_chargedHadRelIso03", &receliso03);
	//tree->SetBranchAddress("el_chargedHadRelIso04", &receliso04);
	tree->SetBranchAddress("el_dz",&receldz);
	tree->SetBranchAddress("el_dxy",&receldxy);
	
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
	//tree->SetBranchAddress("mu_chargedHadRelIso03", &recmuiso03);
	//tree->SetBranchAddress("mu_chargedHadRelIso04", &recmuiso04);
	tree->SetBranchAddress("mu_dz",&recmudz);
	tree->SetBranchAddress("mu_dxy",&recmudxy);
	
	//Start loop over all events
	for (int k = 0; k < n; ++k) {

		tree->GetEntry(k);

		if(abs(leptonId) == 11){
			for(int j=0; j<nrecel;++j){
				//Cut on the denominator
				if((sel_den != "thightId")||((sel_den == "thightId")&&(receltightid[j] == 1 ))){
					//Veto the EE-EB gape
					if(((abs(recel_eta[j]) < 1.479)||(abs(recel_eta[j]) > 1.653))){

					//Variable for matching
					double R = 999;
					double delta_P = 999;
					double delta_charge = 999;

					//Parameter on the xaxis

					double par;

					//if(abs(recel_eta[j]) < 2.4){ 
						//loop over all generated particles to do the matching
						for (int i = 0; i < ngenPart; ++i) {
							if((abs(Id[i]) == leptonId)&&(status[i]== 1)&&(abs(gen_eta[i]) < 2.4)){ 

								//Electrons selection
								double R2 = DeltaR(gen_eta[i],recel_eta[j],gen_phi[i],recel_phi[j] );

								//Minimise DeltaR and Fill the other variables
								if (R > R2) {

									R = R2;
									delta_P = abs(recelPt[j]-Pt[i])/Pt[i];
									delta_charge = abs(recelcharge[j] - charge[i]);
								}
							}
						}
					//}			

					//Choose the parameter to be filled for the eff.
					if(par_x == "Pt"){par = recelPt[j];}
					else if(par_x == "eta"){par = recel_eta[j];}
					else if(par_x == "phi"){par = recel_phi[j];}

					//Fill Pt only for matched events
					if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){
						//Filling the den
						if(abs(recel_eta[j]) < 1.2)histo_den->Fill(par);
						if(abs(recel_eta[j]) >= 1.2)histo_denE->Fill(par);

						//Additional cut on the numerator
						if((sel_num != "tightId")||((sel_num == "tightId")&&(receltightid[j] == 1 ))){
							if((sel_num != "dxy")||((sel_num == "dxy")&&(abs(receldxy[j]) < cut_num ))){
								if((sel_num != "dz")||((sel_num == "dz")&&(abs(receldz[j]) < cut_num ))){
									if(abs(recel_eta[j]) < 1.2)histo_num->Fill(par);
									if(abs(recel_eta[j]) >= 1.2)histo_numE->Fill(par);
								}
							}
						}
					}
					}
				}
			}
		}
		if(abs(leptonId) == 13){
			for(int j=0; j<nrecmu;++j){
				//Cut on the denominator
				if((sel_den != "thightId")||((sel_den == "thightId")&&(recmutightid[j] == 1 ))){
					//Veto the EE-EB gape
					if(((abs(recmu_eta[j]) < 1.479)||(abs(recmu_eta[j]) > 1.653))){

					//Variable for matching
					double R = 999;
					double delta_P = 999;
					double delta_charge = 999;

					//Parameter on the xaxis

					double par;

					//if(abs(recel_eta[j]) < 2.4){ 
						//loop over all generated particles to do the matching
						for (int i = 0; i < ngenPart; ++i) {
							if((abs(Id[i]) == leptonId)&&(status[i]== 1)&&(abs(gen_eta[i]) < 2.4)){ 

								//Electrons selection
								double R2 = DeltaR(gen_eta[i],recmu_eta[j],gen_phi[i],recmu_phi[j] );

								//Minimise DeltaR and Fill the other variables
								if (R > R2) {

									R = R2;
									delta_P = abs(recmuPt[j]-Pt[i])/Pt[i];
									delta_charge = abs(recmucharge[j] - charge[i]);
								}
							}
						}
					//}			

					//Choose the parameter to be filled for the eff.
					if(par_x == "Pt"){par = recmuPt[j];}
					else if(par_x == "eta"){par = recmu_eta[j];}
					else if(par_x == "phi"){par = recmu_phi[j];}

					//Fill Pt only for matched events
					if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){
						//Filling the den
						if(abs(recmu_eta[j]) < 1.2)histo_den->Fill(par);
						if(abs(recmu_eta[j]) >= 1.2)histo_denE->Fill(par);

						//Additional cut on the numerator
						if((sel_num != "tightId")||((sel_num == "tightId")&&(recmutightid[j] == 1 ))){
							if((sel_num != "dxy")||((sel_num == "dxy")&&(abs(recmudxy[j]) < cut_num ))){
								if((sel_num != "dz")||((sel_num == "dz")&&(abs(recmudz[j]) < cut_num ))){
									if(abs(recmu_eta[j]) < 1.2)histo_num->Fill(par);
									if(abs(recmu_eta[j]) >= 1.2)histo_numE->Fill(par);
								}
							}
						}
					}
					}
				}
			}
		}

	}

	histo_num->Sumw2();
	histo_numE->Sumw2();
	histo_den->Sumw2();
	histo_denE->Sumw2();


	//Divide histograms to get the efficiency
	eff->Divide(histo_num,histo_den,1,1,"B"); 
	effE->Divide(histo_numE,histo_denE,1,1,"B"); 

	//Efficiency of the iso cut.
	TCanvas* c1 = new TCanvas("c1","c1");
	eff->Draw();
	eff->GetYaxis()->SetRangeUser(0,1.1);
	eff->GetYaxis()->SetTitle("#epsilon");
	eff->GetXaxis()->SetTitle(_par);
	eff->GetXaxis()->SetRangeUser(0,250);
	eff->SetMarkerStyle(20);
	eff->SetMarkerSize(1);
	eff->SetMarkerColor(4);
	eff->SetLineColor(4);
	eff->SetTitle(_sel_num+" for "+_sel_den+" "+pname+" "+treename+" #||{#eta}<1.2");
	TCanvas* c2 = new TCanvas("c2","c2");
	effE->Draw();
	effE->GetYaxis()->SetRangeUser(0,1.1);
	effE->GetYaxis()->SetTitle("#epsilon");
	effE->GetXaxis()->SetTitle(_par);
	effE->GetXaxis()->SetRangeUser(0,250);
	effE->SetMarkerStyle(20);
	effE->SetMarkerSize(1);
	effE->SetMarkerColor(4);
	effE->SetLineColor(4);
	effE->SetTitle(_sel_num+" for "+_sel_den+" "+pname+" "+treename+" #||{#eta}>1.2");
	//TLegend* leg2 = new TLegend(0.4, 0.2,0.9,0.4);
	//leg2->SetBorderSize(0.);
	//leg2->SetTextFont(43);
	//leg2->SetTextSize(25);
	//leg2->AddEntry(eff_isocut_10, "rel. iso"+isoname+"  #leq "+ (TString)high_cut);  
	//leg2->AddEntry(eff_isocut_7, "rel. iso"+isoname+"  #leq "+ (TString)med_cut);
	//leg2->AddEntry(eff_isocut_5, "rel. iso test"+isoname+"  #leq "+(TString)low_cut);
	//leg2->Draw();
	
	//Define the name of the canvas
	TString c1name = "eff_"+_pname+"_"+treename+"_den_"+_sel_den+"_num_"+_sel_num+"_"+par_x+"eta<1.2";
	TString c2name = "eff_"+_pname+"_"+treename+"_den_"+_sel_den+"_num_"+_sel_num+"_"+par_x+"eta>1.2";
	
	c1->SaveAs(_path+"PDF/"+c1name+".pdf");
	c2->SaveAs(_path+"PDF/"+c2name+".pdf");

	TFile* output = new TFile(_output,"recreate");
	c1->Write();
	c2->Write();
	output->Close();

}
