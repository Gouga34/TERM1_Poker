/*========================================================================
Nom: Logger.cpp       Auteur: Manuel CHATAIGNER
Maj: 26/03/2015            Creation: 26/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Logger.
=========================================================================*/

#include "../../include/Interface/Logger.h"


Logger* Logger::instance = 0;

Logger::Logger(ContenuFenetre *window) : m_window(window)
{

}

Logger::~Logger()
{

}

void Logger::createInstance(ContenuFenetre *window)
{
    if (!instance) {
        instance = new Logger(window);
    }
}

Logger* Logger::getInstance()
{
    return instance;
}

void Logger::deleteInstance()
{
    if (instance) {
        delete instance;
        instance = 0;
    }
}

void Logger::addLogs(const char *content)
{
    m_window->ajoutLogs(QString(content));
}

void Logger::addLogs(std::string content)
{
    m_window->ajoutLogs(QString::fromStdString(content));
}

void Logger::addLogs(QString content)
{
    m_window->ajoutLogs(content);
}
