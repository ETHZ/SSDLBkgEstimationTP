/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : eff_plots2.C 

* Purpose : This study  the relative isolation efficiency for the second TTrees. The name of the branches is different than the previous trees and therefor this it is necessary to rewrite a new file.

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

void eff_plots2(TTree* tree, int leptonId, int mother_Id, Float_t Pt_cut = 10){

	bool tight = true;
	bool reliso03 = false;
	bool chargereliso = false;

	TString idname;
	if(tight == true){idname = "tightId";}
	else if(tight == false){idname = "no Id";}

	Long64_t n = tree->GetEntries();

	//Some initial variables
	int nbins = 10;
	int nbinseta = 120;
	//Cuts
	double iso_high = 0.1;
	double pile_up_low = 30;
	double pile_up_high= 40;

	//To choose electron, set to true. To choose muon, set to false.
	TString pname;
	TString treename;
	TString isoname;//("03");

	if (leptonId == 11) {pname = "e";}
	else if (leptonId == 13){pname = " #mu";}
	if (mother_Id == 23){treename = "from Z";}
	else if (mother_Id == 24){treename = "from W";}

	//Choose the cut on the isolation
	double isocut_high = 0.2;
	double isocut_med = 0.1;
	double isocut_low = 0.05;

	//String corresponding to the cut
	char high_cut[50];
	char low_cut[50];
	char med_cut[50];

	sprintf(high_cut,"%.2f",isocut_high);
	sprintf(low_cut,"%.2f",isocut_low);
	sprintf(med_cut,"%.2f",isocut_med);

	//Choose the rel. iso

	if(reliso03 == true){isoname = "0.3";}
	else if (reliso03 == false){isoname = "0.4";}

	//Declaration of histogram

	//efficiency of the isolation cut
	TH1D *histo_Pt_isocut_10 = new TH1D("histo_Pt_isocut_10","Pt",nbins,0,250);
	TH1D *histo_sel_Pt_isocut_10 = new TH1D("histo_sel_Pt_isocut_10","Pt",nbins,0, 250);
	TH1D *eff_isocut_10 = new TH1D("eff_isocut_10","eff_isocut_10", nbins,0,250);
	TH1D *histo_Pt_isocut_7= new TH1D("histo_Pt_isocut_7","histo_Pt_isocut_7",nbins,0,250);
	TH1D *histo_sel_Pt_isocut_7= new TH1D("histo_sel_Pt_isocut_7","histo_sel_Pt_isocut_7",nbins,0, 250);
	TH1D *eff_isocut_7= new TH1D("eff_isocut_7","eff_isocut_7", nbins,0,250);
	TH1D *histo_Pt_isocut_5= new TH1D("histo_Pt_isocut_5","histo_Pt_isocut_5",nbins,0,250);
	TH1D *histo_sel_Pt_isocut_5= new TH1D("histo_sel_Pt_isocut_5","histo_sel_Pt_isocut_5",nbins,0, 250);
	TH1D *eff_isocut_5= new TH1D("eff_isocut_5","eff_isocut_5", nbins,0,250);

	TH1D *histo_Pt_isocut_low_int_10 = new TH1D("histo_Pt_isocut_low_int_10","Pt",nbins,0,250);
	TH1D *histo_sel_Pt_isocut_low_int_10 = new TH1D("histo_sel_Pt_isocut_low_int_10","Pt",nbins,0, 250);
	TH1D *eff_isocut_low_int_10 = new TH1D("eff_isocut_low_int_10","Pt", nbins,0,250);
	TH1D *histo_Pt_isocut_low_int_7= new TH1D("histo_Pt_isocut_low_int_7","Pt",nbins,0,250);
	TH1D *histo_sel_Pt_isocut_low_int_7= new TH1D("histo_sel_Pt_isocut_low_int_7","Pt",nbins,0, 250);
	TH1D *eff_isocut_low_int_7= new TH1D("eff_isocut_low_int_7","Pt", nbins,0,250);
	TH1D *histo_Pt_isocut_low_int_5= new TH1D("histo_Pt_isocut_low_int_5","Pt",nbins,0,250);
	TH1D *histo_sel_Pt_isocut_low_int_5= new TH1D("histo_sel_Pt_isocut_low_int_5","Pt",nbins,0, 250);
	TH1D *eff_isocut_low_int_5= new TH1D("eff_isocut_low_int_5","Pt", nbins,0,250);

	TH1D *histo_Pt_isocut_med_int_10 = new TH1D("histo_Pt_isocut_med_int_10","Pt",nbins,0,250);
	TH1D *histo_sel_Pt_isocut_med_int_10 = new TH1D("histo_sel_Pt_isocut_med_int_10","Pt",nbins,0, 250);
	TH1D *eff_isocut_med_int_10 = new TH1D("eff_isocut_med_int_10","Pt", nbins,0,250);
	TH1D *histo_Pt_isocut_med_int_7= new TH1D("histo_Pt_isocut_med_int_7","Pt",nbins,0,250);
	TH1D *histo_sel_Pt_isocut_med_int_7= new TH1D("histo_sel_Pt_isocut_med_int_7","Pt",nbins,0, 250);
	TH1D *eff_isocut_med_int_7= new TH1D("eff_isocut_med_int_7","Pt", nbins,0,250);
	TH1D *histo_Pt_isocut_med_int_5= new TH1D("histo_Pt_isocut_med_int_5","Pt",nbins,0,250);
	TH1D *histo_sel_Pt_isocut_med_int_5= new TH1D("histo_sel_Pt_isocut_med_int_5","Pt",nbins,0, 250);
	TH1D *eff_isocut_med_int_5= new TH1D("eff_isocut_med_int_5","Pt", nbins,0,250);

	TH1D *histo_Pt_isocut_high_int_10 = new TH1D("histo_Pt_isocut_high_int_10","Pt",nbins,0,250);
	TH1D *histo_sel_Pt_isocut_high_int_10 = new TH1D("histo_sel_Pt_isocut_high_int_10","Pt",nbins,0, 250);
	TH1D *eff_isocut_high_int_10 = new TH1D("eff_isocut_high_int_10","Pt", nbins,0,250);
	TH1D *histo_Pt_isocut_high_int_7= new TH1D("histo_Pt_isocut_high_int_7","Pt",nbins,0,250);
	TH1D *histo_sel_Pt_isocut_high_int_7= new TH1D("histo_sel_Pt_isocut_high_int_7","Pt",nbins,0, 250);
	TH1D *eff_isocut_high_int_7= new TH1D("eff_isocut_high_int_7","Pt", nbins,0,250);
	TH1D *histo_Pt_isocut_high_int_5= new TH1D("histo_Pt_isocut_high_int_5","Pt",nbins,0,250);
	TH1D *histo_sel_Pt_isocut_high_int_5= new TH1D("histo_sel_Pt_isocut_high_int_5","Pt",nbins,0, 250);
	TH1D *eff_isocut_high_int_5= new TH1D("eff_isocut_high_int_5","Pt", nbins,0,250);

	//efficiency for different number of interaction (using the Branch nTrueInt)
	TH1D *histo_Pt_low_int = new TH1D("histo_Pt_low_int","Pt",nbins,0,250);
	TH1D *histo_eta_low_int = new TH1D("histo_eta_low_int","eta",nbinseta,-6,6);
	TH1D *histo_phi_low_int = new TH1D("histo_phi_low_int","phi",nbins,-4,4);
	TH1D *histo_sel_Pt_low_int = new TH1D("histo_sel_Pt_low_int","Selected Pt",nbins,0,250);
	TH1D *histo_sel_eta_low_int = new TH1D("histo_sel_eta_low_int","eta",nbinseta,-6,6);
	TH1D *histo_sel_phi_low_int = new TH1D("histo_sel_phi_low_int","phi",nbins,-4,4);
	TH1D *eff_Pt_low_int = new TH1D("eff_Pt_low_int","Efficiency Pt",nbins,0,250);
	TH1D *eff_eta_low_int = new TH1D("eff_eta_low_int","eta",nbinseta,-6,6);
	TH1D *eff_phi_low_int = new TH1D("eff_phi_low_int","phi",nbins,-4,4);

	TH1D *histo_Pt_med_int = new TH1D("histo_Pt_med_int","Pt",nbins,0,250);
	TH1D *histo_eta_med_int = new TH1D("histo_eta_med_int","eta",nbinseta,-6,6);
	TH1D *histo_phi_med_int = new TH1D("histo_phi_med_int","phi",nbins,-4,4);
	TH1D *histo_sel_Pt_med_int = new TH1D("histo_sel_Pt_med_int","Selected Pt",nbins,0,250);
	TH1D *histo_sel_eta_med_int = new TH1D("histo_sel_eta_med_int","eta",nbinseta,-6,6);
	TH1D *histo_sel_phi_med_int = new TH1D("histo_sel_phi_med_int","phi",nbins,-4,4);
	TH1D *eff_Pt_med_int = new TH1D("eff_Pt_med_int","Efficiency Pt",nbins,0,250);
	TH1D *eff_eta_med_int = new TH1D("eff_eta_med_int","eta",nbinseta,-6,6);
	TH1D *eff_phi_med_int = new TH1D("eff_phi_med_int","phi",nbins,-4,4);

	TH1D *histo_Pt_high_int = new TH1D("histo_Pt_high_int","Pt",nbins,0,250);
	TH1D *histo_eta_high_int = new TH1D("histo_eta_high_int","eta",nbinseta,-6,6);
	TH1D *histo_phi_high_int = new TH1D("histo_phi_high_int","phi",nbins,-4,4);
	TH1D *histo_sel_Pt_high_int = new TH1D("histo_sel_Pt_high_int","Selected Pt",nbins,0,250);
	TH1D *histo_sel_eta_high_int = new TH1D("histo_sel_eta_high_int","eta",nbinseta,-6,6);
	TH1D *histo_sel_phi_high_int = new TH1D("histo_sel_phi_high_int","phi",nbins,-4,4);
	TH1D *eff_Pt_high_int = new TH1D("eff_Pt_high_int","Efficiency Pt",nbins,0,250);
	TH1D *eff_eta_high_int = new TH1D("eff_eta_high_int","eta",nbinseta,-6,6);
	TH1D *eff_phi_high_int = new TH1D("eff_phi_high_int","phi",nbins,-4,4);

	//efficiency of the selection
	TH1D* histo_Pt_tighId = new TH1D ("histo_Pt_tighId","Pt",nbins,0,250);
	TH1D* histo_sel_Pt_tighId = new TH1D ("histo_sel_Pt_tighId","Pt",nbins,0,250);
	TH1D* eff_Pt_tighId = new TH1D ("eff_Pt_tighId","Pt",nbins,0,250);

	//Event variables
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
	if(chargereliso == false){tree->SetBranchAddress("el_relIso03", &receliso03);
	tree->SetBranchAddress("el_relIso04", &receliso04);}
	else if(chargereliso == true){tree->SetBranchAddress("el_chargedHadRelIso03", &receliso03);
	tree->SetBranchAddress("el_chargedHadRelIso04", &receliso04);}
	//muons
	tree->SetBranchAddress("nmu", &nrecmu);
	tree->SetBranchAddress("mu_tightId", &recmutightid);
	tree->SetBranchAddress("mu_pt", &recmuPt);
	tree->SetBranchAddress("mu_mass", &recmum);
	tree->SetBranchAddress("mu_eta", &recmu_eta);
	tree->SetBranchAddress("mu_phi", &recmu_phi);
	tree->SetBranchAddress("mu_charge", &recmucharge);
	if(chargereliso == false){tree->SetBranchAddress("mu_relIso03", &recmuiso03);
	tree->SetBranchAddress("mu_relIso04", &recmuiso04);}
	else if(chargereliso == true){tree->SetBranchAddress("mu_chargedHadRelIso03", &recmuiso03);
	tree->SetBranchAddress("mu_chargedHadRelIso04", &recmuiso04);}

	//Start loop over all events

	for (int k = 0; k < n; ++k) {

		//cout<<"Event number :"<<k<<endl;

		tree->GetEntry(k);

		//loop over all generated particles
		for (int i = 0; i < ngenPart; ++i) {


			if((abs(Id[i]) == leptonId)&&(status[i]== 1)&&(abs(gen_eta[i]) < 2.4)&&(abs(source[i]) == mother_Id)&&(Pt[i]>Pt_cut)){ 

				//Some variables to plot
				double R = 999;
				double R2;
				double delta_P = 999;
				double delta_charge;
				double iso3 = 999;
				double iso4 = 999;
				double recPt = 999;
				int id = 999;

				//Loop on reconstructed events to minimize DeltaR and do the matching
				if((leptonId == 11)){

					for (int j = 0; j<nrecel; ++j) {
						if((tight == false)||((tight == true)&&(receltightid[j]==1))){

							//Electrons selection

							R2 = DeltaR(gen_eta[i],recel_eta[j],gen_phi[i],recel_phi[j] );

							//Minimise DeltaR and Fill the other variables
							if (R > R2) {

								R = R2;
								delta_P = abs(recelPt[j]-Pt[i])/Pt[i];
								delta_charge = abs(recelcharge[j] - charge[i]);
								iso3 = receliso03[j];
								iso4 = receliso04[j];
								recPt = recelPt[j];//Pt[i];
								id = receltightid[j];

							}
						}
					}   
				}
				

				if((leptonId == 13)){

					for (int j = 0; j<nrecmu; ++j) {
						if((tight == false)||((tight == true)&&(recmutightid[j]==1))){

							//Electrons selection

							R2 = DeltaR(gen_eta[i],recmu_eta[j],gen_phi[i],recmu_phi[j] );

							//Minimise DeltaR and Fill the other variables
							if (R > R2) {

								R = R2;
								delta_P = abs(recmuPt[j]-Pt[i])/Pt[i];
								delta_charge = abs(recmucharge[j] - charge[i]);
								iso3 = recmuiso03[j];
								iso4 = recmuiso04[j];
								recPt = recmuPt[j];//Pt[i];
								id = recmutightid[j];

							}
						}
					}

				}   

				//Histogram for different pile-up range
				if((pile_up <  pile_up_low)){

					histo_Pt_low_int->Fill(recPt);
					histo_eta_low_int->Fill(gen_eta[i]);
					histo_phi_low_int->Fill(gen_phi[i]);

				}

				if((pile_up >  pile_up_low)&&(pile_up < pile_up_high)){

					histo_Pt_med_int->Fill(recPt);
					histo_eta_med_int->Fill(gen_eta[i]);
					histo_phi_med_int->Fill(gen_phi[i]);

				}

				if((pile_up > pile_up_high)){
					histo_Pt_high_int->Fill(recPt);
					histo_eta_high_int->Fill(gen_eta[i]);
					histo_phi_high_int->Fill(gen_phi[i]);

				}                

				//Fill Pt only if the selection criteria is satisfied
				if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){

					//Filling the histogram for the id. efficiency
					histo_Pt_tighId->Fill(recPt);
					if(id==1){
						histo_sel_Pt_tighId->Fill(recPt);}
					//Fill histogram with the cut on the isolation only
					histo_Pt_isocut_10->Fill(recPt);
					histo_Pt_isocut_7->Fill(recPt);
					histo_Pt_isocut_5->Fill(recPt);

					if(reliso03 == true){
						if(iso3 < isocut_high){histo_sel_Pt_isocut_10->Fill(recPt);}
						if(iso3 < isocut_med){histo_sel_Pt_isocut_7->Fill(recPt);}
						if(iso3 < isocut_low){histo_sel_Pt_isocut_5->Fill(recPt);}
					}else if(reliso03 == false){
						if(iso4 < isocut_high){histo_sel_Pt_isocut_10->Fill(recPt);}
						if(iso4 < isocut_med){histo_sel_Pt_isocut_7->Fill(recPt);}
						if(iso4 < isocut_low){histo_sel_Pt_isocut_5->Fill(recPt);}
					}

					//Fill the histograms for different number of Interaction and resolution
					//Fill histogram for different interaction range
					if((pile_up <  pile_up_low)){

						histo_sel_Pt_low_int->Fill(recPt);
						histo_sel_eta_low_int->Fill(gen_eta[i]);
						histo_sel_phi_low_int->Fill(gen_phi[i]);

						histo_Pt_isocut_low_int_10->Fill(recPt);
						histo_Pt_isocut_low_int_7->Fill(recPt);
						histo_Pt_isocut_low_int_5->Fill(recPt);

						if(reliso03 == true){
							if(iso3 < isocut_high){histo_sel_Pt_isocut_low_int_10->Fill(recPt);}
							if(iso3 < isocut_med){histo_sel_Pt_isocut_low_int_7->Fill(recPt);}
							if(iso3 < isocut_low){histo_sel_Pt_isocut_low_int_5->Fill(recPt);}
						} else if (reliso03 == false){
							if(iso4 < isocut_high){histo_sel_Pt_isocut_low_int_10->Fill(recPt);}
							if(iso4 < isocut_med){histo_sel_Pt_isocut_low_int_7->Fill(recPt);}
							if(iso4 < isocut_low){histo_sel_Pt_isocut_low_int_5->Fill(recPt);}

						}

					}                        		

					if((pile_up >  pile_up_low)&&(pile_up < pile_up_high)){

						histo_sel_Pt_med_int->Fill(recPt);
						histo_sel_eta_med_int->Fill(gen_eta[i]);
						histo_sel_phi_med_int->Fill(gen_phi[i]);

						histo_Pt_isocut_med_int_10->Fill(recPt);
						histo_Pt_isocut_med_int_7->Fill(recPt);
						histo_Pt_isocut_med_int_5->Fill(recPt);

						if(reliso03 == true){
							if(iso3 < isocut_high){histo_sel_Pt_isocut_med_int_10->Fill(recPt);}
							if(iso3 < isocut_med){histo_sel_Pt_isocut_med_int_7->Fill(recPt);}
							if(iso3 < isocut_low){histo_sel_Pt_isocut_med_int_5->Fill(recPt);}
						}else if(reliso03 == false){
							if(iso4 < isocut_high){histo_sel_Pt_isocut_med_int_10->Fill(recPt);}
							if(iso4 < isocut_med){histo_sel_Pt_isocut_med_int_7->Fill(recPt);}
							if(iso4 < isocut_low){histo_sel_Pt_isocut_med_int_5->Fill(recPt);}
						}
					}

					if((pile_up > pile_up_high)){

						histo_sel_Pt_high_int->Fill(recPt);
						histo_sel_eta_high_int->Fill(gen_eta[i]);
						histo_sel_phi_high_int->Fill(gen_phi[i]);

						histo_Pt_isocut_high_int_10->Fill(recPt);
						histo_Pt_isocut_high_int_7->Fill(recPt);
						histo_Pt_isocut_high_int_5->Fill(recPt);

						if(reliso03 == true){
							if(iso3 < isocut_high){histo_sel_Pt_isocut_high_int_10->Fill(recPt);}
							if(iso3 < isocut_med){histo_sel_Pt_isocut_high_int_7->Fill(recPt);}
							if(iso3 < isocut_low){histo_sel_Pt_isocut_high_int_5->Fill(recPt);}
						}else if(reliso03 == false){
							if(iso4 < isocut_high){histo_sel_Pt_isocut_high_int_10->Fill(recPt);}
							if(iso4 < isocut_med){histo_sel_Pt_isocut_high_int_7->Fill(recPt);}
							if(iso4 < isocut_low){histo_sel_Pt_isocut_high_int_5->Fill(recPt);}
						}
					}
				}
			}
		}
	}



	histo_sel_Pt_low_int->Sumw2();
	histo_Pt_low_int->Sumw2();
	histo_sel_eta_low_int->Sumw2();
	histo_eta_low_int->Sumw2();
	histo_sel_phi_low_int->Sumw2();
	histo_phi_low_int->Sumw2();

	histo_sel_Pt_med_int->Sumw2();
	histo_Pt_med_int->Sumw2();
	histo_sel_eta_med_int->Sumw2();
	histo_eta_med_int->Sumw2();
	histo_sel_phi_med_int->Sumw2();
	histo_phi_med_int->Sumw2();

	histo_sel_Pt_high_int->Sumw2();
	histo_Pt_high_int->Sumw2();
	histo_sel_eta_high_int->Sumw2();
	histo_eta_high_int->Sumw2();
	histo_sel_phi_high_int->Sumw2();
	histo_phi_high_int->Sumw2();

	histo_sel_Pt_isocut_10->Sumw2();
	histo_sel_Pt_isocut_7->Sumw2();
	histo_sel_Pt_isocut_5->Sumw2();
	histo_Pt_isocut_10->Sumw2();
	histo_Pt_isocut_7->Sumw2();
	histo_Pt_isocut_5->Sumw2();

	histo_Pt_isocut_low_int_10->Sumw2();
	histo_Pt_isocut_low_int_7->Sumw2();
	histo_Pt_isocut_low_int_5->Sumw2();
	histo_Pt_isocut_med_int_10->Sumw2();
	histo_Pt_isocut_med_int_7->Sumw2();
	histo_Pt_isocut_med_int_5->Sumw2();
	histo_Pt_isocut_high_int_10->Sumw2();
	histo_Pt_isocut_high_int_7->Sumw2();
	histo_Pt_isocut_high_int_5->Sumw2();

	histo_sel_Pt_tighId->Sumw2();


	//Divide histograms to get the efficiency
	eff_isocut_10->Divide(histo_sel_Pt_isocut_10, histo_Pt_isocut_10,1,1,"B"); 
	eff_isocut_7->Divide(histo_sel_Pt_isocut_7, histo_Pt_isocut_7,1,1,"B"); 
	eff_isocut_5->Divide(histo_sel_Pt_isocut_5, histo_Pt_isocut_5,1,1,"B"); 
	eff_Pt_tighId->Divide(histo_sel_Pt_tighId, histo_Pt_tighId,1,1,"B");

	//efficiency for interaction events
	//just interaction
	eff_Pt_low_int->Divide(histo_sel_Pt_low_int, histo_Pt_low_int,1,1,"B");
	eff_eta_low_int->Divide(histo_sel_eta_low_int, histo_eta_low_int,1,1,"B");
	eff_phi_low_int->Divide(histo_sel_phi_low_int, histo_phi_low_int,1,1,"B");
	eff_Pt_med_int->Divide(histo_sel_Pt_med_int, histo_Pt_med_int,1,1,"B");
	eff_eta_med_int->Divide(histo_sel_eta_med_int, histo_eta_med_int,1,1,"B");
	eff_phi_med_int->Divide(histo_sel_phi_med_int, histo_phi_med_int,1,1,"B");
	eff_Pt_high_int->Divide(histo_sel_Pt_high_int, histo_Pt_high_int,1,1,"B");
	eff_eta_high_int->Divide(histo_sel_eta_high_int, histo_eta_high_int,1,1,"B");
	eff_phi_high_int->Divide(histo_sel_phi_high_int, histo_phi_high_int,1,1,"B");

	//interaction and isolation
	eff_isocut_low_int_10->Divide(histo_sel_Pt_isocut_low_int_10, histo_Pt_isocut_low_int_10,1,1,"B");
	eff_isocut_low_int_7->Divide(histo_sel_Pt_isocut_low_int_7, histo_Pt_isocut_low_int_7,1,1,"B");
	eff_isocut_low_int_5->Divide(histo_sel_Pt_isocut_low_int_5, histo_Pt_isocut_low_int_5,1,1,"B");
	eff_isocut_med_int_10->Divide(histo_sel_Pt_isocut_med_int_10, histo_Pt_isocut_med_int_10,1,1,"B");
	eff_isocut_med_int_7->Divide(histo_sel_Pt_isocut_med_int_7, histo_Pt_isocut_med_int_7,1,1,"B");
	eff_isocut_med_int_5->Divide(histo_sel_Pt_isocut_med_int_5, histo_Pt_isocut_med_int_5,1,1,"B");
	eff_isocut_high_int_10->Divide(histo_sel_Pt_isocut_high_int_10, histo_Pt_isocut_high_int_10,1,1,"B");
	eff_isocut_high_int_7->Divide(histo_sel_Pt_isocut_high_int_7, histo_Pt_isocut_high_int_7,1,1,"B");
	eff_isocut_high_int_5->Divide(histo_sel_Pt_isocut_high_int_5, histo_Pt_isocut_high_int_5,1,1,"B");

	//eff for histogram on isolation
	eff_isocut_10->SetLineWidth(2);
	eff_isocut_10->SetTitle("#epsilon for iso < 10");
	eff_isocut_10->GetXaxis()->SetTitle("P_{t}");
	eff_isocut_7->SetLineWidth(2);
	eff_isocut_7->SetTitle("#epsilon for iso < 7");
	eff_isocut_7->GetXaxis()->SetTitle("P_{t}");
	eff_isocut_5->SetLineWidth(2);
	eff_isocut_5->SetTitle("#epsilon for iso < 5");
	eff_isocut_5->GetXaxis()->SetTitle("P_{t}");


	//gStyle->SetOptStat(0);

	//Efficiency of the iso cut.
	TCanvas* c1 = new TCanvas("c1","c1");
	eff_isocut_10->Draw();
	eff_isocut_10->GetYaxis()->SetRangeUser(0,1.1);
	eff_isocut_10->SetMarkerStyle(4);
	eff_isocut_10->SetMarkerSize(0.4);
	eff_isocut_10->SetMarkerColor(1);
	eff_isocut_10->SetLineColor(1);
	eff_isocut_10->SetTitle("Rel. Iso. eff. for "+pname+" "+treename+" "+idname);
	eff_isocut_7->Draw("same");
	eff_isocut_7->SetMarkerStyle(4);
	eff_isocut_7->SetMarkerSize(0.4);
	eff_isocut_7->SetMarkerColor(2);
	eff_isocut_7->SetLineColor(2);
	eff_isocut_5->Draw("same");
	eff_isocut_5->SetMarkerStyle(4);
	eff_isocut_5->SetMarkerSize(0.4);
	eff_isocut_5->SetMarkerColor(4);
	eff_isocut_5->SetLineColor(4);
	TLegend* leg2 = new TLegend(0.55, 0.2, 0.75 ,0.4);
	leg2->AddEntry(eff_isocut_10, "rel. iso"+isoname+"  #leq "+ (TString)high_cut);  
	leg2->AddEntry(eff_isocut_7, "rel. iso"+isoname+"  #leq "+ (TString)med_cut);
	leg2->AddEntry(eff_isocut_5, "rel. iso test"+isoname+"  #leq "+(TString)low_cut);
	leg2->Draw();

	TCanvas* c2 = new TCanvas("c2","c2");   
	eff_isocut_low_int_10->Draw();
	eff_isocut_low_int_10->GetYaxis()->SetRangeUser(0,1.1);
	eff_isocut_low_int_10->GetXaxis()->SetTitle("P_{t}");
	eff_isocut_low_int_10->SetTitle("#epsilon of "+pname+" for rel. iso"+isoname+"  #leq "+ (TString)high_cut+" from "+treename+" "+idname);
	eff_isocut_low_int_10->SetMarkerStyle(4);
	eff_isocut_low_int_10->SetMarkerSize(0.4);
	eff_isocut_low_int_10->SetMarkerColor(2);
	eff_isocut_low_int_10->SetLineColor(2);
	eff_isocut_med_int_10->Draw("same");
	eff_isocut_med_int_10->SetMarkerStyle(4);
	eff_isocut_med_int_10->SetMarkerSize(0.4);
	eff_isocut_med_int_10->SetMarkerColor(3);
	eff_isocut_med_int_10->SetLineColor(3);
	eff_isocut_high_int_10->Draw("same");
	eff_isocut_high_int_10->SetMarkerStyle(4);
	eff_isocut_high_int_10->SetMarkerSize(0.4);
	eff_isocut_high_int_10->SetMarkerColor(4);
	eff_isocut_high_int_10->SetLineColor(4);
	TLegend* leg3 = new TLegend(0.55, 0.2, 0.75 ,0.4);
	leg3->AddEntry(eff_isocut_low_int_10, "inter.  #leq  30");
	leg3->AddEntry(eff_isocut_med_int_10, "30  #geq inter.  #leq 40");
	leg3->AddEntry(eff_isocut_high_int_10, "40  #leq  inter. ");
	leg3->Draw();
	TCanvas* c3 = new TCanvas("c3","c3");   
	eff_isocut_low_int_7->Draw();
	eff_isocut_low_int_7->GetYaxis()->SetRangeUser(0,1.1);
	eff_isocut_low_int_7->GetXaxis()->SetTitle("P_{t}");
	eff_isocut_low_int_7->SetTitle("#epsilon of "+pname+" for rel. iso"+isoname+"  #leq "+ (TString)med_cut+" from "+treename+" "+idname);
	eff_isocut_low_int_7->SetMarkerStyle(4);
	eff_isocut_low_int_7->SetMarkerSize(0.4);
	eff_isocut_low_int_7->SetMarkerColor(2);
	eff_isocut_low_int_7->SetLineColor(2);
	eff_isocut_med_int_7->Draw("same");
	eff_isocut_med_int_7->SetMarkerStyle(4);
	eff_isocut_med_int_7->SetMarkerSize(0.4);
	eff_isocut_med_int_7->SetMarkerColor(3);
	eff_isocut_med_int_7->SetLineColor(3);
	eff_isocut_high_int_7->Draw("same");
	eff_isocut_high_int_7->SetMarkerStyle(4);
	eff_isocut_high_int_7->SetMarkerSize(0.4);
	eff_isocut_high_int_7->SetMarkerColor(4);
	eff_isocut_high_int_7->SetLineColor(4);
	leg3->Draw();
	TCanvas* c4 = new TCanvas("c4","c4");   
	eff_isocut_low_int_5->Draw();
	eff_isocut_low_int_5->GetYaxis()->SetRangeUser(0,1.1);
	eff_isocut_low_int_5->GetXaxis()->SetTitle("P_{t}");
	eff_isocut_low_int_5->SetTitle("#epsilon of "+pname+" for rel. iso"+isoname+"  #leq "+ (TString)low_cut+" from "+treename+" "+idname);
	eff_isocut_low_int_5->SetMarkerStyle(4);
	eff_isocut_low_int_5->SetMarkerSize(0.4);
	eff_isocut_low_int_5->SetMarkerColor(2);
	eff_isocut_low_int_5->SetLineColor(2);
	eff_isocut_med_int_5->Draw("same");
	eff_isocut_med_int_5->SetMarkerStyle(4);
	eff_isocut_med_int_5->SetMarkerSize(0.4);
	eff_isocut_med_int_5->SetMarkerColor(3);
	eff_isocut_med_int_5->SetLineColor(3);
	eff_isocut_high_int_5->Draw("same");
	eff_isocut_high_int_5->SetMarkerStyle(4);
	eff_isocut_high_int_5->SetMarkerSize(0.4);
	eff_isocut_high_int_5->SetMarkerColor(4);
	eff_isocut_high_int_5->SetLineColor(4);
	leg3->Draw();

	TCanvas* c5 = new TCanvas("c5","c5");
	eff_Pt_low_int->Draw();
	eff_Pt_low_int->GetYaxis()->SetRangeUser(0,1.1);
	eff_Pt_low_int->SetTitle("Efficiency for "+pname+" from "+treename+" "+idname);
	eff_Pt_low_int->GetXaxis()->SetTitle("P_{t}");
	eff_Pt_low_int->SetMarkerStyle(4);
	eff_Pt_low_int->SetMarkerSize(0.4);
	eff_Pt_low_int->SetMarkerColor(2);
	eff_Pt_low_int->SetLineColor(2);
	eff_Pt_med_int->Draw("same");
	eff_Pt_med_int->SetMarkerStyle(4);
	eff_Pt_med_int->SetMarkerSize(0.4);
	eff_Pt_med_int->SetMarkerColor(3);
	eff_Pt_med_int->SetLineColor(3);
	eff_Pt_high_int->Draw("same");
	eff_Pt_high_int->SetMarkerStyle(4);
	eff_Pt_high_int->SetMarkerSize(0.4);
	eff_Pt_high_int->SetMarkerColor(4);
	eff_Pt_high_int->SetLineColor(4);
	TLegend* leg4 = new TLegend(0.55, 0.2, 0.75 ,0.4);
	leg4->AddEntry(eff_Pt_low_int, "inter.  #leq  30");
	leg4->AddEntry(eff_Pt_med_int, "30  #geq inter.  #leq 40");
	leg4->AddEntry(eff_Pt_high_int, "40  #leq  inter. ");
	leg4->Draw();

	//Efficiency of the tight ID 
	TCanvas* c6 = new TCanvas("c6","c6");
	eff_Pt_tighId->Draw();
	eff_Pt_tighId->GetYaxis()->SetRangeUser(0,1.1);
	eff_Pt_tighId->GetXaxis()->SetTitle("P_{t}");
	eff_Pt_tighId->SetLineWidth(2);
	eff_Pt_tighId->SetMarkerStyle(4);
	eff_Pt_tighId->SetMarkerSize(0.4);
	eff_Pt_tighId->SetMarkerColor(4);
	eff_Pt_tighId->SetLineColor(4);
	eff_Pt_tighId->SetTitle("Id. eff. for "+pname+" "+treename);


}
