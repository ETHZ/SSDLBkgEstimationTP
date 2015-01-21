/*********************************************
 * Description - Compare the dyjets_new and dyjets_good evt by evt
 * Author - Gaël L. Perrin
 * Date - Jan 14 2015
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
#include <algorithm>

//Function to reorder the vector
bool mycompare(pair< Int_t, Float_t> a, pair< Int_t, Float_t> b){

  return(a.second < b.second);

}

map<int, vector<Int_t> > getdyjetsnewmap(TTree* tree_new);

int Compare(){

  //Retrieve the trees

  TString location = "/Users/GLP/Desktop/CERN_data/dyjetsnew/postprocessed/";

  TChain* tree_new = new TChain("treeProducerSusyMultilepton");
  TChain* tree_good = new TChain("treeProducerSusyMultilepton");

  tree_good->Add(location+"dyjets_good.root");
  tree_new->Add(location+"dyjetsnew.root");

  Int_t ngood = tree_good->GetEntries();
  Int_t nnew = tree_new->GetEntries();

  //Define histograms to compare

  TH1D* hgood = new TH1D("hgood","h",5,0,5);
  TH1D* hnew  = new TH1D("hnew","h",5,0,5);
  TH1D* histo = new TH1D("histo","h",5,0,5);//Histo that stores everything independently of matching with _new

  //Declare map

  //cout<<"Debug1"<<endl;

  map<int, vector<Int_t> > map_new = getdyjetsnewmap(tree_new);
  map<int, vector<Int_t> > map_good;

  ///////////////
  //dyjets_good//
  ///////////////

  //Event variables
  Int_t evt;
  //not loose
  Int_t On;
  Int_t Oid[200];
  Double_t Opt[200];
  Int_t Otighte[200];
  //loose
  Int_t 	Gn;
  Int_t 	Gid[200];
  Double_t 	Gpt[200];
  Int_t 	Gtighte[200];

  tree_good->SetBranchAddress("evt", &evt);
  //not loose
  tree_good->SetBranchAddress("nLepOther",&On);
  tree_good->SetBranchAddress("LepOther_pdgId",&Oid);
  tree_good->SetBranchAddress("LepOther_pt",&Opt);
  tree_good->SetBranchAddress("LepOther_eleCutIdCSA14_50ns_v1",&Otighte);
  //Loose
  tree_good->SetBranchAddress("nLepGood",&Gn);
  tree_good->SetBranchAddress("LepGood_pdgId",&Gid);
  tree_good->SetBranchAddress("LepGood_pt",&Gpt);
  tree_good->SetBranchAddress("LepGood_eleCutIdCSA14_50ns_v1",&Gtighte);

  //cout<<"Start looping on dyjets_good"<<endl;

  //ngood = 100000;

  for (int k = 0; k < ngood; ++k) {
    tree_good->GetEntry(k);
    //First check if the evt is included in the map


    //cout<<"Matches !"<<endl;

    //Declaration of event parameters
    Int_t 	evtid[200];
    Double_t 	evtpt[200];
    Int_t 	evttighte[200];

    //run in all leptons in the events to store vector of pt
    vector< pair<Int_t, Float_t> > vect;
    pair<Int_t, Float_t> p;

    for(int j = 0; j < Gn+On; ++j){

      //Loose events
      if(j < On){

	evtid[j]                      = Oid[j];
	evtpt[j]                      = Opt[j];
	evttighte[j]                  = Otighte[j];

	//Good events
      }else if((j >=  On)&&(j < Gn+On)){

	evtid[j]                   = Gid[j-On];
	evtpt[j]                   = Gpt[j-On];
	evttighte[j]               = Gtighte[j-On];

      }

      if(abs(evtid[j]) == 11){
	histo->Fill(evttighte[j]);

	//Fill common histo only if matched
	if(map_new.count(evt) > 0){

	  //Fill the vector
	  p.first = evttighte[j];
	  p.second = evtpt[j];

	  vect.push_back(p);


	}
      }
    }

      //Put vector in increasing pt order
      sort(vect.begin(),vect.end(),mycompare);

      //Fill map vector
      vector<Int_t> map_vect;

      for(int i = 0; i < vect.size(); ++i){

	map_vect.push_back(vect[i].first);

      }
    

      //Add vector to the map with the corresponding evt number
      map_good[evt] = map_vect;

      ////////////////////////
      //Compare the two maps//
      ////////////////////////

      //cout<<"size for event "<<evt<<" good is "<<map_good[evt].size()<<endl;
      //cout<<"size for event "<<evt<<" new  is "<<map_new[evt].size()<<endl;

      if(map_good[evt].size() == map_new[evt].size()){//Should have same number of lepton in the events

	bool identical = true;

	for(int i  = 0; i< map_good[evt].size();++i){

	  if(map_good[evt][i] != map_good[evt][i]){identical = false;}

	}

	for(int i = 0;i < map_good[evt].size(); ++i){
	  if(identical == true){

	    //cout<<"Stuff are filled !"<<endl;

	    hnew->Fill(map_new[evt][i]);
	    hgood->Fill(map_good[evt][i]);
	  }else{cout<<"Non identical branch !"<<endl;return 1;}
	}
      }
    }
  

  ///////////////////////
  //Fill histo and plot//
  ///////////////////////

  TCanvas* c1 = new TCanvas("c1","c1");

  hnew->Draw();
  hnew->SetLineColor(2);
  hgood->Draw("same");
  hgood->SetLineStyle(2);
  TString _path = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_eff/";
  TFile* file_out = new TFile(_path+"compare_evt_dytree"+".root","update");

  hnew->Write("hnew");
  hgood->Write("hgood");
  histo->Write("histo_good");

  file_out->Close();

  return 0;

}

map<int, vector<Int_t> > getdyjetsnewmap(TTree* tree_new){

  //cout<<"Debug2"<<endl;

  Int_t nnew = tree_new->GetEntries();

  //nnew = 100; 

  map<int, vector<Int_t> > map_new;

  //cout<<"Debug3"<<endl;

  ///////////////
  //dyjets_new //
  ///////////////

  //Define histogram to save _new only
  TH1D* histo = new TH1D("histo","h",5,0,5);

  //Event variables
  Int_t evt;
  //not loose
  Int_t On;
  Int_t Oid[200];
  Float_t Opt[200];
  Int_t Otighte[200];
  //loose
  Int_t 	Gn;
  Int_t 	Gid[200];
  Float_t 	Gpt[200];
  Int_t 	Gtighte[200];
  //cout<<"Debug4"<<endl;

  //Assigne branches tree->SetBranchAddress("evt_scale1fb", &scale);
  tree_new->SetBranchAddress("evt", &evt);
  //not loose
  tree_new->SetBranchAddress("nLepOther",&On);
  tree_new->SetBranchAddress("LepOther_pdgId",&Oid);
  tree_new->SetBranchAddress("LepOther_pt",&Opt);
  tree_new->SetBranchAddress("LepOther_eleCutIdCSA14_50ns_v1",&Otighte);
  //Loose
  tree_new->SetBranchAddress("nLepGood",&Gn);
  tree_new->SetBranchAddress("LepGood_pdgId",&Gid);
  tree_new->SetBranchAddress("LepGood_pt",&Gpt);
  tree_new->SetBranchAddress("LepGood_eleCutIdCSA14_50ns_v1",&Gtighte);
  //cout<<"Debug5"<<endl;

  for (int k = 0; k < nnew; ++k) {


    tree_new->GetEntry(k);

    //Declaration of event parameters
    Int_t 	evtloose[200];
    Int_t 	evtid[200];
    Float_t 	evtpt[200];
    Int_t 	evttighte[200];
    //cout<<"Debug6"<<endl;

    //run in all leptons in the events to store vector of pt
    vector< pair<Int_t, Float_t> > vect;
    pair<Int_t, Float_t> p;

    //cout<<"Debug7"<<endl;
    for(int j = 0; j < Gn+On; ++j){

      //cout<<"LepGood + LepOther is "<<Gn+On<<endl;

      //Loose events
      if(j < On){

	evtloose[j]			  = 0;
	evtid[j]                      = Oid[j];
	evtpt[j]                      = Opt[j];
	evttighte[j]                  = Otighte[j];

	//Good events
      }else if((j >=  On)&&(j < Gn+On)){

	evtloose[j]		       = 1;
	evtid[j]                   = Gid[j-On];
	evtpt[j]                   = Gpt[j-On];
	evttighte[j]               = Gtighte[j-On];
      }
      //cout<<"Debug8"<<endl;

      if(abs(evtid[j]) == 11){

	//Fill the vector
	p.first = evttighte[j];
	p.second = evtpt[j];

	//File histo
	histo->Fill(evttighte[j]);

	//cout<<"pfirst is"<<evttighte[j]<<endl;
	//cout<<"psecond is"<<evtpt[j]<<endl;
	//cout<<"2. pfirst is"<<p.first<<endl;
	//cout<<"2. psecond is"<<p.second<<endl;

	vect.push_back(p);

      }
    }
    //cout<<"Debug9"<<endl;

    //Put vector in increasing pt order
    sort(vect.begin(),vect.end(),mycompare);

    //Fill map vector
    vector<Int_t> map_vect;

    for(int i = 0; i < vect.size(); ++i){
      map_vect.push_back(vect[i].first);
    }
    //cout<<"Debug10"<<endl;

    //Add vector to the map with the corresponding evt number
    map_new[evt] = map_vect;

  }

  TString _path = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_eff/";
  TFile* file_out = new TFile(_path+"compare_evt_dytree"+".root","recreate");
  histo->Write("histo_new");
  file_out->Close();

  return map_new;

}
