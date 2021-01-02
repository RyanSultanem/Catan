#include <token/Building.hpp>

namespace token {
namespace building {

const std::unordered_map<card::Ressource, int> SETTLEMENT_COST =
{
	{ card::Ressource::BRICK, 1 },
	{ card::Ressource::GRAIN, 1 },
	{ card::Ressource::LUMBER, 1 },
	{ card::Ressource::WOOL, 1 }
};

const int SETTLEMENT_POINTS = 1;

const std::unordered_map<card::Ressource, int> CITY_COST =
{
	{ card::Ressource::GRAIN, 2},
	{ card::Ressource::ORE, 3}
};

const int CITY_POINTS = 2;

Building::Building(const player::Owner & owner)
   : Token(owner)
{
}

Settlement::Settlement(const player::Owner & owner)
	: Building(owner)
{
}

const std::unordered_map<card::Ressource, int>& Settlement::getCost() const
{
	return SETTLEMENT_COST;
}

int Settlement::getPoints() const
{
	return SETTLEMENT_POINTS;
}

void Settlement::accept(const TokenVisitor & visitor) const
{
	visitor.visit(*this);
}

std::string Settlement::serialize() const
{
   return "S|" + Token::serialize();
}	

City::City(const player::Owner & owner)
	: Building(owner)
{
}

const std::unordered_map<card::Ressource, int>& City::getCost() const
{
	return CITY_COST;
}

int City::getPoints() const
{
	return CITY_POINTS;
}

void City::accept(const TokenVisitor & visitor) const
{
	visitor.visit(*this);
}

std::string City::serialize() const
{
   return "C|" + Token::serialize();
}

} // namespace building
} // namespace token