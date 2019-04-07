#ifndef ACTIONS_ACTIONS_HPP
#define ACTIONS_ACTIONS_HPP

#include <Achievement.fwd.hpp>
#include <Game.fwd.hpp>

#include <board/Board.fwd.hpp>
#include <board/Dice.fwd.hpp>

#include <card/Card.hpp>
#include <card/DevelopmentStock.fwd.hpp>
#include <card/Development.fwd.hpp>

#include <player/Player.fwd.hpp>

#include <utility/NumberGenerator.fwd.hpp>

#include <memory>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>

// TODO: split into different files per action type. for better readabilty

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
	ExchangeCards,
	MoveRobber,
	CardBurn,
	BuyDevelopment,
	UseDevelopment
};

class Action
{
public:
	virtual bool execute(board::Board & board) = 0; // TODO: check if board should really be passed here or for each needed construcotr.
	virtual ActionType getType() const = 0;
};

class PlaceSettlementAction : public Action // TODO: mutualise code between all PlaceTokensAction
{
public:
	PlaceSettlementAction(player::Player & player, int position);

	bool execute(board::Board & board) override;
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
	PlaceInitialSettlementRoadAction(player::Player & player, int settlementPosition, int roadPosition);

	bool execute(board::Board & board) override;

	ActionType getType() const override;
	void setSecondRun(bool secondRun);

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
	PlaceRoadAction(player::Player & player, int position, Achievement & longestRoad);

	bool execute(board::Board & board) override;
	ActionType getType() const override;

private:
	player::Player & m_player;
	int m_position;
	Achievement & m_longestRoad;

	bool preExecute() const;
	void postExecute(board::Board & board) const;
};

class PlaceCityAction : public Action
{
public:
	PlaceCityAction(player::Player & player, int position);

	bool execute(board::Board & board) override;
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
	RollDice(board::Dice & dice, std::vector<player::Player> & players, int activePlayerRef);

	bool execute(board::Board & board) override;
	ActionType getType() const override;

	bool shouldChangeRobber() const;
	bool shouldBurn() const;
	const std::queue<int> & getPlayerBurnQueue() const;

private:
	board::Dice & m_dice;
	std::vector<player::Player> & m_players;
	int m_activePlayer;

	bool m_shouldChangeRobber;
	bool m_shouldBurn;
	std::queue<int> m_playerBurn;

	void giveRessources(const board::Board & board, int diceValue) const;
	void checkCardBurn();
};

class Done : public Action
{
public:
	explicit Done(Game & game); // TODO: Create an ActivePlayer class to be handled here instead of Game. Also could contain changing player logic.

	bool execute(board::Board & board) override;
	ActionType getType() const override;

private:
	Game & m_game;
};


class ExchangeCardsAction : public Action
{
public:
	ExchangeCardsAction(player::Player & player, int typeResult, int typeToTrade);

	bool execute(board::Board& board) override;
	ActionType getType() const override;

private:
	player::Player & m_player;
	int m_typeResult;
	int m_typeToTrade;
};

class MoveRobberAction : public Action
{
public:
	MoveRobberAction(player::Player & player, int cellPosition, int vertexPosition, std::vector<player::Player> & players, const NumberGenerator & numberGenerator);

	bool execute(board::Board & board) override;
	ActionType getType() const override;

private:
	player::Player & m_player;
	std::vector<player::Player> & m_players;
	int m_cellPosition;
	int m_vertexPosition;
	const NumberGenerator & m_numberGenerator;
};

class CardBurnAction : public Action
{
public:
	CardBurnAction(player::Player & player, const std::unordered_map<int, int> & ressourcesToBurn);

	bool execute(board::Board & board) override;
	ActionType getType() const override;

private:
	player::Player & m_player;
	std::unordered_map<card::RessourceType, int> m_ressourcesToBurn;

	bool preExecute() const;
};

class BuyDevelopmentAction : public Action
{
public:
	explicit BuyDevelopmentAction(player::Player & player, DevelopmentStock & developmentStock);

	bool execute(board::Board & board) override;
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

	bool execute(board::Board & board) override;
	ActionType getType() const override;

	bool validPrePlayerDecisionUse(bool developmentUsed) const;
	bool validPlayerDecisionUse(bool developmentUsed) const;

private:
	player::Player & m_player;
	const card::DevelopmentData & m_developmentData;
	Achievement & m_strongestArmy;
};

#endif // !ACTIONS_ACTIONS_HPP