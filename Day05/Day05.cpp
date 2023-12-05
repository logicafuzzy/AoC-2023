#include <iostream>

#include <fstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <optional>

#include "Day05data.h"

using namespace std;

enum range_v {
	dest = 0,
	source,
	length
};

inline bool isInRange(cat_t value, const range_t& range) {
	return value >= range[range_v::source] && value < range[range_v::length] + range[range_v::source];
}

inline cat_t getDestination(cat_t value, const range_t& range) {
	return value - range[range_v::source] + range[range_v::dest];
}

cat_t getFinalDestination(cat_t value, const vector<range_t>& map) {
	for (const range_t& range : map) {
		if (isInRange(value, range)) {
			return getDestination(value, range);
		}
	}
	return value;
}

void part1() {
	printf("Part 1: \n");

	cat_t lowest = numeric_limits<cat_t>::max();

	for (cat_t seed : seeds) {
		cat_t value = seed;
		for (vector<range_t>& map : input_data) {
			value = getFinalDestination(value, map);
		}
		lowest = min(lowest, value);
		printf("Seed %llu mapped to: %llu\n", seed, value);
	}

	printf("Minimum location: %llu\n\n", lowest);
}

void part2() {
	printf("Part 2: \n");

	cat_t lowest = numeric_limits<cat_t>::max();

	for (int i = 0; i < seeds.size(); i += 2) {
		cat_t seed_start = seeds[i];
		cat_t seed_range = seeds[i + 1];
		cat_t perc_step = seed_range >= 100 ? seed_range / 100 : 1;
		for (cat_t seed = seed_start; seed < seed_start + seed_range; ++seed) {
			if ((seed - seed_start) % perc_step == 0) {
				printf("Progress: %llu %% \r", (seed - seed_start) * 100 / seed_range);
			}
			cat_t value = seed;
			for (const vector<range_t>& map : input_data) {
				value = getFinalDestination(value, map);
			}
			lowest = min(lowest, value);
			}
			printf("Seed start %llu, current lowest: %llu\n", seed_start, lowest);
		}

	printf("Minimum location: %llu\n\n", lowest);
}

int main() {
	cout << " AoC 2023 Day05" << endl;
	
	part1();

	part2();

	return 0;
}
