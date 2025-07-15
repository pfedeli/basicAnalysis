#ifndef HISTOGRAMS_HH
#define HISTOGRAMS_HH

#include "Event.hh"
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>

class Histograms {
public:
    Histograms();
    ~Histograms();

    void Draw();
    void Fill(const Event& ev);
    void Save(const std::string& filename);
    TH1* GetHist(const std::string& name);
    void UpdateCanvas(){
        cSDSs_->Update();
        cAngles_->Update();
    }

private:
    TCanvas* cSDSs_ = new TCanvas("cSDS", "cSDS");
    TH2F* hSD1_ = new TH2F("hSD1", "SD1", 100, 0, 10, 100, 0, 10);
    TH2F* hSD2_ = new TH2F("hSD2", "SD2", 100, 0, 10, 100, 0, 10);
    TH2F* hSD3_ = new TH2F("hSD3", "SD3", 100, 0, 10, 100, 0, 10);
    TH2F* hSD4_ = new TH2F("hSD4", "SD4", 100, 0, 10, 100, 0, 10);
    TH1D* hSD1_projX_;
    TH1D* hSD1_projY_;
    TH1D* hSD2_projX_;
    TH1D* hSD2_projY_;
    TH1D* hSD3_projX_;
    TH1D* hSD3_projY_;

    TCanvas *cAngles_ = new TCanvas("cAngles", "cAngles");
    TH1F *hhTheta_ = new TH1F("hhTheta", "Orizontal theta in", 200, -20, 20);
    TH1F *hvTheta_ = new TH1F("hvTheta", "Vertical thata in", 200, -20, 20);
    TH1F *hhthetadiff_ = new TH1F("hhthetadiff", "horizontal deflection", 100, -20, 20);
    TH1F *hvthetadiff_ = new TH1F("hvthetadiff", "vertical deflection", 100, -20, 20);
    TH1F *hhthetaout_ = new TH1F("hhthetaout", "horizontal theta out", 200, -20, 20);
    TH1F *hvthetaout_ = new TH1F("hvthetaout", "vertical theta out", 200, -20, 20);
    TH2F *hhBruco_ = new TH2F("hhBruco", "LG PH vs ROT - theta_x in", 200, -15, 40, 200, 0, 5000);
    TH2F *hvBruco_ = new TH2F("hvBruco", "LG PH vs CRADDLE - theta_y in", 200, -40, 70, 200, 0, 5000);
    TH2F *hEffnum_ = new TH2F("hEffnum", "Efficacia numeratore", 200, -15, 40, 200, -40, 70);
    TH2F *hEffden_ = new TH2F("hEffden", "Efficacia denominatore", 200, -15, 40, 200, -40, 70);

    TCanvas *cQtot_ = new TCanvas("cQtot", "cQtot");
    TH1F *hQtot_ = new TH1F("Qtot0", "", 100, 0, 20000);

    TCanvas *cnclu_ = new TCanvas("cnclu", "cnclu");
    TH1F *hnclu_ = new TH1F("nclu0", "", 20, 0, 20);
};

#endif
