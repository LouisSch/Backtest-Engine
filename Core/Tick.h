#pragma once

#include <cstdint>

struct Tick {
	uint64_t timestamp;
	double price;
	double volume;
};

struct QuoteTick {
	uint64_t timestamp;
	double bid;
	double ask;
	double volume;
};