/*========================================================================
Nom: Resolveur.h         Auteur: Morgane Vidal
Maj: 31/03/2015          Creation: 31/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe permettant de calculer les actions de
                                             l'intelligence artificielle.
=========================================================================*/

#ifndef RESOLVEUR_H
#define RESOLVEUR_H

#include <utility>
#include "../Jeu/Jeu.h"
#include "IntelligenceArtificielle.h"
#include "../Profiling/Profile.h"
#include <cstdlib>

class IntelligenceArtificielle;

class Resolveur{
private:

    bool jeuAgressif;
    profiling::Profile *calibrage;
    double miseTotaleTheoriqueAgressivite;


    IntelligenceArtificielle *ia;

    /**
     * @action calcule l'action à effectuer en fonction du taux d'agressivité
     * @return l'action à effectuer et le montant de sa mise s'il y en a un.
     *          Ce montant est égal à -1 si pas de mise.
    */
    Action calculerActionAgressivite();

    /**
     * @brief calculerActionRationalite
     * @action calcule l'action à effectuer en fonction du taux de rationalité
     * @return l'action à effectuer et le montant de sa mise s'il y en a un.
     *          Ce montant est égal à -1 si pas de mise.
     */
    Action calculerActionRationalite();

    /**
     * @brief calculerTotalMiseTheoriqueAgressivite
     * @action calcule la mise totale théorique de la partie en fonction de l'agressivité
     */
    void calculerTotalMiseTheoriqueAgressivite();

    /**
     * @brief calculerMiseRationalite
     * @param action l'action à effectuer
     * @return les jetons à jouer en fonction de l'action qui sera effectuée
     *          et en fonction du taux de rationalité
     */
    int calculerMiseRationalite(ACTION action);

    /**
     * @brief calculerMiseFusion
     * @param miseAgressivite mise calculée en fonction du taux d'agressivité de l'IA
     * @param miseRationalite mise calculée en fonction du taux de rationalité de l'IA
     * @return le nombre de jetons à miser, en fonction des mises calculées auparavent
     *                                                  et des taux de rationalité et agressivité
     */
    int calculerMiseFusion(int miseAgressivite, int miseRationalite);

public:
    Resolveur(IntelligenceArtificielle* ia2);
    virtual ~Resolveur();

    /**
     * @brief estAgressif
     * @return true si le joueur a joué plus agressif
     */
    bool estAgressif() const;

    /**
     * @brief Modifie le choix de l'action pour que le résolveur privilégie ou non l'agressivité.
     * @param ag true si action agressive prioritaire
     */
    void setJeuAgressif(bool ag);

    /**
    * @brief setCalibrage
    * @param profil nouveau calibrage de l'IA
    */
   void setCalibrage(profiling::Profile profil);

    /**
     * @brief getRationalite
     * @return le taux de rationalité
     */
    profiling::Profile* getCalibrage();


    /**
     * @brief calculerAction
     * @return l'action finale et sa possible mise en nombre de jetons
     */
    Action calculerAction();
};


#endif // RESOLVEUR_H
