#include "Histograms.hh"
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>

Histograms::Histograms() {}
Histograms::~Histograms() {}

void Histograms::Draw() {
    cSDSs_->Divide(3,3);
    cSDSs_->cd(1);
    hSD0_->GetXaxis()->SetTitle("X[cm]");
    hSD0_->GetYaxis()->SetTitle("Y[cm]");
    hSD0_->Draw();
    cSDSs_->cd(2);
    hSD0_projX_ = hSD0_->ProjectionX();
    hSD0_projX_->Draw("hist");
    cSDSs_->cd(3);
    hSD0_projY_ = hSD0_->ProjectionY();
    hSD0_projY_->Draw("hist");
    cSDSs_->cd(4);
    hSD1_->GetXaxis()->SetTitle("X[cm]");
    hSD1_->GetYaxis()->SetTitle("Y[cm]");
    hSD1_->Draw();
    cSDSs_->cd(5);
    hSD1_projX_ = hSD1_->ProjectionX(); 
    hSD1_projX_ ->Draw("hist");
    cSDSs_->cd(6);
    hSD1_projY_ = hSD1_->ProjectionY();
    hSD1_projY_->Draw("hist");
    cSDSs_->cd(7);
    hSD2_->GetXaxis()->SetTitle("X[cm]");
    hSD2_->GetYaxis()->SetTitle("Y[cm]");
    hSD2_->Draw();
    cSDSs_->cd(8);
    hSD2_projX_ = hSD2_->ProjectionX();
    hSD2_projX_->Draw("hist");
    cSDSs_->cd(9);
    hSD2_projY_ = hSD2_->ProjectionY();
    hSD2_projY_->Draw("hist");

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
}

void Histograms::Fill(const Event& ev) {
    hSD0_->Fill(ev.Getpos()[0], ev.Getpos()[1]);
    hSD1_->Fill(ev.Getpos()[2], ev.Getpos()[3]);
    hSD2_->Fill(ev.Getpos()[4], ev.Getpos()[5]);
    hhTheta_->Fill(ev.GetThetaXIn());
    hvTheta_->Fill(ev.GetThetaYIn());
    hhthetaout_->Fill(ev.GetThetaXOut());
    hvthetaout_->Fill(ev.GetThetaYOut());
    hhthetadiff_->Fill(ev.GetThetaDeflX());
    hhthetadiff_->Fill(ev.GetThetaDeflY());
}

void Histograms::Save(const std::string& filename) {
    TFile f(filename.c_str(), "RECREATE");
    if (hSD0_) hSD0_->Write("hSD0");
    f.Close(); 
}

TH1* Histograms::GetHist(const std::string& name) {
    return (TH1*)gDirectory->Get(name.c_str());  // TH2F è compatibile con TH1*
}
