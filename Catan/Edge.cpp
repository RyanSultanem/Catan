#include "Edge.h"
#include "Vertex.h"

namespace board {

Edge::Edge(const Vertex& v1, const Vertex& v2, int id)
	: m_id(id), m_v1(v1), m_v2(v2)
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
	m_road = road;
}

const std::optional<token::Road> & Edge::getRoad() const
{
	return m_road;
}

} // namespace board