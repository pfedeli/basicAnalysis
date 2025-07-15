#include "../include/AllRuns.hh"
#include "../include/Histograms.hh"
#include "../include/Config.hh"

#include <TCanvas.h>
#include <TH1F.h>
#include <TApplication.h>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " config.txt" << std::endl;
        return 1;
    }

    std::string configPath = argv[1];
    Histograms myhists;
    TApplication app("app", &argc, argv);
    AllRuns allRuns;
    if (!allRuns.LoadAllFromASCII(configPath, myhists)) {
        std::cerr << "Errore durante il caricamento delle run." << std::endl;
        return 1;
    }

    Config config;
    if (!config.Load(configPath)) {
        std::cerr << "Errore: impossibile caricare il file di configurazione.\n";
        return false;
    }

    std::vector<std::string> run_ids = config.GetStringList("run_list", ',');
    myhists.Save(run_ids[0]);
    myhists.Draw();
    app.Run();

    return 0;
}
