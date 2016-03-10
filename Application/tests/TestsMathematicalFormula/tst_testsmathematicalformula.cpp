#include <QString>
#include <QtTest>

#include "../../include/MathematicalFormula/MathematicalFormula.h"

class TestsMathematicalFormula : public QObject
{
    Q_OBJECT

public:
    TestsMathematicalFormula();

private Q_SLOTS:
    void testRate();
    void testVariation();
    void testValuePercentage();
    void testCalculateProportionalValue();
};

TestsMathematicalFormula::TestsMathematicalFormula()
{
}

void TestsMathematicalFormula::testRate()
{
    double valueToVerify = (20.0 * 100.0) / 180;
    QCOMPARE(valueToVerify, MathematicalFormula::rate(20, 180));
    QCOMPARE(-1.0, MathematicalFormula::rate(1.0, 0.0));
}

void TestsMathematicalFormula::testVariation() {
    double valueToVerify = (100.0 - 50.0) / 50.0 * 100.0;
    QCOMPARE(valueToVerify, MathematicalFormula::variation(50.0, 100.0));
    QCOMPARE(-1.0, MathematicalFormula::variation(0.0, 50.0));
}

void TestsMathematicalFormula::testValuePercentage() {
    double valueToVerify = (20.0 * 550.0) / 100.0;
    QCOMPARE(valueToVerify, MathematicalFormula::valuePercentage(20.0, 550.0));
}

void TestsMathematicalFormula::testCalculateProportionalValue() {
    double valueToVerify = (15.0 - 10.0) * ((25.0 - 15.0) / (20.0 - 10.0)) + 15.0;
    QCOMPARE(valueToVerify, MathematicalFormula::calculateProportionalValue(10.0, 15.0, 20.0, 15.0, 25.0));
}

QTEST_APPLESS_MAIN(TestsMathematicalFormula)

#include "tst_testsmathematicalformula.moc"
