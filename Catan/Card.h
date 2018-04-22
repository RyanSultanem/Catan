#ifndef CARD_H
#define CARD_H

#include "Card.fwd.h"

namespace card
{
	enum class RessourceType
	{
		LUMBER,
		BRICK,
		WOOL,
		GRAIN,
		ORE,
		NO_RESSOURCE
	};

	class Ressource
	{
	public:
		Ressource(const RessourceType& type);
		RessourceType getType() const;

		bool operator<(const Ressource& other) const;

	private:
		RessourceType m_type;
	};

}

#endif // !CARD_H
