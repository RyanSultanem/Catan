#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include "Board.fwd.h"
#include "Game.fwd.hpp"
#include "Player.fwd.hpp"

#include <vector>
#include <memory>

// TODO: can be replaced by doubledispatch with State. worth it?
// For now it makes more sense to only have a type.
enum class ActionType
{
	PlaceInitialSettlementRoad,
	PlaceSettlement,
	PlaceRoad,
	RollDice,
	Done,
	PlaceCity,
	ExchangeCards
};

class Action
{
public:
	virtual bool execute(board::Board & board) const = 0; // TODO: check if board should really be passed here or for each needed construcotr.
	virtual ActionType getType() const = 0;
};

class PlaceSettlementAction : public Action // TODO: mutualise code between all PlaceTokensAction
{
public:
	PlaceSettlementAction(player::Player & player, int position);

	bool execute(board::Board & board) const override;
	ActionType getType() const override;

private:
	player::Player & m_player;
	int m_position;

	bool preExecute() const;
	void postExecute(const board::Board & board) const;
};

class PlaceInitialSettlementRoadAction : public Action
{
public:
	PlaceInitialSettlementRoadAction(player::Player & player, int settlementPosition, int roadPosition, bool secondRun);

	bool execute(board::Board & board) const override;

	ActionType getType() const override;

private:
	player::Player & m_player;
	int m_settlementPosition;
	int m_roadPosition;
	bool m_secondRun;

	void postExecute(const board::Board & board) const;
};

class PlaceRoadAction : public Action
{
public:
	PlaceRoadAction(player::Player & player, int position);

	bool execute(board::Board & board) const override;
	ActionType getType() const override;

private:
	player::Player & m_player;
	int m_position;

	bool preExecute() const;
	void postExecute() const;
};

class PlaceCityAction : public Action
{
public:
	PlaceCityAction(player::Player & player, int position);

	bool execute(board::Board & board) const override;
	ActionType getType() const override;

private:
	player::Player & m_player;
	int m_position;

	bool preExecute() const;
	void postExecute() const;
};

class RollDice : public Action
{
public:
	explicit RollDice(std::vector<player::Player> & players);

	bool execute(board::Board & board) const override;
	ActionType getType() const override;

private:
	std::vector<player::Player> & m_players;

	void giveRessources(const board::Board & board, int diceValue) const;
};

class Done : public Action
{
public:
	explicit Done(Game & game); // TODO: Create an ActivePlayer class to be handled here instead of Game. Also could contain changing player logic.

	bool execute(board::Board & board) const override;
	ActionType getType() const override;

private:
	Game & m_game;
};


class ExchangeCardsAction : public Action
{
public:
	ExchangeCardsAction(player::Player & player, int typeResult, int typeToTrade);

	bool execute(board::Board& board) const override;
	ActionType getType() const override;

private:
	player::Player & m_player;
	int m_typeResult;
	int m_typeToTrade;
};

#endif // !ACTIONS_HPP