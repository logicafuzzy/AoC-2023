#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

#define PART2

//note for part 2 J is before 2
enum cards {
#ifdef PART2
	J,
#endif
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,
	T,
#ifndef PART2
	J,
#endif
	Q,
	K,
	A
};

enum types {
	CARD,
	PAIR,
	TWO_PAIR,
	THREE,
	FULL,
	FOUR,
	FIVE
};

using hand_t = vector<cards>;

unordered_map<cards, int> get_hand_set(const hand_t hand) {
	unordered_map<cards, int> hand_set;

	for (auto card : hand) {
		if (hand_set.find(card) == hand_set.end())
			hand_set[card] = 1;
		else
			hand_set[card]++;
	}

	return hand_set;
}

enum types get_type(const hand_t& hand) {
	unordered_map<cards, int> hand_set = get_hand_set(hand);

	switch (hand_set.size()) {
	case 1:
		return FIVE;
	case 2:
		switch (hand_set.begin()->second) {
		case 2:
		case 3:
			return FULL;
		case 1:
		case 4:
			return FOUR;
		}
	case 3:
		switch (hand_set.begin()->second) {
		case 3:
			return THREE;
		case 2:
			return TWO_PAIR;
		case 1:
			if (std::next(hand_set.begin(), 1)->second == 3 || std::next(hand_set.begin(),2)->second == 3)
				return THREE;
			else
				return TWO_PAIR;
		}
	case 4:
		return PAIR;
	case 5:
		return CARD;
	}
}

hand_t replace(const hand_t& hand, enum cards what, enum cards with) {
	hand_t result;

	for (enum cards card : hand) {
		result.push_back(card == what ? with : card);
	}

	return result;
}

int count_card(const hand_t& hand, enum cards card) {
	return count_if(hand.begin(), hand.end(), [&card](enum cards el) {return card == el; });
}

enum types get_highest_type(const hand_t& hand) {
	int js = count_card(hand, cards::J);
	
	if (js == 0 || js == 5)
		return get_type(hand);

	enum types max_type = types::CARD;

	for (enum cards card : hand) {
		if (card == cards::J)
			continue;
		else
			max_type = max(max_type, get_type(replace(hand, cards::J, card)));
	}

	return max_type;
}

int compare(const hand_t& first, const hand_t& second) {

#ifdef PART2
	enum types first_type = get_highest_type(first);
	enum types second_type = get_highest_type(second);
#else
	enum types first_type = get_type(first);
	enum types second_type = get_type(second);
#endif

	if (first_type != second_type)
		return first_type > second_type ? 1 : -1;
	else
	{
		for (int i = 0; i < first.size(); ++i) {
			if (first[i] > second[i])
				return 1;
			else if (first[i] < second[i])
				return -1;
			else
				continue;
		}
	}

	return 0;
};

hand_t get_hand(const string& input) {
	hand_t result;
	for (char c : input) {
		switch (c) {
		case '2':
			result.push_back(_2);
			break;
		case '3':
			result.push_back(_3);
			break;
		case '4':
			result.push_back(_4);
			break;
		case '5':
			result.push_back(_5);
			break;
		case '6':
			result.push_back(_6);
			break;
		case '7':
			result.push_back(_7);
			break;
		case '8':
			result.push_back(_8);
			break;
		case '9':
			result.push_back(_9);
			break;
		case 'T':
			result.push_back(T);
			break;
		case 'J':
			result.push_back(J);
			break;
		case 'Q':
			result.push_back(Q);
			break;
		case 'K':
			result.push_back(K);
			break;
		case 'A':
			result.push_back(A);
			break;
		}
	}
	return result;
}

string hand_to_string(const hand_t& hand) {
	string result;
	for (enum cards card : hand)
		result += (char)(card + '1');

	return result;
}

int main() {
	cout << " AoC 2023 Day07" << endl;

	ifstream input("Day07.txt");

	vector<hand_t> hands;
	unordered_map<string, int> bidding;

	while (!input.eof()) {
		string line;
		getline(input, line);
		std::stringstream sline(line);

		string hand_s;
		int bid;

		sline >> hand_s >> bid;

		hand_t hand = get_hand(hand_s);

		bidding[hand_to_string(hand)] = bid;
		hands.push_back(hand);		
		printf("hand: %s bid: %d hash %s\n", hand_s.c_str(), bid, hand_to_string(hand).c_str());
	}

	input.close();

	sort(hands.begin(), hands.end(), 
		[](const hand_t& first, const hand_t& second) {
			return compare(first, second) < 0;
		});

	long sum = 0;
	int rank = 1;
	for (hand_t& hand : hands) {
		sum += bidding[hand_to_string(hand)] * rank++;
	}

	cout << "Sum: " << sum << endl;

	return 0;
}
