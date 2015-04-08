/*========================================================================
Nom: Resolveur.cpp       Auteur: Morgane Vidal
Maj: 31/03/2015          Creation: 31/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les corps des méthodes
                                        la classe Resolveur.
=========================================================================*/


#include "../../include/IA/Resolveur.h"
#include "../../include/Constantes.h"
#include "../../include/Profilage/CalculDonneesProfilage.h"

#include <vector>

using namespace std;

Resolveur::Resolveur(IntelligenceArtificielle *ia2):ia(ia2){

}


Resolveur::~Resolveur(){

}

void Resolveur::setCalibrage(Profil profil){
    calibrage.setAgressivite(profil.getAgressivite());
    calibrage.setRationalite(profil.getRationalite());
}


Profil& Resolveur::getCalibrage() {
    return calibrage;
}


Action Resolveur::calculerActionAgressivite(){

    ACTION action;
    int jetonsAMiser = -1; //mise à effectuer s'il y en a une. Correspond au nombre de jetons

    //On calcule un nombre aléatoire entre 0 et 100
    int random = rand()%101;

    //Si random E [0-(agressivite-1)]
    if(random<(calibrage.getAgressivite()-1)){ //On va effectuer une action agressive : miser

        int miseAgressivite=calculerMiseAgressivite(ACTION::MISER);
        //Si on peut miser, on fera l'action "MISER"
        if(ia->getJeu()->peutMiser(ia->getPosition(), miseAgressivite)){
            action=ACTION::MISER;
            jetonsAMiser=miseAgressivite;
        }
        //Sinon, si on peut relancer, on fait l'action "RELANCER"
        else if(ia->getJeu()->peutRelancer(ia->getPosition(),2*ia->getJeu()->getMiseCourante())){
                action=ACTION::RELANCER;
                jetonsAMiser =calculerMiseAgressivite(action);
        }
        //Si on peut suivre on suit
        else if(ia->getJeu()->peutSuivre(ia->getPosition())){
                action=ACTION::SUIVRE;
            }

        else if(ia->getJeu()->peutChecker(ia->getPosition())){
                    action=ACTION::CHECKER;
            }
        else{
               action=ACTION::TAPIS;
           }
    }
    //random E [agressivite - 100]
    else{ //on va effectuer une action pas agressive (suivre ou checker)
        int randx = rand()%100;
        if(randx%2){
            if(ia->getJeu()->peutSuivre(ia->getPosition())){
                action= ACTION::SUIVRE;
            }
            else{
                action= ACTION::CHECKER;
            }
        }
        else{
            if(ia->getJeu()->peutChecker(ia->getPosition())){
                action= ACTION::CHECKER;
            }
            else{
                action =ACTION::SUIVRE;
            }
        }

    }

    return Action(action,jetonsAMiser);
}


int Resolveur::calculerMiseAgressivite(ACTION action){
    int jetonsAJouer = -1;

    if(action==ACTION::MISER){
        //Si l'action est miser,
        //on calcule la miseTheorique à effectuer en fonction du palier
        //dans lequel se trouve le pourcentage d'agressivité.
        int maxPalierAgressivite;

        if(calibrage.getAgressivite() < AGRESSIVITE::PALIER1::FIN_AG_THEORIQUE){
            maxPalierAgressivite=AGRESSIVITE::PALIER1::FIN_MPH;
        }
        else if(calibrage.getAgressivite()<AGRESSIVITE::PALIER2::FIN_AG_THEORIQUE){
            maxPalierAgressivite=AGRESSIVITE::PALIER2::FIN_MPH;
        }
        else if(calibrage.getAgressivite()<AGRESSIVITE::PALIER3::FIN_AG_THEORIQUE){
            maxPalierAgressivite=AGRESSIVITE::PALIER3::FIN_MPH;
        }
        else{ //tapis
            maxPalierAgressivite=100;
        }

        int randx = rand()%maxPalierAgressivite;
        jetonsAJouer=(randx*ia->getCave())/100;

    }
    else if(action==ACTION::RELANCER){
        //Pour l'instant on relance tj de 2, à voir plus tard si des fois on relancera de plus
        jetonsAJouer=2*ia->getJeu()->getMiseCourante();
    }

    return jetonsAJouer;
}


Action Resolveur::calculerActionRationalite(){
    ACTION action;
    int jetonsAMiser =-1; //-1 si pas une action avec une mise

    //On prend un nombre aléatoire
    int random = rand()%101;
    if(random<(calibrage.getRationalite()-1)){ //Si random E [0 - (rationalite-1)]

        int jetonsMiseTheorique=calculerMiseRationalite(ACTION::MISER);

        //Si on peut pas checker et peut de chances de gain :
        if(ia->getChancesGain()<30.0 && !(ia->getJeu()->peutChecker(ia->getPosition()))){
            action=ACTION::SE_COUCHER;
        }
        else if(ia->getMiseTotale()<jetonsMiseTheorique){
            int jetonsAMiserRationalite=jetonsMiseTheorique-ia->getMiseTotale();

            if(ia->getJeu()->peutMiser(ia->getPosition(),jetonsAMiserRationalite)){ //Si on peut miser
                action=ACTION::MISER;
                jetonsAMiser=jetonsAMiserRationalite;
            }

            //Si on peut Relancer, on relance
            else if(ia->getJeu()->peutRelancer(ia->getPosition(),2*ia->getJeu()->getMiseCourante()) ){
                action=ACTION::RELANCER;
                jetonsAMiser=calculerMiseRationalite(action);
            }
            //Si on peut suivre, on suit
            else if(ia->getJeu()->peutSuivre(ia->getPosition())){
                action=ACTION::SUIVRE;
            }
            //Si on peut checker on checke
            else if(ia->getJeu()->peutChecker(ia->getPosition())){
                action=ACTION::CHECKER;
            }
        }
        else if(ia->getJeu()->peutChecker(ia->getPosition())){
            action=ACTION::CHECKER;
        }
        else if(ia->getJeu()->peutSuivre(ia->getPosition())){
            action=ACTION::SUIVRE;
        }
    }
    else{ //random E [rationalite- 100]
        vector<ACTION> listeActions;

     //Construction de la liste des actions possibles :

        if(ia->getJeu()->peutMiser(ia->getPosition(), 1)){
            //Miser plus que la mise théroque:
            listeActions.push_back(GROSSE_BLIND); //En fait c'est miserplus que théorique mais

            //Miser moins que la mise théorique:
            listeActions.push_back(PETITE_BLIND);
        }
        //se coucher
        if(ia->getChancesGain()>30){
            listeActions.push_back(ACTION::SE_COUCHER);
        }
        else if(ia->getChancesGain()<50){ //Relancer et/ou Suivre

            if(ia->getJeu()->peutRelancer(ia->getPosition(),2*ia->getJeu()->getMiseCourante())){
                listeActions.push_back(ACTION::RELANCER);
            }

            else if(ia->getJeu()->peutSuivre(ia->getPosition())){
                listeActions.push_back(ACTION::SUIVRE);
            }
            else if(ia->getJeu()->peutChecker(ia->getPosition())){
                listeActions.push_back(ACTION::CHECKER);
            }
        }
        else{ //Checker
            if(ia->getJeu()->peutChecker(ia->getPosition())){
                listeActions.push_back(ACTION::CHECKER);
            }
        }


        //Choix aléatoire d'une des actions de la liste :
        int random=rand()%listeActions.size();

        double miseTheorique=CalculDonneesProfilage::miseTheorique(ia->getChancesGain());
        int jetonsAMiserTheorique=(miseTheorique*ia->getCave())/100;


        if(listeActions.at(random)==GROSSE_BLIND){
            action=ACTION::MISER;

            //borneInférieure=10% de la cave + la mise théorique
            int borneInferieure = jetonsAMiserTheorique+((ia->getCave()*10)/100);

            if(borneInferieure<ia->getCave()){
                //On prend un nombre aléatoire entre borneInferieure et ia->getCave();
                jetonsAMiser=(rand()% (ia->getCave()-borneInferieure+1))+borneInferieure;
            }
            else{
                jetonsAMiser=ia->getCave();
            }
        }
        else if(listeActions.at(random)==PETITE_BLIND){
            action=ACTION::MISER;

            //borneSuperieure=mise théorique -10% de la cave
            int borneSuperieure=jetonsAMiserTheorique-((ia->getCave()*10)/100);

            if(borneSuperieure>0){
                jetonsAMiser=rand()% (borneSuperieure+1);
            }
            else{
                jetonsAMiser=ia->getCave();
            }

        }
        else{
            action=listeActions.at(random);

            if(action==ACTION::RELANCER){
                jetonsAMiser=2*ia->getJeu()->getMiseCourante();
            }
            else if(action==ACTION::SUIVRE){
                jetonsAMiser=ia->getJeu()->getMiseCourante();
            }
        }

    }


    return Action(action,jetonsAMiser);
}


int Resolveur::calculerMiseRationalite(ACTION action){
    //On récupère la mise théorique si le joueur est 100% agressif
    double miseTheorique=CalculDonneesProfilage::miseTheorique(ia->getChancesGain());

    int jetonsAMiser=(miseTheorique*ia->getCave())/100;

    if(action==ACTION::CHECKER){
        jetonsAMiser=-1;
    }
    else if(action==ACTION::SUIVRE){
        jetonsAMiser=ia->getJeu()->getMiseCourante();
    }

    return jetonsAMiser;

}


Action Resolveur::calculerAction(){

    Action actionAgressivite=calculerActionAgressivite();
    Action actionRationalite=calculerActionRationalite();

    ACTION action;
    int jetonsAMiser;

    //fusion des deux résultats :

    //Si les actions ne sont pas les mêmes, on choisit une des deux actions:
    if(actionAgressivite.getAction()!=actionRationalite.getAction()){


        int total = calibrage.getRationalite()+calibrage.getAgressivite();


        int random=rand()%total+1;

        //Si random E [0-agressivité], on prend l'action et les jetons de l'agressivité
        if(random<calibrage.getAgressivite()){
            action=actionAgressivite.getAction();
            jetonsAMiser=actionAgressivite.getMontant();
        }
        //Sinon, random E[agressivite+1 - total], on prend l'action et les jetons de la rationalité
        else{
            action=actionRationalite.getAction();
            jetonsAMiser=actionRationalite.getMontant();
        }

   }
    //Sinon, si l'action est relancer ou miser
   else if(actionAgressivite.getAction()==ACTION::RELANCER || actionAgressivite.getAction()==ACTION::MISER){

        action=actionAgressivite.getAction();

        int maxJetonsAMiser, minJetonsAMiser, tauxMax, tauxMin;

        if(actionAgressivite.getMontant()>actionRationalite.getMontant()){
            maxJetonsAMiser=actionAgressivite.getMontant();
            minJetonsAMiser=actionRationalite.getMontant();
        }
        else{
            maxJetonsAMiser=actionRationalite.getMontant();
            minJetonsAMiser=actionAgressivite.getMontant();
        }
        if(calibrage.getAgressivite()>calibrage.getRationalite()){
            tauxMax=calibrage.getAgressivite();
            tauxMin=calibrage.getRationalite();
        }
        else{
            tauxMin=calibrage.getAgressivite();
            tauxMax=calibrage.getRationalite();
        }

        //On va prendre une mise aléatoire comprise entre le min et le max.
        //Celle-ci aura plus de chances d'être plus proche du taux le plus fort.

        int total=calibrage.getAgressivite()+calibrage.getRationalite();
        jetonsAMiser= minJetonsAMiser+((tauxMax/total)*abs(maxJetonsAMiser-minJetonsAMiser));

    }

    return Action(action,jetonsAMiser);
}

