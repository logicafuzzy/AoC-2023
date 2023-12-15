#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <execution>

using namespace std;

using num_t = uint8_t;

struct condition_t {
	string record{};
	vector<num_t> positions{};
	uint16_t id{};
	int32_t match{-1};
};

ostream& operator<<(ostream& os, const vector<num_t>& vector)
{
	for (auto element : vector) {
		os << (int)element << " ";
	}
	return os;
}

condition_t get_condition(const string& line, uint16_t id) {
	stringstream sline(line);

	string record, arrangement;
	getline(sline, record, ' ');
	getline(sline, arrangement);

	//printf("Record: %s Arrangement %s\n", record.c_str(), arrangement.c_str());

	vector<num_t> positions;

	stringstream sarrangement(arrangement);
	string position;

	while (getline(sarrangement, position, ',')) {
		positions.push_back(stoi(position));
		//printf("\t Position: %d\n", positions.back());
	}

	return { record, positions, id };
}

bool match(const string&& record, const string& pattern) {
	if (record.size() != pattern.size())
		return false;

	for (int i = 0; i < record.size(); ++i) {
		if (pattern[i] != '?' && record[i] != pattern[i])
			return false;
	}

	return true;
}

bool match_fast(const condition_t& condition, const vector<num_t> dots) {
	const size_t record_size = condition.record.size();
	const size_t ndots = dots.size();
	const size_t npos = condition.positions.size();
	
	int cursor = 0;

	int idot = 0; int ipos = 0;
	int dotindex = (dots[0] == 0 ? 1 : 0), posindex = 0;
	bool bdot = dots[0] != 0;

	while (cursor < record_size && ((bdot && dotindex < ndots) || (!bdot && posindex < npos))) {
		char record = condition.record[cursor++];

		//if (bdot && dots[dotindex] == 0) {
		//	dotindex++;
		//	bdot = false;
		//}

		if (record != '?' && record != (bdot ? '.' : '#')) {
			return false;
		}

		if (bdot && ++idot == dots[dotindex]) {
				++dotindex;
				idot = 0;
				bdot = false;
		}
		else if (!bdot && ++ipos == condition.positions[posindex]) {
			++posindex;
			ipos = 0;
			bdot = true;
		}

	}

	return true;
}

string make_record(const condition_t& condition, const vector<num_t>& dots) {
	stringstream sresult;
	if (dots.size() > condition.positions.size()) {
		for (int i = 0; i < condition.positions.size(); i++) {
			sresult << string(dots[i], '.') << string(condition.positions[i], '#');
		}
		sresult << string(dots.back(), '.');
	}
	else {
		size_t size = min(condition.positions.size(), dots.size());

		for (int i = 0; i < size; i++) {
			sresult << string(dots[i], '.') << string(condition.positions[i], '#');
		}
	}

	return sresult.str();
}

// unused - works but very inefficient
vector<vector<num_t>> make_dots(const int positions, const int sum) {
	// inspired by: 
	// https://codegolf.stackexchange.com/questions/220718/create-all-arrays-of-non-negative-integers-of-length-n-with-sum-of-parts-equal-t
	// https://tio.run/##hVJdU8IwEHzvr7ip40wzTQC/XkiLP0QdpoQAqZB20mt1xPrTxSRFsDhoXi693b3sJhVlyZZC7HYXalMWBpOZwmpY4VzE8WA1CbIaC1hESiNoCq4g2Vp4PG6kwMIktjUxfFEYz1FpWbxEcYxUE9rQnCvGePP@HplBWVer6SwTz5EidESIkzQpsiuap4rnPB@mSBqW5pfIjcTaaDC8tb60WNdzmaiiQiOzzSRwB20ypSMSbAOw69QPoKxwqiGF7R2FWwo3Lf@DiI54TcFx7/ZM6w66RBYccVuS/dRBpd5kRIBDHCsCnQO3/B1Zdkd7UE@8B@E3hD3IWxJFjZAkEDp96Hbaf1Kv8o0OBzZ51OFR7Z/HTbW0RWRfCMkRPGSQa7mBsef9NNzPqefy1WftdtYCd7phyj6Q2qiue6r@FSDqxPfWaWhPDEPium7OpR3De@I2ODsm7IVsz1wW@28dprSBL/vfasSDdvcpFutsWe3Yyxc

	int s = sum;
	vector<num_t> res;
	for (uint64_t i = (uint64_t)pow(++s, positions), v, j; i--; v || (res.push_back(i), 0))
		for (v = s - 1, j = i; j; j /= s)
			v -= j % s; 

	vector<vector<num_t>> combinations;
	for (int elem : res) {
		vector<num_t> combination;
		bool discard = false;
		for (uint64_t index = 0; index < positions; elem /= -~sum, ++index) {
			int val = elem % -~sum;
			// 0 only allowed at beginning or end
			if (index == 0 || index > 0 && val > 0 || index == positions - 1)
				combination.push_back(val);
			else {
				discard = true;
				break;
			}
		}

		if (!discard)
			combinations.push_back(combination);
	}

	return combinations;
}

/*
* inspired from https://stackoverflow.com/a/56726334
def partitions_nonnegative_fixed_length_ordered(n, r):
	"""Generate the partitions of the nonnegative integer `n` as the
	positions of `r` nonnegative integers, where the order of the integers
	matters. The partitions are tuples and are generated in
	lexicographic order. The number of partitions generated is
	binomialcoefficient(n+r-1, r-1).

	NOTE:   The empty generator is returned for n=r=0, though arguably
			the generator yielding just the empty tuple would satisfy
			the conditions.
	"""
	def partitions_prefixed(prefix, n, r):
		if r == 1:
			yield prefix + (n,)
		else:
			for i in range(n + 1):
				yield from partitions_prefixed(prefix + (i,), n - i, r - 1)

	if n >= 0 and r >= 1 and n == int(n) and r == int(r):
		yield from partitions_prefixed(tuple(), int(n), int(r))
*/

// unused - only for testing
vector<vector<num_t>>& filter_partitions(vector<vector<num_t>>&& partitions) {
	//remove all combinations with zeroes inside
	partitions.erase(remove_if(partitions.begin(), partitions.end(), [](auto& partition) {
		for (int i = 1; i < partition.size() - 1; i++)
			if (partition[i] == 0)
				return true;
		return false;
		}), partitions.end());

	return partitions;
}

vector<vector<num_t>> partitions_prefixed(const vector<num_t>& partition, const int sum, const int positions, const condition_t& condition, uint64_t& accumulator ) {
	if (positions == 1) {
		vector<num_t> prefixed = partition;
		prefixed.push_back(sum);
		
		if (match_fast(condition, prefixed))
			++accumulator;

		return {};
	}
	
	vector<vector<num_t>> result;
	vector<num_t> prefixed = partition;
	prefixed.push_back(0);
	//zeroes can be only at the beginning
	for (int i = (partition.size() == 0 ? 0 : 1); i <= sum; i++) {

		prefixed.back() = i;

		if (!match_fast(condition, prefixed))
			continue;

		auto iteration = partitions_prefixed(prefixed, sum - i, positions - 1, condition, accumulator);
		iteration.erase(remove_if(iteration.begin(), iteration.end(), [&condition](auto& prefix) {
			return !match_fast(condition, prefix);
			}), iteration.end());

		result.insert(result.end(), iteration.begin(), iteration.end());
	}
	return result;
}

uint64_t make_partition(const int positions, const int sum, const condition_t& condition) {
	uint64_t accumulator = 0;

	if (sum >= 0 and positions >= 1)
		partitions_prefixed({}, sum, positions, condition, accumulator);

	return accumulator;
}

long count_combinations(const condition_t& condition) {
	size_t N = condition.record.size();
	int nDots = N - reduce(condition.positions.begin(), condition.positions.end());

	vector<num_t> dots(condition.positions.size() + 1, 0);

	return make_partition(condition.positions.size() + 1, nDots, condition);

}

int main() {
	constexpr bool isPart2 = true;
	constexpr int repeat = 5;

	cout << " AoC 2023 Day12" << endl;

	ifstream input("Day12.txt");

	vector<condition_t> conditions;

	long sum = 0;

	uint16_t id = 0;
	while (!input.eof()) {
		string line;
		getline(input, line);

		conditions.push_back(get_condition(line, id++));
	}

	input.close();

	if (!isPart2) {

		int index = 0;
		for (const auto& condition : conditions) {
			sum += count_combinations(condition);
			printf("%d of %lld           \r", ++index, conditions.size());
		}

		printf("Combinations: %d", sum);
	}
	else
	{

		ifstream saved("output.txt");

		while (!saved.eof()) {
			string line;
			
			getline(saved, line);

			if (line == "")
				break;

			stringstream sline(line);
			int id;
			long result;
			sline >> id >> result;

			conditions[id].match = result;
		}

		saved.close();

		//part2:
		vector<condition_t> unfolded;
		int index = 0;

		ofstream output("output.txt");

		int loaded = 0;

		for (const auto& condition : conditions) {

			if (condition.match != -1) {
				loaded++;
				output << condition.id << " " << condition.match << endl;
				continue;
			}

			condition_t unfold;
			
			for (int i = 0; i < repeat; i++) {
				if (i > 0)
					unfold.record += '?';

				unfold.record += condition.record;
				unfold.positions.insert(unfold.positions.begin(), condition.positions.begin(), condition.positions.end());
				unfold.id = condition.id;
			}
			
			unfolded.push_back(unfold);
		}
		
		atomic<int> progress = 0;

		for_each(execution::par_unseq, unfolded.begin(), unfolded.end(), [&unfolded = std::as_const(unfolded), &progress, &output](auto& condition) {
				int count = count_combinations(condition);
				condition.match = count;
				progress++;
				printf("%d of %lld  id: %d count %d \n", progress.load(), unfolded.size(), condition.id, count);
				output << condition.id << " " << count << endl;
			});

		output.close();

		printf("Combinations: %d", accumulate(unfolded.begin(), unfolded.end(), 0, [](int sum, condition_t& condition) {
			return sum + condition.match;
			}));


	}
	return 0;
}
