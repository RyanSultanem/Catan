#include <board/Cell.hpp>
#include <board/Vertex.hpp>

#include <card/Card.hpp>

namespace board {

Cell::Cell(const card::Ressource& ressource, unsigned int number, const std::vector<board::VertexRef>& vertices, int id)
	: m_id(id)
	, m_ressource(ressource)
	, m_number(number)
	, m_vertices(vertices)
{
}

card::Ressource Cell::getRessource() const
{
	return m_ressource;
}

int Cell::getId() const
{
	return m_id;
}

bool Cell::hasVertex(int vertexId) const
{
	return std::find_if(m_vertices.begin(), m_vertices.end(),
		[&vertexId](const board::VertexRef & vertex)
	{
		return vertex.get().getId() == vertexId;
	}) != m_vertices.end();
}

std::string Cell::serialize() const
{
	return std::to_string(m_id) + ',' +
		std::to_string(m_number) + ',' +
		std::to_string(static_cast<int>(m_ressource));
}

int Cell::getNumber() const
{
	return m_number;
}

void Cell::setNumber(int number)
{
	m_number = number;
}

const std::vector<token::building::Building*> Cell::getActiveBuildings() const
{
	std::vector<token::building::Building*> activeBuildings;
	activeBuildings.reserve(3);

	// TODO: check if can be replaced by copy_if: actually should be copy_if then transofrm.. this might be clearer
	std::for_each(m_vertices.begin(), m_vertices.end(), 
		[&activeBuildings](const board::Vertex & vertex)
	{
		std::optional<token::building::Building*> buildingOpt = vertex.getBuilding();

		if(buildingOpt)
		{
			activeBuildings.push_back(buildingOpt.value());
		}
	});

	return activeBuildings;
}

} // namespace board 

