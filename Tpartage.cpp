#include "Tpartage.hpp"

TPartage::TPartage()
{
    moteurA = "off";
    moteurB = "off";
    moteurC = "off";
    mode = false;
    etatA = false;
    etatB = false;
    etatC = false;
    poidBalance = 0.0;
    recetteAuto = false;
    poidA = 0;
    poidB = 0;
    poidC = 0;

    changeA = false;
    changeB = false;
    changeC = false;
    recetteAutoChange = false;
    poidBalanceChange = false;
    changeEtatA = false;
    changeEtatB = false;
    changeEtatC = false;
    recetteValueChange = false;
}

TPartage::~TPartage()
{
}

void TPartage::setMoteur(char moteur, bool val, bool change)
{
    mutex.take();

    switch (moteur)
    {
    case 'A':
        moteurA = val;
        changeA = change;
        break;
    case 'B':
        moteurB = val;
        changeB = change;
        break;
    case 'C':
        moteurC = val;
        changeC = change;
        break;
    }

    mutex.release();
}

void TPartage::setMode(bool val)
{
    mutex.take();
    mode = val;
    mutex.release();
}

void TPartage::setEtat(char moteur, bool val)
{
    mutex.take();

    switch (moteur)
    {
    case 'A':
        etatA = val;
        changeEtatA = true;
        break;
    case 'B':
        etatB = val;
        changeEtatB = true;
        break;
    case 'C':
        etatC = val;
        changeEtatC = true;
        break;
    }

    mutex.release();
}

void TPartage::setPoidBalance(double poid)
{
    mutex.take();
    poidBalance = poid;
    poidBalanceChange = true;
    mutex.release();
}

void TPartage::setRecetteAuto(bool val)
{
    mutex.take();
    if (recetteAuto != val)
    {
        recetteAuto = val;
        recetteAutoChange = true;
    }
    mutex.release();
}

void TPartage::setPoid(char vis, int poids)
{
    mutex.take();

    switch (vis)
    {
    case 'A':
        poidA = poids;
        break;
    case 'B':
        poidB = poids;
        break;
    case 'C':
        poidC = poids;
        break;
    }
    recetteValueChange = true;
    mutex.release();
}

void TPartage::addRecette(int poidA, int poidB, int poidC)
{
    if (recetteVal.size() > 10)
    {
        return;
    }

    recette_t recette;
    recette.poidA = poidA;
    recette.poidB = poidB;
    recette.poidC = poidC;

    mutex.take();
    recetteVal.push_back(recette);
    mutex.release();
}

void TPartage::removeRecette()
{
    if (recetteVal.empty())
    {
        return;
    }
    mutex.take();
    recetteVal.pop_front();
    mutex.release();
}

TPartage::recette_t TPartage::getRecetteValue()
{
    recette_t recette = {0, 0, 0};
    mutex.take();

    if (!recetteVal.empty())
    {
        recette = recetteVal.front();
    }
    mutex.release();
    return recette;
}

bool TPartage::getMoteur(char moteur)
{
    mutex.take();
    bool ret = false;

    switch (moteur)
    {
    case 'A':
        ret = moteurA;
        break;
    case 'B':
        ret = moteurB;
        break;
    case 'C':
        ret = moteurC;
        break;
    }

    mutex.release();
    return ret;
}

bool TPartage::getMode()
{
    bool ret = false;
    mutex.take();
    ret = mode;
    mutex.release();
    return ret;
}

bool TPartage::getEtat(char moteur)
{
    mutex.take();
    bool ret = false;

    switch (moteur)
    {
    case 'A':
        ret = etatA;
        break;
    case 'B':
        ret = etatB;
        break;
    case 'C':
        ret = etatC;
        break;
    }

    mutex.release();
    return ret;
}

double TPartage::getPoidBalance(void)
{
    double poid;

    mutex.take();
    poid = poidBalance;
    mutex.release();

    return poid;
}

bool TPartage::getRecetteAuto()
{
    bool recette;

    mutex.take();
    recette = recetteAuto;
    mutex.release();

    return recette;
}

int TPartage::getPoid(char vis)
{
    int poid;

    mutex.take();

    switch (vis)
    {
    case 'A':
        poid = poidA;
        break;
    case 'B':
        poid = poidB;
        break;
    case 'C':
        poid = poidC;
        break;
    }

    mutex.release();
    return poid;
}

bool TPartage::isChange(char moteur)
{
    mutex.take();
    bool ret = false;

    switch (moteur)
    {
    case 'A':
        if (changeA)
        {
            changeA = false;
            ret = true;
        }
        break;
    case 'B':
        if (changeB)
        {
            changeB = false;
            ret = true;
        }
        break;
    case 'C':
        if (changeC)
        {
            changeC = false;
            ret = true;
        }
        break;
    }

    mutex.release();
    return ret;
}

bool TPartage::isChangeEtat(char moteur)
{
    bool ret = false;
    mutex.take();

    switch (moteur)
    {
    case 'A':
        if (changeEtatA)
        {
            changeEtatA = false;
            ret = true;
        }
        break;
    case 'B':
        if (changeEtatB)
        {
            changeEtatB = false;
            ret = true;
        }
        break;
    case 'C':
        if (changeEtatC)
        {
            changeEtatC = false;
            ret = true;
        }
        break;
    }

    mutex.release();
    return ret;
}

bool TPartage::isChangePoidBalance()
{
    bool change;

    mutex.take();
    change = poidBalanceChange;
    poidBalanceChange = false;
    mutex.release();

    return change;
}

bool TPartage::isChangeRecetteAuto()
{
    bool change;

    mutex.take();
    change = recetteAutoChange;
    recetteAutoChange = false;
    mutex.release();

    return change;
}

bool TPartage::isChangeRecetteValue()
{
    bool change;

    mutex.take();
    change = recetteValueChange;
    recetteValueChange = false;
    mutex.release();

    return change;
}
