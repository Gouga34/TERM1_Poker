#include <QString>
#include <QtTest>
#include <iostream>

#include "../../include/Assessor/Assessor.h"
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
    void pair(); //Pair
    void hightCard(); //Carte haute
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
        {9, COULEUR_CARTE::CARREAU},
        {10, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand3 {
        {9, COULEUR_CARTE::COEUR},
        {10, COULEUR_CARTE::CARREAU}
    };

    QCOMPARE(Assessor::compareHands(table, hand1, hand2), RESULTAT_PARTIE::GAGNE);
    QCOMPARE(Assessor::compareHands(table, hand2, hand1), RESULTAT_PARTIE::PERDU);
    QCOMPARE(Assessor::compareHands(table, hand2, hand3), RESULTAT_PARTIE::EGALITE);
}

void TestsAssessor::royalFlush() {
    std::vector<Carte> table {
        {RANG_CARTE::Q, COULEUR_CARTE::COEUR},
        {RANG_CARTE::J, COULEUR_CARTE::COEUR},
        {10, COULEUR_CARTE::COEUR},
        {9, COULEUR_CARTE::PIQUE},
        {3, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand {
        {RANG_CARTE::AS, COULEUR_CARTE::COEUR},
        {RANG_CARTE::K, COULEUR_CARTE::COEUR}
    };

    PlayerCards cartes(table, hand);
    QCOMPARE(cartes.getHandCategory(), FORCE_MAIN::QUINTE_FLUSH_ROYALE);
}

void TestsAssessor::straightFlush() {
    std::vector<Carte> table {
        {5, COULEUR_CARTE::COEUR},
        {6, COULEUR_CARTE::COEUR},
        {7, COULEUR_CARTE::COEUR},
        {9, COULEUR_CARTE::PIQUE},
        {3, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand {
        {4, COULEUR_CARTE::COEUR},
        {3, COULEUR_CARTE::COEUR}
    };

    std::vector<Carte> hand2 {
        {9, COULEUR_CARTE::COEUR},
        {8, COULEUR_CARTE::COEUR}
    };

    PlayerCards cartes(table, hand);
    QCOMPARE(cartes.getHandCategory(), FORCE_MAIN::QUINTE_FLUSH);
    QCOMPARE(Assessor::compareHands(table, hand2, hand), RESULTAT_PARTIE::GAGNE);
    QCOMPARE(Assessor::compareHands(table, hand, hand2), RESULTAT_PARTIE::PERDU);
}

void TestsAssessor::fourOfAKind() {
    std::vector<Carte> table {
        {10, COULEUR_CARTE::CARREAU},
        {10, COULEUR_CARTE::PIQUE},
        {10, COULEUR_CARTE::COEUR},
        {10, COULEUR_CARTE::TREFLE},
        {3, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand {
        {5, COULEUR_CARTE::CARREAU},
        {9, COULEUR_CARTE::COEUR}
    };

    std::vector<Carte> hand2 {
        {5, COULEUR_CARTE::PIQUE},
        {9, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand3 {
        {7, COULEUR_CARTE::PIQUE},
        {RANG_CARTE::K, COULEUR_CARTE::TREFLE}
    };

    PlayerCards cartes(table, hand);
    QCOMPARE(cartes.getHandCategory(), FORCE_MAIN::CARRE);
    QCOMPARE(Assessor::compareHands(table, hand, hand2), RESULTAT_PARTIE::EGALITE);
    QCOMPARE(Assessor::compareHands(table, hand3, hand2), RESULTAT_PARTIE::GAGNE);
    QCOMPARE(Assessor::compareHands(table, hand2, hand3), RESULTAT_PARTIE::PERDU);
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

    std::vector<Carte> hand2 {
        {10, COULEUR_CARTE::TREFLE},
        {3, COULEUR_CARTE::CARREAU}
    };

    std::vector<Carte> hand3 {
        {10, COULEUR_CARTE::TREFLE},
        {9, COULEUR_CARTE::CARREAU}
    };

    PlayerCards cartes(table, hand);
    QCOMPARE(cartes.getHandCategory(), FORCE_MAIN::FULL);
    QCOMPARE(Assessor::compareHands(table, hand, hand2), RESULTAT_PARTIE::EGALITE);
    QCOMPARE(Assessor::compareHands(table, hand3, hand2), RESULTAT_PARTIE::GAGNE);
    QCOMPARE(Assessor::compareHands(table, hand2, hand3), RESULTAT_PARTIE::PERDU);
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

    std::vector<Carte> hand2 {
        {3, COULEUR_CARTE::COEUR},
        {5, COULEUR_CARTE::COEUR}
    };

    PlayerCards cartes(table, hand);
    QCOMPARE(cartes.getHandCategory(), FORCE_MAIN::COULEUR);
    QCOMPARE(Assessor::compareHands(table, hand, hand2), RESULTAT_PARTIE::PERDU);
    QCOMPARE(Assessor::compareHands(table, hand2, hand), RESULTAT_PARTIE::GAGNE);
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

    std::vector<Carte> hand2 {
        {8, COULEUR_CARTE::TREFLE},
        {7, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand3 {
        {8, COULEUR_CARTE::TREFLE},
        {RANG_CARTE::Q, COULEUR_CARTE::TREFLE}
    };

    PlayerCards cartes(table, hand);
    QCOMPARE(cartes.getHandCategory(), FORCE_MAIN::SUITE);
    QCOMPARE(Assessor::compareHands(table, hand, hand2), RESULTAT_PARTIE::EGALITE);
    QCOMPARE(Assessor::compareHands(table, hand3, hand2), RESULTAT_PARTIE::GAGNE);
    QCOMPARE(Assessor::compareHands(table, hand2, hand3), RESULTAT_PARTIE::PERDU);
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

    std::vector<Carte> hand2 {
        {10, COULEUR_CARTE::TREFLE},
        {7, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand3 {
        {10, COULEUR_CARTE::TREFLE},
        {9, COULEUR_CARTE::TREFLE}
    };

    PlayerCards cartes(table, hand);
    QCOMPARE(cartes.getHandCategory(), FORCE_MAIN::BRELAN);
    QCOMPARE(Assessor::compareHands(table, hand, hand2), RESULTAT_PARTIE::EGALITE);
    QCOMPARE(Assessor::compareHands(table, hand3, hand2), RESULTAT_PARTIE::GAGNE);
    QCOMPARE(Assessor::compareHands(table, hand2, hand3), RESULTAT_PARTIE::PERDU);
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

    std::vector<Carte> hand1 {
        {3, COULEUR_CARTE::CARREAU},
        {7, COULEUR_CARTE::PIQUE}
    };

    std::vector<Carte> hand2 {
        {9, COULEUR_CARTE::CARREAU},
        {7, COULEUR_CARTE::PIQUE}
    };

    PlayerCards cartes(table, hand);
    QCOMPARE(cartes.getHandCategory(), FORCE_MAIN::DOUBLE_PAIRE);
    QCOMPARE(Assessor::compareHands(table, hand, hand1), RESULTAT_PARTIE::EGALITE);
    QCOMPARE(Assessor::compareHands(table, hand2, hand), RESULTAT_PARTIE::GAGNE);
    QCOMPARE(Assessor::compareHands(table, hand, hand2), RESULTAT_PARTIE::PERDU);

}

void TestsAssessor::pair() {
    std::vector<Carte> table {
        {10, COULEUR_CARTE::CARREAU},
        {RANG_CARTE::J, COULEUR_CARTE::COEUR},
        {10, COULEUR_CARTE::COEUR},
        {9, COULEUR_CARTE::PIQUE},
        {3, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand {
        {5, COULEUR_CARTE::PIQUE},
        {7, COULEUR_CARTE::PIQUE}
    };

    std::vector<Carte> hand2 {
        {5, COULEUR_CARTE::TREFLE},
        {7, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand3 {
        {RANG_CARTE::K, COULEUR_CARTE::TREFLE},
        {7, COULEUR_CARTE::TREFLE}
    };

    PlayerCards cartes(table, hand);
    QCOMPARE(cartes.getHandCategory(), FORCE_MAIN::PAIRE);
    QCOMPARE(Assessor::compareHands(table, hand, hand2), RESULTAT_PARTIE::EGALITE);
    QCOMPARE(Assessor::compareHands(table, hand3, hand2), RESULTAT_PARTIE::GAGNE);
    QCOMPARE(Assessor::compareHands(table, hand2, hand3), RESULTAT_PARTIE::PERDU);
}

void TestsAssessor::hightCard() {
    std::vector<Carte> table {
        {10, COULEUR_CARTE::CARREAU},
        {RANG_CARTE::J, COULEUR_CARTE::COEUR},
        {RANG_CARTE::AS, COULEUR_CARTE::COEUR},
        {9, COULEUR_CARTE::PIQUE},
        {3, COULEUR_CARTE::TREFLE}
    };

    std::vector<Carte> hand {
        {5, COULEUR_CARTE::PIQUE},
        {7, COULEUR_CARTE::PIQUE}
    };

    std::vector<Carte> hand2 {
        {5, COULEUR_CARTE::CARREAU},
        {7, COULEUR_CARTE::CARREAU}
    };

    std::vector<Carte> hand3 {
        {5, COULEUR_CARTE::CARREAU},
        {4, COULEUR_CARTE::CARREAU}
    };


    PlayerCards cartes(table, hand);
    QCOMPARE(cartes.getHandCategory(), FORCE_MAIN::CARTE_HAUTE);
    QCOMPARE(Assessor::compareHands(table, hand, hand2), RESULTAT_PARTIE::EGALITE);
    QCOMPARE(Assessor::compareHands(table, hand2, hand3), RESULTAT_PARTIE::GAGNE);
    QCOMPARE(Assessor::compareHands(table, hand3, hand2), RESULTAT_PARTIE::PERDU);
}

QTEST_APPLESS_MAIN(TestsAssessor)
#include "tst_testsassessor.moc"
