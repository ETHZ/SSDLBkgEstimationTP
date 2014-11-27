/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.  * File Name : ZTTbarRead.C * Purpose : This files read the Z trees together with other events, like ttbar or QCD jets.  * Creation Date : 20-12-2008 * Last Modified : Sat 20 Dec 2008 09:37:30 AM PST * Created By : _._._._._._._._._._._._._._._._._._._._._.*/ 
#include "cmath"
#include "TChain.h"
#include <sys/stat.h>
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
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TLorentzVector.h"
#include "FitInvMass.C"
#include "setTDRStyle.C"
#include "setfreestyle.C"
#include "InvMass.C"
#include "DeltaR.C"
#include "vector"

#include "DrawInvMassBkg.C"
#include "DrawInvMassBkg_v2.C"

void ZTTbarRead(){


	//Set this bool to true is want to work with all data. Set to false if only with one tree
	bool all_data = true;

	//Reading the tree containing the Z events
	TChain* tree = new TChain("treeProducerSusySSDL");

	if(all_data == false){tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/DYJetsM50*100to200*.root");}
	else if (all_data == true){tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/DYJetsM50*.root");}
	
	//tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/DYJetsToLLM50_PU_S14_POSTLS170.root");
	
	if(all_data == false){tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/WJets*100to200*.root");}
	else if (all_data == true){tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/WJets*.root");}

	tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/TTJets_MS*.root"); 

	//if(all_data == false){tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/QCD*5to*.root");} 
	//else if (all_data == true){tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/QCD*.root");}

	//DrawInvMassBkg(tree,13,23,35,40,0.2,3,false);
	DrawInvMassBkg_v2(tree,13,23,10.,50,8,"tightId");

}
