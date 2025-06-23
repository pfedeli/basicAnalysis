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

private:
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
};

#endif
