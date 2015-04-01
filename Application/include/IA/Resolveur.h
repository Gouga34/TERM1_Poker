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
#include "../Profilage/Profil.h"
#include <cstdlib>

class IntelligenceArtificielle;

class Resolveur{
private:

    Profil calibrage;


    IntelligenceArtificielle *ia;

    /**
     * @action calcule l'action à effectuer en fonction du taux d'agressivité
     * @return l'action à effectuer et le montant de sa mise s'il y en a un.
     *          Ce montant est égal à -1 si pas de mise.
    */
    std::pair<ACTION,int> calculerActionAgressivite();

    /**
     * @brief calculerActionRationalite
     * @action calcule l'action à effectuer en fonction du taux de rationalité
     * @return l'action à effectuer et le montant de sa mise s'il y en a un.
     *          Ce montant est égal à -1 si pas de mise.
     */
    std::pair<ACTION,int> calculerActionRationalite();

    /**
     * @brief calculerMiseAgressivite
     * @param action action à effectuer
     * @return les jetons à jouer en fonction de l'action qui sera effectuée
     *          et en fonction du taux d'agressivité
     */
    int calculerMiseAgressivite(ACTION action);

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
    * @brief setCalibrage
    * @param profil nouveau calibrage de l'IA
    */
   void setCalibrage(Profil profil);

    /**
     * @brief getRationalite
     * @return le taux de rationalité
     */
    Profil& getCalibrage();


    /**
     * @brief calculerAction
     * @return l'action finale et sa possible mise en nombre de jetons
     */
    std::pair<ACTION,int> calculerAction();



};


#endif // RESOLVEUR_H
