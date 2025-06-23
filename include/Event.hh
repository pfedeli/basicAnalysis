#ifndef EVENT_HH
#define EVENT_HH

#include <vector>
#include <string>
#include <cmath>

class Event
{
public:
    Event();
    virtual ~Event();

    // getter
    const std::vector<double> &Getpos() const { return pos_; }
    const std::vector<int> &Getnclu() const { return nclu_; }
    const std::vector<double> &Getqtot() const { return qtot_; }
    const std::vector<int> &Getbaseline() const { return baseline_; }
    const std::vector<int> &Gettimes() const { return times_; }
    const std::vector<int> &GetPH() const { return PH_; }
    const std::vector<double> &Getinfogonio() const { return infogonio_; }
    const std::vector<std::string> &Getinfoplus() const { return infoplus_; }
    double GetXcry() const { return xcry_; }
    double GetYcry() const { return ycry_; }
    double GetThetaXIn() const { return thetaxin_; }
    double GetThetaYIn() const { return thetayin_; }
    double GetThetaXOut() const { return thetaxout_; }
    double GetThetaYOut() const { return thetayout_; }
    double GetThetaDeflX() const { return thetaxout_ - thetaxin_; }
    double GetThetaDeflY() const { return thetayout_ - thetayin_; }
    double GetThetaDeflection() const
    {
        double dx = thetaxout_ - thetaxin_;
        double dy = thetayout_ - thetayin_;
        return std::sqrt(dx * dx + dy * dy);
    }

    // setter
    void SetPos(const std::vector<double> &pos) { pos_ = pos; }
    void SetNclu(const std::vector<int> &nclu) { nclu_ = nclu; }
    void SetQtot(const std::vector<double> &qtot) { qtot_ = qtot; }
    void SetBaseline(const std::vector<int> &baseline) { baseline_ = baseline; }
    void SetTimes(const std::vector<int> &times) { times_ = times; }
    void SetPH(const std::vector<int> &ph) { PH_ = ph; }
    void SetInfogonio(const std::vector<double> &infogonio) { infogonio_ = infogonio; }
    void SetInfoplus(const std::vector<std::string> &infoplus) { infoplus_ = infoplus; }
    void SetXYcry(const double &x, const double &y)
    {
        xcry_ = x;
        ycry_ = y;
    };
    void SetThetain(const double &Tx, const double &Ty)
    {
        thetaxin_ = Tx;
        thetayin_ = Ty;
    }
    void SetThetaout(const double &Tx, const double &Ty)
    {
        thetaxout_ = Tx;
        thetayout_ = Ty;
    }

    // functions
    void CalculateAngles(double SD1SD2_Z, double SD1CRY_Z, double CRYSD3_Z);

private:
    std::vector<double> pos_;           // silicon dec position
    std::vector<int> nclu_;             // clusters on silicon det
    std::vector<double> qtot_;          // charge on silicon det
    std::vector<int> baseline_;         // waform baseline digi
    std::vector<int> times_;            // waform time digi
    std::vector<int> PH_;               // waform pulse height digi
    std::vector<double> infogonio_;     // goniometer info
    std::vector<std::string> infoplus_; // other info

    // inferred quantities
    double xcry_;
    double ycry_;
    double thetaxin_;
    double thetayin_;
    double thetaxout_;
    double thetayout_;
};

#endif