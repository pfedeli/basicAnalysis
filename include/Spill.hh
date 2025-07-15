#ifndef SPILL_HH
#define SPILL_HH

#include "Histograms.hh"
#include "Event.hh"
#include <vector>
#include <string>

class Spill{
    public:
        Spill();
        virtual ~Spill();
        bool LoadFromASCII(const std::string& fileSpill, const std::string& configPath, Histograms& hist);
        bool LoadFromRoot(){return false;} //to be implemented for the future
        size_t GetNEvents() const { return spill_.size(); }
        const std::vector<Event> &GetSpill() const {return spill_;}
        const int &GetTotalEvts() const {return totalEvts_;}

    private:
        std::vector<Event> spill_;
        int totalEvts_=0;

};

#endif