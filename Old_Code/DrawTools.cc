#include "DrawTools.h"



TPaveText* DrawTools::getLabelTop( int beamEnergy ) {

  return getLabelTop(Form("%d MeV Electron Beam", beamEnergy));

}


TPaveText* DrawTools::getLabelTop( const std::string& text ) {

  TPaveText* label_top = new TPaveText(0.4,0.900,0.900,0.900, "brNDC");//(0.4,0.953,0.975,0.975, "brNDC");
  label_top->SetFillColor(kWhite);
  label_top->SetTextSize(0.038);
  label_top->SetTextAlign(31); // align right
  label_top->SetTextFont(62);
  label_top->AddText(text.c_str());

  return label_top;

}

TPaveText* DrawTools::getLabelTop_2D( int beamEnergy ) {

  return getLabelTop_2D(Form("%d MeV Electron Beam", beamEnergy));

}


TPaveText* DrawTools::getLabelTop_2D( const std::string& text ) {

  TPaveText* label_top = new TPaveText(0.4,0.953,0.890,0.975, "brNDC");
  label_top->SetFillColor(kWhite);
  label_top->SetTextSize(0.038);
  label_top->SetTextAlign(31); // align right
  label_top->SetTextFont(62);
  label_top->AddText(text.c_str());

  return label_top;

}


TPaveText* DrawTools::getLabelRun( const std::string& runName, bool top ) {

  float yMin = (top) ? 0.86 : 0.18;
  float yMax = (top) ? 0.91 : 0.23;
  TPaveText* label_run = new TPaveText(0.4,yMin,0.95,yMax, "brNDC");
  label_run->SetFillColor(kWhite);
  label_run->SetTextSize(0.03);
  label_run->SetTextAlign(11); // align right
  label_run->SetTextFont(42);
  label_run->AddText(Form("Run %s", runName.c_str()));

  return label_run;

}


TPaveText* DrawTools::getCef3LabelLeft() {

  float xMin = 0.22;
  float yMin = 0.92;
  float xMax = 0.30;
  float yMax = 0.8;
  TPaveText* label_cef3 = new TPaveText(xMin,yMin,xMax,yMax, "brNDC");
  label_cef3->SetFillColor(kWhite);
  label_cef3->SetTextSize(0.035);
  label_cef3->SetTextAlign(11); // align right
  label_cef3->SetTextFont(62);
  label_cef3->AddText("W-CeF_{3} Single Tower");

  return label_cef3;
}


TText* DrawTools::getAuthorsLabel() {
  TText *l1 = new TText(12.8,1.0,"P. Meridiani et al., to be publ. in Proc. IEEE NSS 2014");
  l1->SetTextSize(0.035);
  l1->SetTextFont(42);
  l1->SetTextAngle(90);

  return l1;
}



TStyle* DrawTools::setStyle() {

  // set the TStyle
  TStyle* style = new TStyle("DrawBaseStyle", "");
  style->SetCanvasColor(0);
  style->SetPadColor(0);
  style->SetFrameFillColor(0);
  style->SetStatColor(0);
  style->SetOptStat(0);
  style->SetTitleFillColor(0);
  style->SetCanvasBorderMode(0);
  style->SetPadBorderMode(0);
  style->SetFrameBorderMode(0);
  style->SetPadBottomMargin(0.12);
  style->SetPadLeftMargin(0.12);
  style->cd();

  // For the canvas:
  style->SetCanvasBorderMode(0);
  style->SetCanvasColor(kWhite);
  style->SetCanvasDefH(600); //Height of canvas
  style->SetCanvasDefW(600); //Width of canvas
  style->SetCanvasDefX(0); //POsition on screen
  style->SetCanvasDefY(0);

  // For the Pad:
  style->SetPadBorderMode(0);
  style->SetPadColor(kWhite);
  style->SetPadGridX(false);
  style->SetPadGridY(false);
  style->SetGridColor(0);
  style->SetGridStyle(3);
  style->SetGridWidth(1);

  // For the frame:
  style->SetFrameBorderMode(0);
  style->SetFrameBorderSize(1);
  style->SetFrameFillColor(0);
  style->SetFrameFillStyle(0);
  style->SetFrameLineColor(1);
  style->SetFrameLineStyle(1);
  style->SetFrameLineWidth(1);


  // Margins:
  style->SetPadTopMargin(0.05);
  style->SetPadBottomMargin(0.15);//0.13);
  style->SetPadLeftMargin(0.15);//0.16);
  style->SetPadRightMargin(0.05);//0.02);

  // For the Global title:

  style->SetOptTitle(0);
  style->SetTitleFont(42);
  style->SetTitleColor(1);
  style->SetTitleTextColor(1);
  style->SetTitleFillColor(10);
  style->SetTitleFontSize(0.05);

  // For the axis titles:

  style->SetTitleColor(1, "XYZ");
  style->SetTitleFont(42, "XYZ");
  style->SetTitleSize(0.05, "XYZ");
  style->SetTitleXOffset(1.15);//0.9);
  style->SetTitleYOffset(1.3); // => 1.15 if exponents

  // For the axis labels:

  style->SetLabelColor(1, "XYZ");
  style->SetLabelFont(42, "XYZ");
  style->SetLabelOffset(0.007, "XYZ");
  style->SetLabelSize(0.045, "XYZ");

  // For the axis:

  style->SetAxisColor(1, "XYZ");
  style->SetStripDecimals(kTRUE);
  style->SetTickLength(0.03, "XYZ");
  style->SetNdivisions(510, "XYZ");
  style->SetPadTickX(1); // To get tick marks on the opposite side of the frame
  style->SetPadTickY(1);
  style->cd();

//  const Int_t NRGBs = 2;
//  const Int_t NCont = 300;
//
//  Double_t stops[NRGBs] = { 0.00, 1.00};
//  Double_t red[NRGBs]   = { 1.00, 0.1};
//  Double_t green[NRGBs]   = { 0.00, 0.00};
//  Double_t blue[NRGBs]   = { 0.00, 0.00};

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Float_t r[5];
  Float_t g[5];
  Float_t b[5];
  TColor* c[5];

  int Colors[5]={kBlack,kBlue+2,kBlue-3,kBlue-4,kWhite};

  for(int i=0;i<5;++i){
    c[i]=gROOT->GetColor(Colors[i]);
    c[i]->GetRGB(r[i],g[i],b[i]);
  }


  //  Double_t stops[NRGBs] = { 0.00, 0.40, 0.50, 0.80, 1.00}; original
  Double_t stops[NRGBs] = { 0.00, 0.15, 0.50, 0.80, 1.00};
  Double_t red[NRGBs]   = {r[4],r[3],r[2],r[1],r[0] };
  Double_t green[NRGBs]   = {g[4],g[3],g[2],g[1],g[0] };
  Double_t blue[NRGBs]   = {b[4],b[3],b[2],b[1],b[0] };

  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  style->SetNumberContours(NCont);



  return style;

}

