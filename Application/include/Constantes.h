#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <cstdint>
#include <iostream>

/**
 * Type enum Cutilise pour definir la couleur des cartes
 */
enum COULEUR_CARTE{
    PIQUE   = 0,
    COEUR   = 1,
    TREFLE  = 2,
    CARREAU = 3
};

/**
 * Type enum utilise pour definir le rang des cartes
 */
enum RANG_CARTE{
    T   = 10,
    J   = 11,
    Q   = 12,
    K   = 13,
    AS  = 1
};

/**
 * Type enum utilise pour definir la force des mains
 */
enum FORCE_MAIN{

    QUINTE_FLUSH_ROYALE = 10,
    QUINTE_FLUSH       = 9,
    CARRE              = 8,
    FULL               = 7,
    COULEUR            = 6,
    SUITE              = 5,
    BRELAN             = 4,
    DOUBLE_PAIRE       = 3,
    PAIRE              = 2,
    CARTE_HAUTE        = 1
};

/**
  * enum utilisé lors de la comparaison de deux mains. 
  */
enum {
    EGALITE = 2,
    GAGNE = 1,
    PERDU = 0
};

/**
 * Type enum pour definir les actions des joueurs
 */
namespace TYPES
{
    enum ACTION_LIST
    {
        MISER,
        RELANCER,
        SUIVRE,
        CHECKER,
        SE_COUCHER,
        EN_ATTENTE,
        TAPIS,
        PETITE_BLIND,
        GROSSE_BLIND
    };
}

//Constantes des formules de profilage

//Rationalité
namespace RATIONALITE{

	int NB_PALLIERS=4;

	namespace PALIER1 {
		int DEBUT_GAIN=0;
		int FIN_GAIN=30;

		int DEBUT_RA_THEORIQUE=0;
		int FIN_RA_THEORIQUE=10;
	}

	namespace PALIER2 {
		int DEBUT_GAIN=31;
		int FIN_GAIN=50;

		int DEBUT_RA_THEORIQUE=11;
		int FIN_RA_THEORIQUE=15;	
	}

	namespace PALIER3 {
		int DEBUT_GAIN=51;
		int FIN_GAIN=69;

		int DEBUT_RA_THEORIQUE=16;
		int FIN_RA_THEORIQUE=65;		
	}

	namespace PALIER4 {
		int DEBUT_GAIN=70;
		int FIN_GAIN=100;

		int DEBUT_RA_THEORIQUE=66;
		int FIN_RA_THEORIQUE=100;
	}	
}

//Agressivité
namespace AGRESSIVITE{

	int NB_PALIERS=3;

	//MPH = mise la plus haute
	namespace PALIER1 {
		
		int DEBUT_MPH=0;
		int FIN_MPH=25;

		int DEBUT_AG_THEORIQUE=0;
		int FIN_AG_THEORIQUE=50;

		int RATIO_NB_MISES=0.5;
		int RATIO_TOTAL_MISES=0.5;
	};

	namespace PALIER2 {
		int DEBUT_MPH=26;
		int FIN_MPH=50;

		int DEBUT_AG_THEORIQUE=51;
		int FIN_AG_THEORIQUE=80;

		int RATIO_NB_MISES=0.666666667;
		int RATIO_TOTAL_MISES=0.333333333;
	};

	namespace PALIER3 {
		int DEBUT_MPH=51;
		int FIN_MPH=100;

		int DEBUT_AG_THEORIQUE=81;
		int FIN_AG_THEORIQUE=100;

		int RATIO_NB_MISES=0.666666667;
		int RATIO_TOTAL_MISES=0.333333333;
	};	

}


#endif // CONSTANTES_H
