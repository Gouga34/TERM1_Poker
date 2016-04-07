/*========================================================================
Nom: JoueurHumain.cpp       Auteur: Manuel CHATAIGNER
Maj: 11/04/2015            Creation: 11/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe JoueurHumain.
=========================================================================*/


#include "../../include/Game/HumanPlayer.h"

namespace game {

    HumanPlayer::HumanPlayer(bool isDealer, int tokens, int position, gui::HumanWindowContent *window)
        : Player(isDealer, tokens, position), m_window(window)
    {

    }

    HumanPlayer::HumanPlayer(const HumanPlayer& player, gui::HumanWindowContent *window)
        : Player(player), m_window(window)
    {

    }

    HumanPlayer::~HumanPlayer()
    {

    }

    bool HumanPlayer::isHumain() const {
        return true;
    }

    Action HumanPlayer::play(){

        return m_window->getAction();
    }
}
