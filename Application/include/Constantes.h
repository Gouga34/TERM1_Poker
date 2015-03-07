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
    DOUBLE_PAIRES      = 3,
    PAIRE              = 2,
    CARTE_HAUTE        = 1
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


#endif // CONSTANTES_H
