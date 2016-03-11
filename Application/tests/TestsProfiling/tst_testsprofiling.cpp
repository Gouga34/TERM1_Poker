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

QTEST_APPLESS_MAIN(TestsProfiling)

#include "tst_testsprofiling.moc"
