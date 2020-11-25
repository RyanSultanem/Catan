#ifndef BOARD_HARBOR_HBB
#define BOARD_HARBOR_HBB

#include <board/Harbor.fwd.hpp>

#include <card/Card.hpp>

class Harbor
{
public:
	explicit Harbor(card::Ressource ressource);

	card::Ressource getRessourceType() const;
	int getNewTradeCost() const;

private:
	card::Ressource m_ressource;
};

#endif // !BOARD_HARBOR_HBB