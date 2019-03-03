#ifndef CARD_DEVELOPMENT_HPP
#define CARD_DEVELOPMENT_HPP

#include "Card.h"
#include "Player.fwd.hpp"
#include "Board.fwd.h"
#include "Development.fwd.hpp"
#include <unordered_map>

#include <optional>

namespace card {

enum DevelopmentType // TODO: Check if can have a better solution
{
   Knight,
   FreeRessources,
   BuildTwoFreeRoads,
   Monopoly,
   VictoryPoint
};

class Development
{
public:
	explicit Development(const DevelopmentAction & action);

	bool isUsed() const;

	bool executeAction(player::Player & player, const DevelopmentData & data);
	card::DevelopmentType getType();

private:
	const DevelopmentAction & m_action;
	bool m_used;
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

class DevelopmentAction // should be a Token?
{
public:
	bool execute(player::Player & player, const DevelopmentData & data) const;
	
	virtual DevelopmentType getType() const = 0;

private:
	virtual bool validData(const DevelopmentData & data) const = 0;
	virtual bool apply(player::Player & player, const DevelopmentData & data) const = 0;
};

const std::unordered_map<card::RessourceType,int> & getDevelopmentCost();

class KnightAction : public DevelopmentAction
{
public:
	explicit KnightAction(std::vector<player::Player> & players, board::Board & board);

	DevelopmentType getType() const override;

private:
	std::vector<player::Player> & m_players;
	board::Board & m_board;

	bool validData(const DevelopmentData & data) const override;
	bool apply(player::Player & player, const DevelopmentData & data) const override;
};

class FreeRessourcesAction : public DevelopmentAction
{
public:
	DevelopmentType getType() const override;

private:
	bool validData(const DevelopmentData & data) const override;
	bool apply(player::Player & player, const DevelopmentData & data) const override;
};

class BuildTwoFreeRoadsAction : public DevelopmentAction
{
public:
	explicit BuildTwoFreeRoadsAction(board::Board & board);

	DevelopmentType getType() const override;

private:
	board::Board & m_board;

	bool validData(const DevelopmentData & data) const override;
	bool apply(player::Player & player, const DevelopmentData & data) const override;
};

class MonopolyAction : public DevelopmentAction
{
public:
	explicit MonopolyAction(std::vector<player::Player> & players);

	DevelopmentType getType() const override;

private:
	std::vector<player::Player> & m_players;

	bool validData(const DevelopmentData & data) const override;
	bool apply(player::Player & player, const DevelopmentData & data) const override;
};

class VictoryPointAction : public DevelopmentAction
{
public:
	DevelopmentType getType() const override;

private:
	bool validData(const DevelopmentData & data) const override;
	bool apply(player::Player & player, const DevelopmentData & data) const override;
};

} // namespace card

#endif // !CARD_DEVELOPMENT_HPP