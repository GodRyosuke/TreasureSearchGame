#include "Random.hpp"

Random::Random()
	:mt(static_cast<int>(time(0)))
{

}

uint32_t Random::rnd()
{
	return static_cast<uint32_t>(mt());
}