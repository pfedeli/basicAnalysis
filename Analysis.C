#define Analysis_cxx
#include "include/Analysis.h"
#include "include/Histograms.hh"
#include "include/Helper.hh"

// root include
#include <TF1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

// usage:
// gSystem->Load("lib/libAnalysis.so");
//.L Analysis.C++
// Analysis t("config/config.conf")
// t.Loop()

void Analysis::Loop()
{
   Histograms hists;

   TH2F *hSD1 = dynamic_cast<TH2F *>(hists.GetHist("hSD1"));
   TH2F *hSD2 = dynamic_cast<TH2F *>(hists.GetHist("hSD2"));
   TH2F *hSD3 = dynamic_cast<TH2F *>(hists.GetHist("hSD3"));

   TH1F *hhTheta = dynamic_cast<TH1F *>(hists.GetHist("hhTheta"));
   TH1F *hvTheta = dynamic_cast<TH1F *>(hists.GetHist("hvTheta"));
   TH1F *hhthetadiff = dynamic_cast<TH1F *>(hists.GetHist("hhthetadiff"));
   TH1F *hvthetadiff = dynamic_cast<TH1F *>(hists.GetHist("hvthetadiff"));
   TH1F *hhthetaout = dynamic_cast<TH1F *>(hists.GetHist("hhthetaout"));
   TH1F *hvthetaout = dynamic_cast<TH1F *>(hists.GetHist("hvthetaout"));

   TH2F *hhBruco = dynamic_cast<TH2F *>(hists.GetHist("hhBruco"));
   TH2F *hvBruco = dynamic_cast<TH2F *>(hists.GetHist("hvBruco"));
   TH2F *hEffnum = dynamic_cast<TH2F *>(hists.GetHist("hEffnum"));
   TH2F *hEffden = dynamic_cast<TH2F *>(hists.GetHist("hEffden"));

   if (fChain == 0)
      return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
      if (jentry % 10000 == 0)
         std::cout << "completing jentry: " << jentry << std::endl;
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
      {
         std::cout << "completing jentry: " << jentry - 1 << std::endl;
         break;
      }

      nb = fChain->GetEntry(jentry);
      nbytes += nb;

      xpos[0] -= SD1xcorr_;
      xpos[1] -= SD1ycorr_;
      xpos[2] -= SD2xcorr_;
      xpos[3] -= SD2ycorr_;
      xpos[4] -= SD3xcorr_;
      xpos[5] -= SD3ycorr_;

      RecoEvent(ientry); // calculate angles and stuff

      if (Cut(ientry) < 0)
         continue;

      hSD1->Fill(xpos[0], xpos[1]);
      hSD2->Fill(xpos[2], xpos[3]);
      hSD3->Fill(xpos[4], xpos[5]);
      hhTheta->Fill(thetax_in_);
      hvTheta->Fill(thetay_in_);
      hhthetaout->Fill(thetax_out_);
      hvthetaout->Fill(thetay_out_);
      hhthetadiff->Fill(thetax_out_ - thetax_in_);
      hvthetadiff->Fill(thetay_out_ - thetay_in_);
      hhBruco->Fill(xinfo[0] / 1000 - thetax_in_, thetax_out_ - thetax_in_);
      hvBruco->Fill(xinfo[1] / 1000 - thetay_in_, thetax_out_ - thetax_in_);
   } // end loop
   hists.Draw();

   std::cout << "****************************************************************" << std::endl;
   std::cout << "SD1xcorr=" << hSD1->ProjectionX()->GetMean() << std::endl
             << "SD1ycorr=" << hSD1->ProjectionY()->GetMean() << std::endl
             << "SD2xcorr=" << hSD2->ProjectionX()->GetMean() << std::endl
             << "SD2ycorr=" << hSD2->ProjectionY()->GetMean() << std::endl
             << "SD3xcorr=" << hSD3->ProjectionX()->GetMean() << std::endl
             << "SD3ycorr=" << hSD3->ProjectionY()->GetMean() << std::endl;
   std::cout << "****************************************************************" << std::endl;
   std::cout << "beam divergence X: " << hhTheta->GetFunction("gaus")->GetParameter(2) << "#murad" << std::endl
             << "beam divergence Y: " << hvTheta->GetFunction("gaus")->GetParameter(2) << "#murad" << std::endl;
   std::cout << "****************************************************************" << std::endl;
   std::cout << "beam divergence at crystal X: " << hhTheta->GetFunction("gaus")->GetParameter(2) / 100 * SD1CRY_Z_ << "mm" << std::endl
             << "beam divergence at crystal Y: " << hvTheta->GetFunction("gaus")->GetParameter(2) / 100 * SD1CRY_Z_ << "mm" << std::endl;

}
