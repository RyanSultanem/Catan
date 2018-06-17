#include "Building.hpp"

namespace token {
namespace building {

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

Building::Building(player::PointReceiver& pointReceiver) : m_pointReceiver(pointReceiver)
{
}

std::string Building::serialize() const
{
   return std::to_string(m_pointReceiver.getId());
}

Settlement::Settlement(player::PointReceiver& pointReceiver) : Building(pointReceiver)
{
}

const std::map<card::Ressource, int>& Settlement::getCost() const
{
	return SETTLEMENT_COST;
}

int Settlement::getPoints() const
{
	return SETTLEMENT_POINTS;
}

void Settlement::setPlaced() const
{
   m_pointReceiver.decreaseSettlmentCount();
}

std::string Settlement::serialize() const
{
   return "S" + '|' + Building::serialize();
}

City::City(player::PointReceiver& pointReceiver) : Building(pointReceiver)
{
}

const std::map<card::Ressource, int>& City::getCost() const
{
	return CITY_COST;
}

int City::getPoints() const
{
	return CITY_POINTS;
}

void City::setPlaced() const
{
}

std::string City::serialize() const
{
   return "C" + '|' + Building::serialize();
}

} // namespace building
} // namespace token