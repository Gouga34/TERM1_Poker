/*========================================================================
Nom: CartesDialog.cpp       Auteur: Manuel CHATAIGNER
Maj: 09/03/2015            Creation: 05/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe CartesDialog.
=========================================================================*/

#include "../../include/Interface/CardsDialog.h"
#include <QDialogButtonBox>
#include <QPalette>
#include <QHBoxLayout>
#include <QVBoxLayout>


CardsDialog::CardsDialog(QWidget *parent) : QDialog(parent), m_selectedCards(9, -1)
{
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(20, 127, 20));
    setPalette(pal);

    QHBoxLayout *layout = new QHBoxLayout;

    for (int i = 0; i < 4; i++) {
        m_cardsLayout.setRowMinimumHeight(i, 107);
    }

    for (int i = 1; i <= 13; i++) {
        m_cardsLayout.setColumnMinimumWidth(i-1, 83);

        for (int j = 0; j < 4; j++) {
            ClickableCard *card = new ClickableCard(i, j);
            m_cardsLayout.addWidget(card, j, i, Qt::AlignCenter);

            connect(card, SIGNAL(clicked(int)), this, SLOT(cardSelected(int)));
        }
    }

    QDialogButtonBox *dialogButton = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    m_cardsLayout.addWidget(dialogButton, 4, 12, 1, 2);

    QVBoxLayout *buttonsLayout = new QVBoxLayout;

    m_playerChoice.setText("Joueur");
    m_aiChoice.setText("IA");
    m_middleChoice.setText("Milieu");
    m_playerChoice.setChecked(true);

    buttonsLayout->addWidget(&m_playerChoice);
    buttonsLayout->addWidget(&m_aiChoice);
    buttonsLayout->addWidget(&m_middleChoice);

    QWidget *buttons = new QWidget;
    buttons->setLayout(buttonsLayout);
    buttons->setObjectName(QString("Box"));
    buttons->setStyleSheet("QWidget#Box { background-color: rgb(200, 200, 200); border-style: solid; border-color: black; border-width: 1px }");

    layout->setSpacing(3);
    layout->addWidget(buttons);
    layout->addLayout(&m_cardsLayout);

    setLayout(layout);

    connect(dialogButton, SIGNAL(accepted()), this, SLOT(accept()));
    connect(dialogButton, SIGNAL(rejected()), this, SLOT(reject()));
}

CardsDialog::~CardsDialog()
{

}

std::vector<int> CardsDialog::cardsChoice()
{
    if (exec() == QDialog::Accepted) {
        //&& std::find(cartesSelectionnees.begin(), cartesSelectionnees.end(), -1) == cartesSelectionnees.end()) {
        return m_selectedCards;
    }
    else {
        return std::vector<int>();
    }
}

bool CardsDialog::addCard(int id, CardsList list)
{
    unsigned int pos = 0, nbCards = 2;
    if (list == AI) {
        pos = 2;
    }
    else if (list == MIDDLE) {
        pos = 4;
        nbCards = 5;
    }

    unsigned int i = pos;
    while (i < m_selectedCards.size() && i < (pos + nbCards)
                && m_selectedCards.at(i) != -1) {
        i++;
    }

    if (i < m_selectedCards.size() && i < (pos + nbCards)) {
        m_selectedCards.at(i) = id;
        return true;
    }

    return false;
}

void CardsDialog::cardSelected(int id)
{
    QString style = "border: 5px solid ";
    CardsList list;

    if (m_playerChoice.isChecked()) {
        style += "blue";
        list = PLAYER;
    }
    else if (m_aiChoice.isChecked()) {
        style += "red";
        list = AI;
    }
    else if (m_middleChoice.isChecked()) {
        style += "rgb(100, 250, 100)";
        list = MIDDLE;
    }


    int rank = id % 13 + 1;
    int color = id / 13;

    QWidget *item = m_cardsLayout.itemAtPosition(color, rank)->widget();

    std::vector<int>::iterator it = std::find(m_selectedCards.begin(), m_selectedCards.end(), id);

    // Si on ne trouve pas la carte dans les cartes sélectionnées
    if (it == m_selectedCards.end()) {

        // On essaie de l'ajouter
        if (addCard(id, list)) {
            item->setStyleSheet(style);
        }
    }
    else {  // On supprime la carte de la liste
        *it = -1;
        item->setStyleSheet("");
    }
}
