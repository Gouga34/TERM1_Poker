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


Profilage::Profilage(std::string joueur)
    : nomJoueur(joueur), typeJoueur{0}, partieGagnee(false),
      profil{{false, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}
{

}

Profilage::~Profilage()
{

}

void Profilage::charger()
{
    QString nomFichier = QString::fromStdString(nomJoueur) + ".json";
    QFile fichier("../Application/ressources/Profilage/ProfilageStatique/" + nomFichier);
    if (!fichier.open(QIODevice::ReadOnly)) {
        std::cerr << "Le fichier du pseudo " << nomJoueur << " n'existe pas encore." << std::endl;
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

            profil[i].couche = etape["couche"].toBool();

            profil[i].probaGainAdversaire = etape["probaGainAdversaire"].toDouble();
            profil[i].pot = etape["pot"].toDouble();

            profil[i].tauxAgressivite = etape["agressivité"].toDouble();
            profil[i].tauxRationnalite = etape["rationnalite"].toDouble();
            profil[i].tauxBluff = etape["bluff"].toDouble();
            profil[i].tauxPassivite = etape["passivite"].toDouble();

            profil[i].tauxSuivis = etape["suivis"].toDouble();
            profil[i].tauxChecks = etape["checks"].toDouble();
            profil[i].tauxMises = etape["mises"].toDouble();

            profil[i].misePlusHaute = etape["misePlusHaute"].toDouble();
            profil[i].miseTotaleJoueur = etape["miseTotaleJoueur"].toDouble();
            profil[i].miseTotaleIA = etape["miseTotaleIA"].toDouble();
        }
    }

    fichier.close();
}

void Profilage::sauvegarder() const
{
   QString nomFichier = QString::fromStdString(nomJoueur) + ".json";
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
       out << QString::fromStdString(nomJoueur) << endl;

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

        etape["couche"] = profil[i].couche;

        etape["probaGainAdversaire"] = profil[i].probaGainAdversaire;
        etape["pot"] = profil[i].pot;

        etape["agressivité"] = profil[i].tauxAgressivite;
        etape["rationnalite"] = profil[i].tauxRationnalite;
        etape["bluff"] = profil[i].tauxBluff;
        etape["passivite"] = profil[i].tauxPassivite;

        etape["suivis"] = profil[i].tauxSuivis;
        etape["checks"] = profil[i].tauxChecks;
        etape["mises"] = profil[i].tauxMises;

        etape["misePlusHaute"] = profil[i].misePlusHaute;
        etape["miseTotaleJoueur"] = profil[i].miseTotaleJoueur;
        etape["miseTotaleIA"] = profil[i].miseTotaleIA;

        QString nomEtape = QString::fromStdString(nomEtapes[i]);
        partie[nomEtape] = etape;
   }

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

   fichier.close();
}
