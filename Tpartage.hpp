#ifndef TPARTAGE_HPP
#define TPARTAGE_HPP

#include "mutex.hpp"
#include "singleton.hpp"
#include "sem.hpp"
#include <stdint.h>
#include <string.h>

class TPartage : public TSingleton<TPartage>
{
    friend class TSingleton<TPartage>;

private:
    bool moteurA;
    bool moteurB;
    bool moteurC;
    bool mode;
    bool etatA;
    bool etatB;
    bool etatC;
    double poidBalance;
    bool recetteAuto;
    int poidA;
    int poidB;
    int poidC;

    bool changeA;
    bool changeB;
    bool changeC;
    bool changeEtatA;
    bool changeEtatB;
    bool changeEtatC;
    bool poidBalanceChange;
    bool recetteAutoChange;
    bool recetteValueChange;

public:
    TMutex mutex;

    TPartage();
    ~TPartage();

    void setMoteur(char moteur, bool val, bool change);
    void setMode(bool val);
    void setEtat(char moteur, bool val);
    void setPoidBalance(double poid);
    void setRecetteAuto(bool val);
    void setPoid(char vis, int poids);

    bool getMoteur(char moteur);
    bool getMode();
    bool getEtat(char moteur);
    double getPoidBalance();
    bool getRecetteAuto();
    int getPoid(char vis);

    bool isChange(char moteur);
    bool isChangeEtat(char moteur);
    bool isChangePoidBalance();
    bool isChangeRecetteAuto();
    bool isChangeRecetteValue();
};

#endif // TPARTAGE_HPP