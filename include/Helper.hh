#ifndef HELPER_HH
#define HELPER_HH

class Helper
{
public:
    Helper(){};
    ~Helper(){};

    double GetCryPos(const double &theta, const double& SD2CRY);
    double GetTheta(const double & x1, const double & x2, const double & dist);
    double CalibrateToGev(const double & val, const double & m, const double & q);
    double GetQuadratureSum(const double & val1, const double & val2);

private:
 
};

    


#endif