/*********************************************
* Description - Plots the relative isolation for various pile-up 
* Author - Gaël L. Perrin
* Date - Jan 20 2015
* *******************************************/

#include "../tools/include.C"
#include "../tools/setTDRStyle.C"
#include "../tools/mkfolder.C"

int Plot(TString option = ""){

  TString _path = "/Users/GLP/Dropbox/Physique/Master_Thesis/plots_root/Various_plots/";
  TString _fname = "reliso_vs_nVert";
  mkfolder(_path+_fname);


  //////////////
  //Parameters//
  //////////////

  int isoR = 3;//iso cone

  setTDRStyle();

  //Location of the .root file
  TString location = "/Users/GLP/Desktop/CERN_data/2014-11-13_skim2ll-mva-softbtag/postprocessed/matching2/";
  TString location2 = "/Users/GLP/Desktop/CERN_data/dyjetsnew/postprocessed/matched/";

  //Reading the tree 
  //
  TChain* tree = new TChain("treeProducerSusyMultilepton");

  //DY events
  if(option.Contains("oldtree")){tree->Add(location+"DYJetsToLLM50_PU_S14_POSTLS170.root");}
      else{tree->Add(location2+"dyjetsnew.root");}

      //String



      /////////////////////
      //Declare variables//
      /////////////////////
      
      const int nPUbins = 11;//number of pile-up bins
      double PU[nPUbins+1] = {5,10,15,20,25,30,35,40,45,50,55,60};

      //////////////////
      //Fill histogram//
      //////////////////
      
      TH2D* h_reliso = new TH2D("h_reliso","h",nPUbins,PU,50,0.,0.5);
      tree->Draw(Form("LepOther_relIso0%i:nVert >> h_reliso",isoR),"","goff");
      tree->Draw(Form("LepGood_relIso0%i:nVert >>+ h_reliso",isoR),"","goff");

      TH1D *h = h_reliso->ProfileX()->ProjectionX();


      TCanvas* c1 = new TCanvas("c1","c1");
      h->Scale(100);//put in %
      h->Draw();
      h->SetTitle(Form("Rel. Iso. #Delta R = 0.%i vs. n. Vertex",isoR));
      h->GetXaxis()->SetTitle("n. Vertex");
      h->GetYaxis()->SetTitle(Form("rel. iso #Delta R = 0.%i (%%)",isoR));

      c1->SaveAs(Form(_path+_fname+"/"+"reliso%i_nVert.pdf",isoR));

      return 0;

}
