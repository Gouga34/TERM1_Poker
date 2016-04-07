/*========================================================================
Nom: ContenuFenetre.h       Auteur: Manuel CHATAIGNER
Maj: 20/05/2015          Creation: 20/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant la fenêtre de choix des cartes.
=========================================================================*/

#ifndef WINDOWCONTENT_H
#define WINDOWCONTENT_H

#include "../Game/Game.h"
#include <QWidget>
#include <QTextEdit>

namespace gui
{
    class WindowContent : public QWidget
    {
        protected:

            game::Game *m_game;

            QTextEdit m_logs;

        public:

            WindowContent(game::Game *game);
            virtual ~WindowContent();

            /**
             * @brief Ajoute dans les logs le contenu passé en paramètre
             * @param content Texte à ajouter aux logs
             */
            virtual void addLogs(QString content);

            /**
             * @brief Met à jour l'interface
             */
            virtual void refresh() = 0;
    };
}

#endif // WINDOWCONTENT_H
