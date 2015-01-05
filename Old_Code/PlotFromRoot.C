#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"
#include <string>
#include <functional>
#include <istream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <TROOT.h>
#include <vector>
#include <sstream>
#include <TH1F.h>
#include <TFile.h>
#include <TCanvas.h>

void PlotFromRoot() {

TFile *f = new TFile("Z_mll.root","read");
TH1D* histo;
TCanvas *c1 = (TCanvas*)f->Get("c1");
c1->GetListOfPrimitives()->ls();
histo = (TH1D*)c1->FindObject("histo_RecInvMass");

TCanvas* c10 = new TCanvas("c10","c10");

histo->Draw();


}
