/*========================================================================
Nom: Fenetre.cpp       Auteur: Manuel CHATAIGNER
Maj: 11/02/2015            Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Fenetre.
=========================================================================*/

#include "../../include/Interface/Fenetre.h"
#include "../../include/Interface/CarteGraphique.h"
#include "../../include/Interface/CartesDialog.h"
#include "../../include/Interface/ChoixOptionsDialog.h"
#include "../../include/Interface/Logger.h"
#include "../../include/Jeu/JoueurHumain.h"
#include "../../include/IA/IntelligenceArtificielleProfilage.h"
#include "../../include/IA/CalibrageIdeal.h"

#include <QString>
#include <QVBoxLayout>
#include <iostream>
#include <QScrollBar>
#include <QEventLoop>

using namespace std;

QPixmap *Fenetre::textureCartes = 0;

Fenetre::Fenetre() : QWidget()
{
    move(0,0);
    setWindowTitle(tr("Poker"));
    resize(1280,400);

    // Couleur de fond
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(20, 127, 20));
    setPalette(pal);

    Logger::creerInstance(this);

    //Récupération des options du jeu
    ChoixOptionsDialog fenetreOptions;
    Options options = fenetreOptions.getOptions();

    QString pseudoJoueur = options.pseudo;

    // Envoi du calibrage de l'IA
//    Profil calibrageIa;
//    calibrageIa.setAgressivite(options.agressiviteIA);
//    calibrageIa.setRationalite(options.rationaliteIA);


    nombreDeParties = options.nombreParties;

    jeu = new Jeu(2, 20, CAVE_JOUEURS);

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
    }
    else {
        j1 = new JoueurHumain(true, CAVE_JOUEURS, 0, this);
    }

    jeu->addJoueur(j1);

    IntelligenceArtificielleProfilage *ia = new IntelligenceArtificielleProfilage(false, CAVE_JOUEURS, 1);
    jeu->addJoueur(ia);

    // Envoi du pseudo du joueur
    ia->setPseudoJoueurProfile(pseudoJoueur.toStdString());
    //ia->setCalibrage(calibrageIa);


    // Layout principal
    QHBoxLayout *layout = new QHBoxLayout;


    // ////////////////////////////////////////////////////
    // Logs
    // ////////////////////////////////////////////////////

    QVBoxLayout *layoutLogs = new QVBoxLayout;

    QLabel *titreTableauDeBord = new QLabel("Tableau de bord :");
    titreTableauDeBord->setFixedSize(300, 50);
    titreTableauDeBord->setStyleSheet("QLabel {color : #89DF57; text-align:center; font-weight:bold;}");

    logs.setReadOnly(true);
    logs.setMaximumSize(300, 300);


    boutonLogs.setText("Afficher/Cacher");
    boutonLogs.setFixedWidth(300);

    layoutLogs->setAlignment(Qt::AlignTop);
    layoutLogs->setAlignment(Qt::AlignHCenter);
    layoutLogs->setSpacing(20);

    layoutLogs->addWidget(titreTableauDeBord);
    layoutLogs->addWidget(&logs);
    layoutLogs->addWidget(&boutonLogs);
    layoutLogs->addWidget(&resultatPartie);



    connect(&boutonLogs, SIGNAL(clicked()), this, SLOT(affichageLogs()));


    // ////////////////////////////////////////////////////
    // Cartes
    // ////////////////////////////////////////////////////

    QVBoxLayout *layoutJeu = new QVBoxLayout;

    // Chargement de l'image
    if (!textureCartes){
        textureCartes = new QPixmap(QString::fromStdString(TEXTURE_CARTES));
    }

    QHBoxLayout *layoutAdversaire =new QHBoxLayout;

    //Sous-layout IA
    actionEffectueeIA.setReadOnly(true);
    actionEffectueeIA.setFixedWidth(80);
    actionEffectueeIA.setPlaceholderText("Action");

    layoutAdversaire->addLayout(&layoutMainAdverse);
    layoutAdversaire->addWidget(&actionEffectueeIA);

    layoutJeu->setSpacing(150);
    layoutJeu->setAlignment(Qt::AlignTop);

    layoutJeu->addLayout(layoutAdversaire);
    layoutJeu->addLayout(&layoutCartesCommunes);
    layoutJeu->addLayout(&layoutMain);


    // ////////////////////////////////////////////////////
    // Compteurs
    // ////////////////////////////////////////////////////

    caveIA.display(jeu->getJoueur(1)->getCave());
    pot.display(0);
    caveJoueur.display(jeu->getJoueur(0)->getCave());


    // ////////////////////////////////////////////////////
    // Boutons d'action
    // ////////////////////////////////////////////////////

    QHBoxLayout *layoutDemarrage = new QHBoxLayout;

    boutonDemarrage.setText("Démarrage partie");
    boutonDemarrage.setMaximumWidth(150);

    if (CALCUL_CALIBRAGE_IDEAL) {
        connect(&boutonDemarrage, SIGNAL(clicked()), this, SLOT(demarrageCalibrageIdeal()));
    }
    else {
        connect(&boutonDemarrage, SIGNAL(clicked()), this, SLOT(demarragePartie()));
    }

    boutonChoixCartes.setChecked(false);
    boutonChoixCartes.setText("Choix des cartes");
    boutonChoixCartes.setObjectName(QString("Checkbox"));
    boutonChoixCartes.setStyleSheet("QWidget#Checkbox { background-color: rgb(255, 255, 255); border-style: solid; border-color: black; border-width: 1px; padding : 3px; }");

    layoutDemarrage->setAlignment(Qt::AlignLeft);
    layoutDemarrage->setSpacing(10);
    layoutDemarrage->addWidget(&boutonDemarrage);
    layoutDemarrage->addWidget(&boutonChoixCartes);
    layoutDemarrage->addWidget(&boutonChoixCartes);


    QVBoxLayout *layoutBoutons = new QVBoxLayout;

    layoutBoutons->setSpacing(10);
    layoutBoutons->setAlignment(Qt::AlignTop);

    boutons[CHECKER].setText("Checker");
    boutons[MISER].setText("Miser");
    boutons[SUIVRE].setText("Suivre");
    boutons[RELANCER].setText("Relancer");
    boutons[SE_COUCHER].setText("Se coucher");
    boutons[TAPIS].setText("Tapis");

    layoutBoutons->addWidget(&boutons[CHECKER]);
    layoutBoutons->addWidget(&boutons[MISER]);
    layoutBoutons->addWidget(&boutons[SUIVRE]);
    layoutBoutons->addWidget(&boutons[RELANCER]);
    layoutBoutons->addWidget(&boutons[SE_COUCHER]);
    layoutBoutons->addWidget(&boutons[TAPIS]);

    connect(&boutons[CHECKER], SIGNAL(clicked()), this, SLOT(checker()));
    connect(&boutons[MISER], SIGNAL(clicked()), this, SLOT(miser()));
    connect(&boutons[SUIVRE], SIGNAL(clicked()), this, SLOT(suivre()));
    connect(&boutons[RELANCER], SIGNAL(clicked()), this, SLOT(relancer()));
    connect(&boutons[SE_COUCHER], SIGNAL(clicked()), this, SLOT(seCoucher()));
    connect(&boutons[TAPIS], SIGNAL(clicked()), this, SLOT(tapis()));

    activeBoutons(false);


    // ////////////////////////////////////////////////////
    // Fenetre
    // ////////////////////////////////////////////////////

    QHBoxLayout *layoutJoueur = new QHBoxLayout;

    valeurMise.setRange(1, 10000);

    layoutJoueur->setAlignment(Qt::AlignRight);
    layoutJoueur->setSpacing(50);

    layoutJoueur->addWidget(&caveJoueur);
    layoutJoueur->addWidget(&valeurMise);
    layoutJoueur->addLayout(layoutBoutons);


    QVBoxLayout *layoutOptions = new QVBoxLayout;

    layoutOptions->setSpacing(100);
    layoutOptions->setAlignment(Qt::AlignHCenter);

    layoutOptions->addLayout(layoutDemarrage);
    layoutOptions->addWidget(&caveIA);
    layoutOptions->addWidget(&pot);
    layoutOptions->addLayout(layoutJoueur);


    // Layout principal
    layout->setSpacing(100);
    layout->addLayout(layoutLogs);
    layout->addLayout(layoutJeu);
    layout->addLayout(layoutOptions);

    setLayout(layout);
}

Fenetre::~Fenetre()
{
    Logger::supprimerInstance();
}

void Fenetre::affichageLogs()
{
    bool visible = !logs.isHidden();

    logs.setHidden(visible);
}

void Fenetre::ajoutLogs(QString contenu)
{
    logs.setText(logs.toPlainText() + contenu + "\n");

    // Scroll automatique vers la fin
    QScrollBar *sb=logs.verticalScrollBar();
    sb->setValue(sb->maximum());
}

void Fenetre::demarragePartie()
{
    for (int i = 0; i < NOMBRE_CALIBRAGES; i++) {

        if (!jeu->getJoueur(0)->estHumain()) {
            IntelligenceArtificielle *iaProfilee = static_cast<IntelligenceArtificielle*>(jeu->getJoueur(0));
            iaProfilee->nouveauCalibrage();

            QString pseudo = QString::number(iaProfilee->getCalibrage()->getAgressivite()) + "_" + QString::number(iaProfilee->getCalibrage()->getRationalite());

            IntelligenceArtificielleProfilage *iaQuiProfile = static_cast<IntelligenceArtificielleProfilage*>(jeu->getJoueur(1));
            iaQuiProfile->setPseudoJoueurProfile(pseudo.toStdString());
        }

        for(int j=0;j<NOMBRE_PARTIES;j++){
            std::cout << "Partie " << j+1 << std::endl;

            actionEffectueeIA.clear();
            resultatPartie.setText("");
            boutonDemarrage.hide();
            layoutCartesCommunes.vider();


            Logger::getInstance()->ajoutLogs("Distribution des cartes");

            // Sélection des cartes par l'utilisateur

            if (boutonChoixCartes.isChecked()) {
                CartesDialog fenetreCartes(this);
                std::vector<int> ids = fenetreCartes.choixCartes();

                if (!ids.empty()) {
                    jeu->affectationCarte(ids);
                }
            }

            // Réinitialisation jeu
            jeu->setPot(0);
            jeu->getJoueur(0)->setCave(CAVE_JOUEURS);
            jeu->getJoueur(1)->setCave(CAVE_JOUEURS);

            jeu->distributionMain();

            pot.display(jeu->getPot());
            caveJoueur.display(jeu->getJoueur(0)->getCave());
            caveIA.display(jeu->getJoueur(1)->getCave());


            // Affichage de la main adverse dans les logs

            std::vector<Carte> jeuAdverse = jeu->getJoueur(1)->getMain();

            Logger::getInstance()->ajoutLogs("Jeu adverse : ");
            for (int i = 0; i < jeuAdverse.size(); i++) {
                Logger::getInstance()->ajoutLogs("-> " + QString::number(jeuAdverse.at(i).getRang())
                          + " " + CarteGraphique::couleurs[jeuAdverse.at(i).getCouleur()]);
            }


            // Main du joueur

            layoutMain.vider();
            layoutMain.ajoutCartes(jeu->getJoueur(0)->getMain());

            // Cartes communes
            layoutCartesCommunes.vider();

            for (int i = 0; i < 5; i++) {
                CarteGraphique *dos = new CarteGraphique(0, 0);
                layoutCartesCommunes.addWidget(dos);
            }

            // Main adverse

            CarteGraphique *dos = new CarteGraphique(0, 0);
            CarteGraphique *dos2 = new CarteGraphique(0, 0);

            layoutMainAdverse.vider();
            layoutMainAdverse.addWidget(dos);
            layoutMainAdverse.addWidget(dos2);


            valeurMise.setMinimum(jeu->getBlind());

            pot.display(jeu->getPot());
            caveJoueur.display(jeu->getJoueur(1)->getCave());
            caveIA.display(jeu->getJoueur(1)->getCave());

            jeu->lancer();
            afficheTable();
            partieTermine();
        }
    }
}

void Fenetre::demarrageCalibrageIdeal()
{
    IntelligenceArtificielle *iaQuiProfile = static_cast<IntelligenceArtificielle*>(jeu->getJoueur(1));
    IntelligenceArtificielle *iaProfilee = static_cast<IntelligenceArtificielle*>(jeu->getJoueur(0));

    CalibrageIdeal c(jeu, iaQuiProfile->getCalibrage(), iaProfilee->getCalibrage(), NOMBRE_PARTIES_CALIBRAGE_IDEAL);
    c.lancerParties();
}

void Fenetre::afficheTable()
{
    std::vector<Carte> table = jeu->getTable();

    layoutCartesCommunes.vider();
    layoutCartesCommunes.ajoutCartes(table);

    // On complète la table avec des dos de carte
    for (int i = 0; i < 5 - table.size(); i++) {
        CarteGraphique *dos = new CarteGraphique(0, 0);
        layoutCartesCommunes.addWidget(dos);
    }
}

void Fenetre::activeBoutons(bool active)
{
    // On désactive tous les boutons en fin de tour
    if (!active) {
        for (int i = 0; i < NB_BOUTONS; i++) {
            boutons[i].setEnabled(false);
        }
    }
    else {      // Sinon on vérifie les actions possibles pour les activer

        for (int i = 0; i < NB_BOUTONS; i++) {
            switch (i) {

                case CHECKER:
                    boutons[i].setEnabled(jeu->peutChecker(0));
                    break;

                case MISER:
                    boutons[i].setEnabled(jeu->peutMiser(0, 1));
                    break;

                case SUIVRE:
                    boutons[i].setEnabled(jeu->peutSuivre(0));
                    break;

                case RELANCER:
                    boutons[i].setEnabled(jeu->peutRelancer(0, 2 * jeu->getMiseCourante()));
                    break;

                default:
                    boutons[i].setEnabled(true);
                    break;
            }
        }
    }

    valeurMise.setMinimum(2 * jeu->getMiseCourante());
    valeurMise.setMaximum(jeu->getJoueur(0)->getCave());
}

Action Fenetre::getAction()
{
    // Mise à jour des informations
    afficheTable();
    pot.display(jeu->getPot());
    caveJoueur.display(jeu->getJoueur(0)->getCave());
    caveIA.display(jeu->getJoueur(1)->getCave());


    activeBoutons(true);

    // Attente d'une action de l'utilisateur
    QEventLoop loop;
    connect(this, SIGNAL(actionChoisie()), &loop, SLOT(quit()));
    loop.exec();

    activeBoutons(false);

    return Action(actionUtilisateur, valeurMise.value());
}

void Fenetre::checker()
{
    actionUtilisateur = ACTION::CHECKER;

    Logger::getInstance()->ajoutLogs("Joueur 1 check");

    emit actionChoisie();
}

void Fenetre::miser()
{
    actionUtilisateur = ACTION::MISER;

    Logger::getInstance()->ajoutLogs("Joueur 1 mise " + QString::number(valeurMise.value()));

    emit actionChoisie();
}

void Fenetre::suivre()
{
    actionUtilisateur = ACTION::SUIVRE;

    Logger::getInstance()->ajoutLogs("Joueur 1 suit");

    emit actionChoisie();
}

void Fenetre::relancer()
{
    actionUtilisateur = ACTION::RELANCER;

    Logger::getInstance()->ajoutLogs("Joueur 1 relance " + QString::number(valeurMise.value()));

    emit actionChoisie();
}

void Fenetre::seCoucher()
{
    actionUtilisateur = ACTION::SE_COUCHER;

    Logger::getInstance()->ajoutLogs("Joueur 1 se couche");

    emit actionChoisie();
}

void Fenetre::tapis()
{
    actionUtilisateur = ACTION::TAPIS;

    Logger::getInstance()->ajoutLogs("Joueur 1 fait tapis");

    emit actionChoisie();
}

void Fenetre::partieTermine()
{
    Logger::getInstance()->ajoutLogs("Partie terminée !");
    activeBoutons(false);

    layoutMainAdverse.vider();
    layoutMainAdverse.ajoutCartes(jeu->getJoueur(1)->getMain());


    int gagne = jeu->getResultatPartie();

    if(gagne==GAGNE){
        resultatPartie.setStyleSheet("QLabel {color : #89DF57; font-size : 40px; text-align:center; padding-left:80px; font-weight:bold;}");
        resultatPartie.setText("Gagné !");
    }
    else if(gagne==EGALITE){
        resultatPartie.setStyleSheet("QLabel {color : #23BDFE; font-size : 40px; text-align:center; padding-left:80px; font-weight:bold;}");
        resultatPartie.setText("Egalité");
    }
    else{
        resultatPartie.setStyleSheet("QLabel {color : #FE0000; font-size : 40px; text-align:center; padding-left:70px; font-weight:bold;}");
        resultatPartie.setText("Perdu !");
    }

    jeu->nouvelleMain();

    boutonDemarrage.setText("Rejouer");
    boutonDemarrage.setHidden(false);
}
