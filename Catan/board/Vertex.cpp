#include <board/Vertex.hpp>

#include <board/Edge.hpp>

#include <token/Building.hpp>

#include <algorithm>

namespace board {

Vertex::Vertex(int id) 
	: m_id(id)
	, m_building(std::nullopt)
{
}

void Vertex::addEdge(Edge& edge)
{
	m_edges.push_back(edge);
}

int Vertex::getId() const
{
	return m_id;
}

bool Vertex::hasEdge(int id) const
{
	return std::find_if(m_edges.begin(), m_edges.end(), [&](const Edge& edge)
	{
		return edge.getId() == id;

	}) != m_edges.end();
}

void Vertex::setBuilding(token::building::Building& building)
{
	m_building = &building;
}

const std::optional<token::building::Building*>& Vertex::getBuilding() const
{
	return m_building;
}

void Vertex::setHarbor(const Harbor& harbor)
{
	m_harbor = harbor;
}

const std::optional<Harbor>& Vertex::getHarbor() const
{
	return m_harbor;
}

std::string Vertex::serialize() const
{
   return std::to_string(m_id) + ',' +
      (m_building ? m_building.value()->serialize() : "N");
}

bool Vertex::hasAdjencentBuilding() const
{
	return std::find_if(m_edges.begin(), m_edges.end(),
		[this](const Edge & edge)
	{
		std::optional<VertexCRef> otherVertex = edge.getOtherVertex(*this);
		if (otherVertex)
			return otherVertex->get().getBuilding() != std::nullopt;
		
		return false;

	}) != m_edges.end();
}

bool Vertex::hasAtLeastOneAdjecentRoad(int playerReference) const
{
	return std::find_if(m_edges.begin(), m_edges.end(),
		[playerReference](const Edge & edge)
	{
		return edge.hasRoadOfPlayer(playerReference);

	}) != m_edges.end();
}

bool Vertex::hasBuildingOfPlayer(int playerReference) const
{
	return m_building.has_value() && m_building.value()->getReference() == playerReference;
}

std::vector<EdgeCRef> Vertex::getOtherEdges(const Edge & givenEdge) const
{	
	std::vector<EdgeCRef> edges;

	int edgeId = givenEdge.getId();

	if (!hasEdge(edgeId))
		return edges;

	edges.reserve(2);
	std::for_each(m_edges.begin(), m_edges.end(),
		[&edges, edgeId](const Edge & edge)
	{
		if (edge.getId() != edgeId)
			edges.push_back(edge);
	});

	return edges;
}

} // namespace board