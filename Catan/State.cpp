#include <State.hpp>

#include <actions/DevelopmentAction.hpp>
#include <actions/PlaceTokenAction.hpp>
#include <actions/PlayerAction.hpp>

#include <Players.hpp>

NextStateResult::NextStateResult()
	: m_updated(false)
	, m_newState(nullptr)
{
}

NextStateResult::NextStateResult(std::unique_ptr<State> && newState)
	: m_updated(true)
	, m_newState(std::move(newState))
{
}

bool NextStateResult::isUpdated() const
{
	return m_updated;
}

std::unique_ptr<State> NextStateResult::takeNewState()
{
	return std::move(m_newState);
}

InitialSettlementState::InitialSettlementState()
	: m_secondRun(false)
{
}

bool InitialSettlementState::isValid(const Action & action) const
{
	return action.getType() == ActionType::PlaceInitialSettlementRoad;
}

NextStateResult InitialSettlementState::computeNextState(Players & players, const Action & action)
{
	bool oldSecondRunValue = m_secondRun;
	updateSecondRun(players);
	
	if (m_secondRun && players.getActivePlayerId() == 0)
	{
		return NextStateResult(std::make_unique<PrePlayerDecision>());
	}

	bool updatedSecondRun = m_secondRun;
	bool onSecondRunUpdate = updatedSecondRun != oldSecondRunValue;
	if (!onSecondRunUpdate)
	{
		int currentActivePlayer = players.getActivePlayerId();
		players.setNextActivePlayer(m_secondRun ? --currentActivePlayer : ++currentActivePlayer);
	}

	return NextStateResult();
}

std::vector<ActionType> InitialSettlementState::getPossibleActions()
{
	return { ActionType::PlaceInitialSettlementRoad };
}

void InitialSettlementState::preProcessAction(PlaceInitialSettlementRoadAction & action)
{
	action.setSecondRun(m_secondRun);
}

void InitialSettlementState::updateSecondRun(const Players & players)
{
	if (m_secondRun)
		return;

	m_secondRun = (players.getActivePlayerId() == players.getLastPlayerId());
}

bool PrePlayerDecision::isValid(const Action & action) const
{
	return action.getType() == ActionType::RollDice
		|| validDevelopmentUse(action); 
}

static std::unique_ptr<State> computeRollDiceNextState(const RollDice & rollDiceAction, Players & players, bool developmentUsed)
{
	if (rollDiceAction.shouldBurn())
	{
		return std::make_unique<CardBurnState>(players, players.getActivePlayerId(), rollDiceAction.getPlayerBurnQueue(), developmentUsed);
	}
	
	if (rollDiceAction.shouldChangeRobber())
	{
		return std::make_unique<MovingRobberState>(developmentUsed);
	}
	
	return std::make_unique<PlayerDecision>(developmentUsed);
}

NextStateResult PrePlayerDecision::computeNextState(Players & players, const Action & action)
{
	if (validDevelopmentUse(action))
	{
		m_developmentUsed = true;
	}
	else if (action.getType() == ActionType::RollDice)
	{		
		const RollDice * rollDiceAction = dynamic_cast<const RollDice *>(&action);

		return NextStateResult(std::move(computeRollDiceNextState(*rollDiceAction, players, m_developmentUsed)));
	}

	return NextStateResult();
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

NextStateResult PlayerDecision::computeNextState(Players & players, const Action & action)
{
	if (action.getType() == ActionType::UseDevelopment)
	{
		m_developmentUsed = true;
	}
	else if (action.getType() == ActionType::Done)
	{
		players.setNextActivePlayer();
		return NextStateResult(std::make_unique<PrePlayerDecision>());
	}

	return NextStateResult();
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

CardBurnState::CardBurnState(Players & players, int currentPlayer, const std::queue<int> & playersBurn, bool developmentUsed)
	: m_currentPlayer(currentPlayer)
	, m_currentPlayerDevUsed(developmentUsed)
	, m_playersBurn(playersBurn)
{
	setBurnActivePlayer(players);
}

bool CardBurnState::isValid(const Action & action) const
{
	return action.getType() == ActionType::CardBurn;
}

NextStateResult CardBurnState::computeNextState(Players & players, const Action & action)
{
	if (m_playersBurn.empty())
	{
		players.setNextActivePlayer(m_currentPlayer);
		return NextStateResult(std::make_unique<MovingRobberState>(m_currentPlayerDevUsed));
	}
	else
	{
		setBurnActivePlayer(players);
	}

	return NextStateResult();
}

std::vector<ActionType> CardBurnState::getPossibleActions()
{
	return { ActionType::CardBurn };
}

void CardBurnState::setBurnActivePlayer(Players & players)
{
	players.setNextActivePlayer(m_playersBurn.front());
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

NextStateResult MovingRobberState::computeNextState(Players & players, const Action & /*action*/)
{
	return NextStateResult(std::make_unique<PlayerDecision>(m_developmentUsed));
}

std::vector<ActionType> MovingRobberState::getPossibleActions()
{
	return { ActionType::MoveRobber };
}
