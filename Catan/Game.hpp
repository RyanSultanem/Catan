#ifndef GAME_HPP
#define GAME_HPP

#include <Game.fwd.hpp>

#include <Achievement.hpp>
#include <GameEndingListenner.hpp>
#include <GameInterface.hpp>
#include <State.fwd.hpp>

#include <actions/Actions.fwd.hpp>

#include <board/Board.hpp>
#include <board/Dice.hpp>

#include <card/DevelopmentStock.hpp>

#include <Players.hpp>

#include <utility/NumberGenerator.fwd.hpp>

#include <vector>

class Game : public GameInterface, public GameEndingListenner
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
	bool useDevelopmentCard(const card::DevelopmentData & developmentData) override;

	bool rollDice() override;
	bool done() override;

	bool gameEnded() const override;
	std::optional<int> getWinnerId() const override;
	std::vector<ActionType> getPossibleActions() const override;
	int getActivePlayerId() const override;
	std::string getBoardInfo() const override;
	std::string getPlayersInfo() const override;
	int getDiceValue() const override;

	void updateGameEnd(int playerId) override;

private:
	std::unique_ptr<NumberGenerator> m_numberGenerator;
	std::unique_ptr<State> m_state; // TODO: Possible enhancement, have State as a non polymoprhic type, with and internal polymoprhic data. This way State will handle its own changes, not Game Anymore
	
	board::Board m_board;
	board::Dice m_dice;
	DevelopmentStock m_developmentStock;

	Achievement m_longestRoad;
	Achievement m_strongestArmy;
   
	Players m_players;

	std::optional<int> m_winnerId;

private:
	void setupBoard();
	void setupPlayers(int numberOfPlayers);
	void initalizeDevelopmentStock();

	bool processAction(Action & action);
	void updateStateWithAction(Action & action);
};

#endif // GAME_HPP