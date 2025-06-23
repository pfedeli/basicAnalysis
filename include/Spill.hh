#ifndef SPILL_HH
#define SPILL_HH

#include "Event.hh"
#include <vector>
#include <string>

class Spill{
    public:
        Spill();
        virtual ~Spill();
        bool LoadFromASCII(const std::string& fileSpill, const std::string& configPath);
        bool LoadFromRoot(){return false;} //to be implemented for the future
        size_t GetNEvents() const { return spill_.size(); }
        const std::vector<Event> &GetSpill() const {return spill_;}
    
    private:
        std::vector<Event> spill_;

};

#endif