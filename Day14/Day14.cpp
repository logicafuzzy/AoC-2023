#include <iostream>

#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

using num_t = char;
using row_t = vector<num_t>;
using map_t = vector<row_t>;

void move_north(map_t& map, int row) {
	for (int r = row; r > 0; r--) {
		for (int c = 0; c < map[0].size(); c++) {
			if (map[r][c] == 'O' && map[r - 1][c] == '.') {
				map[r][c] = '.';
				map[r - 1][c] = 'O';
			}
		}
	}
}

void move_south(map_t& map, int row) {
	for (int r = row; r < map.size() - 1; r++) {
		for (int c = 0; c < map[0].size(); c++) {
			if (map[r][c] == 'O' && map[r + 1][c] == '.') {
				map[r][c] = '.';
				map[r + 1][c] = 'O';
			}
		}
	}
}


void move_west(map_t& map, int col) {
	for (int c = col; c > 0; c--) {
		for (int r = 0; r < map.size(); r++) {
			if (map[r][c] == 'O' && map[r][c - 1] == '.') {
				map[r][c] = '.';
				map[r][c - 1] = 'O';
			}
		}
	}
}

void move_east(map_t& map, int col) {
	for (int c = col; c < map[0].size() - 1; c++) {
		for (int r = 0; r < map.size(); r++) {
			if (map[r][c] == 'O' && map[r][c + 1] == '.') {
				map[r][c] = '.';
				map[r][c + 1] = 'O';
			}
		}
	}
}

void move_all_north(map_t& map) {
	for (int r = 1; r < map.size(); ++r)
		move_north(map, r);
}

void move_all_south(map_t& map) {
	for (int r = map.size() - 1; r >= 0; --r)
		move_south(map, r);
}

void move_all_west(map_t& map) {
	for (int c = 1; c < map.size(); ++c)
		move_west(map, c);
}

void move_all_east(map_t& map) {
	for (int c = map[0].size() - 1; c >= 0; --c)
		move_east(map, c);
}

bool operator==(const map_t& map_a, const map_t& map_b) {
	//not needed for the scope of the puzzle, just habit
	//if (map_a.size() != map_b.size() || map_a[0].size() != map_b.size())
	//	return false;

	for (int r = 0; r < map_a.size(); ++r)
		for (int c = 0; c < map_a[0].size(); ++c)
			if (map_a[r][c] != map_b[r][c])
				return false;

	return true;
}

int main() {
	
	cout << " AoC 2023 Day14" << endl;

	ifstream input("Day14.txt");

	map_t map;

	while (!input.eof()) {
		string line;
		getline(input, line);

		row_t row(line.begin(), line.end());
		map.push_back(row);
	}

	input.close();

	uint64_t count = 0;

	constexpr bool isPart2 = true;

	if (!isPart2) {

		move_all_north(map);

	}
	else
	{
		// part2
		constexpr uint32_t cycles = 1000000000;

		vector<map_t> previous{ map };

		long cycle = 0;
		int cycle_length = 0;
		int cycle_start = 0;

		for (cycle = 0; cycle < cycles; ++cycle) {
			move_all_north(map);
			move_all_west(map);
			move_all_south(map);
			move_all_east(map);

			auto el = find(previous.begin(), previous.end(), map);

			if (el != previous.end()) {
				cycle_length = previous.end() - el;
				cycle_start = el - previous.begin();
				break;
			} else
				previous.push_back(map);

			if (cycle % 100000 == 0)
				printf("Cycle %ld             \r", cycle);
		}
		
		long remaining = cycles - cycle_start;
		printf("cycle %ld \t cycle_length %d cycle_start  %d\n", cycle, cycle_length, cycle_start);
		printf("remaining cycles: %d offset: %d \n", remaining, remaining % cycle_length);

		map = previous[cycle_start + (remaining % cycle_length)];

		previous.clear();

	}

	for (int r = 0, w = map.size(); r < map.size(); r++, w--) {
		count += std::count(map[r].begin(), map[r].end(), 'O') * w;
	}

	cout << "Weight: " << count << endl;

	return 0;
}
