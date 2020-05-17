#include <card/Development.hpp>

#include <Achievement.hpp>

#include <actions/PlaceTokenAction.hpp>
#include <actions/PlayerAction.hpp>

#include <board/Board.hpp>
#include <board/factory/BoardFactory.hpp>

#include <player/Player.hpp>
#include <player/PlayerReactions.hpp>

#include <token/Conditions.hpp>

#include <algorithm>

namespace {

const std::unordered_map<card::RessourceType, int> DEVELOPMENT_COST = 
{
	{card::RessourceType::ORE, 1},
	{card::RessourceType::GRAIN, 1},
	{card::RessourceType::WOOL, 1}
};

} // namespace anonymous

namespace card {

Development::Development(const DevelopmentAction & devAction)
	: m_developmentAction(devAction)
	, m_used(false)
{
}

bool Development::isUsed() const
{
	return m_used;
}

bool Development::executeAction(player::Player & player, const DevelopmentData & data)
{
	bool result = m_developmentAction.execute(player, data);
	m_used = result;
	return result;
}

card::DevelopmentType Development::getType() const
{
	return m_developmentAction.getType();
}

std::string getDevelopmentTypeString(DevelopmentType developmentType)
{
	switch (developmentType)
	{
	case DevelopmentType::Knight:				return "K";
	case DevelopmentType::FreeRessources:		return "F";
	case DevelopmentType::BuildTwoFreeRoads:	return "B";
	case DevelopmentType::Monopoly:				return "M";
	case DevelopmentType::VictoryPoint:			return "V";
	default:									return "NA";
	}
}

std::string Development::serialize() const
{
	std::string devType(getDevelopmentTypeString(m_developmentAction.getType()));
	std::string isUsed(m_used ? "U" : "A");
	return std::string(devType + ":" + isUsed);
}

DevelopmentData::DevelopmentData()
	: m_developmentType(DevelopmentType::Knight)
	, m_cellVertexPosition(std::nullopt)
	, m_monopolyRessource(std::nullopt)
	, m_freeRessources(std::nullopt)
	, m_roadPositions(std::nullopt)
{
}

void DevelopmentData::setDevelopmentType(DevelopmentType developmentType)
{
	m_developmentType = developmentType;
}

void DevelopmentData::setCellVertexPosition(const std::pair<int, int> & cellVertexPosition)
{
	m_cellVertexPosition = cellVertexPosition;
}

void DevelopmentData::setMonopolyRessource(RessourceType monopolyRessource)
{
	m_monopolyRessource = monopolyRessource;
}

void DevelopmentData::setFreeRessources(const std::pair<RessourceType, RessourceType>& freeRessources)
{
	m_freeRessources = freeRessources;
}

void DevelopmentData::setRoadPosition(const std::pair<int, int> & roadPositions)
{
	m_roadPositions = roadPositions;
}

DevelopmentType DevelopmentData::getDevelopmentType() const
{
	return m_developmentType;
}

std::optional<std::pair<int, int>> DevelopmentData::getCellVertexPosition() const
{
	return m_cellVertexPosition;
}

std::optional<RessourceType> DevelopmentData::getMonopolyRessource() const
{
	return m_monopolyRessource;
}

std::optional<std::pair<RessourceType, RessourceType>> DevelopmentData::getFreeRessources() const
{
	return m_freeRessources;
}

std::optional<std::pair<int, int>> DevelopmentData::getRoadPositions() const
{
	return m_roadPositions;
}

bool DevelopmentAction::execute(player::Player & player, const DevelopmentData & data) const
{
   bool success = validData(data);

   if (success)
      success = apply(player, data);

   return success;
}

const std::unordered_map<card::RessourceType, int> & getDevelopmentCost()
{
	return DEVELOPMENT_COST;
}

KnightAction::KnightAction(std::vector<player::Player> & players, board::Board & board, Achievement & strongestArmy, const NumberGenerator & numberGenerator)
	: m_players(players)
	, m_board(board)
	, m_strongestArmy(strongestArmy)
	, m_numberGenerator(numberGenerator)
{
}

DevelopmentType KnightAction::getType() const
{
	return DevelopmentType::Knight;
}

bool KnightAction::validData(const DevelopmentData & data) const
{
   return data.getCellVertexPosition().has_value();
}

bool KnightAction::apply(player::Player & player, const DevelopmentData & data) const
{
   std::pair<int,int> cellVertexPositions = data.getCellVertexPosition().value();
   MoveRobberAction moveRobberAction(player, cellVertexPositions.first, cellVertexPositions.second, m_players, m_numberGenerator);
   bool result = moveRobberAction.execute(m_board);
   
   if (result)
	   m_strongestArmy.update(player, StrongestArmyChecker(player));;

   return result;
}

DevelopmentType FreeRessourcesAction::getType() const
{
	return DevelopmentType::FreeRessources;
}

bool FreeRessourcesAction::validData(const DevelopmentData & data) const
{
   return data.getFreeRessources().has_value();
}

bool FreeRessourcesAction::apply(player::Player & player, const DevelopmentData & data) const
{
   std::pair<card::RessourceType, card::RessourceType> ressources = data.getFreeRessources().value();
   player.addRessource(ressources.first, 1);
   player.addRessource(ressources.second, 1);

   return true;
}

BuildTwoFreeRoadsAction::BuildTwoFreeRoadsAction(board::Board & board, Achievement & longestRoad)
   : m_board(board)
   , m_longestRoad(longestRoad)
{
}

DevelopmentType BuildTwoFreeRoadsAction::getType() const
{
	return DevelopmentType::BuildTwoFreeRoads;
}

bool BuildTwoFreeRoadsAction::validData(const DevelopmentData & data) const
{
   return data.getRoadPositions().has_value();
}

static bool applyTwoRoads(board::Board & board, player::Player & player, const DevelopmentData & data, Achievement & longestRoad)
{
	int position1 = data.getRoadPositions().value().first;
	int position2 = data.getRoadPositions().value().second;

	PlaceFreeRoadAction buildRoad1(player, position1, longestRoad);
	PlaceFreeRoadAction buildRoad2(player, position2, longestRoad);

	bool result1 = buildRoad1.execute(board);
	bool result2 = buildRoad2.execute(board);

	if (result2 && !result1)
		result1 = buildRoad1.execute(board);
	else if (result1 && !result2)
		result2 = buildRoad2.execute(board);

	return result1 && result2;
}

bool BuildTwoFreeRoadsAction::apply(player::Player & player, const DevelopmentData & data) const
{
   if (!applyOnCopiesCheck(player, data))
	   return false;

   return applyTwoRoads(m_board, player, data, m_longestRoad);
}

bool BuildTwoFreeRoadsAction::applyOnCopiesCheck(player::Player & player, const DevelopmentData & data) const
{
	player::Player copyPlayer(player);
	board::Board copyBoard = board::BoardFactory().generateBoardCopy(m_board);
	Achievement copyLongestRoad(m_longestRoad);

	return applyTwoRoads(copyBoard, copyPlayer, data, copyLongestRoad);
}

MonopolyAction::MonopolyAction(std::vector<player::Player> & players)
   : m_players(players)
{
}

DevelopmentType MonopolyAction::getType() const
{
	return DevelopmentType::Monopoly;
}

bool MonopolyAction::validData(const DevelopmentData & data) const
{
   return data.getMonopolyRessource().has_value();
}

bool MonopolyAction::apply(player::Player & player, const DevelopmentData & data) const
{
   std::for_each(m_players.begin(), m_players.end(),
      [&player, &data](player::Player & sender)
   {
      card::RessourceType ressource = data.getMonopolyRessource().value();
      player::reactions::stealAllRessources(player, sender, ressource);
   });

   return true;
}

DevelopmentType VictoryPointAction::getType() const
{
	return DevelopmentType::VictoryPoint;
}

bool VictoryPointAction::validData(const DevelopmentData & /*data*/) const
{
   return true;
}

bool VictoryPointAction::apply(player::Player & player, const DevelopmentData & /*data*/) const
{
   player.receivePoints(1);
   return true;
}


} // namespace card

