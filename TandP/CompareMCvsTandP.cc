/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
 * File Name : myfile.c
 * Purpose :
 * Creation Date : 20-12-2008
 * Last Modified : Sat 20 Dec 2008 09:37:30 AM PST
 * Created By :
 _._._._._._._._._._._._._._._._._._._._._.*/

using namespace std;

#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include <string>
#include <functional>
#include <iostream>
#include <iomanip>
#include <TROOT.h>
#include "TStyle.h"
#include <TH1F.h>
#include <TFile.h>
#include <TLegend.h>
#include <TCanvas.h>
#include "MC_Ratio.C"
#include "TandP.C"
#include "TPaveText.h"

//helper functions
#include "../tools/InvMass.C"
#include "../tools/DeltaR.C"
#include "../tools/setTDRStyle.C"
#include "../tools/getmaxallhist.C"

int CompareMCvsTandP(TString _filetag, int leptonId, double* par1, int npar1bins, double* par2, int npar2bins, TString sel_den = "tight", TString sel_num = "", double cut_num = 0.2, TString par_x = "Pt", TString par_y = "eta", TString option_mc = "",TString option_tp = "", TString _sig = "CBxBW");

int CompareMCvsTandP(TString _filetag, int leptonId, TString sel_den = "tight", TString sel_num = "", double cut_num = 0.2, TString par_x = "Pt", TString par_y = "eta", TString option_mc = "", TString option_tp = "", TString _sig = "CBxBW");

int CompareMCvsTandP(TString _filetag, int leptonId, double par_low, double par_upp , int npar1bins, TString sel_den = "tight", TString sel_num = "", double cut_num = 0.2, TString par_x = "Pt", TString par_y = "eta", TString option_mc = "", TString option_tp = "", TString _sig = "CBxBW"){

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
    return CompareMCvsTandP(_filetag, leptonId, par1, npar1bins, par2_eta, npar2bins_eta, sel_den, sel_num, cut_num, par_x, par_y, option_mc,option_tp, _sig);
  }else if(par_y == "Pt"){
    return CompareMCvsTandP(_filetag, leptonId, par1, npar1bins, par2_pt, npar2bins_pt, sel_den, sel_num, cut_num, par_x, par_y, option_mc,option_tp, _sig);
  }else{return 1;}

}

int CompareMCvsTandP( TString _filetag, int leptonId, double* par1, int npar1bins, double* par2, int npar2bins, TString sel_den, TString sel_num, double cut_num, TString par_x, TString par_y, TString option_mc, TString option_tp, TString _sig){

  cout<<"DEBUG1"<<endl;

  setTDRStyle();

  //output of the code
  TString _output ="Compare"+_filetag;
  //TString _outpath ="/shome/gaperrin/tnp_fit/";
  TString _outpath ="/shome/gaperrin/tnp_dir/tnp_compare/";

  //////////////////////////
  //Retrieve MC ratio file//
  //////////////////////////

  TString pname;
  TString _pname;
  TString _par;
  TString _sel_num;
  TString _sel_den;
  TString _option_mc;
  TString _option_tp;
  TString _optionInvM;//Invariant mass option

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
  //option_mc string
  option_mc.Append(" ");
  option_mc.Prepend(" ");
  if(option_mc.Contains(" ll ")){_option_mc += "_ll";}
  if(option_mc.Contains(" unmatched ")){_option_mc += "_unmatched";}
  if(option_mc.Contains(" alleta ")){_option_mc += "_alleta";}
  if(option_mc.Contains(" short ")){_option_mc += "_short";}
  if(option_mc.Contains(" loose ")){_option_mc += "_loose";}
  if(option_mc.Contains(" oldtree ")){_option_mc += "_oldtree";}
  _option_mc += "_";
  //parameter range string
  TString _par1range;
  _par1range = Form("%0.3f_"+par_x+"%0.3f",par1[0],par1[npar1bins]);
  TString _par2range;
  _par2range = Form("%0.3f_"+par_y+"%0.3f",par2[0],par2[npar2bins]);

  cout<<"DEBUG2"<<endl;
  //MC input file
  //TString _path_MC = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/plots_root_t3/MC_eff/";
  TString _path_MC = "/shome/gaperrin/tnp_dir/tnp_mc/";
  TString _fname_MC = "eff"+_filetag+_option_mc+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num;

  cout<<"DEBUG3"<<endl;
  //Check if file exits
  TFile* file_test_MC = new TFile(_path_MC+_fname_MC+".root");
  if(file_test_MC->IsZombie()){
    cout<<"File does not exists"<<endl;
    cout<<"Creating file, please wait..."<<endl;
    //MC_Ratio(_filetag, leptonId,par1,npar1bins,par2, npar2bins, sel_den,sel_num,cut_num,par_x, par_y,option_mc);
    MC_Ratio(_filetag, leptonId, sel_den,sel_num,cut_num,par_x, par_y,option_mc);
    cout<<"Done !"<<endl;
  }
  cout<<"DEBUG4"<<endl;

  TFile* file_in_MC = new TFile(_path_MC+_fname_MC+".root", "read");
  delete file_test_MC;
  cout<<"list of keys of the MC file"<<endl;
  //file_in_MC->GetListOfKeys()->ls();
  cout<<"DEBUG4a"<<endl;


  ////////////////
  //Retrieve T&P//
  ////////////////

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
  //option_mc string
  option_tp.Append(" ");
  option_tp.Prepend(" ");
  if(option_tp.Contains(" matching ")){_option_tp += "_Matched";_optionInvM += "_Matched";}
  if(option_tp.Contains(" nentries ")){_option_tp += "_nentries";}
  if(option_tp.Contains(" short ")){_optionInvM += "_short";cout<<"yes !"<<endl;}
  if(option_tp.Contains(" loose ")){_option_tp += "_loose";_optionInvM += "_loose";}
  if(option_tp.Contains(" oldtree ")){_option_tp += "_oldtree";_optionInvM += "_oldtree";}
  if(option_tp.Contains(" dyonly ")){_option_tp += "_dyonly";_optionInvM += "_dyonly";cout<<"yes2 !"<<endl;}
  if(option_tp.Contains(" bkg_Cheb ")){_option_tp += "_bkg_cheb";}
  if(option_tp.Contains(" bkg_Exp ")){_option_tp += "_bkg_exp";}
  if(option_tp.Contains(" bkg_Novo ")){_option_tp += "_bkg_novo";}
  _option_tp += "_";
  _optionInvM += "_";
  //parameter range string
  _par1range = Form("%0.3f_"+par_x+"%0.3f",par1[0],par1[npar1bins]);
  _par2range = Form("%0.3f_"+par_y+"%0.3f",par2[0],par2[npar2bins]);

  cout<<"DEBUG 4b"<<endl;
  //TP input file
  //TString _path_TP = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/plots_root_t3/ZBkgInvM/";
  TString _path_TP = "/shome/gaperrin/tnp_dir/InvMtnp/";
  TString _fname_TP = "InvM"+_filetag+_option_tp+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num;
  cout<<"The file name TP is "<< _fname_TP <<endl;
  TFile* file_test_TP = new TFile(_path_TP+_fname_TP+"_FIT_eff"+".root","read");
  //Location of mass distribution
  cout<<"will check if file "<<_path_TP+_fname_TP+"_FIT_eff"+".root"<<" exists"<<endl;


  if(file_test_TP->IsZombie()){

    cout<<"File does not exists"<<endl;
    cout<<"Creating file, please wait..."<<endl;
    //TandP(_filetag, leptonId,par1, npar1bins,par2, npar2bins, sel_den,sel_num,cut_num,par_x, par_y,option_tp);
    cout<<"THE OPTION IS "<<option_tp<<endl;
    TandP(_filetag, leptonId, sel_den,sel_num,cut_num,par_x, par_y,option_tp);
    cout<<"Done !"<<endl;

  }

  file_test_TP->Close();
  TFile* file_in_TP = new TFile(_path_TP+_fname_TP+"_FIT_eff"+".root","read");
  cout<<"list of keys of the TP file"<<endl;
  //file_in_TP->GetListOfKeys()->ls();

  //TString _path_invM = "/shome/gaperrin/plots_root/ZBkgInvM/";
  //TString _path_invM = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/plots_root_t3/ZBkgInvM/";
  TString _path_invM = "/shome/gaperrin/tnp_dir/InvMtnp/";
  TString _fname_invM = "InvM"+_filetag+_optionInvM+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num;
  TFile* file_invM = new TFile(_path_invM+_fname_invM+".root","read");
  cout<<"The list of keys of the invM file"<<endl;
  //file_invM->GetListOfKeys()->ls();

  //////////
  //output// 
  //////////

  TFile* file_out = new TFile(_outpath+_output+_option_mc+_option_tp+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num+".root","recreate");


  /////////////////
  //Get the plots//
  /////////////////

  //Efficiency
  TH1D** eff_MC = new TH1D*[npar2bins];
  TH1D** eff_TP = new TH1D*[npar2bins];
  TH1D** ratio  =new TH1D*[npar2bins];

  //Par1 distribution
  TH1D** par1_MC = new TH1D*[npar2bins];
  TH1D** par1_TP = new TH1D*[npar2bins];
  TH1D** par1_dy_TP = new TH1D*[npar2bins];

  //Par1 distribution per bin
  TH1D*** par1bin_MC = new TH1D**[npar2bins];
  TH1D*** par1bin_TP = new TH1D**[npar2bins];
  //TH1D*** par1bin_dy_TP = new TH1D**[npar2bins];

  for(int i = 0; i < npar2bins; ++i){

    TString _parybin;

    //Parameter string
    if(par_y == "Pt"){_parybin = Form("%0.f_Pt%0.f",par2[i],par2[i+1]);}
    else if(par_y == "eta"){_parybin = Form("%0.3f_eta%0.3f",par2[i],par2[i+1]);cout<<"it works !"<<endl;}
    else if(par_y == "phi"){_parybin = Form("%0.3f_phi%0.3f",par2[i],par2[i+1]);}

    //retrieve the histo
    eff_TP[i] = (TH1D*)file_in_TP->Get("Eff_histo_"+sel_num+"_"+_pname+"_"+_par1range+"_"+_parybin);
    cout<<"eff_TP want to retrieve "<<"Eff_histo_"+sel_num+"_"+_pname+"_"+_par1range+"_"+_parybin<<endl;
    eff_MC[i] = (TH1D*)file_in_MC->Get("eff"+_parybin);
    cout<<"eff_MC want to retrieve "<<"eff"+_parybin<<endl;
    ratio[i]  = (TH1D*) eff_TP[i]->Clone("ratio");
    ratio[i]->Divide(eff_MC[i],eff_TP[i],1,1,"B");

    //Retrieve par1 distribution histograms
    cout<<"par1_MC want to retrieve "<<"histo_par1_"+_parybin<<endl;
    par1_MC[i] = (TH1D*) file_in_MC->Get("histo_par1_"+_parybin);
    cout<<"par1_TP want to retrieve "<<"histo_par1_"+_parybin<<endl;
    //cout<<"par1_dy_TP want to retrieve "<<"histo_par1_dyonly"+_parybin<<endl;
    par1_TP[i] = (TH1D*) file_invM->Get("histo_par1_"+_parybin);
    cout<<"debug"<<endl;
    //par1_dy_TP[i] = (TH1D*) file_invM->Get("histo_par1_dyonly"+_parybin);
    cout<<"debug1"<<endl;

    //normalise par1 distributions
    par1_MC[i]->Scale(1./par1_MC[i]->Integral());
    par1_TP[i]->Scale(1./par1_TP[i]->Integral());
    //par1_dy_TP[i]->Scale(1./par1_dy_TP[i]->Integral());
    cout<<"debug3"<<endl;


    TCanvas* c1 = new TCanvas("c1","c1");
    c1->Divide(1,2);
    c1->cd(1);
    c1->GetPad(1)->SetPad(0,0.21,1,1);
    eff_MC[i]->Draw();
    eff_MC[i]->SetLineColor(4);
    eff_MC[i]->SetLineWidth(3);
    eff_MC[i]->SetMarkerColor(4);
    eff_MC[i]->SetMarkerStyle(1);
    eff_MC[i]->GetYaxis()->SetRangeUser(0,1.1);
    eff_MC[i]->GetYaxis()->SetTitle(" #epsilon ");
    eff_MC[i]->SetTitle(_sel_num+" for "+_sel_den+" "+pname+ _parybin);

    eff_TP[i]->Draw("same");
    eff_TP[i]->SetLineWidth(3);
    eff_TP[i]->SetLineColor(2);
    eff_TP[i]->SetMarkerStyle(1);
    eff_TP[i]->SetMarkerColor(2);
    TLegend* leg_B = new TLegend(0.3, 0.3, 0.6 , 0.5);
    leg_B->AddEntry(eff_MC[i], "MC ratio");
    leg_B->AddEntry(eff_TP[i], "T&P","LP");
    leg_B->SetBorderSize(0.);
    leg_B->SetTextFont(43);
    leg_B->SetTextSize(25);
    leg_B->Draw();

    c1->cd(2);
    c1->GetPad(2)->SetPad(0,0.02,1,0.22);
    c1->GetPad(2)->SetGrid(2);
    ratio[i]->SetLineColor(1);
    ratio[i]->SetTitle("");
    ratio[i]->SetLineWidth(1);
    ratio[i]->SetMarkerStyle(20);
    ratio[i]->GetYaxis()->SetRangeUser(0.8,1.2);
    ratio[i]->GetYaxis()->SetRangeUser(0.95,1.05);
    ratio[i]->GetYaxis()->SetLabelSize(0.2);
    ratio[i]->GetYaxis()->SetNdivisions(503);
    ratio[i]->GetYaxis()->SetTitle("");
    ratio[i]->GetXaxis()->SetLabelSize(0.2);
    ratio[i]->GetXaxis()->SetRangeUser(eff_MC[i]->GetXaxis()->GetXmin(),eff_MC[i]->GetXaxis()->GetXmin());
    ratio[i]->GetXaxis()->SetTitle("");
    ratio[i]->Draw();
    cout<<"debug4"<<endl;

    file_out->cd();
    c1->Write("eff"+_parybin);
    c1->SaveAs(_outpath+"PDF/"+_output+"_"+_option_mc+"_"+_option_tp+_pname+"_den_"+_sel_den+"_num_"+_sel_num+"_"+_parybin+".pdf");
    //c2->Write("par1_distr"+_parybin);

    //binned par1

    par1bin_MC[i] 			= new TH1D*[npar1bins];
    par1bin_TP[i] 			= new TH1D*[npar1bins];
    //par1bin_dy_TP[i] 			= new TH1D*[npar1bins];

    TCanvas** c3 = new TCanvas*[npar1bins];
    
    for(int _i = 0; _i < npar1bins; ++_i){

      TString _parxbin;

      if(par_x == "Pt"){_parxbin = Form("%0.f_Pt%0.f",par1[_i],par1[_i+1]);}
      else if(par_x == "eta"){_parxbin = Form("%0.3f_eta%0.3f",par1[_i],par1[_i+1]);}
      else if(par_x == "phi"){_parxbin = Form("%0.3f_phi%0.3f",par1[_i],par1[_i+1]);}


      cout<<"_i is "<<_i<<endl;
      //Retrieve par1 distribution per bin
      par1bin_MC[i][_i] = (TH1D*) file_in_MC->Get("histo_par1_"+_parxbin+"_"+_parybin);
      cout<<"debug1"<<endl;
      par1bin_TP[i][_i] = (TH1D*) file_invM->Get("h_par1_"+_parxbin+"_"+_parybin);
      cout<<"debug2"<<endl;
      //par1bin_dy_TP[i][_i] = (TH1D*) file_invM->Get("h_par1_dyonly"+_parxbin+"_"+_parybin);
      cout<<"debug3"<<endl;

      //normalise par1 distributions
      par1bin_MC[i][_i]->Scale(1./par1bin_MC[i][_i]->Integral());
      cout<<"debug4"<<endl;
      par1bin_TP[i][_i]->Scale(1./par1bin_TP[i][_i]->Integral());
      cout<<"debug5"<<endl;
      //par1bin_dy_TP[i][_i]->Scale(1./par1bin_dy_TP[i][_i]->Integral());
      cout<<"debug6"<<endl;

    //TCanvas* c3 = new TCanvas("c3","c3");
    c3[_i] = new TCanvas(Form("c3%i",_i),Form("c3%i",_i));
    par1bin_MC[i][_i]->Draw(); 
    par1bin_MC[i][_i]->SetLineColor(4);
    par1bin_MC[i][_i]->SetLineWidth(3);
    par1bin_MC[i][_i]->SetMarkerColor(4);
    par1bin_MC[i][_i]->SetMarkerStyle(1);
    par1bin_MC[i][_i]->GetYaxis()->SetTitle("par1");
    par1bin_MC[i][_i]->SetTitle(par_x+" distribution for "+_sel_den+" "+pname+ _parybin);
    par1bin_MC[i][_i]->GetYaxis()->SetRangeUser(0,getmaxallhist(par1bin_MC[i][_i],par1bin_TP[i][_i]));
    par1bin_TP[i][_i]->Draw("same"); 
    par1bin_TP[i][_i]->SetLineColor(2);
    par1bin_TP[i][_i]->SetLineWidth(3);
    par1bin_TP[i][_i]->SetMarkerColor(2);
    par1bin_TP[i][_i]->SetMarkerStyle(1);

    TLegend* leg3 = new TLegend(0.3, 0.3, 0.6 , 0.5);
    leg3->AddEntry(par1bin_MC[i][_i], "MC Ratio");
    leg3->AddEntry(par1bin_TP[i][_i], "T&P","LP");
    //leg3->AddEntry(par1bin_dy_TP[i][_i], "T&P DY only","LP");
    leg3->SetBorderSize(0.);
    leg3->SetTextFont(43);
    leg3->SetTextSize(25);
    leg3->Draw();

    file_out->cd();
    c3[_i]->Write("par1_distr_"+_parxbin+"_"+_parybin);

    }

  }
    file_out->Close();


  return 0;

}

int main(int argc, char** argv){

  if(argc-1 == 10){

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

  return CompareMCvsTandP(argv[1], atof(argv[2]), argv[3],argv[4], atof(argv[5]),argv[6], argv[7],argv[8], argv[9],argv[10]);

  }else{

  cout<<"The number of parameters are "<<argc-1<<endl;
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
  cout<<"The 12th parameter is "<<argv[12]<<endl;
  cout<<"The 13th parameter is "<<argv[13]<<endl;

  return CompareMCvsTandP(argv[1], atof(argv[2]),atof(argv[3]), atof(argv[4]),atof(argv[5]), argv[6],argv[7], atof(argv[8]),argv[9], argv[10],argv[11], argv[12],argv[13]);

  }

}

//Bin already filled in this function
int CompareMCvsTandP(TString _filetag, int leptonId, TString sel_den , TString sel_num, double cut_num , TString par_x , TString par_y , TString option_mc , TString option_tp , TString _sig ){

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
      return CompareMCvsTandP(_filetag, leptonId, par1, npar1bins, par2, npar2bins, sel_den, sel_num, cut_num, par_x, par_y, option_mc, option_tp, _sig);

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

	return CompareMCvsTandP(_filetag, leptonId, par1, npar1bins, par2, npar2bins, sel_den, sel_num, cut_num, par_x, par_y, option_mc, option_tp, _sig);
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

      return CompareMCvsTandP(_filetag, leptonId, par1, npar1bins, par2, npar2bins, sel_den, sel_num, cut_num, par_x, par_y, option_mc, option_tp, _sig);

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

	return CompareMCvsTandP(_filetag, leptonId, par1, npar1bins, par2, npar2bins, sel_den, sel_num, cut_num, par_x, par_y, option_mc, option_tp, _sig);
    }
  }
}
