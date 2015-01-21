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
#include "MC_Ratio.C"
#include "TandP.C"
#include "TPaveText.h"

//helper functions
#include "../tools/InvMass.C"
#include "../tools/DeltaR.C"
#include "../tools/setTDRStyle.C"
#include "../tools/getmaxallhist.C"

int CompareMCvsTandP(TString _filetag, int leptonId, double* par1, int npar1bins, double* par2, int npar2bins, TString sel_den = "tight", TString sel_num = "", double cut_num = 0.2, TString par_x = "Pt", TString par_y = "eta", TString option_mc = "",TString option_tp = "", TString _sig = "CBxBW");

int CompareMCvsTandP(TString _filetag, int leptonId, double par_low, double par_upp , int npar1bins, TString sel_den = "tight", TString sel_num = "", double cut_num = 0.2, TString par_x = "Pt", TString par_y = "eta", TString option_mc = "", TString option_tp = "", TString _sig = "CBxBW"){

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
  double par2_pt[npar2bins_pt+1] = {10,250};

  if(par_y == "eta"){
    return CompareMCvsTandP(_filetag, leptonId, par1, npar1bins, par2_eta, npar2bins_eta, sel_den, sel_num, cut_num, par_x, par_y, option_mc,option_tp, _sig);
  }else if(par_y == "Pt"){
    return CompareMCvsTandP(_filetag, leptonId, par1, npar1bins, par2_pt, npar2bins_pt, sel_den, sel_num, cut_num, par_x, par_y, option_mc,option_tp, _sig);
  }else{return 1;}

}

int CompareMCvsTandP( TString _filetag, int leptonId, double* par1, int npar1bins, double* par2, int npar2bins, TString sel_den, TString sel_num, double cut_num, TString par_x, TString par_y, TString option_mc, TString option_tp, TString _sig){

  setTDRStyle();

  //TString _filetag = "_newloose_neworder";

  _filetag = ""+ _filetag;

  //output of the code
  TString _output ="Compare"+_filetag;
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
  if(option_mc.Contains(" short ")){_option_mc += "_short";}
  if(option_mc.Contains(" loose ")){_option_mc += "_loose";}
  if(option_mc.Contains(" oldtree ")){_option_mc += "_oldtree";}
  _option_mc += "_";
  //parameter range string
  TString _par1range;
  _par1range = Form("%0.3f_"+par_x+"%0.3f",par1[0],par1[npar1bins]);
  TString _par2range;
  _par2range = Form("%0.3f_"+par_y+"%0.3f",par2[0],par2[npar2bins]);

  //MC input file
  //
  TString _path_MC = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_ratio/";
  TString _fname_MC = "eff"+_filetag+_option_mc+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num;

  cout<<"debug 1"<<endl;

  //Check if file exits
  TFile* file_test_MC = new TFile(_path_MC+_fname_MC+".root");
  if(file_test_MC->IsZombie()){
    cout<<"File does not exists"<<endl;
    cout<<"Creating file, please wait..."<<endl;
    MC_Ratio(_filetag, leptonId,par1,npar1bins,par2, npar2bins, sel_den,sel_num,cut_num,par_x, par_y,option_mc);
    cout<<"Done !"<<endl;
  }

  TFile* file_in_MC = new TFile(_path_MC+_fname_MC+".root", "read");
  delete file_test_MC;
  cout<<"list of keys of the MC file"<<endl;
  file_in_MC->GetListOfKeys()->ls();


  //////////
  //output// 
  //////////

  TFile* file_out = new TFile(_outpath+_output+_option_mc+_option_tp+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num+".root","recreate");


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
  if(option_tp.Contains(" matching ")){_option_tp += "_Matched";_optionInvM += "_Matched";}
  if(option_tp.Contains(" nentries ")){_option_tp += "_nentries";}
  if(option_tp.Contains(" short ")){_optionInvM += "_short";}
  if(option_tp.Contains(" loose ")){_option_tp += "_loose";_optionInvM += "_loose";}
  if(option_tp.Contains(" oldtree ")){_option_tp += "_oldtree";_optionInvM += "_oldtree";}
  _option_tp += "_";
  _optionInvM += "_";
  //parameter range string
  _par1range = Form("%0.3f_"+par_x+"%0.3f",par1[0],par1[npar1bins]);
  _par2range = Form("%0.3f_"+par_y+"%0.3f",par2[0],par2[npar2bins]);

  //TP input file
  //
  TString _path_TP = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/";
  TString _fname_TP = "InvM"+_filetag+_option_tp+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num;
  TFile* file_test_TP = new TFile(_path_TP+_fname_TP+"_FIT_eff"+".root","read");
  //Location of mass distribution
  cout<<"will check if file "<<_path_TP+_fname_TP+"_FIT_eff"+".root"<<" exists"<<endl;


  if(file_test_TP->IsZombie()){

    cout<<"File does not exists"<<endl;
    cout<<"Creating file, please wait..."<<endl;
    TandP(_filetag, leptonId,par1, npar1bins,par2, npar2bins, sel_den,sel_num,cut_num,par_x, par_y,option_tp);
    cout<<"Done !"<<endl;

  }

  file_test_TP->Close();
  TFile* file_in_TP = new TFile(_path_TP+_fname_TP+"_FIT_eff"+".root","read");
  cout<<"list of keys of the TP file"<<endl;
  file_in_TP->GetListOfKeys()->ls();

  TString _path_invM  = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/ZBkgInvM/";
  TString _fname_invM = "InvM"+_filetag+_optionInvM+_pname+_par1range+"_"+_par2range+"_den_"+_sel_den+"_num_"+_sel_num;
  TFile* file_invM = new TFile(_path_invM+_fname_invM+".root","read");
  cout<<"The list of keys of the invM file"<<endl;
  file_invM->GetListOfKeys()->ls();

  cout<<"debug 6"<<endl;

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
  TH1D*** par1bin_dy_TP = new TH1D**[npar2bins];

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
    cout<<"par1_dy_TP want to retrieve "<<"histo_par1_dyonly"+_parybin<<endl;
    par1_TP[i] = (TH1D*) file_invM->Get("histo_par1_"+_parybin);
    cout<<"debug"<<endl;
    par1_dy_TP[i] = (TH1D*) file_invM->Get("histo_par1_dyonly"+_parybin);
    cout<<"debug1"<<endl;

    //normalise par1 distributions
    par1_MC[i]->Scale(1./par1_MC[i]->Integral());
    par1_TP[i]->Scale(1./par1_TP[i]->Integral());
    par1_dy_TP[i]->Scale(1./par1_dy_TP[i]->Integral());


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

    TCanvas* c2 = new TCanvas("c2","c2");
    par1_MC[i]->Draw(); 
    par1_MC[i]->SetLineColor(4);
    par1_MC[i]->SetLineWidth(3);
    par1_MC[i]->SetMarkerColor(4);
    par1_MC[i]->SetMarkerStyle(1);
    par1_MC[i]->GetYaxis()->SetTitle("par1");
    par1_MC[i]->SetTitle(par_x+" distribution for "+_sel_den+" "+pname+ _parybin);
    par1_MC[i]->GetYaxis()->SetRangeUser(0,getmaxallhist(par1_MC[i],par1_TP[i],par1_dy_TP[i]));
    par1_TP[i]->Draw("same"); 
    par1_TP[i]->SetLineColor(2);
    par1_TP[i]->SetLineWidth(3);
    par1_TP[i]->SetMarkerColor(2);
    par1_TP[i]->SetMarkerStyle(1);
    par1_dy_TP[i]->Draw("same");    
    par1_dy_TP[i]->SetLineColor(1);
    par1_dy_TP[i]->SetLineWidth(3);
    par1_dy_TP[i]->SetMarkerColor(1);
    par1_dy_TP[i]->SetMarkerStyle(1);

    TLegend* leg2 = new TLegend(0.3, 0.3, 0.6 , 0.5);
    leg2->AddEntry(par1_MC[i], "MC Ratio");
    leg2->AddEntry(par1_TP[i], "T&P","LP");
    leg2->AddEntry(par1_dy_TP[i], "T&P DY only","LP");
    leg2->SetBorderSize(0.);
    leg2->SetTextFont(43);
    leg2->SetTextSize(25);
    leg2->Draw();

    file_out->cd();
    c1->Write("eff"+_parybin);
    c1->SaveAs(_outpath+"PDF/"+_output+"_"+_option_mc+"_"+_option_tp+_pname+"_den_"+_sel_den+"_num_"+_sel_num+"_"+_parybin+".pdf");
    c2->Write("par1_distr"+_parybin);

    //binned par1

    par1bin_MC[i] 			= new TH1D*[npar1bins];
    par1bin_TP[i] 			= new TH1D*[npar1bins];
    par1bin_dy_TP[i] 			= new TH1D*[npar1bins];

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
      par1bin_dy_TP[i][_i] = (TH1D*) file_invM->Get("h_par1_dyonly"+_parxbin+"_"+_parybin);
      cout<<"debug3"<<endl;

      //normalise par1 distributions
      par1bin_MC[i][_i]->Scale(1./par1bin_MC[i][_i]->Integral());
      cout<<"debug4"<<endl;
      par1bin_TP[i][_i]->Scale(1./par1bin_TP[i][_i]->Integral());
      cout<<"debug5"<<endl;
      par1bin_dy_TP[i][_i]->Scale(1./par1bin_dy_TP[i][_i]->Integral());
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
    par1bin_MC[i][_i]->GetYaxis()->SetRangeUser(0,getmaxallhist(par1bin_MC[i][_i],par1bin_TP[i][_i],par1bin_dy_TP[i][_i]));
    par1bin_TP[i][_i]->Draw("same"); 
    par1bin_TP[i][_i]->SetLineColor(2);
    par1bin_TP[i][_i]->SetLineWidth(3);
    par1bin_TP[i][_i]->SetMarkerColor(2);
    par1bin_TP[i][_i]->SetMarkerStyle(1);
    par1bin_dy_TP[i][_i]->Draw("same");    
    par1bin_dy_TP[i][_i]->SetLineColor(1);
    par1bin_dy_TP[i][_i]->SetLineWidth(3);
    par1bin_dy_TP[i][_i]->SetMarkerColor(1);
    par1bin_dy_TP[i][_i]->SetMarkerStyle(1);

    TLegend* leg3 = new TLegend(0.3, 0.3, 0.6 , 0.5);
    leg3->AddEntry(par1bin_MC[i][_i], "MC Ratio");
    leg3->AddEntry(par1bin_TP[i][_i], "T&P","LP");
    leg3->AddEntry(par1bin_dy_TP[i][_i], "T&P DY only","LP");
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
