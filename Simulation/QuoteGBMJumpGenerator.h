#pragma once

#include <random>
#include <vector>
#include "TimeFrame.h"
#include "Tick.h"

class QuoteGBMJumpGenerator {
private:
    std::mt19937 rng;

    size_t nTicks;
    TimeFrame tf;
    double startPrice;
    double mu;
    double impVol;
    double jumpLambda;
    double jumpMu;
    double jumpSigma;

    double spreadMu;
    double spreadSigma;
public:
    QuoteGBMJumpGenerator(size_t nTicks,
        TimeFrame tf,
        double startPrice = 100.0,
        double mu = 0.03,
        double impVol = 0.2,
        double jumpLambda = 0.01,
        double jumpMu = -0.01,
        double jumpSigma = 0.03,
        double spreadMu = 0.01,
        double spreadSigma = 0.002);

    std::vector<QuoteTick> generateTicks();
};
