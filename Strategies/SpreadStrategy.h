#pragma once

#include <iostream>
#include <mutex>
#include "QuoteStrategy.h"
#include "OrderManager.h"

extern std::mutex globalPrintMutex;


class SpreadStrategy : public QuoteStrategy {
private:
	OrderManager* orderManager;
	double orderSize;
	double minSpread;
	double offset;
public:
	SpreadStrategy(double size = 1.0, double minSpread = 0.01, double offset = 0.005) : orderSize(size), minSpread(minSpread), offset(offset) {}

	void setOrderManager(OrderManager* om) override;

	void onTick(const QuoteTick& tick) override;
};