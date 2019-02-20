#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <optional>
#include "Vertex.fwd.h"
#include "Building.fwd.hpp"
#include "Edge.fwd.h"
#include "Harbor.hpp"
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

	void setHarbor(const Harbor & harbor);
	const std::optional<Harbor> & getHarbor() const;

	std::string serialize() const override;

	bool hasAdjencentBuilding() const;
	bool hasAtLeastOneAdjecentRoad(int playerReference) const;
	std::vector<EdgeCRef> getOtherEdges(const Edge & edge) const;

private:
	int m_id;
	std::vector<EdgeRef> m_edges;

	std::optional<token::building::Building*> m_building;

	std::optional<Harbor> m_harbor;
};

} // namespace board

#endif // !VERTEX_H
