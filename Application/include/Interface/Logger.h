/*========================================================================
Nom: Logger.h       Auteur: Manuel CHATAIGNER
Maj: 26/03/2015          Creation: 26/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe de gestion d'écriture des logs dans le tableau de bord.
=========================================================================*/

#ifndef LOGGER_H
#define LOGGER_H

#include "Fenetre.h"

class Logger
{
    private:

        Fenetre *fenetre;

        Logger(Fenetre *f);
        ~Logger();

        /* Instance du singleton */
        static Logger *instance;

    public:

        /**
         * @action Créé l'instance du singleton
         * @param f Pointeur vers la fenêtre utilisée pour afficher les logs
         */
        static void creerInstance(Fenetre *f);

        /**
         * @return Instance du singleton
         */
        static Logger* getInstance();

        /**
         * @action Supprimer l'instance du singleton allouée dynamiquement
         */
        static void supprimerInstance();

        /**
         * @action Ajoute dans les logs de la fenêtre le contenu passé en paramètre
         * @param contenu Contenu à ajouter au tableau de bord
         */
        void ajoutLogs(const char *contenu);

        /**
         * @action Ajoute dans les logs de la fenêtre le contenu passé en paramètre
         * @param contenu Contenu à ajouter au tableau de bord
         */
        void ajoutLogs(std::string contenu);

        /**
         * @action Ajoute dans les logs de la fenêtre le contenu passé en paramètre
         * @param contenu Contenu à ajouter au tableau de bord
         */
        void ajoutLogs(QString contenu);
};

#endif // LOGGER_H
