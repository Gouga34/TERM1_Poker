/*========================================================================
Nom: Profilage.cpp       Auteur: Manuel CHATAIGNER
Maj: 04/03/2015            Creation: 04/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Profilage.
=========================================================================*/

#include "../../include/Profilage/Profilage.h"
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <iostream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>


Profilage::Profilage(Profil *calibrageIA, Profil *profil)
    : profilIA(calibrageIA), profilJoueur(profil),
      etatPartie{{false, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}
{

}

Profilage::~Profilage()
{

}

void Profilage::charger()
{
    QString nomFichier = QString::fromStdString(profilJoueur->getPseudo()) + ".json";
    QFile fichier(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE) + nomFichier);
    if (!fichier.open(QIODevice::ReadOnly)) {
        std::cerr << "Le fichier du pseudo " << profilJoueur->getPseudo() << " n'existe pas encore." << std::endl;
        return;
    }

    QByteArray donneesJson = fichier.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(donneesJson));
    QJsonObject json(doc.object());

    QJsonArray parties = json["parties"].toArray();

    for (int i = 0; i < parties.size(); i++) {
        QJsonObject partie = parties[i].toObject();

        for (int i = 0; i < ETAPE_JEU::NB_ETAPES; i++) {
            QString nomEtape = QString::fromStdString(nomEtapes[i]);
            QJsonObject etape = partie[nomEtape].toObject();

            etatPartie[i].couche = etape["couche"].toBool();

            etatPartie[i].probaGainAdversaire = etape["probaGainAdversaire"].toDouble();

            etatPartie[i].tauxAgressivite = etape["agressivité"].toDouble();
            etatPartie[i].tauxRationnalite = etape["rationalite"].toDouble();
            etatPartie[i].tauxBluff = etape["bluff"].toDouble();
            etatPartie[i].tauxPassivite = etape["passivite"].toDouble();

            etatPartie[i].tauxSuivis = etape["suivis"].toDouble();
            etatPartie[i].tauxChecks = etape["checks"].toDouble();
            etatPartie[i].tauxMises = etape["mises"].toDouble();

            etatPartie[i].misePlusHaute = etape["misePlusHaute"].toDouble();
            etatPartie[i].miseTotaleJoueur = etape["miseTotaleJoueur"].toDouble();
        }

        QJsonObject global = partie["global"].toObject();

        profilJoueur->setAgressivite(global["agressivité"].toDouble());
        profilJoueur->setBluff(global["bluff"].toDouble());
        profilJoueur->setRationalite(global["rationalite"].toDouble());
        profilJoueur->setPassivite(global["passivite"].toDouble());

        QJsonObject calibrageIA = partie["calibrageIA"].toObject();

        profilIA->setAgressivite(calibrageIA["agressivité"].toDouble());
        profilIA->setRationalite(calibrageIA["rationalite"].toDouble());
    }

    fichier.close();
}

void Profilage::sauvegarder()
{
   QString nomFichier = QString::fromStdString(profilJoueur->getPseudo()) + ".json";
   QFile fichier(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE) + nomFichier);

   QJsonDocument doc;
   QJsonObject json;
   QJsonArray parties;

   int nbPartiesGagnees=0;
   int nbParties=0;

   if(profilJoueur->getPseudo().compare("") != 0){
       if (!fichier.open(QIODevice::ReadOnly)) {

           bool dejaPresent = false;

           QFile pseudo(QString::fromStdString(FICHIER_PSEUDOS_PROFILAGE));
           if (!pseudo.open(QIODevice::ReadOnly | QIODevice::Text)) {
               std::cerr << "Erreur lors de l'ouverture du fichier des pseudos !" << std::endl;
               return;
           }

           QTextStream flux(&pseudo);

           QString ligne;
           while(!flux.atEnd()){
               ligne = flux.readLine();
               if(ligne.compare(QString::fromStdString(profilJoueur->getPseudo())) == 0 || ligne.compare(QString::fromStdString("")) == 0){
                   dejaPresent = true;
               }
           }

            pseudo.close();

           if(!dejaPresent){
               // Ouverture de la liste des pseudos
               QFile listePseudos(QString::fromStdString(FICHIER_PSEUDOS_PROFILAGE));
               if (!listePseudos.open(QIODevice::Append | QIODevice::Text)) {
                   std::cerr << "Erreur lors de l'ouverture du fichier des pseudos !" << std::endl;
                   return;
               }

               QTextStream out(&listePseudos);
               out << QString::fromStdString(profilJoueur->getPseudo()) << endl;

               listePseudos.close();
            }
       }
       else {
           // Récupération du contenu existant
           QByteArray donneesJson = fichier.readAll();
           doc = QJsonDocument::fromJson(donneesJson);
           json = doc.object();

           nbParties=json["nombreParties"].toInt();
           nbPartiesGagnees=json["partiesGagneesIAQuiProfile"].toInt();

           parties = json["parties"].toArray();

           fichier.close();
       }

   QJsonObject partie;

   /* Ecriture de valeurs simples */

   int i = ETAPE_JEU::NB_ETAPES;

    QJsonObject etape;

    etape["couche"] = etatPartie[i].couche;

    etape["probaGainAdversaire"] = etatPartie[i].probaGainAdversaire;

    etape["agressivité"] = etatPartie[i].tauxAgressivite;
    etape["rationalite"] = etatPartie[i].tauxRationnalite;
    etape["bluff"] = etatPartie[i].tauxBluff;
    etape["passivite"] = etatPartie[i].tauxPassivite;

    etape["suivis"] = etatPartie[i].tauxSuivis;
    etape["checks"] = etatPartie[i].tauxChecks;
    etape["mises"] = etatPartie[i].tauxMises;

    etape["misePlusHaute"] = etatPartie[i].misePlusHaute;
    etape["miseTotaleJoueur"] = etatPartie[i].miseTotaleJoueur;

    QString nomEtape = "global";
    //QString nomEtape = QString::fromStdString(nomEtapes[i]);
    partie[nomEtape] = etape;

    //Ajout du resultat de la partie
    partie["GainIAQuiProfile"]=partieGagneeIAQuiProfile;
    partie["ScenariosDeTestsEnCours"]=scenarioDeTest;

   QJsonObject calibrageIA;

   calibrageIA["agressivité"] = profilIA->getAgressivite();
   calibrageIA["rationalite"] = profilIA->getRationalite();

   partie["calibrageIA"] = calibrageIA;

   // Ajout de la partie courante à la liste de parties enregistrées
   parties.append(partie);


    json["nombreParties"]=nbParties+1;

    if(partieGagneeIAQuiProfile==GAGNE){
        json["partiesGagneesIAQuiProfile"]=nbPartiesGagnees+1;
    }

   // On modifie le QJsonObject
   json["parties"] = parties;
   // On modifie le QJsonDocument
   doc.setObject(json);

   // On écrit dans le fichier
    if (!fichier.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        std::cerr << "Erreur lors de l'ouverture du fichier " << nomFichier.toStdString() << std::endl;
        return;
    }

    fichier.write(doc.toJson());

    fichier.close();
    }
}

void Profilage::clear(){

    int etape = ETAPE_JEU::NB_ETAPES;

    this->etatPartie[etape].couche = false;
    this->etatPartie[etape].probaGainAdversaire = 0;

    this->etatPartie[etape].tauxAgressivite = 0;
    this->etatPartie[etape].tauxRationnalite = 0;
    this->etatPartie[etape].tauxBluff = 0;
    this->etatPartie[etape].tauxPassivite = 0;

    this->etatPartie[etape].tauxSuivis = 0;
    this->etatPartie[etape].tauxChecks = 0;
    this->etatPartie[etape].tauxMises = 0;

    this->etatPartie[etape].misePlusHaute = 0;
    this->etatPartie[etape].miseTotaleJoueur = 0;

    this->etatPartie[etape].nbTotalActions = 0;
}

int Profilage::getNombreParties(){
    QString nomFichier = QString::fromStdString(profilJoueur->getPseudo()) + ".json";
    QFile fichier(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE) + nomFichier);
    if (!fichier.open(QIODevice::ReadOnly)) {
        std::cerr << "Le fichier du pseudo " << profilJoueur->getPseudo() << " n'existe pas encore." << std::endl;
        return -1;
    }

    QByteArray donneesJson = fichier.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(donneesJson));
    QJsonObject json(doc.object());

    return json["nombreParties"].toInt();
}

int Profilage::getPartiesGagneesParIAQuiProfile(){
    QString nomFichier = QString::fromStdString(profilJoueur->getPseudo()) + ".json";
    QFile fichier(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE) + nomFichier);
    if (!fichier.open(QIODevice::ReadOnly)) {
        std::cerr << "Le fichier du pseudo " << profilJoueur->getPseudo() << " n'existe pas encore." << std::endl;
        return -1;
    }

    QByteArray donneesJson = fichier.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(donneesJson));
    QJsonObject json(doc.object());

    return json["partiesGagneesIAQuiProfile"].toInt();
}
