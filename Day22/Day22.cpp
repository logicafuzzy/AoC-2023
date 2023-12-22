#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <array>
#include <algorithm>

using namespace std;

enum axis {
	x = 0,
	y,
	z
};

using v_t = std::array<int, 3>;
using brick_t = std::pair < v_t, v_t>;

v_t operator+(const v_t& v1, const v_t& v2) {
	return { v1[axis::x] + v2[axis::x],
		v1[axis::y] + v2[axis::y],
		v1[axis::z] + v2[axis::z] };
}


brick_t operator+(const brick_t&& brick, const v_t&& v) {
	
	brick_t res;
	res.first = brick.first + v;
	res.second = brick.second + v;

	return res;
}

bool intersect(const brick_t&& brick, const brick_t&& other) {


	return false;
}


v_t get_edge(string vec) {
	v_t v;

	stringstream svec(vec);

	string coord;
	getline(svec, coord, ',');
	v[0] = atoi(coord.c_str());
	getline(svec, coord, ',');
	v[1] = atoi(coord.c_str());
	getline(svec, coord, ',');
	v[2] = atoi(coord.c_str());

	return v;
}

brick_t get_brick(string line) {
	stringstream sline(line);
	brick_t res;
	string vec;

	getline(sline, vec, '~');
	res.first = get_edge(vec);

	getline(sline, vec, '~');
	res.second = get_edge(vec);

	return res;
}


int main() {
	cout << " AoC 2023 Day00" << endl;

	vector<brick_t> bricks;

	ifstream input("Day00.txt");

	while (!input.eof()) {
		string line;
		getline(input, line);

		bricks.push_back(get_brick(line));
		
	}
	input.close();

	for ()

	return 0;
}
