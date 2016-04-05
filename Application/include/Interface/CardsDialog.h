/*========================================================================
Nom: CartesDialog.h       Auteur: Manuel CHATAIGNER
Maj: 09/03/2015          Creation: 05/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant la fenêtre de choix des cartes.
=========================================================================*/

#ifndef CARDSDIALOG_H
#define CARDSDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QRadioButton>
#include <vector>
#include "ClickableCard.h"

class CardsDialog : public QDialog
{
    Q_OBJECT

    private:

        enum CardsList { PLAYER, AI, MIDDLE };

        QGridLayout m_cardsLayout;

        // Vecteur des identifants des cartes choisies (2 Joueur, 2 IA, 5 milieu)
        std::vector<int> m_selectedCards;

        QRadioButton m_playerChoice;
        QRadioButton m_aiChoice;
        QRadioButton m_middleChoice;

        /**
         * @brief Ajoute la carte sélectionnée dans la liste de cartes correspondante
         * @param id Identifiant de la carte à ajouter
         * @param list Liste dans laquelle ajouter la carte
         * @return true si la carte a été ajoutée, faux sinon (liste pleine)
         */
        virtual bool addCard(int id, CardsList list);

    private slots:

        /**
         * @brief Selectionne la carte cliquée
         * @param id Identifiant de la carte
         */
        void cardSelected(int id);

    public:

        CardsDialog(QWidget *parent = 0);
        virtual ~CardsDialog();

        /**
         * @brief Ouvre la fenêtre permettant au joueur de choisir les 2 cartes de l'adversaire
         * @return Vecteur des identifiants des deux cartes séletionnées, vide si pas de choix
         */
        virtual std::vector<int> cardsChoice();
};

#endif // CARDSDIALOG_H
