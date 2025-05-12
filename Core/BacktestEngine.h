#pragma once

#include <vector>
#include <functional>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>

#include "Strategy.h"
#include "QuoteStrategy.h"
#include "TimeFrame.h"
#include "OrderManager.h"
#include "StatsCollector.h"

struct StrategyContext {
	std::string name;
	std::unique_ptr<Strategy> strategy;
	TimeFrame tf;
	OrderManager orderManager;
	StatsCollector statistics;
	std::thread worker;

	StrategyContext(const std::string& name, std::unique_ptr<Strategy> strategy, const TimeFrame& tf, const double initialCash);
};

extern std::mutex globalPrintMutex;

class BacktestEngine {
private:
	std::vector<Tick> data;
	std::vector<QuoteTick> quoteData;
	std::vector<std::unique_ptr<StrategyContext>> strategies;
public:
	void setTickData(std::vector<Tick>& ticks);

	void setTickData(std::vector<Tick>&& ticks);

	void setTickData(std::vector<QuoteTick>& quoteTicks);

	void setTickData(std::vector<QuoteTick>&& quoteTicks);

	void addStrategy(const std::string& name, std::unique_ptr<Strategy> strategy, const TimeFrame& tf, double initialCash);

	void runAll(const bool saveToCSV = false, const bool verbose = false);
};