#ifndef BOARD_BOARD_HPP
#define BOARD_BOARD_HPP

#include <board/Board.fwd.hpp>

#include <card/Card.fwd.hpp>

#include <board/Cell.hpp>
#include <board/Edge.hpp>
#include <board/Harbor.fwd.hpp>
#include <board/Land.fwd.hpp>
#include <board/Vertex.hpp>

#include <token/Conditions.fwd.hpp>
#include <token/Robber.hpp>

#include <utility/Serializable.hpp>

#include <vector>

namespace board {

// TODO: aray instead of vector for known length? Depends on if we want fixed sized of Boards To decide

class Board : public serialize::Serializable
{
public:
	Board();
	Board(std::vector<cell::Cell> && cells, std::vector<Vertex> && vertices, std::vector<Edge> && edges, const token::Robber & robber);
	int getNumberOfCells() const;
	int getNumberOfLand(const card::RessourceType& ressource) const;
	int getNumberOfVertices() const;
	int getNumberOfEdges() const;
	int getCellId(int index) const;

	bool isConnectedEdges(const std::vector<int> & edgeIds) const;
	
	bool placeSettlement(int position, token::building::Settlement & settlement, const PlaceSettlementCondition & condition);
	bool placeRoad(int position, token::Road & road, const PlaceRoadCondition & condition);
	bool placeCity(int position, token::building::City & city, const PlaceCityCondition & condition);

	bool moveRobber(int position);

	std::vector<card::RessourceType> getRessourcesFromVertexPosition(int position) const;
	// TODO: check if harbor check should really be done through Board, or can find a better solution; could be done through vertex if cost is not directly in player maybe, but in player also makes sense..
	std::optional<HarborCRef> getHarbor(int position) const;
	std::vector<cell::CellCRef> getCellsWithNumber(int value) const;
	std::optional<EdgeCRef> getEdgeAt(int position) const;
	std::optional<int> getVertexPlayerRef(int position) const;

	std::string serialize() const override;

private:
	std::vector<cell::Cell> m_cells;
	std::vector<Vertex> m_vertices;
	std::vector<Edge> m_edges;

	token::Robber m_robber;	
};

} // namespace board

#endif // !BOARD_BOARD_HPP
