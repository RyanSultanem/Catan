#ifndef TOKEN_TOKEN_HPP
#define TOKEN_TOKEN_HPP

#include <card/Card.hpp>

#include <player/Player.fwd.hpp>
#include <player/PlayerInterface.hpp>

#include <token/TokenVisitor.hpp>

#include <utility/Serializable.hpp>

#include <map>

namespace token {

class Token : public serialize::Serializable
{
public:
   explicit Token(const player::Owner & owner) : m_owner(owner) {}

	virtual const std::unordered_map<card::Ressource, int>& getCost() const = 0;

   int getPlayerId() const { return m_owner.get().getId(); }

   virtual void accept(const TokenVisitor & visitor) const = 0;

   std::string serialize() const override { return std::to_string(getPlayerId()); }

   bool operator==(const Token & other) const { return getPlayerId() == other.getPlayerId(); }

	virtual ~Token() {};

private:
   player::OwnerCRef m_owner;
};

} // namespace token

#endif // !TOKEN_TOKEN_HPP
