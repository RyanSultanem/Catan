#include "Edge.h"
#include "Vertex.h"
#include "Road.hpp"

namespace board {

Edge::Edge(const Vertex& v1, const Vertex& v2, int id)
	: m_id(id)
	, m_v1(v1)
	, m_v2(v2)
	, m_road(std::nullopt)
{
}

Edge::Edge(const Edge& edge)
	: m_id(edge.m_id), m_v1(edge.m_v1), m_v2(edge.m_v2)
{
}

int Edge::getId() const
{
	return m_id;
}

bool Edge::hasNeighboorEdge(int id) const
{
	return m_v1.hasEdge(id) || m_v2.hasEdge(id);
}

void Edge::setRoad(token::Road & road)
{
	m_road = &road;
}

const std::optional<token::Road*> & Edge::getRoad() const
{
	return m_road;
}

bool edgesHasPlayerRoad(const std::vector<EdgeCRef> & edges, int playerReference, const Vertex & vertexBetweenRoads)
{
	if (vertexBetweenRoads.getBuilding() && !vertexBetweenRoads.hasBuildingOfPlayer(playerReference))
		return false;

	return std::find_if(edges.begin(), edges.end(),
		[playerReference, &vertexBetweenRoads](const Edge & otherEdge)
	{
		std::optional<token::Road*> optRoad = otherEdge.getRoad();
		if (optRoad && optRoad.value()->getReference() == playerReference)
			return true;

		return false;
	}) != edges.end();
}

bool Edge::hasPlayerNeighboorRoad(int playerReference) const
{
	std::vector<EdgeCRef> v1OtherEdges = m_v1.getOtherEdges(*this);
	if (edgesHasPlayerRoad(v1OtherEdges, playerReference, m_v1))
		return true;

	std::vector<EdgeCRef> v2OtherEdges = m_v2.getOtherEdges(*this);
	return edgesHasPlayerRoad(v2OtherEdges, playerReference, m_v2);
}

bool Edge::hasPlayerNeighboorBuilding(int playerReference) const
{
	return m_v1.hasBuildingOfPlayer(playerReference) || m_v2.hasBuildingOfPlayer(playerReference);
}

bool Edge::hasVertex(int vertexReference) const
{	
	return m_v1.getId() == vertexReference || m_v2.getId() == vertexReference;
}

bool Edge::hasRoadOfPlayer(int playerReference) const
{
	return m_road.has_value() && m_road.value()->getReference() == playerReference;
}

std::optional<VertexCRef> Edge::getOtherVertex(const Vertex& vertex) const
{
	int vertexId = vertex.getId();
	
	if (vertexId != m_v1.getId() && vertexId != m_v2.getId())
		return std::nullopt;

	if (vertexId == m_v1.getId())
		return m_v2;
	
	return m_v1;
}

std::vector<VertexCRef> Edge::getVertices() const
{
	return { m_v1, m_v2 };
}

std::string Edge::serialize() const
{
   return std::to_string(m_id) + ',' +
      (m_road ? m_road.value()->serialize(): "N");
}

} // namespace board