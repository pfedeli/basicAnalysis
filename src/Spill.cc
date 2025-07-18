#include "../include/Spill.hh"
#include "../include/Event.hh"
#include "../include/Config.hh"
#include "../include/Cut.hh"
#include "../include/Histograms.hh"

#include <fstream>
#include <sstream>
#include <iostream>

Spill::Spill() {}

Spill::~Spill() {}

bool Spill::LoadFromASCII(const std::string &fileSpill, const std::string &configPath, Histograms& hist)
{
    const bool debug = false;  // <<< Cambia in false per disabilitare le stampe
    totalEvts_=0;

    if (debug) std::cout << "[Spill::LoadFromASCII] Loading config from: " << configPath << std::endl;

    Config config;
    if (!config.Load(configPath))
    {
        std::cerr << "[Spill::LoadFromASCII] Errore nella lettura del file di configurazione.\n";
        return false;
    }

    Cut cut;
    if (!cut.LoadFromConfig(configPath))
    {
        std::cerr << "[Spill::LoadFromASCII] Errore nel caricamento dei cut dal config.\n";
        return false;
    }
    int nstrips = config.GetInt("nstrips");
    int nclu = config.GetInt("nclu");
    int nqtot = config.GetInt("nqtot");
    int ndigi = config.GetInt("ndigi");
    int ngonio = config.GetInt("ngonio");
    int ninfoplus = config.GetInt("ninfoplus");
    double SD1SD2_Z = config.GetDouble("SD1SD2_Z");
    double SD1CRY_Z = config.GetDouble("SD1CRY_Z");
    double CRYSD3_Z = config.GetDouble("CRYSD3_Z");
    double SD1xcorr = config.GetDouble("SD1xcorr");
    double SD1ycorr = config.GetDouble("SD1ycorr");
    double SD3xcorr = config.GetDouble("SD3xcorr");
    double SD3ycorr = config.GetDouble("SD3ycorr");
    double m = config.GetDouble("m");
    double q = config.GetDouble("q");

    if (debug)
    {
        std::cout << "[Spill::LoadFromASCII] Config parameters loaded:\n"
                  << "nstrips=" << nstrips << ", nclu=" << nclu << ", nqtot=" << nqtot << "\n"
                  << "ndigi=" << ndigi << ", ngonio=" << ngonio << ", ninfoplus=" << ninfoplus << "\n"
                  << "SD1SD2_Z=" << SD1SD2_Z << ", SD1CRY_Z=" << SD1CRY_Z << ", CRYSD3_Z=" << CRYSD3_Z << std::endl;
    }

    std::ifstream file(fileSpill);
    if (!file.is_open())
    {
        std::cerr << "Errore: impossibile aprire il file " << fileSpill << std::endl;
        return false;
    }

    if (debug) std::cout << "[Spill::LoadFromASCII] Reading file: " << fileSpill << std::endl;

    std::string line;
    int lineCount = 0;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        lineCount++;
        if (debug) std::cout << "\n[Spill::LoadFromASCII] Parsing line " << lineCount << std::endl;

        Event evt;
        std::istringstream iss(line);

        std::vector<double> pos(nstrips);
        for (int i = 0; i < nstrips; ++i)
            iss >> pos[i];
        pos[0]-=SD1xcorr;
        pos[1]-=SD1ycorr;
        pos[4]-=SD3xcorr;
        pos[5]-=SD3ycorr;

        evt.SetPos(pos);
        if (debug){
            std::cout << "Pos: ";
            for (int i = 0; i < nstrips; ++i){
                std::cout << pos[i] << " ";
            }
            std::cout << std::endl;
        }
        std::vector<int> ncluvec(nclu);
        for (int i = 0; i < nclu; ++i)
            iss >> ncluvec[i];
        evt.SetNclu(ncluvec);
        if (debug){
            std::cout << "nclu: ";
            for (int i = 0; i < nclu; ++i){
                std::cout << ncluvec[i] << " ";
            }
            std::cout << std::endl;
        }
        std::vector<double> qtot(nqtot);
        for (int i = 0; i < nqtot; ++i)
            iss >> qtot[i];
        evt.SetQtot(qtot);
        if (debug){
            std::cout << "qtot: ";
            for(int i = 0; i < nqtot; ++i){
                std::cout << qtot[i] << " ";
            }
            std::cout << std::endl;
        }

        std::vector<int> baseline(ndigi), times(ndigi), ph(ndigi);
        for (int i = 0; i < ndigi; ++i)
            iss >> baseline[i];
        for (int i = 0; i < ndigi; ++i)
            iss >> times[i];
        for (int i = 0; i < ndigi; ++i)
            iss >> ph[i];
        evt.SetBaseline(baseline);
        evt.SetTimes(times);
        evt.SetPH(ph);

        if (debug){
            std::cout << "baseline: ";
            for(int i = 0; i < ndigi; ++i){
                std::cout << baseline[i] << " ";
            }
            std::cout << std::endl;
            std::cout << "times: ";
            for(int i = 0; i < ndigi; ++i){
                std::cout << times[i] << " ";
            }
            std::cout << std::endl;
            std::cout << "ph: ";
            for(int i = 0; i < ndigi; ++i){
                std::cout << ph[i] << " ";
            }
            std::cout << std::endl;
        }

        std::vector<double> gonio(ngonio);
        for (int i = 0; i < ngonio; ++i)
            iss >> gonio[i];
        evt.SetInfogonio(gonio);
        if (debug){
            std::cout << "gonio: ";
            for(int i = 0; i < ngonio; ++i){
                std::cout << gonio[i] << " ";
            }
            std::cout << std::endl;
        }
        std::vector<std::string> plus(ninfoplus);
        for (int i = 0; i < ninfoplus; ++i)
            iss >> plus[i];
        evt.SetInfoplus(plus);
        if (debug){
            std::cout << "infoplus: ";
            for(int i = 0; i < ninfoplus; ++i){
                std::cout << plus[i] << " ";
            }
            std::cout << std::endl;
        }
        
        evt.CalculateAngles(SD1SD2_Z, SD1CRY_Z, CRYSD3_Z);
        evt.CalibrateToGev(2, m, q);

        if (debug)
        {
            std::cout << " ThetaDeflection=" << evt.GetThetaDeflection()
                      << ", Xcry=" << evt.GetXcry() << ", Ycry=" << evt.GetYcry() << std::endl;
        }

        if (cut.PassesCut(evt.GetXcry(), evt.GetYcry(), evt.GetPH()[0], evt.Getnclu()[0], evt.Getnclu()[1],
                          evt.GetPH()[2], evt.GetThetaIn(), evt.Gettimes()[0], evt.Gettimes()[2]))
        {
            spill_.push_back(evt);
            hist.Fill(evt);
            if (debug) std::cout << "Event accepted." << std::endl;
        }
        else
        {
            if (debug) std::cout << "Event rejected by cut." << std::endl;
        }
        totalEvts_++;
    }

    if (debug)
    {
        std::cout << "\n[Spill::LoadFromASCII] Total events processed: " << totalEvts_
                  << ", events passed cut: " << spill_.size() << std::endl;
    }

    return true;
}
