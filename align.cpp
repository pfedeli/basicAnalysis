#include "../include/AllRuns.hh"
#include "../include/Histograms.hh"
#include "../include/Config.hh"

#include <TCanvas.h>
#include <TH1F.h>
#include <TApplication.h>
#include <TF1.h>


int main(int argc, char **argv)
{

    if (argc < 2)
    {
        std::cerr << "Uso: " << argv[0] << " config.txt" << std::endl;
        return 1;
    }

    std::string configPath = argv[1];
    Histograms myhists;
    TApplication app("app", &argc, argv);

    AllRuns allRuns;
    if (!allRuns.LoadAllFromASCII(configPath, myhists))
    {
        std::cerr << "Errore durante il caricamento delle run." << std::endl;
        return 1;
    }

    Config config;
    if (!config.Load(configPath))
    {
        std::cerr << "Errore: impossibile caricare il file di configurazione.\n";
        return false;
    }

    std::vector<std::string> run_ids = config.GetStringList("run_list", ',');
    if (config.GetInt("save"))
    {
        myhists.Save("prealign");
    }

    myhists.Draw();
    TH2F *hSD1 = dynamic_cast<TH2F *>(myhists.GetHist("hSD1"));
    TH2F *hSD2 = dynamic_cast<TH2F *>(myhists.GetHist("hSD2"));
    TH2F *hSD3 = dynamic_cast<TH2F *>(myhists.GetHist("hSD3"));
    TH1D *hSD1_projX = nullptr;
    TH1D *hSD1_projY = nullptr;
    TH1D *hSD2_projX = nullptr;
    TH1D *hSD2_projY = nullptr;
    TH1D *hSD3_projX = nullptr;
    TH1D *hSD3_projY = nullptr;

    hSD1_projX = hSD1->ProjectionX();
    hSD1_projY = hSD1->ProjectionY();
    hSD2_projX = hSD2->ProjectionX();
    hSD2_projY = hSD2->ProjectionY();
    hSD3_projX = hSD3->ProjectionX();
    hSD3_projY = hSD3->ProjectionY();

    TH1F *hhtheta = dynamic_cast<TH1F *>(myhists.GetHist("hhTheta"));
    TH1F *hvtheta = dynamic_cast<TH1F *>(myhists.GetHist("hvTheta"));

    std::cout << "****************************************************************" << std::endl;
    std::cout << "alignment SD1x: " << hSD1_projX->GetMean()- hSD2_projX->GetMean() << std::endl 
              << "alignment SD1y: " << hSD1_projY->GetMean()- hSD2_projY->GetMean() << std::endl
              << "alignment SD3x: " << hSD3_projX->GetMean()- hSD2_projX->GetMean() << std::endl
              << "alignment SD3x: " << hSD3_projY->GetMean()- hSD2_projY->GetMean() << std::endl;
    std::cout << "****************************************************************" << std::endl;
    std::cout << "beam divergence X: " << hhtheta->GetFunction("gaus")->GetParameter(2) << "mrad" << std::endl
              << "beam divergence Y: " << hvtheta->GetFunction("gaus")->GetParameter(2) << "mrad" << std::endl;
    std::cout << "****************************************************************" << std::endl;

    app.Run();

    return 0;
}
