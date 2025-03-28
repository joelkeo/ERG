/*
  ==============================================================================

    PolynomialPhaseSignal.cpp
    Created: 2 Feb 2025 3:38:23pm
    Author:  Joel Keohane

  ==============================================================================
*/

#include "PolynomialPhaseSignal.h"
PolynomialPhaseSignal::PolynomialPhaseSignal(MyParameter& lengthParameter,
                                             MyParameter& endRateParameter,
                                             MyParameter& zParameter,
                                             MyParameter& powerParameter,
                                            HostInfo& hostInfo) :
VRPS(lengthParameter, endRateParameter, zParameter, powerParameter, hostInfo) {
    formulaChanged();
}

// RETURNS BUFFER IT WAS GIVEN
double* PolynomialPhaseSignal::getSignal(double* buffer,
                                         int formulaStartPosition,
                                         int bufferStartPosition,
                                         int bufferLength) {
    double r = getEndRate();
    double l = getLength();
    double z = getZ();
    double p = powerParameter.getAdjustedValue();
    double a = (r * l - z) / - (std::pow(l, p));
    double term2 = a * std::pow(std::abs(l), p);
    // FIRST CASE IS SPECIAL:
    double x = formulaStartPosition;
    double term1 = - a * std::pow(std::abs(x - l), p);
    double term3 = r * x;
    double unModdedFirst = term1 + term2 + term3;
    // handle modulo and slightly negatives (rounding error)
    buffer[bufferStartPosition] = std::max(0., std::fmod(unModdedFirst, 1.));
    for (int i = 1; i < bufferLength; i++) {
        double x = i + formulaStartPosition;
        double term1 = - a * std::pow(std::abs(x - l), p);
        double term3 = r * x;
        double unModded = term1 + term2 + term3;
        // handle modulo and slightly negatives (rounding error)
        buffer[bufferStartPosition + i] = std::max(0., std::fmod(unModded, 1.));
    }
    // updates listeners
    positionUpdated(((double)formulaStartPosition) / l, std::floor(std::max(0., unModdedFirst)), buffer[0]);
    return buffer;
}

std::function<double(double)> PolynomialPhaseSignal::getPhaseFormula() {
    double r = getEndRateBeats();
    double l = getLengthBeats();
    double z = getZ();
    double p = powerParameter.getAdjustedValue();
    double a = (r * l - z) / - (std::pow(l, p));
    double term2 = a * std::pow(std::abs(l), p);
    return [a, r, term2, l, p](double x) {
        double term1 = - a * std::pow(std::abs(x - l), p);
        double term3 = r * x;
        double unModded = term1 + term2 + term3;
        return unModded;
    };
}
std::function<double(double)> PolynomialPhaseSignal::getRateFormula() {
    double r = getEndRateBeats();
    double l = getLengthBeats();
    double z = getZ();
    double p = powerParameter.getAdjustedValue();
    double a = (r * l - z) / - (std::pow(l, p));
    return [a, r, l, p](double x) {
        return a * p * std::pow(l-x ,p-1) + r;
    };
}
