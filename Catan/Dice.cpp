#include "Dice.hpp"

#include <cstdlib>
#include <ctime>

namespace board {

bool Dice::randInitialized = false;

Dice::Dice()
	: m_die1(0)
	, m_die2(0)
{
	if(!randInitialized)
	{
		srand(time(NULL));
		randInitialized = true;
	}
}

int Dice::roll()
{
	m_die1 = rand()% 6 + 1;
	m_die2 = rand()% 6 + 1;

	return m_die1 + m_die2;
}

int Dice::getValue() const
{
	return m_die1 + m_die2;
}
}
