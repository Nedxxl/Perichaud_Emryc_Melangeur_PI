#ifndef TTCONFIG_HPP
#define TTCONFIG_HPP

#include <string>
#include <iostream>
#include <fstream>

class TConfiguration
{
private:
    std ::string parametreConfiguration;

public:
    TConfiguration(std ::string nomFichierConfiguration);
    ~TConfiguration();
    std ::string getParametreConfiguration(void);
};

#endif // TTASK1_HPP