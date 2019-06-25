#ifndef TOKEN_TOKENVISITOR_HPP
#define TOKEN_TOKENVISITOR_HPP

#include <token/Building.fwd.hpp>
#include <token/Road.fwd.hpp>

namespace token {

class TokenVisitor
{
public:
	virtual void visit(const building::Settlement & settlement) const = 0;
	virtual void visit(const building::City & city) const = 0;
	virtual void visit(const Road & settlement) const = 0;
};

} // namespace token

#endif // !TOKEN_TOKENVISITOR_HPP
