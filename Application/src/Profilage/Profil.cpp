/*========================================================================
Nom: Profil.cpp       Auteur: Manuel CHATAIGNER
Maj: 01/04/2015            Creation: 01/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Profil.
=========================================================================*/

#include "../../include/Profilage/Profil.h"

using namespace std;

Profil::Profil() : pseudo(""), agressivite(0), rationalite(0), bluff(0), passivite(0)
{

}

Profil::Profil(string pseudoProfil) : pseudo(pseudoProfil),
    agressivite(0), rationalite(0), bluff(0), passivite(0)
{

}

Profil::~Profil()
{

}

string Profil::getPseudo() const {
    return pseudo;
}

double Profil::getAgressivite() const {
    return agressivite;
}

double Profil::getRationalite() const {
    return rationalite;
}

double Profil::getBluff() const {
    return bluff;
}

double Profil::getPassivite() const {
    return passivite;
}

void Profil::setPseudo(string nvPseudo) {
    pseudo = nvPseudo;
}

void Profil::setAgressivite(double nvAgr) {
    agressivite = nvAgr;
}

void Profil::setRationalite(double nvRat) {
    rationalite = nvRat;
}

void Profil::setBluff(double nvBluff) {
    bluff = nvBluff;
}

void Profil::setPassivite(double nvPass) {
    passivite = nvPass;
}
