#include <iostream>

#include <fstream>
#include <string>
#include <vector>

using namespace std;
using num_t = char;
using row_t = vector<num_t>;
using map_t = vector<row_t>;

struct galaxy_t {
	int row{};
	int col{};
};

bool all_equal(const row_t& row) {
	for (auto val : row)
		if (val != row[0])
			return false;

	return true;
}

bool all_equal(const map_t& map, int col) {
	for (const row_t& row : map)
		if (row[col] != map[0][col])
			return false;

	return true;
}

void expand_row(map_t& map, int row) {
	row_t new_row(map[0].size(), '.');

	map.insert(map.begin() + row, new_row);
}

void expand_col(map_t& map, int col) {
	for (row_t& row : map)
		row.insert(row.begin() + col, '.');
}

vector<galaxy_t> get_galaxies(const map_t& map) {
	int nrow = 0, ncol = 0, ngal = 0;
	vector<galaxy_t> result;

	for (const row_t row : map) {
		ncol = 0;
		for (const num_t val : row) {
			if (val == '#')
				result.push_back({ nrow, ncol});
			ncol++;
		}
		nrow++;
	}

	return result;
}

void expand(map_t& map) {
	//expand rows
	vector<int> to_expand;
	for (int i = 0; i < map.size(); ++i)
		if (all_equal(map[i]))
			to_expand.push_back(i);

	for (int i = 0; i < to_expand.size(); ++i)
		expand_row(map, to_expand[i] + i);

	//expand cols
	to_expand.clear();
	for (int i = 0; i < map[0].size(); ++i)
		if (all_equal(map, i))
			to_expand.push_back(i);

	for (int i = 0; i < to_expand.size(); ++i)
		expand_col(map, to_expand[i] + i);

}

int main() {
	cout << " AoC 2023 Day00" << endl;

	ifstream input("Day11.txt");

	map_t map;

	while (!input.eof()) {
		string line;
		getline(input, line);

		row_t row;

		for (char c : line)
			row.push_back(c);

		map.push_back(row);
	}

	input.close();

	expand(map);

	vector<galaxy_t> galaxies = get_galaxies(map);

	long sum = 0;

	for (int i = 0; i < galaxies.size(); ++i) {

		for (int j = i + 1; j < galaxies.size(); ++j) {

			int dist = abs(galaxies[i].row - galaxies[j].row) + abs(galaxies[i].col - galaxies[j].col);

			sum += dist;

			//printf("distance %d %d: %d\n", i + 1, j + 1, dist);

		}
		//sum += min_dist;
	}

	printf("Sum min_dist: %d\n", sum);

	return 0;
}
