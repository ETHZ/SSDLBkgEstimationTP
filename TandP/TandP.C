/*********************************************
 * Description - Performs a fit of the invriant mass mass + background and store the result using path.
 Retrieve then the efficiency using the T&P

 * Author - Gaël L. Perrin

 * Date - Jan 05 2015

 * *******************************************/

#include "TString.h"
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include <stdio.h>
#include <sys/stat.h>
#include <iostream>
#include <cmath>

#include "FitInvMassBkg.C"
//#include "DrawInvMassBkg.C"

//Helper functions
#include "../tools/InvMass.C"
#include "../tools/DeltaR.C"
#include "../tools/setTDRStyle.C"

double BinomError(double Nt, double eff) {

  double error=0;
  if(Nt==0) return 1;
  error = sqrt(eff*(1-eff)/Nt) ;
  return error;

}

int TandP(TString _filetag, int leptonId, double* par1, int npar1bins, double* par2, int npar2bins, TString sel_den = "tight", TString sel_num = "", double cut_num = 0.2, TString par_x = "Pt", TString par_y = "eta", TString option = "", TString _sig = "CBxBW");

int TandP(TString _filetag, int leptonId, TString sel_den = "tight", TString sel_num = "", double cut_num = 0.2, TString par_x = "Pt", TString par_y = "eta", TString option = "", TString _sig = "CBxBW");

int TandP(TString _filetag, int leptonId, double par_low, double par_upp , int npar1bins, TString sel_den = "tight", TString sel_num = "", double cut_num = 0.2, TString par_x = "Pt", TString par_y = "eta", TString option = "", TString _sig = "CBxBW"){

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
    return TandP(_filetag, leptonId, par1, npar1bins, par2_eta, npar2bins_eta, sel_den, sel_num, cut_num, par_x, par_y, option, _sig);
  }else if(par_y == "Pt"){
    return TandP(_filetag, leptonId, par1, npar1bins, par2_pt, npar2bins_pt, sel_den, sel_num, cut_num, par_x, par_y, option, _sig);
  }else{return 1;}

}

int TandP(TString _filetag, int leptonId, double* par1, int npar1bins , double* par2 ,int npar2bins, TString sel_den, TString sel_num, double cut_num, TString par_x, TString par_y, TString option, TString _sig ){

  cout<<"DEBUG tnp1"<<endl;

  gROOT->SetBatch(kTRUE); 

  //Path for input and output file.
  //TString _path = "/shome/gaperrin/plots_root/ZBkgInvM/";
  //TString _path = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/plots_root_t3/ZBkgInvM/";
  TString _path = "/shome/gaperrin/tnp_dir/InvMtnp/";
  cout<<"DEBUG tnp1"<<endl;

  //Some variables
  double Dpt = (par1[npar1bins]-par1[0])/npar1bins;
  int nbins = 200;

  /////////////////////
  //Name of the input//
  /////////////////////

  //Declare Strings

  TString pname;
  TString _pname;
  TString _par;
  TString _sel_num;
  TString _sel_den;
  TString _option;
  TString _optionInvM;//Invariant mass option

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
  option.Append(" ");
  option.Prepend(" ");
  if(option.Contains(" matching ")){_option += "_Matched";_optionInvM += "_Matched";}
  if(option.Contains(" short ")){_optionInvM += "_short";}
  if(option.Contains(" nentries ")){_option += "_nentries";}
  if(option.Contains(" loose ")){_option += "_loose";_optionInvM += "_loose";}
  if(option.Contains(" oldtree ")){_option += "_oldtree";_optionInvM += "_oldtree";}
  if(option.Contains(" dyonly ")){_option += "_dyonly";_optionInvM += "_dyonly";cout<<"yes2 !"<<endl;}
  if(option.Contains("bkg_Cheb")){_option += "_bkg_cheb";}
  if(option.Contains("bkg_Exp")){_option += "_bkg_exp";}
  if(option.Contains("bkg_Novo")){_option += "_bkg_novo";}
  _option += "_";
  _optionInvM += "_";
  //parameter range string
  TString _par1range;
  _par1range = Form("%0.3f_"+par_x+"%0.3f",par1[0],par1[npar1bins]);
  TString _par2range;
  _par2range = Form("%0.3f_"+par_y+"%0.3f",par2[0],par2[npar2bins]);

  ///////////////////////////////////////////
  //Write the name of the input/output file//
  ///////////////////////////////////////////

  TString _fname = "InvM"+_filetag+_option+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num;
  TString _fnameInvM = "InvM"+_filetag+_optionInvM+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num;
  cout<<"FNAME IS "<<_fname<<endl;
  cout<<"option is "<<option<<endl;

  //Create folder to store background fitting
  mkdir(_path+_fname+"_FIT_PDF/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  cout<<"Will check if the file "<<_path+_fnameInvM+".root"<<" exists"<<endl;

  //Check if the file exist
  TFile  f_test(_path+_fnameInvM+".root");
  if(f_test.IsZombie()){

    cout<<"File does not exists !"<<endl;
    return 1;
    //cout<<"Creating file, please wait..."<<endl;
    //DrawInvMassBkg(_filetag, leptonId, par1, npar1bins,par2, npar2bins, sel_den,sel_num,cut_num,par_x, par_y, option);
    //cout<<"Done !"<<endl;

  }else{cout<<"The file exists !"<<endl;}

  TFile* file_in 	= new TFile(_path+_fnameInvM+".root","read");//rettrive the invM plot
  TFile* file_out 	= new TFile(_path+_fname+"_FIT"+".root","recreate");
  TFile* file_out2 	= new TFile(_path+_fname+"_FIT_eff"+".root","recreate");
  cout<<"FILE OUT IS "<<file_out<<endl;
  cout<<"FILE2 OUT IS "<<file_out2<<endl;

  /////////////////////////////////////////
  //Recover the invariant mass histograms//
  /////////////////////////////////////////

  //Declaration of the histograms
  //
  
cout<<"Debug1"<<endl;

  //Histo to recover
  TH1D ***histo_M	= new TH1D**[npar2bins];
  TH1D ***histo_M_fail	= new TH1D**[npar2bins];
  //TH1D ***histo_DY 	= new TH1D**[npar2bins];
  //TH1D ***histo_BKG 	= new TH1D**[npar2bins];
  //TH1D ***histo_DY_fail 	= new TH1D**[npar2bins];
  //TH1D ***histo_BKG_fail 	= new TH1D**[npar2bins];

  //Output histo
  TH1D **eff 		= new TH1D*[npar2bins];
  //Declaration of graph for the efficiency
  TGraphErrors** _eff 	= new TGraphErrors*[npar2bins];

  //Recover the histo
  for(int i = 0; i < npar2bins; ++i){ 

    histo_M[i]      	= new TH1D*[npar1bins];
    histo_M_fail[i]     = new TH1D*[npar1bins];
    //histo_DY[i]      	= new TH1D*[npar1bins];
    //histo_BKG[i]     	= new TH1D*[npar1bins];
    //histo_DY_fail[i] 	= new TH1D*[npar1bins];
    //histo_BKG_fail[i]	= new TH1D*[npar1bins];

    eff[i]			= new TH1D("eff_BKG_fail","eff",npar1bins,par1);

    for(int j = 0; j < npar1bins; ++j){ 

      histo_M[i][j] 		       	=  new TH1D("histo_M","M",nbins,0,250);
      histo_M_fail[i][j]     		=  new TH1D("histo_M_fail","M",nbins,0,250);
      //histo_DY[i][j] 		       	=  new TH1D("histo_M_DY","M",nbins,0,250);
      //histo_BKG[i][j]     		=  new TH1D("histo_M_BKG","M",nbins,0,250);
      //histo_DY_fail[i][j] 		=  new TH1D("histo_M_DY_fail","M",nbins,0,250);
      //histo_BKG_fail[i][j]		=  new TH1D("histo_M_BKG_fail","M",nbins,0,250);

    }

  }
cout<<"Debug2"<<endl;

  ////////////////////////////
  //Calculate the efficiency//
  ////////////////////////////

  //Efficiency
  //In order to compute the efficiency, one needs to fill the following variables
  double** Eff = new double*[100];
  double** error_eff = new double*[100];
  double** PT = new double*[100];
  double** error_par1 = new double*[100];

  for(int j = 0; j < npar2bins; ++j){ 

    Eff[j] = new double[npar1bins];
    error_eff[j] = new double[npar1bins];
    PT[j] = new double[npar1bins];
    error_par1[j] = new double[npar1bins];  	
  }

  //Folder to save the output 
  //
  //Starting the loop that read the hisograms and do the fit
  for(int j = 0; j < npar2bins; ++j){ 

    //name of the bins
    TString _parxbin;
    TString _parybin;

    for(int i = 0; i<npar1bins; ++i){

      //Recover the Histograms
      //

      //Parameter string
      if(par_x == "Pt"){_parxbin = Form("%0.f_Pt%0.f",par1[i],par1[i+1]);}
      else if(par_x == "eta"){_parxbin = Form("%0.3f_eta%0.3f",par1[i],par1[i+1]);}
      else if(par_x == "phi"){_parxbin = Form("%0.3f_phi%0.3f",par1[i],par1[i+1]);}
      if(par_y == "Pt"){_parybin = Form("%0.f_Pt%0.f",par2[j],par2[j+1]);}
      else if(par_y == "eta"){_parybin = Form("%0.3f_eta%0.3f",par2[j],par2[j+1]);}
      else if(par_y == "phi"){_parybin = Form("%0.3f_phi%0.3f",par2[j],par2[j+1]);}

      TString _M;
      TString _M_fail;
      //TString _DY;
      //TString _BKG;
      //TString _DY_fail;
      //TString _BKG_fail;

      _M		= "histo_M"+_parxbin+"_"+_parybin;
      _M_fail 		= "histo_M_fail"+_parxbin+"_"+_parybin; 
      //_DY 		= "histo_M_DYJets_bkg"+_parxbin+"_"+_parybin;
      //_BKG 		= "histo_M_bkg"+_parxbin+"_"+_parybin; 
      //_DY_fail 	= "histo_M_DYJets_bkg_fail"+_parxbin+"_"+_parybin; 
      //_BKG_fail 	= "histo_M_bkg_fail"+_parxbin+"_"+_parybin; 


      cout<<"list of keys"<<endl;
      file_in->GetListOfKeys()->ls();

      cout<<"Debug2.5"<<endl;
      histo_M[j][i] 		 	= (TH1D*)file_in->Get(_M);
      cout<<"want to retrieve "<< _M<<endl;
      histo_M[j][i]->GetEntries();
      cout<<"Debug2.75"<<endl;
      histo_M_fail[j][i] 		= (TH1D*)file_in->Get(_M_fail);
      cout<<"want to retrieve "<< _M_fail<<endl;

      //cout<<"Debug2.5"<<endl;
      //histo_DY[j][i] 		 	= (TH1D*)file_in->Get(_DY);
      //cout<<"want to retrieve "<< _DY<<endl;
      //cout<<"Debug2.75"<<endl;
      //histo_BKG[j][i] 		        = (TH1D*)file_in->Get(_BKG);
      //cout<<"want to retrieve "<< _BKG<<endl;
      //histo_DY_fail[j][i] 		= (TH1D*)file_in->Get(_DY_fail);
      //cout<<"want to retrieve "<< _DY_fail<<endl;
      //histo_BKG_fail[j][i] 		= (TH1D*)file_in->Get(_BKG_fail);
      //cout<<"want to retrieve "<< _BKG_fail<<endl;
      //cout<<"Debug2.8"<<endl;

      //histo_DY[j][i]->SetName(_DY);
      //cout<<"Debug2.9"<<endl;
      //histo_BKG[j][i]->SetName(_BKG);
      //histo_DY_fail[j][i]->SetName(_DY_fail);
      //histo_BKG_fail[j][i]->SetName(_BKG_fail);

      //Fit the recovered histograms
      //
cout<<"Debug3"<<endl;

      //TCanvas* c1 = new TCanvas("c1","c1");
      //c1->Divide(1,2);
      //c1->cd(1);
      //file_out->cd();
      //double pass = fitinvmassbkg(histo_dy[j][i],histo_bkg[j][i],_sig,"exp",option);
      //c1->cd(2);
      //double fail = fitinvmassbkg(histo_dy_fail[j][i],histo_bkg_fail[j][i],_sig,"exp",option);

      TCanvas* c1 = new TCanvas("c1","c1");
      c1->Divide(1,2);
      c1->cd(1);
      file_out->cd();
      double pass = FitInvMassBkg(histo_M[j][i],_sig,"exp",option);
      c1->cd(2);
      double fail = FitInvMassBkg(histo_M_fail[j][i],_sig,"exp",option);
      //Compute and fill the efficiency
      

      if(pass !=0){Eff[j][i] = pass/(pass+fail);}
      else{Eff[j][i] = 0;}
      if(pass != 0){error_eff[j][i] = BinomError(pass+fail,Eff[j][i]);}
      else{error_eff[j][i] = 0;}

      PT[j][i] = (par1[i] + par1[i+1])/2.;
      error_par1[j][i] = (par1[i] - par1[i+1])/2.;

      eff[j]->Fill(PT[j][i],Eff[j][i]);
      eff[j]->SetBinError(i+1,error_eff[j][i]);


      TString _cname = _path+_fname+"_FIT_PDF/InvM"+_filetag+"_"+_parxbin+"_"+_parybin;
      _cname += "_"+sel_num+"_"+_pname;
      TString _c1name = _cname+ ".pdf";
      c1->SaveAs(_c1name);

    }


      /////////////////////
      //Saving the output//
      /////////////////////

    _eff[j] = new TGraphErrors(npar1bins,PT[j],Eff[j],error_par1[j],error_eff[j]);

    _eff[j]->Draw("A");
    _eff[j]->SetMarkerStyle(4);
    _eff[j]->SetMarkerSize(0.4);
    _eff[j]->SetMarkerColor(4);
    _eff[j]->GetXaxis()->SetTitle("P_{t}");
    _eff[j]->GetXaxis()->SetRangeUser(0,250);
    _eff[j]->GetYaxis()->SetRangeUser(0,1.1);
    _eff[j]->GetYaxis()->SetTitle(" #epsilon ");
    _eff[j]->GetXaxis()->SetTitle("P_{t}");

    TString _g1name = _path+_fname+"_FIT_PDF/Eff";
    _g1name += "_"+sel_num+"_"+_pname+"_"+_par1range+"_"+_parybin+".pdf";

    TCanvas* b1 = new TCanvas("b1","b1");

    b1->cd();
    eff[j]->Draw();
    b1->SaveAs(_g1name);

    file_out2->cd();
    _eff[j]->Write("Eff_"+sel_num+"_"+_pname+"_"+_par1range+"_"+_parybin);
    eff[j]->Write("Eff_histo_"+sel_num+"_"+_pname+"_"+_par1range+"_"+_parybin);

  }

  file_in->Close();
  file_out->Close();
  file_out2->Close();

  return 0;
}

//Bin already filled in this function
int TandP(TString _filetag, int leptonId, TString sel_den, TString sel_num, double cut_num, TString par_x, TString par_y, TString option, TString _sig){

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
      return TandP(_filetag, leptonId, par1, npar1bins, par2, npar2bins, sel_den, sel_num, cut_num, par_x, par_y, option, _sig);

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

      //Parameter 2
      const int npar2bins = 1;
      double par2[npar2bins+1];
      par2[0] = 7;
      par2[1] = 250;

	return TandP(_filetag, leptonId, par1, npar1bins, par2, npar2bins, sel_den, sel_num, cut_num, par_x, par_y, option, _sig);
    }

  }else if(leptonId == 13){

    if((par_x == "Pt")&&(par_y == "eta")){
      const int npar1bins = 15;
      //Parameter 1
      double* par1 = new double[npar1bins+1];

      par1[0] = 20;
      par1[1] = 25;
      par1[2] = 30;
      par1[3] = 35;
      par1[4] = 40;
      par1[5] = 45;
      par1[6] = 50;
      par1[7] = 60;
      par1[8] = 70;
      par1[9] = 80;
      par1[10] = 90;
      par1[11] = 100;
      par1[12] = 120;
      par1[13] = 140;
      par1[14] = 200;
      par1[15] = 250;

      //Parameter 2
      const int npar2bins = 3;
      double par2[npar2bins+1];
      par2[0] = 0;
      par2[1] = 0.9;
      par2[2] = 1.2;
      par2[3] = 2.5;

      return TandP(_filetag, leptonId, par1, npar1bins, par2, npar2bins, sel_den, sel_num, cut_num, par_x, par_y, option, _sig);

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
      const int npar2bins = 2;
      double par2[npar2bins+1];
      par2[0] = 20;
      par2[1] = 60;
      par2[2] = 250;

	return TandP(_filetag, leptonId, par1, npar1bins, par2, npar2bins, sel_den, sel_num, cut_num, par_x, par_y, option, _sig);
    }
  }
}
