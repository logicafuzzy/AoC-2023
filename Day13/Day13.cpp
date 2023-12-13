#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

using num_t = char;
using row_t = vector<num_t>;
using map_t = vector<row_t>;

bool equal_rows(map_t& map, int a, int b, bool& exact) {
	const int n = map[a].size();

	int smudge_index = -1;
	bool noretry = exact;

	for (int i = 0; i < n; ++i)
		if (map[a][i] != map[b][i])
			if (noretry)
				return false;
			else {
				smudge_index = i;
				noretry = true;
			}
	
	// single chance used
	exact = noretry;

	if (smudge_index >= 0)
 		map[a][smudge_index] = map[b][smudge_index];

	return true;
}

bool equal_cols(map_t& map, int a, int b, bool& exact) {
	const int n = map.size();

	int smudge_index = -1;
	bool noretry = exact;
	for (int i = 0; i < n; ++i)
		if (map[i][a] != map[i][b])
			if (noretry)
				return false;
			else {
				smudge_index = i;
				noretry = true;
			}

	// single chance used
	exact = noretry;

	if (smudge_index >= 0)
		map[smudge_index][a] = map[smudge_index][b];

	return true;
}

bool check_row_reflection(map_t& map, int row, bool& exact) {
	const int steps = min(row, (int)map.size() - row - 2);
	for (int i = 0; i <= steps; ++i) {
		if (!equal_rows(map,row - i, row + i + 1, exact))
			return false;
	}
	return true;
}

int get_row_reflection(map_t& map, bool& exact) {
	const int n = map.size();
	for (int i = 0; i < n - 1; ++i) {
		if (check_row_reflection(map, i, exact))
			return i + 1;
	}

	return 0;
}

bool check_col_reflection(map_t& map, int col, bool& exact) {
	const int steps = min(col, (int)map[0].size() - col - 2);
	for (int i = 0; i <= steps; ++i) {
		if (!equal_cols(map, col - i, col + i + 1, exact))
			return false;
	}
	return true;
}

int get_col_reflection(map_t& map, bool& exact) {
	const int n = map[0].size();
	for (int i = 0; i < n - 1; ++i) {
		if (check_col_reflection(map, i, exact))
			return i + 1;
	}

	return 0;
}

int main() {
	cout << " AoC 2023 Day13" << endl;

	ifstream input("Day13test.txt");

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
				bool keepsmudge = true;
				int col_reflection = get_col_reflection(map, keepsmudge);
				int row_reflection = get_row_reflection(map, keepsmudge);

				cout << "Column reflection: " << col_reflection << endl;
				cout << "Row reflection: " << row_reflection << endl;

				count += col_reflection + 100 * row_reflection;

				map.clear();
			}
			else {
				bool keepsmudge = false;
				int col_reflection = get_col_reflection(map, keepsmudge);
				int row_reflection = get_row_reflection(map, keepsmudge);

				cout << "Column reflection: " << col_reflection << endl;
				cout << "Row reflection  " << row_reflection << endl;

				keepsmudge = true;
				col_reflection = get_col_reflection(map, keepsmudge);
				row_reflection = get_row_reflection(map, keepsmudge);

				cout << "Column reflection (fixed): " << col_reflection << endl;
				cout << "Row reflection  (fixed): " << row_reflection << endl;

				count += col_reflection + 100 * row_reflection;

				map.clear();
			}
		}
	}

	cout << "Count: " << count << endl;

	input.close();

	return 0;
}
