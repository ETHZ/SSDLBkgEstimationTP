#include "cmath"
#include "iostream"
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
#include "TChain.h"

void Z_InvMass()
{

int nbins = 25;
double Pt_cut = 0;
int mother_Id = 23;

//Read root file
TChain* tree = new TChain("treeProducerSusyFullHad");
tree->Add("/Users/GLP/Desktop/CERN_data/DYJetsM50_*.root");
Long64_t n = tree->GetEntries();

//Declaration of histograms
TH1D *histo_GenInvMass[2];// = new TH1D("histo_GenInvMass","m",100,70,110);
TH1D *histo_RecInvMass[2];// = new TH1D("histo_RecInvMass","m",100,70,110);

histo_GenInvMass[0] = new TH1D("histo_GenInvMass_e","m",100,70,110);
histo_RecInvMass[0] = new TH1D("histo_RecInvMass_e","m",100,70,110);
histo_GenInvMass[1] = new TH1D("histo_GenInvMass_mu","m",100,70,110);
histo_RecInvMass[1] = new TH1D("histo_RecInvMass_mu","m",100,70,110);

//Event variables Float_t dxy[200];
Float_t dz[200];
Float_t relIso03[200];
Float_t relIso04[200];
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
Float_t dxy[200];
Int_t GrMa[200];
Int_t ngenPart;
Int_t nrecPart;
Int_t nrecel;
Int_t nelectron;
Int_t nmuon;

//Assigne branches
//generated
tree->SetBranchAddress("genPart_motherId", &Mo);
tree->SetBranchAddress("genPart_pdgId", &Id);
tree->SetBranchAddress("genPart_pt", &Pt);
tree->SetBranchAddress("genPart_mass", &m);
tree->SetBranchAddress("genPart_eta", &gen_eta);
tree->SetBranchAddress("genPart_phi", &gen_phi);
tree->SetBranchAddress("genPart_charge", &charge);
//tree->SetBranchAddress("GenLep_pdgId", &Id);
//tree->SetBranchAddress("GenLep_pt",&Pt);
//tree->SetBranchAddress("GenLep_mass", &m);
//tree->SetBranchAddress("GenLep_eta",&gen_eta);
//tree->SetBranchAddress("GenLep_phi",&gen_phi);
//tree->SetBranchAddress("GenLep_charge", &charge);

tree->SetBranchAddress("genPart_status", &status);
tree->SetBranchAddress("ngenPart", &ngenPart);
tree->SetBranchAddress("genPart_grandmaId",&GrMa);
tree->SetBranchAddress("nElectrons10", &nelectron);
tree->SetBranchAddress("nMuons10",&nmuon);
//reconstructed
tree->SetBranchAddress("lep_pdgId", &recId);
tree->SetBranchAddress("nlep", &nrecPart);
tree->SetBranchAddress("lep_pt", &recPt);
tree->SetBranchAddress("lep_mass", &recm);
tree->SetBranchAddress("lep_eta", &rec_eta);
tree->SetBranchAddress("lep_phi", &rec_phi);
tree->SetBranchAddress("lep_charge", &reccharge);
tree->SetBranchAddress("lep_dxy", &dxy);
tree->SetBranchAddress("lep_dz", &dz);

//Start loop over all events
for (int ev = 0; ev < n; ++ev) {
  
	tree->GetEntry(ev);

        TLorentzVector genemin;
        TLorentzVector genepl;
        TLorentzVector genmumin;
        TLorentzVector genmupl;

        TLorentzVector recemin;
        TLorentzVector recepl;
        TLorentzVector recmumin;
        TLorentzVector recmupl;

	//gen e.
	int nepart = 0;
	int nepl = 0;
	int nemin = 0;
	double inveM  = 999;
	//gen mu.
	int nmupart = 0;
	int nmupl = 0;
	int nmumin = 0;
	double invmuM  = 999;
	//rec ele.
	int nepart2 = 0;
	int nepl2 = 0;
	int nemin2 = 0;
	double inveM2  = 999;
	//rec mu.
	int nmupart2 = 0;
	int nmupl2 = 0;
	int nmumin2 = 0;
	double invmuM2  = 999;

        //Loop over all generated
	for (int i= 0; i < ngenPart; ++i) {

	        if((abs(Id[i]) == 11)&&(abs(Mo[i]) == mother_Id)&&(abs(status[i]) == 1)&&(Pt[i] > Pt_cut)){
		 	++nepart;

	        TLorentzVector vec;
		vec.SetPtEtaPhiM(Pt[i],gen_eta[i],gen_phi[i],m[i]);
		        
			if((abs(charge[i] - 1) < 0.2)){
				++nepl;
			        genepl = vec;
			}

			if((abs(charge[i] + 1) < 0.2)){
				++nemin;
			        genemin = vec;
			}
		}

	        if((abs(Id[i]) == 13)&&(abs(Mo[i]) == mother_Id)&&(Pt[i] > Pt_cut)){
		 	++nmupart;

	        TLorentzVector vec;
		vec.SetPtEtaPhiM(Pt[i],gen_eta[i],gen_phi[i],m[i]);
		        
			if((abs(charge[i] - 1) < 0.2)){
				++nmupl;
			        genmupl = vec;
			}

			if((abs(charge[i] + 1) < 0.2)){
				++nmumin;
			        genmumin = vec;
			}
		}

	}

	for(int i = 0; i < nrecPart; ++i){

	        if((abs(recId[i]) == 11)&&(recPt[i] > Pt_cut)){
			++nepart2;

	        TLorentzVector vec2;
		vec2.SetPtEtaPhiM(recPt[i],rec_eta[i],rec_phi[i],recm[i]);
		
			if((abs(reccharge[i] - 1) < 0.2)){
				++nepl2;
			        recepl= vec2;
			}

			if((abs(reccharge[i] + 1) < 0.2)){
				++nemin2;
			        recemin = vec2;
			}
		}

	        if((abs(recId[i]) == 13)&&(recPt[i] > Pt_cut)){
			++nmupart2;

	        TLorentzVector vec2;
		vec2.SetPtEtaPhiM(recPt[i],rec_eta[i],rec_phi[i],recm[i]);
		
			if((abs(reccharge[i] - 1) < 0.2)){
				++nmupl2;
			        recmupl= vec2;
			}

			if((abs(reccharge[i] + 1) < 0.2)){
				++nmumin2;
			        recmumin = vec2;
			}
		}
	}	
	

if ((nepl == 1)&&(nemin == 1)&&(nepart == 2)){
	TLorentzVector Z =genemin+genepl;
	inveM= Z.M();

	cout<<"The invarian mass of electron is "<<inveM<<endl;

}

if ((nmupl == 1)&&(nmumin == 1)&&(nmupart == 2)){
	TLorentzVector Z =genmumin+genmupl;
	invmuM= Z.M();

	cout<<"The invarian mass of muon is "<<invmuM<<endl;
}

if ((nepl2 == 1)&&(nemin2 == 1)&&(nepart2 == 2)){
	TLorentzVector Z2 = recemin+recepl;
	inveM2 = Z2.M();
	
}

if ((nmupl2 == 1)&&(nmumin2 == 1)&&(nmupart2 == 2)){
	TLorentzVector Z2 = recmumin+recmupl;
	invmuM2 = Z2.M();
	
}

histo_GenInvMass[0]->Fill(inveM);
histo_GenInvMass[1]->Fill(invmuM);
histo_RecInvMass[0]->Fill(inveM2);
histo_RecInvMass[1]->Fill(invmuM2);

}


TCanvas* c1 = new TCanvas("c1","c1");

c1->Divide(1,2);

c1->cd(1);
histo_GenInvMass[0]->Draw();
histo_GenInvMass[0]->SetLineWidth(2);
histo_GenInvMass[0]->SetTitle("Z inv. mass for gen. and rec. electrons"); 
histo_GenInvMass[0]->GetXaxis()->SetTitle("Mass [GeV]");
histo_RecInvMass[0]->Draw("same");
histo_RecInvMass[0]->SetLineWidth(2);
histo_RecInvMass[0]->SetLineColor(2);

c1->cd(2);

histo_GenInvMass[1]->Draw();
histo_GenInvMass[1]->SetLineWidth(2);
histo_GenInvMass[1]->SetTitle("Z inv. mass for gen. and rec. muons"); 
histo_GenInvMass[1]->GetXaxis()->SetTitle("Mass [GeV]");
histo_RecInvMass[1]->Draw("same");
histo_RecInvMass[1]->SetLineWidth(2);
histo_RecInvMass[1]->SetLineColor(2);

//Store result in .root file
TFile* file_out = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/Z_InvMass.root","recreate");

histo_GenInvMass[0]->Write();
histo_RecInvMass[0]->Write();
histo_GenInvMass[1]->Write();
histo_RecInvMass[1]->Write();

file_out->Close();

}

