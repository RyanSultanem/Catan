#include "Conditions.hpp"

#include "Building.hpp"
#include "Vertex.h"
#include "Edge.h"


PlaceSettlementCondition::PlaceSettlementCondition(int playerReference)
	: m_playerReference(playerReference)
{
}

bool PlaceSettlementCondition::checkCondition(const board::Vertex & vertex) const
{
	// TODO: should the checks definitions be moved in this file instad of Vertex/Edge?
	return !vertex.getBuilding() && !vertex.hasAdjencentBuilding() && vertex.hasAtLeastOneAdjecentRoad(m_playerReference);
}

PlaceInitialSettlementCondition::PlaceInitialSettlementCondition(int playerReference)
	: PlaceSettlementCondition(playerReference)
{
}

bool PlaceInitialSettlementCondition::checkCondition(const board::Vertex & vertex) const
{
	return !vertex.getBuilding() && !vertex.hasAdjencentBuilding();
}

PlaceRoadCondition::PlaceRoadCondition(int playerReference)
	: m_playerReference(playerReference)
{
}

bool PlaceRoadCondition::checkCondition(const board::Edge & edge) const
{
	return !edge.getRoad() && edge.hasPlayerNeighboorRoad(m_playerReference) || edge.hasPlayerNeighboorBuilding(m_playerReference);
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

PlaceCityCondition::PlaceCityCondition(int playerReference)
	: m_playerReference(playerReference)
{
}

bool PlaceCityCondition::checkCondition(const board::Vertex & vertex) const
{
	std::optional<token::building::Building *> optBuilding = vertex.getBuilding();
	if (optBuilding)
	{
		token::building::Settlement * settlement = dynamic_cast<token::building::Settlement *>(*optBuilding);

		if (settlement && settlement->getReference() == m_playerReference)
		{
			return true;
		}
	}

	return false;
}

