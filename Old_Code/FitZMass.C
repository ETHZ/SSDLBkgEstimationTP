/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
 
 * File Name : FitZMass.C
 
 * Purpose : This file is used to test some fit on the invariant mass spectra.
 
 * Creation Date : Old
 
 * Last Modified :
 
 * Created By : Gaël L. Perrin
 
 _._._._._._._._._._._._._._._._._._._._._.*/

{
    using namespace RooFit;
    gROOT->ProcessLine(".x ~/rootlogon.C");

    //Read .root file
    TFile* file = new TFile("/Users/GLP/Dropbox/Physique/Master_Thesis/analysis/plots_root/Z_InvMass.root"); 



    TH1D* histo[4];

    histo[0] = (TH1D*)file->Get("histo_GenInvMass_e");
    histo[1] = (TH1D*)file->Get("histo_RecInvMass_e");
    histo[2] = (TH1D*)file->Get("histo_GenInvMass_mu");
    histo[3] = (TH1D*)file->Get("histo_RecInvMass_mu");

    TString title[4];

    title[0] = "Z mass for gen. e ";
    title[1] = "Z mass for rec. e ";
    title[2] = "Z mass for gen.  #mu ";
    title[3] = "Z mass for rec.  #mu ";

    int ndf;

    RooPlot* frame[4];
    
for(int i = 0; i < 4; ++i){

    double hmin0 = histo[i]->GetXaxis()->GetXmin();
    double hmax0 = histo[i]->GetXaxis()->GetXmax();

    //hmin0 = 81;
    //hmax0 = 101;
    
    histo[i]->GetXaxis()->SetRangeUser(hmin0,hmax0);

    // Declare observable x
    RooRealVar x("x","x",hmin0,hmax0) ;
    RooDataHist dh("dh","dh",x,Import(*histo[i])) ;
    
    frame[i] = x.frame(Title(title[i])) ;
    dh.plotOn(frame[i],DataError(RooAbsData::SumW2), MarkerColor(1),MarkerSize(0.9),MarkerStyle(7));  //this will show histogram data points on canvas
    dh.statOn(frame[i]);  //this will display hist stat on canvas

    RooRealVar mean("mean","mean",histo[i]->GetMean(), 70.0, 120.0);
    RooRealVar width("width","width",5, 0.0, 120.0);
    RooRealVar sigma("sigma","sigma",2, 0.0, 120.0);
    RooRealVar A("A","A",90, 0.0, 120.0);
    RooRealVar N("N","N",2, 0.0, 11);


    mean.setRange(88,94);
    width.setRange(0,5);
    sigma.setRange(0,1);
    A.setRange(80,100);
    N.setRange(0,5);

    //width.setConstant(kTRUE);
    //sigma.setConstant(kTRUE);

    //Choose the fitting here
    //RooGaussian fit_fct("fit_fct","fit_fct",x,mean,sigma);ndf = 2;
    //RooBreitWigner fit_fct("fit_fct","fit_fct",x,mean,width);ndf = 2;
    RooCBShape fit_fct("fit_fct","fit_fct",x,mean,sigma,A,N);ndf = 4;
    //RooVoigtian fit_fct("fit_fct","fit_fct",x,mean,width,sigma); ndf = 3;
    
    RooFitResult* filters = fit_fct.fitTo(dh,Range(80,100),"qr");
    fit_fct.plotOn(frame[i],LineColor(4));//this will show fit overlay on canvas
    fit_fct.paramOn(frame[i]); //this will display the fit parameters on canvas
    //filters->Print("v");
    

}

    TCanvas* c1 = new TCanvas("c1","ZmassHisto e",1200,800) ;
    
    c1->Divide(1,2);

    // Draw all frames on a canvas

    c1->cd(1) ; gPad->SetLeftMargin(0.15);
    
    frame[0]->GetXaxis()->SetTitle("Z mass (in GeV/c^{2})");  
    frame[0]->GetXaxis()->SetTitleOffset(1.2);
    float binsize = histo[0]->GetBinWidth(1); 
    frame[0]->Draw() ;
    cout<<"The chi2 is:"<<endl;
    cout<<frame[0]->chiSquare(ndf)<<endl; 
    cout<<" "<<endl;
    
    
    c1->cd(2); 

    frame[1]->GetXaxis()->SetTitle("Z mass (in GeV/c^{2})");  
    frame[1]->GetXaxis()->SetTitleOffset(1.2);
    float binsize = histo[2]->GetBinWidth(1);
    frame[1]->Draw() ;
    cout<<"The chi2 is:"<<endl;
    cout<<frame[1]->chiSquare(ndf)<<endl; 
    cout<<" "<<endl;

    TCanvas* c2 = new TCanvas("c2","ZmassHisto #mu",1200,800) ;
    
    c2->Divide(1,2);

    c2->cd(1) ; gPad->SetLeftMargin(0.15);
    
    frame[2]->GetXaxis()->SetTitle("Z mass (in GeV/c^{2})");  
    frame[2]->GetXaxis()->SetTitleOffset(1.2);
    float binsize = histo[2]->GetBinWidth(1); 
    frame[2]->Draw() ;
    cout<<"The chi2 is:"<<endl;
    cout<<frame[2]->chiSquare(ndf)<<endl; 
    cout<<" "<<endl;
    
    c2->cd(2); 

    frame[3]->GetXaxis()->SetTitle("Z mass (in GeV/c^{2})");  
    frame[3]->GetXaxis()->SetTitleOffset(1.2);
    float binsize = histo[3]->GetBinWidth(1);
    frame[3]->Draw() ;
    cout<<"The chi2 is:"<<endl;
    cout<<frame[3]->chiSquare(ndf)<<endl; 
    cout<<" "<<endl;
}
