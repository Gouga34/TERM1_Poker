/*========================================================================
Nom: Fenetre.cpp       Auteur: Manuel CHATAIGNER
Maj: 11/02/2015            Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Fenetre.
=========================================================================*/

#include "../../include/Interface/Fenetre.h"
#include "../../include/Interface/CarteGraphique.h"
#include "../../include/Interface/ChoixOptionsDialog.h"
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
    Options options = fenetreOptions.getOptions();

    QString pseudoJoueur = options.pseudo;
    nombreDeCalibrages = options.nombreCalibrages;
    nombreDeParties = options.nombreParties;

    // Création du jeu
    jeu = new Jeu(2, 20);

    Joueur *j1;
    if (options.joueurIA) {
        j1 = new IntelligenceArtificielle(true, CAVE_JOUEURS, 0);
        IntelligenceArtificielle *ia = static_cast<IntelligenceArtificielle*>(j1);

        if (options.pseudo.isEmpty()) {
            Profil calibrage = *(ia->getCalibrage());
            pseudoJoueur = QString::number(calibrage.getAgressivite()) + "_" + QString::number(calibrage.getRationalite());
        }
        else {
            Profil calibrage;
            calibrage.setAgressivite(options.pseudo.split("_")[0].toDouble());
            calibrage.setRationalite(options.pseudo.split("_")[1].toDouble());
            ia->setCalibrage(calibrage);
        }

        contenu = new ContenuFenetreIA(jeu);
    }
    else {
        contenu = new ContenuFenetreHumain(jeu, this);
        j1 = new JoueurHumain(true, CAVE_JOUEURS, 0, static_cast<ContenuFenetreHumain*>(contenu));
    }

    jeu->addJoueur(j1);

    IntelligenceArtificielleProfilage *ia = new IntelligenceArtificielleProfilage(false, CAVE_JOUEURS, 1);
    jeu->addJoueur(ia);

    // Envoi du pseudo du joueur
    ia->setPseudoJoueurProfile(pseudoJoueur.toStdString());


    // Layout principal
    QVBoxLayout *layout = new QVBoxLayout;
    boutonDemarrage.setText("Démarrer");
    boutonDemarrage.setFixedSize(boutonDemarrage.sizeHint());

    if (CALCUL_CALIBRAGE_IDEAL) {
        connect(&boutonDemarrage, SIGNAL(clicked()), this, SLOT(demarrageCalibrageIdeal()));
    }
    else {
        connect(&boutonDemarrage, SIGNAL(clicked()), this, SLOT(demarragePartie()));
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

    int nbCalibrages = NOMBRE_CALIBRAGES;
    int nbParties = NOMBRE_PARTIES;

    if (jeu->getJoueur(0)->estHumain()) {
        nbCalibrages = 1;
        nbParties = 1;
    }

    IntelligenceArtificielleProfilage *iaQuiProfile = static_cast<IntelligenceArtificielleProfilage*>(jeu->getJoueur(1));

    for (int i = 0; i < nbCalibrages; i++) {

        if (!jeu->getJoueur(0)->estHumain()) {
            IntelligenceArtificielle *iaProfilee = static_cast<IntelligenceArtificielle*>(jeu->getJoueur(0));
            iaProfilee->nouveauCalibrage();

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

    CalibrageIdeal c(jeu, iaQuiProfile->getCalibrage(), iaProfilee->getCalibrage(), NOMBRE_PARTIES_CALIBRAGE_IDEAL);
    c.lancerParties();
}

