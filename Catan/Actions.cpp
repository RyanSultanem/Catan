#include "Actions.hpp"

#include "Board.h"
#include "Dice.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "PlayerReactions.hpp"
#include "Conditions.hpp"

PlaceSettlementAction::PlaceSettlementAction(player::Player & player, int position)
	: m_player(player)
	, m_position(position)
{
}

bool PlaceSettlementAction::execute(board::Board & board) const
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

bool PlaceInitialSettlementRoadAction::execute(board::Board & board) const
{
	bool isSuccess = false;

	std::optional<token::building::SettlementRef> optSettlement = m_player.getSettlement();
	std::optional<token::RoadRef> optRoad = m_player.getRoad();

	if (optSettlement && optRoad)
	{
		// TODO: fix issue if place settlmenet is succcessful bit road isnt. preExecute with conditions?
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

bool PlaceRoadAction::execute(board::Board & board) const
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
}

PlaceCityAction::PlaceCityAction(player::Player & player, int position)
	: m_player(player)
	, m_position(position)
{
}

bool PlaceCityAction::execute(board::Board & board) const
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

RollDice::RollDice(std::vector<player::Player> & players)
	: m_players(players)
{
}

bool RollDice::execute(board::Board & board) const
{
	board::Dice dice;
	int diceNumber = dice.roll().getValue();

	giveRessources(board, diceNumber);

	return true;
}

ActionType RollDice::getType() const
{
	return ActionType::RollDice;
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

Done::Done(Game & game)
	: m_game(game)
{		
}

bool Done::execute(board::Board & /*board*/) const
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

bool ExchangeCardsAction::execute(board::Board & /*board*/) const
{
	return player::reactions::performExchangeCards(m_player, m_typeResult, m_typeToTrade);
}

ActionType ExchangeCardsAction::getType() const
{
	return ActionType::ExchangeCards;
}
