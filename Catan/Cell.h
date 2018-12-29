#ifndef CELL_H
#define CELL_H

#include <vector>
#include <memory>

#include "Cell.fwd.h"
#include "Land.h"
#include "Vertex.fwd.h"
#include "Serializable.hpp"
#include "Board.h"

#include "Building.fwd.hpp"

namespace cell {

class Cell : public serialize::Serializable
{
public:
	Cell(const card::RessourceType& ressource, unsigned int number, const std::vector<board::VertexRef>& vertices, int id = 0);
	card::Ressource produceLandRessource() const;
	int getId() const;

	bool hasVertex(int vertexId) const;

   std::string serialize() const override;
	int getNumber() const;
	const std::vector<token::building::Building*> getActiveBuildings() const;
	land::Land getLand() const;

private:
	int m_id;
	land::Land m_land;
	unsigned int m_number;
	std::vector<board::VertexRef> m_vertices;
};

} // namespace cell 

#endif // !CELL_H
