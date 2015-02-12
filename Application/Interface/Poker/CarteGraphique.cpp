#include "CarteGraphique.h"
#include "Fenetre.h"

const unsigned int CarteGraphique::LARGEUR_CARTE = 73;
const unsigned int CarteGraphique::HAUTEUR_CARTE = 97;


CarteGraphique::CarteGraphique(int rang, int couleur) : QLabel()
{
    int carte = MAKE_CARD(rang, couleur);
    if (carte == 0){
        setPixmap(Fenetre::textureCartes->copy(QRect((13 * 4 + 1) * LARGEUR_CARTE, 0, LARGEUR_CARTE, HAUTEUR_CARTE)));
    }
    else{
        setPixmap(Fenetre::textureCartes->copy(QRect(carte * LARGEUR_CARTE, 0, LARGEUR_CARTE, HAUTEUR_CARTE)));
    }
}


CarteGraphique::CarteGraphique(Carte c) : QLabel()
{
  setPixmap(Fenetre::textureCartes->copy(QRect(c.getId() * LARGEUR_CARTE, 0, LARGEUR_CARTE, HAUTEUR_CARTE)));
}


CarteGraphique::~CarteGraphique()
{

}
