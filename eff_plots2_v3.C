/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : eff_plots2_v3.C 

* Purpose : This study  the relative isolation efficiency for the second TTrees. The first version of this code started to run over the generated leptons. The drawback is that there could be cases were two different generated leptons are matched to the same reconstructed one. In order to do this, the best is to start the run over the reconstructed particles. This is implemented in this file. 

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By : Gaël L. Perrin

_._._._._._._._._._._._._._._._._._._._._.*/

#include "cmath"
#include "setTDRStyle.C"
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

void eff_plots2_v3(TTree* tree, int leptonId, int mother_Id){

	bool tight = true;
	bool reliso03 = false;
	bool chargereliso = true;

	TString _q = "";
	if(chargereliso == true) _q = "_charged";


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
	TString _pname;
	TString treename;
	TString isoname;

	if (leptonId == 11) {pname = "e";_pname = "e";}
	else if (leptonId == 13){pname = "#mu";_pname = "mu";}
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

	//Name of the output
	TString _output = "/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/ratio_eff/eff_plots2_v3_"+idname+"_"+_pname+_q+"_"+"isocut"+isoname+".root";

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
		tree->SetBranchAddress("el_chargedHadRelIso04", &receliso04);
	}

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
		tree->SetBranchAddress("mu_chargedHadRelIso04", &recmuiso04);
	}

	//Start loop over all events
	for (int k = 0; k < n; ++k) {

		tree->GetEntry(k);

		if(leptonId == 11){
			for(int j=0; j<nrecel;++j){

				double R = 999;
				double delta_P = 999;
				double delta_charge = 999;
				double eta = 999;
				double phi = 999;
				double iso3 = 999;
				double iso4 = 999;
				double recPt = 999;
				int id = 999;

				if((tight == false)||((tight == true)&&(receltightid[j]==1))){
					if(abs(recel_eta[j]) < 2.4){ 
						//loop over all generated particles to do the matching
						for (int i = 0; i < ngenPart; ++i) {
							if((abs(Id[i]) == leptonId)&&(status[i]== 1)&&(abs(gen_eta[i]) < 2.4)&&(abs(source[i]) == mother_Id)){ 

								//Electrons selection
								double R2 = DeltaR(gen_eta[i],recel_eta[j],gen_phi[i],recel_phi[j] );

								//Minimise DeltaR and Fill the other variables
								if (R > R2) {

									R = R2;
									delta_P = abs(recelPt[j]-Pt[i])/Pt[i];
									delta_charge = abs(recelcharge[j] - charge[i]);
									iso3 = receliso03[j];
									iso4 = receliso04[j];
									recPt = recelPt[j];
									id = receltightid[j];
								}
							}
						}
					}			
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
				}
			}
		}

		tree->GetEntry(k);
		if(leptonId == 13){
			for(int j=0; j<nrecmu;++j){

				double R = 999;
				double delta_P = 999;
				double delta_charge = 999;
				double eta = 999;
				double phi = 999;
				double iso3 = 999;
				double iso4 = 999;
				double recPt = 999;
				int id = 999;

				if((tight == false)||((tight == true)&&(recmutightid[j]==1))){
					if(abs(recmu_eta[j]) < 2.4){ 
						//loop over all generated particles to do the matching
						for (int i = 0; i < ngenPart; ++i) {
							if((abs(Id[i]) == leptonId)&&(status[i]== 1)&&(abs(gen_eta[i]) < 2.4)&&(abs(source[i]) == mother_Id)){ 

								//Electrons selection
								double R2 = DeltaR(gen_eta[i],recmu_eta[j],gen_phi[i],recmu_phi[j] );

								//Minimise DeltaR and Fill the other variables
								if (R > R2) {

									R = R2;
									delta_P = abs(recmuPt[j]-Pt[i])/Pt[i];
									delta_charge = abs(recmucharge[j] - charge[i]);
									iso3 = recmuiso03[j];
									iso4 = recmuiso04[j];
									recPt = recmuPt[j];
									id = recmutightid[j];
								}
							}
						}
					}			
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
				}
			}
		}
	}

	histo_sel_Pt_isocut_10->Sumw2();
	histo_sel_Pt_isocut_7->Sumw2();
	histo_sel_Pt_isocut_5->Sumw2();
	histo_Pt_isocut_10->Sumw2();
	histo_Pt_isocut_7->Sumw2();
	histo_Pt_isocut_5->Sumw2();

	histo_sel_Pt_tighId->Sumw2();


	//Divide histograms to get the efficiency
	eff_isocut_10->Divide(histo_sel_Pt_isocut_10, histo_Pt_isocut_10,1,1,"B"); 
	eff_isocut_7->Divide(histo_sel_Pt_isocut_7, histo_Pt_isocut_7,1,1,"B"); 
	eff_isocut_5->Divide(histo_sel_Pt_isocut_5, histo_Pt_isocut_5,1,1,"B"); 
	eff_Pt_tighId->Divide(histo_sel_Pt_tighId, histo_Pt_tighId,1,1,"B");

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
	eff_isocut_10->GetYaxis()->SetTitle("#epsilon");
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
	TLegend* leg2 = new TLegend(0.4, 0.2,0.9,0.4);
	leg2->SetBorderSize(0.);
	leg2->SetTextFont(43);
	leg2->SetTextSize(25);
	leg2->AddEntry(eff_isocut_10, "rel. iso"+isoname+"  #leq "+ (TString)high_cut);  
	leg2->AddEntry(eff_isocut_7, "rel. iso"+isoname+"  #leq "+ (TString)med_cut);
	leg2->AddEntry(eff_isocut_5, "rel. iso test"+isoname+"  #leq "+(TString)low_cut);
	leg2->Draw();

	if(tight == false){
		//Efficiency of the tight ID 
		TCanvas* c2 = new TCanvas("c2","c2");
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

	TFile* output = new TFile(_output,"recreate");
	c1->Write();
	output->Close();

}
