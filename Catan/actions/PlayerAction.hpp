#ifndef ACTIONS_PLAYERACTION_HPP
#define ACTIONS_PLAYERACTION_HPP

#include <actions/Actions.hpp>

#include <board/Board.fwd.hpp>
#include <board/Dice.fwd.hpp>

#include <card/Card.hpp>

#include <Players.fwd.hpp>
#include <player/Player.fwd.hpp>

#include <utility/NumberGenerator.fwd.hpp>

#include <queue>

class RollDice : public Action
{
public:
	RollDice(board::Board & board, board::Dice & dice, Players & players);

	bool execute() override;
	ActionType getType() const override;

	bool shouldChangeRobber() const;
	bool shouldBurn() const;
	const std::queue<int> & getPlayerBurnQueue() const;

private:
	board::Board & m_board;
	board::Dice & m_dice;
	Players & m_players;

	bool m_shouldChangeRobber;
	std::queue<int> m_playerBurn;

	void giveRessources(const board::Board & board, int diceValue) const;
	void checkCardBurn();
};

class Done : public Action
{
public:
	Done();

	bool execute() override;
	ActionType getType() const override;
};


class ExchangeCardsAction : public Action
{
public:
	ExchangeCardsAction(player::Player & player, int typeResult, int typeToTrade);

	bool execute() override;
	ActionType getType() const override;

private:
	player::Player & m_player;
	int m_typeResult;
	int m_typeToTrade;
};

class MoveRobberAction : public Action
{
public:
	MoveRobberAction(Players & players, board::Board & board, int cellPosition, int vertexPosition, const NumberGenerator & numberGenerator);

	bool execute() override;
	ActionType getType() const override;

private:
	Players & m_players;
	board::Board & m_board;
	int m_cellPosition;
	int m_vertexPosition;
	const NumberGenerator & m_numberGenerator;

private:
	bool preExecute();
	void postExecute();
};

class CardBurnAction : public Action
{
public:
	CardBurnAction(player::Player & player, const std::unordered_map<int, int> & ressourcesToBurn);

	bool execute() override;
	ActionType getType() const override;

private:
	player::Player & m_player;
	std::unordered_map<card::Ressource, int> m_ressourcesToBurn;

	bool preExecute() const;
};

#endif //!ACTIONS_PLAYERACTION_HPP