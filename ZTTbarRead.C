/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.  * 
 
 File Name : ZTTbarRead.C * Purpose : This files read the Z trees together with other events, like ttbar or QCD
 jets, and store them into a tree. Other funcitons can then be called to run over those trees.
 
 * Creation Date : 20-12-2008
 
 * Last Modified : Sat 20 Dec 2008 09:37:30 AM PST
 
 * Created By : Gaël L. Perrin
_._._._._._._._._._._._._._._._._._._._._.*/

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
#include "FitInvMass.C"
#include "setTDRStyle.C"
#include "setfreestyle.C"
#include "InvMass.C"
#include "DeltaR.C"
#include "vector"

//#include "DrawInvMassBkg.C"
//#include "DrawInvMassBkg_v2.C"
#include "Efficiency.C"
#include "CutStudy.C"
#include "Efficiency_v2.C"

void ZTTbarRead(){

	//Reading the tree 
	TChain* tree = new TChain("treeProducerSusySSDL");

	//DY events
	//From 100 to Inf HT
	//tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/DYJetsM50*.root");
	//Inclusive
	tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/DYJetsToLLM50_PU_S14_POSTLS170.root");
	
	//WJet events
 	//tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/WJets*.root");

	//TTJets events
	//tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/TTJets_MS*.root"); 

	//DrawInvMassBkg(tree,13,23,35,40,0.2,3,false);
	//DrawInvMassBkg_v2(tree,13,23,10.,50,8,"tightId");
	
	Efficiency(tree,11,23,"tightId","reliso3","Pt",0.,"0.02");
	
	//CutStudy(tree,11,23,"dz","tightId",0.005);

	
	//Do all the efficiency plots using Efficiency_v2

	//TString tightId = "tightId";
	//TString nosell = "";
	//TString dz= "dz";
	//TString dxy= "dxy";
	//TString Pt= "Pt";
	//TString eta= "eta";
	//TString phi= "phi";

	//int leptonId[42] = {11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13};
	//int motherId[42] = {23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23};
	//TString sel_den[42] = {nosell,nosell,nosell,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,nosell,nosell,nosell,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId,tightId};
	//TString sel_num[42] = {tightId,tightId,tightId,dz,dz,dz,dz,dz,dz,dz,dz,dz,dxy,dxy,dxy,dxy,dxy,dxy,dxy,dxy,dxy,tightId,tightId,tightId,dz,dz,dz,dz,dz,dz,dz,dz,dz,dxy,dxy,dxy,dxy,dxy,dxy,dxy,dxy,dxy};

	//TString axis[42] = {Pt,eta,phi,Pt,eta,phi,Pt,eta,phi,Pt,eta,phi,Pt,eta,phi,Pt,eta,phi,Pt,eta,phi,Pt,eta,phi,Pt,eta,phi,Pt,eta,phi,Pt,eta,phi,Pt,eta,phi,Pt,eta,phi,Pt,eta,phi};
	//double cut_den[42] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	//double cut_nom[42] = {0.,0.,0.,0.02,0.02,0.02,0.01,0.01,0.01,0.005,0.005,0.005,0.02,0.02,0.02,0.01,0.01,0.01,0.005,0.005,0.005,0.,0.,0.,0.02,0.02,0.02,0.01,0.01,0.01,0.005,0.005,0.005,0.02,0.02,0.02,0.01,0.01,0.01,0.005,0.005,0.005};

	//Efficiency_v2(tree,42,leptonId,motherId,sel_den,sel_num,axis,cut_den,cut_nom);
}

