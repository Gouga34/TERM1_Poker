/*========================================================================
Nom: JoueurHumain.h       Auteur: Manuel CHATAIGNER
Maj: 11/04/2015          Creation: 11/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant un joueur humain.
=========================================================================*/

#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"
#include "../Gui/HumanWindowContent.h"

namespace game {

    class HumanPlayer : public Player
    {
        private:

            gui::HumanWindowContent *m_window;

        public:

            HumanPlayer(bool isDealer, int tokens, int position, gui::HumanWindowContent *window);
            HumanPlayer(const HumanPlayer& player, gui::HumanWindowContent *window);
            virtual ~HumanPlayer();

            virtual bool isHumain() const;

            virtual Action play();
    };
}

#endif // HUMANPLAYER_H
