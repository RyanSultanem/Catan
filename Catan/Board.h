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

namespace board {

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
   bool placeSettlement(int position, token::building::Settlement & settlement);

   std::string serialize() const override;

private:
	int m_cellId = 0;
	std::vector<cell::Cell> m_cells;

	int m_vertexId = 0;
	std::vector<Vertex> m_vertices;

	int m_edgeId = 0;
	std::vector<Edge> m_edges;

	void createVertices();
	void createEdges();
	void connectVerticesWithTwoEdges(int start, int size, int direction);
	void connectVerticesWithSingleEdge(int start, int size, int direction);

	void initializeCells();
	std::vector<VertexRef> createVerticesVector(const std::vector<int>& indices);
};

} // namespace board

#endif // !BOARD_H
