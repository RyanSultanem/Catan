#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <optional>
#include "Vertex.fwd.h"
#include "Edge.fwd.h"
#include "Building.hpp"

namespace board {

class Vertex
{
public:
	Vertex(int id = 0);
	void addEdge(Edge& edge);
	int getId() const;
	bool hasEdge(int id) const;
	void setBuilding(token::building::Building& building);
	const std::optional<token::building::Building*>& getBuilding() const;

private:
	std::vector<EdgeRef> m_edges;
	int m_id;
	std::optional<token::building::Building*> m_building;
};

} // namespace board

#endif // !VERTEX_H
