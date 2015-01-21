/*********************************************
 * Description - The matching between gen and rec leptons leads to different results in dytree_new and dytree_old. This file studies the differences using common events
 * Author - Gaël L. Perrin
 * Date - Jan 15 2015
 * *******************************************/


#include "cmath"
#include "TChain.h"
#include <sys/stat.h>
#include "iostream"
#include "fstream"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TBranch.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TString.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TLorentzVector.h"
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
#include <map>
#include <vector>
#include "../tools/DeltaR.C"

struct Event{

  double PT;
  double ETA;
  double PHI;
  double DR;
  double RELISO3;
  int NGEN;

};

//Function to reorder the vector
bool mycompare(Event a, Event b){

  if(a.PT == b.PT){

    return(a.ETA < b.ETA);

  }else{ return(a.PT < b.PT);}

}

map<int, vector<Event> > getdyjetsnewmap(TTree* tree_new, TString option);


int Compare(TString option = "short"){

  //Retrieve the trees

  TString location = "/Users/GLP/Desktop/CERN_data/dyjetsnew/postprocessed/";

  TChain* tree_new = new TChain("treeProducerSusyMultilepton");
  TChain* tree_good = new TChain("treeProducerSusyMultilepton");

  tree_good->Add(location+"dyjets_good.root");
  tree_new->Add(location+"dyjetsnew.root");

  Int_t ngood = tree_good->GetEntries();
  Int_t nnew = tree_new->GetEntries();

  map<int, vector<Event> > map_good;
  map<int, vector<Event> > map_new = getdyjetsnewmap(tree_new, option);


  /////////////////////
  //Define histograms//
  /////////////////////
  
TH1D*	h_dRlep = new TH1D("h_dRlep","h",50,-10,10);
TH1D*	h_dGenlep = new TH1D("h_dGenlep","h",50,-10,10);
TH1D*	h_dRmatch = new TH1D("h_dRmatch","h",50,-10,10);
TH1D*	h_dIso3lep = new TH1D("h_dRmatch","h",100,-0.5,0.5);

  ///////////////
  //dyjets_new //
  ///////////////

  //Event variables
  Int_t evt;
  //Generated
  Double_t gen_phi[200];
  Double_t gen_eta[200];
  Double_t Pt[200];
  Double_t m[200];
  Int_t Id[200];
  Int_t Mo[200];
  Double_t charge[200];
  Int_t status[200];
  Int_t GrMa[200];
  Int_t ngenPart;
  Int_t source[200];
  //not loose
  Int_t On;
  Int_t Oid[200];
  Double_t Opt[200];
  Double_t Oeta[200];
  Double_t Ophi[200];
  Double_t Oiso3[200];
  Int_t Otighte[200];
  //loose
  Int_t 	Gn;
  Int_t 	Gid[200];
  Double_t 	Gpt[200];
  Double_t       Geta[200];
  Double_t       Gphi[200];
  Double_t       Giso3[200];
  Int_t 	Gtighte[200];

  tree_good->SetBranchAddress("evt", &evt);
  //generated
  tree_good->SetBranchAddress("nGenPart", &ngenPart);
  tree_good->SetBranchAddress("GenPart_pdgId", &Id);
  tree_good->SetBranchAddress("GenPart_motherId", &Mo);
  tree_good->SetBranchAddress("GenPart_eta", &gen_eta);
  tree_good->SetBranchAddress("GenPart_phi", &gen_phi);
  tree_good->SetBranchAddress("GenPart_pt", &Pt);
  //not loose
  tree_good->SetBranchAddress("nLepOther",&On);
  tree_good->SetBranchAddress("LepOther_pdgId",&Oid);
  tree_good->SetBranchAddress("LepOther_pt",&Opt);
  tree_good->SetBranchAddress("LepOther_eta",&Oeta);
  tree_good->SetBranchAddress("LepOther_phi",&Ophi);
  tree_good->SetBranchAddress("LepOther_relIso03",&Oiso3);
  tree_good->SetBranchAddress("LepOther_eleCutIdCSA14_50ns_v1",&Otighte);
  //Loose
  tree_good->SetBranchAddress("nLepGood",&Gn);
  tree_good->SetBranchAddress("LepGood_pdgId",&Gid);
  tree_good->SetBranchAddress("LepGood_pt",&Gpt);
  tree_good->SetBranchAddress("LepGood_eta",&Geta);
  tree_good->SetBranchAddress("LepGood_phi",&Gphi);
  tree_good->SetBranchAddress("LepGood_relIso03",&Giso3);
  tree_good->SetBranchAddress("LepGood_eleCutIdCSA14_50ns_v1",&Gtighte);

  if(option == "short"){ngood = 10000;}

  for (int k = 0; k < ngood; ++k){

    //Check if event is contained in the map_new
      tree_good->GetEntry(k);
    if(map_new.count(evt) > 0){

      //cout<<"is contained !"<<endl;

      //Declaration of Event parameters
      Int_t 	evtloose[200];
      Int_t 	evtid[200];
      Double_t 	evtpt[200];
      Double_t 	evteta[200];
      Double_t 	evtphi[200];
      Double_t 	evtiso3[200];
      Int_t 	evttighte[200];

      //run in all leptons in the events to store vector of pt
      vector<Event> vect;
      Event ev;

      for(int j = 0; j < Gn+On; ++j){

	//cout<<"LepGood + LepOther is "<<Gn+On<<endl;

	//Loose events
	if(j < On){

	  evtloose[j]			  = 0;
	  evtid[j]                      = Oid[j];
	  evtpt[j]                      = Opt[j];
	  evteta[j]		      = Oeta[j];
	  evtphi[j]		      = Ophi[j];
	  evtiso3[j]		      = Oiso3[j];
	  evttighte[j]                  = Otighte[j];

	  //Good events
	}else if((j >=  On)&&(j < Gn+On)){

	  evtloose[j]		       = 1;
	  evtid[j]                   = Gid[j-On];
	  evtpt[j]                   = Gpt[j-On];
	  evteta[j]		      = Geta[j-On];
	  evtphi[j]		      = Gphi[j-On];
	  evtiso3[j]		      = Giso3[j-On];
	  evttighte[j]               = Gtighte[j-On];
	}

	if(abs(evtid[j]) == 11){//Only interested in electrons


	  //Variable for matching
	  double R = 999;
	  double delta_P = 999;
	  int nlep = 0;//count the number of generated leptons

	  //loop over all generated particles to do the matching
	  for (int i = 0; i < ngenPart; ++i) {//to count the number of electrons in the genpart
	    if((abs(Id[i]) == 11)&&(abs(Mo[i]) != 15)){ 
	      ++nlep;
	      if(Id[i] == evtid[j]){ 

		//Electrons selection
		double R2 = DeltaR(gen_eta[i],evteta[j],gen_phi[i],evtphi[j] );

		//Minimise DeltaR and Fill the other variables
		if (R > R2) {
		  R = R2;
		  delta_P = abs(evtpt[j]-Pt[i])/Pt[i];
		}
	      }
	    }
	  }

	  //This is only filled if the rec particle is an electron
	  ev.DR = R;
	  ev.NGEN = nlep;
	  ev.PHI = evtphi[j];
	  ev.ETA = evteta[j];
	  ev.PT = evtpt[j];
	  ev.RELISO3 = evtiso3[j];
	  vect.push_back(ev);

	}
      }

    //cout<<"Good: The size of the event "<<evt<<" is "<<vect.size()<<endl;

      //Put vector in increasing pt order
      sort(vect.begin(),vect.end(),mycompare);
      //Add vector to the map with the corresponding evt number
      map_good[evt] = vect;

      ////////////////////////
      //Compare the two maps//
      ////////////////////////

      if(map_good[evt].size() == map_new[evt].size()){//Should have same number of lepton in the events
	for(int i  = 0; i< map_good[evt].size();++i){

	double dRlep = 9999;//The difference between the _new and _good leptons
	int dGenlep = 9999;//The difference of the number of generated lep 
	double dRmatch = 9999;//The diffrence of the matching dR between two leptons
	double dIso3lep = 9999;

	dRlep = DeltaR(map_new[evt][i].ETA,map_good[evt][i].ETA,map_new[evt][i].PHI,map_new[evt][i].PHI);

	dGenlep = map_new[evt][i].NGEN - map_good[evt][i].NGEN;

	dRmatch = (map_new[evt][i].DR - map_good[evt][i].DR);
	  if(dRlep != 0){dRlep = dRlep/map_new[evt][i].DR;}

	dIso3lep = map_new[evt][i].RELISO3 - map_good[evt][i].RELISO3;

	h_dRlep->Fill(dRlep);
	h_dGenlep->Fill(dGenlep);
	h_dRmatch->Fill(dRmatch);
	h_dIso3lep->Fill(dIso3lep);

	if((dRlep !=0)||(dGenlep != 0)||(dRmatch != 0)){
	cout<<"this is the event "<<evt<<endl;
	cout<<"the pt of the new is "<<map_new[evt][i].PT<<endl;
	cout<<"the pt of the good is "<<map_good[evt][i].PT<<endl;
	cout<<"the phi of the new is "<<map_new[evt][i].PHI<<endl;
	cout<<"the phi of the good is "<<map_good[evt][i].PHI<<endl;
	cout<<"the eta of the new is "<<map_new[evt][i].ETA<<endl;
	cout<<"the eta of the good is "<<map_good[evt][i].ETA<<endl;
	cout<<"dRmatch is "<<dRmatch<<endl;
	cout<<"dGenlep is "<<dGenlep<<endl;
	cout<<"dRlep is "<<dRlep<<endl;
	cout<<"dIso3lep is "<<dIso3lep<<endl;
	cout<<""<<endl;

	if(dGenlep == 0){cout<<"Error, same number of genlep and different !"<<endl;return 1;}

	}
	}

      }else{cout<<"Error have different size !"<<endl; 
	//cout<<"This happens in event "<<evt<<endl;
	//cout<<"Size of the new is "<<map_new[evt].size()<<endl;
	//cout<<"Size of the good is "<<map_good[evt].size()<<endl;
	
	return 1;}
    }
  }

  ///////////////////////
  //Plot the histograms//
  ///////////////////////

  TCanvas* c1 = new TCanvas("c1","c1");
  h_dRlep->Draw();
  TCanvas* c2 = new TCanvas("c2","c2");
  h_dGenlep->Draw();
  TCanvas* c3 = new TCanvas("c3","c3");
  h_dRmatch->Draw();
  TCanvas* c4 = new TCanvas("c4","c4");
  h_dIso3lep->Draw();

  return 0;

}

map<int, vector<Event> > getdyjetsnewmap(TTree* tree_new, TString option){

  Int_t nnew = tree_new->GetEntries();
  map<int, vector<Event> > map_new;

  ///////////////
  //dyjets_new //
  ///////////////

  //Define histogram to save _new only
  TH1D* histo = new TH1D("histo","h",5,0,5);

  //Event variables
  Int_t evt;
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
  //not loose
  Int_t On;
  Int_t Oid[200];
  Float_t Opt[200];
  Float_t Oeta[200];
  Float_t Ophi[200];
  Float_t Oiso3[200];
  Int_t Otighte[200];
  //loose
  Int_t 	Gn;
  Int_t 	Gid[200];
  Float_t 	Gpt[200];
  Float_t       Geta[200];
  Float_t       Gphi[200];
  Float_t       Giso3[200];
  Int_t 	Gtighte[200];

  //generated
  tree_new->SetBranchAddress("ngenLep", &ngenPart);
  tree_new->SetBranchAddress("genLep_pdgId", &Id);
  tree_new->SetBranchAddress("genLep_sourceId", &source);
  tree_new->SetBranchAddress("genLep_eta", &gen_eta);
  tree_new->SetBranchAddress("genLep_phi", &gen_phi);
  tree_new->SetBranchAddress("genLep_pt", &Pt);
  tree_new->SetBranchAddress("genLep_mass", &m);
  tree_new->SetBranchAddress("genLep_charge", &charge);
  tree_new->SetBranchAddress("genLep_status", &status);
  tree_new->SetBranchAddress("evt", &evt);
  //not loose
  tree_new->SetBranchAddress("nLepOther",&On);
  tree_new->SetBranchAddress("LepOther_pdgId",&Oid);
  tree_new->SetBranchAddress("LepOther_pt",&Opt);
  tree_new->SetBranchAddress("LepOther_eta",&Oeta);
  tree_new->SetBranchAddress("LepOther_phi",&Ophi);
  tree_new->SetBranchAddress("LepOther_relIso03",&Oiso3);
  tree_new->SetBranchAddress("LepOther_eleCutIdCSA14_50ns_v1",&Otighte);
  //Loose
  tree_new->SetBranchAddress("nLepGood",&Gn);
  tree_new->SetBranchAddress("LepGood_pdgId",&Gid);
  tree_new->SetBranchAddress("LepGood_pt",&Gpt);
  tree_new->SetBranchAddress("LepGood_eta",&Geta);
  tree_new->SetBranchAddress("LepGood_phi",&Gphi);
  tree_new->SetBranchAddress("LepGood_relIso03",&Giso3);
  tree_new->SetBranchAddress("LepGood_eleCutIdCSA14_50ns_v1",&Gtighte);

  if(option == "short"){nnew = 10000;}

  for (int k = 0; k < nnew; ++k){

    tree_new->GetEntry(k);

    //Declaration of Event parameters
    Int_t 	evtloose[200];
    Int_t 	evtid[200];
    Float_t 	evtpt[200];
    Float_t 	evteta[200];
    Float_t 	evtphi[200];
    Float_t 	evtiso3[200];
    Int_t 	evttighte[200];

    //run in all leptons in the events to store vector of pt
    vector<Event> vect;
    Event ev;

    for(int j = 0; j < Gn+On; ++j){

      //cout<<"LepGood + LepOther is "<<Gn+On<<endl;

      //Loose events
      if(j < On){

	evtloose[j]			  = 0;
	evtid[j]                      = Oid[j];
	evtpt[j]                      = Opt[j];
	evteta[j]		      = Oeta[j];
	evtphi[j]		      = Ophi[j];
	evtiso3[j]		      = Oiso3[j];
	evttighte[j]                  = Otighte[j];

	//Good events
      }else if((j >=  On)&&(j < Gn+On)){

	evtloose[j]		       = 1;
	evtid[j]                   = Gid[j-On];
	evtpt[j]                   = Gpt[j-On];
	evteta[j]		      = Geta[j-On];
	evtphi[j]		      = Gphi[j-On];
	evtiso3[j]		      = Giso3[j-On];
	evttighte[j]               = Gtighte[j-On];
      }

      if(abs(evtid[j]) == 11){//Only interested in electrons

	//Matching here
	//

	//Variable for matching
	double R = 999;
	double delta_P = 999;
	int nlep = 0;//count the number of generated leptons

	//loop over all generated particles to do the matching
	for (int i = 0; i < ngenPart; ++i) {//to count the number of electrons in the genpart
	  if(abs(Id[i]) == 11){ 
	    ++nlep;
	    if(Id[i] == evtid[j]){ 

	      //Electrons selection
	      double R2 = DeltaR(gen_eta[i],evteta[j],gen_phi[i],evtphi[j] );

	      //Minimise DeltaR and Fill the other variables
	      if (R > R2) {
		R = R2;
		delta_P = abs(evtpt[j]-Pt[i])/Pt[i];
	      }
	    }
	  }
	}

	//This is only filled if the rec particle is an electron
	ev.DR = R;
	ev.NGEN = nlep;
	ev.PHI = evtphi[j];
	ev.ETA = evteta[j];
	ev.PT = evtpt[j];
	ev.RELISO3 = evtiso3[j];
	vect.push_back(ev);
      }
    }

    //Put vector in increasing pt order
    sort(vect.begin(),vect.end(),mycompare);
    //Add vector to the map with the corresponding evt number
    map_new[evt] = vect; 

    //cout<<"The size of the event "<<evt<<" is "<<vect.size()<<endl;
  
  }

  return map_new;

}
