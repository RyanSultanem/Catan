#include <board/Land.hpp>

#include <card/Card.hpp>

namespace land {

Land::Land(const card::Ressource& ressourceType) 
	: m_ressourceType(ressourceType)
{
}

card::Ressource Land::getRessourceType() const
{
	return m_ressourceType;
}

card::Ressource Land::produceRessource() const
{
	return m_ressourceType;
}

} // namespace land


