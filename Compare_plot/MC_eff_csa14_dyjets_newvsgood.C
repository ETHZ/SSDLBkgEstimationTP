/*********************************************
* Description - Compare the MC ratio of dyjets_new and dyjets_good 
* Author - Gaël L. Perrin
* Date - Jan 13 2015
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
#include "../tools/setTDRStyle.C"
#include "TPaveText.h"

int Compare(){

  //output path
  TString _path = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/plot_compare/MC_eff_csa14_dyjets_newvsgood/";

  //Retrieve new
  //tight
  TFile* fnew1 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_ratio/effaddedtaudyjets_new_e-2.400_eta2.400_0.000_Pt1000.000_den__num_tightcut.root ","read");
  //TFile* fnew1pt = new TFile(path_new+"eff_newloose_e10.000_Pt250.000_0.000_eta2.500_den__num_tightcut.root","read");
  TFile* fnew1b = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_ratio/effdyjets_new_unmatched_e-2.400_eta2.400_0.000_Pt1000.000_den__num_tightcut.root","read");
  TFile* fnew2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_ratio/effaddedtaudyjets_new_mu-2.400_eta2.400_0.000_Pt1000.000_den__num_tightcut.root","read");
  //TFile* fnew2pt = new TFile(path_new+"eff_newloose_mu10.000_Pt250.000_0.000_eta2.500_den__num_tightcut.root","read");
  TFile* fnew2b = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_ratio/effaddedtaudyjets_new_unmatched_mu-2.400_eta2.400_0.000_Pt1000.000_den__num_tightcut.root","read");
  

  TH1D* hnew1 = (TH1D*)fnew1->Get("eff0_Pt1000");
  //TH1D* hnew1pt = (TH1D*)fnew1pt->Get("eff0.000_eta2.500");
  TH1D* hnew1b = (TH1D*)fnew1b->Get("eff0_Pt1000");
  TH1D* hnew2 = (TH1D*)fnew2->Get("eff0_Pt1000");
  //TH1D* hnew2pt = (TH1D*)fnew2pt->Get("eff0.000_eta2.500");
  TH1D* hnew2b = (TH1D*)fnew2b->Get("eff0_Pt1000");

  ////reliso
  //TFile* fnew3 = new TFile(path_new+"eff_newloose_e-2.400_eta2.400_0.000_Pt1000.000_den_tightcut_num_reliso3_0.100.root","read");
  //TFile*fnew3pt = new TFile(path_new+"eff_newloose_e10.000_Pt250.000_0.000_eta2.500_den_tightcut_num_reliso3_0.100.root","read");

  //TH1D* hnew3 = (TH1D*)fnew3->Get("eff0_Pt1000");
  //TH1D* hnew3pt = (TH1D*)fnew3pt->Get("eff0.000_eta2.500");

  //TFile* fnew3b = new TFile(path_new+"eff_newloose_unmatched_e-2.400_eta2.400_0.000_Pt1000.000_den__num_reliso3_0.100.root","read");
  //TH1D* hnew3b = (TH1D*)fnew3b->Get("eff0_Pt1000");

  //TFile* fnew4 = new TFile(path_new+"eff_newloose_mu-2.400_eta2.400_0.000_Pt1000.000_den_tightcut_num_reliso3_0.100.root","read");
  //TFile*fnew4pt = new TFile(path_new+"eff_newloose_mu10.000_Pt250.000_0.000_eta2.500_den_tightcut_num_reliso3_0.100.root","read");

  //TH1D* hnew4 = (TH1D*)fnew4->Get("eff0_Pt1000");
  //TH1D* hnew4pt = (TH1D*)fnew4pt->Get("eff0.000_eta2.500");

  //TFile* fnew4b = new TFile(path_new+"eff_newloose_unmatched_mu-2.400_eta2.400_0.000_Pt1000.000_den__num_reliso3_0.100.root","read");
  //TH1D* hnew4b = (TH1D*)fnew4b->Get("eff0_Pt1000");


  //Retrieve new_good14 
  //tight
  TFile* fnew_good1 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_eff/MCeffdyjets_good_e-2.400_eta2.400_0.000_Pt10000.000_den__num_tightcut.root","read");
  //TFile* fnew_good1pt = new TFile(path_new_good+"MCeff_e10.000_Pt250.000_0.000_eta2.500_den__num_tightcut.root","read");
  TFile* fnew_good1b = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_eff/MCeffdyjets_good_unmatched_e-2.400_eta2.400_0.000_Pt10000.000_den__num_tightcut.root","read");
  TFile* fnew_good2 = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_eff/MCeffaddedtaudyjets_good_mu-2.400_eta2.400_0.000_Pt10000.000_den__num_tightcut.root","read");
  //TFile* fnew_good2pt = new TFile(path_new_good+"MCeff_mu10.000_Pt250.000_0.000_eta2.500_den__num_tightcut.root","read");
  TFile* fnew_good2b = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/MC_eff/MCeffaddedtaudyjets_good_unmatched_mu-2.400_eta2.400_0.000_Pt10000.000_den__num_tightcut.root","read");



  TH1D* hnew_good1 = (TH1D*)fnew_good1->Get("eff0_Pt10000");
  //TH1D* hnew_good1pt = (TH1D*)fnew_good1pt->Get("eff0.000_eta2.500");
  TH1D* hnew_good1b = (TH1D*)fnew_good1b->Get("eff0_Pt10000");
  TH1D* hnew_good2 = (TH1D*)fnew_good2->Get("eff0_Pt10000");
  //TH1D* hnew_good2pt = (TH1D*)fnew_good2pt->Get("eff0.000_eta2.500");
  TH1D* hnew_good2b = (TH1D*)fnew_good2b->Get("eff0_Pt10000");

  ////reliso
  //TFile* fnew_good3 = new TFile(path_new_good+"MCeff_e-2.400_eta2.400_0.000_Pt10000.000_den_tightcut_num_reliso3_0.100.root","read");
  //TFile* fnew_good3pt = new TFile(path_new_good+"MCeff_e10.000_Pt250.000_0.000_eta2.500_den_tightcut_num_reliso3_0.100.root","read");

  //TH1D* hnew_good3 = (TH1D*)fnew_good3->Get("eff0_Pt10000");
  //TH1D* hnew_good3pt = (TH1D*)fnew_good3pt->Get("eff0.000_eta2.500");

  //TFile* fnew_good4 = new TFile(path_new_good+"MCeff_mu-2.400_eta2.400_0.000_Pt10000.000_den_tightcut_num_reliso3_0.100.root","read");
  //TFile* fnew_good4pt = new TFile(path_new_good+"MCeff_mu10.000_Pt250.000_0.000_eta2.500_den_tightcut_num_reliso3_0.100.root","read");

  //TH1D* hnew_good4 = (TH1D*)fnew_good4->Get("eff0_Pt10000");
  //TH1D* hnew_good4pt = (TH1D*)fnew_good4pt->Get("eff0.000_eta2.500");

  //TFile* fnew_good3b = new TFile(path_new_good+"MCeff_unmatched_e-2.400_eta2.400_0.000_Pt10000.000_den__num_reliso3_0.100.root","read");
  //TH1D* hnew_good3b = (TH1D*)fnew_good3b->Get("eff0_Pt10000");

  //TFile* fnew_good4b = new TFile(path_new_good+"MCeff_unmatched_mu-2.400_eta2.400_0.000_Pt10000.000_den__num_reliso3_0.100.root","read");
  //TH1D* hnew_good4b = (TH1D*)fnew_good4b->Get("eff0_Pt10000");

  
  //Draw the canvas
  
  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  hnew_good1->Draw();
  hnew_good1->SetMarkerSize(0);
  hnew1->Draw("same");
  hnew1->SetMarkerSize(0);
  hnew1->SetLineColor(2);
  hnew1->SetMarkerColor(2);

  //TCanvas* c1pt = new TCanvas("c1pt","c1pt");
  //c1pt->cd();
  //hnew_good1pt->Draw();
  ////hnew_good1b->Draw("same");
  ////hnew_good1b->SetLineColor(rt);
  ////hnew_good1b->SetMarkerColor(rt);
  //hnew1pt->Draw("same");
  //hnew1pt->SetLineColor(2);
  //hnew1pt->SetMarkerColor(2);

  TCanvas* c1b = new TCanvas("c1b","c1b");
  c1b->cd();
  hnew_good1b->Draw();
  hnew_good1b->SetMarkerSize(0);
  hnew1b->Draw("same");
  hnew1b->SetMarkerSize(0);
  hnew1b->SetLineColor(2);
  hnew1b->SetMarkerColor(2);


  TCanvas* c2 = new TCanvas("c2","c2");
  c2->cd();
  hnew_good2->Draw();
  hnew_good2->SetMarkerSize(0);
  hnew2->Draw("same");
  hnew2->SetLineColor(2);
  hnew2->SetMarkerSize(0);
  hnew2->SetMarkerColor(2);

  //TCanvas* c2pt = new TCanvas("c2pt","c2pt");
  //c2pt->cd();
  //hnew_good2pt->Draw();
  //hnew2pt->Draw("same");
  //hnew2pt->SetLineColor(2);
  //hnew2pt->SetMarkerColor(2);
  ////here

  TCanvas* c2b = new TCanvas("c2b","c2b");
  c2b->cd();
  hnew_good2b->Draw();
  hnew_good2b->SetMarkerSize(0);
  hnew2b->Draw("same");
  hnew2b->SetMarkerSize(0);
  hnew2b->SetLineColor(2);
  hnew2b->SetMarkerColor(2);

  //TCanvas* c3 = new TCanvas("c3","c3");
  //c3->cd();
  //hnew_good3->Draw();
  //hnew3->Draw("same");
  //hnew3->SetLineColor(2);
  //hnew3->SetMarkerColor(2);

  //TCanvas* c3b = new TCanvas("c3b","c3b");
  //c3b->cd();
  //hnew_good3b->Draw();
  //hnew3b->Draw("same");
  //hnew3b->SetLineColor(2);
  //hnew3b->SetMarkerColor(2);

  //TLegend* leg3b = new TLegend(0.6, 0.2,0.89,0.45);
  //leg3b->SetTextFont(43);
  //leg3b->SetTextSize(25);
  //leg3b->AddEntry(hnew_good3b, "dyjets_good");
  //leg3b->AddEntry(hnew3b, "dyjets_new");
  //leg3b->SetBorderSize(0);
  //leg3b->Draw();

  //TCanvas* c3pt = new TCanvas("c3pt","c3pt");
  //c3pt->cd();
  //hnew_good3pt->Draw();
  //hnew_good3pt->SetMarkerSize(0.1);
  //hnew3pt->Draw("same");
  //hnew3pt->SetLineColor(2);
  //hnew3pt->SetMarkerColor(2);
  //hnew3pt->SetMarkerSize(0.1);

  //TCanvas* c4 = new TCanvas("c4","c4");
  //c4->cd();
  //hnew_good4->Draw();
  //hnew_good4->SetMarkerSize(0.1);
  //hnew4->Draw("same");
  //hnew4->SetLineColor(2);
  //hnew4->SetMarkerColor(2);
  //hnew4->SetMarkerSize(0.1);

  //TLegend* leg4 = new TLegend(0.6, 0.65,0.89,0.89);
  //leg4->SetTextFont(43);
  //leg4->SetTextSize(25);
  //leg4->AddEntry(hnew_good4, "dyjets_good");
  //leg4->AddEntry(hnew4, "dyjets_new");
  //leg4->SetBorderSize(0);
  //leg4->Draw();

  //TCanvas* c4b = new TCanvas("c4b","c4b");
  //c4b->cd();
  //hnew_good4b->Draw();
  //hnew_good4b->SetMarkerSize(0.1);
  //hnew4b->Draw("same");
  //hnew4b->SetLineColor(2);
  //hnew4b->SetMarkerColor(2);
  //hnew4b->SetMarkerSize(0.1);

  ////TCanvas* c4pt = new TCanvas("c4pt","c4pt");
  ////c4pt->cd();
  ////hnew_good4pt->Draw();
  ////hnew4pt->Draw("same");
  ////hnew4pt->SetLineColor(2);
  ////hnew4pt->SetMarkerColor(2);

  TFile* file_out = new TFile(_path + "canvas.root","recreate");
  c1->Write();
  //c1pt->Write();
  //c1b->Write();

  //c2->Write();
  //c2pt->Write();
  //c2b->Write();

  //c3->Write();
  //c3pt->Write();

  //c4->Write();
  ////c4pt->Write();

  file_out->Close();

  c1->SaveAs(_path+"c1.pdf");
  //c1pt->SaveAs(_path+"c1pt.pdf");
  //c1b->SaveAs(_path+"c1b.pdf");

  //c2->SaveAs(_path+"c2.pdf");
  //c2pt->SaveAs(_path+"c2pt.pdf");
  //c2b->SaveAs(_path+"c2b.pdf");

  //c3->SaveAs(_path+"c3.pdf");
  //c3pt->SaveAs(_path+"c3pt.pdf");

  //c4->SaveAs(_path+"c4.pdf");
  //c4pt->SaveAs(_path+"c4pt.pdf");

  return 0;

}
