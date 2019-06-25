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
	return ROAD_COST;
}

bool Road::operator==(const Road & other) const
{
   return Token::operator==(other);
}

void Road::accept(const TokenVisitor & visitor) const
{
	visitor.visit(*this);
}

std::string Road::serialize() const
{
   return "R|" + Token::serialize();
}

} // namespace token