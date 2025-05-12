#pragma once

#include "OrderManager.h"
#include <cstdint>

class Strategy {
public:
	virtual ~Strategy() = default;
	virtual void setOrderManager(OrderManager* om) = 0;
	virtual void onStart() {}
	virtual void onEnd() {}
	virtual void onTick(const Tick& tick) = 0;
};