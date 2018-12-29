#include "Actions.hpp"

#include "../Board.h"
#include "../Player.hpp"

#include <unordered_map>

namespace player {

const int BuildSettlement::ACTION_ID	= 0;
const int ExchangeCards::ACTION_ID		= 1;

Action::Action(Player& player, board::Board& board)
	:  m_player(player)
	, m_board(board)
{
	
}

BuildSettlement::BuildSettlement(Player& player, board::Board& board)
	: Action(player, board)
{
}

void BuildSettlement::execute()
{
}

ExchangeCards::ExchangeCards(Player& player, board::Board& board)
	: Action(player, board)
{
}

void ExchangeCards::execute()
{
}


namespace builder {
	
std::unique_ptr<Action> ActionBuilder::makeAction(int actionID, Player & player, board::Board & board)
{
	switch(actionID)
	{
	case BuildSettlement::ACTION_ID:
		return std::make_unique<BuildSettlement>(player, board);
	case ExchangeCards::ACTION_ID:
		return std::make_unique<ExchangeCards>(player, board);
	default:
		return nullptr;
	}
}

} // namespace builder

} // namespace player