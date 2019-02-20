#include "State.hpp"

#include "Actions.hpp"
#include "Game.hpp"

#include <memory>

bool InitialSettlementState::isValid(const Action & action) const
{
	return action.getType() == ActionType::PlaceInitialSettlementRoad;
}

void InitialSettlementState::nextState(Game & game, const Action & action) const
{
	if (!isValid(action))
		return;

	updateGameSecondRun(game);
	
	if (game.isSecondInitialPlacementRun() && game.getActivePlayerId() == 0)
	{
		game.setState(std::make_unique<PrePlayerDecision>()); // TODO: Dangerous.. deletes itself can use bool returned to know if should change state instaed.
		return;
	}
	
	if (!game.isSecondInitialPlacementRun()) // TODO: can create anothe state to take care of that.. not really a state thing
		game.setNextActivePlayer();
	else
		game.setNextActivePlayer(game.getActivePlayerId() - 1); 
}

void InitialSettlementState::updateGameSecondRun(Game & game) const
{
	if (!game.isSecondInitialPlacementRun())
	{
		if (game.getActivePlayerId() == game.getPlayerCount() - 1)
		{
			game.setSecondInitialPlacementRun();
		}
	}
}

bool PrePlayerDecision::isValid(const Action & action) const
{
	return action.getType() == ActionType::RollDice; // TODO: Add choosing a development card as valid.
}

void PrePlayerDecision::nextState(Game & game, const Action & action) const
{
	if (action.getType() == ActionType::RollDice)
	{
		game.setState(std::make_unique<PlayerDecision>());
		return;
	}
}

bool PlayerDecision::isValid(const Action & action) const
{
	// Any action that the player can take.
	return action.getType() == ActionType::PlaceSettlement
		|| action.getType() == ActionType::PlaceRoad
		|| action.getType() == ActionType::PlaceCity
		|| action.getType() == ActionType::ExchangeCards;
}

void PlayerDecision::nextState(Game & game, const Action & action) const
{
	if (action.getType() == ActionType::Done)
	{
		game.setNextActivePlayer();
		game.setState(std::make_unique<PrePlayerDecision>());
		return;
	}
}
