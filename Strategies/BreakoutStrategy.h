#pragma once

#include <iostream>
#include <algorithm>
#include <mutex>
#include <deque>
#include "Strategy.h"

extern std::mutex globalPrintMutex;

template<int WinSize = 20>
class BreakoutStrategy : public Strategy {
private:
	OrderManager* orderManager;
	double lastPrice = -1.0;
	std::deque<double> recentPrices;
	bool inPosition = false;
	double entryPrice = 0.0;
public:
	BreakoutStrategy(OrderManager* om = nullptr) : orderManager(om) {}

	void setOrderManager(OrderManager* om) override {
		orderManager = om;
	}

	void onTick(const Tick& tick) override {
		if (recentPrices.size() >= WinSize) {
			double high = *std::max_element(recentPrices.begin(), recentPrices.end());
			double low = *std::min_element(recentPrices.begin(), recentPrices.end());

			if (!inPosition && tick.price > high) {
				Order buy = { Order::Side::BUY, OrderType::MARKET, tick.timestamp, 1.0, tick.price };
				orderManager->submit(buy);
				entryPrice = tick.price;
				inPosition = true;
				{
					std::lock_guard<std::mutex> lock(globalPrintMutex);
					std::cout << "[BREAKOUT BUY] @ " << tick.price << "\n";
				}
			}
			else if (inPosition && tick.price < low) {
				Order sell = { Order::Side::SELL, OrderType::MARKET, tick.timestamp, 1.0, tick.price };
				orderManager->submit(sell);
				inPosition = false;
				{
					std::lock_guard<std::mutex> lock(globalPrintMutex);
					std::cout << "[BREAKOUT SELL] @ " << tick.price << "\n";
				}
			}
		}

		recentPrices.push_back(tick.price);
		if (recentPrices.size() > WinSize) recentPrices.pop_front();
	}
};