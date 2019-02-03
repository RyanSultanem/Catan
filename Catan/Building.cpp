#include "Building.hpp"

namespace token {
namespace building {

const std::unordered_map<card::Ressource, int> SETTLEMENT_COST =
{
	{ card::Ressource(card::RessourceType::BRICK), 1 },
	{ card::Ressource(card::RessourceType::GRAIN), 1 },
	{ card::Ressource(card::RessourceType::LUMBER), 1 },
	{ card::Ressource(card::RessourceType::WOOL), 1 }
};

const int SETTLEMENT_POINTS = 1;

const std::unordered_map<card::Ressource, int> CITY_COST =
{
	{ card::Ressource(card::RessourceType::GRAIN), 2},
	{ card::Ressource(card::RessourceType::ORE), 3}
};

const int CITY_POINTS = 2;

Building::Building(int reference) 
   : Token(reference)
{
}

Settlement::Settlement(int reference) 
	: Building(reference)
{
}

const std::unordered_map<card::Ressource, int>& Settlement::getCost() const
{
	return getSettlementCost();
}

const std::unordered_map<card::Ressource, int>& Settlement::getSettlementCost()
{
	return SETTLEMENT_COST;
}

int Settlement::getPoints() const
{
	return getSettlementPoints();
}

int Settlement::getSettlementPoints()
{
	return SETTLEMENT_POINTS;
}

std::string Settlement::serialize() const
{
   return "S/" + Token::serialize();
}	

City::City(int reference) 
	: Building(reference)
{
}

const std::unordered_map<card::Ressource, int>& City::getCost() const
{
	return getCityCost();
}

const std::unordered_map<card::Ressource, int>& City::getCityCost()
{
	return CITY_COST;
}

int City::getPoints() const
{
	return getCityPoints();
}

int City::getCityPoints()
{
	return CITY_POINTS;
}

std::string City::serialize() const
{
   return "C/" + Token::serialize();
}

} // namespace building
} // namespace token