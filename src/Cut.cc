#include "Cut.hh"
#include <iostream>

Cut::Cut()
    : cut_x1_(0), cut_x2_(10), cut_x_active_(false),
      cut_y1_(0), cut_y2_(10), cut_y_active_(false),
      ene_cut_(0), ene_cut_active_(false),
      theta_crit_(100), theta_crit_active_(false),
      cut_eneLG_(0), cut_eneLG_active_(false),
      clucut_active_(false), timecut_(10000), timecut_active_(false)
{}

Cut::~Cut() {}

bool Cut::LoadFromConfig(const std::string& configPath)
{
    Config config;
    if (!config.Load(configPath)) {
        std::cerr << "[Cut::LoadFromConfig] Errore nel caricamento del file config: " << configPath << std::endl;
        return false;
    }

    if (config.HasKey("cut_x1") && config.HasKey("cut_x2")) {
        cut_x1_ = config.GetDouble("cut_x1");
        cut_x2_ = config.GetDouble("cut_x2");
        cut_x_active_ = true;
    }

    if (config.HasKey("cut_y1") && config.HasKey("cut_y2")) {
        cut_y1_ = config.GetDouble("cut_y1");
        cut_y2_ = config.GetDouble("cut_y2");
        cut_y_active_ = true;
    }

    if (config.HasKey("ene_cut")) {
        ene_cut_ = config.GetDouble("ene_cut");
        ene_cut_active_ = true;
    }

    if (config.HasKey("theta_crit")) {
        theta_crit_ = config.GetDouble("theta_crit");
        theta_crit_active_ = true;
    }

    if (config.HasKey("cut_eneLG")) {
        cut_eneLG_ = config.GetDouble("cut_eneLG");
        cut_eneLG_active_ = true;
    }

    if (config.HasKey("clucut_active")) {
        clucut_active_ = (config.GetInt("clucut_active", 0) != 0);
    }

    if (config.HasKey("timecut")) {
        timecut_ = config.GetDouble("timecut");
        timecut_active_ = true;
    } 

    return true;
}

bool Cut::PassesCut(double x, double y, double eneCH, int nclusx, int nclusy,
                    int eneLG, double theta, double CHtime, double LGtime) const
{
    bool poscuts = true;
    bool CHenecuts = true;
    bool singleclucut = true;
    bool ENELG = true;
    bool timecut = true;
    bool thcut = true;

    if (cut_x_active_)
        poscuts = (x > cut_x1_ && x < cut_x2_);
    if (cut_y_active_)
        poscuts = poscuts && (y > cut_y1_ && y < cut_y2_);
    if (ene_cut_active_)
        CHenecuts = (eneCH >= ene_cut_);
    if (cut_eneLG_active_)
        ENELG = (eneLG > cut_eneLG_);
    if (theta_crit_active_)
        thcut = (theta < 2 * theta_crit_) && (theta > -2 * theta_crit_);
    if (clucut_active_)
        singleclucut = (nclusx == 1 && nclusy == 1);
    if (timecut_active_)
        timecut = (LGtime - CHtime) < timecut_ && (LGtime - CHtime) > 0;
    
    return (poscuts && CHenecuts && singleclucut && timecut && thcut && ENELG);
}
