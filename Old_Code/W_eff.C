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
//#include "efficiency.C"
#include "eff_plots.C"
#include "LeptonAnalysis.C"

void W_eff()
{

	//Read root file
	bool all_data = true;
	TChain* tree = new TChain("treeProducerSusyFullHad");
	if (all_data == true){tree->Add("/Users/GLP/Desktop/CERN_data/treeProducerSusyFullHad_tree_*.root");}
	else if (all_data == false) {

		tree->Add("/Users/GLP/Desktop/CERN_data/treeProducerSusyFullHad_tree_0.root");
		tree->Add("/Users/GLP/Desktop/CERN_data/treeProducerSusyFullHad_tree_1.root");
		tree->Add("/Users/GLP/Desktop/CERN_data/treeProducerSusyFullHad_tree_11.root");
		tree->Add("/Users/GLP/Desktop/CERN_data/treeProducerSusyFullHad_tree_12.root");
	}

	//efficiency(tree, 11, 24);
	//LeptonAnalysis(tree,11,24);
	eff_plots(tree, 13, 24);

}
