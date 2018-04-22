#include "Road.hpp"

namespace token {

const std::map<card::Ressource, int> ROAD_COST =
{
	{ card::Ressource(card::RessourceType::LUMBER), 1 },
	{ card::Ressource(card::RessourceType::BRICK), 1 }
};

const std::map<card::Ressource, int>& Road::getCost() const
{
	return ROAD_COST;
}

bool Road::operator==(const Road & other) const
{
   return true;
}

} // namespace token