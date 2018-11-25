#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <map>
#include "Card.h"
#include "Serializable.hpp"

namespace token {

class Token : public serialize::Serializable
{
public:
   explicit Token(int reference) : m_reference(reference) {}

	virtual const std::map<card::Ressource, int>& getCost() const = 0;

   int getReference() const { return m_reference; }

   std::string serialize() const override { return std::to_string(getReference()); }

   bool operator==(const Token & other) const { return m_reference == other.m_reference; }

	virtual ~Token() {};

private:
   int m_reference;
};

}
#endif // !TOKEN_HPP
