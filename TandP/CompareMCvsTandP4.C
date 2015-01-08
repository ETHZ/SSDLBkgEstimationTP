/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

 * File Name : myfile.c

 * Purpose :

 * Creation Date : 20-12-2008

 * Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

 * Created By :

 _._._._._._._._._._._._._._._._._._._._._.*/

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
//#include "../setTDRStyle.C"
#include "MC_Ratio.C"
#include "TandP4.C"
#include "TPaveText.h"

//#include "TandP_sidefunction.h"


int CompareMCvsTandP(int leptonId, double* par1, int npar1bins, double* par2, int npar2bins, TString sel_den = "tight", TString sel_num = "", double cut_num = 0.2, TString par_x = "Pt", TString par_y = "eta", TString option_mc = "",TString option_tp = "", TString _sig = "CBxBW");

int CompareMCvsTandP(int leptonId, double par_low, double par_upp , int npar1bins, TString sel_den = "tight", TString sel_num = "", double cut_num = 0.2, TString par_x = "Pt", TString par_y = "eta", TString option_mc = "", TString option_tp = "", TString _sig = "CBxBW"){

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
    return CompareMCvsTandP(leptonId, par1, npar1bins, par2_eta, npar2bins_eta, sel_den, sel_num, cut_num, par_x, par_y, option_mc,option_tp, _sig);
  }else if(par_y == "pt"){
    return CompareMCvsTandP(leptonId, par1, npar1bins, par2_pt, npar2bins_pt, sel_den, sel_num, cut_num, par_x, par_y, option_mc,option_tp, _sig);
  }else{return 1;}

}

int CompareMCvsTandP(int leptonId, double* par1, int npar1bins, double* par2, int npar2bins, TString sel_den, TString sel_num, double cut_num, TString par_x, TString par_y, TString option_mc, TString option_tp, TString _sig){

  setTDRStyle();

  //output of the code
  TString _output ="Compare4";
  TString _outpath ="/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MCvsTandP/";

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
  //option_mc string
  option_mc.Append(" ");
  option_mc.Prepend(" ");
  if(option_mc.Contains(" ll ")){_option_mc += "_ll";}
  if(option_mc.Contains(" unmatched ")){_option_mc += "_unmatched";}
  if(option_mc.Contains(" alleta ")){_option_mc += "_alleta";}
  _option_mc += "_";
  //parameter range string
  TString _par1range;
  _par1range = Form("%0.3f_"+par_x+"%0.3f",par1[0],par1[npar1bins]);
  TString _par2range;
  _par2range = Form("%0.3f_"+par_y+"%0.3f",par2[0],par2[npar2bins]);

  //MC input file
  //
  TString _path_MC = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_ratio/";
  TString _fname_MC = "eff4test"+_option_mc+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num;

  cout<<"debug 1"<<endl;

  //Check if file exits
  TFile* file_test_MC = new TFile(_path_MC+_fname_MC+".root");
  if(file_test_MC->IsZombie()){
    cout<<"File does not exists"<<endl;
    cout<<"Creating file, please wait..."<<endl;
    MC_Ratio(leptonId,par1,npar1bins,par2, npar2bins, sel_den,sel_num,cut_num,par_x, par_y,option_mc);
    cout<<"Done !"<<endl;
  }

  TFile* file_in_MC = new TFile(_path_MC+_fname_MC+".root", "read");
  delete file_test_MC;
  cout<<"list of keys of the MC file"<<endl;
  file_in_MC->GetListOfKeys()->ls();

  cout<<"debug 2"<<endl;

  //////////
  //output// 
  //////////

  TFile* file_out = new TFile(_outpath+_output+_option_mc+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num+".root","recreate");

  cout<<"debug 3"<<endl;

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
  //option_mc string
  option_tp.Append(" ");
  option_tp.Prepend(" ");
  if(option_tp.Contains(" matching ")){_option_mc += "_Matched";}
  _option_tp += "_";
  //parameter range string
  _par1range = Form("%0.3f_"+par_x+"%0.3f",par1[0],par1[npar1bins]);
  _par2range = Form("%0.3f_"+par_y+"%0.3f",par2[0],par2[npar2bins]);

  //MC input file
  //
  TString _path_TP = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/";
  TString _fname_TP = "InvM4test"+_option_tp+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num;
  TFile* file_test_TP = new TFile(_path_TP+_fname_TP+"_FIT_eff"+".root","read");

  if(file_test_TP->IsZombie()){

    cout<<"File does not exists"<<endl;
    cout<<"Creating file, please wait..."<<endl;
    TandP(leptonId,par1, npar1bins,par2, npar2bins, sel_den,sel_num,cut_num,par_x, par_y,option_tp);
    cout<<"Done !"<<endl;

  }

  cout<<"debug 5"<<endl;

  //if(!(f3->GetListOfKeys()->Contains("Eff_histo_"+sel_num+"_"+_pname+"_eta<1.2")||f3->GetListOfKeys()->Contains("eff_eta>1.2"))){
  //	f3->Close();

  //	cout<<"The files doesn't contain the requiered object"<<endl;
  //	cout<<"Doing the fit again"<<endl;
  //	TandP(leptonId,par_low,par_upp,nbins,sel_den,sel_num,cut_num,"CBxBW",option_mc);
  //	cout<<"Done !"<<endl;
  //}

  file_test_TP->Close();
  TFile* file_in_TP = new TFile(_path_TP+_fname_TP+"_FIT_eff"+".root","read");
  cout<<"list of keys of the TP file"<<endl;
  file_in_TP->GetListOfKeys()->ls();

  cout<<"debug 6"<<endl;

  /////////////////
  //Get the plots//
  /////////////////

  TH1D** eff_MC = new TH1D*[npar2bins];
  TH1D** eff_TP = new TH1D*[npar2bins];
  TH1D** ratio  =new TH1D*[npar2bins];

  for(int i = 0; i < npar2bins; ++i){

    TString _parybin;

    //Parameter string
    if(par_y == "Pt"){_parybin = Form("%0.f_Pt%0.f",par2[i],par2[i+1]);}
    else if(par_y == "eta"){_parybin = Form("%0.3f_eta%0.3f",par2[i],par2[i+1]);cout<<"it works !"<<endl;}
    else if(par_y == "phi"){_parybin = Form("%0.3f_phi%0.3f",par2[i],par2[i+1]);}

  cout<<"debug 7"<<endl;

    //retrieve the histo
    eff_TP[i] = (TH1D*)file_in_TP->Get("Eff_histo_"+sel_num+"_"+_pname+"_"+_par1range+"_"+_parybin);
    cout<<"eff_TP want to retrieve "<<"Eff_histo_"+sel_num+"_"+_pname+"_"+_par1range+"_"+_parybin<<endl;
  cout<<"debug 8.5"<<endl;
    eff_MC[i] = (TH1D*)file_in_MC->Get("eff"+_parybin);
    cout<<"eff_MC want to retrieve "<<"eff"+_parybin<<endl;
  cout<<"debug 8.75"<<endl;
    ratio[i]  = (TH1D*) eff_TP[i]->Clone("ratio");
    ratio[i]->Divide(eff_MC[i],eff_TP[i],1,1,"B");

  cout<<"debug 8"<<endl;

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
    eff_MC[i]->GetYaxis()->SetRangeUser(0,1.1);
    eff_MC[i]->GetYaxis()->SetTitle(" #epsilon ");
    eff_MC[i]->SetTitle(_sel_num+" for "+_sel_den+" "+pname+" #||{#eta}<1.2");

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
    ratio[i]->GetXaxis()->SetRangeUser(0,200);
    ratio[i]->GetXaxis()->SetTitle("");
    ratio[i]->Draw();

    file_out->cd();
    c1->Write("eff"+_parybin);
    c1->SaveAs(_outpath+"PDF/"+_output+"_"+option_mc+_pname+"_den_"+_sel_den+"_num_"+_sel_num+"_"+_parybin+".pdf");
    file_out->Close();

  }


  return 0;

}
