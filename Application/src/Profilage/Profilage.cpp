/*========================================================================
Nom: Profilage.cpp       Auteur: Manuel CHATAIGNER
Maj: 04/03/2015            Creation: 04/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Profilage.
=========================================================================*/

#include "../../include/Profilage/Profilage.h"
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <iostream>
#include <QStringList>
#include <string>

Profilage::Profilage(Profil *calibrageIA, Profil *profil)
    : profilIA(calibrageIA), profilJoueur(profil),
      etatPartie{{false, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}
{

}

Profilage::~Profilage()
{

}

//void Profilage::charger()
//{
//    QString nomFichier = QString::fromStdString(profilJoueur->getPseudo()) + ".json";
//    QFile fichier(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE) + nomFichier);
//    if (!fichier.open(QIODevice::ReadOnly)) {
//        std::cerr << "Le fichier du pseudo " << profilJoueur->getPseudo() << " n'existe pas encore." << std::endl;
//        return;
//    }

//    QByteArray donneesJson = fichier.readAll();
//    QJsonDocument doc(QJsonDocument::fromJson(donneesJson));
//    QJsonObject json(doc.object());

//    QJsonArray parties = json["parties"].toArray();

//    for (int i = 0; i < parties.size(); i++) {
//        QJsonObject partie = parties[i].toObject();

//        for (int i = 0; i < ETAPE_JEU::NB_ETAPES; i++) {
//            QString nomEtape = QString::fromStdString(nomEtapes[i]);
//            QJsonObject etape = partie[nomEtape].toObject();

//            etatPartie[i].couche = etape["couche"].toBool();

//            etatPartie[i].probaGainAdversaire = etape["probaGainAdversaire"].toDouble();

//            etatPartie[i].tauxAgressivite = etape["agressivité"].toDouble();
//            etatPartie[i].tauxRationnalite = etape["rationalite"].toDouble();
//            etatPartie[i].tauxBluff = etape["bluff"].toDouble();
//            etatPartie[i].tauxPassivite = etape["passivite"].toDouble();

//            etatPartie[i].tauxSuivis = etape["suivis"].toDouble();
//            etatPartie[i].tauxChecks = etape["checks"].toDouble();
//            etatPartie[i].tauxMises = etape["mises"].toDouble();

//            etatPartie[i].misePlusHaute = etape["misePlusHaute"].toDouble();
//            etatPartie[i].miseTotaleJoueur = etape["miseTotaleJoueur"].toDouble();
//        }

//        QJsonObject global = partie["global"].toObject();

//        profilJoueur->setAgressivite(global["agressivité"].toDouble());
//        profilJoueur->setBluff(global["bluff"].toDouble());
//        profilJoueur->setRationalite(global["rationalite"].toDouble());
//        profilJoueur->setPassivite(global["passivite"].toDouble());

//        QJsonObject calibrageIA = partie["calibrageIA"].toObject();


//        profilIA->setAgressivite(calibrageIA["agressivité"].toDouble());
//        profilIA->setRationalite(calibrageIA["rationalite"].toDouble());
//    }


//    fichier.close();
//}

void Profilage::sauvegarder()
{
   QString nomFichier = QString::fromStdString(profilJoueur->getPseudo()) + ".csv";
   QFile fichier(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE) + nomFichier);

   if(profilJoueur->getPseudo().compare("") != 0){
       if (!fichier.open(QIODevice::ReadWrite)) {

           bool dejaPresent = false;

   //Vérif si le pseudo est déjà présent ds le fichier "pseudos.txt".
           QFile pseudo(QString::fromStdString(FICHIER_PSEUDOS_PROFILAGE));
           if (!pseudo.open(QIODevice::ReadOnly | QIODevice::Text)) {
               std::cerr << "Erreur lors de l'ouverture du fichier des pseudos !" << std::endl;
               return;
           }

           QTextStream flux(&pseudo);

           QString ligne;
           while(!flux.atEnd()){
               ligne = flux.readLine();
               if(ligne.compare(QString::fromStdString(profilJoueur->getPseudo())) == 0 || ligne.compare(QString::fromStdString("")) == 0){
                   dejaPresent = true;
               }
           }

            pseudo.close();

     //Si pas présent, ajout du pseudo dans le fichier
           if(!dejaPresent){
               // Ouverture de la liste des pseudos
               QFile listePseudos(QString::fromStdString(FICHIER_PSEUDOS_PROFILAGE));
               if (!listePseudos.open(QIODevice::Append | QIODevice::Text)) {
                   std::cerr << "Erreur lors de l'ouverture du fichier des pseudos !" << std::endl;
                   return;
               }

               QTextStream out(&listePseudos);
               out << QString::fromStdString(profilJoueur->getPseudo()) << endl;

               listePseudos.close();
            }
       }

       QTextStream out(&fichier);
       QString ligne;
       QStringList liste;
       //Si fichier vide, ajout du nombre de parties et du nombre de parties gagnées par l'IA qui profile.
        if(fichier.size()==0){
            out<<"Nombre de parties:,"<<1<<"\n";
            if(partieGagneeIAQuiProfile==GAGNE){
                out<<" Nombre de parties gagnees par l'IA qui profile :,"<<1<<"\n";
            }
            else{
                out<<" Nombre de parties gagnees par l'IA qui profile:,"<<0<<"\n";
            }

            out<<"\n\n Gain IA profilee,Scenarios de tests en cours,Agressivite IA qui profile, Rationalite IA qui profile,Chances de gain de l'IA profilee,"<<
                 "Agressivite IA profilee,Rationalite IA profilee,Bluff IA profilee,Passivite IA profilee,Nombre de checks,"<<
                 "Nombre de mises,Nombre de suivis,Total des mises de l'IA profilee,Mise la plus haute IA profilee,Joueur profile se couche"<<endl;
        }
       //Si pas vide, incrémentation.
        else{
            ligne=fichier.readLine();
            liste=ligne.split(",");
            int nombreParties=liste.at(1).toInt();
            nombreParties++;
            std::string nombrePartiesAEcrire="Nombre de parties :,"+QString::number(nombreParties).toStdString()+"\n";
            ligne=fichier.readLine();

            std::string nombrePartiesGagneesAEcrire;

            if(partieGagneeIAQuiProfile==GAGNE){
                liste=ligne.split(",");
                int nombrePartiesGagneesIAQuiProfile=liste.at(1).toInt();
                nombrePartiesGagneesIAQuiProfile++;

                nombrePartiesGagneesAEcrire="Nombre de parties gagnees par l'IA qui profile:,"+QString::number(nombrePartiesGagneesIAQuiProfile).toStdString()+"\n";
            }
            fichier.seek(0);


            fichier.write(nombrePartiesAEcrire.c_str());
            if(partieGagneeIAQuiProfile==GAGNE){

                fichier.write(nombrePartiesGagneesAEcrire.c_str());
            }
        }
            //positionnement à la fin du fichier
            ligne=fichier.readLine();
            while(!ligne.isEmpty()){
                ligne=fichier.readLine();
            }

            //Écriture des données
            int i = ETAPE_JEU::NB_ETAPES;
            if(partieGagneeIAQuiProfile==GAGNE){
                out<<1<<",";
            }
            else if(partieGagneeIAQuiProfile==EGALITE){
            out<<2<<",";
            }
            else{
                out<<0<<",";
            }
              out<<scenarioDeTest<<","<<profilIA->getAgressivite()<<","<<profilIA->getRationalite()<<","<<
                    etatPartie[i].probaGainAdversaire<<","<<etatPartie[i].tauxAgressivite<<","<<etatPartie[i].tauxRationnalite<<","<<
                    etatPartie[i].tauxBluff<<","<<etatPartie[i].tauxPassivite<<","<<etatPartie[i].tauxChecks<<","<<
                    etatPartie[i].tauxMises<<","<<etatPartie[i].tauxSuivis<<","<<etatPartie[i].miseTotaleJoueur<<","<<
                    etatPartie[i].misePlusHaute<<","<<etatPartie[i].couche<<endl;




   }
   fichier.close();
}

void Profilage::clear(){

    int etape = ETAPE_JEU::NB_ETAPES;

    this->etatPartie[etape].couche = false;
    this->etatPartie[etape].probaGainAdversaire = 0;

    this->etatPartie[etape].tauxAgressivite = 0;
    this->etatPartie[etape].tauxRationnalite = 0;
    this->etatPartie[etape].tauxBluff = 0;
    this->etatPartie[etape].tauxPassivite = 0;

    this->etatPartie[etape].tauxSuivis = 0;
    this->etatPartie[etape].tauxChecks = 0;
    this->etatPartie[etape].tauxMises = 0;

    this->etatPartie[etape].misePlusHaute = 0;
    this->etatPartie[etape].miseTotaleJoueur = 0;

    this->etatPartie[etape].nbTotalActions = 0;
}
