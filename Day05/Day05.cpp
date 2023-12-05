#include <iostream>

#include <fstream>
#include <string>
#include <cassert>
#include <algorithm>

#include "Day05data.h"

using namespace std;
using cat_t = unsigned long long;
using range_t = vector<cat_t>;

inline bool isInRange(cat_t value, range_t range) {
	return value >= range[1] && value - range[1] <= range[2];
}

inline cat_t getDestination(cat_t value, range_t range) {
	assert(isInRange(value, range));

	return value - range[1] + range[0];
}

cat_t getFinalDestination(cat_t value, vector< vector< cat_t > > map) {
	for (range_t& range : map) {
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

	printf("Minimum location: %llu", lowest);
}

void part2() {
	cat_t lowest = numeric_limits<cat_t>::max();

	vector <pair<cat_t, cat_t> > seed_ranges;
	for (int i = 0; i < seeds.size(); i += 2) {
		cat_t seed_start = seeds[i];
		cat_t seed_range = seeds[i + 1];

		seed_ranges.push_back({ seed_start, seed_range });
	}

	for (auto& seed_pair : seed_ranges)
		{
			for (cat_t seed = seed_pair.first; seed - seed_pair.second <= seed_pair.first; ++seed) {
				cat_t value = seed;
				for (vector<range_t>& map : input_data) {
					value = getFinalDestination(value, map);
				}
				lowest = min(lowest, value);
				//printf("Seed %llu mapped to: %llu\n", seed, value);
			}
			printf("Seed start %llu\n", seed_pair.first);
		}

	printf("Minimum location: %llu", lowest);
}

int main() {
	cout << " AoC 2023 Day05" << endl;
	
	//part1();

	part2();

	return 0;
}
