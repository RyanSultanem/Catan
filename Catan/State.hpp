#ifndef STATE_HPP
#define STATE_HPP

#include "Actions.fwd.hpp"
#include "Game.fwd.hpp"

#include <queue>

class State
{
public:
	virtual bool isValid(const Action & action) const = 0;
	virtual void nextState(Game & game, const Action & action) = 0; // TODO: might need to rename.
};

class InitialSettlementState : public State
{
public:
	bool isValid(const Action & action) const override;
	void nextState(Game& game, const Action & action) override;

private:
	void updateGameSecondRun(Game & game) const;
};

class PrePlayerDecision : public State
{
public:
	bool isValid(const Action & action) const override;
	void nextState(Game & game, const Action & action) override;
};

class PlayerDecision : public State
{
public:
	bool isValid(const Action & action) const override;
	void nextState(Game & game, const Action & action) override;
};

class CardBurnState : public State
{
public:
	explicit CardBurnState(int currentPlayer, const std::queue<int> & playersBurn);

	bool isValid(const Action & action) const override;
	void nextState(Game & game, const Action & action) override;

private:
	int m_currentPlayer;
	std::queue<int> m_playersBurn;
};

class MovingRobberState : public State
{
public:
	bool isValid(const Action & action) const override;
	void nextState(Game & game, const Action & action) override;
};

#endif // !STATE_HPP