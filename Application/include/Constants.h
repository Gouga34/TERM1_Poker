#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>
#include <iostream>
#include <string>



#define TESTS_NUMBER                            10000
#define PLAYERS_CAVE                            1000


#define RESOURCES_FOLDER                       "../Application/ressources/"
#define PREFLOP_PROBABILITIES_FILE              RESOURCES_FOLDER + std::string("Probas/probas_preflops")
#define CARDS_TEXTURE                           RESOURCES_FOLDER + std::string("Interface/deck.png")

//Constantes correspondant à l'accés aux fichiers de profilages
#define PROFILING_FOLDER                        RESOURCES_FOLDER + std::string("Profilage/")
#define STATIC_PROFILING_FOLDER                 PROFILING_FOLDER + std::string("ProfilageStatique/")
#define DYNAMIC_PROFILING_FOLDER                PROFILING_FOLDER + std::string("ProfilageDynamique/")

#define EARNING_ANALYSIS_FOLDER                 STATIC_PROFILING_FOLDER + std::string("AnalyseGains/")
#define PROFILING_NICKNAMES_FILE                STATIC_PROFILING_FOLDER + std::string("pseudos.txt")

//Facteur multiplicateur de la hausse des mises pour l'agressivité
#define INCREASE_AGGRESSIVE_BETS                2

/*
 * Constantes utilisées pour les scénarios de tests:
 * RATIONALITE_IA_PROFILAGE : rationalité de base de l'IA qui profile
 * NOMBRE_PARTIES : nombre de parties lancées d'affilée
 * NOMBRE_PARTIES_PROFILAGE : nombre de parties à effectuer avant de pouvoir jouer
 */
#define PROFILING_IA_RATIONALITY                50.0

#define ALLOWED_VARIATION                       20

// Recherche du coefficient
#define AGGRESSIVENESS_DELTA_USE                false
#define AGGRESSIVENESS_DELTA                    15           // Nombre de parties pour lesquels on sera une fois plus agressif (1 partie sur 5)

/**
 * Couleur des cartes
 */
enum CARD_COLOR {
    SPADES   = 0,
    HEARTS   = 1,
    CLUBS    = 2,
    DIAMONDS = 3
};

/**
 * Rang des cartes
 */
enum CARD_RANK {
    T   = 10,
    J   = 11,
    Q   = 12,
    K   = 13,
    AS  = 1
};



/**
  * Résultat d'une partie
  */
enum GAME_RESULT {
    EQUALITY = 2,
    WON = 1,
    LOOSE = 0
};

/**
 * Actions des joueurs
 */
enum ACTION
{
    BET,
    RAISE,
    CALL,
    CHECK,
    FOLD,
    NO_ACTION,
    ALL_IN,
    SMALL_BLIND,
    BIG_BLIND
};

//Constantes des formules de profilage

//Rationalité
namespace RATIONALITY {

    enum{
	   NB_PALIERS=4
    };

	namespace PALIER1 {
        enum {
             DEBUT_GAIN = 0,
             FIN_GAIN = 30,

             DEBUT_MISE_THEORIQUE = 0,
             FIN_MISE_THEORIQUE = 10
        };
	}

	namespace PALIER2 {
        enum {
            DEBUT_GAIN = 31,
            FIN_GAIN = 50,

            DEBUT_MISE_THEORIQUE = 11,
            FIN_MISE_THEORIQUE = 15
     };
	}

	namespace PALIER3 {
        enum {
            DEBUT_GAIN = 51,
            FIN_GAIN = 69,

            DEBUT_MISE_THEORIQUE = 16,
            FIN_MISE_THEORIQUE = 65
        };		
	}

	namespace PALIER4 {
        enum {
            DEBUT_GAIN = 70,
            FIN_GAIN = 100,

            DEBUT_MISE_THEORIQUE = 66,
            FIN_MISE_THEORIQUE = 100
        };
	}	
}

//Agressivité
namespace AGGRESSIVENESS {
    enum {
       NB_PALIERS = 4
    };
	
    //MPH = mise la plus haute
	namespace PALIER1 {
       enum {
            DEBUT_MISE_TOTALE = 0,
            FIN_MISE_TOTALE = 25,

            DEBUT_AG_THEORIQUE = 0,
            FIN_AG_THEORIQUE = 50,

            RATIO_NB_MISES_DIVISE = 1,
            RATIO_NB_MISES_DIVISEUR = 2,

            RATIO_MPH_DIVISE = 1,
            RATIO_MPH_DIVISEUR = 2
        };
	}

	namespace PALIER2 {
        enum {
            DEBUT_MISE_TOTALE = 25,
            FIN_MISE_TOTALE = 60,

            DEBUT_AG_THEORIQUE = 51,
            FIN_AG_THEORIQUE = 80,

            RATIO_NB_MISES_DIVISE = 2,
            RATIO_NB_MISES_DIVISEUR = 3,

            RATIO_MPH_DIVISE = 1,
            RATIO_MPH_DIVISEUR = 3
        };
	}

	namespace PALIER3 {
        enum {
            DEBUT_MISE_TOTALE = 60,
            FIN_MISE_TOTALE = 100, //jusqu'à 100 exclu

            DEBUT_AG_THEORIQUE = 81,
            FIN_AG_THEORIQUE = 100,

            RATIO_NB_MISES_DIVISE = 2,
            RATIO_NB_MISES_DIVISEUR = 3,
            
            RATIO_MPH_DIVISE = 1,
            RATIO_MPH_DIVISEUR = 3
        };
	}	

    namespace PALIER4 { //PALIER tapis

        enum {
            MISE_TOTALE = 100,

            AG_THEORIQUE = 100,
        };
    }

}


// Etapes de jeu
enum GAME_STEPS { PREFLOP, FLOP, TURN, RIVER, STEPS_NB };

const std::string stepsNames[STEPS_NB] = {"preflop", "flop", "turn", "river" };


namespace PLAYER_PROFILE {
    enum PLAYER_PROFILE {
        AGGRESSIVENESS,
        RATIONALITY,
        PASSIVITY,
        BLUFF,
        PROFILES_NB
    };
}

/**accès aux données des fichiers contenant les scénarios de tests**/
enum CSV_COLUMNS {
    AI_AGGRESSIVENESS,
    AI_WINNING_CHANCES,
    EXPECTED_AGGRESSIVENESS,
    EXPECTED_RATIONALITY,
    REAL_AGGRESSIVENESS,
    REAL_RATIONALITY,
    DISTANCE,
    GLOBAL_DEDUCED_AGGRESSIVENESS,
    GLOBAL_DEDUCED_RATIONALITY,
    SIMILARITY_DEGREE
};

// Phases de jeu de l'IA qui profile
enum AI_PLAY_PHASES {
    PROFILING_PHASE,
    EARNINGS_PHASE
};

#endif // CONSTANTS_H
