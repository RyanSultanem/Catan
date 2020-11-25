#ifndef ACTIONS_ACTIONS_HPP
#define ACTIONS_ACTIONS_HPP

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
	virtual bool execute() = 0;
	virtual ActionType getType() const = 0;
};

#endif // !ACTIONS_ACTIONS_HPP