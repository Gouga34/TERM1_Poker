#include "Fenetre.h"
#include "CarteGraphique.h"
#include <QVBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

QPixmap *Fenetre::textureCartes = 0;

Fenetre::Fenetre() : QWidget()
{
    setWindowTitle(tr("Poker"));
    resize(800,600);

    // Couleur de fond
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(20, 127, 20));
    setAutoFillBackground(true);
    setPalette(pal);

    QVBoxLayout *layout = new QVBoxLayout;

    QHBoxLayout *layoutCommun = new QHBoxLayout;
    QHBoxLayout *layoutJoueur = new QHBoxLayout;


    // ////////////////////////////////////////////////////
    // Cartes
    // ////////////////////////////////////////////////////


    // Chargement de l'image
    if (!textureCartes){
        textureCartes = new QPixmap("deck.png");
    }

    CarteGraphique *c1 = new CarteGraphique(1, 2);
    CarteGraphique *c2 = new CarteGraphique(10, 0);

    CarteGraphique *dos = new CarteGraphique(0, 0);
    CarteGraphique *dos2 = new CarteGraphique(0, 0);

    CarteGraphique *c3 = new CarteGraphique(1, 3);
    CarteGraphique *c4 = new CarteGraphique(3, 0);
    CarteGraphique *c5 = new CarteGraphique(13, 1);

    // Création des listes

    main << c1 << c2;
    mainAdverse << dos << dos2;
    communes << c3 << c4 << c5;

    layoutMain.ajoutCartes(main);
    layoutMainAdverse.ajoutCartes(mainAdverse);
    layoutCartesCommunes.ajoutCartes(communes);


    // ////////////////////////////////////////////////////
    // Compteurs
    // ////////////////////////////////////////////////////

    pot.setMaximumSize(100, 50);
    pot.setSegmentStyle(QLCDNumber::Filled);
    pot.display(0);

    cave.setMaximumSize(100, 50);
    cave.setSegmentStyle(QLCDNumber::Filled);
    cave.display(500);


    // ////////////////////////////////////////////////////
    // Boutons d'action
    // ////////////////////////////////////////////////////

    QVBoxLayout *layoutBoutons = new QVBoxLayout;

    layoutBoutons->setSpacing(10);
    layoutBoutons->setAlignment(Qt::AlignTop);

    QPushButton *boutonChecker = new QPushButton("Checker");
    QPushButton *boutonMiser = new QPushButton("Miser");
    QPushButton *boutonSuivre = new QPushButton("Suivre");
    QPushButton *boutonRelancer = new QPushButton("Relancer");
    QPushButton *boutonSeCoucher = new QPushButton("Se coucher");

    layoutBoutons->addWidget(boutonChecker);
    layoutBoutons->addWidget(boutonMiser);
    layoutBoutons->addWidget(boutonSuivre);
    layoutBoutons->addWidget(boutonRelancer);
    layoutBoutons->addWidget(boutonSeCoucher);


    connect(boutonMiser, SIGNAL(clicked()), this, SLOT(miser()));
    //connect(boutonSeCoucher, SIGNAL(clicked()), this, SLOT(seCoucher()));



    // ////////////////////////////////////////////////////
    // Fenetre
    // ////////////////////////////////////////////////////

    QPushButton *next = new QPushButton("Next");
    connect(next, SIGNAL(clicked()), this, SLOT(ajouterCarte()));

    layoutCommun->addLayout(&layoutCartesCommunes);
    layoutCommun->addWidget(&pot);

    layoutJoueur->addLayout(&layoutMain);
    layoutJoueur->addWidget(&cave);
    layoutJoueur->addWidget(&valeur);
    layoutJoueur->addLayout(layoutBoutons);
    layoutJoueur->addWidget(next);


    layout->setSpacing(150);
    layout->setAlignment(Qt::AlignHCenter);

    layout->addLayout(&layoutMainAdverse);
    layout->addLayout(layoutCommun);
    layout->addLayout(layoutJoueur);

    setLayout(layout);
}

Fenetre::~Fenetre()
{

}

void Fenetre::miser()
{
    int montant = valeur.value();

    cave.display(cave.value() - montant);
    pot.display(pot.value() + montant);
}

void Fenetre::ajouterCarte()
{
    //communes << nouvelleCarte
    //layoutCartesCommunes.addWidget(nouvelleCarte);
}
