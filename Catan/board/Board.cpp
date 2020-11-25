#include <board/Board.hpp>

#include <card/Card.hpp>

#include <token/Building.hpp>
#include <token/Conditions.hpp>

#include <utility/Utility.hpp>

#include <algorithm>
#include <numeric>

namespace board {

Board::Board()
{
}

Board::Board(std::vector<Cell> && cells, std::vector<Vertex> && vertices, std::vector<Edge> && edges, const token::Robber & robber)
	: m_cells(std::move(cells))
	, m_vertices(std::move(vertices))
	, m_edges(std::move(edges))
	, m_robber(robber)
{
}

Board Board::copyBoard(std::vector<Cell>&& cells, std::vector<Vertex>&& vertices, std::vector<Edge>&& edges, token::Robber & robber) const
{
	for (int i = 0; i < vertices.size(); ++i)
	{
		if (m_vertices[i].getBuilding())
			vertices[i].setBuilding(*m_vertices[i].getBuilding().value());
	}

	for (int i = 0; i < edges.size(); ++i)
	{
		if (m_edges[i].getRoad())
			edges[i].setRoad(*m_edges[i].getRoad().value());
	}

	robber = m_robber; // TODO: wrong to do a simple copy. to check how to use moveRobber for correctness

	return Board(std::move(cells), std::move(vertices), std::move(edges), robber);
}

int Board::getNumberOfCells() const
{
	return m_cells.size();
}

int Board::getNumberOfLand(const card::Ressource& ressource) const
{
	return std::count_if(m_cells.begin(), m_cells.end(), [&](const Cell& cell) {
		return cell.getRessource() == ressource;
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

bool Board::isConnectedEdges(const std::vector<int> & edgeIds) const
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

bool Board::checkAdjacent(int cellPosition, int vertexPosition) const
{
	if(!utility::isValidPosition(m_cells, cellPosition) || !utility::isValidPosition(m_vertices, vertexPosition))
		return false;

	 const Cell & cell = m_cells[cellPosition];
	 const Vertex & vertex = m_vertices[vertexPosition];

	 return cell.hasVertex(vertex.getId());
}

bool Board::placeSettlement(int position, token::building::Settlement & settlement, const PlaceSettlementCondition & condition)
{
   if (!utility::isValidPosition(m_vertices, position))
      return false;

   Vertex & vertex = m_vertices[position];
   
   if(condition.checkCondition(vertex))
   {
      vertex.setBuilding(settlement);
      return true;
   }

   return false;
}

bool Board::placeRoad(int position, token::Road & road, const PlaceRoadCondition & condition)
{
	if (!utility::isValidPosition(m_edges, position))
		return false;

	Edge & edge = m_edges[position];

	if(condition.checkCondition(edge))
	{
		edge.setRoad(road);
		return true;
	}

	return false;
}

bool Board::placeCity(int position, token::building::City & city, const PlaceCityCondition & condition)
{
	if (!utility::isValidPosition(m_vertices, position))
		return false;

	Vertex & vertex = m_vertices[position];

	if(condition.checkCondition(vertex))
	{
		vertex.setBuilding(city);
		return true;
	}

	return false;
}

bool Board::moveRobber(int position)
{
	if (!utility::isValidPosition(m_cells, position))
		return false;

	m_robber.applyTo(m_cells[position]);
	return true;
}

const std::optional<CellRef>& Board::getRobbedCell() const
{
	return m_robber.getCell();
}

std::vector<card::Ressource> Board::getRessourcesFromVertexPosition(int position) const
{
	std::vector<card::Ressource> ressources;
	ressources.reserve(3);

	std::for_each(m_cells.begin(), m_cells.end(),
	[&](const Cell & cell)
	{
		if(cell.hasVertex(position))
		{
			card::Ressource ressourceType = cell.getRessource();
			
			if(ressourceType != card::Ressource::NO_RESSOURCE)
				return ressources.push_back(ressourceType);
		}
	});

	return ressources;
}

std::optional<HarborCRef> Board::getHarbor(int position) const
{
	if (!utility::isValidPosition(m_vertices, position))
		return std::nullopt;

	return m_vertices[position].getHarbor();
}

std::string Board::serialize() const
{
   std::string board("Board: \n");

   board = serialize::containerSerialize(m_cells, board, "Cells: ");
   board = serialize::containerSerialize(m_vertices, board, "Vertices: ");
   board = serialize::containerSerialize(m_edges, board, "Edges: ");

   return board;
}

std::vector<CellCRef> Board::getCellsWithNumber(int value) const
{
	std::vector<CellCRef> cellsWithNumber;
	cellsWithNumber.reserve(2);

	// TODO: check if can be replaced by copy_if
	std::for_each(m_cells.begin(), m_cells.end(), 
		[&cellsWithNumber, &value](const Cell & cell)
	{
		if(cell.getNumber() == value)
		{
			cellsWithNumber.push_back(cell);
		}
	});

	return cellsWithNumber;
}

std::optional<EdgeCRef> Board::getEdgeAt(int position) const
{
	if(!utility::isValidPosition(m_edges, position))
		return std::nullopt;

	return m_edges[position];
}

std::optional<int> Board::getVertexPlayerRef(int position) const
{
	if (!utility::isValidPosition(m_vertices, position))
		return std::nullopt;

	const std::optional<token::building::Building*> & building = m_vertices[position].getBuilding();
	
	if (building)
		return building.value()->getPlayerId();
	else
		return std::nullopt;
}

} // namespace board