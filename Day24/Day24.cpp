#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <vector>

using namespace std;

using v_t = std::array<int64_t, 3>;
using ts_t = std::pair<float, v_t>;

constexpr ts_t never{ numeric_limits<float>::max(), {0,0,0} };

struct h_t {
	v_t pos{ 0,0,0 };
	v_t vel{ 0,0,0 };
};

ts_t cross(h_t first, h_t second) {
	// starting from finding a point p which is same for both ray path
	// position(time) = start_position + velocity * time
	// so solving equation for p:
	// p = first.pos + u * first.vel
	// p = second.pos + v * second.vel
	// u and v are time: if u (or v) are negative, intersection is in the past for first (if u is negative) or second (if v is negative)
	float dx = second.pos[0] - first.pos[0];
	float dy = second.pos[1] - first.pos[1];
	float det = second.vel[0] * first.vel[1] - second.vel[1] * first.vel[0];

	if (abs(det) > 1e-4) {
		float u = (dy * second.vel[0] - dx * second.vel[1]) / det;
		float v = (dy * first.vel[0] - dx * first.vel[1]) / det;
		//cout << "u " << u << " v " << v << endl;
		if (u < 0) {
			//intersection in the past for first
			// still return never, we will discard this
			return never;
		}
		if (v < 0) {
			//intersection in the past for second
			return never;
		}

		//cout << first.pos[0] + u * first.vel[0] << ", " << first.pos[1] + u * first.vel[1] << endl;
		//cout << second.pos[0] + v * second.vel[0] << ", " << second.pos[1] + v * second.vel[1] << endl;
		v_t point{ first.pos[0] + u * first.vel[0], first.pos[1] + u * first.vel[1], 0.0 };
		return {u, point};
	}
	else {
		// no intersection
		return never;
	}

	return never;
}

bool is_inside(int64_t min, int64_t max, const v_t& v) {
	return v[0] >= min && v[0] <= max
		&& v[1] >= min && v[1] <= max;
}

int main() {
	cout << " AoC 2023 Day024" << endl;

	vector<h_t> hs;

	ifstream input("Day24.txt");

	while (!input.eof()) {
		string line;
		getline(input, line);
		std::stringstream sline(line);

		int64_t a, b, c, d, e, f;
		sline >> a >> b >> c >> d >> e >> f;

		hs.push_back({ {a, b, c}, {d, e, f} });

	}

	input.close();

	int count = 0;

	//for test
	//constexpr long min_coord = 7;
	//constexpr long max_coord = 27;
	//for solution
	constexpr int64_t min_coord = 200000000000000;
	constexpr int64_t max_coord = 400000000000000;

	for (int i = 0; i < hs.size() - 1; i++) {
		for (int j = i + 1; j < hs.size(); j++) {
			float time;
			ts_t ts = cross(hs[i], hs[j]);
			if (ts != never && is_inside(min_coord, max_coord, ts.second)) {
				count++;
			}
		}
	}

	cout << "Inside: " << count << endl;

	return 0;
}
