#ifndef TPARTAGE_HPP
#define TPARTAGE_HPP

#include "mutex.hpp"
#include "singleton.hpp"
#include "sem.hpp"
#include <stdint.h>
#include <string.h>
#include <deque>

class TPartage : public TSingleton<TPartage>
{
    friend class TSingleton<TPartage>;
public:
    struct recette_t
    {
        int poidA;
        int poidB;
        int poidC;
    };

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

    std::deque<recette_t> recetteVal;



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
    void addRecette(int poidA, int poidB, int poidC);
    void removeRecette();

    bool getMoteur(char moteur);
    bool getMode();
    bool getEtat(char moteur);
    double getPoidBalance();
    bool getRecetteAuto();
    int getPoid(char vis);
    recette_t getRecetteValue();

    bool isChange(char moteur);
    bool isChangeEtat(char moteur);
    bool isChangePoidBalance();
    bool isChangeRecetteAuto();
    bool isChangeRecetteValue();
};

#endif // TPARTAGE_HPP