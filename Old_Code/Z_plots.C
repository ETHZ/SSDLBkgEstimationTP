#include "cmath"
#include "TChain.h"
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
#include "TChain.h"
#include "ReadHistoPDGId.C"
#include <fstream>

void Z_plots()
{

//Read Tree
bool all_data = false;
TChain* tree = new TChain("treeProducerSusyFullHad");
if (all_data == true){tree->Add("/Users/GLP/Desktop/CERN_data/ZJetsToNuNu*.root");}
else if (all_data == false) {
tree->Add("/Users/GLP/Desktop/CERN_data/ZJetsToNuNu_HT100to200_PU_S14_POSTLS170_babytree_QG.root");
}

Long64_t n = tree->GetEntries();

//number of bins
int MoNbins = 12000;
int MoMax = 12000;
int statusbins = 50;
int statusmax = 25;
int nbins = 25;
int nbins2 = 500;
double Pt_cut = 10;
int mother_Id = 23;
Int_t leptonId = 11;
TString pname;
if (leptonId == 11) {pname = "e";}
else if (leptonId == 13){pname = "#mu";}

TH1D *histo_drelIso03 = new TH1D("histo_drelIso03","relIso03",300,0,1);
TH1D *histo_drelIso04 = new TH1D("histo_drelIso04","relIso04",1500,0,100);
TH1D *histo_drelIso_noW03 = new TH1D("histo_drelIso_noW03","relIso_noW03",300,0,1);
TH1D *histo_drelIso_noW04 = new TH1D("histo_drelIso_noW04","relIso_noW04",1500,0,100);
TH1D *histo_dxy = new TH1D("histo_dxy","dxy",nbins2,0,1);
TH1D *histo_dz = new TH1D("histo_z","dz",300,0,2);
TH1D *histo_dxy_noW = new TH1D("histo_dxy_noW","dxy",nbins2,0,1);
TH1D *histo_dz_noW = new TH1D("histo_z_noW","dz",300,0,2);
TH1D *histo_motherId= new TH1D("histo_motherId","Mo",MoNbins,0,MoMax);
TH1D *histo_GrmotherId= new TH1D("Grhisto_motherId","GrMo",MoNbins,0,MoMax);
TH1D *histo_motherId_noW = new TH1D("histo_motherId_noW","Mo_noW",MoNbins,0,MoMax);
TH1D *histo_GrmotherId_noW = new TH1D("Grhisto_motherId_noW","GrMo_noW",MoNbins,0,MoMax);
TH1D *histo_motherId_unsel= new TH1D("histo_motherId_unsel","Mo",MoNbins,0,MoMax);
TH1D *histo_GrmotherId_unsel= new TH1D("Grhisto_motherId_unsel","GrMo",MoNbins,0,MoMax);
//Generated particles whitout matching
TH1D *histo_genMo_W = new TH1D("Grhisto_genMo_W","GrMo",MoNbins,0,MoMax);
TH1D *histo_genGrMo_W = new TH1D("Grhisto_genGrMo_W","GrMo",MoNbins,0,MoMax);
TH1D *histo_genMo= new TH1D("Grhisto_genMo","GrMo",MoNbins,0,MoMax);
TH1D *histo_genGrMo= new TH1D("Grhisto_genGrMo","GrMo",MoNbins,0,MoMax);
TH1D *histo_genStatus_W= new TH1D("Grhisto_genStatus_W","status",statusbins,-statusmax,statusmax);
TH1D *histo_genStatus= new TH1D("Grhisto_genStatus","status",statusbins,-statusmax,statusmax);
TH1D *histo_genCharge_W= new TH1D("Grhisto_genCharge_W","charge",22,-11,11);
TH1D *histo_genPair_W = new TH1D("Grhisto_genPair_W","charge",22,-11,11);
TH1D *histo_genCharge= new TH1D("Grhisto_genCharge","charge",22,-11,11);
TH1D *histo_genPair = new TH1D("Grhisto_genPair_W","charge",22,-11,11);
//Counting prompt vs not prompt
TH1D *histo_nprompt= new TH1D("Grhisto_nprompt","GrMo",100,0,1);
TH1D *histo_nnotprompt= new TH1D("Grhisto_nnotprompt","GrMo",100,0,1);
//Gen. particles removed with status ==1
TH1D *histo_genRemovedMo= new TH1D("Grhisto_genRemovedMo","GrMo",30,0,30);
TH1D *histo_genRemovedGrMo= new TH1D("Grhisto_genRemovedGrMo","GrMo",30,0,30);

//Event variables Float_t dxy[200];
Float_t dz[200];
Float_t relIso03[200];
Float_t relIso04[200];
Float_t gen_phi[200];
Float_t gen_eta[200];
Float_t rec_phi[200];
Float_t rec_eta[200];
Float_t Pt[200];
Float_t m[200];
Float_t recm[200];
Int_t Id[200];
Int_t Mo[200];
Int_t recId[200];
Float_t recPt[200];
Float_t charge[200];
Int_t reccharge[200];
Float_t dxy[200];
Int_t GrMa[200];
Int_t status[200];
Int_t ngenPart;
Int_t nrecPart;
Int_t nrecel;

//Assigne branches //generated
tree->SetBranchAddress("genPart_pdgId", &Id);
tree->SetBranchAddress("genPart_motherId", &Mo);
tree->SetBranchAddress("genPart_grandmaId",&GrMa);
tree->SetBranchAddress("genPart_eta", &gen_eta);
tree->SetBranchAddress("genPart_phi", &gen_phi);
tree->SetBranchAddress("ngenPart", &ngenPart);
tree->SetBranchAddress("genPart_pt", &Pt);
tree->SetBranchAddress("genPart_mass", &m);
tree->SetBranchAddress("genPart_charge", &charge);
tree->SetBranchAddress("genPart_status", &status);
//reconstructed
tree->SetBranchAddress("lep_pdgId", &recId);
tree->SetBranchAddress("nlep", &nrecPart);
tree->SetBranchAddress("nElectrons10", &nrecel);
tree->SetBranchAddress("lep_pt", &recPt);
tree->SetBranchAddress("lep_mass", &recm);
tree->SetBranchAddress("lep_eta", &rec_eta);
tree->SetBranchAddress("lep_phi", &rec_phi);
tree->SetBranchAddress("lep_charge", &reccharge);
tree->SetBranchAddress("lep_dxy", &dxy);
tree->SetBranchAddress("lep_dz", &dz);
tree->SetBranchAddress("lep_relIso03", &relIso03);
tree->SetBranchAddress("lep_relIso04", &relIso04);


int nprompt = 0;
int nnotprompt = 0;
//Start loop over all eventris
for (int k = 0; k < n; ++k) {

	cout<<"Event number :"<<k<<endl;


tree->GetEntry(k);

//To count the number of charged leptons per events

int promptleppos = 0;
int promptlepneg = 0;
int leppos = 0;
int lepneg = 0;

//Loop over all leptons
for (int i= 0; i < ngenPart; ++i) {


//Removed with Status != 1
if((abs(Id[i]) == leptonId)&&(status[i] !=1)&&((abs(Mo[i]) == mother_Id)||((abs(GrMa[i]) == mother_Id)&&(abs(Mo[i]) != 15)))&&(Pt[i]>Pt_cut)){
histo_genRemovedMo->Fill(abs(Mo[i]));
histo_genRemovedGrMo->Fill(abs(GrMa[i]));
}
//This selection is applied to all events
if(/*(!(((abs(Mo[i])>0))&&(abs(Mo[i]) < 7)))&&*/(abs(Id[i]) == leptonId)&&(Pt[i]>Pt_cut)&&(status[i]==1)){

//Delta R and variables to match
double R = 999;
double delta_P = 999;
double delta_charge;
double impact_xy;
double impact_z;
double so03;
double so04;

//Loop on reconstructed events to minimise DeltaR
for (int j = 0; j<nrecPart; ++j) {
//Electrons selection
if ((abs(recId[j]) == leptonId)) {

TLorentzVector genV;
TLorentzVector recV;
			
genV.SetPtEtaPhiM(Pt[i],gen_eta[i],gen_phi[i],m[i]);
recV.SetPtEtaPhiM(recPt[j],rec_eta[j],rec_phi[j],recm[j]);

//Minimise DeltaR and Fill the other variables
if (R > genV.DeltaR(recV)) {
R = genV.DeltaR(recV);
delta_P = abs(recPt[j]-Pt[i])/Pt[i];
delta_charge = abs(reccharge[j] - charge[i]);
impact_xy = dxy[j];
impact_z = dz[j];
so03 = relIso03[j];
so04 = relIso04[j];	
}
}
}

//Generated particles whitout matching
//Prompt
if(((abs(Mo[i]) == mother_Id)||((abs(GrMa[i]) == mother_Id)&&(abs(Mo[i]) != 15)))){
	
++nprompt;
cout<<"The concerned event is "<<k<<endl;
cout<<"The charge is "<<charge[i]<<endl;

//Fill the charges for counting
if(abs(charge[i] - 1) < 0.2){++promptleppos;}
if(abs(charge[i] + 1) < 0.2){++promptlepneg;}
histo_genMo_W->Fill(abs(Mo[i]));
histo_genGrMo_W->Fill(abs(GrMa[i]));
histo_genStatus_W->Fill(status[i]);
cout<<"status "<<status[i]<<endl;
}
//Not Prompt

if((!((abs(Mo[i]) == mother_Id)||((abs(GrMa[i]) == mother_Id)&&(abs(Mo[i]) != 15))))){

++nnotprompt;
//Fill the charges for counting
if(abs(charge[i] - 1) < 0.2){++leppos;}
if(abs(charge[i] + 1) < 0.2){++lepneg;}
histo_genGrMo->Fill(abs(GrMa[i]));
histo_genMo->Fill(abs(Mo[i]));
histo_genStatus->Fill(status[i]);
}
//Now using matching
//Prompt leptons
if (((abs(Mo[i]) == mother_Id)||((abs(GrMa[i]) == mother_Id)&&(abs(Mo[i]) != 15)))&&(Pt[i]>Pt_cut)) {
//Fill Pt only if the selection criteria is satified
if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){
histo_dxy->Fill(abs(impact_xy));
histo_dz->Fill(abs(impact_z));
histo_drelIso03->Fill(so03);
histo_drelIso04->Fill(so04);
histo_motherId->Fill(abs(Mo[i]));
histo_GrmotherId->Fill(abs(GrMa[i]));
}
}

//Not prompt leptons
if ((!((abs(Mo[i]) == mother_Id)||((abs(GrMa[i]) == mother_Id)&&(abs(Mo[i]) != 15))))&&(Pt[i]>Pt_cut)) {
//Fill histograms only if the selection criteria is satified
if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){
histo_dxy_noW->Fill(abs(impact_xy));
histo_dz_noW->Fill(abs(impact_z));
histo_drelIso_noW03->Fill(so03);
histo_drelIso_noW04->Fill(so04);
histo_motherId_noW->Fill(abs(Mo[i]));
histo_GrmotherId_noW->Fill(abs(GrMa[i]));
}
}

//Unselected particles (prompt and non-prompt)
if((Pt[i]>Pt_cut)){
//Fill Pt only if the selection criteria is satified
if((R<0.1)&&(delta_P < 0.2)&&(delta_charge < 0.5)){
histo_motherId_unsel->Fill(abs(Mo[i]));
histo_GrmotherId_unsel->Fill(abs(GrMa[i]));
}
}
}
}

//File the histogram for the charges
//cout<<"promptleppos is "<<promptleppos<<endl;
if(promptleppos != 0){histo_genCharge_W->Fill(promptleppos);}
if(promptlepneg != 0){histo_genCharge_W->Fill(-promptlepneg);}
if((promptleppos == promptlepneg)&&(promptleppos != 0)){histo_genPair_W->Fill(promptleppos);}
else if((promptleppos != promptlepneg)&&((promptleppos != 0)||(promptlepneg != 0))){histo_genPair_W->Fill(10);}
if(leppos != 0){histo_genCharge->Fill(leppos);}
if(lepneg != 0){histo_genCharge->Fill(-lepneg);}
if((leppos == lepneg)&&(leppos != 0)){histo_genPair->Fill(leppos);}
else if((leppos != lepneg)&&((leppos != 0)||(lepneg != 0))){histo_genPair->Fill(10);}

}
//Fill the lepton counting
cout<<"Fraction of prompt "+pname<<(double)nprompt/(nprompt + nnotprompt)<<endl;
cout<<"Fraction of not-prompt "+pname<<(double)nnotprompt/(nprompt + nnotprompt)<<endl;

//Start the plots
TCanvas* c1 = new TCanvas("c1","c1",1000,800);
c1->Divide(2,2);
c1->cd(1);
//gPad->SetLogy();
histo_dxy->Scale(1/(histo_dxy->GetEntries()));
histo_dxy->Draw();
histo_dxy->SetTitle("dxy for "+pname+" ");
histo_dxy->SetLineWidth(2);
histo_dxy_noW->Scale(1/histo_dxy_noW->GetEntries());
histo_dxy_noW->Draw("same");
histo_dxy_noW->SetTitle("dxy for "+pname+" ");
histo_dxy_noW->SetLineWidth(2);
histo_dxy_noW->SetLineColor(2);
TLegend* leg = new TLegend(0.25, 0.6, 0.6 , 0.8);
leg->AddEntry(histo_dxy,"from decay W");
leg->AddEntry(histo_dxy_noW, "not from W decay");
leg->Draw();
c1->cd(2);
//gPad->SetLogy();
histo_dz->Scale(1/histo_dz->GetEntries());
histo_dz->Draw();
histo_dz->SetTitle("dz for "+pname+" ");
histo_dz->SetLineWidth(2);
histo_dz_noW->Scale(1/histo_dz_noW->GetEntries());
histo_dz_noW->Draw("same");
histo_dz_noW->SetLineColor(2);
histo_dz_noW->SetLineWidth(2);
c1->cd(3);
//gPad->SetLogy();
histo_drelIso03->Scale(1/histo_drelIso03->GetEntries());
histo_drelIso03->Draw();
histo_drelIso03->SetTitle("relIso03 for "+pname);
histo_drelIso03->SetLineWidth(2);
histo_drelIso_noW03->Scale(1/histo_drelIso_noW03->GetEntries());
histo_drelIso_noW03->Draw("same");
histo_drelIso_noW03->SetLineColor(2);
histo_drelIso_noW03->SetLineWidth(2);
c1->cd(4);
//gPad->SetLogy();
histo_drelIso04->Scale(1/histo_drelIso04->GetEntries());
histo_drelIso04->Draw();
histo_drelIso04->SetTitle("relIso04 for "+pname);
histo_drelIso04->SetLineWidth(2);
histo_drelIso_noW04->Scale(1/histo_drelIso_noW04->GetEntries());
histo_drelIso_noW04->Draw("same");
histo_drelIso_noW04->SetLineColor(2);
histo_drelIso_noW04->SetLineWidth(2);

TCanvas *c2 =new TCanvas("c2","c2",1000,1200);
c2->Divide(2,3);
c2->cd(1);
histo_motherId->Draw();
histo_motherId->SetTitle("Mother Id for "+pname+ " from W decay");
histo_motherId->SetLineWidth(2);
c2->cd(2);
histo_GrmotherId->Draw();
histo_GrmotherId->SetTitle("GrMother Id for "+pname+ " from W decay");
histo_GrmotherId->SetLineWidth(2);
c2->cd(3);
histo_motherId_noW->Draw();
histo_motherId_noW->SetTitle("Mother Id for "+pname+ " not from W decay"); 
histo_motherId_noW->SetLineWidth(2);
c2->cd(4);
histo_GrmotherId_noW->Draw();
histo_GrmotherId_noW->SetTitle("GrMother Id for "+pname+ " not from W decay");
histo_GrmotherId_noW->SetLineWidth(2);
c2->cd(5);
histo_motherId_unsel->Draw();
histo_motherId_unsel->SetTitle("Mother Id for "+pname+ " for all leptons"); 
histo_motherId_unsel->SetLineWidth(2);
c2->cd(6);
histo_GrmotherId_unsel->Draw();
histo_GrmotherId_unsel->SetTitle("GrMother Id for "+pname+ " for all leptons");
histo_GrmotherId_unsel->SetLineWidth(2);
TCanvas* c4 = new TCanvas("c4","c4",1000,1200);
c4->Divide(2,2);
c4->cd(1);
histo_genMo_W->Draw();
histo_genMo_W->SetTitle("Mother for all "+pname+" from W decay (no matching)");
histo_genMo_W->SetLineWidth(2);
c4->cd(2);
histo_genGrMo_W->Draw();
histo_genGrMo_W->SetTitle("GrMother for all "+pname+" from W decay (no matching)");
histo_genGrMo_W->SetLineWidth(2);
c4->cd(3);
histo_genMo->Draw();
histo_genMo->SetTitle("Mother for all "+pname+" not from W (no matching)");
histo_genMo->SetLineWidth(2);
c4->cd(4);
histo_genGrMo->Draw();
histo_genGrMo->SetTitle("GrMother for all "+pname+" not from W (no matching)");
histo_genGrMo->SetLineWidth(2);
TCanvas* c5 = new TCanvas("c5","c5");//,1000,1200);
c5->Divide(1,2);
c5->cd(1);
histo_genStatus_W->Draw();
histo_genStatus_W->SetTitle("Status for gen. prompt "+pname);
histo_genStatus_W->SetLineWidth(2);
c5->cd(2);
histo_genStatus->Draw();
histo_genStatus->SetLineWidth(2);
TCanvas* c6 = new TCanvas("c6","c6");
c6->Divide(1,2);
c6->cd(1);
histo_genRemovedMo->Draw();
histo_genRemovedMo->SetLineWidth(2);
histo_genRemovedMo->SetTitle("Mo. for "+pname+" removed using status == 1");
c6->cd(2);
histo_genRemovedGrMo->Draw();
histo_genRemovedGrMo->SetLineWidth(2);
histo_genRemovedGrMo->SetTitle("GrMo. for "+pname+" removed using status == 1");
TCanvas* c7 = new TCanvas("c7","c7");
c7->Divide(1,2);
c7->cd(1);
histo_genPair_W->Draw();
histo_genCharge_W->Draw("same");
histo_genPair_W->SetLineWidth(2);
histo_genPair_W->SetLineColor(2);
histo_genCharge_W->SetLineWidth(2);
histo_genCharge_W->SetLineStyle(2);
histo_genPair_W->SetTitle("Charge for gen. prompt "+pname);
c7->cd(2);
histo_genPair->Draw();
histo_genPair->SetLineWidth(2);
histo_genPair->SetLineColor(2);
histo_genCharge->Draw("same");
histo_genCharge->SetLineWidth(2);
histo_genCharge->SetLineStyle(2);
histo_genPair->SetTitle("Charge for gen. not-prompt "+pname);
//TCanvas* c8 = new TCanvas("c8","c8");
//c8->cd();
//histo_nnotprompt->Draw();
//histo_nprompt->Draw("same");
//histo_nnotprompt->SetTitle("fraction of prompt and not-prompt "+pname);
//histo_nprompt->SetLineWidth(2);
//histo_nnotprompt->SetLineWidth(2);
//histo_nnotprompt->SetLineColor(2);
//TLegend* leg2 = new TLegend(0.25, 0.6, 0.6 , 0.8);
//leg2->AddEntry(histo_nprompt, "Fraction of prompt "+pname);
//leg2->AddEntry(histo_nnotprompt, "Fraction of not-prompt "+pname);
//leg2->Draw();



//Giving the number of particles
ReadHistoPDGId(histo_genMo_W);
ReadHistoPDGId(histo_genGrMo_W);
ReadHistoPDGId(histo_genMo);
ReadHistoPDGId(histo_genGrMo);


}






