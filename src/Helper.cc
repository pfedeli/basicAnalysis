
#include "../include/Helper.hh"
#include <cmath>

double Helper::GetCryPos(const double &theta, const double& SD1CRY){
        return (std::tan(theta)*SD1CRY); //if you putht theta horizontale you get x post, theta vertical y pos
}

double Helper::GetTheta(const double & x1, const double & x2, const double & dist){
    return (std::atan2((x2 - x1), dist)); //cm cm cm -> rad
}


double Helper::CalibrateToGev(const double & val, const double & m, const double & q){
    return(val*m +q);
}

double Helper::GetQuadratureSum(const double & val1, const double & val2){
    return sqrt(val1*val1 + val2*val2);
}

