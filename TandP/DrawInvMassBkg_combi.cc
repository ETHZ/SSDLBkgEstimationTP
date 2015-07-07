/*********************************************
 * Description - Create the invariant mass distribution that will be used by the T&P
 * Author - Gael L. Perrin
 * Date - Feb 06 2015
 * *******************************************/
using namespace std;

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
#include "TnPCombinatorics.C"
#include <map>

//Helper functions
#include "../tools/InvMass.C"
#include "../tools/DeltaR.C"
#include "../tools/setTDRStyle.C"
#include "../tools/getmaxallhist.C"


//Int_t 	Gtight[200];
//Int_t 	Gtighte[200];
//Int_t 	Gloose[200];
//Float_t 	Giso3[200];
//Float_t 	Giso4[200];
//Float_t 	Gchiso3[200];
//Float_t 	Gchiso4[200];
//Float_t 	Gdxy[200];
//Float_t 	Gdz[200];
//Int_t         Gfromtau[200];
//Int_t         Gmatched[200];

////////////////////////
//Variable description//
////////////////////////

//Function: Draw the invariant mass. Take array for parx and pary range as argument.
int DrawInvMassBkgMain(TString _filetag, vector<TTree*> tree, int leptonId, double* par1 ,int npar1bins , double* par2, int npar2bins, TString sel_den, TString sel_num , double cut_num, TString par_x, TString par_y, TString option);

int DrawInvMassBkg( TString _filetag, int leptonId, TString sel_den, TString sel_num , double cut_num, TString par_x, TString par_y, TString option );

////////////////////////
//Function definitions//
////////////////////////


int     DrawInvMassBkg(TString _filetag, int leptonId, double* par1, int npar1bins , double* par2 ,int npar2bins , TString sel_den, TString sel_num , double cut_num, TString par_x, TString par_y, TString option = ""){


  //Location of the .root file
  //TString location = "/shome/gaperrin/CERN_data/newcsasample/postprocessed/";//csa14
  TString location = "/shome/gaperrin/tnp_dir/tnp_MiniAOD/DYJetsToLL_M50/";//csa14

  vector< TTree* > tree;

  //Reading the tree 
  //
  TChain* tree_dy = new TChain("tree");

  //For csa14
  //DY events
  //tree_dy->Add(location+"/matched/DYJetsToLLM50_PU_S14_POSTLS170.root");
  cout<<"DEBUG!!!"<<endl;
  tree_dy->Add(location+"tree.root");
  cout<<"Try to read the file"<< location+"tree.root" <<endl;

  tree.push_back(tree_dy);
  cout<<"the number of entries is "<< tree[0]->GetEntries();

  return DrawInvMassBkgMain(_filetag, tree, leptonId, par1, npar1bins, par2, npar2bins, sel_den, sel_num, cut_num, par_x, par_y, option );

}

int     DrawInvMassBkg( TString _filetag, int leptonId, double par_low , double par_upp ,int npar1bins , TString sel_den, TString sel_num , double cut_num, TString par_x, TString par_y, TString option = ""){

  //Parameter 1
  double* par1 = new double[npar1bins+1];
  double Dpar = (double)(par_upp - par_low)/(double)npar1bins;

  for(int i = 0; i < npar1bins+1; ++i){
    par1[i] = par_low + i*Dpar;
  }

  //Parameter 2
  const int npar2bins_eta = 3;
  const int npar2bins_pt = 1;

  double par2_eta[npar2bins_eta+1];
  if(leptonId == 13){
    par2_eta[0] = 0;
    par2_eta[1] = 0.9;
    par2_eta[2] = 1.2;
    par2_eta[3] = 2.5;
  }else if(leptonId == 11){
    par2_eta[0] = 0;
    par2_eta[1] = 1.45;
    par2_eta[2] = 1.67;
    par2_eta[3] = 2.5;
  }
  double par2_pt[npar2bins_pt+1] = {10,250};


  if(par_y == "eta"){
    return DrawInvMassBkg(_filetag, leptonId, par1, npar1bins, par2_eta, npar2bins_eta, sel_den, sel_num, cut_num, par_x, par_y, option );
  }else if(par_y == "Pt"){
    return DrawInvMassBkg(_filetag, leptonId, par1, npar1bins, par2_pt, npar2bins_pt, sel_den, sel_num, cut_num, par_x, par_y, option );
  }else{return 1;}

}


int     DrawInvMassBkgMain(TString _filetag, vector< TTree* > tree, int leptonId, double* par1 ,int npar1bins , double* par2, int npar2bins, TString sel_den, TString sel_num , double cut_num, TString par_x, TString par_y, TString option){

  option.Append(" ");
  option.Prepend(" ");

  TString _path = "/shome/gaperrin/tnp_dir/InvMtnp/";
  cout<< "gonna write the files in"<<_path<<endl;
  cout<< "DEBUG1"<<endl;

  setTDRStyle();

  //TString _filetag = "_newloose_neworder";

  //Some variables
  Long64_t n[] = {tree[0]->GetEntries()};
  int nbins = 200;
  cout<<"The entries of the tree "<<n[0]<<endl;

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
  else if(sel_den == "tightid"){_sel_den = "tightid";}
  else if(sel_den == "mediumid"){_sel_den = "mediumid";}
  else if(sel_den == "softid"){_sel_den = "softid";}
  else if(sel_den == "pfid"){_sel_den = "pfid";}
  else if(sel_den == "tightmva"){_sel_den = "tightmva";}
  else if(sel_den == "loose"){_sel_den = "loose";}
  else if(sel_den == ""){_sel_den = "";}
  else{cout<<"ERROR: wrong sel_denion !";return 1;}
  //sel_num string
  if((sel_num == "tightmva")&&(leptonId == 13)){cout<<"ERROR: no tightId MVA defined for the muon !"<<endl;return 1;}
  if(sel_num == "tightcut"){_sel_num = "tightcut";}
  else if(sel_num == "tightid"){_sel_num = "tightid";}
  else if(sel_num == "mediumid"){_sel_num = "mediumid";}
  else if(sel_num == "softid"){_sel_num = "softid";}
  else if(sel_num == "pfid"){_sel_num = "pfid";}
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
  if(option.Contains(" matching ")){_option += "_Matched";}
  if(option.Contains(" short ")){_option += "_short";}
  if(option.Contains(" loose ")){_option += "_loose";}
  if(option.Contains(" oldtree ")){_option += "_oldtree";}
  if(option.Contains(" dyonly ")){_option += "_dyonly";}
  if(option.Contains(" closestZM ")){_option += "_combi_Zm";}
  if(option.Contains(" looseprobe ")){_option += "_looseprobe";}
  _option += "_";
  //parameter range string
  TString _par1range;
  _par1range = Form("%0.3f_"+par_x+"%0.3f",par1[0],par1[npar1bins]);
  TString _par2range;
  _par2range = Form("%0.3f_"+par_y+"%0.3f",par2[0],par2[npar2bins]);

  /////////////////////////////////////
  //Write the name of the output file//
  /////////////////////////////////////
  cout<< "DEBUG3"<<endl;

  //Define name of the files
  TString _fname = "InvM"+_filetag+_option+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num;

  TFile* file_out = new TFile(_path+_fname+".root","recreate");

  TH1D*** histo_M  = new TH1D**[npar2bins];
  TH1D*** histo_M_fail  = new TH1D**[npar2bins];

  //Histogram for the par1 bin distribution
  TH1D** histo_par1 = new TH1D*[npar2bins];
  //Paramter for each bin
  TH1D*** h_par1 = new TH1D**[npar2bins];

  //Histogram to count the total number of tag and prob per event

  TH1D* h_tag = new TH1D("h_tag","ntag",10,0,10);
  TH1D* h_prob = new TH1D("h_prob","nprob",10,0,10);


  //Histogram to count the total number of tag and prob per bin
  TH1D*** h_tag_bin = new TH1D**[npar2bins];
  TH1D*** h_prob_bin = new TH1D**[npar2bins];


  for(int i = 0; i < npar2bins; ++i){ 

    histo_M[i]  		= new TH1D*[npar1bins];
    histo_M_fail[i]  		= new TH1D*[npar1bins];
    h_tag_bin[i] 			= new TH1D*[npar1bins];
    h_prob_bin[i] 			= new TH1D*[npar1bins];
    histo_par1[i] 			= new TH1D(Form("histo_par1_par2bin_%i",i),"par1",npar1bins*25,par1[0],par1[npar1bins]);
    h_par1[i] 				= new TH1D*[npar1bins];

    for(int j = 0; j < npar1bins; ++j){ 

      histo_M[i][j]  		=  new TH1D(Form("histo_M_DYJets_bkg_par2bin_%i_%i",i,j),"M",nbins,0,250);
      histo_M_fail[i][j]  		=  new TH1D(Form("histo_M_DYJets_bkg_par2bin_%i_%i",i,j),"M",nbins,0,250);
      h_tag_bin[i][j] 				= new TH1D(Form("h_tag_bin_par2bin_%i_%i",i,j),"tag",10,0,10);
      h_prob_bin[i][j] 				= new TH1D(Form("h_prob_bin_par2bin_%i_%i",i,j),"prob",10,0,10);

      //Par1 distribution
      h_par1[i][j] 				= new TH1D(Form("h_par1_par2bin_%i_%i",i,j),"par1",25,par1[j],par1[j+1]);

    }

  }

  //Event variables
  Int_t evt_id;
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
  Int_t Otightid[200];
  Int_t Omediumid[200];
  Int_t Osoftid[200];
  Int_t Opfid[200];
  Int_t Oloose[200];
  Float_t Oiso3[200];
  Float_t Oiso4[200];
  Float_t Ochiso3[200];
  Float_t Ochiso4[200];
  Float_t Odxy[200];
  Float_t Odz[200];
  Int_t Ofromtau[200];
  Int_t Omatched[200];

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
  Int_t 	Gtightid[200];
  Int_t 	Gmediumid[200];
  Int_t 	Gsoftid[200];
  Int_t 	Gpfid[200];
  Int_t 	Gloose[200];
  Float_t 	Giso3[200];
  Float_t 	Giso4[200];
  Float_t 	Gchiso3[200];
  Float_t 	Gchiso4[200];
  Float_t 	Gdxy[200];
  Float_t 	Gdz[200];
  Int_t         Gfromtau[200];
  Int_t         Gmatched[200];
  cout<< "DEBUG4"<<endl;

  int nsample = 1;
  if(option.Contains("dyonly"))nsample = 1;


  for(int tree_i = 0; tree_i < nsample; ++ tree_i){

    //Assigne branches tree->SetBranchAddress("evt_scale1fb", &scale);
    cout<< "DEBUG5"<<endl;
    tree[tree_i]->SetBranchAddress("evt_id", &evt_id);
    //generated
    tree[tree_i]->SetBranchAddress("nGenPart", &ngenPart);
    tree[tree_i]->SetBranchAddress("GenPart_pdgId", &Id);
    tree[tree_i]->SetBranchAddress("GenPart_sourceId", &source);
    tree[tree_i]->SetBranchAddress("GenPart_motherId", &Mo);
    tree[tree_i]->SetBranchAddress("GenPart_eta", &gen_eta);
    tree[tree_i]->SetBranchAddress("GenPart_phi", &gen_phi);
    tree[tree_i]->SetBranchAddress("GenPart_pt", &Pt);
    tree[tree_i]->SetBranchAddress("GenPart_mass", &m);
    tree[tree_i]->SetBranchAddress("GenPart_charge", &charge);
    tree[tree_i]->SetBranchAddress("GenPart_status", &status);
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
    tree[tree_i]->SetBranchAddress("LepOther_tightId",&Otightid);
    tree[tree_i]->SetBranchAddress("LepOther_mediumMuonId",&Omediumid);
    tree[tree_i]->SetBranchAddress("LepOther_softMuonId",&Osoftid);
    tree[tree_i]->SetBranchAddress("LepOther_pfMuonId",&Opfid);
    tree[tree_i]->SetBranchAddress("LepOther_relIso03",&Oiso3);
    tree[tree_i]->SetBranchAddress("LepOther_relIso04",&Oiso4);
    tree[tree_i]->SetBranchAddress("LepOther_chargedHadRelIso03",&Ochiso3);
    tree[tree_i]->SetBranchAddress("LepOther_chargedHadRelIso04",&Ochiso4);
    tree[tree_i]->SetBranchAddress("LepOther_dxy",&Odxy);
    tree[tree_i]->SetBranchAddress("LepOther_dz",&Odz);
    tree[tree_i]->SetBranchAddress("LepOther_mcMatchTau",&Ofromtau);
    if(tree_i == 0)tree[tree_i]->SetBranchAddress("LepOther_matched",&Omatched);
    //Loose
    tree[tree_i]->SetBranchAddress("nLepGood",&Gn);
    tree[tree_i]->SetBranchAddress("LepGood_pdgId",&Gid);
    tree[tree_i]->SetBranchAddress("LepGood_pt",&Gpt);
    tree[tree_i]->SetBranchAddress("LepGood_mass",&Gm);
    tree[tree_i]->SetBranchAddress("LepGood_eta",&Geta);
    tree[tree_i]->SetBranchAddress("LepGood_phi",&Gphi);
    tree[tree_i]->SetBranchAddress("LepGood_charge",&Gq);
    tree[tree_i]->SetBranchAddress("LepGood_tightId",&Gtight);
    tree[tree_i]->SetBranchAddress("LepGood_eleCutIdCSA14_50ns_v1",&Gtighte);
    tree[tree_i]->SetBranchAddress("LepOther_tightId",&Gtightid);
    tree[tree_i]->SetBranchAddress("LepOther_mediumMuonId",&Gmediumid);
    tree[tree_i]->SetBranchAddress("LepOther_softMuonId",&Gsoftid);
    tree[tree_i]->SetBranchAddress("LepOther_pfMuonId",&Gpfid);
    tree[tree_i]->SetBranchAddress("LepGood_relIso03",&Giso3);
    tree[tree_i]->SetBranchAddress("LepGood_relIso04",&Giso4);
    tree[tree_i]->SetBranchAddress("LepGood_chargedHadRelIso03",&Gchiso3);
    tree[tree_i]->SetBranchAddress("LepGood_chargedHadRelIso04",&Gchiso4);
    tree[tree_i]->SetBranchAddress("LepGood_dxy",&Gdxy);
    tree[tree_i]->SetBranchAddress("LepGood_dz",&Gdz);
    tree[tree_i]->SetBranchAddress("LepGood_mcMatchTau",&Gfromtau);
    if(tree_i == 0)tree[tree_i]->SetBranchAddress("LepGood_matched",&Gmatched);
    cout<< "DEBUG7"<<endl;

    int count = 0;

    if(option.Contains(" short ")){n[tree_i] = 100000;cout<<"short !"<<endl;}
    cout<<"DEBUG8"<<endl;

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
      Int_t 	evttightid[200];
      Int_t 	evtmediumid[200];
      Int_t 	evtsoftid[200];
      Int_t 	evtpfid[200];
      Float_t 	evtiso3[200];
      Float_t 	evtiso4[200];
      Float_t 	evtchiso3[200];
      Float_t 	evtchiso4[200];
      Float_t 	evtdxy[200];
      Float_t 	evtdz[200];
      Int_t 	evtfromtau[200];
      Int_t 	evtmatched[200];

      //Preparing variables for T&P
      vector <Tag> vec_tag;
      vector <Probe> vec_probe;

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
	  evttightid[i]                 = Gtightid[i];
	  evtmediumid[i]                = Gmediumid[i];
	  evtsoftid[i]                  = Gsoftid[i];
	  evtpfid[i]                    = Gpfid[i];
	  evtiso3[i]                    = Giso3[i];
	  evtiso4[i]                    = Giso4[i];
	  evtchiso3[i]                  = Gchiso3[i];
	  evtchiso4[i]                  = Gchiso4[i];
	  evtdxy[i]                     = Gdxy[i];
	  evtdz[i]                      = Gdz[i];
	  evtfromtau[i]                 = Gfromtau[i];
	  evtmatched[i]                 = Gmatched[i];

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
	  evttightid[i]              = Otightid[i-Gn];
	  evtmediumid[i]             = Omediumid[i-Gn];
	  evtsoftid[i]               = Osoftid[i-Gn];
	  evtpfid[i]                 = Opfid[i-Gn];
	  evtiso3[i]                 = Oiso3[i-Gn];
	  evtiso4[i]                 = Oiso4[i-Gn];
	  evtchiso3[i]               = Ochiso3[i-Gn];
	  evtchiso4[i]               = Ochiso4[i-Gn];
	  evtdxy[i]                  = Odxy[i-Gn];
	  evtdz[i]                   = Odz[i-Gn];
	  evtfromtau[i]              = Ofromtau[i-Gn];
	  evtmatched[i]              = Omatched[i-Gn];

	}

	////////////
	//Set cuts//
	////////////

	//TnP cuts
	bool lepton(abs(evtid[i]) == leptonId);
	bool twolep(Gn+On > 1);

	//Cut on tag
	bool tagtightid((abs(evttightid[i]) == 1 ));
	bool tagptcut(evtpt[i] > 25 );
	bool tageta(abs(evteta[i] < 2.1));
	bool tagselection = tagtightid && tagptcut && tageta;

	////////////////
	//Numerator cuts
	////////////////

	//Variable cuts
	bool reliso3((sel_num != "reliso3")||((sel_num == "reliso3")&&(evtiso3[i] < cut_num )));
	bool reliso4((sel_num != "reliso4")||((sel_num == "reliso4")&&(evtiso4[i] < cut_num )));
	bool chiso3((sel_num != "chiso3")||((sel_num == "chiso3")&&(evtchiso3[i] < cut_num )));
	bool chiso4((sel_num != "chiso4")||((sel_num == "chiso4")&&(evtchiso4[i] < cut_num )));
	bool dxy((sel_num != "dxy")||((sel_num == "dxy")&&(abs(evtdxy[i])< cut_num )));
	bool dz((sel_num != "dz")||((sel_num == "dz")&&(abs(evtdz[i])< cut_num )));

	//ID cuts
	bool tight((sel_num != "tightcut")||(((abs(evtid[i]) == 13)&&(sel_num == "tightcut")&&(evttight[i] == 1))||((abs(evtid[i]) == 11)&&(sel_num == "tightcut")&&(evttighte[i] >= 3))));
	bool tightid((sel_num != "tightid")||((sel_num == "tightid")&&(abs(evttightid[i]) == 1)));
	bool mediumid((sel_num != "mediumid")||((sel_num == "mediumid")&&(abs(evtmediumid[i]) == 1 )));
	bool softid((sel_num != "softid")||((sel_num == "softid")&&(abs(evtsoftid[i]) == 1 )));
	bool pfid((sel_num != "pfid")||((sel_num == "pfid")&&(abs(evtpfid[i]) == 1 )));
	bool tightmva((sel_num != "tightmva")||((abs(evtid[i]) == 11)&&(sel_num == "tightmva")&&(evttight[i] == 1)));
	bool loose((!option.Contains("loose"))||((option.Contains("loose"))&&(evtloose[i] == 1)));

	//////////////////
	//Denominator cuts
	//////////////////

	//ID cuts
	bool dentight((sel_den != "tightcut")||(((abs(evtid[i]) == 13)&&(sel_den == "tightcut")&&(evttight[i] == 1))||((abs(evtid[i]) == 11)&&(sel_den == "tightcut")&&(evttighte[i] >= 3)))); 
	bool dentightmva((sel_den != "tightmva")||((abs(evtid[i]) == 11)&&(sel_den == "tightmva")&&(evttight[i] == 1)));             
	bool dentightid((sel_den != "tightid")||((sel_den == "tightid")&&(abs(evttightid[i]) == 1)));
	bool denmediumid((sel_den != "mediumid")||((sel_den == "mediumid")&&(abs(evtmediumid[i]) == 1 )));
	bool densoftid((sel_den != "softid")||((sel_den == "softid")&&(abs(evtsoftid[i]) == 1 )));
	bool denpfid((sel_den != "pfid")||((sel_den == "pfid")&&(abs(evtpfid[i]) == 1 )));
	bool match (!option.Contains("matching") || ((option.Contains("matching"))&&(((tree_i == 0)&&(evtmatched[i] == 1)) || (tree_i > 0))));

	//Denominator cuts
	if( lepton && denpfid && dentight && dentightmva && match && twolep){
	  //Need at least two leptons to do the TandP. Should look at it after the selection

	  //Define and fil the prob
	  Probe prob;
	  prob.I = i;
	  prob.PT = evtpt[i];
	  prob.ETA = evteta[i];
	  prob.PHI = evtphi[i];
	  prob.Q = evtq[i];
	  prob.TIGHT = evttight[i];
	  prob.TIGHTID = evttightid[i];
	  prob.MEDIUMID = evtmediumid[i];
	  prob.SOFTID = evtsoftid[i];
	  prob.PFID = evtpfid[i];
	  prob.LOOSE = evtloose[i];
	  prob.RELISO3 = evtiso3[i];
	  prob.RELISO4 = evtiso4[i];
	  prob.DXY= evtdxy[i];
	  prob.DZ = evtdz[i];
	  prob.FROMTAU= evtfromtau[i];
	  prob.MATCHED= evtmatched[i];

	  vec_probe.push_back(prob);

	  //Selection cut for Tag only
	  if(tagselection && reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightid && mediumid && softid && pfid && tightmva){

	    Tag tag;
	    tag.I = i;
	    tag.PT = evtpt[i];
	    tag.ETA = evteta[i];
	    tag.PHI = evtphi[i];
	    tag.M = evtm[i];
	    tag.ID = evtid[i];
	    tag.Q = evtq[i];
	    tag.TIGHT = evttight[i];
	    tag.TIGHTID = evttightid[i];
	    tag.MEDIUMID = evtmediumid[i];
	    tag.SOFTID = evtsoftid[i];
	    tag.PFID = evtpfid[i];
	    tag.LOOSE = evtloose[i];
	    tag.RELISO3 = evtiso3[i];
	    tag.RELISO4 = evtiso4[i];
	    tag.DXY= evtdxy[i];
	    tag.DZ = evtdz[i];
	    tag.FROMTAU= evtfromtau[i];
	    tag.MATCHED= evtmatched[i];

	    vec_tag.push_back(tag);
	  }
	}
      }

      cout<<"The size of the Prob vector is "<<vec_probe.size()<<endl;
      //end of reclep loop

      //need at least two leptons
      if(vec_probe.size() == 1) continue;
      //take events with exaclty two leptons
      if(option.Contains(" 2lep ") && vec_probe.size() != 2) continue;

      //do the loop on all the Tags
      for(int t = 0; t < vec_tag.size(); ++t){

	//Cuts on the tag
	cout<<"DEBUG10b"<<endl;

	//add list of probes in each tag
	vec_tag[t].PROBE = vec_probe;
	//Define the final Tag and Probe using the combinatorics
	Tag _tag = combine(vec_tag[t],option); 

	//do the loop on all the probes for the corresponding Tag
	for(int p = 0; p < _tag.PROBE.size(); ++p){

	  //retrieve the indices of the tag and the probe
	  int l2 = _tag.I;
	  int l1 = _tag.PROBE[p].I;

	  if(l1 == l2){cout<<"ERROR: tag and probe are the same !"<<endl;return 1;}

	  double M = InvMass(evtpt[l1],evteta[l1],evtphi[l1],evtm[l1],evtpt[l2],evteta[l2],evtphi[l2],evtm[l2]);

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
	      bool tightid((sel_num != "tightid")||((sel_num == "tightid")&&(abs(evttightid[l1]) == 1 )));
	      bool mediumid((sel_num != "mediumid")||((sel_num == "mediumid")&&(abs(evtmediumid[l1]) == 1 )));
	      bool softid((sel_num != "softid")||((sel_num == "softid")&&(abs(evtsoftid[l1]) == 1 )));
	      bool pfid((sel_num != "pfid")||((sel_num == "pfid")&&(abs(evtpfid[l1]) == 1 )));
	      bool tightmva((sel_num != "tightmva")||((abs(evtid[l1]) == 11)&&(sel_num == "tightmva")&&(evttight[l1] == 1)));

	      //Find the corresponding histogram for par2
	      int kk = -1;
	      for(int _i = 0; _i < npar2bins; ++_i){
		if((par_2 > par2[_i])&&(par_2 <= par2[_i+1])){kk = _i;}
	      }

	      //Fill the par1 distribution histogram
	      if(kk != -1){histo_par1[kk]->Fill(par_1);}
	      if(kk != -1){h_par1[kk][ii]->Fill(par_1);}

	      //Efficiency cut
	      if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightid && mediumid && softid && pfid && tightmva && (kk != -1)){

		histo_M[kk][ii]->Fill(M);
		cout<<"Filling the tree !"<<endl;

	      }

	      reliso3 = ((sel_num != "reliso3")||((sel_num == "reliso3")&&(evtiso3[l1] >= cut_num )));
	      reliso4 = ((sel_num != "reliso4")||((sel_num == "reliso4")&&(evtiso4[l1] >= cut_num )));
	      chiso3 = ((sel_num != "chiso3")||((sel_num == "chiso3")&&(evtchiso3[l1] >= cut_num )));
	      chiso4 = ((sel_num != "chiso4")||((sel_num == "chiso4")&&(evtchiso4[l1] >= cut_num )));
	      dxy = ((sel_num != "dxy")||((sel_num == "dxy")&&(abs(evtdxy[l1]) >= cut_num )));
	      dz = ((sel_num != "dz")||((sel_num == "dz")&&(abs(evtdz[l1]) >= cut_num )));
	      tight = ((sel_num != "tightcut")||(((abs(evtid[l1]) == 13)&&(sel_num == "tightcut")&&(evttight[l1] != 1))||((abs(evtid[l1]) == 11)&&(sel_num == "tightcut")&&(evttighte[l1] < 3))));
	      tightid = ((sel_num != "tightid")||((sel_num == "tightid")&&(abs(evttightid[l1]) != 1)));
	      mediumid = ((sel_num != "mediumid")||((sel_num == "mediumid")&&(abs(evtmediumid[l1]) != 1)));
	      softid = ((sel_num != "softid")||((sel_num == "softid")&&(abs(evtsoftid[l1]) != 1)));
	      pfid = ((sel_num != "pfid")||((sel_num == "pfid")&&(abs(evtpfid[l1]) != 1)));
	      tightmva = ((sel_num != "tightmva")||((abs(evtid[l1]) == 11)&&(sel_num == "tightmva")&&(evttight[l1] != 1)));

	      if(reliso3 && reliso4 && chiso3 && chiso4 && dxy && dz && tight && tightid && mediumid && softid && pfid && tightmva && (kk != -1)){

		histo_M_fail[kk][ii]->Fill(M);
	      }
	    }
	  }
	}
      }

      h_prob->Fill(nprob);
      h_tag->Fill(ntag);

    }
  }

  /////////////////////////////
  //Build and plot histograms//
  /////////////////////////////

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

      //TCanvas* c1 = new TCanvas("c1","c1");
      TCanvas* c2 = new TCanvas("c2","c2");
      //c1->Divide(1,2);
      c2->Divide(1,2);

      //c1->cd(1);

      //TString _title; 
      TString _stitle = (TString)"Invariant mass for "+pname+" "+_parxtitle+", "+_parytitle+", "+sel_num+" pass";
      TString _stitlefail = (TString)"Invariant mass for "+pname+" "+_parxtitle+", "+_parytitle+", "+sel_num+" fail";


      //Z pass 
      c2->cd(1);
      histo_M[i][j]->Draw();
      histo_M[i][j]->SetTitle(_stitle);
      histo_M[i][j]->GetXaxis()->SetTitle("m [GeV]");
      histo_M[i][j]->SetLineWidth(2);
      histo_M[i][j]->SetLineColor(4);
      histo_M[i][j]->SetMarkerColor(4);
      TLegend* leg2 = new TLegend(0.6, 0.65,0.89,0.89);
      leg2->SetTextFont(43);
      leg2->SetTextSize(25);
      leg2->AddEntry(histo_M[i][j], "Z");
      leg2->SetBorderSize(0);
      leg2->Draw();

      //Z fail
      c2->cd(2);
      histo_M_fail[i][j]->Draw();
      histo_M_fail[i][j]->SetTitle(_stitlefail);
      histo_M_fail[i][j]->GetXaxis()->SetTitle("m [GeV]");
      histo_M_fail[i][j]->SetLineWidth(2);
      histo_M_fail[i][j]->SetLineColor(4);
      histo_M_fail[i][j]->SetMarkerColor(4);

      /////////////////////
      //Saving the output//
      /////////////////////

      canvas_out->cd();

      //c1->Write("Zbkg_"+_parxbin+"_"+_parybin);
      c2->Write("Z_"+_parxbin+"_"+_parybin);

      TString _cname = _path+_fname+"_PDF/InvM"+_filetag+_parxbin;
      _cname += "_"+_sel_num+"_"+_pname;

      TString _c1name = _cname+ "_Zbkg_bkg_"+_parybin;
      TString _c2name = _cname+ "_Z_bkg_"+_parybin;
      TString _c3name = _cname+ "_n_lep_"+_parybin;

      //c1->SaveAs(_c1name+".pdf");
      c2->SaveAs(_c2name+".pdf");

      file_out->cd();
      histo_M[i][j]->Write("histo_M"+_parxbin+"_"+_parybin);
      histo_M_fail[i][j]->Write("histo_M_fail"+_parxbin+"_"+_parybin); 

      h_par1[i][j]->Write("h_par1_"+_parxbin+"_"+_parybin);

    }                    

    file_out->cd();
    histo_par1[i]->Write("histo_par1_"+_parybin);

  }

  file_out->cd();
  h_tag->Write("h_tag");
  h_prob->Write("h_prob");

  file_out->Close();
  canvas_out->Close();

  return 0;

}

int main(int argc, char** argv){
  cout<<"FUUUUUUUUUUUUUUUUCK"<<endl;

  cout<<"The number of parameters are "<<argc-1<<endl;

  if(argc-1 == 8){

    cout<<"The 1th parameter is "<<argv[1]<<endl;
    cout<<"The 2th parameter is "<<argv[2]<<endl;
    cout<<"The 3th parameter is "<<argv[3]<<endl;
    cout<<"The 4th parameter is "<<argv[4]<<endl;
    cout<<"The 5th parameter is "<<argv[5]<<endl;
    cout<<"The 6th parameter is "<<argv[6]<<endl;
    cout<<"The 7th parameter is "<<argv[7]<<endl;
    cout<<"The 8th parameter is "<<argv[8]<<endl;

    return DrawInvMassBkg(argv[1], atof(argv[2]), argv[3],argv[4], atof(argv[5]),argv[6], argv[7],argv[8]);

  }else{

    cout<<"The 1th parameter is "<<argv[1]<<endl;
    cout<<"The 2th parameter is "<<argv[2]<<endl;
    cout<<"The 3th parameter is "<<argv[3]<<endl;
    cout<<"The 4th parameter is "<<argv[4]<<endl;
    cout<<"The 5th parameter is "<<argv[5]<<endl;
    cout<<"The 6th parameter is "<<argv[6]<<endl;
    cout<<"The 7th parameter is "<<argv[7]<<endl;
    cout<<"The 8th parameter is "<<argv[8]<<endl;
    cout<<"The 9th parameter is "<<argv[9]<<endl;
    cout<<"The 10th parameter is "<<argv[10]<<endl;
    cout<<"The 11th parameter is "<<argv[11]<<endl;

    return DrawInvMassBkg(argv[1], atof(argv[2]),atof(argv[3]), atof(argv[4]),atof(argv[5]), argv[6],argv[7], atof(argv[8]),argv[9], argv[10],argv[11]);

  }

}

//Bin already filled in this function
int     DrawInvMassBkg( TString _filetag, int leptonId, TString sel_den, TString sel_num , double cut_num, TString par_x, TString par_y, TString option = ""){

  if(leptonId == 11){
    if((par_x == "Pt")&&(par_y == "eta")){
      const int npar1bins = 18;
      //Parameter 1
      double* par1 = new double[npar1bins+1];

      par1[0] = 7;
      par1[1] = 10;
      par1[2] = 15;
      par1[3] = 20;
      par1[4] = 25;
      par1[5] = 30;
      par1[6] = 35;
      par1[7] = 40;
      par1[8] = 45;
      par1[9] = 50;
      par1[10] = 60;
      par1[11] = 70;
      par1[12] = 80;
      par1[13] = 90;
      par1[14] = 100;
      par1[15] = 120;
      par1[16] = 140;
      par1[17] = 200;
      par1[18] = 250;
      //Parameter 2
      const int npar2bins = 3;
      double par2[npar2bins+1];
      par2[0] = 0;
      par2[1] = 1.45;
      par2[2] = 1.67;
      par2[3] = 2.5;
      return DrawInvMassBkg(_filetag, leptonId, par1, npar1bins, par2, npar2bins, sel_den, sel_num, cut_num, par_x, par_y, option );

    }else if((par_x == "eta")&&(par_y == "Pt")){

      const int npar1bins = 29;
      //Parameter 1
      double* par1 = new double[npar1bins+1];

      par1[0]  = -2.5;
      par1[1]  = -2.4;
      par1[2]  = -2.3;
      par1[3]  = -2.2;
      par1[4]  = -2.1;
      par1[5]  = -1.9;
      par1[6]  = -1.67; 
      par1[7]  = -1.45;
      par1[8]  = -1.3;
      par1[9]  = -1.1;
      par1[10] = -0.9;
      par1[11] = -0.7;
      par1[12] = -0.5;
      par1[13] = -0.3;
      par1[14] = -0.1;
      par1[15] = 0.1;
      par1[16] = 0.3;
      par1[17] = 0.5;
      par1[18] = 0.7;
      par1[19] = 0.9;
      par1[20] = 1.1;
      par1[21] = 1.3;
      par1[22] = 1.45;
      par1[23] = 1.67;
      par1[24] = 1.9;
      par1[25] = 2.1;
      par1[26] = 2.2;
      par1[27] = 2.3;
      par1[28] = 2.4;
      par1[29] = 2.5;

      ////Parameter 2
      //const int npar2bins = 1;
      //double par2[npar2bins+1];
      //par2[0] = 7;
      //par2[1] = 250;
      const int npar2bins = 3;
      double par2[npar2bins+1];
      par2[0] = 7;
      par2[1] = 20;
      par2[2] = 60;
      par2[3] = 250;

      return DrawInvMassBkg(_filetag, leptonId, par1, npar1bins, par2, npar2bins, sel_den, sel_num, cut_num, par_x, par_y, option );
    }

  }else if(leptonId == 13){

    if((par_x == "Pt")&&(par_y == "eta")){
      const int npar1bins = 18;
      //Parameter 1
      double* par1 = new double[npar1bins+1];

      par1[0] = 7;
      par1[1] = 10;
      par1[2] = 15;
      par1[3] = 20;
      par1[4] = 25;
      par1[5] = 30;
      par1[6] = 35;
      par1[7] = 40;
      par1[8] = 45;
      par1[9] = 50;
      par1[10] = 60;
      par1[11] = 70;
      par1[12] = 80;
      par1[13] = 90;
      par1[14] = 100;
      par1[15] = 120;
      par1[16] = 140;
      par1[17] = 200;
      par1[18] = 250;

      //Parameter 2
      const int npar2bins = 3;
      double par2[npar2bins+1];
      par2[0] = 0;
      par2[1] = 0.9;
      par2[2] = 1.2;
      par2[3] = 2.5;

      return DrawInvMassBkg(_filetag, leptonId, par1, npar1bins, par2, npar2bins, sel_den, sel_num, cut_num, par_x, par_y, option );

    }else if((par_x == "eta")&&(par_y == "Pt")){

      const int npar1bins = 31;
      //Parameter 1
      double* par1 = new double[npar1bins+1];

      par1[0]  = -2.5;
      par1[1]  = -2.4;
      par1[2]  = -2.3;
      par1[3]  = -2.2;
      par1[4]  = -2.1;
      par1[5]  = -1.9;
      par1[6]  = -1.7; 
      par1[7]  = -1.5;
      par1[8]  = -1.3;
      par1[9]  = -1.2;
      par1[10] = -1.05;
      par1[11] = -0.9;
      par1[12] = -0.7;
      par1[13] = -0.5;
      par1[14] = -0.3;
      par1[15] = -0.1;
      par1[16] = 0.1;
      par1[17] = 0.3;
      par1[18] = 0.5;
      par1[19] = 0.7;
      par1[20] = 0.9;
      par1[21] = 1.05;
      par1[22] = 1.2;
      par1[23] = 1.3;
      par1[24] = 1.5;
      par1[25] = 1.7;
      par1[26] = 1.9;
      par1[27] = 2.1;
      par1[28] = 2.2;
      par1[29] = 2.3;
      par1[30] = 2.4;
      par1[31] = 2.5;

      //Parameter 2
      //const int npar2bins = 1;
      //double par2[npar2bins+1];
      //par2[0] = 7;
      //par2[1] = 250;
      const int npar2bins = 3;
      double par2[npar2bins+1];
      par2[0] = 7;
      par2[1] = 20;
      par2[2] = 60;
      par2[3] = 250;

      return DrawInvMassBkg(_filetag, leptonId, par1, npar1bins, par2, npar2bins, sel_den, sel_num, cut_num, par_x, par_y, option );
    }
  }
}
