/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : CutStudy.C

* Purpose : This files plots the cuts like reliso, chiso, IP.
 Description of the funciton argument:
 
 _tree:         Name of the used TTree. Can take as parameter "DY", "DYInc", "WJ" and "TT".
 lepton_Id:     Pdg_Id of the corresponding lepton. put 11 for electron and 13 for muon.
 sel_den:       Selection of the electron. can take as parameter "tightId", "reliso3", "reliso4", "chiso3",                  
                "chiso4", "dz", "dxy".
 sel_num:       The cut that you want to study. Takes the same parameter as the sel_den
 cut_den:       If the sel_den is a float (and not int like "tighId"), one has to enter the cut as the den_cut              
                argument.
 
 example:       CutStudy("DY",13,"dz","dxy",0.03) plots the dxy of the IP dxy for the muon coming from DY
                satisfying dz < 0.03
 
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
//#include "FitInvMass.C"
#include "setTDRStyle.C"
#include "setfreestyle.C"
#include "InvMass.C"
#include "DeltaR.C"
#include "vector"

using namespace std;

int CutStudyMain(TString _tree, TTree* tree, int leptonId, TString sel_den , TString sel_num, double cut_den = 0.);

void CutStudy(TString _tree,int lepton_Id, TString sel_den,TString sel_num, double cut_den){

	//Location of the .root file
	TString location = "/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/";

	//Reading the tree 
	//
	TChain* tree = new TChain("treeProducerSusySSDL");

	//DY events
	//Inclusive
	if(_tree.Contains("DYInc")){tree->Add(location+"DYJetsToLLM50_PU_S14_POSTLS170.root");}
	//From 100 to Inf HT
	else if(_tree.Contains("DY")){tree->Add(location+"/DYJetsM50*.root");}
	
	//WJet events
 	if(_tree.Contains("WJ")){tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/WJets*.root");}

	//TTJets events
	if(_tree.Contains("TTInc")){tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/TTJets_MS*.root");}

	//Plot the result
	//
	CutStudyMain(_tree, tree,lepton_Id,sel_den,sel_num, cut_den);

}

int CutStudyMain(TString _tree, TTree* tree, int leptonId, TString sel_den , TString sel_num, double cut_den ){

	setTDRStyle();

	Long64_t n = tree->GetEntries();

	TString _path = "/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/cutstudy/";
	
	//Histogram parameter
	int nbins = -10;
	double par_low = -10;
	double par_upp = -10;

	//Set parameter range
	if(sel_num == "tightId"){nbins = 2;par_low = 0;par_upp = 2.;}
	else if(sel_num == "reliso3"){nbins = 100;par_low = 0;par_upp = 0.5;}
	else if(sel_num == "reliso4"){nbins = 100;par_low = 0;par_upp = 0.5;}
	else if(sel_num == "chiso3"){nbins = 100;par_low = 0;par_upp = 0.5;}
	else if(sel_num == "chiso4"){nbins = 100;par_low = 0;par_upp = 0.5;}
	else if(sel_num == "dz"){nbins = 100;par_low = 0;par_upp = 0.06;}
	else if(sel_num == "dxy"){nbins = 100;par_low = 0;par_upp = 0.07;}

	//Name for storing and final plots
	TString pname;
	TString _pname;
	TString treename;
	TString _par;
	//TString _sel_num;
	TString _sel_den;
	TString _sel_num;
	TString _cut_num;
	TString _cut_den;
	TString _mother_Id;

	//Writing string
	if(_tree.Contains("DY")){treename += "DY";}
	if(_tree.Contains("WJ")){treename += "W";}
	if(_tree.Contains("TT")){treename += "TT";}

	if (leptonId == 11) {pname = "e";_pname = "e";}
	else if (leptonId == 13){pname = " #mu";_pname = "mu";}
	if(sel_num == "tightId"){_par = "tightId";_sel_num = "tight";}
	else if(sel_num == "reliso3"){_par = "rel. iso. #Delta R = 0.3"; _sel_num = "rel. isolation R = 0.3";}
	else if(sel_num == "reliso4"){_par = "rel. iso. #DeltaR = 0.4";_sel_num = "rel. isolation R = 0.4";}
	else if(sel_num == "chiso3"){_par = "chrgd. iso. #DeltaR = 0.3";_sel_num = "rel. isolation R = 0.3";}
	else if(sel_num == "chiso4"){_par = "chrgd. iso. #DeltaR = 0.4";_sel_num = "rel. isolation R = 0.4";}
	else if(sel_num == "dxy"){_par = "dxy [cm]";}
	else if(sel_num == "dz"){_par = "dz [cm]";}
	else{cout<<"ERROR: wrong numerator name !";return 1;}
	//if(sel_num == ""){_sel_num = "unsel";}
	//else if(sel_num == "tightId"){_sel_num = "tightId";}
	//else if(sel_num == "dz"){_sel_num = "dz";}
	//else if(sel_num == "dxy"){_sel_num = "dxy";}
	//else if(sel_num == "reliso3"){_sel_num = "}
	//else if(sel_num == "reliso4"){}
	//else if(sel_num == "chiso3"){}
	//else if(sel_num == "chiso4"){}
	//else{cout<<"ERROR: wrong numeretor selection name !";return 1;}
	if(sel_den == ""){_sel_den = "unsel";}
	else if(sel_den == "tightId"){_sel_den = "tight";}
	else if(sel_den == "reliso3"){_sel_den = Form("Rel. iso03 < %0.3lf ",cut_den);}
	else if(sel_den == "reliso4"){_sel_den = Form("Rel. iso04 < %0.3lf ",cut_den);}
	else if(sel_den == "chiso3"){_sel_den = Form("Ch. iso03 < %0.3lf ",cut_den) ;}
	else if(sel_den == "chiso4"){_sel_den = Form("Ch. iso04 < %0.3lf ",cut_den) ;}
	else if(sel_den == "dxy"){_sel_den = Form("IP dxy < %0.3lf ",cut_den);}
	else if(sel_den == "dz"){_sel_den = Form("IP dz < %0.3lf ",cut_den);}
	else{cout<<"ERROR: wrong denominator selection name !";return 1;}

	//Name of the output
	TString _out = "cut_"+_pname+"_from"+treename+"_den_"+_sel_den+"_"+sel_num;
	TString _output = _path+_out+".root";

	//Declaration of histogram
	//
	//efficiency of the isolation cut
	TH1D *histo= new TH1D("histo","Cut",nbins,par_low,par_upp);

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
	Float_t reccheliso03[200];
	Float_t reccheliso04[200];
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
	Float_t recchmuiso03[200];
	Float_t recchmuiso04[200];
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
	tree->SetBranchAddress("el_chargedHadRelIso03", &reccheliso03);
	tree->SetBranchAddress("el_chargedHadRelIso04", &reccheliso04);
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
	tree->SetBranchAddress("mu_chargedHadRelIso03", &recchmuiso03);
	tree->SetBranchAddress("mu_chargedHadRelIso04", &recchmuiso04);
	tree->SetBranchAddress("mu_dz",&recmudz);
	tree->SetBranchAddress("mu_dxy",&recmudxy);

	//Start loop over all events
	for (int k = 0; k < n; ++k) {

		tree->GetEntry(k);

		if(leptonId == 11){
			for(int j=0; j<nrecel;++j){
				//Cut on the denominator
				if((sel_den != "tightId")||((sel_den == "tightId")&&(receltightid[j] == 1 ))){
				if((sel_den != "reliso3")||((sel_den == "reliso3")&&(receliso03[j] == 1 ))){
				if((sel_den != "reliso4")||((sel_den == "reliso4")&&(receliso04[j] == 1 ))){
				if((sel_den != "chiso3")||((sel_den == "chiso3")&&(reccheliso03[j] == 1 ))){
				if((sel_den != "chiso4")||((sel_den == "chiso4")&&(reccheliso04[j] == 1 ))){
				if((sel_den != "dxy")||((sel_den == "dxy")&&(receldxy[j] < cut_den ))){
				if((sel_den != "dz")||((sel_den == "dz")&&(receldz[j] < cut_den ))){
					//Veto the EE-EB gape
					if(((abs(recel_eta[j]) < 1.479)||(abs(recel_eta[j]) > 1.653))){

					//Variable for matching
					double R = 999;
					double delta_P = 999;
					double delta_charge = 999;

					//Parameter on the xaxis

					double par;

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

					//Choose the parameter to be filled for the eff.
					if(sel_num == "tightId"){par = receltightid[j];}
					else if(sel_num == "reliso3"){par = min((double)receliso03[j],par_upp*(1-1./nbins));}
					else if(sel_num == "reliso4"){par = min((double)receliso04[j],par_upp*(1-1./nbins));}
					else if(sel_num == "chiso3"){par = min((double)reccheliso03[j],par_upp*(1-1./nbins));}
					else if(sel_num == "chiso4"){par = min((double)reccheliso04[j],par_upp*(1-1./nbins));}
					else if(sel_num == "dxy"){par = min(abs((double)receldz[j]),par_upp*(1-1./nbins));}
					else if(sel_num == "dz"){par = min(abs((double)receldxy[j]),par_upp*(1-1./nbins));}

					//Fill Pt only for matched events
					if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){
						//Additional cut on the numerator
						histo->Fill(par);
					}

					}
				}
				}
				}
				}}}}
			}}


		if(leptonId == 13){
			for(int j=0; j<nrecmu;++j){
				//Cut on the denominator
				if((sel_den != "tightId")||((sel_den == "tightId")&&(recmutightid[j] == 1 ))){
				if((sel_den != "reliso3")||((sel_den == "reliso3")&&(recmuiso03[j] == 1 ))){
				if((sel_den != "reliso4")||((sel_den == "reliso4")&&(recmuiso04[j] == 1 ))){
				if((sel_den != "chiso3")||((sel_den == "chiso3")&&(recchmuiso03[j] == 1 ))){
				if((sel_den != "chiso4")||((sel_den == "chiso4")&&(recchmuiso04[j] == 1 ))){
				if((sel_den != "dxy")||((sel_den == "dxy")&&(recmudxy[j] < cut_den ))){
				if((sel_den != "dz")||((sel_den == "dz")&&(recmudz[j] < cut_den ))){
					//Veto the EE-EB gape
					if(((abs(recmu_eta[j]) < 1.479)||(abs(recmu_eta[j]) > 1.653))){

					//Variable for matching
					double R = 999;
					double delta_P = 999;
					double delta_charge = 999;

					//Parameter on the xaxis

					double par;

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

					//Choose the parameter to be filled for the eff.
					if(sel_num == "tightId"){par = recmutightid[j];}
					else if(sel_num == "reliso3"){par = min((double)recmuiso03[j],par_upp*(1-1./nbins));}
					else if(sel_num == "reliso4"){par = min((double)recmuiso04[j],par_upp*(1-1./nbins));}
					else if(sel_num == "chiso3"){par = min((double)recchmuiso03[j],par_upp*(1-1./nbins));}
					else if(sel_num == "chiso4"){par = min((double)recchmuiso04[j],par_upp*(1-1./nbins));}
					else if(sel_num == "dxy"){par = min(abs((double)recmudz[j]),par_upp*(1-1./nbins));}
					else if(sel_num == "dz"){par = min(abs((double)recmudxy[j]),par_upp*(1-1./nbins));}

					//Fill Pt only for matched events
					if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){
						//Additional cut on the numerator
						histo->Fill(par);
					}

					}
				}
				}
				}
				}}}}
			}}

	}

	//Renormalise histogram
	histo->Scale(1./histo->GetEntries());

	//Efficiency of the iso cut.
	TCanvas* c1 = new TCanvas("c1","c1");
	c1->SetLogy();
	histo->Draw();
	histo->GetYaxis()->SetTitle("#epsilon");
	histo->GetXaxis()->SetTitle(_par);
	histo->GetXaxis()->SetNdivisions(509);
	histo->SetMarkerStyle(20);
	histo->SetMarkerSize(1);
	histo->SetMarkerColor(4);
	histo->SetLineColor(4);
	histo->SetLineWidth(2);
	histo->SetTitle(_sel_den+" "+pname+" "+treename);
	
	//Define the name of the canvas
	//TString cname = "eff_"+_pname+"_from"+treename+"_den_"+_sel_den+"_"+sel_num;
	
	c1->SaveAs(_path+"/PDF/"+_out+".pdf");

	TFile* output = new TFile(_output,"recreate");
	c1->Write();
	output->Close();

	return 0;


	}
