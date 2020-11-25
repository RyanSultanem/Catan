#include <token/Conditions.hpp>

#include <board/Vertex.hpp>
#include <board/Edge.hpp>

#include <token/Building.hpp>


PlaceSettlementCondition::PlaceSettlementCondition(int playerReference)
	: m_playerReference(playerReference)
{
}

bool PlaceSettlementCondition::checkCondition(const board::Vertex & vertex) const
{
	return !vertex.getBuilding() && !vertex.hasAdjencentBuilding() && vertex.hasAtLeastOneAdjecentRoad(m_playerReference);
}

PlaceInitialSettlementCondition::PlaceInitialSettlementCondition(int playerReference, int initialRoadPosition)
	: PlaceSettlementCondition(playerReference)
	, m_initialRoadPosition(initialRoadPosition)
{
}

bool PlaceInitialSettlementCondition::checkCondition(const board::Vertex & vertex) const
{
	return !vertex.getBuilding() && !vertex.hasAdjencentBuilding() && vertex.hasEdge(m_initialRoadPosition);
}

PlaceRoadCondition::PlaceRoadCondition(int playerReference)
	: m_playerReference(playerReference)
{
}

bool PlaceRoadCondition::checkCondition(const board::Edge & edge) const
{
	return !edge.getRoad() && (edge.hasPlayerNeighboorRoad(m_playerReference) || edge.hasPlayerNeighboorBuilding(m_playerReference));
}

PlaceInitialRoadCondition::PlaceInitialRoadCondition(int playerReference, int initialSettlementPosition)
	: PlaceRoadCondition(playerReference)
	, m_initialSettlementPosition(initialSettlementPosition)
{
}

bool PlaceInitialRoadCondition::checkCondition(const board::Edge & edge) const
{
	return !edge.getRoad() && edge.hasVertex(m_initialSettlementPosition);
}

PlaceCityCondition::PlaceCityCondition(int playerId)
	: m_playerId(playerId)
{
}

bool PlaceCityCondition::checkCondition(const board::Vertex & vertex) const
{
	std::optional<token::building::Building *> optBuilding = vertex.getBuilding();
	if (optBuilding)
	{
		token::building::Settlement * settlement = dynamic_cast<token::building::Settlement *>(*optBuilding);

		if (settlement && settlement->getPlayerId() == m_playerId)
		{
			return true;
		}
	}

	return false;
}

