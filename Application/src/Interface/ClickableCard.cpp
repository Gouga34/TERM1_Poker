/*========================================================================
Nom: CarteCliquable.cpp       Auteur: Manuel CHATAIGNER
Maj: 09/03/2015            Creation: 09/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe CarteCliquable.
=========================================================================*/

#include "../../include/Interface/ClickableCard.h"


ClickableCard::ClickableCard(int rank, int color)
    : GraphicCard(rank, color)
{

}

ClickableCard::~ClickableCard()
{

}

void ClickableCard::mousePressEvent(QMouseEvent * /*event*/)
{
    emit clicked(m_id);
}

