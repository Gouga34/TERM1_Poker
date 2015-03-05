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

    QUINTE_FLUSH_ROYAL = 0,
    QUINTE_FLUSH       = 1,
    CARRE              = 2,
    FULL               = 3,
    COULEUR            = 4,
    SUITE              = 5,
    BRELAN             = 6,
    DOUBLE_PAIRES      = 7,
    PAIRE              = 8,
    CARTE_HAUTE        = 9
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
