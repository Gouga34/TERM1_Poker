/*========================================================================
Nom: CarteGraphique.cpp       Auteur: Manuel CHATAIGNER
Maj: 11/02/2015               Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe CarteGraphique.
=========================================================================*/

#include "CarteGraphique.h"
#include "Fenetre.h"

// Définition de la taille d'une carte
const unsigned int CarteGraphique::LARGEUR_CARTE = 73;
const unsigned int CarteGraphique::HAUTEUR_CARTE = 97;

const QString CarteGraphique::couleurs[] = {"Pique", "Coeur", "Trèfle", "Carreau" };


CarteGraphique::CarteGraphique(int rang, int couleur) : QLabel()
{
    int carte = makeCard(rang, couleur);

    if (carte == 0){    // Dos de carte
        setPixmap(Fenetre::textureCartes->copy(QRect((13 * 4 + 1) * LARGEUR_CARTE, 0, LARGEUR_CARTE, HAUTEUR_CARTE)));
    }
    else{               // Identifiant de la carte
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

int CarteGraphique::makeCard(int rang, int couleur) const
{
    if(rang == 0)   return 0;
    if(rang >  13) rang = 13;

    if(couleur < 0)    couleur = 0;
    if(couleur > 3)    couleur = 3;

    return ( (couleur * 13) + (rang - 1) );
}
