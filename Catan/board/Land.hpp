#ifndef BOARD_LAND_HPP
#define BOARD_LAND_HPP

#include <board/Land.fwd.hpp>

#include <card/Card.fwd.hpp>

namespace land {

class Land
{
public:
	Land(const card::Ressource& ressourceType);
	card::Ressource getRessourceType() const;
	card::Ressource produceRessource() const;

private:
	card::Ressource m_ressourceType;
};

} // namespace land

#endif // !BOARD_LAND_HPP
