#ifndef RUN_HH
#define RUN_HH

#include "Spill.hh"
#include "Config.hh"

#include <string>
#include <vector>

class Run {
public:
    Run();
    ~Run();

    // Carica gli spill da file ASCII relativi a una singola run
    bool FillSpillsFromASCII(const std::string& run_id, const std::string& configPath);

    // Accesso agli spill
    const std::vector<Spill>& GetSpills() const { return run_; }
    std::vector<Spill>& GetSpills() { return run_; }
    size_t GetNSpills() const { return run_.size(); }
    int GetNEvents() const { return nevt_;}
    int GetNPassed() const { return totalPassed_;}

    // Eventuali altri metodi (analisi, salvataggio, etc.) possono essere aggiunti qui

private:
    std::vector<Spill> run_;  // Contenitore per tutti gli spill della run
    int nevt_=0;
    int totalPassed_=0;
};

#endif // RUN_HH
