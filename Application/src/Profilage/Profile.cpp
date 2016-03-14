/*========================================================================
Nom: Profil.cpp       Auteur: Manuel CHATAIGNER
Maj: 01/04/2015            Creation: 01/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Profil.
=========================================================================*/

#include "../../include/Profilage/Profile.h"

Profile::Profile() : m_pseudo(""), m_aggressiveness(0), m_rationality(0), m_bluff(0), m_passivity(0) {

}

Profile::Profile(std::string pseudo) : m_pseudo(pseudo),
    m_aggressiveness(0), m_rationality(0), m_bluff(0), m_passivity(0) {

}

Profile::~Profile() {

}

std::string Profile::getPseudo() const {
    return m_pseudo;
}

double Profile::getAggressiveness() const {
    return m_aggressiveness;
}

double Profile::getRationality() const {
    return m_rationality;
}

double Profile::getBluff() const {
    return m_bluff;
}

double Profile::getPassivity() const {
    return m_passivity;
}

void Profile::setPseudo(std::string pseudo) {
    m_pseudo = pseudo;
}

void Profile::setAggressiveness(double aggressiveness) {
    m_aggressiveness = aggressiveness;
}

void Profile::setRationality(double rationality) {
    m_rationality = rationality;
}

void Profile::setBluff(double bluff) {
    m_bluff = bluff;
}

void Profile::setPassivity(double passivity) {
    m_passivity = passivity;
}
