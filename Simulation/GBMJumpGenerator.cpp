#include "GBMJumpGenerator.h"

GBMJumpGenerator::GBMJumpGenerator(size_t nTicks,
    TimeFrame tf,
    double startPrice,
    double mu,
    double impVol,
    double jumpLambda,
    double jumpMu,
    double jumpSigma) : rng(std::random_device{}()),
                               nTicks(nTicks),
                               tf(tf),
                               startPrice(startPrice),
                               mu(mu),
                               impVol(impVol),
                               jumpLambda(jumpLambda),
                               jumpMu(jumpMu),
                               jumpSigma(jumpSigma) {}

std::vector<Tick> GBMJumpGenerator::generateTicks() {
    std::vector<Tick> genTicks;
    genTicks.reserve(nTicks);

    std::normal_distribution<double> norm(0.0, 1.0);
    std::uniform_real_distribution<double> volGen(0.5, 1.5);

    std::uniform_real_distribution<double> jumpProb(0.0, 1.0);
    std::normal_distribution<double> jumpDist(jumpMu, jumpSigma);

    double dt = 1.0 / (252.0 * getTicksPerDay(tf));
    double currentPrice = startPrice;
    double jumpFactor = 1.0;

    for (size_t i = 0; i < nTicks; i++) {
        double Z = norm(rng);
        double dS = (mu - 0.5 * impVol * impVol) * dt + impVol * Z * std::sqrt(dt);
        jumpFactor = 1.0;

        double newVolume = volGen(rng);

        if (jumpProb(rng) < jumpLambda) {
            double jump = jumpDist(rng);
            jumpFactor = std::exp(jump);
        }

        currentPrice *= std::exp(dS) * jumpFactor;

        genTicks.push_back({ static_cast<uint64_t>(i), currentPrice, newVolume });
    }

    return genTicks;
}
