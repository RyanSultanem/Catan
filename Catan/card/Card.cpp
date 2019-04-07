#include <card/Card.hpp>

namespace card {

Ressource::Ressource(const RessourceType& type) : m_type(type)
{
}

RessourceType Ressource::getType() const
{
	return m_type;
}

bool Ressource::operator<(const Ressource & other) const
{
	return this->m_type < other.m_type;
}

bool Ressource::operator==(const Ressource & other) const
{
	return this->m_type == other.m_type;
}

} //namespace card