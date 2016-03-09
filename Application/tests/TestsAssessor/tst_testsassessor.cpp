#include <QString>
#include <QtTest>
#include <iostream>

#include "../../include/Evaluateur/Evaluateur.h"
#include "../../include/Jeu/Carte.h"

class TestsAssessor : public QObject
{
    Q_OBJECT

public:
    TestsAssessor();

private:
    virtual std::vector<Carte> getTable();
    virtual std::vector<Carte> getHand1();
    virtual std::vector<Carte> getHand2();

private Q_SLOTS:
    void results();
};

TestsAssessor::TestsAssessor() {

}

std::vector<Carte> TestsAssessor::getTable() {
    std::vector<Carte> table {
        {RANG_CARTE::Q, COULEUR_CARTE::COEUR},
        {RANG_CARTE::J, COULEUR_CARTE::COEUR},
        {10, COULEUR_CARTE::COEUR},
        {9, COULEUR_CARTE::PIQUE},
        {3, COULEUR_CARTE::TREFLE}
    };
    return table;
}

std::vector<Carte> TestsAssessor::getHand1() {
    std::vector<Carte> hand1 {
        {RANG_CARTE::AS, COULEUR_CARTE::COEUR},
        {RANG_CARTE::K, COULEUR_CARTE::COEUR}
    };
    return hand1;
}

std::vector<Carte> TestsAssessor::getHand2() {
    std::vector<Carte> hand2 {
        {10, COULEUR_CARTE::PIQUE},
        {10, COULEUR_CARTE::TREFLE}
    };
    return hand2;
}

void TestsAssessor::results() {
    std::vector<Carte> table = getTable();
    std::vector<Carte> hand1 = getHand1();

    std::vector<Carte> hand2 = getHand2();

    QCOMPARE(RESULTAT_PARTIE::GAGNE, Evaluateur::comparerMains(table, hand1, hand2));
    QCOMPARE(RESULTAT_PARTIE::PERDU, Evaluateur::comparerMains(table, hand2, hand1));
    QCOMPARE(RESULTAT_PARTIE::EGALITE, Evaluateur::comparerMains(table, hand1, hand1));
}

QTEST_APPLESS_MAIN(TestsAssessor)
#include "tst_testsassessor.moc"
