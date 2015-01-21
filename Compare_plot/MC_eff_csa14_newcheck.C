/*********************************************
* Description - To assure that there are no bugs in the MC_Ratio, this file compares the tighte fot the electron obtained using MC_Ratio2.C in the MC_Ratio folder with the histogram obtained from Compare_sample compare_evt_dytree.C 
* Author - Gaël L. Perrin
* Date - Jan 14 2015
* *******************************************/


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
#include <TColor.h>
#include <TLegend.h>
#include <TCanvas.h>
#include "../setTDRStyle.C"
#include "TPaveText.h"

int Compare(){

  setTDRStyle();


  TString path = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_eff/";
  TString pathnew = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_ratio/eff_newloose_unmatched_e-2.400_eta2.400_0.000_Pt1000.000_den__num_tightcut.root";

  //Retrieve good 
  //tight
  TFile* fgood     	= new TFile(path+"MCeff_debugging6__unmatched_e-2.400_eta2.400_0.000_Pt10000.000_den__num_tightcut.root","read");
  TFile* fnew 		= new TFile(pathnew,"read");
  TFile* fcompared 	= new TFile(path+"compare_evt_dytree.root","read");
  TFile* fgood2		= new TFile(path+"compare_evt_dytree.root","read");//fgood with the comparison code
  TFile* fnew2 		= new TFile(pathnew,"compare_evt_dytree.root","read");//fgood with the comparison code


  TH1D* hgood		= (TH1D*)fgood->Get("sel0_Pt10000");
  TH1D* hnew		= (TH1D*)fnew->Get("sel0_Pt1000");
  //TH1D* htotal		= (TH1D*)fgood->Get("totalsel0_Pt10000");
  TH1D* hcompared	= (TH1D*)fcompared->Get("hgood");
  TH1D* hgood2	        = (TH1D*)fcompared->Get("histo_good");
  TH1D* hnew2		= (TH1D*)fcompared->Get("histo_new");

  hgood->Scale(1./hgood->Integral());
  hnew->Scale(1./hnew->Integral());
  //htotal->Scale(1./htotal->Integral());
  hcompared->Scale(1./hcompared->Integral());
  hgood2->Scale(1./hgood2->Integral());
  hnew2->Scale(1./hnew2->Integral());

  TCanvas* c1 = new TCanvas("c1","c1");

  hnew->Draw();
  hnew->SetLineColor(4);
  hnew->SetLineStyle(4);
  hnew->SetTitle("tightcut for e");
  hcompared->Draw("same");
  hcompared->SetLineColor(2);
  hcompared->SetLineStyle(3);
  //htotal->Draw("same");
  hgood->Draw("same");
  //hgood->SetLineColor(2);
  //hgood->SetLineStyle(2);

  TLegend* leg1 = new TLegend(0.6, 0.65,0.89,0.89);
  leg1->SetTextFont(43);
  leg1->SetTextSize(25);
  leg1->AddEntry(hnew, "dyjets new using MC_Ratio");
  leg1->AddEntry(hcompared, "dyjets new intersection good");
  leg1->AddEntry(hgood, "dyjets good using MC_Ratio");
  leg1->SetBorderSize(0);
  leg1->Draw();

  TCanvas* c2 = new TCanvas("c2","c2");

  hnew2->Draw();
  hnew2->SetTitle("tightcut for e");
  hnew2->SetLineColor(4);
  hnew2->SetLineStyle(4);
  hgood2->Draw("same");
  hgood2->SetLineColor(2);
  hgood2->SetLineStyle(3);

  TLegend* leg2 = new TLegend(0.6, 0.65,0.89,0.89);
  leg2->SetTextFont(43);
  leg2->SetTextSize(25);
  leg2->AddEntry(hnew2, "dyjets new");
  leg2->AddEntry(hgood2, "dyjets good");
  leg2->SetBorderSize(0);
  leg2->Draw();

}
