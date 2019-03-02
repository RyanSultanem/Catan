#ifndef DEVELOPMENT_STOCK_HPP
#define DEVELOPMENT_STOCK_HPP

#include "Development.hpp"

#include <memory>
#include <unordered_map>

class DevelopmentStock
{
public:
	DevelopmentStock();
	std::unique_ptr<card::Development> drawCard();
	bool empty() const;

private:
	std::unordered_map<std::unique_ptr<card::Development>, int> m_developmentCards;
};

#endif // !DEVELOPMENT_STOCK_HPP