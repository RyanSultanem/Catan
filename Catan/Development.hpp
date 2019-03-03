#ifndef CARD_DEVELOPMENT_HPP
#define CARD_DEVELOPMENT_HPP

#include "Card.h"
#include "Player.fwd.hpp"
#include "Board.fwd.h"
#include <unordered_map>

#include <optional>

namespace card {

enum DevelopmentType
{
   Knight,
   FreeRessources,
   BuildTwoFreeRoads,
   Monopoly,
   VictoryPoint
};

class DevelopmentData
{
public:
   explicit DevelopmentData(int playerId);

   void setCellVertexPosition(const std::pair<int,int> & cellVertexPosition);
   void setMonopolyRessource(int monopolyRessource);
   void setFreeRessources(const std::pair<RessourceType,RessourceType> & freeRessources);
   void setRoadPosition(const std::pair<int,int> & roadPositions);

   std::optional<std::pair<int,int>> getCellVertexPosition() const;
   std::optional<RessourceType> getMonopolyRessource() const;
   std::optional<std::pair<RessourceType, RessourceType>> getFreeRessources() const;
   std::optional<std::pair<int,int>> getRoadPositions() const;

private:
   std::optional<std::pair<int,int>> m_cellVertexPosition;
   std::optional<RessourceType> m_monopolyRessource;
   std::optional<std::pair<RessourceType,RessourceType>> m_freeRessources;
   std::optional<std::pair<int,int>> m_roadPositions;
};

class Development // should be a Token?
{
public:
	bool execute(player::Player & player, const DevelopmentData & data) const;

private:
   virtual bool validData(const DevelopmentData & data) const = 0;
   virtual bool apply(player::Player & player, const DevelopmentData & data) const = 0;
};

const std::unordered_map<card::RessourceType,int> & getDevelopmentCost();

class KnightDevelopment : public Development
{
public:
   explicit KnightDevelopment(std::vector<player::Player> & players, board::Board & board);
private:
   std::vector<player::Player> & m_players;
   board::Board & m_board;

   bool validData(const DevelopmentData & data) const override;
   bool apply(player::Player & player, const DevelopmentData & data) const override;
};

class FreeRessourcesDevelopment : public Development
{
private:
   bool validData(const DevelopmentData & data) const override;
   bool apply(player::Player & player, const DevelopmentData & data) const override;
};

class BuildTwoFreeRoads : public Development
{
public:
   explicit BuildTwoFreeRoads(board::Board & board);

private:
   board::Board & m_board;

   bool validData(const DevelopmentData & data) const override;
   bool apply(player::Player & player, const DevelopmentData & data) const override;
};

class MonopolyDevelopment : public Development
{
public:
   explicit MonopolyDevelopment(std::vector<player::Player> & players);

private:
   std::vector<player::Player> & m_players;

   bool validData(const DevelopmentData & data) const override;
   bool apply(player::Player & player, const DevelopmentData & data) const override;
};

class VictoryPointDevelopment : public Development
{
private:
   bool validData(const DevelopmentData & data) const override;
   bool apply(player::Player & player, const DevelopmentData & data) const override;
};

} // namespace card

#endif // !CARD_DEVELOPMENT_HPP