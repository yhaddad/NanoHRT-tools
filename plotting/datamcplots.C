#include "setTDRStyle.h"
#include <sstream>
#include <fstream>
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include <vector>
#include "Rtypes.h"
#include "TColor.h"
#include "TVectorF.h"
#include <cstdlib>

void setTDRStyle();
TH1F *create1Dhisto(TString sample,TTree *tree,TString intLumi,TString cuts,TString branch,int bins,float xmin,float xmax,
		    bool useLog,int color, int style,TString name,bool norm,bool data);
TH2F *create2Dhisto(TString name,TTree *tree,TString intLumi,TString cuts,TString xbranch,int xbins,float xmin,float xmax,TString ybranch,int ybins,float ymin,float ymax,bool data);
void makePlotDataMC(TString name, TString dir, 
		    std::vector<TTree*> trees, TString lumi,TString cut, TString addcut,
		    TString var,int nbins,float xmin,float xmax,TString xaxisname,
		    std::vector<TString> legends, std::vector<int> colors, bool logy, int norm=0);


void datamcplots(TString sample) {

  TH1::SetDefaultSumw2(kTRUE);
  setTDRStyle();
  gROOT->SetBatch(false);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  gStyle->SetPalette(1);

  float intLumi = 36.8;
  ostringstream tmpLumi;
  tmpLumi << intLumi;
  TString lumi = tmpLumi.str();

  std::vector<TTree*>   trees;   trees.clear();
  std::vector<TString> legends; legends.clear();
  std::vector<int>     colors;  colors.clear();

  TString cut_ak8, cut_ca15;

  if (sample == "photons") {
    TString path = "root://cmseos.fnal.gov//store//group/lpcjme/noreplica/NanoHRT/Trees/Jan25/photon/";

    //data: always read the data tree first 
    TFile *f_data = TFile::Open(path+"/singlephoton_tree.root" , "READONLY"); TTree *t_data = (TTree*)f_data->Get("Events"); 
    trees.push_back(t_data); legends.push_back("Data"); colors.push_back(1); 

    // mc
    TFile *f_topg   = TFile::Open(path+"/tgplusttg_tree.root" , "READONLY"); TTree *t_topg   = (TTree*)f_topg->Get("Events");
    TFile *f_vg     = TFile::Open(path+"/vg_tree.root"        , "READONLY"); TTree *t_vg     = (TTree*)f_vg->Get("Events");
    TFile *f_qcdmg  = TFile::Open(path+"/qcd-mg_tree.root"    , "READONLY"); TTree *t_qcdmg  = (TTree*)f_qcdmg->Get("Events");
    TFile *f_photon = TFile::Open(path+"/photon_tree.root"    , "READONLY"); TTree *t_photon = (TTree*)f_photon->Get("Events");
    trees.push_back(t_topg);   legends.push_back("t(t#bar{t})#gamma"); colors.push_back(627);     
    trees.push_back(t_vg);     legends.push_back("V#gamma");           colors.push_back(798); 
    trees.push_back(t_qcdmg);  legends.push_back("QCD");               colors.push_back(412);     
    trees.push_back(t_photon); legends.push_back("#gamma+jets");       colors.push_back(419);     

    cut_ak8  = "ht>500 && ak8_1_pt>=300. && n_ak8>=2";
    cut_ca15 = "ht>500 && ca15_1_pt>=200. && n_ca15>=2";
 
  }

  if (sample == "qcd") {
    TString path = "root://cmseos.fnal.gov//store//group/lpcjme/noreplica/NanoHRT/Trees/Jan25/qcd/";

    //data: always read the data tree first 
    TFile *f_data = TFile::Open(path+"/jetht_tree.root" , "READONLY"); TTree *t_data = (TTree*)f_data->Get("Events"); 
    trees.push_back(t_data); legends.push_back("Data"); colors.push_back(1); 

    // mc
    TFile *f_qcdmg  = TFile::Open(path+"/qcd-mg_tree.root"     , "READONLY"); TTree *t_qcdmg  = (TTree*)f_qcdmg->Get("Events");
    //TFile *f_vandvv = TFile::Open(path+"/vandvv_tree.root"     , "READONLY"); TTree *t_vandvv = (TTree*)f_vandvv->Get("Events");
    //TFile *f_top    = TFile::Open(path+"/tandtt_tree.root"     , "READONLY"); TTree *t_tandtt = (TTree*)f_tandtt->Get("Events");
    //TFile *f_qcdher = TFile::Open(path+"/qcd-herwig_tree.root" , "READONLY"); TTree *t_qcdher = (TTree*)f_qcdher->Get("Events");
    trees.push_back(t_qcdmg);  legends.push_back("QCD");        colors.push_back(412);     
    //trees.push_back(t_vandvv); legends.push_back("V/VV");       colors.push_back(618);
    //trees.push_back(t_top);    legends.push_back("t/t#bar{t}"); colors.push_back(425);
    //trees.push_back(t_qcdher); legends.push_back("QCD-Herwig"); colors.push_back(900);

    cut_ak8  = "ht>1000 && ak8_1_pt>=300. && n_ak8>=2";
    cut_ca15 = "ht>1000 && ca15_1_pt>=200. && n_ca15>=2";

  }


  makePlotDataMC(sample+"_jetpt","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_pt",16,200.,1000.,"p_{T}(jet) [GeV]",legends,colors,true,1);
  makePlotDataMC(sample+"_jeteta","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_eta",10,-2.4,2.4,"#eta(jet)",legends,colors,true,1);
  makePlotDataMC(sample+"_jetmsd","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_mass",20,0.,200.,"m_{SD}(jet) [GeV]",legends,colors,true,1);
  makePlotDataMC(sample+"_jettau32","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_tau3/ak8_1_tau2",20,0.,1.,"#tau_{32}(jet)",legends,colors,true,1);
  makePlotDataMC(sample+"_jettau21","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_tau2/ak8_1_tau1",20,0.,1.,"#tau_{21}(jet)",legends,colors,true,1);
  makePlotDataMC(sample+"_ecftoptag","datamcplots_20190126",trees,lumi,cut_ca15,"0==0","ca15_1_ecfTopTagBDT",20,0.,1.,"ECF BDT (t vs. QCD",legends,colors,true,1);
  makePlotDataMC(sample+"_bestwvsqcd","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_best_WvsQCD",20,0.,1.,"BEST (W vs. QCD)",legends,colors,true,1);
  makePlotDataMC(sample+"_besttvsqcd","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_best_TvsQCD",20,0.,1.,"BEST (t vs. QCD)",legends,colors,true,1);
  makePlotDataMC(sample+"_bestzvsqcd","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_best_ZvsQCD",20,0.,1.,"BEST (Z vs. QCD)",legends,colors,true,1);
  makePlotDataMC(sample+"_besthvsqcd","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_best_HvsQCD",20,0.,1.,"BEST (H vs. QCD)",legends,colors,true,1);
  makePlotDataMC(sample+"_deepak8wvsqcd","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_DeepAK8_WvsQCD",20,0.,1.,"DeepAK8 (W vs. QCD)",legends,colors,true,1);
  makePlotDataMC(sample+"_deepak8tvsqcd","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_DeepAK8_TvsQCD",20,0.,1.,"DeepAK8 (t vs. QCD)",legends,colors,true,1);
  makePlotDataMC(sample+"_deepak8zvsqcd","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_DeepAK8_ZvsQCD",20,0.,1.,"DeepAK8 (Z vs. QCD)",legends,colors,true,1);
  makePlotDataMC(sample+"_deepak8hvsqcd","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_DeepAK8_HvsQCD",20,0.,1.,"DeepAK8 (H vs. QCD)",legends,colors,true,1);
  makePlotDataMC(sample+"_deepak8mdwvsqcd","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_DeepAK8MD_WvsQCD",20,0.,1.,"Deepak8-MD (W vs. QCD)",legends,colors,true,1);
  makePlotDataMC(sample+"_deepak8mdtvsqcd","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_DeepAK8MD_TvsQCD",20,0.,1.,"Deepak8-MD (t vs. QCD)",legends,colors,true,1);
  makePlotDataMC(sample+"_deepak8mdzvsqcd","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_DeepAK8MD_ZvsQCD",20,0.,1.,"Deepak8-MD (Z vs. QCD)",legends,colors,true,1);
  makePlotDataMC(sample+"_deepak8mdhvsqcd","datamcplots_20190126",trees,lumi,cut_ak8,"0==0","ak8_1_DeepAK8MD_HvsQCD",20,0.,1.,"Deepak8-MD (H vs. QCD)",legends,colors,true,1);

}

void makePlotDataMC(TString name, TString dir, 
		    std::vector<TTree*> trees, TString lumi,TString cut, TString addcut,
		    TString var,int nbins,float xmin,float xmax,TString xaxisname,
		    std::vector<TString> legends, std::vector<int> colors, bool logy, int norm) {

  TH1::SetDefaultSumw2(kTRUE);
  std::cout << " working on " << name << " ...\n";

  TString sample; 
  if (name.Contains("qcd"))     { sample = "qcd"; }
  if (name.Contains("photons")) { sample = "photons"; }

  TH1F *h_sm; TH1F *h_data; std::vector<TH1F*> mc_histos; mc_histos.clear();
  THStack *hs = new THStack("hs_"+name,"hs_"+name);

  TLegend* leg = new TLegend(0.65,0.55,0.92,0.88);
  leg->SetFillStyle(0);
  leg->SetFillColor(0);
  leg->SetLineWidth(0);

  TString logstr = "lin"; if (logy) { logstr = "log"; }

  TCanvas *c_ = new TCanvas(name,name,600,600); c_->SetName("c_"+name);
  TPad *pMain  = new TPad("pMain_"+name,"pMain+"+name,0.0,0.25,1.0,1.0);
  pMain->SetRightMargin(0.05);
  pMain->SetLeftMargin(0.17);
  pMain->SetBottomMargin(0.);
  pMain->SetTopMargin(0.1);

  TPad *pRatio = new TPad("pRatio_"+name,"pRatio_"+name,0.0,0.0,1.0,0.25);
  pRatio->SetRightMargin(0.05);
  pRatio->SetLeftMargin(0.17);
  pRatio->SetTopMargin(0.);
  pRatio->SetBottomMargin(0.37);
  pMain->Draw();
  pRatio->Draw();
 

  for (unsigned int i=0; i<trees.size(); ++i) {

    ostringstream tmpCount;
    tmpCount << i;
    TString count = tmpCount.str();
 
    bool data = false;  if (i==0) { data = true; }

    pMain->cd();
    TH1F *h = create1Dhisto(sample,trees[i],lumi,cut+" && "+addcut,var,nbins,xmin,xmax,false,1,1,"h_"+name+"_"+count,false,data); 
    if (i==0) { 
      h->SetMarkerSize(1.2); h->SetMarkerStyle(20); h->SetLineWidth(1); h->SetLineColor(colors[i]); h->SetFillColor(0);  
      leg->AddEntry(h,legends[i],"P");
      h->GetYaxis()->SetTitleOffset(1.45);
      h->GetXaxis()->SetTitle(xaxisname);
      if (norm == 0) { h->GetYaxis()->SetTitle("Events / bin"); } else { h->GetYaxis()->SetTitle("a. u."); }
      if (logy) { 
	gPad->SetLogy(); 
	h->GetYaxis()->SetRangeUser(1.,1000.*h->GetBinContent(h->GetMaximumBin())); 
	if (var.Contains("tau") || var.Contains("eta") || var.Contains("ecf")) { h->GetYaxis()->SetRangeUser(1.,100000.*h->GetBinContent(h->GetMaximumBin())); }
      } 
      else { h->GetYaxis()->SetRangeUser(0.,1.7*h->GetBinContent(h->GetMaximumBin())); }
      h->Draw("P E0");
      h_data = (TH1F*)h->Clone("h_"+name+"_data");
    } 
    else { 
      h->SetFillColor(colors[i]); h->SetMarkerSize(0); h->SetLineColor(colors[i]); 
      leg->AddEntry(h,legends[i],"FL"); 
      h->Draw("HIST E0 sames");
    }
    
    if (i==1) { h_sm = (TH1F*)h->Clone("h_sm"); hs->Add(h); }
    if (i>1 ) { h_sm->Add(h); hs->Add(h); }

  }

  // norm MC
  float normVal = h_data->Integral()/h_sm->Integral();
  if (norm !=0) { h_sm->Scale(normVal); }

  // data mc ratio  
  TH1F *h_r = (TH1F*)h_data->Clone("h_"+name+"_r"); h_r->Divide(h_data,h_sm);

  TList *histos = hs->GetHists();
  TIter next(histos);
  TH1F *hist;
  while ((hist =(TH1F*)next())) { hist->Scale(normVal); }
 
  // continue drawing  
  pMain->cd();
  hs->Draw("HIST E0 sames");
  h_data->Draw("P E0 sames");
  leg->Draw("sames");
  c_->RedrawAxis();
  pMain->RedrawAxis();
  
  pRatio->cd();
  gPad->SetBottomMargin(0.2);
  h_r->GetYaxis()->SetTitleOffset(0.9);
  h_r->GetYaxis()->SetTitleSize(0.1);
  h_r->GetYaxis()->SetLabelSize(0.08);
  h_r->GetXaxis()->SetTitleSize(0.1);
  h_r->GetXaxis()->SetLabelSize(0.08);
  h_r->GetXaxis()->SetTitle(xaxisname);
  h_r->GetYaxis()->SetTitle("Data / MC");
  h_r->GetYaxis()->SetRangeUser(0.,2.);
  h_r->Draw("P E0");


  const int dir_err = system("mkdir -p ./"+dir);
  if (-1 == dir_err) {
      printf("Error creating directory!n");
      exit(1);
  }
  c_->Print(dir+"/"+name+"_"+logstr+".png");
  c_->Print(dir+"/"+name+"_"+logstr+".pdf");
   
}



TH1F *create1Dhisto(TString sample,TTree *tree,TString intLumi,TString cuts,TString branch,int bins,float xmin,float xmax,
		    bool useLog,int color, int style,TString name,bool norm,bool data) {
  TH1::SetDefaultSumw2(kTRUE);

  TString cut;
  if (sample == "photons") {
    if (data) { cut ="(passPhoton165_HE10 && "+cuts+")"; } // photon
    else      { cut ="(xsecWeight*puWeight*"+intLumi+")*(passPhoton165_HE10 &&"+cuts+")"; }
  }
  if (sample == "qcd") {
    if (data) { cut ="(passHTTrig && "+cuts+")"; } // QCD
    else      { cut ="(xsecWeight*puWeight*"+intLumi+")*(passHTTrig &&"+cuts+")"; }
  }
  //else { std::cout << name << "\n"; cut ="(xsecWeight*puWeight*topptWeight*"+intLumi+")*("+cuts+")"; }

  TH1F *hTemp = new TH1F(name,name,bins,xmin,xmax); //hTemp->SetName(name);
  tree->Project(name,branch,cut);

  hTemp->SetLineWidth(3);
  hTemp->SetMarkerSize(0);
  hTemp->SetLineColor(color);
  hTemp->SetFillColor(color);
  hTemp->SetLineStyle(style);

  // ad overflow bin             
  double error =0.; double integral = hTemp->IntegralAndError(bins,bins+1,error);
  hTemp->SetBinContent(bins,integral);
  hTemp->SetBinError(bins,error);

  if (norm) { hTemp->Scale(1./(hTemp->Integral())); }

  return hTemp;
} //~ end of create1Dhisto



TH2F *create2Dhisto(TString name,TTree *tree,TString intLumi,TString cuts,TString xbranch,int xbins,float xmin,float xmax,TString ybranch,int ybins,float ymin,float ymax,bool data) {
  TH2::SetDefaultSumw2(kTRUE);
  TString cut;
  if (data) { cut ="(httrigprescalewgt)*("+cuts+")"; }
  else      { cut ="(xsecWeight*puWeight*topptWeight*"+intLumi+")*("+cuts+")"; }

  TH2F *hTemp = new TH2F(name,name,xbins,xmin,xmax,ybins,ymin,ymax); hTemp->SetName(name);
  tree->Project(name,ybranch+":"+xbranch,cut);

  return hTemp;
} //~ end of create1Dhisto

