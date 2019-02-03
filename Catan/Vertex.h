#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <optional>
#include "Vertex.fwd.h"
#include "Edge.fwd.h"
#include "Building.fwd.hpp"
#include "Serializable.hpp"

namespace board {

class Vertex : public serialize::Serializable
{
public:
	Vertex(int id = 0);
	void addEdge(Edge& edge);
	int getId() const;
	bool hasEdge(int id) const;
	void setBuilding(token::building::Building& building);
	const std::optional<token::building::Building*>& getBuilding() const;

   std::string serialize() const override;

   bool hasAdjencentBuilding() const;
   bool hasAtLeastOneAdjecentRoad(int playerReference) const;
   std::vector<EdgeCRef> getOtherEdges(const Edge & edge) const;

private:
	std::vector<EdgeRef> m_edges;
	int m_id;
	std::optional<token::building::Building*> m_building;
};

} // namespace board

#endif // !VERTEX_H
