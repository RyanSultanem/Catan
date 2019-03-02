#include "../Catan/GameInterface.hpp"

#include <iostream>

int main()
{
	std::unique_ptr<GameInterface> gamePtr = builder::buildGame(3);
	GameInterface & game = *gamePtr;

	system("PAUSE");

	return 0;
}