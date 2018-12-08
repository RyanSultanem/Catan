#ifndef CARD_H
#define CARD_H

#include "Card.fwd.h"

#include <functional>

namespace card {

enum class RessourceType
{
	LUMBER,
	BRICK,
	WOOL,
	GRAIN,
	ORE,
	NO_RESSOURCE
};

class Ressource
{
public:
	explicit Ressource(const RessourceType& type);
	RessourceType getType() const;

	bool operator<(const Ressource& other) const;
	bool operator==(const Ressource& other) const;

private:
	RessourceType m_type;
};

}

namespace std {

template<>
struct hash<card::Ressource>
{
	size_t operator()(const card::Ressource & ressource) const
	{
		return hash<card::RessourceType>()(ressource.getType());
	}
};

} // namepsace std

#endif // !CARD_H
