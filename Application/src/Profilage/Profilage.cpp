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


Profilage::Profilage(Profil *profil)
    : profilJoueur(profil), profilIA(), typeJoueur{0}, partieGagnee(false),
      etatPartie{{false, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}
{

}

Profilage::~Profilage()
{

}

void Profilage::charger()
{
    QString nomFichier = QString::fromStdString(profilJoueur->getPseudo()) + ".json";
    QFile fichier("../Application/ressources/Profilage/ProfilageStatique/" + nomFichier);
    if (!fichier.open(QIODevice::ReadOnly)) {
        std::cerr << "Le fichier du pseudo " << profilJoueur->getPseudo() << " n'existe pas encore." << std::endl;
        return;
    }

    QByteArray donneesJson = fichier.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(donneesJson));
    QJsonObject json(doc.object());

    // Type du joueur
    typeJoueur[PROFIL_JOUEUR::RA] = json["RA"].toInt();
    typeJoueur[PROFIL_JOUEUR::RP] = json["RP"].toInt();
    typeJoueur[PROFIL_JOUEUR::BA] = json["BA"].toInt();
    typeJoueur[PROFIL_JOUEUR::BP] = json["BP"].toInt();


    QJsonArray parties = json["parties"].toArray();

    for (int i = 0; i < parties.size(); i++) {
        QJsonObject partie = parties[i].toObject();

        partieGagnee = partie["gainIA"].toBool();

        for (int i = 0; i < ETAPE_JEU::NB_ETAPES; i++) {
            QString nomEtape = QString::fromStdString(nomEtapes[i]);
            QJsonObject etape = partie[nomEtape].toObject();

            etatPartie[i].couche = etape["couche"].toBool();

            etatPartie[i].probaGainAdversaire = etape["probaGainAdversaire"].toDouble();
            etatPartie[i].pot = etape["pot"].toDouble();

            etatPartie[i].tauxAgressivite = etape["agressivité"].toDouble();
            etatPartie[i].tauxRationnalite = etape["rationalite"].toDouble();
            etatPartie[i].tauxBluff = etape["bluff"].toDouble();
            etatPartie[i].tauxPassivite = etape["passivite"].toDouble();

            etatPartie[i].tauxSuivis = etape["suivis"].toDouble();
            etatPartie[i].tauxChecks = etape["checks"].toDouble();
            etatPartie[i].tauxMises = etape["mises"].toDouble();

            etatPartie[i].misePlusHaute = etape["misePlusHaute"].toDouble();
            etatPartie[i].miseTotaleJoueur = etape["miseTotaleJoueur"].toDouble();
            etatPartie[i].miseTotaleIA = etape["miseTotaleIA"].toDouble();
        }

        QJsonObject global = partie["global"].toObject();

        profilJoueur->setAgressivite(global["agressivité"].toDouble());
        profilJoueur->setBluff(global["bluff"].toDouble());
        profilJoueur->setRationalite(global["rationalite"].toDouble());
        profilJoueur->setPassivite(global["passivite"].toDouble());

        QJsonObject calibrageIA = partie["calibrageIA"].toObject();

        profilIA.setAgressivite(calibrageIA["agressivité"].toDouble());
        profilIA.setRationalite(calibrageIA["rationalite"].toDouble());
    }

    fichier.close();
}

void Profilage::sauvegarder()
{
   QString nomFichier = QString::fromStdString(profilJoueur->getPseudo()) + ".json";
   QFile fichier("../Application/ressources/Profilage/ProfilageStatique/" + nomFichier);

   QJsonDocument doc;
   QJsonObject json;
   QJsonArray parties;

   if (!fichier.open(QIODevice::ReadOnly)) {

       // Ouverture de la liste des pseudos
       QFile listePseudos("../Application/ressources/Profilage/ProfilageStatique/pseudos.txt");
       if (!listePseudos.open(QIODevice::Append | QIODevice::Text)) {
           std::cerr << "Erreur lors de l'ouverture du fichier des pseudos !" << std::endl;
           return;
       }

       QTextStream out(&listePseudos);
       out << QString::fromStdString(profilJoueur->getPseudo()) << endl;

       listePseudos.close();
   }
   else {
       // Récupération du contenu existant
       QByteArray donneesJson = fichier.readAll();
       doc = QJsonDocument::fromJson(donneesJson);
       json = doc.object();

       parties = json["parties"].toArray();

       fichier.close();
   }

   QJsonObject partie;

   /* Ecriture de valeurs simples */

   partie["gainIA"] = partieGagnee;

   for (int i = 0; i < ETAPE_JEU::NB_ETAPES; i++) {
        QJsonObject etape;

        etape["couche"] = etatPartie[i].couche;

        etape["probaGainAdversaire"] = etatPartie[i].probaGainAdversaire;
        etape["pot"] = etatPartie[i].pot;

        etape["agressivité"] = etatPartie[i].tauxAgressivite;
        etape["rationalite"] = etatPartie[i].tauxRationnalite;
        etape["bluff"] = etatPartie[i].tauxBluff;
        etape["passivite"] = etatPartie[i].tauxPassivite;

        etape["suivis"] = etatPartie[i].tauxSuivis;
        etape["checks"] = etatPartie[i].tauxChecks;
        etape["mises"] = etatPartie[i].tauxMises;

        etape["misePlusHaute"] = etatPartie[i].misePlusHaute;
        etape["miseTotaleJoueur"] = etatPartie[i].miseTotaleJoueur;
        etape["miseTotaleIA"] = etatPartie[i].miseTotaleIA;

        QString nomEtape = QString::fromStdString(nomEtapes[i]);
        partie[nomEtape] = etape;
   }

   QJsonObject global;

   global["agressivité"] = profilJoueur->getAgressivite();
   global["bluff"] = profilJoueur->getBluff();
   global["rationalite"] = profilJoueur->getRationalite();
   global["passivite"] = profilJoueur->getPassivite();

   partie["global"] = global;

   QJsonObject calibrageIA;

   calibrageIA["agressivité"] = profilIA.getAgressivite();
   calibrageIA["rationalite"] = profilIA.getRationalite();

   partie["calibrageIA"] = calibrageIA;

   // Ajout de la partie courante à la liste de parties enregistrées
   parties.append(partie);

   // Ecriture du type du joueur
   json["RA"] = typeJoueur[PROFIL_JOUEUR::RA];
   json["RP"] = typeJoueur[PROFIL_JOUEUR::RP];
   json["BA"] = typeJoueur[PROFIL_JOUEUR::BA];
   json["BP"] = typeJoueur[PROFIL_JOUEUR::BP];

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

   clear();

   fichier.close();
}

void Profilage::clear(){

    for(int etape=0; etape<ETAPE_JEU::NB_ETAPES;etape++){

        this->etatPartie[etape].couche = false;
        this->etatPartie[etape].probaGainAdversaire = 0;
        this->etatPartie[etape].pot = 0;

        this->etatPartie[etape].tauxAgressivite = 0;
        this->etatPartie[etape].tauxRationnalite = 0;
        this->etatPartie[etape].tauxBluff = 0;
        this->etatPartie[etape].tauxPassivite = 0;

        this->etatPartie[etape].tauxSuivis = 0;
        this->etatPartie[etape].tauxChecks = 0;
        this->etatPartie[etape].tauxMises = 0;

        this->etatPartie[etape].misePlusHaute = 0;
        this->etatPartie[etape].miseTotaleJoueur = 0;
        this->etatPartie[etape].miseTotaleIA = 0;
    }

    this->partieGagnee = false;
}

void Profilage::correction(ETAPE_JEU etape){

    if(this->etatPartie[etape].tauxPassivite < 0){
        this->etatPartie[etape].tauxPassivite = 0;
    }

    if(this->etatPartie[etape].tauxSuivis < 0){
        this->etatPartie[etape].tauxSuivis = 0;
    }

    if(this->etatPartie[etape].tauxChecks < 0){
        this->etatPartie[etape].tauxChecks = 0;
    }

    if(this->etatPartie[etape].tauxMises < 0){
        this->etatPartie[etape].tauxMises = 0;
    }

}
