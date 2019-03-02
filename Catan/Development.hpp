#ifndef CARD_DEVELOPMENT_HPP
#define CARD_DEVELOPMENT_HPP

#include "Card.h"
#include <unordered_map>

namespace card {

class Development // should be a Token?
{
public:
	virtual bool execute() const = 0;
};

const std::unordered_map<card::RessourceType, int> & getDevelopmentCost();

class MonopolyDevelopment : public Development
{
	
};

} // namespace card

#endif // !CARD_DEVELOPMENT_HPP