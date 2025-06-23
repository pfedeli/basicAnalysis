#ifndef CONFIG_HH
#define CONFIG_HH

#include <string>
#include <vector>
#include <unordered_map>

class Config {
public:
    bool Load(const std::string& filename);
    int GetInt(const std::string& key, int defaultVal = 0) const;
    float GetDouble(const std::string& key, float defaultVal =0) const;
    std::string GetString(const std::string& key, const std::string& defaultVal="") const;
    std::vector<std::string> GetStringList(const std::string& key, const char delimiter = ',') const;
    bool HasKey(const std::string& key) const;

private:
    std::unordered_map<std::string, std::string> configMap_;
};

#endif
