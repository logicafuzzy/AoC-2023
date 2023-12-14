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

	for (int r = 1; r < map.size(); ++r)
		move_north(map, r);

	uint64_t count = 0;

	for (int r = 0, w = map.size(); r < map.size(); r++, w--) {
		count += std::count(map[r].begin(), map[r].end(), 'O') * w;
	}

	cout << "Weight: " << count << endl;

	return 0;
}
