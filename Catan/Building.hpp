#ifndef BUILDING_HPP
#define BUILDING_HPP

#include "Building.fwd.hpp"   
#include "Card.h"
#include "Token.hpp"
#include <map>

namespace token {
namespace building {

class Building : public Token
{
public:
   explicit Building(int reference);

   virtual int getPoints() const = 0;
};

class Settlement : public Building
{
public:
   explicit Settlement(int reference);

	const std::unordered_map<card::RessourceType, int>& getCost() const override;
	static const std::unordered_map<card::RessourceType, int>& getSettlementCost();

	std::string serialize() const override;
	
	int getPoints() const override;
	static int getSettlementPoints();
};

class City : public Building
{
public:
   explicit City(int reference);

	const std::unordered_map<card::RessourceType, int>& getCost() const override;
	static const std::unordered_map<card::RessourceType, int>& getCityCost();

	std::string serialize() const override;
	
	int getPoints() const override;
	static int getCityPoints();
};

} // namespace building
} // namespace token

#endif // !BUILDING_HPP
