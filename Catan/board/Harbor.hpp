#ifndef BOARD_HARBOR_HBB
#define BOARD_HARBOR_HBB

#include <board/Harbor.fwd.hpp>

#include <card/Card.hpp>

class Harbor
{
public:
	explicit Harbor(card::RessourceType ressource);

	card::RessourceType getRessourceType() const;
	int getNewTradeCost() const;

private:
	card::RessourceType m_ressource;
};

#endif // !BOARD_HARBOR_HBB