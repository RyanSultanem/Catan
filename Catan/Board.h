#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "Board.fwd.h"
#include "Land.fwd.h"
#include "Card.fwd.h"
#include "Cell.h"
#include "Vertex.h"
#include "Edge.h"
#include "Serializable.hpp"
#include "Conditions.fwd.hpp"
#include "Harbor.fwd.hpp"
#include "Robber.hpp"

namespace board {

// TODO: Board to be constructed outside of the class
// TODO: Randomize the board.
// TODO: aray instead of vector for known length?

class Board : public serialize::Serializable
{
public:
	Board();
	int getCellsSize() const;
	int getNumberOfLand(const card::RessourceType& ressource) const;
	int getNumberOfVertices() const;
	int getNumberOfEdges() const;
	int getCellId(int index) const;

	bool isConnectedEdges(std::vector<int> edgeIds) const;
	
	bool placeSettlement(int position, token::building::Settlement & settlement, const PlaceSettlementCondition & condition);
	bool placeRoad(int position, token::Road & road, const PlaceRoadCondition & condition);
	bool placeCity(int position, token::building::City & city, const PlaceCityCondition & condition);

	bool moveRobber(int position);

	std::vector<card::RessourceType> getRessourcesFromVertexPosition(int position) const;
	// TODO: check if harbor check should really be done through Board, or can find a better solution; could be done through vertex if cost is not directly in player maybe, but in player also makes sense..
	std::optional<HarborCRef> getHarbor(int position) const;
	std::vector<cell::CellCRef> getCellsWithNumber(int value) const;
	std::optional<int> getVertexPlayerRef(int position) const;

	std::string serialize() const override;

private:
	int m_cellId = 0;
	std::vector<cell::Cell> m_cells;

	int m_vertexId = 0;
	std::vector<Vertex> m_vertices;

	int m_edgeId = 0;
	std::vector<Edge> m_edges;

	Robber m_robber;

	// Building the board
	void createVertices();
	void setHarbors();
	void createEdges();
	void connectVerticesWithTwoEdges(int start, int size, int direction);
	void connectVerticesWithSingleEdge(int start, int size, int direction);
	void initializeCells();
	void initializeRobber();
	std::vector<VertexRef> createVerticesVector(const std::vector<int>& indices);
};

} // namespace board

#endif // !BOARD_H
