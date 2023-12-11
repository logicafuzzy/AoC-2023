#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

using num_t = long;
using seq_t = vector<num_t>;
using diff_t = vector<seq_t>;

seq_t diff(const vector<num_t> seq) {
	seq_t result;

	for (size_t i = 1; i < seq.size(); ++i)
		result.push_back(seq[i] - seq[i - 1]);

	return result;
}

bool all_equal(const vector<num_t>& values) {
	for (num_t s : values)
		if (s != values[0])
			return false;

	return true;
}

void print(const vector<num_t>& values) {
	for (num_t v : values)
		cout << " " << v << " ";
	cout << endl;
}

int main() {
	cout << " AoC 2023 Day09" << endl;

	ifstream input("Day09.txt");

	num_t sum_next = 0;
	num_t sum_prev = 0;

	while (!input.eof()) {
		string line;
		getline(input, line);
		std::stringstream sline(line);

		diff_t der;
		seq_t input;

		while (!sline.eof()) {
			num_t value;
			sline >> value;
			input.push_back(value);
		}

		der.push_back(input);
		//print(input);

		int i = 0;

		while (!all_equal(der[i])) {
			der.push_back(diff(der[i++]));
			//print(der[i]);
		}

		num_t next_pred = 0;
		num_t prev_pred = 0;
		while(der.size() > 1) {
			next_pred = der.back().back();
			prev_pred = der.back()[0];
			der.pop_back();
			der.back().push_back(next_pred + der.back().back());
			der.back().insert(der.back().begin(), der.back()[0] - prev_pred);
		}

		printf("Predicted next value: %ld \n", der.back().back());
		printf("Predicted prev value: %ld \n", der.back()[0]);

		sum_next += der.back().back();
		sum_prev += der[0][0];

	}

	input.close();

	printf("Sum next: %ld\n", sum_next);
	printf("Sum prev: %ld\n", sum_prev);

	return 0;
}
