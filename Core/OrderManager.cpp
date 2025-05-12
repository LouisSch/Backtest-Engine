#include "OrderManager.h"

void OrderManager::submit(Order& order) {
	if (order.type == OrderType::MARKET) execute(order);
	else pendingOrders.push_back(order);
}

void OrderManager::handleTick(const Tick& tick) {
	std::vector<Order> stillPending;

	for (auto& order : pendingOrders) {
		bool executed = false;

		if (order.side == Order::Side::BUY && tick.price <= order.price) {
			execute(order);
			executed = true;
		}
		else if (order.side == Order::Side::SELL && tick.price >= order.price) {
			execute(order);
			executed = true;
		}

		if (!executed) stillPending.push_back(order);
	}

	pendingOrders = std::move(stillPending);
}

void OrderManager::handleTick(const QuoteTick& quote) {
	std::vector<Order> stillPending;
	stillPending.reserve(pendingOrders.size());

	for (auto& order : pendingOrders) {
		bool executed = false;

		if (order.side == Order::Side::BUY && order.price >= quote.ask) {
			execute(order);
			executed = true;
		}
		else if (order.side == Order::Side::SELL && order.price <= quote.bid) {
			execute(order);
			executed = true;
		}

		if (!executed) stillPending.push_back(order);
	}

	pendingOrders = std::move(stillPending);
}

void OrderManager::execute(Order& order) {
	if (order.side == Order::Side::BUY) {
		position += order.volume;
		cash -= order.volume * order.price;
	}
	else {
		position -= order.volume;
		cash += order.volume * order.price;
	}
}

double OrderManager::getPnL(double lastPrice) const {
	return cash + position * lastPrice;
}

double OrderManager::getPosition() const {
	return position;
}
