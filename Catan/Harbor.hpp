#ifndef HARBOR_HBB
#define HARBOR_HBB

#include "Harbor.fwd.hpp"
#include "Card.h"

class Harbor
{
public:
	explicit Harbor(card::RessourceType ressource);

	card::RessourceType getRessourceType() const;
	int getNewTradeCost() const;

private:
	card::RessourceType m_ressource;
};

#endif // !HARBOR_HBB