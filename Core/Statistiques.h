#pragma once

#include <cmath>
#include <numeric>

#include "TimeFrame.h"
#include "StatsCollector.h"

inline void registerUserStats(StatsCollector& collector, TimeFrame tf, double riskFreeRate = 0.0, double periodsPerYear = 252.0) {
	double nPeriods = getTicksPerDay(tf) * periodsPerYear;

	collector.addStat("MeanReturn", [&collector]() {
		const auto& ret = collector.getReturnsSeries();
		if (ret.empty()) return 0.0;
		return std::accumulate(ret.begin(), ret.end(), 0.0) / ret.size();
	});

	collector.addStat("TotalReturn", [&collector]() {
		const auto& pnl = collector.getPnLSeries();
		return (std::abs(pnl.front()) > 1e-8) ? pnl.back() / pnl.front() - 1.0 : 0.0;
	});

	collector.addStat("MaxDrawdown", [&collector]() {
		const auto& pnlSeries = collector.getPnLSeries();
		if (pnlSeries.empty()) return 0.0;

		double peak = pnlSeries[0];
		double maxDD = 0.0;

		for (double pnl : pnlSeries) {
			peak = std::max(peak, pnl);
			if (peak > 1e-8) maxDD = std::min(maxDD, (pnl - peak) / peak);
		}

		return maxDD;
	});

	collector.addStat("AnnualizedVolatility", [&collector, nPeriods]() {
		const auto& returns = collector.getReturnsSeries();
		if (returns.size() < 2) return 0.0;

		double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
		double var = 0.0;

		for (double ret : returns) var += (ret - mean) * (ret - mean);
		var /= returns.size();

		return std::sqrt(var) * std::sqrt(nPeriods);
	});

	collector.addStat("Sharpe", [&collector, nPeriods, &riskFreeRate, &periodsPerYear]() {
			const auto& returns = collector.getReturnsSeries();
			if (returns.size() < 2) return 0.0;

			double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
			double var = 0.0;

			for (double ret : returns) {
				var += (ret - mean) * (ret - mean);
			}

			var /= returns.size();

			return (mean - riskFreeRate) / std::sqrt(var + 1e-8) * std::sqrt(nPeriods);
	});

	collector.addStat("Sortino", [&collector, nPeriods, &riskFreeRate, &periodsPerYear]() {
		const auto& returns = collector.getReturnsSeries();
		double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
		double downsideVar = 0.0;
		int downsideCount = 0;
		for (double ret : returns) {
			if (ret < 0) {
				downsideVar += ret * ret;
				downsideCount++;
			}
		}

		if (returns.size() < 2 || downsideCount == 0) return 0.0;

		downsideVar /= downsideCount;

		return (mean - riskFreeRate) / std::sqrt(downsideVar + 1e-8) * std::sqrt(nPeriods);
	});
}