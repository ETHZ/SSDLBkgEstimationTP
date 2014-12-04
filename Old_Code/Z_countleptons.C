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

void Z_countleptons(){


int nbins = 25;
double Pt_cut = 0;
int mother_Id = 23;

//Read root file
TFile *file = new TFile("/Users/GLP/Desktop/CERN_data/DYJetsM50_HT200to400_PU_S14_POSTLS170_babytree.root");
TTree *tree = (TTree*) file->Get("treeProducerSusyFullHad");
Long64_t n = tree->GetEntries();

//Declaration of histograms
TH1D* histo_nelectron = new TH1D("histo_nelectron","ne", 10,0,10);
TH1D* histo_nmuon = new TH1D("histo_nmuon","nmu", 10,0,10);

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
Float_t dxy[200];
Int_t GrMa[200];
Int_t ngenPart;
Int_t nrecPart;
Int_t nrecel;
Int_t nelectron;
Int_t nmuon;

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

//Loop on e and mu
for(int lep = 0; lep < 2; ++lep){

	int leptonId;
	int nleptons;
	TString pname;

//Start loop over all events

	for (int ev = 0; ev < n; ++ev) {
  
	tree->GetEntry(ev);

	int electron_event = 0;
	int muon_event = 0;

	if(lep == 0){ leptonId = 11; pname = "e"; nleptons = nelectron;}
	else if(lep == 1){leptonId = 13; pname ="#mu"; nleptons = nmuon;}


        //Loop over all generated
	for (int i = 0; i < ngenPart; ++i) {

	        if((abs(Id[i]) == leptonId)&&(abs(Mo[i]) == mother_Id)&&(Pt[i] > Pt_cut)){

			if(lep == 0){++electron_event;}
			if(lep == 1){++muon_event;}

		}	
		
	}

cout<<"nelectron "<<electron_event<<endl;
cout<<"nmuon "<<muon_event<<endl;
histo_nelectron->Fill(electron_event);
histo_nmuon->Fill(muon_event);

}
}

TCanvas* c1 = new TCanvas("c1","c1");

histo_nelectron->Draw();
histo_nelectron->SetLineWidth(2);
histo_nmuon->Draw("same");
histo_nmuon->SetLineWidth(2);
histo_nmuon->SetLineColor(2);
histo_nmuon->SetLineStyle(2);

}
