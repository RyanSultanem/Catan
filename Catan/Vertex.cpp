#include <algorithm>

#include "Vertex.h"
#include "Edge.h"

namespace board {

Vertex::Vertex(int id) : m_id(id)
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

} // namespace board