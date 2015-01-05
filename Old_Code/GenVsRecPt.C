#include "TChain.h"
#include "cmath"
#include "iostream"
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
#include "efficiency.C"

void GenVsRecPt()
{
    
//Read root file
bool all_data = false;
TChain* tree = new TChain("treeProducerSusyFullHad");
if (all_data == true){tree->Add("/Users/GLP/Desktop/CERN_data/treeProducerSusyFullHad_tree_*.root");}
else if (all_data == false) {
tree->Add("/Users/GLP/Desktop/CERN_data/treeProducerSusyFullHad_tree_0.root");
tree->Add("/Users/GLP/Desktop/CERN_data/treeProducerSusyFullHad_tree_1.root");
tree->Add("/Users/GLP/Desktop/CERN_data/treeProducerSusyFullHad_tree_11.root");
tree->Add("/Users/GLP/Desktop/CERN_data/treeProducerSusyFullHad_tree_12.root");
}

TH1D* histo1 = new TH1D("histo1","histo1",200,0,200);
TH1D* histo2 = new TH1D("histo2","histo2",200,0,200);

tree->Draw("genPart_pt >> histo1","(abs(genPart_pdgId) == 11)&&(genPart_pt > 10)&&(abs(genPart_eta) < 2.49)&&(abs(genPart_motherId)||((abs(genPart_grandmaId) == 24)&&((genPart_pdgId) != 15)))&&(genPart_status ==1)&&(genPart_pt > 0)");
tree->Draw("lep_pt >> histo2","(abs(lep_pdgId) == 11)&&(lep_pt > 0)");    

TCanvas* c1 = new TCanvas("c1","c1");

histo1->Draw();
histo1->SetTitle("P_{t} distribution for gen and rec electrons");
histo1->GetXaxis()->SetTitle("P_{t} [GeV]");
histo2->Draw("same");
histo2->SetLineColor(2);

TLegend* leg1 = new TLegend(0.25, 0.6, 0.6 , 0.8);
leg1->AddEntry(histo1, "P_{t} of gen. electrons");
leg1->AddEntry(histo2, "P_{t} of rec. electrons");
leg1->Draw();

    
}
