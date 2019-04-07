#ifndef BOARD_LAND_HPP
#define BOARD_LAND_HPP

#include <board/Land.fwd.hpp>

#include <card/Card.fwd.hpp>

namespace land {

class Land
{
public:
	Land(const card::RessourceType& ressourceType);
	card::RessourceType getRessourceType() const;
	card::Ressource produceRessource() const;

private:
	card::RessourceType m_ressourceType;
};

} // namespace land

#endif // !BOARD_LAND_HPP
