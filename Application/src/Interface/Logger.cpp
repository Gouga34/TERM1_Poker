/*========================================================================
Nom: Logger.cpp       Auteur: Manuel CHATAIGNER
Maj: 26/03/2015            Creation: 26/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Logger.
=========================================================================*/

#include "../../include/Interface/Logger.h"


Logger* Logger::instance = 0;

Logger::Logger(Fenetre *f) : fenetre(f)
{

}

Logger::~Logger()
{

}

void Logger::creerInstance(Fenetre *f)
{
    if (!instance) {
        instance = new Logger(f);
    }
}

Logger* Logger::getInstance()
{
    return instance;
}

void Logger::supprimerInstance()
{
    if (instance) {
        delete instance;
        instance = 0;
    }
}

void Logger::ajoutLogs(std::string contenu)
{
    fenetre->ajoutLogs(QString::fromStdString(contenu));
}

void Logger::ajoutLogs(QString contenu)
{
    fenetre->ajoutLogs(contenu);
}
