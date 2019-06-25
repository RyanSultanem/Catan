#ifndef TOKEN_TOKENVISITORPLACED_HPP
#define TOKEN_TOKENVISITORPLACED_HPP

#include <token/TokenVisitor.hpp>

#include <player/Player.fwd.hpp>

namespace token {

class TokenVisitorPlaced : public TokenVisitor
{
public:
	explicit TokenVisitorPlaced(player::Player & player);

	virtual void visit(const building::Settlement & settlement) const override;
	virtual void visit(const building::City & city) const override;
	virtual void visit(const Road & settlement) const override;

private:
	player::Player & m_player;
};

} // namespace token

#endif // !TOKEN_TOKENVISITORPLACED_HPP