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

void eff_plots(TTree* tree, int leptonId, int mother_Id, Float_t Pt_cut = 10){

	Long64_t n = tree->GetEntries();

	//Some initial variables
	int nbins = 50;
	int nbinseta = 120;
	//Cuts
	double iso_high = 0.1;
	double pile_up_low = 30;
	double pile_up_high= 40;

	//To choose electron, set to true. To choose muon, set to false.
	TString pname;

	if (leptonId == 11) {pname = "e";}
	else if (leptonId == 13){pname = "#mu";}

	//Declaration of histogram

	//Histograms for efficiency
	TH1D *histo_dR = new TH1D("histo_dR","#Delta R",nbins,0,0.02);
	TH1D *histo_dR_zoomout = new TH1D("histo_dR_zoomout","#Delta R",nbins,0,1000);
	TH1D *histo_dP = new TH1D("histo_dP","#Delta P",nbins,0,1);
	TH1D *histo_Pt = new TH1D("histo_Pt","Pt",nbins,0,250);
	TH1D *histo_eta = new TH1D("histo_eta","eta",nbinseta,-6,6);
	TH1D *histo_phi = new TH1D("histo_phi","phi",nbins,-4,4);
	TH1D *histo_sel_Pt = new TH1D("histo_sel_Pt","Selected Pt",nbins,0,250);
	TH1D *histo_sel_eta = new TH1D("histo_sel_eta","eta",nbinseta,-6,6);
	TH1D *histo_sel_phi = new TH1D("histo_sel_phi","phi",nbins,-4,4);
	TH1D *eff_Pt = new TH1D("eff_Pt","Efficiency Pt",nbins,0,250);
	TH1D *eff_eta = new TH1D("eff_eta","eta",nbinseta,-6,6);
	TH1D *eff_phi = new TH1D("eff_phi","phi",nbins,-4,4);

	//Counting number of generated and corresponding reconstructed lepton per event
	TH1D *nleptons_per_event = new TH1D("n_leptons_per_event","nlep",20,0,20);
	TH1D *nrecleptons_per_event = new TH1D("n_recleptons_per_event","nlep",20,0,20);

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

	//Event variables
	Float_t gen_phi[200];
	Float_t gen_eta[200];
	Float_t rec_phi[200];
	Float_t rec_eta[200];
	Float_t Pt[200];
	Float_t m[200];
	Float_t recm[200];
	Int_t Id[200];
	Int_t Mo[200];
	Int_t recId[200];
	Float_t recPt[200];
	Float_t charge[200];
	Int_t reccharge[200];
	Int_t status[200];
	Int_t GrMa[200];
	Int_t nvert[200];
	Int_t pile_up;
	Int_t ngenPart;
	Int_t nrecPart;
	Int_t nrecel;
	Float_t Iso03[200];
	Float_t Iso04[200];

	//Assigne branches
	//generated
	tree->SetBranchAddress("genPart_pdgId", &Id);
	tree->SetBranchAddress("genPart_motherId", &Mo);
	tree->SetBranchAddress("genPart_eta", &gen_eta);
	tree->SetBranchAddress("genPart_phi", &gen_phi);
	tree->SetBranchAddress("ngenPart", &ngenPart);
	tree->SetBranchAddress("genPart_pt", &Pt);
	tree->SetBranchAddress("genPart_mass", &m);
	tree->SetBranchAddress("genPart_charge", &charge);
	tree->SetBranchAddress("genPart_status", &status);
	tree->SetBranchAddress("genPart_grandmaId",&GrMa);
	tree->SetBranchAddress("nTrueInt",&pile_up);
	tree->SetBranchAddress("nVert",&nvert);
	//reconstructed
	tree->SetBranchAddress("lep_pdgId", &recId);
	tree->SetBranchAddress("nlep", &nrecPart);
	tree->SetBranchAddress("nElectrons10", &nrecel);
	tree->SetBranchAddress("lep_pt", &recPt);
	tree->SetBranchAddress("lep_mass", &recm);
	tree->SetBranchAddress("lep_eta", &rec_eta);
	tree->SetBranchAddress("lep_phi", &rec_phi);
	tree->SetBranchAddress("lep_charge", &reccharge);
	tree->SetBranchAddress("lep_relIso03", &Iso03);
	tree->SetBranchAddress("lep_relIso04", &Iso04);

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


			if((abs(Id[i]) == leptonId)&&(status[i]== 1)&&(abs(gen_eta[i]) < 2.4)&&(((abs(Mo[i]) == mother_Id)||(((abs(GrMa[i]) == mother_Id)&&(abs(Mo[i]) != 15)))))&&(Pt[i]>Pt_cut)){ //For ttbar sample

				//Prompt E
				++nleptons;

				//Some variables to plot
				double R = 999;
				double delta_P = 999;
				double delta_charge;
				double iso3 = 999;
				double iso4 = 999;

				//Loop on reconstructed events to minimize DeltaR and do the matching
				for (int j = 0; j<nrecPart; ++j) {

					//Electrons selection
					if ((abs(recId[j]) == leptonId)) {

							if(counted == false){++nrecleptons;}

						TLorentzVector genV;
						TLorentzVector recV;

						genV.SetPtEtaPhiM(Pt[i],gen_eta[i],gen_phi[i],m[i]);
						recV.SetPtEtaPhiM(recPt[j],rec_eta[j],rec_phi[j],recm[j]);

						double R2 = genV.DeltaR(recV);

						//Minimise DeltaR and Fill the other variables
						if (R > R2) {

							R = genV.DeltaR(recV);
							delta_P = abs(recPt[j]-Pt[i])/Pt[i];
							delta_charge = abs(reccharge[j] - charge[i]);
							iso3 = Iso03[j];
							iso4 = Iso04[j];

						}   
					}                                         
				}

				counted = true;

				//Fill histograms
				histo_dR->Fill(R);
				histo_dP->Fill(delta_P);
				histo_dR_zoomout->Fill(R);
				//Histo for eff. 
				histo_Pt->Fill(Pt[i]);
				histo_eta->Fill(gen_eta[i]);
				histo_phi->Fill(gen_phi[i]);

				//Histogram for different pile-up range
				if((pile_up <  pile_up_low)){

					histo_Pt_low_int->Fill(Pt[i]);
					histo_eta_low_int->Fill(gen_eta[i]);
					histo_phi_low_int->Fill(gen_phi[i]);

				}

				if((pile_up >  pile_up_low)&&(pile_up < pile_up_high)){

					histo_Pt_med_int->Fill(Pt[i]);
					histo_eta_med_int->Fill(gen_eta[i]);
					histo_phi_med_int->Fill(gen_phi[i]);

				}

				if((pile_up > pile_up_high)){
					histo_Pt_high_int->Fill(Pt[i]);
					histo_eta_high_int->Fill(gen_eta[i]);
					histo_phi_high_int->Fill(gen_phi[i]);

				}                

				//Fill Pt only if the selection criteria is satisfied
				if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){

					histo_sel_Pt->Fill(Pt[i]);
					histo_sel_eta->Fill(gen_eta[i]);
					histo_sel_phi->Fill(gen_phi[i]);

					//Fill histogram with the cut on the isolation only
					histo_Pt_isocut_10->Fill(Pt[i]);
					histo_Pt_isocut_7->Fill(Pt[i]);
					histo_Pt_isocut_5->Fill(Pt[i]);
					if(iso3 < 0.1){histo_sel_Pt_isocut_10->Fill(Pt[i]);}
					if(iso3 < 0.07){histo_sel_Pt_isocut_7->Fill(Pt[i]);}
					if(iso3 < 0.05){histo_sel_Pt_isocut_5->Fill(Pt[i]);}

					//Fill the histograms for different number of Interaction and resolution

					//Fill histogram for different interaction range
					if((pile_up <  pile_up_low)){

						histo_sel_Pt_low_int->Fill(Pt[i]);
						histo_sel_eta_low_int->Fill(gen_eta[i]);
						histo_sel_phi_low_int->Fill(gen_phi[i]);

						histo_Pt_isocut_low_int_10->Fill(Pt[i]);
						histo_Pt_isocut_low_int_7->Fill(Pt[i]);
						histo_Pt_isocut_low_int_5->Fill(Pt[i]);

						if(iso3 < 0.1){histo_sel_Pt_isocut_low_int_10->Fill(Pt[i]);}
						if(iso3 < 0.07){histo_sel_Pt_isocut_low_int_7->Fill(Pt[i]);}
						if(iso3 < 0.05){histo_sel_Pt_isocut_low_int_5->Fill(Pt[i]);}

					}                        		

					if((pile_up >  pile_up_low)&&(pile_up < pile_up_high)){

						histo_sel_Pt_med_int->Fill(Pt[i]);
						histo_sel_eta_med_int->Fill(gen_eta[i]);
						histo_sel_phi_med_int->Fill(gen_phi[i]);

						histo_Pt_isocut_med_int_10->Fill(Pt[i]);
						histo_Pt_isocut_med_int_7->Fill(Pt[i]);
						histo_Pt_isocut_med_int_5->Fill(Pt[i]);

						if(iso3 < 0.1){histo_sel_Pt_isocut_med_int_10->Fill(Pt[i]);}
						if(iso3 < 0.07){histo_sel_Pt_isocut_med_int_7->Fill(Pt[i]);}
						if(iso3 < 0.05){histo_sel_Pt_isocut_med_int_5->Fill(Pt[i]);}
					}

					if((pile_up > pile_up_high)){

						histo_sel_Pt_high_int->Fill(Pt[i]);
						histo_sel_eta_high_int->Fill(gen_eta[i]);
						histo_sel_phi_high_int->Fill(gen_phi[i]);

						histo_Pt_isocut_high_int_10->Fill(Pt[i]);
						histo_Pt_isocut_high_int_7->Fill(Pt[i]);
						histo_Pt_isocut_high_int_5->Fill(Pt[i]);

						if(iso3 < 0.1){histo_sel_Pt_isocut_high_int_10->Fill(Pt[i]);}
						if(iso3 < 0.07){histo_sel_Pt_isocut_high_int_7->Fill(Pt[i]);}
						if(iso3 < 0.05){histo_sel_Pt_isocut_high_int_5->Fill(Pt[i]);}

					}
				}
			}
		}
		nleptons_per_event->Fill(nleptons);
		nrecleptons_per_event->Fill(nrecleptons);
	}

	//Putting errors into the histograms   
	histo_sel_Pt->Sumw2();
	histo_Pt->Sumw2();
	histo_sel_eta->Sumw2();
	histo_eta->Sumw2();
	histo_sel_phi->Sumw2();
	histo_phi->Sumw2();

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


	//Divide histograms to get the efficiency
	eff_Pt->Divide(histo_sel_Pt,histo_Pt,1,1,"B");
	eff_eta->Divide(histo_sel_eta,histo_eta,1,1,"B");
	eff_phi->Divide(histo_sel_phi,histo_phi,1,1,"B");
	eff_isocut_10->Divide(histo_sel_Pt_isocut_10, histo_Pt_isocut_10,1,1,"B"); 
	eff_isocut_7->Divide(histo_sel_Pt_isocut_7, histo_Pt_isocut_7,1,1,"B"); 
	eff_isocut_5->Divide(histo_sel_Pt_isocut_5, histo_Pt_isocut_5,1,1,"B"); 

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
	eff_Pt->DrawCopy();
	c1->cd(5);
	eff_eta->DrawCopy();
	c1->cd(6);
	eff_phi->DrawCopy();
	c1->Write();

	TCanvas* c2 = new TCanvas("c2","c2");
	c2->cd();
	nrecleptons_per_event->Draw();
	nleptons_per_event->Draw("same");
	nleptons_per_event->SetLineStyle(2);
	nrecleptons_per_event->SetLineColor(2);
	nleptons_per_event->SetTitle("n "+pname+ " events per bin");
	nrecleptons_per_event->GetXaxis()->SetTitle("Number of "+pname+" per events");

	TLegend* leg = new TLegend(0.25, 0.6, 0.6 ,0.8);
	leg->AddEntry(nleptons_per_event, "n gen. "+pname+" per event");
	leg->AddEntry(nrecleptons_per_event, "n rec. "+pname+" per event");
	leg->Draw();

	//eff of the iso cut
	TCanvas* c3a = new TCanvas("c3a","c3a");
	eff_isocut_10->Draw();
	eff_isocut_10->SetMarkerStyle(4);
	eff_isocut_10->SetMarkerSize(0.4);
	eff_isocut_10->SetMarkerColor(4);
	eff_isocut_10->SetLineColor(4);
	TCanvas* c3b = new TCanvas("c3b","c3b");
	eff_isocut_7->Draw();
	eff_isocut_7->SetMarkerStyle(4);
	eff_isocut_7->SetMarkerSize(0.4);
	eff_isocut_7->SetMarkerColor(4);
	eff_isocut_7->SetLineColor(4);
	TCanvas* c3c = new TCanvas("c3c","c3c");
	eff_isocut_5->Draw();
	eff_isocut_5->SetMarkerStyle(4);
	eff_isocut_5->SetMarkerSize(0.4);
	eff_isocut_5->SetMarkerColor(4);
	eff_isocut_5->SetLineColor(4);

	TCanvas* c4a = new TCanvas("c4a","eff_iso_0.1");   
	eff_isocut_low_int_10->Draw();
	eff_isocut_low_int_10->GetXaxis()->SetTitle("P_{t}");
	eff_isocut_low_int_10->SetTitle("#epsilon for Iso < 0.1");
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
	TLegend* leg2 = new TLegend(0.25, 0.6, 0.6 ,0.8);
	leg2->AddEntry(eff_Pt_low_int, "inter.  #leq  30");
	leg2->AddEntry(eff_Pt_med_int, "30  #geq inter.  #leq 40");
	leg2->AddEntry(eff_Pt_high_int, "40  #leq  inter. ");
	leg2->Draw();
	TCanvas* c4b = new TCanvas("c4b","eff_iso_0.07");   
	eff_isocut_low_int_7->Draw();
	eff_isocut_low_int_7->GetXaxis()->SetTitle("P_{t}");
	eff_isocut_low_int_7->SetTitle("#epsilon for Iso < 0.07");
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
	leg2->Draw();
	TCanvas* c4c = new TCanvas("c4c","eff_iso_0.05");   
	eff_isocut_low_int_5->Draw();
	eff_isocut_low_int_5->GetXaxis()->SetTitle("P_{t}");
	eff_isocut_low_int_5->SetTitle("#epsilon for Iso < 0.05");
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
	leg2->Draw();

	TCanvas* c7 = new TCanvas("c7","c7");
	//c5->Divide(3,1);
	//eff_Pt->Draw();
	//eff_Pt->SetLineWidth(1);
	eff_Pt_low_int->Draw();
	eff_Pt_low_int->SetTitle("Efficiency for "+pname);
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

	//Same for the 
	//c4->cd(2);
	//eff_eta->Draw();
	//eff_eta->SetLineWidth(1);
	//eff_eta_low_int->Draw("same");
	//eff_eta_low_int->SetLineColor(2);
	//eff_eta_med_int->Draw("same");
	//eff_eta_med_int->SetLineColor(3);
	//eff_eta_high_int->Draw("same");
	//eff_eta_high_int->SetLineColor(4);
	//c4->cd(3);
	//eff_phi->Draw();
	//eff_phi->SetLineWidth(1);
	//eff_phi_low_int->Draw("same");
	//eff_phi_low_int->SetLineColor(2);
	//eff_phi_med_int->Draw("same");
	//eff_phi_med_int->SetLineColor(3);
	//eff_phi_high_int->Draw("same");
	//eff_phi_high_int->SetLineColor(4);

}
