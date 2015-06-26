/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

 * File Name : AddMatching.C++

 * Purpose : Takes a TTree and keeps only matched leptons

 * Created By : Gaël Perrin

 _._._._._._._._._._._._._._._._._._._._._.*/
#include <cmath>

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "TFile.h"
#include "TMath.h"
#include "TVector2.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"

#include "../tools/DeltaR.C"

int AddMatching_phys14(){

  ///////////////
  //Get the TTree
  ///////////////

  //TString _path = "/shome/gaperrin/CERN_data/newcsasample/postprocessed/";
  //TString _path = "/Users/GLP/Desktop/CERN_data/dyjetsnew/postprocessed/";
  TString _path = "/shome/gaperrin/CERN_data/phys14sample/DYJets/postprocessed/";//phys14
  //TString _path = "dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat/store/user/gaperrin/data/phys14/TTJets_v3/";

  //TString _root = "WJetsToLNu_13TeV-madgraph-pythia8-tauola.root";
  //TString _root = "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170.root";
  //TString _root = "DYJetsToLLM50_PU_S14_POSTLS170.root";
  //TString _root = "dyjetsnew.root";
  TString _root = "tree.root";//phy14

  TString _tree = "tree";

  bool bkg = true;


  TFile *in = TFile::Open(_path+_root, "READ");
  TTree* t = (TTree*)in->Get(_tree);

  TFile *out = TFile::Open(_path+"matched/"+_root, "RECREATE","",0);

  //Clone the tree
  TTree *clone;
  clone = t->CloneTree(-1,"fast");

  cout << "Cloning is DONE!!!" << endl;

  //New Branch
  int Gmatched[4];
  int Omatched[4];

  TBranch* Gb = clone->Branch("LepGood_matched", &Gmatched, "LepGood_matched[4]/I");
  TBranch* Ob = clone->Branch("LepOther_matched", &Omatched, "LepGood_matched[4]/I");
  //clone->Branch("LepGood_matched",&matched,"matching");
  //Check if the lepton are matched
  //
  //Event variables
  Int_t evt_id;
  Float_t scale;
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
  //not loose
  Int_t On;
  Int_t Oid[200];
  Float_t Opt[200];
  Float_t Om[200];
  Float_t Oeta[200];
  Float_t Ophi[200];
  Int_t   Oq[200];
  Int_t Otight[200];
  Int_t Otighte[200];
  Float_t Oiso3[200];
  Float_t Oiso4[200];
  Float_t Ochiso3[200];
  Float_t Ochiso4[200];
  Float_t Odxy[200];
  Float_t Odz[200];
  Int_t Ofromtau[200];
  //loose
  Int_t 	Gn;
  Int_t 	Gid[200];
  Float_t 	Gpt[200];
  Float_t 	Gm[200];
  Float_t 	Geta[200];
  Float_t 	Gphi[200];
  Int_t   	Gq[200];
  Int_t 	Gtight[200];
  Int_t 	Gtighte[200];
  Float_t 	Giso3[200];
  Float_t 	Giso4[200];
  Float_t 	Gchiso3[200];
  Float_t 	Gchiso4[200];
  Float_t 	Gdxy[200];
  Float_t 	Gdz[200];
  Int_t         Gfromtau[200];

  //Assigne branches tree->SetBranchAddress("evt_scale1fb", &scale);
  clone->SetBranchAddress("evt_id", &evt_id);
  //generated
  clone->SetBranchAddress("nGenPart", &ngenPart);
  clone->SetBranchAddress("GenPart_pdgId", &Id);
  clone->SetBranchAddress("GenPart_sourceId", &source);
  clone->SetBranchAddress("GenPart_motherId", &Mo);
  clone->SetBranchAddress("GenPart_eta", &gen_eta);
  clone->SetBranchAddress("GenPart_phi", &gen_phi);
  clone->SetBranchAddress("GenPart_pt", &Pt);
  clone->SetBranchAddress("GenPart_mass", &m);
  clone->SetBranchAddress("GenPart_charge", &charge);
  clone->SetBranchAddress("GenPart_status", &status);
  //not loose
  clone->SetBranchAddress("nLepOther",&On);
  clone->SetBranchAddress("LepOther_pdgId",&Oid);
  clone->SetBranchAddress("LepOther_pt",&Opt);
  clone->SetBranchAddress("LepOther_mass",&Om);
  clone->SetBranchAddress("LepOther_eta",&Oeta);
  clone->SetBranchAddress("LepOther_phi",&Ophi);
  clone->SetBranchAddress("LepOther_charge",&Oq);
  clone->SetBranchAddress("LepOther_tightId",&Otight);
  clone->SetBranchAddress("LepOther_eleCutIdCSA14_50ns_v1",&Otighte);
  clone->SetBranchAddress("LepOther_relIso03",&Oiso3);
  clone->SetBranchAddress("LepOther_relIso04",&Oiso4);
  clone->SetBranchAddress("LepOther_chargedHadRelIso03",&Ochiso3);
  clone->SetBranchAddress("LepOther_chargedHadRelIso04",&Ochiso4);
  clone->SetBranchAddress("LepOther_dxy",&Odxy);
  clone->SetBranchAddress("LepOther_dz",&Odz);
  clone->SetBranchAddress("LepOther_mcMatchTau",&Ofromtau);
  //Loose
  clone->SetBranchAddress("nLepGood",&Gn);
  clone->SetBranchAddress("LepGood_pdgId",&Gid);
  clone->SetBranchAddress("LepGood_pt",&Gpt);
  clone->SetBranchAddress("LepGood_mass",&Gm);
  clone->SetBranchAddress("LepGood_eta",&Geta);
  clone->SetBranchAddress("LepGood_phi",&Gphi);
  clone->SetBranchAddress("LepGood_charge",&Gq);
  clone->SetBranchAddress("LepGood_tightId",&Gtight);
  clone->SetBranchAddress("LepGood_eleCutIdCSA14_50ns_v1",&Gtighte);
  clone->SetBranchAddress("LepGood_relIso03",&Giso3);
  clone->SetBranchAddress("LepGood_relIso04",&Giso4);
  clone->SetBranchAddress("LepGood_chargedHadRelIso03",&Gchiso3);
  clone->SetBranchAddress("LepGood_chargedHadRelIso04",&Gchiso4);
  clone->SetBranchAddress("LepGood_dxy",&Gdxy);
  clone->SetBranchAddress("LepGood_dz",&Gdz);
  clone->SetBranchAddress("LepGood_mcMatchTau",&Gfromtau);


  //Start loop over all events
  for (int k = 0; k < clone->GetEntries(); ++k) {

    clone->GetEntry(k);

    Gmatched[0] = -1;
    Gmatched[1] = -1;
    Gmatched[2] = -1;
    Gmatched[3] = -1;

    Omatched[0] = -1;
    Omatched[1] = -1;
    Omatched[2] = -1;
    Omatched[3] = -1;

    //loop on reconstructed
    for(int j=0; j<Gn;++j){

      if(bkg == true){Gmatched[j] = 1;}
      else if(bkg == false){

	//Variable for matching
	double R = 999;
	double delta_P = 999;
	double delta_charge = 999;

	//loop over all generated particles to do the matching
	for (int i = 0; i < ngenPart; ++i) {

	  //Has to be the same part
	  if((Id[i] == Gid[j])&&(abs(Mo[i]) != 15)){ 

	    //Electrons selection
	    double R2 = DeltaR(gen_eta[i],Geta[j],gen_phi[i],Gphi[j] );

	    //Minimise DeltaR and Fill the other variables
	    if (R > R2) {
	      R = R2;
	      //delta_P = abs(Gpt[j]-Pt[i])/Pt[i];
	      //delta_charge = abs(Gq[j] - charge[i]);
	    }
	  }
	}

	//Fill
	if(R<0.1){
	  Gmatched[j] = 1;
	}else{Gmatched[j] = 0;}
      }

    }

    //loop on other
    for(int j=0; j<On;++j){

      if(bkg == true){Omatched[j] = 1;}
      else if(bkg == false){

	//Variable for matching
	double R = 999;
	//double delta_P = 999;
	//double delta_charge = 999;

	//loop over all generated particles to do the matching
	for (int i = 0; i < ngenPart; ++i) {

	  //Has to be the same part
	  if((Id[i] == Oid[j])&&(abs(Mo[i]) != 15)){ 

	    //Electrons selection
	    double R2 = DeltaR(gen_eta[i],Oeta[j],gen_phi[i],Ophi[j] );

	    //Minimise DeltaR and Fill the other variables
	    if (R > R2) {
	      R = R2;
	      //delta_P = abs(Opt[j]-Pt[i])/Pt[i];
	      //delta_charge = abs(Oq[j] - charge[i]);
	    }
	  }
	}

	//Fill
	if(R<0.1){
	  Omatched[j] = 1;
	}else{Omatched[j] = 0;}
      }

    }

    Gb->Fill();
    Ob->Fill();

  }

  clone->Write();
  out->Close();
  in->Close();
  return 0;
}
