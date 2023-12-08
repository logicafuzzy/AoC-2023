#include <iostream>

#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main() {
	cout << " AoC 2023 Day09" << endl;

	ifstream input("Day09test.txt");

	while (!input.eof()) {
		string line;
		getline(input, line);
		std::stringstream sline(line);

		int a, b, c, d, e;
		sline >> a >> b >> c >> d >> e;

		cout << a << " " << b << " " << c << " " << d << " " << e << endl;
	}

	input.close();

	return 0;
}
