/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : CutCorrelationStudy.C++ 

* Purpose : Similar to CutStudy but do the correlation plot between selection variables. 

Arguments : 

_tree: 	Name of the used TTree. Can take as parameter "DY", "DYInc", "WJ" and "TT".

lepton_Id: Pdg_Id of the corresponding lepton. put 11 for electron and 13 for muon.

sel_den: Selection of the electron. can take as parameter "tightId", "reliso3", "reliso4", "chiso3",                  
         "chiso4", "dz", "dxy".

narray:  Number of id cut under sutdy.

sel_num1: array of length narray containing all the cuts to be in the x-axis. The cuts are the same parameter as in sel_den.

sel_num2: array of length narray containing all the cuts to be in the y-axis. The cuts are the same parameter as in sel_den.

cut_den:       If the sel_den is a float (and not int like "tighId"), one has to enter the cut as the den_cut              
               argument.

example:       CutCorrelationStudy("WJTT",11,"tightId",6,s1,s1,0), where s1 is defined as:
	       TString* s1 = new TString[6];
	       s1[0] = "dz";s1[1] = "dxy";s1[2] = "reliso3";s1[3] = "reliso4";s1[4] = "chiso3";s1[5] = "chiso4"
		
	       this will save canvas containing the correlation of all the possible combination from above cuts
  

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
#include "TH2D.h"
#include "TBranch.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TString.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TLorentzVector.h"
#include "setTDRStyle.C"
#include "setfreestyle.C"
#include "InvMass.C"
#include "DeltaR.C"
#include "vector"

using  namespace std;

int CutCorrelationStudyMain(TString _tree, TTree* tree, int leptonId, TString sel_den , int narray, TString* sel_num1, TString* sel_num2, double cut_den );

void CutCorrelationStudy(TString _tree,int lepton_Id, TString sel_den , int narray, TString* sel_num1, TString* sel_num2, double cut_den ){

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
	if(_tree.Contains("TT")){tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/TTJets_MS*.root");}

	//Plot the result
	
	CutCorrelationStudyMain(_tree, tree,lepton_Id,sel_den, narray, sel_num1, sel_num2, cut_den);

}

int CutCorrelationStudyMain(TString _tree, TTree* tree, int leptonId, TString sel_den ,int narray, TString* sel_num1, TString* sel_num2, double cut_den ){

	setTDRStyle();

	Long64_t n = tree->GetEntries();

	TString _path = "/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/cutstudy/";
	

	//Name for storing and final plots
	TString pname;
	TString _pname;
	TString treename;
	TString _treename;
	TString* _par1 = new TString[narray];
	TString* _par2 = new TString[narray];
	TString* _sel_num1 = new TString[narray];
	TString* _sel_num2 = new TString[narray];
	//TString _sel_num;
	TString _sel_den;
	TString _cut_den;

	//Writing string
	if(_tree.Contains("DYInc")){treename += "_DYInc";_treename += " DYInc";}
	else if(_tree.Contains("DY")){treename += "_DY";_treename += " DY";}
	if(_tree.Contains("WJ")){treename += "_W";_treename += " W";}
	if(_tree.Contains("TT")){treename += "_TT";_treename += "_TT";}

	if (leptonId == 11) {pname = "e";_pname = "e";}
	else if (leptonId == 13){pname = " #mu";_pname = "mu";}

	int nbins1[narray];
	double par_low1[narray];
	double par_upp1[narray];
	//Histogram parameter
	int nbins2[narray];
	double par_low2[narray];
	double par_upp2[narray];

	//Name of the output
	TString _out = "corr_"+_pname+"_from"+treename;
	TString _output = _path+_out+".root";

	//Output file
	TFile* output = new TFile(_output,"recreate");

	//Name of the canvas to be saved
	TString** _canvas = new TString*[narray];

	for(int ll = 0; ll < narray; ++ll){_canvas[ll] = new TString[narray];}

	//Declaration of corr histo
	TH2D*** histo = new TH2D**[narray];

	for(int ii = 0; ii < narray; ++ii){

	histo[ii] = new TH2D*[narray];

		for(int kk = 0; kk< narray; ++ kk){    

	//Histogram parameter

	//Set parameter range
	if(sel_num1[ii] == "tightId"){nbins1[ii] = 2;par_low1[ii] = 0;par_upp1[ii] = 2.;}
	else if(sel_num1[ii] == "reliso3"){nbins1[ii] = 100;par_low1[ii] = 0;par_upp1[ii] = 0.5;}
	else if(sel_num1[ii] == "reliso4"){nbins1[ii] = 100;par_low1[ii] = 0;par_upp1[ii] = 0.5;}
	else if(sel_num1[ii] == "chiso3"){nbins1[ii] = 100;par_low1[ii] = 0;par_upp1[ii] = 0.5;}
	else if(sel_num1[ii] == "chiso4"){nbins1[ii] = 100;par_low1[ii] = 0;par_upp1[ii] = 0.5;}
	else if(sel_num1[ii] == "dz"){nbins1[ii] = 100;par_low1[ii] = 0;par_upp1[ii] = 0.06;}
	else if(sel_num1[ii] == "dxy"){nbins1[ii] = 100;par_low1[ii] = 0;par_upp1[ii] = 0.07;}

	if(sel_num2[kk] == "tightId"){nbins2[kk] = 2;par_low2[kk] = 0;par_upp2[kk] = 2.;}
	else if(sel_num2[kk] == "reliso3"){nbins2[kk] = 100;par_low2[kk] = 0;par_upp2[kk] = 0.5;}
	else if(sel_num2[kk] == "reliso4"){nbins2[kk] = 100;par_low2[kk] = 0;par_upp2[kk] = 0.5;}
	else if(sel_num2[kk] == "chiso3"){nbins2[kk] = 100;par_low2[kk] = 0;par_upp2[kk] = 0.5;}
	else if(sel_num2[kk] == "chiso4"){nbins2[kk] = 100;par_low2[kk] = 0;par_upp2[kk] = 0.5;}
	else if(sel_num2[kk] == "dz"){nbins2[kk] = 100;par_low2[kk] = 0;par_upp2[kk] = 0.06;}
	else if(sel_num2[kk] == "dxy"){nbins2[kk] = 100;par_low2[kk] = 0;par_upp2[kk] = 0.07;}

	histo[ii][kk] = new TH2D("histo", "corr", nbins1[ii], par_low1[ii], par_upp1[ii], nbins2[kk], par_low2[kk], par_upp2[kk]);

	if(sel_num1[ii] == "tightId"){_par1[ii] = "tightId";_sel_num1[ii] = "tight";}
	else if(sel_num1[ii] == "reliso3"){_par1[ii] = "rel. iso. #Delta R = 0.3"; _sel_num1[ii] = "rel. isolation R = 0.3";}
	else if(sel_num1[ii] == "reliso4"){_par1[ii] = "rel. iso. #DeltaR = 0.4";_sel_num1[ii] = "rel. isolation R = 0.4";}
	else if(sel_num1[ii] == "chiso3"){_par1[ii] = "chrgd. iso. #DeltaR = 0.3";_sel_num1[ii] = "rel. isolation R = 0.3";}
	else if(sel_num1[ii] == "chiso4"){_par1[ii] = "chrgd. iso. #DeltaR = 0.4";_sel_num1[ii] = "rel. isolation R = 0.4";}
	else if(sel_num1[ii] == "dxy"){_par1[ii] = "dxy [cm]";}
	else if(sel_num1[ii] == "dz"){_par1[ii] = "dz [cm]";}
	else{cout<<"ERROR: wrong numerator name !";return 1;}
	if(sel_num2[kk] == "tightId"){_par2[kk] = "tightId";sel_num2[kk] = "tight";}
	else if(sel_num2[kk] == "reliso3"){_par2[kk] = "rel. iso. #Delta R = 0.3"; _sel_num2[kk] = "rel. isolation R = 0.3";}
	else if(sel_num2[kk] == "reliso4"){_par2[kk] = "rel. iso. #DeltaR = 0.4";_sel_num2[kk] = "rel. isolation R = 0.4";}
	else if(sel_num2[kk] == "chiso3"){_par2[kk] = "chrgd. iso. #DeltaR = 0.3";_sel_num2[kk] = "rel. isolation R = 0.3";}
	else if(sel_num2[kk] == "chiso4"){_par2[kk] = "chrgd. iso. #DeltaR = 0.4";_sel_num2[kk] = "rel. isolation R = 0.4";}
	else if(sel_num2[kk] == "dxy"){_par2[kk] = "dxy [cm]";}
	else if(sel_num2[kk] == "dz"){_par2[kk] = "dz [cm]";}
	else{cout<<"ERROR: wrong numerator name !";return 1;}
	if(sel_den == ""){_sel_den = "unsel";}
	else if(sel_den == "tightId"){_sel_den = "tight";}
	else if(sel_den == "reliso3"){_sel_den = Form("Rel. iso03 < %0.3lf ",cut_den);}
	else if(sel_den == "reliso4"){_sel_den = Form("Rel. iso04 < %0.3lf ",cut_den);}
	else if(sel_den == "chiso3"){_sel_den = Form("Ch. iso03 < %0.3lf ",cut_den) ;}
	else if(sel_den == "chiso4"){_sel_den = Form("Ch. iso04 < %0.3lf ",cut_den) ;}
	else if(sel_den == "dxy"){_sel_den = Form("IP dxy < %0.3lf ",cut_den);}
	else if(sel_den == "dz"){_sel_den = Form("IP dz < %0.3lf ",cut_den);}
	else{cout<<"ERROR: wrong denominator selection name !";return 1;}

	_canvas[ii][kk] = "corr_"+_pname+"_from"+treename+"_"+sel_den+"_num1_"+sel_num1[ii]+"_num2_"+sel_num2[kk];

		}
	}

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
		
	for(int ii = 0; ii < narray; ++ii){
		for(int kk = 0; kk< ii; ++ kk){    

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

					double par1;
					double par2;

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
					if(sel_num1[ii] == "tightId"){par1 = receltightid[j];}
					else if(sel_num1[ii] == "reliso3"){par1 = min((double)receliso03[j],par_upp1[ii]*(1-1./nbins1[ii]));}
					else if(sel_num1[ii] == "reliso4"){par1 = min((double)receliso04[j],par_upp1[ii]*(1-1./nbins1[ii]));}
					else if(sel_num1[ii] == "chiso3"){par1 = min((double)reccheliso03[j],par_upp1[ii]*(1-1./nbins1[ii]));}
					else if(sel_num1[ii] == "chiso4"){par1 = min((double)reccheliso04[j],par_upp1[ii]*(1-1./nbins1[ii]));}
					else if(sel_num1[ii] == "dxy"){par1 = min(abs((double)receldz[j]),par_upp1[ii]*(1-1./nbins1[ii]));}
					else if(sel_num1[ii] == "dz"){par1 = min(abs((double)receldxy[j]),par_upp1[ii]*(1-1./nbins1[ii]));}
					//Choose the parameter to be filled for the eff.
					if(sel_num2[kk] == "tightId"){par1 = receltightid[j];}
					else if(sel_num2[kk] == "reliso3"){par2 = min((double)receliso03[j],par_upp2[kk]*(2-1./nbins2[kk]));}
					else if(sel_num2[kk] == "reliso4"){par2 = min((double)receliso04[j],par_upp2[kk]*(1-1./nbins2[kk]));}
					else if(sel_num2[kk] == "chiso3"){par2 = min((double)reccheliso03[j],par_upp2[kk]*(1-1./nbins2[kk]));}
					else if(sel_num2[kk] == "chiso4"){par2 = min((double)reccheliso04[j],par_upp2[kk]*(1-1./nbins2[kk]));}
					else if(sel_num2[kk] == "dxy"){par2 = min(abs((double)receldz[j]),par_upp2[kk]*(1-1./nbins2[kk]));}
					else if(sel_num2[kk] == "dz"){par2 = min(abs((double)receldxy[j]),par_upp2[kk]*(1-1./nbins2[kk]));}

					//Fill Pt only for matched events
					if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){
						//Additional cut on the numerator
						histo[ii][kk]->Fill(par1,par2);
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

					double par1;
					double par2;

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
					if(sel_num1[ii] == "tightId"){par1 = recmutightid[j];}
					else if(sel_num1[ii] == "reliso3"){par1 = min((double)recmuiso03[j],par_upp1[ii]*(1-1./nbins1[ii]));}
					else if(sel_num1[ii] == "reliso4"){par1 = min((double)recmuiso04[j],par_upp1[ii]*(1-1./nbins1[ii]));}
					else if(sel_num1[ii] == "chiso3"){par1 = min((double)recchmuiso03[j],par_upp1[ii]*(1-1./nbins1[ii]));}
					else if(sel_num1[ii] == "chiso4"){par1 = min((double)recchmuiso04[j],par_upp1[ii]*(1-1./nbins1[ii]));}
					else if(sel_num1[ii] == "dxy"){par1 = min(abs((double)recmudz[j]),par_upp1[ii]*(1-1./nbins1[ii]));}
					else if(sel_num1[ii] == "dz"){par1 = min(abs((double)recmudxy[j]),par_upp1[ii]*(1-1./nbins1[ii]));}
					//Choose the parameter to be filled for the eff.
					if(sel_num2[kk] == "tightId"){par1 = recmutightid[j];}
					else if(sel_num2[kk] == "reliso3"){par2 = min((double)recmuiso03[j],par_upp2[kk]*(2-1./nbins2[kk]));}
					else if(sel_num2[kk] == "reliso4"){par2 = min((double)recmuiso04[j],par_upp2[kk]*(1-1./nbins2[kk]));}
					else if(sel_num2[kk] == "chiso3"){par2 = min((double)recchmuiso03[j],par_upp2[kk]*(1-1./nbins2[kk]));}
					else if(sel_num2[kk] == "chiso4"){par2 = min((double)recchmuiso04[j],par_upp2[kk]*(1-1./nbins2[kk]));}
					else if(sel_num2[kk] == "dxy"){par2 = min(abs((double)recmudz[j]),par_upp2[kk]*(1-1./nbins2[kk]));}
					else if(sel_num2[kk] == "dz"){par2 = min(abs((double)recmudxy[j]),par_upp2[kk]*(1-1./nbins2[kk]));}

					//Fill Pt only for matched events
					if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){
						//Additional cut on the numerator
						histo[ii][kk]->Fill(par1,par2);
					}

					}
				}
				}
				}
				}}}}
			}}

	}

		}
	}

	for(int ii = 0; ii < narray; ++ii){
		for(int kk = 0; kk< ii; ++ kk){    

			//We are not interested into the correlation between 2x the same variable
			//if(sel_num1[ii] != sel_num2[kk]){

				//Efficiency of the iso cut.
				TCanvas* c1 = new TCanvas("c1","c1");
				histo[ii][kk]->Draw();
				histo[ii][kk]->GetXaxis()->SetTitle(_par1[ii]);
				histo[ii][kk]->GetYaxis()->SetTitle(_par2[kk]);
				histo[ii][kk]->SetTitle(_sel_den+" "+pname+" from "+_treename);
				histo[ii][kk]->SetMarkerStyle(20);
				histo[ii][kk]->SetMarkerSize(0.2);

				//histo->SetMarkerColor(4);
				//histo->SetLineColor(4);
				//histo->SetLineWidth(2);
				//histo->SetTitle(_sel_den+" "+pname+" "+treename);

				//Define the name of the canvas

				c1->SaveAs(_path+"/PDF/"+_canvas[ii][kk]+".pdf");

				c1->Write(_canvas[ii][kk]);


			//}
		}
	}

	output->Close();

	return 0;

}
