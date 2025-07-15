#include "../include/AllRuns.hh"

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
    TApplication app("app", &argc, argv);
    AllRuns allRuns;
    if (!allRuns.LoadAllFromASCII(configPath)) {
        std::cerr << "Errore durante il caricamento delle run." << std::endl;
        return 1;
    }

    app.Run();

    return 0;
}
