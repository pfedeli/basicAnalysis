#ifndef HISTOGRAMS_HH
#define HISTOGRAMS_HH

#include "Event.hh"
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include "Config.hh"

const int nRangeSD = 2; //cm
const int nBinSD = 200;

class Histograms {

public:
    Histograms();
    ~Histograms();

    void Draw();
    void Fill(const Event& ev);
    void FillEff(const Event& ev, const bool& cut);
    void Save(const std::string& filename);
    TH1* GetHist(const std::string& name);
    void UpdateCanvas(){};

private:

    TH2F* hSD1_ = new TH2F("hSD1", "SD1", nBinSD, -nRangeSD, nRangeSD, nBinSD, -nRangeSD, nRangeSD);
    TH2F* hSD2_ = new TH2F("hSD2", "SD2", nBinSD, -nRangeSD, nRangeSD, nBinSD, -nRangeSD, nRangeSD);
    TH2F* hSD3_ = new TH2F("hSD3", "SD3", nBinSD, -nRangeSD, nRangeSD, nBinSD, -nRangeSD, nRangeSD);
    TH2F* hSD4_ = new TH2F("hSD4", "SD4", nBinSD, -nRangeSD, nRangeSD, nBinSD, -nRangeSD, nRangeSD);
    TH1D* hSD1_projX_;
    TH1D* hSD1_projY_;
    TH1D* hSD2_projX_;
    TH1D* hSD2_projY_;
    TH1D* hSD3_projX_;
    TH1D* hSD3_projY_;

    TH1F *hhTheta_ = new TH1F("hhTheta", "Orizontal theta in", 100, -0.2, 0.2);
    TH1F *hvTheta_ = new TH1F("hvTheta", "Vertical thata in", 100, -0.2, 0.2);
    TH1F *hhthetaout_ = new TH1F("hhthetaout", "horizontal theta out", 100, -0.2, 0.2);
    TH1F *hvthetaout_ = new TH1F("hvthetaout", "vertical theta out", 100, -0.2, 0.2);
    TH1F *hhthetadiff_ = new TH1F("hhthetadiff", "horizontal deflection", 100, -0.2, 0.2);
    TH1F *hvthetadiff_ = new TH1F("hvthetadiff", "vertical deflection", 100, -0.2, 0.2);

    TH2F *hhBruco_ = new TH2F("hhBruco", "Deflection vs ROT - theta_x in", 20, -54, -48, 20, -2, 2);
    TH2F *hvBruco_ = new TH2F("hvBruco", "Deflection vs CRADDLE - theta_y in", 200, -15, 40, 200, -20, 20);
    TH2F *hEffnum_ = new TH2F("hEffnum", "Efficacia numeratore", 200, -15, 40, 200, -40, 70);
    TH2F *hEffden_ = new TH2F("hEffden", "Efficacia denominatore", 200, -15, 40, 200, -40, 70);

};

#endif
