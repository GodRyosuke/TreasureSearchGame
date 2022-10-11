#include <random>

class Random {
public:
	Random();
	uint32_t rnd();

private:
	std::mt19937 mt;

};