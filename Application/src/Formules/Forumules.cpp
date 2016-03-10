#include "../../include/Formules/Formules.h"


double Formules::rate(const double value, const double referenceValue) {
    if (referenceValue == 0) {
        return -1;
    }
    else {
        return ((value * 100.0) / referenceValue);
    }
}

double Formules::variation(const double value1, const double value2) {
    if (value1 == 0) {
        return -1;
    }
    else {
        return (value2 - value1) / value1 * 100.0;
    }
}

double Formules::valuePercentage(const double percentage, const double value) {
    return (value * percentage) / 100.0;
}

double Formules::calculateProportionalValue(const double minVal1, const double val1, const double maxVal1, const double minVal2, const double maxVal2) {
    return ((val1 - minVal1) * ((maxVal2 - minVal2) / (maxVal1 - minVal1)) + minVal2);
}

