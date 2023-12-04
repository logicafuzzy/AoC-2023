#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <unordered_map>

using namespace std;

constexpr bool isTest = false;
constexpr short nwin = isTest ? 5 : 10;


int main() {
	cout << " AoC 2023 Day04" << endl;

	ifstream input("Day04.txt");

	int id = 1;

	int total = 0;

	//for part 2
	//keep track of how many cards for each id and points per card recorded
	unordered_map<int, int> mult_table;

	while (!input.eof()) {
		string line;
		getline(input, line);
		std::stringstream sline(line);

		set<short> win;

		int read = 0;

		if (mult_table.find(id) == mult_table.end())
			mult_table[id] = 1;

		while (!sline.eof()) {
			int val;
			sline >> val;
			if (read++ < nwin)
				win.insert(val);
			else
				win.erase(val);

		}

		short match = nwin - win.size();
		int points = (match == 0) ? 0 : pow(2, match - 1);

		printf(" Card %d Winning: %d Points: %d \n", id, nwin - win.size(), points);

		total += points;

		for (int i = id + 1; i <= id + match; ++i) {
			if (mult_table.find(i) == mult_table.end())
				mult_table[i] = 1;
			mult_table[i] += mult_table[id];
		}

		id++;
	}

	input.close();

	printf("Points %d\n", total);

	int tot_cards = 0;
	for (auto& n : mult_table) {
		tot_cards += n.second;
	}

	printf("Total cards (part 2): %d", tot_cards);

	return 0;
}
