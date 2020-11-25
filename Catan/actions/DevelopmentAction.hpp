#ifndef ACTIONS_DEVELOPMENTACTION_HPP
#define ACTIONS_DEVELOPMENTACTION_HPP

#include <actions/Actions.hpp>

#include <Achievement.fwd.hpp>

#include <card/DevelopmentStock.fwd.hpp>
#include <card/Development.fwd.hpp>

#include <player/Player.fwd.hpp>

class BuyDevelopmentAction : public Action
{
public:
	explicit BuyDevelopmentAction(player::Player & player, DevelopmentStock & developmentStock);

	bool execute() override;
	ActionType getType() const override;

private:
	player::Player & m_player;
	DevelopmentStock & m_developmentStock;

	bool preExecute();
};

class UseDevelopmentAction : public Action
{
public:
	UseDevelopmentAction(player::Player & player, const card::DevelopmentData & developmentData, Achievement & strongestArmy);

	bool execute() override;
	ActionType getType() const override;

	bool validPrePlayerDecisionUse(bool developmentUsed) const;
	bool validPlayerDecisionUse(bool developmentUsed) const;

private:
	player::Player & m_player;
	const card::DevelopmentData & m_developmentData;
	Achievement & m_strongestArmy;
};

#endif //!ACTIONS_DEVELOPMENTACTION_HPP