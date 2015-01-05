//Side function to read .txt files

#include <sstream>
#include <fstream>
#include <cmath>
#include <cstdlib>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"

using namespace std;

vector<string> readfile(string mData="InvMass.txt"){
  
  cout<<"File Name = "<<mData.c_str()<<endl;
  
  vector<string> vec;
  ifstream meta(mData.c_str());
  string line;

  while(std::getline(meta,line)){
    istringstream ss(line);
    if((ss.str()[0])==(string)"#") continue;
    string input,input2; 

    ss >> input;
    ss >> input2;

    cout << "input: " 
	 << input  << " , "  
	 << input2 <<endl;  
    vec.push_back(input);
    vec.push_back(input2);


  }

  return vec;
}
  







