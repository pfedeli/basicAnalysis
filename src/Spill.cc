#include "../include/Spill.hh"
#include "../include/Event.hh"
#include "../include/Config.hh"
#include "../include/Cut.hh"

#include <fstream>
#include <sstream>
#include <iostream>

Spill::Spill() {}

Spill::~Spill() {}

bool Spill::LoadFromASCII(const std::string &fileSpill, const std::string &configPath)
{
    Config config;
    if (!config.Load(configPath))
    {
        std::cerr << "Errore nella lettura del file di configurazione.\n";
        return false;
    }

    Cut cut;
    if (!cut.LoadFromConfig(configPath))
    {
        std::cerr << "Errore nel caricamento dei cut dal config.\n";
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

    std::ifstream file(fileSpill);
    if (!file.is_open())
    {
        std::cerr << "Errore: impossibile aprire il file " << fileSpill << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        Event evt;
        std::istringstream iss(line);

        std::vector<double> pos(nstrips);
        for (int i = 0; i < nstrips; ++i)
            iss >> pos[i];
        evt.SetPos(pos);

        std::vector<int> ncluvec(nclu);
        for (int i = 0; i < nclu; ++i)
            iss >> ncluvec[i];
        evt.SetNclu(ncluvec);

        std::vector<double> qtot(nqtot);
        for (int i = 0; i < nqtot; ++i)
            iss >> qtot[i];
        evt.SetQtot(qtot);

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

        std::vector<double> gonio(ngonio);
        for (int i = 0; i < ngonio; ++i)
            iss >> gonio[i];
        evt.SetInfogonio(gonio);

        std::vector<std::string> plus(ninfoplus);
        for (int i = 0; i < ninfoplus; ++i)
            iss >> plus[i];
        
        
        evt.SetInfoplus(plus);

        spill_.push_back(evt);
    }

    return true;
}
