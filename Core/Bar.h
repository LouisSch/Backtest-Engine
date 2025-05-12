#pragma once

#include <cstdint>

struct Bar {
	uint64_t startTimestamp;
	uint64_t endTimestamp;
	double open;
	double high;
	double low;
	double close;
	double volume;
};