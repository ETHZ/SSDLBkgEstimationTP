/*********************************************
 * Description - A modification of MC_Ratio2 to take in account the lep_other, which was not done correctly in MC_Ratio2
 Final version

 * Author - Gaël L. Perrin

 * Date - Jan 05 2015
 * *******************************************/

#include "cmath"
#include "TChain.h"
#include <sys/stat.h>
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

//Helper functions
#include "../tools/InvMass.C"
#include "../tools/DeltaR.C"
#include "../tools/setTDRStyle.C"

#include "iostream"


int MC_Ratio(TString _filetag, int leptonId, double* par1, int npar1bins, double* par2, int npar2bins, TString sel_den , TString sel_num, double cut_num = 0., TString par_x = "Pt", TString par_y = "eta", TString option = "");

int MC_Ratio(TString _filetag,int leptonId, double par_low, double par_upp, int npar1bins, TString sel_den , TString sel_num, double cut_num = 0., TString par_x = "Pt", TString par_y = "eta", TString option = ""){


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
  double par2_pt[npar2bins_pt+1] = {0,1000};

  if(par_y == "eta"){
    return MC_Ratio(_filetag,leptonId, par1, npar1bins, par2_eta, npar2bins_eta, sel_den, sel_num, cut_num, par_x, par_y, option );
  }else if(par_y == "Pt"){
    return MC_Ratio(_filetag,leptonId, par1, npar1bins, par2_pt, npar2bins_pt, sel_den, sel_num, cut_num, par_x, par_y, option );
  }else{return 1;}

 
	  if((!option.Contains("loose"))||((option.Contains("loose"))&&(evtloose[j] == 1))){
	    if((sel_den != "tightcut")||(((abs(evtid[j]) == 13)&&(sel_den == "tightcut")&&(evttight[j] == 1 ))||((abs(evtid[j]) == 11)&&(sel_den == "tightcut")&&(evttighte[j] >= 3)))){
	      if((sel_den != "tightmva")||((abs(evtid[j]) == 11)&&(sel_den == "tightmva")&&(evttight[j] == 1))){

		//Variable for matching
		double R = 999;
		double delta_P = 999;
		double delta_charge = 999;

		//Parameter on the xaxis

		double par;
		double par_2;

		//loop over all generated particles to do the matching
		for (int i = 0; i < ngenPart; ++i) {
		  if(Id[i] == evtid[j]){ 

		    //Electrons selection
		    double R2 = DeltaR(gen_eta[i],evteta[j],gen_phi[i],evtphi[j] );

		    //Minimise DeltaR and Fill the other variables
		    if (R > R2) {
		      R = R2;
		      delta_P = abs(evtpt[j]-Pt[i])/Pt[i];
		      delta_charge = abs(evtq[j] - charge[i]);
		    }
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
		    if((par_2 > par2[ii])&&(par_2 <= par2[ii+1])){histo_den[ii]->Fill(par);histo_par1[ii]->Fill(par);

		      if(evtloose[j] == 1){histo_good_sel->Fill(evttighte[j]);}
		      else if(evtloose[j] == 0){histo_other_sel->Fill(evttighte[j]);}
		      else{cout<<"Error !"<<endl; return 1;}

		      //Fill par1 distribution
		      for(int pp = 0; pp <npar1bins; ++pp){
			if((par > par1[pp])&&(par <= par1[pp+1])){h_par1[ii][pp]->Fill(par);}
		      }
		    }
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
		  //if((sel_num == "mvaid")&&(abs(evtmvaid[j]) >= cut_num)){a = 9;}

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
	//}
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
    eff[i]->GetYaxis()->SetRangeUser(0,1.1);
    eff[i]->GetXaxis()->SetTitle(_par);
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


    //TCanvas* cO = new TCanvas("cO","cO");
    //cO->cd();
    //eff_O[i]->Draw();
    //eff_O[i]->GetYaxis()->SetTitle("#epsilon");
    //eff_O[i]->GetXaxis()->SetTitle(_par);
    //eff_O[i]->SetMarkerStyle(20);
    //eff_O[i]->SetMarkerSize(1);
    //eff_O[i]->SetMarkerColor(4);
    //eff_O[i]->SetLineColor(4);
    //eff_O[i]->SetTitle(_sel_num+" for "+_sel_den+" "+pname+" "+_parytitle+" , LepOther");

    //TCanvas* cG = new TCanvas("cG","cG");
    //cG->cd();
    //eff_G[i]->Draw();
    //eff_G[i]->GetYaxis()->SetTitle("#epsilon");
    //eff_G[i]->GetXaxis()->SetTitle(_par);
    //eff_G[i]->SetMarkerStyle(20);
    //eff_G[i]->SetMarkerSize(1);
    //eff_G[i]->SetMarkerColor(4);
    //eff_G[i]->SetLineColor(4);
    //eff_G[i]->SetTitle(_sel_num+" for "+_sel_den+" "+pname+" "+_parytitle+" , LepGood");

    //TCanvas* ccpar = new TCanvas("ccpar","ccpar");
    //ccpar->cd();
    //histo_counter_par1[i]->Draw();

    //TCanvas* cc = new TCanvas("cc","cc");
    //cc->cd();
    //histo_counter[i]->Draw();
    //
    histo_good_sel->Add(histo_other_sel);
    TCanvas* csel = new TCanvas("csel","csel");
    csel->cd();
    histo_good_sel->Draw();

    /////////////////////
    //Saving the output//
    /////////////////////

    //Write pdf
    TString cname = "eff"+_filetag+_option+_pname+_par1range+"_"+_parybin+"_den_"+_sel_den+"_num_"+_sel_num;
    c1->SaveAs(_path+_fname+"_PDF/"+cname+".pdf");
    //c_par1->SaveAs(_path+_fname+"_PDF/"+cname+"par_distr.pdf");
    csel->SaveAs(_path+_fname+"_PDF/"+cname+"sel.pdf");
    //cO->SaveAs(_path+_fname+"_PDF/"+cname+"_LepOther.pdf");
    //cG->SaveAs(_path+_fname+"_PDF/"+cname+"_LepGood.pdf");
    //ccpar->SaveAs(_path+_fname+"_PDF/"+cname+"_counter_par.pdf");
    //cc->SaveAs(_path+_fname+"_PDF/"+cname+"_counter.pdf");

    //Write in output file
    file_out->cd();
    eff[i]->Write("eff"+_parybin);
    histo_par1[i]->Write("histo_par1_"+_parybin);
    histo_good_sel->Write("sel"+_parybin);
    //eff_O[i]->Write("eff_LepOther"+_parybin);
    //eff_G[i]->Write("eff_LepGood"+_parybin);
    //histo_counter_par1[i]->Write("counter_par"+_parybin);
    //histo_counter[i]->Write("counter"+_parybin);
    //
    //
    for( int _i = 0; _i < npar1bins; ++_i){

      TString _parxbin;

      if(par_x == "Pt"){_parxbin = Form("%0.f_Pt%0.f",par1[_i],par1[_i+1]);}
      else if(par_x == "eta"){_parxbin = Form("%0.3f_eta%0.3f",par1[_i],par1[_i+1]);}
      else if(par_x == "phi"){_parxbin = Form("%0.3f_phi%0.3f",par1[_i],par1[_i+1]);}

      h_par1[i][_i]->Write("histo_par1_"+_parxbin+"_"+_parybin);

    }

  }

  file_out->Close();


  return 0;

}
