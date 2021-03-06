/*********************************************
 Update w.r.t. previous code: all lepton are now considered (loose and tight). 
 * Description - A modification of DrawInvMassBkg2.C to take in account the lep_other, which was not done correctly in MC_Ratio2
 Final version


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
#include <map>

//Helper functions
#include "../tools/InvMass.C"
#include "../tools/DeltaR.C"
#include "../tools/setTDRStyle.C"
#include "../tools/getmaxallhist.C"

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
//
struct Prob{

  int LEP;
  double PT;
  double ETA;
  double PHI;
  double M;

};

//Function: Draw the invariant mass. Take array for parx and pary range as argument.
int DrawInvMassBkgMain(TString _filetag, vector<TTree*> tree, int leptonId, double* par1 ,int npar1bins , double* par2, int npar2bins, TString sel_den, TString sel_num , double cut_num, TString par_x, TString par_y, TString option);

////////////////////////
//Function definitions//
////////////////////////

int     DrawInvMassBkg(TString _filetag, int leptonId, double* par1, int npar1bins , double* par2 ,int npar2bins , TString sel_den, TString sel_num , double cut_num, TString par_x, TString par_y, TString option = ""){

  cout<<"Start to creating the file"<<endl;

  //Location of the .root file
  TString location = "/Users/GLP/Desktop/CERN_data/2014-11-13_skim2ll-mva-softbtag/postprocessed/matched/";
  TString location2 = "/Users/GLP/Desktop/CERN_data/dyjetsnew/postprocessed/";

  vector< TTree* > tree;

  //Reading the tree 
  //
  TChain* tree_dy = new TChain("treeProducerSusyMultilepton");
  TChain* tree_wjet = new TChain("treeProducerSusyMultilepton");
  TChain* tree_ttjet = new TChain("treeProducerSusyMultilepton");

  //DY events
  //tree->Add(location+"DYJetsToLLM50_PU_S14_POSTLS170.root");
  tree_dy->Add(location2+"dyjetsnew.root");

  //WJet events
  tree_wjet->Add(location+"WJetsToLNu_13TeV-madgraph-pythia8-tauola.root");

  //TTJets events
  tree_ttjet->Add(location+"TTJets_MSDecaysCKM_central_PU_S14_POSTLS170.root");

  cout<<"will make the file"<<endl;

  tree.push_back(tree_dy);
  tree.push_back(tree_wjet);
  tree.push_back(tree_ttjet);

  return DrawInvMassBkgMain(_filetag, tree, leptonId, par1, npar1bins, par2, npar2bins, sel_den, sel_num, cut_num, par_x, par_y, option );

}

int     DrawInvMassBkg( TString _filetag, int leptonId, double par_low , double par_upp ,int npar1bins , TString sel_den, TString sel_num , double cut_num, TString par_x, TString par_y, TString option = ""){

  cout<<"will make the file"<<endl;

  //Parameter 1
  double* par1 = new double[npar1bins+1];
  double Dpar = (double)(par_upp - par_low)/(double)npar1bins;

  for(int i = 0; i < npar1bins+1; ++i){
    par1[i] = par_low + i*Dpar;
  }

  //Parameter 2
  const int npar2bins_eta = 2;
  //const int npar2bins_pt = 19;
  const int npar2bins_pt = 1;

  double par2_eta[npar2bins_eta+1] = {0,1.2,2.5};
  //double par2_pt[npar2bins_pt+1] = {10,20,30,40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200};
  double par2_pt[npar2bins_pt+1] = {10,250};//,30,40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200};


  if(par_y == "eta"){
    return DrawInvMassBkg(_filetag, leptonId, par1, npar1bins, par2_eta, npar2bins_eta, sel_den, sel_num, cut_num, par_x, par_y, option );
  }else if(par_y == "Pt"){
    return DrawInvMassBkg(_filetag, leptonId, par1, npar1bins, par2_pt, npar2bins_pt, sel_den, sel_num, cut_num, par_x, par_y, option );
  }else{return 1;}

}

int     DrawInvMassBkgMain(TString _filetag, vector< TTree* > tree, int leptonId, double* par1 ,int npar1bins , double* par2, int npar2bins, TString sel_den, TString sel_num , double cut_num, TString par_x, TString par_y, TString option){

  TString _path= "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/";

  setTDRStyle();

  //TString _filetag = "_newloose_neworder";

  //Some variables
  Long64_t n[] = {tree[0]->GetEntries(),tree[1]->GetEntries(),tree[2]->GetEntries()};
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
  if(option.Contains(" short ")){_option += "_short";}
  if(option.Contains(" loose ")){_option += "_loose";}
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
  TString _fname = "InvM"+_filetag+_option+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num;

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

  //Histogram for the par1 bin distribution
  TH1D** histo_par1 = new TH1D*[npar2bins];
  TH1D** histo_par1_dyonly = new TH1D*[npar2bins];//This histograms takes the par1 of the prob that are coming from the dy sample only
  //Paramter for each bin
  TH1D*** h_par1 = new TH1D**[npar2bins];
  TH1D*** h_par1_dyonly = new TH1D**[npar2bins];//This histograms takes the par1 of the prob that are coming from the dy sample only

  //Histogram to count the total number of tag and prob per event

  TH1D* h_tag = new TH1D("h_tag","ntag",10,0,10);
  TH1D* h_prob = new TH1D("h_prob","nprob",10,0,10);
  

  for(int i = 0; i < npar2bins; ++i){ 

    histo_M_DYJets_bkg[i]  		= new TH1D*[npar1bins];
    histo_M_DYJets[i]  			= new TH1D*[npar1bins];
    histo_M_bkg[i]  			= new TH1D*[npar1bins];
    histo_M_WJets[i]  			= new TH1D*[npar1bins];
    histo_M_TTJets[i]  			= new TH1D*[npar1bins];
    histo_M_DYJets_bkg_fail[i] 		= new TH1D*[npar1bins];
    histo_M_DYJets_fail[i]     		= new TH1D*[npar1bins];
    histo_M_bkg_fail[i]  		= new TH1D*[npar1bins];
    histo_M_WJets_fail[i]  		= new TH1D*[npar1bins];
    histo_M_TTJets_fail[i]  		= new TH1D*[npar1bins];

    histo_par1[i] 			= new TH1D("histo_par1","par1",npar1bins*25,par1[0],par1[npar1bins]);
    histo_par1_dyonly[i] 		= new TH1D("histo_par1_dyonly","par1",npar1bins*25,par1[0],par1[npar1bins]);

    h_par1[i] 				= new TH1D*[npar1bins];
    h_par1_dyonly[i] 			= new TH1D*[npar1bins];

    for(int j = 0; j < npar1bins; ++j){ 

      histo_M_DYJets_bkg[i][j]  		=  new TH1D("histo_M_DYJets_bkg","M",nbins,0,250);
      histo_M_DYJets[i][j]  			=  new TH1D("histo_M_DYJets","M",nbins,0,250);
      histo_M_bkg[i][j]  			=  new TH1D("histo_M_bkg","M",nbins,0,250);
      histo_M_WJets[i][j]  			=  new TH1D("histo_M_WYJets","M",nbins,0,250);
      histo_M_TTJets[i][j]  			=  new TH1D("histo_M_TTJets","M",nbins,0,250);
      histo_M_DYJets_bkg_fail[i][j]	 	=  new TH1D("histo_M_DYJets_bkg_fail","M",nbins,0,250);
      histo_M_DYJets_fail[i][j]     		=  new TH1D("histo_M_DYJets_fail","M",nbins,0,250);
      histo_M_bkg_fail[i][j]  			=  new TH1D("histo_M_bkg_fail","M",nbins,0,250);
      histo_M_WJets_fail[i][j]  		=  new TH1D("histo_M_WJets_fail","M",nbins,0,250);
      histo_M_TTJets_fail[i][j]  		=  new TH1D("histo_M_TTJets_fail","M",nbins,0,250);

      //Par1 distribution
      h_par1[i][j] 				= new TH1D("h_par1","par1",25,par1[j],par1[j+1]);
      h_par1_dyonly[i][j] 			= new TH1D("h_par1_dyonly","par1",25,par1[j],par1[j+1]);

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
  Int_t   Ofromtau[200];
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
  Int_t   Gfromtau[200];

  for(int tree_i = 0; tree_i < 3; ++ tree_i){

  tree[tree_i]->SetBranchStatus("*",0);
  tree[tree_i]->SetBranchStatus("evt_scale1fb",1);
  tree[tree_i]->SetBranchStatus("evt_id",1);
  tree[tree_i]->SetBranchStatus("nLepOther",1);
  tree[tree_i]->SetBranchStatus("nLepGood",1);
  tree[tree_i]->SetBranchStatus("LepOther_*",1);
  tree[tree_i]->SetBranchStatus("LepGood_*",1);

  //Assigne branches
  tree[tree_i]->SetBranchAddress("evt_scale1fb", &scale);
  tree[tree_i]->SetBranchAddress("evt_id", &evt_id);
  //not loose
  tree[tree_i]->SetBranchAddress("nLepOther",&On);
  tree[tree_i]->SetBranchAddress("LepOther_pdgId",&Oid);
  tree[tree_i]->SetBranchAddress("LepOther_pt",&Opt);
  tree[tree_i]->SetBranchAddress("LepOther_mass",&Om);
  tree[tree_i]->SetBranchAddress("LepOther_eta",&Oeta);
  tree[tree_i]->SetBranchAddress("LepOther_phi",&Ophi);
  tree[tree_i]->SetBranchAddress("LepOther_charge",&Oq);
  tree[tree_i]->SetBranchAddress("LepOther_tightId",&Otight);
  tree[tree_i]->SetBranchAddress("LepOther_eleCutIdCSA14_50ns_v1",&Otighte);
  tree[tree_i]->SetBranchAddress("LepOther_relIso03",&Oiso3);
  tree[tree_i]->SetBranchAddress("LepOther_relIso04",&Oiso4);
  tree[tree_i]->SetBranchAddress("LepOther_chargedHadRelIso03",&Ochiso3);
  tree[tree_i]->SetBranchAddress("LepOther_chargedHadRelIso04",&Ochiso4);
  tree[tree_i]->SetBranchAddress("LepOther_dxy",&Odxy);
  tree[tree_i]->SetBranchAddress("LepOther_dz",&Odz);
  tree[tree_i]->SetBranchAddress("LepOther_mcMatchTau",&Ofromtau);
  //Loose
  tree[tree_i]->SetBranchAddress("nLepGood",&Gn);
  tree[tree_i]->SetBranchAddress("LepGood_matched",&matched);
  tree[tree_i]->SetBranchAddress("LepGood_pdgId",&Gid);
  tree[tree_i]->SetBranchAddress("LepGood_pt",&Gpt);
  tree[tree_i]->SetBranchAddress("LepGood_mass",&Gm); 
  tree[tree_i]->SetBranchAddress("LepGood_eta",&Geta); 
  tree[tree_i]->SetBranchAddress("LepGood_phi",&Gphi); 
  tree[tree_i]->SetBranchAddress("LepGood_charge",&Gq); 
  tree[tree_i]->SetBranchAddress("LepGood_tightId",&Gtight); 
  tree[tree_i]->SetBranchAddress("LepGood_eleCutIdCSA14_50ns_v1",&Gtighte); 
  tree[tree_i]->SetBranchAddress("LepGood_relIso03",&Giso3); 
  tree[tree_i]->SetBranchAddress("LepGood_relIso04",&Giso4); 
  tree[tree_i]->SetBranchAddress("LepGood_chargedHadRelIso03",&Gchiso3);
  tree[tree_i]->SetBranchAddress("LepGood_chargedHadRelIso04",&Gchiso4);
  tree[tree_i]->SetBranchAddress("LepGood_dxy",&Gdxy);
  tree[tree_i]->SetBranchAddress("LepGood_dz",&Gdz);
  tree[tree_i]->SetBranchAddress("LepGood_mcMatchTau",&Gfromtau);

  int count = 0;

  cout<<"HELLO"<<endl;
  if(option.Contains(" short ")){n[tree_i] = 1000000;cout<<"short !"<<endl;}

  //Start loop over all events
  for (int k = 0; k < n[tree_i]; ++k) {

    //Declaration of event parameters
    Int_t 	evtn;
    Int_t 	evtloose[200];
    Int_t 	evtid[200];
    Float_t 	evtpt[200];
    Float_t 	evtm[200];
    Float_t 	evteta[200];
    Float_t 	evtphi[200];
    Int_t   	evtq[200];
    Int_t 	evttight[200];
    Int_t 	evttighte[200];
    Float_t 	evtiso3[200];
    Float_t 	evtiso4[200];
    Float_t 	evtchiso3[200];
    Float_t 	evtchiso4[200];
    Float_t 	evtdxy[200];
    Float_t 	evtdz[200];
    Int_t 	evtfromtau[200];

    //if( 100*(double)k/n> count){cout<<count<<endl;++count;}

    //Definitions to use T&P
    //More than two T&P because of combinatorics
    int tag[10];
    Prob prob[10];

    for(int _i = 0; _i < 10; ++_i){

    tag[_i] = 9999;
    prob[_i].LEP = 9999;
      
    }

    tree[tree_i]->GetEntry(k);


    //to count number tag/probe
      int ntag  = 0;
      int nprob = 0;

    for (int i = 0; i < On+Gn; ++i) {

      //Separate here other from loose
      if(i < Gn){

	evtloose[i]			  = 1;
	evtid[i]                      = Gid[i];
	evtpt[i]                      = Gpt[i];
	evtm[i]                       = Gm[i];
	evteta[i]                     = Geta[i];
	evtphi[i]                     = Gphi[i];
	evtq[i]                       = Gq[i];
	evttight[i]                   = Gtight[i];
	evttighte[i]                  = Gtighte[i];
	evtiso3[i]                    = Giso3[i];
	evtiso4[i]                    = Giso4[i];
	evtchiso3[i]                  = Gchiso3[i];
	evtchiso4[i]                  = Gchiso4[i];
	evtdxy[i]                     = Gdxy[i];
	evtdz[i]                      = Gdz[i];
	evtfromtau[i]                 = Gfromtau[i];

      }else if((i >=  Gn)&&(i < Gn+On)){

	evtloose[i]		       = 0;
	evtid[i]                   = Oid[i-Gn];
	evtpt[i]                   = Opt[i-Gn];
	evtm[i]                    = Om[i-Gn];
	evteta[i]                  = Oeta[i-Gn];
	evtphi[i]                  = Ophi[i-Gn];
	evtq[i]                    = Oq[i-Gn];
	evttight[i]                = Otight[i-Gn];
	evttighte[i]               = Otighte[i-Gn];
	evtiso3[i]                 = Oiso3[i-Gn];
	evtiso4[i]                 = Oiso4[i-Gn];
	evtchiso3[i]               = Ochiso3[i-Gn];
	evtchiso4[i]               = Ochiso4[i-Gn];
	evtdxy[i]                  = Odxy[i-Gn];
	evtdz[i]                   = Odz[i-Gn];
	evtfromtau[i]              = Ofromtau[i-Gn];

      }

      //define sel_den using bools
      bool reliso3((sel_num != "reliso3")||((sel_num == "reliso3")&&(evtiso3[i] < cut_num )));
      bool reliso4((sel_num != "reliso4")||((sel_num == "reliso4")&&(evtiso4[i] < cut_num )));
      bool chiso3((sel_num != "chiso3")||((sel_num == "chiso3")&&(evtchiso3[i] < cut_num )));
      bool chiso4((sel_num != "chiso4")||((sel_num == "chiso4")&&(evtchiso4[i] < cut_num )));
      bool dxy((sel_num != "dxy")||((sel_num == "dxy")&&(abs(evtdxy[i])< cut_num )));
      bool dz((sel_num != "dz")||((sel_num == "dz")&&(abs(evtdz[i])< cut_num )));
      bool tight((sel_num != "tightcut")||(((abs(evtid[i]) == 13)&&(sel_num == "tightcut")&&(evttight[i] == 1))||((abs(evtid[i]) == 11)&&(sel_num == "tightcut")&&(evttighte[i] >= 3))));
      bool tightmva((sel_num != "tightmva")||((abs(evtid[i]) == 11)&&(sel_num == "tightmva")&&(evttight[i] == 1)));

      //Store the values from both the loose and tight in the same variable

      //Prob sel_denion cut
      if(abs(evtid[i]) == leptonId){
      //if(evtfromtau[i] == 0){
      if((!option.Contains("loose"))||((option.Contains("loose"))&&(evtloose[i] == 1))){
	if(Gn+On > 1){//Need at least two leptons to do the TandP. Should look at it after the selection
	  if((sel_den != "tightcut")||(((abs(evtid[i]) == 13)&&(sel_den == "tightcut")&&(evttight[i] == 1))||((abs(evtid[i]) == 11)&&(sel_den == "tightcut")&&(evttighte[i] >= 3)))){ 
	    if((sel_den != "tightmva")||((abs(evtid[i]) == 11)&&(sel_den == "tightmva")&&(evttight[i] == 1))){ 
	      if((!option.Contains("matching"))||((option.Contains("matching"))&&(matched[i] == 1))){ 

		++nprob;

		//Fill prob for each lepton passing the loose selection
		prob[i].LEP  = i;
		prob[i].PT   = evtpt[i];
		prob[i].ETA  = evteta[i];
		prob[i].PHI  = evtphi[i];
		prob[i].M    = evtm[i];

		//Selection cut for Tag only
		if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightmva){

		  ++ntag;

		  tag[i] = i;

		}
	      }
	    }
	  }
	}
      }
    }
    }

      //select best match
      if(prob[2].LEP != 9999){//If there are more than 2 electron passing the loose id

	
      }

      //Fill to count number tag/prob
      h_prob->Fill(nprob);
      h_tag->Fill(ntag);

    //loop over all the rec particles to find the isolation
    //We requiere one tag at least 
    while((tag[0] != 9999)||(tag[1] != 9999)){

      int l1;
      int l2;

      if(tag[0] != 9999){l1 = prob[1]; l2 = tag[0]; tag[0] = 9999;}
      else if(tag[1] != 9999){l1 = prob[0]; l2 = tag[1]; tag[1] = 9999;}

      double M = InvMass(evtpt[l1],evteta[l1],evtphi[l1],evtm[l1],evtpt[l2],evteta[l2],evtphi[l2],evtm[l2]);

      if(l1 != 9999){

	//Parameter on the xaxis

	double par_1;
	double par_2;

	//Choose the parameter to be filled for the eff.
	if(par_x == "Pt"){par_1 = evtpt[l1];}
	else if(par_x == "eta"){par_1 = evteta[l1];}
	else if(par_x == "phi"){par_1 = evtphi[l1];}
	if(par_y == "Pt"){par_2 = evtpt[l1];}
	else if(par_y == "eta"){par_2 = abs(evteta[l1]);}
	else if(par_y == "phi"){par_2 = abs(evtphi[l1]);}

	for(int ii = 0; ii < npar1bins; ++ii){
	  if((par_1 > par1[ii])&&(par_1 < par1[ii+1])){


	    bool reliso3((sel_num != "reliso3")||((sel_num == "reliso3")&&(evtiso3[l1] < cut_num )));
	    bool reliso4((sel_num != "reliso4")||((sel_num == "reliso4")&&(evtiso4[l1] < cut_num )));
	    bool chiso3((sel_num != "chiso3")||((sel_num == "chiso3")&&(evtchiso3[l1] < cut_num )));
	    bool chiso4((sel_num != "chiso4")||((sel_num == "chiso4")&&(evtchiso4[l1] < cut_num )));
	    bool dxy((sel_num != "dxy")||((sel_num == "dxy")&&(abs(evtdxy[l1])< cut_num )));
	    bool dz((sel_num != "dz")||((sel_num == "dz")&&(abs(evtdz[l1])< cut_num )));
	    bool tight((sel_num != "tightcut")||(((abs(evtid[l1]) == 13)&&(sel_num == "tightcut")&&(evttight[l1] == 1))||((abs(evtid[l1]) == 11)&&(sel_num == "tightcut")&&(evttighte[l1] >= 3))));
	    bool tightmva((sel_num != "tightmva")||((abs(evtid[l1]) == 11)&&(sel_num == "tightmva")&&(evttight[l1] == 1)));

	    //Find the corresponding histogram for par2
	    int kk = -1;
	    for(int _i = 0; _i < npar2bins; ++_i){
	      if((par_2 > par2[_i])&&(par_2 <= par2[_i+1])){kk = _i;}
	    }

	    //Fill the par1 distribution histogram
	    if(kk != -1){histo_par1[kk]->Fill(par_1);}
	    if((kk != -1)&&(evt_id == 701)){histo_par1_dyonly[kk]->Fill(par_1);}
	    if(kk != -1){h_par1[kk][ii]->Fill(par_1);}
	    if((kk != -1)&&(evt_id == 701)){h_par1_dyonly[kk][ii]->Fill(par_1);}

	    //Efficiency cut
	    if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightmva && (kk != -1)){

	      histo_M_DYJets_bkg[kk][ii]->Fill(M,scale);

	      if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)||(evt_id == 300)){

		histo_M_bkg[kk][ii]->Fill(M,scale);

		//cout<<"Filled some bkg. The event id is "<<evt_id<<endl;

		//Just the Wjets bkg
		if ((evt_id == 500)||(evt_id == 502)||(evt_id == 503)||(evt_id == 504)||(evt_id == 505)){

		  histo_M_WJets[kk][ii]->Fill(M,scale);

		//cout<<"Filled some wejts. The event id is "<<evt_id<<endl;

		}else if (evt_id == 300){

		  histo_M_TTJets[kk][ii]->Fill(M,scale);
		//cout<<"Filled some TTjets. The event id is "<<evt_id<<endl;

		}

	      }else if((evt_id == 702)||(evt_id == 703)||(evt_id == 704)||(evt_id == 705)||(evt_id == 701)){

		histo_M_DYJets[kk][ii]->Fill(M,scale);

	      }

	    }
    //cout<<"debug8"<<endl;


	    reliso3 = ((sel_num != "reliso3")||((sel_num == "reliso3")&&(evtiso3[l1] >= cut_num )));
	    reliso4 = ((sel_num != "reliso4")||((sel_num == "reliso4")&&(evtiso4[l1] >= cut_num )));
	    chiso3 = ((sel_num != "chiso3")||((sel_num == "chiso3")&&(evtchiso3[l1] >= cut_num )));
	    chiso4 = ((sel_num != "chiso4")||((sel_num == "chiso4")&&(evtchiso4[l1] >= cut_num )));
	    dxy = ((sel_num != "dxy")||((sel_num == "dxy")&&(abs(evtdxy[l1]) >= cut_num )));
	    dz = ((sel_num != "dz")||((sel_num == "dz")&&(abs(evtdz[l1]) >= cut_num )));
	    tight = ((sel_num != "tightcut")||(((abs(evtid[l1]) == 13)&&(sel_num == "tightcut")&&(evttight[l1] != 1))||((abs(evtid[l1]) == 11)&&(sel_num == "tightcut")&&(evttighte[l1] < 3))));
	    tightmva = ((sel_num != "tightmva")||((abs(evtid[l1]) == 11)&&(sel_num == "tightmva")&&(evttight[l1] != 1)));

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
  }

  mkdir(_path+_fname+"_PDF/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  TFile* canvas_out = new TFile(_path+_fname+"_PDF/histograms.root","recreate"); 

  for(int i = 0; i < npar2bins; ++i){ 

    //String for output file name
      TString _parxbin;
      TString _parybin;
      TString _parxtitle;
      TString _parytitle;
      TString _parx;
      TString _pary;

    for(int j = 0; j < npar1bins; ++j){ 

      ////////////////////
      //Build histograms//
      ////////////////////

      //String for name of the ouput files and histograms titles
      //Parameter string
      TString _par;
      if(par_x == "Pt"){_parxbin = Form("%0.f_Pt%0.f",par1[j],par1[j+1]);}
      else if(par_x == "eta"){_parxbin = Form("%0.3f_eta%0.3f",par1[j],par1[j+1]);}
      else if(par_x == "phi"){_parxbin = Form("%0.3f_phi%0.3f",par1[j],par1[j+1]);}
      if(par_y == "Pt"){_parybin = Form("%0.f_Pt%0.f",par2[i],par2[i+1]);}
      else if(par_y == "eta"){_parybin = Form("%0.3f_eta%0.3f",par2[i],par2[i+1]);cout<<"it works !"<<endl;}
      else if(par_y == "phi"){_parybin = Form("%0.3f_phi%0.3f",par2[i],par2[i+1]);}

      //Title string
      if(par_x == "Pt"){_parxtitle = Form("%0.f #leq P_{t} #leq %0.f",par1[j],par1[j+1]); _parx = "P_{t}";}
      else if(par_x == "eta"){_parxtitle = Form("%0.3f #leq #eta #leq %0.3f",par1[j],par1[j+1]); _parx = "#eta";}
      else if(par_x == "phi"){_parxtitle = Form("%0.3f #leq #phi #leq %0.3f",par1[j],par1[j+1]); _parx = "#phi";}
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

      //Number of tag/prob per events
      
      TCanvas* c3 = new TCanvas("c3","c3");
      h_tag->Draw();
      h_tag->GetYaxis()->SetRangeUser(0,getmaxallhist(h_tag,h_prob));
      h_tag->GetXaxis()->SetTitle("N "+_pname+ "per event");
      h_tag->SetLineColor(4);
      h_tag->SetLineWidth(2);
      h_prob->Draw("same");
      h_prob->SetLineColor(2);
      h_prob->SetLineWidth(2);

      TLegend* leg3 = new TLegend(0.6, 0.65,0.89,0.89);
      leg3->SetTextFont(43);
      leg3->SetTextSize(25);
      leg3->AddEntry(h_tag, "n. tag");
      leg3->AddEntry(h_prob, "n. prob");
      leg3->SetBorderSize(0);
      leg3->Draw();

      /////////////////////
      //Saving the output//
      /////////////////////

      canvas_out->cd();

      c1->Write("Zbkg_"+_parxbin+"_"+_parybin);
      c2->Write("Z_"+_parxbin+"_"+_parybin);

      TString _cname = _path+_fname+"_PDF/InvM"+_filetag+_parxbin;
      _cname += "_"+_sel_num+"_"+_pname;

      TString _c1name = _cname+ "_Zbkg_bkg_"+_parybin;
      TString _c2name = _cname+ "_Z_bkg_"+_parybin;
      TString _c3name = _cname+ "_n_lep_"+_parybin;

      c1->SaveAs(_c1name+".pdf");
      c2->SaveAs(_c2name+".pdf");
      c2->SaveAs(_c3name+".pdf");

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

      h_par1[i][j]->Write("h_par1_"+_parxbin+"_"+_parybin);
      h_par1_dyonly[i][j]->Write("h_par1_dyonly"+_parxbin+"_"+_parybin);

    }                    

      file_out->cd();
      histo_par1[i]->Write("histo_par1_"+_parybin);
      histo_par1_dyonly[i]->Write("histo_par1_dyonly"+_parybin);

  }

      file_out->cd();
      h_tag->Write("h_tag");
      h_prob->Write("h_prob");

  file_out->Close();
  canvas_out->Close();

  return 0;

}
