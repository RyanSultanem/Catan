#include "NumberGenerator.hpp"

#include <ctime>
#include <cstdlib>

RandomNumberGenerator::RandomNumberGenerator()
{
	srand(time(0));
}

int RandomNumberGenerator::generateNumber(int min, int max) const
{
	// TODO: see if checks are needed min > max? min < 0 ?
	return rand() % max + min;
}
