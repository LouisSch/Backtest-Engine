#include "QuoteGBMJumpGenerator.h"

QuoteGBMJumpGenerator::QuoteGBMJumpGenerator(size_t nTicks,
    TimeFrame tf,
    double startPrice,
    double mu,
    double impVol,
    double jumpLambda,
    double jumpMu,
    double jumpSigma,
    double spreadMu,
    double spreadSigma)
    : rng(std::random_device{}()),
    nTicks(nTicks),
    tf(tf),
    startPrice(startPrice),
    mu(mu),
    impVol(impVol),
    jumpLambda(jumpLambda),
    jumpMu(jumpMu),
    jumpSigma(jumpSigma),
    spreadMu(spreadMu),
    spreadSigma(spreadSigma) {
}

std::vector<QuoteTick> QuoteGBMJumpGenerator::generateTicks() {
    std::vector<QuoteTick> genTicks;
    genTicks.reserve(nTicks);

    std::normal_distribution<double> norm(0.0, 1.0);
    std::uniform_real_distribution<double> volGen(0.5, 1.5);
    std::uniform_real_distribution<double> jumpProb(0.0, 1.0);
    std::normal_distribution<double> jumpDist(jumpMu, jumpSigma);
    std::normal_distribution<double> spreadGen(spreadMu, spreadSigma);

    double dt = 1.0 / (252.0 * getTicksPerDay(tf));
    double currentPrice = startPrice;

    for (size_t i = 0; i < nTicks; ++i) {
        double Z = norm(rng);
        double dS = (mu - 0.5 * impVol * impVol) * dt + impVol * Z * std::sqrt(dt);

        double jumpFactor = 1.0;
        if (jumpProb(rng) < jumpLambda) {
            double jump = jumpDist(rng);
            jumpFactor = std::exp(jump);
        }

        currentPrice *= std::exp(dS) * jumpFactor;
        double volume = volGen(rng);

        // Modélisation du spread (tronqué à min 0.001 pour éviter bid > ask)
        double spread = std::max(0.001, spreadGen(rng));
        double bid = currentPrice - spread / 2.0;
        double ask = currentPrice + spread / 2.0;

        genTicks.push_back({ static_cast<uint64_t>(i), bid, ask, volume });
    }

    return genTicks;
}
