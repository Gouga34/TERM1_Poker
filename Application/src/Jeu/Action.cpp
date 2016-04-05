/*========================================================================
Nom: Action.cpp       Auteur: Manuel CHATAIGNER
Maj: 07/04/2015            Creation: 07/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Action.
=========================================================================*/

#include "../../include/Jeu/Action.h"


Action::Action() : m_action(PAS_ENCORE_D_ACTION), m_tokens(-1)
{

}

Action::Action(ACTION action, int tokens) : m_action(action), m_tokens(tokens)
{

}

Action::~Action()
{

}

ACTION Action::getAction() const
{
    return m_action;
}

int Action::getTokens() const
{
    return m_tokens;
}

void Action::setAction(ACTION action)
{
    m_action = action;
}

void Action::setTokens(int tokens)
{
    m_tokens = tokens;
}
