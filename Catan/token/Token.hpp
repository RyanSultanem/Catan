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
   explicit Token(int reference) : m_reference(reference) {}

	virtual const std::unordered_map<card::Ressource, int>& getCost() const = 0;

   int getReference() const { return m_reference; }

   virtual void accept(const TokenVisitor & visitor) const = 0;

   std::string serialize() const override { return std::to_string(getReference()); }

   bool operator==(const Token & other) const { return m_reference == other.m_reference; }

	virtual ~Token() {};

private:
   int m_reference; // TODO: should be playerId/playerReference or Owner?
};

} // namespace token

#endif // !TOKEN_TOKEN_HPP
