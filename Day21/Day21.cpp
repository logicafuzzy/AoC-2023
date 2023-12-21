#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <set>

using namespace std;

using coord_t = pair<int, int>;
using cell_t = char;
using row_t = vector<cell_t>;
using map_t = vector<row_t>;

//part1
constexpr int nsteps = 64;
constexpr bool wrap = false;
//part2
//constexpr int nsteps = 26501365;
//constexpr bool wrap = true;

bool is_outside(const map_t& map, const coord_t& coord) {
	int row = coord.first, col = coord.second;
	return row < 0 || row >= map.size() || col < 0 || col >= map[0].size();
}

bool is_outside(const map_t& map, int row, int col) {
	return row < 0 || row >= map.size() || col < 0 || col >= map[0].size();
}

bool can_move(const map_t& map, int row, int col) {
	return !is_outside(map, row, col) && map[row][col] == '.';
}

void print_path(map_t& map, const set<coord_t>& to_check, const set<coord_t> newsteps) {
	printf("Map: \n");

	coord_t current{ 0, 0 };
	for (const auto& row : map) {
		
		current.second = 0;

		cout << " ";
		
		for (const auto& c : row) {
			if (to_check.find(current) != to_check.end())
				printf("\033[1;32m%c\033[0;m", c);
			else if (newsteps.find(current) != newsteps.end())
				printf("\033[1;33m%c\033[0;m", c);
			else
				printf("%c", c);

			current.second++;
		}
		
		cout << endl;
		
		current.first++;
	}
}

int main() {
	cout << " AoC 2023 Day21" << endl;

	ifstream input("Day21.txt");

	map_t map;

	coord_t start;

	coord_t current{ 0, 0 };

	while (!input.eof()) {
		string line;
		getline(input, line);

		row_t row;
		current.second = 0;
		for (char c : line) {
			row.push_back(c);

			if (c == 'S')
				start = current;

			current.second++;
		}

		map.push_back(row);
		
		current.first++;
	}

	input.close();

	set<coord_t> steps{ start };

	set<coord_t> to_check = steps;

	for (int i = 0; i < nsteps; i++) {
		set<coord_t> newsteps;
		for (const coord_t& step : to_check) {
			int nextrow, nextcol;
			int row = step.first, col = step.second;

			nextrow = row - 1; nextcol = col;
			if (can_move(map, nextrow, nextcol))
				newsteps.insert({ nextrow, nextcol });

			nextrow = row + 1; nextcol = col;
			if (can_move(map, nextrow, nextcol))
				newsteps.insert({ nextrow, nextcol });

			nextrow = row; nextcol = col - 1;
			if (can_move(map, nextrow, nextcol))
				newsteps.insert({ nextrow, nextcol });

			nextrow = row; nextcol = col + 1;
			if (can_move(map, nextrow, nextcol))
				newsteps.insert({ nextrow, nextcol });
		}
		
		//print_path(map, to_check, newsteps);

		steps.insert(to_check.begin(), to_check.end());
		to_check = newsteps;
	}
	steps.insert(to_check.begin(), to_check.end());

	print_path(map, to_check, {});

	cout << "Result: " << to_check.size() + 1 << endl;

	return 0;
}
