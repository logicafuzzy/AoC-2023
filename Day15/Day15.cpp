#include <iostream>

#include <fstream>
#include <string>
#include <sstream>

using namespace std;

uint8_t compute_hash(string s) {
	int val = 0;
	for (char c : s)
		val = ((val + c) * 17) % 256;
	
	return val;
}

int main() {
	cout << " AoC 2023 Day15" << endl;

	int sum = 0;

	ifstream input("Day15.txt");

	while (!input.eof()) {
		string line;
		getline(input, line);
		std::stringstream sline(line);

		string val;
		
		while (!sline.eof()) {
			sline >> val;
			sum += compute_hash(val);
		}
	}

	input.close();

	printf("Total sum: %d", sum);

	return 0;
}
