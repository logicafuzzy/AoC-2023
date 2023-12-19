#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace std;

enum class attr_t {
	x,
	m,
	a,
	s
};

enum class cmp_t {
	lt,
	gt,
	nop
};

struct part_t {
	int x{};
	int m{};
	int a{};
	int s{};

	int get(attr_t attr) const {
		switch (attr) {
		case attr_t::x:
			return x;
		case attr_t::m:
			return m;
		case attr_t::a:
			return a;
		case attr_t::s:
			return s;
		}
	}

	int sum() const {
		return x + m + a + s;
	}
};

struct rule_t {
	attr_t attr{};
	cmp_t cmp{};
	int val{};
	string dst{};
};

unordered_map<string, vector<rule_t>> workflows;
unordered_map<string, vector<part_t>> status;

attr_t get_attr(char c) {
	switch (c) {
	case 'x':
		return attr_t::x;
	case 'm':
		return attr_t::m;
	case 'a':
		return attr_t::a;
	case 's':
		return attr_t::s;
	}
}

bool is_attr(string attr) {
	return attr == "x" || attr == "m" || attr == "a" || attr == "s";
}

void parse_rule(string line) {
	std::stringstream sline(line);

	string name; sline >> name;
	string ignore;
	sline >> ignore;

	string token;
	int el;
	rule_t rule;
	string tmp;
	do {
		sline >> token;
		if (is_attr(token)) {
			rule.attr = get_attr(token[0]);
			sline >> token;
			if (token == ">") {
				rule.cmp = cmp_t::gt;
				sline >> rule.val;
			}
			else if (token == "<") {
				rule.cmp = cmp_t::lt;
				sline >> rule.val;
			}
			sline >> ignore;
			sline >> rule.dst;
		}
		else {
			rule.dst = token;
			rule.cmp = cmp_t::nop;
			workflows[name].push_back(rule);
			break;
		}
		sline >> token;

		if (token == "," || token == "}")
			workflows[name].push_back(rule);

	} while (token != "}");

}

part_t parse_part(string attribs) {
	std::stringstream spart(attribs);

	string attr;
	string ignore;
	int value;

	spart >> ignore; // the first {
	
	part_t part;

	do {
		spart >> attr;
		spart >> ignore; // =
		spart >> value;
		spart >> ignore;

		switch (attr[0]) {
		case 'x':
			part.x = value;
			break;
		case 'm':
			part.m = value;
			break;
		case 'a':
			part.a = value;
			break;
		case 's':
			part.s = value;
			break;
		}


	} while (ignore != "}");

	return part;
}

bool process(const rule_t& rule, const part_t& part) {
	if (rule.cmp == cmp_t::nop) {
		return true;
	}

	if (rule.cmp == cmp_t::gt) {
		if (part.get(rule.attr) > rule.val) {
			return true;
		}
	}
	else {
		if (part.get(rule.attr) < rule.val) {
			return true;
		}
	}

	return false;
}

string process(const vector<rule_t>& workflow, const part_t& part) {
	string dst = "";

	for (const rule_t& rule : workflow) {
		if (process(rule, part)) {
			dst = rule.dst;
			break;
		}
	}

	if (dst != "A" && dst != "R")
		dst = process(workflows[dst], part);
	else 
		status[dst].push_back(part);
	
	return dst;
}

int main() {
	cout << " AoC 2023 Day19" << endl;

	ifstream input("Day19.txt");

	bool rules = true;

	while (!input.eof()) {
		string line;
		getline(input, line);
		

		if (line.size() > 0)
			if (rules)
				parse_rule(line);
			else {
				// starts from "in" rule;
				process(workflows["in"], parse_part(line));
			}
		else
			rules = false;
	}

	input.close();

	long sum = 0;
	for (const part_t& part : status["A"])
		sum += part.sum();

	cout << "Sum: " << sum << endl;

	return 0;
}
