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
#include "../Interface/HumanWindowContent.h"

namespace game {

    class HumanPlayer : public Player
    {
        private:

            HumanWindowContent *m_window;

        public:

            HumanPlayer(bool isDealer, int tokens, int position, HumanWindowContent *window);
            HumanPlayer(const HumanPlayer& player, HumanWindowContent *window);
            virtual ~HumanPlayer();

            virtual bool isHumain() const;

            virtual Action play();
    };
}

#endif // HUMANPLAYER_H
