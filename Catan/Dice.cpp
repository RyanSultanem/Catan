#include "Dice.hpp"

#include "NumberGenerator.hpp"

namespace board {

Dice::Dice(const NumberGenerator & numberGenerator)
	: m_die1(0)
	, m_die2(0)
	, m_numberGenerator(numberGenerator)
{
}

const Dice & Dice::roll()
{
	m_die1 = m_numberGenerator.generateNumber(1, 6);
	m_die2 = m_numberGenerator.generateNumber(1, 6);

	return *this;
}

int Dice::getValue() const
{
	return m_die1 + m_die2;
}
}
