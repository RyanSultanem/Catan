#ifndef ACTIONS_PLAYERACTION_HPP
#define ACTIONS_PLAYERACTION_HPP

#include <actions/Actions.hpp>

#include <board/Dice.fwd.hpp>

#include <card/Card.hpp>

#include <Players.fwd.hpp>
#include <player/Player.fwd.hpp>

#include <utility/NumberGenerator.fwd.hpp>

#include <queue>

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
	Done();

	bool execute(board::Board & board) override;
	ActionType getType() const override;
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

#endif //!ACTIONS_PLAYERACTION_HPP