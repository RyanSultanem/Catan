#include <board/Harbor.hpp>

Harbor::Harbor(card::Ressource ressource)
	: m_ressource(ressource)
{
}

card::Ressource Harbor::getRessourceType() const
{
	return m_ressource;
}

int Harbor::getNewTradeCost() const
{
	return m_ressource == card::Ressource::NO_RESSOURCE ? 3 : 2;
}


