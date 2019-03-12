#ifndef GAME_HPP
#define GAME_HPP

#include "Game.fwd.hpp"

#include "Board.h"
#include "Dice.hpp"
#include "Player.hpp"
#include "GameInterface.hpp"

#include "Actions.fwd.hpp"
#include "State.fwd.hpp"
#include "DevelopmentStock.hpp"
#include "NumberGenerator.fwd.hpp"

#include <vector>

class Game : public GameInterface
{
public:
	Game(int numberOfPlayers, std::unique_ptr<NumberGenerator> && numberGenerator);
	Game(int numberOfPlayers);
	~Game();

	bool placeInitialSetlementRoad(int settlementPosition, int roadPosition) override;
	bool placeSettlement(int position) override;
	bool placeRoad(int position) override;
	bool placeCity(int position) override;
	bool exchangeCards(int resultType, int typeToTrade) override;
	bool moveRobber(int cellPosition, int vertexPosition) override;
	bool burnCards(const std::unordered_map<int, int> & ressourcesToBurn) override;
	bool buyDevelopmentCard() override;
	bool useDevelopmentCard(const card::DevelopmentType & developmentType, const card::DevelopmentData & developmentData) override;

	bool rollDice() override;
	bool done() override;

	bool gameEnded() const override;
	std::vector<ActionType> getPossibleActions() const override;
	int getActivePlayerId() const override;
	std::string getBoardInfo() const override;
	std::string getPlayersInfo() const override;
	int getDiceValue() const override;

	int getPlayerCount() const;
	void setNextActivePlayer();
	void setNextActivePlayer(int playerId);

	void setState(std::unique_ptr<State> && state);

private:
	std::unique_ptr<NumberGenerator> m_numberGenerator;
	std::unique_ptr<State> m_state;
	board::Board m_board;
	board::Dice m_dice;
	DevelopmentStock m_developmentStock;

	std::vector<player::Player> m_players;
	int m_activePlayer;
   
	bool m_gameEnded; // TODO: should be removed or changed


	void setupPlayers(int numberOfPlayers);
	void initalizeDevelopmentStock();

	bool processAction(Action & action);
};

#endif // GAME_HPP