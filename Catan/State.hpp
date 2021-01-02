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

	bool isUpdated() const;
	std::unique_ptr<State> takeNewState();

private:
	bool m_updated = false;
	std::unique_ptr<State> m_newState = nullptr;
};

// TODO: Rename to Phase?
// TODO: Change State to be non polymorphic and contains strategy to correct state, + becomes selfsufficient, no need to reconstruct in game
class State
{
public:
	virtual bool isValid(const Action & action) const = 0;
	virtual NextStateResult computeNextState(Players & players, const Action & action) = 0;

	virtual std::vector<ActionType> getPossibleActions() = 0;
};

class InitialSettlementState : public State
{
public:
	InitialSettlementState();

	bool isValid(const Action & action) const override;
	NextStateResult computeNextState(Players & players, const Action & action) override;

	std::vector<ActionType> getPossibleActions() override;
	void preProcessAction(PlaceInitialSettlementRoadAction & action);

private:
	bool m_secondRun;

	void updateSecondRun(const Players & players);
};

class PrePlayerDecision : public State
{
public:
	bool isValid(const Action & action) const override;
	NextStateResult computeNextState(Players & players, const Action & action) override;

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
	NextStateResult computeNextState(Players & players, const Action & action) override;

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
	NextStateResult computeNextState(Players & players, const Action & action) override;

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
	NextStateResult computeNextState(Players & players, const Action & action) override;

	std::vector<ActionType> getPossibleActions() override;

private:
	bool m_developmentUsed;
};

#endif // !STATE_HPP