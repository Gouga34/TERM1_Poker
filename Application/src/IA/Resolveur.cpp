/*========================================================================
Nom: Resolveur.cpp       Auteur: Morgane Vidal
Maj: 31/03/2015          Creation: 31/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les corps des méthodes
                                        la classe Resolveur.
=========================================================================*/


////////////////::TODO//////////////////////////////////////////////
/// action tapis et autre action diff : à gérer dans la fusion /////
/// ////////////////////////////////////////////////////////////////

#include "../../include/IA/Resolveur.h"
#include "../../include/Constantes.h"
#include "../../include/Profilage/CalculDonneesProfilage.h"

#include <vector>

using namespace std;

Resolveur::Resolveur(IntelligenceArtificielle *ia2):jeuAgressif(false), ia(ia2){
    calibrage=new Profil;
}


Resolveur::~Resolveur(){
    delete calibrage;
}

bool Resolveur::estAgressif() const {
    return jeuAgressif;
}

void Resolveur::setJeuAgressif(bool ag) {
    jeuAgressif = ag;
}

void Resolveur::setCalibrage(Profil profil){

    calibrage->setAgressivite(profil.getAgressivite());
    calibrage->setRationalite(profil.getRationalite());

}


Profil* Resolveur::getCalibrage() {
    return calibrage;
}


Action Resolveur::calculerActionAgressivite(){
    calculerTotalMiseTheoriqueAgressivite();

    ACTION action=PAS_ENCORE_D_ACTION;
    int jetonsAMiser = -1; //mise à effectuer s'il y en a une. Correspond au nombre de jetons

    double miseTheoriqueTour=ia->getCumulMisesEtRelances()*HAUSSE_MISES_AGRESSIVITE - ia->getCumulMisesEtRelances();
    //Si le total des mises de l'IA est supérieur à mth, on checke ou on se couche.
    //Sinon, on mise/relance/suit
    if(miseTheoriqueTour>miseTotaleTheoriqueAgressivite-ia->getCumulMisesEtRelances() || ia->getJeu()->getEtape()!=RIVER){
        miseTheoriqueTour=miseTotaleTheoriqueAgressivite-ia->getCumulMisesEtRelances();
    }

    //Si on peut miser
    if(ia->getJeu()->peutMiser(ia->getPosition(),1)){
        // Si on a l'argent
        if(ia->getJeu()->peutMiser(ia->getPosition(),miseTheoriqueTour)){
            action=MISER;
            jetonsAMiser=miseTheoriqueTour;
        }
        else{   // On fait tapis
            action=TAPIS;
        }
    }
    //Si le joueur adverse a misé ou relancé
    else if(ia->getJeu()->getLastAction(ia->getJeu()->getPositionJoueurAdverse(ia->getPosition())) == MISER
                || ia->getJeu()->getLastAction(ia->getJeu()->getPositionJoueurAdverse(ia->getPosition())) == RELANCER
                || ia->getJeu()->getLastAction(ia->getJeu()->getPositionJoueurAdverse(ia->getPosition())) == GROSSE_BLIND
                || ia->getJeu()->getLastAction(ia->getJeu()->getPositionJoueurAdverse(ia->getPosition())) == TAPIS){

        double variationAutoriseeMiseAdversaire=(VARIATION_AUTORISEE/100) * ia->getJeu()->getMiseCourante();

        //Si la mise courante est inférieure à ce qu'il reste à miser +10% de la mise
        if(ia->getJeu()->getMiseCourante()<=miseTotaleTheoriqueAgressivite-ia->getCumulMisesEtRelances()+variationAutoriseeMiseAdversaire){

            //On continue à jouer

            //Si la mise théorique est inférieure à la relance minimum (-10%), on suit
            if(miseTheoriqueTour<2*ia->getJeu()->getMiseCourante()-variationAutoriseeMiseAdversaire){
                action=SUIVRE;
            }
            else{   // On relance
                //Si on peut relancer
                if(ia->getJeu()->peutRelancer(ia->getPosition(), 2*ia->getJeu()->getMiseCourante())){
                    if(miseTheoriqueTour<2*ia->getJeu()->getMiseCourante()){
                        jetonsAMiser=2*ia->getJeu()->getMiseCourante();
                    }
                    else{
                        if(ia->getJeu()->peutRelancer(ia->getPosition(), miseTheoriqueTour)){
                            jetonsAMiser=miseTheoriqueTour;
                        }
                        else{
                            jetonsAMiser=ia->getCave();
                        }
                    }
                    action=RELANCER;
                }
                else{

                    //AJOUTE
                    if(ia->getJeu()->peutSuivre(ia->getPosition())){
                        action=SUIVRE;
                    }
                }
            }
        }
        else{
            if(ia->getJeu()->peutChecker(ia->getPosition())){
                action=CHECKER;
            }
            else{
                action=SE_COUCHER;
            }
        }
    }
    else if(ia->getJeu()->peutChecker(ia->getPosition())){
        action=CHECKER;
    }

    if(action==PAS_ENCORE_D_ACTION){
        std::cout<<"pas encore d'action"<<std::endl;
    }

   // std::cout<<"action: "<<action<<" jetons a miser: "<<jetonsAMiser<<std::endl;

    return Action(action,jetonsAMiser);
}



void Resolveur::calculerTotalMiseTheoriqueAgressivite(){
    double tauxMiseTotale=0.0;

    double palierSuperieur;
    double palierInferieur;
    double miseTheoriqueInferieure;
    double miseTheoriqueSuperieure;
    if(calibrage->getAgressivite()<AGRESSIVITE::PALIER1::FIN_AG_THEORIQUE){
        palierInferieur=AGRESSIVITE::PALIER1::DEBUT_AG_THEORIQUE;
        palierSuperieur=AGRESSIVITE::PALIER1::FIN_AG_THEORIQUE;
        miseTheoriqueInferieure=AGRESSIVITE::PALIER1::DEBUT_MISE_TOTALE;
        miseTheoriqueSuperieure=AGRESSIVITE::PALIER1::FIN_MISE_TOTALE;
    }
    else if(calibrage->getAgressivite()<AGRESSIVITE::PALIER2::FIN_AG_THEORIQUE){
        palierInferieur=AGRESSIVITE::PALIER2::DEBUT_AG_THEORIQUE;
        palierSuperieur=AGRESSIVITE::PALIER2::FIN_AG_THEORIQUE;
        miseTheoriqueInferieure=AGRESSIVITE::PALIER2::DEBUT_MISE_TOTALE;
        miseTheoriqueSuperieure=AGRESSIVITE::PALIER2::FIN_MISE_TOTALE;
    }
    else if(calibrage->getAgressivite()<AGRESSIVITE::PALIER3::FIN_AG_THEORIQUE){
        palierInferieur=AGRESSIVITE::PALIER3::DEBUT_AG_THEORIQUE;
        palierSuperieur=AGRESSIVITE::PALIER3::FIN_AG_THEORIQUE;
        miseTheoriqueInferieure=AGRESSIVITE::PALIER3::DEBUT_MISE_TOTALE;
        miseTheoriqueSuperieure=AGRESSIVITE::PALIER3::FIN_MISE_TOTALE;
    }
    else{
        tauxMiseTotale = AGRESSIVITE::PALIER4::MISE_TOTALE;
    }

    if(tauxMiseTotale==0.0){
        tauxMiseTotale=((calibrage->getAgressivite()-palierInferieur)*((miseTheoriqueSuperieure-miseTheoriqueInferieure)/(palierSuperieur-palierInferieur)))+miseTheoriqueInferieure;
    }
    miseTotaleTheoriqueAgressivite=(tauxMiseTotale/100) * CAVE_JOUEURS;
}


Action Resolveur::calculerActionRationalite(){
    ACTION action;
    int jetonsAMiser =-1; //-1 si pas une action avec une mise

    //On prend un nombre aléatoire
    int random = rand()%101;
    if(random<(calibrage->getRationalite()-1)){ //Si random E [0 - (rationalite-1)]

        int jetonsMiseTheorique=calculerMiseRationalite(ACTION::MISER);

        //Si on peut pas checker et peut de chances de gain :
        if(ia->getChancesGain()<30.0 && !(ia->getJeu()->peutChecker(ia->getPosition()))){
            action=ACTION::SE_COUCHER;
        }
        else if(ia->getCumulMisesEtRelances()<jetonsMiseTheorique){
            int jetonsAMiserRationalite=jetonsMiseTheorique-ia->getCumulMisesEtRelances();

            if(ia->getJeu()->peutMiser(ia->getPosition(),jetonsAMiserRationalite)){ //Si on peut miser
                action=ACTION::MISER;
                jetonsAMiser=jetonsAMiserRationalite;
            }

            //Si on peut Relancer, on relance
            else if(ia->getJeu()->peutRelancer(ia->getPosition(),calculerMiseRationalite(ACTION::RELANCER)) ){
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
        if(ia->getChancesGain()>70){
            listeActions.push_back(ACTION::SE_COUCHER);
        }
        else if(ia->getChancesGain()<=70){ //Relancer et/ou Suivre

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
                jetonsAMiser=(rand()% borneSuperieure) + 1;
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
    else if(action==ACTION::RELANCER){
        //Si on peut relancer
        if(jetonsAMiser>=2*ia->getJeu()->getMiseCourante()){
            jetonsAMiser=2*ia->getJeu()->getMiseCourante();
        }
        else{
            jetonsAMiser=0;
        }
    }

    return jetonsAMiser;

}


Action Resolveur::calculerAction(){

    Action actionAgressivite=calculerActionAgressivite();
    Action actionRationalite=calculerActionRationalite();

    bool tirageAleatoire=false;
    ACTION action=PAS_ENCORE_D_ACTION;
    int jetonsAMiser = -1;

    if (jeuAgressif) {
        action = actionAgressivite.getAction();
        jetonsAMiser = actionAgressivite.getMontant();
    }
    // Fusion des deux résultats :Si les actions ne sont pas les mêmes, on choisit une des deux actions:
    else if(actionAgressivite.getAction()!=actionRationalite.getAction()){
        //CHECKER et SE_COUCHER
        if((actionAgressivite.getAction()==CHECKER && actionRationalite.getAction()==SE_COUCHER)
                ||(actionAgressivite.getAction()==SE_COUCHER && actionRationalite.getAction()==CHECKER) ){
            action=CHECKER;
        }
        //CHECKER et SUIVRE
        if((actionAgressivite.getAction()==CHECKER && actionRationalite.getAction()==SUIVRE)
                ||(actionAgressivite.getAction()==SUIVRE && actionRationalite.getAction()==CHECKER) ){
            tirageAleatoire=true;
        }
        //CHECKER et MISER
        if((actionAgressivite.getAction()==CHECKER && actionRationalite.getAction()==MISER)
                ||(actionAgressivite.getAction()==MISER && actionRationalite.getAction()==CHECKER)){
            if(actionAgressivite.getAction()==CHECKER){
                actionAgressivite.setAction(MISER);
                actionAgressivite.setMontant(1);
            }
            else{
                actionRationalite.setAction(MISER);
                actionRationalite.setMontant(1);
            }
        }
        //SUIVRE et RELANCER
        if((actionAgressivite.getAction()==SUIVRE && actionRationalite.getAction()==RELANCER)
                ||(actionAgressivite.getAction()==RELANCER && actionRationalite.getAction()==SUIVRE)){
            if(actionAgressivite.getAction()==RELANCER){
                if(actionAgressivite.getMontant()>ia->getJeu()->getMiseCourante()*2){
                    jetonsAMiser=ia->getJeu()->getMiseCourante()*2;
                    action=RELANCER;
                }
                else{
                    action=SUIVRE;
                }

            }
            else{
                if(actionRationalite.getMontant()>ia->getJeu()->getMiseCourante()*2){
                    jetonsAMiser=ia->getJeu()->getMiseCourante()*2;
                    action=RELANCER;
                }
                else{
                    action=SUIVRE;
                }
            }
        }
        //SUIVRE et SE_COUCHER
        if((actionAgressivite.getAction()==SUIVRE && actionRationalite.getAction()==SE_COUCHER)
                ||(actionAgressivite.getAction()==SE_COUCHER && actionRationalite.getAction()==SUIVRE)){
            tirageAleatoire=true;
        }
        //RELANCER et SE_COUCHER
        if((actionAgressivite.getAction()==SE_COUCHER && actionRationalite.getAction()==RELANCER)
                ||(actionAgressivite.getAction()==RELANCER && actionRationalite.getAction()==SE_COUCHER)){
            action=SUIVRE;
        }
        //MISER et SE_COUCHER
        if((actionAgressivite.getAction()==MISER && actionRationalite.getAction()==SE_COUCHER)
                ||(actionAgressivite.getAction()==SE_COUCHER && actionRationalite.getAction()==MISER)){
            if(actionAgressivite.getAction()==SE_COUCHER){
                actionAgressivite.setAction(MISER);

                actionRationalite.setMontant(actionRationalite.getMontant()/2);
            }
            else{
                actionRationalite.setAction(MISER);

                actionAgressivite.setMontant(actionAgressivite.getMontant()/2);
            }
        }
        //TAPIS et MISER
        if((actionAgressivite.getAction()==TAPIS && actionRationalite.getAction()==MISER)
                || (actionAgressivite.getAction()==MISER && actionRationalite.getAction()==TAPIS)){
            if(actionAgressivite.getAction()==TAPIS){
                actionAgressivite.setAction(MISER);
                actionAgressivite.setMontant(ia->getCave());
            }else{
                actionRationalite.setAction(MISER);
                actionRationalite.setMontant(ia->getCave());
            }
        }
        //TAPIS et RELANCER
        if((actionAgressivite.getAction()==TAPIS && actionRationalite.getAction()==RELANCER)
                || (actionAgressivite.getAction()==RELANCER && actionRationalite.getAction()==TAPIS)){
            if(actionAgressivite.getAction()==TAPIS){
                actionAgressivite.setAction(RELANCER);
                actionAgressivite.setMontant(ia->getCave());
            }
            else{
                actionRationalite.setAction(RELANCER);
                actionRationalite.setMontant(ia->getCave());
            }
        }
        //TAPIS et SE_COUCHER
        if((actionAgressivite.getAction()==TAPIS && actionRationalite.getAction()==SE_COUCHER)
            || (actionAgressivite.getAction()==SE_COUCHER && actionRationalite.getAction()==TAPIS) ){

            int jetons = (ia->getCave() == 1) ? 1 : ia->getCave()/2;

            if(ia->getJeu()->peutSuivre(ia->getPosition())){
                action=SUIVRE;
            }
            else if(ia->getJeu()->peutMiser(ia->getPosition(),jetons)){
                action=MISER;
                jetonsAMiser=jetons;
            }
        }
        //TAPIS et CHECKER
        if((actionAgressivite.getAction()==TAPIS && actionRationalite.getAction()==CHECKER)
                || (actionAgressivite.getAction()==CHECKER && actionRationalite.getAction()==TAPIS)){
            if(ia->getJeu()->peutSuivre(ia->getPosition())){
                action=SUIVRE;
            }
            else{
                int jetons = (ia->getCave() == 1) ? 1 : ia->getCave()/2;

                if(ia->getJeu()->peutMiser(ia->getPosition(),jetons)){
                    action=MISER;
                    jetonsAMiser=jetons;
                }
            }
        }


        if(tirageAleatoire){
            int total = calibrage->getRationalite()+calibrage->getAgressivite();

            int random=rand()%total+1;

            //Si random E [0-agressivité], on prend l'action et les jetons de l'agressivité
            if(random<calibrage->getAgressivite()){
                action=actionAgressivite.getAction();
                jetonsAMiser=actionAgressivite.getMontant();
            }
            //Sinon, random E[agressivite+1 - total], on prend l'action et les jetons de la rationalité
            else{
                action=actionRationalite.getAction();
                jetonsAMiser=actionRationalite.getMontant();
            }

        }
   }
    //Sinon, si l'action est relancer ou miser
   else if(actionAgressivite.getAction()==actionRationalite.getAction() && (actionAgressivite.getAction()==ACTION::RELANCER || actionAgressivite.getAction()==ACTION::MISER)){

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
        if(calibrage->getAgressivite()>calibrage->getRationalite()){
            tauxMax=calibrage->getAgressivite();
            tauxMin=calibrage->getRationalite();
        }
        else{
            tauxMin=calibrage->getAgressivite();
            tauxMax=calibrage->getRationalite();
        }

        //On va prendre une mise comprise entre le min et le max.
        //Celle-ci aura plus de chances d'être plus proche du taux le plus fort.

        int total=calibrage->getAgressivite()+calibrage->getRationalite();
        jetonsAMiser= minJetonsAMiser+((tauxMax/total)*abs(maxJetonsAMiser-minJetonsAMiser));

    }else{
       if(action==PAS_ENCORE_D_ACTION){
            action=actionAgressivite.getAction();
       }
    }

    if(jetonsAMiser > ia->getCave()){
        jetonsAMiser = ia->getCave();
    }

    //std::cout<<"Joueur: "<<ia->getPosition()<<" action: "<<action<<" jetonsAMiser: "<<jetonsAMiser<<"mthTot: "<<miseTotaleTheoriqueAgressivite<<" totMises"<<ia->getMiseTotale()<<std::endl;
    return Action(action,jetonsAMiser);
}

