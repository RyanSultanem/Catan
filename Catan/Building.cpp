#include "Building.hpp"

namespace token {
namespace building {


bool Building::isOnBoard() const
{
   return m_onBoard;
}

void Building::setOnBoard(bool onBoard)
{
   m_onBoard = onBoard;
}

const std::map<card::Ressource, int> SETTLEMENT_COST =
{
	{ card::Ressource(card::RessourceType::BRICK), 1 },
	{ card::Ressource(card::RessourceType::GRAIN), 1 },
	{ card::Ressource(card::RessourceType::LUMBER), 1 },
	{ card::Ressource(card::RessourceType::WOOL), 1 }
};

const int SETTLEMENT_POINTS = 1;

const std::map<card::Ressource, int> CITY_COST =
{
	{ card::Ressource(card::RessourceType::GRAIN), 2},
	{ card::Ressource(card::RessourceType::ORE), 3}
};

const int CITY_POINTS = 2;

const std::map<card::Ressource, int>& Settlement::getCost() const
{
	return SETTLEMENT_COST;
}

int Settlement::getPoints() const
{
	return SETTLEMENT_POINTS;
}

const std::map<card::Ressource, int>& City::getCost() const
{
	return CITY_COST;
}

int City::getPoints() const
{
	return CITY_POINTS;
}

} // namespace building
} // namespace token