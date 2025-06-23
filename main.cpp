#include "../include/AllRuns.hh"

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " config.txt" << std::endl;
        return 1;
    }

    std::string configPath = argv[1];

    AllRuns allRuns;
    if (!allRuns.LoadAllFromASCII(configPath)) {
        std::cerr << "Errore durante il caricamento delle run." << std::endl;
        return 1;
    }

    // Ad esempio: numero totale di spill
    size_t totalSpills = 0;
    for (const auto& run : allRuns.GetRuns()) {
        totalSpills += run.GetSpills().size();
    }

    std::cout << "Totale spill caricati: " << totalSpills << std::endl;

    return 0;
}
