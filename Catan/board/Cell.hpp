#ifndef BOARD_CELL_HPP
#define BOARD_CELL_HPP

#include <board/Cell.fwd.hpp>
#include <board/Land.hpp>
#include <board/Vertex.fwd.hpp>

#include <token/Building.fwd.hpp>

#include <utility/Serializable.hpp>

#include <vector>
#include <memory>

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
	void setNumber(int number); // TODO: THis was done for the robber, check if wreally needed or can totally replace the cell by the robber for effect.
	const std::vector<token::building::Building*> getActiveBuildings() const;
	land::Land getLand() const;

private:
	int m_id;
	land::Land m_land;
	unsigned int m_number;
	std::vector<board::VertexRef> m_vertices;
};

} // namespace cell 

#endif // !BOARD_CELL_HPP
