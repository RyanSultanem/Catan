#include <board/factory/BoardFactory.hpp>

#include <board/Cell.hpp>

namespace board {

board::BoardFactory::BoardFactory()
{
}

Board board::BoardFactory::generateBoard()
{
	resetBoardComponents();

	createVertices();
	createEdges();
	initializeCells();
	initializeRobber();

	return Board(std::move(m_cells), std::move(m_vertices), std::move(m_edges), m_robber);
}

void BoardFactory::resetBoardComponents()
{
	m_cellId = 0;
	m_cells.clear();

	m_vertexId = 0;
	m_vertices.clear();

	m_edgeId = 0;
	m_edges.clear();
}

void BoardFactory::createVertices()
{
	m_vertices.reserve(54);

	for (int i = 0; i < 54; ++i)
		m_vertices.push_back(Vertex(m_vertexId++));

	setHarbors();
}

void BoardFactory::setHarbors()
{
	m_vertices.at(0).setHarbor(Harbor(card::RessourceType::NO_RESSOURCE));
	m_vertices.at(3).setHarbor(Harbor(card::RessourceType::NO_RESSOURCE));

	m_vertices.at(1).setHarbor(Harbor(card::RessourceType::GRAIN));
	m_vertices.at(5).setHarbor(Harbor(card::RessourceType::GRAIN));

	m_vertices.at(10).setHarbor(Harbor(card::RessourceType::ORE));
	m_vertices.at(15).setHarbor(Harbor(card::RessourceType::ORE));

	m_vertices.at(26).setHarbor(Harbor(card::RessourceType::NO_RESSOURCE));
	m_vertices.at(32).setHarbor(Harbor(card::RessourceType::NO_RESSOURCE));

	m_vertices.at(42).setHarbor(Harbor(card::RessourceType::WOOL));
	m_vertices.at(46).setHarbor(Harbor(card::RessourceType::WOOL));

	m_vertices.at(49).setHarbor(Harbor(card::RessourceType::NO_RESSOURCE));
	m_vertices.at(52).setHarbor(Harbor(card::RessourceType::NO_RESSOURCE));

	m_vertices.at(51).setHarbor(Harbor(card::RessourceType::NO_RESSOURCE));
	m_vertices.at(47).setHarbor(Harbor(card::RessourceType::NO_RESSOURCE));

	m_vertices.at(33).setHarbor(Harbor(card::RessourceType::BRICK));
	m_vertices.at(38).setHarbor(Harbor(card::RessourceType::BRICK));

	m_vertices.at(11).setHarbor(Harbor(card::RessourceType::LUMBER));
	m_vertices.at(16).setHarbor(Harbor(card::RessourceType::LUMBER));
}

void BoardFactory::createEdges()
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

void BoardFactory::connectVerticesWithTwoEdges(int start, int size, int direction)
{
	for (int i = start; i < start + size; ++i)
	{
		Edge edge1(m_vertices[i], m_vertices[i + direction * (size + 1 * (direction - 1) / -2)], m_edgeId++);
		m_edges.push_back(edge1);
		m_vertices[i].addEdge(m_edges.back());
		m_vertices[i + direction * (size + 1 * (direction - 1) / -2)].addEdge(m_edges.back());

		Edge edge2(m_vertices[i], m_vertices[i + direction * (size + 1 * (direction + 1) / 2)], m_edgeId++);
		m_edges.push_back(edge2);
		m_vertices[i].addEdge(m_edges.back());
		m_vertices[i + direction * (size + 1 * (direction + 1) / 2)].addEdge(m_edges.back());
	}
}

void BoardFactory::connectVerticesWithSingleEdge(int start, int size, int direction)
{
	for (int i = start; i < start + size; ++i)
	{
		Edge edge(m_vertices[i], m_vertices[i + direction * size], m_edgeId++);
		m_edges.push_back(edge);
		m_vertices[i].addEdge(m_edges.back());
		m_vertices[i + direction * size].addEdge(m_edges.back());
	}
}

void BoardFactory::initializeCells()
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
	m_cells.push_back(cell::Cell(card::RessourceType::GRAIN, 2, createVerticesVector({ 33,28,34,39,43,38 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::WOOL, 9, createVerticesVector({ 22,17,23,29,34,28 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::LUMBER, 3, createVerticesVector({ 12,8,13,18,23,17 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::ORE, 12, createVerticesVector({ 13,9,14,19,24,18 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::GRAIN, 11, createVerticesVector({ 24,19,25,31,36,30 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::BRICK, 3, createVerticesVector({ 35,30,36,41,45,40 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::LUMBER, 8, createVerticesVector({ 34,29,35,40,44,39 }), m_cellId++));
	m_cells.push_back(cell::Cell(card::RessourceType::NO_RESSOURCE, 0, createVerticesVector({ 23,18,24,30,35,30 }), m_cellId++));
}

void BoardFactory::initializeRobber()
{
	auto desertIt = std::find_if(m_cells.begin(), m_cells.end(),
		[](const cell::Cell & cell)
		{
			return (cell.getLand().getRessourceType() == card::RessourceType::NO_RESSOURCE);
		});

	if (desertIt != m_cells.end())
		m_robber.initializeRobberCell(*desertIt);
}

std::vector<VertexRef> BoardFactory::createVerticesVector(const std::vector<int>& indices)
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

} // namespace board

