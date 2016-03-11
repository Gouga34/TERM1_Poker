#include <QString>
#include <QtTest>

#include "../../include/Profilage/CalculDonneesProfilage.h"

class TestsProfiling : public QObject
{
    Q_OBJECT

public:
    TestsProfiling();

private Q_SLOTS:
    void testTheoreticalBet();
    void testRationality();
    void testAggressiveness();
    void testBluff();
};

TestsProfiling::TestsProfiling()
{
}

void TestsProfiling::testTheoreticalBet()
{
    QCOMPARE(CalculDonneesProfilage::miseTheorique(15), 5.0);
    QCOMPARE(CalculDonneesProfilage::miseTheorique(31), 11.0);
    QCOMPARE(CalculDonneesProfilage::miseTheorique(51), 16.0);
    QCOMPARE(CalculDonneesProfilage::miseTheorique(70), 66.0);
    QCOMPARE(CalculDonneesProfilage::miseTheorique(100), 100.0);
}

void TestsProfiling::testRationality() {
    QCOMPARE(CalculDonneesProfilage::rationalite(15.0, 5.0), 100.0);
    QCOMPARE(CalculDonneesProfilage::rationalite(100.0, 0.0), 0.0);
    QCOMPARE(CalculDonneesProfilage::rationalite(51.0, 55.0), 61.0);
}

void TestsProfiling::testAggressiveness() {
    QCOMPARE(CalculDonneesProfilage::agressivite(100.0, 100.0, 100.0), 100.0);
    QCOMPARE(CalculDonneesProfilage::agressivite(0.0, 0.0, 0.0), 0.0);
    QCOMPARE(CalculDonneesProfilage::agressivite(1, 25.0, 25.0), 28.25);
    QCOMPARE(CalculDonneesProfilage::agressivite(1, 70.0, 50.0), 64.1);
    QCOMPARE(CalculDonneesProfilage::agressivite(1, 80.0, 85.0), 93.05);
}

void TestsProfiling::testBluff() {
    QCOMPARE(CalculDonneesProfilage::bluff(100.0), 0.0);
    QCOMPARE(CalculDonneesProfilage::bluff(50.0), 50.0);
    QCOMPARE(CalculDonneesProfilage::bluff(0.0), 100.0);
}

QTEST_APPLESS_MAIN(TestsProfiling)

#include "tst_testsprofiling.moc"
