#ifndef BOARD_EDGE_HPP
#define BOARD_EDGE_HPP

#include <memory>
#include <optional>

#include <board/Edge.fwd.hpp>
#include <board/Vertex.fwd.hpp>

#include <token/Road.fwd.hpp>

#include <utility/Serializable.hpp>

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
	bool hasRoadOfPlayer(int playerReference) const;
	std::optional<VertexCRef> getOtherVertex(const Vertex & vertex) const;
	std::vector<VertexCRef> getVertices() const;

	std::string serialize() const override;

private:
	int m_id;
	const Vertex & m_v1;
	const Vertex & m_v2;

   std::optional<token::Road*> m_road;
};

} // namespace board

#endif // !BOARD_EDGE_HPP
