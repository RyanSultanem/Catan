#ifndef STATE_HPP
#define STATE_HPP

#include "Actions.fwd.hpp"
#include "Game.fwd.hpp"

#include <vector>
#include <queue>
#include "Actions.hpp"

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
};

class PlayerDecision : public State
{
public:
	bool isValid(const Action & action) const override;
	void nextState(Game & game, const Action & action) override;

	std::vector<ActionType> getPossibleActions() override;
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