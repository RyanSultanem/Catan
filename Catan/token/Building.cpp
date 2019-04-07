#include <token/Building.hpp>

namespace token {
namespace building {

const std::unordered_map<card::RessourceType, int> SETTLEMENT_COST =
{
	{ card::RessourceType::BRICK, 1 },
	{ card::RessourceType::GRAIN, 1 },
	{ card::RessourceType::LUMBER, 1 },
	{ card::RessourceType::WOOL, 1 }
};

const int SETTLEMENT_POINTS = 1;

const std::unordered_map<card::RessourceType, int> CITY_COST =
{
	{ card::RessourceType::GRAIN, 2},
	{ card::RessourceType::ORE, 3}
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

const std::unordered_map<card::RessourceType, int>& Settlement::getCost() const
{
	return getSettlementCost();
}

const std::unordered_map<card::RessourceType, int>& Settlement::getSettlementCost()
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
   return "S|" + Token::serialize();
}	

City::City(int reference) 
	: Building(reference)
{
}

const std::unordered_map<card::RessourceType, int>& City::getCost() const
{
	return getCityCost();
}

const std::unordered_map<card::RessourceType, int>& City::getCityCost()
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
   return "C|" + Token::serialize();
}

} // namespace building
} // namespace token