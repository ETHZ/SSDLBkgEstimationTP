#include <sstream>
#include <fstream>
#include <cmath>
#include <cstdlib>

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"

using namespace std;


int postProcessing(string inputFile="treeSmall1.root",
		   string outputFile="output.root",
		   string treeName="treeProducerSusyMultilepton",
		   float filter=1.0, float kfactor=1.0, float xsec=1.0,int id=-1);


int run(string mData="mData.txt",
	string treeName="treeProducerSusySSDL"){
  
  cout<<"File Name = "<<mData.c_str()<<endl;

  ifstream meta(mData.c_str());
  string line;

  while(std::getline(meta,line)){
    istringstream ss(line);
    if((ss.str()[0])==(string)"#") continue;
    string input,outputS,filterS,kfactorS,xsecS,idS; 

    ss >> input;
    ss >> outputS;
    ss >> filterS;
    ss >> kfactorS;
    ss >> xsecS;
    ss >> idS;

    float filter,kfactor,xsec;
    filter = (float)atof(filterS.c_str());
    kfactor = (float)atof(kfactorS.c_str());
    xsec = (float)atof(xsecS.c_str());
    int id = (int)atoi(idS.c_str());
    
    cout << "input,output,f,k,x: " 
	 << input << " , " 
	 << outputS << " , " 
	 << filter << " , " 
	 << kfactor << " , " 
	 << xsec << " , "
	 << id << endl;

    postProcessing(input, outputS, treeName, filter, kfactor, xsec,id);
  }

  return 0;
}

int postProcessing(string inputFile,
		   string outputFile,
		   string treeName,
		   float filter, float kfactor, float xsec,int id){

  //TString infname, TString outfile, unsigned int events, 
  //Float_t xsec, Float_t kfactor,
  //Float_t filt_eff, bool SortBasketsByEntry ) {
  
  cout << "Processing File " << inputFile << endl;

  TFile *f = TFile::Open(inputFile.c_str(), "READ");
  if (! f || f->IsZombie()) {
    cout << "File does not exist!" << endl;
    return 1;
  }
  
  TTree* t = (TTree*)f->Get(treeName.c_str());
  if (! t || t->IsZombie()) {
    cout << "Tree does not exist!" << endl;
    return 2;
  }
        
  
  // This line should be uncommented for all the branches that we want to overwrite.
  // If the branch is not in the input tree, we don't need this.
  //
  //t->SetBranchStatus("scale1fb", 0);

  TFile *out = TFile::Open(outputFile.c_str(), "RECREATE","",0);
  TTree *clone;

  clone = t->CloneTree(-1,"fast");
  //clone = t->CloneTree(400000);

  cout << "Cloning is DONE!!!" << endl;

  /*
  if(SortBasketsByEntry)
    clone = t->CloneTree(-1, "fastSortBasketsByEntry");
  else 
    clone = t->CloneTree(-1, "fast");
  */

  //-------------------------------------------------------------

  //Calculate scaling factor and put variables into tree 
  int events = t->GetEntries();
  float scale1fb = xsec*kfactor*1000*filter/(Float_t)events;
  //Float_t scale1fb = 6789;


  /*
  if(isdata){
	scale1fb = 1.0;
	cout<< "Data file. scale1fb: " << scale1fb << endl;
	
  }else{
	cout << "scale1fb: " << scale1fb << endl; 
  }
  */

  TBranch* b1 = clone->Branch("evt_scale1fb", &scale1fb, "evt_scale1fb/F");
  TBranch* b2 = clone->Branch("evt_xsec", &xsec, "evt_xsec/F");
  TBranch* b3 = clone->Branch("evt_kfactor", &kfactor, "evt_kfactor/F");
  TBranch* b4 = clone->Branch("evt_filter", &filter, "evt_filter/F");
  TBranch* b5 = clone->Branch("evt_nEvts", &events, "evt_nEvts/I");
  TBranch* b6 = clone->Branch("evt_id", &id, "evt_id/I");
   

  Int_t nentries = clone->GetEntries();
  cout << "nentries: " << nentries << endl;
  for(Int_t i = 0; i < nentries; i++) {
    if(i%10000==0) cout << "analyzing event: " << i << endl;
    b1->Fill();
    b2->Fill();
    b3->Fill();
    b4->Fill();
    b5->Fill();
    b6->Fill();
  }
  //-------------------------------------------------------------

  clone->SetName("treeProducerSusyMultilepton");
  clone->Write(); 
  out->Close();
  f->Close();
  return 0;
  
}






