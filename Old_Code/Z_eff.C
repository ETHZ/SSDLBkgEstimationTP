#include "cmath" 
#include "TChain.h" 
#include "iostream" 
#include "TFile.h" 
#include "TTree.h" 
#include "TH1.h"
#include "TBranch.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TString.h"
#include "TLegend.h"
#include "TLorentzVector.h"
#include "DeltaR.C"
//#include "efficiency.C"
#include "eff_plots.C"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TandP.C"
#include "LeptonAnalysis.C"


void Z_eff()
{

	//Read Tree
	bool all_data = true;
	TChain* tree = new TChain("treeProducerSusyFullHad");
	if (all_data == true){tree->Add("/Users/GLP/Desktop/CERN_data/DYJetsM50_HT200to400_PU_S14_POSTLS170_babytree_QG_1.root");}

	//efficiency(tree, 11, 23,0);
	//eff_plots(tree,11,23,0);
	TandP(tree, 11,23,0,0.12);
	

}
