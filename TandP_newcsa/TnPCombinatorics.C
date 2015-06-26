/*********************************************
 * Description - This function takes a Tag object (define below) and arrange the Probe it contains according to the combinatorics in argument
 * Author - Gaël L. Perrin
 * Date - Feb 06 2015
 * *******************************************/

using namespace std;

#include "cmath"
#include "TChain.h"
#include <sys/stat.h>
#include "iostream"
#include "fstream"
#include "vector"
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
#include <map>
#include <algorithm>

//Helper functions
#include "../tools/InvMass.C"
#include "../tools/DeltaR.C"
#include "../tools/setTDRStyle.C"
#include "../tools/getmaxallhist.C"


struct Probe{

  //indices of the event
  int I;
  //kinematic variables
  Double_t PT;
  Double_t ETA;
  Double_t PHI;
  Double_t M;
  //other variables
  Int_t ID;
  Int_t Q;
  //selection variables
  Int_t         TIGHT;
  Int_t         LOOSE;
  Double_t      RELISO3;
  Double_t      RELISO4;
  Double_t      DXY;
  Double_t      DZ;
  Int_t         FROMTAU;
  Int_t         MATCHED;

};

struct Tag{

  vector< Probe > PROBE;

  //indices of the event
  int I;
  //kinematic variables
  Double_t PT;
  Double_t ETA;
  Double_t PHI;
  Double_t M;
  //other variables
  Int_t ID;
  Int_t Q;
  //selection variables
  Int_t         TIGHT;
  Int_t         LOOSE;
  Double_t      RELISO3;
  Double_t      RELISO4;
  Double_t      DXY;
  Double_t      DZ;
  Int_t         FROMTAU;
  Int_t         MATCHED;

};

//Function to reorder the vector
bool mycompare(pair< Probe, Double_t> a, pair< Probe, Double_t > b){
  return(abs(a.second - 91.1876) < abs(b.second - 91.1876) );
}

Tag combine(Tag tag, TString option){

  //Define return TAG
  Tag rtag = tag;

  //Define prob vector
  vector <Probe> probe = tag.PROBE;

  /////////////////////////////////////
  //Define vector to do combinatorics//
  /////////////////////////////////////
  //Closest ZM
  vector <pair <Probe, double> > M;
  //Take all loose 
  vector <Probe > _probe;
  //Take loose probes only
  vector <Probe > _looseprobe;

  for(int p = 0; p < probe.size(); ++p){

    pair <Probe, double> pair_M;

    //only match different leptons
    if(probe[p].I == tag.I) continue;

    //Fill the combinatorics vectors
    pair_M.first = probe[p];
    pair_M.second = InvMass(probe[p].PT,probe[p].ETA,probe[p].PHI,probe[p].M,tag.PT,tag.ETA,tag.PHI,tag.M);
    M.push_back(pair_M);
    
    _probe.push_back(probe[p]);

    if(probe[p].LOOSE == 1) _looseprobe.push_back(probe[p]);

  }

  if(option.Contains(" closestZM ")){
    sort(M.begin(),M.end(),mycompare);
    rtag.PROBE.clear();
    rtag.PROBE.push_back(M[0].first);
  }else if(option.Contains(" looseprobe ")){
    rtag.PROBE.clear();
    rtag.PROBE = _looseprobe;
  }else{
    rtag.PROBE.clear();
    rtag.PROBE = _probe;
  }

  return rtag;

}
