#include <token/Road.hpp>

namespace token {

const std::unordered_map<card::RessourceType, int> ROAD_COST =
{
	{ card::RessourceType::LUMBER, 1 },
	{ card::RessourceType::BRICK, 1 }
};

Road::Road(int reference) 
   : Token(reference)
{
}

const std::unordered_map<card::RessourceType, int>& Road::getCost() const
{
	return getRoadCost();
}

bool Road::operator==(const Road & other) const
{
   return Token::operator==(other);
}

std::string Road::serialize() const
{
   return "R|" + Token::serialize();
}

const std::unordered_map<card::RessourceType, int>& Road::getRoadCost()
{
	return ROAD_COST;
}

} // namespace token