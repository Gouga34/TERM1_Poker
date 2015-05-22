/*========================================================================
Nom: CalibrageIdeal.cpp       Auteur: Manuel CHATAIGNER
Maj: 07/05/2015            Creation: 07/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe CalibrageIdeal.
=========================================================================*/


#include "../../include/IA/CalibrageIdeal.h"
#include "../../include/Constantes.h"

#include <QFile>
#include <QTextStream>


CalibrageIdeal::CalibrageIdeal(Jeu *j, Profil *iaQuiProfile, Profil *iaProfilee, int nbParties)
    : jeu(j), calibrageIAQuiProfile(iaQuiProfile), calibrageIAProfilee(iaProfilee), nombreParties(nbParties),
      nombrePartiesGagnees(0), argentTotalGagne(0)
{
    listeTauxIAQuiProfile << 15 << 30 << 45 << 60 << 75 << 90 << 100;
}

CalibrageIdeal::~CalibrageIdeal()
{

}

void CalibrageIdeal::ecrireRsultatTotalParties() const
{
    QString nomFichier = QString::number(calibrageIAProfilee->getAgressivite()) + "_"
                            + QString::number(calibrageIAProfilee->getRationalite()) + "_testsCalibrageIdeal.csv";
    QFile fichier(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE) + nomFichier);

    if (!fichier.open(QIODevice::Append | QIODevice::Text)) {
        std::cerr << "Erreur lors de l'ouverture du fichier de calibrage ideal" << std::endl;
        return;
    }

    QTextStream out(&fichier);

    // Si le fichier est vide, on ajoute l'en-tête
    if (fichier.size() == 0) {
        out << "Calibrage teste,Parties gagnees,Argent gagne" << endl;
    }

    double partiesGagnees = (static_cast<double>(nombrePartiesGagnees) * 100.0) / nombreParties;

    out << calibrageIAQuiProfile->getAgressivite() << "-" << calibrageIAQuiProfile->getRationalite()
        << "," << partiesGagnees << "," << argentTotalGagne << endl;

    fichier.close();
}

void CalibrageIdeal::lancerParties()
{
    for (int i = 0; i < listeTauxIAQuiProfile.size(); i++) {
        for (int j = 0; j < listeTauxIAQuiProfile.size(); j++) {

            calibrageIAQuiProfile->setAgressivite(listeTauxIAQuiProfile[i]);
            calibrageIAQuiProfile->setRationalite(listeTauxIAQuiProfile[j]);

            nombrePartiesGagnees = 0;
            argentTotalGagne = 0;

            std::cout << "Calibrage " << calibrageIAQuiProfile->getAgressivite() << "-" << calibrageIAQuiProfile->getRationalite() << std::endl;

            for (int n = 0; n < nombreParties; n++) {
                std::cout << "\tPartie " << n+1 << std::endl;

                jeu->reinitialisationCaves();
                jeu->distributionMain();

                jeu->lancer();

                // Si c'est l'IA qui a gagné
                if (jeu->getResultatPartie() == PERDU) {
                    nombrePartiesGagnees++;
                }

                argentTotalGagne += jeu->getJoueur(1)->getCave() - CAVE_JOUEURS;

                jeu->nouvelleMain();
            }

            ecrireRsultatTotalParties();
        }
    }
}

