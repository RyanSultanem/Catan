#ifndef LAND_H
#define LAND_H

#include "Land.fwd.h"
#include "Card.fwd.h"

namespace land
{
	class Land
	{
	public:
		Land(const card::RessourceType& ressourceType);
		card::RessourceType getRessourceType() const;
		card::Ressource produceRessource() const;

	private:
		card::RessourceType m_ressourceType;
	};
}

#endif // !LAND_H
