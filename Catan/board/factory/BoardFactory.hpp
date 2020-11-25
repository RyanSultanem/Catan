#ifndef BOARD_FACTORY_BOARD_HPP
#define BOARD_FACTORY_BOARD_HPP

#include <board/Board.hpp>

namespace board {

// TODO: Randomize the board. Add a new function

class BoardFactory
{
public:
	BoardFactory();

	Board generateBoard();
	Board generateBoardCopy(const Board & other);

private:
	int m_cellId = 0;
	std::vector<Cell> m_cells;

	int m_vertexId = 0;
	std::vector<Vertex> m_vertices;

	int m_edgeId = 0;
	std::vector<Edge> m_edges;

	token::Robber m_robber;
	
	void prepareBoardComponents();
	void resetBoardComponents();

	void createVertices();
	void setHarbors();
	void createEdges();
	void connectVerticesWithTwoEdges(int start, int size, int direction);
	void connectVerticesWithSingleEdge(int start, int size, int direction);
	void initializeCells();
	void initializeRobber();
	std::vector<VertexRef> createVerticesVector(const std::vector<int>& indices);
};



} // namespce board

#endif // !BOARD_FACTORY_BOARD_HPP