/*========================================================================
Nom: Profilage.cpp       Auteur: Manuel CHATAIGNER
Maj: 04/03/2015            Creation: 04/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Profilage.
=========================================================================*/

#include "../../include/Jeu/Profilage.h"
#include <QFile>
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

void Profilage::sauvegarder() const
{
   QString nomFichier = QString::fromStdString(nomJoueur) + ".json";
   QFile fichier(nomFichier);
   if (!fichier.open(QIODevice::ReadWrite)) {
       std::cerr << "Erreur lors de l'ouverture du fichier " << nomFichier.toStdString() << std::endl;
       return;
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
