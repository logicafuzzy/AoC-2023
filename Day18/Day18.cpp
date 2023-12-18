#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <map>
#include <tuple>

using namespace std;

using row_t = map<int,char>;
using map_t = map<int,row_t>;
using coords_t = pair<int, int>;

void dig(map_t& map, coords_t& cursor, char dir, int length, coords_t& min_coords, coords_t& max_coords) {

	for (int i = 0; i < length; i++) {
		switch (dir) {
		case 'U':
			cursor.first--;
			break;
		case 'D':
			cursor.first++;
			break;
		case 'L':
			cursor.second--;
			break;
		case 'R':
			cursor.second++;
			break;
		}
		map[cursor.first][cursor.second] = dir;
	}

	min_coords = { min(min_coords.first, cursor.first), min(min_coords.second, cursor.second) };
	max_coords = { max(max_coords.first, cursor.first), max(max_coords.second, cursor.second) };
}

void print_map(map_t& map, const coords_t& min_coords, const coords_t& max_coords) {
	for (int row = min_coords.first; row <= max_coords.first; row++) {
		cout << " ";
		for (int col = min_coords.second; col <= max_coords.second; col++) {
			if (map.find(row) != map.end() && map[row].find(col) != map[row].end())
				cout << map[row][col];
			else
				cout << '.';
		}
		cout << endl;
	}
}

long fill_map(map_t& map, const coords_t& min_coords, const coords_t& max_coords) {
	long count = 0; // count #

	for (int row = min_coords.first; row <= max_coords.first; row++) {
		bool inside = false;
		char prev = '.';
		for (int col = min_coords.second; col <= max_coords.second; col++) {
			if (map.find(row) != map.end() && map[row].find(col) != map[row].end()) {
				count++;
				//flip only npn Up or Down
				char v = map[row][col];
				if ((v == 'U' || v == 'D') && map[row][col] != prev)
					inside = !inside;

				prev = (v == 'U' || v == 'D') ? v : prev;
			}
			else {
				if (inside) {
					map[row][col] = '#';
					count++;
				}
				else
					map[row][col] = '.';
			}

		}
	}
	return count;
}

int main() {
	cout << " AoC 2023 Day18" << endl;

	ifstream input("Day18.txt");

	coords_t max_coords{ 0, 0 };
	coords_t min_coords{ 0, 0 };

	map_t map;

	map[0][0] = '#';

	coords_t cursor{ 0, 0 };

	while (!input.eof()) {
		string line;
		getline(input, line);
		std::stringstream sline(line);

		char dir;
		int length;
		string color;

		sline >> dir >> length >> color;
		
		dig(map, cursor, dir, length, min_coords, max_coords);
	}

	input.close();

	print_map(map, min_coords, max_coords);

	long count = fill_map(map, min_coords, max_coords);

	cout << endl;
	print_map(map, min_coords, max_coords);

	cout << "Count: " << count << endl;

	return 0;
}
