#include <QString>
#include <QtTest>

#include "../../include/Profilage/CalculateProfilingData.h"

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
    void testPassivity();
};

TestsProfiling::TestsProfiling()
{
}

void TestsProfiling::testTheoreticalBet()
{
    QCOMPARE(CalculateProfilingData::theoreticalBet(15), 5.0);
    QCOMPARE(CalculateProfilingData::theoreticalBet(31), 11.0);
    QCOMPARE(CalculateProfilingData::theoreticalBet(51), 16.0);
    QCOMPARE(CalculateProfilingData::theoreticalBet(70), 66.0);
    QCOMPARE(CalculateProfilingData::theoreticalBet(100), 100.0);
}

void TestsProfiling::testRationality() {
    QCOMPARE(CalculateProfilingData::rationality(15.0, 5.0), 100.0);
    QCOMPARE(CalculateProfilingData::rationality(100.0, 0.0), 0.0);
    QCOMPARE(CalculateProfilingData::rationality(51.0, 55.0), 61.0);
}

void TestsProfiling::testAggressiveness() {
    QCOMPARE(CalculateProfilingData::aggressiveness(100.0, 100.0, 100.0), 100.0);
    QCOMPARE(CalculateProfilingData::aggressiveness(0.0, 0.0, 0.0), 0.0);
    QCOMPARE(CalculateProfilingData::aggressiveness(1, 25.0, 25.0), 28.25);
    QCOMPARE(CalculateProfilingData::aggressiveness(1, 70.0, 50.0), 64.1);
    QCOMPARE(CalculateProfilingData::aggressiveness(1, 80.0, 85.0), 93.05);
}

void TestsProfiling::testBluff() {
    QCOMPARE(CalculateProfilingData::bluff(100.0), 0.0);
    QCOMPARE(CalculateProfilingData::bluff(50.0), 50.0);
    QCOMPARE(CalculateProfilingData::bluff(0.0), 100.0);
}

void TestsProfiling::testPassivity() {
    QCOMPARE(CalculateProfilingData::passivity(50.0, 50.0), 100.0);
    QCOMPARE(CalculateProfilingData::passivity(0.0, 100.0), 100.0);
    QCOMPARE(CalculateProfilingData::passivity(100.0, 0.0), 100.0);
    QCOMPARE(CalculateProfilingData::passivity(10.0, 0.0), 10.0);
    QCOMPARE(CalculateProfilingData::passivity(0.0,0.0), 0.0);
}

QTEST_APPLESS_MAIN(TestsProfiling)

#include "tst_testsprofiling.moc"
