/*********************************************
 * Description - This file plots the histograms for Z+bkg. THis used the files contained in 2014-11-13_skim2ll-mva-softbtag. 
 Update w.r.t. previous code: all lepton are now considered (loose and tight). 


 * Author - Gaël L. Perrin

 * Date - Jan 06 2015

 * *******************************************/

#include "cmath"
#include "TChain.h"
#include <sys/stat.h>
#include "iostream"
#include "fstream"
#include "vector"
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
//#include "../setTDRStyle.C"
//#include "../InvMass.C"
//#include "../DeltaR.C"

//#include "TandP_sidefunction.h"

////////////////////////
//Variable description//
////////////////////////

//tree:		TTree to be analysed
//leptonId:	pdgId of the lepton
//par_low	low boundary of Pt bins
//par_upp	high boundary of Pt bins
//nparbins	number of Pt bins
//sel_den	sel_denion of Tag AND Probe. Can take parameters such as: "loose", "tightcut", "tightmva" (for electron only).
//sel_num	additional requirement on Tag (the sel_denion on tag is sel_den + sel_num). Can take the parameter: "loose", "tightcut", "tightmva", "dxy", "dz", "reliso3", "reliso4", "chiso3", "chiso4".
//cut_num		cut_num parameter of sel_num is possible.
//option	can take: matching (a matching between the Tag and Probe is required).

////////////////////////////////////
//Declaration of all the functions//
////////////////////////////////////

//Function: loading the tree and calling DrawInvMassBkgMain
//int DrawInvMassBkg(int leptonId, double* par1 , int npar1bins , double* par2, int npar2bins ,TString sel_den = "tightcut", TString sel_num = "reliso3", double cut_num = 0.2, TString par_x = "Pt", TString par_y = "eta", TString option = "");

//Function: Draw the invariant mass. Take boundary's of parx and array of pary as argument.
//int DrawInvMassBkgMain(TTree* tree, int leptonId, double par_low = 10, double par_upp = 50,int nparbins = 10, TString sel_den = "tightcut", TString sel_num = "reliso3", double cut_num = 0.2,TString par_x = "Pt", TString par_y = "eta", TString option ="");

//Function: Draw the invariant mass. Take array for parx and pary range as argument.
int DrawInvMassBkgMain(TTree* tree, int leptonId, double* par1 ,int npar1bins , double* par2, int npar2bins, TString sel_den, TString sel_num , double cut_num, TString par_x, TString par_y, TString option);

////////////////////////
//Function definitions//
////////////////////////

int     DrawInvMassBkg( int leptonId, double* par1, int npar1bins , double* par2 ,int npar2bins , TString sel_den, TString sel_num , double cut_num, TString par_x, TString par_y, TString option = ""){

  //Location of the .root file
  TString location = "/Users/GLP/Desktop/CERN_data/2014-11-13_skim2ll-mva-softbtag/postprocessed/matched/";

  //Reading the tree 
  //
  TChain* tree = new TChain("treeProducerSusyMultilepton");

  //DY events
  tree->Add(location+"DYJetsToLLM50_PU_S14_POSTLS170.root");

  //WJet events
  tree->Add(location+"WJetsToLNu_13TeV-madgraph-pythia8-tauola.root");

  //TTJets events
  tree->Add(location+"TTJets_MSDecaysCKM_central_PU_S14_POSTLS170.root");

  cout<<"will make the file"<<endl;


  return DrawInvMassBkgMain(tree, leptonId, par1, npar1bins, par2, npar2bins, sel_den, sel_num, cut_num, par_x, par_y, option );

}

int     DrawInvMassBkg( int leptonId, double par_low , double par_upp ,int npar1bins , TString sel_den, TString sel_num , double cut_num, TString par_x, TString par_y, TString option = ""){

  cout<<"will make the file"<<endl;

  //Parameter 1
  double* par1 = new double[npar1bins+1];
  double Dpar = (double)(par_upp - par_low)/(double)npar1bins;

  for(int i = 0; i < npar1bins+1; ++i){
    par1[i] = par_low + i*Dpar;
  }

  //Parameter 2
  const int npar2bins_eta = 2;
  const int npar2bins_pt = 19;

  double par2_eta[npar2bins_eta+1] = {0,1.2,2.5};
  double par2_pt[npar2bins_pt+1] = {10,20,30,40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200};


  if(par_y == "eta"){
    return DrawInvMassBkg(leptonId, par1, npar1bins, par2_eta, npar2bins_eta, sel_den, sel_num, cut_num, par_x, par_y, option );
  }else if(par_y == "pt"){
    return DrawInvMassBkg(leptonId, par1, npar1bins, par2_pt, npar2bins_pt, sel_den, sel_num, cut_num, par_x, par_y, option );
  }else{return 1;}

}

int     DrawInvMassBkgMain(TTree* tree, int leptonId, double* par1 ,int npar1bins , double* par2, int npar2bins, TString sel_den, TString sel_num , double cut_num, TString par_x, TString par_y, TString option){

  TString _path= "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/";

  setTDRStyle();

  //Some variables
  Long64_t n = tree->GetEntries();
  int nbins = 200;

  /////////////////////
  //Name of the files//
  /////////////////////

  //Declare Strings

  TString pname;
  TString _pname;
  TString _par;
  TString _sel_num;
  TString _sel_den;
  TString _option;

  //particle string
  if(abs(leptonId) == 11){pname = "e"; _pname = "e";}
  if(abs(leptonId) == 13){pname = " #mu"; _pname = "mu";}
  //Parameter string
  if(par_x == "Pt"){_par = "P_{t}";}
  else if(par_x == "eta"){_par = "#eta";}
  else if(par_x == "phi"){_par = "#phi";}
  //sel_den string
  if((sel_den == "tightmva")&&(leptonId == 13)){cout<<"ERROR: no tightId MVA defined for the muon !"<<endl;return 1;}
  if(sel_den == "tightcut"){_sel_den = "tightcut";}
  else if(sel_den == "tightmva"){_sel_den = "tightmva";}
  else if(sel_den == "loose"){_sel_den = "loose";}
  else if(sel_den == ""){_sel_den = "";}
  else{cout<<"ERROR: wrong sel_denion !";return 1;}
  //sel_num string
  if((sel_num == "tightmva")&&(leptonId == 13)){cout<<"ERROR: no tightId MVA defined for the muon !"<<endl;return 1;}
  if(sel_num == "tightcut"){_sel_num = "tightcut";}
  else if(sel_num == "tightmva"){_sel_num = "tightmva";}
  else if(sel_num == ""){_sel_num = "";}
  else if(sel_num == "loose"){_sel_num = "loose";}
  else if(sel_num == "reliso3"){_sel_num = Form("reliso3_%0.3lf",cut_num);}
  else if(sel_num == "reliso4"){_sel_num = Form("reliso4_%0.3lf",cut_num);}
  else if(sel_num == "chiso3"){_sel_num = Form("chiso3_%0.3lf",cut_num);}
  else if(sel_num == "chiso4"){_sel_num = Form("chiso4_%0.3lf",cut_num);}
  else if(sel_num == "dxy"){_sel_num = Form("dxy_%0.3lf",cut_num);}
  else if(sel_num == "dz"){_sel_num = Form("dz_%0.3lf",cut_num);}
  else{cout<<"ERROR: wrong numerator name !";return 1;}
  //option string
  option.Append(" ");
  option.Prepend(" ");
  if(option.Contains(" matching ")){_option += "_Matched";}
  _option += "_";
  //parameter range string
  TString _par1range;
  _par1range = Form("%0.3f_"+par_x+"%0.3f",par1[0],par1[npar1bins]);
  TString _par2range;
  _par2range = Form("%0.3f_"+par_y+"%0.3f",par2[0],par2[npar2bins]);

  /////////////////////////////////////
  //Write the name of the output file//
  /////////////////////////////////////

  //Define name of the files
  TString _fname = "InvM4test"+_option+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num;

  TFile* file_out = new TFile(_path+_fname+".root","recreate");

  TH1D*** histo_M_DYJets_bkg  = new TH1D**[npar2bins];
  TH1D*** histo_M_DYJets  = new TH1D**[npar2bins];
  TH1D*** histo_M_bkg  = new TH1D**[npar2bins];
  TH1D*** histo_M_WJets  = new TH1D**[npar2bins];
  TH1D*** histo_M_TTJets  = new TH1D**[npar2bins];
  TH1D*** histo_M_DYJets_bkg_fail  = new TH1D**[npar2bins];
  TH1D*** histo_M_DYJets_fail  = new TH1D**[npar2bins];
  TH1D*** histo_M_bkg_fail  = new TH1D**[npar2bins];
  TH1D*** histo_M_WJets_fail  = new TH1D**[npar2bins];
  TH1D*** histo_M_TTJets_fail  = new TH1D**[npar2bins];

  for(int i = 0; i < npar2bins; ++i){ 

    histo_M_DYJets_bkg[i]  		= new TH1D*[npar1bins];
    histo_M_DYJets[i]  		= new TH1D*[npar1bins];
    histo_M_bkg[i]  		= new TH1D*[npar1bins];
    histo_M_WJets[i]  		= new TH1D*[npar1bins];
    histo_M_TTJets[i]  		= new TH1D*[npar1bins];
    histo_M_DYJets_bkg_fail[i] 	= new TH1D*[npar1bins];
    histo_M_DYJets_fail[i]     	= new TH1D*[npar1bins];
    histo_M_bkg_fail[i]  		= new TH1D*[npar1bins];
    histo_M_WJets_fail[i]  		= new TH1D*[npar1bins];
    histo_M_TTJets_fail[i]  	= new TH1D*[npar1bins];

    for(int j = 0; j < npar1bins; ++j){ 

      histo_M_DYJets_bkg[i][j]  		=  new TH1D("histo_M_DYJets_bkg","M",nbins,0,250);
      histo_M_DYJets[i][j]  			=  new TH1D("histo_M_DYJets","M",nbins,0,250);
      histo_M_bkg[i][j]  			=  new TH1D("histo_M_bkg","M",nbins,0,250);
      histo_M_WJets[i][j]  			=  new TH1D("histo_M_WYJets","M",nbins,0,250);
      histo_M_TTJets[i][j]  			=  new TH1D("histo_M_TTJets","M",nbins,0,250);
      histo_M_DYJets_bkg_fail[i][j]	 	=  new TH1D("histo_M_DYJets_bkg_fail","M",nbins,0,250);
      histo_M_DYJets_fail[i][j]     		=  new TH1D("histo_M_DYJets_fail","M",nbins,0,250);
      histo_M_bkg_fail[i][j]  		=  new TH1D("histo_M_bkg_fail","M",nbins,0,250);
      histo_M_WJets_fail[i][j]  		=  new TH1D("histo_M_WJets_fail","M",nbins,0,250);
      histo_M_TTJets_fail[i][j]  		=  new TH1D("histo_M_TTJets_fail","M",nbins,0,250);


    }

  }

  //Event variables
  ////Generated
  Int_t evt_id;
  Float_t scale;
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
  Int_t Oloose[200];
  Float_t Oiso3[200];
  Float_t Oiso4[200];
  Float_t Ochiso3[200];
  Float_t Ochiso4[200];
  Float_t Odxy[200];
  Float_t Odz[200];
  //loose
  Int_t Gn;
  Int_t matched[200];
  Int_t Gid[200];
  Float_t Gpt[200];
  Float_t Gm[200];
  Float_t Geta[200];
  Float_t Gphi[200];
  Int_t   Gq[200];
  Int_t Gtight[200];
  Int_t Gtighte[200];
  Int_t Gloose[200];
  Float_t Giso3[200];
  Float_t Giso4[200];
  Float_t Gchiso3[200];
  Float_t Gchiso4[200];
  Float_t Gdxy[200];
  Float_t Gdz[200];

  //Assigne branches
  tree->SetBranchAddress("evt_scale1fb", &scale);
  tree->SetBranchAddress("evt_id", &evt_id);
  //not loose
  tree->SetBranchAddress("nLepOther",&On);
  tree->SetBranchAddress("LepOther_pdgId",&Oid);
  tree->SetBranchAddress("LepOther_pt",&Opt);
  tree->SetBranchAddress("LepOther_mass",&Om);
  tree->SetBranchAddress("LepOther_eta",&Oeta);
  tree->SetBranchAddress("LepOther_phi",&Ophi);
  tree->SetBranchAddress("LepOther_charge",&Oq);
  tree->SetBranchAddress("LepOther_tightId",&Otight);
  tree->SetBranchAddress("LepGood_eleCutIdCSA14_50ns_v1",&Otighte);
  tree->SetBranchAddress("LepOther_relIso03",&Oiso3);
  tree->SetBranchAddress("LepOther_relIso04",&Oiso4);
  tree->SetBranchAddress("LepOther_chargedHadRelIso03",&Ochiso3);
  tree->SetBranchAddress("LepOther_chargedHadRelIso04",&Ochiso4);
  tree->SetBranchAddress("LepOther_dxy",&Odxy);
  tree->SetBranchAddress("LepOther_dz",&Odz);
  //Loose
  tree->SetBranchAddress("nLepGood",&Gn);
  tree->SetBranchAddress("LepGood_matched",&matched);
  tree->SetBranchAddress("LepGood_pdgId",&Gid);
  tree->SetBranchAddress("LepGood_pt",&Gpt);
  tree->SetBranchAddress("LepGood_mass",&Gm);
  tree->SetBranchAddress("LepGood_eta",&Geta);
  tree->SetBranchAddress("LepGood_phi",&Gphi);
  tree->SetBranchAddress("LepGood_charge",&Gq);
  tree->SetBranchAddress("LepGood_tightId",&Gtight);
  tree->SetBranchAddress("LepGood_eleCutIdCSA14_50ns_v1",&Gtighte);
  tree->SetBranchAddress("LepGood_relIso03",&Giso3);
  tree->SetBranchAddress("LepGood_relIso04",&Giso4);
  tree->SetBranchAddress("LepGood_chargedHadRelIso03",&Gchiso3);
  tree->SetBranchAddress("LepGood_chargedHadRelIso04",&Gchiso4);
  tree->SetBranchAddress("LepGood_dxy",&Gdxy);
  tree->SetBranchAddress("LepGood_dz",&Gdz);

  int count = 0;

  //Start loop over all events
  for (int k = 0; k < n; ++k) {

    if( 100*(double)k/n> count){cout<<count<<endl;++count;}

    //Definitions to use T&P
    int tag[2];
    int prob[2];
    tag[0] = 9999;
    tag[1] = 9999;
    prob[0] = 9999;
    prob[1] = 9999;

    tree->GetEntry(k);

    //looseId leptons
    if(sel_den != "loose"){
      for (int i = 0; i < On; ++i) {
	//cout<<"debug3"<<endl;

	//define sel_denions using bools
	bool reliso3((sel_num != "reliso3")||((sel_num == "reliso3")&&(Oiso3[i] < cut_num )));
	bool reliso4((sel_num != "reliso4")||((sel_num == "reliso4")&&(Oiso4[i] < cut_num )));
	bool chiso3((sel_num != "chiso3")||((sel_num == "chiso3")&&(Ochiso3[i] < cut_num )));
	bool chiso4((sel_num != "chiso4")||((sel_num == "chiso4")&&(Ochiso4[i] < cut_num )));
	bool dxy((sel_num != "dxy")||((sel_num == "dxy")&&(abs(Odxy[i])< cut_num )));
	bool dz((sel_num != "dz")||((sel_num == "dz")&&(abs(Odz[i])< cut_num )));
	bool tight((sel_num != "tightcut")||(((abs(Oid[i]) == 13)&&(sel_num == "tightcut")&&(Otight[i] == 1))||((abs(Oid[i]) == 11)&&(sel_num == "tightcut")&&(Otighte[i] >= 3))));
	bool tightmva((sel_num != "tightmva")||((abs(Oid[i]) == 11)&&(sel_num == "tightmva")&&(Otight[i] == 1)));


	//Store the values from both the loose and tight in the same variable

	//Prob sel_denion cut
	if(abs(Oid[i]) == leptonId){
	  if((sel_den != "tightcut")||(((abs(Oid[i]) == 13)&&(sel_den == "tightcut")&&(Otight[i] == 1))||((abs(Oid[i]) == 11)&&(sel_den == "tightcut")&&(Otighte[i] >= 3)))){ 
	    if((sel_den != "tightmva")||((abs(Oid[i]) == 11)&&(sel_den == "tightmva")&&(Otight[i] == 1))){ 
	      if((!option.Contains("matching"))||((option.Contains("matching"))&&(matched[i] == 1))){ 

		//Prob1
		if(prob[0] == 9999){prob[0] = i;}
		//Prob2
		if((prob[0] != 9999)&&(prob[0] != i)&&(prob[1] == 9999)){prob[1] = i;}

		//Selection cut for Tag only
		if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightmva){

		  if(prob[0] == i){tag[0] = i; }
		  if(prob[1] == i){tag[1] = i; }

		}
	      }
	    }
	  }
	}
      }

      //ok

      //loop over all the rec particles to find the isolation
      //We requiere one tag at least 
      while((tag[0] != 9999)||(tag[1] != 9999)){

	int l1;
	int l2;

	if(tag[0] != 9999){l1 = prob[1]; l2 = tag[0]; tag[0] = 9999;}
	else if(tag[1] != 9999){l1 = prob[0]; l2 = tag[1]; tag[1] = 9999;}

	double M = InvMass(Opt[l1],Oeta[l1],Ophi[l1],Om[l1],Opt[l2],Oeta[l2],Ophi[l2],Om[l2]);

	if(l1 != 9999){

	  //Get the x and y parameter to sel_den the histograms
	  double par_1;
	  double par_2;

	  //Choose the parameter to be filled for the eff.
	  if(par_x == "Pt"){par_1 = Opt[l1];}
	  else if(par_x == "eta"){par_1 = Oeta[l1];}
	  else if(par_x == "phi"){par_1 = Ophi[l1];}
	  if(par_y == "Pt"){par_2 = Opt[l1];}
	  else if(par_y == "eta"){par_2 = abs(Oeta[l1]);}
	  else if(par_y == "phi"){par_2 = abs(Ophi[l1]);}

	  for(int ii = 0; ii < npar1bins; ++ii){

	    if((par_1 > par1[ii])&&(par_1 < par1[ii+1])){

	      bool reliso3((sel_num != "reliso3")||((sel_num == "reliso3")&&(Oiso3[l1] < cut_num )));
	      bool reliso4((sel_num != "reliso4")||((sel_num == "reliso4")&&(Oiso4[l1] < cut_num )));
	      bool chiso3((sel_num != "chiso3")||((sel_num == "chiso3")&&(Ochiso3[l1] < cut_num )));
	      bool chiso4((sel_num != "chiso4")||((sel_num == "chiso4")&&(Ochiso4[l1] < cut_num )));
	      bool dxy((sel_num != "dxy")||((sel_num == "dxy")&&(abs(Odxy[l1])< cut_num )));
	      bool dz((sel_num != "dz")||((sel_num == "dz")&&(abs(Odz[l1])< cut_num )));
	      bool tight((sel_num != "tightcut")||(((abs(Oid[l1]) == 13)&&(sel_num == "tightcut")&&(Otight[l1] == 1))||((abs(Oid[l1]) == 11)&&(sel_num == "tightcut")&&(Otighte[l1] >= 3))));
	      bool tightmva((sel_num != "tightmva")||((abs(Oid[l1]) == 11)&&(sel_num == "tightmva")&&(Otight[l1] == 1)));

	      //Find the corresponding histogram for par_y

	      int kk = -1;
	      for(int _i = 0; _i < npar2bins; ++_i){

		if((par_2 > par2[_i])&&(par_2 <= par2[_i+1])){kk = _i;}

	      }

	      //Efficiency cut
	      if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightmva && (kk!= -1)){

		histo_M_DYJets_bkg[kk][ii]->Fill(M,scale);

		if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){
		  histo_M_bkg[kk][ii]->Fill(M,scale);

		  //Just the Wjets bkg
		  if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){

		    histo_M_WJets[kk][ii]->Fill(M,scale);

		  }else if (evt_id == 300){

		    histo_M_TTJets[kk][ii]->Fill(M,scale);

		  }

		}else if((evt_id == 702)||(evt_id == 703)||(evt_id == 704)||(evt_id == 705)||(evt_id == 701)){

		  histo_M_DYJets[kk][ii]->Fill(M,scale);

		}


	      }

	      reliso3 = ((sel_num != "reliso3")||((sel_num == "reliso3")&&(Oiso3[l1] >= cut_num )));
	      reliso4 = ((sel_num != "reliso4")||((sel_num == "reliso4")&&(Oiso4[l1] >= cut_num )));
	      chiso3 = ((sel_num != "chiso3")||((sel_num == "chiso3")&&(Ochiso3[l1] >= cut_num )));
	      chiso4 = ((sel_num != "chiso4")||((sel_num == "chiso4")&&(Ochiso4[l1] >= cut_num )));
	      dxy = ((sel_num != "dxy")||((sel_num == "dxy")&&(abs(Odxy[l1]) >= cut_num )));
	      dz = ((sel_num != "dz")||((sel_num == "dz")&&(abs(Odz[l1]) >= cut_num )));
	      tight = ((sel_num != "tightcut")||(((abs(Oid[l1]) == 13)&&(sel_num == "tightcut")&&(Otight[l1] != 1))||((abs(Oid[l1]) == 11)&&(sel_num == "tightcut")&&(Otighte[l1] < 3))));
	      tightmva = ((sel_num != "tightmva")||((abs(Oid[l1]) == 11)&&(sel_num == "tightmva")&&(Otight[l1] != 1)));

	      if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightmva && (kk!= -1)){

		histo_M_DYJets_bkg_fail[kk][ii]->Fill(M,scale);

		if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){
		  histo_M_bkg_fail[kk][ii]->Fill(M,scale);

		  if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){

		    histo_M_WJets_fail[kk][ii]->Fill(M,scale);

		  }else if (evt_id == 300){

		    histo_M_TTJets_fail[kk][ii]->Fill(M,scale);

		  }

		}else if((evt_id == 702)||(evt_id == 703)||(evt_id == 704)||(evt_id == 705)||(evt_id == 701)){

		  histo_M_DYJets_fail[kk][ii]->Fill(M,scale);

		}

	      }
	    }
	  }
	}
      }
    }


    for (int i = 0; i < Gn; ++i) {

      //define sel_denions using bools

      bool reliso3((sel_num != "reliso3")||((sel_num == "reliso3")&&(Giso3[i] < cut_num )));
      bool reliso4((sel_num != "reliso4")||((sel_num == "reliso4")&&(Giso4[i] < cut_num )));
      bool chiso3((sel_num != "chiso3")||((sel_num == "chiso3")&&(Gchiso3[i] < cut_num )));
      bool chiso4((sel_num != "chiso4")||((sel_num == "chiso4")&&(Gchiso4[i] < cut_num )));
      bool dxy((sel_num != "dxy")||((sel_num == "dxy")&&(abs(Gdxy[i])< cut_num )));
      bool dz((sel_num != "dz")||((sel_num == "dz")&&(abs(Gdz[i])< cut_num )));
      bool tight((sel_num != "tightcut")||(((abs(Gid[i]) == 13)&&(sel_num == "tightcut")&&(Gtight[i] == 1))||((abs(Gid[i]) == 11)&&(sel_num == "tightcut")&&(Gtighte[i] >= 3))));
      bool tightmva((sel_num != "tightmva")||((abs(Gid[i]) == 11)&&(sel_num == "tightmva")&&(Gtight[i] == 1)));


      //Store the values from both the loose and tight in the same variable

      //Prob sel_denion cut
      if(abs(Gid[i]) == leptonId){
	if((sel_den != "tightcut")||(((abs(Gid[i]) == 13)&&(sel_den == "tightcut")&&(Gtight[i] == 1))||((abs(Gid[i]) == 11)&&(sel_den == "tightcut")&&(Gtighte[i] >= 3)))){ 
	  if((sel_den != "tightmva")||((abs(Gid[i]) == 11)&&(sel_den == "tightmva")&&(Gtight[i] == 1))){ 
	    if((!option.Contains("matching"))||((option.Contains("matching"))&&(matched[i] == 1))){ 

	      //Prob1
	      if(prob[0] == 9999){prob[0] = i;}
	      //Prob2
	      if((prob[0] != 9999)&&(prob[0] != i)&&(prob[1] == 9999)){prob[1] = i;}

	      //Selection cut for Tag only
	      if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightmva){

		if(prob[0] == i){tag[0] = i; }
		if(prob[1] == i){tag[1] = i; }

	      }
	    }
	  }
	}
      }
    }

    //loop over all the rec particles to find the isolation
    //We requiere one tag at least 
    while((tag[0] != 9999)||(tag[1] != 9999)){

      int l1;
      int l2;

      if(tag[0] != 9999){l1 = prob[1]; l2 = tag[0]; tag[0] = 9999;}
      else if(tag[1] != 9999){l1 = prob[0]; l2 = tag[1]; tag[1] = 9999;}

      double M = InvMass(Gpt[l1],Geta[l1],Gphi[l1],Gm[l1],Gpt[l2],Geta[l2],Gphi[l2],Gm[l2]);

      if(l1 != 9999){

	//Parameter on the xaxis

	double par_1;
	double par_2;

	//Choose the parameter to be filled for the eff.
	if(par_x == "Pt"){par_1 = Gpt[l1];}
	else if(par_x == "eta"){par_1 = Geta[l1];}
	else if(par_x == "phi"){par_1 = Gphi[l1];}
	if(par_y == "Pt"){par_2 = Gpt[l1];}
	else if(par_y == "eta"){par_2 = abs(Geta[l1]);}
	else if(par_y == "phi"){par_2 = abs(Gphi[l1]);}

	for(int ii = 0; ii < npar1bins; ++ii){

	  if((par_1 > par1[ii])&&(par_1 < par1[ii+1])){


	    bool reliso3((sel_num != "reliso3")||((sel_num == "reliso3")&&(Giso3[l1] < cut_num )));
	    bool reliso4((sel_num != "reliso4")||((sel_num == "reliso4")&&(Giso4[l1] < cut_num )));
	    bool chiso3((sel_num != "chiso3")||((sel_num == "chiso3")&&(Gchiso3[l1] < cut_num )));
	    bool chiso4((sel_num != "chiso4")||((sel_num == "chiso4")&&(Gchiso4[l1] < cut_num )));
	    bool dxy((sel_num != "dxy")||((sel_num == "dxy")&&(abs(Gdxy[l1])< cut_num )));
	    bool dz((sel_num != "dz")||((sel_num == "dz")&&(abs(Gdz[l1])< cut_num )));
	    bool tight((sel_num != "tightcut")||(((abs(Gid[l1]) == 13)&&(sel_num == "tightcut")&&(Gtight[l1] == 1))||((abs(Gid[l1]) == 11)&&(sel_num == "tightcut")&&(Gtighte[l1] >= 3))));
	    bool tightmva((sel_num != "tightmva")||((abs(Gid[l1]) == 11)&&(sel_num == "tightmva")&&(Gtight[l1] == 1)));

	    //Find the corresponding histogram for par2

	    int kk = -1;
	    for(int _i = 0; _i < npar2bins; ++_i){

	      if((par_2 > par2[_i])&&(par_2 <= par2[_i+1])){kk = _i;}
	    }

	    //Efficiency cut
	    if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightmva && (kk != -1)){

	      histo_M_DYJets_bkg[kk][ii]->Fill(M,scale);

	      if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){

		histo_M_bkg[kk][ii]->Fill(M,scale);

		//Just the Wjets bkg
		if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){

		  histo_M_WJets[kk][ii]->Fill(M,scale);

		}else if (evt_id == 300){

		  histo_M_TTJets[kk][ii]->Fill(M,scale);


		}

	      }else if((evt_id == 702)||(evt_id == 703)||(evt_id == 704)||(evt_id == 705)||(evt_id == 701)){

		histo_M_DYJets[kk][ii]->Fill(M,scale);

	      }

	    }


	    reliso3 = ((sel_num != "reliso3")||((sel_num == "reliso3")&&(Giso3[l1] >= cut_num )));
	    reliso4 = ((sel_num != "reliso4")||((sel_num == "reliso4")&&(Giso4[l1] >= cut_num )));
	    chiso3 = ((sel_num != "chiso3")||((sel_num == "chiso3")&&(Gchiso3[l1] >= cut_num )));
	    chiso4 = ((sel_num != "chiso4")||((sel_num == "chiso4")&&(Gchiso4[l1] >= cut_num )));
	    dxy = ((sel_num != "dxy")||((sel_num == "dxy")&&(abs(Gdxy[l1]) >= cut_num )));
	    dz = ((sel_num != "dz")||((sel_num == "dz")&&(abs(Gdz[l1]) >= cut_num )));
	    tight = ((sel_num != "tightcut")||(((abs(Gid[l1]) == 13)&&(sel_num == "tightcut")&&(Gtight[l1] != 1))||((abs(Gid[l1]) == 11)&&(sel_num == "tightcut")&&(Gtighte[l1] < 3))));
	    tightmva = ((sel_num != "tightmva")||((abs(Gid[l1]) == 11)&&(sel_num == "tightmva")&&(Gtight[l1] != 1)));

	    if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightmva && (kk != -1)){

	      histo_M_DYJets_bkg_fail[kk][ii]->Fill(M,scale);

	      if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){

		histo_M_bkg_fail[kk][ii]->Fill(M,scale);

		if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){

		  histo_M_WJets_fail[kk][ii]->Fill(M,scale);

		}else if (evt_id == 300){

		  histo_M_TTJets_fail[kk][ii]->Fill(M,scale);

		}

	      }else if((evt_id == 702)||(evt_id == 703)||(evt_id == 704)||(evt_id == 705)||(evt_id == 701)){

		histo_M_DYJets_fail[kk][ii]->Fill(M,scale);
	      }

	    }
	  }
	}
      }
    }
  }

  mkdir(_path+_fname+"_PDF/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  TFile* canvas_out = new TFile(_path+_fname+"_PDF/histograms.root","recreate"); 

  for(int i = 0; i < npar2bins; ++i){ 

    for(int j = 0; j < npar1bins; ++j){ 

      ////////////////////
      //Build histograms//
      ////////////////////

      //String for name of the ouput files and histograms titles

      TString _parxbin;
      TString _parybin;

      //Parameter string
      TString _par;
      if(par_x == "Pt"){_parxbin = Form("%0.f_Pt%0.f",par1[j],par1[j+1]);}
      else if(par_x == "eta"){_parxbin = Form("%0.3f_eta%0.3f",par1[j],par1[j+1]);}
      else if(par_x == "phi"){_parxbin = Form("%0.3f_phi%0.3f",par1[j],par1[j+1]);}
      if(par_y == "Pt"){_parybin = Form("%0.f_Pt%0.f",par2[i],par2[i+1]);}
      else if(par_y == "eta"){_parybin = Form("%0.3f_eta%0.3f",par2[i],par2[i+1]);cout<<"it works !"<<endl;}
      else if(par_y == "phi"){_parybin = Form("%0.3f_phi%0.3f",par2[i],par2[i+1]);}

      TString _parxtitle;
      TString _parytitle;

      //Title string
      if(par_x == "Pt"){_parxtitle = Form("%0.f #leq P_{t} #leq %0.f",par1[j],par1[j+1]);}
      else if(par_x == "eta"){_parxtitle = Form("%0.3f #leq #eta #leq %0.3f",par1[j],par1[j+1]);}
      else if(par_x == "phi"){_parxtitle = Form("%0.3f #leq #phi #leq %0.3f",par1[j],par1[j+1]);}
      if(par_y == "Pt"){_parytitle = Form("%0.f #leq P_{t} #leq %0.f",par2[i],par2[i+1]);}
      else if(par_y == "eta"){_parytitle = Form("%0.3f #leq #||{#eta}  #leq %0.3f",par2[i],par2[i+1]);cout<<"it works !"<<endl;}
      else if(par_y == "phi"){_parytitle = Form("%0.3f #leq #||{#phi}  #leq %0.3f",par2[i],par2[i+1]);}

      //Plot histograms


      TCanvas* c1 = new TCanvas("c1","c1");
      TCanvas* c2 = new TCanvas("c2","c2");
      c1->Divide(1,2);
      c2->Divide(1,2);

      c1->cd(1);

      //TString _title; 
      TString _stitle = (TString)"Invariant mass for "+pname+" "+_parxtitle+", "+_parytitle+", "+sel_num+" pass";
      TString _stitlefail = (TString)"Invariant mass for "+pname+" "+_parxtitle+", "+_parytitle+", "+sel_num+" fail";

      histo_M_DYJets_bkg[i][j]->Draw();
      histo_M_DYJets_bkg[i][j]->SetTitle(_stitle);
      histo_M_DYJets_bkg[i][j]->GetXaxis()->SetTitle("m [GeV]");
      histo_M_DYJets_bkg[i][j]->SetLineWidth(2);
      histo_M_DYJets_bkg[i][j]->SetLineColor(4);
      histo_M_DYJets_bkg[i][j]->SetMarkerColor(4);
      histo_M_bkg[i][j]->Draw("same");
      histo_M_bkg[i][j]->SetLineWidth(2);
      histo_M_bkg[i][j]->SetMarkerColor(2);
      histo_M_bkg[i][j]->SetLineColor(2);
      histo_M_WJets[i][j]->Draw("same");
      histo_M_WJets[i][j]->SetLineWidth(3);
      histo_M_WJets[i][j]->SetLineColor(3);
      histo_M_WJets[i][j]->SetMarkerColor(3);
      histo_M_TTJets[i][j]->Draw("same");
      histo_M_TTJets[i][j]->SetLineWidth(3);
      histo_M_TTJets[i][j]->SetLineColor(6);
      histo_M_TTJets[i][j]->SetMarkerColor(6);
      TLegend* leg = new TLegend(0.6, 0.65,0.89,0.89);
      leg->AddEntry(histo_M_DYJets_bkg[i][j], "Z + bkg");
      leg->SetTextFont(43);
      leg->SetTextSize(25);
      leg->AddEntry(histo_M_bkg[i][j], "TTJets + WJets");
      leg->AddEntry(histo_M_WJets[i][j], "WJets");
      leg->AddEntry(histo_M_TTJets[i][j], "TTJets");
      leg->SetBorderSize(0);
      leg->Draw();

      //Zbkg fail
      c1->cd(2);
      histo_M_DYJets_bkg_fail[i][j]->Draw();
      histo_M_DYJets_bkg_fail[i][j]->SetTitle(_stitlefail);
      histo_M_DYJets_bkg_fail[i][j]->GetXaxis()->SetTitle("m [GeV]");
      histo_M_DYJets_bkg_fail[i][j]->SetLineWidth(2);
      histo_M_DYJets_bkg_fail[i][j]->SetLineColor(4);
      histo_M_DYJets_bkg_fail[i][j]->SetMarkerColor(4);
      histo_M_bkg_fail[i][j]->Draw("same");
      histo_M_bkg_fail[i][j]->SetLineWidth(2);
      histo_M_bkg_fail[i][j]->SetMarkerColor(2);
      histo_M_bkg_fail[i][j]->SetLineColor(2);
      histo_M_WJets_fail[i][j]->Draw("same");
      histo_M_WJets_fail[i][j]->SetLineWidth(3);
      histo_M_WJets_fail[i][j]->SetLineColor(3);
      histo_M_WJets_fail[i][j]->SetMarkerColor(3);
      histo_M_TTJets_fail[i][j]->Draw("same");
      histo_M_TTJets_fail[i][j]->SetLineWidth(3);
      histo_M_TTJets_fail[i][j]->SetLineColor(6);
      histo_M_TTJets_fail[i][j]->SetMarkerColor(6);


      //Z pass eta<1.2
      c2->cd(1);
      histo_M_DYJets[i][j]->Draw();
      histo_M_DYJets[i][j]->SetTitle(_stitle);
      histo_M_DYJets[i][j]->GetXaxis()->SetTitle("m [GeV]");
      histo_M_DYJets[i][j]->SetLineWidth(2);
      histo_M_DYJets[i][j]->SetLineColor(4);
      histo_M_DYJets[i][j]->SetMarkerColor(4);
      histo_M_bkg[i][j]->Draw("same");
      histo_M_bkg[i][j]->SetLineWidth(2);
      histo_M_bkg[i][j]->SetMarkerColor(2);
      histo_M_bkg[i][j]->SetLineColor(2);
      TLegend* leg2 = new TLegend(0.6, 0.65,0.89,0.89);
      leg2->SetTextFont(43);
      leg2->SetTextSize(25);
      leg2->AddEntry(histo_M_DYJets[i][j], "Z");
      leg2->AddEntry(histo_M_bkg[i][j], "TTJets + WJets");
      leg2->SetBorderSize(0);
      leg2->Draw();

      //Z fail eta<1.2
      c2->cd(2);
      histo_M_DYJets_fail[i][j]->Draw();
      histo_M_DYJets_fail[i][j]->SetTitle(_stitlefail);
      histo_M_DYJets_fail[i][j]->GetXaxis()->SetTitle("m [GeV]");
      histo_M_DYJets_fail[i][j]->SetLineWidth(2);
      histo_M_DYJets_fail[i][j]->SetLineColor(4);
      histo_M_DYJets_fail[i][j]->SetMarkerColor(4);
      histo_M_bkg_fail[i][j]->Draw("same");
      histo_M_bkg_fail[i][j]->SetLineWidth(2);
      histo_M_bkg_fail[i][j]->SetMarkerColor(2);
      histo_M_bkg_fail[i][j]->SetLineColor(2);

      /////////////////////
      //Saving the output//
      /////////////////////

      canvas_out->cd();

      c1->Write("Zbkg_"+_parxbin+"_"+_parybin);
      c2->Write("Z_"+_parxbin+"_"+_parybin);

      TString _cname = _path+_fname+"_PDF/InvM4test_"+_parxbin;
      _cname += "_"+_sel_num+"_"+_pname;

      TString _c1name = _cname+ "_Zbkg_bkg_"+_parybin;
      TString _c2name = _cname+ "_Z_bkg_"+_parybin;

      c1->SaveAs(_c1name+".pdf");
      c2->SaveAs(_c2name+".pdf");

      file_out->cd();

      histo_M_DYJets_bkg[i][j]->Write("histo_M_DYJets_bkg"+_parxbin+"_"+_parybin);
      histo_M_DYJets[i][j]->Write("histo_M_DYJets"+_parxbin+"_"+_parybin); 
      histo_M_bkg[i][j]->Write("histo_M_bkg"+_parxbin+"_"+_parybin); 
      histo_M_WJets[i][j]->Write("histo_M_WJets"+_parxbin+"_"+_parybin); 
      histo_M_TTJets[i][j]->Write("histo_M_TTJets"+_parxbin+"_"+_parybin); 
      histo_M_DYJets_bkg_fail[i][j]->Write("histo_M_DYJets_bkg_fail"+_parxbin+"_"+_parybin); 
      histo_M_DYJets_fail[i][j]->Write("histo_M_DYJets_fail"+_parxbin+"_"+_parybin); 
      histo_M_bkg_fail[i][j]->Write("histo_M_bkg_fail"+_parxbin+"_"+_parybin); 
      histo_M_WJets_fail[i][j]->Write("histo_M_WJets_fail"+_parxbin+"_"+_parybin); 
      histo_M_TTJets_fail[i][j]->Write("histo_M_TTJets_fail"+_parxbin+"_"+_parybin); 

    }                    

  }

  file_out->Close();
  canvas_out->Close();

  return 0;

}
