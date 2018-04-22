#ifndef ROAD_HPP
#define ROAD_HPP

#include "Token.hpp"

namespace token {

class Road : public Token
{
public:
	const std::map<card::Ressource, int>& getCost() const override;
   bool operator==(const Road& other) const;
};

} //  namespace token

#endif // !ROAD_HPP
