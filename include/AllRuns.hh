#ifndef ALLRUNS_HH
#define ALLRUNS_HH

#include "Histograms.hh"
#include "Run.hh"
#include "Config.hh"

#include <iostream>
#include <string>
#include <vector>

class AllRuns {
public:
    AllRuns();
    ~AllRuns();

    // Carica tutte le run elencate nel config
    bool LoadAllFromASCII(const std::string& configPath);
    const std::vector<Run>& GetRuns() const { return runs_; }
    std::vector<Run>& GetRuns() { return runs_; }

private:
    std::vector<Run> runs_;
    int nevt_AR_=0;
    int totalPassed_AR_=0;
    Histograms histos_;
};

#endif // ALLRUNS_HH
