#include "State.hpp"

#include "Actions.hpp"
#include "Game.hpp"

#include <memory>

bool InitialSettlementState::isValid(const Action & action) const
{
	return action.getType() == ActionType::PlaceInitialSettlementRoad;
}

void InitialSettlementState::nextState(Game & game, const Action & action)
{
	if (!isValid(action)) // TODO: needed?
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

std::vector<ActionType> InitialSettlementState::getPossibleActions()
{
	return { ActionType::PlaceInitialSettlementRoad };
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
	// TODO: check if a development wasnt already used; also pass to PlayerDecisionState

	return action.getType() == ActionType::RollDice
		|| action.getType() == ActionType::UseDevelopment; // TODO: Maybe force to Knight only ? Check if needed.
}

void PrePlayerDecision::nextState(Game & game, const Action & action)
{
	if (action.getType() == ActionType::RollDice)
	{
		std::unique_ptr<State> nextState;
		
		const RollDice * rollDiceAction = dynamic_cast<const RollDice *>(&action);
		if (rollDiceAction && rollDiceAction->shouldBurn()) //TODO: check if rollDiceAction shoudl really be checked for validity.
			nextState = std::make_unique<CardBurnState>(game.getActivePlayerId(), rollDiceAction->getPlayerBurnQueue());
		else if (rollDiceAction && rollDiceAction->shouldChangeRobber())
			nextState = std::make_unique<MovingRobberState>();
		else
			nextState = std::make_unique<PlayerDecision>();

		game.setState(std::move(nextState));
		return;
	}
}

std::vector<ActionType> PrePlayerDecision::getPossibleActions()
{
	return { ActionType::RollDice , ActionType::UseDevelopment };
}

bool PlayerDecision::isValid(const Action & action) const
{
	// Any action that the player can take.
   return action.getType() == ActionType::PlaceSettlement
      || action.getType() == ActionType::PlaceRoad
      || action.getType() == ActionType::PlaceCity
      || action.getType() == ActionType::ExchangeCards
      || action.getType() == ActionType::BuyDevelopment
      || action.getType() == ActionType::UseDevelopment
	  || action.getType() == ActionType::Done; // TODO: check if not already used
}

void PlayerDecision::nextState(Game & game, const Action & action)
{
	if (action.getType() == ActionType::Done)
	{
		game.setNextActivePlayer();
		game.setState(std::make_unique<PrePlayerDecision>());
		return;
	}
}

std::vector<ActionType> PlayerDecision::getPossibleActions()
{
	return 
	{
		ActionType::PlaceSettlement, ActionType::PlaceRoad, ActionType::PlaceCity,
		ActionType::ExchangeCards, ActionType::BuyDevelopment, ActionType::UseDevelopment,
		ActionType::Done
	};
}

CardBurnState::CardBurnState(int currentPlayer, const std::queue<int> & playersBurn)
	: m_currentPlayer(currentPlayer)
	, m_playersBurn(playersBurn)
{
}

bool CardBurnState::isValid(const Action & action) const
{
	return action.getType() == ActionType::CardBurn;
}

void CardBurnState::nextState(Game & game, const Action & action) 
{
	if (m_playersBurn.empty())
	{
		game.setNextActivePlayer(m_currentPlayer);
		game.setState(std::make_unique<MovingRobberState>());
		return;
	}
	else
	{
		game.setNextActivePlayer(m_playersBurn.front());
		m_playersBurn.pop();
	}
}

std::vector<ActionType> CardBurnState::getPossibleActions()
{
	return { ActionType::CardBurn };
}

bool MovingRobberState::isValid(const Action & action) const
{
	return action.getType() == ActionType::MoveRobber;
}

void MovingRobberState::nextState(Game & game, const Action & /*action*/)
{
	game.setState(std::make_unique<PlayerDecision>());
}

std::vector<ActionType> MovingRobberState::getPossibleActions()
{
	return { ActionType::MoveRobber };
}
