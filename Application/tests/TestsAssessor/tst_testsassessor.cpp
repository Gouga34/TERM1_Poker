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

private Q_SLOTS:
    void compareHands();
    void royalFlush(); //Quinte Flush Royale
    void straightFlush(); //Quinte Flush
    void fourOfAKind(); //Carré
    void fullHouse(); //Full
    void flush(); //Couleur
    void straight(); //Suite
    void threeOfAKind(); //Brelan
    void twoPair(); //Double Paire
};

TestsAssessor::TestsAssessor() {

}

void TestsAssessor::compareHands() {
    std::vector<Carte> table {
        {RANG_CARTE::Q, COULEUR_CARTE::COEUR},
        {RANG_CARTE::J, COULEUR_CARTE::COEUR},
        {10, COULEUR_CARTE::COEUR},
        {9, COULEUR_CARTE::PIQUE},
        {3, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand1 {
        {RANG_CARTE::AS, COULEUR_CARTE::COEUR},
        {RANG_CARTE::K, COULEUR_CARTE::COEUR}
    };

    std::vector<Carte> hand2 {
        {10, COULEUR_CARTE::PIQUE},
        {10, COULEUR_CARTE::TREFLE}
    };

    QCOMPARE(Evaluateur::comparerMains(table, hand1, hand2), RESULTAT_PARTIE::GAGNE);
    QCOMPARE(Evaluateur::comparerMains(table, hand2, hand1), RESULTAT_PARTIE::PERDU);
    QCOMPARE(Evaluateur::comparerMains(table, hand1, hand1), RESULTAT_PARTIE::EGALITE);
}

void TestsAssessor::royalFlush() {
    std::vector<Carte> table {
        {RANG_CARTE::Q, COULEUR_CARTE::COEUR},
        {RANG_CARTE::J, COULEUR_CARTE::COEUR},
        {10, COULEUR_CARTE::COEUR},
        {9, COULEUR_CARTE::PIQUE},
        {3, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand1 {
        {RANG_CARTE::AS, COULEUR_CARTE::COEUR},
        {RANG_CARTE::K, COULEUR_CARTE::COEUR}
    };

    CartesJoueur cartes(table, hand1);
    QCOMPARE(cartes.getCombinaison(), FORCE_MAIN::QUINTE_FLUSH_ROYALE);
}

void TestsAssessor::straightFlush() {
    std::vector<Carte> table {
        {RANG_CARTE::Q, COULEUR_CARTE::COEUR},
        {RANG_CARTE::J, COULEUR_CARTE::COEUR},
        {10, COULEUR_CARTE::COEUR},
        {9, COULEUR_CARTE::PIQUE},
        {3, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand {
        {9, COULEUR_CARTE::COEUR},
        {RANG_CARTE::K, COULEUR_CARTE::COEUR}
    };

    CartesJoueur cartes(table, hand);
    QCOMPARE(cartes.getCombinaison(), FORCE_MAIN::QUINTE_FLUSH);
}

void TestsAssessor::fourOfAKind() {
    std::vector<Carte> table {
        {10, COULEUR_CARTE::CARREAU},
        {RANG_CARTE::J, COULEUR_CARTE::COEUR},
        {10, COULEUR_CARTE::COEUR},
        {9, COULEUR_CARTE::PIQUE},
        {3, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand {
        {10, COULEUR_CARTE::PIQUE},
        {10, COULEUR_CARTE::TREFLE}
    };

    CartesJoueur cartes(table, hand);
    QCOMPARE(cartes.getCombinaison(), FORCE_MAIN::CARRE);
}

void TestsAssessor::fullHouse() {
    std::vector<Carte> table {
        {10, COULEUR_CARTE::CARREAU},
        {RANG_CARTE::J, COULEUR_CARTE::COEUR},
        {10, COULEUR_CARTE::COEUR},
        {9, COULEUR_CARTE::PIQUE},
        {3, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand {
        {10, COULEUR_CARTE::PIQUE},
        {3, COULEUR_CARTE::PIQUE}
    };

    CartesJoueur cartes(table, hand);
    QCOMPARE(cartes.getCombinaison(), FORCE_MAIN::FULL);
}

void TestsAssessor::flush() {
    std::vector<Carte> table {
        {RANG_CARTE::Q, COULEUR_CARTE::COEUR},
        {RANG_CARTE::J, COULEUR_CARTE::COEUR},
        {10, COULEUR_CARTE::COEUR},
        {9, COULEUR_CARTE::PIQUE},
        {3, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand {
        {2, COULEUR_CARTE::COEUR},
        {4, COULEUR_CARTE::COEUR}
    };

    CartesJoueur cartes(table, hand);
    QCOMPARE(cartes.getCombinaison(), FORCE_MAIN::COULEUR);
}

void TestsAssessor::straight() {
    std::vector<Carte> table {
        {10, COULEUR_CARTE::CARREAU},
        {RANG_CARTE::J, COULEUR_CARTE::COEUR},
        {10, COULEUR_CARTE::COEUR},
        {9, COULEUR_CARTE::PIQUE},
        {3, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand {
        {8, COULEUR_CARTE::PIQUE},
        {7, COULEUR_CARTE::PIQUE}
    };

    CartesJoueur cartes(table, hand);
    QCOMPARE(cartes.getCombinaison(), FORCE_MAIN::SUITE);
}

void TestsAssessor::threeOfAKind() {
    std::vector<Carte> table {
        {10, COULEUR_CARTE::CARREAU},
        {RANG_CARTE::J, COULEUR_CARTE::COEUR},
        {10, COULEUR_CARTE::COEUR},
        {9, COULEUR_CARTE::PIQUE},
        {3, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand {
        {10, COULEUR_CARTE::PIQUE},
        {7, COULEUR_CARTE::PIQUE}
    };

    CartesJoueur cartes(table, hand);
    QCOMPARE(cartes.getCombinaison(), FORCE_MAIN::BRELAN);
}

void TestsAssessor::twoPair() {
    std::vector<Carte> table {
        {10, COULEUR_CARTE::CARREAU},
        {RANG_CARTE::J, COULEUR_CARTE::COEUR},
        {10, COULEUR_CARTE::COEUR},
        {9, COULEUR_CARTE::PIQUE},
        {3, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand {
        {3, COULEUR_CARTE::PIQUE},
        {7, COULEUR_CARTE::PIQUE}
    };

    CartesJoueur cartes(table, hand);
    QCOMPARE(cartes.getCombinaison(), FORCE_MAIN::DOUBLE_PAIRE);
}

QTEST_APPLESS_MAIN(TestsAssessor)
#include "tst_testsassessor.moc"
