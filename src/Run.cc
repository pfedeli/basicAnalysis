#include "../include/Run.hh"
#include "../include/Spill.hh"

#include <filesystem>
#include <regex>
#include <iostream>
#include <vector>
#include <string>

namespace fs = std::filesystem;

Run::Run() {}
Run::~Run() {}

    bool Run::FillSpillsFromASCII(const std::string& run_id, const std::string& configPath, Histograms &hist)
{
    bool debug = true;
    size_t total_file_to_read =0;
    int perc_completing=0;
    if(debug) std::cout << "[Run::FillSpillsFromASCII] Caricamento run " << run_id << " da config: " << configPath << std::endl;

    Config config;
    if (!config.Load(configPath)) {
        std::cerr << "Errore: impossibile caricare il file di configurazione.\n";
        return false;
    }
    std::cout << "[Run::FillSpillsFromASCII] Config caricata correttamente\n";

    std::string directory = config.GetString("run_dir", ".");
    if(debug) std::cout << "[Run::FillSpillsFromASCII] Directory: " << directory << std::endl;

    std::regex pattern("run" + run_id + "_\\d{6}\\.dat");
    std::vector<fs::path> files_to_read;

    for (const auto& entry : fs::directory_iterator(directory))
    {
        std::string filename = entry.path().filename().string();
        if (std::regex_match(filename, pattern))
        {
            files_to_read.push_back(entry.path());
        }
    }
    total_file_to_read = files_to_read.size();
    if(debug) std::cout << "[Run::FillSpillsFromASCII] Trovati " << total_file_to_read << " file\n";

    if (files_to_read.empty())
    {
        std::cerr << "[Run::FillSpillsFromASCII] Nessun file trovato per run " << run_id << std::endl;
        return false;
    }

    for (const auto& file : files_to_read)
    {
        Spill spill;
        if (!spill.LoadFromASCII(file.string(), configPath, hist)) {
            std::cerr << "[Run::FillSpillsFromASCII] Errore nella lettura di " << file << std::endl;
            continue;
        }
        totalPassed_ += spill.GetNEvents();
        nevt_+= spill.GetTotalEvts();
        run_.push_back(spill);
        perc_completing++;
        std::cout << "completing run at: " << perc_completing/float(total_file_to_read)*100.0 << "%" << std::endl;
    }

    if(debug) std::cout << "[Run::FillSpillsFromASCII] Completato. Totale spill caricati: " << run_.size() << " Eventi cut/total: " << totalPassed_ <<"/"<<nevt_ <<" = "<< float(totalPassed_) / float(nevt_)*100 <<"%"<< std::endl;
    return true;
}
