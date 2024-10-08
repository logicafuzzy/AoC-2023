#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <execution>

#include <cmath>

using namespace std;

using num_t = uint8_t;
constexpr inline auto execution_strategy = std::execution::par_unseq;

constexpr bool isPart2 = true;
constexpr int repeat = 5;

struct condition_t {
	string record{};
	vector<num_t> positions{};
	uint16_t id{};
	int64_t match{-1};
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

bool match_fast(const condition_t& condition, const vector<num_t>& dots) {
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
//vector<vector<num_t>> make_dots(const int positions, const int sum) {
//	// inspired by: 
//	// https://codegolf.stackexchange.com/questions/220718/create-all-arrays-of-non-negative-integers-of-length-n-with-sum-of-parts-equal-t
//	// https://tio.run/##hVJdU8IwEHzvr7ip40wzTQC/XkiLP0QdpoQAqZB20mt1xPrTxSRFsDhoXi693b3sJhVlyZZC7HYXalMWBpOZwmpY4VzE8WA1CbIaC1hESiNoCq4g2Vp4PG6kwMIktjUxfFEYz1FpWbxEcYxUE9rQnCvGePP@HplBWVer6SwTz5EidESIkzQpsiuap4rnPB@mSBqW5pfIjcTaaDC8tb60WNdzmaiiQiOzzSRwB20ypSMSbAOw69QPoKxwqiGF7R2FWwo3Lf@DiI54TcFx7/ZM6w66RBYccVuS/dRBpd5kRIBDHCsCnQO3/B1Zdkd7UE@8B@E3hD3IWxJFjZAkEDp96Hbaf1Kv8o0OBzZ51OFR7Z/HTbW0RWRfCMkRPGSQa7mBsef9NNzPqefy1WftdtYCd7phyj6Q2qiue6r@FSDqxPfWaWhPDEPium7OpR3De@I2ODsm7IVsz1wW@28dprSBL/vfasSDdvcpFutsWe3Yyxc
//
//	int s = sum;
//	vector<num_t> res;
//	for (uint64_t i = (uint64_t)pow(++s, positions), v, j; i--; v || (res.push_back(i), 0))
//		for (v = s - 1, j = i; j; j /= s)
//			v -= j % s; 
//
//	vector<vector<num_t>> combinations;
//	for (int elem : res) {
//		vector<num_t> combination;
//		bool discard = false;
//		for (uint64_t index = 0; index < positions; elem /= -~sum, ++index) {
//			int val = elem % -~sum;
//			// 0 only allowed at beginning or end
//			if (index == 0 || index > 0 && val > 0 || index == positions - 1)
//				combination.push_back(val);
//			else {
//				discard = true;
//				break;
//			}
//		}
//
//		if (!discard)
//			combinations.push_back(combination);
//	}
//
//	return combinations;
//}

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

vector<vector<num_t>> partitions_prefixed(const vector<num_t>& partition, const size_t sum, const size_t positions, const condition_t& condition, uint64_t& accumulator ) {
	if (positions == 1) {
		vector<num_t> prefixed = partition;
		prefixed.push_back(sum);
		
		if (match_fast(condition, prefixed))
			++accumulator;

		return {};
	}
	
	vector<num_t> prefixed = partition;
	prefixed.push_back(0);

	const uint64_t start = partition.size() == 0 ? 0 : 1;
	const int p_size = sum - start + 1;
	vector<uint64_t> ids(p_size); // if partition.size() == 0 -> [0, sum] (size sum + 1), else [1,sum] (size = sum)
	std::iota(ids.begin(), ids.end(), start);
	
	vector<vector<vector<num_t>>> results(p_size);
	vector<uint64_t> accumulators(p_size, 0);
	vector<vector<num_t>> prefixeds(p_size, prefixed);

	//zeroes can be only at the beginning
	std::for_each(execution_strategy, ids.begin(), ids.end(), [&](uint64_t id) {
		const uint64_t i = id - start;

		prefixeds[i].back() = id;

		if (!match_fast(condition, prefixeds[i]))
			return;

		auto iteration = partitions_prefixed(prefixeds[i], sum - id, positions - 1, condition, accumulators[i]);
		iteration.erase(remove_if(iteration.begin(), iteration.end(), [&condition](auto& prefix) {
			return !match_fast(condition, prefix);
			}), iteration.end());

		results[i].insert(results[i].end(), iteration.begin(), iteration.end());
	});

	vector<vector<num_t>> retval = {};
	size_t retval_size = std::accumulate(results.begin(), results.end(), 0, [](const size_t& acc, const vector<vector<num_t>>& value) {return acc + value.size(); });
	retval.reserve(retval_size);

	for (const auto& result : results) {
		retval.insert(retval.end(), result.begin(), result.end());
	}

	accumulator = std::accumulate(accumulators.begin(), accumulators.end(), accumulator);

	return retval;
}

uint64_t make_partition(const size_t positions, const size_t sum, const condition_t& condition) {
	uint64_t accumulator = 0;

	if (sum >= 0 and positions >= 1)
		partitions_prefixed({}, sum, positions, condition, accumulator);

	return accumulator;
}

uint64_t count_combinations(const condition_t& condition) {
	size_t N = condition.record.size();
	size_t nDots = N - reduce(condition.positions.begin(), condition.positions.end());

	vector<num_t> dots(condition.positions.size() + 1, 0);

	return make_partition(condition.positions.size() + 1, nDots, condition);

}

condition_t repeat_condition(const condition_t& condition, int reps) {
	
	condition_t result;
	
	for (int i = 0; i < reps; i++) {
		if (i > 0)
			result.record += '?';

		result.record += condition.record;
		result.positions.insert(result.positions.begin(), condition.positions.begin(), condition.positions.end());
		result.id = condition.id;
	}

	return result;
}

bool test_arrangements(const vector<condition_t>& conditions, const vector<uint64_t>& results, const vector<string>& arrangements) {
	int id = 0;
	bool result = false;
	bool passed = true;
	uint64_t count;

	for (const auto& condition : conditions) {

		count = count_combinations(condition);
		result = count == results[id];
		passed &= result;

		if (result)
			cout << " [ OK ] ";			
		else
			cout << "[ ERROR ] ";

		cout << "test " << id << " [" << arrangements[id] << "]: count " << count << " expected " << results[id] << endl;

		id++;
	}

	return passed;
}

vector<condition_t> get_conditions(const vector<string>& arrangements) {
	vector<condition_t> conditions;

	int id = 0;
	for (auto& arrangement : arrangements)
		conditions.push_back(get_condition(arrangement, id++));

	return conditions;
}

vector<condition_t> get_conditions(const vector<string>& arrangements, int reps) {
	vector<condition_t> conditions;

	int id = 0;
	for (auto& arrangement : arrangements)
		conditions.push_back(repeat_condition(get_condition(arrangement, id++), reps));

	return conditions;
}

bool test() {
	//part 1
	vector<uint64_t> results{ 1,4,1,1,4,10 };
	vector<uint64_t> results_part2{ 1, 16384, 1, 16, 2500, 506250};


	vector<string> arrangements{
		"???.### 1,1,3",
		".??..??...?##. 1,1,3",
		"?#?#?#?#?#?#?#? 1,3,1,6",
		"????.#...#... 4,1,1",
		"????.######..#####. 1,6,5",
		"?###???????? 3,2,1"
	};

	cout << "Testing part1..." << endl;

	if (!test_arrangements(get_conditions(arrangements), results, arrangements)) {
		cout << "test part1 failed" << endl;
		return false;
	}

	cout << "Testing part2..." << endl;

	if (!test_arrangements(get_conditions(arrangements, repeat), results_part2, arrangements)) {
		cout << "test part2 failed" << endl;
		return false;
	}

	cout << "passed" << endl;
	return true;
}

int main() {

	cout << " AoC 2023 Day12" << endl;
	
	if (!test()) {
		cout << "failed" << endl;
		exit(1);
	}

	ifstream input("Day12.txt");

	vector<condition_t> conditions;

	uint64_t sum = 0;

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
			printf("%d of %zu           \r", ++index, conditions.size());
		}

		printf("Combinations: %llu", sum);
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

		ofstream output("output.txt");

		int loaded = 0;

		for (const auto& condition : conditions) {

			if (condition.match != -1) {
				loaded++;
				output << condition.id << " " << condition.match << endl;
				continue;
			}
			
			unfolded.push_back(repeat_condition(condition, repeat));
		}
		
		atomic<int> progress = 0;

		for_each(std::execution::seq, unfolded.begin(), unfolded.end(), [&unfolded = std::as_const(unfolded), &progress, &output](auto& condition) {
				uint64_t count = count_combinations(condition);
				condition.match = count;
				printf("%d of %zu  id: %d count %lu \n", progress.load(), unfolded.size(), condition.id, count);
				progress++;
				output << condition.id << " " << count << endl;
			});

		output.close();

		printf("Combinations: %lu", accumulate(unfolded.begin(), unfolded.end(), (uint64_t)0, [](uint64_t sum, condition_t& condition) {
			return sum + condition.match;
			}));


	}
	return 0;
}
