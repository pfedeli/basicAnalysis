#ifndef CUT_HH
#define CUT_HH

#include "Config.hh"
#include <string>

class Cut {
public:
    Cut();
    ~Cut();

    bool LoadFromConfig(const std::string& configPath);

    bool PassesCut(double x, double y, double eneCH, int nclusx, int nclusy,
                   int eneLG, double theta, double CHtime, double LGtime) const;
    double GetCutVal(const std::string& val) const {
        if (val == "timecut") return timecut_;
        if (val == "theta_crit") return theta_crit_;
        if (val == "cut_eneLG") return cut_eneLG_;
        if (val == "ene_cut") return ene_cut_;
        if (val == "cut_x1") return cut_x1_;
        if (val == "cut_x2") return cut_x2_;
        if (val == "cut_y1") return cut_y1_;
        if (val == "cut_y2") return cut_y2_;
        throw std::invalid_argument("Unknown cut value requested: " + val);
    }

private:

    bool cutflag_;

    double cut_x1_, cut_x2_;
    bool cut_x_active_;

    double cut_y1_, cut_y2_;
    bool cut_y_active_;

    double ene_cut_;
    bool ene_cut_active_;

    double theta_crit_;
    bool theta_crit_active_;

    double cut_eneLG_;
    bool cut_eneLG_active_;

    bool clucut_active_;

    double timecut_;
    bool timecut_active_;
};

#endif
