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

class Profil
{
    private:

        std::string pseudo;

        double agressivite;

        double rationalite;

        double bluff;

        double passivite;

    public:

        Profil();
        Profil(std::string pseudoProfil);
        ~Profil();

        std::string getPseudo() const;

        double getAgressivite() const;
        double getRationalite() const;
        double getBluff() const;
        double getPassivite() const;

        void setPseudo(std::string nvPseudo);

        void setAgressivite(double nvAgr);
        void setRationalite(double nvRat);
        void setBluff(double nvBluff);
        void setPassivite(double nvPass);
};

#endif // CALIBRAGE_H
