/*========================================================================
Nom: ScenariosDeTests.cpp       Auteur: Morgane VIDAL
Maj: 01/04/2015          Creation: 01/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant l'implémentation de la classe
                                                    ScenariosDeTests.
=========================================================================*/

#include "../../include/Constantes.h"
#include "../../include/Profilage/ScenariosDeTests.h"
#include "../../include/Profilage/CalculDonneesProfilage.h"

#include <QFile>
#include <QString>
#include <QIODevice>
#include <QJsonArray>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <cstdlib>

using namespace std;

ScenariosDeTests::ScenariosDeTests(Profil *profilJoueur, Profil *calibrageIA){

    calibrageActuelIA=calibrageIA;
    actionReelleJoueur=profilJoueur;


    QString nomFichier = QString::fromStdString(profilJoueur->getPseudo())+"_scenarios_tests.json";
    //On vérifie si on a déjà commencé à profiler le joueur
    QFile fichier(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE)+nomFichier);

    //Si on a pas réussi à ouvrir le fichier,
    //le profilage n'a pas encore commencé
    if(!fichier.open(QIODevice::ReadOnly)){
        numeroDuTestActuel=1;
        calibrageActuelIA->setAgressivite(100);
        calibrageActuelIA->setRationalite(100);
        numeroCalibrage=0;

    }
    else{//On a déjà commencé à profiler le joueur.
        //On récupère la taille du fichier pour avoir le nombre de parties déjà effectuées.
        QByteArray donneesJson = fichier.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(donneesJson));
        QJsonObject json(doc.object());
        QJsonArray scenarioDeTests = json["ScenariosDeTests"].toArray();
        QJsonObject profilJoueur=json["ProfilJoueur"].toObject();
        //QJsonArray calibrages = scenarioDeTests["calibrages"].toArray();

        //On récupère le dernier calibrage
        int nombreParties=profilJoueur["nombreParties"].toInt();
        numeroCalibrage= scenarioDeTests[nombreParties].toInt();
        QJsonObject calibrageIA=scenarioDeTests.at(nombreParties-1).toObject();
        calibrageActuelIA->setAgressivite(calibrageIA["agressivite"].toDouble());
        calibrageActuelIA->setRationalite(calibrageIA["rationalite"].toDouble());

        fichier.close();
        //On passe au calibrage suivant (donc au scenario de tests suivant)
        scenarioSuivant(scenarioDeTests);
    }

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


void ScenariosDeTests::setNumeroDuTestActuel(int numeroTest){
    numeroDuTestActuel=numeroTest;
}

int ScenariosDeTests::getNumeroDuTestActuel() const{
    return numeroDuTestActuel;
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

//TODO : REPRENDRE LE CODE DE CETTE METHODE
void ScenariosDeTests::sauvegarderPartie(){

    //On ouvre le fichier dans lequel on sauvegardera les données.
    QString nomFichier=QString::fromStdString(actionReelleJoueur->getPseudo())+"_scenarios_tests.json";
    QFile fichier(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE)+nomFichier);


    if(!fichier.open(QIODevice::ReadWrite)){
        std::cerr<<"Erreur lors de l'ouverture du fichier "<<std::endl;
    }
    else{
        //Récupération du contexte existant :
        QByteArray donneesJson = fichier.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(donneesJson));
        QJsonObject json(doc.object());
        QJsonArray calibrages = json["ScenariosDeTests"].toArray();

        //Données de la partie :
        QJsonObject partie;

        partie["ChancesGain"]=getChancesDeGain(); //Chances de gains

        //Action attendue du joueur
        calculerActionAttendueJoueur(json);

        QJsonObject actionAttendue;

        actionAttendue["agressivite"]=actionAttendueJoueur.getAgressivite();
        actionAttendue["rationalite"]=actionAttendueJoueur.getRationalite();
        actionAttendue["bluff"]=actionAttendueJoueur.getBluff();
        actionAttendue["passivite"]=actionAttendueJoueur.getPassivite();

        partie["ProfilAttendu"]=actionAttendue;

        //Action réelle
        QJsonObject actionReelle;

        actionReelle["agressivite"]=actionReelleJoueur->getAgressivite();
        actionReelle["rationalite"]=actionReelleJoueur->getRationalite();
        actionReelle["bluff"]=actionReelleJoueur->getBluff();
        actionReelle["passivite"]=actionReelleJoueur->getPassivite();

        partie["ProfilReel"]=actionReelle;

        //Distance entre l'action attendue et l'action réelle
        calculerDistance();

        partie["distance"]=getDistance();


        //Incrémentation des competurs de profil:
        //Si le fichier json est vide, on crée le profil du joueur

        QJsonObject profilJoueur;

        if(json.size()==1){
            profilJoueur["agressivite"]=0;
            profilJoueur["rationalite"]=0;
            profilJoueur["passivite"]=0;
            profilJoueur["bluff"]=0;
            profilJoueur["nombreParties"]=1;
        }
        else{
            profilJoueur=json["ProfilJoueur"].toObject();
        }

        PROFIL_JOUEUR::PROFIL_JOUEUR maxTaux = PROFIL_JOUEUR::AGRESSIVITE, secondTaux = PROFIL_JOUEUR::AGRESSIVITE;
        double max=0;

        if(actionReelleJoueur->getAgressivite()>max){
            secondTaux=maxTaux;
            maxTaux=PROFIL_JOUEUR::AGRESSIVITE;
            max=actionReelleJoueur->getAgressivite();
        }
        if(actionReelleJoueur->getRationalite()>max){
            secondTaux=maxTaux;
            maxTaux=PROFIL_JOUEUR::RATIONALITE;
            max=actionReelleJoueur->getRationalite();
        }
        if(actionReelleJoueur->getBluff()>max){
            secondTaux=maxTaux;
            maxTaux=PROFIL_JOUEUR::BLUFF;
            max=actionReelleJoueur->getBluff();
        }
        if(actionReelleJoueur->getPassivite()>max){
            secondTaux=maxTaux;
            maxTaux=PROFIL_JOUEUR::PASSIVITE;
            max=actionReelleJoueur->getPassivite();
        }

        if(maxTaux==PROFIL_JOUEUR::AGRESSIVITE){
            profilJoueur["agressivite"] = profilJoueur["agressivite"].toInt()+1;
        }
        else if(maxTaux==PROFIL_JOUEUR::RATIONALITE){
            profilJoueur["rationalite"]=profilJoueur["rationalite"].toInt()+1;
        }
        else if(maxTaux==PROFIL_JOUEUR::PASSIVITE){
            profilJoueur["passivite"]=profilJoueur["passivite"].toInt()+1;
        }
        else if(maxTaux==PROFIL_JOUEUR::BLUFF){
            profilJoueur["bluff"]=profilJoueur["bluff"].toInt()+1;
        }

        if(secondTaux==PROFIL_JOUEUR::AGRESSIVITE){
            profilJoueur["agressivite"]= profilJoueur["agressivite"].toInt()+1;
        }
        else if(secondTaux==PROFIL_JOUEUR::RATIONALITE){
            profilJoueur["rationalite"]=profilJoueur["rationalite"].toInt()+1;
        }
        else if(secondTaux==PROFIL_JOUEUR::PASSIVITE){
            profilJoueur["passivite"]=profilJoueur["passivite"].toInt()+1;
        }
        else if(secondTaux==PROFIL_JOUEUR::BLUFF){
             profilJoueur["bluff"]=profilJoueur["bluff"].toInt()+1;
        }



        //ajout du calibrage de l'IA s'il n'existe pas encore
        if(calibrages.size()<25){
            QJsonObject calibrage;
            QJsonArray iterations;

            //Si le calibrage n'existe pas encore
            if(calibrages.size() <= numeroCalibrage){
                calibrage["agressivite"]=calibrageActuelIA->getAgressivite();
                calibrage["rationalite"]=calibrageActuelIA->getRationalite();
                calibrage["distanceMoyenne"]=100;
            }
            else{
                calibrage=calibrages.at(numeroCalibrage).toObject();
                iterations=calibrage["iterations"].toArray();
            }

            iterations.append(partie);

            calibrage["iterations"]=iterations;

            //Modification de la distance moyenne du calibrage:
            //On commence par calculer la moyenne du calibrage
            double sommeDistances=0;
            for(int i=0;i<iterations.size();i++){
                sommeDistances+=iterations[i].toObject()["distance"].toDouble();
            }

            calibrage["distanceMoyenne"]=sommeDistances/static_cast<double>(iterations.size());

            //Si le calibrage n'existe pas encore
            if(numeroCalibrage < calibrages.size()
                && calibrages.at(numeroCalibrage).toObject()["agressivite"]==calibrageActuelIA->getAgressivite()
                && calibrages.at(numeroCalibrage).toObject()["rationalite"]==calibrageActuelIA->getRationalite()){

                calibrages[numeroCalibrage]=calibrage;
            }
            else{
                calibrages.append(calibrage);
            }
        }

        json["ScenariosDeTests"]=calibrages;
        json["ProfilJoueur"]=profilJoueur;

        scenarioSuivant(calibrages);

        doc.setObject(json);
        fichier.resize(0);
        fichier.write(doc.toJson());

        fichier.close();
    }


}

void ScenariosDeTests::calculerActionAttendueJoueur(QJsonObject json){

    QJsonArray calibrages = json["calibrages"].toArray();
    QJsonObject calibrage=calibrages.at(numeroCalibrage).toObject();
    int numeroLigneLaPlusProche=anciennesChancesDeGain(calibrage);


    //Si on n'a pas de ligne ayant des chances de gains égales aux chances de gains du jeu actuel
    if(numeroDuTestActuel==0 || numeroLigneLaPlusProche==-1){

        if(calibrages.size()>=1){
            QJsonObject profilJoueur=json["ProfilJoueur"].toObject();
            actionAttendueJoueur.setAgressivite(CalculDonneesProfilage::taux(profilJoueur["agressivite"].toDouble(),profilJoueur["nombreParties"].toDouble()));
            actionAttendueJoueur.setRationalite(CalculDonneesProfilage::taux(profilJoueur["rationalite"].toDouble(),profilJoueur["nombreParties"].toDouble()));
            actionAttendueJoueur.setBluff(CalculDonneesProfilage::taux(profilJoueur["bluff"].toDouble(),profilJoueur["nombreParties"].toDouble()));
            actionAttendueJoueur.setPassivite(CalculDonneesProfilage::taux(profilJoueur["passivite"].toDouble(),profilJoueur["nombreParties"].toDouble()));
        }
        else{
            actionAttendueJoueur.setAgressivite(20.0);
            actionAttendueJoueur.setRationalite(100.0);
            actionAttendueJoueur.setBluff(0.0);
            actionAttendueJoueur.setPassivite(20.0);
        }
    }
    else{
        QJsonObject actionPrecedente=calibrage["actionReelle"].toObject();
        actionAttendueJoueur.setAgressivite(actionPrecedente["agressivite"].toDouble());
        actionAttendueJoueur.setBluff(actionPrecedente["bluff"].toDouble());
        actionAttendueJoueur.setPassivite(actionPrecedente["rationalite"].toDouble());
        actionAttendueJoueur.setRationalite(actionPrecedente["rationalite"].toDouble());
    }
}

void ScenariosDeTests::calculerDistance(){
    //On récupère les distances entre chaque paramètre (agressivité, rationalité, bluff, passivité) :
    double distanceAgressivite=abs(actionAttendueJoueur.getAgressivite()-actionReelleJoueur->getAgressivite());
    double distanceRationalite=abs(actionAttendueJoueur.getRationalite()-actionReelleJoueur->getRationalite());
    double distancePassivite=abs(actionAttendueJoueur.getPassivite()-actionReelleJoueur->getPassivite());
    double distanceBluff=abs(actionAttendueJoueur.getBluff()-actionReelleJoueur->getBluff());

    //On calcule une distance moyenne:
    distance=(distanceAgressivite+distanceBluff+distancePassivite+distanceRationalite)/4;
}

void ScenariosDeTests::scenarioSuivant(QJsonArray calibrages){
    //On commence par regarder si on est dans les 125 premiers tests.
    QJsonObject dernierCalibrage=calibrages.at(calibrages.size()-1).toObject();
    if(calibrages.size()<25 || dernierCalibrage.size()<NOMBRE_ITERRATIONS_TESTS){
        if(numeroDuTestActuel<NOMBRE_ITERRATIONS_TESTS){
            numeroDuTestActuel++;
        }
        else{
            numeroCalibrage++;
            numeroDuTestActuel=0;
            if(calibrageActuelIA->getRationalite()==0){
                calibrageActuelIA->setRationalite(100);
                calibrageActuelIA->setAgressivite(calibrageActuelIA->getAgressivite()-25);
            }
            else{
                calibrageActuelIA->setRationalite(calibrageActuelIA->getRationalite()-25);
            }
        }
    }
    //Les 125 tests ont déjà été faits du coup, on regarde le calibrage pour lequel la distance est la plus mauvaise
    else{
        double distanceLaPlusGrande=0;
        int numeroLigneCalibrage=-1;

        //On récupère la distance moyenne la plus grande
        for(int i=0;i<calibrages.size();i++){
            if(calibrages.at(i).toObject()["distanceMoyenne"].toDouble()>distanceLaPlusGrande){
                distanceLaPlusGrande=calibrages[i].toObject()["distanceMoyenne"].toDouble();
                numeroLigneCalibrage=i;
            }
        }

        calibrageActuelIA->setAgressivite(calibrages[numeroLigneCalibrage].toObject()["agressivite"].toDouble());
        calibrageActuelIA->setRationalite(calibrages[numeroLigneCalibrage].toObject()["rationalite"].toDouble());
        numeroCalibrage=numeroLigneCalibrage;

    }

}


int ScenariosDeTests::anciennesChancesDeGain(QJsonObject calibrage){
    int numeroLigneLaPlusProche= -1;
    double chancesGainLesPlusProches=-1000;

    for(int i=0;i<calibrage.size();i++){
        if(chancesDeGain+VARIATION_AUTORISEE>calibrage["ChancesGain"].toDouble()
        && chancesDeGain-VARIATION_AUTORISEE<calibrage["ChancesGain"].toDouble()
        && abs(chancesDeGain-calibrage["ChancesGain"].toDouble() )<abs(chancesDeGain-chancesGainLesPlusProches)){

            numeroLigneLaPlusProche=i;
            chancesGainLesPlusProches=calibrage["ChancesGain"].toDouble() ;

        }
    }

    return numeroLigneLaPlusProche;
}
