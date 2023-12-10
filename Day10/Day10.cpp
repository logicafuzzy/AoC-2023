#include <iostream>

#include <fstream>
#include <string>
#include <optional>
#include <unordered_map>
#include <vector>

using namespace std;

struct cell_t {

	cell_t(char c_, int row_, int col_) :
		c(c_),
		row(row_),
		col(col_)
	{}

	char c{ 0 };
	optional<reference_wrapper<cell_t>> next;
	optional<reference_wrapper<cell_t>> prev;
	int row;
	int col;

	bool operator==(const cell_t& other) const {
		return row == other.row && col == other.col;
	}

	bool operator==(const cell_t&& other) const {
		return row == other.row && col == other.col;
	}

	bool operator!=(const cell_t& other) const {
		return !(*this == other);
	}

	bool operator!=(const cell_t&& other) const {
		return !(*this == other);
	}
};

using row_t = vector<cell_t>;
using map_t = vector<row_t>;

using simple_row_t = vector<char>;
using simple_map_t = vector<simple_row_t>;

void fill_adj(map_t& map, cell_t& cell) {
	int up = cell.row - 1;
	int down = cell.row + 1;
	int left = cell.col - 1;
	int right = cell.col + 1;

	int next_row = cell.row, next_col = cell.col;
	int prev_row = cell.row, prev_col = cell.col;

	switch (cell.c) {
	case '|':
		prev_row = up;
		next_row = down;
		break;
	case '-':
		prev_col = left;
		next_col = right;
		break;
	case 'L':
		prev_row = up;
		next_col = right;
		break;
	case 'J':
		prev_row = up;
		next_col = left;
		break;
	case '7':
		prev_col = left;
		next_row = down;
		break;
	case 'F':
		prev_row = down;
		next_col = right;
		break;
	case '.':
	case 'S':
		return;
	}
	if (next_row >= 0 && next_row < map.size() && next_col >= 0 && next_col < map[0].size())
		cell.next = map[next_row][next_col];

	if (prev_row >= 0 && prev_row < map.size() && prev_col >= 0 && prev_col < map[0].size())
		cell.prev = map[prev_row][prev_col];
}

void connect_start(map_t& map, cell_t& start) {
	for (auto& row : map)
		for (auto& cell : row) {
			if (cell.next.has_value() && cell.next.value().get() == start) {
				start.prev = cell;
				if (start.next.has_value())
					return;
			}

			if (cell.prev.has_value() && cell.prev.value().get() == start) {
				start.next = cell;
				if (start.prev.has_value())
					return;
			}
		}
}

cell_t& get_start(map_t& map) {
	for (auto& row : map)
		for (auto& cell : row) {
			if (cell.c == 'S') {
				return cell;
			}
		}
}

void replace_start(cell_t& start) {

}

optional<reference_wrapper<cell_t>> get_next(const cell_t& current, const cell_t& previous) {
	if (current.next.has_value()) {
		cell_t& next = *current.next;
		if (next != previous)
			return next;
	}
	if (current.prev.has_value()) {
		cell_t& prev = *current.prev;
		if (prev != previous)
			return prev;
	}
}

simple_map_t clean_map(const map_t& map, const row_t& path) {
	simple_row_t row(map[0].size(), '.');
	simple_map_t result(map.size(), row);

	for (auto& cell : path)
		result[cell.row][cell.col] = cell.c;

	return result;
}

void print(const simple_map_t& map) {
	for (auto& row : map) {
		for (auto& c : row) {
			switch (c) {
			case 'F':
			case 'J':
			case 'L':
			case '7':
			case '-':
			case '|':
				cout << "\033[1;32m";
				break;
			case 'I':
				cout << "\033[1;33m";
				break;
			case 'O':
				cout << "\033[1;34m";
				break;
			}
			cout << c;
			cout << "\033[0;m";
		}

		cout << endl;
	}
}

int main() {
	cout << " AoC 2023 Day10" << endl;

	ifstream input("Day10test3.txt");

	map_t map;

	int rows = 0;
	int cols = 0;

	while (!input.eof()) {
		string line;
		getline(input, line);

		cols = 0;

		row_t row;

		for (char c : line)
			row.emplace_back(c, rows, cols++);

		map.push_back(row);
		rows++;
	}

	input.close();

	for (auto& row : map)
		for (auto& cell : row)
			fill_adj(map, cell);

	cell_t start = get_start(map);
	connect_start(map, start);

	int steps = 1;

	cell_t& current = *start.next;
	cell_t& previous = start;

	row_t path{ previous, current };
	printf("%c%c", previous.c, current.c);

	do {
		cell_t& next = *get_next(current, previous);
		printf("%c", next.c);
		
		path.push_back(next);

		previous = current;
		current = next;
		steps++;
	} while (current.c != 'S');

	printf("\n\nFarthest: %d remainder: %d\n", steps / 2, steps % 2);

	printf("Part2: \n\n");

	simple_map_t simple_map = clean_map(map, path);

	int count = 0;

	for (auto& row : simple_map) {
		bool isOut = true;
		for (auto& c : row) {
			if (c == '.') {
				c = isOut ? 'O' : 'I';
				if (!isOut)
					++count;
			}
			else {
				if (c != '-')
					isOut = !isOut;
			}
		}
	}
	
	print(simple_map);

	printf("Cells inside: %d\n", count);

	return 0;
}
