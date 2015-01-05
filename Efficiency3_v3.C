/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : Efficiency3_v3.C

* Purpose :  Plot the efficiency for a given cut that is applied to reconstructed data. A matching between the gen. and rec. data is done.
  Barrel/endcape are separation 


* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By :

_._._._._._._._._._._._._._._._._._._._._.*/

//#include "cmath"
//#include "TChain.h"
//#include <sys/stat.h>
//#include "iostream"
//#include "fstream"
//#include "TFile.h"
//#include "TTree.h"
//#include "TH1.h"
//#include "TBranch.h"
//#include "TCanvas.h"
//#include "TStyle.h"
//#include "TROOT.h"
//#include "TString.h"
//#include "TLegend.h"
//#include "TGraphErrors.h"
//#include "TGraph.h"
//#include "TLorentzVector.h"
//#include "setTDRStyle.C"
//#include "InvMass.C"
//#include "DeltaR.C"
//#include "vector"
//#include <iostream>
//#include "TFile.h"
//#include "TTree.h"
//#include "TH1.h"
//#include "TBranch.h"
//#include "TCanvas.h"
//#include "TLorentzVector.h"
//#include "TStyle.h"
//#include "TROOT.h"
//#include "TString.h"
//#include "TLegend.h"
//#include "TObject.h"

int Efficiency_etasep(int leptonId, double par_low, double par_upp, int nbins, TString sel_den , TString sel_num, double cut_den = 0., double cut_num = 0., TString par_x = "Pt", TString option = ""){

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

	//Name for storing and final plots
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
	if(sel_num == ""){_sel_num = "unsel";}
	else if(sel_num == "tight"){_sel_num = "tight";}
	else if(sel_num == "loose"){_sel_num = "loose";}
	else if(sel_num == "reliso3"){_sel_num = Form("reliso3_%0.3lf",cut_num);}
	else if(sel_num == "reliso4"){_sel_num = Form("reliso4_%0.3lf",cut_num);}
	else if(sel_num == "chiso3"){_sel_num = Form("chiso3_%0.3lf",cut_num);}
	else if(sel_num == "chiso4"){_sel_num = Form("chiso4_%0.3lf",cut_num);}
	else if(sel_num == "dxy"){_sel_num = Form("dxy_%0.3lf",cut_num);}
	else if(sel_num == "dz"){_sel_num = Form("dz_%0.3lf",cut_num);}
	else{cout<<"ERROR: wrong numerator name !";return 1;}
	cout<<"The selection on the numerator is"<<_sel_num<<endl;
	cout<<"cut_num is "<<cut_num<<endl;
	if(sel_den == ""){_sel_den = "unsel";}
	else if(sel_den == "tight"){_sel_den = "tight";}
	else{cout<<"ERROR: wrong denominator selection name !";return 1;}
	if(cut_den == 0.){_cut_den = "";}
	else{_cut_den = Form("%0.3lf",cut_den);}
	if(option.Contains("ll")){_option += "ll";}
	if(option.Contains("sep")){_option += "_sep";}
	_option += "_";

	//Name of the output
	TString _output = _path+"eff3"+_option+_pname+"_den_"+_sel_den+"_num_"+_sel_num+"_"+par_x+".root";

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
	tree->SetBranchAddress("LepGood_looseIdSusy",&Gloose);
	tree->SetBranchAddress("LepGood_relIso03",&Giso3);
	tree->SetBranchAddress("LepGood_relIso04",&Giso4);
	tree->SetBranchAddress("LepGood_chargedHadRelIso03",&Gchiso3);
	tree->SetBranchAddress("LepGood_chargedHadRelIso04",&Gchiso4);
	tree->SetBranchAddress("LepGood_dxy",&Gdxy);
	tree->SetBranchAddress("LepGood_dz",&Gdz);


	//Start loop over all events
	for (int k = 0; k < n; ++k) {

		tree->GetEntry(k);

		//loop on reconstructed
		for(int j=0; j<Gn;++j){
			if((!option.Contains("ll"))||((option.Contains("ll"))&&(Gn == 2)&&(Gid[0] == -Gid[1]))){
				if(abs(Gid[j]) == leptonId){
					//Cut on the denominator
					if((sel_den != "thight")||((sel_den == "thight")&&(Gtight[j] == 1 ))){
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
						if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){
							//Filling the den
							if(abs(Geta[j]) < 1.2)histo_den->Fill(par);
							if(abs(Geta[j]) >= 1.2)histo_denE->Fill(par);

							//Additional cut on the numerator
							int a = 0;

							if((sel_num == "tight")&&(Gtight[j] == 1)){a = 1;}
							if((sel_num == "reliso3")&&(Giso3[j] <= cut_num)){a = 2;}
							if((sel_num == "reliso4")&&(Giso4[j] <= cut_num)){a = 3;}
							if((sel_num == "chiso3")&&(Gchiso3[j] <= cut_num)){a = 4;}
							if((sel_num == "chiso4")&&(Gchiso4[j] <= cut_num)){a = 5;}
							if((sel_num == "dxy")&&(abs(Gdxy[j]) <= cut_num)){a = 6;}
							if((sel_num == "dz")&&(abs(Gdz[j]) <= cut_num)){a = 7;}


							switch(a){

								case 0:

									break;

								case 1:
									if(abs(Geta[j]) < 1.2)histo_num->Fill(par);
									if(abs(Geta[j]) >= 1.2)histo_numE->Fill(par);
									break;

								case 2:
									if(abs(Geta[j]) < 1.2)histo_num->Fill(par);
									if(abs(Geta[j]) >= 1.2)histo_numE->Fill(par);
									break;
								case 3:
									if(abs(Geta[j]) < 1.2)histo_num->Fill(par);
									if(abs(Geta[j]) >= 1.2)histo_numE->Fill(par);
									break;
								case 4:
									if(abs(Geta[j]) < 1.2)histo_num->Fill(par);
									if(abs(Geta[j]) >= 1.2)histo_numE->Fill(par);
									break;
								case 5:
									if(abs(Geta[j]) < 1.2)histo_num->Fill(par);
									if(abs(Geta[j]) >= 1.2)histo_numE->Fill(par);
									break;
								case 6:
									if(abs(Geta[j]) < 1.2)histo_num->Fill(par);
									if(abs(Geta[j]) >= 1.2)histo_numE->Fill(par);
									break;

								case 7:
									if(abs(Geta[j]) < 1.2)histo_num->Fill(par);
									if(abs(Geta[j]) >= 1.2)histo_numE->Fill(par);
									break;
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
	eff->GetYaxis()->SetTitle("#epsilon");
	eff->GetXaxis()->SetTitle(_par);
	eff->GetXaxis()->SetRangeUser(0,250);
	eff->SetMarkerStyle(20);
	eff->SetMarkerSize(1);
	eff->SetMarkerColor(4);
	eff->SetLineColor(4);
	eff->SetTitle(_sel_num+" for "+_sel_den+" "+pname+" #||{#eta}<1.2");
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
	effE->SetTitle(_sel_num+" for "+_sel_den+" "+pname+" #||{#eta}>1.2");

	//Define the name of the canvas
	TString c1name = "eff"+option+_pname+"_den_"+_sel_den+"_num_"+_sel_num+"_"+par_x+"_eta<1.2";
	TString c2name = "eff"+option+_pname+"_den_"+_sel_den+"_num_"+_sel_num+"_"+par_x+"_eta>1.2";

	c1->SaveAs(_path+"PDF/"+c1name+".pdf");
	c2->SaveAs(_path+"PDF/"+c2name+".pdf");

	TFile* output = new TFile(_output,"recreate");
	//c1->Write();
	//c2->Write();
	eff->Write("eff_eta<1.2");
	effE->Write("eff_eta>1.2");
	output->Close();

	return 0;

}
