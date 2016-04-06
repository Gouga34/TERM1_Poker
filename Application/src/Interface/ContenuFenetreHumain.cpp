/*========================================================================
Nom: ContenuFenetreHumain.cpp       Auteur: Manuel Chataigner
Maj: 20/05/2015            Creation: 20/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe ContenuFenetreHumain.
=========================================================================*/

#include "../../include/Interface/ContenuFenetreHumain.h"
#include "../../include/Interface/CardsDialog.h"
#include "../../include/Interface/Logger.h"

#include <QString>
#include <QHBoxLayout>
#include <QEventLoop>


QPixmap *ContenuFenetreHumain::textureCartes = 0;

ContenuFenetreHumain::ContenuFenetreHumain(game::Game *j, Window *f) : ContenuFenetre(j)
{
    // Couleur de fond
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(20, 127, 20));
    f->setPalette(pal);

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
    // Boutons d'action
    // ////////////////////////////////////////////////////

    QHBoxLayout *layoutDemarrage = new QHBoxLayout;

    boutonDemarrage.setText("Démarrage partie");
    boutonDemarrage.setMaximumWidth(150);
    boutonDemarrage.hide();
    connect(&boutonDemarrage, SIGNAL(clicked()), f, SLOT(demarragePartie()));

    boutonChoixCartes.setChecked(false);
    boutonChoixCartes.setText("Choix des cartes");
    boutonChoixCartes.setObjectName(QString("Checkbox"));
    boutonChoixCartes.setStyleSheet("QWidget#Checkbox { background-color: rgb(255, 255, 255); border-style: solid; border-color: black; border-width: 1px; padding : 3px; }");

    layoutDemarrage->setAlignment(Qt::AlignLeft);
    layoutDemarrage->setSpacing(10);
    layoutDemarrage->addWidget(&boutonDemarrage);
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

ContenuFenetreHumain::~ContenuFenetreHumain()
{

}

void ContenuFenetreHumain::debutPartie()
{
    // Sélection des cartes par l'utilisateur
    if (boutonChoixCartes.isChecked()) {
        CardsDialog fenetreCartes(this);
        std::vector<int> ids = fenetreCartes.cardsChoice();

        if (!ids.empty()) {
            jeu->affectsCards(ids);
        }
    }

    boutonDemarrage.hide();
    boutonChoixCartes.hide();
    resultatPartie.clear();

    Logger::getInstance()->addLogs("Distribution des cartes");
    afficheTable();


    // Affichage de la main adverse dans les logs
    std::vector<game::Card> jeuAdverse = jeu->getPlayer(1)->getHand();

    Logger::getInstance()->addLogs("Jeu adverse : ");
    for (unsigned int i = 0; i < jeuAdverse.size(); i++) {
        Logger::getInstance()->addLogs("-> " + QString::number(jeuAdverse.at(i).getRank())
                  + " " + GraphicCard::colors[jeuAdverse.at(i).getColor()]);
    }

    // Main du joueur
    layoutMain.clear();
    layoutMain.addCards(jeu->getPlayer(0)->getHand());

    // Cartes communes
    layoutCartesCommunes.clear();

    for (int i = 0; i < 5; i++) {
        GraphicCard *dos = new GraphicCard(0, 0);
        layoutCartesCommunes.addWidget(dos);
    }

    // Main adverse

    GraphicCard *dos = new GraphicCard(0, 0);
    GraphicCard *dos2 = new GraphicCard(0, 0);

    layoutMainAdverse.clear();
    layoutMainAdverse.addWidget(dos);
    layoutMainAdverse.addWidget(dos2);

    activeBoutons(false);
}

void ContenuFenetreHumain::actualiser()
{
    pot.display(jeu->getPot());
    caveJoueur.display(jeu->getPlayer(0)->getCave());
    caveIA.display(jeu->getPlayer(1)->getCave());

    valeurMise.setMinimum(jeu->getBlind());

    partieTermine();
}

game::Action ContenuFenetreHumain::getAction()
{
    // Mise à jour des informations
    afficheTable();
    pot.display(jeu->getPot());
    caveJoueur.display(jeu->getPlayer(0)->getCave());
    caveIA.display(jeu->getPlayer(1)->getCave());

    activeBoutons(true);

    // Attente d'une action de l'utilisateur
    QEventLoop loop;
    connect(this, SIGNAL(actionChoisie()), &loop, SLOT(quit()));
    loop.exec();

    activeBoutons(false);

    return game::Action(actionUtilisateur, valeurMise.value());
}

void ContenuFenetreHumain::affichageLogs()
{
    bool visible = !logs.isHidden();

    logs.setHidden(visible);
}

void ContenuFenetreHumain::activeBoutons(bool active)
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
                    boutons[i].setEnabled(jeu->canCheck(0));
                    break;

                case MISER:
                    boutons[i].setEnabled(jeu->canBet(0, 1));
                    break;

                case SUIVRE:
                    boutons[i].setEnabled(jeu->canCall(0));
                    break;

                case RELANCER:
                    boutons[i].setEnabled(jeu->canRaise(0, 2 * jeu->getCurrentBet()));
                    break;

                default:
                    boutons[i].setEnabled(true);
                    break;
            }
        }

        valeurMise.setMinimum(2 * jeu->getCurrentBet());
        valeurMise.setMaximum(jeu->getPlayer(0)->getCave());
    }
}

void ContenuFenetreHumain::afficheTable()
{
    std::vector<game::Card> table = jeu->getTable();

    layoutCartesCommunes.clear();
    layoutCartesCommunes.addCards(table);

    // On complète la table avec des dos de carte
    for (unsigned int i = 0; i < 5 - table.size(); i++) {
        GraphicCard *dos = new GraphicCard(0, 0);
        layoutCartesCommunes.addWidget(dos);
    }
}

void ContenuFenetreHumain::checker()
{
    actionUtilisateur = ACTION::CHECKER;

    Logger::getInstance()->addLogs("Joueur 1 check");

    emit actionChoisie();
}

void ContenuFenetreHumain::miser()
{
    actionUtilisateur = ACTION::MISER;

    Logger::getInstance()->addLogs("Joueur 1 mise " + QString::number(valeurMise.value()));

    emit actionChoisie();
}

void ContenuFenetreHumain::suivre()
{
    actionUtilisateur = ACTION::SUIVRE;

    Logger::getInstance()->addLogs("Joueur 1 suit");

    emit actionChoisie();
}

void ContenuFenetreHumain::relancer()
{
    actionUtilisateur = ACTION::RELANCER;

    Logger::getInstance()->addLogs("Joueur 1 relance " + QString::number(valeurMise.value()));

    emit actionChoisie();
}

void ContenuFenetreHumain::seCoucher()
{
    actionUtilisateur = ACTION::SE_COUCHER;

    Logger::getInstance()->addLogs("Joueur 1 se couche");

    emit actionChoisie();
}

void ContenuFenetreHumain::tapis()
{
    actionUtilisateur = ACTION::TAPIS;

    Logger::getInstance()->addLogs("Joueur 1 fait tapis");

    emit actionChoisie();
}

void ContenuFenetreHumain::partieTermine()
{
    Logger::getInstance()->addLogs("Partie terminée !");
    activeBoutons(false);

    layoutMainAdverse.clear();
    layoutMainAdverse.addCards(jeu->getPlayer(1)->getHand());

    afficheTable();

    int gagne = jeu->getGameResult();

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

    boutonDemarrage.setText("Rejouer");
    boutonDemarrage.show();
    boutonChoixCartes.show();
}
