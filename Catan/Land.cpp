#include "Land.h"
#include "Card.h"

namespace land
{
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
}


