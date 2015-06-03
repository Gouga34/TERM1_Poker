/*========================================================================
Nom: CalibrageIdeal.h       Auteur: Manuel CHATAIGNER
Maj: 07/05/2015          Creation: 07/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe déterminant le calibrage optimal pour gagner
                face à un profilage donné.
=========================================================================*/

#ifndef CALIBRAGEIDEAL_H
#define CALIBRAGEIDEAL_H

#include <QVector>
#include "../Profilage/Profil.h"
#include "../Jeu/Jeu.h"

class CalibrageIdeal
{
    private:

        Jeu *jeu;

        Profil *calibrageIAQuiProfile;
        Profil *calibrageIAProfilee;

        int nombreParties;
        int nombrePartiesGagnees;
        int argentTotalGagne;

        QVector<int> listeTauxIAQuiProfile;


        /**
         * @brief Ajout une ligne dans le fichier avec les résultats
         *        du calibrage donné face au calibrage testé.
         */
        void ecrireRsultatTotalParties() const;

    public:

        CalibrageIdeal(Jeu *j, Profil *iaQuiProfile, Profil *iaProfilee, int nbParties);
        ~CalibrageIdeal();

        /**
         * @brief Effectue nombreParties pour chaque calibrage possible face au calibrage adverse
         */
        void lancerParties();
};

#endif // CALIBRAGEIDEAL_H
