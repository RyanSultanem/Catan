#include "Actions.hpp"

#include "Board.h"
#include "Dice.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "PlayerReactions.hpp"
#include "Conditions.hpp"
#include "Utility.hpp"
#include "Development.hpp"
#include <iterator>

PlaceSettlementAction::PlaceSettlementAction(player::Player & player, int position)
	: m_player(player)
	, m_position(position)
{
}

bool PlaceSettlementAction::execute(board::Board & board)
{
	bool isSuccess = preExecute();

	std::optional<token::building::SettlementRef> optSettlement = m_player.getSettlement();

	if (isSuccess && optSettlement)
	{
		isSuccess = board.placeSettlement(m_position, *optSettlement, PlaceSettlementCondition(m_player.getId()));
			
		if(isSuccess)
			postExecute(board);
	}

	return isSuccess;
}

ActionType PlaceSettlementAction::getType() const
{
	return ActionType::PlaceSettlement;
}

bool PlaceSettlementAction::preExecute() const
{
	return player::reactions::settlementRessourcesAvailable(m_player);
}

namespace {

void updateHarborPlacement(player::Player & player, const board::Board & board, int settlementPosition)
{
	const std::optional<HarborCRef> & optHarbor = board.getHarbor(settlementPosition);
	if (optHarbor)
		player::reactions::settlementPlacedOnHarbor(player, *optHarbor);
}

} // namesapce anonymous

void PlaceSettlementAction::postExecute(const board::Board & board) const
{
	if (player::reactions::settlementPay(m_player))
	{
		player::reactions::settlmentPlaced(m_player);
		updateHarborPlacement(m_player, board, m_position);
	}
}

PlaceInitialSettlementRoadAction::PlaceInitialSettlementRoadAction(player::Player & player, int settlementPosition, int roadPosition, bool secondRun)
	: m_player(player)
	, m_settlementPosition(settlementPosition)
	, m_roadPosition(roadPosition)
	, m_secondRun(secondRun)
{
}

bool PlaceInitialSettlementRoadAction::execute(board::Board & board)
{
	bool isSuccess = false;

	std::optional<token::building::SettlementRef> optSettlement = m_player.getSettlement();
	std::optional<token::RoadRef> optRoad = m_player.getRoad();

	if (optSettlement && optRoad)
	{
		// TODO: fix issue if place settlmenet is succcessful bit road isnt. preExecute with conditions?
		// TODO: check road and settlement are adjencebt before placing
		isSuccess = board.placeSettlement(m_settlementPosition, *optSettlement, PlaceInitialSettlementCondition(m_player.getId()));
		isSuccess = isSuccess && board.placeRoad(m_roadPosition, *optRoad, PlaceInitialRoadCondition(m_player.getId(), m_settlementPosition));

		if (isSuccess)
			postExecute(board);
	}

	return isSuccess;
}

ActionType PlaceInitialSettlementRoadAction::getType() const
{
	return ActionType::PlaceInitialSettlementRoad;
}

void PlaceInitialSettlementRoadAction::postExecute(const board::Board & board) const
{
	player::reactions::settlmentPlaced(m_player);
	player::reactions::roadPlaced(m_player);
	
	updateHarborPlacement(m_player, board, m_settlementPosition);

	if (m_secondRun)
	{
		const std::vector<card::RessourceType> ressources = board.getRessourcesFromVertexPosition(m_settlementPosition);
		std::for_each(ressources.begin(), ressources.end(), 
			[this](const card::RessourceType ressource)
		{
			m_player.addRessource(ressource, 1);
		});
	}
}

PlaceRoadAction::PlaceRoadAction(player::Player & player, int position)
	: m_player(player)
	, m_position(position)
{
}

bool PlaceRoadAction::execute(board::Board & board)
{
	bool isSuccess = preExecute();

	std::optional<token::RoadRef> optRoad = m_player.getRoad();

	if(isSuccess && optRoad)
	{
		isSuccess = board.placeRoad(m_position, *optRoad, PlaceRoadCondition(m_player.getId()));

		if (isSuccess)
			postExecute();
	}

	return isSuccess;
}

ActionType PlaceRoadAction::getType() const
{
	return ActionType::PlaceRoad;
}

bool PlaceRoadAction::preExecute() const
{
	return player::reactions::roadRessourcesAvailable(m_player);
}

void PlaceRoadAction::postExecute() const
{
	if(player::reactions::roadPay(m_player))
		player::reactions::roadPlaced(m_player);
   // check for longest here
}

PlaceCityAction::PlaceCityAction(player::Player & player, int position)
	: m_player(player)
	, m_position(position)
{
}

bool PlaceCityAction::execute(board::Board & board)
{
	bool isSuceess = preExecute();

	std::optional<token::building::CityRef> optCity = m_player.getCity();

	if(isSuceess && optCity)
	{
		isSuceess = board.placeCity(m_position, *optCity, PlaceCityCondition(m_player.getId()));

		if (isSuceess)
			postExecute();
	}

	return isSuceess;
}

ActionType PlaceCityAction::getType() const
{
	return ActionType::PlaceCity;
}

bool PlaceCityAction::preExecute() const
{
	return player::reactions::cityRessourcesAvailable(m_player);
}

void PlaceCityAction::postExecute() const
{
	if(player::reactions::cityPay(m_player))
		player::reactions::cityPlaced(m_player);
}

RollDice::RollDice(std::vector<player::Player> & players, int activePlayer)
	: m_players(players)
	, m_activePlayer(activePlayer)
	, m_shouldChangeRobber(false)
	, m_shouldBurn(false)
{
}

bool RollDice::execute(board::Board & board)
{
	board::Dice dice;
	int diceValue = dice.roll().getValue();

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
	m_game.setNextActivePlayer();

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
	return player::reactions::performExchangeCards(m_player, m_typeResult, m_typeToTrade);
}

ActionType ExchangeCardsAction::getType() const
{
	return ActionType::ExchangeCards;
}

MoveRobberAction::MoveRobberAction(player::Player & player, int cellPosition, int vertexPosition, std::vector<player::Player> & players)
	: m_player(player)
	, m_cellPosition(cellPosition)
	, m_vertexPosition(vertexPosition)
	, m_players(players)
{
}

bool MoveRobberAction::execute(board::Board & board)
{
	bool isSucess = board.moveRobber(m_cellPosition);

	if (isSucess)
	{
		std::optional<int> playerRef = board.getVertexPlayerRef(m_vertexPosition);
		if(playerRef)
		{
			player::Player & fromPlayer = m_players.at(playerRef.value());
			player::reactions::stealPlayerCard(m_player, fromPlayer);
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
	, m_ressourcesToBurn(ressourcesToBurn)
{
}

bool CardBurnAction::execute(board::Board & /*board*/)
{
	if (!preExecute())
		return false;

	std::unordered_map<card::RessourceType, int> realRessources(m_ressourcesToBurn.size());

	std::transform(m_ressourcesToBurn.begin(), m_ressourcesToBurn.end(), std::inserter(realRessources, realRessources.begin()),
		[](const std::pair<int, int> & element)
	{
		return std::pair<card::RessourceType, int>(static_cast<card::RessourceType>(element.first), element.second);
	});

	return player::reactions::burnCards(m_player, realRessources);
}

ActionType CardBurnAction::getType() const
{
	return ActionType::CardBurn;
}

bool CardBurnAction::preExecute() const
{
	return m_player.getNumberOfRessources() / 2 == utility::getCount(m_ressourcesToBurn);
}

BuyDevelopmentAction::BuyDevelopmentAction(player::Player & player, DevelopmentStock & developmentStock)
	: m_player(player)
	, m_developmentStock(developmentStock)
{
}

bool BuyDevelopmentAction::execute(board::Board & board)
{
	bool isSuccess = preExecute();

	if (isSuccess)
	{
		if(player::reactions::developmentPay(m_player))
			m_player.receiveDevelopment(*m_developmentStock.drawCard());
	}

	return isSuccess;
}

ActionType BuyDevelopmentAction::getType() const
{
	return ActionType::BuyDevelopment;
}

bool BuyDevelopmentAction::preExecute()
{
	return player::reactions::developmentRessourceAvailable(m_player) && !m_developmentStock.empty();
}

UseDevelopmentAction::UseDevelopmentAction(player::Player & player, const card::DevelopmentType & developmentType, const card::DevelopmentData & developmentData)
   : m_player(player)
   , m_developmentType(developmentType)
   , m_developmentData(developmentData)
{
}

bool UseDevelopmentAction::execute(board::Board & board)
{
    std::optional<card::DevelopmentRef> optDevelopmentCard = m_player.getUnusedDevelopment(m_developmentType);
	if (!optDevelopmentCard)
		return false;

	card::Development & developmentCard = *optDevelopmentCard;
    return developmentCard.executeAction(m_player, m_developmentData);

    // check for longest and strongest here
}

ActionType UseDevelopmentAction::getType() const
{
   return ActionType::UseDevelopment;
}

