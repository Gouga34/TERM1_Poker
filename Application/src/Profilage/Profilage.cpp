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
#include <QStringList>
#include <string>

Profilage::Profilage(Profil *calibrageIA, Profil *profil)
    : profilIA(calibrageIA), profilJoueur(profil)
{
    reinitialiser();

    nombreParties = 0;
    nbPartiesGagneesProfilage = 0;
    nbPartiesProfilage = 0;
    nbPartiesGagneesJeu = 0;
    nbPartiesJeu = 0;

    gainsProfilage = 0;
    gainsJeu = 0;

    phaseProfilageCourante.newPhase(0);
    phaseJeuCourante.newPhase(0);
}

Profilage::~Profilage()
{

}

void Profilage::reinitialiser()
{
    int etape = ETAPE_JEU::NB_ETAPES;

    etatPartie[etape].couche = false;
    etatPartie[etape].probaGainAdversaire = 0;

    etatPartie[etape].tauxAgressivite = 0;
    etatPartie[etape].tauxRationnalite = 0;
    etatPartie[etape].tauxBluff = 0;
    etatPartie[etape].tauxPassivite = 0;

    etatPartie[etape].tauxSuivis = 0;
    etatPartie[etape].tauxChecks = 0;
    etatPartie[etape].tauxMises = 0;

    etatPartie[etape].misePlusHaute = 0;
    etatPartie[etape].miseTotaleJoueur = 0;

    etatPartie[etape].nbTotalActions = 0;
}

void Profilage::sauvegarder()
{
   QString nomFichier = QString::fromStdString(profilJoueur->getPseudo()) + ".csv";
   QFile fichier(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE) + nomFichier);

   if(profilJoueur->getPseudo().compare("") != 0){
       if (!fichier.open(QIODevice::ReadWrite | QIODevice::Text)) {

           bool dejaPresent = false;

           //Vérif si le pseudo est déjà présent ds le fichier "pseudos.txt".
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

            //Si pas présent, ajout du pseudo dans le fichier
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

       QTextStream out(&fichier);
       QString ligne;

       //Si fichier vide, ajout du nombre de parties et du nombre de parties gagnées par l'IA qui profile.
        if(fichier.size()==0) {

            out<<"Gain IA qui profile,Jetons gagnes IA qui profile,Scenarios de tests en cours,Jeu agressif,Agressivite IA qui profile, Rationalite IA qui profile,Chances de gain de l'IA profilee,"<<
                 "Agressivite IA profilee,Rationalite IA profilee"/*,Bluff IA profilee,Passivite IA profilee*/<<",Nombre de checks,"<<
                 "Nombre de mises,Nombre de suivis,Total des mises de l'IA profilee,Mise la plus haute IA profilee,Joueur profile se couche"<<endl;

            // Ajout du contenu de fin
            out << endl
                << "Nombre de parties:,0" << endl
                << "Nombre de parties gagnees par l'IA qui profile pendant profilage :,0,sur,0" << endl
                << "Nombre de parties gagnees par l'IA qui profile pendant jeu :,0,sur,0" << endl
                << "Gains de l'IA qui profile pendant profilage :,0" << endl
                << "Gains de l'IA qui profile pendant jeu :,0" << endl;
        }

        // On va chercher les informations à la fin du fichier
        fichier.seek(0);
        ligne = fichier.readLine();

        // Tant qu'on est pas à la fin
        while (!ligne.isEmpty() && ligne != "\n") {
            ligne = fichier.readLine();
        }

        ligne = fichier.readLine();
        nombreParties = ligne.split(",").at(1).toInt();

        ligne = fichier.readLine();
        nbPartiesGagneesProfilage = ligne.split(",").at(1).toInt();
        nbPartiesProfilage = ligne.split(",").at(3).toInt();

        ligne = fichier.readLine();
        nbPartiesGagneesJeu = ligne.split(",").at(1).toInt();
        nbPartiesJeu = ligne.split(",").at(3).toInt();

        ligne = fichier.readLine();
        gainsProfilage = ligne.split(",").at(1).toInt();

        ligne = fichier.readLine();
        gainsJeu = ligne.split(",").at(1).toInt();


        // On se replace au niveau du total
        fichier.seek(0);
        ligne = fichier.readLine();

        // Tant qu'on est pas à la fin
        while (!ligne.isEmpty() && ligne != "\n") {
            ligne = fichier.readLine();
        }

        // On efface la fin
        fichier.seek(fichier.pos() - 1);
        fichier.resize(fichier.pos());

        //Écriture de la ligne supplémentaire
        int i = ETAPE_JEU::NB_ETAPES;
        if(partieGagneeIAQuiProfile==GAGNE){
            out<<1<<",";
        }
        else if(partieGagneeIAQuiProfile==EGALITE){
            out<<2<<",";
        }
        else{
            out<<0<<",";
        }

        out<<nbJetonsGagnesIAQuiProfile<<","<<scenarioDeTest<<","<<jeuAgressif<<","<<profilIA->getAgressivite()<<","<<profilIA->getRationalite()<<","<<
                etatPartie[i].probaGainAdversaire<<","<<etatPartie[i].tauxAgressivite<<","<<etatPartie[i].tauxRationnalite<<","<<
                /*etatPartie[i].tauxBluff<<","<<etatPartie[i].tauxPassivite<<","<<*/etatPartie[i].tauxChecks<<","<<
                etatPartie[i].tauxMises<<","<<etatPartie[i].tauxSuivis<<","<<etatPartie[i].miseTotaleJoueur<<","<<
                etatPartie[i].misePlusHaute<<","<<etatPartie[i].couche<<endl;

        // On écrit le total

        nombreParties++;
        if (scenarioDeTest) {
            nbPartiesProfilage++;

            if (partieGagneeIAQuiProfile == GAGNE) {
                nbPartiesGagneesProfilage++;
            }

            gainsProfilage += nbJetonsGagnesIAQuiProfile;
        }
        else {
            nbPartiesJeu++;

            if (partieGagneeIAQuiProfile == GAGNE) {
                nbPartiesGagneesJeu++;
            }

            gainsJeu += nbJetonsGagnesIAQuiProfile;
        }

        out << endl
            << "Nombre de parties:," << nombreParties << endl
            << "Nombre de parties gagnees par l'IA qui profile pendant profilage :," << nbPartiesGagneesProfilage << ",sur," << nbPartiesProfilage << endl
            << "Nombre de parties gagnees par l'IA qui profile pendant jeu :," << nbPartiesGagneesJeu << ",sur," << nbPartiesJeu << endl
            << "Gains de l'IA qui profile pendant profilage :," << gainsProfilage << endl
            << "Gains de l'IA qui profile pendant jeu :," << gainsJeu << endl;

        fichier.close();
    }
}

