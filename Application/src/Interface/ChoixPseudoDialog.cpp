
/*========================================================================
Nom: ChoixPseudoDialog.cpp       Auteur: Morgane Vidal
Maj: 11/03/2015            Creation: 11/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe
                                                        ChoixPseudoDialog.
=========================================================================*/
#include <QLabel>
#include<iostream>
#include<QFormLayout>
#include<QFile>
#include<QTextStream>
#include<QString>
#include "../../include/Interface/ChoixPseudoDialog.h"


using namespace std;


ChoixPseudoDialog::ChoixPseudoDialog() : QDialog(){

    //titre fenêtre
    setWindowTitle("Choix du pseudo");


    //fenetre
    QVBoxLayout *boite = new QVBoxLayout;

    //bouton valider
    QPushButton *bouton=new QPushButton("Valider");

    //Formulaire
    QFormLayout *formulaire = new QFormLayout;
    formulaire->addRow(tr("&Nouveau pseudo "),&nouveau);
    formulaire->addRow(tr("&Pseudo déjà existant"),&pseudos);
    ajouterPseudosConnus();

    //Ajout du formulaire et du bouton valider dans la fenetre
    boite->addLayout(formulaire);
    boite->addWidget(bouton);

    //setLayout(formulaire);
    setLayout(boite);

    //Connection avec l'interface graphique du jeu
    connect(bouton, SIGNAL(clicked()), this, SLOT(accept()));

}

void ChoixPseudoDialog::ajouterPseudosConnus(){

    //Ajout d'un pseudo vide :
    pseudos.addItem("");
    //Ouverture du fichier
    QFile fichierPseudos("../Application/ressources/Profilage/ProfilageStatique/pseudos.txt");

    if(!fichierPseudos.open(QIODevice::ReadOnly | QIODevice::Text)){
        cerr<<"Erreur lors de l'ouverture du fichier contenant la liste des pseudos"<<endl;
    }
    QTextStream flux(&fichierPseudos);
    QString ligne;
    while(! flux.atEnd()){
        ligne = flux.readLine();
        pseudos.addItem(ligne);
    }
}

QString ChoixPseudoDialog::getPseudo(){

    if (exec() == QDialog::Accepted) {
        QString nouveauPseudo = nouveau.text();
       // QString ancienPseudo = pseudos.itemData(pseudos.currentIndex()).toString();
        QString ancienPseudo = pseudos.currentText();

        if(ancienPseudo.isEmpty() && !nouveauPseudo.isEmpty()){
            return nouveauPseudo;
        }
        else if(!ancienPseudo.isEmpty()){
                return ancienPseudo;
        }
        else{

            cerr<<"Pas de pseudo"<<endl;
            return "";
        }

    }
    else {
        return "";
    }


}
