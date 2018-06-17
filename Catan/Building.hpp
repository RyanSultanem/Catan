#ifndef BUILDING_HPP
#define BUILDING_HPP

#include "Building.fwd.hpp"   
#include "Card.h"
#include "Token.hpp"
#include <map>
#include "PlayerInterface.hpp"

namespace token {
namespace building {

class Building : public Token
{
public:
   explicit Building(player::PointReceiver & pointReceiver);
   
   virtual int getPoints() const = 0;
   std::string serialize() const override;

private:
   player::PointReceiver & m_pointReceiver;
};

class Settlement : public Building
{
public:
   explicit Settlement(player::PointReceiver & pointReceiver);

	const std::map<card::Ressource, int>& getCost() const override;
	int getPoints() const override;

   std::string serialize() const override;
};

class City : public Building
{
public:
   explicit City(player::PointReceiver & pointReceiver);

	const std::map<card::Ressource, int>& getCost() const override;
	int getPoints() const override;

   std::string serialize() const override;
};

} // namespace building
} // namespace token

#endif // !BUILDING_HPP
