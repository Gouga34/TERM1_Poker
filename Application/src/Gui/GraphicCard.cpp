/*========================================================================
Nom: CarteGraphique.cpp       Auteur: Manuel CHATAIGNER
Maj: 11/02/2015               Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe CarteGraphique.
=========================================================================*/

#include "../../include/Gui/GraphicCard.h"
#include "../../include/Gui/HumanWindowContent.h"


namespace gui
{
    // Définition de la taille d'une carte
    const unsigned int GraphicCard::CARD_WIDTH = 73;
    const unsigned int GraphicCard::CARD_HEIGHT = 97;

    const std::array<QString, 4> GraphicCard::colors = { "Pique", "Coeur", "Trèfle", "Carreau" };


    GraphicCard::GraphicCard(int rank, int color) : QLabel()
    {
        m_id = makeCard(rank, color);


        if (m_id == -1) {    // Dos de carte
            setPixmap(HumanWindowContent::m_cardsTexture->copy(QRect((13 * 4 + 1) * CARD_WIDTH, 0, CARD_WIDTH, CARD_HEIGHT)));
        }
        else {               // Identifiant de la carte
            setPixmap(HumanWindowContent::m_cardsTexture->copy(QRect(m_id * CARD_WIDTH, 0, CARD_WIDTH, CARD_HEIGHT)));
        }
    }

    GraphicCard::GraphicCard(game::Card c) : QLabel()
    {
        m_id = c.getId();

        setPixmap(HumanWindowContent::m_cardsTexture->copy(QRect(m_id * CARD_WIDTH, 0, CARD_WIDTH, CARD_HEIGHT)));
    }


    GraphicCard::~GraphicCard()
    {

    }

    int GraphicCard::makeCard(int rank, int color) const
    {
        if (rank == 0)      return -1;
        if (rank >  13)     rank = 13;

        if (color < 0)      color = 0;
        if (color > 3)      color = 3;

        return ((color * 13) + (rank - 1));
    }
}
