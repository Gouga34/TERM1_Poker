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
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include "../../include/Interface/ChoixOptionsDialog.h"
#include "../../include/Constantes.h"


using namespace std;


ChoixOptionsDialog::ChoixOptionsDialog() : QDialog(){

    //titre fenêtre
    setWindowTitle("Choix des options");

    //fenetre
    QVBoxLayout *boite = new QVBoxLayout;

    //ajouterPseudosConnus();

    choixAdversaire.setText("IA contre IA");
    choixAdversaire.setChecked(true);

    boiteNombreCalibrages.setValue(1);
    boiteNombreCalibrages.setMaximum(500);

    boiteNombreParties.setValue(10);
    boiteNombreParties.setMaximum(500);

    boiteNombrePartiesProfilage.setValue(10);
    boiteNombrePartiesReprofilage.setValue(5);
    boiteNombrePartiesGains.setValue(10);
    boiteNombrePartiesGains.setMaximum(500);

    //QLabel *calibrageDefaut = new QLabel("Rationalité : " + QString::number(rationaliteDefaut)
    //                                       + "%\nAgressivité : " + QString::number(agressiviteDefaut) + "%");

    // Rationalité
    QHBoxLayout *layoutRationalite = new QHBoxLayout;

    rationalite.setRange(0, 100);
    rationalite.setValue(rationaliteDefaut);
    rationalite.setOrientation(Qt::Horizontal);

    QLabel *valRationalite = new QLabel(QString::number(rationaliteDefaut));

    connect(&rationalite, SIGNAL(valueChanged(int)), valRationalite, SLOT(setNum(int)));

    layoutRationalite->addWidget(&rationalite);
    layoutRationalite->addWidget(valRationalite);


    // Agressivité
//    QHBoxLayout *layoutAgressivite = new QHBoxLayout;

//    agressivite.setRange(0, 100);
//    agressivite.setValue(agressiviteDefaut);
//    agressivite.setOrientation(Qt::Horizontal);

//    QLabel *valAgressivite = new QLabel(QString::number(agressiviteDefaut));

//    connect(&agressivite, SIGNAL(valueChanged(int)), valAgressivite, SLOT(setNum(int)));

//    layoutAgressivite->addWidget(&agressivite);
//    layoutAgressivite->addWidget(valAgressivite);


    //bouton valider
    QPushButton *bouton=new QPushButton("Valider");

    //Formulaire
    QFormLayout *formulaire = new QFormLayout;
    //formulaire->addRow("Nouveau pseudo ",&nouveau);
    //formulaire->addRow("Pseudo déjà existant",&pseudos);

    formulaire->addRow("Nombre de calibrages à profiler", &boiteNombreCalibrages);
    formulaire->addRow("Nombre de parties", &boiteNombreParties);
    formulaire->addRow("Nombre de parties profilage", &boiteNombrePartiesProfilage);
    formulaire->addRow("Nombre de parties reprofilage", &boiteNombrePartiesReprofilage);
    formulaire->addRow("Nombre de parties minimum phase de gains", &boiteNombrePartiesGains);
    formulaire->addRow("Analyse des gains de parties", &caseAnalyseGainsParties);

    //formulaire->addRow("Calibrage par défaut", calibrageDefaut);
    //formulaire->addRow("Rationalité", layoutRationalite);
    //formulaire->addRow("Agressivité", layoutAgressivite);

    //Ajout du formulaire et du bouton valider dans la fenetre
    boite->setAlignment(Qt::AlignHCenter);
    boite->addWidget(&choixAdversaire);
    boite->addLayout(formulaire);
    boite->addWidget(bouton);

    //setLayout(formulaire);
    setLayout(boite);

    //Connection avec l'interface graphique du jeu
    connect(bouton, SIGNAL(clicked()), this, SLOT(accept()));

}

void ChoixOptionsDialog::ajouterPseudosConnus(){

    //Ajout d'un pseudo vide :
    pseudos.addItem("Calibrage aléatoire");
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
    }
}

Options ChoixOptionsDialog::getOptions(){

    Options options;

    if (exec() == QDialog::Accepted) {
        QString nouveauPseudo = nouveau.text();
        QString ancienPseudo = pseudos.currentText();

        options.joueurIA = choixAdversaire.isChecked();
        options.nombreCalibrages = boiteNombreCalibrages.value();
        options.nombreParties = boiteNombreParties.value();

        options.nombrePartiesProfilage = boiteNombrePartiesProfilage.value();
        options.nombrePartiesReprofilage = boiteNombrePartiesReprofilage.value();
        options.nombrePartiesGains = boiteNombrePartiesGains.value();

        options.analyseGainsParties = caseAnalyseGainsParties.isChecked();

        if(!nouveauPseudo.isEmpty()){
            options.pseudo = nouveauPseudo;
        }
        else if(pseudos.currentIndex() == 0){
            options.pseudo = "";
        }
        else{
            options.pseudo = ancienPseudo;
        }

        options.rationaliteIA = rationalite.value();
        options.agressiviteIA = agressivite.value();
    }

    return options;
}
