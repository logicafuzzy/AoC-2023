#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>

using namespace std;
using lens_t = std::pair<string, short>;
using box_t = vector<lens_t>;

uint8_t compute_hash(string s) {
	int val = 0;
	for (char c : s)
		val = ((val + c) * 17) % 256;
	
	return val;
}

int main() {
	cout << " AoC 2023 Day15" << endl;

	constexpr bool part1 = false;

	int sum = 0;
	
	box_t empty = {};
	vector<box_t> map(256, empty);

	ifstream input("Day15.txt");



	while (!input.eof()) {
		string line;
		getline(input, line);
		std::stringstream sline(line);

		string val;

			while (!sline.eof()) {
				sline >> val;

				if (part1) {
					sum += compute_hash(val);
				}
				else {
					int minus = val.find('-');
					if (minus != string::npos) {
						string label = val.substr(0, minus);
						int index = compute_hash(label);
						
						box_t& box = map[index];

						
						box.erase(std::remove_if(box.begin(), box.end(), [&label](auto& el) {
							return el.first == label;
							}), box.end());
					}
					else {
						//command == '='
						int equal = val.find('=');
						int lens = val[equal + 1] - '0';

						string label = val.substr(0, equal);
						int index = compute_hash(label);

						box_t& box = map[index];

						auto found = std::find_if(box.begin(), box.end(), [&label, lens](auto& el) {
							if (el.first == label) {
								el.second = lens;
								return true;
							}
							return false;
							});

						if (found == box.end())
							box.push_back({ label, lens });

					}
				}
			}

			int box_id = 0;
			sum = 0;
			for (box_t& box : map) {
				if (box.size() > 0) {
					cout << "Box " << box_id << ": ";
					int slot = 0;
					for (lens_t lens : box) {
						cout << "[" << lens.first << lens.second << "]";
						sum += (box_id + 1) * (slot + 1) * lens.second;
						slot++;
					}
					cout << endl;
				}
				box_id++;
			}


	}

	input.close();

	printf("Total sum: %d", sum);

	return 0;
}
