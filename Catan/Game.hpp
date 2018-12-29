#ifndef GAME_HPP
#define GAME_HPP

#include "Board.h"
#include "Player.hpp"
#include "Interface.hpp"

#include <vector>

class Game
{
public:
	Game(Interface & interface, int numberOfPlayers);
	~Game();

	void play();

private:
	board::Board m_board;

	std::vector<player::Player> m_players;
   
	Interface & m_interface;

	bool m_gameEnded = false;

	void setupPlayers(int numberOfPlayers);
	void initialSettlmentPlacement();

	void showStatus();

	void giveRessources(int value);
	void checkGameEnded(int turnCount);
	void playTurns();
};

#endif // GAME_HPP