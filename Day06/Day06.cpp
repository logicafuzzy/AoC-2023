#include <iostream>
#include <string>

#include "Day06.h"

using namespace std;

int main() {
	cout << " AoC 2023 Day06" << endl;

	int64_t res = 1;

	for (int i = 0; i < t.size(); ++i) {
		int tx1 = ceil((t[i] - sqrt(t[i] * t[i] - 4 * d[i])) / 2.0);		

		if (t[i] * tx1 - tx1 * tx1 == d[i])
			++tx1;


		int tx2 = floor((t[i] + sqrt(t[i] * t[i] - 4 * d[i])) / 2.0);

		if (t[i] * tx2 - tx2 * tx2 == d[i])
			--tx2;

		res *= tx2 - tx1 + 1;

		printf("tx1 %d tx2 %d tx1 - tx2 + 1 = %d\n", tx1, tx2, tx2 - tx1 + 1);

		printf("%d %d %f \n", t[i] * t[i], 4 * d[i], sqrt(t[i] * t[i] - 4 * d[i]));
	}

	printf("res: %lld\n", res);

	printf("Part2");

	int64_t tx1 = ceil((t2 - sqrt(t2 * t2 - 4 * d2)) / 2.0);

	if (t2 * tx1 - tx1 * tx1 == d2)
		++tx1;


	int64_t tx2 = floor((t2 + sqrt(t2 * t2 - 4 * d2)) / 2.0);

	if (t2 * tx2 - tx2 * tx2 == d2)
		--tx2;

	res = tx2 - tx1 + 1;

	printf("tx1 %lld tx2 %lld tx1 - tx2 + 1 = %lld\n", tx1, tx2, tx2 - tx1 + 1);

	printf("%lld %lld %f \n", t2 * t2, 4 * d2, sqrt(t2 * t2 - 4 * d2));

	printf("res: %lld\n", res);

	return 0;
}
