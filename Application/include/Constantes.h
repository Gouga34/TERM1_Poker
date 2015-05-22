#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <cstdint>
#include <iostream>
#include <string>



#define NOMBRE_DE_TESTS                         10000
#define CAVE_JOUEURS                            1000


#define DOSSIER_RESSOURCES                      "../Application/ressources/"
#define FICHIER_PROBAS_PREFLOP                  DOSSIER_RESSOURCES+std::string("Probas/probas_preflops")
#define TEXTURE_CARTES                          DOSSIER_RESSOURCES+std::string("Interface/deck.png")

//Constantes correspondant à l'accés aux fichiers de profilages
#define DOSSIER_PROFILAGE                       DOSSIER_RESSOURCES+std::string("Profilage/")
#define DOSSIER_PROFILAGE_STATIQUE              DOSSIER_PROFILAGE+std::string("ProfilageStatique/")                 
#define DOSSIER_PROFILAGE_DYNAMIQUE             DOSSIER_PROFILAGE+std::string("ProfilageDynamique/")

#define DOSSIER_ANALYSE_GAINS                   DOSSIER_PROFILAGE_STATIQUE+std::string("AnalyseGains/")
#define FICHIER_PSEUDOS_PROFILAGE               DOSSIER_PROFILAGE_STATIQUE+std::string("pseudos.txt")

//Facteur multiplicateur de la hausse des mises pour l'agressivité
#define HAUSSE_MISES_AGRESSIVITE                2

/*
 * Constantes utilisées pour les scénarios de tests:
 * RATIONALITE_IA_PROFILAGE : rationalité de base de l'IA qui profile
 * NOMBRE_PARTIES : nombre de parties lancées d'affilée
 * NOMBRE_PARTIES_PROFILAGE : nombre de parties à effectuer avant de pouvoir jouer
 */
#define RATIONALITE_IA_PROFILAGE                50.0

// Profilage
#define NOMBRE_CALIBRAGES                       1
#define NOMBRE_PARTIES                          30
#define VARIATION_AUTORISEE                     20

#define NOMBRE_PARTIES_PROFILAGE                10
#define NOMBRE_PARTIES_REPROFILAGE              5
#define NOMBRE_PARTIES_GAINS                    10

// Calibrage optimal
#define CALCUL_CALIBRAGE_IDEAL                  false
#define NOMBRE_PARTIES_CALIBRAGE_IDEAL          100

// Si vrai, écrit dans un fichier la comparaison des gains parties/jetons avant et après profilage
#define ANALYSE_GAINS_PARTIES                   true

// Recherche du coefficient
#define UTILISATION_DELTA_AGRESSIVITE           true
#define DELTA_AGRESSIVITE                       5           // Nombre de parties pour lesquels on sera une fois plus agressif (1 partie sur 5)

/**
 * Couleur des cartes
 */
enum COULEUR_CARTE{
    PIQUE   = 0,
    COEUR   = 1,
    TREFLE  = 2,
    CARREAU = 3
};

/**
 * Rang des cartes
 */
enum RANG_CARTE{
    T   = 10,
    J   = 11,
    Q   = 12,
    K   = 13,
    AS  = 1
};

/**
 * Combinaisons des mains
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
  * Résultat d'une partie
  */
enum RESULTAT_PARTIE{
    EGALITE = 2,
    GAGNE = 1,
    PERDU = 0
};

/**
 * Actions des joueurs
 */
enum ACTION
{
    MISER,
    RELANCER,
    SUIVRE,
    CHECKER,
    SE_COUCHER,
    PAS_ENCORE_D_ACTION,
    TAPIS,
    PETITE_BLIND,
    GROSSE_BLIND
};

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
            DEBUT_MISE_TOTALE=0,
            FIN_MISE_TOTALE=25,

    		DEBUT_AG_THEORIQUE=0,
    		FIN_AG_THEORIQUE=50,

    		RATIO_NB_MISES_DIVISE=1,
            RATIO_NB_MISES_DIVISEUR=2,

            RATIO_MPH_DIVISE=1,
            RATIO_MPH_DIVISEUR=2
        };
	}

	namespace PALIER2 {
        enum{
            DEBUT_MISE_TOTALE=25,
            FIN_MISE_TOTALE=60,

    		DEBUT_AG_THEORIQUE=51,
    		FIN_AG_THEORIQUE=80,

    		RATIO_NB_MISES_DIVISE=2,
            RATIO_NB_MISES_DIVISEUR=3,

            RATIO_MPH_DIVISE=1,
            RATIO_MPH_DIVISEUR=3
        };
	}

	namespace PALIER3 {
        enum{
            DEBUT_MISE_TOTALE=60,
            FIN_MISE_TOTALE=100, //jusqu'à 100 exclu

    		DEBUT_AG_THEORIQUE=81,
    		FIN_AG_THEORIQUE=100,

    		RATIO_NB_MISES_DIVISE=2,
            RATIO_NB_MISES_DIVISEUR=3,
            
            RATIO_MPH_DIVISE=1,
            RATIO_MPH_DIVISEUR=3
        };
	}	

    namespace PALIER4{ //PALIER tapis

        enum{
            MISE_TOTALE=100,

            AG_THEORIQUE=100,
        };
    }

}


// Etapes de jeu
enum ETAPE_JEU { PREFLOP, FLOP, TURN, RIVER, NB_ETAPES };

const std::string nomEtapes[NB_ETAPES] = {"preflop", "flop", "turn", "river" };


namespace PROFIL_JOUEUR{
    enum PROFIL_JOUEUR {
        AGRESSIVITE,
        RATIONALITE,
        PASSIVITE,
        BLUFF,
        NB_PROFILS
    };
}

/**accès aux données des fichiers contenant les scénarios de tests**/
enum COLONNES_CSV{
    AGRESSIVITE_IA,
    CHANCES_GAIN_IA,
    AGRESSIVITE_ATTENDUE,
    RATIONALITE_ATTENDUE,
    AGRESSIVITE_REELLE,
    RATIONALITE_REELLE,
    DISTANCE,
    AGRESSIVITE_DEDUITE_GLOBALE,
    RATIONALITE_DEDUITE_GLOBALE,
    TAUX_SIMILARITE
};

// Phases de jeu de l'IA qui profile
enum PHASE_JEU_IA {
    PHASE_PROFILAGE,
    PHASE_GAINS
};


#endif // CONSTANTES_H
