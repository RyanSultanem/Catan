#ifndef TOKEN_ROAD_HPP
#define TOKEN_ROAD_HPP

#include <token/Token.hpp>

#include <token/Road.fwd.hpp>

#include <card/Card.hpp>

#include <player/PlayerInterface.hpp>

namespace token {

class Road : public Token
{
public:
   explicit Road(int reference);

	const std::unordered_map<card::Ressource, int>& getCost() const override;

	bool operator==(const Road& other) const;

	void accept(const TokenVisitor & visitor) const override;
	std::string serialize() const override;
};

} //  namespace token

#endif // !TOKEN_ROAD_HPP
