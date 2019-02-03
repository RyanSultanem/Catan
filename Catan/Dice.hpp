#ifndef DICE_HPP
#define DICE_HPP

namespace board {
	
class Dice
{
public:
	Dice();

	const Dice & roll();
	int getValue() const;

private:
	int m_die1;
	int m_die2;
};

}


#endif // !DICE_HPP