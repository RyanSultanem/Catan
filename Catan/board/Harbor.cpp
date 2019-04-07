#include <board/Harbor.hpp>

Harbor::Harbor(card::RessourceType ressource)
	: m_ressource(ressource)
{
}

card::RessourceType Harbor::getRessourceType() const
{
	return m_ressource;
}

int Harbor::getNewTradeCost() const
{
	return m_ressource == card::RessourceType::NO_RESSOURCE ? 3 : 2;
}


