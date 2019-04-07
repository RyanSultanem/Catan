#ifndef BOARD_DICE_HPP
#define BOARD_DICE_HPP

#include <utility/NumberGenerator.fwd.hpp>

namespace board {
	
class Dice
{
public:
	explicit Dice(const NumberGenerator & numberGenerator);

	const Dice & roll();
	int getValue() const;

private:
	int m_die1;
	int m_die2;

	const NumberGenerator & m_numberGenerator;
};

} // namespace board


#endif // !BOARD_DICE_HPP