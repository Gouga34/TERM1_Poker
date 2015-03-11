/*========================================================================
Nom: Profilage.cpp       Auteur: Manuel CHATAIGNER
Maj: 04/03/2015            Creation: 04/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Profilage.
=========================================================================*/

#include "../../include/Jeu/Profilage.h"
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <iostream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>


const QString Profilage::etapes[] = {"preflop", "flop", "turn", "river" };


Profilage::Profilage(std::string joueur)
    : nomJoueur(joueur)
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

    QJsonArray parties = json["parties"].toArray();

    for (int i = 0; i < parties.size(); i++) {
        QJsonObject partie = parties[i].toObject();

        partieGagnee = partie["gainIA"].toBool();

        for (int i = 0; i < NB_ETAPES; i++) {
            QString nomEtape = etapes[i];
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

            profil[i].misePlusHaute = etape["misePlusHaute"].toInt();
            profil[i].miseTotaleJoueur = etape["miseTotaleJoueur"].toInt();
            profil[i].miseTotaleIA = etape["miseTotaleIA"].toDouble();
        }
    }

    fichier.close();
}

void Profilage::sauvegarder() const
{
   QString nomFichier = QString::fromStdString(nomJoueur) + ".json";
   QFile fichier("../Application/ressources/Profilage/ProfilageStatique/" + nomFichier);

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

       // Création du fichier du joueur
       if (!fichier.open(QIODevice::ReadWrite)) {
           std::cerr << "Erreur lors de l'ouverture du fichier " << nomFichier.toStdString() << std::endl;
           return;
       }
   }

   QJsonObject partie;

   /* Ecriture de valeurs simples */

   partie["gainIA"] = partieGagnee;

   for (int i = 0; i < NB_ETAPES; i++) {
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

        QString nomEtape = etapes[i];
        partie[nomEtape] = etape;
   }

   /* Ajout de la partie courante à la liste de parties enregistrées */

   // Récupération du contenu existant
   QByteArray donneesJson = fichier.readAll();
   QJsonDocument doc(QJsonDocument::fromJson(donneesJson));
   QJsonObject json(doc.object());

   QJsonArray parties = json["parties"].toArray();
   parties.append(partie);


   // On modifie le QJsonObject
   json["parties"] = parties;
   // On modifie le QJsonDocument
   doc.setObject(json);
   // On écrit dans le fichier
   fichier.resize(0);
   fichier.write(doc.toJson());

   fichier.close();
}
