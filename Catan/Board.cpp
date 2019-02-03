#include "Board.h"
#include "Card.h"
#include "Land.h"
#include "Building.hpp"
#include "Conditions.hpp"
#include <algorithm>
#include <numeric>

namespace board {

Board::Board()
{
	createVertices();
	createEdges();
	initializeCells();
}

void Board::createVertices()
{
	m_vertices.reserve(54);

	for (int i = 0; i < 54; ++i)
		m_vertices.push_back(Vertex(m_vertexId++));
}

void Board::createEdges()
{
	m_edges.reserve(72);

	connectVerticesWithTwoEdges(0, 3, 1);
	connectVerticesWithSingleEdge(3, 4, 1);

	connectVerticesWithTwoEdges(7, 4, 1);
	connectVerticesWithSingleEdge(11, 5, 1);

	connectVerticesWithTwoEdges(16, 5, 1);
	connectVerticesWithSingleEdge(21, 6, 1);

	connectVerticesWithTwoEdges(33, 5, -1);
	connectVerticesWithSingleEdge(38, 5, -1);

	connectVerticesWithTwoEdges(43, 4, -1);
	connectVerticesWithSingleEdge(47, 4, -1);

	connectVerticesWithTwoEdges(51, 3, -1);
}

void Board::connectVerticesWithTwoEdges(int start, int size, int direction)
{
	for (int i = start; i < start + size; ++i)
	{
		Edge edge1(m_vertices[i], m_vertices[i + direction*(size + 1 * (direction - 1) / -2)], m_edgeId++);
		m_edges.push_back(edge1);
		m_vertices[i].addEdge(m_edges.back());
		m_vertices[i + direction*(size + 1 * (direction - 1) / -2)].addEdge(m_edges.back());

		Edge edge2(m_vertices[i], m_vertices[i + direction*(size + 1 * (direction + 1) / 2)], m_edgeId++);
		m_edges.push_back(edge2);
		m_vertices[i].addEdge(m_edges.back());
		m_vertices[i + direction*(size + 1 * (direction + 1) / 2)].addEdge(m_edges.back());
	}
}

void Board::connectVerticesWithSingleEdge(int start, int size, int direction)
{
	for (int i = start; i < start + size; ++i)
	{
		Edge edge(m_vertices[i], m_vertices[i + direction*size], m_edgeId++);
		m_edges.push_back(edge);
		m_vertices[i].addEdge(m_edges.back());
		m_vertices[i + direction*size].addEdge(m_edges.back());
	}
}

void Board::initializeCells()
{
	m_cells.reserve(19);

	m_cells.push_back(cell::Cell(card::RessourceType::ORE, 5, createVerticesVector({ 21,16,22,28,33,27 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::GRAIN, 11, createVerticesVector({ 11,7,12,17,22,16 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::ORE, 8, createVerticesVector({ 3,0,4,8,12,7 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::LUMBER, 4, createVerticesVector({ 4,1,5,9,13,8 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::BRICK, 6, createVerticesVector({ 5,2,6,10,14,9 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::WOOL, 10, createVerticesVector({ 14,10,15,20,25,19 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::GRAIN, 9, createVerticesVector({ 25,20,26,32,37,31 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::WOOL, 6, createVerticesVector({ 36,31,37,42,46,41 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::LUMBER, 5, createVerticesVector({ 45,41,46,50,53,49 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::BRICK, 4, createVerticesVector({ 44,40,45,49,52,48 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::WOOL, 10, createVerticesVector({ 43,39,44,48,51,47 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::GRAIN, 3, createVerticesVector({ 33,28,34,39,43,38 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::WOOL, 9, createVerticesVector({ 22,17,23,29,34,28 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::LUMBER, 3, createVerticesVector({ 12,8,13,18,23,17 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::ORE, 12, createVerticesVector({ 13,9,14,19,24,18 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::GRAIN, 11, createVerticesVector({ 24,19,25,31,36,30 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::BRICK, 3, createVerticesVector({ 35,30,36,41,45,40 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::LUMBER, 8, createVerticesVector({ 34,29,35,40,44,39 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::NO_RESSOURCE, 0, createVerticesVector({ 23,18,24,30,35,30 }), m_cellId++));
}

std::vector<VertexRef> Board::createVerticesVector(const std::vector<int>& indices)
{
	std::vector<VertexRef> cellVertices;
   cellVertices.reserve(indices.size());

   //std::transform(indices.begin(), indices.end(), std::back_inserter(cellVertices), [&](int i) {
   //   return m_vertices[i];
   //});

	std::for_each(begin(indices), end(indices), [&](int i) {
		cellVertices.push_back(m_vertices[i]);
	});

	return cellVertices;
}

int Board::getCellsSize() const
{
	return m_cells.size();
}

int Board::getNumberOfLand(const card::RessourceType& ressource) const
{
	return std::count_if(m_cells.begin(), m_cells.end(), [&](const cell::Cell& p) {
		return p.getLand().getRessourceType() == ressource;
	});
}

int Board::getNumberOfVertices() const
{
	return m_vertices.size();
}

int Board::getNumberOfEdges() const
{
	return m_edges.size();
}

int Board::getCellId(int index) const
{
	return m_cells[index].getId();
}

bool Board::isConnectedEdges(std::vector<int> edgeIds) const
{
	for (auto id : edgeIds)
	{
		if (id >= m_edges.size() || id < 0)
		{
			return false;
		}
	}
	int sizeM1 = edgeIds.size() - 1;
	for (int i = 0; i < sizeM1; ++i)
	{
		if (!m_edges[edgeIds[i]].hasNeighboorEdge(edgeIds[i + 1]))
		{
			return false;
		}
	}

	return true;
}

bool Board::placeSettlement(int position, token::building::Settlement & settlement, const PlaceSettlementCondition & condition)
{
   if (position < 0 || position >= m_vertices.size())
      return false;

   Vertex & vertex = m_vertices.at(position);
   
   if(condition.checkCondition(vertex))
   {
      vertex.setBuilding(settlement);
      return true;
   }

   return false;
}

bool Board::placeRoad(int position, token::Road & road, const PlaceRoadCondition & condition)
{
	if (position < 0 || position >= m_edges.size())
		return false;

	Edge & edge = m_edges.at(position);

	if(condition.checkCondition(edge))
	{
		edge.setRoad(road);
		return true;
	}

	return false;
}

bool Board::placeCity(int position, token::building::City & city, const PlaceCityCondition & condition)
{
	if (position < 0 || position >= m_vertices.size())
		return false;

	Vertex & vertex = m_vertices.at(position);

	if(condition.checkConditon(vertex))
	{
		vertex.setBuilding(city);
		return true;
	}

	return false;
}

std::vector<card::RessourceType> Board::getRessourcesFromVertexPosition(int position)
{
	std::vector<card::RessourceType> ressources;
	ressources.reserve(3);

	std::for_each(m_cells.begin(), m_cells.end(),
	[&](const cell::Cell & cell)
	{
		if(cell.hasVertex(position))
		{
			card::RessourceType ressourceType = cell.produceLandRessource().getType();
			
			if(ressourceType != card::RessourceType::NO_RESSOURCE)
				return ressources.push_back(ressourceType);
		}
	});

	return ressources;
}


std::string Board::serialize() const
{
   std::string board;

   board = serialize::containerSerialize(m_cells, board, "Cells: ");
   board = serialize::containerSerialize(m_vertices, board, "Vertices: ");
   board = serialize::containerSerialize(m_edges, board, "Edges: ");

   return board;
}

std::vector<cell::CellCRef> Board::getCellsWithNumber(int value) const
{
	std::vector<cell::CellCRef> cellsWithNumber;
	cellsWithNumber.reserve(2);

	// TODO: check if can be replaced by copy_if
	std::for_each(m_cells.begin(), m_cells.end(), 
		[&cellsWithNumber, &value](const cell::Cell & cell)
	{
		if(cell.getNumber() == value)
		{
			cellsWithNumber.push_back(cell);
		}
	});

	return cellsWithNumber;
}

} // namespace board