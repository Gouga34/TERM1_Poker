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
    QCOMPARE(mathematicalformula::MathematicalFormula::rate(20, 180), valueToVerify);
    QCOMPARE(mathematicalformula::MathematicalFormula::rate(1.0, 0.0), -1.0);
}

void TestsMathematicalFormula::testVariation() {
    double valueToVerify = (100.0 - 50.0) / 50.0 * 100.0;
    QCOMPARE(mathematicalformula::MathematicalFormula::variation(50.0, 100.0), valueToVerify);
    QCOMPARE(mathematicalformula::MathematicalFormula::variation(0.0, 50.0), -1.0);
}

void TestsMathematicalFormula::testValuePercentage() {
    double valueToVerify = (20.0 * 550.0) / 100.0;
    QCOMPARE(mathematicalformula::MathematicalFormula::valuePercentage(20.0, 550.0), valueToVerify);
}

void TestsMathematicalFormula::testCalculateProportionalValue() {
    double valueToVerify = (15.0 - 10.0) * ((25.0 - 15.0) / (20.0 - 10.0)) + 15.0;
    QCOMPARE(mathematicalformula::MathematicalFormula::calculateProportionalValue(10.0, 15.0, 20.0, 15.0, 25.0), valueToVerify);
}

QTEST_APPLESS_MAIN(TestsMathematicalFormula)

#include "tst_testsmathematicalformula.moc"
