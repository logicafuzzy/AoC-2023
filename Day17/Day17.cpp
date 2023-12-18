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
using adj_t = vector<link_t>;

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

bool can_move(vector<int> prev, int from, int to, int col_size) {
	//don't want to find max_consecutive_moves same directions 
	bool result = true;
	direction_t dir = get_dir(from, to, col_size);

	prev[to] = from;

	for (int i = 0; i <= max_consecutive_moves; ++i) {
		if (prev[to] == -1)
			return true;

		result = result && get_dir(prev[to], to, col_size) == dir;
		to = prev[to];
	}

	return !result;

}

void print_path(map_t map, const vector<int>& prev, int target) {
	printf("Path: \n");
	//printf("Can move: ");
	int id = target;
	
	int path = 0;

	while (id != 0) {
		if (id == -1)
			continue;

		//cout << can_move(prev, prev[id], id, col_size);

		auto coords = get_coords(id, col_size);
		map[coords.first][coords.second] = -map[coords.first][coords.second] ;
		id = prev[id];
	}

	//cout << endl;

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
		, dist(_n, numeric_limits<int>::max())
		, prev(_n, -1)
		, visited(_n, false)
	{};

	void add(int from, int to, int weight) {
		adj[from].push_back({ to, weight });
	}

	// output shortest path from src node id
	// adaptation of dijstra algorithm
	vector<int> path(int src)
	{
		dist[src] = 0;

		int current = src;

		/* Looping till priority queue becomes empty (or all
		distances are not finalized) */
		while (true) {
			

			// 'i' is used to get all adjacent vertices of a
			// vertex
			for (link_t& i : adj[u]) {
				// Get vertex label and weight of current
				// adjacent of u.
				int v = i.first;
				int weight = i.second;

				// If there is shorted path to v through u.
				if (can_move(prev, u, v, col_size) && dist[v] > dist[u] + weight) {
					// Updating distance of v
					dist[v] = dist[u] + weight;	
					prev[v] = u;

					pq.push({ dist[v], v });

					//print_path(map, prev, v);
					//cout << endl << endl;
				}
			}
		}

		return dist;
	}

	vector<int> dist;
	vector<int> prev;
	vector<bool> visited;

	map_t map;
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
		//graph.add(new_id, id, map[row][col]);
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
	
	graph.map = map;

	col_size = map[0].size();

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

	vector<int> path = graph.path(0);
	cout << "Min loss : " << path[map.size() * map[0].size() - 1] << endl;
	//cout << "Min loss2: " << get_loss(map, graph.prev, map.size() * map[0].size() - 1) << endl;

	print_path(map, graph.prev, map.size() * map[0].size() - 1);

	return 0;
}
