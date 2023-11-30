#include <iostream>

#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main() {
	cout << " AoC 2023 Day01" << endl;

	ifstream input("Day01.txt");

	while (!input.eof()) {
		string line;
		getline(input, line);
		std::stringstream sline(line);
	}

	input.close();

	return 0;
}