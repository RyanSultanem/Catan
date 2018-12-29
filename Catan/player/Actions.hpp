#ifndef PLAYER_ACTIONS_HPP
#define PLAYER_ACTIONS_HPP

#include "../Board.fwd.h"
#include "../Player.fwd.hpp"

#include <memory>

namespace player { 
	
class Action
{
public:
	Action(Player & player, board::Board & board);

	virtual void execute() = 0;

	virtual ~Action(){}

protected:
	Player & m_player;
	board::Board & m_board;
};


class Pass : public Action
{
public:

};

class BuildSettlement : public Action
{
public:
	BuildSettlement(Player & player, board::Board & board);
	void execute() override;

	static const int ACTION_ID;
};

class ExchangeCards : public Action
{
public:
	ExchangeCards(Player & player, board::Board & board);
	void execute() override;

	static const int ACTION_ID;
};

namespace builder {
	
class ActionBuilder
{
public:
	static std::unique_ptr<Action> makeAction(int actionID, Player & player, board::Board & board);
};

} // namespace builder

} // namespace player

#endif // !PLAYER_ACTIONS_HPP