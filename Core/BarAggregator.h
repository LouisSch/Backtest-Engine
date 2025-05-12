#pragma once

#include <optional>
#include "Tick.h"
#include "Bar.h"

class BarAggregator {
private:
	uint64_t windowSize;
	uint64_t currentWindow = 0;
	std::optional<Bar> currentBar;
public:
	BarAggregator(uint64_t windowSize) : windowSize(windowSize) {}

	std::optional<Bar> update(const Tick& tick) {
		uint64_t tickWindow = tick.timestamp / windowSize * windowSize;

		// New Bar
		if (!currentBar || tickWindow != currentWindow) {
			std::optional<Bar> completed = currentBar;
			currentBar = Bar{
				.startTimestamp = tickWindow,
				.endTimestamp = tickWindow + windowSize,
				.open = tick.price,
				.high = tick.price,
				.low = tick.price,
				.close = tick.price,
				.volume = tick.volume,
			};
			currentWindow = tickWindow;

			// Return completed Bar
			return completed;
		}

		// Update Bar
		currentBar->high = std::max(currentBar->high, tick.price);
		currentBar->low = std::min(currentBar->low, tick.price);
		currentBar->close = tick.price;
		currentBar->volume += tick.volume;

		return std::nullopt;
	}

	std::optional<Bar> flush() const {
		return currentBar;
	}
};