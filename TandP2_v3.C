/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : TanP2_v3.C 

* Purpose : Third version of the Tag and Prob file. It calculates the efficiency on the reconstructed particles using the tag and prob method. The previous version used the same selection criteria for both Tag and Prob, which is not how the method is supposed to work. We implement here a different selection criteria for the tag and prob method which is independent of the lepton order.

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By : Gaël L. Perrin

_._._._._._._._._._._._._._._._._._._._._.*/


void TandP2_v3(TTree* tree, int leptonId, int mother_Id, double Pt_cut = 0, double iso_cut = 0.1){

	bool reliso03 = true;
	bool tight = true;
	bool chargereliso = false;

	TString idname;
	if(tight == true){idname = "tightId";}
	else if(tight == false){idname = "no Id";}

	TString chargename;
	if(chargereliso == true){chargename = "charged_";}
	else if(chargereliso == false){chargename = "";}

	//Name of the lepton
	TString pname;
	if(leptonId == 11){pname = "e";}
	if(leptonId == 13){pname = "mu";}

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

	double Chi2[100];
	double Chi2_fail[100];

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
	if(reliso03 == true){ sprintf(_iso,"isocut03=%0.2f",iso_cut);}
	else if(reliso03 == false){sprintf(_iso,"isocut04=%0.2f",iso_cut);}
	TString f_name = (TString)"TandP2_v3_ZInvM_nentries"+idname+"_"+(TString)pname+"_"+(TString)chargename+(TString)_iso;
	TString f_name2 = (TString)"TandP2_v3_Zeffs_nentries"+idname+"_"+(TString)pname+"_"+(TString)chargename+(TString)_iso;
	TFile* file_out = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/"+f_name+".root","recreate");
	cout<<"This should be the name of the tfile fot the invariant mass :"<<f_name<<endl;

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

		int tag[2];
		int prob[2];
		tag[0] = 9999;
		tag[1] = 9999;
		prob[0] = 9999;
		prob[1] = 9999;

		tree->GetEntry(k);

		if(leptonId == 11){		

			for (int i = 0; i < nrecel; ++i) {

				//Prob selection cut
				if((tight == false)||((tight == true)&&(receltightid[i]==1))){
				if((abs(recel_eta[i]) < 2.4)&&(recelPt[i]>Pt_cut)){ 

						//Prob1
						if(prob[0] == 9999){prob[0] = i;}
						//Prob2
						if((prob[0] != 9999)&&(prob[0] != i)&&(prob[1] == 9999)){prob[1] = i;}
						//Selection cut for Tag only
						if(receliso03[i] < iso_cut ){

							if(prob[0] == i){tag[0] = i; }
							if(prob[1] == i){tag[1] = i; }
						}
					}

				}
			}

			//cout<<"P1 is "<<prob[0]<<endl;
			//cout<<"P2 is "<<prob[1]<<endl;
			//cout<<"T1 is "<<tag[0]<<endl;
			//cout<<"T2 is "<<tag[1]<<endl;


			//loop over all the rec particles to find the isolation
			//We requiere one tag at least 
			while((tag[0] != 9999)||(tag[1] != 9999)){

				int l1;
				int l2;

				if(tag[0] != 9999){l1 = prob[1]; l2 = tag[0]; tag[0] = 9999;}
				else if(tag[1] != 9999){l1 = prob[0]; l2 = tag[1]; tag[1] = 9999;}


				for(int _i = 0; _i < nptbins; ++_i){ 

				double M = InvMass(recelPt[l1],recel_eta[l1],recel_phi[l1],m[l1],recelPt[l2],recel_eta[l2],recel_phi[l2],m[l2]);

					if(l1 != 9999){

						if((_i*D_pt <= recelPt[l1])&&( recelPt[l1] <= (_i+1)*D_pt)){

							//Put Id whose eff. is gonna be estimated here
							if(reliso03 == true){
								if((receliso03[l1] < iso_cut)){histo_M[_i]->Fill(M);}
								else if(receliso03[l1] >= iso_cut){histo_fail_M[_i]->Fill(M);}
							}else if(reliso03 == false){
								if((receliso04[l1] < iso_cut)){histo_M[_i]->Fill(M);}
								else if(receliso04[l1] >= iso_cut){histo_fail_M[_i]->Fill(M);}
							}

						}
					}
				}
			}
		}

		if(leptonId == 13){		

			for (int i = 0; i < nrecmu; ++i) {

				//Prob selection cut
				if((tight == false)||((tight == true)&&(recmutightid[i]==1))){
					if((abs(recmu_eta[i]) < 2.4)&&(recmuPt[i]>Pt_cut)){ 

						//Prob1
						if(prob[0] == 9999){prob[0] = i;}
						//Prob2
						if((prob[0] != 9999)&&(prob[0] != i)&&(prob[1] == 9999)){prob[1] = i;}
						//Selection cut for Tag only
						if(recmuiso03[i] < iso_cut ){

							if(prob[0] == i){tag[0] = i; }
							if(prob[1] == i){tag[1] = i; }
						}
					}

				}
			}

			//We requiere one tag at least 
			while((tag[0] != 9999)||(tag[1] != 9999)){

				int l1;
				int l2;

				if(tag[0] != 9999){l1 = prob[1]; l2 = tag[0]; tag[0] = 9999;}
				else if(tag[1] != 9999){l1 = prob[0]; l2 = tag[1]; tag[1] = 9999;}


				for(int _i = 0; _i < nptbins; ++_i){ 

				double M = InvMass(recmuPt[l1],recmu_eta[l1],recmu_phi[l1],m[l1],recmuPt[l2],recmu_eta[l2],recmu_phi[l2],m[l2]);

					if(l1 != 9999){

						if((_i*D_pt <= recmuPt[l1])&&( recmuPt[l1] <= (_i+1)*D_pt)){

							//Put Id whose eff. is gonna be estimated here
							if(reliso03 == true){
								if((recmuiso03[l1] < iso_cut)){histo_M[_i]->Fill(M);}
								else if(recmuiso03[l1] >= iso_cut){histo_fail_M[_i]->Fill(M);}
							}else if(reliso03 == false){
								if((recmuiso04[l1] < iso_cut)){histo_M[_i]->Fill(M);}
								else if(recmuiso04[l1] >= iso_cut){histo_fail_M[_i]->Fill(M);}
							}

						}
					}
				}
			}
		}
	}

	for(int _i = 0; _i < nptbins; ++_i){ 

		vec1[_i] = FitInvMass(histo_M[_i]);
		vec2[_i] = FitInvMass(histo_fail_M[_i]);

		Eff[_i] = vec1[_i][0]/(vec1[_i][0]+vec2[_i][0]);

		PT[_i] = (_i+0.5)*D_pt; 
		error_eff[_i] = 0;
		error_pt[_i] = D_pt/2;

		Chi2[_i] = vec1[_i][1];
		Chi2_fail[_i] = vec2[_i][1];

	}

	file_out->Close();

	TCanvas* c1 = new TCanvas ("c1","c1");
	c1->cd();

	TGraphErrors* eff_plot= new TGraphErrors(nptbins,PT,Eff,error_pt,error_eff);
	eff_plot->Draw("A*");
	eff_plot->SetMarkerStyle(4);
	eff_plot->SetMarkerSize(0.4);
	eff_plot->SetMarkerColor(4);
	eff_plot->SetMarkerColor(4);
	eff_plot->SetLineColor(4);
	eff_plot->SetTitle("#epsilon for "+(TString)_iso+" for  "+pname+" from Z");
	eff_plot->GetXaxis()->SetTitle("P_{t}");

	TCanvas* c2 = new TCanvas ("c2","c2");
	c2->cd();
	TGraph* chi2_plot= new TGraph(nptbins,PT,Chi2);
	chi2_plot->Draw("AL*");
	chi2_plot->SetMarkerStyle(4);
	chi2_plot->SetMarkerSize(0.4);
	chi2_plot->SetMarkerColor(4);
	chi2_plot->SetLineColor(4);
	chi2_plot->SetTitle("#Chi^{2} for "+(TString)_iso+" for  "+pname+" from Z");
	chi2_plot->GetXaxis()->SetTitle("P_{t}");

	TCanvas* c3 = new TCanvas ("c3","c3");
	c3->cd();
	TGraph* chi2_fail_plot= new TGraph(nptbins,PT,Chi2_fail);
	chi2_fail_plot->Draw("AL*");
	chi2_fail_plot->SetMarkerStyle(4);
	chi2_fail_plot->SetMarkerSize(0.4);
	chi2_fail_plot->SetMarkerColor(4);
	chi2_fail_plot->SetLineColor(4);
	chi2_fail_plot->SetTitle("#Chi^{2} of rej. for "+(TString)_iso+" for  "+pname+" from Z");
	chi2_fail_plot->GetXaxis()->SetTitle("P_{t}");


	TFile* file_out2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/"+f_name2+".root","recreate");
	eff_plot->Write();
	chi2_plot->Write();
	chi2_fail_plot->Write();
	file_out2->Close();

	delete[] histo_M;
	delete[] histo_fail_M;

}
