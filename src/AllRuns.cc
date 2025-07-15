#include "../include/AllRuns.hh"
#include <iostream>

AllRuns::AllRuns() {}
AllRuns::~AllRuns() {}

bool AllRuns::LoadAllFromASCII(const std::string& configPath)
{
    bool debug = true;
    std::cout << "[AllRuns::LoadAllFromASCII] Caricamento config da: " << configPath << std::endl;
    Config config;
    if (!config.Load(configPath)) {
        std::cerr << "Errore: impossibile caricare il file di configurazione.\n";
        return false;
    }

    std::vector<std::string> run_ids = config.GetStringList("run_list", ',');

    if (run_ids.empty()) {
        std::cerr << "[AllRuns::LoadAllFromASCII] Lista di run vuota nel config.\n";
        return false;
    }

    for (const auto& run_id : run_ids) {
        if(debug) std::cout << "[AllRuns::LoadAllFromASCII] Caricamento run " << run_id << std::endl;
        Run run;
        if (!run.FillSpillsFromASCII(run_id, configPath)) {
            std::cerr << "[AllRuns::LoadAllFromASCII] Errore nella lettura della run " << run_id << std::endl;
            continue;
        }
        runs_.push_back(run);
    }

    if(debug) std::cout << "[AllRuns::LoadAllFromASCII] Completato. Run caricate: " << runs_.size() << std::endl;
    return true;
}
