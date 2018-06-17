#ifndef ROAD_HPP
#define ROAD_HPP

#include "Road.fwd.hpp"
#include "Token.hpp"
#include "PlayerInterface.hpp"

namespace token {

class Road : public Token
{
public:
   Road(player::Owner & owner);

	const std::map<card::Ressource, int>& getCost() const override;
   bool operator==(const Road& other) const;

   std::string serialize() const override;

private:
   player::Owner & m_owner;
};

} //  namespace token

#endif // !ROAD_HPP
