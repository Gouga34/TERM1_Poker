/*========================================================================
Nom: ScenariosDeTests.cpp       Auteur: Morgane VIDAL
Maj: 07/04/2015          Creation: 01/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant l'implémentation de la classe
                                                    ScenariosDeTests.
=========================================================================*/

#include "../../include/Constantes.h"
#include "../../include/Profilage/ScenariosDeTests.h"
#include "../../include/Profilage/CalculDonneesProfilage.h"
#include "../../include/Interface/Logger.h"


#include <QString>
#include <QIODevice>
#include <QJsonArray>
#include <QByteArray>
#include <QStringList>

#include <cstdlib>
#include <QTextStream>

using namespace std;

ScenariosDeTests::ScenariosDeTests(Profil *profilJoueur, Profil *calibrageIA, Profil calibrageAdversaire){

    calibrageRecherche = calibrageAdversaire;

    calibrageActuelIA=calibrageIA;
    actionReelleJoueur=profilJoueur;

    int agressivite=rand()%100+1;
    calibrageActuelIA->setAgressivite(agressivite);
    actionAttendueJoueur.setAgressivite(-1);
    actionAttendueJoueur.setRationalite(-1);

    calibrageActuelIA->setRationalite(RATIONALITE_IA_PROFILAGE);
}

ScenariosDeTests::~ScenariosDeTests(){

}

void ScenariosDeTests::setCalibrageActuelIA(Profil *calibrage){
    calibrageActuelIA=calibrage;
}

void ScenariosDeTests::setJoueurSeCouche(bool couche){
    joueurSeCouche=couche;
}

bool ScenariosDeTests::getJoueurSeCouche(){
    return joueurSeCouche;
}

void ScenariosDeTests::setDistance(double dist){
    distance=dist;
}

double ScenariosDeTests::getDistance(){
    return distance;
}


Profil* ScenariosDeTests::getCalibrageActuelIA(){
    return calibrageActuelIA;
}

void ScenariosDeTests::setActionAttendueJoueur(Profil actionAttendue){
    actionAttendueJoueur=actionAttendue;
}

Profil ScenariosDeTests::getActionAttendueJoueur() const{
    return actionAttendueJoueur;
}

void ScenariosDeTests::setActionReelleJoueur(Profil *action){
    actionReelleJoueur=action;
}

Profil* ScenariosDeTests::getActionReelleJoueur()const{
    return actionReelleJoueur;
}

void ScenariosDeTests::setChancesDeGain(double chancesGain){
    chancesDeGain=chancesGain;
}

double ScenariosDeTests::getChancesDeGain(){
    return chancesDeGain;
}


bool ScenariosDeTests::sauvegarderPartie(){

    //On ouvre le fichier dans lequel on sauvegardera les données.
    QString nomFichier=QString::fromStdString(actionReelleJoueur->getPseudo())+"_scenarios_tests.csv";
    QFile fichier(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE)+nomFichier);

    int nbParties=0;

    if(!fichier.open(QIODevice::ReadWrite | QIODevice::Text)){
        std::cerr<<"Erreur lors de l'ouverture du fichier "<<std::endl;
    }
    else{

        QTextStream out(&fichier);

        //Si le fichiers est vide, on fait l'initialisation:
        calculerActionAttendueJoueur(fichier);
        calculerDistance();

        //calcul de la rationalité déduite globale et l'agressivité déduite globale:
        double agressiviteGlobale=0.0;
        double rationaliteGlobale=0.0;
        double tauxSimilarite = 0.0;


        if(fichier.size()==0){
            agressiviteGlobale=actionAttendueJoueur.getAgressivite();
            rationaliteGlobale=actionAttendueJoueur.getRationalite();
        }else{
            fichier.seek(0);
            QString ligne=fichier.readLine();

            QStringList liste;
            double rationaliteGlobalePrec=0;
            double agressiviteGlobalePrec=0;

            int donneesReferences=0;
            while(!ligne.isEmpty()){
                liste = ligne.split(",");
                if(donneesReferences==1){
                    rationaliteGlobalePrec=liste.at(RATIONALITE_DEDUITE_GLOBALE).toDouble();
                    agressiviteGlobalePrec=liste.at(AGRESSIVITE_DEDUITE_GLOBALE).toDouble();
                }

                if(liste.at(0)=="agressivite IA"){
                    donneesReferences=1;
                }
                ligne=fichier.readLine();
            }

            fichier.seek(0);
            ligne=fichier.readLine();
            //On récupère le nombre de parties :

            while(!ligne.isEmpty()){
                nbParties++;
                ligne=fichier.readLine();
            }

            agressiviteGlobale=(actionAttendueJoueur.getAgressivite()+(agressiviteGlobalePrec*(nbParties-1)))/nbParties;
            rationaliteGlobale=(actionAttendueJoueur.getRationalite()+(rationaliteGlobalePrec*(nbParties-1)))/nbParties;

            double similariteAgressivite = 100 - abs(agressiviteGlobale - calibrageRecherche.getAgressivite());
            double similariteRationalite = 100 - abs(rationaliteGlobale - calibrageRecherche.getRationalite());

            tauxSimilarite = (similariteAgressivite + similariteRationalite) / 2;
        }

        if(fichier.size()==0){
          out<<"agressivite IA,Chances de gain IA qui profile,Agressivite attendue,Rationalite attendue,Agressivite reelle,Rationalite Reelle,distance moyenne,Agressivite deduite globale,Rationalite deduite globale,taux similarite"<<endl;
        }

         //On écrit déjà l'agressivité de l'IA qui profile et ses chances de gain.
        out<<getCalibrageActuelIA()->getAgressivite()<<","<<getChancesDeGain()<<","<<actionAttendueJoueur.getAgressivite()<<","
           <<actionAttendueJoueur.getRationalite()<<","<<actionReelleJoueur->getAgressivite()<<","<<actionReelleJoueur->getRationalite()<<","
           <<getDistance()<<","<<agressiviteGlobale<<","<<rationaliteGlobale<<","<<tauxSimilarite<<endl;

       fichier.close();
    }

    scenarioSuivant();

    return (nbParties < NOMBRE_PARTIES_PROFILAGE);
}


void ScenariosDeTests::calculerActionAttendueJoueur(QFile& fichierProfil){

    double intervalleAgressiviteAttendueInferieur=0;
    double intervalleAgressiviteAttendueSuperieur=0;

    double intervalleCalibrageInferieur=0;
    double intervalleCalibrageSuperieur=0;

    double intervalleChancesGainIAInferieur=0;
    double intervalleChancesGainIASuperieur=0;

    //Si le fichier n'est pas vide, on regarde dedans si on a pas déjà une valeur pour les chances de gains et l'agressivité actuelle
    ancienneSituationLaPlusProche(fichierProfil);

    //Si on n'a pas d'agressivité attendue et de rationalité attendue:
    if(actionAttendueJoueur.getAgressivite()==-1 || actionAttendueJoueur.getRationalite()==-1){

        //On regarde dans le fichier dans lequel se trouvent les données de base.
        QString nomFichier=QString::fromStdString("scenarios_tests_basiques.csv");
        QFile fichier(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE)+nomFichier);


        if(!fichier.open(QIODevice::ReadWrite)){
            std::cerr<<"Erreur lors de l'ouverture du fichier "<<std::endl;
        }
        else{
            QString ligne=fichier.readLine();
            QStringList liste;
            int ligneCalibrage=0;
            int ligneChancesGains=0;
            int ligneAgressiviteAttendue=0;
            int ligneRationaliteAttendue=0;
            int cpt=0;

            int calibrage=1;

            while(!ligne.isEmpty()){

                 liste = ligne.split(",");

                 //Si on a comme mot clef agressivité, ça veut dire qu'on a déjà traité les palliers de calibrage
                 //et qu'on en est aux palliers d'agressivité
                if(liste.at(0)=="Agressivite"){
                    calibrage=0;
                }
                else if(liste.at(0)=="agressivite IA"){
                    calibrage=2;
                }

                cpt++;

                //Si on cherche l'intervalle du calibrage
                if(calibrage==1)
                {
                    //On regarde si on est dans l'intervalle de l'agressivité de l'IA
                    if(liste.at(0).toDouble()<=calibrageActuelIA->getAgressivite()
                            && calibrageActuelIA->getAgressivite()<=liste.at(1).toDouble()){
                        intervalleCalibrageInferieur=liste.at(0).toDouble();
                        intervalleCalibrageSuperieur=liste.at(1).toDouble();
                        ligneCalibrage=cpt;
                    }

                    // Les paliers pour les chances de gain sont les même
                    //On regarde si on est dans l'intervalle des chances de gains de l'IA
                    if(liste.at(0).toDouble()<=getChancesDeGain() && getChancesDeGain()<=liste.at(1).toDouble()){
                        intervalleChancesGainIAInferieur=liste.at(0).toDouble();
                        intervalleChancesGainIASuperieur=liste.at(1).toDouble();
                        ligneChancesGains=cpt;
                    }
                }

                //Si l'intervalle d'agressivité de l'IA correspond au cptCalibrage
                //et que les chances de gain de l'IA sont dans le bon intervalle,
                if(calibrage==2
                   && liste.at(0).toInt()==ligneCalibrage
                   && liste.at(1).toInt()==ligneChancesGains ){

                    ligneAgressiviteAttendue=liste.at(2).toInt();
                    ligneRationaliteAttendue=liste.at(3).toInt();
                    break;
                    break;
                }

                ligne=fichier.readLine();
            }

            //On récupère l'intervalle d'agressivité attendue du joueur :
            cpt=0;
            fichier.seek(0);
            ligne=fichier.readLine();
            while(!ligne.isEmpty()){
                liste = ligne.split(",");
                cpt++;
                if(cpt==ligneAgressiviteAttendue){
                    intervalleAgressiviteAttendueInferieur=liste.at(0).toDouble();
                    intervalleAgressiviteAttendueSuperieur=liste.at(1).toDouble();
                    break;
                    break;
                }
                ligne=fichier.readLine();
            }

            fichier.seek(0);
        }

        //On calcule le taux d'agressivité attendu :
       double agressiviteAttendue=((getChancesDeGain()-intervalleChancesGainIAInferieur)
                                    *( (intervalleAgressiviteAttendueSuperieur-intervalleAgressiviteAttendueInferieur)/
                                                    (intervalleChancesGainIASuperieur-intervalleChancesGainIAInferieur)))+intervalleAgressiviteAttendueInferieur;

       actionAttendueJoueur.setAgressivite(agressiviteAttendue);

        //Le taux de rationalité attendu est à 50% par défaut :
        actionAttendueJoueur.setRationalite(50.0);
    }
}

void ScenariosDeTests::calculerDistance(){
    //On récupère les distances entre chaque paramètre (agressivité, rationalité, bluff, passivité) :
    double distanceAgressivite=abs(actionAttendueJoueur.getAgressivite()-actionReelleJoueur->getAgressivite());
    double distanceRationalite=abs(actionAttendueJoueur.getRationalite()-actionReelleJoueur->getRationalite());

    //On calcule une distance moyenne:
    distance=(distanceAgressivite+distanceRationalite)/2;
}


void ScenariosDeTests::scenarioSuivant(){

    actionAttendueJoueur.setAgressivite(-1);
    actionAttendueJoueur.setRationalite(-1);

    //On tire aléatoirement un nouveau taux d'agressivite:
    int agressivite=rand()%100+1;

    calibrageActuelIA->setAgressivite(agressivite);
    Logger::getInstance()->ajoutLogs("Calibrage IA profilage: agressivité: "+QString::number(agressivite)+" rationalité: "+QString::number(RATIONALITE_IA_PROFILAGE));
}

void ScenariosDeTests::ancienneSituationLaPlusProche(QFile& fichierProfil){
    ///////////////////////////TODO///////////////////////////////
    ///Faire le même test pour les chances de gain du joueur   ///
    ///quand elles seront implémentées.                        ///
    //////////////////////////////////////////////////////////////
    if(fichierProfil.size()!=0){
        fichierProfil.seek(0);
        QString ligne=fichierProfil.readLine();
        ligne=fichierProfil.readLine(); //on lit une deuxième fois pour sauter la première ligne
        QStringList liste;
        int differenceAgressivitePrecedente=100;
        int differenceChancesGainIAPrecedente=100;
        int distancePrecedence=1000;

        //On va parcourir tout le fichier pour récupérer le profil le plus proche possible s'il y en a un
        while(!ligne.isEmpty()){
            liste=ligne.split(",");

            int differenceAgressivite=abs(liste.at(AGRESSIVITE_IA).toDouble()-calibrageActuelIA->getAgressivite());

            //Si l'agressivité varie de + ou - la variation autorisée, On fait la même chose pour les chances de gain de l'IA
            if(differenceAgressivite>=0){

                //On regarde la différence de chances de gains de l'IA:
                int differenceChancesGainIA=abs(liste.at(CHANCES_GAIN_IA).toDouble()-getChancesDeGain());
                if(differenceChancesGainIA>=0){

                    // On regarde si on est dans le même palier

                    double moyenneDiffPrecedente = static_cast<double>(differenceAgressivitePrecedente + differenceChancesGainIAPrecedente) / 2;
                    double moyenneActuelle = static_cast<double>(differenceAgressivite + differenceChancesGainIA) / 2;

                    if(moyenneActuelle<moyenneDiffPrecedente){

                        differenceAgressivitePrecedente = differenceAgressivite;
                        differenceChancesGainIAPrecedente = differenceChancesGainIA;

                        actionAttendueJoueur.setAgressivite(liste.at(AGRESSIVITE_REELLE).toDouble());
                        actionAttendueJoueur.setRationalite(liste.at(RATIONALITE_REELLE).toDouble());
                        distancePrecedence=liste.at(DISTANCE).toDouble();
                    }
                }
            }

            ligne=fichierProfil.readLine();
        }
    }

}


