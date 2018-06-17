#include "Road.hpp"

namespace token {

const std::map<card::Ressource, int> ROAD_COST =
{
	{ card::Ressource(card::RessourceType::LUMBER), 1 },
	{ card::Ressource(card::RessourceType::BRICK), 1 }
};

Road::Road(player::Owner& owner) : m_owner(owner)
{
}

const std::map<card::Ressource, int>& Road::getCost() const
{
	return ROAD_COST;
}

void Road::setPlaced() const
{

}

bool Road::operator==(const Road & other) const
{
   return true;
}

std::string Road::serialize() const
{
   return "R" + '|' + std::to_string(m_owner.getId());
}

} // namespace token