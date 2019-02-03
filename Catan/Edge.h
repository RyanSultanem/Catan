#ifndef EDGE_H
#define EDGE_H

#include <memory>
#include <optional>

#include "Edge.fwd.h"
#include "Vertex.fwd.h"
#include "Road.fwd.hpp"
#include "Serializable.hpp"

namespace board {

class Edge : public serialize::Serializable
{
public:
	Edge(const Vertex& v1, const Vertex& v2, int id = 0);
	Edge(const Edge& edge);

	int getId() const;
	bool hasNeighboorEdge(int id) const;

	void setRoad(token::Road& road);
	const std::optional<token::Road*>& getRoad() const; // TODO: return RoadCRef ?

	bool hasPlayerNeighboorRoad(int playerReference) const;
	bool hasPlayerNeighboorBuilding(int playerReference) const;
	bool hasVertex(int vertexReference) const;
	std::optional<VertexCRef> getOtherVertex(const Vertex & vertex) const;

	std::string serialize() const override;

private:
	int m_id;
	const Vertex& m_v1;
	const Vertex& m_v2;

   std::optional<token::Road*> m_road;
};

} // namespace board

#endif // !EDGE_H
