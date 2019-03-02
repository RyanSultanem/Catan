#ifndef GAME_INTERFACE_HPP
#define GAME_INTERFACE_HPP

#include <memory>

class GameInterface // TODO: should change to contain game
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

	virtual bool rollDice() = 0;
	virtual bool done() = 0;
};

namespace builder {

std::unique_ptr<GameInterface> buildGame(int numberOfPlayer);

}

#endif // !GAME_INTERFACE_HPP
