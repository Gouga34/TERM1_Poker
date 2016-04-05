/*========================================================================
Nom: Fenetre.cpp       Auteur: Manuel CHATAIGNER
Maj: 11/02/2015            Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Fenetre.
=========================================================================*/

#include "../../include/Interface/Fenetre.h"
#include "../../include/Interface/CarteGraphique.h"
#include "../../include/Interface/ContenuFenetreHumain.h"
#include "../../include/Jeu/HumainPlayer.h"
#include "../../include/AI/ArtificialIntelligenceProfiling.h"
#include "../../include/AI/IdealCalibration.h"
#include "../../include/Interface/ContenuFenetreIA.h"

#include <QVBoxLayout>
#include <iostream>

using namespace std;

Fenetre::Fenetre() : QWidget(), jeu(0), contenu(0)
{
    move(0,0);
    setWindowTitle(tr("Poker"));
    resize(1280,400);

    /** Fenêtre **/

    // Layout principal
    layout = new QVBoxLayout;
    boutonDemarrage.setFixedSize(100, 30);

    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(&boutonDemarrage);

    initialiser();

    setLayout(layout);
}

Fenetre::~Fenetre()
{
    if (jeu) {
        delete jeu;
    }

    if (contenu) {
        delete contenu;
    }
}

void Fenetre::initialiser()
{
    if (jeu) {
        delete jeu;
        jeu = 0;
    }

    layout->removeWidget(contenu);

    if (contenu) {
        delete contenu;
        contenu = 0;
    }


    hide();

    //Récupération des options du jeu
    ChoixOptionsDialog fenetreOptions;

    // Création du jeu
    jeu = new Jeu(2, 20, fenetreOptions.getOptions());

    /** Joueur 1 **/

    Joueur *j1;
    QString pseudoJoueur = "inconnu";

    if (jeu->getOptions().joueurIA) {
        j1 = new ai::ArtificialIntelligence(true, CAVE_JOUEURS, 0);
        ai::ArtificialIntelligence *ia = static_cast<ai::ArtificialIntelligence*>(j1);

        // Si on lance une recherche de calibrage optimal ou si on a choisi le calibrage
        if (!jeu->getOptions().profilage || jeu->getOptions().calibrageIaProfileeFixe) {
            ia->setCalibration(jeu->getOptions().iaProfilee);
        }

        profiling::Profile calibrage = *(ia->getCalibration());
        pseudoJoueur = QString::number(calibrage.getAggressiveness()) + "_" + QString::number(calibrage.getRationality());

        contenu = new ContenuFenetreIA(jeu, this);
    }
    else {
        if (!jeu->getOptions().pseudo.isEmpty()) {
            pseudoJoueur = jeu->getOptions().pseudo;
        }

        contenu = new ContenuFenetreHumain(jeu, this);
        j1 = new HumanPlayer(true, CAVE_JOUEURS, 0, static_cast<ContenuFenetreHumain*>(contenu));
    }

    jeu->addJoueur(j1);


    /** IA qui profile **/

    ai::ArtificialIntelligenceProfiling *ia = new ai::ArtificialIntelligenceProfiling(false, CAVE_JOUEURS, 1);

    if (jeu->getOptions().calibrageIaQuiProfileFixe) {
        ia->setCalibration(jeu->getOptions().iaQuiProfile);
    }

    jeu->addJoueur(ia);

    // Envoi du pseudo du joueur
    ia->setProfiledPlayerNickname(pseudoJoueur.toStdString());


    /** Bouton démarrer **/

    disconnect(&boutonDemarrage, SIGNAL(clicked()), this, SLOT(initialiser()));
    if (jeu->getOptions().profilage) {
        connect(&boutonDemarrage, SIGNAL(clicked()), this, SLOT(demarragePartie()));
    }
    else {
        connect(&boutonDemarrage, SIGNAL(clicked()), this, SLOT(demarrageCalibrageIdeal()));
    }


    boutonDemarrage.setText("Démarrer");
    layout->addWidget(contenu);
    show();
}

void Fenetre::demarragePartie()
{
    boutonDemarrage.hide();

    int nbCalibrages = jeu->getOptions().nombreCalibrages;
    int nbParties = jeu->getOptions().nombreParties;

    if (jeu->getJoueur(0)->isHumain()) {
        nbCalibrages = 1;
        nbParties = 1;
    }

    ai::ArtificialIntelligenceProfiling *iaQuiProfile = static_cast<ai::ArtificialIntelligenceProfiling*>(jeu->getJoueur(1));

    for (int i = 0; i < nbCalibrages; i++) {

        if (!jeu->getJoueur(0)->isHumain()) {
            ai::ArtificialIntelligence *iaProfilee = static_cast<ai::ArtificialIntelligence*>(jeu->getJoueur(0));

            if (!jeu->getOptions().calibrageIaProfileeFixe) {
                iaProfilee->changeRandomlyCalibration();
            }

            std::cout<<"Calibrage IA profilée : agressivité: "<<iaProfilee->getCalibration()->getAggressiveness()
                    <<" rationalité: "<<iaProfilee->getCalibration()->getRationality()<<std::endl;

            QString pseudo = QString::number(iaProfilee->getCalibration()->getAggressiveness()) + "_" + QString::number(iaProfilee->getCalibration()->getRationality());

            iaQuiProfile->setProfiledPlayerNickname(pseudo.toStdString());
        }

        for(int j=0;j<nbParties;j++){
            std::cout << "Partie " << j+1 << std::endl;

            jeu->nouvellePartie();

            if (jeu->getJoueur(0)->isHumain()) {
                ContenuFenetreHumain *c = static_cast<ContenuFenetreHumain*>(contenu);
                c->debutPartie();
            }

            jeu->lancerPartie();

            contenu->actualiser();

            jeu->nouvelleMain();
        }

        iaQuiProfile->writeEarningsAnalysis();
    }

    disconnect(&boutonDemarrage, SIGNAL(clicked()), this, SLOT(demarragePartie()));
    connect(&boutonDemarrage, SIGNAL(clicked()), this, SLOT(initialiser()));
    boutonDemarrage.setText("Réinitialiser");
    boutonDemarrage.show();
}

void Fenetre::demarrageCalibrageIdeal()
{
    ai::ArtificialIntelligence *iaQuiProfile = static_cast<ai::ArtificialIntelligence*>(jeu->getJoueur(1));
    ai::ArtificialIntelligence *iaProfilee = static_cast<ai::ArtificialIntelligence*>(jeu->getJoueur(0));

    ai::IdealCalibration c(jeu, iaQuiProfile->getCalibration(), iaProfilee->getCalibration(), jeu->getOptions().nombreParties);
    c.launchParts();
}

