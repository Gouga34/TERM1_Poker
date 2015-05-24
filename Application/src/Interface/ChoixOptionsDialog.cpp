/*========================================================================
Nom: ChoixOptionsDialog.cpp       Auteur: Morgane Vidal
Maj: 11/03/2015            Creation: 11/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe
                                                        ChoixOptionsDialog.
=========================================================================*/

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include "../../include/Interface/ChoixOptionsDialog.h"
#include "../../include/Constantes.h"


using namespace std;


ChoixOptionsDialog::ChoixOptionsDialog()
    : QDialog(), calibrageIAProfilee("IA profilée"), calibrageIAQuiProfile("IA qui profile"),
        calibrageAdversaire("") {

    //titre fenêtre
    setWindowTitle("Choix des options");

    //fenetre
    QVBoxLayout *boite = new QVBoxLayout;

    //ajouterPseudosConnus();

    modeProfilage.setText("Profilage");
    modeCalibrageOptimal.setText("Recherche du calibrage optimal");

    connect(&modeProfilage, SIGNAL(clicked()), this, SLOT(changePageLayout()));
    connect(&modeCalibrageOptimal, SIGNAL(clicked()), this, SLOT(changePageLayout()));

    QGroupBox *parametres = new QGroupBox("Paramètres");

    QWidget *widgetProfliage = new QWidget;
    QVBoxLayout *layoutProfilage = new QVBoxLayout;

    choixAdversaire.setTitle("Joueur humain");
    choixAdversaire.setCheckable(true);
    choixAdversaire.setChecked(false);

    QFormLayout *layoutJoueurHumain = new QFormLayout;
    layoutJoueurHumain->addRow("Pseudo", &pseudo);
    choixAdversaire.setLayout(layoutJoueurHumain);

    boiteNombreCalibrages.setValue(1);
    boiteNombreCalibrages.setMaximum(500);

    boiteNombreParties.setValue(10);
    boiteNombreParties.setMaximum(500);

    boiteNombrePartiesCalibrageOptimal.setMaximum(1000);
    boiteNombrePartiesCalibrageOptimal.setValue(100);

    boiteNombrePartiesProfilage.setValue(10);
    boiteNombrePartiesReprofilage.setValue(5);
    boiteNombrePartiesGains.setValue(10);
    boiteNombrePartiesGains.setMaximum(500);

    caseReinitialisationCaves.setChecked(true);

    //bouton valider
    QPushButton *bouton=new QPushButton("Valider");

    //Formulaire
    QFormLayout *formulaire = new QFormLayout;

    formulaire->addRow("Nombre de calibrages à profiler", &boiteNombreCalibrages);
    formulaire->addRow("Nombre de parties", &boiteNombreParties);
    formulaire->addRow("Nombre de parties profilage", &boiteNombrePartiesProfilage);
    formulaire->addRow("Nombre de parties reprofilage", &boiteNombrePartiesReprofilage);
    formulaire->addRow("Nombre de parties minimum phase de gains", &boiteNombrePartiesGains);
    formulaire->addRow("Analyse des gains de parties", &caseAnalyseGainsParties);
    formulaire->addRow("Réinitialisation des caves entre chaque partie", &caseReinitialisationCaves);

    layoutProfilage->addWidget(&choixAdversaire);
    layoutProfilage->addLayout(formulaire);
    layoutProfilage->addWidget(&calibrageIAProfilee);
    layoutProfilage->addWidget(&calibrageIAQuiProfile);
    layoutProfilage->addWidget(bouton);

    widgetProfliage->setLayout(layoutProfilage);


    QWidget *widgetCalibrageOptimal = new QWidget;
    QFormLayout *layoutCalibrageOptimal = new QFormLayout;

    calibrageAdversaire.setCheckable(false);

    layoutCalibrageOptimal->addRow("Nombre de parties", &boiteNombrePartiesCalibrageOptimal);
    layoutCalibrageOptimal->addRow("Calibrage adverse", &calibrageAdversaire);

    widgetCalibrageOptimal->setLayout(layoutCalibrageOptimal);


    layoutParametres.addWidget(widgetProfliage);
    layoutParametres.addWidget(widgetCalibrageOptimal);

    modeProfilage.setChecked(true);

    parametres->setLayout(&layoutParametres);

    //Ajout du formulaire et du bouton valider dans la fenetre
    boite->setAlignment(Qt::AlignHCenter);
    boite->addWidget(&modeProfilage);
    boite->addWidget(&modeCalibrageOptimal);
    boite->addWidget(parametres);


    //setLayout(formulaire);
    setLayout(boite);

    //Connection avec l'interface graphique du jeu
    connect(bouton, SIGNAL(clicked()), this, SLOT(accept()));
}

ChoixOptionsDialog::~ChoixOptionsDialog(){

}

void ChoixOptionsDialog::changePageLayout(){

    if (modeProfilage.isChecked()) {
        layoutParametres.setCurrentIndex(0);
    }
    else {
        layoutParametres.setCurrentIndex(1);
    }
}

void ChoixOptionsDialog::ajouterPseudosConnus(){

    //Ajout d'un pseudo vide :
    /*pseudos.addItem("Calibrage aléatoire");
    //Ouverture du fichier
    QFile fichierPseudos(QString::fromStdString(FICHIER_PSEUDOS_PROFILAGE));

    if(!fichierPseudos.open(QIODevice::ReadOnly | QIODevice::Text)){
        cerr<<"Erreur lors de l'ouverture du fichier contenant la liste des pseudos"<<endl;
        return;
    }
    QTextStream flux(&fichierPseudos);
    QString ligne;
    while(! flux.atEnd()){
        ligne = flux.readLine();
        pseudos.addItem(ligne);
    }*/
}

Options ChoixOptionsDialog::getOptions(){

    Options options;

    if (exec() == QDialog::Accepted) {
        QString pseudoJoueur = pseudo.text();

        options.joueurIA = !choixAdversaire.isChecked();
        options.nombreCalibrages = boiteNombreCalibrages.value();
        options.nombreParties = boiteNombreParties.value();

        options.nombrePartiesProfilage = boiteNombrePartiesProfilage.value();
        options.nombrePartiesReprofilage = boiteNombrePartiesReprofilage.value();
        options.nombrePartiesGains = boiteNombrePartiesGains.value();

        options.analyseGainsParties = caseAnalyseGainsParties.isChecked();

        options.pseudo = pseudoJoueur;

        //options.rationaliteIA = rationalite.value();
        //options.agressiviteIA = agressivite.value();
    }

    return options;
}
