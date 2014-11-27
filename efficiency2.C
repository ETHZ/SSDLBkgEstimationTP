/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : efficiency2.C

* Purpose : This compute the efficiency for the second TTrees. The name of the branches is different than the previous trees and therefor this it is necessary to rewrite a new file.

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

void efficiency2( TTree* tree, int leptonId, int mother_Id, Float_t Pt_cut = 10)
{

	gStyle->SetOptStat(0);
	Long64_t n = tree->GetEntries();

	//Some initial variables
	int nbins = 10;
	int nbinseta = 120;
	//To choose electron, set to true. To choose muon, set to false.
	TString pname;

	if (leptonId == 11) {pname = "e";}
	else if (leptonId == 13){pname = " #mu";}

	//Declaration of histograms
	TH1D *histo_dR = new TH1D("histo_dR","#Delta R",nbins,0,0.02);
	TH1D *histo_dR_zoomout = new TH1D("histo_dR_zoomout","#Delta R",nbins,0,1000);
	TH1D *histo_dP = new TH1D("histo_dP","#Delta P",nbins,0,1);
	TH1D *histo_Pt = new TH1D("histo_Pt","Pt",nbins,0,250);
	TH1D *histo_sel_Pt = new TH1D("histo_sel_Pt","Selected Pt",nbins,0,250);
	TH1D *histo_eta = new TH1D("histo_eta","eta",nbinseta,-6,6);
	TH1D *histo_phi = new TH1D("histo_phi","phi",nbins,-4,4);
	TH1D *histo_sel_eta = new TH1D("histo_sel_eta","eta",nbinseta,-6,6);
	TH1D *histo_sel_phi = new TH1D("histo_sel_phi","phi",nbins,-4,4);
	TH1D *eff_Pt = new TH1D("eff_Pt","Efficiency Pt",nbins,0,250);
	TH1D *eff_eta = new TH1D("eff_eta","eta",nbinseta,-6,6);
	TH1D *eff_phi = new TH1D("eff_phi","phi",nbins,-4,4);
	//Counting number of generated and corresponding reconstructed lepton per event
	TH1D *nleptons_per_event = new TH1D("n_leptons_per_event","nlep",20,0,20);
	TH1D *nrecleptons_per_event = new TH1D("n_recleptons_per_event","nlep",20,0,20);

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
	//Reconstructed
	//Electrons
	Int_t nrecel;
	Float_t recelPt[200];
	Float_t recelm[200];
	Float_t recel_eta[200];
	Float_t recel_phi[200];
	Int_t recelcharge[200];
	Float_t receliso03[200];
	Float_t receliso04[200];
	//Muons
	Int_t nrecmu;
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
	//reconstructed
	//electron
	tree->SetBranchAddress("nel", &nrecel);
	tree->SetBranchAddress("el_pt", &recelPt);
	tree->SetBranchAddress("el_mass", &recelm);
	tree->SetBranchAddress("el_eta", &recel_eta);
	tree->SetBranchAddress("el_phi", &recel_phi);
	tree->SetBranchAddress("el_charge", &recelcharge);
	tree->SetBranchAddress("el_relIso03", &receliso03);
	tree->SetBranchAddress("el_relIso04", &receliso04);
	//muons
	tree->SetBranchAddress("nmu", &nrecmu);
	tree->SetBranchAddress("mu_pt", &recmuPt);
	tree->SetBranchAddress("mu_mass", &recmum);
	tree->SetBranchAddress("mu_eta", &recmu_eta);
	tree->SetBranchAddress("mu_phi", &recmu_phi);
	tree->SetBranchAddress("mu_charge", &recmucharge);
	tree->SetBranchAddress("mu_relIso03", &recmuiso03);
	tree->SetBranchAddress("mu_relIso04", &recmuiso04);

	//Start loop over all events
	for (int k = 0; k < n; ++k) {

		cout<<"Event number :"<<k<<endl;

		tree->GetEntry(k);

		//Counting of the gen. and rec. leptons per events.
		int nleptons = 0;
		int nrecleptons = 0;
		bool counted = false;//This variable tels if we already run over all the rec. events

		//loop over all generated particles
		for (int i = 0; i < ngenPart; ++i) {

			//For gen electrons
			if( leptonId == 11){

				if((abs(Id[i]) == leptonId)&&(status[i]== 1)&&(abs(gen_eta[i]) < 2.4)&&(abs(source[i])==23)&&(Pt[i]>Pt_cut)){ 

					//Prompt E
					++nleptons;

					//Some variables to plot
					double R = 999;
					double delta_P = 999;
					double delta_charge;
					double iso3 = 999;
					double iso4 = 999;

					//Loop on reconstructed events to minimise DeltaR and do the matching
					for (int j = 0; j<nrecel; ++j) {
						//Electrons selection

						if(counted == false){++nrecleptons;}

						cout<<"gen_eta "<<gen_eta[i]<<endl;
						cout<<"rec_el_eta"<<recel_eta[j]<<endl;
						cout<<"gen_phi"<<gen_phi[i]<<endl;
						cout<<"recel_phi"<<recel_phi[j]<<endl;

						double R2 = DeltaR(gen_eta[i],recel_eta[j],gen_phi[i],recel_phi[j] );

						//Minimise DeltaR and Fill the other variables
						if (R > R2) {

							R = R2;
							delta_P = abs(recelPt[j]-Pt[i])/Pt[i];
							delta_charge = abs(recelcharge[j] - charge[i]);
							iso3 = receliso03[j];
							iso4 = receliso04[j];

						}

					}

					counted = true;

					//Fill histo_elgrams
					histo_dR->Fill(R);
					histo_dP->Fill(delta_P);
					histo_dR_zoomout->Fill(R);
					//Histo for eff. 
					histo_Pt->Fill(Pt[i]);
					histo_eta->Fill(gen_eta[i]);
					histo_phi->Fill(gen_phi[i]);

					//Fill Pt only if the selection criteria is satified
					if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){

						histo_sel_Pt->Fill(Pt[i]);
						histo_sel_eta->Fill(gen_eta[i]);
						histo_sel_phi->Fill(gen_phi[i]);

					}
				}
			}

			//For gen electrons
			else if( leptonId == 13){

				if((abs(Id[i]) == leptonId)&&(status[i]== 1)&&(abs(gen_eta[i]) < 2.4)&&(abs(source[i])==23)&&(Pt[i]>Pt_cut)){ 

					//Prompt E
					++nleptons;

					//Some variables to plot
					double R = 999;
					double delta_P = 999;
					double delta_charge;
					double iso3 = 999;
					double iso4 = 999;

					//Loop on reconstructed events to minimise DeltaR and do the matching
					for (int j = 0; j<nrecmu; ++j) {
						//Electrons selection

						if(counted == false){++nrecleptons;}

						double R2 = DeltaR(gen_eta[i],recmu_eta[j],gen_phi[i],recmu_phi[j] );

						//Minimise DeltaR and Fill the other variables
						if (R > R2) {

							R = R2;
							delta_P = abs(recmuPt[j]-Pt[i])/Pt[i];
							delta_charge = abs(recmucharge[j] - charge[i]);
							iso3 = recmuiso03[j];
							iso4 = recmuiso04[j];

						}

					}

					counted = true;

					//Fill histo_elgrams
					histo_dR->Fill(R);
					histo_dP->Fill(delta_P);
					histo_dR_zoomout->Fill(R);
					//Histo for eff. 
					histo_Pt->Fill(Pt[i]);
					histo_eta->Fill(gen_eta[i]);
					histo_phi->Fill(gen_phi[i]);

					//Fill Pt only if the selection criteria is satified
					if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){

						histo_sel_Pt->Fill(Pt[i]);
						histo_sel_eta->Fill(gen_eta[i]);
						histo_sel_phi->Fill(gen_phi[i]);

					}
				}
			}
		}

		nleptons_per_event->Fill(nleptons);
		nrecleptons_per_event->Fill(nrecleptons);
	}

		//Filling the efficiency histograms including the errors
		histo_sel_Pt->Sumw2();
		histo_Pt->Sumw2();
		histo_sel_eta->Sumw2();
		histo_eta->Sumw2();
		histo_sel_phi->Sumw2();
		histo_phi->Sumw2();

		eff_Pt->Divide(histo_sel_Pt,histo_Pt,1,1,"B");
		eff_eta->Divide(histo_sel_eta,histo_eta,1,1,"B");
		eff_phi->Divide(histo_sel_phi,histo_phi,1,1,"B");

		//Drawing properties of the histograms
		histo_dR->SetLineWidth(2);
		histo_dR->SetMarkerColor(2);
		histo_dR->SetFillColor(4);
		histo_dR->SetFillStyle(3005);
		histo_dR->SetMarkerColor(2);
		histo_dR->SetMarkerSize(1);
		histo_dR->SetMarkerStyle(21);
		histo_dR->SetTitle("#Delta R for "+pname+" from 0 to 5");
		histo_dR->GetXaxis()->SetTitle("#Delta R");
		histo_dR->GetYaxis()->SetTitle("");

		histo_dR_zoomout->SetLineWidth(2);
		histo_dR_zoomout->SetMarkerColor(2);
		histo_dR_zoomout->SetFillColor(4);
		histo_dR_zoomout->SetFillStyle(3005);
		histo_dR_zoomout->SetMarkerSize(1);
		histo_dR_zoomout->SetMarkerStyle(21);
		histo_dR_zoomout->SetTitle("#Delta R for "+pname+" from 0 to 1000");
		histo_dR_zoomout->GetXaxis()->SetTitle("#Delta R");
		histo_dR_zoomout->GetYaxis()->SetTitle("");

		histo_dP->SetLineWidth(2);
		histo_dP->SetTitle("#frac{#Delta P_{t}}{P_{t}} for "+pname);
		histo_dP->GetXaxis()->SetTitle("#frac{#Delta P_{t}}{P_{t}}");
		histo_dP->GetXaxis()->SetTitleOffset(1.4);
		histo_dP->GetXaxis()->SetTitleSize(0.03);
		histo_dP->SetFillColor(4);
		histo_dP->SetFillStyle(3005);

		histo_Pt->SetLineWidth(2);
		histo_Pt->SetLineColor(2);

		histo_eta->SetLineWidth(2);
		histo_eta->SetLineColor(2);

		histo_phi->SetLineWidth(2);
		histo_phi->SetLineColor(2);

		eff_phi->SetLineWidth(2);
		eff_phi->SetTitle("#epsilon_{#phi} for gen. "+pname+" after selection");
		eff_phi->GetXaxis()->SetTitle("#phi");

		eff_eta->SetLineWidth(2);
		eff_eta->SetTitle("#epsilon_{#eta} for gen. "+pname);
		eff_eta->GetXaxis()->SetTitle("#eta");

		eff_Pt->SetLineWidth(2);
		eff_Pt->SetTitle("#epsilon_{P_{t}} distribution for "+pname);
		eff_Pt->GetXaxis()->SetTitle("P_{t} [GeV]");

		gStyle->SetOptStat(0);

		TCanvas* c1 = new TCanvas("c1","c1",1600,900);
		c1->Divide(3,2);
		c1->cd(1);
		histo_dR->Draw();
		c1->cd(2);
		histo_dR_zoomout->Draw();
		c1->cd(3);
		histo_dP->Draw();
		c1->cd(4);
		eff_Pt->Draw();
		c1->cd(5);
		eff_eta->Draw();
		c1->cd(6);
		eff_phi->Draw();

		TCanvas* c2 = new TCanvas("c2","c2");
		c2->cd();
		nrecleptons_per_event->Draw();
		nleptons_per_event->Draw("same");
		nleptons_per_event->SetLineStyle(2);
		nrecleptons_per_event->SetLineColor(2);
		nleptons_per_event->SetTitle("n "+pname+ " events per bin");
		nrecleptons_per_event->GetXaxis()->SetTitle("Number of "+pname+" per events");

		TLegend* leg = new TLegend(0.25, 0.6, 0.6 , 0.8);//(0.8, 0.9, .2, .4);
		leg->AddEntry(nleptons_per_event, "n gen. "+pname+" per event");
		leg->AddEntry(nrecleptons_per_event, "n rec. "+pname+" per event");
		leg->Draw();


}
