#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <map>
#include <thread>
#include <chrono>
#include <regex>

#include <TApplication.h>
#include <TCanvas.h>
#include <TH2F.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TLatex.h>
#include <TMath.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TFitResult.h>
#include <TF1.h>
#include <TProfile.h>
#include <TEfficiency.h>
#include <TFile.h>
#include <TProfile2D.h>
#include <TGraphAsymmErrors.h>

namespace fs = std::filesystem;

const double SD1SD2_Z = 330.0, SD1CRY_Z = 383.2, CRYSD3_Z = 626, SD2SD3_Z = 679.2; // cm
// const double SD1SD2_Z = 330.0, SD1CRY_Z = 383.2, CRYSD3_Z = 10.7, SD2SD3_Z = 6.8;
const int nstrips = 8, nqtot = 4, nclu = 4, ndigi = 8, ngonio = 5, ninfoplus = 2;
const double cut_x1 = 4.4, cut_x2 = 5.1, cut_y1 = 6.9, cut_y2 = 7.5, ene_cut = 20, cut_eneLG = 20, theta_crit = 0.785; //0.793 @2.86

const double align_SD1x = 0.107, align_SD1y = -2.21; // to align at sd2 sd1-align_sd1
// const double align_SD3x = 0.484, align_SD3y = -2.053; // to align at sd2 sd3-align_sd
const double align_SD3x = 1.125, align_SD3y = -2.797; // to align at sd2 sd3-align_sd

int tot_evt = 0;

struct SpillData
{
    double pos[nstrips];
    int nclustrip[nclu];
    double qtot[nqtot];
    int baseline[ndigi];
    int times[ndigi];
    int pulse_height[ndigi];
    double goniometro[ngonio];
    std::string info_plus[ninfoplus];
    std::string Spill_number;
};

bool FillSpill(const std::string &line, SpillData &Spill)
{
    std::istringstream iss(line);
    for (int i = 0; i < nstrips; ++i)
        if (!(iss >> Spill.pos_[i]))
            return false;
    for (int i = 0; i < nclu; ++i)
        if (!(iss >> Spill.nclu_[i]))
            return false;
    for (int i = 0; i < nqtot; ++i)
        if (!(iss >> Spill.qtot[i]))
            return false;
    for (int i = 0; i < ndigi; ++i)
        if (!(iss >> Spill.baseline[i]))
            return false;
    for (int i = 0; i < ndigi; ++i)
        if (!(iss >> Spill.times[i]))
            return false;
    for (int i = 0; i < ndigi; ++i)
        if (!(iss >> Spill.pulse_height[i]))
            return false;
    for (int i = 0; i < ngonio; ++i)
        if (!(iss >> Spill.goniometro[i]))
            return false;
    for (int i = 0; i < ninfoplus; ++i)
        if (!(iss >> Spill.info_plus[i]))
            return false;
    if (!(iss >> Spill.Spill_number))
        return false;
    return true;
}

//thcut only for axial
bool set_cuts(double x, double y, double eneCH, int nclusx, int nclusy, int eneLG, double theta, double CHtime, double LGtime)
{
    bool am = false;

    bool poscuts = (x > cut_x1 && x < cut_x2 && y > cut_y1 && y < cut_y2);
    bool CHenecuts = eneCH > ene_cut;
    bool clucut = nclusx == 1 && nclusy ==1;
    bool ENELG = eneLG > cut_eneLG;
    bool timecut = (LGtime - CHtime) < 40 && (LGtime - CHtime) > 0;
    bool thcut = (theta < 2* theta_crit) && (theta > -2*theta_crit);

    if(am){
        return(poscuts && CHenecuts && clucut && timecut);
    }
    return (poscuts && CHenecuts && clucut && timecut && thcut);
};

int main(int argc, char *argv[])
{

    TApplication app("app", &argc, argv);
    gStyle->SetOptFit(1);

    std::string directory = "../tmpmount/ASCII_MICHELA/ascii_daq_sshfs/";
    std::ifstream filelist;
    std::vector<std::string> run_number;

    TH1F *hhTheta = new TH1F("hhTheta", "Orizontal theta in", 200, -10, 10);
    TH1F *hvTheta = new TH1F("hvTheta", "Vertical thata in", 200, -10, 10);
    TH2F *hSDs[nstrips / 2];
    TH1F *hQtot[nqtot];
    TH1I *hdigiene[ndigi], *hdigitime[ndigi], *hdigipede[ndigi], *hNclu[nclu];
    TH1F *hhthetadiff = new TH1F("hhthetadiff", "horizontal deflection", 100, -10, 10);
    TH1F *hvthetadiff = new TH1F("hvthetadiff", "vertical deflection", 100, -10, 10);
    TH1F *hhthetaout = new TH1F("hhthetaout", "horizontal theta out", 200, -10, 10);
    TH1F *hvthetaout = new TH1F("hvthetaout", "vertical theta out", 200, -10, 10);
    TH2F *hhBruco = new TH2F("hhBruco", "LG PH vs ROT - theta_x in", 200, -15, 40, 200, 0, 5000);
    TH2F *hvBruco = new TH2F("hvBruco", "LG PH vs CRADDLE - theta_y in", 200, -40, 70, 200, 0, 5000);
    TH2F *hthetaxBruco = new TH2F("hthetaxBruco", "deflection x vs theta_x in", 200, -15, 40, 500, 0, 50);
    TH2F *hthetayBruco = new TH2F("hthetayBruco", "deflection y vs theta_y in", 200, -40, 70, 500, 0, 50);
    TH2F *hxqBruco = new TH2F("hxqBruco", "charge x vs pos x", 100, 0, 10, 500, 0, 2000);
    TH2F *hyqBruco = new TH2F("hyqBruco", "charge x vs pos y", 100, 0, 10, 500, 0, 2000);
    TH2F *hEffnum = new TH2F("hEffnum", "Efficacia numeratore", 200, -15, 40, 200, -40, 70);
    TH2F *hEffden = new TH2F("hEffden", "Efficacia denominatore", 200, -15, 40, 200, -40, 70);
    TProfile2D *pROTCRAD = new TProfile2D("pROTCRAD", "ENE LG ON Z", 200, -15, 40, 200, -40, 70);
    TProfile2D *hROTCRAD = new TProfile2D("hROTCRAD", "DEFLESSION ON Z", 200, -15, 40, 200, -40, 70);
    TProfile2D *hROTCRADnclu = new TProfile2D("hROTCRADnclu", "NCLCU ON Z", 200, -15, 40, 200, -40, 70);
    TProfile2D *hROTCRADnqtot = new TProfile2D("hROTCRADnqtot", "QTOT ON Z", 200, -15, 40, 200, -40, 70);
    TH1F *hTimediff = new TH1F("hTimediff", "hTimediff", 200, -200, 1000);
    TProfile2D *pdeflxy = new TProfile2D("pdeflxy", "Deflession x and y", 200, -10, 10, 200, -10, 10);
    TH2F *hdeflxy = new TH2F("hdeflxy", "Deflession x and y", 200, -10, 10, 200, -10, 10);

    for (int a = 0; a < ndigi; a++)
    {
        hdigiene[a] = new TH1I(Form("ch_%d_ene", a), "", 200, 0, 2000);
        hdigitime[a] = new TH1I(Form("ch_%d_time", a), "", 500, 0, 2000);
        hdigipede[a] = new TH1I(Form("ch_%d_pede", a), "", 200, 0, 2000);
    }

    for (int a = 0; a < nstrips / 2; a++)
    {
        hSDs[a] = new TH2F(Form("SD%d", a), Form("MONITOR SD%d", a + 1), 100, 0, 10, 100, 0, 20);
    }

    for (int a = 0; a < nqtot; a++)
    {
        hQtot[a] = new TH1F(Form("Qtot%d", a), "", 100, 0, 50000);
    }

    for (int a = 0; a < nclu; a++)
    {
        hNclu[a] = new TH1I(Form("nclu%d", a), Form("nclu %d", a), 25, 0, 25);
    }

    // variable
    int perc = 0;

    if (argc < 2)
    {
        std::cout << "format: ./SignleRun runNumber(e.g. 730134)" << std::endl;
    }
    else
    {
        for (int a = 1; a < argc; a++)
        {
            run_number.push_back(argv[a]);
        }
    }
    for (int run_id = 0; run_id < run_number.size(); run_id++)
    {
        tot_evt = 0;
        double temp_craddle;
        double temp_rot;
        bool first = true;
        std::regex pattern("run" + run_number[run_id] + "_\\d{6}\\.dat");
        std::vector<fs::path> files_to_read;

        for (const auto &entry : fs::directory_iterator(directory))
        {
            if (std::regex_match(entry.path().filename().string(), pattern))
            {
                files_to_read.push_back(entry.path());
            }
        }

        if (files_to_read.empty())
        {
            std::cerr << "No files found for run " << run_number[run_id] << std::endl;
            return 1;
        }

        std::cout << "Found " << files_to_read.size() << " file(s) for run " << run_number[run_id] << std::endl;

        std::vector<SpillData> Spills;
        for (const auto &filepath : files_to_read)
        {
            std::ifstream file(filepath);
            if (!file.is_open())
                continue;
            std::string line;
            while (std::getline(file, line))
            {
                tot_evt++;
                if (line.empty() || line[0] == '#')
                    continue;
                SpillData sp;
                if (FillSpill(line, sp))
                {
                    sp.pos[0] -= align_SD1x;
                    sp.pos[1] -= align_SD1y;
                    sp.pos[4] -= align_SD3x;
                    sp.pos[5] -= align_SD3y;

                    double hTheta = TMath::ATan((sp.pos[2] - sp.pos[0]) / SD1SD2_Z) * 1000;
                    double vTheta = TMath::ATan((sp.pos[3] - sp.pos[1]) / SD1SD2_Z) * 1000;

                    float xcry = sp.pos[0] + (sp.pos[2] - sp.pos[0]) / SD1SD2_Z * SD1CRY_Z;
                    float ycry = sp.pos[1] + (sp.pos[3] - sp.pos[1]) / SD1SD2_Z * SD1CRY_Z;

                    double htheta_out = TMath::ATan((sp.pos[4] - xcry) / CRYSD3_Z) * 1000; //-XCRY
                    double vtheta_out = TMath::ATan((sp.pos[5] - ycry) / CRYSD3_Z) * 1000; //-ycry CRYSD3_Z

                    double htheta_diff = htheta_out - hTheta;
                    double vtheta_diff = vtheta_out - vTheta;
                    double theta_diff = sqrt(hTheta*hTheta + vTheta*vTheta);
                    temp_rot = sp.goniometro[0];
                    temp_craddle = sp.goniometro[1];
                    hEffden->Fill(sp.goniometro[0] / 1000 - htheta_diff, sp.goniometro[1] / 1000 - vtheta_diff);
                    if (sp.pulse_height[2] > 160.0 && sp.pulse_height[2] < 460.360)
                    {
                        hEffnum->Fill(sp.goniometro[0] / 1000 - htheta_diff, sp.goniometro[1] / 1000 - vtheta_diff);
                    }

                    if (set_cuts(xcry, ycry, sp.pulse_height[0], sp.nclustrip[0],sp.nclustrip[1], sp.pulse_height[2], theta_diff, sp.times[0], sp.times[2]))
                    {
                        Spills.push_back(sp);
                        for (int a = 0; a < nstrips; a += 2)
                        {
                            int histIndex = a / 2;
                            hSDs[histIndex]->Fill(sp.pos[a], sp.pos[a + 1], sp.pulse_height[2]);
                        }
                        hhTheta->Fill(hTheta);
                        hvTheta->Fill(vTheta);
                        hhthetadiff->Fill(htheta_diff);
                        hvthetadiff->Fill(vtheta_diff);
                        hhthetaout->Fill(htheta_out);
                        hvthetaout->Fill(vtheta_out);
                        hdeflxy->Fill(htheta_diff, vtheta_diff);
                        pdeflxy->Fill(htheta_diff, vtheta_diff, 1);
                        for (int a = 0; a < ndigi; a++)
                        {
                            hdigiene[a]->Fill(sp.pulse_height[a]);
                            hdigitime[a]->Fill(sp.times[a]);
                            hdigipede[a]->Fill(sp.baseline[a]);
                        }
                        hTimediff->Fill(sp.times[2] - sp.times[0]);

                        for (int a = 0; a < nqtot; a++)
                        {
                            hQtot[a]->Fill(sp.qtot[a]);
                        }

                        for (int a = 0; a < nclu; a++)
                        {
                            hNclu[a]->Fill(sp.nclustrip[a]);
                        }

                        // scan plots
                        hhBruco->Fill(sp.goniometro[0] / 1000 - hTheta, sp.pulse_height[2]);
                        hvBruco->Fill(sp.goniometro[1] / 1000 - vTheta, sp.pulse_height[2]);
                        if (sp.nclustrip[0] == 1 && sp.nclustrip[1] == 1)
                        {
                            hthetaxBruco->Fill(sp.goniometro[0] / 1000 - hTheta, abs(htheta_diff));
                            hthetayBruco->Fill(sp.goniometro[1] / 1000 - vTheta, abs(vtheta_diff));
                        }
                        pROTCRAD->Fill(sp.goniometro[0] / 1000, sp.goniometro[1] / 1000, sp.pulse_height[2]);
                        hROTCRAD->Fill(sp.goniometro[0] / 1000, sp.goniometro[1] / 1000, sqrt(htheta_diff * htheta_diff + vtheta_diff * vtheta_diff));
                        hROTCRADnqtot->Fill(sp.goniometro[0] / 1000, sp.goniometro[1] / 1000, sqrt(sp.qtot[0] * sp.qtot[0] + sp.qtot[1] * sp.qtot[1]));

                        if (sp.nclustrip[0] > 1 && sp.nclustrip[1] > 1)
                        {
                            hROTCRADnclu->Fill(sp.goniometro[0] / 1000, sp.goniometro[1] / 1000, sp.nclustrip[0] + sp.nclustrip[1]);
                        }
                        hxqBruco->Fill(sp.pos[4], sp.qtot[0]);
                        hyqBruco->Fill(sp.pos[5], sp.qtot[0]);
                    }
                }
            }
            perc++;
            std::cerr << Form("completing: %d/%ld at %.f percent", run_id + 1, run_number.size(), perc / float(files_to_read.size()) * 100) << std::endl;
        }

        std::cout << "Total events cutted: " << Spills.size() << "/" << tot_evt << "= " << 1 - Spills.size() / float(tot_evt) << std::endl
                  << std::endl;
        perc = 0;
    }

    // out of loops
    TCanvas *cSD = new TCanvas("cSD", "Silicon detectors", 900, 800);
    cSD->Divide(3, (nstrips - 2) / 2);

    // canvas
    for (int a = 0; a < (nstrips - 2) / 2; a++)
    {
        cSD->cd(a * 3 + 1);
        hSDs[a]->Draw("colz");
        cSD->cd(a * 3 + 2);
        hSDs[a]->ProjectionX()->Fit("gaus", "q");
        hSDs[a]->ProjectionX()->Draw("hist");
        cSD->cd(a * 3 + 3);
        hSDs[a]->ProjectionY()->Fit("gaus", "q");
        hSDs[a]->ProjectionY()->Draw("hist");
    }

    cSD->Update();

    // TCanvas *cbruco = new TCanvas("cbruco", "bruco canvas PH", 900, 800);
    // cbruco->Divide(3, 2);
    // cbruco->cd(1);
    // hhBruco->GetXaxis()->SetTitle("ROT-#theta_x [mrad]");
    // hhBruco->GetYaxis()->SetTitle("LG PH [ADC]");
    // hhBruco->Draw("colz");
    // cbruco->cd(2);
    // hhBruco->ProfileX()->Draw();
    // cbruco->cd(3);
    // hhBruco->ProfileY()->Draw();
    // cbruco->cd(4);
    // hvBruco->GetXaxis()->SetTitle("CRAD-#theta_y [mrad]");
    // hvBruco->GetYaxis()->SetTitle("LG PH [ADC]");
    // hvBruco->Draw("colz");
    // cbruco->cd(5);
    // hvBruco->ProfileX()->Draw();
    // cbruco->cd(6);
    // hvBruco->ProfileY()->Draw();

    // TCanvas *cbrucodefl = new TCanvas("cbrucodefl", "bruco canvas DEFL", 900, 800);
    // cbrucodefl->Divide(3, 2);
    // cbrucodefl->cd(1);
    // hthetaxBruco->GetXaxis()->SetTitle("ROT-#theta_x [mrad]");
    // hthetaxBruco->GetYaxis()->SetTitle("deflection X [mrad]");
    // hthetaxBruco->Draw("colz");
    // cbrucodefl->cd(2);
    // hthetaxBruco->ProfileX()->Draw();
    // cbrucodefl->cd(3);
    // hthetaxBruco->ProfileY()->Draw();
    // cbrucodefl->cd(4);
    // hthetayBruco->GetXaxis()->SetTitle("CRAD-#theta_y [mrad]");
    // hthetayBruco->GetYaxis()->SetTitle("deflection y [mrad]");
    // hthetayBruco->Draw("colz");
    // cbrucodefl->cd(5);
    // hthetayBruco->ProfileX()->Draw();
    // cbrucodefl->cd(6);
    // hthetayBruco->ProfileY()->Draw();

    // efficiency plot
    TH2F *hEff = (TH2F *)hEffnum->Clone("hEff");
    hEff->Divide(hEffden);
    TEfficiency *pEff = new TEfficiency(*hEffnum, *hEffden);

    // pEff->GetPaintedGraph()->GetXaxis()->SetTitle("ROT - #thetax [mrad]");
    // pEff->GetPaintedGraph()->GetYaxis()->SetTitle("CRAD - #thetay [mrad]");

    // auto c2 = new TCanvas("c2", "c2");
    // c2->Divide(2);
    // c2->cd(1);
    // hQtot[0]->GetXaxis()->SetTitle("charge on SD3x [ADC]");
    // hQtot[0]->Draw();
    // c2->cd(2);
    // hNclu[0]->GetXaxis()->SetTitle("N cluster on SD3x [counts]");
    // hNclu[0]->Draw();

    // auto c4 = new TCanvas("c4", "c4");
    // c4->Divide(2);
    // c4->cd(1);
    // double minVal = 1e9;
    // for (int i = 1; i <= pROTCRAD->GetNbinsX(); ++i)
    // {
    //     for (int j = 1; j <= pROTCRAD->GetNbinsY(); ++j)
    //     {
    //         double content = pROTCRAD->GetBinContent(i, j);
    //         if (content > 0 && content < minVal)
    //         {
    //             minVal = content;
    //         }
    //     }
    // }
    // pROTCRAD->SetMinimum(minVal);
    // gStyle->SetPalette(kRainBow);
    // pROTCRAD->Draw("colz");
    // pROTCRAD->GetXaxis()->SetTitle("ROT [mrad]");
    // pROTCRAD->GetYaxis()->SetTitle("CRAD [mrad]");
    // c4->cd(2);

    // double minVal3 = 1e9;
    // for (int i = 1; i <= hROTCRAD->GetNbinsX(); ++i)
    // {
    //     for (int j = 1; j <= hROTCRAD->GetNbinsY(); ++j)
    //     {
    //         double content3 = hROTCRAD->GetBinContent(i, j);
    //         if (content3 > 0 && content3 < minVal3)
    //         {
    //             minVal3 = content3;
    //         }
    //     }
    // }
    // hROTCRAD->SetMinimum(minVal3);
    // gStyle->SetPalette(kRainBow);
    // hROTCRAD->Draw("colz");
    // hROTCRAD->GetXaxis()->SetTitle("ROT [mrad]");
    // hROTCRAD->GetYaxis()->SetTitle("CRAD [mrad]");

    auto c7 = new TCanvas("c7", "c7");
    c7->Divide(3, 2);
    c7->cd(1);
    hhTheta->Fit("gaus", "q");
    hhTheta->GetXaxis()->SetTitle("#thetax in [mrad]");
    c7->cd(2);
    hhthetaout->Fit("gaus", "q");
    hhthetaout->GetXaxis()->SetTitle("#thetax out [mrad]");
    c7->cd(3);
    hhthetadiff->Fit("gaus", "q");
    hhthetadiff->GetXaxis()->SetTitle("#thetax deflection [mrad]");
    c7->cd(4);
    hvTheta->Fit("gaus", "q");
    hhTheta->GetXaxis()->SetTitle("#thetay in [mrad]");
    c7->cd(5);
    hvthetaout->Fit("gaus", "q");
    hvthetaout->GetXaxis()->SetTitle("#thetay out [mrad]");
    c7->cd(6);
    hvthetadiff->Fit("gaus", "q");
    hvthetadiff->GetXaxis()->SetTitle("#thetay deflection [mrad]");

    auto c8 = new TCanvas("c8", "c8");
    c8->Divide(3);
    c8->cd(1);
    hSDs[2]->Draw("colz");
    c8->cd(2);
    hSDs[2]->ProjectionY()->Fit("gaus", "q");
    hSDs[2]->ProjectionY()->Draw("hist");
    c8->cd(3);
    hSDs[2]->ProjectionX()->Fit("gaus", "q");
    hSDs[2]->ProjectionX()->Draw("hist");

    auto c9 = new TCanvas("c9", "c9");
    hdeflxy->Draw("colz");

    std::string filename = "data/Run" + run_number[0] + ".root";
    TFile f2(filename.c_str(), "recreate");
    hhthetadiff->Write(("hdefl" + run_number[0]).c_str());
    hvthetadiff->Write(("vdefl" + run_number[0]).c_str());
    hSDs[2]->Write(("sd3_pos" + run_number[0]).c_str());
    hdeflxy->Write(("hdeflxy" + run_number[0]).c_str());
    pdeflxy->Write(("pdeflxy" + run_number[0]).c_str());

    f2.Close();
    app.Run();
    return 0;
}
