#ifndef ROAD_HPP
#define ROAD_HPP

#include "Road.fwd.hpp"
#include "Token.hpp"
#include "PlayerInterface.hpp"

namespace token {

class Road : public Token
{
public:
   explicit Road(int reference);

	const std::unordered_map<card::RessourceType, int>& getCost() const override;
	static const std::unordered_map<card::RessourceType, int>& getRoadCost();

	bool operator==(const Road& other) const;

	std::string serialize() const override;
};

} //  namespace token

#endif // !ROAD_HPP
