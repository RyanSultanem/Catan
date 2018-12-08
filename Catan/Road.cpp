#include "Road.hpp"

namespace token {

const std::unordered_map<card::Ressource, int> ROAD_COST =
{
	{ card::Ressource(card::RessourceType::LUMBER), 1 },
	{ card::Ressource(card::RessourceType::BRICK), 1 }
};

Road::Road(int reference) 
   : Token(reference)
{
}

const std::unordered_map<card::Ressource, int>& Road::getCost() const
{
	return ROAD_COST;
}

bool Road::operator==(const Road & other) const
{
   return Token::operator==(other);
}

std::string Road::serialize() const
{
   return "R|" + Token::serialize();
}

} // namespace token