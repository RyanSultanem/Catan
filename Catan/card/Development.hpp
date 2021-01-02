#ifndef CARD_DEVELOPMENT_HPP
#define CARD_DEVELOPMENT_HPP

#include <Achievement.fwd.hpp>
#include <board/Board.fwd.hpp>

#include <card/Card.hpp>
#include <card/Development.fwd.hpp>

#include <player/Player.fwd.hpp>
#include <Players.fwd.hpp>

#include <utility/NumberGenerator.fwd.hpp>
#include <utility/Serializable.hpp>

#include <optional>
#include <unordered_map>

namespace card {

enum class DevelopmentType // TODO: Check if can have a better solution
{
   Knight,
   FreeRessources,
   BuildTwoFreeRoads,
   Monopoly,
   VictoryPoint
};

class Development : public serialize::Serializable // should be a Token?
{
public:
	explicit Development(const DevelopmentAction & devAction);

	bool isUsed() const;
	card::DevelopmentType getType() const;

	bool executeAction(player::Player & player, const DevelopmentData & data);

	std::string serialize() const override;

private:
	const DevelopmentAction & m_developmentAction;
	bool m_used;
};

class DevelopmentData // TODO: Change to use Variant
{
public:
	DevelopmentData();

	void setDevelopmentType(DevelopmentType developmentType);
	void setCellVertexPosition(const std::pair<int,int> & cellVertexPosition);
	void setMonopolyRessource(Ressource monopolyRessource);
	void setFreeRessources(const std::pair<Ressource,Ressource> & freeRessources);
	void setRoadPosition(const std::pair<int,int> & roadPositions);

	DevelopmentType getDevelopmentType() const;
	std::optional<std::pair<int,int>> getCellVertexPosition() const;
	std::optional<Ressource> getMonopolyRessource() const;
	std::optional<std::pair<Ressource, Ressource>> getFreeRessources() const;
	std::optional<std::pair<int,int>> getRoadPositions() const;

private:
	DevelopmentType m_developmentType;
	std::optional<std::pair<int,int>> m_cellVertexPosition;
	std::optional<Ressource> m_monopolyRessource;
	std::optional<std::pair<Ressource,Ressource>> m_freeRessources;
	std::optional<std::pair<int,int>> m_roadPositions;
};

class DevelopmentAction 
{
public:
	bool execute(player::Player & player, const DevelopmentData & data) const;
	
	virtual DevelopmentType getType() const = 0;

private:
	virtual bool validData(const DevelopmentData & data) const = 0;
	virtual bool apply(player::Player & player, const DevelopmentData & data) const = 0;
};

const std::unordered_map<card::Ressource,int> & getDevelopmentCost();

class KnightAction : public DevelopmentAction
{
public:
	explicit KnightAction(Players & players, board::Board & board, Achievement & strongestArmy, const NumberGenerator & numberGenerator);

	DevelopmentType getType() const override;

private:
	Players & m_players;
	board::Board & m_board;
	Achievement & m_strongestArmy;
	const NumberGenerator & m_numberGenerator;

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
	explicit BuildTwoFreeRoadsAction(board::Board & board, Achievement & longestRoad);

	DevelopmentType getType() const override;

private:
	board::Board & m_board;
	Achievement & m_longestRoad;

	bool validData(const DevelopmentData & data) const override;
	bool apply(player::Player & player, const DevelopmentData & data) const override;

	bool applyOnCopiesCheck(player::Player & player, const DevelopmentData & data) const;
};

class MonopolyAction : public DevelopmentAction
{
public:
	explicit MonopolyAction(Players & players);

	DevelopmentType getType() const override;

private:
	Players & m_players;

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