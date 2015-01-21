/*********************************************
* Description - Sometimes there are more than one histogram per plot. This funcitons takes all the histograms into account and return the higest value in order to set the range of the plot 
* Author - Gaël L. Perrin
* Date - Jan 18 2015
* *******************************************/

#ifndef GETMAXALLHIST 
#define GETMAXALLHIST 

#include <iostream>
#include <algorithm>
#include <vector>
#include "TH1.h"

double getmaxallhist(TH1* h1, TH1* h2, TH1* h3){

  double h1max = h1->GetBinContent(h1->GetMaximumBin());
  double h2max = h2->GetBinContent(h2->GetMaximumBin());
  double h3max = h3->GetBinContent(h3->GetMaximumBin());

vector <double> hmax;
hmax.push_back(h1max);
hmax.push_back(h2max);
hmax.push_back(h3max);

return (*max_element(hmax.begin(),hmax.end()))*1.1;

}

double getmaxallhist(TH1* h1, TH1* h2){

  double h1max = h1->GetBinContent(h1->GetMaximumBin());
  double h2max = h2->GetBinContent(h2->GetMaximumBin());

vector <double> hmax;
hmax.push_back(h1max);
hmax.push_back(h2max);

return (*max_element(hmax.begin(),hmax.end()))*1.1;

}

#endif
