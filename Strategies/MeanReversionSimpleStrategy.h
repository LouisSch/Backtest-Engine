#pragma once

#include <iostream>
#include <mutex>
#include "Strategy.h"
#include "OrderManager.h"

extern std::mutex globalPrintMutex;

class MeanReversionSimple : public Strategy {
private:
	OrderManager* orderManager;
	double lastPrice = -1.0;
	bool inPosition = false;
	double entryPrice = 0.0;
public:
	MeanReversionSimple(OrderManager* om = nullptr) : orderManager(om) {}

	void setOrderManager(OrderManager* om) override;

	void onTick(const Tick& tick) override;
};