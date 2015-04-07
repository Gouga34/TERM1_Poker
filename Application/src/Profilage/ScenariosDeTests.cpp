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

    ////////////////////////////////////////////////////////////////
    //////////                   TODO                     //////////
    ////////////////////////////////////////////////////////////////
    /*
        Trouver pourquoi les chances de gains fournies par l'IA sont toujours égales à 0.
        Effectuer le calcul des résultats en fonction des résultats précédents.
    */




    std::cout<<"chances gain IA : "<<getChancesDeGain()<<std::endl;

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



        calculerActionAttendueJoueur();
        calculerDistance();

        //calcul de la rationalité déduite globale et l'agressivité déduite globale:
        double agressiviteGlobale=0.0;
        double rationaliteGlobale=0.0;

        std::cout<<"taille : "<<fichier.size()<<std::endl;
        if(fichier.size()==0){
            agressiviteGlobale=actionReelleJoueur->getAgressivite();
            rationaliteGlobale=actionReelleJoueur->getRationalite();
        }else{

            QString ligne=fichierReference.readLine();
            QStringList liste;
            double rationaliteGlobalePrec=0;
            double agressiviteGlobalePrec=0;
            int nbParties=0;

            int donneesReferences=0;
            while(!ligne.isNull()){
                liste = ligne.split(";");


                //std:cout<<"ligne: "<<ligne.toStdString()<<std::endl;
                if(donneesReferences==1){
                    nbParties++;
                    rationaliteGlobalePrec=liste.at(RATIONALITE_DEDUITE_GLOBALE).toDouble();
                    agressiviteGlobalePrec=liste.at(AGRESSIVITE_DEDUITE_GLOBALE).toDouble();
                }

                if(liste.at(0)=="agressivite IA"){
                    donneesReferences=1;
                }
                ligne=fichier.readLine();
            }

            agressiviteGlobale=(actionReelleJoueur->getAgressivite()+(agressiviteGlobalePrec*(nbParties-1)))/nbParties;
            rationaliteGlobale=(actionReelleJoueur->getRationalite()+(rationaliteGlobalePrec*(nbParties-1)))/nbParties;

        }

        if(fichier.size()==0){
          out<<"agressivité IA;Chances de gain IA;Agressivite attendue;Rationalite attendue;Agressivite reelle;Rationalite Reelle;distance moyenne;Agressivite deduite globale;Rationalite deduite globale"<<endl;
        }

        //On écrit déjà l'agressivité de l'IA qui profile et ses chances de gain.
        out<<getCalibrageActuelIA()->getAgressivite()<<";"<<getChancesDeGain()<<";"<<actionAttendueJoueur.getAgressivite()<<";"
           <<actionAttendueJoueur.getRationalite()<<";"<<actionReelleJoueur->getAgressivite()<<";"<<actionReelleJoueur->getRationalite()<<";"
           <<getDistance()<<";"<<agressiviteGlobale<<";"<<rationaliteGlobale<<";"<<endl;

       fichier.close();
    }
}


void ScenariosDeTests::calculerActionAttendueJoueur(){

    //TODO: Calculer actions en fonction des tests précédents.

    int intervalleAgressiviteAttendueInferieur=0;
    int intervalleAgressiviteAttendueSuperieur=0;
    int intervalleCalibrageInferieur=0;
    int intervalleCalibrageSuperieur=0;

    //si le fichier envoyé en param est vide, on prend pr action attendue ce qu'on avait mis en place.
    //if(fichier.size()==0){
        //On regarde dans le fichier dans lequel se trouvent les données de base.

    QString nomFichier=QString::fromStdString("scenarios_tests_basiques.csv");
    QFile fichier(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE)+nomFichier);


    if(!fichier.open(QIODevice::ReadWrite)){
        std::cerr<<"Erreur lors de l'ouverture du fichier "<<std::endl;
    }
    else{


            QString ligne=fichier.readLine();
            QStringList liste;


            int calibrage=1;
            while(!ligne.isNull()){

                 liste = ligne.split(";");

                 //Si on a comme mot clef agressivité, ça veut dire qu'on a déjà traité les palliers de calibrage et qu'on en est aux palliers d'agressivité
                if(liste.at(0)=="Agressivite"){
                    calibrage=0;
                }


                //Si on cherche l'intervalle du calibrage
                if(calibrage==1
                        && liste.at(0).toDouble()<=calibrageActuelIA->getAgressivite()
                        && calibrageActuelIA->getAgressivite()<=liste.at(1).toDouble()){

                    intervalleCalibrageInferieur=liste.at(0).toDouble();
                    intervalleCalibrageSuperieur=liste.at(1).toDouble();
                }

                //Si on cherche l'intervale de l'agressivite attendue
                if(calibrage==0
                        && liste.at(0).toDouble() <= getChancesDeGain()
                        && getChancesDeGain()<=liste.at(1).toDouble()){

                    intervalleAgressiviteAttendueInferieur=liste.at(0).toDouble();
                    intervalleAgressiviteAttendueSuperieur=liste.at(1).toDouble();
                    break;
                }

                ligne=fichier.readLine();
            }


        //On calcule le taux d'agressivité attendu :
        double agressiviteAttendue=((getChancesDeGain()-intervalleCalibrageInferieur)
                                    *( (intervalleAgressiviteAttendueSuperieur-intervalleAgressiviteAttendueInferieur)/
                                                    (intervalleCalibrageSuperieur-intervalleCalibrageInferieur)));

        actionAttendueJoueur.setAgressivite(agressiviteAttendue);

        //Le taux de rationalité attendu est à 80% par défaut :
        actionAttendueJoueur.setRationalite(80.0);

        fichier.seek(0);
    }
   //}
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


