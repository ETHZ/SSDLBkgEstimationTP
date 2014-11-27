/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

* File Name : FitInvMassBkg.C 

 * Purpose : This file does a fit of the histogram of the Z lineshape (given by tree1) plus background (tree2). The first part of the file use trees from different samples to plot the Invariant mass histograms of the leptons which pdg_id is given as an argument. The scaling factor is not stored in the TTree right now so we have to put it manually.

 * Creation Date : 20-12-2008

 * Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

 * Created By : Gaël L. Perrin

 _._._._._._._._._._._._._._._._._._._._._.*/

int DrawInvMassBkg(TTree* tree, int leptonId, int mother_Id, Float_t Pt_low = 0, Float_t Pt_upp = 999, Float_t iso_cut = 999, int iso0n = 3,bool barrel = true){

	char _Ptrange[50];
	sprintf(_Ptrange,"%0.f #leq P_{t} #leq %0.f",Pt_low,Pt_upp);
	char _isocutpass[50];
	sprintf(_isocutpass,"iso0%u #leq %0.01f",iso0n, iso_cut);
	char _isocutfail[50];
	sprintf(_isocutfail,"iso0%u > %0.01f",iso0n, iso_cut);
	char _eta[50];
	if(barrel == true){
		sprintf(_eta,"#eta #leq %0.1f",1.2);
	} else if (barrel == false){
		sprintf(_eta,"#eta > %0.1f",1.2);}

	//Name of the output file
	char _output[100];
	//For isolation efficiency
	//if(barrel == true){
	//	sprintf(_output,"DrawInvMassBkg_Pt_%0.f_%0.f_reliso0%u_%0.01f_eta<1.2",Pt_low,Pt_upp,iso0n,iso_cut);}
	//else if (barrel == false){sprintf(_output,"DrawInvMassBkg_Pt_%0.f_%0.f_reliso0%u_%0.01f_eta>1.2",Pt_low,Pt_upp,iso0n,iso_cut);}
	
	//if(barrel == true){
        //	sprintf(_output,"DrawInvMassBkg_tight_Pt_%0.f_%0.f_eta<1.2",Pt_low,Pt_upp);}
	//else if (barrel == false){sprintf(_output,"DrawInvMassBkg_tight_Pt_%0.f_%0.f_eta>1.2",Pt_low,Pt_upp);}//Name of the lepton
	
	if(barrel == true){
          	sprintf(_output,"DrawInvMassBkg_100toInf_tight_Pt_%0.f_%0.f_eta<1.2",Pt_low,Pt_upp);}
	else if (barrel == false){sprintf(_output,"DrawInvMassBkg_100toInf_tight_Pt_%0.f_%0.f_eta>1.2",Pt_low,Pt_upp);}
	
	TString pname;
	if(leptonId == 11){pname = "e";}
	if(leptonId == 13){pname = "mu";}

	//Some variables
	Long64_t n = tree->GetEntries();

	//Declaration of histograms
	//Histo variables
	int nbins = 200;
	//Histograms

	TH1D* histo_M_DYJets_bkg = new TH1D("histo_M_DYJets_bkg","M",nbins,0,nbins);
	TH1D* histo_M_bkg = new TH1D("histo_M_bkg","M",nbins,0,nbins);
	TH1D* histo_M_WJets = new TH1D("histo_M_WJets","M",nbins,0,nbins);
	TH1D* histo_M_DYJets_bkg_fail = new TH1D("histo_M_DYJets_bkg_fail","M",nbins,0,nbins);
	TH1D* histo_M_bkg_fail = new TH1D("histo_M_bkg_fail","M",nbins,0,nbins);
	TH1D* histo_M_WJets_fail = new TH1D("histo_M_WJets_fail","M",nbins,0,nbins);

	//Event variables
	//Generated
	Int_t evt_id;
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
	Float_t scale;
	Float_t recmum[200];
	Float_t recmu_eta[200];
	Float_t recmu_phi[200];
	Int_t recmucharge[200];
	Float_t recmuiso03[200];

	//Assigne branches
	tree->SetBranchAddress("evt_scale1fb", &scale);
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
	//reconstructed
	//electron
	tree->SetBranchAddress("nel", &nrecel);
	tree->SetBranchAddress("el_tightId", &receltightid);
	tree->SetBranchAddress("el_pt", &recelPt);
	tree->SetBranchAddress("el_mass", &recelm);
	tree->SetBranchAddress("el_eta", &recel_eta); 
	tree->SetBranchAddress("el_phi", &recel_phi);
	tree->SetBranchAddress("el_charge", &recelcharge);
	if(iso0n == 3){tree->SetBranchAddress("el_relIso03", &receliso03);}
	else if(iso0n == 4){
		tree->SetBranchAddress("el_relIso04", &receliso03);}
	else{ cout<<"Invalide isolation !"<<endl; return 1;}

	//muons
	tree->SetBranchAddress("nmu", &nrecmu);
	tree->SetBranchAddress("mu_tightId", &recmutightid);
	tree->SetBranchAddress("mu_pt", &recmuPt);
	tree->SetBranchAddress("mu_mass", &recmum);
	tree->SetBranchAddress("mu_eta", &recmu_eta);
	tree->SetBranchAddress("mu_phi", &recmu_phi);
	tree->SetBranchAddress("mu_charge", &recmucharge);
	if(iso0n == 3){tree->SetBranchAddress("mu_relIso03", &recmuiso03);}
	else if(iso0n == 4){
		tree->SetBranchAddress("mu_relIso04", &recmuiso03);}
	else{ cout<<"Invalide isolation !"<<endl; return 1;}


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
				//if(receltightid[i] == 1){ 

					//Prob1
					if(prob[0] == 9999){prob[0] = i;}
					//Prob2
					if((prob[0] != 9999)&&(prob[0] != i)&&(prob[1] == 9999)){prob[1] = i;}
					//Selection cut for Tag only
					//if(receliso03[i] < iso_cut ){
					if(receltightid[i] == 1){ 

						if(prob[0] == i){tag[0] = i; }
						if(prob[1] == i){tag[1] = i; }
					}
				//}
			}

			//loop over all the rec particles to find the isolation
			//We requiere one tag at least 
			while((tag[0] != 9999)||(tag[1] != 9999)){

				int l1;
				int l2;

				if(tag[0] != 9999){l1 = prob[1]; l2 = tag[0]; tag[0] = 9999;}
				else if(tag[1] != 9999){l1 = prob[0]; l2 = tag[1]; tag[1] = 9999;}

				double M = InvMass(recelPt[l1],recel_eta[l1],recel_phi[l1],m[l1],recelPt[l2],recel_eta[l2],recel_phi[l2],m[l2]);

				if(l1 != 9999){
					//Add all the signal to the Z
					if((recelPt[l1] > Pt_low)&&(recelPt[l1]< Pt_upp)){
						if(((barrel == true)&&(abs(recel_eta[l1]) <= 1.2))||((barrel == false)&&(abs(recel_eta[l1]) > 1.2))){

							//Efficiency cut
							//if(receliso03[l1] < iso_cut){
							if(receltightid[l1] == 1){ 
								histo_M_DYJets_bkg->Fill(M,scale);

								if ((evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){
									histo_M_bkg->Fill(M,scale);

									//Just the Wjets bkg
									if ((evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){
										histo_M_WJets->Fill(M,scale);

									}
								}
							}

							//if(receliso03[l1] >= iso_cut){
							if(receltightid[l1] != 1){ 
								histo_M_DYJets_bkg_fail->Fill(M,scale);

								//Put all the background
								if ((evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){
									histo_M_bkg_fail->Fill(M,scale);

									//Just the Wjets bkg
									//Just the Wjets bkg
									if ((evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){
										histo_M_WJets_fail->Fill(M,scale);

									}
								}
							}
						}
					}
				}
			}
		}					

		if(leptonId == 13){		

			for (int i = 0; i < nrecmu; ++i) {

				//Prob selection cut
				//if(recmutightid[i] == 1){ 

					//Prob1
					if(prob[0] == 9999){prob[0] = i;}
					//Prob2
					if((prob[0] != 9999)&&(prob[0] != i)&&(prob[1] == 9999)){prob[1] = i;}
					//Selection cut for Tag only
					//if(recmuiso03[i] < iso_cut ){
					if(recmutightid[i] == 1){ 

						if(prob[0] == i){tag[0] = i; }
						if(prob[1] == i){tag[1] = i; }
					}
				//}
			}

			//loop over all the rec particles to find the isolation
			//We requiere one tag at least 
			while((tag[0] != 9999)||(tag[1] != 9999)){

				int l1;
				int l2;

				if(tag[0] != 9999){l1 = prob[1]; l2 = tag[0]; tag[0] = 9999;}
				else if(tag[1] != 9999){l1 = prob[0]; l2 = tag[1]; tag[1] = 9999;}

				double M = InvMass(recmuPt[l1],recmu_eta[l1],recmu_phi[l1],m[l1],recmuPt[l2],recmu_eta[l2],recmu_phi[l2],m[l2]);

				if(l1 != 9999){
					//Add all the signal to the Z
					if((recmuPt[l1] > Pt_low)&&(recmuPt[l1]< Pt_upp)){
						if(((barrel == true)&&(abs(recmu_eta[l1]) <= 1.2))||((barrel == false)&&(abs(recmu_eta[l1]) > 1.2))){

							//Efficiency cut
							//if(receliso03[l1] < iso_cut){
							if(recmutightid[l1] == 1){ 
								histo_M_DYJets_bkg->Fill(M,scale);

								if ((evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){
									histo_M_bkg->Fill(M,scale);

									//Just the Wjets bkg
									if ((evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){
										histo_M_WJets->Fill(M,scale);

									}
								}
							}

							//if(receliso03[l1] >= iso_cut){
							if(recmutightid[l1] != 1){ 
								histo_M_DYJets_bkg_fail->Fill(M,scale);

								//Put all the background
								if ((evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){
									histo_M_bkg_fail->Fill(M,scale);

									//Just the Wjets bkg
									//Just the Wjets bkg
									if ((evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){
										histo_M_WJets_fail->Fill(M,scale);

									}
								}
							}
						}
					}
				}
			}
		}					

	}

	setfreestyle();
	//setTDRStyle();


	TCanvas* c1 = new TCanvas("c1","c1");
	histo_M_DYJets_bkg->Draw();
	//histo_M_DYJets_bkg->SetTitle("Invariant mass for "+pname+", "+(TString)_isocutpass+", "+(TString) _Ptrange+", "+(TString) _eta);
	histo_M_DYJets_bkg->SetTitle("Invariant mass for "+pname+", 100 to Inf HT, "+"tight, "+(TString) _Ptrange+", "+(TString) _eta);
	//histo_M_DYJets_bkg->SetTitle("Invariant mass for "+pname+", 100 to Inf HT, "+(TString) _Ptrange);
	histo_M_DYJets_bkg->GetXaxis()->SetTitle("m [GeV]");
	histo_M_DYJets_bkg->SetLineWidth(2);
	histo_M_DYJets_bkg->SetLineColor(4);
	histo_M_bkg->Draw("same");
	histo_M_bkg->SetLineWidth(2);
	histo_M_bkg->SetMarkerColor(2);
	histo_M_bkg->SetLineColor(2);
	histo_M_WJets->Draw("same");
	histo_M_WJets->SetLineWidth(3);
	histo_M_WJets->SetLineColor(3);
	histo_M_WJets->SetMarkerColor(3);
	TLegend* leg = new TLegend(0.5, 0.6,0.89,0.89);
	leg->AddEntry(histo_M_DYJets_bkg, "Z + TTJets + WJets");
	leg->AddEntry(histo_M_bkg, "TTJets + WJets");
	leg->AddEntry(histo_M_WJets, "WJets");
	leg->SetTextSize(0.05);
	leg->SetBorderSize(0);
	leg->SetTextFont(42);
	leg->Draw();

	TCanvas* c2 = new TCanvas("c2","c2");
	histo_M_DYJets_bkg_fail->Draw();
	//histo_M_DYJets_bkg_fail->SetTitle("Invariant mass for "+pname+", "+(TString)_isocutfail+", "+(TString) _Ptrange+", "+(TString) _eta);
	histo_M_DYJets_bkg_fail->SetTitle("Invariant mass for "+pname+", 100 to Inf HT, "+"not tight, "+(TString) _Ptrange+", "+(TString) _eta);
	//histo_M_DYJets_bkg_fail->SetTitle("Invariant mass for "+pname+", 100 to Inf HT, "+(TString) _Ptrange);
	histo_M_DYJets_bkg_fail->GetXaxis()->SetTitle("m [GeV]");
	histo_M_DYJets_bkg_fail->SetLineWidth(2);
	histo_M_DYJets_bkg_fail->SetLineColor(4);
	histo_M_DYJets_bkg_fail->SetMarkerColor(4);
	histo_M_bkg_fail->Draw("same");
	histo_M_bkg_fail->SetLineWidth(2);
	histo_M_bkg_fail->SetLineColor(2);
	histo_M_bkg_fail->SetMarkerColor(2);
	histo_M_WJets_fail->Draw("same");
	histo_M_WJets_fail->SetLineWidth(3);
	histo_M_WJets_fail->SetLineColor(3);
	leg->Draw();

	TFile* file_out = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/ZBkgInvM/ZInvM/"+(TString)_output+"_"+(TString)pname+".root","recreate");
	histo_M_DYJets_bkg->Write(histo_M_DYJets_bkg->GetName());
	histo_M_bkg->Write(histo_M_bkg->GetName());
	histo_M_DYJets_bkg_fail->Write(histo_M_DYJets_bkg_fail->GetName());
	histo_M_bkg_fail->Write(histo_M_bkg_fail->GetName());
	c1->Write();
	c2->Write();	
	file_out->Close();

	return 0;
}
