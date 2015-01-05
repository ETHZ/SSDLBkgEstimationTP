/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : Efficiency_v2.C 

* Purpose : Plot the efficiency for a given cut that is applied to matched reconstruct data.
            This code use array to generate all the plot at in one single run over all the entries.
 
* DataSet:  2

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By : Gaël L. Perrin

_._._._._._._._._._._._._._._._._._._._._.*/

#include "cmath"
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

int Efficiency_v2(TTree* tree, int narray, int* leptonId, int* mother_Id, TString* sel_den , TString* sel_num, TString* par_x, double* cut_den, double* cut_num){

setTDRStyle();

Long64_t n = tree->GetEntries();

//Path for input and output file. Written in FitDataPath.txt
ifstream file("EfficiencyPath.txt");
string str;
getline(file,str);
TString _path = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/efficiencys/MCratio/";

//Histogram parameter
int nbins[narray];
double par_low[narray];
double par_upp[narray];

//Declaration of histogram
//
//efficiency of the isolation cut
TH1D **histo_num= new TH1D*[narray];
TH1D **histo_den= new TH1D*[narray];
//efficiency of the selection
TH1D **eff = new TH1D*[narray];

//Name for storing and final plots
//
TString* pname = new TString[narray];
TString* _pname = new TString[narray];
TString* treename = new TString[narray];
TString* _par = new TString[narray];
TString* _sel_num = new TString[narray];
TString* _sel_den = new TString[narray];
TString* _cut_num = new TString[narray];
TString* _cut_den = new TString[narray];
TString* _mother_Id = new TString[narray];

//Name of the output
TString _output; 
_output = _path+"eff.root";
TFile* output = new TFile(_output,"recreate");


for(int i =0; i < narray; ++i){

//Set parameter range
if(par_x[i] == "Pt"){nbins[i] = 24;par_low[i] = 10;par_upp[i] = 250;}
else if(par_x[i]== "eta"){nbins[i] = 24;par_low[i] = -2.4;par_upp[i] = 2.4;}
else if(par_x[i] == "phi"){nbins[i] = 24;par_low[i] = -3.2;par_upp[i] = 3.2;}


//Writing string
if(mother_Id[i] == 23){_mother_Id[i] = "Z";}
else if(mother_Id[i] == 24){_mother_Id[i] = "W";}
if (leptonId[i] == 11) {pname[i] = "e";_pname[i] = "e";}
else if (leptonId[i] == 13){pname[i] = " #mu";_pname[i] = "mu";}
if (mother_Id[i] == 23){treename[i] = "from Z";}
else if (mother_Id[i] == 24){treename[i] = "from W";}
if(par_x[i] == "Pt"){_par[i] = "P_{t}";}
else if(par_x[i] == "eta"){_par[i] = "#eta";}
else if(par_x[i] == "phi"){_par[i] = "#phi";}
else{cout<<"ERROR: wrong parameter name !";return 1;}
if(sel_num[i] == ""){_sel_num[i] = "unsel";}
	else if(sel_num[i] == "tightId"){_sel_num[i] = "tightId";}
	else if(sel_num[i] == "dz"){_sel_num[i] = Form("IP dz < %0.3lf ",cut_num[i]);}
	else if(sel_num[i] == "dxy"){_sel_num[i] = Form("IP dxy < %0.3lf ", cut_num[i]) ;}
	else{cout<<"ERROR: wrong numeretor selection name !";return 1;}
	if(sel_den[i] == ""){_sel_den[i] = "unsel";}
	else if(sel_den[i] == "tightId"){_sel_den[i] = "tight";}
	else{cout<<"ERROR: wrong denominator selection name !";return 1;}
	if(cut_den[i] == 0.){_cut_den[i] = "";}
	else{_cut_den[i] = Form("%0.3lf",cut_den[i]);}
	if(cut_num[i] == 0.){_cut_num[i] = "";}
	else{_cut_num[i] = Form("%0.3lf",cut_num[i]);}

	//Assigning histo
	histo_num[i] = new TH1D("histo_num","Pt",nbins[i],par_low[i],par_upp[i]);
	histo_den[i] = new TH1D("histo_den","Pt",nbins[i],par_low[i],par_upp[i]);
	eff[i] = new TH1D("histo_eff","Pt",nbins[i],par_low[i],par_upp[i]);


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
	tree->SetBranchAddress("el_chargedHadRelIso03", &receliso03);
	tree->SetBranchAddress("el_chargedHadRelIso04", &receliso04);
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
	tree->SetBranchAddress("mu_chargedHadRelIso03", &recmuiso03);
	tree->SetBranchAddress("mu_chargedHadRelIso04", &recmuiso04);
	tree->SetBranchAddress("mu_dz",&recmudz);
	tree->SetBranchAddress("mu_dxy",&recmudxy);
	
	int counter = 1;
	//Start loop over all events
	for (int k = 0; k < n; ++k) {

		tree->GetEntry(k);

		for(int ii = 0; ii <narray; ++ii){  

		if(k%n > counter*100000){cout<<100.*k/n<<" completed"<<endl;++counter;}

		if(leptonId[ii] == 11){
			for(int j=0; j<nrecel;++j){
				//Cut on the denominator
				if((sel_den[ii] != "tightId")||((sel_den[ii] == "tightId")&&(receltightid[j] == 1 ))){
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
							if((abs(Id[i]) == leptonId[ii])&&(status[i]== 1)&&(abs(gen_eta[i]) < 2.4)&&(abs(source[i]) == mother_Id[ii])){ 

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
					if(par_x[ii] == "Pt"){par = recelPt[j];}
					else if(par_x[ii] == "eta"){par = recel_eta[j];}
					else if(par_x[ii] == "phi"){par = recel_phi[j];}

					//Fill Pt only for matched events
					if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){
						//Filling the den
						histo_den[ii]->Fill(par);

						//Additional cut on the numerator
						if((sel_num[ii] != "tightId")||((sel_num[ii] == "tightId")&&(receltightid[j] == 1 ))){
							if((sel_num[ii] != "dxy")||((sel_num[ii] == "dxy")&&(abs(receldxy[j]) < cut_num[ii] ))){
								if((sel_num[ii] != "dz")||((sel_num[ii] == "dz")&&(abs(receldz[j]) < cut_num[ii] ))){
									histo_num[ii]->Fill(par);
								}
							}
						}
					}
					}
				}
			}
		}

		if(leptonId[ii] == 13){
			for(int j=0; j<nrecmu;++j){
				//Cut on the denominator
				if((sel_den[ii] != "tightId")||((sel_den[ii] == "tightId")&&(recmutightid[j] == 1 ))){
					//Veto the EE-EB gape
					if(((abs(recmu_eta[j]) < 1.479)||(abs(recmu_eta[j]) > 1.653))){

					//Variable for matching
					double R = 999;
					double delta_P = 999;
					double delta_charge = 999;

					//Parameter on the xaxis

					double par;

					//if(abs(recmu_eta[j]) < 2.4){ 
						//loop over all generated particles to do the matching
						for (int i = 0; i < ngenPart; ++i) {
							if((abs(Id[i]) == leptonId[ii])&&(status[i]== 1)&&(abs(gen_eta[i]) < 2.4)&&(abs(source[i]) == mother_Id[ii])){ 

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
					if(par_x[ii] == "Pt"){par = recmuPt[j];}
					else if(par_x[ii] == "eta"){par = recmu_eta[j];}
					else if(par_x[ii] == "phi"){par = recmu_phi[j];}

					//Fill Pt only for matched events
					if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){
						//Filling the den
						histo_den[ii]->Fill(par);

						//Additional cut on the numerator
						if((sel_num[ii] != "tightId")||((sel_num[ii] == "tightId")&&(recmutightid[j] == 1 ))){
							if((sel_num[ii] != "dxy")||((sel_num[ii] == "dxy")&&(abs(recmudxy[j]) < cut_num[ii] ))){
								if((sel_num[ii] != "dz")||((sel_num[ii] == "dz")&&(abs(recmudz[j]) < cut_num[ii] ))){
									histo_num[ii]->Fill(par);
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
	for(int ii = 0; ii < narray;++ii){


	histo_num[ii]->Sumw2();
	histo_den[ii]->Sumw2();

	//Divide histograms to get the efficiency
	eff[ii]->Divide(histo_num[ii],histo_den[ii],1,1,"B"); 

	//Efficiency of the iso cut.
	TCanvas* c1 = new TCanvas("c1","c1");
	eff[ii]->Draw();
	eff[ii]->GetYaxis()->SetRangeUser(0,1.1);
	eff[ii]->GetYaxis()->SetTitle("#epsilon");
	eff[ii]->GetXaxis()->SetTitle(_par[ii]);
	eff[ii]->SetMarkerStyle(20);
	eff[ii]->SetMarkerSize(1);
	eff[ii]->SetMarkerColor(4);
	eff[ii]->SetLineColor(4);
	eff[ii]->SetTitle(_sel_num[ii]+" for "+_sel_den[ii]+" "+pname[ii]+" "+treename[ii]);
	
	//Define the name of the canvas
	TString cname = (TString)"eff_"+_pname[ii]+"_"+treename[ii]+"_den_"+_sel_den[ii]+"_num_"+_sel_num[ii]+"_"+par_x[ii];
	
	c1->SaveAs(_path+"PDF/"+cname+".pdf");
	output->cd();
	c1->Write(cname);
	}

	output->Close();
}
