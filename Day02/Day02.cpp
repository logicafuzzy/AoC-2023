#include <iostream>

#include <fstream>
#include <string>
#include <sstream>

#include <regex>

using namespace std;

struct Game {
	int id{ 0 };
	int maxRed{ 0 };
	int maxGreen{ 0 };
	int maxBlue{ 0 };

	void addGrab(int red, int green, int blue) {
		if (red > maxRed)
			maxRed = red;
		if (green > maxGreen)
			maxGreen = green;
		if (blue > maxBlue)
			maxBlue = blue;
	}

	void print() {
		cout << "Game: " << id << " maxRed: " << maxRed << " maxGreen: " << maxGreen << " maxBlue: " << maxBlue << endl;
	}

	bool operator<=(const Game& other) const {
		return maxRed <= other.maxRed && maxGreen <= other.maxGreen && maxBlue <= other.maxBlue;
	}

	long power() const {
		return maxRed * maxGreen * maxBlue;
	}

};

Game getGame(string line) {
	Game game;

	stringstream sline(line);

	sline >> game.id;

	while (!sline.eof()) {
		int value;
		sline >> value;
		string color;
		sline >> color;
		if (color.compare("red") == 0)
			game.addGrab(value, 0, 0);
		else if (color.compare("green") == 0)
			game.addGrab(0, value, 0);
		else if (color.compare("blue") == 0)
			game.addGrab(0, 0, value);
		else
			cout << "Unknown color: " << color << endl;
	}

	return game;
}

int main() {
	cout << " AoC 2023 Day02" << endl;

	ifstream input("Day02.txt");

	Game ref{ 0, 12, 13, 14 };

	int sumIds = 0;

	long sumpower = 0;

	while (!input.eof()) {
		string line;
		getline(input, line);

		Game game = getGame(line);
		if (game <= ref) {
			sumIds += game.id;
			game.print();
		}
		else
			cout << "Game: " << game.id << " is not a match" << endl;

		cout << "Game.id: " << game.id << " minPower: " << game.power() << endl;

		sumpower += game.power();
	}

	cout << " sumIds: " << sumIds << endl;
	cout << " sumpower: " << sumpower << endl;

	input.close();

	return 0;
}