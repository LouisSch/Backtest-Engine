#pragma once

#include <cstdint>
#include <vector>

#include "Tick.h"

enum class OrderType { MARKET, LIMIT };

struct Order {
	enum class Side { BUY, SELL };
	Side side;
	OrderType type;
	uint64_t timestamp;
	double volume;
	double price;
};

class OrderManager {
private:
	std::vector<Order> pendingOrders;
	double position = 0.0;
	double cash;
public:
	OrderManager(int cash) : cash(cash) {}

	void submit(Order& order);

	void execute(Order& order);

	void handleTick(const Tick& tick);

	void handleTick(const QuoteTick& quote);

	double getPnL(double lastPrice) const;

	double getPosition() const;
};