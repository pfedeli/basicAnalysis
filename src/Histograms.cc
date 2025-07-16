#include "Histograms.hh"
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <iostream>

Histograms::Histograms() {}
Histograms::~Histograms() {}

void Histograms::Draw() {
    cSDSs_->Divide(3,3);
    cSDSs_->cd(1);
    hSD1_->GetXaxis()->SetTitle("X[cm]");
    hSD1_->GetYaxis()->SetTitle("Y[cm]");
    hSD1_->Draw();
    cSDSs_->cd(2);
    hSD1_projX_ = hSD1_->ProjectionX();
    hSD1_projX_->Draw("hist");
    cSDSs_->cd(3);
    hSD1_projY_ = hSD1_->ProjectionY();
    hSD1_projY_->Draw("hist");
    cSDSs_->cd(4);
    hSD2_->GetXaxis()->SetTitle("X[cm]");
    hSD2_->GetYaxis()->SetTitle("Y[cm]");
    hSD2_->Draw();
    cSDSs_->cd(5);
    hSD2_projX_ = hSD2_->ProjectionX(); 
    hSD2_projX_ ->Draw("hist");
    cSDSs_->cd(6);
    hSD2_projY_ = hSD2_->ProjectionY();
    hSD2_projY_->Draw("hist");
    cSDSs_->cd(7);
    hSD3_->GetXaxis()->SetTitle("X[cm]");
    hSD3_->GetYaxis()->SetTitle("Y[cm]");
    hSD3_->Draw();
    cSDSs_->cd(8);
    hSD3_projX_ = hSD3_->ProjectionX();
    hSD3_projX_->Draw("hist");
    cSDSs_->cd(9);
    hSD3_projY_ = hSD3_->ProjectionY();
    hSD3_projY_->Draw("hist");

    cAngles_->cd();
    cAngles_->Divide(3,2);
    cAngles_->cd(1);
    hhTheta_->GetXaxis()->SetTitle("incoming X angle [mrad]");
    hhTheta_->Draw();
    cAngles_->cd(2);
    hhthetaout_->GetXaxis()->SetTitle("outgoing X angle [mrad]");
    hhthetaout_->Draw();
    cAngles_->cd(3);
    hhthetadiff_->GetXaxis()->SetTitle("deflection X angle [mrad]");
    hhthetadiff_->Draw();
    cAngles_->cd(4);
    hvTheta_->GetXaxis()->SetTitle("incoming Y angle [mrad]");
    hvTheta_->Draw();
    cAngles_->cd(5);
    hvthetaout_->GetXaxis()->SetTitle("outgoing Y angle [mrad]");
    hvthetaout_->Draw();
    cAngles_->cd(6);
    hvthetadiff_->GetXaxis()->SetTitle("deflection Y angle [mrad]");
    hvthetadiff_->Draw();

    cQtot_->cd();
    hQtot_->GetXaxis()->SetTitle("Charge on SD3 [ADC]");
    hQtot_->Draw();

    cnclu_->cd();
    hnclu_->GetXaxis()->SetTitle("Ncluster on SD3 [ADC]");
    hnclu_->Draw();
}

void Histograms::Fill(const Event& ev) {
    hSD1_->Fill(ev.Getpos()[0], ev.Getpos()[1]);
    hSD2_->Fill(ev.Getpos()[2], ev.Getpos()[3]);
    hSD3_->Fill(ev.Getpos()[4], ev.Getpos()[5]);
    hhTheta_->Fill(ev.GetThetaXIn());
    hvTheta_->Fill(ev.GetThetaYIn());
    hhthetaout_->Fill(ev.GetThetaXOut());
    hvthetaout_->Fill(ev.GetThetaYOut());
    hhthetadiff_->Fill(ev.GetThetaDeflX());
    hvthetadiff_->Fill(ev.GetThetaDeflY());
    hQtot_->Fill(ev.Getqtot()[0]);
    hnclu_->Fill(ev.Getnclu()[0]);

}

void Histograms::Save(const std::string& runidname) {
    std::string filename = "/home/pierluigi/Analysis/epiboost/data2/Run" + runidname + ".root";
    TFile f(filename.c_str(), "recreate");
    hQtot_->Write(("hqtot" + runidname).c_str());
    hnclu_->Write(("hnclu" + runidname).c_str());
    f.Close();
}

TH1* Histograms::GetHist(const std::string& name) {
    return (TH1*)gDirectory->Get(name.c_str());  // TH2F è compatibile con TH1*
}
