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
#include <QVBoxLayout>

QPixmap *Fenetre::textureCartes = 0;

Fenetre::Fenetre(Jeu *j) : QWidget()
{
    setWindowTitle(tr("Poker"));
    resize(800,600);

    // Couleur de fond
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(20, 127, 20));
    setPalette(pal);

    this->jeu = j;

    for (int i = 0; i < NB_BOUTONS; i++) {
        activationBoutons[i] = true;
    }


    // Layout principal
    QHBoxLayout *layout = new QHBoxLayout;


    // ////////////////////////////////////////////////////
    // Logs
    // ////////////////////////////////////////////////////

    QVBoxLayout *layoutLogs = new QVBoxLayout;

    logs.setReadOnly(true);
    logs.setMaximumSize(300, 300);

    boutonLogs.setText("Afficher/Cacher");
    boutonLogs.setFixedWidth(300);

    layoutLogs->setAlignment(Qt::AlignTop);
    layoutLogs->setSpacing(20);

    layoutLogs->addWidget(&logs);
    layoutLogs->addWidget(&boutonLogs);

    connect(&boutonLogs, SIGNAL(clicked()), this, SLOT(affichageLogs()));


    // ////////////////////////////////////////////////////
    // Cartes
    // ////////////////////////////////////////////////////

    QVBoxLayout *layoutJeu = new QVBoxLayout;

    // Chargement de l'image
    if (!textureCartes){
        textureCartes = new QPixmap("../Application/ressources/Interface/deck.png");
    }

    layoutJeu->setSpacing(150);
    layoutJeu->setAlignment(Qt::AlignTop);

    layoutJeu->addLayout(&layoutMainAdverse);
    layoutJeu->addLayout(&layoutCartesCommunes);
    layoutJeu->addLayout(&layoutMain);


    // ////////////////////////////////////////////////////
    // Compteurs
    // ////////////////////////////////////////////////////

    pot.setMaximumSize(100, 50);
    pot.setSegmentStyle(QLCDNumber::Filled);
    pot.display(0);

    caveJoueur.setMaximumSize(100, 50);
    caveJoueur.setSegmentStyle(QLCDNumber::Filled);
    caveJoueur.display(jeu->getJoueur(0).getCave());

    caveIA.setMaximumSize(100, 50);
    caveIA.setSegmentStyle(QLCDNumber::Filled);
    caveIA.display(jeu->getJoueur(1).getCave());


    // ////////////////////////////////////////////////////
    // Boutons d'action
    // ////////////////////////////////////////////////////

    boutonDemarrage.setText("Démarrage partie");
    boutonDemarrage.setMaximumWidth(150);
    connect(&boutonDemarrage, SIGNAL(clicked()), this, SLOT(demarragePartie()));


    QVBoxLayout *layoutBoutons = new QVBoxLayout;

    layoutBoutons->setSpacing(10);
    layoutBoutons->setAlignment(Qt::AlignTop);

    boutons[CHECKER].setText("Checker");
    boutons[MISER].setText("Miser");
    boutons[SUIVRE].setText("Suivre");
    boutons[RELANCER].setText("Relancer");
    boutons[SE_COUCHER].setText("Se coucher");

    layoutBoutons->addWidget(&boutons[CHECKER]);
    layoutBoutons->addWidget(&boutons[MISER]);
    layoutBoutons->addWidget(&boutons[SUIVRE]);
    layoutBoutons->addWidget(&boutons[RELANCER]);
    layoutBoutons->addWidget(&boutons[SE_COUCHER]);

    connect(&boutons[CHECKER], SIGNAL(clicked()), this, SLOT(checker()));
    connect(&boutons[MISER], SIGNAL(clicked()), this, SLOT(miser()));
    connect(&boutons[SUIVRE], SIGNAL(clicked()), this, SLOT(suivre()));
    connect(&boutons[RELANCER], SIGNAL(clicked()), this, SLOT(relancer()));
    connect(&boutons[SE_COUCHER], SIGNAL(clicked()), this, SLOT(seCoucher()));

    activeBoutons(false);


    // ////////////////////////////////////////////////////
    // Fenetre
    // ////////////////////////////////////////////////////

    QHBoxLayout *layoutJoueur = new QHBoxLayout;

    valeurMise.setMaximum(10000);

    layoutJoueur->setAlignment(Qt::AlignRight);
    layoutJoueur->setSpacing(50);

    layoutJoueur->addWidget(&caveJoueur);
    layoutJoueur->addWidget(&valeurMise);
    layoutJoueur->addLayout(layoutBoutons);


    QVBoxLayout *layoutOptions = new QVBoxLayout;

    layoutOptions->setSpacing(150);
    layoutOptions->setAlignment(Qt::AlignHCenter);

    layoutOptions->addWidget(&boutonDemarrage);
    layoutOptions->addWidget(&caveIA);
    layoutOptions->addWidget(&pot);
    layoutOptions->addLayout(layoutJoueur);


    // Layout principal
    layout->setSpacing(100);
    layout->addLayout(layoutLogs);
    layout->addLayout(layoutJeu);
    layout->addLayout(layoutOptions);

    setLayout(layout);

    connect(this, SIGNAL(tourFini()), this, SLOT(prochainJoueur()));
}

Fenetre::~Fenetre()
{

}

void Fenetre::affichageLogs()
{
    bool visible = !logs.isHidden();

    logs.setHidden(visible);
}

void Fenetre::ajoutLogs(QString contenu)
{
    logs.setText(logs.toPlainText() + contenu + "\n");
}

void Fenetre::demarragePartie()
{
    boutonDemarrage.hide();
    layoutCartesCommunes.vider();

    ajoutLogs("Distribution des cartes");

    // Sélection des cartes par l'utilisateur

    CartesDialog fenetreCartes(this);
    std::vector<int> ids = fenetreCartes.choixCartes();

    /*if (!ids.empty()) {
        // Appeler la méthode de jeu
    }
    else {

    }*/

    jeu->distributionMain();
    
    pot.display(jeu->getPot());
    caveJoueur.display(jeu->getJoueur(0).getCave());
    caveIA.display(jeu->getJoueur(1).getCave());


    // Affichage de la main adverse dans les logs

    std::vector<Carte> jeuAdverse = jeu->getJoueur(1).getMain();

    ajoutLogs("Jeu adverse : ");
    for (int i = 0; i < jeuAdverse.size(); i++) {
        ajoutLogs("-> " + QString::number(jeuAdverse.at(i).getRang())
                  + " " + CarteGraphique::couleurs[jeuAdverse.at(i).getCouleur()]);
    }


    // Main du joueur

    layoutMain.vider();
    layoutMain.ajoutCartes(jeu->getJoueur(0).getMain());

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

    if (jeu->getJoueurCourant() == 0) {     // Joueur humain
        joueurCourant();
    }
    else {                                  // Intelligence artificielle
        jeuIA();
    }
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

    ajoutLogs("Ajout de cartes sur la table");
}

void Fenetre::activeBoutons(bool active)
{
    // Pour chaque bouton, soit on le désactive ou on regarde son état si activation

    for (int i = 0; i < NB_BOUTONS; i++) {
        bool enable = active ? activationBoutons[i] : active;
        boutons[i].setEnabled(enable);
    }
}

void Fenetre::joueurCourant()
{
    activeBoutons(true);
}

void Fenetre::jeuIA()
{
    static_cast<IntelligenceArtificielle>(jeu->getJoueur(jeu->getJoueurCourant())).jouer();

    switch (jeu->getAction()) {
        case TYPES::ACTION_LIST::CHECKER:
            ajoutLogs("IA check");
            break;
        case TYPES::ACTION_LIST::MISER:
            activationBoutons[MISER] = false;
            activationBoutons[CHECKER] = false;

            valeurMise.setMinimum(2 * jeu->getMise());

            caveIA.display(jeu->getJoueur(1).getCave());
            pot.display(jeu->getPot());

            ajoutLogs("IA mise " + QString::number(jeu->getMise()));
            break;
        case TYPES::ACTION_LIST::SUIVRE:
            caveIA.display(jeu->getJoueur(1).getCave());
            pot.display(jeu->getPot());

            ajoutLogs("IA suit");
            break;
        case TYPES::ACTION_LIST::RELANCER:
            valeurMise.setMinimum(2 * jeu->getMise());

            caveIA.display(jeu->getJoueur(1).getCave());
            pot.display(jeu->getPot());

            ajoutLogs("IA relance " + QString::number(jeu->getMise()));
            break;
        case TYPES::ACTION_LIST::SE_COUCHER:
            ajoutLogs("IA se couche");
            partieTermine();
            return;
            break;
        default:
            break;
    }

    emit tourFini();
}

void Fenetre::prochainJoueur()
{
    activeBoutons(false);

    if (!jeu->prochainJoueur()){
        partieTermine();
        return;
    }

    if (jeu->debutTour()) {
        afficheTable();
    }

    if (jeu->getJoueurCourant() == 0) {     // Joueur humain
        joueurCourant();
    }
    else {                                  // Intelligence artificielle
        jeuIA();
    }
}

void Fenetre::checker()
{
    jeu->checker(0);

    ajoutLogs("Joueur 1 check");

    emit tourFini();
}

void Fenetre::miser()
{
    int montant = valeurMise.value();

    jeu->miser(0, montant);

    caveJoueur.display(jeu->getJoueur(0).getCave());
    pot.display(jeu->getPot());

    ajoutLogs("Joueur 1 mise " + QString::number(montant));

    emit tourFini();
}

void Fenetre::suivre()
{
    jeu->suivre(0);

    caveJoueur.display(jeu->getJoueur(0).getCave());
    pot.display(jeu->getPot());

    activationBoutons[CHECKER] = true;
    activationBoutons[MISER] = true;
    valeurMise.setMinimum(jeu->getBlind());

    ajoutLogs("Joueur 1 suit");

    emit tourFini();
}

void Fenetre::relancer()
{
    int montant = valeurMise.value();

    jeu->relancer(0, montant);

    caveJoueur.display(jeu->getJoueur(0).getCave());
    pot.display(jeu->getPot());

    activationBoutons[CHECKER] = true;
    activationBoutons[MISER] = true;
    valeurMise.setMinimum(jeu->getBlind());

    ajoutLogs("Joueur 1 relance " + QString::number(montant));

    emit tourFini();
}

void Fenetre::seCoucher()
{
    jeu->seCoucher(0);

    ajoutLogs("Joueur 1 se couche");

    partieTermine();
}

void Fenetre::partieTermine()
{
    ajoutLogs("Partie terminée !");
    activeBoutons(false);

    layoutMainAdverse.vider();
    layoutMainAdverse.ajoutCartes(jeu->getJoueur(1).getMain());

    jeu->nouvelleMain(0);

    boutonDemarrage.setText("Rejouer");
    boutonDemarrage.setHidden(false);
}
