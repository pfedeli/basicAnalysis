#include "../include/Event.hh"
#include "../include/Config.hh"

#include <cmath>
#include<iostream>

Event::Event() {
}

Event::~Event() {
}

void Event::CalculateAngles(double SD1SD2_Z, double SD1CRY_Z, double CRYSD3_Z) {
    if (pos_.size() < 6) {
        std::cerr << "Errore: pos_ contiene meno di 6 elementi, impossibile calcolare angoli.\n";
        return;
    }
    double thetax_in = std::atan((pos_[2] - pos_[0]) / SD1SD2_Z) * 1000.0; //mrad
    double thetay_in = std::atan((pos_[3] - pos_[1]) / SD1SD2_Z) * 1000.0;

    SetThetain(thetax_in, thetay_in);

    double xcry = pos_[0] + (pos_[2] - pos_[0]) / SD1SD2_Z * SD1CRY_Z;
    double ycry = pos_[1] + (pos_[3] - pos_[1]) / SD1SD2_Z * SD1CRY_Z;

    SetXYcry(xcry, ycry);

    double thetax_out = std::atan((pos_[4] - xcry) / CRYSD3_Z) * 1000.0;
    double thetay_out = std::atan((pos_[5] - ycry) / CRYSD3_Z) * 1000.0;

    SetThetaout(thetax_out, thetay_out);
}

void Event::CalibrateToGev(int idx, double m, double q) {
    energy_ = PH_.at(idx)*m+q;
}
