#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <map>
#include <tuple>
#include <numeric>
#include <algorithm>
#include <execution>

using namespace std;

using row_t = map<int64_t,char>;
using map_t = map<int64_t,row_t>;
using coords_t = pair<int64_t, int64_t>;

void dig(map_t& map, coords_t& cursor, char dir, int64_t length, coords_t& min_coords, coords_t& max_coords) {

	for (int i = 0; i < length; i++) {
		char& prev = map[cursor.first][cursor.second];
		char new_dir = dir;
		switch (dir) {
		case 'U':
			cursor.first--;
			break;
		case 'D':
			cursor.first++;
			//right->down == '7'
			prev = prev == 'R' ? '7' : 
				//left->down == 'F'
				(prev == 'L' ? 'F' : dir);
			break;
		case 'L':
			//up->left == '7'
			prev = prev == 'U' ? '7' : dir;
			cursor.second--;
			break;
		case 'R':
			//up->right == 'F'
			prev = prev == 'U' ? 'F' : dir;
			cursor.second++;
			break;
		}

		map[cursor.first][cursor.second] = dir;
	}

	min_coords = { min(min_coords.first, cursor.first), min(min_coords.second, cursor.second) };
	max_coords = { max(max_coords.first, cursor.first), max(max_coords.second, cursor.second) };
}

void print_map(map_t& map, const coords_t& min_coords, const coords_t& max_coords) {
	for (int64_t row = min_coords.first; row <= max_coords.first; row++) {
		cout << " ";
		for (int64_t col = min_coords.second; col <= max_coords.second; col++) {
			if (map.find(row) != map.end() && map[row].find(col) != map[row].end())
				cout << map[row][col];
			else
				cout << '.';
		}
		cout << endl;
	}
}

uint64_t fill_map(map_t& map, const coords_t& min_coords, const coords_t& max_coords) {
	uint64_t count = 0; // count #

	/* from part 10
			if (c == '.') {
				c = isOut ? 'O' : 'I';
				if (!isOut)
					++count;
			}
			else {
				if (c == '|' || c == '7' || c == 'F')
				isOut = !isOut;
			}
	*/

	vector<int64_t> rows(max_coords.first - min_coords.first + 1, 0);
	std::iota(rows.begin(), rows.end(), min_coords.first);

	for_each(execution::par_unseq, rows.begin(), rows.end(), [&map, &min_coords, &max_coords](int64_t& initial_row) {
		bool inside = false;
		char prev = '.';
		uint64_t row = initial_row;
		initial_row = 0; // will accumulate result
		row_t& map_row = map[row];
		for (int64_t col = min_coords.second; col <= max_coords.second; col++) {
			if (map.find(row) != map.end() && map_row.find(col) != map_row.end()) {
				initial_row++;
				//flip only on Up or Down, or 7 / F type corner -> UR, 
				char v = map_row[col];
				if (((v == 'U' || v == 'D') && prev == '.') || v == '7' || v == 'F')
					inside = !inside;
				prev = v;
			}
			else {
				if (inside) {
					map_row[col] = '#';
					initial_row++;
				}
				else
					map_row[col] = '.';


				prev = '.';
			}

		}
		});

	return std::accumulate(rows.begin(), rows.end(), (uint64_t)0, plus<int64_t>());
}

pair<char, long> color_to_dir_length(string color) {
	std::array<char, 4> id_to_dir{ 'R', 'D', 'L', 'U' };

	const int size = color.size();
	char dir = id_to_dir[color[size - 2] - '0'];

	color = string(color.substr(2, size - 4));

	stringstream scolor(color);
	long value;

	scolor >> std::hex >> value;

	return { dir, value };
}

int main() {
	cout << " AoC 2023 Day18" << endl;

	ifstream input("Day18.txt");

	coords_t max_coords{ 0, 0 };
	coords_t min_coords{ 0, 0 };

	map_t map;

	map[0][0] = 'L';

	coords_t cursor{ 0, 0 };

	constexpr bool isPart1 = true;

	while (!input.eof()) {
		string line;
		getline(input, line);
		std::stringstream sline(line);

		char dir;
		int64_t length;
		string color;

		sline >> dir >> length >> color;

		if (isPart1)
			dig(map, cursor, dir, length, min_coords, max_coords);
		else {
			auto dir_length = color_to_dir_length(color);
			dig(map, cursor, dir_length.first, dir_length.second, min_coords, max_coords);
		}
	}

	dig(map, cursor, 'L', 1, min_coords, max_coords);

	input.close();

	if (isPart1)
		print_map(map, min_coords, max_coords);

	uint64_t count = fill_map(map, min_coords, max_coords);

	cout << endl;

	if (isPart1)
		print_map(map, min_coords, max_coords);

	cout << "Count: " << count << endl;

	return 0;
}
