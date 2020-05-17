#include <actions/PlayerAction.hpp>

#include <board/Board.hpp>
#include <board/Dice.hpp>

#include <player/Player.hpp>
#include <player/PlayerReactions.hpp>

#include <token/Building.hpp>

#include <utility/NumberGenerator.hpp>
#include <utility/Utility.hpp>

#include <iterator>

RollDice::RollDice(board::Dice & dice, std::vector<player::Player> & players, int activePlayer)
	: m_dice(dice)
	, m_players(players)
	, m_activePlayer(activePlayer)
	, m_shouldChangeRobber(false)
	, m_shouldBurn(false)
	, m_playerBurn()
{
}

bool RollDice::execute(board::Board & board)
{
	int diceValue = m_dice.roll().getValue();

	if (diceValue == 7)
	{
		m_shouldChangeRobber = true;
		checkCardBurn();
	}

	giveRessources(board, diceValue);

	return true;
}

ActionType RollDice::getType() const
{
	return ActionType::RollDice;
}

bool RollDice::shouldChangeRobber() const
{
	return m_shouldChangeRobber;
}

bool RollDice::shouldBurn() const
{
	return m_shouldBurn;
}

const std::queue<int>& RollDice::getPlayerBurnQueue() const
{
	return m_playerBurn;
}

void RollDice::giveRessources(const board::Board & board, int diceValue) const
{
	// TODO: ...ugly
	const std::vector<cell::CellCRef> activeCells = board.getCellsWithNumber(diceValue);

	std::for_each(activeCells.begin(), activeCells.end(),
		[this](const cell::Cell & activeCell)
		{
			const std::vector<token::building::Building*> activeBuildings = activeCell.getActiveBuildings();

			std::for_each(activeBuildings.begin(), activeBuildings.end(),
				[this, &activeCell](const token::building::Building * activeBuilding)
				{
					player::Player & player = m_players.at(activeBuilding->getReference());
					player.addRessource(activeCell.produceLandRessource().getType(), activeBuilding->getPoints());
				});
		});
}

void RollDice::checkCardBurn()
{
	int numberOfPlayers = m_players.size();
	int playerIndex = m_activePlayer;

	// TODO: kind of weird way to do the check.. but might be okay
	// TODO: should CERTAINLY NOT be here and expose the queue.
	do
	{
		playerIndex = (playerIndex + 1) % numberOfPlayers; // Start with next player
		if (m_players.at(playerIndex).getNumberOfRessources() >= 8) // TODO: Magic Number
		{
			m_playerBurn.push(playerIndex);
			m_shouldBurn = true;
		}
	} while (playerIndex != m_activePlayer);
}

Done::Done(Game & game)
	: m_game(game)
{
}

bool Done::execute(board::Board & /*board*/)
{
	// TODO: chcek if this should be here: for now it is also in state change. so removed below
	 //m_game.setNextActivePlayer();

	return true;
}

ActionType Done::getType() const
{
	return ActionType::Done;
}

ExchangeCardsAction::ExchangeCardsAction(player::Player & player, int typeResult, int typeToTrade)
	: m_player(player)
	, m_typeResult(typeResult)
	, m_typeToTrade(typeToTrade)
{
}

bool ExchangeCardsAction::execute(board::Board & /*board*/)
{
	return player::reactions::performExchangeCards(m_player, static_cast<card::RessourceType>(m_typeResult), static_cast<card::RessourceType>(m_typeToTrade));
}

ActionType ExchangeCardsAction::getType() const
{
	return ActionType::ExchangeCards;
}

MoveRobberAction::MoveRobberAction(player::Player & player, int cellPosition, int vertexPosition, std::vector<player::Player> & players, const NumberGenerator & numberGenerator)
	: m_player(player)
	, m_cellPosition(cellPosition)
	, m_vertexPosition(vertexPosition)
	, m_players(players)
	, m_numberGenerator(numberGenerator)
{
}

bool MoveRobberAction::execute(board::Board & board)
{
	// TODO: check cellPosition and VertexPosition are adjacent..
	bool isSucess = board.moveRobber(m_cellPosition);

	if (isSucess)
	{
		std::optional<int> playerRef = board.getVertexPlayerRef(m_vertexPosition);
		if (playerRef)
		{
			player::Player & fromPlayer = m_players.at(playerRef.value());
			int ressourceIndex = m_numberGenerator.generateNumber(1, fromPlayer.getNumberOfRessources());
			player::reactions::stealPlayerCard(m_player, fromPlayer, ressourceIndex);
		}
	}

	return isSucess;
}

ActionType MoveRobberAction::getType() const
{
	return ActionType::MoveRobber;
}

CardBurnAction::CardBurnAction(player::Player & player, const std::unordered_map<int, int> & ressourcesToBurn)
	: m_player(player)
	, m_ressourcesToBurn(ressourcesToBurn.size())
{
	std::transform(ressourcesToBurn.begin(), ressourcesToBurn.end(), std::inserter(m_ressourcesToBurn, m_ressourcesToBurn.begin()),
		[](const std::pair<int, int> & element)
		{
			return std::pair<card::RessourceType, int>(static_cast<card::RessourceType>(element.first), element.second);
		});
}

bool CardBurnAction::execute(board::Board & /*board*/)
{
	if (!preExecute())
		return false;

	/*std::unordered_map<card::RessourceType, int> realRessources(m_ressourcesToBurn.size());

	std::transform(m_ressourcesToBurn.begin(), m_ressourcesToBurn.end(), std::inserter(realRessources, realRessources.begin()),
		[](const std::pair<int, int> & element)
	{
		return std::pair<card::RessourceType, int>(static_cast<card::RessourceType>(element.first), element.second);
	});*/

	return player::reactions::burnCards(m_player, m_ressourcesToBurn);
}

ActionType CardBurnAction::getType() const
{
	return ActionType::CardBurn;
}

bool CardBurnAction::preExecute() const
{
	return m_player.getNumberOfRessources() / 2 == utility::getCount(m_ressourcesToBurn)
		&& player::reactions::ressourcesAvailable(m_player, m_ressourcesToBurn);

}
