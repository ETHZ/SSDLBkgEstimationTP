/*********************************************
* Description - Compare directly branches form dyjets_new and dyjets_old 
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
#include "../tools/setTDRStyle.C"

int Compare(){

  setTDRStyle();

  TString location = "/Users/GLP/Desktop/CERN_data/dyjetsnew/postprocessed/";

  TChain* tree_new = new TChain("treeProducerSusyMultilepton");
  TChain* tree_good = new TChain("treeProducerSusyMultilepton");

  tree_new->Add(location+"dyjetsnew.root");
  tree_good->Add(location+"dyjets_good.root");

  //TH1D* hegt_good = new TH1D("hegt_good","t",5,0,5);
  //TH1D* heot_good = new TH1D("heot_good","t",5,0,5);

  //TH1D* hegt_new= new TH1D("hegt_new","t",5,0,5);
  //TH1D* heot_new= new TH1D("heot_new","t",5,0,5);

  //TH1D* hegt_good = new TH1D("hegt_good","t",200,0,0.5);
  //TH1D* heot_good = new TH1D("heot_good","t",200,0,0.5);

  //TH1D* hegt_new= new TH1D("hegt_new","t",200,0,0.5);
  //TH1D* heot_new= new TH1D("heot_new","t",200,0,0.5);

  TH1D* hegt_good = new TH1D("hegt_good","t",2,0,2);
  TH1D* heot_good = new TH1D("heot_good","t",2,0,2);

  TH1D* hegt_new= new TH1D("hegt_new","t",2,0,2);
  TH1D* heot_new= new TH1D("heot_new","t",2,0,2);

  //tree_new->Draw("LepGood_eleCutIdCSA14_50ns_v1 >> hegt_new", "abs(LepGood_pdgId) == 11");
  //tree_new->Draw("LepOther_eleCutIdCSA14_50ns_v1 >> heot_new", "abs(LepOther_pdgId) == 11");

  //tree_good->Draw("LepGood_eleCutIdCSA14_50ns_v1 >> hegt_good", "abs(LepGood_pdgId) == 11");
  //tree_good->Draw("LepOther_eleCutIdCSA14_50ns_v1 >> heot_good", "abs(LepOther_pdgId) == 11");
 
  tree_new->Draw("LepGood_tightId>> hegt_new", "abs(LepGood_pdgId) == 13");
  tree_new->Draw("LepOther_tightId>> heot_new", "abs(LepOther_pdgId) == 13");

  tree_good->Draw("LepGood_tightId>> hegt_good", "abs(LepGood_pdgId) == 13");
  tree_good->Draw("LepOther_tightId>> heot_good", "abs(LepOther_pdgId) == 13");

  hegt_new->Add(heot_new);
  hegt_good->Add(heot_good);

  hegt_new->Scale(1./(hegt_new->Integral()));
  hegt_good->Scale(1./(hegt_good->Integral()));

  TCanvas* c1 = new TCanvas("c1","c1");

  hegt_good->Draw();
  hegt_good->SetTitle("Rel. isolation #Delta R = 0.3 distribution (normalised)");
  hegt_good->GetXaxis()->SetTitle("rel. iso #Delta R = 0.3");
  //hegt_good->GetYaxis()->SetRangeUser(0,0.05);
  hegt_good->SetMarkerSize(0);
  hegt_new->Draw("same");
  hegt_new->SetMarkerSize(0);
  hegt_new->SetLineColor(2);
  hegt_new->SetMarkerColor(2);
  hegt_new->SetLineStyle(2);

  TLegend* leg = new TLegend(0.6, 0.65,0.89,0.89);
  leg->SetTextFont(43);
  leg->SetTextSize(25);
  leg->AddEntry(hegt_good, "dyjets_good");
  leg->AddEntry(hegt_new, "dyjets_new");
  leg->SetBorderSize(0);
  leg->Draw();


  return 0;

}

