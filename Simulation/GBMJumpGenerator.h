#pragma once

#include <random>

#include "Tick.h"
#include "TimeFrame.h"

class GBMJumpGenerator {
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
public:
	GBMJumpGenerator(size_t nTicks,
		TimeFrame tf,
		double startPrice = 100.0,
		double mu = 0.03,
		double impVol = 0.2,
		double jumpLambda = 0.01,
		double jumpMu = -0.01,
		double jumpSigma = 0.03);

	std::vector<Tick> generateTicks();
};