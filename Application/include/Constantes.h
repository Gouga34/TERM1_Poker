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

    enum{
	   NB_PALIERS=4
    };

	namespace PALIER1 {
        enum{
    		 DEBUT_GAIN=0,
    		 FIN_GAIN=30,

    		 DEBUT_MISE_THEORIQUE=0,
    		 FIN_MISE_THEORIQUE=10
        };
	}

	namespace PALIER2 {
        enum{
    		DEBUT_GAIN=31,
    		FIN_GAIN=50,

    		DEBUT_MISE_THEORIQUE=11,
    	   	FIN_MISE_THEORIQUE=15	
     };
	}

	namespace PALIER3 {
        enum{
    		DEBUT_GAIN=51,
    		FIN_GAIN=69,

    		DEBUT_MISE_THEORIQUE=16,
    		FIN_MISE_THEORIQUE=65
        };		
	}

	namespace PALIER4 {
        enum{
    		DEBUT_GAIN=70,
    		FIN_GAIN=100,

    		DEBUT_MISE_THEORIQUE=66,
    		FIN_MISE_THEORIQUE=100
        };
	}	
}

//Agressivité
namespace AGRESSIVITE{
    enum{
	   NB_PALIERS=4
    };
	
    //MPH = mise la plus haute
	namespace PALIER1 {
	   enum{	
    		DEBUT_MPH=0,
    		FIN_MPH=25,

    		DEBUT_AG_THEORIQUE=0,
    		FIN_AG_THEORIQUE=50,

    		RATIO_NB_MISES_DIVISE=1,
            RATIO_NB_MISES_DIVISEUR=2,

    		RATIO_TOTAL_MISES_DIVISE=1,
            RATIO_TOTAL_MISES_DIVISEUR=2
        };
	}

	namespace PALIER2 {
        enum{
    		DEBUT_MPH=26,
    		FIN_MPH=50,

    		DEBUT_AG_THEORIQUE=51,
    		FIN_AG_THEORIQUE=80,

    		RATIO_NB_MISES_DIVISE=2,
            RATIO_NB_MISES_DIVISEUR=3,

    		RATIO_TOTAL_MISES_DIVISE=1,
            RATIO_TOTAL_MISES_DIVISEUR=3
        };
	}

	namespace PALIER3 {
        enum{
    		DEBUT_MPH=51,
    		FIN_MPH=100, //jusqu'à 100 exclu

    		DEBUT_AG_THEORIQUE=81,
    		FIN_AG_THEORIQUE=100,

    		RATIO_NB_MISES_DIVISE=2,
            RATIO_NB_MISES_DIVISEUR=3,
            
    		RATIO_TOTAL_MISES_DIVISE=1,
            RATIO_TOTAL_MISES_DIVISEUR=3
        };
	}	

    namespace PALIER4{ //PALIER tapis

        enum{
            MPH=100,

            AG_THEORIQUE=100,
        };
    }

}


// Etapes de jeu
enum ETAPE_JEU { PREFLOP, FLOP, TURN, RIVER, NB_ETAPES };

const std::string nomEtapes[NB_ETAPES] = {"preflop", "flop", "turn", "river" };


// Profils de l'adversaire
/**
 * Rationnel Agressif (RA)
 * Rationnel Passif (RP)
 * Bluffeur Agressif (BA)
 * Bluffeur Passif (BP)
 */
enum PROFIL_JOUEUR { RA, RP, BA, BP, NB_PROFILS };


#endif // CONSTANTES_H
