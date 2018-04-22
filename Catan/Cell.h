#ifndef CELL_H
#define CELL_H

#include <vector>
#include <memory>

#include "Cell.fwd.h"
#include "Land.h"
#include "Vertex.fwd.h"

namespace cell {

class Cell
{
public:
	Cell(const card::RessourceType& ressource, unsigned int number, const std::vector<board::VertexRef>& vertices, int id = 0);
	card::RessourceType getLandRessourceType() const;
	int getId() const;

private:
	int m_id;
	land::Land m_land;
	unsigned int m_number;
	std::vector<board::VertexRef> m_vertices;
};

} // namespace cell 

#endif // !CELL_H
