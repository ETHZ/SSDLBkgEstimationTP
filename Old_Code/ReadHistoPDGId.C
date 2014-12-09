#include <iostream>
#include "TString.h"
#include "TH1.h"

void ReadHistoPDGId(TH1D* histo){

Int_t nbins= histo->GetSize()-2;
Int_t nentries = histo->GetEntries();

int n0 = 0;
int nquarks = 0;
int nelectron = 0;
int nmuon = 0;
int ntau = 0;
int ngluon = 0;
int nW = 0;
int npi0 = 0;
int neta = 0;
int ndiquark = 0;
int nsmeson = 0;
int ncmeson = 0;
int nbmeson = 0;
int ncbaryon = 0;
int nbbaryon = 0;

for(Int_t bins = 1; bins <= nbins; ++ bins){

int content = histo->GetBinContent(bins);

TString pname;

if(content > 0){

bins = bins -1;
if (bins == 0){pname = "0";n0 += content;}
else if((bins >0)&&(bins < 7)){pname = "quarks";nquarks += content;}
else if (bins == 11){pname = "electron";nelectron += content;}
else if (bins == 13){pname = "muon";nmuon += content;}
else if(bins == 15){pname = "tau";ntau += content;}
else if(bins == 21){pname = "gluons";ngluon += content;}
else if (bins == 24){pname = "W";nW += content;} 
else if (bins == 111){pname = "pi0";npi0 += content;}
else if (bins == 221){pname = "eta";neta += content;}
else if ((bins == 2101)||(bins == 2203)||(bins == 3101)||(bins == 1103)||(bins == 3103)||(bins == 3201)||(bins==3303)){pname = "diquarks";ndiquark += content;}
else if((bins > 300)&&(bins < 400)){pname = "s meson";nsmeson += content;}
else if(((bins > 400)&&(bins < 500))||((bins > 10400)&&(bins < 10500))){pname = "c meson";ncmeson += content;}
else if((bins > 500)&&(bins < 600)){pname = "b meson";nbmeson += content;}
else if((bins > 4000)&&(bins < 5000)){pname = "c baryons";ncbaryon += content;}
else if((bins > 5000)&&(bins < 6000)){pname = "b baryons";nbbaryon += content;}
else{pname = "Something else";}

double frac = ((double)content)/(nentries);

cout<<"PDG Id no "<<bins<< " Particle: "+pname<<" | Number of particles "<<frac<<endl;

++bins;

}
}
cout<<" "<<endl;
cout<<"n0 = "<< ((double) n0)/nentries<<endl;
cout<<"nelectron = "<< ((double) nelectron)/nentries<<endl;
cout<<"nquarks = "<< ((double) nquarks)/nentries<<endl;
cout<<"nmuon = "<< ((double) nmuon)/nentries<<endl;
cout<<"ntau = "<< ((double) ntau)/nentries<<endl;
cout<<"ngluon = "<< ((double) ngluon)/nentries<<endl;
cout<<"nW  = "<< ((double) nW)/nentries<<endl;
cout<<"npi0 = "<< ((double) npi0)/nentries<<endl;
cout<<"neta = "<< ((double) neta)/nentries<<endl;
cout<<"ndiquark = "<< ((double) ndiquark)/nentries<<endl;
cout<<"nsmeson = "<< ((double) nsmeson)/nentries<<endl;
cout<<"ncmeson = "<< ((double) ncmeson)/nentries<<endl;
cout<<"nbmeson = "<< ((double) nbmeson)/nentries<<endl;
cout<<"ncbaryon = "<< ((double) ncbaryon)/nentries<<endl;
cout<<"nbbaryon = "<< ((double) nbbaryon)/nentries<<endl;
cout<<"total = "<< ((double)(n0 + nelectron + nquarks + nmuon + ntau + ngluon + nW + npi0 + neta + ndiquark + nsmeson + ncmeson + nbmeson + ncbaryon + nbbaryon))/nentries<<endl;
cout<<" "<<endl;
		       	
}
