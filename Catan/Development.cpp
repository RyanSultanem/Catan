#include "Development.hpp"

#include "Actions.hpp"
#include "Player.hpp"
#include "PlayerReactions.hpp"
#include "Board.h"
#include "Conditions.hpp"
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

DevelopmentData::DevelopmentData(int playerId)
   : m_cellVertexPosition(std::nullopt)
   , m_monopolyRessource(std::nullopt)
   , m_freeRessources(std::nullopt)
   , m_roadPositions(std::nullopt)
{
}

bool Development::execute(player::Player & player, const DevelopmentData & data) const
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

KnightDevelopment::KnightDevelopment(std::vector<player::Player> & players, board::Board & board)
   : m_players(players)
   , m_board(board)
{
}

bool KnightDevelopment::validData(const DevelopmentData & data) const
{
   return data.getCellVertexPosition().has_value();
}

bool KnightDevelopment::apply(player::Player & player, const DevelopmentData & data) const
{
   std::pair<int,int> cellVertexPositions = data.getCellVertexPosition().value();
   MoveRobberAction moveRobberAction(player, cellVertexPositions.first, cellVertexPositions.second, m_players);
   return moveRobberAction.execute(m_board);
}

bool FreeRessourcesDevelopment::validData(const DevelopmentData & data) const
{
   return data.getFreeRessources().has_value();
}

bool FreeRessourcesDevelopment::apply(player::Player & player, const DevelopmentData & data) const
{
   std::pair<card::RessourceType, card::RessourceType> ressources = data.getFreeRessources().value();
   player.addRessource(ressources.first, 1);
   player.addRessource(ressources.second, 1);

   return true;
}

BuildTwoFreeRoads::BuildTwoFreeRoads(board::Board & board)
   : m_board(board)
{
}

bool BuildTwoFreeRoads::validData(const DevelopmentData & data) const
{
   return data.getRoadPositions().has_value();
}

bool BuildTwoFreeRoads::apply(player::Player & player, const DevelopmentData & data) const
{
   // TODO: To rewrite/refactor URGENT, i think its correct though..

   int position1 = data.getRoadPositions().value().first;
   int position2 = data.getRoadPositions().value().second;

   std::optional<token::RoadRef> optRoad1 = player.getRoad();

   if (!optRoad1)
      return false;

   PlaceRoadCondition condition(player.getId());
   bool firstRoadPositionPlaced = m_board.placeRoad(position1, *optRoad1, condition);

   if (firstRoadPositionPlaced)
   {
      player::reactions::roadPlaced(player);
      std::optional<token::RoadRef> optRoad2 = player.getRoad();
      if (optRoad2)
      {
         bool secondRoadPositionPlaced = m_board.placeRoad(position2, *optRoad2, condition);
         if (secondRoadPositionPlaced)
            player::reactions::roadPlaced(player);
      }
   }
   else
   {
      bool secondRoadPositionPlaced = m_board.placeRoad(position2, *optRoad1, condition);
      if (secondRoadPositionPlaced)
      {
         player::reactions::roadPlaced(player);
         std::optional<token::RoadRef> optRoad2 = player.getRoad();
         if (optRoad2)
         {
            bool firstRoadPositionPlaced = m_board.placeRoad(position1, *optRoad2, condition);
            if (firstRoadPositionPlaced)
               player::reactions::roadPlaced(player);
         }
      }
      else
         return false; // TODO: check.
   }

   return true;
}

MonopolyDevelopment::MonopolyDevelopment(std::vector<player::Player> & players)
   : m_players(players)
{
}

bool MonopolyDevelopment::validData(const DevelopmentData & data) const
{
   return data.getMonopolyRessource().has_value();
}

bool MonopolyDevelopment::apply(player::Player & player, const DevelopmentData & data) const
{
   std::for_each(m_players.begin(), m_players.end(),
      [&player, &data](player::Player & sender)
   {
      card::RessourceType ressource = data.getMonopolyRessource().value();
      player::reactions::stealAllRessources(player, sender, ressource);
   });

   return true;
}

bool VictoryPointDevelopment::validData(const DevelopmentData & /*data*/) const
{
   return true;
}

bool VictoryPointDevelopment::apply(player::Player & player, const DevelopmentData & /*data*/) const
{
   player.receivePoints(1);
   return true;
}


} // namespace card

