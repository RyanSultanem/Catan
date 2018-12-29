#ifndef DICE_HPP
#define DICE_HPP

namespace board {
	
class Dice
{
public:
	Dice();

	int roll();
	int getValue() const;

private:
	int m_die1;
	int m_die2;

	static bool randInitialized;
};

}


#endif // !DICE_HPP