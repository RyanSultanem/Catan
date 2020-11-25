#ifndef BOARD_CELL_HPP
#define BOARD_CELL_HPP

#include <board/Cell.fwd.hpp>
#include <board/Vertex.fwd.hpp>

#include <card/Card.hpp>

#include <token/Building.fwd.hpp>

#include <utility/Serializable.hpp>

#include <vector>
#include <memory>

namespace board {

class Cell : public serialize::Serializable
{
public:
	Cell(const card::Ressource& ressource, unsigned int number, const std::vector<board::VertexRef>& vertices, int id = 0);
	card::Ressource getRessource() const;
	int getId() const;

	bool hasVertex(int vertexId) const;

	int getNumber() const;
	void setNumber(int number);
	const std::vector<token::building::Building*> getActiveBuildings() const;

	std::string serialize() const override;

private:
	int m_id;
	card::Ressource m_ressource;
	unsigned int m_number;
	std::vector<board::VertexRef> m_vertices;
};

} // namespace board 

#endif // !BOARD_CELL_HPP
