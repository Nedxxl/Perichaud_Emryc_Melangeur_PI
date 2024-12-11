#include "Tconfig.hpp"

TConfiguration::TConfiguration(std::string nomFichierConfiguration)
{
    parametreConfiguration = nomFichierConfiguration;
}

TConfiguration::~TConfiguration()
{
}

std::string TConfiguration::getParametreConfiguration(void)
{
    std::string fileName = parametreConfiguration;
    std::ifstream configFile(fileName);

    if (!configFile.is_open()) {
        std::ofstream newConfigFile(fileName);
        newConfigFile.close();
        configFile.open(fileName);
    }

    std::string firstLine;
    if (std::getline(configFile, firstLine)) {
        configFile.close();
        return firstLine;
    }

    configFile.close();
    return "";
}
