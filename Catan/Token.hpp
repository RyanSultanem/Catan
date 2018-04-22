#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <map>
#include "Card.h"

namespace token {

class Token
{
public:
	virtual const std::map<card::Ressource, int>& getCost() const = 0;
	virtual ~Token() {};
};

}
#endif // !TOKEN_HPP
