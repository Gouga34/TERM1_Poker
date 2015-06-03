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
#include "../../include/Jeu/JoueurHumain.h"
#include "../../include/IA/IntelligenceArtificielleProfilage.h"
#include "../../include/IA/CalibrageIdeal.h"
#include "../../include/Interface/ContenuFenetreIA.h"

#include <QVBoxLayout>
#include <iostream>

using namespace std;

Fenetre::Fenetre() : QWidget()
{
    move(0,0);
    setWindowTitle(tr("Poker"));
    resize(1280,400);

    //Récupération des options du jeu
    ChoixOptionsDialog fenetreOptions;

    // Création du jeu
    jeu = new Jeu(2, 20, fenetreOptions.getOptions());

    /** Joueur 1 **/

    Joueur *j1;
    QString pseudoJoueur = "inconnu";

    if (jeu->getOptions().joueurIA) {
        j1 = new IntelligenceArtificielle(true, CAVE_JOUEURS, 0);
        IntelligenceArtificielle *ia = static_cast<IntelligenceArtificielle*>(j1);

        // Si on lance une recherche de calibrage optimal ou si on a choisi le calibrage
        if (!jeu->getOptions().profilage || jeu->getOptions().calibrageIaProfileeFixe) {
            ia->setCalibrage(jeu->getOptions().iaProfilee);
        }

        Profil calibrage = *(ia->getCalibrage());
        pseudoJoueur = QString::number(calibrage.getAgressivite()) + "_" + QString::number(calibrage.getRationalite());

        contenu = new ContenuFenetreIA(jeu);
    }
    else {
        if (!jeu->getOptions().pseudo.isEmpty()) {
            pseudoJoueur = jeu->getOptions().pseudo;
        }

        contenu = new ContenuFenetreHumain(jeu, this);
        j1 = new JoueurHumain(true, CAVE_JOUEURS, 0, static_cast<ContenuFenetreHumain*>(contenu));
    }

    jeu->addJoueur(j1);


    /** IA qui profile **/

    IntelligenceArtificielleProfilage *ia = new IntelligenceArtificielleProfilage(false, CAVE_JOUEURS, 1);

    if (jeu->getOptions().calibrageIaQuiProfileFixe) {
        ia->setCalibrage(jeu->getOptions().iaQuiProfile);
    }

    jeu->addJoueur(ia);

    // Envoi du pseudo du joueur
    ia->setPseudoJoueurProfile(pseudoJoueur.toStdString());


    /** Fenêtre **/

    // Layout principal
    QVBoxLayout *layout = new QVBoxLayout;
    boutonDemarrage.setText("Démarrer");
    boutonDemarrage.setFixedSize(boutonDemarrage.sizeHint());

    if (jeu->getOptions().profilage) {
        connect(&boutonDemarrage, SIGNAL(clicked()), this, SLOT(demarragePartie()));
    }
    else {
        connect(&boutonDemarrage, SIGNAL(clicked()), this, SLOT(demarrageCalibrageIdeal()));
    }

    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(&boutonDemarrage);
    layout->addWidget(contenu);

    setLayout(layout);
}

Fenetre::~Fenetre()
{
    delete jeu;
}

void Fenetre::demarragePartie()
{
    boutonDemarrage.hide();

    int nbCalibrages = jeu->getOptions().nombreCalibrages;
    int nbParties = jeu->getOptions().nombreParties;

    if (jeu->getJoueur(0)->estHumain()) {
        nbCalibrages = 1;
        nbParties = 1;
    }

    IntelligenceArtificielleProfilage *iaQuiProfile = static_cast<IntelligenceArtificielleProfilage*>(jeu->getJoueur(1));

    for (int i = 0; i < nbCalibrages; i++) {

        if (!jeu->getJoueur(0)->estHumain()) {
            IntelligenceArtificielle *iaProfilee = static_cast<IntelligenceArtificielle*>(jeu->getJoueur(0));

            if (!jeu->getOptions().calibrageIaProfileeFixe) {
                iaProfilee->nouveauCalibrage();
            }

            std::cout<<"Calibrage IA profilée : agressivité: "<<iaProfilee->getCalibrage()->getAgressivite()
                    <<" rationalité: "<<iaProfilee->getCalibrage()->getRationalite()<<std::endl;

            QString pseudo = QString::number(iaProfilee->getCalibrage()->getAgressivite()) + "_" + QString::number(iaProfilee->getCalibrage()->getRationalite());

            iaQuiProfile->setPseudoJoueurProfile(pseudo.toStdString());
        }

        for(int j=0;j<nbParties;j++){
            std::cout << "Partie " << j+1 << std::endl;

            jeu->nouvellePartie();

            if (jeu->getJoueur(0)->estHumain()) {
                ContenuFenetreHumain *c = static_cast<ContenuFenetreHumain*>(contenu);
                c->debutPartie();
            }

            jeu->lancerPartie();

            contenu->actualiser();

            jeu->nouvelleMain();
        }

        iaQuiProfile->ecritureAnalyseDesGains();
    }
}

void Fenetre::demarrageCalibrageIdeal()
{
    IntelligenceArtificielle *iaQuiProfile = static_cast<IntelligenceArtificielle*>(jeu->getJoueur(1));
    IntelligenceArtificielle *iaProfilee = static_cast<IntelligenceArtificielle*>(jeu->getJoueur(0));

    CalibrageIdeal c(jeu, iaQuiProfile->getCalibrage(), iaProfilee->getCalibrage(), jeu->getOptions().nombreParties);
    c.lancerParties();
}

