#include <actions/PlayerAction.hpp>

#include <board/Board.hpp>
#include <board/Dice.hpp>

#include <Players.hpp>

#include <player/Player.hpp>
#include <player/PlayerReactions.hpp>

#include <token/Building.hpp>

#include <utility/NumberGenerator.hpp>
#include <utility/Utility.hpp>

#include <iterator>

RollDice::RollDice(board::Board & board, board::Dice & dice, Players & players)
	: m_board(board)
	, m_dice(dice)
	, m_players(players)
	, m_shouldChangeRobber(false)
	, m_playerBurn()
{
}

bool RollDice::execute()
{
	int diceValue = m_dice.roll().getValue();

	if (diceValue == 7)
	{
		m_shouldChangeRobber = true;
		checkCardBurn();
	}

	giveRessources(m_board, diceValue);

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
	return !m_playerBurn.empty();
}

const std::queue<int>& RollDice::getPlayerBurnQueue() const
{
	return m_playerBurn;
}

void RollDice::giveRessources(const board::Board & board, int diceValue) const
{
	// TODO: ...ugly
	// TODO: should be moved under player::reactions
	const std::vector<board::CellCRef> activeCells = board.getCellsWithNumber(diceValue);

	std::for_each(activeCells.begin(), activeCells.end(),
		[this](const board::Cell & activeCell)
		{
			const std::vector<token::building::Building*> activeBuildings = activeCell.getActiveBuildings();

			std::for_each(activeBuildings.begin(), activeBuildings.end(),
				[this, &activeCell](const token::building::Building * activeBuilding)
				{
					player::Player & player = m_players.getActivePlayer();
					player.addRessource(activeCell.getRessource(), activeBuilding->getPoints());
				});
		});
}

void RollDice::checkCardBurn()
{
	int numberOfPlayers = m_players.getNumberOfPlayers();
	int playerIndex = m_players.getActivePlayerId();

	// TODO: kind of weird way to do the check.. but might be okay
	// TODO: should CERTAINLY NOT be here and expose the queue.
	do
	{
		playerIndex = (playerIndex + 1) % numberOfPlayers; // Start with next player
		if (m_players.getPlayer(playerIndex).getNumberOfRessources() >= 8) // TODO: Magic Number
		{
			m_playerBurn.push(playerIndex);
		}
	} while (playerIndex != m_players.getActivePlayerId());
}

Done::Done()
{
}

bool Done::execute()
{
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

bool ExchangeCardsAction::execute()
{
	return player::reactions::performExchangeCards(m_player, static_cast<card::Ressource>(m_typeResult), static_cast<card::Ressource>(m_typeToTrade));
}

ActionType ExchangeCardsAction::getType() const
{
	return ActionType::ExchangeCards;
}

MoveRobberAction::MoveRobberAction(Players & players, board::Board & board, int cellPosition, int vertexPosition, const NumberGenerator & numberGenerator)
	: m_players(players)
	, m_board(board)
	, m_cellPosition(cellPosition)
	, m_vertexPosition(vertexPosition)
	, m_numberGenerator(numberGenerator)
{
}

bool MoveRobberAction::execute()
{
	bool isSuccess = preExecute();
	
	if (isSuccess)
	{
		isSuccess = m_board.moveRobber(m_cellPosition);

		if (isSuccess)
			postExecute();
	}

	return isSuccess;
}

ActionType MoveRobberAction::getType() const
{
	return ActionType::MoveRobber;
}

bool MoveRobberAction::preExecute()
{
	bool validCellToBeRobbed = m_board.getRobbedCell().has_value() ? m_board.getRobbedCell()->get().getId() != m_cellPosition : true;

	return m_board.checkAdjacent(m_cellPosition, m_vertexPosition)
		&& validCellToBeRobbed;
}

void MoveRobberAction::postExecute()
{
	std::optional<int> playerRef = m_board.getVertexPlayerRef(m_vertexPosition);
	if (playerRef)
	{
		player::Player & fromPlayer = m_players.getPlayer(playerRef.value());
		int ressourceIndex = m_numberGenerator.generateNumber(1, fromPlayer.getNumberOfRessources());
		player::reactions::stealPlayerCard(m_players.getActivePlayer(), fromPlayer, ressourceIndex);
	}
}

CardBurnAction::CardBurnAction(player::Player & player, const std::unordered_map<int, int> & ressourcesToBurn)
	: m_player(player)
	, m_ressourcesToBurn(ressourcesToBurn.size())
{
	std::transform(ressourcesToBurn.begin(), ressourcesToBurn.end(), std::inserter(m_ressourcesToBurn, m_ressourcesToBurn.begin()),
		[](const std::pair<int, int> & element)
		{
			return std::pair<card::Ressource, int>(static_cast<card::Ressource>(element.first), element.second);
		});
}

bool CardBurnAction::execute()
{
	if (!preExecute())
		return false;

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
