#include "MeanReversionSimpleStrategy.h"

void MeanReversionSimple::setOrderManager(OrderManager* om) {
	orderManager = om;
}

void MeanReversionSimple::onTick(const Tick& tick) {
	if (lastPrice < 0) {
		lastPrice = tick.price;
		return;
	}

	if (!inPosition && tick.price < lastPrice * 0.995) {
		Order buy = { Order::Side::BUY, OrderType::MARKET, tick.timestamp, 1.0, tick.price };
		orderManager->submit(buy);
		entryPrice = tick.price;
		inPosition = true;
		{
			std::lock_guard<std::mutex> lock(globalPrintMutex);
			std::cout << "[MEAN REVERSION BUY] @ " << tick.price << "\n";
		}
	}
	else if (inPosition && tick.price > entryPrice * 1.005) {
		Order sell = { Order::Side::SELL, OrderType::MARKET, tick.timestamp, 1.0, tick.price };
		orderManager->submit(sell);
		inPosition = false;
		{
			std::lock_guard<std::mutex> lock(globalPrintMutex);
			std::cout << "[MEAN REVERSION SELL] @ " << tick.price << "\n";
		}
	}

	lastPrice = tick.price;
}
