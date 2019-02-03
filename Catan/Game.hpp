#ifndef GAME_HPP
#define GAME_HPP

#include "Game.fwd.hpp"

#include "Board.h"
#include "Player.hpp"
#include "Interface.hpp"
#include "GameInterface.hpp"

#include "Actions.fwd.hpp"
#include "State.hpp"

#include <vector>

class Game : public GameInterface
{
public:
	Game(Interface & interface, int numberOfPlayers);
	~Game();

	//void play();

	bool placeInitialSetlementRoad(int settlementPosition, int roadPosition) override;
	bool placeSettlement(int position) override;
	bool placeRoad(int position) override;
	bool placeCity(int position) override;

	bool rollDice() override;
	bool done() override;

	int getPlayerCount() const;
	int getActivePlayerId() const;
	void setNextActivePlayer();
	void setNextActivePlayer(int playerId);

	void setSecondInitialPlacementRun();
	bool isSecondInitialPlacementRun() const;

	void setState(std::unique_ptr<State> && state);

private:
	std::unique_ptr<State> m_state;
	board::Board m_board;

	std::vector<player::Player> m_players;
	int m_activePlayer;
	bool m_secondInitialPlacementRun; // TODO: maybe find a better solution for second run.. State?
   
	//Interface & m_interface;

	void setupPlayers(int numberOfPlayers);

	bool processAction(const Action & action);

	//void showStatus();

	/*void giveRessources(int value);
	void playTurns();*/
};

#endif // GAME_HPP