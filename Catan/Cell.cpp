#include "Cell.h"

cell::Cell::Cell(const card::RessourceType& ressource, unsigned int number, const std::vector<board::VertexRef>& vertices, int id)
	: m_id(id), m_land(ressource), m_number(number), m_vertices(vertices)
{
}

card::RessourceType cell::Cell::getLandRessourceType() const
{
	return m_land.getRessourceType();
}

int cell::Cell::getId() const
{
	return m_id;
}

std::string cell::Cell::serialize() const
{
   return std::to_string(m_id) + ',' + 
      std::to_string(m_number) + ',' + 
      std::to_string(static_cast<int>(m_land.getRessourceType()));
}
