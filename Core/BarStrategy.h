#pragma once

#include <iostream>
#include <mutex>

#include "Strategy.h"
#include "BarAggregator.h"
#include "OrderManager.h"

extern std::mutex globalPrintMutex;

class BarStrategy : public Strategy {
private:
	BarAggregator aggregator;
	OrderManager* orderManager = nullptr;
public:
	BarStrategy(uint64_t windowSize = 60) : aggregator(windowSize) {}

	virtual ~BarStrategy() = default;

	virtual void onBar(const Bar& bar) = 0;

	void onTick(const Tick& tick) override {
		auto bar = aggregator.update(tick);
		if (bar.has_value()) onBar(bar.value());
	}
};