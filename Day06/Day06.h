#include <vector>

//#define TEST

#ifdef TEST

std::vector<int> t{ 7,15, 30 };
std::vector<int> d{ 9, 40, 200 };

int64_t t2{ 71530 };
int64_t d2{ 940200 };

#else

std::vector<int> t{ 61, 70, 90, 66 };
std::vector<int> d{ 643, 1184, 1362, 1041 };


int64_t t2{ 61709066 };
int64_t d2{ 643118413621041 };

#endif