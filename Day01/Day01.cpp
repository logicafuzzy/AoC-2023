#include <iostream>

#include <fstream>
#include <string>
#include <sstream>

#include <regex>

using namespace std;

int isTextDigit(const string& text, int index) {
	if (text.size() < index + 3)
		return 0;
	else {
		if (text[index] == 'o' && text[index + 1] == 'n' && text[index + 2] == 'e')
			return 1;
		if (text[index] == 't' && text[index + 1] == 'w' && text[index + 2] == 'o')
			return 2;
		if (text[index] == 's' && text[index + 1] == 'i' && text[index + 2] == 'x') 
			return 6;
	}

	if (text.size() < index + 4)
		return 0;
	else {

		if (text[index] == 'f' && text[index + 1] == 'o' && text[index + 2] == 'u' && text[index + 3] == 'r')
			return 4;
		if (text[index] == 'f' && text[index + 1] == 'i' && text[index + 2] == 'v' && text[index + 3] == 'e')
			return 5;
		if (text[index] == 'n' && text[index + 1] == 'i' && text[index + 2] == 'n' && text[index + 3] == 'e')
			return 9;
	}

	if (text.size() < index + 5)
		return 0;
	else {
		if (text[index] == 't' && text[index + 1] == 'h' && text[index + 2] == 'r' && text[index + 3] == 'e' && text[index + 4] == 'e')
			return 3;
		if (text[index] == 's' && text[index + 1] == 'e' && text[index + 2] == 'v' && text[index + 3] == 'e' && text[index + 4] == 'n')
			return 7;
		if (text[index] == 'e' && text[index + 1] == 'i' && text[index + 2] == 'g' && text[index + 3] == 'h' && text[index + 4] == 't')
			return 8;
	}

	return 0;
}

int main() {
	cout << " AoC 2023 Day01" << endl;

	ifstream input("Day01.txt");

	long sum = 0;

	while (!input.eof()) {
		string line;
		getline(input, line);

		cout << "Line: " << line << endl;

		int findFirst = true;
		int first = 0, second = 0;

		for (int i = 0; i < line.length(); i++) {
			int digit = isTextDigit(line, i);
			if (line[i] > '0' && line[i] <= '9' || digit > 0) {
				if (findFirst) {
					first = digit > 0 ? digit : line[i] - '0';
					second = first;
					findFirst = false;
				}
				else {
					second = digit > 0 ? digit : line[i] - '0';
				}
			}
		}

		int result = first * 10 + second;
		cout << "Number: " << result << endl;

		sum += result;
		
	}

	cout << "Result: " << sum << endl;

	input.close();

	return 0;
}