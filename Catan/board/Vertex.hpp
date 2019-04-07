#ifndef BOARD_VERTEX_HPP
#define BOARD_VERTEX_HPP

#include <board/Vertex.fwd.hpp>

#include <board/Edge.fwd.hpp>
#include <board/Harbor.hpp>

#include <token/Building.fwd.hpp>

#include <utility/Serializable.hpp>

#include <vector>
#include <optional>

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

	bool hasAdjencentBuilding() const;
	bool hasAtLeastOneAdjecentRoad(int playerReference) const;
	bool hasBuildingOfPlayer(int playerReference) const;

	std::vector<EdgeCRef> getOtherEdges(const Edge & edge) const;

	std::string serialize() const override;

private:
	int m_id;
	std::vector<EdgeRef> m_edges;

	std::optional<token::building::Building*> m_building;

	std::optional<Harbor> m_harbor;
};

} // namespace board

#endif // !BOARD_VERTEX_HPP
