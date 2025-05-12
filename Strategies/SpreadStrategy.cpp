#include "SpreadStrategy.h"

void SpreadStrategy::setOrderManager(OrderManager* om) {
	orderManager = om;
}

void SpreadStrategy::onTick(const QuoteTick& tick) {
	double spread = tick.ask - tick.bid;
	double position = orderManager->getPosition();

	if (spread < minSpread) return;

	double bidQuote = tick.bid - offset;
	double askQuote = tick.ask + offset;

	if (position < 5.0) {
		Order buy = Order{
			.side = Order::Side::BUY,
			.type = OrderType::LIMIT,
			.timestamp = tick.timestamp,
			.volume = orderSize,
			.price = bidQuote
		};
		orderManager->submit(buy);

		{
			std::lock_guard<std::mutex> lock(globalPrintMutex);
			std::cout << "[SPREAD] Placing LIMIT BUY @ " << bidQuote << "\n";
		}
	}

	if (position > -5.0) {
		Order buy = Order{
			.side = Order::Side::SELL,
			.type = OrderType::LIMIT,
			.timestamp = tick.timestamp,
			.volume = orderSize,
			.price = askQuote
		};
		orderManager->submit(buy);

		{
			std::lock_guard<std::mutex> lock(globalPrintMutex);
			std::cout << "[SPREAD] Placing LIMIT SELL @ " << askQuote << "\n";
		}
	}

	/*std::lock_guard<std::mutex> lock(globalPrintMutex);
	std::cout << "[SPREAD] Pos : " << position << " | Spread : " << spread << " | BidQ " << bidQuote << " | AskQ " << askQuote << "\n";*/
}
