/*========================================================================
Nom: ContenuFenetre.h       Auteur: Manuel CHATAIGNER
Maj: 20/05/2015          Creation: 20/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant la fenêtre de choix des cartes.
=========================================================================*/

#ifndef CONTENUFENETRE_H
#define CONTENUFENETRE_H

#include "../Jeu/Game.h"
#include <QWidget>
#include <QTextEdit>

class ContenuFenetre : public QWidget
{
    protected:

        Game *jeu;

        QTextEdit logs;

    public:

        ContenuFenetre(Game *j);
        virtual ~ContenuFenetre();

        /**
         * @action Ajoute dans les logs le contenu passé en paramètre
         * @param contenu Texte à ajouter aux logs
         */
        void ajoutLogs(QString contenu);

        /**
         * @brief Met à jour l'interface
         */
        virtual void actualiser() = 0;
};

#endif // CONTENUFENETRE_H
