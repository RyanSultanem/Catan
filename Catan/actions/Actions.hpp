#ifndef ACTIONS_ACTIONS_HPP
#define ACTIONS_ACTIONS_HPP

#include <board/Board.fwd.hpp>

// TODO: can be replaced by doubledispatch with State. worth it?
// For now it makes more sense to only have a type.
enum class ActionType
{
	PlaceInitialSettlementRoad,
	PlaceSettlement,
	PlaceRoad,
	RollDice,

	Done,
	PlaceCity,
	ExchangeCards,
	MoveRobber,
	CardBurn,

	BuyDevelopment,
	UseDevelopment
};

class Action
{
public:
	virtual bool execute(board::Board & board) = 0; // TODO: check if board should really be passed here or for each needed construcotr.
	virtual ActionType getType() const = 0;
};

#endif // !ACTIONS_ACTIONS_HPP