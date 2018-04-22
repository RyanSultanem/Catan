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
	virtual int getPoints() const = 0;
   bool isOnBoard() const;
   void setOnBoard(bool onBoard);

private:
   bool m_onBoard;
};

class Settlement : public Building
{
public:
	const std::map<card::Ressource, int>& getCost() const override;
	int getPoints() const override;
};

class City : public Building
{
public:
	const std::map<card::Ressource, int>& getCost() const override;
	int getPoints() const override;
};

} // namespace building
} // namespace token

#endif // !BUILDING_HPP
