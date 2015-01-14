/*********************************************
 * Description -  This makes the MC ratio plots for the new DY data. The latter contains the necessary branch to study tight efficiency

 * Author - Gaël L. Perrin

 * Date - Jan 05 2015
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
#include "../InvMass.C"
#include "../DeltaR.C"
#include "vector"
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

#include "../setTDRStyle.C"

int MC_Ratio(int leptonId, double* par1, int npar1bins, double* par2, int npar2bins, TString sel_den , TString sel_num, double cut_num = 0., TString par_x = "Pt", TString par_y = "eta", TString option = "");

int MC_Ratio(int leptonId, double par_low, double par_upp, int npar1bins, TString sel_den , TString sel_num, double cut_num = 0., TString par_x = "Pt", TString par_y = "eta", TString option = ""){


  //Parameter 1
  double* par1 = new double[npar1bins+1];
  double Dpar = (double)(par_upp - par_low)/(double)npar1bins;

  for(int i = 0; i < npar1bins+1; ++i){
    par1[i] = par_low + i*Dpar;
  }

  //Parameter 2
  const int npar2bins_eta = 1;
  //const int npar2bins_pt = 19;
  const int npar2bins_pt = 1;

  double par2_eta[npar2bins_eta+1] = {0,2.5};
  double par2_pt[npar2bins_pt+1] = {0,10000};

  if(par_y == "eta"){
    return MC_Ratio(leptonId, par1, npar1bins, par2_eta, npar2bins_eta, sel_den, sel_num, cut_num, par_x, par_y, option );
  }else if(par_y == "Pt"){
    return MC_Ratio(leptonId, par1, npar1bins, par2_pt, npar2bins_pt, sel_den, sel_num, cut_num, par_x, par_y, option );
  }else{return 1;}

}

int MC_Ratio(int leptonId, double par_low, double par_upp, int npar1bins, double par2_low, double par2_upp, int npar2bins, TString sel_den , TString sel_num, double cut_num = 0., TString par_x = "Pt", TString par_y = "eta", TString option = ""){


  //Parameter 1
  double* par1 = new double[npar1bins+1];
  double Dpar = (double)(par_upp - par_low)/(double)npar1bins;

  for(int i = 0; i < npar1bins+1; ++i){
    par1[i] = par_low + i*Dpar;
  }

  //Parameter 2
  double* par2 = new double[npar2bins+1];
  double Dpar2 = (double)(par2_upp - par2_low)/(double)npar1bins;

  for(int i = 0; i < npar2bins+1; ++i){
    par2[i] = par2_low + i*Dpar2;
  }

    return MC_Ratio(leptonId, par1, npar1bins, par2, npar2bins, sel_den, sel_num, cut_num, par_x, par_y, option );

}

int MC_Ratio(int leptonId, double* par1, int npar1bins, double* par2, int npar2bins, TString sel_den , TString sel_num, double cut_num , TString par_x , TString par_y , TString option ){

  setTDRStyle();

  ///////////////
  //Get the TTree
  ///////////////

  //Location of the .root file
  TString location2 = "/Users/GLP/Desktop/CERN_data/dyjetsnew/postprocessed/";

  //Reading the tree 
  //
  TChain* tree = new TChain("treeProducerSusyMultilepton");

  //DY events
  //tree->Add(location+"DYJetsToLLM50_PU_S14_POSTLS170.root");
  tree->Add(location2+"dyjets_good.root");

  //Plot the result

  Long64_t n = tree->GetEntries();

  //Path for input and output file. Written in FitDataPath.txt
  TString _path = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_eff/";

  //////////////////////
  //Name for the plots//
  //////////////////////

  TString _filetag = "";

  TString pname;
  TString _pname;
  TString _par;
  TString _sel_num;
  TString _sel_den;
  TString _option;

  ////Writing string
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
  if(option.Contains(" ll ")){_option += "_ll";}
  if(option.Contains(" unmatched ")){_option += "_unmatched";}
  if(option.Contains(" alleta ")){_option += "_alleta";}
  if(option.Contains(" short ")){_option += "_short";}
  _option += "_";
  //parameter range string
  TString _par1range;
  _par1range = Form("%0.3f_"+par_x+"%0.3f",par1[0],par1[npar1bins]);
  TString _par2range;
  _par2range = Form("%0.3f_"+par_y+"%0.3f",par2[0],par2[npar2bins]);

  /////////////////////////////////////
  //Write the name of the output file//
  /////////////////////////////////////

  TString _fname = "MCeff"+_filetag+_option+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num;

  //Output file
  cout<<"going to create the file"<<endl;
  TFile* file_out = new TFile(_path+_fname+".root","recreate");
  cout<<"done ! "<<endl;

  //Declaration of histogram
  TH1D **histo_num = new TH1D*[npar2bins];
  TH1D **histo_den = new TH1D*[npar2bins];
  TH1D **eff = new TH1D*[npar2bins];


  //Par1 distribution histogram
  TH1D** histo_par1 = new TH1D*[npar2bins];
//Histo separate in Lep_good Lep_other
  //
  TH1D **histo_num_O = new TH1D*[npar2bins];
  TH1D **histo_den_O = new TH1D*[npar2bins];
  TH1D **eff_O = new TH1D*[npar2bins];
  TH1D **histo_num_G = new TH1D*[npar2bins];
  TH1D **histo_den_G = new TH1D*[npar2bins];
  TH1D **eff_G = new TH1D*[npar2bins];

  //Counter LepGood vs LepOther

  TH1D **histo_counter = new TH1D*[npar2bins];
  TH1D **histo_counter_G_par1 = new TH1D*[npar2bins];
  TH1D **histo_counter_O_par1 = new TH1D*[npar2bins];
  TH1D **histo_counter_par1 = new TH1D*[npar2bins];


  for(int _i = 0; _i < npar2bins; ++_i){ 

    histo_num[_i] = new TH1D("histo_num","Pt",npar1bins,par1[0],par1[npar1bins]);
    histo_den[_i] = new TH1D("histo_den","Pt",npar1bins,par1[0],par1[npar1bins]);
    eff[_i] = new TH1D("eff","Pt",npar1bins,par1[0],par1[npar1bins]);

    //
    histo_par1[_i] = new TH1D("histo_par1","par1",npar1bins*25,par1[0],par1[npar1bins]);

    //
    histo_num_O[_i] = new TH1D("histo_num_O","Pt",npar1bins,par1[0],par1[npar1bins]);
    histo_den_O[_i] = new TH1D("histo_den_O","Pt",npar1bins,par1[0],par1[npar1bins]);
    eff_O[_i] = new TH1D("eff_O","Pt",npar1bins,par1[0],par1[npar1bins]);

    histo_num_G[_i] = new TH1D("histo_num_G","Pt",npar1bins,par1[0],par1[npar1bins]);
    histo_den_G[_i] = new TH1D("histo_den_G","Pt",npar1bins,par1[0],par1[npar1bins]);
    eff_G[_i] = new TH1D("eff_G","Pt",npar1bins,par1[0],par1[npar1bins]);

    histo_counter_G_par1[_i] = new TH1D("histo_counter_G_par1","count",npar1bins,par1[0],par1[npar1bins]);
    histo_counter_O_par1[_i] = new TH1D("histo_counter_O_par1","count",npar1bins,par1[0],par1[npar1bins]);
    histo_counter_par1[_i] = new TH1D("histo_counter_par1","count",npar1bins,par1[0],par1[npar1bins]);

    histo_counter[_i] = new TH1D("histo_counter","count",2,0,2); 

  }

  //Event variables
  Int_t evt_id;
  Double_t scale;
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
  Int_t pile_up;
  //not loose
  Int_t On;
  Int_t Oid[200];
  Double_t Opt[200];
  Double_t Om[200];
  Double_t Oeta[200];
  Double_t Ophi[200];
  Int_t   Oq[200];
  Int_t Otight[200];
  Int_t Otighte[200];
  //Double_t Omvaid[200];
  Int_t Oloose[200];
  Double_t Oiso3[200];
  Double_t Oiso4[200];
  Double_t Ochiso3[200];
  Double_t Ochiso4[200];
  Double_t Odxy[200];
  Double_t Odz[200];
  //loose
  Int_t 	Gn;
  Int_t 	Gid[200];
  Double_t 	Gpt[200];
  Double_t 	Gm[200];
  Double_t 	Geta[200];
  Double_t 	Gphi[200];
  Int_t   	Gq[200];
  Int_t 	Gtight[200];
  Int_t 	Gtighte[200];
  Int_t 	Gloose[200];
  Double_t 	Giso3[200];
  Double_t 	Giso4[200];
  Double_t 	Gchiso3[200];
  Double_t 	Gchiso4[200];
  Double_t 	Gdxy[200];
  Double_t 	Gdz[200];

  //Assigne branches tree->SetBranchAddress("evt_scale1fb", &scale);
  tree->SetBranchAddress("evt_id", &evt_id);
  //generated
  tree->SetBranchAddress("nGenPart", &ngenPart);
  tree->SetBranchAddress("GenPart_pdgId", &Id);
  tree->SetBranchAddress("GenPart_sourceId", &source);
  tree->SetBranchAddress("GenPart_eta", &gen_eta);
  tree->SetBranchAddress("GenPart_phi", &gen_phi);
  tree->SetBranchAddress("GenPart_pt", &Pt);
  tree->SetBranchAddress("GenPart_mass", &m);
  tree->SetBranchAddress("GenPart_charge", &charge);
  tree->SetBranchAddress("GenPart_status", &status);
  //not loose
  tree->SetBranchAddress("nLepOther",&On);
  tree->SetBranchAddress("LepOther_pdgId",&Oid);
  tree->SetBranchAddress("LepOther_pt",&Opt);
  tree->SetBranchAddress("LepOther_mass",&Om);
  tree->SetBranchAddress("LepOther_eta",&Oeta);
  tree->SetBranchAddress("LepOther_phi",&Ophi);
  tree->SetBranchAddress("LepOther_charge",&Oq);
  tree->SetBranchAddress("LepOther_tightId",&Otight);
  tree->SetBranchAddress("LepOther_eleCutIdCSA14_50ns_v1",&Otighte);
  tree->SetBranchAddress("LepOther_relIso03",&Oiso3);
  tree->SetBranchAddress("LepOther_relIso04",&Oiso4);
  tree->SetBranchAddress("LepOther_chargedHadRelIso03",&Ochiso3);
  tree->SetBranchAddress("LepOther_chargedHadRelIso04",&Ochiso4);
  tree->SetBranchAddress("LepOther_dxy",&Odxy);
  tree->SetBranchAddress("LepOther_dz",&Odz);
  //Loose
  tree->SetBranchAddress("nLepGood",&Gn);
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

  //Count lepgood/other

  int goodcount = 0;
  int othercount = 0;


  if(option.Contains(" short ")){n = 100000;}

  //Start loop over all events
  for (int k = 0; k < n; ++k) {

    //Declaration of event parameters
    Int_t 	evtn;
    Int_t 	evtloose[200];
    Int_t 	evtid[200];
    Double_t 	evtpt[200];
    Double_t 	evtm[200];
    Double_t 	evteta[200];
    Double_t 	evtphi[200];
    Int_t   	evtq[200];
    Int_t 	evttight[200];
    Int_t 	evttighte[200];
    Double_t 	evtiso3[200];
    Double_t 	evtiso4[200];
    Double_t 	evtchiso3[200];
    Double_t 	evtchiso4[200];
    Double_t 	evtdxy[200];
    Double_t 	evtdz[200];

    if( 100*(double)k/n> count){cout<<count<<endl;++count;}

    tree->GetEntry(k);

    //Selection on denominator
    for(int j = 0; j < Gn+On; ++j){

      //Separate here other from loose
      if(j < On){

	evtloose[j]			  = 0;
	evtid[j]                      = Oid[j];
	evtpt[j]                      = Opt[j];
	evtm[j]                       = Om[j];
	evteta[j]                     = Oeta[j];
	evtphi[j]                     = Ophi[j];
	evtq[j]                       = Oq[j];
	evttight[j]                   = Otight[j];
	evttighte[j]                  = Otighte[j];
	evtiso3[j]                    = Oiso3[j];
	evtiso4[j]                    = Oiso4[j];
	evtchiso3[j]                  = Ochiso3[j];
	evtchiso4[j]                  = Ochiso4[j];
	evtdxy[j]                     = Odxy[j];
	evtdz[j]                      = Odz[j];


      }else if((j >=  On)&&(j < Gn+On)){

	evtloose[j]		       = 1;
	evtid[j]                   = Gid[j-On];
	evtpt[j]                   = Gpt[j-On];
	evtm[j]                    = Gm[j-On];
	evteta[j]                  = Geta[j-On];
	evtphi[j]                  = Gphi[j-On];
	evtq[j]                    = Gq[j-On];
	evttight[j]                = Gtight[j-On];
	evttighte[j]               = Gtighte[j-On];
	evtiso3[j]                 = Giso3[j-On];
	evtiso4[j]                 = Giso4[j-On];
	evtchiso3[j]               = Gchiso3[j-On];
	evtchiso4[j]               = Gchiso4[j-On];
	evtdxy[j]                  = Gdxy[j-On];
	evtdz[j]                   = Gdz[j-On];

      }

      if((!option.Contains(" ll "))||((option.Contains(" ll "))&&(Gn+On == 2)&&(evtq[0] == -evtq[1]))){
	if(abs(evtid[j]) == leptonId){
	  //Cut on the denominator
	  //if((sel_den != "loose")||(evtloose[j] == 1)){
	  //if(evtloose[j] == 1){
	    if((sel_den != "tightcut")||(((abs(evtid[j]) == 13)&&(sel_den == "tightcut")&&(evttight[j] == 1 ))||((abs(evtid[j]) == 11)&&(sel_den == "tightcut")&&(evttighte[j] >= 3)))){
	      if((sel_den != "tightmva")||((abs(evtid[j]) == 11)&&(sel_den == "tightmva")&&(evttight[j] == 1))){
		//Veto the EE-EB gape

		//Variable for matching
		double R = 999;
		double delta_P = 999;
		double delta_charge = 999;

		//Parameter on the xaxis

		double par;
		double par_2;


		//loop over all generated particles to do the matching
		for (int i = 0; i < ngenPart; ++i) {
		  if((abs(Id[i]) == leptonId)){ 

		    //Electrons selection
		    double R2 = DeltaR(gen_eta[i],evteta[j],gen_phi[i],evtphi[j] );

		    //Minimise DeltaR and Fill the other variables
		    if (R > R2) {

		      R = R2;
		      delta_P = abs(evtpt[j]-Pt[i])/Pt[i];
		      delta_charge = abs(evtq[j] - charge[i]);
		    //}
		  }
		}

		//Choose the parameter to be filled for the eff.
		if(par_x == "Pt"){par = evtpt[j];}
		else if(par_x == "eta"){par = evteta[j];}
		else if(par_x == "phi"){par = evtphi[j];}
		if(par_y == "Pt"){par_2 = evtpt[j];}
		else if(par_y == "eta"){par_2 = abs(evteta[j]);}
		else if(par_y == "phi"){par_2 = abs(evtphi[j]);}

		//Fill Pt only for matched events
		if(((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5))||option.Contains(" unmatched ")){

		  for(int ii = 0; ii < npar2bins; ++ii){
		    if((par_2 > par2[ii])&&(par_2 <= par2[ii+1])){histo_den[ii]->Fill(par);histo_par1[ii]->Fill(par);}
		  }

		  //Additional cut on the numerator
		  int a = 0;

		  if((sel_num == "tightcut")&&(abs(evtid[j]) == 13)&&(evttight[j] == 1)){a = 1;}
		  if((sel_num == "tightcut")&&(abs(evtid[j]) == 11)&&(evttighte[j] >= 3)){a = 1;}
		  if((sel_num == "reliso3")&&(evtiso3[j] <= cut_num)){a = 2;}
		  if((sel_num == "reliso4")&&(evtiso4[j] <= cut_num)){a = 3;}
		  if((sel_num == "chiso3")&&(evtchiso3[j] <= cut_num)){a = 4;}
		  if((sel_num == "chiso4")&&(evtchiso4[j] <= cut_num)){a = 5;}
		  if((sel_num == "dxy")&&(abs(evtdxy[j]) <= cut_num)){a = 6;}
		  if((sel_num == "dz")&&(abs(evtdz[j]) <= cut_num)){a = 7;}
		  if((sel_num == "tightmva")&&(abs(evtid[j]) == 11)&&(evttight[j] == 1)){a = 9;}
		  if((sel_num == "loose")&&(evtloose[j]) == 1){a = 8;}

		  //Find the corresponding histogram for par2
		  TH1D* hist;
		  TH1D* hist_evt;

		  bool found = false;
		  for(int _i = 0; _i < npar2bins; ++_i){
		    if((par_2 > par2[_i])&&(par_2 <= par2[_i+1])){hist = histo_num[_i];found = true;}
		  }
		  if(!found){a = 0;}

		  switch(a){

		    case 0:

		      break;

		    case 1:
		      hist->Fill(par);
		      break;

		    case 2:
		      hist->Fill(par);
		      break;
		    case 3:
		      hist->Fill(par);
		      break;
		    case 4:
		      hist->Fill(par);
		      break;
		    case 5:
		      hist->Fill(par);
		      break;
		    case 6:
		      hist->Fill(par);
		      break;

		    case 7:
		      hist->Fill(par);
		      break;

		    case 8:
		      hist->Fill(par);
		      break;

		    case 9:
		      hist->Fill(par);
		      break;
		  }
		}
	      }
	    }
	  }
	}
	}
      }
    }

    mkdir(_path+_fname+"_PDF/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    ///////////////////
    //Draw histograms//
    ///////////////////

    //Canvas declaration
    for(int i = 0; i < npar2bins; ++i){

      ////////////////////
      //Build histograms//
      ////////////////////

      histo_num[i]->Sumw2();
      histo_den[i]->Sumw2();
      eff[i]->Divide(histo_num[i],histo_den[i],1,1,"B");

      //histo_num_O[i]->Sumw2();
      //histo_den_O[i]->Sumw2();
      //eff_O[i]->Divide(histo_num_O[i],histo_den_O[i],1,1,"B");

      //histo_num_G[i]->Sumw2();
      //histo_den_G[i]->Sumw2();
      //eff_G[i]->Divide(histo_num_G[i],histo_den_G[i],1,1,"B");

      //histo_counter_G_par1[i]->Sumw2();
      //histo_counter_O_par1[i]->Sumw2();
      //histo_counter_par1[i]->Divide(histo_counter_O_par1[i],histo_counter_G_par1[i],1,1,"B");

      //histo_counter[i]->Fill(0.5,goodcount);
      //histo_counter[i]->Fill(1.5,othercount);

      //String for name of the ouput files and histograms titles
      //

      TString _parybin;

      //Parameter string
      if(par_y == "Pt"){_parybin = Form("%0.f_Pt%0.f",par2[i],par2[i+1]);}
      else if(par_y == "eta"){_parybin = Form("%0.3f_eta%0.3f",par2[i],par2[i+1]);cout<<"it works !"<<endl;}
      else if(par_y == "phi"){_parybin = Form("%0.3f_phi%0.3f",par2[i],par2[i+1]);}

      TString _parytitle;

      //Title string
      if(par_y == "Pt"){_parytitle = Form("%0.f #leq P_{t} #leq %0.f",par2[i],par2[i+1]);}
      else if(par_y == "eta"){_parytitle = Form("%0.3f #leq #||{#eta}  #leq %0.3f",par2[i],par2[i+1]);cout<<"it works !"<<endl;}
      else if(par_y == "phi"){_parytitle = Form("%0.3f #leq #||{#phi}  #leq %0.3f",par2[i],par2[i+1]);}

      //Draw histograms
      TCanvas* c1 = new TCanvas("c1","c1");
      c1->cd();
      eff[i]->Draw();
      eff[i]->GetYaxis()->SetTitle("#epsilon");
      eff[i]->GetXaxis()->SetTitle(_par);
      eff[i]->GetYaxis()->SetRangeUser(0,1.1);
      eff[i]->SetMarkerStyle(20);
      eff[i]->SetMarkerSize(1);
      eff[i]->SetMarkerColor(4);
      eff[i]->SetLineColor(4);
      eff[i]->SetTitle(_sel_num+" for "+_sel_den+" "+pname+" "+_parytitle);

      TCanvas* c_par1 = new TCanvas("cpar1","cpar1");
      TString _partitle = _par + (TString)" distribution for "+pname+", "+_parytitle+", "+sel_num;
      c_par1->cd();
      histo_par1[i]->Draw();
      histo_par1[i]->SetTitle(_partitle);
      histo_par1[i]->GetXaxis()->SetTitle(_par);
      histo_par1[i]->SetLineWidth(2);
      histo_par1[i]->SetLineColor(4);
      histo_par1[i]->SetMarkerColor(4);

      /////////////////////
      //Saving the output//
      /////////////////////

      //Write pdf
      TString cname = "eff"+_filetag+_option+_pname+_par1range+"_"+_parybin+"_den_"+_sel_den+"_num_"+_sel_num;
      c1->SaveAs(_path+_fname+"_PDF/"+cname+".pdf");
      c_par1->SaveAs(_path+_fname+"_PDF/"+cname+"par_distr.pdf");

      //Write in output file
      file_out->cd();
      eff[i]->Write("eff"+_parybin);
      //histo_par1[i]->Write("histo_par1_"+_parybin);

    }

    file_out->Close();

    return 0;


  }
