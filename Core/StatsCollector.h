#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <cmath>
#include <numeric>

using StatsMap = std::unordered_map<std::string, double>;
using StatsFunction = std::function<double()>;

class StatsCollector {
private:
	double initialPnL;
	std::vector<double> pnlSeries;
	std::vector<double> returnsSeries;
	std::unordered_map<std::string, StatsFunction> statsFunction;
public:
	StatsCollector() : initialPnL(0.0) {}

	void exportPnLToCSV(const std::string& filename) const;

	void exportStatsToCSV(const std::string& filename, const StatsMap& metrics) const;

	void recordPnL(double pnl);

	void addStat(std::string name, StatsFunction function);

	StatsMap computeStats();

	const double getInitialPnL() const;

	const std::vector<double>& getPnLSeries() const;

	const std::vector<double>& getReturnsSeries() const;
};