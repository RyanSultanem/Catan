#ifndef STATE_HPP
#define STATE_HPP

#include <State.fwd.hpp>

#include <actions/Actions.fwd.hpp>

#include <Players.fwd.hpp>

#include <memory>
#include <queue>
#include <vector>

class NextStateResult
{
public:
	NextStateResult();
	explicit NextStateResult(std::unique_ptr<State> && newState);

	bool getIsUpdated() const;
	std::unique_ptr<State> getNewState();

private:
	bool m_updated = false; // TODO: can be replaced by an interface and 2 subclasses - UpdatedState SameState with NexStateResult as interface - Probably not worth it and better this way.
	std::unique_ptr<State> m_newState = nullptr;
};

// TODO: Rename to Phase?
class State
{
public:
	virtual bool isValid(const Action & action) const = 0;
	virtual NextStateResult nextState(Players & players, const Action & action) = 0; // TODO: might need to rename.

	virtual std::vector<ActionType> getPossibleActions() = 0;
};

class InitialSettlementState : public State // TODO: Fix name
{
public:
	InitialSettlementState();

	bool isValid(const Action & action) const override;
	NextStateResult nextState(Players & players, const Action & action) override;

	std::vector<ActionType> getPossibleActions() override;
	void preProcessAction(PlaceInitialSettlementRoadAction & action);

private:
	bool m_secondRun;

	bool updatePlayersSecondRun(Players & players);
};

class PrePlayerDecision : public State
{
public:
	bool isValid(const Action & action) const override;
	NextStateResult nextState(Players & players, const Action & action) override;

	std::vector<ActionType> getPossibleActions() override;

private:
	bool m_developmentUsed = false;

	bool validDevelopmentUse(const Action & action) const;
};

class PlayerDecision : public State
{
public:
	explicit PlayerDecision(bool developmentUsed);

	bool isValid(const Action & action) const override;
	NextStateResult nextState(Players & players, const Action & action) override;

	std::vector<ActionType> getPossibleActions() override;

private:
	bool m_developmentUsed;

	bool validDevelopmentUse(const Action & action) const;
};

class CardBurnState : public State
{
public:
	explicit CardBurnState(Players & players, int currentPlayer, const std::queue<int> & playersBurn, bool developmentUsed);

	bool isValid(const Action & action) const override;
	NextStateResult nextState(Players & players, const Action & action) override;

	std::vector<ActionType> getPossibleActions() override;

private:
	int m_currentPlayer;
	bool m_currentPlayerDevUsed;
	std::queue<int> m_playersBurn;

	void setBurnActivePlayer(Players & players);
};

class MovingRobberState : public State
{
public:
	explicit MovingRobberState(bool developmentUsed);

	bool isValid(const Action & action) const override;
	NextStateResult nextState(Players & players, const Action & action) override;

	std::vector<ActionType> getPossibleActions() override;

private:
	bool m_developmentUsed;
};

#endif // !STATE_HPP