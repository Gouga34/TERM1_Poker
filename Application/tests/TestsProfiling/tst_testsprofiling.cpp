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

QTEST_APPLESS_MAIN(TestsProfiling)

#include "tst_testsprofiling.moc"
