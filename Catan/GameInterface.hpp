#ifndef GAME_INTERFACE_HPP
#define GAME_INTERFACE_HPP

class GameInterface
{
public:
	virtual bool placeInitialSetlementRoad(int settlementPosition, int roadPosition) = 0;
	virtual bool placeSettlement(int position) = 0;
	virtual bool placeRoad(int position) = 0;
	virtual bool placeCity(int position) = 0;
	virtual bool exchangeCards(int resultType, int typeToTrade) = 0; // TODO: use enum instead for allignemnt.

	virtual bool rollDice() = 0;
	virtual bool done() = 0;
};

#endif // !GAME_INTERFACE_HPP
