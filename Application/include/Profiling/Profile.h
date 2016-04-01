/*========================================================================
Nom: Profil.h         Auteur: Manuel Chataigner
Maj: 01/04/2015          Creation: 01/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe contenant les informations du profil d'un joueur.
=========================================================================*/

#ifndef CALIBRAGE_H
#define CALIBRAGE_H

#include <string>

namespace profiling {

    class Profile
    {
        private:

            std::string m_pseudo;

            double m_aggressiveness;
            double m_rationality;
            double m_bluff;
            double m_passivity;

        public:

            Profile();
            Profile(std::string pseudo);
            ~Profile();

            std::string getPseudo() const;

            double getAggressiveness() const;
            double getRationality() const;
            double getBluff() const;
            double getPassivity() const;

            void setPseudo(std::string pseudo);

            void setAggressiveness(double aggressiveness);
            void setRationality(double rationality);
            void setBluff(double bluff);
            void setPassivity(double passivity);
    };
}

#endif // CALIBRAGE_H
