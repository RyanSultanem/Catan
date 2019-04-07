#include <State.hpp>

#include <Game.hpp>

#include <actions/Actions.hpp>

#include <memory>

// TODO: dynamic_casts can be changed with ActionData from alll Actions with optionals (like Development), check if should be done.

InitialSettlementState::InitialSettlementState()
	: m_secondRun(false)
{
}

bool InitialSettlementState::isValid(const Action & action) const
{
	return action.getType() == ActionType::PlaceInitialSettlementRoad;
}

void InitialSettlementState::nextState(Game & game, const Action & action)
{
	if (!isValid(action)) // TODO: needed?
		return;

	// TODO: check if can do something more understandable: currently works but not very clear
	bool updated = updateGameSecondRun(game);
	
	if (m_secondRun && game.getActivePlayerId() == 0)
	{
		game.setState(std::make_unique<PrePlayerDecision>()); // TODO: Dangerous.. deletes itself can use bool returned to know if should change state instaed.
		return;
	}

	if (!updated)
	{
		int currentActivePlayer = game.getActivePlayerId();
		game.setNextActivePlayer(m_secondRun ? --currentActivePlayer : ++currentActivePlayer);
	}
}

std::vector<ActionType> InitialSettlementState::getPossibleActions()
{
	return { ActionType::PlaceInitialSettlementRoad };
}

void InitialSettlementState::preProcessAction(PlaceInitialSettlementRoadAction & action)
{
	action.setSecondRun(m_secondRun);
}

bool InitialSettlementState::updateGameSecondRun(Game & game)
{
	bool updated = false;

	if (!m_secondRun)
	{
		if (game.getActivePlayerId() == game.getPlayerCount() - 1)
		{
			m_secondRun = true;
			updated = true;
		}
	}

	return updated;
}

bool PrePlayerDecision::isValid(const Action & action) const
{
	return action.getType() == ActionType::RollDice
		|| validDevelopmentUse(action); 
}

void PrePlayerDecision::nextState(Game & game, const Action & action)
{
	if (validDevelopmentUse(action))
	{
		m_developmentUsed = true;
	}
	else if (action.getType() == ActionType::RollDice)
	{
		std::unique_ptr<State> nextState;
		
		const RollDice * rollDiceAction = dynamic_cast<const RollDice *>(&action);
		if (rollDiceAction && rollDiceAction->shouldBurn()) //TODO: check if rollDiceAction shoudl really be checked for validity.
		{
			nextState = std::make_unique<CardBurnState>(game, game.getActivePlayerId(), rollDiceAction->getPlayerBurnQueue(), m_developmentUsed);
		}
		else if (rollDiceAction && rollDiceAction->shouldChangeRobber())
		{
			nextState = std::make_unique<MovingRobberState>(m_developmentUsed);
		}
		else
			nextState = std::make_unique<PlayerDecision>(m_developmentUsed);

		game.setState(std::move(nextState));
		return;
	}
}

std::vector<ActionType> PrePlayerDecision::getPossibleActions()
{
	return { ActionType::RollDice , ActionType::UseDevelopment };
}

bool PrePlayerDecision::validDevelopmentUse(const Action& action) const
{
	if (action.getType() != ActionType::UseDevelopment)
		return false;

	const UseDevelopmentAction & useDevelopmentAction = dynamic_cast<const UseDevelopmentAction &>(action);
	return useDevelopmentAction.validPrePlayerDecisionUse(m_developmentUsed);
}

PlayerDecision::PlayerDecision(bool developmentUsed)
	: m_developmentUsed(developmentUsed)
{
}

bool PlayerDecision::isValid(const Action & action) const
{
	// Any action that the player can take.
   return action.getType() == ActionType::PlaceSettlement
      || action.getType() == ActionType::PlaceRoad
      || action.getType() == ActionType::PlaceCity
      || action.getType() == ActionType::ExchangeCards
      || action.getType() == ActionType::BuyDevelopment
      || validDevelopmentUse(action)
	  || action.getType() == ActionType::Done; 
}

void PlayerDecision::nextState(Game & game, const Action & action)
{
	if (action.getType() == ActionType::UseDevelopment)
	{
		m_developmentUsed = true;
	}
	else if (action.getType() == ActionType::Done)
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

bool PlayerDecision::validDevelopmentUse(const Action & action) const
{
	if (action.getType() != ActionType::UseDevelopment)
		return false;

	const UseDevelopmentAction & useDevelopmentAction = dynamic_cast<const UseDevelopmentAction &>(action);
	return useDevelopmentAction.validPlayerDecisionUse(m_developmentUsed);
}

CardBurnState::CardBurnState(Game & game, int currentPlayer, const std::queue<int> & playersBurn, bool developmentUsed)
	: m_currentPlayer(currentPlayer)
	, m_currentPlayerDevUsed(developmentUsed)
	, m_playersBurn(playersBurn)
{
	setBurnActivePlayer(game);
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
		game.setState(std::make_unique<MovingRobberState>(m_currentPlayerDevUsed));
		return;
	}
	else
	{
		setBurnActivePlayer(game);
	}
}

std::vector<ActionType> CardBurnState::getPossibleActions()
{
	return { ActionType::CardBurn };
}

void CardBurnState::setBurnActivePlayer(Game & game)
{
	game.setNextActivePlayer(m_playersBurn.front());
	m_playersBurn.pop();
}

MovingRobberState::MovingRobberState(bool developmentUsed)
	: m_developmentUsed(developmentUsed)
{
}

bool MovingRobberState::isValid(const Action & action) const
{
	return action.getType() == ActionType::MoveRobber;
}

void MovingRobberState::nextState(Game & game, const Action & /*action*/)
{
	game.setState(std::make_unique<PlayerDecision>(m_developmentUsed));
}

std::vector<ActionType> MovingRobberState::getPossibleActions()
{
	return { ActionType::MoveRobber };
}
