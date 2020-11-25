#ifndef GAME_INTERFACE_HPP
#define GAME_INTERFACE_HPP

#include <actions/Actions.fwd.hpp>

#include <card/Development.fwd.hpp>

#include <utility/NumberGenerator.fwd.hpp>

#include <memory>
#include <unordered_map>
#include <vector>

class GameInterface // TODO: change to contain game ? will expose Game.hpp
{
public:
	virtual bool placeInitialSetlementRoad(int settlementPosition, int roadPosition) = 0;
	virtual bool placeSettlement(int position) = 0;
	virtual bool placeRoad(int position) = 0;
	virtual bool placeCity(int position) = 0;
	virtual bool exchangeCards(int resultType, int typeToTrade) = 0; // TODO: use enum instead for allignemnt.
	virtual bool moveRobber(int cellPosition, int vertexPosition) = 0;
	virtual bool burnCards(const std::unordered_map<int, int> & ressourcesToBurn) = 0;
	virtual bool buyDevelopmentCard() = 0;
	virtual bool useDevelopmentCard(const card::DevelopmentData & developmentData) = 0;

	virtual bool rollDice() = 0;
	virtual bool done() = 0;

	// Game End info
	virtual bool gameEnded() const = 0;
	virtual std::optional<int> getWinnerId() const = 0;
	
	// Info for display
	virtual int getActivePlayerId() const = 0;
	virtual std::vector<ActionType> getPossibleActions() const = 0;
	virtual std::string getBoardInfo() const = 0;
	virtual std::string getPlayersInfo() const = 0;
	virtual int getDiceValue() const = 0;
};

namespace builder {

std::unique_ptr<GameInterface> buildGame(int numberOfPlayer, std::unique_ptr<NumberGenerator> && numberGenerator);

}

#endif // !GAME_INTERFACE_HPP
