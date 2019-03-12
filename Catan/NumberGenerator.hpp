#ifndef NUMBERGENERATOR_HPP
#define NUMBERGENERATOR_HPP

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

#endif // !NUMBERGENERATOR_HPP