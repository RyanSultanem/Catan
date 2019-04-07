#include <board/Land.hpp>

#include <card/Card.hpp>

namespace land {

Land::Land(const card::RessourceType& ressourceType) : m_ressourceType(ressourceType)
{
}

card::RessourceType Land::getRessourceType() const
{
	return m_ressourceType;
}

card::Ressource Land::produceRessource() const
{
	return card::Ressource(m_ressourceType);
}

} // namespace land


