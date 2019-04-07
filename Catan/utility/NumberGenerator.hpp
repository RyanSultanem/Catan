#ifndef UTILITY_NUMBERGENERATOR_HPP
#define UTILITY_NUMBERGENERATOR_HPP

class NumberGenerator
{
public:
	virtual int generateNumber(int min, int max) const = 0;
};

class RandomNumberGenerator : public NumberGenerator
{
public:
	RandomNumberGenerator();

	int generateNumber(int min, int max) const override;
};

#endif // !UTILITY_NUMBERGENERATOR_HPP