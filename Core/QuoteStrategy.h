#pragma once
#include <cstdint>

#include "Strategy.h"
#include "OrderManager.h"

class QuoteStrategy : public Strategy {
public:
	virtual ~QuoteStrategy() = default;
	virtual void setOrderManager(OrderManager* om) = 0;
	virtual void onStart() {}
	virtual void onEnd() {}

	void onTick(const Tick&) override {} // Dummy override
	virtual void onTick(const QuoteTick& tick) = 0;
};