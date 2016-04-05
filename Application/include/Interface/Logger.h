/*========================================================================
Nom: Logger.h       Auteur: Manuel CHATAIGNER
Maj: 26/03/2015          Creation: 26/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe de gestion d'écriture des logs dans le tableau de bord.
=========================================================================*/

#ifndef LOGGER_H
#define LOGGER_H

#include "ContenuFenetre.h"

class Logger
{
    private:

        ContenuFenetre *m_window;

        Logger(ContenuFenetre *window);
        virtual ~Logger();

        /* Instance du singleton */
        static Logger *instance;

    public:

        /**
         * @brief Créé l'instance du singleton
         * @param window Pointeur vers la fenêtre utilisée pour afficher les logs
         */
        static void createInstance(ContenuFenetre *window);

        /**
         * @return Instance du singleton
         */
        static Logger* getInstance();

        /**
         * @brief Supprimer l'instance du singleton allouée dynamiquement
         */
        static void deleteInstance();

        /**
         * @brief Ajoute dans les logs de la fenêtre le contenu passé en paramètre
         * @param content Contenu à ajouter au tableau de bord
         */
        virtual void addLogs(const char *content);

        /**
         * @brief Ajoute dans les logs de la fenêtre le contenu passé en paramètre
         * @param content Contenu à ajouter au tableau de bord
         */
        virtual void addLogs(std::string content);

        /**
         * @brief Ajoute dans les logs de la fenêtre le contenu passé en paramètre
         * @param content Contenu à ajouter au tableau de bord
         */
        virtual void addLogs(QString content);
};

#endif // LOGGER_H
