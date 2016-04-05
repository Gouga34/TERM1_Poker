/*========================================================================
Nom: JoueurHumain.cpp       Auteur: Manuel CHATAIGNER
Maj: 11/04/2015            Creation: 11/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe JoueurHumain.
=========================================================================*/


#include "../../include/Game/HumainPlayer.h"

namespace game {

    HumanPlayer::HumanPlayer(bool isDealer, int tokens, int position, ContenuFenetreHumain *window)
        : Player(isDealer, tokens, position), m_window(window)
    {

    }

    HumanPlayer::HumanPlayer(const HumanPlayer& player, ContenuFenetreHumain *window)
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
