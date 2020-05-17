#include <board/Board.hpp>

#include <board/Land.hpp>

#include <card/Card.hpp>

#include <token/Building.hpp>
#include <token/Conditions.hpp>

#include <algorithm>
#include <numeric>

namespace board {

Board::Board()
{
}

Board::Board(std::vector<cell::Cell> && cells, std::vector<Vertex> && vertices, std::vector<Edge> && edges, const token::Robber & robber)
	: m_cells(std::move(cells))
	, m_vertices(std::move(vertices))
	, m_edges(std::move(edges))
	, m_robber(robber)
{
}

Board Board::copyBoard(std::vector<cell::Cell>&& cells, std::vector<Vertex>&& vertices, std::vector<Edge>&& edges, token::Robber & robber) const
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

	if(condition.checkCondition(vertex))
	{
		vertex.setBuilding(city);
		return true;
	}

	return false;
}

bool Board::moveRobber(int position)
{
	if (position < 0 || position >= m_cells.size())
		return false;

	m_robber.applyTo(m_cells.at(position));
	return true;
}

std::vector<card::RessourceType> Board::getRessourcesFromVertexPosition(int position) const
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

std::optional<HarborCRef> Board::getHarbor(int position) const
{
	if (position < 0 || position >= m_vertices.size())
		return std::nullopt;

	return m_vertices.at(position).getHarbor();
}

std::string Board::serialize() const
{
   std::string board("Board: \n");

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

std::optional<EdgeCRef> Board::getEdgeAt(int position) const
{
	if(position < 0 || position >= m_edges.size())
		return std::nullopt;

	return m_edges.at(position);
}

std::optional<int> Board::getVertexPlayerRef(int position) const
{
	if (position < 0 || position >= m_vertices.size())
		return std::nullopt;

	const std::optional<token::building::Building*> & building = m_vertices.at(position).getBuilding();
	
	//return building ? building.value()->getReference() : std::nullopt; // TODO: ???
	if (building)
		return building.value()->getReference();
	else
		return std::nullopt;
}

} // namespace board