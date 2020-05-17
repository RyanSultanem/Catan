#include <actions/PlaceTokenAction.hpp>

#include <Achievement.hpp>

#include <board/Board.hpp>

#include <player/Player.hpp>
#include <player/PlayerReactions.hpp>

#include <token/Conditions.hpp>

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

		if (isSuccess)
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
	std::optional<token::building::SettlementRef> settlmentOpt = m_player.getSettlement();
	return settlmentOpt ? player::reactions::tokenRessourcesAvailable(m_player, settlmentOpt.value()) : false;
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
	if (player::reactions::tokenPayRessources(m_player, m_player.getSettlement().value()))
	{
		player::reactions::tokenPlaced(m_player, m_player.getSettlement().value());
		updateHarborPlacement(m_player, board, m_position);
	}
}

PlaceInitialSettlementRoadAction::PlaceInitialSettlementRoadAction(player::Player & player, int settlementPosition, int roadPosition)
	: m_player(player)
	, m_settlementPosition(settlementPosition)
	, m_roadPosition(roadPosition)
	, m_secondRun(false)
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
	else
		isSuccess = false;

	return isSuccess;
}

ActionType PlaceInitialSettlementRoadAction::getType() const
{
	return ActionType::PlaceInitialSettlementRoad;
}

void PlaceInitialSettlementRoadAction::setSecondRun(bool secondRun)
{
	m_secondRun = secondRun;
}

void PlaceInitialSettlementRoadAction::postExecute(const board::Board & board) const
{
	player::reactions::tokenPlaced(m_player, m_player.getSettlement().value());
	player::reactions::tokenPlaced(m_player, m_player.getRoad().value());

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

PlaceRoadAction::PlaceRoadAction(player::Player & player, int position, Achievement & longestRoad)
	: m_player(player)
	, m_position(position)
	, m_longestRoad(longestRoad)
{
}

bool PlaceRoadAction::execute(board::Board & board)
{
	bool isSuccess = preExecute(m_player);

	std::optional<token::RoadRef> optRoad = m_player.getRoad();

	if (isSuccess && optRoad)
	{
		isSuccess = board.placeRoad(m_position, *optRoad, PlaceRoadCondition(m_player.getId()));

		if (isSuccess)
			postExecute(board);
	}
	else
		isSuccess = false;

	return isSuccess;
}

ActionType PlaceRoadAction::getType() const
{
	return ActionType::PlaceRoad;
}

bool PlaceRoadAction::preExecute(player::Player & player) const
{
	std::optional<token::RoadRef> roadOpt = player.getRoad();
	return roadOpt ? player::reactions::tokenRessourcesAvailable(player, roadOpt.value()) : false;
}

void PlaceRoadAction::postExecute(board::Board & board) const
{
	if (player::reactions::tokenPayRessources(m_player, m_player.getRoad().value()))
	{
		player::reactions::tokenPlaced(m_player, m_player.getRoad().value());
		m_longestRoad.update(m_player, LongestRoadChecker(*board.getEdgeAt(m_position)));
	}
}

PlaceCityAction::PlaceCityAction(player::Player & player, int position)
	: m_player(player)
	, m_position(position)
{
}

bool PlaceCityAction::execute(board::Board & board)
{
	bool isSuccess = preExecute();

	std::optional<token::building::CityRef> optCity = m_player.getCity();

	if (isSuccess && optCity)
	{
		isSuccess = board.placeCity(m_position, *optCity, PlaceCityCondition(m_player.getId()));

		if (isSuccess)
			postExecute();
	}
	else
		isSuccess = false;

	return isSuccess;
}

ActionType PlaceCityAction::getType() const
{
	return ActionType::PlaceCity;
}

bool PlaceCityAction::preExecute() const
{
	std::optional<token::building::CityRef> cityOpt = m_player.getCity();
	return cityOpt ? player::reactions::tokenRessourcesAvailable(m_player, cityOpt.value()) : false;
}

void PlaceCityAction::postExecute() const
{
	if (player::reactions::tokenPayRessources(m_player, m_player.getCity().value()))
		player::reactions::tokenPlaced(m_player, m_player.getCity().value());
}

PlaceFreeRoadAction::PlaceFreeRoadAction(player::Player & player, int position, Achievement & longestRoad)
	: PlaceRoadAction(player, position, longestRoad)
{
}

bool PlaceFreeRoadAction::preExecute(player::Player & player) const
{
	return player.getRoad() != std::nullopt;
}