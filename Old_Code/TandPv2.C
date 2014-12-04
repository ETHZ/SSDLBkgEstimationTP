/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : TandPv2.c

* Purpose : Second version of the Tag and Prob file. It calculates the efficiency on the gen particles using the tag and prob method. The runtime has been improved compared to the previous version.

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By : Gaël L. Perrin

_._._._._._._._._._._._._._._._._._._._._.*/



void TandPv2(TTree* tree, int leptonId, int mother_Id, double Pt_cut = 0, double iso_cut = 0.1){

	//Name of the lepton
	TString pname;
	if(leptonId == 11){pname = "electron";}
	if(leptonId == 13){pname = "muon";}

	//Some variables
	Long64_t n = tree->GetEntries();
	//The Pt binning for leptons 
	int nbins = 250;
	//Number of bins for the efficiency
	int nptbins = 10;
	//Bin width for the eff
	double D_pt = (double)nbins/nptbins;

	//Array to fill the efficiency
	double Eff[100];
	double PT[100];
	double error_eff[100];
	double error_pt[100];

	//Inv mass histo
	//TH1D *histo_M[nptbins];
	//TH1D *histo_fail_M[nptbins];


	//	histo_M[0] = new TH1D("histo_M","test",nbins,0,250);
	//	histo_M[1] = new TH1D("histo_M","test1",nbins,0,250);

	//Vector that contains the eff and chi2 from fit
	vector<double> *vec1 = new vector<double>[nptbins];
	vector<double> *vec2 = new vector<double>[nptbins];


	//Declaration of InvM Histograms
	TH1D **histo_M = new TH1D*[nptbins];
	TH1D **histo_fail_M = new TH1D*[nptbins];

	for(int _i = 0; _i < nptbins; ++_i){ 

		char _pt[50];
		char _pt_fail[50];

		sprintf(_pt,"M_pt_low_%F",_i*D_pt);
		sprintf(_pt_fail,"M_fail_pt_low_%F",_i*D_pt);
		histo_M[_i] = new TH1D("histo_M",_pt,nbins,0,250);
		histo_fail_M[_i] = new TH1D("histo_fail_M",_pt_fail,nbins,0,250);
	}

	//Write the inv. mass into .root file
	char _iso[50];
	sprintf(_iso,"isocut=%f",iso_cut);
	TString f_name = (TString)"ZInvM_beta_"+(TString)pname+(TString)" "+(TString)_iso;
	TString f_name2 = (TString)"Zeff_beta_"+(TString)pname+(TString)" "+(TString)_iso;
	TFile* file_out = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/"+f_name+".root","recreate");
	cout<<"This should be the name of the tfile fot the invariant mass :"<<f_name<<endl;


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

		int l1 = 9999;
		int l2 = 9999;

		tree->GetEntry(k);

		//loop over all generated particles
		//to find the leptons
		for (int i = 0; i < ngenPart; ++i) {

			if((abs(Id[i]) == leptonId)&&(status[i] == 1)&&(abs(gen_eta[i]) < 2.4)&&(((abs(Mo[i]) == mother_Id)||(((abs(GrMa[i]) == mother_Id)&&(abs(Mo[i]) != 15)))))){ 

				//Tag
				if(l1 == 9999){l1 = i;}
				//Prob
				if((l1 != 9999)&&(l1 != i)&&(l2 == 9999)){l2 = i;}
			}

		}

		//loop over all the rec particles to find the isolation
		if((l1 != 9999)&&(l2 != 9999)){

			//Some variables to plot
			double R = 999;
			double delta_P = 999;
			double delta_charge;
			double iso3 = 999;
			double iso4 = 999;

			double M = InvMass(Pt[l1],gen_eta[l1],gen_phi[l1],m[l1],Pt[l2],gen_eta[l2],gen_phi[l2],m[l2]);
			//Loop on reconstructed events to minimise DeltaR and do the matching.
			for (int j = 0; j<nrecPart; ++j) {
				//Electrons selection
				if ((abs(recId[j]) == leptonId)) {

					double R2 = DeltaR(gen_eta[l2],rec_eta[j],gen_phi[l2],rec_phi[j]);

					//Minimise DeltaR and Fill the other variables
					if (R > R2) {

						R = R2;
						delta_P = abs(recPt[j]-Pt[l2])/Pt[l2];
						delta_charge = abs(reccharge[j] - charge[l2]);
						iso3 = Iso03[j];
						iso4 = Iso04[j];
					}
				}
			}

			//Fill Pt only if the identification occured
			if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){

				for(int _i = 0; _i < nptbins; ++_i){ 

					if((_i*D_pt <= Pt[l2])&&((_i+1)*D_pt)){

						//Put Id whose eff. is gonna be estimated here
						if((iso3 < iso_cut)){histo_M[_i]->Fill(M);}
						else if(iso3 >= iso_cut){histo_fail_M[_i]->Fill(M);}

						cout<<"histo "<<_i<<" was filled"<<endl;

						//vec1[_i] = FitInvMass(histo_M[_i]);
						//vec2[_i] = FitInvMass(histo_fail_M[_i]);

						//Eff[_i] = vec1[0][_i]/(vec1[0][_i]+vec2[0][_i]);
						//PT[_i] = (1+0.5)*D_pt; 
						//error_eff[_i] = 0;
						//error_pt[_i] = D_pt/2;

						//cout<<"Chi2 of the passed prob is "<<vec1[1][1]<<endl;
						//cout<<"Chi2 of the rej. prob is "<<vec2[1][1]<<endl;

					}
				}

			}
		}
	}

	for(int _i = 0; _i < nptbins; ++_i){ 

		cout<<"histogram "<<_i<<" is filled"<<endl;

		vec1[_i] = FitInvMass(histo_M[_i]);
		vec2[_i] = FitInvMass(histo_fail_M[_i]);

		Eff[_i] = vec1[0][_i]/(vec1[0][_i]+vec2[0][_i]);
		PT[_i] = (_i+0.5)*D_pt; 
		error_eff[_i] = 0;
		error_pt[_i] = D_pt/2;

		//cout<<"Chi2 of the passed prob is "<<vec1[1][1]<<endl;
		//cout<<"Chi2 of the rej. prob is "<<vec2[1][1]<<endl;

	}

	file_out->Close();

	TGraphErrors* eff_plot= new TGraphErrors(nptbins,PT,Eff,error_pt,error_eff);
	eff_plot->Draw("AL*");
	eff_plot->SetMarkerStyle(4);
	eff_plot->SetMarkerSize(0.4);
	eff_plot->SetMarkerColor(1);
	eff_plot->SetTitle("#epsilon for "+(TString)_iso+" "+pname);
	eff_plot->GetXaxis()->SetTitle("P_{t}");

	TFile* file_out2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/"+f_name2+".root","recreate");
	eff_plot->Write();
	file_out2->Close();



	delete[] histo_M;
	delete[] histo_fail_M;




}
