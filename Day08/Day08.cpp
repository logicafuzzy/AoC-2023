#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <tuple>

using namespace std;
using map_t = unordered_map<string, pair<string, string>>;

long distance(map_t& nodes, const vector<bool>& choices, const string& start, const string& end) {
	long result = 0;

	string current = start;

	while (current != end) {
		current = choices[result++ % choices.size()] ? nodes[current].second : nodes[current].first;
	}

	return result;
}

pair<long, string> distance2(map_t& nodes, const vector<bool>& choices, const string& start, const char end, long cursor = 0) {
	long result = cursor;

	string current = start;

	do {
		current = choices[result++ % choices.size()] ? nodes[current].second : nodes[current].first;
	} while (current[2] != end);

	return { result, current };
}

bool all_equal(const vector<long>& steps) {
	for (long s : steps)
		if (s != steps[0])
			return false;

	return true;
}


long get_min_index(const vector<long>& values) {
	long min = values[0];
	int result = 0;

	for (int i = 1; i < values.size(); ++i) {
		if (values[i] < min) {
			result = i;
			min = values[i];
		}
	}
		
	return result;
}

void print(const vector<long>& values) {
	for (long v : values)
		cout << " " << v << " ";
	cout << endl;
}

int main() {
	cout << " AoC 2023 Day03" << endl;

	const string start{ "AAA" };
	const string end("ZZZ");
	constexpr char end_char = 'Z';

	vector<string> starts;

	vector<bool> choices;

	map_t nodes;

	ifstream input("Day08.txt");

	string line;
	getline(input, line);

	for (char c : line)
		choices.push_back(c == 'L' ? 0 : 1);


	while (!input.eof()) {

		getline(input, line);
		std::stringstream sline(line);

		string node, left, right;
		sline >> node >> left >> right;

		nodes[node] = make_pair(left, right);

		if (node[2] == 'A')
			starts.push_back(node);
	}

	input.close();

	//navigate graph

	long step = 0;

	//step = distance(nodes, choices, start, end);

	printf("Part1: \n");
	printf("Steps: %d \n", step);

	printf("Part2: \n");

	vector<string> currents = starts;
	vector<long> steps(currents.size(), 0);

	do {
		int i = get_min_index(steps);
		
		long s = 0;
		bool store = false;

		tie(steps[i], currents[i]) = distance2(nodes, choices, currents[i], end_char, steps[i]);


		if (i == 0)
			print(steps);

	} while (!all_equal(steps));

	printf("Steps: %d", steps[0]);

	return 0;
}
