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


#include <QString>
#include <QIODevice>
#include <QJsonArray>
#include <QByteArray>
#include <QStringList>

#include <cstdlib>
#include <QTextStream>

using namespace std;

ScenariosDeTests::ScenariosDeTests(Profil *profilJoueur, Profil *calibrageIA){

    calibrageActuelIA=calibrageIA;
    actionReelleJoueur=profilJoueur;

    int agressivite=rand()%100+1;
    calibrageActuelIA->setAgressivite(agressivite);
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


void ScenariosDeTests::sauvegarderPartie(){

    //On ouvre le fichier dans lequel on sauvegardera les données.
    QString nomFichier=QString::fromStdString(actionReelleJoueur->getPseudo())+"_scenarios_tests.csv";
    QFile fichier(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE)+nomFichier);

    QString nomFichierReference=QString::fromStdString("scenarios_tests_basiques.csv");
    QFile fichierReference(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE)+nomFichierReference);

    if(!fichier.open(QIODevice::ReadWrite) || !fichierReference.open(QIODevice::ReadOnly)){
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


        if(fichier.size()==0){
            agressiviteGlobale=actionReelleJoueur->getAgressivite();
            rationaliteGlobale=actionReelleJoueur->getRationalite();
        }else{

            QString ligne=fichierReference.readLine();
            QTextStream fichRefe(&fichierReference);
            QStringList liste;
            double rationaliteGlobalePrec=0;
            double agressiviteGlobalePrec=0;
            int nbParties=0;

            int donneesReferences=0;
            while(fichRefe.atEnd()){
                liste = ligne.split(",");


                if(donneesReferences==1){

                    rationaliteGlobalePrec=liste.at(RATIONALITE_DEDUITE_GLOBALE).toDouble();
                    agressiviteGlobalePrec=liste.at(AGRESSIVITE_DEDUITE_GLOBALE).toDouble();
                }

                if(liste.at(0)=="agressivite IA"){
                    donneesReferences=1;
                }
                ligne=fichierReference.readLine();
            }

            //On récupère le nombre de parties :
            fichier.seek(0);
            while(!ligne.isEmpty()){
                nbParties++;
                ligne=fichier.readLine();
            }

            agressiviteGlobale=(actionReelleJoueur->getAgressivite()+(agressiviteGlobalePrec*(nbParties-1)))/nbParties;
            rationaliteGlobale=(actionReelleJoueur->getRationalite()+(rationaliteGlobalePrec*(nbParties-1)))/nbParties;

        }

        if(fichier.size()==0){
          out<<"agressivite IA,Chances de gain IA,Agressivite attendue,Rationalite attendue,Agressivite reelle,Rationalite Reelle,distance moyenne,Agressivite deduite globale,Rationalite deduite globale"<<endl;
        }


         //On écrit déjà l'agressivité de l'IA qui profile et ses chances de gain.
        out<<getCalibrageActuelIA()->getAgressivite()<<","<<getChancesDeGain()<<","<<actionAttendueJoueur.getAgressivite()<<","
           <<actionAttendueJoueur.getRationalite()<<","<<actionReelleJoueur->getAgressivite()<<","<<actionReelleJoueur->getRationalite()<<","
           <<getDistance()<<","<<agressiviteGlobale<<","<<rationaliteGlobale<<","<<endl;

       fichier.close();
    }
}


void ScenariosDeTests::calculerActionAttendueJoueur(QFile& fichierProfil){

    //Si le fichier n'est pas vide, on regarde dedans si on a pas déjà une valeur pour les chances de gains et l'agressivité actuelle

    QString ligneFichierProfil=fichierProfil.readLine();
    QStringList listeLLecture;
    QStringList listeProfilLePlusProche;
    int diffAgressivite=1000;
    int diffChancesGain=1000;

    while(!ligneFichierProfil.isEmpty()){
        listeLLecture = ligneFichierProfil.split(",");

        if(listeLLecture.at(0)!="agressivite IA"){
            int diffAgLigne=abs(calibrageActuelIA->getAgressivite()-listeLLecture.at(0).toDouble());
            int diffCgLigne=abs(getChancesDeGain()-listeLLecture.at(1).toDouble());

            //Si sur la ligne on a une ag et des chances de gains plus proches des valeurs actuelles
            //On garde la nouvelle ligne
            if(diffAgLigne<=diffAgressivite && diffCgLigne<=diffChancesGain){
                diffAgressivite=diffAgLigne;
                diffChancesGain=diffCgLigne;
                listeProfilLePlusProche=listeLLecture;
            }
        }
        ligneFichierProfil=fichierProfil.readLine();
    }

    //Si la différence est inférieure ou égale à 10%
    if(diffAgressivite<=10 && diffChancesGain<=10){
        actionAttendueJoueur.setAgressivite(listeProfilLePlusProche.at(4).toDouble());
        actionAttendueJoueur.setRationalite(listeProfilLePlusProche.at(5).toDouble());

    }

    else{


    double intervalleAgressiviteAttendueInferieur=0;
    double intervalleAgressiviteAttendueSuperieur=0;

    double intervalleCalibrageInferieur=0;
    double intervalleCalibrageSuperieur=0;

    double intervalleChancesGainIAInferieur=0;
    double intervalleChancesGainIASuperieur=0;

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
            int cpt=0;

            int calibrage=1;
            //int chancesGain=1;

            while(!ligne.isEmpty()){

                 liste = ligne.split(",");

                 //Si on a comme mot clef agressivité, ça veut dire qu'on a déjà traité les palliers de calibrage et qu'on en est aux palliers d'agressivité
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

                    //On regarde si on est dans l'intervalle des chances de gains de l'IA
                    if(liste.at(0).toDouble()<=getChancesDeGain() && getChancesDeGain()<=liste.at(1).toDouble()){
                        intervalleChancesGainIAInferieur=liste.at(0).toDouble();
                        intervalleChancesGainIASuperieur=liste.at(1).toDouble();
                        ligneChancesGains=cpt;
                    }



                }

                //Si l'intervalle d'agressivité de l'IA correspond au cptCalibrage et que les chances de gain de l'IA sont dans le bon intervalle,
                if(calibrage==2
                   && liste.at(0).toInt()==ligneCalibrage
                   && liste.at(1).toInt()==ligneChancesGains ){

                    ligneAgressiviteAttendue=liste.at(2).toInt();
                    break;
                    break;
                }

                ligne=fichier.readLine();
            }

        //On récupère l'intervalle d'agressivité attendue du joueur :
        cpt=0;
        fichier.seek(0);
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


        //On calcule le taux d'agressivité attendu :
       double agressiviteAttendue=((getChancesDeGain()-intervalleChancesGainIAInferieur)
                                    *( (intervalleAgressiviteAttendueSuperieur-intervalleAgressiviteAttendueInferieur)/
                                                    (intervalleChancesGainIASuperieur-intervalleChancesGainIAInferieur)))+intervalleAgressiviteAttendueInferieur;


        actionAttendueJoueur.setAgressivite(agressiviteAttendue);

        //Le taux de rationalité attendu est à 80% par défaut :
        actionAttendueJoueur.setRationalite(80.0);

        fichier.seek(0);
    }
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
    //On tire aléatoirement un nouveau taux d'agressivite:
    int agressivite=rand()%100+1;
    calibrageActuelIA->setAgressivite(agressivite);

}


