#ifndef TOKEN_TOKEN_HPP
#define TOKEN_TOKEN_HPP

#include <card/Card.hpp>

#include <token/TokenVisitor.hpp>

#include <utility/Serializable.hpp>

#include <map>

namespace token {

class Token : public serialize::Serializable
{
public:
   explicit Token(int playerId) : m_playerId(playerId) {}

	virtual const std::unordered_map<card::Ressource, int>& getCost() const = 0;

   int getPlayerId() const { return m_playerId; }

   virtual void accept(const TokenVisitor & visitor) const = 0;

   std::string serialize() const override { return std::to_string(getPlayerId()); }

   bool operator==(const Token & other) const { return m_playerId == other.m_playerId; }

	virtual ~Token() {};

private:
   int m_playerId; // TODO: should be player::Owner?
};

} // namespace token

#endif // !TOKEN_TOKEN_HPP
