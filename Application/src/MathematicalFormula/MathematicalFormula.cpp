#include "../../include/MathematicalFormula/MathematicalFormula.h"

namespace mathematicalformula {

    double MathematicalFormula::rate(const double value, const double referenceValue) {
        if (referenceValue == 0) {
            return -1;
        }
        else {
            return ((value * 100.0) / referenceValue);
        }
    }

    double MathematicalFormula::variation(const double value1, const double value2) {
        if (value1 == 0) {
            return -1;
        }
        else {
            return (value2 - value1) / value1 * 100.0;
        }
    }

    double MathematicalFormula::valuePercentage(const double percentage, const double value) {
        return (value * percentage) / 100.0;
    }

    double MathematicalFormula::calculateProportionalValue(const double minVal1, const double val1, const double maxVal1, const double minVal2, const double maxVal2) {
        return ((val1 - minVal1) * ((maxVal2 - minVal2) / (maxVal1 - minVal1)) + minVal2);
    }

}
