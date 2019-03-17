#ifndef STATE_HPP
#define STATE_HPP

#include "Actions.fwd.hpp"
#include "Game.fwd.hpp"

#include <vector>
#include <queue>

// TODO: Rename to Phase?
class State
{
public:
	virtual bool isValid(const Action & action) const = 0;
	virtual void nextState(Game & game, const Action & action) = 0; // TODO: might need to rename.

	virtual std::vector<ActionType> getPossibleActions() = 0;
};

class InitialSettlementState : public State // TODO: Fix name
{
public:
	InitialSettlementState();

	bool isValid(const Action & action) const override;
	void nextState(Game & game, const Action & action) override;

	std::vector<ActionType> getPossibleActions() override;
	void preProcessAction(PlaceInitialSettlementRoadAction & action);

private:
	bool m_secondRun;

	bool updateGameSecondRun(Game & game);
};

class PrePlayerDecision : public State
{
public:
	bool isValid(const Action & action) const override;
	void nextState(Game & game, const Action & action) override;

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
	void nextState(Game & game, const Action & action) override;

	std::vector<ActionType> getPossibleActions() override;

private:
	bool m_developmentUsed;

	bool validDevelopmentUse(const Action & action) const;
};

class CardBurnState : public State
{
public:
	explicit CardBurnState(Game & game, int currentPlayer, const std::queue<int> & playersBurn);

	bool isValid(const Action & action) const override;
	void nextState(Game & game, const Action & action) override;

	std::vector<ActionType> getPossibleActions() override;

private:
	int m_currentPlayer;
	std::queue<int> m_playersBurn;

	void setBurnActivePlayer(Game & game);
};

class MovingRobberState : public State
{
public:
	bool isValid(const Action & action) const override;
	void nextState(Game & game, const Action & action) override;

	std::vector<ActionType> getPossibleActions() override;
};

#endif // !STATE_HPP