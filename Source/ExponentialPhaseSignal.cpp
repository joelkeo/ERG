/*
  ==============================================================================

    ExponentialPhaseSignal.cpp
    Created: 30 Jan 2025 5:10:24pm
    Author:  Joel Keohane

  ==============================================================================
*/

#include "ExponentialPhaseSignal.h"
ExponentialPhaseSignal::ExponentialPhaseSignal(MyParameter& lengthParameter,
                                               MyParameter& endRateParameter,
                                               MyParameter& zParameter,
                                               MyParameter& powerParameter,
                                               HostInfo& hostInfo) :
VRPS(lengthParameter, endRateParameter, zParameter, powerParameter, hostInfo) {}

double* ExponentialPhaseSignal::getSignal(double* buffer,
                                          int formulaStartPosition,
                                          int bufferStartPosition,
                                          int bufferLength) {
    double s = getS();
    double c = getC();
    for (int i = 0; i < bufferLength; i++) {
        double nonNormalized = s * log(c * ((double) (i + formulaStartPosition)) + 1);
        buffer[i + bufferStartPosition] = fmod(nonNormalized, 1.);
    }
    return buffer;
}

double ExponentialPhaseSignal::getS() {
    double r = getEndRate();
    double l = getLength();
    double c = getC();
    return r * l + r / c;
}
double ExponentialPhaseSignal::getC() {
    return cachedC;
}
double ExponentialPhaseSignal::getLength() {
    double bpm = 120.;
    double sampleRate = 44100.;
    return (lengthParameter.get() / (bpm / 60.)) * sampleRate;
}
double ExponentialPhaseSignal::getEndRate() {
    double bpm = 120.;
    double sampleRate = 44100.;
    double result = (endRateParameter.get() / (bpm / 60.)) / sampleRate;
    return result;
}
int ExponentialPhaseSignal::getZ() {
    return zParameter.get();
}
// nasty C solving
double ExponentialPhaseSignal::solveC(int targetZ, double endRate, double length) {
    std::vector<double> bounds = findBounds(targetZ, endRate, length);
    return binarySearch(targetZ, bounds[0], bounds[1], endRate, length);
}
double ExponentialPhaseSignal::binarySearch(int targetZ, double lowerBound, double upperBound, double endRate, double length) {
    double error = .0000001;
    int iterations = 0;
    while (iterations++ < 50) {
        double mid = lowerBound + (upperBound - lowerBound) / 2.;
        double evaluated = solveZ(mid, endRate, length);
        DBG("targetZ: ");
        DBG(targetZ);
        double difference = evaluated - targetZ;
        if (abs(difference) < error) {
            return mid;
        }
        else if (evaluated > targetZ) {
            upperBound = mid;
        }
        else {
            lowerBound = mid;
        }
    }
    jassertfalse;
    return 1.;
}
std::vector<double> ExponentialPhaseSignal::findBounds(int targetZ, double endRate, double length) {
    // TODO: yeah, just no
    double lowerBound = .00000001;
    if (inBounds(targetZ, lowerBound, 1., endRate, length))  {
        return {lowerBound, 1.};
    }
    lowerBound = 1.;
    int iterations = 0;
    // avoiding infinite loop, in case of bbugs
    while (iterations++ < 1000) {
        if (inBounds(targetZ, lowerBound, lowerBound * 2., endRate, length)) {
            return  {lowerBound, lowerBound * 2.};
        }
        lowerBound *= 2.;
    }
    jassertfalse;
    return {};
}

bool ExponentialPhaseSignal::inBounds(int targetZ, double bottom, double top, double endRate, double length) {
    return solveZ(bottom, endRate, length) <= targetZ && solveZ(top, endRate, length) >= targetZ;
}

double ExponentialPhaseSignal::solveZ(double c, double endRate, double length) {
    DBG("solving z");
    DBG("c: ");
    DBG(c);
    DBG("endRate: ");
    DBG(endRate);
    DBG("length: ");
    DBG(length);
    double result = log(c * length + 1.) * (endRate * (c * length * + 1.)) / c;
    DBG("result: ");
    DBG(result);
    return result;
}

void ExponentialPhaseSignal::compute() {
    cachedC = solveC(getZ(), getEndRate(), getLength());
}
