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

	for (int row = min_coords.first; row <= max_coords.first; row++) {
		bool inside = false;
		char prev = '.';
		for (int col = min_coords.second; col <= max_coords.second; col++) {
			if (map.find(row) != map.end() && map[row].find(col) != map[row].end()) {
				count++;
				//flip only on Up or Down, or 7 / F type corner -> UR, 
				char v = map[row][col];
				if (((v == 'U' || v == 'D') && prev == '.') || v == '7' || v == 'F')
					inside = !inside;
				prev = v;
			}
			else {
				if (inside) {
					map[row][col] = '#';
					count++;
				}
				else 
					map[row][col] = '.';


				prev = '.';
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

	map[0][0] = 'L';

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

	dig(map, cursor, 'L', 1, min_coords, max_coords);

	input.close();

	print_map(map, min_coords, max_coords);

	long count = fill_map(map, min_coords, max_coords);

	cout << endl;
	print_map(map, min_coords, max_coords);

	cout << "Count: " << count << endl;

	return 0;
}
