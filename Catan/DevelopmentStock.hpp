#ifndef DEVELOPMENT_STOCK_HPP
#define DEVELOPMENT_STOCK_HPP

#include "Development.hpp"

#include <memory>
#include <unordered_map>

class DevelopmentStock
{
public:
	DevelopmentStock();
	card::DevelopmentType drawCardType();
   const card::Development & getCard(card::DevelopmentType developmentType) const;
	bool empty() const;

private:
	std::unordered_map<card::DevelopmentType, int> m_developmentCardCount;
   std::unordered_map<card::DevelopmentType, card::Development> m_developmentCards;
};

#endif // !DEVELOPMENT_STOCK_HPP