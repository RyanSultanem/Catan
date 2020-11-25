#ifndef ACTIONS_PLACETOEKENACTION_HPP
#define ACTIONS_PLACETOEKENACTION_HPP

#include <actions/Actions.hpp>

#include <Achievement.fwd.hpp>

#include <board/Board.fwd.hpp>

#include <player/Player.fwd.hpp>

class PlaceSettlementAction : public Action // TODO: mutualise code between all PlaceTokensAction?
{
public:
	PlaceSettlementAction(player::Player & player, board::Board & board, int position);

	bool execute() override;
	ActionType getType() const override;

private:
	player::Player & m_player;
	board::Board & m_board;
	int m_position;

	bool preExecute() const;
	void postExecute(const board::Board & board) const;
};

class PlaceInitialSettlementRoadAction : public Action
{
public:
	PlaceInitialSettlementRoadAction(player::Player & player, board::Board & board, int settlementPosition, int roadPosition);

	bool execute() override;

	ActionType getType() const override;
	void setSecondRun(bool secondRun);

private:
	player::Player & m_player;
	board::Board & m_board;
	int m_settlementPosition;
	int m_roadPosition;
	bool m_secondRun;

	void postExecute(const board::Board & board) const;
};

class PlaceRoadAction : public Action
{
public:
	PlaceRoadAction(player::Player & player, board::Board & board, int position, Achievement & longestRoad);

	bool execute() override;
	ActionType getType() const override;

private:
	player::Player & m_player;
	board::Board & m_board;
	int m_position;
	Achievement & m_longestRoad;

	virtual bool preExecute(player::Player & player) const;
	void postExecute(board::Board & board) const;
};

class PlaceFreeRoadAction : public PlaceRoadAction
{
public:
	PlaceFreeRoadAction(player::Player & player, board::Board & board, int position, Achievement & longestRoad);

private:
	bool preExecute(player::Player & player) const override;
};

class PlaceCityAction : public Action
{
public:
	PlaceCityAction(player::Player & player, board::Board & board, int position);

	bool execute() override;
	ActionType getType() const override;

private:
	player::Player & m_player;
	board::Board & m_board;
	int m_position;

	bool preExecute() const;
	void postExecute() const;
};

#endif // !ACTIONS_PLACETOEKENACTION_HPP