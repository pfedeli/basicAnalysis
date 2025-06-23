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

bool Run::FillSpillsFromASCII(const std::string& run_id, const std::string& configPath)
{
    std::cout << "[Run::FillSpillsFromASCII] Caricamento run " << run_id << " da config: " << configPath << std::endl;

    nevt_ = 0; // azzera il contatore eventi

    Config config;
    if (!config.Load(configPath)) {
        std::cerr << "Errore: impossibile caricare il file di configurazione.\n";
        return false;
    }
    std::cout << "[Run::FillSpillsFromASCII] Config caricata correttamente\n";

    std::string directory = config.GetString("run_dir", ".");
    std::cout << "[Run::FillSpillsFromASCII] Directory: " << directory << std::endl;

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
    std::cout << "[Run::FillSpillsFromASCII] Trovati " << files_to_read.size() << " file\n";

    if (files_to_read.empty())
    {
        std::cerr << "[Run::FillSpillsFromASCII] Nessun file trovato per run " << run_id << std::endl;
        return false;
    }

    for (const auto& file : files_to_read)
    {
        Spill spill;
        if (!spill.LoadFromASCII(file.string(), configPath)) {
            std::cerr << "[Run::FillSpillsFromASCII] Errore nella lettura di " << file << std::endl;
            continue;
        }

        nevt_ += spill.GetNEvents();
        run_.push_back(spill);
    }

    std::cout << "[Run::FillSpillsFromASCII] Completato. Totale spill caricati: " << run_.size() << " Totale eventi nella run: " << nevt_ << std::endl;
    return true;
}
