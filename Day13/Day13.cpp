#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

using num_t = char;
using row_t = vector<num_t>;
using map_t = vector<row_t>;

bool equal_rows(const map_t& map, int a, int b) {
	const int n = map[a].size();

	for (int i = 0; i < n; ++i)
		if (map[a][i] != map[b][i])
			return false;

	return true;
}

bool equal_cols(const map_t& map, int a, int b) {
	const int n = map.size();

	for (int i = 0; i < n; ++i)
		if (map[i][a] != map[i][b])
			return false;

	return true;
}

bool check_row_reflection(const map_t& map, int row) {
	const int steps = min(row, (int)map.size() - row - 2);
	for (int i = 0; i <= steps; ++i) {
		if (!equal_rows(map,row - i, row + i + 1))
			return false;
	}
	return true;
}


int get_row_reflection(const map_t& map) {
	const int n = map.size();
	for (int i = 0; i < n - 1; ++i) {
		if (check_row_reflection(map, i))
			return i + 1;
	}

	return 0;
}

bool check_col_reflection(const map_t& map, int col) {
	const int steps = min(col, (int)map[0].size() - col - 2);
	for (int i = 0; i <= steps; ++i) {
		if (!equal_cols(map, col - i, col + i + 1))
			return false;
	}
	return true;
}

int get_col_reflection(const map_t& map) {
	const int n = map[0].size();
	for (int i = 0; i < n - 1; ++i) {
		if (check_col_reflection(map, i))
			return i + 1;
	}

	return 0;
}

int diff_rows(const map_t& map, int a, int b) {
	const int n = map[a].size();

	int diff = 0;

	for (int i = 0; i < n; ++i)
		if (map[a][i] != map[b][i])
			diff++;

	return diff;
}

int diff_cols(const map_t& map, int a, int b) {
	const int n = map.size();

	int diff = 0;

	for (int i = 0; i < n; ++i)
		if (map[i][a] != map[i][b])
			diff++;

	return diff;
}

int diff_row_reflection(const map_t& map, int row) {
	const int steps = min(row, (int)map.size() - row - 2);

	int diff = 0;

	for (int i = 0; i <= steps; ++i)
		diff += diff_rows(map, row - i, row + i + 1);

	return diff;
}

int diff_col_reflection(const map_t& map, int col) {
	const int steps = min(col, (int)map[0].size() - col - 2);

	int diff = 0;

	for (int i = 0; i <= steps; ++i)
		diff += diff_cols(map, col - i, col + i + 1);

	return diff;
}

int get_row_reflection_smudged(const map_t& map) {
	const int n = map.size();
	for (int i = 0; i < n - 1; ++i) {
		if (diff_row_reflection(map, i) == 1)
			return i + 1;
	}

	return 0;
}

int get_col_reflection_smudged(const map_t& map) {
	const int n = map[0].size();
	for (int i = 0; i < n - 1; ++i) {
		if (diff_col_reflection(map, i) == 1)
			return i + 1;
	}

	return 0;
}

int main() {
	cout << " AoC 2023 Day13" << endl;

	ifstream input("Day13.txt");

	long count = 0;

	map_t map;

	constexpr bool exact = false;

	while (!input.eof()) {
		string line;
		getline(input, line);
		
		if (line.size() != 0) {
			row_t row(line.begin(), line.end());
			map.push_back(row);
		}
		else {
			if (exact) {// part1
				int col_reflection = get_col_reflection(map);
				int row_reflection = get_row_reflection(map);

				cout << "Column reflection: " << col_reflection << endl;
				cout << "Row reflection: " << row_reflection << endl;

				count += col_reflection + 100 * row_reflection;

				map.clear();
			}
			else {
				int col_reflection = get_col_reflection_smudged(map);
				int row_reflection = get_row_reflection_smudged(map);

				cout << "Column reflection smudged: " << col_reflection << endl;
				cout << "Row reflection  smudged: " << row_reflection << endl;

				count += col_reflection + 100 * row_reflection;

				map.clear();
			}
		}
	}

	cout << "Count: " << count << endl;

	input.close();

	return 0;
}
