#include <fstream>

#include "StatsCollector.h"

void StatsCollector::exportPnLToCSV(const std::string& filename) const {
	std::ofstream file(filename);
	if (!file.is_open()) return;

	file << "Index,PnL\n";

	for (size_t i = 0; i < pnlSeries.size(); i++) {
		file << i << "," << pnlSeries[i] << "\n";
	}

	file.close();
}

void StatsCollector::exportStatsToCSV(const std::string& filename, const StatsMap& metrics) const {
	std::ofstream file(filename);
	if (!file.is_open()) return;

	file << "Metric,Value\n";

	for (const auto& [name, value] : metrics) {
		file << name << "," << value << "\n";
	}

	file.close();
}

void StatsCollector::recordPnL(double pnl) {
	if (pnlSeries.empty()) initialPnL = pnl;

	if (!pnlSeries.empty()) {
		double prev = pnlSeries.back();
		returnsSeries.push_back((pnl - prev) / (std::abs(prev) + 1e-8));
	}
	pnlSeries.push_back(pnl);
}

void StatsCollector::addStat(std::string name, StatsFunction function) {
	if (statsFunction.count(name)) return;
	statsFunction[name] = std::move(function);
}

StatsMap StatsCollector::computeStats() {
	StatsMap results;
	if (pnlSeries.size() < 2) return results;

	for (const auto& [name, fn] : statsFunction) {
		results[name] = fn();
	}

	return results;
}

const double StatsCollector::getInitialPnL() const {
	return initialPnL;
}

const std::vector<double>& StatsCollector::getPnLSeries() const {
	return pnlSeries;
}

const std::vector<double>& StatsCollector::getReturnsSeries() const {
	return returnsSeries;
}
