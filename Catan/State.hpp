#ifndef STATE_HPP
#define STATE_HPP

#include "Actions.fwd.hpp"
#include "Game.fwd.hpp"

class State
{
public:
	virtual bool isValid(const Action & action) const = 0;
	virtual void nextState(Game & game, const Action & action) const = 0;
};

class InitialSettlementState : public State
{
public:
	bool isValid(const Action & action) const override;
	void nextState(Game& game, const Action & action) const override;

private:
	void updateGameSecondRun(Game & game) const;
};

class PrePlayerDecision : public State
{
public:
	bool isValid(const Action & action) const override;
	void nextState(Game & game, const Action & action) const override;
};

class PlayerDecision : public State
{
public:
	bool isValid(const Action & action) const override;
	void nextState(Game & game, const Action & action) const override;
};

#endif // !STATE_HPP