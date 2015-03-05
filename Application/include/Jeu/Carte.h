#ifndef Carte_h
#define Carte_h

/**

Couleur  { Pique = 0, Coeur = 1, Trefle = 2, Carreau = 3 };
Rang { As = 1, 2, 3, 4, 5,  6,  7, 8,  9, 10, Valet = 11, Dame = 12, Roi = 13 };

**/

#include "../Constantes.h"

class Carte{

	//Attributs
	private:
		int id;
		
	//Constructeur et destructeur
	public:

        /**
        *@param  : La couleur et le rang de la carte
        *@action : Initialise une nouvelle carte
        **/
        Carte(int rang, int couleur);

        /**
        *@action : Destructeur de la classe Carte
        **/
		~Carte();
		
	//Accesseurs

        /**
        *@action : Permet d'obtenir la couleur de la carte
        *@return : La couleur de la carte
        **/
        int     getCouleur() 	const;

        /**
        *@action : Permet d'obtenir le rang de la carte
        *@return : Le rang de la carte
        **/
        int     getRang()       const;

        /**
        *@action : Permet d'obtenir l'id de la carte
        *@return : L'id de la carte
        **/
        int 	getId()         const;

};

#endif
