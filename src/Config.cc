#include "Config.hh"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

bool Config::Load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        //std::cerr << "Errore: impossibile aprire config " << filename << std::endl;
        return false;
    }

    std::string line;
    //std::cout << "[Config::Load] Inizio caricamento da file: " << filename << std::endl;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            configMap_[key] = value;
            //std::cout << "[Config::Load] Chiave: '" << key << "' Valore: '" << value << "'" << std::endl;
        }
    }
    //std::cout << "[Config::Load] Caricamento completato con " << configMap_.size() << " coppie chiave-valore." << std::endl;
    return true;
}


int Config::GetInt(const std::string& key, int defaultVal) const {
    auto it = configMap_.find(key);
    if (it != configMap_.end()) {
        return std::stoi(it->second);
    }
    return defaultVal;
}

float Config::GetDouble(const std::string& key, float defaultVal) const {
    auto it = configMap_.find(key);
    if (it != configMap_.end()) {
        return std::stof(it->second);
    }
    return defaultVal;
}

std::string Config::GetString(const std::string& key, const std::string& defaultVal) const {
    auto it = configMap_.find(key);
    if (it != configMap_.end()) {
        return it->second;
    }
    return defaultVal;
}

std::vector<std::string> Config::GetStringList(const std::string& key, const char delimiter) const {
    std::vector<std::string> result;
    auto it = configMap_.find(key);
    if (it != configMap_.end()) {
        std::stringstream ss(it->second);
        std::string item;
        while (std::getline(ss, item, delimiter)) {
            result.push_back(item);
        }
    }
    return result;

}

bool Config::HasKey(const std::string& key) const
{
    return configMap_.find(key) != configMap_.end();
}