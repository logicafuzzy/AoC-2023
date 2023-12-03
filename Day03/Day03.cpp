#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <set>
#include <cassert>

using namespace std;

enum class cell_type {
	empty,
	number,
	symbol,
	part_number
};

struct cell_info {
	cell_type type{ cell_type::empty };
	int value{ 0 };
	int digit{ 0 };
	vector<int> part_numbers;
};

using map_row = std::unordered_map<int, cell_info>;
using map_type = std::unordered_map<int, map_row>;

bool is_part_number(map_type& map, int x, int y) {

	int value = map[x][y].value;

	//up
	const bool has_up = map[x].find(y - 1) != map[x].end();
	if (has_up && map[x][y - 1].type == cell_type::symbol) {
		map[x][y - 1].part_numbers.push_back(value);
		return true;
	}

	// down
	const bool has_down = map[x].find(y + 1) != map[x].end();
	if (has_down && map[x][y + 1].type == cell_type::symbol) {
		map[x][y + 1].part_numbers.push_back(value);
		return true;
	}

	//left
	const bool has_left = map.find(x - 1) != map.end();
	if (has_left && map[x - 1][y].type == cell_type::symbol) {
		map[x - 1][y].part_numbers.push_back(value);
		return true;
	}

	//right
	const bool has_right = map.find(x + 1) != map.end();
	if (has_right && map[x + 1][y].type == cell_type::symbol) {
		map[x + 1][y].part_numbers.push_back(value);
		return true;
	}

	//top-left
	if (has_left && map[x - 1].find(y - 1) != map[x - 1].end()
		&& map[x - 1][y - 1].type == cell_type::symbol) {
		map[x - 1][y - 1].part_numbers.push_back(value);
		return true;
	}

	//top-right
	if (has_right && map[x + 1].find(y - 1) != map[x + 1].end()
		&& map[x + 1][y - 1].type == cell_type::symbol) {
		map[x + 1][y - 1].part_numbers.push_back(value);
		return true;
	}

	//bottom-left
	if (has_left && map[x - 1].find(y + 1) != map[x - 1].end()
		&& map[x - 1][y + 1].type == cell_type::symbol) {
		map[x - 1][y + 1].part_numbers.push_back(value);
		return true;
	}

	//bottom-right
	if (has_right && map[x + 1].find(y + 1) != map[x + 1].end()
		&& map[x + 1][y + 1].type == cell_type::symbol) {
		map[x + 1][y + 1].part_numbers.push_back(value);
		return true;
	}

	return false;
}

void add_number(map_type& map, int x, int y, const vector<short>& digits) {
	short mult = 1;
	int val = 0;
	for (int n : digits) {
		val += n * mult;
		mult *= 10;
	}
	for (int i = 0; i < digits.size(); i++) {
		map[x - i - 1][y] = { cell_type::number, val, i};
	}
}

void clear_number(map_type& map, int x, int y) {
	cell_info& info = map[x][y];
	short digits = 1;

	if (info.value > 9)
		digits = 2;

	if (info.value > 99)
		digits = 3;

	for (int i = - digits + info.digit + 1; i <= info.digit; ++i) {
		map[x + i][y].type = cell_type::empty;
	}
	
}

int main() {
	cout << " AoC 2023 Day03" << endl;

	ifstream input("Day03.txt");

	map_type map;

	vector<int> part_numbers;

	int y = 0;
	while (!input.eof()) {
		string line;
		getline(input, line);

		int x = 0;

		vector<short> digits;

		for (char c : line) {
			if (c >= '0' && c <= '9') {
				digits.insert(digits.begin(), c - '0');
			}
			else {
				if (!digits.empty()) {
					add_number(map, x, y, digits);					
					digits.clear();
				}
				if (c != '.') {
					map[x][y] = { cell_type::symbol, 0};
				}
			}
			++x;
		}
		//if number end at the end of line
		if (!digits.empty()) {
			add_number(map, x, y, digits);
			digits.clear();
		}
		++y;
	}

	input.close();

	for (auto& row : map) {
		for (auto& cell : row.second) {
			const cell_info& info = row.second[cell.first];
			if (info.type == cell_type::number) {
				if (is_part_number(map, row.first, cell.first)) {
					part_numbers.push_back(info.value);
					clear_number(map, row.first, cell.first);
				}
			}
		}
	}

	int sum = 0;
	for (int v : part_numbers) {
		sum += v;
	}

	cout << "Part numbers: " << part_numbers.size() << " sum: " << sum << endl;

	long gear_ratio_sum = 0;

	//find gears
	for (auto& row : map) {
		for (auto& cell : row.second) {
			const cell_info& info = row.second[cell.first];
			if (info.type == cell_type::symbol) {
				if (info.part_numbers.size() == 2)
					gear_ratio_sum += info.part_numbers[0] * info.part_numbers[1];
			}
		}
	}

	cout << "Gear ratios: " << gear_ratio_sum << endl;

	return 0;
}
