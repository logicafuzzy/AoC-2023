#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <tuple>

using namespace std;

enum class direction_t {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum class cell_type_t {
	EMPTY,
	MIRROR_U, // -->/ goes upward
	MIRROR_D, // -->\ goes down
	SPLIT_V, // |
	SPLIT_H // -
};

struct cell_t {
	cell_type_t type{ cell_type_t::EMPTY };
	int energy{ 0 };
};

struct beam_t {
	int row{};
	int col{};
	direction_t dir{};
	vector<beam_t> history{};

	void checkpoint() {
		history.push_back({row, col, dir});
	}
		 
	// this is < other if this is in other's history
	bool operator<(const beam_t& other) const {
		return find(other.history.begin(), other.history.end(), *this) != other.history.end();
	}

	bool operator==(const beam_t& other) const {
		return row == other.row && col == other.col && dir == other.dir;
	}

	bool operator<=(const beam_t& other) const {
		return *this == other || *this < other;
	}
};

using row_t = vector<cell_t>;
using map_t = vector<row_t>;

using cell_ref_t = std::reference_wrapper<cell_t>;

bool is_outside(const map_t& map, int row, int col) {
	return row < 0 || row >= map.size() || col < 0 || col >= map[0].size();
}

bool evolve_beam(map_t& map, beam_t& beam, vector<beam_t>& new_beams) {

	cell_type_t type = map[beam.row][beam.col].type;
	//map[beam.row][beam.col].energy--;
	beam.checkpoint();

	switch (beam.dir) {
	case direction_t::UP:
		switch (type) {
		case cell_type_t::MIRROR_D: /* \ */
			beam.col--;
			beam.dir = direction_t::LEFT;
			break;
		case cell_type_t::MIRROR_U: /* / */
			beam.col++;
			beam.dir = direction_t::RIGHT;
			break;
		case cell_type_t::SPLIT_H:
			if (!is_outside(map, beam.row, beam.col + 1)) 
				new_beams.push_back({ beam.row, beam.col + 1, direction_t::RIGHT, beam.history });

			beam.col--;
			beam.dir = direction_t::LEFT;

			break;
		case cell_type_t::SPLIT_V:
		case cell_type_t::EMPTY:
			beam.row--;
		}
		break;
	case direction_t::DOWN:
		switch (type) {
		case cell_type_t::MIRROR_D:  /* \ */
			beam.col++;
			beam.dir = direction_t::RIGHT;
			break;
		case cell_type_t::MIRROR_U:  /* / */
			beam.col--;
			beam.dir = direction_t::LEFT;
			break;
		case cell_type_t::SPLIT_H:
			if (!is_outside(map, beam.row, beam.col + 1))
				new_beams.push_back({ beam.row, beam.col + 1, direction_t::RIGHT, beam.history });
			
			beam.col--;
			beam.dir = direction_t::LEFT;

			break;
		case cell_type_t::SPLIT_V:
		case cell_type_t::EMPTY:
			beam.row++;
		}
		break;
	case direction_t::RIGHT:
		switch (type) {
		case cell_type_t::MIRROR_D:  /* \ */
			beam.row++;
			beam.dir = direction_t::DOWN;
			break;
		case cell_type_t::MIRROR_U:  /* / */
			beam.row--;
			beam.dir = direction_t::UP;
			break;
		case cell_type_t::SPLIT_V:
			if (!is_outside(map, beam.row + 1, beam.col))
				new_beams.push_back({ beam.row + 1, beam.col, direction_t::DOWN, beam.history });

			beam.row--;
			beam.dir = direction_t::UP;

			break;
		case cell_type_t::SPLIT_H:
		case cell_type_t::EMPTY:
			beam.col++;
		}
		break;
	case direction_t::LEFT:
		switch (type) {
		case cell_type_t::MIRROR_D:  /* \ */
			beam.row--;
			beam.dir = direction_t::UP;
			break;
		case cell_type_t::MIRROR_U:  /* / */
			beam.row++;
			beam.dir = direction_t::DOWN;
			break;
		case cell_type_t::SPLIT_V:
			if (!is_outside(map, beam.row + 1, beam.col))
				new_beams.push_back({ beam.row + 1, beam.col, direction_t::DOWN, beam.history });

			beam.row--;
			beam.dir = direction_t::UP;

			break;
		case cell_type_t::SPLIT_H:
		case cell_type_t::EMPTY:
			beam.col--;
		}
		break;
	}

	//gone outsize
	if (is_outside(map, beam.row, beam.col))
		return false;

	map[beam.row][beam.col].energy++;
	return true;

}

 int count_energized(map_t& map, beam_t initial) {
	vector<beam_t> beams{ initial };
	vector<beam_t> outside{};
	vector<beam_t> loops{};

	map[initial.row][initial.col].energy++;

	while (beams.size() > 0) {
		vector<beam_t> new_beams;
		vector<beam_t> to_remove;

		for (beam_t& beam : beams)
			if (!evolve_beam(map, beam, new_beams)) {
				to_remove.push_back(beam);
				outside.push_back(beam);
			}

		//remove beams fallen outside
		for (auto& el : to_remove)
			beams.erase(std::remove(beams.begin(), beams.end(), el), beams.end());

		//remove beams that are part of a loop (same position and direction as history
		to_remove.clear();
		vector<beam_t> new_to_remove;

		for (auto& el : new_beams) {
			//chek if el is in an existing loop
			for (auto& beam : loops) {
				if (el < beam) {
					new_to_remove.push_back(el);
					to_remove.push_back(beam);
				}
			}
			// check if new el is in existing beam history (hence a new loop)
			// so if beam is in the past of el
			for (auto& beam : beams) {
				if (beam <= el) {
					to_remove.push_back(beam);
					loops.push_back(el);
				}
			}
			// or if new el is in a path the leads to outside
			// so if el is in the past of beam fallen outside
			for (auto& beam : outside) {
				if (el < beam) {
					to_remove.push_back(beam);
				}
			}
		}

		//remove loops from both beams and new_beams
		for (auto& beam : to_remove)
			beams.erase(std::remove(beams.begin(), beams.end(), beam), beams.end());
		for (auto& beam : new_to_remove)
			new_beams.erase(std::remove(new_beams.begin(), new_beams.end(), beam), new_beams.end());

		//insert all remaining
		beams.insert(beams.end(), new_beams.begin(), new_beams.end());
		for (auto& new_beam : new_beams)
			map[new_beam.row][new_beam.col].energy++;
	}

	int count = 0;

	for (auto& row : map) {
		for (auto& cell : row) {
			count += cell.energy > 0 ? 1 : 0;
			//cout << cell.energy;
		}
		//cout << endl;
	}

	return count;
}

int main() {
	cout << " AoC 2023 Day16" << endl;

	map_t map;

	ifstream input("Day16.txt");

	int row_id = 0;

	while (!input.eof()) {
		string line;
		getline(input, line);

		row_id++;
		int col_id = 0;
		row_t row;

		for (char c : line) {
			switch (c) {
			case '.':
				row.push_back({ cell_type_t::EMPTY});
				break;
			case '/':
				row.push_back({ cell_type_t::MIRROR_U });
				break;
			case '\\':
				row.push_back({ cell_type_t::MIRROR_D });
				break;
			case '-':
				row.push_back({ cell_type_t::SPLIT_H });
				break;
			case '|':
				row.push_back({ cell_type_t::SPLIT_V });
				break;
			}
			col_id++;
		}

		map.push_back(row);

	}

	input.close();

	int max_count = 0;

	for (int row = 0; row < map.size(); row++) {
		map_t new_map = map;
		int count = count_energized(new_map, { row, 0, direction_t::RIGHT });
		cout << "row " << row << " ->: " << count << endl;
		max_count = max(max_count, count);

		new_map = map;
		count = count_energized(new_map, { row, (int)new_map[row].size() - 1, direction_t::LEFT});
		cout << "row " << row << " <-: " << count << endl;
		max_count = max(max_count, count);
	}

	for (int col = 0; col < map[0].size(); col++) {
		map_t new_map = map;
		int count = count_energized(new_map, { 0, col, direction_t::DOWN });
		cout << "col " << col << " v: " << count << endl;
		max_count = max(max_count, count);

		new_map = map;
		count = count_energized(new_map, { (int)new_map.size() - 1, col, direction_t::UP});
		cout << "col " << col << " ^: " << count << endl;
		max_count = max(max_count, count);
	}

	cout << "Energized: " << max_count << endl;

	return 0;
}