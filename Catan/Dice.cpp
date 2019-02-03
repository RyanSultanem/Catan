#include "Dice.hpp"

#include <cstdlib>

namespace board {

Dice::Dice()
	: m_die1(0)
	, m_die2(0)
{
}

const Dice & Dice::roll()
{
	m_die1 = rand()% 6 + 1;
	m_die2 = rand()% 6 + 1;

	return *this;
}

int Dice::getValue() const
{
	return m_die1 + m_die2;
}
}
