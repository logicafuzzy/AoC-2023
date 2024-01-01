#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <unordered_map>
#include <limits>
#include <tuple>
#include <queue>
#include <numeric>
#include <cstdlib>
#include <set>

using namespace std;

using cell_t = int;
using row_t = vector<cell_t>;
using map_t = vector<row_t>;

constexpr int max_consecutive_moves = 3;
int col_size;

enum class direction_t {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

using link_t = pair<int, int>;
using adj_t = set<link_t>;

inline int get_id(int row, int col, int col_size) {
	return row * col_size + col;
}

inline pair<int, int> get_coords(int id, int col_size) {
	div_t res = div(id, col_size);
	return { res.quot, res.rem };
}

direction_t get_dir(int from, int to, int col_size) {
	pair<int, int> from_coords = get_coords(from, col_size);
	pair<int, int> to_coords = get_coords(to, col_size);

	if (from_coords.first > to_coords.first)
		return direction_t::UP;

	if (from_coords.first < to_coords.first)
		return direction_t::DOWN;

	if (from_coords.second > to_coords.second)
		return direction_t::LEFT;

	if (from_coords.second < to_coords.second)
		return direction_t::RIGHT;

}

vector<int> get_path(int src, int tar, vector<int> const& prev) {
	vector<int> path;

	for (int v = tar; v != src; v = prev[v])
		path.push_back(v);

	path.push_back(src);

	reverse(path.begin(), path.end());
	return path;
}

void print_path(map_t map, const vector<int>& prev, int target) {
	printf("Path: \n");
	
	vector<int> path = get_path(0, target, prev);

	for (int id : path) {
		auto coords = get_coords(id, col_size);
		map[coords.first][coords.second] = -map[coords.first][coords.second];
	}

	for (const auto& row : map) {
		cout << " ";
		for (const auto& c : row) {
			if (c <= 0)
				printf("\033[1;32m%d\033[0;m", -c);
			else
				printf("%d", c);
		}
		cout << endl;
	}
}

// This class represents a directed graph using
// adjacency list representation
class graph_t {
	int _n; // No. of vertices

	// In a weighted graph, we need to store vertex
	// and weight pair for every edge
	unordered_map<int, adj_t> adj;

public:
	graph_t(int N) 
		: _n(N) 
		, _dist(_n, numeric_limits<int>::max())
		, _prev(_n, -1)
		, _visited(_n, false)
		, _col_size(0)
	{};

	void add(int from, int to, int weight) {
		adj[from].insert({ to, weight });
	}

	int min_dist_index() {
		int min_dist = numeric_limits<int>::max();
		int min_index = 0;

		for (int to = 0; to < _n; to++) {
			if (!_visited[to]
				&& _dist[to] <= min_dist) {

				min_dist = _dist[to];
				min_index = to;
			}
		}

		return min_index;
	}

	int count_same_dir(int from, int to) {
		vector<int> prev = _prev;

		direction_t dir = get_dir(from, to, _col_size);

		prev[to] = from;

		int same_dir = 0;

		for (int i = 0; i <= max_consecutive_moves; ++i) {
			if (prev[to] == -1)
				return same_dir;

			if (get_dir(prev[to], to, _col_size) == dir)
				same_dir++;
			else
				return same_dir;

			to = prev[to];
		}

		return same_dir;
	}

	bool can_move(int from, int to) {
		return count_same_dir(from, to) <= max_consecutive_moves;
	}

	//get adjacency considering max_same_direction_count
	adj_t get_adj(int from) {
		adj_t res;

		for (auto& link : adj[from]) {
			int to = link.first;
			int same_dir = count_same_dir(from, to);
			int weight = link.second;

			if (can_move(from, to)) {
				res.insert({to, weight});
			}
		}

		return res;
	}


	// output shortest path from src node id
	// adaptation of dijstra algorithm
	vector<int> path(int src)
	{
		_dist[src] = 0;

		for (int i = 0; i < _n; i++) {
			int from = min_dist_index();

			_visited[from] = true;

			for (const link_t i : get_adj(from)) {
				int to = i.first;
				int weight = i.second;

				if (!_visited[to]
					&& _dist[from] + weight < _dist[to]
					/* && can_move(_prev, from, to, _col_size)*/) {

					_dist[to] = _dist[from] + weight;
					_prev[to] = from;

				}
			}
		}

		return _dist;
	}

	void print() {
		for (auto& nodes : adj) {
			auto coord = get_coords(nodes.first, _col_size);
			printf("Node %d (%d, %d): \n", nodes.first, coord.first, coord.second);
			for (auto& node : nodes.second) {
				coord = get_coords(node.first, _col_size);
				printf("\t %d -> (%d, %d)\n", node.second, coord.first, coord.second);
			}
			printf("\n");
		}
	}

	vector<int> _dist;
	vector<int> _prev;
	vector<bool> _visited;
	int _col_size; //little hack
};


bool is_outside(const map_t& map, int row, int col) {
	return row < 0 || row >= map.size() || col < 0 || col >= map[0].size();
}

long get_loss(const map_t& map, const vector<int>& prev, int target) {
	long loss = 0;
	int id = target;

	while (id != 0) {
		if (id == -1)
			continue;

		auto coords = get_coords(id, col_size);
		loss += map[coords.first][coords.second];
		id = prev[id];
	}

	return loss;
}


void link(graph_t& graph, const map_t& map, int row, int col, int nextrow, int nextcol) {
	const int ncols = map[0].size();
	if (!is_outside(map, nextrow, nextcol)) {
		int id = get_id(row, col, ncols);
		int new_id = get_id(nextrow, nextcol, ncols);
		
		graph.add(id, new_id, map[nextrow][nextcol]);
	}
}

int main() {
	cout << " AoC 2023 Day17" << endl;

	ifstream input("Day17test.txt");

	map_t map;

	while (!input.eof()) {
		string line;
		getline(input, line);

		row_t row;
		for (char c : line) {
			row.push_back(c - '0');
		}

		map.push_back(row);
	}

	input.close();

	graph_t graph(map.size() * map[0].size());

	col_size = map[0].size();

	graph._col_size = col_size;

	const int nrows = map.size();
	const int ncols = map[0].size();
	
	for (int row = 0; row < nrows; row++) {
		for (int col = 0; col < ncols; col++) {
			int nextrow, nextcol;

			nextrow = row - 1; nextcol = col;
			link(graph, map, row, col, nextrow, nextcol);

			nextrow = row + 1; nextcol = col;
			link(graph, map, row, col, nextrow, nextcol);

			nextrow = row; nextcol = col - 1;
			link(graph, map, row, col, nextrow, nextcol);

			nextrow = row; nextcol = col + 1;
			link(graph, map, row, col, nextrow, nextcol);
	
		}
	}
	
	//cout << endl;
	//graph.print();
	//cout << endl;

	vector<int> path = graph.path(0);
	cout << "Min loss (modified) : " << path[map.size() * map[0].size() - 1] << endl;
	cout << "Min loss (actual): " << get_loss(map, graph._prev, map.size() * map[0].size() - 1) << endl;

	print_path(map, graph._prev, map.size() * map[0].size() - 1);

	return 0;
}
